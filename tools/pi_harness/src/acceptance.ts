/**
 * Acceptance utilities for the pi-harness.
 *
 * Deliberately minimal: snapshot/restore of writable-scope files (plain
 * copies — NO git operations), lint via tools/llm_decomp/lint.py, compile
 * check via `coop run build`, acceptance via `batch-cycle.py`, and target
 * claims. Restore happens only when the candidate does not compile;
 * regression and match acceptance are decided by `batch-cycle.py`.
 * Never use git machinery in the revert path.
 *
 * @module acceptance
 */

import { execFile } from "node:child_process";
import { copyFile, mkdir, readFile } from "node:fs/promises";
import { existsSync } from "node:fs";
import { join, dirname } from "node:path";
import type { BatchResult, Target } from "./types.js";

/** Promisified execFile.
 *
 * IMPORTANT: node's execFile error object does NOT carry stdout/stderr — they
 * are separate callback args. On non-zero exit we attach them to the
 * rejection, or every hexdiff mismatch (exit 5, JSON on stdout) would lose
 * the diff payload and the acceptance path would see a generic failure
 * (adversarial review C1: this was the exact bug that made onVerify blind to
 * every genuinely-mismatched target).
 */
export function execFilePromise(
  command: string,
  args: string[],
  options: { cwd: string },
): Promise<{ stdout: string; stderr: string }> {
  return new Promise((resolve, reject) => {
    execFile(command, args, { cwd: options.cwd, maxBuffer: 32 * 1024 * 1024 }, (err, stdout, stderr) => {
      if (err) {
        (err as { stdout?: string; stderr?: string }).stdout = stdout;
        (err as { stdout?: string; stderr?: string }).stderr = stderr;
        reject(err);
      } else {
        resolve({ stdout, stderr });
      }
    });
  });
}

// ─────────────────────────────────────────────────────────────────────
//  Snapshot / restore (plain file copies)
// ─────────────────────────────────────────────────────────────────────

export interface Snapshot {
  unit: string;
  /** Repo-relative paths that were copied (existed at snapshot time). */
  files: string[];
  /** Absolute path to the snapshot directory. */
  dir: string;
}

/**
 * Copy the writable-scope files into
 * `<repoRoot>/build/pi-harness/snapshots/<unit>/<timestamp>/`, preserving
 * relative paths.
 */
export async function snapshotUnit(
  repoRoot: string,
  unit: string,
  files: string[],
): Promise<Snapshot> {
  const sanitizedUnit = unit.replace(/\//g, "__");
  const timestamp = new Date().toISOString().replace(/[:.]/g, "-");
  const dir = join(repoRoot, "build", "pi-harness", "snapshots", sanitizedUnit, timestamp);
  await mkdir(dir, { recursive: true });

  const snapshotted: string[] = [];
  for (const file of files) {
    const src = join(repoRoot, file);
    if (!existsSync(src)) continue;
    const dst = join(dir, file);
    await mkdir(dirname(dst), { recursive: true });
    await copyFile(src, dst);
    snapshotted.push(file);
  }
  return { unit: sanitizedUnit, files: snapshotted, dir };
}

/**
 * Restore the snapshotted files from their copies. Called ONLY when the
 * candidate fails to compile — this is never a scope-enforcement tool.
 */
export async function restoreSnapshot(repoRoot: string, snapshot: Snapshot): Promise<void> {
  for (const file of snapshot.files) {
    const src = join(snapshot.dir, file);
    const dst = join(repoRoot, file);
    if (existsSync(src)) {
      await mkdir(dirname(dst), { recursive: true });
      await copyFile(src, dst);
    }
  }
}

// ─────────────────────────────────────────────────────────────────────
//  Lint (delta lint gate from tools/llm_decomp/lint.py)
// ─────────────────────────────────────────────────────────────────────

export interface LintOutcome {
  ok: boolean;
  violations: { path: string; rule: string; line?: number; detail?: string }[];
}

/** Run lint_delta over each snapshotted file (old copy vs current). */
export async function runLint(
  repoRoot: string,
  python: string,
  snapshot: Snapshot | null,
  files?: string[],
): Promise<LintOutcome> {
  const all: LintOutcome["violations"] = [];

  // Snapshots disabled: lint each listed file WHOLE (--file mode; every line
  // counts as added, so the full file is checked — the same rules, just no
  // delta baseline). The caller passes the source-file subset of the writable
  // scope. Nothing to check when no file list is given.
  if (!snapshot) {
    for (const file of files ?? []) {
      const repoPath = join(repoRoot, file);
      if (!existsSync(repoPath)) continue;
      try {
        const { stdout } = await execFilePromise(
          python,
          ["tools/pi_harness/lint_cli.py", "--file", repoPath],
          { cwd: repoRoot },
        );
        const result = JSON.parse(stdout) as { ok?: boolean; violations?: LintOutcome["violations"] };
        if (Array.isArray(result.violations)) all.push(...result.violations);
      } catch (err) {
        const msg = err instanceof Error ? err.message : String(err);
        process.stderr.write(`[pi-harness] WARNING: lint_cli.py failed for ${file}: ${msg}\n`);
        all.push({ path: file, rule: "lint-crash", detail: msg.slice(0, 500) });
      }
    }
    return { ok: all.length === 0, violations: all };
  }

  for (const file of snapshot.files) {
    const repoPath = join(repoRoot, file);
    const snapshotFile = join(snapshot.dir, file);
    if (!existsSync(repoPath) || !existsSync(snapshotFile)) continue;

    try {
      const { stdout } = await execFilePromise(
        python,
        ["tools/pi_harness/lint_cli.py", "--pair", snapshotFile, repoPath],
        { cwd: repoRoot },
      );
      const result = JSON.parse(stdout) as { ok?: boolean; violations?: LintOutcome["violations"] };
      if (Array.isArray(result.violations)) all.push(...result.violations);
    } catch (err) {
      // A linter crash must not silently pass — fail closed.
      const msg = err instanceof Error ? err.message : String(err);
      process.stderr.write(`[pi-harness] WARNING: lint_cli.py failed for ${file}: ${msg}\n`);
      all.push({ path: file, rule: "lint-crash", detail: msg.slice(0, 500) });
    }
  }

  return { ok: all.length === 0, violations: all };
}

// ─────────────────────────────────────────────────────────────────────
//  Compile check
// ─────────────────────────────────────────────────────────────────────

/**
 * Build the unit's object under the repo-wide build lock (same flock as
 * hexdiff) so harness acceptance builds cannot race agent hexdiff builds
 * or other workers. Returns ok + trimmed output for feedback.
 */
export async function buildUnit(
  repoRoot: string,
  python: string,
  region: string,
  unit: string,
): Promise<{ ok: boolean; output: string }> {
  // cmd_build acquires the repo build lock ITSELF (run.py _with_build_lock,
  // build-only scope, same flock as hexdiff) — do NOT wrap it in
  // build_lock.py. The wrapper would hold the flock while the inner
  // cmd_build blocks on the same flock (subprocess does not inherit the fd)
  // until the outer 900s timeout kills it: every buildUnit call deadlocked
  // ~15 min and reported ok=false with no ninja ever running (run31).
  try {
    const { stdout, stderr } = await execFilePromise(
      python,
      [
        "tools/coop/run.py", "build", unit,
      ],
      { cwd: repoRoot },
    );
    return { ok: true, output: (stdout + stderr).slice(-1500) };
  } catch (err) {
    const e = err as { stdout?: string; stderr?: string; message?: string };
    const output = ((e.stdout ?? "") + (e.stderr ?? "") || e.message || String(err)).slice(-1500);
    return { ok: false, output };
  }
}

// ─────────────────────────────────────────────────────────────────────
//  Batch cycle (acceptance)
// ─────────────────────────────────────────────────────────────────────

/**
 * Run `batch-cycle.py` for the given target ids and report per-target
 * status from targets.json. accepted = FULL_MATCH | EQUIVALENT_MATCH.
 * Non-zero exit is tolerated (failures are per-target, not fatal).
 */
/** Read per-target statuses from targets.json without running the cycle.
 *  Used by the certify-request accept path, where the witness cycles already
 *  updated the registry and a full batch-cycle run would be redundant.
 *  Applies the same acceptance rule as runBatchCycle (status + BACKLOG gate).
 *  When `witnessEnabled` is false, only FULL_MATCH is accepted (no
 *  EQUIVALENT_MATCH — the witness cannot produce one anyway). */
export async function readBatchResults(
  repoRoot: string,
  targetIds: string[],
  witnessEnabled = true,
): Promise<BatchResult[]> {
  let allTargets: Target[] = [];
  try {
    const parsed = JSON.parse(
      await readFile(join(repoRoot, "tools/coop/targets.json"), "utf-8"),
    ) as { targets?: unknown };
    if (Array.isArray(parsed.targets)) allTargets = parsed.targets as Target[];
  } catch {
    // fall through — everything reports UNKNOWN
  }
  const statusById = new Map(allTargets.map((t) => [t.id, t.status]));
  const acceptedStatuses = witnessEnabled
    ? new Set(["FULL_MATCH", "EQUIVALENT_MATCH"])
    : new Set(["FULL_MATCH"]);
  // Function acceptance is per-function (unit size no longer gates it — the
  // size gate lives in TU-final promotion, user policy 2026-08). A BACKLOG
  // workflow is a legacy size artifact; it must not block acceptance.
  return targetIds.map((id) => {
    const status = statusById.get(id) ?? "UNKNOWN";
    return { targetId: id, status, accepted: acceptedStatuses.has(status) };
  });
}

export async function runBatchCycle(
  repoRoot: string,
  python: string,
  targetIds: string[],
  witnessTimeoutMs = 0,
  witnessEnabled = true,
): Promise<BatchResult[]> {
  try {
    await execFilePromise(
      python,
      [
        // batch-cycle.py builds as it cycles; cmd_build now takes the repo
        // build lock ITSELF (build-only scope, run.py _with_build_lock) so
        // the witness / equivalence evaluation that follows the build runs
        // LOCK-FREE — a slow z3 simplify in the witness must not freeze
        // every other agent's builds (run30 incident: one acceptance held
        // the lock ~30 min spinning in z3).
        python,
        "tools/coop/batch-cycle.py",
        "--default-hypothesis", "pi-harness batch match",
        "--default-next-change", "accept if pass",
        "--witness-timeout", String(witnessTimeoutMs),
        ...(witnessEnabled ? [] : ["--no-witness"]),
        // `--` end-of-options: target ids come from targets.json and must
        // never be parsed as argparse flags.
        "--",
        ...targetIds,
      ],
      { cwd: repoRoot },
    );
  } catch {
    // tolerated — read targets.json below for actual results
  }

  return readBatchResults(repoRoot, targetIds, witnessEnabled);
}

// ─────────────────────────────────────────────────────────────────────
//  Hexdiff (mismatch feedback for re-prompts)
// ─────────────────────────────────────────────────────────────────────

interface HexdiffJson {
  mismatch_count?: number;
  structural_count?: number;
  reg_swap_count?: number;
  pure_reg_swap_count?: number;
  size_check?: { ok?: boolean; over_by?: number } | null;
  reg_mapping?: Record<string, string>;
  instructions?: { match: boolean; retail_asm?: string; decomp_asm?: string }[];
}

/** Extract a compact, model-readable summary from hexdiff JSON output. */
function extractHexdiffSummary(result: HexdiffJson): string {
  const parts: string[] = [];
  if (result.mismatch_count !== undefined) {
    parts.push(`mismatch_count: ${result.mismatch_count}`);
  }
  if (result.structural_count !== undefined) {
    parts.push(`structural: ${result.structural_count}`);
  }
  if (result.reg_swap_count !== undefined) {
    parts.push(`reg_swap: ${result.reg_swap_count}`);
  }
  if (result.size_check && result.size_check.ok === false) {
    parts.push(`size_over_budget: ${result.size_check.over_by ?? "?"}`);
  }
  if (result.reg_mapping && Object.keys(result.reg_mapping).length > 0) {
    parts.push(`reg_mapping: ${JSON.stringify(result.reg_mapping)}`);
  }
  // Show up to 10 mismatched instructions
  if (result.instructions) {
    const mismatches = result.instructions.filter(i => !i.match);
    const shown = mismatches.slice(0, 10);
    if (shown.length > 0) {
      parts.push(`mismatched instructions (showing ${shown.length}/${mismatches.length}):`);
      for (const m of shown) {
        parts.push(`  retail: ${m.retail_asm ?? "?"}`);
        parts.push(`  decomp: ${m.decomp_asm ?? "?"}`);
      }
    }
  }
  return parts.join("\n") || "(no structured diff data)";
}

/**
 * Run hexdiff for a single symbol and return a compact mismatch summary.
 * Does NOT use build_lock.py — the object was just built by buildUnit/runBatchCycle.
 * Uses --no-build so hexdiff never re-invokes ninja (the object exists already).
 *
 * hexdiff exits 5 when there ARE mismatches (the useful case), so we
 * parse stdout from the error object rather than treating non-zero as failure.
 */
export async function runHexdiff(
  repoRoot: string,
  python: string,
  unit: string,
  symbol: string,
  opts: { allowBuildRetry?: boolean } = {},
): Promise<{ ok: boolean; output: string; mismatchCount: number; structuralCount: number }> {
  const parse = (stdout: string) => {
    const result = JSON.parse(stdout) as HexdiffJson;
    return {
      ok: true,
      output: extractHexdiffSummary(result),
      mismatchCount: result.mismatch_count ?? -1,
      structuralCount: result.structural_count ?? -1,
    };
  };
  const runOnce = async (noBuild: boolean) => {
    const args = [
      "tools/coop/hexdiff.py", unit, "--symbol", symbol, "--json",
    ];
    if (noBuild) args.push("--no-build");
    try {
      const { stdout } = await execFilePromise(python, args, { cwd: repoRoot });
      return parse(stdout);
    } catch (err) {
      // hexdiff exits 5 when there ARE mismatches — parse stdout from error.
      const e = err as { stdout?: string; stderr?: string; code?: number; message?: string };
      if (e.stdout) {
        try {
          return parse(e.stdout);
        } catch {
          // JSON parse failed — fall through to error path
        }
      }
      return null;
    }
  };

  // Fast path: the object was just built by buildUnit/runBatchCycle, so read
  // it without re-invoking ninja. If that read fails (e.g. the object was
  // mid-rebuild right after a batch-cycle per-target build — seen on the
  // accept-path banking race), retry once letting hexdiff build it under
  // ninja's own .ninja_lock. Never more than one retry.
  let result = await runOnce(true);
  if (!result && opts.allowBuildRetry) {
    result = await runOnce(false);
  }
  if (result) return result;

  const output = `hexdiff failed for ${unit} ${symbol}`;
  process.stderr.write(`[pi-harness] WARNING: ${output}\n`);
  return { ok: false, output, mismatchCount: -1, structuralCount: -1 };
}

// ─────────────────────────────────────────────────────────────────────
//  Claims
// ─────────────────────────────────────────────────────────────────────

async function claimOp(
  repoRoot: string,
  python: string,
  op: "claim" | "release",
  ids: string[],
  owner: string,
  onClaimed?: (id: string) => void,
): Promise<{ ok: string[]; failed: string[] }> {
  const ok: string[] = [];
  const failed: string[] = [];
  if (ids.length === 0) return { ok, failed };
  // Batch the whole set into ONE `run.py targets claim/release` subprocess:
  // per-id subprocesses each rewrote the full registry under the exclusive
  // lock (run32: ~300 serialized rewrites blew the 120s lock timeout). The
  // batch path holds the lock once + writes once. On partial failure (an id
  // claimed by another owner), the whole call fails atomically — fall back
  // to per-id claims so one bad id doesn't block the rest.
  try {
    await execFilePromise(
      python,
      ["tools/coop/run.py", "targets", op, "--owner", owner, "--", ...ids],
      { cwd: repoRoot },
    );
    for (const id of ids) {
      ok.push(id);
      if (op === "claim") onClaimed?.(id);
    }
    return { ok, failed };
  } catch {
    // Atomic batch failed (unknown id / conflicting owner / lock timeout).
    // Retry each id individually so a single bad id doesn't fail the batch.
    for (const id of ids) {
      try {
        await execFilePromise(
          python,
          ["tools/coop/run.py", "targets", op, id, "--owner", owner],
          { cwd: repoRoot },
        );
        ok.push(id);
        if (op === "claim") onClaimed?.(id);
      } catch (err) {
        failed.push(id);
        process.stderr.write(
          `[pi-harness] WARNING: targets ${op} failed for ${id}: ${err instanceof Error ? err.message : String(err)}\n`,
        );
      }
    }
    return { ok, failed };
  }
}

/**
 * Claim targets. Returns which claims succeeded/failed — the caller must
 * treat ANY failure as fatal for the batch (never edit unclaimed targets).
 * `onClaimed` fires per id immediately after each successful claim (for
 * signal-handler tracking).
 */
export function claimTargets(
  repoRoot: string, python: string, ids: string[], owner: string,
  onClaimed?: (id: string) => void,
): Promise<{ ok: string[]; failed: string[] }> {
  return claimOp(repoRoot, python, "claim", ids, owner, onClaimed);
}

export function releaseTargets(
  repoRoot: string, python: string, ids: string[], owner: string,
): Promise<{ ok: string[]; failed: string[] }> {
  return claimOp(repoRoot, python, "release", ids, owner);
}

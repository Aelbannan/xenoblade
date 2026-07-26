/**
 * Acceptance utilities for the pi-harness.
 *
 * Deliberately minimal: snapshot/restore of writable-scope files (plain
 * copies — NO git operations), lint via tools/llm_decomp/lint.py, compile
 * check via `coop run build`, acceptance via `batch-cycle.py`, and target
 * claims. Restore happens ONLY when the build fails or lint rejects —
 * never as a scope enforcement mechanism.
 *
 * @module acceptance
 */

import { execFile } from "node:child_process";
import { randomUUID } from "node:crypto";
import { copyFile, mkdir, readFile, rm } from "node:fs/promises";
import { existsSync } from "node:fs";
import { join, dirname } from "node:path";
import type { BatchResult, Target } from "./types.js";

/** Promisified execFile. */
export function execFilePromise(
  command: string,
  args: string[],
  options: { cwd: string },
): Promise<{ stdout: string; stderr: string }> {
  return new Promise((resolve, reject) => {
    execFile(command, args, { cwd: options.cwd, maxBuffer: 32 * 1024 * 1024 }, (err, stdout, stderr) => {
      if (err) reject(err);
      else resolve({ stdout, stderr });
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
 * build fails or lint rejects — this is never a scope-enforcement tool.
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
  snapshot: Snapshot,
): Promise<LintOutcome> {
  const all: LintOutcome["violations"] = [];

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

/** Build the unit's object; returns ok + trimmed output for feedback. */
export async function buildUnit(
  repoRoot: string,
  python: string,
  unit: string,
): Promise<{ ok: boolean; output: string }> {
  try {
    const { stdout, stderr } = await execFilePromise(
      python,
      ["tools/coop/run.py", "build", unit],
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
export async function runBatchCycle(
  repoRoot: string,
  python: string,
  targetIds: string[],
): Promise<BatchResult[]> {
  const tmpFile = join(repoRoot, "build", "pi-harness", `batch-cycle-summary-${randomUUID()}.json`);
  await mkdir(dirname(tmpFile), { recursive: true });

  try {
    await execFilePromise(
      python,
      [
        "tools/coop/batch-cycle.py",
        "--default-hypothesis", "pi-harness batch match",
        "--default-next-change", "accept if pass",
        "--summary", tmpFile,
        // `--` end-of-options: target ids come from targets.json and must
        // never be parsed as argparse flags.
        "--",
        ...targetIds,
      ],
      { cwd: repoRoot },
    );
  } catch {
    // tolerated — read targets.json below for actual results
  } finally {
    await rm(tmpFile, { force: true }).catch(() => {});
  }

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
  const acceptedStatuses = new Set(["FULL_MATCH", "EQUIVALENT_MATCH"]);
  return targetIds.map((id) => {
    const status = statusById.get(id) ?? "UNKNOWN";
    return { targetId: id, status, accepted: acceptedStatuses.has(status) };
  });
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
): Promise<void> {
  for (const id of ids) {
    try {
      await execFilePromise(
        python,
        ["tools/coop/run.py", "targets", op, id, "--owner", owner],
        { cwd: repoRoot },
      );
    } catch (err) {
      process.stderr.write(
        `[pi-harness] WARNING: targets ${op} failed for ${id}: ${err instanceof Error ? err.message : String(err)}\n`,
      );
    }
  }
}

export function claimTargets(
  repoRoot: string, python: string, ids: string[], owner: string,
): Promise<void> {
  return claimOp(repoRoot, python, "claim", ids, owner);
}

export function releaseTargets(
  repoRoot: string, python: string, ids: string[], owner: string,
): Promise<void> {
  return claimOp(repoRoot, python, "release", ids, owner);
}

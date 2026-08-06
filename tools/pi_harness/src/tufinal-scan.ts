/**
 * TU-final match-regression / size / data tracking.
 *
 * The TU-final session polishes a fully-matched unit. The old logic scored
 * only lint violations, so a retry that reduced lint while regressing a
 * matched function got KEPT. This module:
 *
 *   1. Captures a BASELINE at TU-final start: per-target (structural,
 *      mismatch) pairs from one `hexdiff --all --json` call, plus TU size
 *      (run.py size) and data-section match % (run.py diff --no-smt).
 *   2. After EACH session attempt, re-scans with the same single calls and
 *      compares — regressions and improvements are stated loudly with the
 *      symbol + before→after amounts.
 *   3. Scores the attempt's state lexicographically (match regressions >
 *      TU size/data > lint) so the harness keeps the best state and never
 *      keeps a state that regressed a match.
 *
 * "Matched" = mismatch:0 OR structural:0 (reg-swap-only is witness-
 * certifiable). Comparison is lexicographic on (structural, mismatch).
 */

import { execFile } from "node:child_process";
import { Type } from "typebox";
import { defineTool, type ToolDefinition } from "@earendil-works/pi-coding-agent";
import { run } from "./session-tools.js";

/** Per-function match counts (hexdiff --all --json shape). */
export interface UnitFunctionState {
  symbol: string;
  present: boolean;
  match: boolean;
  mismatch: number;
  structural: number;
  reg_swap: number;
  pure_reg_swap: number;
  reloc: number;
  retail_size: number;
  decomp_size: number;
}

export interface UnitScan {
  unit: string;
  functions: UnitFunctionState[];
  matched: number;
  total: number;
  size_check: {
    ok: boolean;
    budget: number | null;
    retail_text: number | null;
    decomp_text: number | null;
    over_by: number | null;
    notes: string | null;
  } | null;
  /** data-section match % from run.py diff --no-smt (null if unavailable). */
  dataPercent: number | null;
  /** raw run.py size output tail (for the model). */
  sizeOutput: string;
  /** Per-symbol witness certification for reg-swap-only targets (structural:0,
   *  mismatch>0): true = the register-renaming witness certifies it as
   *  EQUIVALENT. Populated by an extra witness pass — a reg-swap-only target
   *  that NO LONGER certifies is a regression even though hexdiff says
   *  structural:0. */
  witnessCert: Map<string, boolean>;
}

/** One target's baseline/current pair; structurally lexicographic. */
export function targetOrder(a: { structural: number; mismatch: number }): number {
  return a.structural * 1_000_000 + a.mismatch;
}

/** True when the (structural, mismatch) pair counts as matched. */
export function isMatched(s: { structural: number; mismatch: number }): boolean {
  return s.structural === 0 && s.mismatch === 0;
}

/** True when a target counts as certified (0-mismatch, or reg-swap-only that
 *  the witness certifies). witnessCert is consulted for structural:0,
 *  mismatch>0 targets. */
export function isCertified(
  s: { structural: number; mismatch: number },
  witnessCert: boolean | undefined,
): boolean {
  if (s.structural === 0 && s.mismatch === 0) return true;
  if (s.structural === 0 && s.mismatch > 0) return witnessCert === true;
  return false;
}

export interface TargetDelta {
  symbol: string;
  before: { structural: number; mismatch: number };
  after: { structural: number; mismatch: number };
  /** true = regressed (worse), false = improved (better) */
  regressed: boolean;
  /** set when the witness cert status changed (lost = regression). */
  witnessBefore?: boolean;
  witnessAfter?: boolean;
}

/** Compare a current scan against a baseline; returns per-target deltas.
 *  Uses witnessCert: a structural:0 target that LOST its witness cert is a
 *  regression even though hexdiff counts are unchanged. */
export function diffUnitScans(
  baseline: UnitScan,
  current: UnitScan,
): TargetDelta[] {
  const byName = new Map(current.functions.map((f) => [f.symbol, f]));
  const deltas: TargetDelta[] = [];
  for (const b of baseline.functions) {
    if (!b.present) continue;
    const c = byName.get(b.symbol);
    if (!c || !c.present) continue;
    const before = { structural: b.structural, mismatch: b.mismatch };
    const after = { structural: c.structural, mismatch: c.mismatch };
    const witnessBefore = baseline.witnessCert.get(b.symbol);
    const witnessAfter = current.witnessCert.get(c.symbol);
    const bCert = isCertified(before, witnessBefore);
    const aCert = isCertified(after, witnessAfter);
    const bScore = targetOrder(before);
    const aScore = targetOrder(after);
    const scoreChanged = aScore !== bScore;
    const certChanged = bCert !== aCert;
    if (scoreChanged || certChanged) {
      const certLoss = bCert && !aCert;
      const certGain = !bCert && aCert;
      const regressed = certLoss || (scoreChanged && aScore > bScore && !certGain);
      const d: TargetDelta = { symbol: b.symbol, before, after, regressed };
      if (certChanged) {
        d.witnessBefore = bCert;
        d.witnessAfter = aCert;
      }
      deltas.push(d);
    }
  }
  return deltas.sort((x, y) => Number(y.regressed) - Number(x.regressed));
}

/** Score a scan state; LOWER is better. Lexicographic per the priority:
 *  1. match regressions (count, then severity)
 *  2. TU size (over budget is bad) + data section match %
 *  3. lint violations
 * Returns a comparable tuple (bigint-encodable). */
export function scoreState(
  scan: UnitScan,
  baseline: UnitScan,
  lintViolations: number,
): bigint {
  // Build-broken / unverifiable guard (adversarial review C3): when the unit
  // fails to build, scanUnitState returns an EMPTY scan (functions: []).
  // diffUnitScans would then find ZERO regressions vacuously, overBudget=0
  // and dataPercent=0, so the broken state could outscore a healthy baseline
  // and be restored at end-of-session. Score any state that lost every
  // function as worst possible so it can never win best-state selection.
  if (baseline.functions.length > 0 && scan.functions.length === 0) {
    // ~1e30: larger than any real score (max ~1e16), and no field can
    // overflow into it.
    return 1_000_000_000_000_000_000_000_000_000_000n;
  }
  const deltas = diffUnitScans(baseline, scan);
  const regressed = deltas.filter((d) => d.regressed);
  const improved = deltas.filter((d) => !d.regressed);
  const regressedCount = regressed.length;
  const regressedSeverity = regressed.reduce((s, d) => s + (targetOrder(d.after) - targetOrder(d.before)) + (d.witnessBefore === true && d.witnessAfter === false ? 1_000_000 : 0), 0);
  const improvedCount = improved.length;
  const overBudget = scan.size_check && !scan.size_check.ok ? (scan.size_check.over_by ?? 0) : 0;
  const dataPct = scan.dataPercent ?? 0;
  // Encode (LOWER is better), with NO field overlap:
  //   [0-99]      regressedCount     × 100_000_000_000_000
  //   [0-999999]  regressedSeverity  × 100_000_000
  //   [0-999999]  overBudget         × 100
  //   [0-100]     (100 - dataPct)    × 1
  //   [0-999]     lintViolations     × 1/1000 (fractional, low priority)
  //   improvements are a BONUS (subtracted) so they never outrank a
  //   regression, and overBudget cannot overflow into another field.
  let score = BigInt(Math.max(0, Math.min(99, regressedCount))) * 100_000_000_000_000n;
  score += BigInt(Math.max(0, Math.min(999999, regressedSeverity))) * 100_000_000n;
  score += BigInt(Math.max(0, Math.min(999999, overBudget))) * 100n;
  score += BigInt(100 - Math.max(0, Math.min(100, Math.round(dataPct))));
  score += BigInt(Math.max(0, Math.min(999, lintViolations)));
  // Improvements reduce the score within the same regression tier (best
  // possible bonus ≈ 99*1_000_000 = 99,000,000 — under the 100M regressedSeverity
  // digit, so an improvement can never hide a regression or overflow).
  score -= BigInt(Math.max(0, Math.min(99, improvedCount))) * 1_000_000n;
  return score;
}

/**
 * Loud regression/improvement feedback. Always includes the unit-level
 * summary (size + data), then per-symbol regressions (with before→after),
 * then improvements. Used in the TU-final re-prompt and the initial prompt.
 */
export function buildUnitFeedback(
  baseline: UnitScan,
  current: UnitScan,
  lintViolations: number | null,
): string {
  const deltas = diffUnitScans(baseline, current);
  const regressed = deltas.filter((d) => d.regressed);
  const improved = deltas.filter((d) => !d.regressed);
  const parts: string[] = [];

  // ── Unit-level summary (MOST IMPORTANT per the TU-final contract) ──
  const sz = current.size_check;
  const sizeLine = sz
    ? `TU size: ${sz.ok ? "PASS" : "OVER BUDGET"} decomp ${sz.decomp_text ?? "?"}B vs budget ${sz.budget ?? "?"}B` +
      (sz.over_by ? ` (over by ${sz.over_by}B)` : "")
    : "TU size: unavailable";
  parts.push(
    `## TU Unit Status\n\n` +
    `- ${sizeLine}\n` +
    `- data section match: ${current.dataPercent !== null ? `${current.dataPercent.toFixed(1)}%` : "unavailable"}\n` +
    `- matched functions: ${current.matched}/${current.total}`,
  );

  // ── Match regressions (LOUD, first) ──
  if (regressed.length > 0) {
    parts.push(
      `## ⚠️ MATCH REGRESSION — ${regressed.length} function(s) regressed\n\n` +
      `The polish REGRESSED previously-matched functions. Restore or fix these — ` +
      `a regression is never acceptable:\n\n` +
      regressed.map((d) =>
        `- \`${d.symbol}\`: structural ${d.before.structural}→${d.after.structural}, ` +
        `mismatch ${d.before.mismatch}→${d.after.mismatch}` +
        (d.witnessBefore === true && d.witnessAfter === false
          ? ` — WITNESS NO LONGER CERTIFIES (was EQUIVALENT_MATCH)`
          : ""),
      ).join("\n"),
    );
  }

  // ── Match improvements (also loud, positive) ──
  if (improved.length > 0) {
    parts.push(
      `## ✅ Match improvements — ${improved.length} function(s) improved\n\n` +
      improved.map((d) =>
        `- \`${d.symbol}\`: structural ${d.before.structural}→${d.after.structural}, ` +
        `mismatch ${d.before.mismatch}→${d.after.mismatch}` +
        (d.witnessBefore === false && d.witnessAfter === true
          ? ` — now WITNESS-CERTIFIED`
          : ""),
      ).join("\n"),
    );
  }

  // ── Size/data deltas ──
  const bsz = baseline.size_check;
  if (sz && bsz && sz.decomp_text !== null && bsz.decomp_text !== null && sz.decomp_text !== bsz.decomp_text) {
    const delta = sz.decomp_text - bsz.decomp_text;
    parts.push(`- TU size ${delta > 0 ? "grew" : "shrank"} by ${Math.abs(delta)}B (${bsz.decomp_text}B → ${sz.decomp_text}B)`);
  }
  if (baseline.dataPercent !== null && current.dataPercent !== null && baseline.dataPercent !== current.dataPercent) {
    const delta = current.dataPercent - baseline.dataPercent;
    parts.push(`- data section match ${delta > 0 ? "improved" : "dropped"} by ${Math.abs(delta).toFixed(1)}pp (${baseline.dataPercent.toFixed(1)}% → ${current.dataPercent.toFixed(1)}%)`);
  }

  if (lintViolations !== null) {
    parts.push(`- lint violations: ${lintViolations}`);
  }

  return parts.join("\n\n");
}

/** One-call whole-unit scan: hexdiff --all --json + size + data%, plus a
 *  witness re-check on every reg-swap-only (structural:0, mismatch>0) target
 *  so EQUIVALENT_MATCH states that stopped certifying are caught. When
 *  `witnessEnabled` is false the witness re-check is skipped entirely
 *  (reg-swap-only targets are simply not certified — only byte-identical
 *  FULL_MATCH counts). */
export async function scanUnitState(
  repoRoot: string, python: string, unit: string, witnessEnabled = true,
): Promise<UnitScan> {
  let scan: UnitScan = {
    unit, functions: [], matched: 0, total: 0,
    size_check: null, dataPercent: null, sizeOutput: "", witnessCert: new Map(),
  };
  try {
    // hexdiff --all --json exits 5 when any function mismatches — run()
    // REJECTS on non-zero exit, so we must read stdout from the rejection
    // error (C1: swallowing the rejection returned an empty scan, making the
    // whole regression detector a no-op for any non-byte-identical unit).
    let stdout = "";
    try {
      const r = await run(python, ["tools/coop/hexdiff.py", unit, "--all", "--json"], repoRoot);
      stdout = r.stdout;
    } catch (err) {
      const e = err as { stdout?: string };
      if (e.stdout && e.stdout.trimStart().startsWith("{")) stdout = e.stdout;
      else throw err; // genuinely broken (build failure) — empty scan is correct
    }
    const parsed = JSON.parse(stdout) as {
      unit?: string; functions?: unknown[]; matched?: number; total?: number; size_check?: UnitScan["size_check"];
    };
    scan = {
      unit: parsed.unit ?? unit,
      functions: (parsed.functions ?? []).map((f) => ({
        symbol: String((f as Record<string, unknown>).symbol ?? "?"),
        present: Boolean((f as Record<string, unknown>).present),
        match: Boolean((f as Record<string, unknown>).match),
        mismatch: Number((f as Record<string, unknown>).mismatch ?? 0),
        structural: Number((f as Record<string, unknown>).structural ?? 0),
        reg_swap: Number((f as Record<string, unknown>).reg_swap ?? 0),
        pure_reg_swap: Number((f as Record<string, unknown>).pure_reg_swap ?? 0),
        reloc: Number((f as Record<string, unknown>).reloc ?? 0),
        retail_size: Number((f as Record<string, unknown>).retail_size ?? 0),
        decomp_size: Number((f as Record<string, unknown>).decomp_size ?? 0),
      })),
      matched: parsed.matched ?? 0,
      total: parsed.total ?? 0,
      size_check: parsed.size_check ?? null,
      dataPercent: null,
      sizeOutput: "",
      witnessCert: new Map(),
    };
    // Witness re-check: run run.py diff --no-smt per reg-swap-only target to
    // confirm the witness still certifies it (EQUIVALENT_MATCH integrity).
    // Cheap: only structural:0 & mismatch>0 functions get a witness probe.
    // Skipped entirely when the witness is disabled (witnessEnabled=false).
    if (witnessEnabled) {
      for (const f of scan.functions) {
        if (f.present && f.structural === 0 && f.mismatch > 0) {
          let stdout = "";
          try {
            const r = await run(
              python, ["tools/coop/run.py", "diff", unit, "--symbol", f.symbol, "--no-smt"], repoRoot,
            );
            stdout = r.stdout;
          } catch (err) {
            // run.py diff prints the verdict to stdout then exits 1 on size
            // over-budget — read the verdict from the rejection (H2: discarding
            // it produced false 'WITNESS NO LONGER CERTIFIES' regressions).
            const e = err as { stdout?: string };
            if (e.stdout && /status: \S+/.test(e.stdout)) stdout = e.stdout;
            else { scan.witnessCert.set(f.symbol, false); continue; }
          }
          const eq = stdout.match(/equivalence: (\S+)/)?.[1];
          const status = stdout.match(/status: (\S+)/)?.[1];
          // run.py prints lowercase (full_match / equivalent) for the
          // equivalence: line (r5 finding 5: the old uppercase checks were dead
          // code — only the status: line, which is uppercase, ever matched).
          const certified =
            eq === "EQUIVALENT_MATCH" || eq === "FULL_MATCH" ||
            eq === "equivalent" || eq === "full_match" ||
            status === "EQUIVALENT_MATCH" || status === "FULL_MATCH";
          scan.witnessCert.set(f.symbol, certified);
        }
      }
    }
  } catch {
    // hexdiff --all failed (build broken or unit not resolvable) — return
    // the empty scan; the caller scores it as worst (build-broken).
  }

  // data% from run.py diff --no-smt (single call). Thread --no-witness when
  // the witness is disabled so the scan never re-enables the witness probe
  // through cmd_diff (DeepSeek F5).
  try {
    const { stdout } = await run(
      python, ["tools/coop/run.py", "diff", unit, "--no-smt", ...(witnessEnabled ? [] : ["--no-witness"])], repoRoot,
    );
    const m = stdout.match(/data:\s*([\d.]+)%/);
    if (m) scan.dataPercent = parseFloat(m[1]);
  } catch { /* data% unavailable */ }

  // size tail for the model (single call).
  try {
    const { stdout } = await run(python, ["tools/coop/run.py", "size", unit], repoRoot);
    scan.sizeOutput = stdout.slice(-1000);
  } catch { /* size unavailable */ }

  return scan;
}

// ─────────────────────────────────────────────────────────────────────
//  unit-status tool — one-call whole-unit match scan (hexdiff --all + size
//  + data% + witness re-checks on reg-swap targets)
// ─────────────────────────────────────────────────────────────────────

/**
 * unit-status custom tool: single call that scans the WHOLE unit — one
 * `hexdiff --all --json` (per-function match counts), one `run.py size`
 * (split budget), one `run.py diff --no-smt` (data-section %), and a
 * witness re-check on every reg-swap-only target. Returns the same summary
 * the TU-final regression sweep uses. Use in TU-final (or batch) to see the
 * whole unit's state at once instead of per-symbol hexdiff calls.
 */
export function unitStatusTool(repoRoot: string, python: string, witnessEnabled = true): ToolDefinition {
  return defineTool({
    name: "unit-status",
    label: "unit-status",
    description:
      "One-call whole-unit scan: hexdiff --all (per-function match counts), TU split size, data-section match %, and witness certification status for reg-swap-only targets. Use to see the entire unit's matching state at once — for TU-final verification or to check whether any function regressed.",
    promptSnippet: "unit-status <unit> — whole-unit match/size/data/witness scan (one call)",
    parameters: Type.Object({
      unit: Type.String({ description: "objdiff unit hint (e.g. kyoshin/CSaveLoad)" }),
    }),
    execute: async (_id, params) => {
      const scan = await scanUnitState(repoRoot, python, params.unit, witnessEnabled);
      const parts: string[] = [];
      parts.push(`## unit-status: ${scan.unit}`);
      const sz = scan.size_check;
      parts.push(
        sz
          ? `- TU size: ${sz.ok ? "PASS" : "OVER BUDGET"} decomp ${sz.decomp_text ?? "?"}B vs budget ${sz.budget ?? "?"}B` +
            (sz.over_by ? ` (over by ${sz.over_by}B)` : "")
          : "- TU size: unavailable (build broken?)",
      );
      parts.push(`- data section match: ${scan.dataPercent !== null ? `${scan.dataPercent.toFixed(1)}%` : "unavailable"}`);
      parts.push(`- matched: ${scan.matched}/${scan.total}`);
      const regSwap = scan.functions.filter((f) => f.present && f.structural === 0 && f.mismatch > 0);
      if (regSwap.length > 0) {
        // MEDIUM-LOW (DeepSeek F5): when the witness is disabled the scan
        // skips the re-check entirely — say so instead of "❌ NOT certified".
        parts.push(witnessEnabled
          ? `\n### Reg-swap-only (${regSwap.length}) — witness status`
          : `\n### Reg-swap-only (${regSwap.length}) — witness disabled (not checked)`);
        for (const f of regSwap.slice(0, 25)) {
          const cert = scan.witnessCert.get(f.symbol);
          parts.push(witnessEnabled
            ? `- ${cert === true ? "✅ certifies" : cert === false ? "❌ NOT certified by witness" : "❓ untested"} \`${f.symbol}\` (${f.mismatch} reg-swaps)`
            : `- ⏭ witness disabled — not checked \`${f.symbol}\` (${f.mismatch} reg-swaps)`);
        }
        if (regSwap.length > 25) parts.push(`- … (${regSwap.length - 25} more)`);
      }
      const unmatched = scan.functions.filter((f) => f.present && !(f.structural === 0 && f.mismatch === 0) && !(f.structural === 0 && f.mismatch > 0));
      if (unmatched.length > 0) {
        parts.push(`\n### Not matched (${unmatched.length})`);
        for (const f of unmatched.slice(0, 25)) {
          parts.push(`- \`${f.symbol}\`: structural ${f.structural}, mismatch ${f.mismatch}`);
        }
        if (unmatched.length > 25) parts.push(`- … (${unmatched.length - 25} more)`);
      }
      if (scan.sizeOutput) {
        parts.push(`\n### size output\n\`\`\`text\n${scan.sizeOutput.slice(-400)}\n\`\`\``);
      }
      return {
        content: [{ type: "text", text: parts.join("\n") }],
        details: { ok: true, matched: scan.matched, total: scan.total, sizeOk: sz?.ok ?? null, dataPercent: scan.dataPercent },
      };
    },
  });
}

// ---------------------------------------------------------------------------
// Custom session tools for pi-harness agents (tangOS-style structured tools).
//
// Batch sessions get read/edit/write/grep/find/ls + {hexdiff, symbols,
// targets} and NO bash: the whole matching loop (hexdiff + symbol lookup +
// target identity) is exposed as structured tools, which hard-blocks SMT
// (`cycle --smt`, `diff` without `--no-smt`), git reverts, ninja/configure,
// and registry writes at the tool level — not by prompt.
//
// TU-final sessions additionally get a `bash` tool behind a spawnHook
// allowlist (run.py diff/size/symbols + build_lock configure/ninja) because
// the finalisation flow legitimately needs the shell.
// ---------------------------------------------------------------------------

import { execFile } from "node:child_process";
import { access as fsAccess, readFile, writeFile } from "node:fs/promises";
import { join, relative, resolve } from "node:path";
import { Type } from "typebox";
import {
  defineTool,
  createBashTool,
  createEditToolDefinition,
  createWriteToolDefinition,
  type BashSpawnHook,
  type EditOperations,
  type ToolDefinition,
  type WriteOperations,
} from "@earendil-works/pi-coding-agent";

// Imported lazily to avoid a module cycle: tufinal-scan imports `run` from
// this module, and this module exposes the unit-status tool defined there.
// The cycle is benign (function declarations hoist), but the import sits at
// the top level for clarity.
import { unitStatusTool } from "./tufinal-scan.js";

/** Promisified execFile with a generous buffer (hexdiff JSON can be big).
 *  IMPORTANT: node's execFile error object does NOT carry stdout/stderr —
 *  they are separate callback args. On non-zero exit we must attach them
 *  to the rejection, or every hexdiff mismatch (exit 5) would lose the
 *  diff JSON and the tool would fall into the "build failed" path. */
export function run(python: string, args: string[], cwd: string): Promise<{ stdout: string; stderr: string }> {
  return new Promise((resolve, reject) => {
    execFile(python, args, { cwd, maxBuffer: 64 * 1024 * 1024 }, (err, stdout, stderr) => {
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

/** True when a hexdiff stdout blob is the JSON result. hexdiff always emits
 *  an OBJECT ({...}) — never an array — so require a `{` prefix after trim.
 *  This rejects ninja progress lines (`[1/1] MWCC ...`), the .note.split
 *  postprocess line, and any other stdout noise. */
export function looksLikeJson(s: string): boolean {
  return s.trimStart().startsWith("{");
}

// ─────────────────────────────────────────────────────────────────────
//  hexdiff — the iteration loop, with reloc surfacing
// ─────────────────────────────────────────────────────────────────────

/**
 * Compact structured summary of a hexdiff JSON result, INCLUDING reloc
 * drift + suggestions (the objdiff-reloc workflow agents previously needed
 * bash for). Prefers --no-build (object exists) with a build-retry fallback
 * (the accept-path banking race fix).
 */
async function runHexdiffTool(
  repoRoot: string, python: string, unit: string, symbol: string, brief: boolean,
): Promise<string> {
  const baseArgs = ["tools/coop/hexdiff.py", unit, "--symbol", symbol, "--json"];
  // Transient failures (ninja lock contention with a parallel run, mid-write
  // objects, `premature end of file`) present as empty stdout + no real
  // compiler error. Retry a few times with backoff before surfacing.
  const sleep = (ms: number): Promise<void> => new Promise((r) => setTimeout(r, ms));
  const build = async (noBuild: boolean): Promise<{ stdout: string | null; stderr: string }> => {
    const args = noBuild ? [...baseArgs, "--no-build"] : baseArgs;
    let lastStderr = "";
    for (let attempt = 0; attempt < 3; attempt++) {
      try {
        const { stdout } = await run(python, args, repoRoot);
        return { stdout, stderr: "" };
      } catch (err) {
        const e = err as { stdout?: string; stderr?: string; message?: string };
        // exit 5 (mismatches) carries JSON on stdout — but a failed build
        // ALSO writes a non-JSON line ("added .note.split to ...") to stdout
        // before failing. Only treat stdout as a result if it actually parses
        // as JSON; otherwise surface stderr (the real compiler error).
        if (e.stdout && looksLikeJson(e.stdout)) return { stdout: e.stdout, stderr: e.stderr ?? "" };
        lastStderr = e.stderr ?? e.message ?? String(err);
        // Empty stderr + non-zero exit = transient (no compiler error was
        // produced). Retry; a real compile error always has mwcceppc output.
        const realError = /mwcceppc|Error|error:/i.test(lastStderr);
        if (realError || attempt === 2) break;
        await sleep(1500 * (attempt + 1));
      }
    }
    return { stdout: null, stderr: lastStderr };
  };
  // ALWAYS build: the model edits source between hexdiff calls, so a
  // --no-build fast path would return STALE mismatch counts from the
  // pre-edit object (adversarial finding 1: a codegen change showed 13
  // stale mismatches vs 68 fresh). The iteration loop needs fresh bytes
  // every call. (The acceptance path has its own post-build --no-build
  // read in acceptance.ts where the object was JUST built.)
  let result = await build(false);
  if (!result.stdout) {
    // Surface the ACTUAL compiler error, not the ninja command dump that
    // precedes it (which is ~15 lines of flags). mwcceppc errors look like:
    //   ### mwcceppc.exe Compiler:
    //   #    File: src\foo.cpp
    //   #     67:     THIS_SHOULD_NOT_COMPILE_XYZ;
    //   #   Error:     ^^^^^^^^^^^^^^^^^^^
    //   #   (10140) undefined identifier 'THIS_SHOULD_NOT_COMPILE_XYZ'
    //
    // A symbol-not-found error is NOT a build failure — the object built
    // fine; the requested symbol name is wrong. Surface that distinctly
    // with the available symbols, so the model fixes the NAME (often a
    // mangled vs base-name mismatch) instead of chasing a phantom build
    // error.
    const errLines = result.stderr.trim().split("\n");
    const symErr = errLines.find((l) => /symbol .* not found/i.test(l));
    if (symErr) {
      const availIdx = errLines.findIndex((l) => /available:/.test(l));
      const avail = availIdx >= 0 ? errLines.slice(availIdx).join("\n") : "";
      return (
        `ERROR: symbol '${symbol}' not found in ${unit} (the object built fine).\n` +
        `Use the EXACT symbol from the target brief (base name, no C++ mangling).\n` +
        `\`\`\`text\n${symErr}\n${avail}\n\`\`\``
      );
    }
    const ccIdx = errLines.findIndex((l) => l.includes("mwcceppc.exe Compiler") || l.includes("### "));
    const errTail = (ccIdx >= 0 ? errLines.slice(ccIdx) : errLines.slice(-25)).join("\n");
    return (
      `ERROR: hexdiff build failed for ${unit} ${symbol}.\n` +
      `The object could not be built — the model must fix the compile error first.\n` +
      `\`\`\`text\n${errTail || "(no compiler output)"}\n\`\`\``
    );
  }

  let raw: string | null = result.stdout;
  let d: Record<string, unknown>;
  try {
    d = JSON.parse(raw) as Record<string, unknown>;
  } catch {
    return "ERROR: hexdiff returned non-JSON output.";
  }

  const n = (k: string): number | undefined => (typeof d[k] === "number" ? (d[k] as number) : undefined);
  const relocs = (k: string): Array<Record<string, unknown>> =>
    Array.isArray(d[k]) ? (d[k] as Array<Record<string, unknown>>) : [];
  const drifts = relocs("reloc_drift");
  const suggestions = (d.reloc_suggestions ?? {}) as Record<string, unknown>;

  const lines: string[] = [];
  lines.push(`## hexdiff: ${symbol}`);
  lines.push(`- mismatch: ${n("mismatch_count") ?? "?"} | structural: ${n("structural_count") ?? "?"} | reg_swap: ${n("reg_swap_count") ?? "?"}`);
  const sizeCheck = d.size_check as { ok?: boolean; over_by?: number } | null | undefined;
  if (sizeCheck && sizeCheck.ok === false) {
    lines.push(`- SIZE OVER BUDGET by ${sizeCheck.over_by ?? "?"} bytes (never matches)`);
  }
  lines.push(`- retail ${n("retail_size") ?? "?"}B vs decomp ${n("decomp_size") ?? "?"}B`);

  if (drifts.length > 0) {
    lines.push(`\n### Reloc drift (${drifts.length})`);
    for (const dr of drifts) {
      const off = dr.offset !== undefined ? `+0x${Number(dr.offset).toString(16).padStart(4, "0")}` : "?";
      lines.push(
        `- ${off} [${dr.type ?? "?"}] ${dr.retail_symbol ?? "?"} → ${dr.decomp_symbol ?? "?"}` +
          (dr.kind ? ` (${dr.kind})` : ""),
      );
    }
    for (const [off, sugg] of Object.entries(suggestions)) {
      const arr = Array.isArray(sugg) ? (sugg as string[]) : [String(sugg)];
      lines.push(`- fix @${off}: ${arr.join(" | ")}`);
    }
  } else if (relocs("retail_relocations").length > 0 || relocs("decomp_relocations").length > 0) {
    lines.push(`\n### Relocations (no drift): retail ${relocs("retail_relocations").length}, decomp ${relocs("decomp_relocations").length}`);
  }

  if (brief) {
    // Keep only the first mismatch block of instructions (already in summary form above).
    return lines.join("\n");
  }

  // Full mode: include the per-instruction diff table (compact).
  const instrs = Array.isArray(d.instructions) ? (d.instructions as Array<Record<string, unknown>>) : [];
  const shown = instrs.filter((i) => i.match === false).slice(0, 15);
  if (shown.length > 0) {
    lines.push(`\n### First ${shown.length} mismatched instructions`);
    for (const m of shown) {
      lines.push(`- retail: ${m.retail_asm ?? "?"}`);
      lines.push(`  decomp: ${m.decomp_asm ?? "?"}`);
    }
    if (instrs.filter((i) => i.match === false).length > shown.length) {
      lines.push(`- … (${instrs.filter((i) => i.match === false).length - shown.length} more)`);
    }
  }
  return lines.join("\n");
}

/** hexdiff custom tool definition. */
export function hexdiffTool(repoRoot: string, python: string): ToolDefinition {
  return defineTool({
    name: "hexdiff",
    label: "hexdiff",
    description:
      "Diff one function's decompiled bytes against retail. Returns mismatch/structural/reg_swap counts, size check, reloc drift + fix suggestions. Use for the iteration loop: edit source, hexdiff, repeat.",
    promptSnippet: "hexdiff <unit> <symbol> — diff one function (counts + reloc drift)",
    parameters: Type.Object({
      unit: Type.String({ description: "objdiff unit hint (e.g. kyoshin/CGame) or source path" }),
      symbol: Type.String({ description: "mangled function symbol (e.g. wkRender__5CGameFv)" }),
      brief: Type.Optional(Type.Boolean({ description: "counts + reloc summary only (default false = includes first mismatched instructions)" })),
    }),
    execute: async (_id, params) => {
      const out = await runHexdiffTool(repoRoot, python, params.unit, params.symbol, params.brief ?? false);
      const ok = !out.startsWith("ERROR");
      return { content: [{ type: "text", text: out }], details: { ok, error: ok ? undefined : out } };
    },
  });
}

// ─────────────────────────────────────────────────────────────────────
//  symbols — symbol table lookup
// ─────────────────────────────────────────────────────────────────────

/** symbols custom tool definition: hexdiff --list per unit, optional filter. */
export function symbolsTool(repoRoot: string, python: string): ToolDefinition {
  return defineTool({
    name: "symbols",
    label: "symbols",
    description:
      "List retail function symbols in a unit (address | size | name), optionally filtered by substring. Use to find callee names / mangled symbols mentioned in ASM.",
    promptSnippet: "symbols <unit> [substr] — list unit symbols",
    parameters: Type.Object({
      unit: Type.String({ description: "objdiff unit hint" }),
      substr: Type.Optional(Type.String({ description: "case-insensitive substring filter on symbol name" })),
    }),
    execute: async (_id, params) => {
      const args = ["tools/coop/hexdiff.py", params.unit, "--list"];
      if (params.substr) args.push(params.substr);
      try {
        const { stdout, stderr } = await run(python, args, repoRoot);
        const table = stdout.trim() || stderr.trim() || "(no symbols)";
        const lines = table.split("\n");
        return {
          content: [{ type: "text", text: lines.length > 120 ? lines.slice(0, 120).join("\n") + `\n… (${lines.length - 120} more)` : table }],
          details: { ok: true, count: lines.length },
        };
      } catch (err) {
        return { content: [{ type: "text", text: `ERROR: ${(err as Error).message}` }], details: { ok: false, count: 0 } };
      }
    },
  });
}

// ─────────────────────────────────────────────────────────────────────
//  witness — read-only equivalence certification check (no SMT)
// ─────────────────────────────────────────────────────────────────────

/**
 * witness custom tool: `run.py diff --no-smt` — READ-ONLY. Runs the cheap
 * register-renaming witness (position-aligned same-mnemonic pairs whose
 * diffs are register-only) and reports whether the CURRENT code would
 * certify as EQUIVALENT_MATCH / FULL_MATCH. Does NOT touch the target
 * registry (unlike `run.py cycle`, which is harness-owned and writes
 * acceptance).
 *
 * The model should call this when hexdiff shows `structural: 0` (code
 * structure is right, only registers differ) — if it says certifiable,
 * report it so the harness can run the accepting cycle. Also useful to
 * confirm FULL_MATCH on 0-mismatch states.
 */
export function witnessTool(repoRoot: string, python: string): ToolDefinition {
  return defineTool({
    name: "witness",
    label: "witness",
    description:
      "READ-ONLY equivalence check: runs the register-renaming witness (no SMT) on the current code and reports whether it would certify as FULL_MATCH or EQUIVALENT_MATCH. Call when hexdiff shows structural: 0 (only registers differ) or mismatch: 0 — if certifiable, tell the harness to run the accepting cycle. Never modifies the target registry or the attempt log. (May refresh the reloc-map cache sidecar, which is benign.)",
    promptSnippet: "witness <unit> <symbol> — check current code certifies (no SMT, read-only)",
    parameters: Type.Object({
      unit: Type.String({ description: "objdiff unit hint (e.g. kyoshin/menu/CMenuMapSelect)" }),
      symbol: Type.String({ description: "function symbol (base name, e.g. func_80242368)" }),
    }),
    execute: async (_id, params) => {
      const args = ["tools/coop/run.py", "diff", params.unit, "--symbol", params.symbol, "--no-smt"];
      // Transient ninja lock contention (parallel runs sharing the build
      // dir) surfaces as a non-zero exit with no verdict — retry a few
      // times before surfacing.
      const sleep = (ms: number): Promise<void> => new Promise((r) => setTimeout(r, ms));
      for (let attempt = 0; attempt < 3; attempt++) {
        try {
          const { stdout, stderr } = await run(python, args, repoRoot);
          const full = (stdout + stderr).trim();
          // A verdict is present when we see status:/equivalence:/code:
          if (/status: \S+/.test(full)) {
            // Parse the verdict lines.
            const status = full.match(/status: (\S+)/)?.[1] ?? "?";
            const eq = full.match(/equivalence: (\S+)/)?.[1];
            const codeMatch = full.match(/code: ([\d.]+)%/)?.[1] ?? "?";
            const symMatch = full.match(/symbol: \S+  match: ([\d.]+)%/)?.[1] ?? "?";
            const certifiable =
              eq === "FULL_MATCH" || eq === "EQUIVALENT_MATCH" ||
              status === "FULL_MATCH" || status === "EQUIVALENT_MATCH";

            const lines: string[] = [];
            lines.push(`## witness: ${params.symbol}`);
            lines.push(`- status: ${status} | code: ${codeMatch}% | symbol match: ${symMatch}%`);
            if (eq) lines.push(`- equivalence: ${eq}`);
            lines.push(certifiable
              ? "- ✅ CERTIFIABLE — this code would be accepted (the harness should run the accepting cycle)"
              : "- ❌ NOT certifiable yet — keep iterating (structure and/or semantics still differ)");
            const verdictLines = full.split("\n").filter((l) =>
              /^(status|equivalence|certificate|code|symbol):/.test(l));
            if (verdictLines.length > 0) lines.push("", "### raw verdict", ...verdictLines.map((l) => `- ${l}`));
            const text = lines.join("\n");
            return {
              content: [{ type: "text", text }],
              details: { ok: true, certifiable, status, equivalence: eq ?? null },
            };
          }
          // No verdict yet — transient (lock contention). Retry.
          if (attempt < 2) await sleep(2000 * (attempt + 1));
        } catch (err) {
          const e = err as { stdout?: string; stderr?: string; message?: string };
          // cmd_diff prints the verdict to stdout then exits 1 when the unit
          // is over the split-size budget. e.stdout carries the verdict —
          // surface it (with the size note) instead of discarding it for the
          // generic error (adversarial finding 2: the exit-1 stdout was
          // dropped, repeating the run() bug class).
          if (e.stdout && /status: \S+/.test(e.stdout)) {
            const full = e.stdout.trim();
            const verdictLines = full.split("\n").filter((l) =>
              /^(status|equivalence|certificate|code|symbol|size):/.test(l));
            // Include the verdict so the model knows whether the CODE is
            // done (size is a separate unit-level concern) — Kimi finding 5:
            // dropping the verdict made the model keep 'fixing' finished code.
            const eq = full.match(/equivalence: (\S+)/)?.[1];
            const certifiable = eq === "FULL_MATCH" || eq === "EQUIVALENT_MATCH";
            const lines = [
              `## witness: ${params.symbol}`,
              `- ❗ UNIT SIZE OVER BUDGET (the function's own verdict is below)`,
              ...verdictLines.map((l) => `- ${l}`),
              certifiable
                ? "- ✅ CODE IS CERTIFIABLE — fix the unit split size, then re-run"
                : "- ❌ NOT certifiable yet — keep iterating",
            ];
            return {
              content: [{ type: "text", text: lines.join("\n") }],
              details: { ok: false, certifiable, status: "size-failed", equivalence: eq ?? null },
            };
          }
          const detail = e.stderr || e.message || String(err);
          if (/lock|premature|contention/i.test(detail) && attempt < 2) {
            await sleep(2000 * (attempt + 1));
            continue;
          }
          return {
            content: [{ type: "text", text: `ERROR: ${detail}` }],
            details: { ok: false, certifiable: false, status: "error", equivalence: null },
          };
        }
      }
      return {
        content: [{ type: "text", text: "ERROR: witness tool could not get a verdict after 3 attempts (build lock contention?)" }],
        details: { ok: false, certifiable: false, status: "error", equivalence: null },
      };
    },
  });
}

// ─────────────────────────────────────────────────────────────────────
//  certify — request the harness to run the accepting witness cycle
// ─────────────────────────────────────────────────────────────────────

/**
 * certify custom tool: READ-ONLY request that the harness run the accepting
 * witness cycle for a target. Does NOT mutate the registry itself — it
 * instructs the model to emit a structured `CERTIFY: <target-id>` marker in
 * its FINAL response, which the harness's onVerify parses and executes via
 * runWitnessCycle (build lock + claim check + registry re-verification, no
 * SMT). Use after `witness` says CERTIFIABLE (or when hexdiff shows
 * mismatch: 0 / structural: 0).
 */
export function certifyTool(): ToolDefinition {
  return defineTool({
    name: "certify",
    label: "certify",
    description:
      "REQUEST acceptance: tells the harness to run the accepting witness cycle for a target. Call AFTER witness reports CERTIFIABLE (or when hexdiff shows mismatch: 0 / structural: 0). Read-only — the harness executes the actual cycle. IMPORTANT: include a line exactly like 'CERTIFY: <target-id>' in your FINAL response so the harness acts on it.",
    promptSnippet: "certify <target-id> — request the harness's accepting witness cycle",
    parameters: Type.Object({
      targetId: Type.String({ description: "target id to certify (from the brief, e.g. us-80244520)" }),
    }),
    execute: async (_id, params) => {
      const text =
        `CERTIFY request noted for ${params.targetId}.\n\n` +
        `In your FINAL response, include this exact line so the harness runs ` +
        `the accepting witness cycle:\n\n` +
        `CERTIFY: ${params.targetId}\n\n` +
        `The harness will verify the code still certifies (witness, no SMT) ` +
        `and record acceptance. If it does not certify, the target returns to ` +
        `the batch pool and you can keep iterating.`;
      return {
        content: [{ type: "text", text }],
        details: { ok: true, requested: true, targetId: params.targetId },
      };
    },
  });
}

// ─────────────────────────────────────────────────────────────────────
//  targets — read-only target identity
// ─────────────────────────────────────────────────────────────────────

/** targets custom tool definition: run.py targets show, read-only. */
export function targetsTool(repoRoot: string, python: string): ToolDefinition {
  return defineTool({
    name: "targets",
    label: "targets",
    description:
      "Show one target's registry record (identity, status, required_level, unit, symbol). Read-only — cannot claim or modify targets.",
    promptSnippet: "targets <id> — show target record",
    parameters: Type.Object({
      targetId: Type.String({ description: "target id from the brief (e.g. us-802bad98)" }),
    }),
    execute: async (_id, params) => {
      try {
        const { stdout, stderr } = await run(
          python, ["tools/coop/run.py", "targets", "show", params.targetId], repoRoot,
        );
        const text = (stdout + stderr).trim();
        return { content: [{ type: "text", text: text || "(no record)" }], details: { ok: true } };
      } catch (err) {
        return { content: [{ type: "text", text: `ERROR: ${(err as Error).message}` }], details: { ok: false } };
      }
    },
  });
}

// ─────────────────────────────────────────────────────────────────────
//  kb — MWCC knowledge base search (identity + mismatch terms)
// ─────────────────────────────────────────────────────────────────────

interface KbRow {
  id: string;
  source_kind: string;
  title: string;
  status?: string;
  match_percent?: number;
  symbol?: string | null;
  unit?: string | null;
  tags?: string[];
  snippet?: string;
  section?: string;
}

/**
 * Run `mwcc_kb.py search --json` and parse rows.
 */
async function kbSearch(
  repoRoot: string, python: string,
  query: string, args: string[], limit: number,
): Promise<KbRow[]> {
  const full = [
    "tools/mwcc_kb.py", "search", query,
    "--json", "--limit", String(limit),
    ...args,
  ];
  try {
    const { stdout } = await run(python, full, repoRoot);
    // stdout may be prefixed with the "indexed N entries" notice
    const jsonStart = stdout.indexOf("{");
    if (jsonStart < 0) return [];
    const d = JSON.parse(stdout.slice(jsonStart)) as { results?: KbRow[] };
    return d.results ?? [];
  } catch {
    return [];
  }
}

function kbRowLine(r: KbRow): string {
  const status = r.status ? ` [${r.status}${r.match_percent !== undefined ? ` ${r.match_percent}%` : ""}]` : "";
  const tags = r.tags && r.tags.length > 0 ? ` (${r.tags.join(", ")})` : "";
  const snippet = r.snippet ? `\n  ${r.snippet.replace(/\n/g, "\n  ").slice(0, 220)}` : "";
  return `- ${r.title}${status}${tags}${snippet}`;
}

/**
 * Improved KB tool: the skill's dual search (identity + mismatch terms) in
 * one call. Auto-detects whether the query is a symbol (mangled/addr style)
 * or free mismatch terms:
 *  - symbol query → attempt hits (sibling tries, status, match %) + matching
 *    reference patterns, both capped.
 *  - terms query  → reference + attempt hits on the terms.
 * Optional kind/tag/status filters pass through to the underlying search.
 */
export function kbTool(repoRoot: string, python: string): ToolDefinition {
  return defineTool({
    name: "kb",
    label: "MWCC KB",
    description:
      "Search the MWCC matching knowledge base (docs/MWCC_REFERENCE.md patterns + attempt history) by function/symbol or short mismatch terms. Returns top reference patterns and sibling attempts with status + match% + tags — use before iterating on a target to find known codegen patterns and what others already tried.",
    promptSnippet: "kb <query> — MWCC knowledge search (symbol or mismatch terms)",
    parameters: Type.Object({
      query: Type.String({ description: "mangled symbol, function name, or short mismatch terms (e.g. 'reg swap mullw')" }),
      kind: Type.Optional(Type.String({ description: "filter: reference | attempt | kb_contribution (default: both)" })),
      tag: Type.Optional(Type.String({ description: "filter by tag (compiler_flags, regalloc, struct_layout, stack_frame, vtable, ...)" })),
      status: Type.Optional(Type.String({ description: "filter by match status (e.g. FULL_MATCH, EQUIVALENT_MATCH, HIGH_MATCH)" })),
      limit: Type.Optional(Type.Integer({ description: "max rows per kind (default 4)" })),
    }),
    execute: async (_id, params) => {
      const limit = params.limit ?? 4;
      const isSymbol = /(?:func_[0-9A-Fa-f]{8}|_[A-Za-z0-9_]+__|__[A-Za-z0-9_]+Fv)/.test(params.query);
      const common: string[] = [];
      if (params.tag) common.push("--tag", params.tag);
      if (params.status) common.push("--status", params.status);

      // Identity search (attempts for this exact symbol/function).
      const attemptArgs = ["--kind", "attempt", ...common];
      const refArgs = ["--kind", "reference", ...common];

      const [attempts, refs] = await Promise.all([
        kbSearch(repoRoot, python, params.query, attemptArgs, limit),
        kbSearch(repoRoot, python, params.query, refArgs, limit),
      ]);

      // If a kind filter was given, drop the other side.
      const showAttempts = params.kind ? params.kind === "attempt" || params.kind === "kb_contribution" : true;
      const showRefs = params.kind ? params.kind === "reference" : true;

      const lines: string[] = [];
      if (showRefs && refs.length > 0) {
        lines.push(`## MWCC reference patterns${isSymbol ? " (symbol query)" : ""}`);
        for (const r of refs) lines.push(kbRowLine(r));
      }
      if (showAttempts && attempts.length > 0) {
        lines.push(`\n## Sibling attempts${isSymbol ? " for this symbol" : ""}`);
        for (const a of attempts) lines.push(kbRowLine(a));
      }
      if (lines.length === 0) {
        lines.push("(no KB hits — try shorter mismatch terms or a mangled symbol)");
      }
      const text = lines.join("\n");
      return { content: [{ type: "text", text }], details: { ok: true, attempts: attempts.length, refs: refs.length } };
    },
  });
}

// ─────────────────────────────────────────────────────────────────────
//  ctx — decomp.me context for a source file
// ─────────────────────────────────────────────────────────────────────

/** ctx custom tool: run.py ctx <source> (read-only decomp.me context). */
export function ctxTool(repoRoot: string, python: string): ToolDefinition {
  return defineTool({
    name: "ctx",
    label: "ctx",
    description:
      "Generate decomp.me context (struct layouts, type definitions, related decls) for a source file. Use when a function's struct field offsets or types are unclear and the brief's header block isn't enough.",
    promptSnippet: "ctx <source> — decomp.me context for a source file",
    parameters: Type.Object({
      source: Type.String({ description: "source file path (e.g. src/kyoshin/help/CHelp_LandMark.cpp) or objdiff unit hint" }),
    }),
    execute: async (_id, params) => {
      try {
        const { stdout, stderr } = await run(python, ["tools/coop/run.py", "ctx", params.source], repoRoot);
        const text = (stdout + stderr).trim();
        const truncated = text.length > 8000 ? text.slice(0, 8000) + "\n… (truncated)" : text;
        return { content: [{ type: "text", text: truncated || "(no context generated)" }], details: { ok: true } };
      } catch (err) {
        return { content: [{ type: "text", text: `ERROR: ${(err as Error).message}` }], details: { ok: false } };
      }
    },
  });
}

// ─────────────────────────────────────────────────────────────────────
//  TU-final bash with spawnHook allowlist
// ─────────────────────────────────────────────────────────────────────

/**
 * spawnHook that hard-blocks everything except the whitelisted TU-final
 * commands. This is the no-bash-for-batch counterpart: TU-final legitimately
 * needs run.py diff/size/symbols + build_lock configure/ninja, so it gets a
 * constrained shell instead of a raw one.
 *
 * Allowlist (command prefixes, matched on the first token(s)):
 *   - <python> tools/coop/run.py diff|size|symbols
 *   - <python> tools/pi_harness/build_lock.py
 *   - <python> tools/coop/hexdiff.py   (verification during finalisation)
 *   - <python> configure.py / ninja    (only via build_lock — enforced below)
 * Everything else — git, cycle, batch-cycle, targets writes, --smt anywhere —
 * is blocked at the tool level.
 */
export function tuFinalSpawnHook(python: string): BashSpawnHook {
  const allowPrefixes = [
    `${python} tools/coop/run.py diff`,
    `${python} tools/coop/run.py size`,
    `${python} tools/coop/run.py symbols`,
    `${python} tools/coop/hexdiff.py`,
    `${python} tools/pi_harness/build_lock.py`,
    `${python} configure.py`,
    "ninja",
  ];
  return (ctx) => {
    const cmd = ctx.command.trim();
    const lower = cmd.toLowerCase();
    // Hard block: SMT anywhere (including `diff` WITHOUT --no-smt, which
    // defaults the probe ON), git reverts, harness-owned cycles, registry
    // writes.
    if (/\-\-smt\b|\-\-linked\b|smt\s*=\s*true/i.test(cmd)) {
      throw new Error(`BLOCKED: SMT is disabled in this repo (command contained --smt/--linked).`);
    }
    if (/\btools\/coop\/run\.py\s+diff\b(?!.*\-\-no-smt)/i.test(cmd)) {
      throw new Error("BLOCKED: run.py diff defaults the SMT probe ON — add --no-smt (or use the hexdiff tool).");
    }
    if (/\bgit\s+(checkout|reset|restore|clean|push|fetch|merge|rebase)\b/.test(lower)) {
      throw new Error("BLOCKED: git revert/push/fetch commands are disabled (shared branch).");
    }
    if (/\btools\/coop\/run\.py\s+(cycle|batch-cycle|queue|harness)\b/.test(lower) ||
        /\bbatch-cycle\.py\b/.test(lower)) {
      throw new Error("BLOCKED: cycle/batch-cycle/queue/harness are harness-owned.");
    }
    if (/\btools\/coop\/run\.py\s+targets\s+(claim|release|sync|recertify|migrate|import|edit|write)/.test(lower)) {
      throw new Error("BLOCKED: target registry mutations are harness-owned.");
    }
    // Command-substitution / injection block (adversarial review C2): the
    // segment-split below only splits on [;&|], so a NEWLINE or `$(...)` /
    // backtick inside an otherwise-allowed prefix would smuggle a second
    // command through (e.g. `ninja\nrm -rf …` or `ninja $(rm -rf …)`).
    if (/[`$]\(/.test(cmd) || /`/.test(cmd)) {
      throw new Error(
        `BLOCKED: command substitution (backticks / $()) is not allowed in TU-final bash. Got: ${cmd.slice(0, 120)}`,
      );
    }
    // Newlines are command separators to bash; forbid them so the prefix
    // allowlist cannot be bypassed with a second line.
    if (/\n/.test(cmd)) {
      throw new Error(
        `BLOCKED: multi-line commands are not allowed in TU-final bash (prefix check is per-line). Got: ${cmd.slice(0, 120)}`,
      );
    }
    // Whitelist: everything else is blocked. Multi-command chains are
    // rejected unless EVERY segment starts with an allowed prefix.
    const segments = cmd.split(/[;&|]\s*/).map((s) => s.trim()).filter(Boolean);
    const allAllowed = segments.length > 0 && segments.every((seg) =>
      allowPrefixes.some((p) => seg.startsWith(p) || seg.toLowerCase().startsWith(p.toLowerCase()))
    );
    if (!allAllowed) {
      throw new Error(
        `BLOCKED: command not in the TU-final allowlist. Allowed: run.py diff/size/symbols, hexdiff, build_lock.py, configure.py, ninja. Got: ${cmd.slice(0, 120)}`,
      );
    }
    // build_lock.py is a passthrough runner: whatever follows `--` is
    // executed. An allowed prefix on the OUTER command must not smuggle an
    // arbitrary inner command (adversarial review C2: `build_lock.py us --
    // python3 -c '…'` or `build_lock.py us -- git status`). Validate the
    // inner command against the same allowlist.
    const blMatch = cmd.match(/build_lock\.py\s+(?:--timeout\s+\d+\s+)?[^\s]+\s+--\s+(.+)$/);
    if (blMatch) {
      const inner = blMatch[1].trim();
      const innerSegments = inner.split(/[;&|]\s*/).map((s) => s.trim()).filter(Boolean);
      const innerAllowed = innerSegments.length > 0 && innerSegments.every((seg) =>
        allowPrefixes.some((p) => seg.startsWith(p) || seg.toLowerCase().startsWith(p.toLowerCase()))
      );
      if (!innerAllowed) {
        throw new Error(
          `BLOCKED: build_lock.py inner command not in the TU-final allowlist. Got: ${inner.slice(0, 120)}`,
        );
      }
    }
    return ctx;
  };
}

/** TU-final bash tool (built-in bash behind the spawnHook allowlist). */
export function tuFinalBashTool(repoRoot: string, python: string): ToolDefinition {
  // createBashTool returns an AgentTool with a TypeBox schema already baked
  // in — reuse it with the spawnHook restriction.
  const bash = createBashTool(repoRoot, {
    spawnHook: tuFinalSpawnHook(python),
    exposeSessionEnvironment: false,
  });
  return bash as unknown as ToolDefinition;
}

/** True when an absolute path is inside the session's writable scope
 *  (adversarial review H4: the writable scope was prompt-level only, so the
 *  SDK edit/write tools could modify out-of-scope files — including
 *  configure.py and tools/coop/targets.json — and the snapshot restore never
 *  reverted them because they were never snapshotted). */
function assertWritablePath(repoRoot: string, writable: string[], absPath: string): void {
  const abs = resolve(absPath);
  const rel = relative(repoRoot, abs);
  // Allow anything inside build/pi-harness state dirs (transcripts etc.).
  if (rel.startsWith("build/pi-harness")) return;
  const allowed = writable.map((w) => resolve(repoRoot, w));
  if (!allowed.some((a) => abs === a || abs.startsWith(a + "/"))) {
    throw new Error(
      `BLOCKED: ${absPath} is outside the writable scope for this session. ` +
        `Allowed files: ${writable.length > 0 ? writable.join(", ") : "(none)"}`,
    );
  }
}

/** True when a DIRECTORY is an ancestor of (or equal to) an allowed path —
 *  the SDK write tool calls mkdir(dirname(file)), and a parent directory is
 *  legitimately creatable even though it is not itself in the writable list. */
function assertWritableDir(repoRoot: string, writable: string[], absDir: string): void {
  const dir = resolve(absDir);
  const rel = relative(repoRoot, dir);
  if (rel.startsWith("build/pi-harness")) return;
  const allowed = writable.map((w) => resolve(repoRoot, w));
  const isAncestorOfAllowed = allowed.some((a) => a === dir || a.startsWith(dir + "/"));
  if (!isAncestorOfAllowed) {
    throw new Error(
      `BLOCKED: ${absDir} is not an ancestor of any writable file for this session. ` +
        `Allowed files: ${writable.length > 0 ? writable.join(", ") : "(none)"}`,
    );
  }
}

/** Edit tool restricted to the session's writable scope. Overrides the
 *  built-in `edit` (custom tools replace built-ins by name). */
export function scopedEditTool(repoRoot: string, writable: string[]): ToolDefinition {
  const ops: EditOperations = {
    readFile: async (p) => {
      assertWritablePath(repoRoot, writable, p);
      return readFile(p);
    },
    writeFile: async (p, content) => {
      assertWritablePath(repoRoot, writable, p);
      await writeFile(p, content, "utf-8");
    },
    access: async (p) => {
      assertWritablePath(repoRoot, writable, p);
      await fsAccess(p);
    },
  };
  return createEditToolDefinition(repoRoot, { operations: ops }) as unknown as ToolDefinition;
}

/** Write tool restricted to the session's writable scope. Overrides the
 *  built-in `write` (custom tools replace built-ins by name). */
export function scopedWriteTool(repoRoot: string, writable: string[]): ToolDefinition {
  const ops: WriteOperations = {
    writeFile: async (p, content) => {
      assertWritablePath(repoRoot, writable, p);
      await writeFile(p, content, "utf-8");
    },
    mkdir: async (dir) => {
      const { mkdir } = await import("node:fs/promises");
      // The SDK passes dirname(file); allow it when it is an ancestor of an
      // allowed file (it is NOT itself in the writable list).
      assertWritableDir(repoRoot, writable, dir);
      await mkdir(dir, { recursive: true });
    },
  };
  return createWriteToolDefinition(repoRoot, { operations: ops }) as unknown as ToolDefinition;
}

/** Batch-session tool set: no bash at all. The built-in edit/write tools are
 *  replaced by scoped versions restricted to `writable` (H4). */
export function batchSessionTools(repoRoot: string, python: string, writable: string[] = []): ToolDefinition[] {
  return [
    scopedEditTool(repoRoot, writable),
    scopedWriteTool(repoRoot, writable),
    hexdiffTool(repoRoot, python),
    symbolsTool(repoRoot, python),
    targetsTool(repoRoot, python),
    witnessTool(repoRoot, python),
    certifyTool(),
    unitStatusTool(repoRoot, python),
    kbTool(repoRoot, python),
    ctxTool(repoRoot, python),
  ];
}

/** TU-final tool set: batch tools + constrained bash. */
export function tuFinalSessionTools(repoRoot: string, python: string, writable: string[] = []): ToolDefinition[] {
  return [...batchSessionTools(repoRoot, python, writable), tuFinalBashTool(repoRoot, python)];
}

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
import { join } from "node:path";
import { Type } from "typebox";
import {
  defineTool,
  createBashTool,
  type BashSpawnHook,
  type ToolDefinition,
} from "@earendil-works/pi-coding-agent";

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
  // Fast path: --no-build read of the existing object. If it fails (missing
  // or mid-write object), rebuild — and on build failure surface the ACTUAL
  // compiler error (stderr), never a generic message.
  let result = await build(true);
  if (!result.stdout) result = await build(false);
  if (!result.stdout) {
    // Surface the ACTUAL compiler error, not the ninja command dump that
    // precedes it (which is ~15 lines of flags). mwcceppc errors look like:
    //   ### mwcceppc.exe Compiler:
    //   #    File: src\foo.cpp
    //   #     67:     THIS_SHOULD_NOT_COMPILE_XYZ;
    //   #   Error:     ^^^^^^^^^^^^^^^^^^^
    //   #   (10140) undefined identifier 'THIS_SHOULD_NOT_COMPILE_XYZ'
    const errLines = result.stderr.trim().split("\n");
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

/** Batch-session tool set: no bash at all. */
export function batchSessionTools(repoRoot: string, python: string): ToolDefinition[] {
  return [
    hexdiffTool(repoRoot, python),
    symbolsTool(repoRoot, python),
    targetsTool(repoRoot, python),
    kbTool(repoRoot, python),
    ctxTool(repoRoot, python),
  ];
}

/** TU-final tool set: batch tools + constrained bash. */
export function tuFinalSessionTools(repoRoot: string, python: string): ToolDefinition[] {
  return [...batchSessionTools(repoRoot, python), tuFinalBashTool(repoRoot, python)];
}

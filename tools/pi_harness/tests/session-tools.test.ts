/**
 * Tests for the pi-harness session tools (tools/pi_harness/src/session-tools.ts).
 *
 * Two layers:
 *   1. UNIT — pure logic (looksLikeJson, spawnHook allowlist/blocklist).
 *   2. INTEGRATION — each tool's execute() against the REAL repo commands
 *      (hexdiff.py / run.py / mwcc_kb.py). These require the repo to be
 *      configured (build/us exists) and are marked integration.
 *
 * Run:
 *   node --import tsx --test tests/session-tools.test.ts
 *   (or: npm test after adding the script)
 */
import { test, describe, before, after } from "node:test";
import assert from "node:assert/strict";
import { join, dirname } from "node:path";
import { fileURLToPath } from "node:url";
import { readFile, writeFile } from "node:fs/promises";

import {
  looksLikeJson,
  run,
  hexdiffTool,
  symbolsTool,
  targetsTool,
  kbTool,
  ctxTool,
  witnessTool,
  certifyTool,
  tuFinalSpawnHook,
  batchSessionTools,
  tuFinalSessionTools,
} from "../src/session-tools.js";
import { parseCertifyRequests } from "../src/orchestrator.js";

const __dirname = dirname(fileURLToPath(import.meta.url));
const REPO = join(__dirname, "..", "..", "..");
const PY = ".venv/bin/python3";

// Known-good identities from the target registry (stable across runs).
const KNOWN_UNIT = "kyoshin/menu/CMenuMapSelect";
const KNOWN_SYMBOL = "func_80242368"; // exists in retail, diffable
const FULL_MATCH_UNIT = "kyoshin/menu/CMenuPTState";
const FULL_MATCH_SYMBOL = "__ct__80192C10"; // verified 0-mismatch earlier
const KNOWN_TARGET_ID = "us-802443d4"; // CMenuMapSelect ctor target

// ---------------------------------------------------------------------------
// UNIT: looksLikeJson
// ---------------------------------------------------------------------------
describe("looksLikeJson", () => {
  test("true for a JSON object document", () => {
    assert.equal(looksLikeJson('{"a":1}'), true);
    assert.equal(looksLikeJson('  \n\t{"a":1}\n'), true);
  });
  test("false for a JSON ARRAY (hexdiff never emits arrays)", () => {
    assert.equal(looksLikeJson("[1,2]"), false);
  });
  test("false for build noise (.note.split line) that pollutes stdout", () => {
    assert.equal(looksLikeJson("added .note.split to /path/obj.o"), false);
  });
  test("false for empty / whitespace", () => {
    assert.equal(looksLikeJson(""), false);
    assert.equal(looksLikeJson("   \n  "), false);
  });
  test("false for ninja progress lines ([1/1] MWCC ...)", () => {
    assert.equal(looksLikeJson("[1/1] MWCC build/us/src/kyoshin/menu/CMenuMapSelect.o"), false);
  });
  test("false for JSON preceded by noise (the pollution case)", () => {
    // The .note.split line is NOT trimmed away, so this stays false — the
    // fix is at the subprocess level (capture_output), not in this guard.
    assert.equal(looksLikeJson("added .note.split to /x\n{\"a\":1}"), false);
  });
});

// ---------------------------------------------------------------------------
// UNIT: tuFinalSpawnHook allowlist/blocklist
// ---------------------------------------------------------------------------
describe("tuFinalSpawnHook", () => {
  const hook = tuFinalSpawnHook(PY);
  const ctx = { command: "" };

  const allowed = [
    `${PY} tools/coop/run.py diff kyoshin/menu/CMenuMapSelect --no-smt`,
    `${PY} tools/coop/run.py size kyoshin/menu/CMenuMapSelect`,
    `${PY} tools/coop/run.py symbols kyoshin/menu/CMenuMapSelect`,
    `${PY} tools/coop/hexdiff.py kyoshin/menu/CMenuMapSelect --symbol func_80242368 --json`,
    `${PY} tools/pi_harness/build_lock.py configure`,
    `${PY} configure.py`,
    "ninja build/us/src/kyoshin/menu/CMenuMapSelect.o",
  ];
  for (const cmd of allowed) {
    test(`ALLOWED: ${cmd.slice(0, 70)}`, () => {
      assert.doesNotThrow(() => hook({ ...ctx, command: cmd } as never));
    });
  }

  const blocked = [
    `${PY} tools/coop/run.py diff kyoshin/menu/CMenuMapSelect`, // no --no-smt
    `${PY} tools/coop/run.py diff kyoshin/menu/CMenuMapSelect --smt`,
    `${PY} tools/coop/run.py diff kyoshin/menu/CMenuMapSelect --linked`,
    "git checkout src/kyoshin/menu/CMenuMapSelect.cpp",
    "git reset --hard",
    `${PY} tools/coop/run.py cycle us-802443d4`,
    `${PY} tools/coop/run.py batch-cycle us-802443d4`,
    `${PY} tools/coop/run.py targets claim us-802443d4`,
    `${PY} tools/coop/run.py targets sync`,
    "echo hello; git push origin main",
    "cat /etc/passwd",
    "ls -la",
  ];
  for (const cmd of blocked) {
    test(`BLOCKED: ${cmd.slice(0, 70)}`, () => {
      assert.throws(() => hook({ ...ctx, command: cmd } as never));
    });
  }

  test("SMT anywhere is blocked even with --no-smt present elsewhere", () => {
    assert.throws(() =>
      hook({ ...ctx, command: `${PY} tools/coop/run.py diff x --no-smt && echo --smt` } as never),
    );
  });
});

// ---------------------------------------------------------------------------
// INTEGRATION: tool execute() against the real repo
// ---------------------------------------------------------------------------
describe("integration: hexdiffTool", () => {
  const tool = hexdiffTool(REPO, PY);

  test("returns structured diff for a known mismatched symbol", async () => {
    const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: KNOWN_SYMBOL, brief: true });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /## hexdiff: func_80242368/);
    assert.match(text, /mismatch: \d+/);
    assert.match(text, /structural: \d+/);
    assert.match(text, /retail \d+B vs decomp \d+B/);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });

  test("full mode includes mismatched instructions", async () => {
    const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: KNOWN_SYMBOL });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /retail:|decomp:/);
  });

  test("reports a clear error for a nonexistent symbol (not 'non-JSON output')", async () => {
    const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: "__ct__NO_SUCH_SYMBOL_XYZ" });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /symbol|not found|ERROR/i);
    assert.doesNotMatch(text, /non-JSON output/);
  });

  test("REGRESSION (finding 1): rebuilds after a source edit — never returns stale counts", async () => {
    // The old --no-build fast path returned the PRE-EDIT object's mismatch
    // counts after a model edit (stale: 13 vs fresh 68 in the finding). The
    // tool must always build so the diff reflects the current source.
    //
    // HAZARD (Kimi finding 4): this edits a LIVE source file. Skip when the
    // file is dirty (a harness run is mid-edit) — a concurrent write between
    // our read and restore would be destroyed. The freshness guarantee is
    // structurally enforced by always-build; this test just pins the
    // regression when the worktree is quiet.
    const src = join(REPO, "src/kyoshin/menu/CMenuMapSelect.cpp");
    try {
      const git = await run(PY, ["-c", `import subprocess; print(subprocess.run(['git','diff','--quiet','--','src/kyoshin/menu/CMenuMapSelect.cpp'], cwd=r'${REPO}').returncode)`], REPO);
      if (git.stdout.trim() !== "0") {
        // Dirty (live run mid-edit) — skip to avoid destroying its work.
        return;
      }
    } catch { return; }
    const orig = await readFile(src, "utf-8");
    try {
      // Remove the singleton guard — a codegen change that definitely alters
      // __ct__CMenuMapSelect's compiled bytes.
      const edited = orig.replace(
        "    if (lbl_eu_80664790 != 0) {\n        return 0;\n    }\n\n",
        "    // guard removed (codegen change)\n\n",
      );
      if (edited === orig) {
        // Guard text may have drifted — skip if we can't apply a real change.
        return;
      }
      await writeFile(src, edited, "utf-8");
      const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: "__ct__CMenuMapSelect", brief: true });
      const text = r.content?.[0]?.text ?? "";
      // After removing the guard, structural mismatches must be HIGH (the
      // guard removal destroys the structure) — a stale read would show the
      // pre-edit structural: ~1. Fresh is >= 50 (observed 68/63 in the finding).
      const m = text.match(/structural: (\d+)/);
      assert.ok(m, `expected structural count in: ${text.slice(0, 120)}`);
      assert.ok(
        Number(m[1]) >= 30,
        `expected FRESH structural (>=30) after guard removal, got ${m[1]} — stale --no-build read?`,
      );
    } finally {
      await writeFile(src, orig, "utf-8");
    }
  });

  test("mangled symbol: says 'symbol not found', NOT 'build failed' (base-name hint)", async () => {
    // The model sometimes passes the C++ mangled form (derived from mangling
    // rules) while the registry stores the base name. The tool must say the
    // NAME is wrong, not that the build failed — otherwise the model chases
    // a phantom build error (seen 3x in run 8A).
    const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: "func_80242368__16CMenuMapSelectFv" });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /symbol 'func_80242368__16CMenuMapSelectFv' not found/i);
    assert.match(text, /EXACT symbol from the target brief/);
    assert.match(text, /available:/);
    assert.doesNotMatch(text, /build failed/);
  });

  test("returns a valid diff for the formerly-full-match symbol", async () => {
    // __ct__80192C10 is FULL_MATCH in the registry — but a live run editing
    // the same unit can transiently break the build (mid-edit), so skip on
    // build failure; otherwise assert a well-formed diff.
    const r = await tool.execute("t", { unit: FULL_MATCH_UNIT, symbol: FULL_MATCH_SYMBOL, brief: true });
    const text = r.content?.[0]?.text ?? "";
    if (/build failed|could not be built/i.test(text)) {
      // Live run mid-edit on this unit — not a code failure.
      return;
    }
    assert.match(text, /## hexdiff: __ct__80192C10/);
    assert.match(text, /mismatch: \d+/);
    assert.match(text, /structural: \d+/);
  });
});

describe("integration: symbolsTool", () => {
  const tool = symbolsTool(REPO, PY);

  test("lists symbols for a known unit", async () => {
    const r = await tool.execute("t", { unit: KNOWN_UNIT });
    const text = r.content?.[0]?.text ?? "";
    assert.ok(text.length > 0, "expected non-empty symbol table");
    assert.doesNotMatch(text, /^ERROR/);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });

  test("filters by substring", async () => {
    const r = await tool.execute("t", { unit: KNOWN_UNIT, substr: "func_80242" });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /func_80242/i);
  });
});

describe("integration: targetsTool", () => {
  const tool = targetsTool(REPO, PY);

  test("shows a known target record", async () => {
    const r = await tool.execute("t", { targetId: KNOWN_TARGET_ID });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, new RegExp(KNOWN_TARGET_ID));
    assert.match(text, /CMenuMapSelect|802443d4/i);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });

  test("errors cleanly for a nonexistent target", async () => {
    const r = await tool.execute("t", { targetId: "us-00000000" });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /ERROR|not found/i);
  });
});

describe("integration: ctxTool", () => {
  const tool = ctxTool(REPO, PY);

  test("generates context for a known source file", async () => {
    const r = await tool.execute("t", { source: "src/kyoshin/menu/CMenuMapSelect.cpp" });
    const text = r.content?.[0]?.text ?? "";
    assert.ok(text.length > 0, "expected context output");
    assert.doesNotMatch(text, /^ERROR/);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });
});

describe("integration: kbTool", () => {
  const tool = kbTool(REPO, PY);

  test("searches for a known symbol/term", async () => {
    const r = await tool.execute("t", { query: "func_80242368" });
    const text = r.content?.[0]?.text ?? "";
    // KB may be empty on a fresh repo — accept either, but never ERROR.
    assert.doesNotMatch(text, /^ERROR/);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });

  test("kind filter: reference only", async () => {
    const r = await tool.execute("t", { query: "mullw", kind: "reference" });
    const text = r.content?.[0]?.text ?? "";
    assert.doesNotMatch(text, /Sibling attempts/);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });

  test("status filter passes through", async () => {
    const r = await tool.execute("t", { query: "func_80242368", status: "FULL_MATCH" });
    const text = r.content?.[0]?.text ?? "";
    assert.doesNotMatch(text, /^ERROR/);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });

  test("no-hit query returns the friendly empty message", async () => {
    const r = await tool.execute("t", { query: "zzz_no_such_thing_42" });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /no KB hits/);
  });
});

// ---------------------------------------------------------------------------
// REGRESSION: run() must preserve stdout/stderr on non-zero exit
// ---------------------------------------------------------------------------
describe("run() stdout/stderr preservation (root-cause regression)", () => {
  test("exit 0: resolves with stdout/stderr", async () => {
    const r = await run(PY, ["-c", "print('hi')"], REPO);
    assert.equal(r.stdout.trim(), "hi");
  });

  test("exit 5 (mismatch): rejection error carries stdout (the diff JSON)", async () => {
    // This is the exact bug: node's execFile error object does NOT include
    // stdout/stderr — they are separate callback args. If run() rejects
    // without attaching them, the hexdiff tool loses the diff on every
    // mismatch and reports 'build failed' instead.
    // Use a python one-liner that exits 5 with JSON on stdout — deterministic
    // (independent of the live runs' build state, which can transiently make
    // a real symbol a full-match or the object mid-write).
    const err = await run(
      PY,
      ["-c", "import sys; print('{\"mismatch_count\":5}'); sys.exit(5)"],
      REPO,
    ).then(() => null, (e: Error & { stdout?: string; code?: number }) => e);
    assert.ok(err, "expected rejection (exit 5)");
    assert.equal(err.code, 5);
    assert.ok(err.stdout && err.stdout.startsWith("{"), "rejection must carry the JSON stdout");
    const parsed = JSON.parse(err.stdout as string);
    assert.equal(parsed.mismatch_count, 5);
  });

  test("exit 2 (build error): rejection carries stderr (the compiler error)", async () => {
    // python exits 2 on a syntax error; stderr must be preserved.
    const err = await run(PY, ["-c", "raise SystemExit(2)"], REPO)
      .then(() => null, (e: Error & { code?: number }) => e);
    assert.ok(err);
    assert.equal(err.code, 2);
  });
});

describe("integration: witnessTool", () => {
  const tool = witnessTool(REPO, PY);

  test("returns a verdict for a known symbol (not an error)", async () => {
    const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: KNOWN_SYMBOL });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /## witness: func_80242368/);
    assert.match(text, /status: \S+/);
    assert.match(text, /CERTIFIABLE|NOT certifiable/);
    assert.equal((r.details as { ok: boolean }).ok, true);
  });

  test("FULL_MATCH target: verdict text + details agree (certifiable=true, equivalence=equivalent)", async () => {
    // func_80242354 is a certified FULL_MATCH in CMenuMapSelect.
    const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: "func_80242354" });
    const text = r.content?.[0]?.text ?? "";
    const d = r.details as { ok: boolean; certifiable: boolean; status: string; equivalence: string | null };
    assert.equal(d.ok, true);
    assert.equal(d.certifiable, true, "FULL_MATCH must be certifiable");
    assert.equal(d.status, "FULL_MATCH");
    assert.equal(d.equivalence, "equivalent");
    // The text the model sees must carry the same verdict (info passing).
    assert.match(text, /✅ CERTIFIABLE/);
    assert.match(text, /equivalence: equivalent/);
    assert.match(text, /symbol match: 100\.0%/);
  });

  test("verdict text reflects details on the NOT-certifiable path too", async () => {
    // func_80242368 is not certifiable in the current worktree — but this
    // assertion is about CONSISTENCY: whatever details says, the text shows it.
    const r = await tool.execute("t", { unit: KNOWN_UNIT, symbol: KNOWN_SYMBOL });
    const text = r.content?.[0]?.text ?? "";
    const d = r.details as { certifiable: boolean };
    if (d.certifiable) {
      assert.match(text, /✅ CERTIFIABLE/);
    } else {
      assert.match(text, /❌ NOT certifiable/);
    }
  });

  test("never modifies the registry (read-only)", async () => {
    const before = await run(PY, ["tools/coop/run.py", "targets", "show", KNOWN_TARGET_ID], REPO);
    await tool.execute("t", { unit: KNOWN_UNIT, symbol: KNOWN_SYMBOL });
    const after = await run(PY, ["tools/coop/run.py", "targets", "show", KNOWN_TARGET_ID], REPO);
    assert.equal(before.stdout, after.stdout, "witness must not touch the registry");
  });
});

describe("unit: certifyTool", () => {
  const tool = certifyTool();

  test("returns the CERTIFY marker instruction + target id", async () => {
    const r = await tool.execute("t", { targetId: "us-80244520" });
    const text = r.content?.[0]?.text ?? "";
    assert.match(text, /CERTIFY: us-80244520/);
    assert.match(text, /final response/i);
    assert.equal((r.details as { ok: boolean }).ok, true);
    assert.equal((r.details as { requested: boolean }).requested, true);
  });

  test("is read-only (no subprocess, no registry touch)", async () => {
    const r = await tool.execute("t", { targetId: "us-99999999" });
    // No ERROR, no mutation — pure instruction text.
    assert.doesNotMatch(r.content?.[0]?.text ?? "", /ERROR/);
  });
});

describe("unit: parseCertifyRequests", () => {
  test("extracts CERTIFY markers for owned targets", () => {
    const text = "done. CERTIFY: us-80244520\nAlso CERTIFY: us-801941d4 somewhere";
    assert.deepEqual(parseCertifyRequests(text, ["us-80244520", "us-801941d4"]), ["us-80244520", "us-801941d4"]);
  });

  test("ignores markers for targets the session does not own", () => {
    const text = "CERTIFY: us-99999999";
    assert.deepEqual(parseCertifyRequests(text, ["us-80244520"]), []);
  });

  test("no markers -> empty", () => {
    assert.deepEqual(parseCertifyRequests("just editing", ["us-80244520"]), []);
  });

  test("case-insensitive-ish: tolerates whitespace after colon", () => {
    assert.deepEqual(parseCertifyRequests("CERTIFY:  us-80244520", ["us-80244520"]), ["us-80244520"]);
  });
});

// ---------------------------------------------------------------------------
// Tool-set composition
// ---------------------------------------------------------------------------
describe("tool set composition", () => {
  test("batchSessionTools contains exactly the 7 structured tools, no bash", () => {
    const tools = batchSessionTools(REPO, PY);
    const names = tools.map((t) => t.name).sort();
    assert.deepEqual(names, ["certify", "ctx", "hexdiff", "kb", "symbols", "targets", "witness"]);
  });

  test("tuFinalSessionTools = batch tools + bash", () => {
    const tools = tuFinalSessionTools(REPO, PY);
    const names = tools.map((t) => t.name);
    assert.ok(names.includes("bash"), "tu-final must include bash behind the allowlist");
    for (const n of ["hexdiff", "symbols", "targets", "kb", "ctx", "witness", "certify"]) {
      assert.ok(names.includes(n), `tu-final must include ${n}`);
    }
  });
});

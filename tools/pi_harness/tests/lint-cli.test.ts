import { mkdtempSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";
import { test } from "node:test";
import assert from "node:assert/strict";
import { execFilePromise } from "../src/acceptance.js";

const here = dirname(fileURLToPath(import.meta.url));
const repoRoot = join(here, "..", "..", "..");
const python = join(repoRoot, ".venv", "bin", "python3");

async function lintCli(args: string[]): Promise<{ ok: boolean; rules: string[] }> {
  const { stdout } = await execFilePromise(
    python,
    ["tools/pi_harness/lint_cli.py", ...args],
    { cwd: repoRoot },
  );
  const parsed = JSON.parse(stdout) as { ok?: boolean; violations?: Array<{ rule: string; line?: number }> };
  return { ok: parsed.ok ?? false, rules: (parsed.violations ?? []).map((v) => v.rule) };
}

test("lint_cli --file lints the whole file (pragma flagged)", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-file-"));
  const f = join(dir, "f.cpp");
  writeFileSync(f, "void f() {\n#pragma push\n    int x = 1;\n}\n");
  const { ok, rules } = await lintCli(["--file", f]);
  assert.equal(ok, false);
  assert.ok(rules.includes("no_pragmas"), `expected no_pragmas, got ${rules}`);
});

test("lint_cli --file clean file passes", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-clean-"));
  const f = join(dir, "g.cpp");
  writeFileSync(f, "void g() {\n    int x = 1;\n}\n");
  const { ok, rules } = await lintCli(["--file", f]);
  assert.equal(ok, true, `rules=${rules}`);
  assert.deepEqual(rules, []);
});

test("lint_cli --pair still does delta lint (only added lines)", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-pair-"));
  const oldF = join(dir, "old.cpp");
  const newF = join(dir, "new.cpp");
  writeFileSync(oldF, "void f() {\n    int x = 1;\n}\n");
  writeFileSync(newF, "void f() {\n#pragma push\n    int x = 1;\n}\n");
  const { ok, rules } = await lintCli(["--pair", oldF, newF]);
  assert.equal(ok, false);
  assert.ok(rules.includes("no_pragmas"));
});

test("lint_cli flags unkN unknown-name placeholders in new code", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-unk-var-"));
  const f = join(dir, "unk.cpp");
  writeFileSync(f, "u32 unk0 = this->unk0;\n");
  const { ok, rules } = await lintCli(["--file", f]);
  assert.equal(ok, false);
  assert.ok(rules.includes("no_unk_name"), `expected no_unk_name, got ${rules}`);
});

test("lint_cli flags UnkClass/UnkVirtualFunc generated types in new code", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-unk-gen-"));
  const f = join(dir, "gen.cpp");
  writeFileSync(f, "UnkClass_805764CC* p = get();\n");
  const { ok, rules } = await lintCli(["--file", f]);
  assert.equal(ok, false);
  assert.ok(rules.includes("no_unk_generated"), `expected no_unk_generated, got ${rules}`);
});

test("lint_cli delta mode ignores PRE-EXISTING unk usage (only added lines)", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-unk-delta-"));
  const oldF = join(dir, "old.cpp");
  const newF = join(dir, "new.cpp");
  writeFileSync(oldF, "u32 unk0 = this->unk0;\n");
  // Adding a clean line next to existing unk code must not flag the unk.
  writeFileSync(newF, "u32 unk0 = this->unk0;\nint count = 5;\n");
  const { ok, rules } = await lintCli(["--pair", oldF, newF]);
  assert.equal(ok, true, `rules=${rules}`);
  assert.deepEqual(rules, []);
});

test("lint_cli flags DECOMP_ASM_INSN single-instruction asm shims", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-asm-shim-"));
  const f = join(dir, "shim.cpp");
  writeFileSync(
    f,
    "void f() {\nDECOMP_ASM_INSN_BEGIN\nasm { b done }\nDECOMP_ASM_INSN_END\n}\n",
  );
  const { ok, rules } = await lintCli(["--file", f]);
  assert.equal(ok, false);
  assert.ok(rules.includes("no_asm_insn_shim"), `expected no_asm_insn_shim, got ${rules}`);
});

test("lint_cli flags the init-list side-effect trick (multi-line cast)", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-init-side-"));
  const f = join(dir, "seq.cpp");
  writeFileSync(
    f,
    [
      "struct T { T(); };",
      "T::T()",
      "    : mHandle(reinterpret_cast<SeqSoundHandle*>(",
      "          mPreparedFlag = mLoadingFlag = false)),",
      "      mOther(0) {}",
      "",
    ].join("\n"),
  );
  const { ok, rules } = await lintCli(["--file", f]);
  assert.equal(ok, false);
  assert.ok(rules.includes("no_init_side_effect"), `expected no_init_side_effect, got ${rules}`);
});

test("lint_cli delta mode flags an added one-line cast assignment", async () => {
  const dir = mkdtempSync(join(tmpdir(), "lint-init-delta-"));
  const oldF = join(dir, "old.cpp");
  const newF = join(dir, "new.cpp");
  writeFileSync(oldF, "struct T { T(); }; T::T() : mHandle(0), mOther(0) {}\n");
  writeFileSync(
    newF,
    "struct T { T(); }; T::T() : mHandle(reinterpret_cast<T*>(mFlag = 0)), mOther(0) {}\n",
  );
  const { ok, rules } = await lintCli(["--pair", oldF, newF]);
  assert.equal(ok, false);
  assert.ok(rules.includes("no_init_side_effect"), `expected no_init_side_effect, got ${rules}`);
});

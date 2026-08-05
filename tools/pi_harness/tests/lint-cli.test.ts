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

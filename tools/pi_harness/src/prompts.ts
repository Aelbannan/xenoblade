// ---------------------------------------------------------------------------
// Prompt assembly: batch-match and TU-finalisation prompts.
// ---------------------------------------------------------------------------

/** Append harness-specific instructions to the generated brief. */
export function buildBatchPrompt(opts: {
  brief: string;
  unit: string;
  targetIds: string[];
  pythonBin: string;
}): string {
  const { brief, unit, targetIds, pythonBin } = opts;
  return (
    brief +
    "\n## Harness instructions\n\n" +
    "- The `xenoblade-decomp` skill is already loaded — follow it (MWCC knowledge-base search, hexdiff loop, coding guidelines).\n" +
    `- Match as many of the ${targetIds.length} listed target(s) as possible, in order: ${targetIds.map((id) => `\`${id}\``).join(", ")}. Partial progress is fine — the harness accepts whatever passes.\n` +
    "- For diffs use the **`hexdiff` tool**: `hexdiff <unit> <symbol>` (counts + reloc drift + fix suggestions). For symbol lookup use **`symbols`**; for target records use **`targets`**.\n" +
    "- **Certification (IMPORTANT)**: when `hexdiff` shows `mismatch: 0` or `structural: 0` (only registers differ), call the **`witness` tool**: `witness <unit> <symbol>` — it runs the register-renaming witness (no SMT) and tells you if the code would be accepted. If it says CERTIFIABLE, call **`certify <target-id>`** and then include `CERTIFY: <target-id>` in your FINAL response so the harness accepts it. **Do NOT keep editing past a match** — you will regress it.\n" +
    "- **Before iterating on a target, search the MWCC knowledge base with `kb`**: `kb <symbol>` (sibling attempts + reference patterns with status/match%) and `kb <short mismatch terms> tag=<category>` for known codegen fixes. Use **`ctx <source>`** for struct layouts when the brief's headers aren't enough.\n" +
    "- There is NO bash in this session — the structured tools (`hexdiff`, `symbols`, `targets`, `kb`, `ctx`, `witness`, `certify`, `unit-status`) cover the whole loop. No SMT anywhere: `--smt`/`--linked` and plain `run.py diff` are blocked at the tool level; the register-renaming witness is the only equivalence path (the harness runs it in acceptance).\n" +
    "- Do NOT run `cycle`, `batch-cycle`, `ninja`, or `configure.py` — the harness owns acceptance (and you have no shell to run them with).\n" +
    "- NEVER revert using git — other agents share this branch (and you have no shell).\n" +
    "- Put new or updated struct/class/enum type definitions into the corresponding `.hpp` header file, not the `.cpp` source. If a type is only used by this TU, put it in the unit's own header; if it's shared, use the appropriate shared header.\n" +
    "- Reminder: add comments to complicated code.\n" +
    "\n## Tool call budget\n" +
    "- Write code FIRST, then hexdiff to verify. Search only if stuck.\n" +
    "- Maximum 5 grep/search commands per function — if you haven't found it after 3 searches, write your best guess and hexdiff it.\n" +
    "- Cap `kb` calls too (~3 per function): one symbol query + one mismatch-terms query per function is the expected pattern.\n" +
    "- Do NOT search for the same symbol or pattern more than once.\n" +
    "- Do NOT use `nm`, `objdump`, or `readelf` — use the `symbols` tool and the brief's provided file/symbol info instead. (`find`/`ls` ARE available and cheap for locating files; cap them like grep.)\n" +
    "- Prefer writing code based on the assembly provided over searching for existing implementations.\n" +
    "\n## Anti-patterns (will cause lint rejection)\n\n" +
    "- NEVER use `extern \"C\"` on new function definitions — the symbol map handles linking. `extern \"C\"` is a crutch; use proper C++ declarations from the appropriate header. Only `lbl_*` reloc/data names may stay `extern \"C\"`.\n" +
    "- NEVER write constructors/destructors as C-style free functions taking `* self` (e.g. `extern \"C\" CFoo* __ct__4CFooFv(CFoo* self)`). The retail symbols are mangled members — write real member ctors/dtors: `CFoo::CFoo(...)` / `CFoo::~CFoo(...)`.\n" +
    "- Remove existing `extern \"C\"` stubs whenever possible — replace with proper C++ declarations from the appropriate header.\n" +
    "- NEVER use `void*` — use a proper struct/class pointer, or `u8*` for opaque buffers.\n" +
    "- NEVER write `*(u32*)(ptr + 0xNN)` or similar cast+offset arithmetic — define a struct with `field_0xNN` members instead.\n" +
    "- NEVER add new `#pragma` directives (except `#pragma once` in headers) — the build system handles compiler options.\n" +
    "- NEVER use binary-patching escapes (`insn_patches`, `insert_insns`, `reloc_offset_moves`, `postprocess_reloc_names.py`) — chase EQUIVALENT_MATCH, not byte-identity patches. (`bake_linker_addrs`/`force_symbol_relocs` for DOL-split absolutes are the only allowed exceptions.)\n" +
    "- NEVER write characters with no Shift-JIS encoding (e.g. em-dash `—`, en-dash `–`) — the build rejects them. Use ASCII (`-`) or genuine Japanese that has an SJIS encoding.\n"
  );
}

/** TU-finalisation prompt: runs once every function in the unit matches. */
export function buildTuFinalPrompt(opts: {
  unit: string;
  sourceFiles: string[];
  pythonBin: string;
  region: string;
}): string {
  const { unit, sourceFiles, pythonBin, region } = opts;

  return `## TU Finalisation: ${unit}

Every function in unit \`${unit}\` is now FULL_MATCH or EQUIVALENT_MATCH.
Your job is to finalise the translation unit. The \`xenoblade-decomp\` skill
is already loaded — follow it (high-level C++ only, no asm/register tricks).

Source files in scope:
${sourceFiles.map((f) => `- \`${f}\``).join("\n")}

### 0. NEVER revert using git

Other agents share this branch. Do NOT run \`git checkout\`, \`git reset\`,
\`git restore\`, or any command that would revert or discard changes. If you
need to undo your own edits, use the editor to make forward fixes.
(This session's bash is constrained to the TU-final allowlist — git,
\`cycle\`/\`batch-cycle\`, \`--smt\`/\`--linked\`, and plain \`run.py diff\` are
blocked at the tool level. The \`hexdiff\`, \`kb\`, \`symbols\`, \`targets\`,
\`ctx\`, and \`unit-status\` tools are available as in batch sessions.)

If the harness rejects this session's polish (lint violations or a build
break), it reverts to the pre-session snapshot and re-runs with the specific
violations fed back — fix ONLY those, do not redo unrelated work.

### 1. Verify the full TU matches — including data

\`\`\`bash
${pythonBin} tools/coop/run.py diff ${unit} --no-smt
${pythonBin} tools/coop/run.py size ${unit}
\`\`\`

Check every symbol, including data sections. The decompiled object's
\`.text\` must fit the retail split budget. (\`--no-smt\`: this repo never
runs the SMT probe — the register-renaming witness is the only equivalence
path, and it runs inside \`cycle\` when needed.)

### 2. Create or fix classes and structs

Make the code proper high-level C++: real classes/structs with correct
field types and widths, not offset arithmetic. Keep compiled bytes identical.

### 3. Clean up the code (byte-identical polish)

Naming, comments, structure. The compiled output must not change. Also:
**\`extern "C"\` stubs are compile-only scaffolding — try your best to get
rid of every \`extern "C"\`** by using proper C++ declarations from headers.
Constructors/destructors must be real member functions (\`Class::Class\` /
\`Class::~Class\`), never C-style free functions taking \`* self\` — the
retail symbols are mangled members (\`__ct__\`/\`__dt__\`).

### 4. Rename to human-readable names — only when confident

Class/struct fields, parameters, local variables, and function names. For
type renames use the symbol-recovery tooling, dry-run first, and prefer
same-length names:

\`\`\`bash
${pythonBin} tools/coop/run.py symbols rename-all <OldName> <NewName> --dry-run
\`\`\`

Unknown struct fields keep \`field_0xNN\` names — a wrong guess is worse
than no name.

### 5. Add comments to complex code or functions

### 6. Promote to Matching and rebuild

Flip the unit's object in \`configure.py\` from \`NonMatching\` to
\`Matching\`, then rebuild and re-verify. ALWAYS wrap configure/build
commands in \`build_lock.py\` — it holds the repo-wide build lock and
serialises you against other agents' hexdiff builds:

\`\`\`bash
${pythonBin} tools/pi_harness/build_lock.py ${region} -- ${pythonBin} configure.py
${pythonBin} tools/pi_harness/build_lock.py ${region} -- ninja
${pythonBin} tools/pi_harness/build_lock.py ${region} -- ${pythonBin} tools/coop/run.py size ${unit}
\`\`\`

### Summary

End with a summary of: changes made, renames applied, verification results
(diff/size output), and remaining risks.
`;
}

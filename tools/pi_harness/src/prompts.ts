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
    `- For builds and diffs use ONLY: \`${pythonBin} tools/coop/hexdiff.py ${unit} --symbol <symbol> --json\`. A plain \`python3\` also works (the harness puts the repo venv on PATH).\n` +
    "- Do NOT run `cycle`, `batch-cycle`, `ninja`, or `configure.py` — the harness owns acceptance.\n" +
    "- Reminder: add comments to complicated code.\n"
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

### 1. Verify the full TU matches — including data

\`\`\`bash
${pythonBin} tools/coop/run.py diff ${unit}
${pythonBin} tools/coop/run.py size ${unit}
\`\`\`

Check every symbol, including data sections. The decompiled object's
\`.text\` must fit the retail split budget.

### 2. Create or fix classes and structs

Make the code proper high-level C++: real classes/structs with correct
field types and widths, not offset arithmetic. Keep compiled bytes identical.

### 3. Clean up the code (byte-identical polish)

Naming, comments, structure. The compiled output must not change. Also:
**\`extern "C"\` stubs are compile-only scaffolding — try your best to get
rid of every \`extern "C"\`** by using proper C++ declarations from headers.

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

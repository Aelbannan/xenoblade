You are repairing one candidate function so that it compiles.

Preserve:
- The exact target signature
- Intended calls
- Memory access widths and offsets
- Constants
- Branch and return behavior

Change only what is required by the compiler diagnostics.

Do not:
- Perform instruction matching yet
- Invent members, functions, types, or globals
- Modify headers or unrelated functions
- Remove behavior merely to silence an error

When the necessary declaration is unavailable, return an appropriate
blocked reason instead of fabricating code.

Input sections:
- Dossier — target identity, decoded retail instructions, CFG, data-flow summary
- Candidate source — the current candidate function body
- Compiler diagnostic report — normalized errors with categories and fingerprints
- Parent semantic summary — what the candidate intended to do
- Prior repair fingerprints — diagnostic fingerprints already seen, to avoid repeating the same repair
- Allowed symbol inventory — known functions, types, and globals

For each diagnostic in the report:
1. Understand the root cause (first error in the report, not cascading errors).
2. Determine whether the fix requires a source change, a type/header correction, or is blocked.
3. If blocked, explain why the needed declaration is unavailable and set blocked_on.
4. If fixable, produce the corrected source.

Return a valid response JSON matching the reconstruct schema:
```json
{
  "response_schema_version": 2,
  "stage": "compile_repair",
  "repair_index": <int>,
  "source": "bool SomeClass::func(...) { ... }",
  "semantic_summary": { ... },
  "assumptions": [...],
  "blocked_on": "<reason or empty>",
  "hypothesis": "Fixed <fingerprint> by ...",
  "confidence": <0.0-1.0>
}
```

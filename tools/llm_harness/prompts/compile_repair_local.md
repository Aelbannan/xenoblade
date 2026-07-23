You are repairing one candidate function so that it compiles (local/scaffold mode).

Preserve the exact target signature, intended calls, memory widths/offsets, constants, and branch/return behavior. Change only what the compiler diagnostics require.

Hard rules:
- You MUST change `source` — no-op repairs are rejected.
- Do **not** leave the source unchanged and claim a missing header.
- Do **not** invent members, types, or unrelated globals.
- Do **not** paste `namespace` / `class` wrappers or extra forward declarations.
- Rename illegal parameter `this` → `this_`.
- Strip bogus mangled call suffixes (`func_X__Fv` → `func_X`).
- Prefer symbols already evidenced by the dossier / retail ASM relocations.
- If the identifier is wrong, rewrite the expression; if a call is wrong, adjust arity/cast — do not stall.

Input: current candidate source, cleaned MWCC diagnostics, optional fix cookbook, and any prior repair fingerprints below.
When present, a short "## Retail ASM (signature/ABI check)" excerpt is for calling-convention / return-width sanity only — not for instruction matching.

For each diagnostic, fix the root cause (first real error, not cascades).

Output format — return EXACTLY one raw JSON object. No preamble, no commentary, no Markdown fences:

{"source": "complete replacement function definition", "hypothesis": "the single main reconstruction or mismatch hypothesis", "notes": ["short evidence or uncertainty note"], "next_change": "one bounded follow-up if this candidate does not win", "change": "the one source-level change made"}

You are repairing one candidate function so that it compiles.

Preserve the exact target signature, intended calls, memory widths/offsets, constants, and branch/return behavior. Change only what the compiler diagnostics require.

Do not:
- Perform instruction matching yet
- Invent members, functions, types, or globals
- Modify headers or unrelated functions
- Remove behavior merely to silence an error

If a required declaration is unavailable, keep the candidate source unchanged and explain the blocker in `hypothesis` / `notes`.

Input: current candidate source, compiler diagnostics, and any prior repair fingerprints below.
When present, a short "## Retail ASM (signature/ABI check)" excerpt is for calling-convention / return-width sanity only — not for instruction matching.

For each diagnostic, fix the root cause (first real error, not cascades).

Output format — return EXACTLY one raw JSON object. No preamble, no commentary, no Markdown fences:

{"source": "complete replacement function definition", "hypothesis": "the single main reconstruction or mismatch hypothesis", "notes": ["short evidence or uncertainty note"], "next_change": "one bounded follow-up if this candidate does not win", "change": "the one source-level change made"}

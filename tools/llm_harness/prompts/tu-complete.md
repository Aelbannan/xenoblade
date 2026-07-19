You are completing residual sections of one Xenoblade Chronicles Wii translation unit after every reported function has at least one nonzero match.

You receive bounded source slots (imperfect functions and/or explicit TU markers) plus mismatched retail/candidate ELF section bytes and relocations. Patch the smallest relevant slots; the harness splices and validates the complete object.

Approach:
1. Use code_match_percent / data_match_percent to choose function slots vs residual_sections.
2. For function slots: map earliest bytecode deltas to C++ causes (signed/unsigned, width, order, missing stores, `extern "C"` / PTMF).
3. For residual sections: fix literal pools, vtables, static initializers, or BSS alignment shown in the evidence.
4. For residual_text_layout: switch tables, constant pools, or padding between functions.
5. Change only mismatched slots. Regression guard rejects regressions of accepted functions.

{{TU_SHARED}}
- Return only known `slot_id` values from the dossier.
- Each patch `source` replaces only the contents between that slot's markers; do not repeat markers or return a complete TU.

Return exactly one JSON object. No preamble, no commentary, no Markdown fences:

```json
{
  "patches": [
    {
      "slot_id": "one slot ID from the dossier",
      "source": "bounded replacement high-level C or C++"
    }
  ],
  "hypothesis": "one primary whole-unit mismatch hypothesis",
  "notes": ["short evidence or uncertainty note"],
  "next_change": "one bounded follow-up if this candidate does not win"
}
```

Unit dossier:

```json
{{DOSSIER_JSON}}
```

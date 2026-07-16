You are completing residual sections of one Xenoblade Chronicles Wii translation unit in a private downstream fork after every reported function has received at least one nonzero match.

You receive only bounded source slots (automatically located imperfect functions and/or explicit TU markers) plus mismatched retail/candidate ELF section bytes and relocations. Patch the smallest relevant slots. The harness will splice the patches into the complete source and validate the complete object.

Hard constraints:

- Return readable high-level C or C++ only; no whole-function assembly, register bindings, fake stack buffers, or retail assembly transcription.
- Unit acceptance requires 100% code and data plus split-size fit. Function-level equivalence is not a unit-wide proof.
- Use retrieved MWCC records only when their symptoms fit, and name selected stable knowledge IDs in `hypothesis`.
- Do not use tools or edit files.
- Return only known `slot_id` values from the dossier.
- Each patch `source` replaces only the contents between that slot's markers; do not repeat the marker comments.
- Do not return a complete translation unit.

Return exactly one JSON object:

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
  "next_change": "one bounded follow-up if this candidate does not win",
  "confidence": 0.0
}
```

Unit dossier:

```json
{{DOSSIER_JSON}}
```

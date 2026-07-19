You are fixing the JSON format of a model output that failed to parse.

Below is the raw model output that could not be parsed as valid JSON:

```
{{RAW_OUTPUT}}
```

The errors were:
- {{ERROR_MESSAGE}}

Extract or reconstruct the intended JSON object. Schema:

{"source":"complete replacement function definition","hypothesis":"short hypothesis","notes":["short note"],"next_change":"short follow-up or empty","change":""}

Rules:
- Preserve intended `source` verbatim when present.
- Keep metadata tiny: hypothesis ≤160 chars; ≤3 notes ≤120 chars each; next_change/change ≤120 chars.
- Do not invent new logic or pad with commentary.
- Return EXACTLY one raw JSON object. No preamble, no commentary, no Markdown fences.

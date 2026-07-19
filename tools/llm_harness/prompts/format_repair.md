You are fixing the JSON format of a model output that failed to parse.

Below is the raw model output that could not be parsed as valid JSON:

```
{{RAW_OUTPUT}}
```

The errors were:
- {{ERROR_MESSAGE}}

Your task is to extract or reconstruct the intended JSON object from this output. The JSON must have exactly this schema:
{
  "source": "complete replacement function definition",
  "hypothesis": "the single main reconstruction or mismatch hypothesis",
  "notes": ["short evidence or uncertainty note"],
  "next_change": "one bounded follow-up if this candidate does not win"
}

Rules:
- Preserve all intended source code, hypothesis, notes, and next_change from the original output.
- Do not invent new source code or change the intended logic.
- If the original output had a "source" field, keep it verbatim.
- If the original output was truncated, reconstruct the JSON structure around the available content.
- Return EXACTLY one raw JSON object. No preamble, no commentary, no Markdown fences, no text before or after.

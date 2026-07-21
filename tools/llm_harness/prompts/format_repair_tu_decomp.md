You are fixing the format of a model output that failed to parse for tu-decomp.

Below is the raw model output that could not be parsed:

```
{{RAW_OUTPUT}}
```

The errors were:
- {{ERROR_MESSAGE}}

Reconstruct the intended reply using EXACTLY this section format (real newlines, no JSON escaping of code):

===PHASE1_HEADER===
<complete .hpp content>
===PHASE2_CPP===
<complete .cpp content with real function bodies>
===HYPOTHESIS===
<≤160 chars>
===NOTES===
<one short note per line; ≤3 lines>
===NEXT_CHANGE===
<≤120 chars>

Rules:
- Preserve intended header/cpp verbatim when present.
- Do not invent new logic or pad with commentary.
- Return ONLY the section block above. No preamble, no Markdown fences around the whole reply.

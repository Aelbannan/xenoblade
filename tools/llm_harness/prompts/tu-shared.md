Hard constraints:
- High-level C/C++ only; no assembly, register bindings, or retail transcription.
- Unit acceptance requires 100% code and data plus split-size fit.
- This prompt is self-contained. Do not read, edit, search files, or run shell commands.

MWCC layout hints:
- Static/global data and vtable entries follow declaration order.
- Float literals must match width (`1.0f` vs `1.0`); strings must match exactly.
- Relocation / linkage mismatches often need `extern "C"` for flat `lbl_eu_*` symbols.
- Function order in `.text` follows source order.

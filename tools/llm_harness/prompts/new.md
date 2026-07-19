Produce a conservative one-shot decompilation for the named function (from scratch or a placeholder slot).

1. Recover the signature from the demangled name / type snippets.
2. Walk the retail ASM listing (`retail_asm`): calls, global loads/stores, branches, immediates. Ignore prologue shape.
3. Prefer typed members and named locals over pointer arithmetic / register temporaries.
4. Flat `lbl_eu_*` data accessed via ADDR16_HI/HA/LO needs `extern "C"` (avoid C++ mangling).
5. PTMF: three consecutive words from `lbl_eu_*` into a member is a descriptor copy — use `extern "C"` data + word copy/`memcpy`, not `&Class::method` (that creates a TU-local pool).
6. When uncertain, prefer the simpler bytecode-faithful reading; refine later via `improve`.

Return only the complete target function definition. Do not invent unrelated declarations.

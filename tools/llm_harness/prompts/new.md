Produce a conservative one-shot decompilation for the named function (from scratch or a placeholder slot).

1. Walk `retail_asm` first: calls, global loads/stores, branches, immediates, and ABI clues (`blr` after `li`/`lwz`/`lfs` → non-void; `stfs`/`stw` via `r3`+offset without setup → likely member/`this`). Ignore prologue shape.
2. Recover the signature from demangled name / type snippets only when they do not contradict that ASM. Placeholder stubs like `void func_…() {}` or dossier `void`/no-params are not authoritative — write the ASM-faithful signature in `source`.
3. Prefer typed members and named locals over pointer arithmetic / register temporaries.
4. Flat `lbl_eu_*` data accessed via ADDR16_HI/HA/LO needs `extern "C"` (avoid C++ mangling).
5. PTMF: three consecutive words from `lbl_eu_*` into a member is a descriptor copy — use `extern "C"` data + word copy/`memcpy`, not `&Class::method` (that creates a TU-local pool).
6. When uncertain, prefer the simpler bytecode-faithful reading; refine later via `improve`.

Return only the complete target function definition. Do not invent unrelated declarations.

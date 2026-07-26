// TU: monolib/src/effect/code_804C8684
//
// These functions exist in the retail binary but are no-ops
// (each compiles to a single blr instruction).

extern "C" void func_804C8684(void) {}
extern "C" void func_804C8688(void) {}
extern "C" void func_804C868C(void) {}

extern "C" void func_804C8690() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_804C86C0() {}

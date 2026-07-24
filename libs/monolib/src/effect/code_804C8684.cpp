// TU: monolib/src/effect/code_804C8684
//
// These functions exist in the retail binary but are no-ops
// (each compiles to a single blr instruction).

extern "C" void func_804C8684(void) {}
extern "C" void func_804C8688(void) {}
extern "C" void func_804C868C(void) {}

// LLM-HARNESS-BEGIN: us-804cc7ec
extern "C" void func_804C8690() {}
// LLM-HARNESS-END: us-804cc7ec

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// LLM-HARNESS-BEGIN: us-804cc81c
extern "C" void sinit_804C86C0() {}
// LLM-HARNESS-END: us-804cc81c

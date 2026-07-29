// Auto-scaffolded catalog TU for monolib/src/scn/code_804BC9EC
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "code_804BC9EC.hpp"

// Forward declaration — not yet declared in the shared header
void func_804B7E0C(ScnResData* self);

extern u32 lbl_eu_80665968;
int func_804BC9EC__Fv(void* self) { return (int)(intptr_t)&lbl_eu_80665968; }

void func_804BC9F4__FPvUl(){}

void func_804BCC10(){}

void func_804BCC1C__FPv(){}

void func_804BCC30(){}

void func_804BCC3C(){}

// Tail-calls func_804B7E0C with the global ScnResData, discarding caller's first arg
void func_804BCC48(ScnResData*) {
    func_804B7E0C(&lbl_eu_8065F32C);
}

// Tail-calls func_804B80A4 with the global ScnResData, forwarding the index arg
ScnResData* func_804BCC54(ScnResData*, int index) {
    return func_804B80A4(&lbl_eu_8065F32C, index);
}

// Tail-calls func_804B8078 with the global ScnResData, forwarding the index arg
void func_804BCC60(ScnResData*, int index) {
    func_804B8078(&lbl_eu_8065F32C, index);
}

void func_804BCC6C(){}

void func_804BCC78(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BD8A0(){}

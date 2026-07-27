// Auto-scaffolded catalog TU for monolib/src/scn/CScnBlend
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void __ct__CScnBlend(){}

void CScnBlend::~CScnBlend() {}

struct CScnBlendState {
    u8 _00[0x54];
    u8 flags;
};

void func_80498D98(void* r3, int r4) {
    CScnBlendState* state = (CScnBlendState*)r3;
    if (r4 != 0) {
        state->flags |= 1;
    } else {
        state->flags &= 0xFE;
    }
}

void func_80498DC0(){}

void func_80498DE8(){}

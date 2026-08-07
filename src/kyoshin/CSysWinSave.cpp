// Auto-scaffolded catalog TU for kyoshin/CSysWinSave
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// forward declarations for scaffold thunk references
void __dt__11CSysWinSaveFv(void*);
void cbRenderBefore__11CSysWinSaveFv(void*);


void func_8029480C(void* self) { ((void(*)(void*))__dt__11CSysWinSaveFv)((char*)self - 0x6c); }


void func_80294814(void* self) { ((void(*)(void*))cbRenderBefore__11CSysWinSaveFv)((char*)self - 0x70); }

void func_8029481C(void* self) { ((void(*)(void*))__dt__11CSysWinSaveFv)((char*)self - 0x70); }

extern "C" void func_80294824__FPv(void* self) {
    extern float lbl_eu_80668BA0;
    float v = lbl_eu_80668BA0;
    *(float*)self = v;
    *(float*)((u8*)self + 4) = v;
}

extern "C" void func_80294834__FPv(void* self) {
    extern float lbl_eu_80668BA0;
    float v = lbl_eu_80668BA0;
    *(float*)self = v;
    *(float*)((u8*)self + 4) = v;
}

void func_80294844(){}

extern "C" unsigned long func_80294624() {
    extern unsigned long lbl_eu_80664A08;
    return lbl_eu_80664A08 != 0;
}

extern "C" void func_80294638() {}

extern "C" void Init__11CSysWinSaveFv() {}
extern "C" void Term__11CSysWinSaveFv() {}
extern "C" void Move__11CSysWinSaveFv() {}

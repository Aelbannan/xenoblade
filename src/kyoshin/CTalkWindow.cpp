// Auto-scaffolded catalog TU for kyoshin/CTalkWindow
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
void __dt__11CTalkWindowFv(void* self);
void cbRenderBefore__11CTalkWindowFv(void* self);
extern "C" void Draw__11CTalkWindowFv() {}

void func_8012BDD0(void){}

void func_8012D3D8(){}

void func_8012D8C0(){}

void func_8012DA6C(){}

void func_8012DF78(void* self) { ((void(*)(void*))__dt__11CTalkWindowFv)((char*)self - 0x6c); }

void func_8012DF80(void* self) { ((void(*)(void*))cbRenderBefore__11CTalkWindowFv)((char*)self - 0x70); }

extern "C" void func_8012DF88(void* self) { ((void(*)(void*))__dt__11CTalkWindowFv)((char*)self - 0x70); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8012DE98(){}

extern u32 lbl_eu_80664044;
extern "C" bool func_8012CD24() {
    u32 v = lbl_eu_80664044;
    return ((-v) & ~v) >> 31;
}

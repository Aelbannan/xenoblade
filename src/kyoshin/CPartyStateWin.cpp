// Auto-scaffolded catalog TU for kyoshin/CPartyStateWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u32 func_801F9694(void) {
    extern u32 lbl_eu_80663E10;
    return lbl_eu_80663E10;
}



extern "C" u32 func_801F9684(void* self) { return *(u32*)((u8*)self + 0x6C); }

extern "C" u32 func_801F968C(void* self) { return *(u32*)((u8*)self + 0x70); }








void func_801F9894(){}

void func_801F9914(){}

void func_801F9998(){}

void func_801F9A48(){}

void func_801F9B18(){}

void func_801F9CB4(){}

extern "C" void func_801FA220(void* r3, const void* r4) {
    unsigned int* destWords = (unsigned int*)((char*)r3 + 4);
    const unsigned int* srcWords = (const unsigned int*)((char*)r4 + 4);
    destWords[0] = srcWords[0];
    destWords[1] = srcWords[1];
    destWords[2] = srcWords[2];
    destWords[3] = srcWords[3];
    char* destBytes = (char*)r3;
    const char* srcBytes = (const char*)r4;
    destBytes[0x14] = srcBytes[0x14];
    destBytes[0x15] = srcBytes[0x15];
}

void func_801FA254(){}

void func_801FA338(){}

extern "C" u8 func_801FA4EC(void* self) { return ((u8*)self)[0x6BE5]; }

void func_801FA4F4(){}

void func_801FA524(){}

void func_801FA59C(){}

void func_801FA614(){}

void func_801FA674(){}

void func_801FA8AC(){}

void func_801FA92C(){}

void func_801FAA10(){}

void func_801FAA60(){}

void func_801FB560(){}

void func_801FB60C(){}

void func_801FB66C(){}

void func_801FB6CC(){}

void func_801FB72C(){}

void func_801FB834(){}

void func_801FB8B0(){}

void func_801FB900(){}

void func_801FBBE0(){}

void func_801FBC30(){}

void func_801FBC7C(){}

void CPartyStateWin::cbRenderBefore() {}

extern "C" void func_801FBDB8(void* self) { ((void(*)(void*))cbRenderBefore__14CPartyStateWinFv)((char*)self - 0x4); }

void __dt__14CPartyStateWinFv(void* self);
void func_801FBDC0(void* self) { ((void(*)(void*))__dt__14CPartyStateWinFv)((char*)self - 0x4); }

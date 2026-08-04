// Auto-scaffolded catalog TU for monolib/src/scn/CScnRootNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void* func_8048FDDC(void* self) { return (void*)((u8*)self + 0x44c); }










extern "C" void* func_8048FDE4(void* self) { return (void*)((u8*)self + 0x45c); }


extern "C" void* func_8048FDEC(void* self) {
    if (*(u32*)((u8*)self + 0x4C4) == 0xFFFFFFFF) return (u8*)self + 0x44C;
    return (u8*)self + 0x46C;
}

extern "C" void* func_8048FE0C(void* self) {
    if (*(u32*)((u8*)self + 0x4D0) == 0xFFFFFFFF) return (u8*)self + 0x44C;
    return (u8*)self + 0x47C;
}

extern "C" void* func_8048FE2C(void* self) { return (void*)((u8*)self + 0x49c); }

void func_8048FE34(){}

void func_8048FEC4(){}

void func_8048FED8(){}

void func_8048FF90(){}

void func_8048FFBC(){}

extern "C" u32 func_80490038(void* self) { return *(u32*)((u8*)self + 0x14); }

void func_80490040(){}

extern "C" void func_80490088(void* self) { ((void(*)(void*))func_8048FED8)((char*)self - 0xc); }

extern "C" void __dt__12CScnRootNw4rFv(void* self, int deleteFlag);
void func_80490090(void* self) { ((void(*)(void*))__dt__12CScnRootNw4rFv)((char*)self - 0xc); }

extern "C" u32 func_80490098__Fv(void) {
    extern u32 lbl_eu_806639A8;
    return lbl_eu_806639A8;
}

extern "C" void func_804900A0__FUl(u32 param) {
    extern u32 lbl_eu_806658FC;
    if (param) {
        lbl_eu_806658FC += 1;
    } else {
        lbl_eu_806658FC -= 1;
    }
}

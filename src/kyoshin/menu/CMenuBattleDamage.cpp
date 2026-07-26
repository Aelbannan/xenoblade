// Auto-scaffolded catalog TU for kyoshin/menu/CMenuBattleDamage
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CMenuBattleDamage() {}

extern "C" void __dt__17CMenuBattleDamageFv(void* self) {}

extern "C" void Init__17CMenuBattleDamageFv() {}

extern "C" void Term__17CMenuBattleDamageFv() {}

extern "C" void Move__17CMenuBattleDamageFv() {}

extern "C" void cbRenderBefore__17CMenuBattleDamageFv(void* self) {}

extern "C" void func_801096B8() {}

extern "C" void func_80109734() {}

extern "C" void func_8010975C(unsigned char val) {
    extern unsigned long lbl_eu_80663F28;
    unsigned char* p = (unsigned char*)lbl_eu_80663F28;
    if (p != 0) p[0x774] = val;
}

extern "C" void func_80109770(unsigned char val) {
    extern unsigned long lbl_eu_80663F28;
    unsigned char* p = (unsigned char*)lbl_eu_80663F28;
    if (p != 0) p[0x775] = val;
}

extern "C" void func_80109784() {}

extern "C" void func_80109874() {}

extern "C" void func_80109888() {}

extern "C" void func_8010989C() {}

extern "C" void func_801098B0() {}

extern "C" void func_8010A67C(void* self) { ((void(*)(void*))__dt__17CMenuBattleDamageFv)((char*)self - 0x58); }

extern "C" void func_8010A684(void* self) { ((void(*)(void*))cbRenderBefore__17CMenuBattleDamageFv)((char*)self - 0x5c); }

extern "C" void func_8010A68C(void* self) { ((void(*)(void*))__dt__17CMenuBattleDamageFv)((char*)self - 0x5c); }

extern "C" void __dt__17CPcSelectCursor01Fv() {}

extern "C" void func_8010A6F0(char* this_) {
    if (*(int*)(this_ + 0x2c) != 0) {
        return;
    }
    *(int*)(this_ + 0x2c) = 1;
    this_[0x28] = 0;
}

extern "C" void func_8010A710() {}

extern "C" void func_8010A7A8() {}

extern "C" u8 func_8010A840(void* self) { return ((u8*)self)[0x28]; }

extern "C" void func_8010A848() {}

extern "C" void func_8010A8E4() {}

extern "C" void func_8010A940() {}

extern "C" void func_8010ACC4() {}

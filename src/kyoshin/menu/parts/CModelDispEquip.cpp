// Auto-scaffolded catalog TU for kyoshin/menu/parts/CModelDispEquip
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__CModelDispEquip(){}

extern "C" void func_801FF6DC(void* self) {
    *(unsigned long*)self = 0;
    *(unsigned long*)((char*)self + 4) = 0;
    ((unsigned char*)self)[8] = 0;
}

void __dt__801FF6F0(){}

void CModelDispEquip::~CModelDispEquip() {}

void func_801FF7B0(){}

void func_801FF82C(){}

void func_801FF874(){}

extern "C" u8 func_801FF95C(void* self) { return ((u8*)self)[0x1020]; }

extern "C" u8 func_801FF964(void* self) { return ((u8*)self)[0x1021]; }

extern "C" void func_801FF96C(unsigned char* p) {
    if (p[0x1014] != 0) {
        return;
    }
    p[0x1014] = 1;
    p[0x1020] = 0;
}

extern "C" void func_801FF98C(void* self) {
    ((unsigned char*)self)[0x1014] = 3;
    ((unsigned char*)self)[0x1020] = 0;
}

void func_801FF9A0(){}

void func_801FF9AC(){}

extern "C" void func_801FFAB4(float* dest, float a, float b, float c, float d) {
    dest[0] = a;
    dest[1] = b;
    dest[2] = c;
    dest[3] = d;
}

void func_801FFAC8(){}

void func_801FFADC(){}

void func_801FFBC4(){}

void func_801FFDB0(){}

void func_801FFDC4(){}

void func_80200388(){}

void func_80200394(){}

void func_80200C20(){}

void func_80200CE8(){}

void CModelDispEquip::OnFileEvent() {}

void func_80200E94(){}

void func_80200F08(){}

void func_80200F9C(void* self){}

void func_80200FB0(){}

void func_80201148(){}

void func_80201298(void* self){}

extern "C" void func_80201318(void) {}

void func_8020131C(void* self){}

extern "C" void func_80201440(void) {}

void func_80201444(void* self){}

void func_8020147C(){}

void func_802014C0(){}

void func_8020151C(){}

void func_80201570(){}

void func_802015D4(){}

void func_80201638(){}

void func_802016BC(){}

void func_80201740(){}

void func_802017A4(){}

void func_80201808(){}

void func_80201900(){}

extern "C" void func_80201A44(void* self) { ((void(*)(void*))func_80201298)((char*)self - 0x4); }

extern "C" void func_80201A4C(void* self) { ((void(*)(void*))func_80201440)((char*)self - 0x4); }

extern "C" void func_80201A54(void* self) { ((void(*)(void*))func_8020131C)((char*)self - 0x4); }

extern "C" void func_80201A5C(void* self) { ((void(*)(void*))func_80201318)((char*)self - 0x4); }

extern "C" void func_80201A64(void* self) { ((void(*)(void*))func_80200F9C)((char*)self - 0x4); }

extern "C" void func_80201A6C(void* self) { ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)self - 0x4); }

extern "C" void func_80201A74(void* self) { ((void(*)(void*))func_80201444)((char*)self - 0x8); }

extern "C" void func_80201A7C(void* self) { ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)self - 0x8); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802019F8(){}

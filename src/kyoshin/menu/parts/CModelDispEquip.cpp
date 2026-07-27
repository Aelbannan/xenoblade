// Auto-scaffolded catalog TU for kyoshin/menu/parts/CModelDispEquip
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/parts/CModelDispEquip.hpp"

// Forward declarations for symbols referenced by thunks
extern "C" void __dt__15CModelDispEquipFv();

void __ct__CModelDispEquip(){}

void CModelDispEquip_resetBase(CModelDispEquip* self) {
    *(unsigned long*)self = 0;
    *(unsigned long*)((char*)self + 4) = 0;
    ((unsigned char*)self)[8] = 0;
}

void __dt__801FF6F0(){}

CModelDispEquip::~CModelDispEquip() {}

void func_801FF7B0(){}

void func_801FF82C(){}

void func_801FF874(){}

u8 CModelDispEquip_getState20(CModelDispEquip* self) { return self->field_1020; }

u8 CModelDispEquip_getState21(CModelDispEquip* self) { return self->field_1021; }

void CModelDispEquip_setState14(unsigned char* p) {
    if (p[0x1014] != 0) {
        return;
    }
    p[0x1014] = 1;
    p[0x1020] = 0;
}

void CModelDispEquip_setState14_3(CModelDispEquip* self) {
    ((unsigned char*)self)[0x1014] = 3;
    ((unsigned char*)self)[0x1020] = 0;
}

void func_801FF9A0(){}

void func_801FF9AC(){}

void CModelDispEquip_storeFloats(float* dest, float a, float b, float c, float d) {
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

void CModelDispEquip_vfunc18(void) {}

void func_8020131C(void* self){}

void CModelDispEquip_vfunc40(void) {}

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

void CModelDispEquip_thunk4_1298(char* self) { ((void(*)(void*))func_80201298)(self - 0x4); }

void CModelDispEquip_thunk4_1440(char* self) { ((void(*)(void*))CModelDispEquip_vfunc40)(self - 0x4); }

void CModelDispEquip_thunk4_131C(char* self) { ((void(*)(void*))func_8020131C)(self - 0x4); }

void CModelDispEquip_thunk4_1318(char* self) { ((void(*)(void*))CModelDispEquip_vfunc18)(self - 0x4); }

void CModelDispEquip_thunk4_F9C(char* self) { ((void(*)(void*))func_80200F9C)(self - 0x4); }

void CModelDispEquip_thunk4_dtor(char* self) { ((void(*)(void*))__dt__15CModelDispEquipFv)(self - 0x4); }

void CModelDispEquip_thunk8_1444(char* self) { ((void(*)(void*))func_80201444)(self - 0x8); }

extern "C" void CModelDispEquip_thunk8_dtor(char* self) { ((void(*)(void*))__dt__15CModelDispEquipFv)(self - 0x8); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802019F8(){}

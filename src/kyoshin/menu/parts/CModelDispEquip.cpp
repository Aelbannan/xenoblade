// Auto-scaffolded catalog TU for kyoshin/menu/parts/CModelDispEquip
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/parts/CModelDispEquip.hpp"

// Forward declarations for symbols referenced by thunks
extern "C" void __dt__15CModelDispEquipFv();

void __ct__CModelDispEquip(){}

void CModelDispEquip::resetBase() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

void __dt__801FF6F0(){}

CModelDispEquip::~CModelDispEquip() {}

void func_801FF7B0(){}

void func_801FF82C(){}

void func_801FF874(){}

u8 CModelDispEquip::getState20() { return field_1020; }

u8 CModelDispEquip::getState21() { return field_1021; }

void CModelDispEquip::setState14() {
    if (field_1014 != 0) {
        return;
    }
    field_1014 = 1;
    field_1020 = 0;
}

void CModelDispEquip::setState14_3() {
    field_1014 = 3;
    field_1020 = 0;
}

void func_801FF9A0(){}

void func_801FF9AC(){}

void CModelDispEquip::storeFloats(float* dest, float a, float b, float c, float d) {
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

void CModelDispEquip::func_80200F9C(){}

void func_80200FB0(){}

void func_80201148(){}

void CModelDispEquip::func_80201298(){}

void CModelDispEquip::vfunc18() {}

void CModelDispEquip::func_8020131C(){}

void CModelDispEquip::vfunc40() {}

void CModelDispEquip::func_80201444(){}

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

void CModelDispEquip::thunk4_1298() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->func_80201298();
}

void CModelDispEquip::thunk4_1440() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->vfunc40();
}

void CModelDispEquip::thunk4_131C() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->func_8020131C();
}

void CModelDispEquip::thunk4_1318() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->vfunc18();
}

void CModelDispEquip::thunk4_F9C() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 4)->func_80200F9C();
}

void CModelDispEquip::thunk4_dtor() {
    ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)this - 4);
}

void CModelDispEquip::thunk8_1444() {
    reinterpret_cast<CModelDispEquip*>(reinterpret_cast<char*>(this) - 8)->func_80201444();
}

void CModelDispEquip::thunk8_dtor() {
    ((void(*)(void*))__dt__15CModelDispEquipFv)((char*)this - 8);
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802019F8(){}

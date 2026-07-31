// Auto-scaffolded catalog TU for kyoshin/menu/parts/CModelDispEquip
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/parts/CModelDispEquip.hpp"

// Forward declarations for symbols referenced by thunks
extern "C" void __dt__15CModelDispEquipFv();

void __ct__CModelDispEquip(void* self, u32 somePtr, s32 equipSlot) {
    CModelDispEquip* obj = (CModelDispEquip*)self;
    obj->somePtr = somePtr;
    obj->equipSlot = equipSlot;
    obj->state = 0;
    obj->alpha = 0.0f;
    obj->alphaTimer = 1.0f;
    obj->state20 = 1;
    obj->state21 = 1;
    obj->weaponId = 0;
    obj->timer = 0;
    obj->unk_55C = 0;
    obj->currentModelPtr = 0;
    obj->modelFileHandle = 0;
    obj->modelData = 0;
    for (int i = 0; i < 2; i++) {
        obj->animModelPtrs[i] = 0;
        obj->animPtrs[i] = 0;
    }
    for (int i = 0; i < 8; i++) {
        obj->equipPtrs[i] = 0;
    }
    for (int i = 0; i < 9; i++) {
        obj->fileSlots[i].handle = 0;
        obj->fileSlots[i].data = 0;
        obj->fileSlots[i].flag = 0;
    }
}

void CModelDispEquip::resetBase() {
    *(unsigned long*)this = 0;
    *(unsigned long*)((char*)this + 4) = 0;
    ((unsigned char*)this)[8] = 0;
}

void __dt__801FF6F0(void* self, int param) {
    if (self == nullptr) return;
    if (param <= 0) return;
}

CModelDispEquip::~CModelDispEquip() {
    if (this == nullptr) return;
}

void func_801FF7B0(){}

void func_801FF82C(){}

void func_801FF874(){}

u8 CModelDispEquip::getState20() { return state20; }

u8 CModelDispEquip::getState21() { return state21; }

void CModelDispEquip::setState14() {
    if (state != 0) return;
    state = 1;
    state20 = 0;
}

void CModelDispEquip::setState14_3() {
    state = 3;
    state20 = 0;
}

u32 func_801FF9A0(CModelDispEquip* self) { return (u32)(u8)self->equipSlot; }

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

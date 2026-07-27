// Auto-scaffolded catalog TU for kyoshin/CEquipChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipChange.hpp"

u8 func_802023C0(CEquipChange* self) { return self->field_4D; }








void func_802865A0(void* self);
void func_802023C8(CEquipChange* self) { func_802865A0(&self->mEquipItemBox); }



void func_80286650(void* self);
void func_8020247C(CEquipChange* self) { func_80286650(&self->mEquipItemBox); }


void func_802024CC(){}

void func_80202578(){}

void func_80202644(){}

void func_80202790(){}

void func_802028E4(){}

void func_80202A70(){}

void func_80202BFC(){}

void func_80202C4C(){}

void func_80202CCC(){}

void func_80202EB4(){}

int func_80203138(CEquipChange* self) {
    signed char value = self->field_98;
    if (value == 0)
        return 2;
    if (value == 4)
        return 4;
    if (value == 6)
        return 5;
    if (value == 8)
        return 6;
    if (value == 10)
        return 7;
    if (value == 12)
        return 8;
    return 3;
}

void func_802031A0(){}

void func_80203210(){}

void func_8020392C(){}

void func_801D2E4C(void* self);
void func_8020397C(CEquipChange* self) { func_801D2E4C(self->field_80); }

void func_80287FE0(void* self);
void func_80203984(CEquipChange* self) { func_80287FE0(&self->mEquipItemBox); }

void func_802886D8(void* self);
void func_8020398C(CEquipChange* self) { func_802886D8(&self->mEquipItemBox); }

void func_80203994(){}

void func_802039F4(){}

void func_80203A98(){}

void func_80203C9C(){}

void func_80203CE0(){}

void func_80203D78(){}

void func_80203E00(){}

void func_80203E98(){}

void func_80203EE4(){}

void func_80203F84(){}

void func_80203FCC(){}

void func_8020404C(){}

void func_802040A0(){}

void func_802040FC(){}

void func_802042C0(){}

void func_80205294(void* dst, void* src) {
    u16 a = *(u16*)src;
    u32 b = *(u32*)((u8*)src + 4);
    *(u16*)dst = a;
    *(u32*)((u8*)dst + 4) = b;
}

void func_802052A8(){}

bool CEquipChange::OnFileEvent(CEventFile* file) { return true; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_802059E8(){}

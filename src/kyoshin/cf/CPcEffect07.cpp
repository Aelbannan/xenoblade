// Auto-scaffolded catalog TU for kyoshin/cf/CPcEffect07
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CPcEffect07.hpp"

namespace cf {}
using namespace cf;
void __ct__cf_CPcEffect07(){}

cf::CPcEffect07::~CPcEffect07() {}

void func_801B19F0(){}

void func_801B1C5C(){}

PcEffectData* lbl_eu_80664398;

int getEffectMax() {
    if (lbl_eu_80664398 == 0) {
        return 0;
    }
    return lbl_eu_80664398->maxEffects;
}

void func_801B1CCC(){}

int findActiveEntryID(int index)
{
    if (lbl_eu_80664398 == 0)
        return -1;

    if (index < 0)
    {
        if (lbl_eu_80664398->entry2_active == 1) return lbl_eu_80664398->entry2_id;
        if (lbl_eu_80664398->entry1_active == 1) return lbl_eu_80664398->entry1_id;
        if (lbl_eu_80664398->entry0_active == 1) return lbl_eu_80664398->entry0_id;
        return -1;
    }

    if (index == 0) { if (lbl_eu_80664398->entry0_active == 1) return lbl_eu_80664398->entry0_id; }
    else if (index == 1) { if (lbl_eu_80664398->entry1_active == 1) return lbl_eu_80664398->entry1_id; }
    else if (index == 2) { if (lbl_eu_80664398->entry2_active == 1) return lbl_eu_80664398->entry2_id; }
    return -1;
}

void func_801B1DCC(){}

void func_801B1E74(){}

s32 getFirstFreeSlot() {
    PcEffectData* data = lbl_eu_80664398;
    if (!data) return -1;
    if (data->entry0_active == 0) return 0;
    if (data->entry1_active == 0) return 1;
    if (data->entry2_active == 0) return 2;
    return -1;
}

int isSlotActive(int index) {
    if (lbl_eu_80664398 == 0) return 0;
    s16 val;
    switch (index) {
    case 0: val = lbl_eu_80664398->entry0_active; break;
    case 1: val = lbl_eu_80664398->entry1_active; break;
    case 2: val = lbl_eu_80664398->entry2_active; break;
    default: return 0;
    }
    return (val == 1) ? 1 : 0;
}

bool hasAnyActiveSlot() {
    PcEffectData* data = lbl_eu_80664398;
    if (!data) return false;
    if (data->entry0_active != 0) return true;
    if (data->entry1_active != 0) return true;
    if (data->entry2_active != 0) return true;
    return false;
}

int countActiveSlots() {
    PcEffectData* data = lbl_eu_80664398;
    if (!data) return 0;
    int result = 0;
    if (data->entry0_active == 1) result = 1;
    if (data->entry1_active == 1) result++;
    if (data->entry2_active == 1) result++;
    return result;
}

void func_801B20C8(void* self){}

void func_801B218C(){}

void func_801B21E0(void* self){}

void func_801B2318(void* self){}

void func_801B23D0(){}

void func_801B248C(){}

void thunk_adj4_reset(void* self) { ((void(*)(void*))func_801B21E0)((char*)self - 0x4); }

void thunk_adj4_update(void* self) { ((void(*)(void*))func_801B2318)((char*)self - 0x4); }

extern "C" void func_801B2554(void* self) { ((void(*)(void*))func_801B20C8)((char*)self - 0x4); }

extern "C" void func_801B255C(void* self) { reinterpret_cast<cf::CPcEffect07*>((char*)self - 0x4)->~CPcEffect07(); }

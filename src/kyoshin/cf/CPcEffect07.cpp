// Auto-scaffolded catalog TU for kyoshin/cf/CPcEffect07
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

#include "kyoshin/cf/CPcEffect07.hpp"
#include "kyoshin/realtimeevt/CREvtEffect.hpp"

extern "C" void* __dt__Q22cf11CPcEffect07Fv(void*, int);

extern "C" { extern char lbl_eu_80533744[]; }  // vtable data (retail .data)
extern "C" void func_801B23D0(cf::CPcEffect07*);  // effect cleanup

namespace cf {}
using namespace cf;
void __ct__cf_CPcEffect07(){}

// Retail D1 dtor: write the two vtable slots (+0 / +4, retail stw pair from
// lbl_eu_80533744), run the cleanup, clear the singleton global when it
// points at this, then delete-on-flag; returns this. Written as a
// freestanding extern "C" function (the C++ dtor stays declared so the
// virtual dtor symbol and thunk calls resolve here at link).
extern "C" void* __dt__Q22cf11CPcEffect07Fv(void* self, int flag) {
    cf::CPcEffect07* this_ = reinterpret_cast<cf::CPcEffect07*>(self);
    if (this_ != 0) {
        *(u32*)((char*)this_ + 0) = (u32)lbl_eu_80533744;
        *(u32*)((char*)this_ + 4) = (u32)lbl_eu_80533744 + 0x10;
        func_801B23D0(this_);
        if ((void*)lbl_eu_80664398 == (void*)this_) {
            lbl_eu_80664398 = 0;
        }
        if (flag > 0) {
            ::operator delete(this_);
        }
    }
    return self;
}

void func_801B19F0(){}

// Current effect-bank byte: -1 when the global bank is absent or the lookup
// fails, else the bank byte at +0x58 (retail reloads it for the return).
extern "C" int func_801B19F0(u8, int);
extern "C" int func_801B1C5C(void) {
    if (lbl_eu_80664398 == 0)
        return -1;
    if (func_801B19F0(*(u8*)((u8*)lbl_eu_80664398 + 0x58), -1) < 0)
        return -1;
    return *(u8*)((u8*)lbl_eu_80664398 + 0x58);
}

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
        if (lbl_eu_80664398->entries[2].mActive == 1) return lbl_eu_80664398->entries[2].mId;
        if (lbl_eu_80664398->entries[1].mActive == 1) return lbl_eu_80664398->entries[1].mId;
        if (lbl_eu_80664398->entries[0].mActive == 1) return lbl_eu_80664398->entries[0].mId;
        return -1;
    }

    if (index == 0) { if (lbl_eu_80664398->entries[0].mActive == 1) return lbl_eu_80664398->entries[0].mId; }
    else if (index == 1) { if (lbl_eu_80664398->entries[1].mActive == 1) return lbl_eu_80664398->entries[1].mId; }
    else if (index == 2) { if (lbl_eu_80664398->entries[2].mActive == 1) return lbl_eu_80664398->entries[2].mId; }
    return -1;
}

// Counts how many slots match the given id (or, for index 0, how many slots
// are active - retail walks the pointer backwards with a countdown that never
// terminates by the counter; reproduced verbatim).
u32 func_801B1DCC(u32 index) {
    PcEffectData* data = lbl_eu_80664398;
    if (data == 0) return 0;

    int count = 0;
    if (index != 0) {
        if (data->entries[0].mActive == 1 && index == data->entries[0].mId) count = 1;
        if (data->entries[1].mActive == 1 && index == data->entries[1].mId) count++;
        if (data->entries[2].mActive == 1 && index == data->entries[2].mId) count++;
    } else {
        int i = 0;
        while (i < 3) {
            if (data->entries[0].mActive == 1) count++;
            data = (PcEffectData*)((u8*)data - 0x18);
            i--;
        }
    }
    return count;
}

void func_801B1E74(){}

s32 getFirstFreeSlot() {
    PcEffectData* data = lbl_eu_80664398;
    if (!data) return -1;
    if (data->entries[0].mActive == 0) return 0;
    if (data->entries[1].mActive == 0) return 1;
    if (data->entries[2].mActive == 0) return 2;
    return -1;
}

int isSlotActive(int index) {
    if (lbl_eu_80664398 == 0) return 0;
    s16 val;
    switch (index) {
    case 0: val = lbl_eu_80664398->entries[0].mActive; break;
    case 1: val = lbl_eu_80664398->entries[1].mActive; break;
    case 2: val = lbl_eu_80664398->entries[2].mActive; break;
    default: return 0;
    }
    return (val == 1) ? 1 : 0;
}

bool hasAnyActiveSlot() {
    PcEffectData* data = lbl_eu_80664398;
    if (!data) return false;
    if (data->entries[0].mActive != 0) return true;
    if (data->entries[1].mActive != 0) return true;
    if (data->entries[2].mActive != 0) return true;
    return false;
}

int countActiveSlots() {
    PcEffectData* data = lbl_eu_80664398;
    if (!data) return 0;
    int result = 0;
    if (data->entries[0].mActive == 1) result = 1;
    if (data->entries[1].mActive == 1) result++;
    if (data->entries[2].mActive == 1) result++;
    return result;
}

// Fetch the schedule holder for a slot: id 0 for the non-negative selector,
// or the id looked up in the wstring table for negative selectors. When a
// positive amount is given and a schedule is attached, drive its fixed
// timestep via func_804E3CDC.
PcEffectSchedHolder* func_801B20C8(CPcEffect07* self, u32 id, s32 sel, s32 amount) {
    PcEffectActorView* actor = (PcEffectActorView*)self->mField0C;
    PcEffectSchedHolder* result;
    if (sel >= 0) {
        result = actor->mSub.GetSlot(0);
    } else {
        result = actor->mSub.GetSlot(lbl_eu_805049F8[id - 0x700]);
    }
    if (amount > 0 && result != 0 && result->mSched != 0) {
        func_804E3CDC(result->mSched, (float)amount, lbl_eu_80667DF4);
    }
    return result;
}

// Clear the caller's +0xB0 slot, then scan the 3-entry table at the global
// bank's +0x10 (24-byte stride) for the entry pointing at r4 and wipe it.
extern "C" void func_801B218C(void* a1, void* r4) {
    if (lbl_eu_80664398 == 0)
        return;
    *(u32*)((u8*)r4 + 0xB0) = 0;
    PcEffectData* g = lbl_eu_80664398;
    for (int i = 0; i < 3; i++) {
        PcEffectEntry* e = &g->entries[i];
        if (*(void**)e == r4) {
            *(u32*)e = 0;
            *(u16*)((u8*)e + 4) = 0;
            *(u16*)((u8*)e + 6) = 0;
            *(u8*)((u8*)e + 0xC) = 0;
            return;
        }
    }
}

void func_801B21E0(void* self){}

// Per-frame update: for each active slot, push the transform's translation and
// rotation into the effect object through its SetPos/SetRot virtuals.
void func_801B2318(CPcEffect07* self) {
    // Two induction pointers mirror retail: e walks the entry array
    // (self+0x10+i*0x18), base walks the interleaved mat slots (self+i*0x18
    // with the CMat34* at +0x18). Mat pointers are re-read (not cached) so
    // retail's reload after the SetPos call is reproduced.
    PcEffectEntry* e = self->mEntries;
    u8* base = (u8*)self;
    for (int i = 0; i < 3; i++) {
        if (e->mActive == 1) {
            ml::CVec3 rot;
            ml::CVec3 pos(
                (*(ml::CMat34**)(base + 0x18))->m[0][3],
                (*(ml::CMat34**)(base + 0x18))->m[1][3],
                (*(ml::CMat34**)(base + 0x18))->m[2][3]);
            e->mObj->SetPos(pos);
            (*(ml::CMat34**)(base + 0x18))->getRotXYZ(rot);
            e->mObj->SetRot(rot);
        }
        e++;
        base += 0x18;
    }
}

// Release every active slot: clear the effect object's aux field, set its
// 0x40 flag, then drop the slot (pointer, active flag, aux flag).
void func_801B23D0(PcEffectData* data) {
    if (data == 0) data = lbl_eu_80664398;
    if (data == 0) return;

    if (data->entries[0].mActive != 0) {
        data->entries[0].mObj->mFieldB0 = 0;
        data->entries[0].mObj->mFlags68 |= 0x40;
        data->entries[0].mObj = 0;
        data->entries[0].mActive = 0;
        data->entries[0].mField06 = 0;
    }
    if (data->entries[1].mActive != 0) {
        data->entries[1].mObj->mFieldB0 = 0;
        data->entries[1].mObj->mFlags68 |= 0x40;
        data->entries[1].mObj = 0;
        data->entries[1].mActive = 0;
        data->entries[1].mField06 = 0;
    }
    PcEffectData* d2 = (PcEffectData*)((u8*)data + 0x30);
    if (d2->entries[0].mActive != 0) {
        d2->entries[0].mObj->mFieldB0 = 0;
        d2->entries[0].mObj->mFlags68 |= 0x40;
        d2->entries[0].mObj = 0;
        d2->entries[0].mActive = 0;
        d2->entries[0].mField06 = 0;
    }
}

// Release every active slot (same as func_801B23D0 but only for slots whose
// active flag is exactly 1).
void func_801B248C(PcEffectData* data) {
    if (data == 0) data = lbl_eu_80664398;
    if (data == 0) return;

    if (data->entries[0].mActive == 1) {
        data->entries[0].mObj->mFieldB0 = 0;
        data->entries[0].mObj->mFlags68 |= 0x40;
        data->entries[0].mObj = 0;
        data->entries[0].mActive = 0;
        data->entries[0].mField06 = 0;
    }
    if (data->entries[1].mActive == 1) {
        data->entries[1].mObj->mFieldB0 = 0;
        data->entries[1].mObj->mFlags68 |= 0x40;
        data->entries[1].mObj = 0;
        data->entries[1].mActive = 0;
        data->entries[1].mField06 = 0;
    }
    if (data->entries[2].mActive == 1) {
        data->entries[2].mObj->mFieldB0 = 0;
        data->entries[2].mObj->mFlags68 |= 0x40;
        data->entries[2].mObj = 0;
        data->entries[2].mActive = 0;
        data->entries[2].mField06 = 0;
    }
}

void thunk_adj4_reset(void* self) { ((void(*)(void*))func_801B21E0)((char*)self - 0x4); }

void thunk_adj4_update(void* self) { ((void(*)(void*))func_801B2318)((char*)self - 0x4); }

extern "C" void func_801B2554(u8* self) { ((void(*)(void*))func_801B20C8)((char*)self - 0x4); }

extern "C" void func_801B255C(u8* self) { ((void(*)(void*))__dt__Q22cf11CPcEffect07Fv)((char*)self - 0x4); }

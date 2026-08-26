// Auto-scaffolded catalog TU for kyoshin/cf/CPcEffect07
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

#include "kyoshin/cf/CPcEffect07.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // lbl_eu_80663E14 scene view
#include <monolib/scn/CScnTimeApi.hpp>       // func_80496288 frame-delta query
#include "kyoshin/realtimeevt/CREvtEffect.hpp"

extern "C" void* __dt__Q22cf11CPcEffect07Fv(void*, int);

extern "C" { extern char lbl_eu_80533744[]; }  // vtable data (retail .data)
extern "C" void func_801B23D0(cf::CPcEffect07*);  // effect cleanup

namespace cf {}
using namespace cf;
// Retail ctor: installs the twin vtable pointers (vtbl and vtbl+0x10, second
// base subobject at +4), stores the actor view pointer, value-initializes the
// three effect entries (first element inline, remainder via bdnz loop),
// zeroes the bank byte, stores two copies of an sdata2 float, then publishes
// the singleton.
namespace cf {
CPcEffect07::CPcEffect07(PcEffectActorView* actor) {
    *(u32*)this = (u32)lbl_eu_80533744;
    *((u32*)this + 1) = (u32)lbl_eu_80533744 + 0x10;
    mField08 = 0;
    mField0C = actor;

    // Element 0 written inline; remaining elements via pointer-walk (< walk
    // keeps MWCC's count-computing template instead of full unrolling).
    PcEffectEntry* e = mEntries;
    e->mObj = NULL;
    e->mActive = 0;
    e->mField06 = 0;
    e->mMat = NULL;
    e->mId = 0;
    e->_10 = 0;
    e->_14 = 0;

    PcEffectEntry* end = &mEntries[3];
    PcEffectEntry* p = &mEntries[1];
    while (p < end) {
        p->mObj = NULL;
        p->mActive = 0;
        p->mField06 = 0;
        p->mMat = NULL;
        p->mId = 0;
        p->_10 = 0;
        p->_14 = 0;
        ++p;
    }

    maxEffects = 0;
    mField5C = lbl_eu_80667DE8;
    mField60 = lbl_eu_80667DE8;
    lbl_eu_80664398 = (PcEffectData*)this;
}
} // namespace cf

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

// Spawn/attach an effect object into a slot.
// - selector picks the effect id via the 0x700+n table (switch below).
// - negative selectors scan slots 2..0 for the first non-active entry;
//   non-negative selectors address the slot directly (no bounds check).
// The created object is linked to the actor's schedule sub-object, the
// transform holder from the sub-object is stored as the entry's mMat and
// pushed into the object through SetPos/SetRot, then the bank byte is
// recorded in the entry and the global maxEffects byte.
// r3 = effect type (selects the 0x700+n id via the switch AND is recorded as
// the entry/global id byte); r4 = slot selector (negative = auto-pick).
extern "C" s32 func_801B19F0(u8 type, s32 slotSel) {
    PcEffectData* data = lbl_eu_80664398;
    s32 slot = slotSel;
    if (data == 0)
        return -1;
    if (data->mField5C > lbl_eu_80667DE8)
        return -1;

    // Effect id lookup: type -> 0x700 + n.
    u32 id;
    switch (type) {
    case 6: id = 0x700; break;
    case 4: id = 0x701; break;
    case 7: id = 0x702; break;
    case 8: id = 0x703; break;
    case 9: id = 0x704; break;
    case 5: id = 0x705; break;
    default: id = 0; break;
    }

    // Slot selection: negative selector scans for the first non-active slot.
    if (slot < 0) {
        if (data->entries[0].mActive != 1)
            slot = 0;
        else if (data->entries[1].mActive != 1)
            slot = 1;
        else if (data->entries[2].mActive != 1)
            slot = 2;
    }
    if (slot < 0)
        return slot;

    s32 off = slot * 0x18;
    // Target entry must be free.
    if (((PcEffectSpawnRec*)((char*)data + off))->mActive == 1)
        return -1;

    // Create the effect object through the bank's factory virtual (slot 0x28).
    CPcEffect07Obj* obj =
        reinterpret_cast<PcEffectFactoryView*>(data)->Create(id, -1, 0, 0);
    // Clear the new object's aux slot and stash the bank pointer there.
    obj->mFieldB0 = (u32)lbl_eu_80664398;

    // Link the object to the actor's schedule sub-object (+0x3E9C).
    PcEffectScheduleSub* sub =
        reinterpret_cast<PcEffectScheduleSub*>(lbl_eu_80664398->mActor);
    if (sub != NULL)
        sub = &reinterpret_cast<PcEffectActorView*>(sub)->mSub;
    func_800ACF78(obj, sub, 0);

    // Store the object into the entry, then replace the table-sourced
    // transform pointer with the holder fetched from the sub-object
    // (vtable slot 0x12C, keyed by the per-slot table value). The record
    // address is recomputed at each use (retail keeps only the offset).
    ((PcEffectSpawnRec*)((char*)lbl_eu_80664398 + off))->mObj = obj;
    u32 key = lbl_eu_80533738[slot];
    // Recomputed here (retail reloads the actor pointer after the attach call).
    sub = reinterpret_cast<PcEffectScheduleSub*>(lbl_eu_80664398->mActor);
    if (sub != NULL)
        sub = &reinterpret_cast<PcEffectActorView*>(sub)->mSub;
    PcEffectSchedHolder* holder =
        reinterpret_cast<PcEffectSchedFetchSub*>(sub)->GetHolder(key);
    // Short-lived record view: dies before the virtual calls below.
    PcEffectSpawnRec* rec = (PcEffectSpawnRec*)((char*)lbl_eu_80664398 + off);
    rec->mMat = reinterpret_cast<ml::CMat34*>(holder);
    rec->mActive = 1;

    // Push the holder's translation into the object through SetPos.
    ml::CMat34* mat = rec->mMat;
    f32 z = mat->m[2][3];
    f32 y = mat->m[1][3];
    f32 x = mat->m[0][3];
    ml::CVec3 pos(x, y, z);
    obj->SetPos(pos);

    // Rotation: holder is re-fetched from the entry (not cached across the
    // SetPos call), via getRotXYZ, then SetRot.
    ml::CVec3 rot;
    ((PcEffectSpawnRec*)((char*)lbl_eu_80664398 + off))
        ->mMat->getRotXYZ(rot);
    obj->SetRot(rot);

    // Record the type byte in the entry and globally, reset the cooldown.
    ((PcEffectSpawnRec*)((char*)lbl_eu_80664398 + off))->mId = type;
    lbl_eu_80664398->maxEffects = type;
    lbl_eu_80664398->mField5C = lbl_eu_80667DEC;
    return slot;
}

// Current effect-bank byte: -1 when the global bank is absent or the lookup
// fails, else the bank byte at +0x58 (retail reloads it for the return).
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

// Slot id lookup: negative index scans slots 2..0 for the first active entry;
// a non-negative index addresses the slot directly. Returns the entry's slot
// ID byte, or -1 when no bank/no active entry.
s32 func_801B1CCC(int index) {
    PcEffectData* data = lbl_eu_80664398;
    if (data == 0)
        return -1;

    if (index < 0) {
        int slot;
        for (slot = 2; slot >= 0; slot--) {
            if (data->entries[slot].mActive != 0)
                return data->entries[slot].mId;
        }
        return -1;
    }

    if (data->entries[index].mActive != 0)
        return data->entries[index].mId;
    return -1;
}

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

// Spawn an effect into a slot: index < 0 scans slots 2..0 for the first one
// whose active flag is exactly 1; otherwise index addresses the slot directly
// (no bounds check, matching retail). The entry must be active; its object is
// attached to the handle, the active flag becomes 2, and the per-args state
// is stored. When amount <= 0 the object also gets a func_800ACC14 poke with
// the sign-folded flag (retail's neg/or/srwi idiom always yields 1).
s32 func_801B1E74(u32 handle, s32 index, s32 bankByte, s32 amount) {
    PcEffectData* data = lbl_eu_80664398;
    if (data == 0)
        return 0;
    if (data->mField60 > lbl_eu_80667DE8)
        return -1;

    PcEffectEntry* e = NULL;
    if (index < 0) {
        // Backward scan over slots 2..0 for the first active entry (record
        // view aliases the entry array shifted by -0x10).
        PcEffectScanRec* p = (PcEffectScanRec*)data + 2;
        s32 slot;
        for (slot = 2; slot >= 0; slot--) {
            if (p->mActive == 1) {
                e = &data->entries[slot];
                break;
            }
            p--;
        }
        if (e == NULL)
            return 0;
    } else {
        e = &data->entries[index];
    }

    if (e->mActive != 1)
        return 0;

    func_800ACFD8(e->mObj, (void*)handle);
    e->mActive = 2;
    f32 lim = lbl_eu_80667DF0;
    e->_10 = amount;
    e->_14 = bankByte;
    lbl_eu_80664398->mField60 = lim;
    if (amount <= 0) {
        // Retail folds the byte into a constant via (-v | v) >> 31 + 1.
        func_800ACC14(e->mObj, (s8)((((u32)(-bankByte | bankByte)) >> 31) + 1));
    }
    return e->mId;
}

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
PcEffectSchedHolder* func_801B20C8(CPcEffect07* self, s32 id, s32 sel, s32 amount) {
    PcEffectSchedHolder* result;
    if (sel >= 0) {
        result = self->mField0C->mSub.GetSlot(0);
    } else {
        // Negative selectors index the wstring table of slot ids (id - 0x700).
        s32 tblIdx = id - 0x700;
        result = self->mField0C->mSub.GetSlot(lbl_eu_805049F8[tblIdx]);
    }
    if (amount > 0 && result != NULL && result->mSched != NULL) {
        // s32 -> f32 via the shared 0x43300000 magic double; the xor of the
        // high bit reproduces MWCC's xoris sign-bias conversion.
        PcEffectF64Conv conv;
        conv.w[1] = (u32)amount ^ 0x80000000;
        conv.w[0] = 0x43300000;
        // No explicit f32 cast: MWCC folds the double subtraction straight
        // into the f32 parameter register (retail emits a lone fsubs).
        func_804E3CDC(result->mSched, conv.d - lbl_eu_80667DF8, lbl_eu_80667DF4);
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

// Per-frame tick: while the game manager allows updates, every running slot
// (active flag == 2) counts up its aux timer and decrements its duration;
// when the duration hits zero the object gets a func_800ACC14 poke with a
// sign-folded constant. Afterwards the two timers at +0x5C/+0x60 are stepped
// down by an sdata2 epsilon and clamped to it.
void func_801B21E0(CPcEffect07* self) {
    if (lbl_eu_80664398 == NULL)
        return;
    if (cf::CfGameManager::func_800829B8())
        return;

    for (int i = 0; i < 3; i++) {
        PcEffectEntry* e = &self->mEntries[i];
        if (e->mActive == 2) {
            // Aux timer lives in the same-strided record of the global bank
            // (aliases this entry's mField06).
            lbl_eu_80664398->entries[i].mField06++;
            if ((s32)e->_10 > 0) {
                --e->_10;
                if (e->_10 == 0) {
                    u8 flag = e->_14;
                    // (-v | v) >> 31 + 1 folds any nonzero byte to 1.
                    func_800ACC14(e->mObj,
                                  (s8)(((((u32)(-(s32)(int)flag) | (u32)flag) >> 31) + 1)));
                }
            }
        }
    }

    // Step each timer down by the scene's frame delta and clamp to epsilon.
    f32 nv;
    if (self->mField5C > lbl_eu_80667DE8) {
        nv = self->mField5C - func_80496288(lbl_eu_80663E14);
        self->mField5C = nv;
        if (nv < lbl_eu_80667DE8)
            self->mField5C = lbl_eu_80667DE8;
    }
    if (self->mField60 > lbl_eu_80667DE8) {
        nv = self->mField60 - func_80496288(lbl_eu_80663E14);
        self->mField60 = nv;
        if (nv < lbl_eu_80667DE8)
            self->mField60 = lbl_eu_80667DE8;
    }
}

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
            ml::CMat34* mat = *(ml::CMat34**)(base + 0x18);
            f32 z = mat->m[2][3];
            f32 y = mat->m[1][3];
            f32 x = mat->m[0][3];
            ml::CVec3 pos(x, y, z);
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
// Retail order matters: the global bank is null-checked FIRST (before the
// argument defaulting), then the argument defaults to it.
void func_801B23D0(PcEffectData* data) {
    if (lbl_eu_80664398 == 0)
        return;
    if (data == 0)
        data = lbl_eu_80664398;
    if (data == 0)
        return;

    // Retail fully unrolls this 3-slot loop.
    for (int i = 0; i < 3; i++) {
        if (data->entries[i].mActive != 0) {
            data->entries[i].mObj->mFieldB0 = 0;
            data->entries[i].mObj->mFlags68 |= 0x40;
            data->entries[i].mObj = NULL;
            data->entries[i].mActive = 0;
            data->entries[i].mField06 = 0;
        }
    }
}

// Release every active slot (same as func_801B23D0 but only for slots whose
// active flag is exactly 1).
// Retail order matters: the global bank is loaded and null-checked FIRST,
// then the argument defaults to it.
void func_801B248C(PcEffectData* data) {
    PcEffectData* bank = lbl_eu_80664398;
    if (bank == 0)
        return;
    if (data == 0)
        data = bank;
    if (data == 0)
        return;

    // Retail unrolls this 3-slot loop inline (single hoisted zero, bnelr on
    // the final slot's guard).
    int i;
    for (i = 0; i < 2; i++) {
        if (data->entries[i].mActive != 1)
            continue;
        data->entries[i].mObj->mFieldB0 = 0;
        data->entries[i].mObj->mFlags68 |= 0x40;
        data->entries[i].mObj = NULL;
        data->entries[i].mActive = 0;
        data->entries[i].mField06 = 0;
    }
    // Peeled third slot: retail ends with a bnelr guard + straight-line body.
    if (data->entries[2].mActive != 1)
        return;
    data->entries[2].mObj->mFieldB0 = 0;
    data->entries[2].mObj->mFlags68 |= 0x40;
    data->entries[2].mObj = NULL;
    data->entries[2].mActive = 0;
    data->entries[2].mField06 = 0;
}

void thunk_adj4_reset(void* self) { ((void(*)(void*))func_801B21E0)((char*)self - 0x4); }

void thunk_adj4_update(void* self) { ((void(*)(void*))func_801B2318)((char*)self - 0x4); }

extern "C" void func_801B2554(u8* self) { ((void(*)(void*))func_801B20C8)((char*)self - 0x4); }

extern "C" void func_801B255C(u8* self) { ((void(*)(void*))__dt__Q22cf11CPcEffect07Fv)((char*)self - 0x4); }

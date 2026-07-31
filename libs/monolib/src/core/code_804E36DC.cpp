// Translation unit: monolib/src/core/code_804E36DC
//
// CSchedule runtime (update/step/draw/distance/child management) plus the
// CScheduleItem pool element methods (setup, entry loading, slot update).
// CSchedule layout: see monolib/core/CSchedule.hpp. CScheduleItem layout:
// see CScheduleItem.hpp.

#include <types.h>
#include <decomp.h>
#include <revolution/MTX.h>
#include "monolib/core/CSchedule.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/core/CScheduleItem.hpp"

// Scene/view helpers (other TUs)
extern "C" f32 func_80496288(void* view);
extern "C" void* func_80496264(void* view, s32 idx);

// Effect system helpers (monolib/src/core/code_804DEDA8, effect TUs)
extern "C" void func_804DCA88(void* mtx, const void* params);
extern "C" void* func_804DFBF4(s16 handle);   // item pool lookup
extern "C" void func_804DFE20(s16 handle);    // release object handle
extern "C" CEffectObj* func_804DFEAC(s16 handle); // object pool lookup
extern "C" s16 func_804DFCC4(ScheduleEntry* entries, CItemEntry* entry,
                             CSchedule* sched, s32 slotVal, u32 index);
extern "C" void* func_804CB5FC(CEffectObj* obj);
extern "C" void func_804DF808(CSchedule* sched, void* arg, CEffectObj* obj, f32 delta);
extern "C" void func_804C8D90(CEffectObj* obj, f32 delta);
extern "C" void func_804C9D24(CEffectObj* obj);
extern "C" void func_804C9D30(CEffectObj* obj);
extern "C" void func_804CAA94(CEffectObj* obj);

// ScheduleList callbacks / lookup
extern "C" void func_804E536C(void* arg);
extern "C" void* func_804DF2F0(void* table, void* key);

extern "C" void __dl__FPv(void* ptr);
extern "C" f64 lbl_eu_8066B2F0;
extern "C" f64 lbl_eu_8066B2E8;

// Forward declarations of functions defined below
extern "C" void func_804E39E8(CSchedule* self, f32 delta);
extern "C" void func_804E41B4(CScheduleItem* item);

// ---------------------------------------------------------------------------
// CScheduleItem methods
// ---------------------------------------------------------------------------

// us-804e8224: initialize a pool item (all slots closed).
extern "C" void func_804E3D88(CScheduleItem* item) {
    item->mEntryData = NULL;
    item->mOwner = NULL;
    item->mFlags = 0;
    item->mLifetime = 0;
    item->mCount = 0;
    item->mPoolIndex = -1;
    for (int i = 0; i < 32; i++) {
        item->mSlots[i] = -1;
    }
}

// us-804e82c8: release every slot and clear the item.
extern "C" void func_804E3E2C(CScheduleItem* item) {
    for (int i = 0; i < 32; i++) {
        func_804DFE20(item->mSlots[i]);
        item->mSlots[i] = -1;
    }
    item->mEntryData = NULL;
    item->mOwner = NULL;
    item->mFlags = 0;
    item->mLifetime = 0;
    item->mPoolIndex = -1;
    item->mCount = 0;
}

// us-804e8350: bind an entry blob slice to the item.
extern "C" int func_804E3EB4(CScheduleItem* item, u8* base,
                             const CScheduleEntryData* data, void* owner) {
    item->mLifetime = data->mLifetime;
    u8* entryData = (data->mOffset != 0) ? base + data->mOffset : NULL;
    item->mEntryData = entryData;
    item->mOwner = owner;
    if (entryData != NULL) {
        return 1;
    }
    return 0;
}

// us-804e844c: load the entry blob into slots (once).
extern "C" void func_804E3FB0(CScheduleItem* item, ScheduleEntry* entries,
                              CSchedule* sched) {
    CItemEntry* entry = (CItemEntry*)item->mEntryData;
    u32 count = 0;
    while (entry->mType != 0x4000) {
        s16 slotIndex = entry->mSlotIndex;
        s16 slotVal;
        if (slotIndex >= 0 && slotIndex < 32) {
            slotVal = item->mSlots[slotIndex];
        } else {
            slotVal = -1;
        }
        s16 handle = func_804DFCC4(entries, entry, sched, slotVal, count & 0xFF);
        item->mSlots[count] = handle;
        if (handle >= 0) {
            if (slotIndex >= 0) {
                CEffectObj* obj = func_804DFEAC(slotVal);
                if (obj != NULL) {
                    obj = func_804DFEAC(slotVal);
                    obj->mFlags1C |= 0x2000;
                }
            }
            count++;
        }
        entry = (CItemEntry*)((u8*)entry + 0x16);
    }
    item->mCount = (u8)count;
}

// us-804e8530: advance all slots; drop finished objects.
extern "C" void func_804E4094(CScheduleItem* item, f32 delta) {
    if (item->mCount == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = func_804DFEAC(item->mSlots[i]);
        if (obj != NULL) {
            if (obj->mFlags1C & 0x8000) {
                func_804C8D90(obj, delta);
            } else {
                int referenced = 0;
                for (int j = 0; j < item->mCount; j++) {
                    CEffectObj* other = func_804DFEAC(item->mSlots[j]);
                    if (other != NULL && other->mField20 == item->mSlots[i]) {
                        referenced = 1;
                        break;
                    }
                }
                if (!referenced) {
                    func_804DFE20(item->mSlots[i]);
                    item->mSlots[i] = -1;
                    if (item->mCount != 0) {
                        item->mCount--;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = func_804DFEAC(item->mSlots[i]);
        if (obj != NULL) {
            func_804C9D24(obj);
        }
    }
}

// us-804e8390: per-frame item step used by CSchedule::func_804E39E8.
extern "C" DECOMP_DONT_INLINE int func_804E3EF4(CScheduleItem* item, f32 time, f32 delta,
                             ScheduleEntry* entries, CSchedule* sched) {
    if (!(item->mFlags & 0x80)) {
        func_804E3FB0(item, entries, sched);
        item->mFlags |= 0x80;
    }
    if (item->mLifetime != 0) {
        if (time < (f32)item->mLifetime) {
            return 1;
        }
    }
    func_804E4094(item, delta);
    return item->mCount != 0;
}

// us-804e8650: emit/update all slot objects (two passes).
extern "C" DECOMP_DONT_INLINE void func_804E41B4(CScheduleItem* item) {
    if (item->mCount == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = func_804DFEAC(item->mSlots[i]);
        if (obj != NULL) {
            func_804C9D30(obj);
        }
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = func_804DFEAC(item->mSlots[i]);
        if (obj != NULL) {
            func_804CAA94(obj);
        }
    }
}

// us-804e86e8: feed distance feedback into renderable slot objects.
extern "C" DECOMP_DONT_INLINE void func_804E424C(CScheduleItem* item, f32 delta, CSchedule* sched) {
    if (item->mCount == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = func_804DFEAC(item->mSlots[i]);
        if (obj != NULL && obj->mField2E > 0) {
            void* arg = func_804CB5FC(obj);
            func_804DF808(sched, arg, obj, delta);
        }
    }
}

// ---------------------------------------------------------------------------
// CSchedule methods
// ---------------------------------------------------------------------------

// us-804e7b78: main update: transform flags, fixed-timestep stepping.
extern "C" void func_804E36DC(CSchedule* self, f32 dt) {
    if (self->field_0x00 & 0x100) {
        return;
    }

    f32 step = dt * self->field_0x50 * func_80496288(self->field_0x10);

    u32 hasTranslation =
        (self->field_0x1c != lbl_eu_8066B2E4) || (self->field_0x20 != lbl_eu_8066B2E4) || (self->field_0x24 != lbl_eu_8066B2E4);
    self->field_0x00 = (self->field_0x00 & ~0x2000) | (hasTranslation << 13);

    u32 hasRotation =
        (self->field_0x28 != lbl_eu_8066B2E4) || (self->field_0x2c != lbl_eu_8066B2E4) || (self->field_0x30 != lbl_eu_8066B2E4);
    self->field_0x00 = (self->field_0x00 & ~0x1000) | (hasRotation << 12);

    u32 hasScale =
        (self->field_0x34 != lbl_eu_8066B2E0) || (self->field_0x38 != lbl_eu_8066B2E0) || (self->field_0x3c != lbl_eu_8066B2E0);
    self->field_0x00 = (self->field_0x00 & ~0x800) | (hasScale << 11);

    if ((self->field_0x00 & 0x2000) || (self->field_0x00 & 0x1000) || (self->field_0x00 & 0x800)) {
        func_804DCA88(&self->mMatrix, &self->field_0x28);
        self->mMatrix.m[0][3] += self->field_0x1c;
        self->mMatrix.m[1][3] += self->field_0x20;
        self->mMatrix.m[2][3] += self->field_0x24;
    } else {
        self->mMatrix = ml::CMat34::identity;
    }

    if (self->mEntryCount == 0) {
        self->field_0x90 = lbl_eu_8066B2E4;
        self->field_0x94 = lbl_eu_8066B2E0;
        return;
    }

    if (self->field_0x90 != lbl_eu_8066B2E4) {
        s32 denom = (s32)self->field_0x94;
        s32 numer = (s32)(step + self->field_0x90);
        s32 count = numer / denom;
        s32 rem = numer - count * denom;
        step = (f32)rem;
        if (step <= lbl_eu_8066B2E4) {
            step = (f32)denom;
            count--;
        }
        for (s32 i = 0; i < count; i++) {
            func_804E39E8(self, (f32)denom);
            if (self->mEntryCount != 0) {
                for (int j = 0; j < 32; j++) {
                    CScheduleItem* item = (CScheduleItem*)func_804DFBF4(self->mHandles[j]);
                    if (item != NULL) {
                        func_804E41B4(item);
                    }
                }
            }
        }
        self->field_0x90 = lbl_eu_8066B2E4;
        self->field_0x94 = lbl_eu_8066B2E0;
    }

    func_804E39E8(self, step);
}

// us-804e7e84: step all resolved entries once; retire when none remain active.
extern "C" void func_804E39E8(CSchedule* self, f32 delta) {
    if (self->mEntryCount == 0) {
        return;
    }
    self->field_0xd9 = 0;
    for (int i = 0; i < 32; i++) {
        CScheduleItem* item = (CScheduleItem*)func_804DFBF4(self->mHandles[i]);
        if (item != NULL) {
            if (func_804E3EF4(item, self->field_0x54, delta, self->mEntries, self) != 0) {
                self->field_0xd9++;
            }
        }
    }
    if (self->field_0xd9 <= 0) {
        self->field_0x00 = (u16)DECOMP_PPC_RLWINM((u32)self->field_0x00, 0, 17, 15);
        if (self->mEntryCount != 0) {
            for (int i = 0; i < 32; i++) {
                if (self->mHandles[i] >= 0) {
                    func_804DFB88(self->mHandles[i]);
                    self->mHandles[i] = -1;
                }
            }
            self->mEntryCount = 0;
        }
    }
    self->field_0x54 += delta;
}

// us-804e7fa4: emit/update every resolved item.
extern "C" void func_804E3B08(CSchedule* self) {
    if (self->mEntryCount == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        CScheduleItem* item = (CScheduleItem*)func_804DFBF4(self->mHandles[i]);
        if (item != NULL) {
            func_804E41B4(item);
        }
    }
}

// us-804e8008: compute distance to the schedule target and feed it to items.
extern "C" void func_804E3B6C(CSchedule* self) {
    if (self->mEntryCount == 0) {
        return;
    }
    f32 dist;
    if (self->field_0x14 != 0) {
        CSchedulePosLink* link = reinterpret_cast<CSchedulePosLink*>(self->field_0x14);
        const ml::CVec3* pos = (const ml::CVec3*)link->vfunc11();
        void* target = func_80496264(self->field_0x10, -1);
        ml::CVec3 diff = *(const ml::CVec3*)((u8*)target + 0x10C) - *pos;
        dist = PSVECMag(diff);
    } else {
        void* target = func_80496264(self->field_0x10, -1);
        ml::CVec3 diff = *(const ml::CVec3*)((u8*)target + 0x10C) - *(const ml::CVec3*)&self->field_0x1c;
        dist = PSVECMag(diff);
    }
    for (int i = 0; i < 32; i++) {
        CScheduleItem* item = (CScheduleItem*)func_804DFBF4(self->mHandles[i]);
        if (item != NULL) {
            func_804E424C(item, dist, self);
        }
    }
}

// us-804e8168: clear the retired flag and notify listeners.
extern "C" void func_804E3CCC(CSchedule* self) {
    self->field_0x00 = (u16)DECOMP_PPC_RLWINM((u32)self->field_0x00, 0, 17, 15);
    func_804E536C(self);
}

// us-804e8178: set the fixed-timestep accumulator/interval (non-zero only).
extern "C" void func_804E3CDC(CSchedule* self, f32 accumulator, f32 interval) {
    if (accumulator == lbl_eu_8066B2E4) {
        return;
    }
    if (interval == lbl_eu_8066B2E4) {
        return;
    }
    self->field_0x90 = accumulator;
    self->field_0x94 = interval;
}

// us-804e8198: look up an entry by key.
extern "C" void* func_804E3CFC(CSchedule* self) {
    return func_804DF2F0(self->field_0x0c, self->mEntries);
}

// us-804e81a8: register a child in the first free child slot.
extern "C" void func_804E3D0C(CSchedule* self, CScheduleChild* child) {
    for (int i = 0; i < 4; i++) {
        if (self->mChildren[i] == NULL) {
            self->mChildren[i] = child;
            return;
        }
    }
}

// us-804e81e4: unregister a child.
extern "C" void func_804E3D48(CSchedule* self, CScheduleChild* child) {
    for (int i = 0; i < 4; i++) {
        if (self->mChildren[i] == child) {
            self->mChildren[i] = NULL;
            return;
        }
    }
}

// us-804e8790: deleting destructor (EU __dt__804E42F4).
extern "C" void* __dt__804E42F4(void* self, s32 freeIt) {
    if (self != NULL && freeIt > 0) {
        __dl__FPv(self);
    }
    return self;
}

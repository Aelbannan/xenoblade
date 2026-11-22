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
#include "monolib/core/code_804E36DC.hpp"


// ---------------------------------------------------------------------------
// CScheduleItem methods
// ---------------------------------------------------------------------------

// us-804e8224: initialize a pool item (all slots closed).
void CScheduleItem::ClearSlots() {
    this->mEntryData = NULL;
    this->mOwner = NULL;
    this->mFlags = 0;
    this->mLifetime = 0;
    this->mCount = 0;
    this->mPoolIndex = -1;
    for (int i = 0; i < 32; i++) {
        this->mSlots[i] = -1;
    }
}

// us-804e82c8: release every slot and clear the item.
void CScheduleItem::ReleaseSlots() {
    for (int i = 0; i < 32; i++) {
        SchedFx_ReleaseHandle(this->mSlots[i]);
        this->mSlots[i] = -1;
    }
    this->mEntryData = NULL;
    this->mOwner = NULL;
    this->mFlags = 0;
    this->mLifetime = 0;
    this->mPoolIndex = -1;
    this->mCount = 0;
}

// us-804e8350: bind an entry blob slice to the item.
int CScheduleItem::InitFromData(u8* base, const CScheduleEntryData* data, CSchedule* owner) {
    this->mLifetime = data->mLifetime;
    u16 offset = data->mOffset;
    u8* entryData = (offset != 0) ? base + offset : NULL;
    this->mEntryData = entryData;
    this->mOwner = owner;
    if (entryData == NULL) {
        return 0;
    }
    return 1;
}

// us-804e844c: load the entry blob into slots (once).
void CScheduleItem::LoadSlots(ScheduleEntry* entries, CSchedule* sched) {
    CItemEntry* entry = (CItemEntry*)this->mEntryData;
    u32 count = 0;
    while (entry->mType != 0x4000) {
        s16 slotIndex = entry->mSlotIndex;
        s16 slotVal;
        if (slotIndex >= 0 && slotIndex < 32) {
            slotVal = this->mSlots[slotIndex];
        } else {
            slotVal = -1;
        }
        s16 handle = SchedFx_ClaimConstruct(entries, entry, sched, slotVal, count & 0xFF);
        this->mSlots[count] = handle;
        if (handle >= 0) {
            if (slotIndex >= 0) {
                CEffectObj* obj = SchedFx_FetchByHandle(slotVal);
                if (obj != NULL) {
                    obj = SchedFx_FetchByHandle(slotVal);
                    obj->mFlags1C |= 0x2000;
                }
            }
            count++;
        }
        entry = (CItemEntry*)((u8*)entry + 0x16);
    }
    this->mCount = (u8)count;
}

// us-804e8530: advance all slots; drop finished objects.
void CScheduleItem::StepSlots(f32 delta) {
    if (this->mCount == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = SchedFx_FetchByHandle(this->mSlots[i]);
        if (obj != NULL) {
            if (obj->mFlags1C & 0x8000) {
                EffObj_Update(obj, delta);
            } else {
                int referenced = 0;
                for (int j = 0; j < this->mCount; j++) {
                    CEffectObj* other = SchedFx_FetchByHandle(this->mSlots[j]);
                    if (other != NULL && other->mField20 == this->mSlots[i]) {
                        referenced = 1;
                        break;
                    }
                }
                if (!referenced) {
                    SchedFx_ReleaseHandle(this->mSlots[i]);
                    this->mSlots[i] = -1;
                    if (this->mCount != 0) {
                        this->mCount--;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = SchedFx_FetchByHandle(this->mSlots[i]);
        if (obj != NULL) {
            EffSys_AdvanceChain(obj);
        }
    }
}

// us-804e8650: emit/update all slot objects (two passes).
void CScheduleItem::ReleaseObjects() {
    if (this->mCount == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = SchedFx_FetchByHandle(this->mSlots[i]);
        if (obj != NULL) {
            EffFrame_Update(obj);
        }
    }
    for (int i = 0; i < 32; i++) {
        CEffectObj* obj = SchedFx_FetchByHandle(this->mSlots[i]);
        if (obj != NULL) {
            EffObj_StepNode(obj);
        }
    }
}

// us-804e86e8: feed distance feedback into renderable slot objects.
void CScheduleItem::FeedDistance(f32 delta, CSchedule* sched) {
    if (this->mCount == 0) {
        return;
    }
    CEffectObj* obj;
    for (int i = 0; i < 32; i++) {
        obj = SchedFx_FetchByHandle(this->mSlots[i]);
        if (obj != NULL && obj->mNodeHead.field_0x0a > 0) {
            void* arg = EffObj_GetPriority(obj);
            SchedElem_ClaimNode(sched, arg, obj, delta);
        }
    }
}

// ---------------------------------------------------------------------------
// CSchedule methods
// ---------------------------------------------------------------------------

// us-804e7b78: main update: transform flags, fixed-timestep stepping.
void CSchedule::Update(f32 dt) {
    CScheduleFlags* flags = &this->field_0x00;
    if (flags->b8) {
        return;
    }

    f32 step = dt * this->field_0x50 * Scn_GetFrameDelta(this->field_0x10);

    u32 hasTranslation =
        (this->field_0x1c != lbl_eu_8066B2E4) || (this->field_0x20 != lbl_eu_8066B2E4) || (this->field_0x24 != lbl_eu_8066B2E4);
    flags->b13 = hasTranslation;

    u32 hasRotation =
        (this->field_0x28 != lbl_eu_8066B2E4) || (this->field_0x2c != lbl_eu_8066B2E4) || (this->field_0x30 != lbl_eu_8066B2E4);
    flags->b12 = hasRotation;

    u32 hasScale =
        (this->field_0x34 != lbl_eu_8066B2E0) || (this->field_0x38 != lbl_eu_8066B2E0) || (this->field_0x3c != lbl_eu_8066B2E0);
    flags->b11 = hasScale;

    if (flags->b13 || flags->b12 || flags->b11) {
        CERotEulerXYZ(&this->mMatrix, &this->field_0x28);
        this->mMatrix.m[0][3] += this->field_0x1c;
        this->mMatrix.m[1][3] += this->field_0x20;
        this->mMatrix.m[2][3] += this->field_0x24;
    } else {
        this->mMatrix = ml::CMat34::identity;
    }

    if (this->mEntryCount == 0) {
        this->field_0x94 = 1.0f;
        this->field_0x90 = 0.0f;
        return;
    }

    if (this->field_0x90 != lbl_eu_8066B2E4) {
        s32 denom = (s32)this->field_0x94;
        s32 numer = (s32)(step + this->field_0x90);
        s32 count = numer / denom;
        s32 rem = numer - count * denom;
        step = (f32)rem;
        if (step <= lbl_eu_8066B2E4) {
            step = (f32)denom;
            count--;
        }
        for (s32 i = 0; i < count; i++) {
            this->StepEntries((f32)denom);
            if (this->mEntryCount != 0) {
                for (int j = 0; j < 32; j++) {
                    CScheduleItem* item = (CScheduleItem*)SchedItem_FetchByHandle(this->mHandles[j]);
                    if (item != NULL) {
                        item->ReleaseObjects();
                    }
                }
            }
        }
        this->field_0x94 = 1.0f;
        this->field_0x90 = 0.0f;
    }

    this->StepEntries(step);
}

// us-804e7e84: step all resolved entries once; retire when none remain active.
void CSchedule::StepEntries(f32 delta) {
    s16* h2;
    s16* h;
    int i;
    CScheduleItem* item;
    if (this->mEntryCount == 0) {
        return;
    }
    this->field_0xd9 = 0;
    h = (s16*)this;
    i = 0;
    for (; i < 32; i++, h++) {
        item = (CScheduleItem*)SchedItem_FetchByHandle(h[76]);
        if (item != NULL) {
            if (item->TickLifetime(this->field_0x54, delta, this->mEntries, this) != 0) {
                this->field_0xd9++;
            }
        }
    }
    if (this->field_0xd9 <= 0) {
        this->field_0x00.b15 = 0;
        if (this->mEntryCount != 0) {
            h2 = (s16*)this;
            i = 0;
            for (; i < 32; i++, h2++) {
                if (h2[76] >= 0) {
                    SchedItem_ReleaseHandle(h2[76]);
                    h2[76] = -1;
                }
            }
            this->mEntryCount = 0;
        }
    }
    this->field_0x54 += delta;
}

// us-804e7fa4: emit/update every resolved item.
extern "C" void schedReleaseAllItems(CSchedule* self) {
    if (self->mEntryCount == 0) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        CScheduleItem* item = (CScheduleItem*)SchedItem_FetchByHandle(self->mHandles[i]);
        if (item != NULL) {
            item->ReleaseObjects();
        }
    }
}

// us-804e8008: compute distance to the schedule target and feed it to items.
void CSchedule::FeedTargetDist() {
    if (this->mEntryCount == 0) {
        return;
    }
    f32 dist;
    if (this->field_0x14 != 0) {
        CSchedulePosLink* link = reinterpret_cast<CSchedulePosLink*>(this->field_0x14);
        const ml::CVec3* pos = (const ml::CVec3*)link->vfunc11();
        u8* target = (u8*)Scn_FindCamItem(this->field_0x10, -1);
        ml::CVec3 diff = *(const ml::CVec3*)((u8*)target + 0x10C) - *pos;
        dist = PSVECMag(diff);
    } else {
        u8* target = (u8*)Scn_FindCamItem(this->field_0x10, -1);
        ml::CVec3 diff = *(const ml::CVec3*)((u8*)target + 0x10C) - *(const ml::CVec3*)&this->field_0x1c;
        dist = PSVECMag(diff);
    }
    for (int i = 0; i < 32; i++) {
        CScheduleItem* item = (CScheduleItem*)SchedItem_FetchByHandle(this->mHandles[i]);
        if (item != NULL) {
            item->FeedDistance(dist, this);
        }
    }
}

// us-804e8168: clear the retired flag and notify listeners.
extern "C" void schedClearFlag15Update(CSchedule* self) {
    self->field_0x00.b15 = 0;
    SchedList_DropBoth(self);
}

// us-804e8178: set the fixed-timestep accumulator/interval (non-zero only).
extern "C" void schedSetStepIntervals(CSchedule* self, f32 accumulator, f32 interval) {
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
extern "C" void* schedFindEntryByKey(CSchedule* self) {
    return SchedRes_FindValueByKey(self->field_0x0c, self->mEntries);
}

// us-804e81a8: register a child in the first free child slot.
extern "C" void schedAttachChildSlot(CSchedule* self, CScheduleChild* child) {
    for (int i = 0; i < 4; i++) {
        if (self->mChildren[i] == NULL) {
            self->mChildren[i] = child;
            return;
        }
    }
}

// us-804e81e4: unregister a child.
extern "C" void schedDetachChildSlot(CSchedule* self, CScheduleChild* child) {
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
        operator delete(self);
    }
    return self;
}

// us-804e8390: per-frame item step used by CSchedule::StepEntries.
int CScheduleItem::TickLifetime(f32 time, f32 delta, ScheduleEntry* entries, CSchedule* sched) {
    if (!((this->mFlags >> 7) & 1)) {
        this->LoadSlots(entries, sched);
        this->mFlags |= 0x80;
    }
    if (this->mLifetime != 0) {
        if (time < (f32)this->mLifetime) {
            return 1;
        }
    }
    this->StepSlots(delta);
    return this->mCount != 0;
}

// Translation unit: monolib/src/core/CSchedule
// CSchedule manages a list of schedule entries, resolving each to a handle.

#include "monolib/core/CSchedule.hpp"
#include <string.h>

// us-804e7730: CSchedule::__ct__CSchedule()
extern "C" void __ct__CSchedule(CSchedule* self) {
    self->mTablePtr = lbl_eu_80570068;
    self->mEntries = nullptr;
    self->field_0x08 = nullptr;
    self->field_0x0c = nullptr;
    self->field_0x10 = nullptr;
    self->mEntryCount = 0;
    for (int i = 0; i < 32; i++) {
        self->mHandles[i] = -1;
    }
}

// us-804e7ab0: CSchedule::func_804E3614()
extern "C" int func_804E3614(CSchedule* self) {
    self->mEntryCount = 0;
    ScheduleEntry* entry = self->mEntries;
    s16 count = 0;
    while (entry->type != 0xFFFF) {
        // No more room -> bail out success (converges on the post-loop return 1).
        if (count >= 0x20) break;
        if (entry->flags == 0 || entry->flags >= 0xFFFF) {
            return 0;
        }
        s16 handle = func_804DFA84(self->mEntries, entry, self);
        if (handle < 0) {
            return 0;
        }
        // Write through explicit pointer arithmetic with s16 sign-extension and the
        // array offset last, so MWCC keeps the base+displacement sth (@REF:17).
        *(s16*)((u8*)self + (u32)(s16)count * 2 + 0x98) = handle;
        count++;
        entry++;
        self->mEntryCount++;
    }
    return 1;
}

// us-804e77dc: CSchedule::~CSchedule()
CSchedule::~CSchedule() {
    mTablePtr = lbl_eu_80570068;
    if (mEntries != nullptr) {
        // Notify each child (virtual call at vtable[3], passing this)
        for (u32 i = 0; i < 4; i++) {
            CScheduleChild* child = mChildren[i];
            if (child != nullptr) {
                void (**vfn)(void*, void*) = *(void(***)(void*, void*))child;
                vfn[3](child, this);
            }
        }
        // Release resolved handles
        if (mEntryCount > 0) {
            for (int i = 0; i < 0x20; i++) {
                if (mHandles[i] >= 0) {
                    func_804DFB88(mHandles[i]);
                    mHandles[i] = -1;
                }
            }
            mEntryCount = 0;
        }
        mEntries = nullptr;
        field_0x08 = nullptr;
        field_0x0c = nullptr;
        field_0x10 = nullptr;
        mEntryCount = 0;
    }
}

// us-804e78d0: CSchedule::func_804E3434()
// Full initialization: sets up all fields and resolves schedule entries.
// Parameters: p4=entries, p5=field_0x08, p6=field_0x0c,
//             flags_a=packed flags from r7, flags_b=packed flags from r8, p9=field_0x10
extern "C" int func_804E3434(CSchedule* self, ScheduleEntry* entries, u8* field_0x08_ptr, u8* field_0x0c_ptr, u32 flags_a, u32 flags_b, u8* field_0x10_ptr) {
    float zero;
    float one;
    u32 flags;
    u32 zero32;

    if (entries == nullptr) return 0;
    if (field_0x08_ptr == nullptr) return 0;

    one = lbl_eu_8066B2E0;
    zero = lbl_eu_8066B2E4;
    zero32 = 0;

    self->mEntries = entries;
    self->field_0x08 = field_0x08_ptr;

    self->field_0x14 = zero32;
    self->field_0x18 = zero32;

    // Build flags: 0xC000 base with bits 21/22 from flags_a/flags_b
    // (ori 0xC000 + rlwimi per retail)
    flags = 0xC000 | ((flags_a & 0x800) << 10) | ((flags_b & 0x2000) << 9);
    self->field_0x00 = static_cast<s16>(flags);

    self->field_0x0c = field_0x0c_ptr;
    self->field_0x10 = field_0x10_ptr;

    self->field_0x50 = one;
    self->field_0x54 = zero;
    self->field_0x1c = zero;
    self->field_0x20 = zero;
    self->field_0x24 = zero;
    self->field_0x28 = zero;
    self->field_0x2c = zero;
    self->field_0x30 = zero;
    self->field_0x34 = one;
    self->field_0x38 = one;
    self->field_0x3c = one;
    self->field_0x4c = one;
    self->field_0x48 = one;
    self->field_0x44 = one;
    self->field_0x40 = one;

    // Copy identity matrix
    self->mMatrix = ml::CMat34::identity;

    self->field_0x90 = zero;
    self->field_0x94 = one;

    self->field_0x58 = 0x40;
    self->field_0x59 = 0;
    self->field_0x5c = zero32;

    for (int i = 0; i < 32; i++) {
        self->mHandles[i] = -1;
    }
    memset(self->mChildren, 0, sizeof(self->mChildren));

    self->mEntryCount = 0;
    self->field_0xd9 = 0;

    return func_804E3614(self);
}

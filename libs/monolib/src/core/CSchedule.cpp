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
    int count = 0;
    while (entry->type != 0xFFFF) {
        if (count >= 0x20) break;
        if (entry->flags == 0 || entry->flags >= 0xFFFF) {
            return 0;
        }
        s16 handle = func_804DFA84(self->mEntries, entry, self);
        if (handle < 0) return 0;
        self->mHandles[count] = handle;
        count++;
        self->mEntryCount++;
        entry++;
    }
    return 1;
}

// us-804e77dc: CSchedule::__dt__9CScheduleFv()
extern "C" void __dt__9CScheduleFv(CSchedule* self, int del) {
    if (self == nullptr) return;
    self->mTablePtr = lbl_eu_80570068;
    if (self->mEntries != nullptr) {
        // Destroy child objects (each has a virtual destructor at vtable[3])
        for (int i = 0; i < 4; i++) {
            CScheduleChild* child = self->mChildren[i];
            if (child != nullptr) {
                void** vtable = *reinterpret_cast<void***>(child);
                void (*destructorFn)(void*, int) = reinterpret_cast<void(*)(void*, int)>(vtable[3]);
                destructorFn(child, 1);
            }
        }
        // Release resolved handles
        if (self->mEntryCount > 0) {
            for (int i = 0; i < 0x20; i++) {
                s16 handle = self->mHandles[i];
                if (handle >= 0) {
                    func_804DFB88(handle);
                    self->mHandles[i] = -1;
                }
            }
            self->mEntryCount = 0;
        }
        self->mEntries = nullptr;
        self->field_0x08 = nullptr;
        self->field_0x0c = nullptr;
        self->field_0x10 = nullptr;
        self->mEntryCount = 0;
    }
    if (del > 0) {
        ::operator delete(self);
    }
}

// us-804e78d0: CSchedule::func_804E3434()
// Full initialization: sets up all fields and resolves schedule entries.
// Parameters: p4=entries, p5=field_0x08, p6=field_0x0c,
//             flags_a=packed flags from r7, flags_b=packed flags from r8, p9=field_0x10
extern "C" int func_804E3434(CSchedule* self, ScheduleEntry* entries, u8* field_0x08_ptr, u8* field_0x0c_ptr, u32 flags_a, u32 flags_b, u8* field_0x10_ptr) {
    if (entries == nullptr) return 0;
    if (field_0x08_ptr == nullptr) return 0;

    self->mEntries = entries;
    self->field_0x08 = field_0x08_ptr;

    // Build flags: 0xC000 with optional bits from flags_a/flags_b packed at specific positions
    u32 flags = 0xC000;
    flags |= (flags_a << 10) & 0x200000;
    flags |= (flags_b << 9) & 0x400000;
    self->field_0x00 = static_cast<s16>(flags);

    self->field_0x0c = field_0x0c_ptr;
    self->field_0x10 = field_0x10_ptr;
    self->field_0x14 = 0;
    self->field_0x18 = 0;

    self->field_0x50 = lbl_eu_8066B2E0;
    self->field_0x54 = lbl_eu_8066B2E4;
    self->field_0x1c = lbl_eu_8066B2E4;
    self->field_0x20 = lbl_eu_8066B2E4;
    self->field_0x24 = lbl_eu_8066B2E4;
    self->field_0x28 = lbl_eu_8066B2E4;
    self->field_0x2c = lbl_eu_8066B2E4;
    self->field_0x30 = lbl_eu_8066B2E4;
    self->field_0x34 = lbl_eu_8066B2E0;
    self->field_0x38 = lbl_eu_8066B2E0;
    self->field_0x3c = lbl_eu_8066B2E0;
    self->field_0x4c = lbl_eu_8066B2E0;
    self->field_0x48 = lbl_eu_8066B2E0;
    self->field_0x44 = lbl_eu_8066B2E0;
    self->field_0x40 = lbl_eu_8066B2E0;

    // Copy identity matrix
    self->mMatrix = ml::CMat34::identity;

    self->field_0x90 = lbl_eu_8066B2E4;
    self->field_0x94 = lbl_eu_8066B2E0;

    for (int i = 0; i < 32; i++) {
        self->mHandles[i] = -1;
    }
    memset(self->mChildren, 0, sizeof(self->mChildren));

    self->field_0x58 = 0x40;
    self->field_0x59 = 0;
    self->field_0x5c = 0;
    self->mEntryCount = 0;
    self->field_0xd9 = 0;

    return func_804E3614(self);
}

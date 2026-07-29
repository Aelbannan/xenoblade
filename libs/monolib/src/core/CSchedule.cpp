#include "monolib/core/CSchedule.hpp"
#include <string.h>

// us-804e7730: CSchedule::__ct__CSchedule()
void __ct__CSchedule(void* thisPtr) {
    CSchedule* self = static_cast<CSchedule*>(thisPtr);
    self->mTablePtr = lbl_eu_80570068;
    self->field_0x04 = nullptr;
    self->field_0x08 = nullptr;
    self->field_0x0c = nullptr;
    self->field_0x10 = nullptr;
    self->field_0xd8 = 0;
    for (int i = 0; i < 32; i++) {
        self->field_0x98[i] = -1;
    }
}

// us-804e7ab0: CSchedule::func_804E3614()
// Resolves each schedule entry to a handle via func_804DFA84.
int func_804E3614(void* thisPtr) {
    CSchedule* self = static_cast<CSchedule*>(thisPtr);
    self->field_0xd8 = 0;
    ScheduleEntry* entry = static_cast<ScheduleEntry*>(self->field_0x04);
    int count = 0;
    while (entry->type != 0xFFFF) {
        if (count >= 0x20) break;
        if (entry->flags == 0 || entry->flags >= 0xFFFF) {
            return 0;
        }
        s16 handle = func_804DFA84(self->field_0x04, entry, self);
        if (handle < 0) return 0;
        self->field_0x98[count] = handle;
        count++;
        self->field_0xd8++;
        entry++;
    }
    return 1;
}

// us-804e77dc: CSchedule::__dt__9CScheduleFv()
void __dt__9CScheduleFv(void* thisPtr, int del) {
    CSchedule* self = static_cast<CSchedule*>(thisPtr);
    if (self == nullptr) return;
    self->mTablePtr = lbl_eu_80570068;
    if (self->field_0x04 != nullptr) {
        // Destroy child objects (each has a virtual destructor at vtable[3])
        for (int i = 0; i < 4; i++) {
            void* child = self->field_0xdc[i];
            if (child != nullptr) {
                void** vtable = *reinterpret_cast<void***>(child);
                void* destructorFn = vtable[3];
                reinterpret_cast<void(*)(void*, int)>(destructorFn)(child, 1);
            }
        }
        // Release resolved handles
        if (self->field_0xd8 > 0) {
            for (int i = 0; i < 0x20; i++) {
                s16 handle = self->field_0x98[i];
                if (handle >= 0) {
                    func_804DFB88(handle);
                    self->field_0x98[i] = -1;
                }
            }
            self->field_0xd8 = 0;
        }
        self->field_0x04 = nullptr;
        self->field_0x08 = nullptr;
        self->field_0x0c = nullptr;
        self->field_0x10 = nullptr;
        self->field_0xd8 = 0;
    }
    if (del > 0) {
        // operator delete
        ::operator delete(thisPtr);
    }
}

// us-804e78d0: CSchedule::func_804E3434()
// Full initialization: sets up all fields and resolves schedule entries.
int func_804E3434(void* thisPtr, void* p4, void* p5, void* p6, u32 r7, u32 r8, void* p9) {
    CSchedule* self = static_cast<CSchedule*>(thisPtr);
    if (p4 == nullptr) return 0;
    if (p5 == nullptr) return 0;

    self->field_0x04 = p4;
    self->field_0x08 = p5;

    // Build packed flags halfword at field_0x00
    u16 flags = 0xC000;
    if (r7 & 0x400) flags |= 0x0400;  // rlwimi bit 21 from r7<<10 → bit 9
    if (r8 & 0x800) flags |= 0x0800;  // rlwimi bit 22 from r8<<9 → bit 11
    self->field_0x00 = static_cast<s16>(flags);

    self->field_0x0c = p6;
    self->field_0x10 = p9;
    self->field_0x14 = 0;
    self->field_0x18 = 0;

    // Initialize float fields
    self->field_0x50 = lbl_eu_8066B2E0;  // 1.0
    self->field_0x54 = lbl_eu_8066B2E4;  // 0.0
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
    self->mMatrix = *reinterpret_cast<ml::CMat34*>(lbl_eu_80570068);  // wrong, need identity

    self->field_0x90 = lbl_eu_8066B2E4;
    self->field_0x94 = lbl_eu_8066B2E0;

    for (int i = 0; i < 32; i++) {
        self->field_0x98[i] = -1;
    }
    memset(self->field_0xdc, 0, 0x10);

    self->field_0xd8 = 0;
    self->field_0xd9 = 0;

    return func_804E3614(self);
}

// Translation unit: monolib/src/core/CSchedule
// CSchedule manages a list of schedule entries, resolving each to a handle.

#include "monolib/core/CSchedule.hpp"
#include <string.h>

// Retail destructor mangled name (CSchedule::~CSchedule) for the vtable reloc.
extern "C" void __dt__9CScheduleFv();

// === .rodata size=0x10 align=8 ===
extern "C" __declspec(align(8)) const char lbl_eu_80524730[16] = {
    0x43,0x53,0x63,0x68,0x65,0x64,0x75,0x6C,0x65,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,
};

// === .sdata size=0x8 align=8 ===
// RTTI locator for "CSchedule" (name ptr + 0).
extern "C" u32 __RTTI__9CSchedule[2] = { (u32)&lbl_eu_80524730, 0x00000000 };

// === .data size=0x10 align=8 ===
// CSchedule vtable (16B): typeinfo locator + dtor.
extern "C" u32 lbl_eu_80570068[4] = {
    (u32)&__RTTI__9CSchedule, 0x00000000, (u32)&__dt__9CScheduleFv, 0x00000000,
};

// us-804e7730: CSchedule::__ct__CSchedule()
extern "C" void __ct__CSchedule(CSchedule* self) {
    self->mTablePtr = (u8*)lbl_eu_80570068;
    self->mEntries = nullptr;
    self->field_0x08 = nullptr;
    self->field_0x0c = nullptr;
    self->field_0x10 = nullptr;
    self->mEntryCount = 0;
    for (int i = 0; i < 32; i++) {
        self->mHandles[i] = -1;
    }
}

// Release resolved handles (inlined into ~CSchedule).
inline void CSchedule::ReleaseHandles() {
    if (mEntryCount > 0) {
        for (int i = 0; i < 0x20; i++) {
            if (mHandles[i] >= 0) {
                SchedItem_ReleaseHandle(mHandles[i]);
                mHandles[i] = -1;
            }
        }
        mEntryCount = 0;
    }
}

// us-804e77dc: CSchedule::~CSchedule()
CSchedule::~CSchedule() {
    mTablePtr = (u8*)lbl_eu_80570068;
    if (mEntries != nullptr) {
        // Notify each registered child
        for (u32 i = 0; i < 4; i++) {
            CScheduleChild* child = mChildren[i];
            if (child != nullptr) {
                child->OnScheduleDelete(this);
            }
        }
        ReleaseHandles();
        mEntries = nullptr;
        field_0x08 = nullptr;
        field_0x0c = nullptr;
        field_0x10 = nullptr;
        mEntryCount = 0;
    }
}

// us-804e78d0: CSchedule::Sched_Init()
// Full initialization: sets up all fields and resolves schedule entries.
// Parameters: p4=entries, p5=field_0x08, p6=field_0x0c,
//             flags_a=packed flags from r7, flags_b=packed flags from r8, p9=field_0x10
int CSchedule::Init(ScheduleEntry* entries, u8* field_0x08_ptr, u8* field_0x0c_ptr, unsigned int flags_a, unsigned int flags_b, u8* field_0x10_ptr) {
    float zero;
    float one;
    u32 zero32;

    if (entries == nullptr) return 0;
    if (field_0x08_ptr == nullptr) return 0;

    one = lbl_eu_8066B2E0;
    zero = lbl_eu_8066B2E4;
    zero32 = 0;

    this->mEntries = entries;
    this->field_0x08 = field_0x08_ptr;
    CScheduleFlags flags = {0};
    flags.b15 = 1;
    flags.b14 = 1;
    flags.b10 = flags_a;
    flags.b9 = flags_b;
    this->field_0x00 = flags;
    this->field_0x0c = field_0x0c_ptr;
    this->field_0x10 = field_0x10_ptr;
    this->field_0x14 = zero32;
    this->field_0x18 = zero32;

    this->field_0x50 = one;
    this->field_0x54 = zero;
    this->field_0x1c = zero;
    this->field_0x20 = zero;
    this->field_0x24 = zero;
    this->field_0x28 = zero;
    this->field_0x2c = zero;
    this->field_0x30 = zero;
    this->field_0x34 = one;
    this->field_0x38 = one;
    this->field_0x3c = one;
    this->field_0x4c = one;
    this->field_0x48 = one;
    this->field_0x44 = one;
    this->field_0x40 = one;

    // Copy identity matrix
    this->mMatrix = ml::CMat34::identity;

    this->field_0x90 = zero;
    this->field_0x94 = one;

    this->field_0x58 = 0x40;
    this->field_0x59 = 0;
    this->field_0x5c = zero32;

    for (int i = 0; i < 32; i++) {
        this->mHandles[i] = -1;
    }
    memset(this->mChildren, 0, sizeof(this->mChildren));

    this->mEntryCount = 0;
    this->field_0xd9 = 0;

    return this->RebuildHandles();
}

// us-804e7ab0: CSchedule::Sched_RebuildHandles()
int CSchedule::RebuildHandles() {
    this->mEntryCount = 0;
    ScheduleEntry* entry = this->mEntries;
    s16 count = 0;
    while (entry->type != 0xFFFF) {
        // No more room -> bail out success (converges on the post-loop return 1).
        if (count >= 0x20) break;
        if (entry->flags == 0 || entry->flags >= 0xFFFF) {
            return 0;
        }
        s16 handle = SchedItem_ClaimFree(this->mEntries, entry, this);
        if (handle < 0) {
            return 0;
        }
        // Write through explicit pointer arithmetic with s16 sign-extension and the
        // array offset last, so MWCC keeps the base+displacement sth (@REF:17).
        *(s16*)((u8*)this + (u32)(s16)count * 2 + 0x98) = handle;
        count++;
        entry++;
        this->mEntryCount++;
    }
    return 1;
}

#pragma once

#include "monolib/math/CMat34.hpp"
#include <types.h>

// Forward declarations for functions in other TUs
struct CSchedule;
struct CScheduleChild;

extern "C" s16 SchedItem_ClaimFree(void* param0, void* param1, void* param2);
extern "C" void SchedItem_ReleaseHandle(s16 handle);
extern "C" u32 lbl_eu_80570068[];
extern "C" float lbl_eu_8066B2E0;
extern "C" float lbl_eu_8066B2E4;

// CSchedule::field_0x00 state bits (MWCC allocates bitfields MSB-first).
struct CScheduleFlags {
    u16 b15 : 1; // 0x8000: running (cleared when the schedule retires)
    u16 b14 : 1; // 0x4000
    u16 b13 : 1; // 0x2000: has translation
    u16 b12 : 1; // 0x1000: has rotation
    u16 b11 : 1; // 0x0800: has scale
    u16 b10 : 1; // 0x0400: destroy request (Init flags_a)
    u16 b9 : 1;  // 0x0200: Init flags_b
    u16 b8 : 1;  // 0x0100: paused
    u16 : 8;
};

// Schedule entry: {u16 type, u16 flags}
struct ScheduleEntry {
    u16 type;
    u16 flags;
};

// Interface of objects registered on a CSchedule (schedAttachChildSlot /
// schedDetachChildSlot). ~CSchedule calls OnScheduleDelete(this) on every
// registered child. Implemented (as a private base, so it does not appear in
// the RTTI base lists) by cf::CREvtEffect (+0x14) and cf::CREvtModelObj (+0x3C).
struct CScheduleChild {
    virtual ~CScheduleChild() {}
    virtual void OnScheduleDelete(CSchedule* pSchedule) = 0;
};

// CSchedule manages a list of schedule entries, resolving each to a handle.
// The vtable pointer lives at offset 0xec (non-standard layout).
// Total size: 0xf0.
struct CSchedule {
    ~CSchedule();
    int Init(ScheduleEntry* entries, u8* field_0x08_ptr, u8* field_0x0c_ptr, unsigned int flags_a, unsigned int flags_b, u8* field_0x10_ptr);
    int RebuildHandles();
    void ReleaseHandles();
    void Update(f32 dt);
    void StepEntries(f32 delta);
    void FeedTargetDist();

    CScheduleFlags field_0x00; // 0x00: packed flags built from init params
    ScheduleEntry* mEntries; // 0x04: pointer to ScheduleEntry array
    u8* field_0x08;          // 0x08
    u8* field_0x0c;          // 0x0c
    u8* field_0x10;          // 0x10
    u32 field_0x14;          // 0x14
    u32 field_0x18;          // 0x18
    float field_0x1c;        // 0x1c
    float field_0x20;        // 0x20
    float field_0x24;        // 0x24
    float field_0x28;        // 0x28
    float field_0x2c;        // 0x2c
    float field_0x30;        // 0x30
    float field_0x34;        // 0x34
    float field_0x38;        // 0x38
    float field_0x3c;        // 0x3c
    float field_0x40;        // 0x40
    float field_0x44;        // 0x44
    float field_0x48;        // 0x48
    float field_0x4c;        // 0x4c
    float field_0x50;        // 0x50
    float field_0x54;        // 0x54
    u8 field_0x58;           // 0x58
    u8 field_0x59;           // 0x59
    u32 field_0x5c;          // 0x5c
    ml::CMat34 mMatrix;      // 0x60: transform matrix (identity by default)
    float field_0x90;        // 0x90
    float field_0x94;        // 0x94
    s16 mHandles[32];        // 0x98: resolved handles (initialized to -1)
    u8 mEntryCount;          // 0xd8: entry count
    s8 field_0xd9;           // 0xd9: active-item count for the last step
    CScheduleChild* mChildren[4]; // 0xdc: child objects with virtual destructors
    u8* mTablePtr;           // 0xec: pointer to vtable-like structure
};

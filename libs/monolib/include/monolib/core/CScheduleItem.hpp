#pragma once

#include <types.h>

// Forward declarations
struct CSchedule;
struct ScheduleEntry;

// Pool-managed schedule item (0x58 bytes, pool at lbl_eu_80661718, stride 0x58).
// Allocated/looked up via handles through func_804DFBF4.
struct CScheduleItem {
    u8* mEntryData;   // 0x00: pointer to entry blob (NULL when free)
    u8 mFlags;        // 0x04: bit 0x80 = entries loaded
    u8 _pad05;        // 0x05
    u16 mLifetime;    // 0x06: frame lifetime limit (0 = unlimited)
    s16 mSlots[32];   // 0x08: effect object handles
    u8 mCount;        // 0x48: number of active slots
    u8 _pad49[3];     // 0x49
    void* mOwner;     // 0x4C: owning CSchedule
    s16 mPoolIndex;   // 0x50: index in the pool (-1 = free)
    u8 _pad52[6];     // 0x52
};                  // size 0x58

// Entry blob header: {lifetime, offset} pairs resolved by func_804E3EB4.
struct CScheduleEntryData {
    u16 mLifetime; // 0x00
    u16 mOffset;   // 0x02: offset into entry blob (0 = none)
};

// One 0x16-byte entry inside an item entry blob. Terminator: mType == 0x4000.
struct CItemEntry {
    s16 mType;      // 0x00
    u8 _pad02[8];   // 0x02
    s16 mSlotIndex; // 0x0A: referenced slot index (negative = none)
    u8 _pad0C[0x0A]; // 0x0C
};                  // size 0x16

// Effect object (pool at lbl_eu_80661728, stride 0x22C), looked up via
// func_804DFEAC(handle).
struct CEffectObj {
    u8 _pad00[0x1C];
    u16 mFlags1C;   // 0x1C: bit 0x2000 = parent-linked, bit 0x8000 = update flag
    u8 _pad1E[2];   // 0x1E
    s16 mField20;   // 0x20: parent slot handle
    u8 _pad22[0xC]; // 0x22
    s16 mField2E;   // 0x2E
};

// Object with a position accessor virtual at vtable offset 0x34.
struct CSchedulePosLink {
    virtual void* vfunc00();
    virtual void* vfunc01();
    virtual void* vfunc02();
    virtual void* vfunc03();
    virtual void* vfunc04();
    virtual void* vfunc05();
    virtual void* vfunc06();
    virtual void* vfunc07();
    virtual void* vfunc08();
    virtual void* vfunc09();
    virtual void* vfunc10();
    virtual void* vfunc11();
    virtual void* vfunc12(); // 0x34: returns world position
};

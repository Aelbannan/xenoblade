#pragma once

#include <types.h>

// External data referenced by sub-object pointers at +0x74.
// Both getShortValue_A and getShortValue_B read from this.
struct CActParamDataRef {
    u8 _pad_00[0x08];  // 0x00-0x07
    s16 mShort08;       // 0x08: read in getShortValue_A
    s16 mShort0A;       // 0x0A: read in getShortValue_B
};

// Pointer-offset return for getSubObjPtr (val + 0x14).
struct CActParamDataSub {
    u8 _pad_00[0x14];  // 0x00-0x13
    void* mPtr14;       // 0x14: returned by getSubObjPtr
};

class CActParamData {
public:
    static void buildVTable();
    static void func_80057CDC();
    CActParamData();
    virtual ~CActParamData();

    // +0x00: vtable
    char _pad_04[0x260 - 0x04];     // 0x04-0x25F
    // Sub-object A at 0x260 (0x74 bytes, to 0x2D3)
    char mSubObj260[0x274 - 0x260]; // 0x260-0x273
    u32 mField274;                   // 0x274: flag (getNonNullPtr)
    char mSubObj260_tail[0x2D4 - 0x278]; // 0x278-0x2D3
    u32 mField2D4;                  // 0x2D4 (tag/ptr for SubObjA)
    void* mPtr2D8;                  // 0x2D8 (getNonNullPtr)
    u32 _pad_2DC;                   // 0x2DC
    // Sub-object B at 0x2E0 (0x14 bytes, to 0x2F3)
    char mSubObj2E0[0x2F4 - 0x2E0]; // 0x2E0-0x2F3
    void* mPtr2F4;                  // 0x2F4 (tag/ptr, getSubObjPtr)
    char _pad_2F8[0x354 - 0x2F8];   // 0x2F8-0x353
    u32 mField354;                  // 0x354 (tag/ptr for SubObjB)
    void* mPtr358;                  // 0x358 (getNonNullPtr)
};
#pragma once

#include <types.h>

// Global singleton object referenced by the CfCamEvent cam logic
// (address lbl_eu_80663DF0, accessed via sda21 as a pointer).
struct CfCamEventGlobal {
    u8  _000[0x38];
    u32 field_0x38;    // 0x38 - pointer/state word
    u8  _03C[0x3E - 0x3C];
    s16 field_0x3E;    // 0x3E
    u8  _040[0x46 - 0x40];
    u8  field_0x46;    // 0x46
};

// Sub-object embedded at manager offset 0x1F4.
// A single effect slot object referenced by the manager's 3-element slot
// array at offset 0.  Destructor is virtual (defined in another TU).
class CfCamEventSlot {
public:
    virtual ~CfCamEventSlot();
};
struct CfCamEventSub1F4 {
    u8 _000[0x04];            // 0x1F4 + 0x00
    CfCamEventElem elems[42]; // element array base at 0x1F4 + 0x04
    u8 _34C[0x356 - 0x34C];
    u8 field_0x162;    // 0x1F4 + 0x162 = 0x356
    u8 field_0x163;    // 0x1F4 + 0x163 = 0x357
    u8 _358[0x35A - 0x358];
    s16 field_0x166;   // 0x1F4 + 0x166 = 0x35A (shake table element count)
    u8 _168[0x2DA - 0x168];
    u8 field_0x2DA;    // 0x1F4 + 0x2DA = 0x4CE
    u8 field_0x2DB;    // 0x1F4 + 0x2DB = 0x4CF
};

// Single element of the manager's internal shake table (0x14 bytes each,
// element 0 base sits at manager offset 0x1F8). Only +0x04 is used for the
// shake update; the rest of the stride is padding.
struct CfCamEventElem {
    f32 x0;       // +0x00
    f32 x4;       // +0x04 - value that gets shaken toward the previous element
    u8  _08[0x14 - 0x08];
};

// Camera event manager — only forward-declared elsewhere; the complete layout
// is reconstructed here from the cam-event accessors.
class CfCamEventManager {
public:
    CfCamEventSlot* slots[3];  // 0x00 - 3 effect slot pointers
    u8  _00C[0x38 - 0x0C];
    u32 field_0x38;     // 0x38
    u8  _03C[0x3E - 0x3C];
    s16 field_0x3E;     // 0x3E - cam state id
    u8  _040[0x47 - 0x40];
    u8  field_0x47;     // 0x47
    u8  _048[0x50 - 0x48];
    u32 field_0x50;     // 0x50 - flags word
    u32 field_0x54;     // 0x54
    u32 field_0x58;     // 0x58
    u32 field_0x5C;     // 0x5C
    u32 field_0x60;     // 0x60
    u32 field_0x64;     // 0x64
    u32 field_0x68;     // 0x68
    u32 field_0x6C;     // 0x6C
    u32 field_0x70;     // 0x70
    u32 field_0x74;     // 0x74
    f32 field_0x78;     // 0x78
    u8  _07C[0x1DE - 0x7C];
    u8  field_0x1DE;    // 0x1DE
    u8  field_0x1DF;    // 0x1DF
    u8  _1E0[0x1E2 - 0x1E0];
    s16 field_0x1E2;    // 0x1E2
    u8  _1E4[0x1F4 - 0x1E4];
    CfCamEventSub1F4 sub_0x1F4;  // 0x1F4
};
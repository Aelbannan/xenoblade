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
struct CfCamEventSub1F4 {
    u8 _000[0x162];
    u8 field_0x162;    // 0x1F4 + 0x162 = 0x356
    u8 _163[0x2DA - 0x163];
    u8 field_0x2DA;    // 0x1F4 + 0x2DA = 0x4CE
};

// Camera event manager — only forward-declared elsewhere; the complete layout
// is reconstructed here from the cam-event accessors.
class CfCamEventManager {
public:
    u8  _000[0x54];
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
    u8  _1DF[0x1F4 - 0x1DF];
    CfCamEventSub1F4 sub_0x1F4;  // 0x1F4
};
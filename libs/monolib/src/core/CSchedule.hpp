#pragma once

#include "monolib/math/CMat34.hpp"
#include <types.h>

// Forward declarations for functions in other TUs
extern "C" s16 func_804DFA84(void* param0, void* param1, void* param2);
extern "C" void func_804DFB88(s16 handle);
extern "C" void* lbl_eu_80570068;
extern "C" float lbl_eu_8066B2E0;
extern "C" float lbl_eu_8066B2E4;

// Schedule entry: {u16 type, u16 flags}
struct ScheduleEntry {
    u16 type;
    u16 flags;
};

// CSchedule manages a list of schedule entries, resolving each to a handle.
// The vtable pointer lives at offset 0xec (non-standard layout).
// Total size: 0xf0.
struct CSchedule {
    s16 field_0x00;          // 0x00: packed flags built from init params
    void* field_0x04;        // 0x04: pointer to ScheduleEntry array
    void* field_0x08;        // 0x08
    void* field_0x0c;        // 0x0c
    void* field_0x10;        // 0x10
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
    s16 field_0x98[32];      // 0x98: resolved handles (initialized to -1)
    u8 field_0xd8;           // 0xd8: entry count
    u8 field_0xd9;           // 0xd9
    void* field_0xdc[4];     // 0xdc: child objects with virtual destructors
    void* mTablePtr;         // 0xec: pointer to vtable-like structure
};

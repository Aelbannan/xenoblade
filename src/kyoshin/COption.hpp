#pragma once

#include <types.h>

class CCur19 {
public:
    CCur19();

    // TODO: add fields
};

// Full object layout for COption (used by C-linkage accessors)
struct COptionFull {
    u8 _00[0x18];
    u32 field_0x18;
    u8 _1C[0x28 - 0x1C];
    u8 field_0x28;
    u8 field_0x29;
    u8 field_0x2A;
    u8 field_2B;
    u8 _2C[0x30 - 0x2C];
    u8 field_30;
    u8 _31;
    u8 field_32;
    u8 _33[0x78 - 0x33];
    // CBaseCur subobject at 0x78 (0x16 bytes)
    u8 cur1[0x16];
    u8 _8E[0xA8 - 0x8E];
    // CSysWin subobject at 0xA8 (0x3C bytes)
    u8 syswin[0x3C];
    // CBaseCur subobject at 0xE4 (0x16 bytes)
    u8 cur2[0x16];
};

class COption {
public:
    COption();
    virtual ~COption();
    void OnFileEvent();

    // vtable at 0x00 (implicit)
    u8 _04[0x18 - 0x04];
    u32 field_0x18;              // 0x18 - checked for non-zero; layout pointer or ready flag
    u8 _1C[0x28 - 0x1C];
    u8 field_0x28;               // 0x28
    u8 field_0x29;               // 0x29 - menu state/phase
    u8 field_0x2A;               // 0x2A
    u8 field_0x2B;               // 0x2B
    u8 _2C[0x30 - 0x2C];
    u8 field_0x30;               // 0x30
    u8 _31;                      // 0x31
    u8 field_0x32;               // 0x32
    u8 _33[0x78 - 0x33];
    // CBaseCur subobject at 0x78 (0x16 bytes)
    u8 cur1[0x16];
    u8 _8E[0xA8 - 0x8E];
    // CSysWin subobject at 0xA8 (0x3C bytes)
    u8 syswin[0x3C];
    // CBaseCur subobject at 0xE4 (0x16 bytes)
    u8 cur2[0x16];
};


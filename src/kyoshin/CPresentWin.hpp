#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// Opaque object at CPresentWin+0x14; has a float scale factor at offset 0x44
struct CPresentWinUnk14 {
    u8 _pad[0x44];
    float mScale;
};

class CPresentWin {
public:
    CPresentWin();
    virtual ~CPresentWin();

    // +0x00: vtable
    u8* mpField04;               // 0x04
    nw4r::lyt::Layout* mpLayout;  // 0x08
    u8* mpField0C;               // 0x0C
    u8* mpField10;               // 0x10
    CPresentWinUnk14* mpField14;  // 0x14 - ptr to object with float at +0x44
    u8* mpField18;               // 0x18
    u8* mpField1C;               // 0x1C
    u8* mpField20;               // 0x20
    u8* mpField24;               // 0x24
    u8* mpField28;               // 0x28
    u8* mpField2C;               // 0x2C
    u8 mField30;                 // 0x30
    u8 mField31;                 // 0x31
    u8 mField32;                 // 0x32
    u8 mField33;                 // 0x33
    u16 mField34;                // 0x34
    u8 mField36;                 // 0x36
    u8 mField37;                 // 0x37
    u8 mField38;                 // 0x38
    u8 mDataArray[8];            // 0x39-0x40 (indexed by func_8022E868)
    u8 mDataCount;               // 0x41
};


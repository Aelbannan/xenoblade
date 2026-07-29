#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CPresentWin {
public:
    CPresentWin();
    virtual ~CPresentWin();

    // +0x00: vtable
    u8 _pad_04[0x08 - 0x04];       // 0x04-0x07
    nw4r::lyt::Layout* mpLayout;    // 0x08
    nw4r::lyt::AnimTransform* mpAnimTransform; // 0x0C
    u8 _pad_10[0x30 - 0x10];       // 0x10-0x2F
    u8 mField30;               // 0x30
    u8 mField31;               // 0x31
    u8 mField32;               // 0x32
    u8 mField33;               // 0x33
    u16 mField34;              // 0x34
    u8 mField36;               // 0x36
    u8 mField37;               // 0x37
    u8 mField38;               // 0x38
    u8 mDataArray[8];          // 0x39-0x40 (indexed by func_8022E868)
    u8 mDataCount;             // 0x41
};


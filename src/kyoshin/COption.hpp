#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CBaseCur;

class CCur19 {
public:
    CCur19();
    virtual ~CCur19();

    // TODO: add fields
};

// Full object layout for COption (used by C-linkage accessors)
struct COptionFull {
    u8 _00[0x20];
    nw4r::lyt::AnimTransform* field_0x20;
    u8 _24[0x29 - 0x24];
    u8 field_0x29;
    u8 field_0x2A;
    u8 field_2B;
    u8 _2C[0x30 - 0x2C];
    u8 field_30;
    u8 _31;
    u8 field_32;
    u8 _33[0x38 - 0x33];
    u8 mScrollBar[0x40];
    u8 _78[0xA8 - 0x78];
    u8 mSysWin[0x3C];
};

class COption {
public:
    COption();
    virtual ~COption();
    void OnFileEvent();

    // TODO: add fields
};


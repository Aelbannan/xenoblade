#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"

// Vtable symbol for CCur19 (6 entries, .data)
extern "C" void* lbl_eu_805396D0[];

class CCur19 : public CBaseCur {
public:
    CCur19(nw4r::lyt::ArcResourceAccessor* arcResAcc);

    // CBaseCur: 0x00-0x15
    u8 _16[0x20 - 0x16];                         // +0x16
    nw4r::lyt::AnimTransform* mpAnimTrans2;      // +0x20
    nw4r::lyt::AnimTransform* mpAnimTrans3;      // +0x24
    u8 _28;                                       // +0x28
    u8 mState;                                    // +0x29
};

// Full object layout for COption (used by C-linkage accessors)
struct COptionFull {
    u8 _00[0x2B];
    u8 field_2B;
    u8 _2C[0x30 - 0x2C];
    u8 field_30;
    u8 _31;
    u8 field_32;
};

class COption {
public:
    COption();
    virtual ~COption();
    void OnFileEvent();

    // TODO: add fields
};


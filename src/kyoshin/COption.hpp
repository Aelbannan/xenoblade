#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "kyoshin/CBaseCur.hpp"

class CCur19 {
public:
    CCur19();

    // TODO: add fields
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
    void func_8029D10C();
    void func_8029C03C();
    void func_8029C0C4();

    // Fields start after implicit vtable pointer (offset 0x04)
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // 0x04
    nw4r::lyt::Layout* mpLayout;                  // 0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;       // 0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;       // 0x10
    u8 mActive;                                    // 0x14
    u8 mVisible;                                   // 0x15
    u8 _16[0x24 - 0x16];                          // 0x16-0x23
    nw4r::lyt::AnimTransform* mAnimTransform24;   // 0x24
    u8 _28;
    u8 field_0x29;                                 // 0x29
    u8 _2A;
    u8 field_0x2B;                                 // 0x2B
    u8 _2C[0x30 - 0x2C];                          // 0x2C-0x2F
    u8 field_0x30;                                 // 0x30
    u8 _31;
    u8 field_0x32;                                 // 0x32
    u8 _33[0x78 - 0x33];                          // 0x33-0x77
    CBaseCur mSubCur1;                             // 0x78 (size 0x18)
    CBaseCur mSubCur2;                             // 0x90 (size 0x18)
};


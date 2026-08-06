#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"

// Cursor widget class (embedded on top of CBaseCur base at +0x00).
// Retail symbol names are the plain `__ct__CCur19` / `__dt__8029BF18` forms.
class CCur19 : public CBaseCur {
public:
    CCur19();
    ~CCur19();

    // 0x00 CBaseCur base (16 bytes)
    // 0x16 ... derived fields
};

// Object used by func_8029C734: embedded CScrollBar at +0x38 and CSysWin at
// +0xA8 with a visibility/state byte at +0x2A.
struct COptionWindow {
    u8 _00[0x2A];                 // 0x00-0x29
    u8 field_0x2A;                // 0x2A
    u8 _2B[0x38 - 0x2B];          // 0x2B-0x37
    CScrollBar mScrollBar;        // 0x38 (0x40 bytes)
    u8 _78[0xA8 - 0x78];          // 0x78-0xA7
    CSysWin mSysWin;              // 0xA8
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

// Option sub-object referenced at +0x1C. Virtual dispatch used by the
// func_8029E144 / func_8029E1CC animation-configurators (vtable offsets
// 0x1C/0x24/0x2C/0x38).
class COptionSub {
public:
    virtual ~COptionSub();
    virtual void v0() = 0;                        // 0x08
    virtual void v1() = 0;                        // 0x0C
    virtual void v2() = 0;                        // 0x10
    virtual void v3() = 0;                        // 0x14
    virtual void v4() = 0;                        // 0x18
    virtual void v5(nw4r::lyt::AnimTransform*) = 0; // 0x1C
    virtual void v6() = 0;                        // 0x20
    virtual void v7() = 0;                        // 0x24
    virtual void v8() = 0;                        // 0x28
    virtual void v9(nw4r::lyt::AnimTransform*, int) = 0; // 0x2C
    virtual void v10() = 0;                       // 0x30
    virtual void v11() = 0;                       // 0x34
    virtual void v12(int) = 0;                    // 0x38
};

class COption {
public:
    COption();
    virtual ~COption();
    void OnFileEvent();

    // Fields start after implicit vtable pointer (offset 0x04)
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // 0x04
    nw4r::lyt::Layout* mpLayout;                  // 0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;       // 0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;       // 0x10
    u8 mActive;                                    // 0x14
    u8 mVisible;                                   // 0x15
    u8 _16[0x1C - 0x16];                          // 0x16-0x1B
    COptionSub* mSubObj;                           // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransform20;   // 0x20
    nw4r::lyt::AnimTransform* mAnimTransform24;   // 0x24
    u8 field_0x28;                                 // 0x28
    u8 field_0x29;                                 // 0x29
    u8 field_0x2A;                                 // 0x2A
    u8 field_0x2B;                                 // 0x2B
    u8 field_0x2C;                                 // 0x2C
    u8 field_0x2D;                                 // 0x2D
    u8 field_0x2E;                                 // 0x2E
    u8 field_0x2F;                                 // 0x2F
    u8 field_0x30;                                 // 0x30
    u8 field_0x31;                                 // 0x31
    u8 field_0x32;                                 // 0x32
    u8 _33[0x38 - 0x33];                          // 0x33-0x37
    CScrollBar mScrollBar;                         // 0x38 (size 0x40)
    CBaseCur mSubCur1;                             // 0x78 (size 0x18)
    CBaseCur mSubCur2;                             // 0x90 (size 0x18)
    u8 mSysWin[0x3C];                              // 0xA8 CSysWin region (size 0x3C)
    CBaseCur mSubCur3;                             // 0xE4 (size 0x18)
};


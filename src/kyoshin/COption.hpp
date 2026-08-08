#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "kyoshin/code_80296898.hpp"

class CEventFile;

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
// 0x1C/0x24/0x2C/0x38 = v5/v7/v9/v12 after MWCC's 2 implicit vtable entries).
class COptionSub {
public:
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

// OnFileEvent loads the option layout into the early region of the object.
// The true retail early layout (recovered from the load sequence) differs from
// the approximate COption members above, so OnFileEvent accesses those offsets
// through this overlay. mFHandle is the file this panel was last loaded from.
struct COptionMem {
    void* mVtbl;                            // 0x00
    UnkClass_8045F564 mRegion;              // 0x04
    CFileHandle* mFHandle;                  // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcAcc; // 0x18
    nw4r::lyt::Layout* mpLayout;            // 0x1C
    nw4r::lyt::AnimTransform* mAnim0;       // 0x20
    nw4r::lyt::AnimTransform* mAnim1;       // 0x24
};

class COption {
public:
    COption();
    ~COption();
    bool OnFileEvent(CEventFile* pEventFile);

    // Explicit vtable pointer (CBaseCur-style convention, NOT C++ `virtual`, so
    // MWCC does not inject a vptr-reset into the destructor).
    void* mVtable;                                // +0x00
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
    s8 field_0x2D;                                  // 0x2D (signed byte in retail: extsb'd)
    u8 field_0x2E;                                 // 0x2E
    u8 field_0x2F;                                 // 0x2F
    u8 field_0x30;                                 // 0x30
    u8 field_0x31;                                 // 0x31
    u8 field_0x32;                                 // 0x32
    u8 field_0x33;                                 // 0x33
    u8 _34[0x38 - 0x34];                          // 0x34-0x37
    u8 mScrollBar[0x40];                           // 0x38 CScrollBar storage (raw; destroyed via retail __dt__10CScrollBarFv)
    CBaseCur mSubCur1;                             // 0x78 (size 0x18)
    CBaseCur mSubCur2;                             // 0x90 (size 0x18)
    u8 mSysWin[0x3C];                              // 0xA8 CSysWin region (size 0x3C)
    CBaseCur mSubCur3;                             // 0xE4 (size 0x18)
    u32 field_0xFC;                       // 0xFC (advance/state word set by handlers)
    u32 field_0x100;                      // 0x100
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __ct__CCur18(void* self, void* accessor);
extern "C" void func_8018B0FC(void* dest, void* src);
extern "C" void* func_801355F4();
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);
extern "C" void* lbl_eu_805396D0[];
extern "C" Class_80296898 lbl_eu_80577308;
extern "C" u8 lbl_eu_8053948C[0x30];
extern "C" void __ct__8CBaseCurFv(CBaseCur*);
extern "C" void __dt__8CBaseCurFv(void*, int);
extern "C" void* __dt__6CCur18Fv(void*, int);
extern "C" void* __dt__7CSysWinFv(void*, int);
extern "C" void* __dt__10CScrollBarFv(void*, int);
extern "C" void* __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" int CScrollBar_isVisible(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CSysWin_isActive(u8*);
extern "C" u32 CSysWin_getUnk34(u8*);
extern "C" void func_801F369C(void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F367C(void*);
extern "C" void func_801F3850(void*, u16);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801F3540(void*);
extern "C" void func_801F35DC(void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_8022B7C8(void*, void*);
extern "C" void func_801D216C(void*, u8);
extern "C" void func_801D2174(CBaseCur*);
extern "C" void func_801D21CC(void*);
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
extern "C" void func_8029E254(COption*);
extern "C" void func_8029E1CC(COption*);
extern "C" void func_8029E144(COption*);
extern "C" void func_8029D420(COption*);
extern "C" u8 func_8029D634(COption*, u8);
extern "C" u8 func_8029D7E8(COption* self);
extern "C" void func_8022B8E4(void*);
extern "C" void func_80296A04__FP14Class_80296898(void*);


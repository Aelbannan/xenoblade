#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "kyoshin/code_80296898.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CWorkSystemPack.hpp"

class CEventFile;
class CfEnumObject;
namespace cf { class CfObject; }

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

// Abstract view into the embedded CCur18 cursor vtable (MWCC prepends 2
// entries, so the second declared virtual lands at vtable +0x0C - the cursor
// teardown virtual invoked by func_8029C66C). Same scheme as CSysWinSave.hpp.
class CCursor18 {
public:
    virtual void vf2(int) = 0;  // slot 2 (+0x08)
    virtual void vf3() = 0;     // slot 3 (+0x0C)
};

// Vtable view for the cursor teardown virtual at +0x08 (slot 2 after the
// 2-entry RTTI prefix) invoked by OnFileEvent after copying each cursor temp.
// Declared as a no-arg virtual so MWCC dispatches through r12 like retail.
class COptionCurTearView {
public:
    virtual void vf0() = 0;  // +0x08 - cursor teardown
};

// Full cursor vtable view for func_8029E254: vf04 is declared index 2, which
// lands at vtable +0x10 after MWCC's 2-entry RTTI prefix - the "apply window
// data" virtual dispatched on the sub-cursors. Same scheme as CCur18View.
class COptionCurView {
public:
    virtual void vf02() = 0;      // index 0 -> +0x08
    virtual void vf03(int) = 0;   // index 1 -> +0x0C (unused here)
    virtual void vf04(u8*) = 0;   // index 2 -> +0x10
};

// Vtable view for the +0x1C sub-object's message source (the pointer at
// +0x10 of the sub-object): v0D is declared index 13, which lands at vtable
// +0x3C (slot 15) - the "resolve message text to a pane" virtual used by
// func_8029E254.
class COptionMsgObj {
public:
    virtual void v0() = 0;                             // +0x08
    virtual void v1() = 0;                             // +0x0C
    virtual void v2() = 0;                             // +0x10
    virtual void v3() = 0;                             // +0x14
    virtual void v4() = 0;                             // +0x18
    virtual void v5() = 0;                             // +0x1C
    virtual void v6() = 0;                             // +0x20
    virtual void v7() = 0;                             // +0x24
    virtual void v8() = 0;                             // +0x28
    virtual void v9() = 0;                             // +0x2C
    virtual void v0A() = 0;                            // +0x30
    virtual void v0B() = 0;                            // +0x34
    virtual void v0C() = 0;                            // +0x38
    virtual nw4r::lyt::Pane* v0D(const char*, int) = 0; // +0x3C
};

// Data overlay for the +0x1C sub-object: +0x10 is the message source pointer.
struct COptionSubData {
    u8 _00[0x10];              // 0x00-0x0F
    COptionMsgObj* mMsgObj;    // 0x10
};

// CSysWin vtable view exposing the layout-build virtual at +0x88 (slot 34 =
// declared index 32 after the RTTI prefix) invoked by func_8029C35C.
class COptionSysWinView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual void v0D() = 0;
    virtual void v0E() = 0;
    virtual void v0F() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v1A() = 0;
    virtual void v1B() = 0;
    virtual void v1C() = 0;
    virtual void v1D() = 0;
    virtual void v1E() = 0;
    virtual void v1F() = 0;
    virtual void v20() = 0;  // index 32 -> +0x88 - layout-build virtual
};

// View into the object returned by CDeviceFont::getFontInfo: vtable+0x24
// (slot 9, no args) yields the u32 pushed onto the root pane by func_8013676C.
// All-pure so no vtable is emitted.
class COptionFontView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
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
    u8 _16[2];                                     // 0x16-0x17
    void* field_18;                                // 0x18 (null-checked in func_8029D3C0)
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
    u8 field_0x34;                                 // 0x34 (constructor arg: window kind)
    u8 _35[0x38 - 0x35];                          // 0x35-0x37
    u8 mScrollBar[0x40];                           // 0x38 CScrollBar storage (raw; destroyed via retail __dt__10CScrollBarFv)
    CBaseCur mSubCur1;                             // 0x78 (size 0x18)
    CBaseCur mSubCur2;                             // 0x90 (size 0x18)
    u8 mSysWin[0x3C];                              // 0xA8 CSysWin region (size 0x3C)
    CBaseCur mSubCur3;                             // 0xE4 (size 0x18)
    s32 field_0xFC;                       // 0xFC (advance/state word set by handlers; signed compares in retail)
    u32 field_0x100;                      // 0x100
};


// Global data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80663E28;   // .sbss mode flag; bit 0x01000000 gates the config re-sync lists
// .sbss last-synced config byte (written by func_8029D278)
extern u8 lbl_eu_80664A40;
// .data option text tables (size 0x154): u16 enable table at +0x00, u16 name
// table at +0x2C, 12-byte-row u16 value table at +0x58, u8 label table at
// +0x16C - read by func_8029D420.
extern u8 lbl_eu_80539338[];

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __ct__CCur18(void* self, void* accessor);
extern "C" void func_8018B0FC(void* dest, void* src);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);
extern "C" void* lbl_eu_805396D0[];
extern void* lbl_eu_80539638[];             // COption vtable (stored at +0x00 by __ct__COption)

// Sub-object constructors (retail C-linkage names; C linkage keeps the
// __-prefixed names unmangled, so call sites bind to the retail relocs).
extern "C" void __ct__CScrollBar(CScrollBar* self, int arg);
extern "C" void __ct__CSysWin(CSysWin* self, int arg);
extern "C" Class_80296898 lbl_eu_80577308;
extern "C" u8 lbl_eu_8053948C[0x30];
// updateConfig retail symbol (1-arg mangling); func_8029D278 calls it with a
// dead second argument (retail emits li r4,1), so declare the literal
// mangled identifier under C linkage with both args.
extern "C" void updateConfig__FPUc(u8* src, int mode);
// Voice/enum-object list iteration helpers (retail C-linkage wrappers in
// CfGameManager.cpp that leave the inner getter's result in r3).
extern "C" CfEnumObject* triggerVoiceDown();
extern "C" VoiceSource* getVoiceSourcePtr();
extern "C" CfEnumObject* resetVoiceSystem(CfEnumObject* obj);
extern "C" VoiceSource* getVoiceSourceForSystem(VoiceSource* source);
extern "C" void func_800BF2F8(cf::CfObject* obj);
extern "C" void func_801F34F4(CScrollBar* self);
extern "C" void func_8022C1B4(u8* out, u8* syswin, u8 sel);
extern "C" void __ct__UnkClass_8011C974(u8* dst, u8* src);
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, IWorkEvent*, int, int);
extern "C" void __ct__8CBaseCurFv(CBaseCur*);
extern "C" void __dt__8CBaseCurFv(void*, int);
extern "C" void* __dt__6CCur18Fv(void*, int);
extern "C" void* __dt__7CSysWinFv(void*, int);
extern "C" void* __dt__10CScrollBarFv(CScrollBar*, int);
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
extern "C" void func_8022B7F4(void*);
extern "C" void func_801D216C(void*, u8);
extern "C" void func_801D2174(CBaseCur*);
extern "C" void func_801D21CC(void*);
extern "C" void func_8029E254(COption*);
extern "C" int func_8029E3F8(COption*);
extern "C" void func_8029E1CC(COption*);
extern "C" void func_8029E144(COption*);
extern "C" void func_8029D420(COption*);
extern "C" u8 func_8029D634(COption*, u8);
extern "C" u8 func_8029D7E8(COption* self);
extern "C" void func_8029DD6C(COption* self);
extern "C" void func_8022B8E4(void*);
extern "C" void func_8022B748(void*);
extern "C" void func_8022B9B4(void*, void*, void*);
extern "C" void func_8022BF6C(void*, void*, void*);
extern "C" void func_8022B8B8(void*);
extern "C" void func_801D202C(void*);
extern "C" char* func_80136190(const void*, const void*, int);
extern "C" void initInstance__FP14Class_80296898(void*);


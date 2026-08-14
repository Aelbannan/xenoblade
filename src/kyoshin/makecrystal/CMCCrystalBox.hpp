#pragma once

#include <types.h>
class CBaseCur;

namespace nw4r { namespace lyt { class AnimTransform; class Layout; class Pane; } }

// Pane +0x2C is the protected mTranslate VEC3 (lyt_pane.h); mirror it so the
// crystal-box refresh (func_802180B4) can hand the field to copyVEC3.
struct PaneTranslateMirror {
    f32 mX;   // +0x2C
    f32 mY;   // +0x30
    f32 mZ;   // +0x34
};

// SDA21 float constants referenced by the anim-completion callbacks (retail
// symbol names; the sentinel is the completion-check value passed to
// func_80137444 / func_80137510).
extern const float lbl_eu_80668470;  // anim sentinel constant used by completion checks
extern float lbl_eu_8066845C;
extern float lbl_eu_80668474;
extern float lbl_eu_80668478;

// BDAT table pointer (sda21) and sdata2 constants used by the crystal-box
// refresh callbacks (func_80217098 / func_802177D0 / func_802180B4).
extern char* lbl_eu_806640EC;     // BDAT table pointer (sda21)
extern double lbl_eu_80668460;    // 2^52 double conversion constant (unsigned path)
extern double lbl_eu_80668468;    // 2^52 + 2^31 double conversion constant (signed path)
extern float lbl_eu_8066847C;

// 8-byte crystal box param: two s16 counters (selected id, quantity), a
// spare s16, and two flag bytes. Inlined low-level value type for the
// 32-slot sub-tables at CMCCrystalBox+0x2D8 and the transient sort table.
struct CMCCrystalBoxParam {
    s16 m0;   // +0x00 selected id
    s16 m2;   // +0x02 quantity
    s16 m4;   // +0x04 spare
    u8  m6;   // +0x06 flag
    u8  m7;   // +0x07 flag
};

// TU-local crystal-box param helpers. Retail symbols are unmangled C names
// (per retail relocs); declared with C linkage so call sites emit the retail
// reloc names. func_80213E04 returns its first argument (callers reuse it as
// the copy source for func_80213E20).
extern "C" CMCCrystalBoxParam* func_80213E04(CMCCrystalBoxParam*, s16, s16);
extern "C" void func_80213E20(CMCCrystalBoxParam*, const CMCCrystalBoxParam*);

// Entry in the crystal-state table rooted at CMCCrystalBox+0x3D8.
struct CMCCrystalDataEntry {
    s16  id;     // +0x00
    u8   flag;   // +0x02
    u8   pad3;   // +0x03
};

// Mirror of the CScrollBar layout (0x40 bytes) for the copy-init in
// func_80213FE4 (func_8011C998); the real CScrollBar.hpp cannot be included
// here (conflicting ctor stubs). Layout identical to CSortMenu.hpp's mirror.
struct CScrollBarData {
    void* mVtbl;         // +0x00
    u32 mMemRegion[4];   // +0x04
    void* mFileHandle;   // +0x14
    void* mAccessor;     // +0x18
    void* mLayout;       // +0x1C
    void* mAnimTrans;    // +0x20
    u8 mReady;           // +0x24
    u8 mVisible;         // +0x25
    u8 mState;           // +0x26
    u8 mActive;          // +0x27
    f32 mAnimOffset;     // +0x28
    f32 mScrollPosY;     // +0x2C
    f32 mScrollRatio;    // +0x30
    f32 mThumbHeight;    // +0x34
    f32 mContentHeight;  // +0x38
    u8 mDirection;       // +0x3C
};

// Sort-menu sub-object at CMCCrystalBox+0xFC (implemented in CSortMenu.cpp).
// Full layout mirror of CSortMenu.hpp's class (0xF0 bytes); the temp
// copy-init in func_80213FE4 copies every member except the +0x6C array.
struct CSortMenu {
    u32 mVtable;             // +0x00
    u8  mUnk04[0x10];        // +0x04 (UnkClass_8045F564)
    void* mFileHandle;       // +0x14
    void* mArcResAcc;        // +0x18
    void* mpLayout;          // +0x1C
    void* mpAnimTrans0;      // +0x20
    void* mpAnimTrans1;      // +0x24
    u8 field_0x28;           // +0x28 (active flag)
    u8 field_0x29;           // +0x29 (layout loaded flag)
    u8 field_0x2A;           // +0x2A (state)
    u8 field_0x2B;           // +0x2B (button flag)
    u8 mScrollBar[0x40];     // +0x2C (CScrollBar, copied via func_8011C998)
    s32 mArray[32];          // +0x6C (0x80 bytes; NOT copied by the ctor)
    u8 mCount;               // +0xEC
    u8 mPage;                // +0xED
    u8 mSubPage;             // +0xEE
};

// Crystal-info sub-object at CMCCrystalBox+0x1EC (implemented in
// CMCCrystalInfo.cpp). Layout mirror of CMCCrystalInfo.hpp's class; the tail
// words at +0x52..+0x61 are copied word-wise by func_80213FE4's copy-init.
struct CMCCrystalInfo {
    u32 mVtable;             // +0x00
    u8  mMemRegion1[0x10];   // +0x04 (UnkClass_8045F564)
    u8  mMemRegion2[0x10];   // +0x14 (UnkClass_8045F564)
    u32 mFileHandle1;        // +0x24
    u32 mFileHandle2;        // +0x28
    void* mArcResAccessor;   // +0x2C
    u32 mField30;            // +0x30
    void* mLayout;           // +0x34
    void* mAnimTransform1;   // +0x38
    void* mAnimTransform2;   // +0x3C
    void* mAnimTransform3;   // +0x40
    void* mAnimTransform4;   // +0x44
    u8 mEnabled;             // +0x48
    s32 mState;              // +0x4C
    u8 mIsActive;            // +0x50
    u8 mField51;             // +0x51
    u8 field52[4];           // +0x52 (unaligned tail words, copied word-wise)
    u8 field56[4];           // +0x56
    u8 field5A[4];           // +0x5A
    u8 field5E[4];           // +0x5E
};

// Mirror of the CSysWin body at CMCCrystalBox+0x290 (opaque region). Used by
// func_80213FE4's copy-init from a stack temp. Layout matches CSysWin.hpp.
struct CSysWinData {
    void* mVtbl;             // +0x00
    u8 mMemRegion[0x10];     // +0x04 (UnkClass_8045F564)
    void* mFileHandle;       // +0x14
    void* mTagProcessor;     // +0x18
    void* mArcAccessor;      // +0x1C
    void* mLayout;           // +0x20
    void* mAnimTrans;        // +0x24
    u8 field_28;             // +0x28 (ready flag)
    u8 _29[3];               // +0x29
    u32 field_2C;            // +0x2C
    u32 field_30;            // +0x30
    u8 field_34;             // +0x34 (open flag)
    u8 field_35;             // +0x35
    u8 field_36;             // +0x36
    u8 field_37;             // +0x37
    u8 field_38;             // +0x38
    u8 field_39;             // +0x39
};

// Crystal-state table rooted at CMCCrystalBox+0x3D8: 1024 entries (4 bytes
// each), then a small state header, then the name buffer for sprintf.
struct CMCCrystalData {
    CMCCrystalDataEntry entries[0x400]; // +0x000
    u16  count;                         // +0x1000
    u8   byte_1002;                     // +0x1002 (category)
    u8   limit;                         // +0x1003
    s8   current;                       // +0x1004
    char name[0x10C8 - 0x1005];         // +0x1005 (buffer passed to sprintf)
};

// novtable: retail's dtor emits NO class-vtable store (the retail vtable is
// a .data object in another unit); without it MWCC emits the lis/addi/stw
// __vt__13CMCCrystalBox reset (+3 insns) before the dtor body.
class __declspec(novtable) CMCCrystalBox {
public:
    CMCCrystalBox(u8 parentType);
    virtual ~CMCCrystalBox();
    void OnFileEvent();

    // Fields - vtable at +0x00 (implicit), user fields start at +0x04
    u8 pad_00[0x1C];                    // +0x04 to +0x1F
    u8 unk20;                           // +0x20: entry count (0 = no entries)
    u8 pad_21[0x24 - 0x21];
    nw4r::lyt::AnimTransform* subObjPtrs[0x0F]; // +0x24 to +0x5F: array of 15 sub-object pointers
    u8 unk60;                           // +0x60
    u8 pad_61_pad_63[3];
    s32 unk64;                          // +0x64: state/status
    u8 unk68;                           // +0x68
    u8 unk69;                           // +0x69: active flag
    u8 pad_6A[0x6C - 0x6A];
    // +0x6C: CCur07
    // +0x84: CCur09
    // +0x9C: CCur18
    // +0xB4: CCur11
    // +0xCC: CCur16
    // +0xE4: CCur18
    // +0xFC: CSortMenu
    // +0x1EC: CMCCrystalInfo
    // +0x250: CScrollBar
    // +0x290: CSysWin
    u8 pad_6C[0xFC - 0x6C];
    CSortMenu sortMenu;                 // +0xFC
    CMCCrystalInfo crystalInfo;         // +0x1EC
    u8 pad_250[0x290 - 0x250];       // +0x250 to +0x28F (CScrollBar region)
    u8 sysWin[0x2CC - 0x290];        // +0x290 to +0x2CB (opaque CSysWin)
    u8 unk2CC;                          // +0x2CC
    s8 unk2CD;                          // +0x2CD
    u8 unk2CE;                          // +0x2CE
    u8 unk2CF;                          // +0x2CF
    u8 unk2D0;                          // +0x2D0: parent type
    u8 unk2D1;                          // +0x2D1
    u8 unk2D2;                          // +0x2D2
    u8 unk2D3;                          // +0x2D3
    u8 unk2D4;                          // +0x2D4: read by getByte_2D4_8021624C
    u8 unk2D5;                          // +0x2D5: read by getByte_2D5_80216254
    u8 unk2D6;                          // +0x2D6: selected-crystal table cursor (written/returned by func_8021625C)
    u8 unk2D7;                          // +0x2D7: current item id byte
    CMCCrystalBoxParam subTable[32];    // +0x2D8 to +0x3D8: selected-crystal table (32 slots)
    CMCCrystalData data;                // +0x3D8: crystal-state table (ends at +0x14A0)
    u8 field_14A0;                      // +0x14A0
    u8 pad_14A1[0x14AA - 0x14A1];       // +0x14A1..0x14A9
    u16 field_14AA[0x20];               // +0x14AA..0x14E9 (u16 page-slot table)
    u8 field_14EA;                      // +0x14EA (page-slot count)
    u8 field_14EB;                      // +0x14EB
    u8 field_14EC[5];                   // +0x14EC..0x14F0 (indexed by field_14F2)
    u8 field_14F1;                      // +0x14F1
    u8 field_14F2;                      // +0x14F2: countdown ticker / index
    u8 pad_14F3[0x14F4 - 0x14F3];       // +0x14F3
    f32 field_14F4;                     // +0x14F4
    f32 field_14F8;                     // +0x14F8
    f32 field_14FC;                     // +0x14FC
    u8 unk1500;                         // +0x1500
    u8 unk1501;                         // +0x1501
    u8 unk1502;                         // +0x1502
    u8 unk1503;                         // +0x1503
    u16 unk1504;                        // +0x1504
    u8 field_1506;                      // +0x1506
    u8 field_1507;                      // +0x1507
};

// Dispatch-only view of the item-implementation vtable (name getter at slot
// 0x20). Never instantiated; pure virtuals keep the vtable unemitted. Lets
// func_802138B8 dispatch through r12 like retail instead of a cast call.
class CItemImplDispatch {
public:
    // MWCC puts two RTTI slots at vtable 0x00/0x04, so the first user
    // virtual is at 0x08. Six fillers + getName = slot 0x20 (retail name
    // getter).
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual char* getName(void* slot) = 0;   // vtable slot 0x20
};

// Fake-SI dispatch views for the sub-object vtables (never instantiated).
// Real virtual calls make MWCC emit the retail lwz r12,off; addi r3; lwz
// r12,slot; mtctr; bctrl sequence; cast-calls through ((void***)p)[n] emit an
// extra scratch load. CCurVf0C targets vtable slot 0x0C (second user virtual,
// after the two RTTI slots + one filler), CLytVf08 targets slot 0x08.
class CCurVf0C {
public:
    virtual void vf_00() = 0;   // slot 0x08
    virtual void vf_04() = 0;   // slot 0x0C
};
class CLytVf08 {
public:
    virtual void vf_00(int) = 0;   // slot 0x08
};
class CCurVf10 {   // virtual at vtable slot 0x10 (user slot 2)
public:
    virtual void vf_00() = 0;   // slot 0x08
    virtual void vf_04() = 0;   // slot 0x0C
    virtual void vf_08(void*) = 0;   // slot 0x10
};

// Item-implementation vtable view up to slot 0x64 (GetName/GetFlag) for the
// selection loop in func_8021625C. Never instantiated; pure virtuals keep the
// vtable unemitted. Slot 0x4C = GetName (u16), slot 0x64 = GetFlag (u8).
class CItemImplFacade2 {
public:
    virtual u8 GetCount(void* item) = 0;      // slot 0x08
    virtual void vf_04() = 0;                 // 0x0C
    virtual void vf_08(void* item) = 0;       // 0x10
    virtual void vf_0C() = 0; virtual void vf_10() = 0;
    virtual void vf_14() = 0; virtual void vf_18() = 0;
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual void vf_24() = 0; virtual void vf_28() = 0;
    virtual void vf_2C() = 0; virtual void vf_30() = 0;
    virtual void vf_34() = 0; virtual void vf_38() = 0;
    virtual void vf_3C() = 0; virtual void vf_40() = 0;
    virtual void vf_44() = 0;                 // 0x48
    virtual u16 GetName(void* item, u8 idx) = 0;  // slot 0x4C
    virtual void vf_50() = 0; virtual void vf_54() = 0;
    virtual void vf_58() = 0; virtual void vf_5C() = 0;
    virtual void vf_60() = 0;                 // 0x60
    virtual u8 GetFlag(void* item, u8 idx) = 0;   // slot 0x64
};

// Item-implementation vtable view for the crystal-slot refresh callbacks
// (func_80217098 / func_802177D0): slot 0x08 is the count getter (u32 raw;
// callers mask to u16/u8), slot 0x80 the boxed/u16 state getter. Never
// instantiated; pure virtuals keep the vtable unemitted.
class CItemImplFacade3 {
public:
    virtual u32 GetCount(void* item) = 0;     // slot 0x08
    virtual void vf_04() = 0; virtual void vf_08() = 0;
    virtual void vf_0C() = 0; virtual void vf_10() = 0;
    virtual void vf_14() = 0; virtual void vf_18() = 0;
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual void vf_24() = 0; virtual void vf_28() = 0;
    virtual void vf_2C() = 0; virtual void vf_30() = 0;
    virtual void vf_34() = 0; virtual void vf_38() = 0;
    virtual void vf_3C() = 0; virtual void vf_40() = 0;
    virtual void vf_44() = 0; virtual void vf_48() = 0;
    virtual void vf_4C() = 0; virtual void vf_50() = 0;
    virtual void vf_54() = 0; virtual void vf_58() = 0;
    virtual void vf_5C() = 0; virtual void vf_60() = 0;
    virtual void vf_64() = 0; virtual void vf_68() = 0;
    virtual void vf_6C() = 0; virtual void vf_70() = 0;
    virtual void vf_74() = 0;                 // 0x7C
    virtual u16 GetBoxed(void* item) = 0;     // slot 0x80
};

// CSysWin vtable view: virtual at vtable slot 0x88 (user slot 32), reached
// by func_80213FE4's tail call. 32 fillers keep the target at the right slot.
class CSysWinVf88 {
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual void vf_18() = 0; virtual void vf_1C() = 0;
    virtual void vf_20() = 0; virtual void vf_24() = 0;
    virtual void vf_28() = 0; virtual void vf_2C() = 0;
    virtual void vf_30() = 0; virtual void vf_34() = 0;
    virtual void vf_38() = 0; virtual void vf_3C() = 0;
    virtual void vf_40() = 0; virtual void vf_44() = 0;
    virtual void vf_48() = 0; virtual void vf_4C() = 0;
    virtual void vf_50() = 0; virtual void vf_54() = 0;
    virtual void vf_58() = 0; virtual void vf_5C() = 0;
    virtual void vf_60() = 0; virtual void vf_64() = 0;
    virtual void vf_68() = 0; virtual void vf_6C() = 0;
    virtual void vf_70() = 0; virtual void vf_74() = 0;
    virtual void vf_78() = 0; virtual void vf_7C() = 0;
    virtual void vf_88() = 0;   // vtable slot 0x88
};
class CLytVf38 {   // virtual at vtable slot 0x38 (user slot 12)
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual void vf_18() = 0; virtual void vf_1C() = 0;
    virtual void vf_20() = 0; virtual void vf_24() = 0;
    virtual void vf_28() = 0; virtual void vf_2C() = 0;
    virtual void vf_30(int) = 0;   // slot 0x38
};
class CLytVf3C {   // virtual at vtable slot 0x3C (user slot 13)
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual void vf_18() = 0; virtual void vf_1C() = 0;
    virtual void vf_20() = 0; virtual void vf_24() = 0;
    virtual void vf_28() = 0; virtual void vf_2C() = 0;
    virtual void vf_30() = 0;                    // user slot 12 = 0x38
    virtual nw4r::lyt::Pane* vf_3C(char* s, int flag) = 0;  // slot 0x3C
};

// C-linkage imports
// -----------------
// Retail symbols with C linkage (unmangled names, per retail relocs).
// Previously declared inline in each TU as pseudo-imports; kept here as real
// imports. The names ARE the retail symbol names - keep verbatim.
extern "C" void code80135FDC_setVec3(float*, float, float, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_801D216C(void*, u8);  // retail symbol is unmangled
extern "C" u32 func_801D32DC(void*);
extern "C" int func_8021A8EC(void*);
extern "C" int CScrollBar_isVisible(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" void func_8021A984(void*);
extern "C" void func_8021A918(void*);
extern "C" void func_8021A960(void*);
extern "C" void func_801F369C(void*);
extern "C" char lbl_eu_8050888C[];
extern "C" unsigned long lbl_eu_80508870[];
// BDAT table pointer loaded via sda21 at the func_8013639C call sites.
extern unsigned long lbl_eu_806646D0;
extern "C" void func_80213570(void*, u8);
extern "C" unsigned long func_802165CC(unsigned long*, unsigned int);
extern "C" void func_80217434(CMCCrystalBox*, unsigned short, unsigned int*, unsigned char);
extern "C" void func_80217C0C(CMCCrystalBox*, unsigned short, unsigned int*, unsigned char);
// Crystal-slot refresh callbacks (defined in this TU).
extern "C" void func_80217098(CMCCrystalBox*, unsigned short, unsigned int*, unsigned char);
extern "C" void func_802177D0(CMCCrystalBox*, unsigned short, unsigned int*, unsigned char);
// Item-kind check (defined in this TU); noinline keeps the bl in the slot
// refresh callbacks instead of inlining the two-word test.
extern "C" __declspec(noinline) u8 func_80217BDC(void*);
// Message-resource / text helpers shared with the other kyoshin menu TUs.
extern "C" char* func_80138F78(u32);
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" void func_80137E7C(void*, void*, void*);
extern "C" void copyVEC3(void*, void*);
extern "C" char* func_802138B8(CMCCrystalData*, int);
extern "C" void func_8021A93C(void*);
extern "C" void func_8021A8F4(void*);
extern "C" void func_8021A840(void*, void*);
extern "C" void func_8021A9A8(void*, unsigned short, void*);
extern "C" int func_801D3320(void*);
extern "C" void func_801D31F8(void*, void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D350C(void*);
extern "C" void func_801D3518(void*, void*);
extern "C" void func_801D353C(void*, u8);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_8022B7C8(void*, void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F36BC(void*, unsigned long, unsigned long);
extern "C" void func_801F3850(void*, unsigned long);
extern "C" void func_801F367C(void*);
extern "C" void* CItem_initItemImplInstances(void*);
extern "C" u16 func_8015780C(int);
extern "C" char* func_80136190(const void*, const void*, int);
extern "C" void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
extern "C" void func_80124270(void*, u32);   // pane visibility setter
extern "C" void* func_80157C4C(u32, s16);
extern "C" int func_80213710(void*, unsigned char);
extern "C" void* func_8021384C(void*, unsigned char);
extern "C" unsigned short func_802137DC(void*, unsigned char);
extern "C" void func_80213964(int, void*, void*);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__14CMCCrystalInfoFv(void*, int);
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__6CCur16Fv(void*, int);
extern "C" void __dt__6CCur11Fv(void*, int);
extern "C" void __dt__6CCur09Fv(void*, int);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" CBaseCur* __ct__CCur07(void*, void*);
extern "C" CBaseCur* __ct__CCur09(void*, void*);
extern "C" CBaseCur* __ct__CCur18(void*, void*);
extern "C" CBaseCur* __ct__CCur11(void*, void*);
extern "C" CBaseCur* __ct__CCur16(void*, void*);
extern "C" void __ct__CSortMenu(void*);
extern "C" void __ct__CMCCrystalInfo(void*);
extern "C" void __ct__CScrollBar(void*, int);
extern "C" void __ct__CSysWin(void*, int);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void**);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" void func_80139198(u32);
extern "C" void func_801D3258(void*);
extern "C" void func_8021A860(void*);
extern "C" void func_801F35DC(void*);
extern "C" void func_8022B7F4(void*);
extern "C" int  func_801D3328(void*);
extern "C" void func_801D3408(void*);
extern "C" void func_801D3330(void*);
extern "C" void func_801D3430(void*, void*);
extern "C" void func_801D3454(void*, void*);
extern "C" void func_80137924(void*, void*, void*, void*);
extern "C" int CSysWin_getUnk34(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void func_8022B90C(void*, int);
extern "C" void func_8022B9B4(void*, u32, int);
extern "C" void func_8022BFC8(void*, int);
extern "C" void func_8022B8B8(void*);
extern "C" char* func_8013639C(const void*, const void*, int);
extern "C" void func_801D3160(void*);
extern "C" void func_8021A780(void*);
extern "C" void func_801D202C(void*);
extern "C" void func_801F3540(void*);
extern "C" void func_8022B748(void*);
extern "C" void func_80213488(void*);
extern "C" u32  func_801C6E90(void*);
extern "C" u16 func_80139358(u32);
extern "C" void func_80218018(CMCCrystalBox* self);
extern "C" void func_8021488C(CMCCrystalBox* self);
extern "C" int func_80216A20(CMCCrystalBox* self);
extern "C" void func_80219094(CMCCrystalBox* self);
extern "C" void func_80215B78(CMCCrystalBox* self);
extern "C" void func_8021852C(CMCCrystalBox* self);
extern "C" void func_8021900C(CMCCrystalBox* self);
extern "C" void func_8021911C(CMCCrystalBox* self);
extern "C" void func_802191A4(CMCCrystalBox* self);
extern "C" void func_8021922C(CMCCrystalBox* self);
// State-machine anim-completion callbacks (defined in this TU; retail symbols
// are unmangled). extern "C" first declaration keeps the switch call relocs
// (func_802142C4) unmangled like retail; noinline keeps the bodies out-of-line.
extern "C" __declspec(noinline) void func_80216B7C(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216BC8(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216C3C(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216C88(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80219464(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216CE0(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216D38(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216D84(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216DD8(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216E1C(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216E6C(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216EB0(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216EFC(CMCCrystalBox* self);
extern "C" void func_80219994(CMCCrystalBox* self, int);
extern "C" void func_80213788(void* self);
extern "C" void func_802137B4(void* self);
extern "C" void func_802136E0(void*, int, unsigned char);
extern "C" int func_80215AE8(void*);
extern "C" void func_80215B18(CMCCrystalBox* self);
extern "C" void func_80218A80(CMCCrystalBox* self);
extern "C" void func_802193D0(CMCCrystalBox* self);
extern "C" void func_8021AED0(char*, void*, unsigned char);
extern "C" void func_801375A0(char*, nw4r::lyt::Pane*);
extern "C" void func_801D2670(void*, unsigned char);
extern "C" int func_801392B4(unsigned char);
extern "C" unsigned short func_8013A7D0(unsigned char, unsigned char);
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);
extern char* lbl_eu_80664090;   // BDAT table pointer (sda21)
extern float lbl_eu_80668480;
extern float lbl_eu_80668484;
extern "C" void func_802180B4(CMCCrystalBox* self);
extern "C" void func_80218460(CMCCrystalBox* self);
extern "C" void func_80218B10(CMCCrystalBox* self);
extern "C" void func_8021899C(CMCCrystalBox* self);

// Selection/refresh helpers for the update callbacks and the ctor copy-init.
// Retail symbols are unmangled; declared here so the call relocs bind to the
// retail names. func_802165E8 returns its first argument (callers chain it as
// the copy source for func_80213E20); its definition lives in this TU under
// the name copyCrystalBoxParam_802165E8 (linked by address via symbols.txt).
extern "C" void func_801D3698(void*);   // sort menu: scroll down
// (func_801D3724 / func_801D377C are the page-up / page-down variants)
extern "C" void func_801D3724(void*);
extern "C" void func_801D377C(void*);
extern "C" u8 code80135FDC_getByte_64077();
extern "C" int func_80213748(void*);   // all-flags-set check (code_80213488.cpp)
extern "C" void func_802194EC(CMCCrystalBox* self);
extern "C" int func_80219AF0(CMCCrystalBox* self);
extern "C" void func_802156C0(CMCCrystalBox* self, int);
extern "C" __declspec(noinline) void func_80215408(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80215490(CMCCrystalBox* self);
// noinline: these update-callback helpers must stay out-of-line (retail
// emits bl to them); without it MWCC inlines their bodies into the callers.
extern "C" __declspec(noinline) void func_80216614(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80216698(CMCCrystalBox* self);
extern "C" __declspec(noinline) void func_80219348(CMCCrystalBox* self);

// func_80213FE4 (ctor-style copy-init) imports.
extern "C" void* getHandleMEM1__Q23mtl10MemManagerFv();
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" void __ct__UnkClass_8011C974(void*, void*);   // 4-word mem-region copy
// (func_8011C998 is the CScrollBar-data copy; retail symbol is unmangled)
extern "C" void func_8011C998(void*, void*);
extern "C" void func_80157824(u8, u32);
extern "C" void func_8021A718(void*);   // crystal-info file-load (CMCCrystalInfo.cpp)
extern "C" void func_801D3064(void*);   // sort-menu layout read (CSortMenu.cpp)
extern "C" void func_801F34F4(void*);   // scrollbar layout read (CScrollBar.cpp)
extern "C" u8* func_80213D74(u8*);      // param-table init (this TU)
extern "C" CMCCrystalBoxParam* func_802165E8(CMCCrystalBoxParam*, const CMCCrystalBoxParam*);

#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CBaseCur.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"

/* Cursor widget embedded at +0x34 (loads layout "curs22"). Its constructor
   and destructor are defined in the CCur unit (__ct__CCur22 / __dt__6CCur22Fv);
   CPartyState's destructor calls __dt__6CCur22Fv explicitly. */

class CEventFile;

class CPartyState {
public:
    CPartyState();
    ~CPartyState();
    bool OnFileEvent(CEventFile* pEventFile);
    u8 func_801FD17C();
    u8 func_801FD184();
    u8 func_801FD18C();
    u32 func_801FD580();
    u8 func_801FD5F4();
    u8 func_801FD5FC();

    void* mVtbl;                                // 0x00 - lbl_eu_805353C8
    UnkClass_8045F564 mMemRegion;               // 0x04 (0x10 bytes)
    CFileHandle* mFileHandle;                   // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // 0x18
    nw4r::lyt::Layout* mLayout;                 // 0x1C
    nw4r::lyt::AnimTransform* mAnimTrans1;      // 0x20
    nw4r::lyt::AnimTransform* mAnimTrans0;      // 0x24
    u8 field_0x28;                              // 0x28
    u8 _29[0x2C - 0x29];                        // 0x29
    s32 field_0x2C;                             // 0x2C (advance/state word; retail compares signed)
    u8 field_0x30;                              // 0x30
    u8 field_0x31;                              // 0x31
    u8 _32[0x34 - 0x32];                        // 0x32
    CBaseCur mCur22;                            // 0x34 (0x18 bytes)
    u8 field_0x4C;                              // 0x4C
    s8 field_0x4D;                              // 0x4D
    u8 field_0x4E;                              // 0x4E
    u8 field_0x4F[0x56 - 0x4F];                 // 0x4F - 7 per-member slot bytes (pane-name index)
    u8 field_0x56;                              // 0x56 - slot-write counter (func_801FDA7C)
    u8 field_0x57;                              // 0x57
};

// Vtable view of the party-slot stats sub-object embedded at +0x17C of the
// func_8009EC9C result. Only the dispatch slots used by this TU are reached
// (0x26/0x42/0x4A/0x4B/0x7A/0x7C); the table is fn-pointer indexed so the
// call sites reproduce the retail two-step vtable dispatch.
struct CPartySlotStats;
typedef int (*CPartySlotFn)(CPartySlotStats*);
typedef float (*CPartySlotFnF)(CPartySlotStats*);
struct CPartySlotStatsVtbl {
    CPartySlotFn fn[0x7D];
};
struct CPartySlotStats {
    CPartySlotStatsVtbl* mVtbl;   // 0x00
};

// 0x20-byte slot entry; func_801FE20C probes word at +i*0x20 for i = 1..5.
struct CPartySlotEntry {
    u32 word;                    // 0x00
    u8 _pad04[0x20 - 0x04];
};

// 5-float position table copy used by func_801FE39C (matches the retail's
// mtctr-2 + tail copy loop).
struct CPartyStateFiveFloats {
    float f[5];
};

// 0xC4-byte slot record: 6 entries (0x00..0xC0) + pad.
struct CPartySlotRecord {
    CPartySlotEntry entries[6];  // 0x00
    u8 _padC0[0xC4 - 0xC0];
};

// Slot-record blob at +0x3534 of the char data: 7 records, then a
// slot-indexed u32 array at +0x888, then the level word at +0x89C.
struct CPartySlotArea {
    CPartySlotRecord records[7];  // 0x00
    u8 _pad55C[0x888 - 7 * 0xC4]; // 0x55C..0x887
    u32 arr2[5];                  // 0x888 (slot-indexed u32 array, 0x888..0x89C)
    u32 field_0x3DD0;             // 0x89C (absolute 0x3DD0: level word)
};

// Character/party-slot object returned by func_8009EC9C. +0x176C holds a
// state word that is 1 when the slot's character is busy (cannot be swapped);
// +0x17C embeds the stats sub-object; +0x3534 the slot-record blob.
struct CPartyCharData {
    u8 _pad[0x17C];
    CPartySlotStats stats;               // 0x17C
    u8 _pad180[0x176C - 0x180];
    s32 field_0x176C;                    // 0x176C busy flag
    u8 _pad1770[0x3534 - 0x1770];
    CPartySlotArea slotArea;             // 0x3534
};

// Abstract view of the object returned by CDeviceFont::func_80452C10; the
// 8th user virtual (vtable+0x24) yields the u32 bound by func_8013676C.
// All-pure, never constructed directly.
class FontHelper {
public:
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual u32 v7() = 0;  // vtable offset 0x24
};

// 7-byte seed blob copied from .sdata2 constants (word + halfword + byte),
// mixed into the slot table by func_801FDA7C.
union PartyStateSeed {
    u8 bytes[7];
    struct {
        u32 w;  // 0x0
        u16 h;  // 0x4
        u8 b;   // 0x6
    } f;
};

/* Vtable-dispatch view of the embedded cursor. MWCC puts two implicit dtor
   slots at the start of every polymorphic vtable, so func_801D2180 (retail
   cursor slot 3) is declared as the SECOND virtual to land at vtable offset
   0xC, matching the retail dispatch in func_801FD0F4. All-pure, never
   constructed directly; only used to reproduce the retail vtable dispatch. */
class CPartyStateCur {
public:
    virtual void vfn0() = 0;          // MWCC vtable slot 2
    virtual void func_801D2180() = 0; // MWCC vtable slot 3 (retail slot 3)
};

/* ---- imports (declared here, never locally in the .cpp) ---- */

// UI sound effect; retail symbol is the C++ mangled func_80138078__FUl.
void func_80138078(u32);

// nw4r anim-transform frame checker; retail symbol is the C++ mangled
// func_80137444__FPQ34nw4r3lyt13AnimTransformf.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);

// nw4r anim-transform frame checker; retail symbol is unmangled (C linkage).
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

// Layout/accessor release helpers (C++ mangled retail symbols, mirroring the
// canonical declarations in code_80135FDC.hpp, which conflicts with
// CEquipItemBox.hpp's func_80136190 in this TU).
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// C-linkage helpers from other units (retail symbols are unmangled).
// func_801D216C / func_80139198 are declared in CEquipItemBox.hpp.
extern "C" void func_801D2BFC(CBaseCur*, u8);
extern "C" u8 code80135FDC_getByte_64077();

// Party-select helpers (unmangled retail symbols).
extern "C" int* func_8009ECB0();
extern "C" void func_8009E168(int*, u8, u8);
extern "C" u8 func_801392C8(u32);
extern "C" u32 func_8009CF8C(u32);
extern "C" u16 func_80136254(const void*, const void*, int);
extern "C" char* func_80138F78(u32);

// Party-slot data-table pointers (.sdata).
extern void* lbl_eu_80664090;
extern char* lbl_eu_80664098;

// Character stats getters (unmangled retail symbols).
extern "C" char* func_801355BC();
extern "C" u32 func_801355D8();
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" char* func_801571FC();

// BDAT string/value lookups (unmangled retail symbols).
extern "C" u8 func_8013600C(const char*, const char*, u32);

// Layout text/pane binding helpers (unmangled retail symbols; the canonical
// code_80135FDC.hpp / CItemBoxGrid.hpp declarations conflict with
// CEquipItemBox.hpp in this TU).
extern "C" void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" void func_8013676C(nw4r::lyt::Pane*, void*);
extern "C" void func_80137E7C(nw4r::lyt::Layout*, const char*, u32);
extern "C" void func_80124270(void*, u32);

// Layout + anim builders (retail symbols are the C++ mangled names).
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void func_801368C0(nw4r::lyt::Layout*, char*, u32);
void func_80136910(nw4r::lyt::Layout*, char*, u8);

// Slot-pane byte counter getter (unmangled retail symbol).
extern "C" u8 code80135FDC_getByte_6407E();

// cf::CfGameManager helper. US retail keeps the no-arg Fv suffix but the call
// site passes two arguments; declare the retail symbol verbatim under C
// linkage (a C++ declaration would re-mangle the suffix into an overload).
extern "C" void func_80080888__Q22cf13CfGameManagerFv(u32, u32);

// CDeviceFile::getFileSize - retail symbol keeps the single-arg FPCc suffix
// but call sites pass a second flags argument; the C++ declaration in
// CDeviceFile.hpp only covers the single-arg form.
extern "C" int getFileSize__11CDeviceFileFPCc(const char*, int);

// Scene object behind the common-archive alloc handle (virtual at vtable
// 0x2C of *lbl_eu_80663E14; returns an mtl alloc handle).
class CScn;
extern CScn* lbl_eu_80663E14;
extern "C" u32 func_80495FF0(CScn*);

// Party-state layout arc path pointer (.sdata).
extern char* lbl_eu_80662728;

// Unit-local helpers (defined in this TU; unmangled retail symbols).
extern "C" void func_801FE154(CPartyState*);
extern "C" void func_801FD848(CPartyState*);
extern "C" void func_801FE0C8(CPartyState*);
extern "C" void func_801FE20C(CPartyState*, u32, const char*);

// Cursor per-frame update (defined in the CCur unit).
extern "C" void func_801D202C(void*);

// Embedded cursor ctor/dtor (defined in the CCur unit).
extern "C" CBaseCur* __ct__CCur22(CBaseCur*, nw4r::lyt::ArcResourceAccessor*);
extern "C" CBaseCur* __dt__6CCur22Fv(CBaseCur*, int);

// Internal helpers of this unit (unmangled retail symbols).
extern "C" void func_801FD8F8(CPartyState*);
extern "C" void func_801FDA7C(CPartyState*);
extern "C" void func_801FD604(CPartyState*);
extern "C" void func_801FD48C(CPartyState*);
extern "C" void func_801FE39C(CPartyState*, float, float, u32, u32);

// C library formatted output (retail symbol unmangled).
extern "C" int sprintf(char*, const char*, ...);

// Frame-timer getter (kyoshin/cf/CfTFile.cpp, retail unmangled).
extern "C" u32 func_8006A80C();

// Text/layout binding helper (code_80135FDC.cpp, retail unmangled; the
// canonical header code_80135FDC.hpp conflicts with CEquipItemBox.hpp in
// this TU).
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);

// Character-data lookup (retail unmangled).
extern "C" void* func_8009EC9C(u32);

// Cursor pane-position/show helpers (CCur unit, retail unmangled).
extern "C" void func_801D2C80(CBaseCur*, const nw4r::math::VEC3*, u8);
extern "C" void func_801D2CF4(CBaseCur*, u8, u8);

// Pane translate accumulation (code_80135FDC.cpp, retail unmangled).
extern "C" void func_801375A0(nw4r::math::VEC3*, nw4r::lyt::Pane*);

// Pane size/position copy helpers (retail unmangled).
extern "C" void func_80127BC4(float*, float*);
extern "C" void func_80124288(nw4r::lyt::Pane*, float*);

// Party-state string table (.rodata).
extern char lbl_eu_80507D40[];
// Party-state 5-float position table (.rodata).
extern float lbl_eu_80507D20[];

// Party-state panel math constants (.sdata2).
extern const float lbl_eu_80668230;
extern const float lbl_eu_80668234;
extern const float lbl_eu_80668240;
extern const float lbl_eu_80668244;
extern const float lbl_eu_80668248;

// Party-state seed blob constants (.sdata2).
extern const u16 lbl_eu_8066821C;
extern const u8 lbl_eu_8066821E;
extern const u32 lbl_eu_80668220;

// CPartyState vtable (.data) - stored at +0 by the constructor. Declared as
// an array so MWCC uses full 32-bit (lis/addi) addressing like the retail.
extern "C" void* lbl_eu_805353C8[];

// Animation target frame for func_801FD76C (.sdata2, retail lbl_eu_80668218).
// const lets MWCC hoist the lfs to the function top (matches the matched
// CItemBoxInfo/CCollepedia frame-check shapes).
extern const float lbl_eu_80668218;

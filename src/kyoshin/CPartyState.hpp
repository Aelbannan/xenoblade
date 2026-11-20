#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>

#include "kyoshin/CBaseCur.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

/* Cursor widget embedded at +0x34 (loads layout "curs22"). Its constructor
   and destructor are defined in the CCur unit (__ct__CCur22 / __dt__6CCur22Fv);
   CPartyState's destructor calls __dt__6CCur22Fv explicitly. */

class CEventFile;

class CPartyState {
public:
    CPartyState();
    ~CPartyState();
    bool OnFileEvent(CEventFile* pEventFile);
    int isPartySettled();
    int isPartyIdle();
    int isPartyReady();
    int isPartyHighlightSet();
    int isPartyCloseRequested();
    int getPartySelectIndex();

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
    CCur22 mCur22;                              // 0x34 (0x18 bytes)
    u8 field_0x4C;                              // 0x4C
    s8 field_0x4D;                              // 0x4D
    u8 field_0x4E;                              // 0x4E
    u8 field_0x4F[0x56 - 0x4F];                 // 0x4F - 7 per-member slot bytes (pane-name index)
    u8 field_0x56;                              // 0x56 - slot-write counter (rebuildPartySelect)
    u8 field_0x57;                              // 0x57
};

// Vtable view of the party-slot stats sub-object embedded at +0x17C of the
// CtrlObjectParam_GetCharEntry result. Only the dispatch slots used by this TU are reached
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

// 0x20-byte slot entry; scalePartySlotPane probes word at +i*0x20 for i = 1..5.
struct CPartySlotEntry {
    u32 word;                    // 0x00
    u8 _pad04[0x20 - 0x04];
};

// 5-float position table copy used by positionPartyPane (matches the retail's
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

// Character/party-slot object returned by CtrlObjectParam_GetCharEntry. +0x176C holds a
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

// Abstract view of the object returned by CDeviceFont::getFontInfo; the
// 8th user virtual (vtable+0x24) yields the u32 bound by ApplyFontToTxtPanes.
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
// mixed into the slot table by rebuildPartySelect.
union PartyStateSeed {
    u8 bytes[7];
    struct {
        u32 w;  // 0x0
        u16 h;  // 0x4
        u8 b;   // 0x6
    } f;
};

/* Vtable-dispatch view of the embedded cursor. MWCC puts two implicit dtor
   slots at the start of every polymorphic vtable, so checkDeactivate (retail
   cursor slot 3) is declared as the SECOND virtual to land at vtable offset
   0xC, matching the retail dispatch in termPartyState. All-pure, never
   constructed directly; only used to reproduce the retail vtable dispatch. */
class CPartyStateCur {
public:
    virtual void vfn0() = 0;          // MWCC vtable slot 2
    virtual void checkDeactivate() = 0; // MWCC vtable slot 3 (retail slot 3)
};

/* ---- imports (declared here, never locally in the .cpp) ---- */

// UI sound effect; retail symbol is the C++ mangled playUISound__FUl.

// nw4r anim-transform frame checker; retail symbol is the C++ mangled
// advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf.

// nw4r anim-transform frame checker; retail symbol is unmangled (C linkage).

// Layout/accessor release helpers (C++ mangled retail symbols, mirroring the
// canonical declarations in code_80135FDC.hpp, which conflicts with
// CEquipItemBox.hpp's BdatTouchStringCell in this TU).

// C-linkage helpers from other units (retail symbols are unmangled).
// The cursor visibility/placement helpers are CBaseCur/CCur22 members
// (CBaseCur.hpp); BuildSlotItemLists is declared in CEquipItemBox.hpp.

// Party-select helpers (unmangled retail symbols).
extern "C" int* CtrlObjectParam_GetSlotTableBase();
#include "kyoshin/cf/CtrlObjectParamSymbols.hpp"

extern "C" u32 CtrlRemote_TouchBitByArg(u32);

// Party-slot data-table pointers (.sdata).
extern void* lbl_eu_80664090;
extern char* lbl_eu_80664098;

// Character stats getters (unmangled retail symbols).
extern "C" char* CItemBlock_getPtr20E8();

// BDAT string/value lookups (unmangled retail symbols).

// Layout text/pane binding helpers (unmangled retail symbols; the canonical
// code_80135FDC.hpp / CItemBoxGrid.hpp declarations conflict with
// CEquipItemBox.hpp in this TU).

// Layout + anim builders (retail symbols are the C++ mangled names).

// Slot-pane byte counter getter (unmangled retail symbol).

// cf::CfGameManager helper. US retail keeps the no-arg Fv suffix but the call
// site passes two arguments; declare the retail symbol verbatim under C
// linkage (a C++ declaration would re-mangle the suffix into an overload).
extern "C" void func_80080888__Q22cf13CfGameManagerFv(u32, u32);

// CDeviceFile::getFileSize - retail symbol keeps the single-arg FPCc suffix
// but call sites pass a second flags argument; the C++ declaration in
// CDeviceFile.hpp only covers the single-arg form.
extern "C" int getFileSize__11CDeviceFileFPCc(const char*, int);

// Scene object behind the common-archive alloc handle (virtual at vtable
// 0x2C of *gmScene; returns an mtl alloc handle).
class CScn;

// Party-state layout arc path pointer (.sdata).
extern char* lbl_eu_80662728;

// Unit-local helpers (defined in this TU; unmangled retail symbols).
extern "C" void refreshPartyHeader(CPartyState*);
extern "C" void finishPartyPanelClose(CPartyState*);
extern "C" void swapPartyMembers(CPartyState*);
extern "C" void scalePartySlotPane(CPartyState*, u32, const char*);

// Cursor per-frame update (defined in the CCur unit).

// Embedded cursor dtor (defined in the CCur unit).
extern "C" CBaseCur* __dt__6CCur22Fv(CBaseCur*, int);

// Internal helpers of this unit (unmangled retail symbols).
extern "C" void refreshPartyCursor(CPartyState*);
extern "C" void rebuildPartySelect(CPartyState*);
extern "C" void confirmPartySelect(CPartyState*);
extern "C" void cancelPartySelect(CPartyState*);
extern "C" void positionPartyPane(CPartyState*, float, float, u32, u32);

// C library formatted output (retail symbol unmangled).
extern "C" int sprintf(char*, const char*, ...);


// Text/layout binding helper (code_80135FDC.cpp, retail unmangled; the
// canonical header code_80135FDC.hpp conflicts with CEquipItemBox.hpp in
// this TU).

// Character-data lookup (retail unmangled).
extern "C" void* CtrlObjectParam_GetCharEntry(u32);

// Cursor pane-position/show helpers (CCur unit, retail unmangled).

// Pane translate accumulation (code_80135FDC.cpp, retail unmangled).

// Pane size/position copy helpers (retail unmangled).
extern "C" void TagCopyVec2f(float*, float*);
extern "C" void writePanePos(nw4r::lyt::Pane*, float*);

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

// Animation target frame for finishPartyIntroAnim (.sdata2, retail lbl_eu_80668218).
// const lets MWCC hoist the lfs to the function top (matches the matched
// CItemBoxInfo/CCollepedia frame-check shapes).
extern const float lbl_eu_80668218;

#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CEquipItemBox.hpp"
#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CItemBoxInfo.hpp"

namespace nw4r {
namespace lyt {
    class AnimTransform;
    class DrawInfo;
    class Pane;
}
namespace math {
    struct VEC3;
}
}

/* CEquipChange - equip-change page of the party-state window (embedded in
   CPartyStateWin at +0x4150, retail size 0x2A58). Retail vtable
   lbl_eu_80535688: RTTI "CEquipChange", ~CEquipChange, IWorkEvent slots,
   OnFileEvent at +0x10; base list names IWorkEvent. Member order and types
   from the dtor's member-destructor calls (+0x2B0, +0xA4, +0x80, +0x68,
   +0x50, +0x14, +0x04). */
class CEquipChange : public IWorkEvent {
public:
    CEquipChange();
    virtual ~CEquipChange();
    virtual bool OnFileEvent(CEventFile*);

    void LoadFiles();
    void AdvanceScreen();
    void DrawLayouts(nw4r::lyt::DrawInfo* drawInfo);
    void CleanupFiles();
    int GetSelWhenReady();
    u32 IsActiveFlag();
    int CheckBoxOpen();
    int IsSortOrSubPage();
    int IsWeaponRow();
    int IsMenuBusy();
    int IsSubcurBusy();
    void InitScreen();
    void CloseScreen();
    void CursorPrev();
    void CursorNext();
    void CursorUp();
    void CursorRight();
    void ConfirmSort();
    void OpenSubPage();
    void ConfirmSelect();
    void RefreshCategory(u8 cat);
    int MapCursorToCat();
    int MapCursorToSlot();
    int RunMainFlow();
    int GetBoxCount();
    void ShowSubPanes(bool visible);
    void HandleEIBName();
    void TryCloseEIBRow();
    int GetLockedState();
    int GetMenuId();
    int GetEquippedResId(u32 param);
    u32 GetBoxTailByte();
    void OnAnim38ToSt2();
    void OnAnim3CToSt3();
    void OnAnim3CToSt5();
    void OnAnim38ToIdle();
    void OnAnim40ToSt7();
    void OnSubPageClose();
    void OnApplyToStA();
    void OnAnim40ToSt3();
    void MaybeRebuild();
    void RefreshCursorPos();
    void RebuildCursorList();
    void* ResolveEquipItem();

    UnkClass_8045F564 mRegion04;              // 0x04
    UnkClass_8045F564 mRegion14;              // 0x14
    u32 field_24;                             // 0x24 - file handle 1
    u32 field_28;                             // 0x28 - file handle 2
    nw4r::lyt::ArcResourceAccessor* field_2C; // 0x2C - arc resource accessor
    u32 field_30;                             // 0x30 - second arc resource accessor
    u32 field_34;                             // 0x34 - layout
    nw4r::lyt::AnimTransform* field_38;       // 0x38
    nw4r::lyt::AnimTransform* field_3C;       // 0x3C
    nw4r::lyt::AnimTransform* field_40;       // 0x40
    u8 field_44;                              // 0x44 - visible
    u32 field_48;                             // 0x48 - state
    u8 field_4C;                              // 0x4C
    u8 field_4D;                              // 0x4D - active
    CCur14 mCur14;                            // 0x50
    CCur15 mCur15;                            // 0x68
    CSubCur mSubCur;                          // 0x80
    s8 field_98;                              // 0x98 - cursor position
    u8 field_99;                              // 0x99 - party slot
    u8 mSlotMarks[10];                        // 0x9A
    CItemBoxInfo mItemBoxInfo;                // 0xA4
    CEquipItemBox mEquipItemBox;              // 0x2B0
};



// C++ linkage so MWCC mangles to the retail symbol advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf.

struct CEqChCursorRec {
    u16 field_00;
    u32 field_04;
};

// Per-slot bdat message-table handles read indexed by CEquipChange::RebuildCursorList
// (lbl_eu_806640D8 is an array of handles; the scalar extern comes from
// CEquipItemBox.hpp).
#define lbl_eu_806640D8_arr ((void**)(void*)&lbl_eu_806640D8)

// Character-data table returned by CtrlObjectParam_GetCharEntry; +0x176C holds a state word
// that is 1 when the character is busy (same object as CPartyCharData). The
// +0x1C..+0x26 s16 slots hold the per-category equipped item ids (weapon/
// armour rows) read by CEquipChange::RunMainFlow / CEquipChange::RebuildCursorList.
struct CBdatCharData {
    u8 _pad[0x1C];
    s16 field_1C;   // 0x1C
    s16 field_1E;   // 0x1E
    s16 field_20;   // 0x20
    s16 field_22;   // 0x22
    s16 field_24;   // 0x24
    s16 field_26;   // 0x26
    u8 _pad2[0x176C - 0x28];
    s32 field_176C;   // 0x176C - busy flag
};

// 8-byte colour pair copied from lbl_eu_80508120 by CEquipChange::RefreshCursorPos's palette
// copy loop (the loop writes 8 pairs into the 7-pair local - retail shape).
struct CEquipColorPair {
    u32 a;   // 0x0
    u32 b;   // 0x4
};

// 0x38-byte palette block used by CEquipChange::RefreshCursorPos's copy (block-copy path
// emits the retail lwzu/stwu counted loop - 7 pairs of 8 bytes).
struct CEquipPaletteBlock {
    CEquipColorPair pair[7];   // 0x38
};

// Cast-only view of nw4r::lyt::Pane exposing the mScale VEC2 at +0x44 as raw
// words (CEquipChange::RefreshCursorPos copies it via the GPR struct-copy path - retail shape).
struct CPaneScaleView {
    u8 _pad[0x44];
    CEquipColorPair mScale;    // 0x44 - {x, y} raw words
};


// Color/sound palette entries initialised by sinit_802059E8 (sdata2).
extern void* sEquipCursorSrcColorA;
extern void* sEquipCursorSrcColorB;
extern GXColorS10 sEquipCursorColorA0;
extern GXColorS10 sEquipCursorColorB0;
extern GXColorS10 sEquipCursorColorA1;
extern GXColorS10 sEquipCursorColorB1;

// Global equip-page flag (sdata), cleared by CEquipChange::CleanupFiles.
extern u32 sEquipChangeAccessor;

// Palette source (16 u32 colours) and string pool used by CEquipChange::RefreshCursorPos and
// the bind-file loader CEquipChange::LoadBindFiles.
extern u32 lbl_eu_80508120[];
extern char lbl_eu_80508168[];

// Sub-cursor equip lookup tables (CEquipChange::RefreshCategory): a u32 category word at
// byte offset 2*i plus a byte at 2*i+4 per row i. Fixed sizes keep the
// symbols sdata-eligible so MWCC emits the retail sda21 addressing.
extern u8 lbl_eu_806682A0[8];
extern u8 lbl_eu_806682A4[8];

// Result of the item-impl fetch hook
// CEquipChange::ResolveEquipItem: packed word at +0 and a halfword at +4 whose low bit picks
// between the direct-equip and bdat-rebuild paths.
struct CEquipV9Result {
    u32 field_00;
    u16 field_04;
};

// Scratch CItem record rebuilt from bdat by CEquipChange::ResolveEquipItem's fallback path
// (cleared once, guarded by sEquipWorkItemInitialized).
struct CEquipWorkItem {
    u32 field_00;
    u16 field_04;
    u8 _pad06[0xF8];
};
extern CEquipWorkItem sEquipChangeWorkItem;
extern s8 sEquipWorkItemInitialized;

// bdat helpers used by CEquipChange::ResolveEquipItem's rebuild path.
extern "C" u32 Bdat_GetMaxRow_B1EC(void* file);
// CItem_initSlot is declared in kyoshin/cf/CItem.hpp.


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail emits these as C-style (unmangled) symbols, so reference them with C
// linkage. The subcursor gates are CSubCur members (BothSubPanesOn returns status).

// CItemBoxInfo helpers used by the equip-change screen (C-ABI retail names).

// Free-function form (retail: advanceItemBoxState__FP12CItemBoxInfo).

// subcur visibility setter (CSubCur::ShowTwoSubPanes; see CCur.cpp).
extern "C" void CtrlObjectParam_SyncParamFromActorEx(void* charObj, u32 flag);
extern "C" void CtrlObjectParam_GetArtsStatsRow(void* rows, u32 count);
extern "C" void* CtrlObjectParam_GetCharEntry(u32);
// Canonical return type from the owner header (cf/CItem.hpp). Kept attribute-
// free here: CItemExt is only ever used through this pointer.
struct CItemExt;

// CEquipItemBox gate/handler imports used by the equip-change handlers.

// bdat item-name table (sdata pointer; canonical type from cf/CItem.hpp) and
// message-count lookup used by CEquipChange::RunMainFlow's category equip checks.
extern void* lbl_eu_806640EC;
extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);

// Per-category equipped-item row setters (CEquipChange::RunMainFlow case tails).
extern "C" void CtrlObjectParam_SetEquipSlot5(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot0(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot1(void*, int);
extern "C" void CtrlObjectParam_SetEquipSlot4(void*, int);
extern "C" void CtrlObjectParam_SyncParamFromActor(void*);

// 3-word scene-name table read by CEquipChange::RunMainFlow's refresh block (.rodata).
extern const u32 sEquipSceneNameTags[3];

// CfObjEnumList helper family (opaque void* params repo-wide - see
// CPartyStateWin.hpp); struct views for the stack holder / list / slot used
// by CEquipChange::RunMainFlow's refresh block.
struct CEquipEnumList { u8 _00[0x620]; u32 field_0x620; };
struct CEquipEnumListSlot { u8 _00[0x4]; void* field_0x4; };
extern "C" void startEnumObjects(void*, u32, u32);
extern "C" void* getEntryAt(void*, u32);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* objMove);
extern "C" void CfObjectPc_setupFrame(void* obj, u32 flag);

// Sub-cursor activate (defined in CCur.cpp).

// Accumulate a pane's translate into output (CPartyState.hpp also declares
// this C-ABI helper).


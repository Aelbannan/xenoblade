#pragma once

// Real base classes for the retail composite vtable.  Their inline-empty
// destructors keep the derived cleanup list in retail order.
#define IUIWINDOW_INLINE_DTOR
#define IWORK_EVENT_INLINE_DTOR

#include <types.h>
#include "kyoshin/IUIWindow.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <revolution/gx/GXTypes.h>

#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

namespace nw4r {
namespace lyt {
class AnimTransform;
class Layout;
class DrawInfo;
}
}

class CFileHandle;
class CScn;

struct CMenuGetItemMultiEntry {
    u32 packed;
    u16 value;
    u8 rest[0x2e];
    // Default ctor zeroes value (+4, halfword) then packed (+0): the retail
    // CMenuGetItemMulti ctor's array-init loop stores in that order.
    CMenuGetItemMultiEntry() { value = 0; packed = 0; }
};

struct CMenuGetItemPaneView {
    u8 _00[0x4c];
    f32 width;
    f32 height;
    u8 _54[0xbb - 0x54];
    u8 flags;
};


class CEventFile;

class CMenuGetItemMulti : public IUIWindow,
                          public IWorkEvent,
                          public IScnRender {
public:
    CMenuGetItemMulti(CScn* scn, u32 item0, u32 item1, u32 item2, u32 item3,
                      u32 argF6, u32 arg201, u8 argSpecial);
    virtual ~CMenuGetItemMulti();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();
    virtual bool OnFileEvent(CEventFile* pEvent);

    // IUIWindow (+0x00..+0x6B), IWorkEvent (+0x6C) and IScnRender (+0x70)
    // are real bases; MWCC emits their vptrs and the composite vtable.
    CScn* mScn;                              // +0x74
    u8 mRegion[0x10];                        // +0x78
    nw4r::lyt::AnimTransform* mAnim;         // +0x88
    u32 mInitialItems[4];                    // +0x8C
    u32 mVisibleItemCount;                   // +0x9C
    CMenuGetItemMultiEntry mFirstEntry;      // +0xA0 (ctor inlined by MWCC)
    CMenuGetItemMultiEntry mRestEntries[3];  // +0xD4 (array-init bdnz loop)
    u32 mEntryCount;                         // +0x170
    CCur18 mCursor;                          // +0x174
    u8 mSystemWindow[0x3C];                  // +0x18C CSysWin storage (raw;
                                             // ctor/dtor driven via C-ABI
                                             // __ct__7CSysWinFUc/__dt__7CSysWinFv)
    u8 mMaxVisibleItems;                     // +0x1C8
    u8 mHasSpecialItem;                      // +0x1C9
    u8 mPaneVisible[6];                      // +0x1CA
    CMenuGetItemMultiEntry* mVisibleEntries[6]; // +0x1D0
    u16 mVisibleItemIds[6];                  // +0x1E8
    u8 field_1F4;
    u8 field_1F5;
    u8 field_1F6;
    u8 field_1F7;
    u32 field_1F8;
    u32 field_1FC;
    u8 field_200;
    u8 field_201;
    u8 field_202[2];
    CFileHandle* mFileHandle;
    u32 field_208;
    u8 field_20C;
    u8 field_20D;
    u8 field_20E;
    u8 field_20F;
    // Rank-item slot display arrays (shared by the category-3/9 item
    // handlers GetItemMulti_FillEntryRankPane / GetItemMulti_ShowItemRank / GetItemMulti_FillRankSlots): per-slot item
    // ids, positions and the "slot filled" flag byte. Slots 8-11 are the
    // rank-item display. The +0x2C4/+0x2D0 arrays are only written by
    // GetItemMulti_FillRankSlots (extra per-slot rank data).
    u16 mRankSlotIds[12];             // +0x210
    nw4r::math::VEC3 mRankSlotPos[12]; // +0x228
    u8 mRankSlotFlag[12];             // +0x2B8
    u8 mRankSlotFlag2[12];            // +0x2C4
    u16 mRankSlotIds2[12];            // +0x2D0
    u8 field_2E8;                     // +0x2E8 slot-window selection (A-press scan)
    u8 unk2EC[0x80];                  // +0x2EC unknown; retail sizeof is 0x36C (CreateInstance/CreateSimple alloc), no access in this TU
};

// Vtable-dispatch view of the per-category item impl (the +0x4C/+0x54/+0x64/
// +0x90 slots this TU calls are typed here; CItem.hpp's CItemImpl keeps those
// as placeholders). Offsets match the real CItemImpl vtable; never
// constructed directly (instances come from CItem_initItemImplInstances).
class CMenuGetItemImpl {
public:
    virtual u16 getRankCount(CMenuGetItemMultiEntry*) = 0;    // +0x08
    virtual void v0C() = 0;
    virtual void v10() = 0;
    virtual void v14() = 0;
    virtual void v18() = 0;
    virtual void v1C() = 0;
    virtual char* getName(CMenuGetItemMultiEntry*) = 0;       // +0x20
    virtual void v24() = 0;
    virtual void v28() = 0;
    virtual void* getSlot(CMenuGetItemMultiEntry*, u8) = 0;   // +0x2C
    virtual u8 hasSlot(CMenuGetItemMultiEntry*) = 0;          // +0x30
    virtual void v34() = 0;
    virtual void v38() = 0;
    virtual void v3C() = 0;
    virtual s16 getSlotId(CMenuGetItemMultiEntry*, u8) = 0;   // +0x40
    virtual void v44() = 0;                                   // +0x44
    virtual void v48() = 0;                                   // +0x48
    virtual u32 vf4C(CMenuGetItemMultiEntry*, u8) = 0;        // +0x4C slot item id
    virtual void v50() = 0;                                   // +0x50
    virtual u32 vf54(CMenuGetItemMultiEntry*) = 0;            // +0x54 item id
    virtual void v58() = 0;
    virtual void v5C() = 0;
    virtual void v60() = 0;
    virtual u32 vf64(CMenuGetItemMultiEntry*, u8) = 0;        // +0x64 slot id
    virtual void v68() = 0;
    virtual void v6C() = 0;
    virtual void v70() = 0;
    virtual void v74() = 0;
    virtual void v78() = 0;
    virtual void v7C() = 0;
    virtual void v80() = 0;
    virtual void v84() = 0;
    virtual void v88() = 0;
    virtual void v8C() = 0;
    virtual int vf90(CMenuGetItemMultiEntry*) = 0;           // +0x90 rank text value
};

// --- unit imports ---

// Get-item-multi singleton instance pointer (retail SDA symbol).
extern u32 gpItemMultiInstance;

// Currently-loaded item file buffer pointer (released by Term/GetItemMulti_OnFileEvent).
extern u32 gpItemMultiSkillBdat;

// Item-name font pointer (.sbss, read by GetItemMulti_FillEntryRankPane's item-name lookup).

// Pane colour/position defaults (.sbss, written by sinit_801B9FC8).
extern GXColorS10 gItemMultiPaneColor00;
extern GXColorS10 gItemMultiPaneColor01;
extern GXColorS10 gItemMultiPaneColor02;
extern GXColorS10 gItemMultiPaneColor03;
extern GXColorS10 gItemMultiPaneColor04;
extern GXColorS10 gItemMultiPaneColor05;
extern GXColorS10 gItemMultiPaneColor06;
extern GXColorS10 gItemMultiPaneColor07;
extern GXColorS10 gItemMultiPaneColor08;
extern GXColorS10 gItemMultiPaneColor09;
extern GXColorS10 gItemMultiPaneColor0A;
extern GXColorS10 gItemMultiPaneColor0B;
extern GXColorS10 gItemMultiPaneColor0C;
extern GXColorS10 gItemMultiPaneColor0D;

// Global flag word: bit 21 (0x200000) gates the render path (cbRenderBefore)
// and is cleared by Term.
extern u32 gmPauseFlags;

// Pane size/position step factor used by GetItemMulti_FitPanes's pane adjustments
// (.sdata2). Sibling lbl_eu_80667E18 is the int->double conversion magic
// (0x43300000_80000000) that MWCC emits for the (f32)(s32) casts there.
// Plain (non-const) extern: a `const f64` reference gets internalized by
// MWCC into an anonymous literal pool (@12670), breaking the reloc name.
extern f32 lbl_eu_80667E14;
extern f64 lbl_eu_80667E18;

// Anim frame target used by Move's open/close states (advanceAnimTransform /
// AnimRewindFrame argument), plus the rank-window size/position constants
// GetItemMulti_ShowEquipped scales the item window by.
extern f32 lbl_eu_80667E10;
extern f32 lbl_eu_80667E20;
extern f32 lbl_eu_80667E24;
extern f32 lbl_eu_80667E28;

// Slot-window page step: GetItemMulti_HandlePad's A-press multiplies the selected
// slot index by this to derive the page (field_20D), using the sibling
// int->double magic constant 80667E08 for the (f64) index conversion.
extern f32 lbl_eu_80667E2C;
// Non-const: a `const f64` here gets internalized by MWCC into an anonymous
// literal pool (@12738), breaking the reloc name.
extern f64 lbl_eu_80667E08;

// Item-window font/table pointers (.sbss) used by the category-4/8 item
// handlers (GetItemMulti_ShowEquipped / GetItemMulti_ShowEntryDetailPanes) and GetItemMulti_FillRankSlots.

// Item-table pointer + fallback name-table font pointer used by
// GetItemMulti_ShowIdCategoryPanes's description/rank text lookups (.sbss).

#include "kyoshin/CTaskGame.hpp"

// Minimal view of the CfGameManager fields Term's action-source teardown
// touches (the full class exposes these offsets as byte arrays).
struct CfGameManagerTermFields {
    u8 _00[0x64];
    u32 field_64;              // +0x64 action-source flags (bit 8 = active)
    u32 field_68;              // +0x68 flags (|= 0x60 on teardown)
    u8 _6C[0x80 - 0x6C];
    u32 field_80;              // +0x80 action-source id slot
};

// C-ABI imports (retail emits these unmangled). The .sbss/.rodata labels each
// interacts with are noted on the decl.
extern "C" void Bdat_GetTable_AA34();                    // bdat refresh (paired with the lbl_eu_80504A3C fp lookup)
extern "C" void __ct__17UnkClass_8045F564Fv(void* self); // layout-memory region ctor

// GetItemMulti_HandlePad); sysWinSwitchKindPane/BFC8 take CSysWin* (declared in CSysWin.hpp).
// Item removal helpers (item sweeps in GetItemMulti_PruneEntries): release an entry
// and notify the item system.
extern "C" void CItemData_lookupOrAlloc(CMenuGetItemMultiEntry* entry, u32 flags);
extern "C" void CItem_thunkAllocRecord(u32 id, u32 flags);
extern "C" void UIWin_BuildFlagBuf(u32 a, u32 id, u32 b);
// Slot-window name source used by GetItemMulti_HandlePad's A-press open (no args).
// Cursor position + window selection helpers (pad-input handlers).
// cursor visibility setter
// Per-frame system-window/cursor updates (Move's tail after the state switch).
// System-window lifecycle queries used by Move's state machine.
// Item-menu active check (CMenuItem.cpp) and item-created callback (CUICfManager.cpp).
// Defined in CMenuItem.cpp with C++ linkage.
unsigned long ItemMenu_IsPresent();
extern "C" u32 CUICfManager_queuePauseItemMenu(u8 self, u8 arg1, u8 arg2);
// Rank-window geometry helpers (CItemBoxInfo.cpp family).
extern "C" void removeTboxById(u32 obj);             // action-source teardown (Term, after the gmGameFlags clear)
// UI sound effect (retail pre-mangled name).
// Rank-item pane text setter (retail symbol is the C++-mangled name).
// Retail ctor symbol is the unmangled `__ct__CMenuGetItemMulti` (constructs
// the singleton stored in gpItemMultiInstance); 8 reg args + a byte on the stack.
extern "C" u8* __ct__CMenuGetItemMulti(u8* obj, CScn* pScene, u32 a, u32 b, u32 c,
                                       u32 d, u32 e, u32 f, u8 g);

// C++-linkage actor-id lookup; canonical owner is code_800B06A4.hpp.
void* findObjectById(int id);



// Character-record lookup helpers (GetItemMulti_ShowCat13's category-13 display):
// CtrlObjectParam_GetCharEntry fetches the manager object for a slot id, and
// CtrlObjectParam_GetCurrentRowKey reads the character index from it.
extern "C" u32 CtrlObjectParam_GetCurrentRowKey(void* mgr);
// Active-character id read off the slot-manager object
// (GetItemMulti_ShowEquipped's equipped-item window scaling).
extern "C" u32 CtrlObjectParam_GetArtsDataWord(void* mgr);

// Per-rank slot record returned by the item impl's getSlot (+0x2C): bit 0
// of the +4 word marks the slot filled, bits 4-15 hold the slot item id,
// bits 7-9 the rank index and bits 10-15 a signed rank value.
struct CMenuGetItemRankSlot {
    u8 _00[4];
    u16 bits04;
};

// Category-13 record view: the flag byte (+0xE8) and the two-bit select byte
// (+0xE9) that pick the alternate rank-name string.
struct CMenuGetItemCat13Record {
    u8 _00[0xe8];
    u8 flagE8;
    u8 bitsE9;
};

class CProcess;

// Free functions defined in this TU (real C++ linkage, retail-mangled names).
IUIWindow* GetItemMulti_CreateInstance(CProcess* pParent, CScn* pScene,
                                       u32 a, u32 b, u32 c, u32 d, u32 e,
                                       u32 f, u8 g);
u8* GetItemMulti_CreateSimple(CProcess* pParent, u32 arg2, u32 arg3);

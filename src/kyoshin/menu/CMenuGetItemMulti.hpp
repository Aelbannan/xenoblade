#pragma once

#include <types.h>

#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"

namespace nw4r {
namespace lyt {
class AnimTransform;
class Layout;
class DrawInfo;
}
}

class CFileHandle;
class CScn;
class IUIWindow;

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

// 4 x s16 pane colour/position entry (RGBA-style; alpha (.d) stays 0).
struct CMenuGetItemFourShorts {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
};

class CMenuGetItemMulti {
public:
    CMenuGetItemMulti();
    ~CMenuGetItemMulti();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent();

    // Deliberately NON-polymorphic (CMenuGetItem idiom): the CProcess base
    // region (incl. composite vtable at +0x10) is opaque storage; the
    // IWorkEvent/IScnRender subobject vtable slots are explicit fields below.
    u8 _00[0x10];
    u32 field_10;            // composite vtable (interim -> final)
    u8 _14[0x28];            // 0x14-0x3B rest of the CProcess base region
    u32 mPtmfSlot3C[3];      // 0x3C null PTMF callback slot (__ptmf_null)
    u32 mPtmfSlot48[3];      // 0x48 null PTMF callback slot (__ptmf_null)
    nw4r::lyt::Layout* mLayout;              // +0x54
    u32 field_58;
    u32 field_5C;
    s32 field_60;
    u8 field_64;
    u8 field_65;
    u8 field_66;
    u8 field_67;
    u32 field_68;
    u32 mIWorkEventVtable;                   // +0x6C
    u32 mIScnRenderVtable;                   // +0x70
    CScn* mScn;                              // +0x74
    u8 mRegion[0x10];                        // +0x78
    nw4r::lyt::AnimTransform* mAnim;         // +0x88
    u32 mInitialItems[4];                    // +0x8C
    u32 mVisibleItemCount;                   // +0x9C
    CMenuGetItemMultiEntry mFirstEntry;      // +0xA0 (ctor inlined by MWCC)
    CMenuGetItemMultiEntry mRestEntries[3];  // +0xD4 (array-init bdnz loop)
    u32 mEntryCount;                         // +0x170
    CBaseCur mCursor;                        // +0x174
    u8 mSystemWindow[0x3C];                  // +0x18C CSysWin storage (raw;
                                             // ctor/dtor driven via C-ABI
                                             // __ct__CSysWin/__dt__7CSysWinFv)
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
    // handlers func_801B7440 / func_801B70BC / func_801B7A58): per-slot item
    // ids, positions and the "slot filled" flag byte. Slots 8-11 are the
    // rank-item display. The +0x2C4/+0x2D0 arrays are only written by
    // func_801B7A58 (extra per-slot rank data).
    u16 mRankSlotIds[12];             // +0x210
    nw4r::math::VEC3 mRankSlotPos[12]; // +0x228
    u8 mRankSlotFlag[12];             // +0x2B8
    u8 mRankSlotFlag2[12];            // +0x2C4
    u16 mRankSlotIds2[12];            // +0x2D0
    u8 field_2E8;                     // +0x2E8 slot-window selection (A-press scan)
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
    virtual u8 vf90(CMenuGetItemMultiEntry*) = 0;            // +0x90 rank text value
};

// --- unit imports ---

// Get-item-multi singleton instance pointer (retail SDA symbol).
extern u32 lbl_eu_80664414;

// Currently-loaded item file buffer pointer (released by Term/func_801B45A0).
extern u32 lbl_eu_80664418;

// Item-name font pointer (.sbss, read by func_801B7440's item-name lookup).
extern void* lbl_eu_806640D8;

// Pane colour/position defaults (.sbss, written by sinit_801B9FC8).
extern CMenuGetItemFourShorts lbl_eu_806643A0;
extern CMenuGetItemFourShorts lbl_eu_806643A8;
extern CMenuGetItemFourShorts lbl_eu_806643B0;
extern CMenuGetItemFourShorts lbl_eu_806643B8;
extern CMenuGetItemFourShorts lbl_eu_806643C0;
extern CMenuGetItemFourShorts lbl_eu_806643C8;
extern CMenuGetItemFourShorts lbl_eu_806643D0;
extern CMenuGetItemFourShorts lbl_eu_806643D8;
extern CMenuGetItemFourShorts lbl_eu_806643E0;
extern CMenuGetItemFourShorts lbl_eu_806643E8;
extern CMenuGetItemFourShorts lbl_eu_806643F0;
extern CMenuGetItemFourShorts lbl_eu_806643F8;
extern CMenuGetItemFourShorts lbl_eu_80664400;
extern CMenuGetItemFourShorts lbl_eu_80664408;

// Global flag word: bit 21 (0x200000) gates the render path (cbRenderBefore)
// and is cleared by Term.
extern u32 lbl_eu_80663E28;

// Pane size/position step factor used by func_801B5630's pane adjustments
// (.sdata2). Sibling lbl_eu_80667E18 is the int->double conversion magic
// (0x43300000_80000000) that MWCC emits for the (f32)(s32) casts there.
// Plain (non-const) extern: a `const f64` reference gets internalized by
// MWCC into an anonymous literal pool (@12670), breaking the reloc name.
extern f32 lbl_eu_80667E14;
extern f64 lbl_eu_80667E18;

// Anim frame target used by Move's open/close states (func_80137444 /
// func_80137510 argument), plus the rank-window size/position constants
// func_801B6184 scales the item window by.
extern f32 lbl_eu_80667E10;
extern f32 lbl_eu_80667E20;
extern f32 lbl_eu_80667E24;
extern f32 lbl_eu_80667E28;

// Slot-window page step: func_801B82E8's A-press multiplies the selected
// slot index by this to derive the page (field_20D), using the sibling
// int->double magic constant 80667E08 for the (f64) index conversion.
extern f32 lbl_eu_80667E2C;
// Non-const: a `const f64` here gets internalized by MWCC into an anonymous
// literal pool (@12738), breaking the reloc name.
extern f64 lbl_eu_80667E08;

// Item-window font/table pointers (.sbss) used by the category-4/8 item
// handlers (func_801B6184 / func_801B69F4) and func_801B7A58.
extern u32 lbl_eu_806640F4;
extern u32 lbl_eu_806640F8;

// Item-table pointer + fallback name-table font pointer used by
// func_801B76CC's description/rank text lookups (.sbss).
extern void* lbl_eu_80664104;
extern void* lbl_eu_806640A8;

// Minimal CTaskGame decl (same shape as CSystemWindow.hpp) - only the
// statics cbRenderBefore consumes.
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool isFlag01Set();
};

// Vtable-dispatch view of the embedded cursor (same scheme as CPartyStateCur):
// MWCC puts two implicit dtor slots at the start of every polymorphic vtable,
// so checkDeactivate (retail cursor slot 3) is declared as the SECOND virtual
// to land at vtable offset 0xC. All-pure, never constructed directly.
class CMenuGetItemMultiCur {
public:
    virtual void vfn0() = 0;                             // MWCC vtable slot 2
    virtual void checkDeactivate() = 0;                    // MWCC vtable slot 3 (retail slot 3)
    virtual void vfn4(const nw4r::math::VEC3*) = 0;      // MWCC vtable slot 4 (+0x10) cursor move
};

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
extern "C" void func_8003AA34();                    // bdat refresh (paired with the lbl_eu_80504A3C fp lookup)
extern "C" void func_8003AA78__5CBdatFUlPv(u32 value, u8* data);
extern "C" void getEntry__5CBdatFUl(u32 value);
extern "C" u8 func_8013B980();                      // flag reset alongside code80135FDC_getByte_64080
extern "C" u8 code80135FDC_getByte_64080();
extern "C" void func_8022B7F4(u8* syswin);          // CSysWin teardown (Term, after the lbl_eu_80664414/18 clears)
extern "C" void func_8022B7C8(u8* syswin, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801D20B0(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8022B8E4(u8* syswin);          // CSysWin re-layout
// CSysWin content setters (grade-up window setup in func_801B8E2C /
// func_801B82E8); func_8022B90C/BFC8 take CSysWin* (declared in CSysWin.hpp).
extern "C" void func_8022B9B4(u8* syswin, char* text, u32 flag);
extern "C" void func_8022B8B8(u8* syswin);
extern "C" void func_8022BF6C(u8* syswin, char* a, char* b);
// Item removal helpers (item sweeps in func_801B8E2C): release an entry
// and notify the item system.
extern "C" void func_801599D4(CMenuGetItemMultiEntry* entry, u32 flags);
extern "C" void func_801586D4(u32 id, u32 flags);
extern "C" void func_80140E00(u32 a, u32 id, u32 b);
// Slot-window name source used by func_801B82E8's A-press open (no args).
extern "C" char* func_801D3C74();
// Cursor position + window selection helpers (pad-input handlers).
extern "C" void func_8022C1B4(nw4r::math::VEC3* out, u8* syswin, u8 sel);
extern "C" void func_801D216C(CBaseCur* cur, u8 val); // cursor visibility setter
// Per-frame system-window/cursor updates (Move's tail after the state switch).
extern "C" void func_8022B748(u8* syswin);
extern "C" void func_801D202C(CBaseCur* cur);
// System-window lifecycle queries used by Move's state machine.
extern "C" int CSysWin_isActive(u8* syswin);
extern "C" u32 CSysWin_isReady(u8* syswin);
extern "C" u32 CSysWin_getUnk34(u8* syswin); // window state query (pad handlers)
// Item-menu active check (CMenuItem.cpp) and item-created callback (CUICfManager.cpp).
extern "C" u32 func_80167A18();
extern "C" u32 func_80133E58(u8 self, u8 arg1, u8 arg2);
// Rank-window geometry helpers (CItemBoxInfo.cpp family).
extern "C" f32 func_8013B380(u32 idx);
extern "C" void func_80139C98(u16 a, u16 b, int c, f32 d);
extern "C" void func_80137F88(void* pane, void* tex); // pane texture setter
extern "C" void func_800B7320(u32 obj);             // action-source teardown (Term, after the lbl_eu_80663E24 clear)
extern "C" void func_80137924(nw4r::math::VEC3* out, nw4r::lyt::Pane* a,
                               nw4r::lyt::Pane* b, nw4r::lyt::Pane* root); // cursor position from two panes
// UI sound effect (retail pre-mangled name).
extern "C" void func_80138078__FUl(u32 sound);
// Rank-item pane text setter (retail symbol is the C++-mangled name).
void func_80136910(nw4r::lyt::Layout* layout, char* paneName, u8 value);
// Retail ctor symbol is the unmangled `__ct__CMenuGetItemMulti` (constructs
// the singleton stored in lbl_eu_80664414); 8 reg args + a byte on the stack.
extern "C" u8* __ct__CMenuGetItemMulti(u8* obj, CScn* pScene, u32 a, u32 b, u32 c,
                                       u32 d, u32 e, u32 f, u8 g);

// C++-linkage import: retail symbol is the mangled findObjectById__Fi
// (actor id -> action source).
int findObjectById(int id);

// Character-record lookup helpers (func_801B78B4's category-13 display):
// func_8009EC9C fetches the manager object for a slot id, and
// func_800A32BC reads the character index from it.
extern "C" u32 func_800A32BC(void* mgr);
// Active-character id read off the slot-manager object
// (func_801B6184's equipped-item window scaling).
extern "C" u32 func_800A082C(void* mgr);

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

#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"

#include <nw4r/lyt.h>

#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>
#include "kyoshin/IUICf.hpp"

#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp" // CTitleAHelp (0x38 subobject at menu +0x80)
#include "kyoshin/code_80135FDC_Lyt.hpp"

class CScn;

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// 8-byte award list entry (array element).
class CPlayAwardEntry {
public:
    CPlayAwardEntry();
    ~CPlayAwardEntry();

    u32 word0;
    u32 word1;
};

// Entry-array region of CPlayAwardList (+0x94): 512 entries (0x1000 bytes)
// followed by the trailing state fields (page index for entry lookup at +0x100A).
class CPlayAwardEntryArray {
public:
    CPlayAwardEntryArray();

    CPlayAwardEntry mEntries[0x200]; // 0x00-0xFFF
    u8 field_1000;                   // 0x1000
    u8 field_1001;                   // 0x1001
    f32 field_1004;                  // 0x1004
    u8 field_1008[2];                // 0x1008-0x1009: per-page entry counts (field_1008[page])
    u8 mPageIndex;                   // 0x100A: page index for entry lookup (<< 11)
    s16 field_100C;                  // 0x100C (s16; PlayAward_RequestOpen reads it via a u16 cast)
    s16 field_100E;                  // 0x100E (s16; PlayAward_RequestOpen reads it via a u16 cast)
};

// Minimal mirror of cf::CfPadData (see kyoshin/cf/CfPadData.hpp; the real
// struct pulls monolib/core.hpp which this TU's include set avoids).
struct CPlayAwardPadData {
    u32 mPadHeldFlags;          // +0x00: CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // +0x04: CPad::mPressedButtonFlags
    u8 _pad[0xF8 - 0x08];       // +0x08-0xF7: rest of CPad
    u32 mHeldButtonFlags;       // +0xF8
    u32 mPrevHeldButtonFlags;   // +0xFC
    u32 mPressedButtonFlags;    // +0x100
    u32 mTurboPressButtonFlags; // +0x104
};

// Award texture object returned by ArcResourceAccessor::GetResource("timg")
// (CQstLogList's CQstLogListMsgObj pattern): chain pointer at +0x08, the
// chain's coords at +0x00 (column u16 at +0x00, row u16 at +0x02).
struct CPlayAwardMsgChain {
    struct CPlayAwardCoords* pCoords; // +0x00
};
struct CPlayAwardCoords {
    u16 col; // +0x00
    u16 row; // +0x02
};
struct CPlayAwardMsgObj {
    u32 field_0;                // +0x00
    u32 field_4;                // +0x04
    CPlayAwardMsgChain* chain;  // +0x08
};

// Mirror of nw4r::lyt::Pane's protected mSize region (cf. CSortMenu.hpp).
struct CPlayAwardPaneSize {
    u8 pad[0x4C];  // +0x00-0x4B
    f32 width;     // +0x4C
    f32 height;    // +0x50
};

// View into a nw4r::lyt::Pane's trailing user-data region: OnFileEvent writes
// the tag-processor pointer (field_0x2C) into the pane at +0xF8 after binding
// each award pane's name.
struct CPlayAwardPaneUser {
    u8 pad[0xF8];   // +0x00-0xF7
    u32 field_F8;   // +0xF8
};

// View into the object returned by CDeviceFont::getFontInfo (cf.
// CMenuBattleCommu.hpp): with MWCC's 2-entry vtable prefix the 8th virtual
// lands at +0x24 and returns the font handle passed to ApplyFontToTxtPanes.
// Never instantiated here, so no vtable is emitted.
class CPlayAwardFontObj {
public:
    virtual void vf08();              // +0x08
    virtual void vf0C();              // +0x0C
    virtual void vf10();              // +0x10
    virtual void vf14();              // +0x14
    virtual void vf18();              // +0x18
    virtual void vf1C();              // +0x1C
    virtual void vf20();              // +0x20
    virtual u32 getFontHandle();      // +0x24
};

// Object behind CPlayAwardList::field_0x2C: only observed uses are `delete`
// through the virtual dtor (vtable +0x08) and passing the pointer bits as the
// second award text attr (PlayAward_RefreshEntryPanes). Never constructed here, so the dtor
// stays undefined and no vtable is emitted.
class CPlayAwardAttrObj {
public:
    virtual ~CPlayAwardAttrObj();
};

class CPlayAwardList : public IWorkEvent {
public:
    CPlayAwardList(int arg = 0); // retail passes 0; the arg is unused by the ctor body
    virtual ~CPlayAwardList();
    virtual bool OnFileEvent(CEventFile* event);

    // Layout (IWorkEvent base at 0x00-0x03, then members):
    // 0x04-0x13: UnkClass_8045F564 layout memory region
    // 0x14/0x18: CDeviceFile read handles (readFile / readCommonArchiveFile)
    // 0x20:      Layout* render target (drawn by PlayAward_DrawList)
    // 0x24:      AnimTransform* reverse-advance target (PlayAward_FinishCloseAnimB/80271480)
    // 0x28:      AnimTransform* reverse-advance target (PlayAward_FinishCloseAnimA)
    // 0x30-0x47: cursor region (drawn via CBaseCur::DrawLayout)
    // 0x48-0x87: CScrollBar region (0x40, opaque)
    // 0x88:      render gate (nonzero enables PlayAward_DrawList) | 0x89: state byte
    // 0x8A/0x8B: flag bytes
    // 0x8C/0x8E: s16 scroll position fields | 0x90: page byte | 0x94: entry array
    UnkClass_8045F564 mMemRegion;           // 0x04-0x13: layout memory region
    CFileHandle* mFileHandle;               // 0x14: readFile handle
    CFileHandle* mFileHandle2;              // 0x18: readCommonArchiveFile handle
    nw4r::lyt::ArcResourceAccessor* mArcAccessor1C; // 0x1C: layout arc accessor (freed by PlayAward_ReleaseList)
    nw4r::lyt::Layout* mLayout20;           // 0x20: layout drawn by PlayAward_DrawList
    nw4r::lyt::AnimTransform* mAnimTrans24; // 0x24
    nw4r::lyt::AnimTransform* mAnimTrans28; // 0x28
    u32 field_0x2C;                         // 0x2C: second award text color/attr (PlayAward_RefreshEntryPanes)
    CCur18 mCursor;                         // 0x30-0x47
    CScrollBar mScrollBar;                  // 0x48-0x87
    u8 field_0x88;                          // 0x88: render gate checked by PlayAward_DrawList
    u8 field_0x89;                          // 0x89: state byte (5/0/2 on transitions)
    u8 field_0x8A;                          // 0x8A: checked by PlayAward_IsListReady
    u8 field_0x8B;                          // 0x8B: some state/flag byte
    s16 field_0x8C;                         // 0x8C: scroll position (halfword)
    s16 field_0x8E;                         // 0x8E: scroll offset (halfword)
    s8 field_0x90;                          // 0x90: page byte used by PlayAward_GetPageCue
    CPlayAwardEntryArray mEntryArray;       // 0x94: 512-entry array + state fields
};

class CMenuPlayAward : public IUICf, public IScnRender {
public:
    CMenuPlayAward(CScn* scene);
    virtual ~CMenuPlayAward();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    static CMenuPlayAward* create(CProcess* parent, CScn* scene);
    static CMenuPlayAward* sInstance;

    // IUICf task base (0x00-0x57) and the IScnRender subobject (0x58) come first.
    CScn* mScene;                  // 0x5C: owning scene
    CBgTex mBgTex;                 // 0x60-0x7F
    CTitleAHelp mTitleAHelp;       // 0x80-0xB7
    CPlayAwardList mPlayAwardList; // 0xB8-0x115B
    f32 mField115C;                // 0x115C: float state (ctor: lbl_eu_80668998)
    u8 mField1160;                 // 0x1160: help-display byte (set to 2 by PlayAward_Phase1_Advance)
};

// MWCC runtime operator delete (freeing dtor helper).

// .sdata2 float pool: 1.0f -- animation frame step for the award list
// open/close transitions (PlayAward_FinishCloseAnimA / PlayAward_FinishCloseAnimB / PlayAward_FinishOpenAnimA).
extern const float lbl_eu_806689C0;

// .sdata2 float pool: entry-array init constant (PlayAward_InitEntryArray) and the
// scrollbar init vector for PlayAward_RequestOpen (x/y at 806689B8/BC, z at 806689A8).
extern const float lbl_eu_806689A8;
extern const float lbl_eu_806689B8;
extern const float lbl_eu_806689BC;

// .sdata2 float pool: award-list menu frame-step (8066899C) and clamp
// (806689A0) for the mField115C accumulator in PlayAward_Phase2_HandleInput.
extern const float lbl_eu_8066899C;
extern const float lbl_eu_806689A0;

// .sdata2 double pool: 2^52 (0x4330000000000000) -- unsigned->float conversion
// constant for the award texture row/column sizes (PlayAward_RefreshPageTitles). The lfd
// reloc binds a TU-local pool entry (@N) that cannot be named in source
// (makecrystal precedent); the value matches the retail symbol.
extern const double lbl_eu_806689B0;

// Loaded-BDAT pointer for the award tables (stored by OnFileEvent branch 2
// from getFP; read by PlayAward_BuildEntries's sort/list build).
extern void* lbl_eu_806648A8;

// Second string pool (split .rodata): title/help arc paths used by Init.
extern char lbl_eu_8050E7A0[];

// .sdata2 float pool: award-list page-progress scale for PlayAward_BuildEntries's
// field_1004 ratio (const * field_1000 / field_1001).
extern const float lbl_eu_806689AC;

// Render-gate mode bitfield (.sbss; bit 21 = task busy, gates cbRenderBefore).
extern u32 lbl_eu_80663E28;

// Shared string pool (split .rodata): award-list arc file paths at +0x5A / +0x74.
extern char lbl_eu_8050E7C0[];



// .sdata2 float pool: menu init float (ctor stores to +0x115C).
extern const float lbl_eu_80668998;

// MEM2 alloc-handle query (retail-unmangled; C linkage so the zero-arg name
// stays bare instead of mangling to KyoshinHeap_GetField44__Fv).
extern "C" int KyoshinHeap_GetField44();

// Scene-active query (retail-unmangled; gates cbRenderBefore draws).

// DrawInfo layout setup (retail MANGLED name SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo;
// declared with C++ linkage so MWCC re-derives the mangled reloc).

// Retail-unmangled free functions of this unit (US retail strips the
// mangling). C linkage keeps call relocs on the bare retail names;
// __declspec(noinline) keeps the same-TU calls as real `bl` instructions.
// Return types are int (not u8) so callers compare with cmpwi directly.
extern "C" int __declspec(noinline) PlayAward_IsReadyFlag(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_BindCloseAnims(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_BindOpenAnims(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_RefreshCursor(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_ReleaseList(CPlayAwardList* self);
extern "C" int __declspec(noinline) PlayAward_IsListReady(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_RequestOpen(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_FinishOpenAnimA(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_FinishOpenAnimB(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_FinishCloseAnimA(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_FinishCloseAnimB(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_RefreshEntryPanes(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_FlipPage(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_MoveCursorUp(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_ScrollPageDown(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_MoveCursorDown(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_LoadEntries(CPlayAwardList* self);
extern "C" void __declspec(noinline) PlayAward_RefreshPageTitles(CPlayAwardList* self);
extern "C" void PlayAward_BuildEntries(CPlayAwardEntryArray* self);
extern "C" void __declspec(noinline) PlayAward_DrawList(CPlayAwardList* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" __declspec(noinline) u8* PlayAward_FindEntry(CPlayAwardEntryArray* self, int param);

// Menu phase handlers + per-frame list update (retail-unmangled names; same
// C-linkage scheme as the rest of this unit -- keeps call relocs on the bare
// retail names).
extern "C" void PlayAward_Phase0_ShowHelp(CMenuPlayAward* self);
extern "C" void PlayAward_Phase1_Advance(CMenuPlayAward* self);
extern "C" void PlayAward_Phase2_HandleInput(CMenuPlayAward* self);
extern "C" void PlayAward_Phase3_RequestClose(CMenuPlayAward* self);
extern "C" void PlayAward_UpdateList(CPlayAwardList* self);
extern "C" void PlayAward_LoadLayouts(CPlayAwardList* self);
extern "C" void PlayAward_CopyEntry(CPlayAwardEntry* dst, const CPlayAwardEntry* src);
extern "C" void PlayAward_ScrollPageUp(CPlayAwardList* self);
extern "C" s32 __declspec(noinline) PlayAward_GetPageCue(CPlayAwardList* self);

// Award-list state handlers / helpers (retail-unmangled names; see above).
extern "C" void __declspec(noinline) PlayAward_RequestClose(CPlayAwardList* self);


// Title/help bar idle query -- free-function form of CTitleAHelp::isIdle()
// (retail reloc is the mangled member name; declared with int return so the
// caller's `!= 0` test compiles to a direct cmpwi, cf. CMenuTutorialList.hpp).

// CBgTex/CTitleAHelp widget helpers (retail-unmangled callee names; US retail
// strips the mangling for these widget helpers, cf. CMenuTutorialList.hpp).
// Return types are int (not u8) so callers compare with cmpwi directly.
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void teardown(CTitleAHelp* self);
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);

// Per-frame widget updates (retail-unmangled; called by CMenuPlayAward::Move).
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void updateHelp(CTitleAHelp* self);

// Title/help bar close + mode-set helpers (PlayAward_Phase2_HandleInput).
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);

// CfGameManager controller-type query + save/skip busy queries (retail
// pre-mangled / unmangled C symbols; same scheme as CMenuTutorialList.hpp).
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();

// Shared layout-arc resource manager (retail-unmangled; ArcResourceAccessor
// with GetResource at vtable +0x0C, used by PlayAward_RefreshPageTitles).

// CfGameManager controller-type query (retail pre-mangled name; declared
// under C linkage so calls bind the literal identifier).
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);

// Cursor draw import (retail-unmangled name).

// Cursor/scrollbar helpers used by the close sequence (PlayAward_RequestClose).

// Layout text-binding helper (retail-unmangled; 3rd arg is the raw text word).

// Pane-position translation helper (retail-unmangled): resolve a position in
// pane1's space into pane2/root space (cf. CSelShopWin/CSysWin usage).

// Scroll-bar layout load + per-frame update/draw helpers, and the cursor
// update helper (retail-unmangled names; see above).

// Class_8045F858 scratch-region RAII guard used by OnFileEvent around the
// layout build (retail C-linkage names; the class itself is declared in
// monolib/lib/UnkClass_8045F564.hpp).
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);

// OnFileEvent / Init layout-build imports (retail-unmangled C symbols).
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" void* __ct__CTagProcessor(void*);
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);
extern "C" u32 CtrlRemote_TouchBitByArg(u32);
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.



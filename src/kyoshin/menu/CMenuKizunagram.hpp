#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/CFade.hpp"
#include "kyoshin/CTitleAHelp.hpp"

// Shield CLoad.hpp's C++-linkage helper declarations: this TU needs the
// CLoad class, but declares the func_802AExxxx helpers itself below as
// extern "C" so call relocs carry the retail UNMANGLED symbol names
// (identical plain decls emit mangled __FP5CLoad refs). MWCC 10505.
#define CLoadBeginFileRequest CLoadBeginFileRequest_CLoadHdr
#define func_802AE560 func_802AE560_CLoadHdr
#define CLoadDrawIfVisible CLoadDrawIfVisible_CLoadHdr
#define CLoadTeardownLayout CLoadTeardownLayout_CLoadHdr
#define CLoadIsLoadReady CLoadIsLoadReady_CLoadHdr
#define CLoadIsLoaded CLoadIsLoaded_CLoadHdr
#define CLoadIsAnimSettled CLoadIsAnimSettled_CLoadHdr
#define CLoadStartFadeInStep CLoadStartFadeInStep_CLoadHdr
#define CLoadStartRetryStep CLoadStartRetryStep_CLoadHdr
#define CLoadAdvanceFadeHold CLoadAdvanceFadeHold_CLoadHdr
#define CLoadFinishRetryIdle CLoadFinishRetryIdle_CLoadHdr
#include "kyoshin/CLoad.hpp"
#undef CLoadBeginFileRequest
#undef func_802AE560
#undef CLoadDrawIfVisible
#undef CLoadTeardownLayout
#undef CLoadIsLoadReady
#undef CLoadIsLoaded
#undef CLoadIsAnimSettled
#undef CLoadStartFadeInStep
#undef CLoadStartRetryStep
#undef CLoadAdvanceFadeHold
#undef CLoadFinishRetryIdle

// Opaque kizuna-chart sub-object at +0x98 (spans 0x98..0x178). Driven by
// kizChartOpen (CKizunagram unit); detailed layout TBD.
struct UnkKizunaMenuSub98 {
    u8 data[0xE0];
};

// Opaque slot for the CPcKizunagram sub-object at +0x178 (sizeof 0x4C).
// CPcKizunagram.hpp is not included here: its extern "C" KizunagramIsOpen
// declares a u8 return, which makes MWCC emit an rlwinm byte mask at the
// call site; retail compares r3 directly, so the call-site declaration must
// return int.
struct UnkKizunaMenuPcSub {
    u8 data[0x4C];
};

// Kizuna gram (affinity chart) screen shown on the World Map.
// Inherits from CProcess (task system) and owns an IScnRender render-callback
// subobject at offset +0x58 (requires this-adjusting thunks).
//
// Layout (mirrors CMenuMapSelectSC; recovered from retail member offsets):
//   0x00: CProcess
//   0x3C-0x53: null PMF callback slots (2 groups of 3 words)
//   0x54/0x55: u8 flags
//   0x58: IScnRender (member subobject, vptr)
//   0x5C: CProcess* parent reference
//   0x60: CTitleAHelp
//   0x98: kizuna chart sub-object (opaque, CKizunagram 0xE0)
//  0x178: CPcKizunagram
//  0x1C4: CLoad
//  0x1F4: CFade
//  0x21C: u8 state
class CMenuKizunagram : public CProcess {
public:
    CMenuKizunagram();
    virtual ~CMenuKizunagram();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void KizunagramRenderBeforeThunk();
    void KizunagramDeleteDtorThunk();

    u32 ptmf0[3];                    // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];                    // 0x48-0x53: null PMF callback slot group 2
    u8 field_0x54;                   // 0x54
    u8 field_0x55;                   // 0x55
    u8 _pad56[2];                    // 0x56-0x57
    IScnRender mIScnRender;          // 0x58: render-callback subobject (vptr)
    CProcess* mParentRef;            // 0x5C
    CTitleAHelp mTitleAHelp;         // 0x60 (sizeof 0x38)
    UnkKizunaMenuSub98 mSub98;       // 0x98 (opaque CKizunagram slot, 0xE0)
    UnkKizunaMenuPcSub mPcKizunagram; // 0x178 (CPcKizunagram slot)
    CLoad mLoad;                     // 0x1C4 (sizeof 0x30)
    CFade mFade;                     // 0x1F4 (sizeof 0x28)
    u8 field_0x21C;                  // 0x21C
    u8 field_0x21D;                  // 0x21D
    u8 field_0x21E;                  // 0x21E
};

// CScn / CTaskGame forward declarations (their full headers pull broken
// monolib umbrella headers; keep the pointers opaque, same scheme as
// CMenuBattleEnd / CMenuMapSelectSC).
class CScn;
class CTaskGame;

// Retail-unmangled callee names (US strips mangling for these func_ helpers);
// declared extern "C" so call sites emit the literal retail reloc names.
// Return types are int (not u8) so callers compare with cmpwi directly
// (no byte mask), matching retail.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
extern "C" int kizChartOpen(UnkKizunaMenuSub98* self);
extern "C" int KizunagramIsOpen(UnkKizunaMenuPcSub* self);
extern "C" int CFade_IsReady(CFade* self);
extern "C" void CFade_FadeIn(CFade* self);

// Additional retail-unmangled callee names for the kizuna-gram screen state
// helpers (same scheme as CMenuMapSelectSC / CMakeCrystalWin). Return types
// are int where retail compares r3 with cmpwi directly (no byte mask);
// kizChartStatus is u8 because retail masks its result with clrlwi.
// Retail-unmangled views of the CLoad helpers (see include shield above).
extern "C" int CLoadIsLoadReady(CLoad* self);
extern "C" void CLoadStartFadeInStep(CLoad* self);
extern "C" void CLoadStartRetryStep(CLoad* self);
extern "C" int CFade_IsVisible(CFade* self);
extern "C" void CFade_FadeOut(CFade* self);
extern "C" int kizChartReady(UnkKizunaMenuSub98* self);
extern "C" u8 kizChartStatus(UnkKizunaMenuSub98* self);
extern "C" void kizStartChart(UnkKizunaMenuSub98* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void reopenFromClose(CTitleAHelp* self);
extern "C" void setNameText(CTitleAHelp* self, char* name);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" char* BdatTouchStringCell(char* a, char* b, int id);

// D2-form subobject/base destructor helpers (explicit delete flags), used by
// the free-function form of ~CMenuKizunagram. __dt__800FED0C is the CProcess
// base dtor retail symbol (the game-side D2 wrapper, distinct from the
// library D1 __dt__8CProcessFv). __dt__13CPcKizunagramFv / __dt__11CKizunagramFv
// are the retail dtor names for the opaque sub-objects at +0x178 / +0x98.
extern "C" void __dt__5CFadeFv(CFade* _this, int flags);
extern "C" void __dt__5CLoadFv(CLoad* _this, int flags);
extern "C" void __dt__13CPcKizunagramFv(UnkKizunaMenuPcSub* _this, int flags);
extern "C" void __dt__11CKizunagramFv(UnkKizunaMenuSub98* _this, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* _this, int flags);
extern "C" void __dt__800FED0C(CProcess* _this, int flags);

// Retail constructor (unmangled global, ctor with two explicit args) and the
// D2-form destructor (keeps its C++ mangling in US). The IScnRender dtor
// thunk references __dt__15CMenuKizunagramFv by this symbol.
extern "C" CMenuKizunagram* __ct__CMenuKizunagram(CMenuKizunagram* self, int a, int b);
extern "C" CMenuKizunagram* __dt__15CMenuKizunagramFv(CMenuKizunagram* _this, int flags);

// C++-linkage helper - retail emits the mangled form playUISound__FUl.
void playUISound(unsigned long op);

// Scene/task-system helpers (retail-unmangled callee names).
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
extern "C" int Scn_SetPauseFlag(CScn* scn, int flag);
extern "C" CTaskGame* getInstance__9CTaskGameFv();
extern "C" int isFlag01Set__9CTaskGameFv();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);

// code_80135FDC helpers (retail-unmangled).
extern "C" u8 DecMenuCounter64080();
extern "C" u8 code80135FDC_getByte_64080();
extern "C" void func_801338C8();
extern "C" int IsMenuState621F0();

// CTitleAHelp / CFade helpers used by Term / cbRenderBefore (retail strips
// the mangling for these member helpers).
extern "C" void teardown(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void markReplayClose(CTitleAHelp* self);
extern "C" void CFade_Draw(CFade* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void CFade_Unload(CFade* self);

// CLoad draw/reset helpers.
extern "C" void CLoadTeardownLayout(CLoad* self);
extern "C" void CLoadDrawIfVisible(CLoad* self, nw4r::lyt::DrawInfo* drawInfo);

// Kizuna-chart sub-object helpers (retail-unmangled; CKizunagram unit).
extern "C" void teardownKizuna(UnkKizunaMenuSub98* self);
extern "C" void drawKizLayouts(UnkKizunaMenuSub98* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void kizOpenWinNop(UnkKizunaMenuSub98* self);
extern "C" int kizChartBusy(UnkKizunaMenuSub98* self);
extern "C" int kizHasSelFlag(UnkKizunaMenuSub98* self);
extern "C" void kizCloseChart(UnkKizunaMenuSub98* self, int arg);
extern "C" void kizToggleHelp(UnkKizunaMenuSub98* self);
extern "C" void kizCursorHoldA(UnkKizunaMenuSub98* self);
extern "C" void kizCursorHoldB(UnkKizunaMenuSub98* self);
extern "C" void kizCursorDir1(UnkKizunaMenuSub98* self);
extern "C" void kizCursorDir2(UnkKizunaMenuSub98* self);
extern "C" void kizCursorDir3(UnkKizunaMenuSub98* self);
extern "C" void kizCursorDir4(UnkKizunaMenuSub98* self);
extern "C" void tickKizMove(UnkKizunaMenuSub98* self);
extern "C" void beginClose(CTitleAHelp* self);
// CPcKizunagram helpers. Return types are int (not u8) so callers compare
// with cmpwi directly (no byte mask), matching retail.
extern "C" int KizunagramIsHidden(UnkKizunaMenuPcSub* self);
extern "C" void func_8025D9C4(UnkKizunaMenuPcSub* self);
extern "C" void KizunagramDraw(UnkKizunaMenuPcSub* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void KizunagramOpen(UnkKizunaMenuPcSub* self);
extern "C" void KizunagramClose(UnkKizunaMenuPcSub* self);
extern "C" void KizunagramCursorUp(UnkKizunaMenuPcSub* self);
extern "C" void KizunagramCursorDown(UnkKizunaMenuPcSub* self);
extern "C" void KizunagramCursorPageUp(UnkKizunaMenuPcSub* self);
extern "C" void KizunagramCursorPageDown(UnkKizunaMenuPcSub* self);

// Move() state-dispatch helpers (retail-unmangled; defined in this TU).
// Declared extern "C" so the switch-dispatch call sites in Move() emit the
// literal retail reloc names (the definitions below inherit C linkage).
extern "C" void KizunagramBeginFadeIn(CMenuKizunagram* self);
extern "C" void func_80257360(CMenuKizunagram* self);
extern "C" void KizunagramConfirmChartAdvance(CMenuKizunagram* self);
extern "C" void KizunagramOpenCharWindow(CMenuKizunagram* self);
extern "C" void func_80257498(CMenuKizunagram* self);
extern "C" void KizunagramSetPhaseFlag54(CMenuKizunagram* self);
extern "C" void KizunagramAdvanceToPcWin(CMenuKizunagram* self);
extern "C" void KizunagramAdvanceToState9(CMenuKizunagram* self);
extern "C" void func_80257840(CMenuKizunagram* self);
extern "C" void KizunagramAdvanceToState7(CMenuKizunagram* self);
extern "C" void KizunagramReopenCharWindow(CMenuKizunagram* self);

// Move() per-frame sub-object refresh helpers (retail-unmangled).
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void tickKizMain(UnkKizunaMenuSub98* self);
extern "C" void KizunagramUpdateMainState(UnkKizunaMenuPcSub* self);
extern "C" void func_802AE560(CLoad* self);
extern "C" void CFade_Update(CFade* self);

// Sound helpers used by Move() (retail-unmangled).
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();

// Sub-object / base constructors (retail-unmangled ctor names).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__CKizunagram(UnkKizunaMenuSub98* self, int arg);
extern "C" void __ct__CPcKizunagram(UnkKizunaMenuPcSub* self);
extern "C" void __ct__CLoad(CLoad* self, u8 arg);
extern "C" void __ct__CFade(CFade* self);

// Ctor tail helpers (retail-unmangled).
extern "C" void CTaskGame_deleteLoad();
extern "C" void MiniMapSetActiveFlag();

// C++-linkage helper - retail emits the mangled form
// func_80137250__FPQ34nw4r3lyt8DrawInfo.
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
// nw4r DrawInfo pre-mangled ct/dt names (retail emits the direct calls; a
// C++ local would virtual-dispatch its scope-exit destructor and bloat the
// body).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);

// Flat pad-data view exposing only the flag words func_80257840 reads:
// CPad::mPressedButtonFlags at +0x4 and CfPadData::mTurboPressButtonFlags at
// +0x104 (see kyoshin/cf/CfPadData.hpp for the full layout).
struct KizunaPadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of the pad data
    u32 mTurboPressButtonFlags; // 0x104
};
extern "C" KizunaPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

// Data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_806647E0;    // CMenuKizunagram singleton pointer (sbss)
extern char lbl_eu_8050CAB8[]; // kizuna-gram string pool base
extern u32 lbl_eu_80663E28;    // global mode bitfield (bit 0x200000 = kizuna gate)
extern u8 lbl_eu_8052BF70[];   // temporary vtable (pre-PMF copy)
extern u8 lbl_eu_805374B0[];   // final vtable; IScnRender vtable at +0x24
extern u32 __ptmf_null[3];     // null pointer-to-member-function constant

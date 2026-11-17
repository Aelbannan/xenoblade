// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/CTaskGame.hpp"
#include <new>
#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuKizunagram.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/core/CPadManager.hpp"   // CPad flag words (getCurrentPad)
#include "kyoshin/cf/CfGameManager.hpp"   // cf::CfGameManager::getCurrentPad
#include "kyoshin/cf/CfPadData.hpp"        // cf::CfPadData layout
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/CFade.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Shield CLoad.hpp's C++-linkage helper declarations: this TU needs the
// CLoad class, but declares the func_802AExxxx helpers itself below as
// extern "C" so call relocs carry the retail UNMANGLED symbol names
// (identical plain decls emit mangled __FP5CLoad refs). MWCC 10505.
#define CLoadBeginFileRequest CLoadBeginFileRequest_CLoadHdr
#define CLoadUpdateLayout CLoadUpdateLayout_CLoadHdr
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
#include "kyoshin/code_80135FDC_Bdat.hpp"
#undef CLoadBeginFileRequest
#undef CLoadUpdateLayout
#undef CLoadDrawIfVisible
#undef CLoadTeardownLayout
#undef CLoadIsLoadReady
#undef CLoadIsLoaded
#undef CLoadIsAnimSettled
#undef CLoadStartFadeInStep
#undef CLoadStartRetryStep
#undef CLoadAdvanceFadeHold
#undef CLoadFinishRetryIdle

// CScn / CTaskGame forward declarations (their full headers pull broken
// monolib umbrella headers; keep the pointers opaque, same scheme as
// CMenuBattleEnd / CMenuMapSelectSC).
class CScn;
class CTaskGame;

// Raw-storage slots of the CLoad / CFade members (constructed by hand).
static inline CLoad* loadOf(CMenuKizunagram* self) { return reinterpret_cast<CLoad*>(self->mLoad); }
static inline CFade* fadeOf(CMenuKizunagram* self) { return reinterpret_cast<CFade*>(self->mFade); }

// Retail-unmangled callee names (US strips mangling for these func_ helpers);
// declared extern "C" so call sites emit the literal retail reloc names.
// Return types are int (not u8) so callers compare with cmpwi directly
// (no byte mask), matching retail.
extern "C" int kizChartOpen(UnkKizunaMenuSub98* self);
extern "C" int KizunagramIsOpen(UnkKizunaMenuPcSub* self);
extern "C" int CFade_IsReady(CFade* self);
// (memberized: CFade_FadeIn -> CFade::In; declaration moved into class CFade)

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

extern "C" void __dt__5CFadeFv(CFade* _this, int flags);
extern "C" void __dt__5CLoadFv(CLoad* _this, int flags);
extern "C" void __dt__13CPcKizunagramFv(UnkKizunaMenuPcSub* _this, int flags);
extern "C" void __dt__11CKizunagramFv(UnkKizunaMenuSub98* _this, int flags);


// C++-linkage helper - retail emits the mangled form playUISound__FUl.

// Scene/task-system helpers (retail-unmangled callee names).
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
extern "C" int Scn_SetPauseFlag(CScn* scn, int flag);
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);

// code_80135FDC helpers (retail-unmangled).
extern "C" void CUICfManager_createQueuedMenus();

// CTitleAHelp / CFade helpers used by Term / cbRenderBefore (retail strips
// the mangling for these member helpers).
extern "C" void teardown(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void markReplayClose(CTitleAHelp* self);
extern "C" void CFade_Draw(CFade* self, nw4r::lyt::DrawInfo* drawInfo);
// (memberized: CFade_Unload -> CFade::Unload; declaration moved into class CFade)

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
extern "C" void KizunagramTerm(UnkKizunaMenuPcSub* self);
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
extern "C" void KizunagramDriveChart(CMenuKizunagram* self);
extern "C" void KizunagramSetPhaseFlag54(CMenuKizunagram* self);
extern "C" void KizunagramAdvanceToPcWin(CMenuKizunagram* self);
extern "C" void KizunagramAdvanceToState9(CMenuKizunagram* self);
extern "C" void KizunagramHandleCharInput(CMenuKizunagram* self);
extern "C" void KizunagramAdvanceToState7(CMenuKizunagram* self);
extern "C" void KizunagramReopenCharWindow(CMenuKizunagram* self);

// Move() per-frame sub-object refresh helpers (retail-unmangled).
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void tickKizMain(UnkKizunaMenuSub98* self);
extern "C" void KizunagramUpdateMainState(UnkKizunaMenuPcSub* self);
extern "C" void CLoadUpdateLayout(CLoad* self);
extern "C" void CFade_Update(CFade* self);

// Sound helpers used by Move() (retail-unmangled).
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();

extern "C" void __ct__CKizunagram(UnkKizunaMenuSub98* self, int arg);
extern "C" void __ct__CPcKizunagram(UnkKizunaMenuPcSub* self);
extern "C" void __ct__CLoad(CLoad* self, u8 arg);
extern "C" void __ct__5CFadeFv(CFade* self);

// Ctor tail helpers (retail-unmangled).
extern "C" void CTaskGame_deleteLoad();
extern "C" void MiniMapSetActiveFlag();

// C++-linkage helper - retail emits the mangled form
// SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo.

// Flat pad-data view exposing only the flag words KizunagramHandleCharInput reads:
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


// ---------------------------------------------------------------------------
// CMenuKizunagram::CMenuKizunagram (us-80258ba4) - constructor with two args:
// `parentRef` becomes the parent-scene reference at 0x5C, `b` the byte at
// 0x21E. The IUICf base (vtables, PMF slots, 0x54/0x55 flags) and the
// IScnRender base are built by the compiler; the CTitleAHelp member is a real
// member. The opaque sub-object regions are constructed by hand in ascending
// address order.
// ---------------------------------------------------------------------------
CMenuKizunagram::CMenuKizunagram(CProcess* parentRef, int b)
    : mParentRef(parentRef), mTitleAHelp(0, 0) {
    __ct__CKizunagram(&mSub98, 0);
    __ct__CPcKizunagram(&mPcKizunagram);
    __ct__CLoad(loadOf(this), 0);
    __ct__5CFadeFv(fadeOf(this));

    field_0x21C = 0;
    field_0x21D = 1;
    field_0x21E = (u8)b;
    CTaskGame_deleteLoad();
    MiniMapSetActiveFlag();
}

// ---------------------------------------------------------------------------
// ~CMenuKizunagram (us-80258c90). Opaque sub-objects are torn down by hand in
// reverse construction order; the compiler then destroys mTitleAHelp and the
// IUICf / IScnRender bases.
// ---------------------------------------------------------------------------
CMenuKizunagram::~CMenuKizunagram() {
    __dt__5CFadeFv(fadeOf(this), -1);
    __dt__5CLoadFv(loadOf(this), -1);
    __dt__13CPcKizunagramFv(&mPcKizunagram, -1);
    __dt__11CKizunagramFv(&mSub98, -1);
}

void CMenuKizunagram::Init() {}

void CMenuKizunagram::Term() {
    waitForDrawDone__9CDeviceVIFv();
    Scn_SetPauseFlag(reinterpret_cast<CScn*>(mParentRef), 1);

    removeRenderCB__4CScnFP10IScnRender(reinterpret_cast<CScn*>(mParentRef), this);

    teardown(&mTitleAHelp);
    teardownKizuna(&mSub98);
    KizunagramTerm(&mPcKizunagram);
    CLoadTeardownLayout(loadOf(this));
    (fadeOf(this))->Unload();

    lbl_eu_806647E0 = 0;
    DecMenuCounter64080();
    if (code80135FDC_getByte_64080() == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(0);
    }
    CUICfManager_createQueuedMenus();
}

void CMenuKizunagram::Move() {
    // Gate: when the global mode bit (0x200000) is set, skip the kizuna
    // Move body. The `if (A && B) goto body; goto end; end: return; body:`
    // chain (exit label BEFORE the body label) keeps the body off the
    // fallthrough so MWCC emits retail's branch-over-branch: `bne end` for
    // the first disjunct, `beq body; b end` for the second (MWCC_CASES
    // §&&-gate branch-over-branch).
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() == 0 && (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    // Confirm-press (classic bit 23 / wiimote bit 10) while the screen is
    // active skips the intro: play the SE and jump straight to state 8.
    if (field_0x21C != 0) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        int bit;
        if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
            bit = (pad->mPressedButtonFlags >> 23) & 1;
        } else {
            bit = (pad->mPressedButtonFlags >> 10) & 1;
        }
        if (bit != 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
            }
            playUISound(6);
            field_0x21C = 8;
            mField54 = 1;
        }
    }
    // Per-state helpers (0..10), then per-frame sub-object refresh.
    switch (field_0x21C) {
    case 0: KizunagramBeginFadeIn(this); break;
    case 1: func_80257360(this); break;
    case 2: KizunagramConfirmChartAdvance(this); break;
    case 3: KizunagramOpenCharWindow(this); break;
    case 4: KizunagramDriveChart(this); break;
    case 5: KizunagramSetPhaseFlag54(this); break;
    case 6: KizunagramAdvanceToPcWin(this); break;
    case 7: KizunagramAdvanceToState9(this); break;
    case 8: KizunagramHandleCharInput(this); break;
    case 9: KizunagramAdvanceToState7(this); break;
    case 10: KizunagramReopenCharWindow(this); break;
    }
    updateHelp(&mTitleAHelp);
    tickKizMain(&mSub98);
    KizunagramUpdateMainState(&mPcKizunagram);
    CLoadUpdateLayout(loadOf(this));
    CFade_Update(fadeOf(this));
}

void CMenuKizunagram::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0) {
        return;
    }
    // Gate: when the global mode bit (0x200000) is set, skip the kizuna
    // render. The adjacent `goto end; goto body;` pair hits MWCC's
    // branch-over-branch peephole, emitting retail's beq-into-body / b-end.
    if ((lbl_eu_80663E28 & 0x200000) != 0) {
        goto end;
    }
    goto body;
body:
    if (IsMenuState621F0() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawKizLayouts(&mSub98, &drawInfo);
    KizunagramDraw(&mPcKizunagram, &drawInfo);
    // Draw the title/help bar once the per-character window is past the
    // opening phase (state >= 8) or when its visibility flag is set.
    if (field_0x21D != 0 || field_0x21C >= 8) {
        drawHelp(&mTitleAHelp, &drawInfo);
    }
    CFade_Draw(fadeOf(this), &drawInfo);
    CLoadDrawIfVisible(loadOf(this), &drawInfo);
end:
    ;
}

// ---------------------------------------------------------------------------
// KizunagramCreateSingleton (us-802594c8) - create the kizuna-gram singleton: returns 0
// when it already exists, otherwise allocates the object (0x220 bytes),
// constructs it, stores the singleton and registers it as a CProcess child
// of `parent`.
// ---------------------------------------------------------------------------
extern "C" CMenuKizunagram* KizunagramCreateSingleton(CProcess* parent, int a, int b) {
    if (lbl_eu_806647E0 != 0) {
        return 0;
    }

    CMenuKizunagram* obj = (CMenuKizunagram*)mtl::MemManager::allocate(
        0x220, CWorkThreadSystem::getWorkMem());
    // Placement-new performs the null check that guards the constructor call
    // (retail's single `if (obj != 0)` branch); assigning the result lets MWCC
    // reuse r3 after the call instead of preserving `obj` in a callee-saved
    // register (retail keeps only 3 saved regs here).
    obj = new (obj) CMenuKizunagram(reinterpret_cast<CProcess*>(a), b);
    lbl_eu_806647E0 = (u32)obj;
    obj->Regist(parent, false);
    return (CMenuKizunagram*)lbl_eu_806647E0;
}



// ---------------------------------------------------------------------------
// KizunagramBeginFadeIn (us-80259554)
// When the fade overlay is loaded/ready, mark the screen active (state byte)
// and start the fade-in animation.
// ---------------------------------------------------------------------------
void KizunagramBeginFadeIn(CMenuKizunagram* self) {
    if (CFade_IsReady(fadeOf(self)) != 0) {
        self->field_0x21C = 1;
        (fadeOf(self))->In();
    }
}

// ---------------------------------------------------------------------------
// func_80257360 (us-8025959c) - when the loading overlay and the fade are
// both ready, start the fade-out (state 2) and dismiss the overlay.
// ---------------------------------------------------------------------------
void func_80257360(CMenuKizunagram* self) {
    if (CLoadIsLoadReady(loadOf(self)) != 0 && CFade_IsVisible(fadeOf(self)) != 0) {
        self->field_0x21C = 2;
        CLoadStartFadeInStep(loadOf(self));
    }
}

// ---------------------------------------------------------------------------
// KizunagramConfirmChartAdvance (us-802595f4) - when the title help and kizuna chart are
// ready, sync the chart selection into the help bar, dismiss both, play the
// confirm sound and advance to state 3.
// ---------------------------------------------------------------------------
void KizunagramConfirmChartAdvance(CMenuKizunagram* self) {
    if (isInitialized(&self->mTitleAHelp) != 0 && kizChartReady(&self->mSub98) != 0) {
        func_801C41E8(&self->mTitleAHelp, kizChartStatus(&self->mSub98));
        func_801C412C(&self->mTitleAHelp);
        kizStartChart(&self->mSub98);
        CLoadStartRetryStep(loadOf(self));
        CFade_FadeOut(fadeOf(self));
        self->field_0x21C = 3;
        playUISound(0x6d);
    }
}

// ---------------------------------------------------------------------------
// KizunagramOpenCharWindow (us-80259684)
// When the title help is idle and the kizuna chart is open, show the
// per-character window (state 4).
// ---------------------------------------------------------------------------
void KizunagramOpenCharWindow(CMenuKizunagram* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        kizChartOpen(&self->mSub98) != 0) {
        self->field_0x21C = 4;
    }
}

// ---------------------------------------------------------------------------
// KizunagramDriveChart (us-802596d4) - chart open state: poll classic/wiimote input
// and drive the kizuna-chart window transitions (state 5/6), the help-bar
// visibility toggle, and the per-direction cursor helpers. Then, when the
// chart is not busy, run the turbo-button cursor helpers (or the default
// chart refresh when none fired).
// ---------------------------------------------------------------------------
void KizunagramDriveChart(CMenuKizunagram* self) {
    if (kizChartOpen(&self->mSub98) == 0) {
        return;
    }
    // Declaration order drives MWCC's callee-saved allocation.
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 f1, f2, f3, f4;
    u32 openWin, dismiss, toggle, cursorA, cursorB;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) == 0) {
        goto wiimote;
    }
    // Classic controller layout (fall-through).
    f1 = !!(pad->mHeldButtonFlags & 0x8004);
    // 0x10008 = bit 16 | bit 3: MWCC splits the over-16-bit mask into
    // rlwinm + rlwimi, matching retail.
    f2 = !!(pad->mHeldButtonFlags & 0x10008);
    f3 = !!(pad->mHeldButtonFlags & 0x2001);
    f4 = !!(pad->mHeldButtonFlags & 0x4002);
    openWin = (pad->mPad.mPressedButtonFlags >> 21) & 1;
    dismiss = (pad->mPad.mPressedButtonFlags >> 22) & 1;
    toggle = (pad->mPad.mPressedButtonFlags >> 24) & 1;
    cursorA = (pad->mPad.mHeldButtonFlags >> 10) & 1;
    cursorB = (pad->mPad.mHeldButtonFlags >> 9) & 1;
    goto merged;
wiimote:
    // Wii Remote layout.
    f1 = !!(pad->mHeldButtonFlags & 0x8004);
    f2 = !!(pad->mHeldButtonFlags & 0x10008);
    f3 = !!(pad->mHeldButtonFlags & 0x2001);
    f4 = !!(pad->mHeldButtonFlags & 0x4002);
    openWin = (pad->mPad.mPressedButtonFlags >> 4) & 1;
    dismiss = (pad->mPad.mPressedButtonFlags >> 5) & 1;
    toggle = (pad->mPad.mPressedButtonFlags >> 11) & 1;
    cursorA = (pad->mPad.mHeldButtonFlags >> 6) & 1;
    cursorB = (pad->mPad.mHeldButtonFlags >> 7) & 1;
merged:
    if (openWin != 0) {
        // Open the per-character window: only when the chart is idle and the
        // follow-up check passes, dismiss the chart and advance to state 6.
        kizOpenWinNop(&self->mSub98);
        if (kizChartBusy(&self->mSub98) == 0 && kizHasSelFlag(&self->mSub98) != 0) {
            markReplayClose(&self->mTitleAHelp);
            kizCloseChart(&self->mSub98, 0);
            self->field_0x21C = 6;
        }
    } else if (dismiss != 0) {
        // Back out of the chart: dismiss the help bar and go to state 5.
        beginClose(&self->mTitleAHelp);
        kizCloseChart(&self->mSub98, 1);
        self->field_0x21C = 5;
    } else if (toggle != 0) {
        // Toggle the help-bar visibility flag and play the corresponding SE.
        self->field_0x21D = (self->field_0x21D ^ 1) != 0;
        kizToggleHelp(&self->mSub98);
        if (self->field_0x21D != 0) {
            playUISound(0xd);
        } else {
            playUISound(0xe);
        }
    } else if (cursorA != 0) {
        kizCursorHoldA(&self->mSub98);
    } else if (cursorB != 0) {
        kizCursorHoldB(&self->mSub98);
    }
    // Mirror the chart selection into the help bar, then run the turbo
    // cursor helpers; refresh the chart only when none of them fired.
    func_801C41E8(&self->mTitleAHelp, kizChartStatus(&self->mSub98));
    if (kizChartBusy(&self->mSub98) == 0) {
        // changed stays 1 only when no turbo helper fired; the default
        // chart refresh then runs.
        int changed = 1;
        if (f1) {
            kizCursorDir1(&self->mSub98);
            changed = 0;
        }
        if (f2) {
            kizCursorDir2(&self->mSub98);
            changed = 0;
        }
        if (f3) {
            kizCursorDir3(&self->mSub98);
            changed = 0;
        }
        if (f4) {
            kizCursorDir4(&self->mSub98);
            changed = 0;
        }
        if (changed) {
            tickKizMove(&self->mSub98);
        }
    }
}

// ---------------------------------------------------------------------------
// KizunagramSetPhaseFlag54 (us-80259940)
// When the title help is idle and the kizuna chart is open, set the phase
// flag at 0x54.
// ---------------------------------------------------------------------------
void KizunagramSetPhaseFlag54(CMenuKizunagram* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        kizChartOpen(&self->mSub98) != 0) {
        self->mField54 = 1;
    }
}

void KizunagramAdvanceToPcWin(CMenuKizunagram* self) {
    // Open the per-character window: once the title bar, kizuna chart and PC
    // window are all idle/ready, set the help text from the string pool,
    // animate it, dismiss the chart and advance to state 8.
    if (self->mTitleAHelp.isIdle() != 0 &&
        kizChartOpen(&self->mSub98) != 0 &&
        KizunagramIsHidden(&self->mPcKizunagram) != 0) {
        char* name = BdatTouchStringCell(lbl_eu_8050CAB8, lbl_eu_8050CAB8 + 0xb, 2);
        setNameText(&self->mTitleAHelp, name);
        func_801C41E8(&self->mTitleAHelp, 0x59);
        reopenFromClose(&self->mTitleAHelp);
        KizunagramOpen(&self->mPcKizunagram);
        self->field_0x21C = 8;
    }
}

// ---------------------------------------------------------------------------
// KizunagramAdvanceToState9 (us-80259a2c)
// When the title help is idle and the per-character window is open, set the
// screen state to 9.
// ---------------------------------------------------------------------------
void KizunagramAdvanceToState9(CMenuKizunagram* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        KizunagramIsOpen(&self->mPcKizunagram) != 0) {
        self->field_0x21C = 9;
    }
}

// ---------------------------------------------------------------------------
// KizunagramHandleCharInput (us-80259a7c) - react to confirm/cancel input while the
// per-character window is open: the classic controller's B/A buttons map to
// bits 10/9 of mPressedButtonFlags, the Wii Remote's to bits 27/26. Turbo
// presses on the classic shoulder/C buttons trigger per-direction cursor
// helpers on the PC window.
// ---------------------------------------------------------------------------
void KizunagramHandleCharInput(CMenuKizunagram* self) {
    if (KizunagramIsOpen(&self->mPcKizunagram) == 0) {
        return;
    }
    KizunaPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    // Declaration order drives MWCC's register colors (a before b);
    // assignment order below drives the load schedule.
    u32 a, b, f1, f2, f3, f4;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        f1 = !!(turbo & 0x8004);
        // 0x10008 = bit 16 | bit 3: MWCC splits the over-16-bit mask into
        // rlwinm + rlwimi, matching retail (cf. KizunagramDriveChart).
        f2 = !!(turbo & 0x10008);
        f3 = !!(turbo & 0x2001);
        b = (pressed >> 21) & 1;
        f4 = !!(turbo & 0x4002);
        a = (pressed >> 22) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        f1 = !!(turbo & 0x8004);
        f2 = !!(turbo & 0x10008);
        f3 = !!(turbo & 0x2001);
        b = (pressed >> 4) & 1;
        f4 = !!(turbo & 0x4002);
        a = (pressed >> 5) & 1;
    }
    if (b) {
        return;
    }
    if (a) {
        markReplayClose(&self->mTitleAHelp);
        KizunagramClose(&self->mPcKizunagram);
        self->field_0x21C = 0xa;
    } else if (f1) {
        KizunagramCursorUp(&self->mPcKizunagram);
    } else if (f2) {
        KizunagramCursorDown(&self->mPcKizunagram);
    } else if (f3) {
        KizunagramCursorPageUp(&self->mPcKizunagram);
    } else if (f4) {
        KizunagramCursorPageDown(&self->mPcKizunagram);
    }
}

// ---------------------------------------------------------------------------
// KizunagramAdvanceToState7 (us-80259bd0) - when the title help is idle and the
// per-character window is open, set the help bar text from the kizuna chart
// string pool, start its animation, dismiss the chart and advance to state 7.
// ---------------------------------------------------------------------------
void KizunagramAdvanceToState7(CMenuKizunagram* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        KizunagramIsOpen(&self->mPcKizunagram) != 0) {
        char* name = BdatTouchStringCell(lbl_eu_8050CAB8, lbl_eu_8050CAB8 + 0xb, 1);
        setNameText(&self->mTitleAHelp, name);
        func_801C41E8(&self->mTitleAHelp, kizChartStatus(&self->mSub98));
        reopenFromClose(&self->mTitleAHelp);
        kizStartChart(&self->mSub98);
        self->field_0x21C = 7;
    }
}

// ---------------------------------------------------------------------------
// KizunagramReopenCharWindow (us-80259c68)
// When the title help is idle and the kizuna chart is open, show the
// per-character window (state 4). Same shape as KizunagramOpenCharWindow.
// ---------------------------------------------------------------------------
void KizunagramReopenCharWindow(CMenuKizunagram* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        kizChartOpen(&self->mSub98) != 0) {
        self->field_0x21C = 4;
    }
}

extern "C" unsigned long KizunagramIsCreated(void) { return lbl_eu_806647E0 != 0; }

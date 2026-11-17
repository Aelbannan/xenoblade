#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CSkipTimer
// High-level C/C++ reconstruction.

#include "kyoshin/CSkipTimer.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CSysWinImports.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceFile.hpp"

#include <cstdio>
#include "kyoshin/CSkipTimerImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// .sdata2 float constant used as the "animation reached frame" bound.
// (AnimRewindFrame stays as declared in code_80135FDC.hpp - read-only header)
// CfT_PlayClockSnapshot is declared extern "C" in code_80135FDC.hpp.
// CSkipTimer2OnFwdDone / CSkipTimer2OnRewindDone / CSkipTimer_UpdateSlotFrame / CSkipTimerBindAnim24 /
// CSkipTimerBindAnim20 / CSkipTimer2Start are declared extern "C" in CSkipTimer.hpp.
// Retail keeps CSkipTimerStampSlotText out-of-line (CSkipTimer_UpdateSlotFrame emits a real `bl`);
// __declspec(noinline) stops -inline auto from inlining the body into it.
extern "C" __declspec(noinline) void CSkipTimerStampSlotText(CSkipTimer* self, int arg1, int arg2);

// ============================================================================
// CSkipTimer2 constructor (retail short-form symbol, C linkage)
// ============================================================================
// optimize_for_size selects the stmw r30/lmw r30 save pair like retail; the
// unit's -O4,p splits it into individual stw/lwz (+4 bytes).
#pragma push
#pragma auto_inline off
CSkipTimer2::CSkipTimer2(void* parent)
    : mParent(parent),
      mField18(nullptr),
      mAnimTransform(nullptr),
      mField20(0),
      mField21(0),
      mField22(0),
      mField23(1),
      mField24(0) {}
#pragma pop

// ============================================================================
// CSkipTimer2 destructor
// ============================================================================
#pragma push
#pragma auto_inline off
CSkipTimer2::~CSkipTimer2() {
    // member mMemRegion destroyed implicitly (retail emits external call)
}
#pragma pop

// CSkipTimer2Reset (CSkipTimer2): reset the sub-controller - clear state, release
// the layout object through its vtable[2] entry, drop the pointer, and free
// the scratch mem region.
#pragma push
#pragma auto_inline off
void CSkipTimer2Reset(CSkipTimer2* self) {
    // Redundant inner null check: MWCC CSEs the repeated test into one cmpi
    // and re-tests it, reproducing retail's second (dead) beq.
    self->mField20 = 0;
    if (self->mField18 != 0) {
        if (self->mField18 != 0) {
            self->mField18->vf2(1);
        }
        self->mField18 = 0;
    }
    self->mMemRegion.deleteRegion();
}
#pragma pop

// func_8029F6EC: check forward-anim reached end -> state 5.
// Body lives in retail; declared extern so CSkipTimer2DriveState emits a direct `bl`
// instead of inlining (functions defined in this TU are otherwise inlined).
// CSkipTimer2SelectSlotPane (CSkipTimer2): clear the alpha of all 7 slot panes, then
// (for arg 1..7) activate the pane selected by arg. The panes are named by
// the 9-byte-strided strings str[0x5a..0x90] in the pool.
// optimize_for_size selects the stmw/lmw save pair like retail; noinline
// keeps the body out of CSkipTimer2Start (retail emits a direct `bl`).
#pragma push
__declspec(noinline) void CSkipTimer2SelectSlotPane(CSkipTimer2* self, u8 arg) {
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x5a], true),
                  0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x63], true),
                  0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x6c], true),
                  0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x75], true),
                  0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x7e], true),
                  0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x87], true),
                  0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x90], true),
                  0);
    switch (arg) {
    case 1:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x5a], true),
                      1);
        break;
    case 2:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x63], true),
                      1);
        break;
    case 3:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x6c], true),
                      1);
        break;
    case 4:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x75], true),
                      1);
        break;
    case 5:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x7e], true),
                      1);
        break;
    case 6:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x87], true),
                      1);
        break;
    case 7:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x90], true),
                      1);
        break;
    }
}
#pragma pop

// CSkipTimer2OnAdvanceEnd: check forward-anim reached end -> ready state (retail body).
// CSkipTimer2OnRewindDone likewise lives in retail (mirror pane alpha);
// all three are extern here so CSkipTimer2DriveState calls them with a `bl`, matching
// the retail dispatch (defined-in-TU copies would be inlined).
//
// CSkipTimer2OnFwdDone: when the forward anim has reached the end frame (delta > 0),
// set state 2 and mirror the color-alpha byte (offset 0xB8) from the slot pane
// "str[0x52]" onto "str[0x41]".
// Retail callers emit a direct `bl` to this helper (see CSkipTimer2DriveState); the
// pragma guard stops -inline auto from inlining the body into them.
#pragma push
#pragma auto_inline off
void CSkipTimer2OnFwdDone(CSkipTimer2* self) {
    if (advanceAnimTransform(self->mAnimTransform, 1.0f) != 0) {
        self->mField21 = 2;
        self->mField23 = 1;
    }
    nw4r::lyt::Pane* pan52 =
        reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
            ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x52], true);
    reinterpret_cast<PaneAlphaB8*>(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                                       ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true))
        ->field_0xB8 =
        reinterpret_cast<PaneAlphaB8*>(pan52)->field_0xB8;
}
#pragma pop

// CSkipTimer2OnRewindDone: when the reverse anim has reached the end frame (delta == 0),
// clear state 0, clear the slot pane "str[0x41]", then (always) mirror the
// color-alpha byte (offset 0xB8) from "str[0x52]" onto "str[0x41]".
#pragma push
#pragma auto_inline off
void CSkipTimer2OnRewindDone(CSkipTimer2* self) {
    if (AnimRewindFrame(self->mAnimTransform, 1.0f) != 0) {
        self->mField21 = 0;
        self->mField23 = 1;
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true),
                      0);
    }
    nw4r::lyt::Pane* pan52 =
        reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
            ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x52], true);
    reinterpret_cast<PaneAlphaB8*>(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                                       ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true))
        ->field_0xB8 =
        reinterpret_cast<PaneAlphaB8*>(pan52)->field_0xB8;
}
#pragma pop

// CSkipTimer_UpdateSlotFrame: advance the skip-timer animation to the frame computed from
// the slot key (mField2C*60 + mField2E), wrapped to the anim frame count, then
// stamp the slot text and run the sub-controller.
#pragma push
#pragma auto_inline off
void CSkipTimer_UpdateSlotFrame(CSkipTimer* self) {
    // Slot key -> animation frame: (mField2C*60 + mField2E) frames at 30fps,
    // wrapped modulo the anim length. The signed (float)(s32) casts select
    // MWCC's xoris/0x4330-double conversion idiom like retail.
    float cur;
    int total = (int)self->mField2C * 0x3c + (int)(s16)self->mField2E;
    cur = lbl_eu_80662C78 * (float)total + lbl_eu_80664A50;
    float max = (float)(u16)self->mAnimTransform24->GetFrameSize();
    if (cur > max) {
        cur -= max;
    }
    self->mAnimTransform24->SetFrame(cur);
    self->mLayout2->Animate(0);
    // Retail passes the CSkipTimer2 sub-controller (+0x70): its mField18 layout
    // is the pane stamped by CSkipTimerStampSlotText.
    CSkipTimerStampSlotText(reinterpret_cast<CSkipTimer*>(&self->mSkipTimer2),
                  self->mField2C, (s16)self->mField2E);
}
#pragma pop

// CSkipTimerStampSlotText: format slot name and stamp it into the layout pane.
// optimize_for_size selects the stmw r30/lmw r30 save pair like retail; the
// unit's -O4,p would emit individual stw/lwz (+4 bytes, shifting all bl sites).
#pragma push
extern "C" __declspec(noinline) void CSkipTimerStampSlotText(CSkipTimer* self, int arg1, int arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_80510568[0x4a], arg1, arg2);
    LayoutSetTextBoxText(self->mLayout, &lbl_eu_80510568[0x41], buf, 0);
}
#pragma pop

// CSkipTimer constructor.
// Retail strips the mangling on this ctor (short-form literal symbol
// __ct__CSkipTimer, cf. MWCC_CASES sec. 3824 / __ct__CSkipTimer2 above), so it
// is defined as a C-linkage function carrying the exact retail name. Retail
// also returns the object pointer (callers ignore it).
CSkipTimer::CSkipTimer()
    : mFileHandle(nullptr),
      mLayout(nullptr),
      mLayout2(nullptr),
      mAnimTransform20(nullptr),
      mAnimTransform24(nullptr),
      mField28(0),
      mField29(0),
      mField2A(0),
      mField2B(1),
      mField2C(0),
      mField2E(0),
      mActive(0),
      mSysWin(0),
      mSkipTimer2(nullptr) {}

// CSkipTimer destructor.
// Member dtors and operator delete (retail names). UnkClass_8045F564's dtor
// is declared as a real C++ member in its header; the call below goes
// through the class to keep the __dt__17UnkClass_8045F564Fv reloc.
CSkipTimer::~CSkipTimer() {}

// CSkipTimerOnFwdGate: forward-anim gate for CSkipTimer.
// auto_inline off: retail callers (CSkipTimerTick) emit direct `bl`s to the
// state handlers - keep each out-of-line.
#pragma push
#pragma auto_inline off
void CSkipTimer::OnFwdGate() {
    if (advanceAnimTransform(this->mAnimTransform20, 1.0f) != 0) {
        this->mField29 = 2;
        CSkipTimerBindAnim24(this);
        // Intermediate local: retail keeps the action id in r0 across the
        // sub-controller address computation before the u8 conversion.
        int action = CSkipTimerMapKeyToAction(this);
        CSkipTimer2Start(&this->mSkipTimer2,
                      static_cast<u8>(action));
    }
}
#pragma pop

// CSkipTimerOnReverseDone: reverse-anim reached end -> clear skip state.
#pragma push
#pragma auto_inline off
void CSkipTimer::OnReverseDone() {
    if (AnimRewindFrame(this->mAnimTransform20, 1.0f) != 0) {
        this->mField29 = 0;
        this->mField2B = 1;
    }
}
#pragma pop

// CSkipTimer2DriveState (CSkipTimer2): per-frame state machine driver.
// Sparse switch (1,3,4,5) reproduces retail's linear cmpwi+beq dispatch; the
// four handlers are extern retail bodies so each arm is a direct `bl`.
#pragma push
#pragma auto_inline off
extern "C" void CSkipTimer2DriveState(CSkipTimer2* self) {
    if (self->mField20 == 0) return;
    switch (self->mField21) {
    case 1: CSkipTimer2OnFwdDone(self); break;
    case 3: CSkipTimer2OnRewindDone(self); break;
    case 4: func_8029F6EC(self); break;
    case 5: CSkipTimer2OnAdvanceEnd(self); break;
    }
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->Animate(0);
}
#pragma pop

// CSkipTimer2Start (CSkipTimer2): start the skip-timer UI sub-state.
// auto_inline off: retail callers (CSkipTimerOnFwdGate) emit a direct `bl`.
#pragma push
#pragma auto_inline off
void CSkipTimer2Start(CSkipTimer2* self, u8 arg) {
    if (self->mField21 != 0) return;
    self->mField21 = 1;
    self->mField24 = arg;
    self->mField23 = 0;
    nw4r::lyt::Layout* layout = reinterpret_cast<nw4r::lyt::Layout*>(self->mField18);
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true);
    setPaneVisible(pane, 1);
    CSkipTimer2SelectSlotPane(self, self->mField24);
}
#pragma pop

// CSkipTimerBindAnim24 (CSkipTimer): bind anim24 to layout2 + animate.
// No cached `layout` temp: retail reloads mLayout2 before each virtual call.
// Pragma guard: retail callers (CSkipTimerActivateTimer / CSkipTimerOnFwdGate) emit a direct
// `bl` to this helper - do not inline it into them.
#pragma push
#pragma auto_inline off
void CSkipTimerBindAnim24(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform24);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform24, true);
    self->mLayout2->Animate(0);
}
#pragma pop

// CSkipTimerBindAnim20 (CSkipTimer): bind anim20 to layout2 + animate.
// Same no-temp shape as CSkipTimerBindAnim24; retail reloads mLayout2 per call.
#pragma push
#pragma auto_inline off
void CSkipTimerBindAnim20(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform20);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform20, true);
    self->mLayout2->Animate(0);
}
#pragma pop

// CSkipTimerMapKeyToAction: map skip-timer index (mField2C s16) to an action id (1..7).
// if/else gives the exact retail dispatch tree (structural-0): 17 first, then
// the >=17 (<=18->5, >=19->6) arm, then <5->7 / ==5->1 / <10->2 / else->3.
// (Retail's leaf-block grouping implies a switch; this MWCC revision builds a
// 10-pivot tree for that switch, so the if/else chain is the byte-closest.)
// auto_inline off: retail callers (CSkipTimerOnFwdGate) emit a direct `bl`.
// CSkipTimerMapKeyToAction: map skip-timer index (mField2C s16) to an action id (1..7).
// Winning shape: monotonic-range if/else chain with the RETURNS in ascending
// value order (7,1,2,3,4,5,6) - MWCC rebuilds the checks into retail's exact
// 4-pivot tree (17, 19, 5, 10) and emits the leaves in the source return
// order. (A tree-shaped if/else - 17 first, then arms - emits the leaves in
// source order and mismatches; a switch builds a 10-pivot tree.)
// auto_inline off: retail callers (CSkipTimerOnFwdGate) emit a direct `bl`.
#pragma push
#pragma auto_inline off
extern "C" int CSkipTimerMapKeyToAction(CSkipTimer* self) {
    s16 key = self->mField2C;
    if (key < 5) return 7;
    if (key == 5) return 1;
    if (key < 10) return 2;
    if (key < 17) return 3;
    if (key == 17) return 4;
    if (key == 18) return 5;
    return 6;
}
#pragma pop

// --- remaining scaffolding stubs (not harness targets) ---

__declspec(noinline) void CSkipTimer2MarkActive(u8* self) {
    if (*(u32*)(self + 0x14) != 0) {
        self[0x22] = 1;
        self[0x20] = 1;
    }
}

// CSkipTimer2RebindAnim (CSkipTimer2): rebind + animate the sub-controller layout.
// No cached `layout` temp: retail reloads mField18 before each virtual call.
// noinline: called out-of-line by CSkipTimer2RebuildLayout (which compiles under
// optimize_for_size, which re-enables auto-inlining). C linkage comes from
// the extern "C" declaration in CSkipTimer.hpp (retail strips mangling).
__declspec(noinline) void CSkipTimer2RebindAnim(CSkipTimer2* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->BindAnimation(self->mAnimTransform);
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->SetAnimationEnable(self->mAnimTransform, true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->Animate(0);
}

// CSkipTimer_BeginLoad: kick off the System.arc load for the skip-timer widget.
// Reads the arc through the MEM2 allocator (self receives the file event),
// then builds a temporary CSysWin and copy-initializes the embedded panel
// blob at +0x34 from it (copy-ctor semantics: member-wise, vtable NOT
// copied), destroys the temp, and fires the panel's load-complete virtual
// (vtable offset 0x88). Finally clears the ready flag at +0x2A.
void CSkipTimer_BeginLoad(CSkipTimer* self) {
    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), &lbl_eu_80510568[0x99],
        reinterpret_cast<IWorkEvent*>(self), 0, 0);

    self->mSysWin = CSysWin(0);

    self->mSysWin.loadSystemArc();
    self->mField2A = 0;
}

// Retail strips mangling on these state-handler symbols (US short-form func_
// names), so both declarations and definitions carry C linkage.
// (memberized: CSkipTimerOnFwdGate -> CSkipTimer::OnFwdGate; declaration moved into class CSkipTimer)
// (memberized: CSkipTimerReengageInput -> CSkipTimer::ReengageInput; declaration moved into class CSkipTimer)
// (memberized: CSkipTimerEndSkip -> CSkipTimer::EndSkip; declaration moved into class CSkipTimer)
// (memberized: CSkipTimerOnReverseDone -> CSkipTimer::OnReverseDone; declaration moved into class CSkipTimer)
// (memberized: CSkipTimerActivateSkipUI -> CSkipTimer::ActivateSkipUI; declaration moved into class CSkipTimer)
// (memberized: CSkipTimerLockSkipButton -> CSkipTimer::LockSkipButton; declaration moved into class CSkipTimer)
extern "C" void CSkipTimer2DriveState(CSkipTimer2* self);
extern "C" void CSkipTimer2HoldRelease(CSkipTimer2* self, u8 arg);

// CSkipTimerTick (CSkipTimer): per-frame driver. A switch over mField29 gives
// retail's cmpi/beq-to-case-block dispatch; then animate layout2,
// tick the sub-controller state machine, and update the syswin panel.
void CSkipTimerTick(CSkipTimer* self) {
    if (self->mField28 == 0) return;
    switch (self->mField29) {
    case 1:
        self->OnFwdGate();
        break;
    case 2:
        self->ReengageInput();
        break;
    case 3: {
        // Intermediate local: retail keeps the action id in r0 across the
        // sub-controller address computation before the u8 conversion.
        int action = CSkipTimerMapKeyToAction(self);
        CSkipTimer2HoldRelease(&self->mSkipTimer2,
                      static_cast<u8>(action));
        break;
    }
    case 4:
        self->EndSkip();
        break;
    case 5:
        self->OnReverseDone();
        break;
    case 6:
        self->ActivateSkipUI();
        break;
    case 7:
        self->LockSkipButton();
        break;
    }
    self->mLayout2->Animate(0);
    CSkipTimer2DriveState(&self->mSkipTimer2);
    sysWinDispatchPhase(&self->mSysWin);
}

// CSkipTimerDraw (CSkipTimer): draw helper - render both layouts + syswin.
// No cached sub-controller pointer: retail reads the nested fields as direct
// self-relative offsets (0x88 / 0x90), so the casts are inlined.
// Retail saves r30/r31 with a single stmw; the unit's -O4,p would split them
// (+8 bytes) - per-function optimize_for_size selects the stmw/lmw pair.
#pragma push
void CSkipTimerDraw(CSkipTimer* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField28 == 0) return;
    drawLayout(self->mLayout2, drawInfo, 0, 1);
    if (self->mSkipTimer2.mField20 != 0) {
        drawLayout(
            reinterpret_cast<nw4r::lyt::Layout*>(
                self->mSkipTimer2.mField18),
            drawInfo, 0, 1);
    }
    sysWinDrawLayout(&self->mSysWin, drawInfo);
}
#pragma pop

// CSkipTimerTeardown (CSkipTimer): tear down the whole widget (dtor-style cleanup).
void CSkipTimerTeardown(CSkipTimer* self) {
    closeFileHandle(&self->mFileHandle);
    nw4r::lyt::Layout* layout = self->mLayout2;
    self->mField28 = 0;
    if (layout != 0) {
        delete layout;
        self->mLayout2 = 0;
    }
    CSkipTimer2Reset(&self->mSkipTimer2);
    releaseArcResourceAccessor(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mLayout));
    self->mLayout = 0;
    sysWinTermLayout(&self->mSysWin);
    self->mMemRegion.deleteRegion();
}

// CSkipTimerIsReady: return the syswin panel's ready flag when ready, else 0.
u8 CSkipTimerIsReady(CSkipTimer* self) {
    if (CSysWin_isReady(&self->mSysWin) != 0) {
        return self->mField2A;
    }
    return 0;
}

// CSkipTimerGetSkipButton: return the skip button state only while the sub-controller is
// mid-anim (mField23), else the "released" value 0.
u8 CSkipTimerGetSkipButton(CSkipTimer* self) {
    CSkipTimer2* sub = &self->mSkipTimer2;
    if (sub->mField23 != 0) {
        return self->mField2B;
    }
    return 0;
}

// CSkipTimerNextKey (CSkipTimer): advance the switch key (wrap up) + cue sound.
void CSkipTimerNextKey(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWin) != 0) return;
    s16 t = self->mField2C + 1;
    self->mField2C = t;
    if (t > 0x17) self->mField2C = 0;
    CSkipTimer_UpdateSlotFrame(self);
    playUISound(0x22);
}

// CSkipTimerPrevKey (CSkipTimer): retreat the switch key (wrap down) + cue sound.
void CSkipTimerPrevKey(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWin) != 0) return;
    s16 t = self->mField2C - 1;
    self->mField2C = t;
    if (t < 0) self->mField2C = 0x17;
    CSkipTimer_UpdateSlotFrame(self);
    playUISound(0x22);
}

void CSkipTimerFlagButton(u8* obj) {
    if (obj[0x29] != 0) return;
    obj[0x29] = 1;
    obj[0x2b] = 0;
}

// CSkipTimerLeaveSkip: leave the skip state machine (state 3 -> 4), zero the sub
// controller's anim fields, and raise the leave sound.
void CSkipTimerLeaveSkip(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    CSkipTimer2* sub = &self->mSkipTimer2;
    self->mField29 = 4;
    self->mField2B = 0;
    sub->mField21 = 3;
    sub->mField23 = 0;
    playUISound(6);
}

// CSkipTimerConfirmSkip (CSkipTimer): confirm/cancel handler while in state 3.
// If the syswin panel is engaged (getUnk34 != 0) and active, commit the skip:
// state 7 + release button, close the panel, fire the bgm switch from the
// slot key, then sfx 3. Otherwise (panel not engaged) show the confirmation
// text built by BdatTouchStringCell and advance the window kind, then sfx 3.
extern "C" void CSkipTimerConfirmSkip(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWin) != 0) {
        if (CSysWin_isActive(&self->mSysWin) == 0) return;
        self->mField29 = 7;
        self->mField2B = 0;
        sysWinAdvancePhase3(&self->mSysWin);
        func_80086B5C__Q22cf13CfGameManagerFv((u32)(u16)self->mField2C,
                                              (u32)(u16)self->mField2E, 1);
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
        }
        playUISound(3);
    } else {
        self->mField29 = 6;
        self->mField2B = 0;
        char* msg = BdatTouchStringCell(&lbl_eu_80510568[0xaf], &lbl_eu_80510568[0xb8], 0x3a);
        sysWinSetKindContent(&self->mSysWin, msg, 0);
        sysWinApplyKind(&self->mSysWin, 1);
        sysWinOpenPhase1(&self->mSysWin);
        playUISound(3);
    }
}

// CSkipTimerEngageSkip: engage the skip UI once the syswin panel is active (state 7 +
// reset the button, fire the fade sfx + bgm switch), else mark the timer
// active and cue sfx 6.
void CSkipTimerEngageSkip(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWin) != 0) {
        if (CSysWin_isActive(&self->mSysWin) == 0) return;
        self->mField29 = 7;
        self->mField2B = 0;
        sysWinAdvancePhase3(&self->mSysWin);
        func_80086B5C__Q22cf13CfGameManagerFv((u32)(u16)self->mField2C,
                                              (u32)(u16)self->mField2E, 1);
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
        }
        playUISound(3);
    } else {
        self->mActive = 1;
        playUISound(6);
    }
}

// CSkipTimerIsEngaged: report 1 while the syswin panel is engaged (getUnk34 != 0),
// otherwise the plain active flag at +0x30.
u8 CSkipTimerIsEngaged(CSkipTimer* self) {
    if (CSysWin_getUnk34(&self->mSysWin) != 0) {
        return 1;
    }
    return self->mActive;
}

// CSkipTimerReengageInput: (re)engage skip input: frame 3, button state 1.
#pragma push
#pragma auto_inline off
void CSkipTimer::ReengageInput() {
    CSkipTimer2* sub = &this->mSkipTimer2;
    if (sub->mField23 == 0) return;
    this->mField29 = 3;
    this->mField2B = 1;
}
#pragma pop

// CSkipTimerEndSkip: end the skip: frame 5, then rebind the forward anim (tail call).
#pragma push
#pragma auto_inline off
void CSkipTimer::EndSkip() {
    CSkipTimer2* sub = &this->mSkipTimer2;
    if (sub->mField23 == 0) return;
    this->mField29 = 5;
    CSkipTimerBindAnim20(this);
}
#pragma pop

// CSkipTimerActivateSkipUI: (re)activate skip UI: frame 3 + button state 1 when the syswin
// panel is engaged.
#pragma push
#pragma auto_inline off
void CSkipTimer::ActivateSkipUI() {
    if (CSysWin_isActive(&this->mSysWin) == 0) return;
    this->mField29 = 3;
    this->mField2B = 1;
}
#pragma pop

// CSkipTimerLockSkipButton: lock the skip button on while the syswin panel is engaged.
#pragma push
#pragma auto_inline off
void CSkipTimer::LockSkipButton() {
    if (CSysWin_isActive(&this->mSysWin) == 0) return;
    this->mField2B = 1;
    this->mActive = 1;
}
#pragma pop

// CSkipTimer2HoldRelease (CSkipTimer2): hold-and-release transition for the sub
// controller: from anim state 2 to 4, unless the target pane already matches.
// auto_inline off: retail callers (CSkipTimerTick) emit a direct `bl`.
#pragma push
#pragma auto_inline off
extern "C" void CSkipTimer2HoldRelease(CSkipTimer2* self, u8 arg) {
    if (self->mField21 != 2) return;
    if (arg == self->mField24) return;
    self->mField21 = 4;
    self->mField23 = 0;
    self->mField24 = arg;
}
#pragma pop

// CSkipTimerActivateTimer (CSkipTimer): (re)activate the skip timer state.
// optimize_for_size selects the stmw/lmw save pair like retail; the unit's
// default -O4,p would emit individual stw's (+8B). __declspec(noinline) keeps
// the body out of OnFileEvent (retail emits a direct `bl`); plain
// auto_inline off is insufficient because optimize_for_size re-enables
// inlining.
#pragma push
__declspec(noinline) void CSkipTimerActivateTimer(CSkipTimer* self) {
    if (self->mLayout == 0) return;
    CSkipTimerBindAnim24(self);
    self->mField2C = 0;
    self->mField2E = 0;
    CfT_PlayClockSnapshot(reinterpret_cast<u16*>(&self->mField2C), reinterpret_cast<u16*>(&self->mField2E));
    self->mField2E = 0;
    CSkipTimer_UpdateSlotFrame(self);
    CSkipTimerBindAnim20(self);
    self->mField2A = 1;
    self->mField28 = 1;
}
#pragma pop

// CSkipTimer_UpdateSlotFrame (CSkipTimer): sibling helper, not a harness target.
// Declared extern "C" at top (resolved to retail at link); not defined here.

// OnFileEvent: rebuild the whole skip-timer widget from the freshly-exchanged
// file data. Matches the CBgTex/CSaveLoad event pattern: create the scratch
// region (guarded by a Class_8045F858 RAII temp), attach the arc resource,
// build layout + both anims, bind them, then copy-initialize the embedded
// CSkipTimer2 sub-controller from a stack temporary (mem region via the
// UnkClass_8011C974 copy helper, rest field-by-field).
// optimize_for_size: retail saves r28-r31 with one stmw; the unit's -O4,p
// would split them into four stw's (+12B). Callees with auto_inline off
// (ctor/dtor/CSkipTimerBindAnim24/CSkipTimerActivateTimer) stay out-of-line.
#pragma push
bool CSkipTimer::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
        mMemRegion.createRegion(mem2, 0x10000, &lbl_eu_80510568[0xbd], 0);
        Class_8045F858 memHost(&mMemRegion);
        void* fileData = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);
        // 0x18 is the arc accessor here (CSkipTimerTeardown releases it as one); the
        // header models it as mLayout (CSkipTimerStampSlotText stamps text through it).
        mLayout = reinterpret_cast<nw4r::lyt::Layout*>(CLibLayout::createArcResourceAccessor());
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout)
            ->Attach(fileData, &lbl_eu_80510568[0xc8]);
        buildLayout(&mLayout2, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                      &lbl_eu_80510568[0xcc]);
        bindLayoutAnimTransform(mLayout2, &mAnimTransform20,
                      reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                      &lbl_eu_80510568[0xe5]);
        bindLayoutAnimTransform(mLayout2, &mAnimTransform24,
                      reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                      &lbl_eu_80510568[0x101]);
        CSkipTimerBindAnim20(this);

        mSkipTimer2 = CSkipTimer2(mLayout);
        CSkipTimer2RebuildLayout(&mSkipTimer2);
        CSkipTimerActivateTimer(this);
        mFileHandle = 0;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
#pragma pop

// --- hard-symbol stub (scaffold_hard_symbols) ---
// sinit_802A07D8: static initializer folding the skip-timer fade bound into
// lbl_eu_80664A50 as (mConst8C48 * (mConst68C4C * mConst2C78)).
void sinit_802A07D8() {
    lbl_eu_80664A50 = lbl_eu_80668C48 * (lbl_eu_80668C4C * lbl_eu_80662C78);
}

// CSkipTimer2RebuildLayout (CSkipTimer2): post-build hook - (re)build the sub-controller
// layout from the arc accessor held in mParent under a fresh scratch region,
// bind the animation, stamp the font into pane "str[0x41]" and clear its
// alpha. auto_inline off keeps the call out of OnFileEvent (retail emits a
// direct `bl`). optimize_for_size selects the stmw r30/lmw r30 save pair like
// retail; noinline keeps the body out of OnFileEvent since optimize_for_size
// re-enables inlining.
#pragma push
#pragma auto_inline off
__declspec(noinline) void CSkipTimer2RebuildLayout(CSkipTimer2* self) {
    self->mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x10000,
                                  lbl_eu_80510568, 0);
    Class_8045F858 regionHost(&self->mMemRegion);
    // No cached accessor temp: retail reloads mParent (+0x14) for each build
    // call, keeping only the string-pool base and self in callee-saved regs.
    buildLayout(reinterpret_cast<nw4r::lyt::Layout**>(&self->mField18),
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mParent),
                  &lbl_eu_80510568[0xc]);
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18),
                  &self->mAnimTransform,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mParent),
                  &lbl_eu_80510568[0x25]);
    CSkipTimer2RebindAnim(self);
    setLayoutTextBoxFont(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18),
                  &lbl_eu_80510568[0x41], (u32)CUICfManager_getPackedFontD8());
    nw4r::lyt::Pane* pane = reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                                ->GetRootPane()
                                ->FindPaneByName(&lbl_eu_80510568[0x41], true);
    setPaneVisible(pane, 0);
    CSkipTimer2MarkActive(reinterpret_cast<u8*>(self));
    self->mMemRegion.validateHeap();
}
#pragma pop

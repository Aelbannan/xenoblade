// Auto-scaffolded catalog TU for kyoshin/CSkipTimer
// High-level C/C++ reconstruction.

#include "kyoshin/CSkipTimer.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

#include <cstdio>

// .sdata2 float constant used as the "animation reached frame" bound.
extern const float lbl_eu_80668C30;
void func_8006A234(u16*, u16*);
// (func_80137510 stays as declared in code_80135FDC.hpp - read-only header)
void func_8029F504(CSkipTimer2* self);
void func_8029F5CC(CSkipTimer2* self);
void func_802A041C(CSkipTimer* self);
void func_8029F364(CSkipTimer2* self, u8 arg);
void func_8029F440(CSkipTimer* self, int arg1, int arg2);
void func_802A055C(CSkipTimer* self);
void func_802A05E4(CSkipTimer* self);

// ============================================================================
// CSkipTimer2 constructor
// ============================================================================
CSkipTimer2::CSkipTimer2(void* parent) {
    mVtbl = lbl_eu_80539884;
    __ct__17UnkClass_8045F564Fv(&mMemRegion);
    mParent = parent;
    mField18 = 0;
    mAnimTransform = 0;
    mField20 = 0;
    mField21 = 0;
    mField22 = 0;
    mField23 = 1;
    mField24 = 0;
}

// ============================================================================
// CSkipTimer2 destructor
// ============================================================================
CSkipTimer2::~CSkipTimer2() {
    // member mMemRegion destroyed implicitly (retail emits external call)
}

// func_8029F2FC: reset the sub-controller.
// Body lives in retail; declared in CSkipTimer.hpp so func_8029FE30 emits a
// direct `bl` instead of inlining a defined-in-TU copy.

// func_8029F6EC: check forward-anim reached end -> state 5.
// Body lives in retail; declared extern so func_8029F26C emits a direct `bl`
// instead of inlining (functions defined in this TU are otherwise inlined).
// func_8029F82C (CSkipTimer2): clear the alpha of all 7 slot panes, then
// (for arg 1..7) activate the pane selected by arg. The panes are named by
// the 9-byte-strided strings str[0x5a..0x90] in the pool.
void func_8029F82C(CSkipTimer2* self, u8 arg) {
    func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x5a], true),
                  0);
    func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x63], true),
                  0);
    func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x6c], true),
                  0);
    func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x75], true),
                  0);
    func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x7e], true),
                  0);
    func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x87], true),
                  0);
    func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x90], true),
                  0);
    switch (arg) {
    case 1:
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x5a], true),
                      1);
        break;
    case 2:
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x63], true),
                      1);
        break;
    case 3:
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x6c], true),
                      1);
        break;
    case 4:
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x75], true),
                      1);
        break;
    case 5:
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x7e], true),
                      1);
        break;
    case 6:
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x87], true),
                      1);
        break;
    case 7:
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x90], true),
                      1);
        break;
    }
}

// func_8029F73C: check forward-anim reached end -> ready state (retail body).
// func_8029F5CC likewise lives in retail (mirror pane alpha);
// all three are extern here so func_8029F26C calls them with a `bl`, matching
// the retail dispatch (defined-in-TU copies would be inlined).
//
// func_8029F504: when the forward anim has reached the end frame (delta > 0),
// set state 2 and mirror the color-alpha byte (offset 0xB8) from the slot pane
// "str[0x52]" onto "str[0x41]".
void func_8029F504(CSkipTimer2* self) {
    if (func_80137444(self->mAnimTransform, lbl_eu_80668C30) != 0) {
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

// func_8029F5CC: when the reverse anim has reached the end frame (delta == 0),
// clear state 0, clear the slot pane "str[0x41]", then (always) mirror the
// color-alpha byte (offset 0xB8) from "str[0x52]" onto "str[0x41]".
void func_8029F5CC(CSkipTimer2* self) {
    if (func_80137510(self->mAnimTransform, lbl_eu_80668C30) != 0) {
        self->mField21 = 0;
        self->mField23 = 1;
        func_80124270(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
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

// func_802A041C: advance the skip-timer animation to the frame computed from
// the slot key (mField2C*60 + mField2E), wrapped to the anim frame count, then
// stamp the slot text and run the sub-controller.
void func_802A041C(CSkipTimer* self) {
    int total = (int)self->mField2C * 0x3c + (int)(s16)self->mField2E;
    float cur = lbl_eu_80662C78 * (float)total + lbl_eu_80664A50;
    float max = (float)(u16)self->mAnimTransform24->GetFrameSize();
    if (cur > max) {
        cur -= max;
    }
    self->mAnimTransform24->SetFrame(cur);
    self->mLayout2->Animate(0);
    func_8029F440(self, self->mField2C, (s16)self->mField2E);
}

// func_8029F440: format slot name and stamp it into the layout pane.
void func_8029F440(CSkipTimer* self, int arg1, int arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_80510568[0x4a], arg1, arg2);
    func_80136A1C(self->mLayout, &lbl_eu_80510568[0x41], buf, 0);
}

// CSkipTimer constructor.
CSkipTimer::CSkipTimer() {
    mVtbl = lbl_eu_805397F0;
    __ct__17UnkClass_8045F564Fv(&mMemRegion);
    mFileHandle = 0;
    mLayout = 0;
    mLayout2 = 0;
    mAnimTransform20 = 0;
    mAnimTransform24 = 0;
    mField28 = 0;
    mField29 = 0;
    mField2A = 0;
    mField2B = 1;
    mField2C = 0;
    mField2E = 0;
    mActive = 0;
    // Retail calls the C-linkage ctor symbol __ct__CSysWin; the C++ placement
    // new would emit MWCC's natural __ct__7CSysWinFv instead.
    __ct__CSysWin(&mSysWinData[0]);
    new (&mSkipTimer2Data[0]) CSkipTimer2(0);
}

// CSkipTimer destructor.
CSkipTimer::~CSkipTimer() {
    // Destruction order matches retail: mSkipTimer2 (0x70), CSysWin (0x34),
    // then mMemRegion (0x04, auto-destroyed last as first-declared member).
    reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->~CSkipTimer2();
    __dt__7CSysWinFv(&mSysWinData[0], -1);
}

// func_802A0234: forward-anim gate for CSkipTimer.
void func_802A0234(CSkipTimer* self) {
    if (func_80137444(self->mAnimTransform20, lbl_eu_80668C30) != 0) {
        self->mField29 = 2;
        func_802A05E4(self);
        func_8029F364(reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]),
                      static_cast<u8>(func_802A04F0(self)));
    }
}

// func_802A02D4: reverse-anim reached end -> clear skip state.
void func_802A02D4(CSkipTimer* self) {
    if (func_80137510(self->mAnimTransform20, lbl_eu_80668C30) != 0) {
        self->mField29 = 0;
        self->mField2B = 1;
    }
}

// func_8029F26C (CSkipTimer2): per-frame state machine driver.
// Sparse switch (1,3,4,5) reproduces retail's linear cmpwi+beq dispatch; the
// four handlers are extern retail bodies so each arm is a direct `bl`.
void func_8029F26C(CSkipTimer2* self) {
    if (self->mField20 == 0) return;
    switch (self->mField21) {
    case 1: func_8029F504(self); break;
    case 3: func_8029F5CC(self); break;
    case 4: func_8029F6EC(self); break;
    case 5: func_8029F73C(self); break;
    }
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->Animate(0);
}

// func_8029F364 (CSkipTimer2): start the skip-timer UI sub-state.
void func_8029F364(CSkipTimer2* self, u8 arg) {
    if (self->mField21 != 0) return;
    self->mField21 = 1;
    self->mField24 = arg;
    self->mField23 = 0;
    nw4r::lyt::Layout* layout = reinterpret_cast<nw4r::lyt::Layout*>(self->mField18);
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true);
    func_80124270(pane, 1);
    func_8029F82C(self, self->mField24);
}

// func_802A05E4 (CSkipTimer): bind anim24 to layout2 + animate.
// No cached `layout` temp: retail reloads mLayout2 before each virtual call.
void func_802A05E4(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform24);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform24, true);
    self->mLayout2->Animate(0);
}

// func_802A055C (CSkipTimer): bind anim20 to layout2 + animate.
// Same no-temp shape as func_802A05E4; retail reloads mLayout2 per call.
void func_802A055C(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform20);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform20, true);
    self->mLayout2->Animate(0);
}

// func_802A04F0: map skip-timer index (mField2C s16) to an action id (1..7).
// if/else gives the exact retail dispatch tree (structural-0): 17 first, then
// the >=17 (<=18->5, >=19->6) arm, then <5->7 / ==5->1 / <10->2 / else->3.
// (Retail's leaf-block grouping implies a switch; this MWCC revision builds a
// 10-pivot tree for that switch, so the if/else chain is the byte-closest.)
extern "C" int func_802A04F0(CSkipTimer* self) {
    s16 key = self->mField2C;
    if (key == 17) return 4;
    if (key >= 17) {              // 18,19,20,...
        if (key >= 19) return 6;  // >=19
        return 5;                 // 18
    }
    // key <= 16
    if (key == 5) return 1;
    if (key < 5) return 7;        // 0..4 (and negatives)
    if (key >= 10) return 3;      // 10..16
    return 2;                     // 6..9
}

// --- remaining scaffolding stubs (not harness targets) ---

void func_8029F788(u8* self) {
    if (*(u32*)(self + 0x14) != 0) {
        self[0x22] = 1;
        self[0x20] = 1;
    }
}

// func_8029F7A4 (CSkipTimer2): rebind + animate the sub-controller layout.
// No cached `layout` temp: retail reloads mField18 before each virtual call.
void func_8029F7A4(CSkipTimer2* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->BindAnimation(self->mAnimTransform);
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->SetAnimationEnable(self->mAnimTransform, true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->Animate(0);
}

void func_8029FBE0(){}
void func_8029FCDC(){}

// func_8029FDBC (CSkipTimer): draw helper - render both layouts + syswin.
void func_8029FDBC(CSkipTimer* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField28 == 0) return;
    func_80137038(self->mLayout2, drawInfo, 0, 1);
    CSkipTimer2* sub = reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]);
    if (sub->mField20 != 0) {
        func_80137038(
            reinterpret_cast<nw4r::lyt::Layout*>(sub->mField18), drawInfo, 0, 1);
    }
    func_8022B7C8(&self->mSysWinData[0], drawInfo);
}

// func_8029FE30 (CSkipTimer): tear down the whole widget (dtor-style cleanup).
void func_8029FE30(CSkipTimer* self) {
    func_801390E0(&self->mFileHandle);
    nw4r::lyt::Layout* layout = self->mLayout2;
    self->mField28 = 0;
    if (layout != 0) {
        delete layout;
        self->mLayout2 = 0;
    }
    func_8029F2FC(reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]));
    func_80139124(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mLayout));
    self->mLayout = 0;
    func_8022B7F4(&self->mSysWinData[0]);
    self->mMemRegion.func_8045F778();
}

// func_8029FEBC: return the syswin panel's ready flag when ready, else 0.
u8 func_8029FEBC(CSkipTimer* self) {
    if (CSysWin_isReady(&self->mSysWinData[0]) != 0) {
        return self->mField2A;
    }
    return 0;
}

// func_8029FF00: return the skip button state only while the sub-controller is
// mid-anim (mField23), else the "released" value 0.
u8 func_8029FF00(CSkipTimer* self) {
    CSkipTimer2* sub = reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]);
    if (sub->mField23 != 0) {
        return self->mField2B;
    }
    return 0;
}

// func_8029FF24 (CSkipTimer): advance the switch key (wrap up) + cue sound.
void func_8029FF24(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWinData[0]) != 0) return;
    s16 t = self->mField2C + 1;
    self->mField2C = t;
    if (t > 0x17) self->mField2C = 0;
    func_802A041C(self);
    func_80138078__FUl(0x22);
}

// func_8029FF98 (CSkipTimer): retreat the switch key (wrap down) + cue sound.
void func_8029FF98(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWinData[0]) != 0) return;
    s16 t = self->mField2C - 1;
    self->mField2C = t;
    if (t < 0) self->mField2C = 0x17;
    func_802A041C(self);
    func_80138078__FUl(0x22);
}

void func_802A0008(u8* obj) {
    if (obj[0x29] != 0) return;
    obj[0x29] = 1;
    obj[0x2b] = 0;
}

// func_802A0028: leave the skip state machine (state 3 -> 4), zero the sub
// controller's anim fields, and raise the leave sound.
void func_802A0028(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    CSkipTimer2* sub = reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]);
    self->mField29 = 4;
    self->mField2B = 0;
    sub->mField21 = 3;
    sub->mField23 = 0;
    func_80138078__FUl(6);
}

void func_802A005C(){}

void func_802A0148(){}

// func_802A01F0: report 1 while the syswin panel is engaged (getUnk34 != 0),
// otherwise the plain active flag at +0x30.
u8 func_802A01F0(CSkipTimer* self) {
    if (CSysWin_getUnk34(&self->mSysWinData[0]) != 0) {
        return 1;
    }
    return self->mActive;
}

// func_802A0298: (re)engage skip input: frame 3, button state 1.
void func_802A0298(CSkipTimer* self) {
    CSkipTimer2* sub = reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]);
    if (sub->mField23 == 0) return;
    self->mField29 = 3;
    self->mField2B = 1;
}

// func_802A02B8: end the skip: frame 5, then rebind the forward anim (tail call).
void func_802A02B8(CSkipTimer* self) {
    CSkipTimer2* sub = reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0]);
    if (sub->mField23 == 0) return;
    self->mField29 = 5;
    func_802A055C(self);
}

// func_802A0320: (re)activate skip UI: frame 3 + button state 1 when the syswin
// panel is engaged.
void func_802A0320(CSkipTimer* self) {
    if (CSysWin_isActive(&self->mSysWinData[0]) == 0) return;
    self->mField29 = 3;
    self->mField2B = 1;
}

// func_802A0368: lock the skip button on while the syswin panel is engaged.
void func_802A0368(CSkipTimer* self) {
    if (CSysWin_isActive(&self->mSysWinData[0]) == 0) return;
    self->mField2B = 1;
    self->mActive = 1;
}

// func_8029F410 (CSkipTimer2): hold-and-release transition for the sub
// controller: from anim state 2 to 4, unless the target pane already matches.
void func_8029F410(CSkipTimer2* self, u8 arg) {
    if (self->mField21 != 2) return;
    if (arg == self->mField24) return;
    self->mField21 = 4;
    self->mField23 = 0;
    self->mField24 = arg;
}

// func_802A03AC (CSkipTimer): (re)activate the skip timer state.
void func_802A03AC(CSkipTimer* self) {
    if (self->mLayout == 0) return;
    func_802A05E4(self);
    self->mField2C = 0;
    self->mField2E = 0;
    func_8006A234(reinterpret_cast<u16*>(&self->mField2C), &self->mField2E);
    self->mField2E = 0;
    func_802A041C(self);
    func_802A055C(self);
    self->mField2A = 1;
    self->mField28 = 1;
}

// func_802A041C (CSkipTimer): sibling helper, not a harness target.
// Declared extern "C" at top (resolved to retail at link); not defined here.

bool CSkipTimer::OnFileEvent(CEventFile* pEventFile) { return false; }

// --- hard-symbol stub (scaffold_hard_symbols) ---
// sinit_802A07D8: static initializer folding the skip-timer fade bound into
// lbl_eu_80664A50 as (mConst8C48 * (mConst68C4C * mConst2C78)).
void sinit_802A07D8() {
    lbl_eu_80664A50 = lbl_eu_80668C48 * (lbl_eu_80668C4C * lbl_eu_80662C78);
}

extern "C" void func_8029F168() {}

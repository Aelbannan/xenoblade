// Auto-scaffolded catalog TU for kyoshin/CSkipTimer
// High-level C/C++ reconstruction.

#include "kyoshin/CSkipTimer.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"

#include <cstdio>

// .sdata2 float constant used as the "animation reached frame" bound.
extern const float lbl_eu_80668C30;
// (func_80137510 stays as declared in code_80135FDC.hpp - read-only header)
// func_8006A234 is declared extern "C" in code_80135FDC.hpp.
// func_8029F504 / func_8029F5CC / func_802A041C / func_802A05E4 /
// func_802A055C are declared extern "C" in CSkipTimer.hpp.
void func_8029F364(CSkipTimer2* self, u8 arg);
// Retail keeps func_8029F440 out-of-line (func_802A041C emits a real `bl`);
// __declspec(noinline) stops -inline auto from inlining the body into it.
__declspec(noinline) void func_8029F440(CSkipTimer* self, int arg1, int arg2);

// ============================================================================
// CSkipTimer2 constructor (retail short-form symbol, C linkage)
// ============================================================================
#pragma push
#pragma auto_inline off
void __ct__CSkipTimer2(CSkipTimer2* self, void* parent) {
    self->mVtbl = lbl_eu_80539884;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->mParent = parent;
    self->mField18 = 0;
    self->mAnimTransform = 0;
    self->mField20 = 0;
    self->mField21 = 0;
    self->mField22 = 0;
    self->mField23 = 1;
    self->mField24 = 0;
}
#pragma pop

// ============================================================================
// CSkipTimer2 destructor
// ============================================================================
#pragma push
#pragma auto_inline off
#pragma optimize_for_size on
CSkipTimer2::~CSkipTimer2() {
    // member mMemRegion destroyed implicitly (retail emits external call)
}
#pragma pop

// func_8029F2FC: reset the sub-controller.
// Body lives in retail; declared in CSkipTimer.hpp so func_8029FE30 emits a
// direct `bl` instead of inlining a defined-in-TU copy.

// func_8029F6EC: check forward-anim reached end -> state 5.
// Body lives in retail; declared extern so func_8029F26C emits a direct `bl`
// instead of inlining (functions defined in this TU are otherwise inlined).
// func_8029F82C (CSkipTimer2): clear the alpha of all 7 slot panes, then
// (for arg 1..7) activate the pane selected by arg. The panes are named by
// the 9-byte-strided strings str[0x5a..0x90] in the pool.
// optimize_for_size selects the stmw/lmw save pair like retail; noinline
// keeps the body out of func_8029F364 (retail emits a direct `bl`).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void func_8029F82C(CSkipTimer2* self, u8 arg) {
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
#pragma pop

// func_8029F73C: check forward-anim reached end -> ready state (retail body).
// func_8029F5CC likewise lives in retail (mirror pane alpha);
// all three are extern here so func_8029F26C calls them with a `bl`, matching
// the retail dispatch (defined-in-TU copies would be inlined).
//
// func_8029F504: when the forward anim has reached the end frame (delta > 0),
// set state 2 and mirror the color-alpha byte (offset 0xB8) from the slot pane
// "str[0x52]" onto "str[0x41]".
// Retail callers emit a direct `bl` to this helper (see func_8029F26C); the
// pragma guard stops -inline auto from inlining the body into them.
#pragma push
#pragma auto_inline off
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
#pragma pop

// func_8029F5CC: when the reverse anim has reached the end frame (delta == 0),
// clear state 0, clear the slot pane "str[0x41]", then (always) mirror the
// color-alpha byte (offset 0xB8) from "str[0x52]" onto "str[0x41]".
#pragma push
#pragma auto_inline off
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
#pragma pop

// func_802A041C: advance the skip-timer animation to the frame computed from
// the slot key (mField2C*60 + mField2E), wrapped to the anim frame count, then
// stamp the slot text and run the sub-controller.
#pragma push
#pragma auto_inline off
void func_802A041C(CSkipTimer* self) {
    int total = (int)self->mField2C * 0x3c + (int)(s16)self->mField2E;
    float cur = lbl_eu_80662C78 * (float)total + lbl_eu_80664A50;
    float max = (float)(u16)self->mAnimTransform24->GetFrameSize();
    if (cur > max) {
        cur -= max;
    }
    self->mAnimTransform24->SetFrame(cur);
    self->mLayout2->Animate(0);
    // Retail passes the CSkipTimer2 sub-controller (+0x70): its mField18 layout
    // is the pane stamped by func_8029F440.
    func_8029F440(reinterpret_cast<CSkipTimer*>(&self->mSkipTimer2Data[0]),
                  self->mField2C, (s16)self->mField2E);
}
#pragma pop

// func_8029F440: format slot name and stamp it into the layout pane.
__declspec(noinline) void func_8029F440(CSkipTimer* self, int arg1, int arg2) {
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
    __ct__CSkipTimer2(reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0]), 0);
}

// CSkipTimer destructor.
// Member dtors and operator delete (retail names). UnkClass_8045F564's dtor
// is declared as a real C++ member in its header; the call below goes
// through the class to keep the __dt__17UnkClass_8045F564Fv reloc.
extern "C" void __dt__11CSkipTimer2Fv(void* self, int flags);
extern "C" void __dt__7CSysWinFv(void* self, int flags);
extern "C" void __dl__FPv(void* p);

// Retail dtor is a plain free function: destroys mSkipTimer2 (0x70), CSysWin
// (0x34) and mMemRegion (0x04), then frees the object when the delete flag
// is positive.
#pragma optimize_for_size on
extern "C" void* __dt__10CSkipTimerFv(void* self, int flags) {
    if (self != 0) {
        __dt__11CSkipTimer2Fv((u8*)self + 0x70, -1);
        __dt__7CSysWinFv((u8*)self + 0x34, -1);
        ((UnkClass_8045F564*)((u8*)self + 0x04))->~UnkClass_8045F564();
        if (flags > 0)
            __dl__FPv(self);
    }
    return self;
}
#pragma optimize_for_size off

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
// auto_inline off: retail callers (func_802A0234) emit a direct `bl`.
#pragma push
#pragma auto_inline off
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
#pragma pop

// func_802A05E4 (CSkipTimer): bind anim24 to layout2 + animate.
// No cached `layout` temp: retail reloads mLayout2 before each virtual call.
// Pragma guard: retail callers (func_802A03AC / func_802A0234) emit a direct
// `bl` to this helper - do not inline it into them.
#pragma push
#pragma auto_inline off
void func_802A05E4(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform24);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform24, true);
    self->mLayout2->Animate(0);
}
#pragma pop

// func_802A055C (CSkipTimer): bind anim20 to layout2 + animate.
// Same no-temp shape as func_802A05E4; retail reloads mLayout2 per call.
#pragma push
#pragma auto_inline off
void func_802A055C(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform20);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform20, true);
    self->mLayout2->Animate(0);
}
#pragma pop

// func_802A04F0: map skip-timer index (mField2C s16) to an action id (1..7).
// if/else gives the exact retail dispatch tree (structural-0): 17 first, then
// the >=17 (<=18->5, >=19->6) arm, then <5->7 / ==5->1 / <10->2 / else->3.
// (Retail's leaf-block grouping implies a switch; this MWCC revision builds a
// 10-pivot tree for that switch, so the if/else chain is the byte-closest.)
// auto_inline off: retail callers (func_802A0234) emit a direct `bl`.
#pragma push
#pragma auto_inline off
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
#pragma pop

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
// No cached sub-controller pointer: retail reads the nested fields as direct
// self-relative offsets (0x88 / 0x90), so the casts are inlined.
// Retail saves r30/r31 with a single stmw; the unit's -O4,p would split them
// (+8 bytes) - per-function optimize_for_size selects the stmw/lmw pair.
#pragma push
#pragma optimize_for_size on
void func_8029FDBC(CSkipTimer* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField28 == 0) return;
    func_80137038(self->mLayout2, drawInfo, 0, 1);
    if (reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0])->mField20 != 0) {
        func_80137038(
            reinterpret_cast<nw4r::lyt::Layout*>(
                reinterpret_cast<CSkipTimer2*>(&self->mSkipTimer2Data[0])->mField18),
            drawInfo, 0, 1);
    }
    func_8022B7C8(&self->mSysWinData[0], drawInfo);
}
#pragma pop

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

// func_802A0148: engage the skip UI once the syswin panel is active (state 7 +
// reset the button, fire the fade sfx + bgm switch), else mark the timer
// active and cue sfx 6.
void func_802A0148(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (CSysWin_getUnk34(&self->mSysWinData[0]) != 0) {
        if (CSysWin_isActive(&self->mSysWinData[0]) == 0) return;
        self->mField29 = 7;
        self->mField2B = 0;
        func_8022B8E4(&self->mSysWinData[0]);
        func_80086B5C__Q22cf13CfGameManagerFv((u32)(u16)self->mField2C,
                                              (u32)(u16)self->mField2E, 1);
        if (func_800FEDF8() != 0) {
            func_800FF914();
        }
        func_80138078__FUl(3);
    } else {
        self->mActive = 1;
        func_80138078__FUl(6);
    }
}

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
// optimize_for_size selects the stmw/lmw save pair like retail; the unit's
// default -O4,p would emit individual stw's (+8B). __declspec(noinline) keeps
// the body out of OnFileEvent (retail emits a direct `bl`); plain
// auto_inline off is insufficient because optimize_for_size re-enables
// inlining.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void func_802A03AC(CSkipTimer* self) {
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
#pragma pop

// func_802A041C (CSkipTimer): sibling helper, not a harness target.
// Declared extern "C" at top (resolved to retail at link); not defined here.

// OnFileEvent: rebuild the whole skip-timer widget from the freshly-exchanged
// file data. Matches the CBgTex/CSaveLoad event pattern: create the scratch
// region (guarded by a Class_8045F858 RAII temp), attach the arc resource,
// build layout + both anims, bind them, then copy-initialize the embedded
// CSkipTimer2 sub-controller from a stack temporary (mem region via the
// UnkClass_8011C974 copy helper, rest field-by-field).
// optimize_for_size: retail saves r28-r31 with one stmw; the unit's -O4,p
// would split them into four stw's (+12B). Callees with auto_inline off
// (ctor/dtor/func_802A05E4/func_802A03AC) stay out-of-line.
#pragma push
#pragma optimize_for_size on
bool CSkipTimer::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
        mMemRegion.createRegion(mem2, 0x10000, &lbl_eu_80510568[0xbd], 0);
        Class_8045F858 memHost(&mMemRegion);
        void* fileData = mFileHandle->getData();
        mtl::MemManager::func_80434A4C(false);
        // 0x18 is the arc accessor here (func_8029FE30 releases it as one); the
        // header models it as mLayout (func_8029F440 stamps text through it).
        mLayout = reinterpret_cast<nw4r::lyt::Layout*>(CLibLayout::createArcResourceAccessor());
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout)
            ->Attach(fileData, &lbl_eu_80510568[0xc8]);
        func_80136E84(&mLayout2, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                      &lbl_eu_80510568[0xcc]);
        func_80136F08(mLayout2, &mAnimTransform20,
                      reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                      &lbl_eu_80510568[0xe5]);
        func_80136F08(mLayout2, &mAnimTransform24,
                      reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                      &lbl_eu_80510568[0x101]);
        func_802A055C(this);

        // Copy-initialize the embedded sub-controller from a stack temporary
        // (CMenuSave/CMenuCollepedia widget-rebuild pattern): build the temp via
        // the C-linkage ctor, copy the mem region through the UnkClass_8011C974
        // helper, then the remaining fields, then destroy the temp.
        // No cached temp/sub pointers: retail folds every address to sp/r28
        // relative (a pointer variable would be materialised in a saved reg).
        u8 temp[0x28];
        __ct__CSkipTimer2(reinterpret_cast<CSkipTimer2*>(temp), mLayout);
        __ct__UnkClass_8011C974(
            &reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mMemRegion,
            &reinterpret_cast<CSkipTimer2*>(temp)->mMemRegion);
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mParent =
            reinterpret_cast<CSkipTimer2*>(temp)->mParent;
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mField18 =
            reinterpret_cast<CSkipTimer2*>(temp)->mField18;
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mAnimTransform =
            reinterpret_cast<CSkipTimer2*>(temp)->mAnimTransform;
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mField20 =
            reinterpret_cast<CSkipTimer2*>(temp)->mField20;
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mField21 =
            reinterpret_cast<CSkipTimer2*>(temp)->mField21;
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mField22 =
            reinterpret_cast<CSkipTimer2*>(temp)->mField22;
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mField23 =
            reinterpret_cast<CSkipTimer2*>(temp)->mField23;
        reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0])->mField24 =
            reinterpret_cast<CSkipTimer2*>(temp)->mField24;
        reinterpret_cast<CSkipTimer2*>(temp)->~CSkipTimer2();
        func_8029F168(reinterpret_cast<CSkipTimer2*>(&mSkipTimer2Data[0]));
        func_802A03AC(this);
        mFileHandle = 0;
        mMemRegion.func_8045F810();
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

// func_8029F168: empty stub (body lives in retail). auto_inline off keeps the
// call out of OnFileEvent (retail emits a direct `bl`; the empty body would
// otherwise be inlined away under optimize_for_size).
#pragma push
#pragma auto_inline off
extern "C" void func_8029F168(CSkipTimer2* self) {}
#pragma pop

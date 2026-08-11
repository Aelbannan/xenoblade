// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <new>
#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuKizunagram.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/core/CPadManager.hpp"   // CPad flag words (getCurrentPad)
#include "kyoshin/cf/CfGameManager.hpp"   // cf::CfGameManager::getCurrentPad
#include "kyoshin/cf/CfPadData.hpp"        // cf::CfPadData layout

extern "C" void cbRenderBefore__15CMenuKizunagramFv(void*);

// ---------------------------------------------------------------------------
// ~CMenuKizunagram (us-80258c90) - complete-object dtor, retail D2 form.
// Written with an explicit flags parameter so the base-class destruction can
// target the retail CProcess dtor symbol __dt__800FED0C (the game-side D2
// wrapper). A real member destructor would make MWCC auto-emit
// __dt__8CProcessFv (the library D1) for the base call, which cannot match
// the retail reloc name at the call site. Subobjects are destroyed in
// reverse construction order, then conditional operator delete when flags
// > 0 (same shape as CMenuMapSelectSC's dtor).
// ---------------------------------------------------------------------------
extern "C" CMenuKizunagram* __dt__15CMenuKizunagramFv(CMenuKizunagram* _this, int flags) {
    if (_this != 0) {
        __dt__5CFadeFv(&_this->mFade, -1);
        __dt__5CLoadFv(&_this->mLoad, -1);
        __dt__13CPcKizunagramFv(&_this->mPcKizunagram, -1);
        __dt__11CKizunagramFv(&_this->mSub98, -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuKizunagram::Init() {}

void CMenuKizunagram::Term() {
    waitForDrawDone__9CDeviceVIFv();
    func_804962A0(reinterpret_cast<CScn*>(mParentRef), 1);

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    removeRenderCB__4CScnFP10IScnRender(reinterpret_cast<CScn*>(mParentRef), renderCB);

    func_801C40A0(&mTitleAHelp);
    func_8025C6F0(&mSub98);
    func_8025D9C4(&mPcKizunagram);
    func_802AE62C(&mLoad);
    func_8024448C(&mFade);

    lbl_eu_806647E0 = 0;
    func_8013B980();
    if (code80135FDC_getByte_64080() == 0) {
        func_8008294C__Q22cf13CfGameManagerFv(0);
    }
    func_801338C8();
}

void CMenuKizunagram::Move() {
    // Gate: when the global mode bit (0x200000) is set, skip the kizuna
    // Move body. The `if (A && B) goto body; goto end; end: return; body:`
    // chain (exit label BEFORE the body label) keeps the body off the
    // fallthrough so MWCC emits retail's branch-over-branch: `bne end` for
    // the first disjunct, `beq body; b end` for the second (MWCC_REFERENCE
    // §&&-gate branch-over-branch).
    getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv() == 0 && (lbl_eu_80663E28 & 0x200000) == 0) {
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
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
            bit = (pad->mPressedButtonFlags >> 23) & 1;
        } else {
            bit = (pad->mPressedButtonFlags >> 10) & 1;
        }
        if (bit != 0) {
            if (func_800FEDF8() != 0) {
                func_800FF914();
            }
            func_80138078(6);
            field_0x21C = 8;
            field_0x54 = 1;
        }
    }
    // Per-state helpers (0..10), then per-frame sub-object refresh.
    switch (field_0x21C) {
    case 0: func_80257318(this); break;
    case 1: func_80257360(this); break;
    case 2: func_802573B8(this); break;
    case 3: func_80257448(this); break;
    case 4: func_80257498(this); break;
    case 5: func_80257704(this); break;
    case 6: func_80257754(this); break;
    case 7: func_802577F0(this); break;
    case 8: func_80257840(this); break;
    case 9: func_80257994(this); break;
    case 10: func_80257A2C(this); break;
    }
    func_801C3FF0(&mTitleAHelp);
    func_8025C580(&mSub98);
    func_8025D8C4(&mPcKizunagram);
    func_802AE560(&mLoad);
    func_802443E8(&mFade);
}

void CMenuKizunagram::cbRenderBefore() {
    getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv() != 0) {
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
    if (func_8013BE50() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt
    // calls to match the retail direct calls (a C++ local would
    // virtual-dispatch its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_8025C61C(&mSub98, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_8025D954(&mPcKizunagram, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    // Draw the title/help bar once the per-character window is past the
    // opening phase (state >= 8) or when its visibility flag is set.
    if (field_0x21D != 0 || field_0x21C >= 8) {
        func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    }
    func_80244460(&mFade, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_802AE5F0(&mLoad, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
end:
    ;
}

// ---------------------------------------------------------------------------
// func_8025728C (us-802594c8) - create the kizuna-gram singleton: returns 0
// when it already exists, otherwise allocates the object (0x220 bytes),
// constructs it, stores the singleton and registers it as a CProcess child
// of `parent`.
// ---------------------------------------------------------------------------
CMenuKizunagram* func_8025728C(CProcess* parent, int a, int b) {
    if (lbl_eu_806647E0 != 0) {
        return 0;
    }

    CMenuKizunagram* obj = (CMenuKizunagram*)mtl::MemManager::allocate(
        0x220, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // The retail ctor returns the object in r3; assigning the result lets
        // MWCC reuse r3 after the call instead of preserving `obj` in a
        // callee-saved register (retail keeps only 3 saved regs here).
        obj = __ct__CMenuKizunagram(obj, a, b);
    }
    lbl_eu_806647E0 = (u32)obj;
    obj->Regist(parent, false);
    return (CMenuKizunagram*)lbl_eu_806647E0;
}

// ---------------------------------------------------------------------------
// __ct__CMenuKizunagram (us-80258ba4) - constructor with two explicit args:
// `a` becomes the parent-scene reference at 0x5C, `b` the byte at 0x21E.
// Written as a free function (retail unmangled global) so the base ctor call
// and the vtable fixups land in the retail order; subobjects are constructed
// in ascending address order.
// ---------------------------------------------------------------------------
extern "C" CMenuKizunagram* __ct__CMenuKizunagram(CMenuKizunagram* self, int a, int b) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    // Post-increment walk folds the first load into lwzu (btm_sco_init
    // pattern); the second group re-reads the base with plain loads so MWCC
    // reuses the already-materialised base register.
    u32* src = __ptmf_null;
    u32 pmfV0 = *src++;
    u32 pmfV1 = *src++;
    self->ptmf0[1] = pmfV1;
    self->ptmf0[0] = pmfV0;
    u32 pmfV2 = *src++;
    self->ptmf0[2] = pmfV2;
    u32* src2 = __ptmf_null;
    u32 pmfU0 = src2[0];
    u32 pmfU1 = src2[1];
    self->ptmf1[1] = pmfU1;
    self->ptmf1[0] = pmfU0;
    u32 pmfU2 = src2[2];
    self->ptmf1[2] = pmfU2;
    self->field_0x54 = 0;
    self->field_0x55 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_805374B0;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_805374B0 + 0x24;
    self->mParentRef = (CProcess*)a;

    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CKizunagram(&self->mSub98, 0);
    __ct__CPcKizunagram(&self->mPcKizunagram);
    __ct__CLoad(&self->mLoad, 0);
    __ct__CFade(&self->mFade);

    self->field_0x21C = 0;
    self->field_0x21D = 1;
    self->field_0x21E = (u8)b;
    func_80042874();
    func_8011C400();
    return self;
}


// ---------------------------------------------------------------------------
// func_80257318 (us-80259554)
// When the fade overlay is loaded/ready, mark the screen active (state byte)
// and start the fade-in animation.
// ---------------------------------------------------------------------------
void func_80257318(CMenuKizunagram* self) {
    if (func_80244508(&self->mFade) != 0) {
        self->field_0x21C = 1;
        func_80244518(&self->mFade);
    }
}

// ---------------------------------------------------------------------------
// func_80257360 (us-8025959c) - when the loading overlay and the fade are
// both ready, start the fade-out (state 2) and dismiss the overlay.
// ---------------------------------------------------------------------------
void func_80257360(CMenuKizunagram* self) {
    if (func_802AE6AC(&self->mLoad) != 0 && func_80244510(&self->mFade) != 0) {
        self->field_0x21C = 2;
        func_802AE6C4(&self->mLoad);
    }
}

// ---------------------------------------------------------------------------
// func_802573B8 (us-802595f4) - when the title help and kizuna chart are
// ready, sync the chart selection into the help bar, dismiss both, play the
// confirm sound and advance to state 3.
// ---------------------------------------------------------------------------
void func_802573B8(CMenuKizunagram* self) {
    if (func_801C4114(&self->mTitleAHelp) != 0 && func_8025C770(&self->mSub98) != 0) {
        func_801C41E8(&self->mTitleAHelp, func_8025CBCC(&self->mSub98));
        func_801C412C(&self->mTitleAHelp);
        func_8025C7D0(&self->mSub98);
        func_802AE758(&self->mLoad);
        func_80244538(&self->mFade);
        self->field_0x21C = 3;
        func_80138078(0x6d);
    }
}

// ---------------------------------------------------------------------------
// func_80257448 (us-80259684)
// When the title help is idle and the kizuna chart is open, show the
// per-character window (state 4).
// ---------------------------------------------------------------------------
void func_80257448(CMenuKizunagram* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8025C78C(&self->mSub98) != 0) {
        self->field_0x21C = 4;
    }
}

// ---------------------------------------------------------------------------
// func_80257498 (us-802596d4) - chart open state: poll classic/wiimote input
// and drive the kizuna-chart window transitions (state 5/6), the help-bar
// visibility toggle, and the per-direction cursor helpers. Then, when the
// chart is not busy, run the turbo-button cursor helpers (or the default
// chart refresh when none fired).
// ---------------------------------------------------------------------------
void func_80257498(CMenuKizunagram* self) {
    if (func_8025C78C(&self->mSub98) == 0) {
        return;
    }
    // Declaration order drives MWCC's callee-saved allocation.
    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    u32 f1, f2, f3, f4;
    u32 openWin, dismiss, toggle, cursorA, cursorB;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) == 0) {
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
        func_8025C870(&self->mSub98);
        if (func_8025CBBC(&self->mSub98) == 0 && func_8025CBC4(&self->mSub98) != 0) {
            func_801C4198(&self->mTitleAHelp);
            func_8025C7FC(&self->mSub98, 0);
            self->field_0x21C = 6;
        }
    } else if (dismiss != 0) {
        // Back out of the chart: dismiss the help bar and go to state 5.
        func_801C414C(&self->mTitleAHelp);
        func_8025C7FC(&self->mSub98, 1);
        self->field_0x21C = 5;
    } else if (toggle != 0) {
        // Toggle the help-bar visibility flag and play the corresponding SE.
        self->field_0x21D = (self->field_0x21D ^ 1) != 0;
        func_8025CC70(&self->mSub98);
        if (self->field_0x21D != 0) {
            func_80138078(0xd);
        } else {
            func_80138078(0xe);
        }
    } else if (cursorA != 0) {
        func_8025CAE4(&self->mSub98);
    } else if (cursorB != 0) {
        func_8025CB50(&self->mSub98);
    }
    // Mirror the chart selection into the help bar, then run the turbo
    // cursor helpers; refresh the chart only when none of them fired.
    func_801C41E8(&self->mTitleAHelp, func_8025CBCC(&self->mSub98));
    if (func_8025CBBC(&self->mSub98) == 0) {
        // changed stays 1 only when no turbo helper fired; the default
        // chart refresh then runs.
        int changed = 1;
        if (f1) {
            func_8025C874(&self->mSub98);
            changed = 0;
        }
        if (f2) {
            func_8025C904(&self->mSub98);
            changed = 0;
        }
        if (f3) {
            func_8025C994(&self->mSub98);
            changed = 0;
        }
        if (f4) {
            func_8025CA24(&self->mSub98);
            changed = 0;
        }
        if (changed) {
            func_8025CAB4(&self->mSub98);
        }
    }
}

// ---------------------------------------------------------------------------
// func_80257704 (us-80259940)
// When the title help is idle and the kizuna chart is open, set the phase
// flag at 0x54.
// ---------------------------------------------------------------------------
void func_80257704(CMenuKizunagram* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8025C78C(&self->mSub98) != 0) {
        self->field_0x54 = 1;
    }
}

void func_80257754(CMenuKizunagram* self) {
    // Open the per-character window: once the title bar, kizuna chart and PC
    // window are all idle/ready, set the help text from the string pool,
    // animate it, dismiss the chart and advance to state 8.
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8025C78C(&self->mSub98) != 0 &&
        func_8025DA40(&self->mPcKizunagram) != 0) {
        char* name = func_80136190(lbl_eu_8050CAB8, lbl_eu_8050CAB8 + 0xb, 2);
        func_801C41C0(&self->mTitleAHelp, name);
        func_801C41E8(&self->mTitleAHelp, 0x59);
        func_801C416C(&self->mTitleAHelp);
        func_8025DA50(&self->mPcKizunagram);
        self->field_0x21C = 8;
    }
}

// ---------------------------------------------------------------------------
// func_802577F0 (us-80259a2c)
// When the title help is idle and the per-character window is open, set the
// screen state to 9.
// ---------------------------------------------------------------------------
void func_802577F0(CMenuKizunagram* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8025DA48(&self->mPcKizunagram) != 0) {
        self->field_0x21C = 9;
    }
}

// ---------------------------------------------------------------------------
// func_80257840 (us-80259a7c) - react to confirm/cancel input while the
// per-character window is open: the classic controller's B/A buttons map to
// bits 10/9 of mPressedButtonFlags, the Wii Remote's to bits 27/26. Turbo
// presses on the classic shoulder/C buttons trigger per-direction cursor
// helpers on the PC window.
// ---------------------------------------------------------------------------
void func_80257840(CMenuKizunagram* self) {
    if (func_8025DA48(&self->mPcKizunagram) == 0) {
        return;
    }
    KizunaPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    u32 b, a, f1, f2, f3, f4;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        f1 = (turbo & 0x8004) != 0;
        f2 = ((turbo >> 15) & 1) | ((turbo >> 28) & 1);
        f3 = (turbo & 0x2001) != 0;
        b = (pressed >> 10) & 1;
        f4 = (turbo & 0x4002) != 0;
        a = (pressed >> 9) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        f1 = (turbo & 0x8004) != 0;
        f2 = ((turbo >> 15) & 1) | ((turbo >> 28) & 1);
        f3 = (turbo & 0x2001) != 0;
        b = (pressed >> 27) & 1;
        f4 = (turbo & 0x4002) != 0;
        a = (pressed >> 26) & 1;
    }
    if (b) {
        return;
    }
    if (a) {
        func_801C4198(&self->mTitleAHelp);
        func_8025DA78(&self->mPcKizunagram);
        self->field_0x21C = 0xa;
    } else if (f1) {
        func_8025DAE8(&self->mPcKizunagram);
    } else if (f2) {
        func_8025DB30(&self->mPcKizunagram);
    } else if (f3) {
        func_8025DB78(&self->mPcKizunagram);
    } else if (f4) {
        func_8025DBC0(&self->mPcKizunagram);
    }
}

// ---------------------------------------------------------------------------
// func_80257994 (us-80259bd0) - when the title help is idle and the
// per-character window is open, set the help bar text from the kizuna chart
// string pool, start its animation, dismiss the chart and advance to state 7.
// ---------------------------------------------------------------------------
void func_80257994(CMenuKizunagram* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8025DA48(&self->mPcKizunagram) != 0) {
        char* name = func_80136190(lbl_eu_8050CAB8, lbl_eu_8050CAB8 + 0xb, 1);
        func_801C41C0(&self->mTitleAHelp, name);
        func_801C41E8(&self->mTitleAHelp, func_8025CBCC(&self->mSub98));
        func_801C416C(&self->mTitleAHelp);
        func_8025C7D0(&self->mSub98);
        self->field_0x21C = 7;
    }
}

// ---------------------------------------------------------------------------
// func_80257A2C (us-80259c68)
// When the title help is idle and the kizuna chart is open, show the
// per-character window (state 4). Same shape as func_80257448.
// ---------------------------------------------------------------------------
void func_80257A2C(CMenuKizunagram* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_8025C78C(&self->mSub98) != 0) {
        self->field_0x21C = 4;
    }
}

// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
extern "C" void func_80257A7C(void* self) {
    ((void(*)(void*))cbRenderBefore__15CMenuKizunagramFv)((char*)self - 0x58);
}

// IScnRender vtable adjustor thunk for ~CMenuKizunagram.
extern "C" void func_80257A84(void* self) {
    ((void(*)(void*))__dt__15CMenuKizunagramFv)((char*)self - 0x58);
}

extern "C" unsigned long func_80257308(void) { return lbl_eu_806647E0 != 0; }

// Catalog TU for kyoshin/menu/CMenuCollepedia
// FULL_MATCH: Colle_ThunkRender, Colle_ThunkDtor

#include "kyoshin/menu/CMenuCollepedia.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"

extern "C" void cbRenderBefore__15CMenuCollepediaFv(void*);

// This-unit phase handlers (retail-unmangled func_ names), referenced by
// func_80252DD8 before their definitions below. extern "C" keeps the call
// relocs bare (retail keeps the unmangled names at these call sites).
extern "C" void Colle_NavigateBack(CMenuCollepedia* self);
extern "C" void Colle_CloseMenu(CMenuCollepedia* self);
extern "C" void func_80252CE4(CMenuCollepedia* self);
extern "C" void Colle_OpenMenu(CMenuCollepedia* self);
extern "C" void func_80252DD8(CMenuCollepedia* self);
extern "C" void Colle_MarkClosing(CMenuCollepedia* self);
extern "C" void clpDoFrameUpd(CCollepedia* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void updateHelp(CTitleAHelp* self);

// Complete-object dtor (us-8025494c). Free-function D2 form so the base
// call reloc is retail __dt__800FED0C (game D2), not library __dt__8CProcessFv.
extern "C" void __dt__800FED0C(CProcess* self, int flags);
extern "C" void __dl__FPv(void* p);
extern "C" CMenuCollepedia* __dt__15CMenuCollepediaFv(CMenuCollepedia* self,
                                                     int flags);

// Retail constructor symbol (unmangled global in US). Written as a free
// function so the factory emits a real bl to the bare retail symbol; returns
// `this` in r3 like retail. Mirrors the CMenuSave ctor pattern: base ctor,
// temp vtable store, null PMF data copy, then the composite vtable + the
// IScnRender sub-vtable at +0x58, then each embedded widget's ctor and the
// final state byte + menu timer.
extern "C" __declspec(noinline) CMenuCollepedia* __ct__CMenuCollepedia(
    CMenuCollepedia* self, CScn* scene) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    self->ptmf0[0] = __ptmf_null[0];
    self->ptmf0[1] = __ptmf_null[1];
    self->ptmf0[2] = __ptmf_null[2];
    self->ptmf1[0] = __ptmf_null[0];
    self->ptmf1[1] = __ptmf_null[1];
    self->ptmf1[2] = __ptmf_null[2];
    self->mField54 = 0;
    self->mField55 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_805372A0;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_805372A0 + 0x24;
    self->mScene = scene;

    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CCollepedia(&self->mCollepedia);
    self->mState = 0;
    self->mTimer = lbl_eu_806687E0;
    return self;
}

// Complete-object dtor (us-8025494c). Free-function D2 form so the base
// call reloc is retail __dt__800FED0C (game D2), not library __dt__8CProcessFv.
extern "C" CMenuCollepedia* __dt__15CMenuCollepediaFv(CMenuCollepedia* self,
                                                     int flags) {
    if (self != 0) {
        __dt__11CCollepediaFv(&self->mCollepedia, -1);
        __dt__11CTitleAHelpFv(&self->mTitleAHelp, -1);
        __dt__6CBgTexFv(&self->mBgTex, -1);
        __dt__800FED0C(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Re-initialise the collepedia menu screen: rebuild each embedded widget
// (CBgTex / CTitleAHelp / CCollepedia) by constructing a stack temporary and
// copying its body into the embedded storage, then register this screen as a
// render callback on the owning scene (same pattern as CMenuSave::Init).
void CMenuCollepedia::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__CBgTex((CBgTex*)tempBgTex, 0);

    __ct__UnkClass_8011C974(&mBgTex.mMemRegion, tempBgTex + 0x4);
    mBgTex.mFileHandle = *(CFileHandle**)(tempBgTex + 0x14);
    mBgTex.mLayout = *(nw4r::lyt::Layout**)(tempBgTex + 0x18);
    mBgTex.mLayoutReady = *(bool*)(tempBgTex + 0x1c);
    mBgTex.mLoaded = *(u8*)(tempBgTex + 0x1d);
    mBgTex.mPtmMode = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv((CBgTex*)tempBgTex, -1);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = BdatTouchStringCell(lbl_eu_8050C688, lbl_eu_8050C688 + 0xc, 0x9);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp((CTitleAHelp*)tempTitle, name, 0x50);

    __ct__UnkClass_8011C974(&mTitleAHelp.unk4, tempTitle + 0x4);
    mTitleAHelp.mFileHandle = *(CFileHandle**)(tempTitle + 0x14);
    mTitleAHelp.mArcResourceAccessor =
        *(nw4r::lyt::ArcResourceAccessor**)(tempTitle + 0x18);
    mTitleAHelp.mLayout = *(nw4r::lyt::Layout**)(tempTitle + 0x1c);
    mTitleAHelp.mAnimTrans20 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x20);
    mTitleAHelp.mAnimTrans24 = *(nw4r::lyt::AnimTransform**)(tempTitle + 0x24);
    mTitleAHelp.unk28 = *(u8*)(tempTitle + 0x28);
    mTitleAHelp.unk2c = *(s32*)(tempTitle + 0x2c);
    mTitleAHelp.mName = *(char**)(tempTitle + 0x30);
    mTitleAHelp.unk34 = *(u8*)(tempTitle + 0x34);
    mTitleAHelp.unk35 = *(u8*)(tempTitle + 0x35);
    mTitleAHelp.unk36 = *(u8*)(tempTitle + 0x36);
    mTitleAHelp.unk37 = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv((CTitleAHelp*)tempTitle, -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded CCollepedia via a temporary ---
    u8 tempCollepedia[0x2900];
    __ct__CCollepedia((CCollepedia*)tempCollepedia);

    __ct__UnkClass_8011C974(&mCollepedia.field_4, tempCollepedia + 0x4);
    __ct__UnkClass_8011C974(&mCollepedia._14[0], tempCollepedia + 0x14);

    mCollepedia.field_24 = *(void**)(tempCollepedia + 0x24);
    mCollepedia.field_28 = *(void**)(tempCollepedia + 0x28);
    mCollepedia.field_2C = *(void**)(tempCollepedia + 0x2C);
    mCollepedia.field_30 = *(nw4r::lyt::ArcResourceAccessor**)(tempCollepedia + 0x30);
    mCollepedia.field_34 = *(nw4r::lyt::ArcResourceAccessor**)(tempCollepedia + 0x34);
    mCollepedia.field_38 = *(nw4r::lyt::Layout**)(tempCollepedia + 0x38);
    mCollepedia.field_3C = *(nw4r::lyt::AnimTransform**)(tempCollepedia + 0x3C);
    mCollepedia.field_40 = *(nw4r::lyt::AnimTransform**)(tempCollepedia + 0x40);
    mCollepedia.field_44 = *(nw4r::lyt::AnimTransform**)(tempCollepedia + 0x44);
    mCollepedia.field_48 = *(u8*)(tempCollepedia + 0x48);
    mCollepedia.field_49 = *(u8*)(tempCollepedia + 0x49);
    mCollepedia.field_4C = *(u32*)(tempCollepedia + 0x4C);
    mCollepedia.field_50 = *(u8*)(tempCollepedia + 0x50);
    mCollepedia.field_51 = *(u8*)(tempCollepedia + 0x51);
    // Copy the 0x54 sub-object region via the 0x10-byte copy helper.
    func_8018B0FC((u8*)&mCollepedia + 0x54, tempCollepedia + 0x54);
    // Region inside the 0x54 sub-object pad: copied as raw words/bytes.
    *(u32*)((u8*)&mCollepedia + 0x70) = *(u32*)(tempCollepedia + 0x70);
    *(u32*)((u8*)&mCollepedia + 0x74) = *(u32*)(tempCollepedia + 0x74);
    *(u32*)((u8*)&mCollepedia + 0x78) = *(u32*)(tempCollepedia + 0x78);
    *(u32*)((u8*)&mCollepedia + 0x7C) = *(u32*)(tempCollepedia + 0x7C);
    *(u8*)((u8*)&mCollepedia + 0x80) = *(u8*)(tempCollepedia + 0x80);
    *(u8*)((u8*)&mCollepedia + 0x81) = *(u8*)(tempCollepedia + 0x81);
    // Copy the 0x84 sub-object region via the same helper.
    func_8018B0FC((u8*)&mCollepedia + 0x84, tempCollepedia + 0x84);
    __ct__UnkClass_8011C974((u8*)&mCollepedia + 0xA0, tempCollepedia + 0xA0);
    // Tail of the pre-loop region (+0xB0..+0xE9), then the sub-array loop.
    *(u32*)((u8*)&mCollepedia + 0xB0) = *(u32*)(tempCollepedia + 0xB0);
    u32 count = 0x500;
    CollepediaSubEntry* dst = (CollepediaSubEntry*)((u8*)&mCollepedia + 0xE8);
    CollepediaSubEntry* src = (CollepediaSubEntry*)(tempCollepedia + 0xE8);
    *(u32*)((u8*)&mCollepedia + 0xB4) = *(u32*)(tempCollepedia + 0xB4);
    *(u32*)((u8*)&mCollepedia + 0xB8) = *(u32*)(tempCollepedia + 0xB8);
    *(u32*)((u8*)&mCollepedia + 0xBC) = *(u32*)(tempCollepedia + 0xBC);
    *(u32*)((u8*)&mCollepedia + 0xC0) = *(u32*)(tempCollepedia + 0xC0);
    *(u8*)((u8*)&mCollepedia + 0xC4) = *(u8*)(tempCollepedia + 0xC4);
    *(u32*)((u8*)&mCollepedia + 0xC8) = *(u32*)(tempCollepedia + 0xC8);
    *(u32*)((u8*)&mCollepedia + 0xCC) = *(u32*)(tempCollepedia + 0xCC);
    *(u8*)((u8*)&mCollepedia + 0xD0) = *(u8*)(tempCollepedia + 0xD0);
    *(u8*)((u8*)&mCollepedia + 0xD1) = *(u8*)(tempCollepedia + 0xD1);
    *(u8*)((u8*)&mCollepedia + 0xD2) = *(u8*)(tempCollepedia + 0xD2);
    *(u8*)((u8*)&mCollepedia + 0xD3) = *(u8*)(tempCollepedia + 0xD3);
    *(u8*)((u8*)&mCollepedia + 0xD4) = *(u8*)(tempCollepedia + 0xD4);
    *(u8*)((u8*)&mCollepedia + 0xD5) = *(u8*)(tempCollepedia + 0xD5);
    *(u8*)((u8*)&mCollepedia + 0xD8) = *(u8*)(tempCollepedia + 0xD8);
    *(u8*)((u8*)&mCollepedia + 0xD9) = *(u8*)(tempCollepedia + 0xD9);
    *(u8*)((u8*)&mCollepedia + 0xDA) = *(u8*)(tempCollepedia + 0xDA);
    *(u32*)((u8*)&mCollepedia + 0xDC) = *(u32*)(tempCollepedia + 0xDC);
    *(u32*)((u8*)&mCollepedia + 0xE0) = *(u32*)(tempCollepedia + 0xE0);
    *(u32*)((u8*)&mCollepedia + 0xE4) = *(u32*)(tempCollepedia + 0xE4);
    *(u8*)((u8*)&mCollepedia + 0xE8) = *(u8*)(tempCollepedia + 0xE8);
    *(u8*)((u8*)&mCollepedia + 0xE9) = *(u8*)(tempCollepedia + 0xE9);

    // 0x500-entry sub-array at +0xE8 (8-byte stride) copied element by
    // element so MWCC emits the retail 8-byte lwzu/stwu loop.
    do {
        dst->field_4 = src->field_4;
        dst->field_0 = src->field_0;
        dst++;
        src++;
    } while (--count != 0);

    // Tail state (end of the embedded CCollepedia body).
    *(u32*)((u8*)&mCollepedia + 0x28EC) = *(u32*)(tempCollepedia + 0x28EC);
    mCollepedia.field_28F0 = *(nw4r::lyt::Layout**)(tempCollepedia + 0x28F0);
    *(u32*)((u8*)&mCollepedia + 0x28F4) = *(u32*)(tempCollepedia + 0x28F4);
    *(u8*)((u8*)&mCollepedia + 0x28F8) = *(u8*)(tempCollepedia + 0x28F8);
    mCollepedia.field_28F9 = *(u8*)(tempCollepedia + 0x28F9);
    mCollepedia.field_28FA = *(u8*)(tempCollepedia + 0x28FA);
    __dt__11CCollepediaFv((CCollepedia*)tempCollepedia, -1);

    clpStartLoads(&mCollepedia);

    // Register this screen as a render callback on the owning scene (the
    // `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = &mIScnRender;
    }
    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, cb, 0xd, 0);
}

// Tear down the collepedia menu: detach the render callback, release the
// background/title widgets and the collepedia core, clear the singleton, and
// hand control back to the game manager.
void CMenuCollepedia::Term() {
    waitForDrawDone__9CDeviceVIFv();

    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRender);
    }
    removeRenderCB__4CScnFP10IScnRender(mScene, cb);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    clpFreeAllRes(&mCollepedia);

    lbl_eu_806647D0 = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuCollepedia::Move() {
    switch (mState) {
    case 0:
        func_80252CE4(this);
        break;
    case 1:
        Colle_OpenMenu(this);
        break;
    case 2:
        func_80252DD8(this);
        break;
    case 3:
        Colle_MarkClosing(this);
        break;
    default:
        break;
    }
    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    clpDoFrameUpd(&mCollepedia);
}

void CMenuCollepedia::cbRenderBefore() {}

void func_80252C60(){}

void stub_us_80254f10() {}

void func_80252CE4(){}

// Advance the collepedia menu: once the title bar is idle and the collepedia
// core is ready, open the menu (state 2).
void Colle_OpenMenu(CMenuCollepedia* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (clpGetInputOk(&self->mCollepedia) != 0) {
            self->mState = 2;
        }
    }
}

// Advance the collepedia menu input (retail func_80252DD8): once the menu is
// open, tick the open timer (clamped to 10s), then read the pad and dispatch
// to the collepedia core / menu handlers. Classic-controller (co-op) and
// Wiimote pad layouts map the same actions to different button bits.
extern "C" void func_80252DD8(CMenuCollepedia* self) {
    if (GetItemMulti_IsActiveFlag() != 0) {
        return;
    }
    self->mTimer += lbl_eu_806687E4;
    if (self->mTimer > lbl_eu_806687E8) {
        self->mTimer = lbl_eu_806687E8;
    }

    MenuCollepediaPadData* pad =
        (MenuCollepediaPadData*)cf::CfGameManager::getCfPadData();
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // Classic-controller (co-op) layout.
        if (clpCanClaim(&self->mCollepedia) != 0) {
            func_801C41E8(&self->mTitleAHelp, 0x50);
        } else {
            func_801C41E8(&self->mTitleAHelp, 0x51);
        }
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((turbo >> 6) & 1) {
            clpPrevCatPg(&self->mCollepedia);
            return;
        }
        if ((turbo >> 5) & 1) {
            clpNextCatPg(&self->mCollepedia);
            return;
        }
        if ((pressed >> 10) & 1) {
            clpOnConfirm(&self->mCollepedia);
            return;
        }
        if ((pressed >> 9) & 1) {
            Colle_NavigateBack(self);
            return;
        }
        if (turbo & 0x8004) {
            clpCursorLeft(&self->mCollepedia);
            return;
        }
        if (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) {
            clpCursorRght(&self->mCollepedia);
            return;
        }
        if (turbo & 0x2001) {
            clpCursorUp(&self->mCollepedia);
            return;
        }
        if (turbo & 0x4002) {
            clpCursorDown(&self->mCollepedia);
            return;
        }
        if ((pressed >> 8) & 1) {
            Colle_CloseMenu(self);
            return;
        }
    } else {
        // Single-player Wiimote-style layout.
        if (clpCanClaim(&self->mCollepedia) != 0) {
            func_801C41E8(&self->mTitleAHelp, 0x50);
        } else {
            func_801C41E8(&self->mTitleAHelp, 0x51);
        }
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        // Holding the confirm button (bit 19) with the core idle plays the
        // open sound once and restarts the timer, then feeds the pad through.
        if (((pad->mPadHeldFlags >> 19) & 1) &&
            clpIsBusy(&self->mCollepedia) == 0) {
            func_801C41E8(&self->mTitleAHelp, 0x52);
            if (self->mTimer > lbl_eu_806687E4) {
                playUISound__FUl(2);
            }
            self->mTimer = lbl_eu_806687E0;
            if (turbo & 0x2001) {
                clpPrevCatPg(&self->mCollepedia);
                return;
            }
            if (turbo & 0x4002) {
                clpNextCatPg(&self->mCollepedia);
                return;
            }
            return;
        }
        if ((pressed >> 27) & 1) {
            clpOnConfirm(&self->mCollepedia);
            return;
        }
        if ((pressed >> 26) & 1) {
            Colle_NavigateBack(self);
            return;
        }
        if (turbo & 0x8004) {
            clpCursorLeft(&self->mCollepedia);
            return;
        }
        if (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) {
            clpCursorRght(&self->mCollepedia);
            return;
        }
        if (turbo & 0x2001) {
            clpCursorUp(&self->mCollepedia);
            return;
        }
        if (turbo & 0x4002) {
            clpCursorDown(&self->mCollepedia);
            return;
        }
        if ((pressed >> 21) & 1) {
            Colle_CloseMenu(self);
        }
    }
}

// Same idle+advance check as Colle_OpenMenu, but marks the menu as closing
// (mField54 = 1) instead of opening.
void Colle_MarkClosing(CMenuCollepedia* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (clpGetInputOk(&self->mCollepedia) != 0) {
            self->mField54 = 1;
        }
    }
}

// Close the collepedia menu: when the core is not blocking, run the close
// sequence (save prompt + close sound) and set state 4.
void Colle_CloseMenu(CMenuCollepedia* self) {
    if (clpIsBusy(&self->mCollepedia) == 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound__FUl(6);
        }
        self->mState = 4;
        self->mField54 = 1;
    }
}

// Navigate the collepedia menu: forward input to the core, or close the
// detail view and return to the list (state 3).
void Colle_NavigateBack(CMenuCollepedia* self) {
    if (clpIsOverlay(&self->mCollepedia) != 0) {
        clpOnCancel(&self->mCollepedia);
    } else {
        if (clpIsBusy(&self->mCollepedia) == 0) {
            beginClose(&self->mTitleAHelp);
            clpReqCloseVw(&self->mCollepedia);
            self->mState = 3;
        }
    }
}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuCollepedia.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__15CMenuCollepediaFv
 */
extern "C" void Colle_ThunkRender(void* self) {
    ((void(*)(void*))cbRenderBefore__15CMenuCollepediaFv)((char*)self - 0x58);
}

/**
 * IScnRender vtable this-adjusting thunk for destructor.
 *
 * Same adjustment as Colle_ThunkRender.
 *
 * Retail: subi r3, r3, 0x58; b __dt__15CMenuCollepediaFv
 */
extern "C" void Colle_ThunkDtor(void* self) {
    ((void (*)(CMenuCollepedia*))__dt__15CMenuCollepediaFv)(
        (CMenuCollepedia*)((char*)self - 0x58));
}

extern "C" int Colle_HasInstance(void) { return lbl_eu_806647D0 != 0; }

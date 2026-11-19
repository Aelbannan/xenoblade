// Catalog TU for kyoshin/menu/CMenuCollepedia
// FULL_MATCH: ctor, dtor, Colle_ThunkRender/Colle_ThunkDtor (now compiler-emitted thunks)

#include "kyoshin/menu/CMenuCollepedia.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// This-unit phase handlers (retail-unmangled func_ names), referenced by
// Colle_HandleInput before their definitions below. extern "C" keeps the call
// relocs bare (retail keeps the unmangled names at these call sites).
// (memberized: Colle_NavigateBack -> CMenuCollepedia::Colle_NavigateBack; declaration moved into class CMenuCollepedia)
// (memberized: Colle_CloseMenu -> CMenuCollepedia::Colle_CloseMenu; declaration moved into class CMenuCollepedia)
extern "C" void Colle_BeginOpen(CMenuCollepedia* self);
// (memberized: Colle_OpenMenu -> CMenuCollepedia::Colle_OpenMenu; declaration moved into class CMenuCollepedia)
// (memberized: Colle_HandleInput -> CMenuCollepedia::Colle_HandleInput; declaration moved into class CMenuCollepedia)
extern "C" void Colle_MarkClosing(CMenuCollepedia* self);
extern "C" void clpDoFrameUpd(CCollepedia* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);

CMenuCollepedia::CMenuCollepedia(CScn* scene)
    : mScene(scene), mBgTex(0), mTitleAHelp(nullptr, 0), mState(0), mTimer(0.0f) {}

CMenuCollepedia::~CMenuCollepedia() {}

// Re-initialise the collepedia menu screen: rebuild each embedded widget
// (CBgTex / CTitleAHelp / CCollepedia) by constructing a stack temporary and
// copying its body into the embedded storage, then register this screen as a
// render callback on the owning scene (same pattern as CMenuSave::Init).
void CMenuCollepedia::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    mBgTex = CBgTex(0);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = BdatTouchStringCell(lbl_eu_8050C688, lbl_eu_8050C688 + 0xc, 0x9);

    mTitleAHelp = CTitleAHelp(name, 0x50);

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CCollepedia via a temporary ---
    u8 tempCollepedia[0x2900];
    __ct__CCollepedia((CCollepedia*)tempCollepedia);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mCollepedia.field_4, tempCollepedia + 0x4);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mCollepedia._14[0], tempCollepedia + 0x14);

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
    *reinterpret_cast<CBaseCur*>((u8*)&mCollepedia + 0x54) = *reinterpret_cast<const CBaseCur*>(tempCollepedia + 0x54);
    // Region inside the 0x54 sub-object pad: copied as raw words/bytes.
    *(u32*)((u8*)&mCollepedia + 0x70) = *(u32*)(tempCollepedia + 0x70);
    *(u32*)((u8*)&mCollepedia + 0x74) = *(u32*)(tempCollepedia + 0x74);
    *(u32*)((u8*)&mCollepedia + 0x78) = *(u32*)(tempCollepedia + 0x78);
    *(u32*)((u8*)&mCollepedia + 0x7C) = *(u32*)(tempCollepedia + 0x7C);
    *(u8*)((u8*)&mCollepedia + 0x80) = *(u8*)(tempCollepedia + 0x80);
    *(u8*)((u8*)&mCollepedia + 0x81) = *(u8*)(tempCollepedia + 0x81);
    // Copy the 0x84 sub-object region via the same helper.
    *reinterpret_cast<CBaseCur*>((u8*)&mCollepedia + 0x84) = *reinterpret_cast<const CBaseCur*>(tempCollepedia + 0x84);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)&mCollepedia + 0xA0, tempCollepedia + 0xA0);
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

    // Register this screen as a render callback on the owning scene.
    mScene->addRenderCB(this, 0xd, 0);
}

// Tear down the collepedia menu: detach the render callback, release the
// background/title widgets and the collepedia core, clear the singleton, and
// hand control back to the game manager.
void CMenuCollepedia::Term() {
    waitForDrawDone__9CDeviceVIFv();

    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    clpFreeAllRes(&mCollepedia);

    lbl_eu_806647D0 = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuCollepedia::Move() {
    switch (mState) {
    case 0:
        Colle_BeginOpen(this);
        break;
    case 1:
        this->Colle_OpenMenu();
        break;
    case 2:
        this->Colle_HandleInput();
        break;
    case 3:
        Colle_MarkClosing(this);
        break;
    default:
        break;
    }
    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    clpDoFrameUpd(&mCollepedia);
}

void CMenuCollepedia::cbRenderBefore() {}

void CollepediaCreateSingleton(){}

void stub_us_80254f10() {}

void Colle_BeginOpen(){}

// Advance the collepedia menu: once the title bar is idle and the collepedia
// core is ready, open the menu (state 2).
void CMenuCollepedia::Colle_OpenMenu() {
    if (this->mTitleAHelp.func_801C4124() != 0) {
        if (clpGetInputOk(&this->mCollepedia) != 0) {
            this->mState = 2;
        }
    }
}

// Advance the collepedia menu input (retail Colle_HandleInput): once the menu is
// open, tick the open timer (clamped to 10s), then read the pad and dispatch
// to the collepedia core / menu handlers. Classic-controller (co-op) and
// Wiimote pad layouts map the same actions to different button bits.
void CMenuCollepedia::Colle_HandleInput() {
    if (GetItemMulti_IsActiveFlag() != 0) {
        return;
    }
    this->mTimer += 1.0f;
    if (this->mTimer > 10.0f) {
        this->mTimer = 10.0f;
    }

    MenuCollepediaPadData* pad =
        (MenuCollepediaPadData*)cf::CfGameManager::getCfPadData();
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Classic-controller (co-op) layout.
        if (clpCanClaim(&this->mCollepedia) != 0) {
            this->mTitleAHelp.func_801C41E8(0x50);
        } else {
            this->mTitleAHelp.func_801C41E8(0x51);
        }
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((turbo >> 6) & 1) {
            clpPrevCatPg(&this->mCollepedia);
            return;
        }
        if ((turbo >> 5) & 1) {
            clpNextCatPg(&this->mCollepedia);
            return;
        }
        if ((pressed >> 10) & 1) {
            clpOnConfirm(&this->mCollepedia);
            return;
        }
        if ((pressed >> 9) & 1) {
            this->Colle_NavigateBack();
            return;
        }
        if (turbo & 0x8004) {
            clpCursorLeft(&this->mCollepedia);
            return;
        }
        if (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) {
            clpCursorRght(&this->mCollepedia);
            return;
        }
        if (turbo & 0x2001) {
            clpCursorUp(&this->mCollepedia);
            return;
        }
        if (turbo & 0x4002) {
            clpCursorDown(&this->mCollepedia);
            return;
        }
        if ((pressed >> 8) & 1) {
            this->Colle_CloseMenu();
            return;
        }
    } else {
        // Single-player Wiimote-style layout.
        if (clpCanClaim(&this->mCollepedia) != 0) {
            this->mTitleAHelp.func_801C41E8(0x50);
        } else {
            this->mTitleAHelp.func_801C41E8(0x51);
        }
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        // Holding the confirm button (bit 19) with the core idle plays the
        // open sound once and restarts the timer, then feeds the pad through.
        if (((pad->mPadHeldFlags >> 19) & 1) &&
            clpIsBusy(&this->mCollepedia) == 0) {
            this->mTitleAHelp.func_801C41E8(0x52);
            if (this->mTimer > 1.0f) {
                playUISound(2);
            }
            this->mTimer = 0.0f;
            if (turbo & 0x2001) {
                clpPrevCatPg(&this->mCollepedia);
                return;
            }
            if (turbo & 0x4002) {
                clpNextCatPg(&this->mCollepedia);
                return;
            }
            return;
        }
        if ((pressed >> 27) & 1) {
            clpOnConfirm(&this->mCollepedia);
            return;
        }
        if ((pressed >> 26) & 1) {
            this->Colle_NavigateBack();
            return;
        }
        if (turbo & 0x8004) {
            clpCursorLeft(&this->mCollepedia);
            return;
        }
        if (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) {
            clpCursorRght(&this->mCollepedia);
            return;
        }
        if (turbo & 0x2001) {
            clpCursorUp(&this->mCollepedia);
            return;
        }
        if (turbo & 0x4002) {
            clpCursorDown(&this->mCollepedia);
            return;
        }
        if ((pressed >> 21) & 1) {
            this->Colle_CloseMenu();
        }
    }
}

// Same idle+advance check as Colle_OpenMenu, but marks the menu as closing
// (mField54 = 1) instead of opening.
void Colle_MarkClosing(CMenuCollepedia* self) {
    if (self->mTitleAHelp.func_801C4124() != 0) {
        if (clpGetInputOk(&self->mCollepedia) != 0) {
            self->mField54 = 1;
        }
    }
}

// Close the collepedia menu: when the core is not blocking, run the close
// sequence (save prompt + close sound) and set state 4.
void CMenuCollepedia::Colle_CloseMenu() {
    if (clpIsBusy(&this->mCollepedia) == 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound(6);
        }
        this->mState = 4;
        this->mField54 = 1;
    }
}

// Navigate the collepedia menu: forward input to the core, or close the
// detail view and return to the list (state 3).
void CMenuCollepedia::Colle_NavigateBack() {
    if (clpIsOverlay(&this->mCollepedia) != 0) {
        clpOnCancel(&this->mCollepedia);
    } else {
        if (clpIsBusy(&this->mCollepedia) == 0) {
            this->mTitleAHelp.beginClose();
            clpReqCloseVw(&this->mCollepedia);
            this->mState = 3;
        }
    }
}

extern "C" int Colle_HasInstance(void) { return lbl_eu_806647D0 != 0; }

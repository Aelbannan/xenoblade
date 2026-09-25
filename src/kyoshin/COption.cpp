// FULL_MATCH: CMenuOption_renderBeforeAdj58, CMenuOption_dtorAdj58

#include "kyoshin/menu/CMenuOption.hpp"

// NOTE: kyoshin/harness_catalog.hpp deliberately omitted - its CScn clashes
// with monolib/scn.hpp (pulled in by CTaskGame.hpp below).
#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

/* Retail constructor symbol (unmangled member form). Mirrors the matched
 * CMenuKizunaTalkList / CCol6Invite ctor pattern: base CProcess ctor, temp
 * vtable store, null PMF data copy (two 3-word slots at 0x3C/0x48), then the
 * composite vtable + IScnRender sub-vtable at +0x58, then each embedded
 * widget's ctor and the final state bytes. */
CMenuOption* __ct__CMenuOption(CMenuOption* _this, CProcess* parent, u32 arg) {
    __ct__8CProcessFv((CProcess*)_this);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58.
    reinterpret_cast<CMenuOptionVtblView*>(_this)->mProcVtable = (u32*)lbl_eu_8052BF70;
    // Post-increment walk forces MWCC's lwzu fold for the base (btm_sco_init
    // pattern). The second group restarts from the array base so its loads
    // stay at disp 0/4/8.
    u32 pmfV0;
    u32 pmfV1;
    u32 pmfV2;
    u32 pmfU1;
    u32 pmfU0;
    u32 pmfU2;
    u32* src = __ptmf_null;
    pmfV0 = *src++;
    pmfV1 = *src++;
    _this->mPtmfCallbacks[1] = pmfV1;
    _this->mPtmfCallbacks[0] = pmfV0;
    pmfV2 = *src++;
    _this->mPtmfCallbacks[2] = pmfV2;
    src = __ptmf_null;
    pmfU0 = *src++;
    pmfU1 = *src++;
    _this->mPtmfCallbacks[4] = pmfU1;
    _this->mPtmfCallbacks[3] = pmfU0;
    pmfU2 = *src++;
    _this->mPtmfCallbacks[5] = pmfU2;
    _this->mField54 = 0;
    _this->mField55 = 0;

    CMenuOptionVtblView* vtSlots = reinterpret_cast<CMenuOptionVtblView*>(_this);
    vtSlots->mProcVtable = (u32*)lbl_eu_805392C8;
    vtSlots->mScnRenderVt = (u32*)(lbl_eu_805392C8 + 0x24);
    _this->mParentRef = parent;

    __ct__CBgTex(&_this->mBgTex, 0);
    __ct__CTitleAHelp(&_this->mTitleAHelp, 0, 0);
    __ct__COption(&_this->mOption, 0);
    _this->mState = 0;
    _this->mArg = arg;
    return _this;
}

// ---------------------------------------------------------------------------
// CMenuOption::~CMenuOption (us-8029dc90) - complete-object dtor.
// Written as the retail D2 form (explicit flags param) so the base-class
// destruction can target the retail CProcess dtor symbol __dt__800FED0C
// (0x800FF7F4, the game-side D2 wrapper). A real member destructor would make
// MWCC auto-emit __dt__8CProcessFv (the library D1 at 0x804474D0) for the
// base call, which cannot match the retail reloc name at the call site.
// Subobjects are destroyed in reverse construction order, then conditional
// operator delete when flags > 0 (same shape as CSysWinSave's dtor).
// ---------------------------------------------------------------------------
CMenuOption* __dt__11CMenuOptionFv(CMenuOption* _this, int flags) {
    if (_this != 0) {
        __dt__7COptionFv(&_this->mOption, -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

/* Re-initialise each embedded widget via a temporary object + field copy,
 * following CMenuTutorial::Init / CMenuSkipTimer::Init / CMenuKizunaTalkList::
 * Init. Each widget is constructed on the stack, its unk4 sub-region rebuilt
 * with its copy-ctor, the remaining scalar fields copied out, the temp
 * destroyed, then the member's load/readFile helper is invoked. Finally the
 * widget is registered as an IScnRender render callback on its parent scene. */
void CMenuOption::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the embedded CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__CBgTex(reinterpret_cast<CBgTex*>(tempBgTex), 0);

    __ct__UnkClass_8011C974((u8*)this + 0x64, tempBgTex + 0x4);
    *(u32*)((u8*)this + 0x74) = *(u32*)(tempBgTex + 0x14);
    *(u32*)((u8*)this + 0x78) = *(u32*)(tempBgTex + 0x18);
    *(u8*)((u8*)this + 0x7c) = *(u8*)(tempBgTex + 0x1c);
    *(u8*)((u8*)this + 0x7d) = *(u8*)(tempBgTex + 0x1d);
    *(u8*)((u8*)this + 0x7e) = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv(reinterpret_cast<CBgTex*>(tempBgTex), -1);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = BdatTouchStringCell(lbl_eu_805103C4, lbl_eu_805103C4 + 9, 0x3b);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(tempTitle), name, 0x6c);

    __ct__UnkClass_8011C974((u8*)this + 0x84, tempTitle + 0x4);
    *(u32*)((u8*)this + 0x94) = *(u32*)(tempTitle + 0x14);
    *(u32*)((u8*)this + 0x98) = *(u32*)(tempTitle + 0x18);
    *(u32*)((u8*)this + 0x9c) = *(u32*)(tempTitle + 0x1c);
    *(u32*)((u8*)this + 0xa0) = *(u32*)(tempTitle + 0x20);
    *(u32*)((u8*)this + 0xa4) = *(u32*)(tempTitle + 0x24);
    *(u8*)((u8*)this + 0xa8) = *(u8*)(tempTitle + 0x28);
    *(u32*)((u8*)this + 0xac) = *(u32*)(tempTitle + 0x2c);
    *(u32*)((u8*)this + 0xb0) = *(u32*)(tempTitle + 0x30);
    *(u8*)((u8*)this + 0xb4) = *(u8*)(tempTitle + 0x34);
    *(u8*)((u8*)this + 0xb5) = *(u8*)(tempTitle + 0x35);
    *(u8*)((u8*)this + 0xb6) = *(u8*)(tempTitle + 0x36);
    *(u8*)((u8*)this + 0xb7) = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(tempTitle), -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded COption via a temporary ---
    u8 tempOption[0x104];
    __ct__COption(reinterpret_cast<COption*>(tempOption), mArg);

    __ct__UnkClass_8011C974((u8*)this + 0xbc, tempOption + 0x4);
    *(u32*)((u8*)this + 0xcc) = *(u32*)(tempOption + 0x14);
    *(u32*)((u8*)this + 0xd0) = *(u32*)(tempOption + 0x18);
    *(u32*)((u8*)this + 0xd4) = *(u32*)(tempOption + 0x1c);
    *(u32*)((u8*)this + 0xd8) = *(u32*)(tempOption + 0x20);
    *(u32*)((u8*)this + 0xdc) = *(u32*)(tempOption + 0x24);
    *(u8*)((u8*)this + 0xe0) = *(u8*)(tempOption + 0x28);
    *(u8*)((u8*)this + 0xe1) = *(u8*)(tempOption + 0x29);
    *(u8*)((u8*)this + 0xe2) = *(u8*)(tempOption + 0x2a);
    *(u8*)((u8*)this + 0xe3) = *(u8*)(tempOption + 0x2b);
    *(u8*)((u8*)this + 0xe4) = *(u8*)(tempOption + 0x2c);
    *(u8*)((u8*)this + 0xe5) = *(u8*)(tempOption + 0x2d);
    *(u8*)((u8*)this + 0xe6) = *(u8*)(tempOption + 0x2e);
    *(u8*)((u8*)this + 0xe7) = *(u8*)(tempOption + 0x2f);
    *(u8*)((u8*)this + 0xe8) = *(u8*)(tempOption + 0x30);
    *(u8*)((u8*)this + 0xe9) = *(u8*)(tempOption + 0x31);
    *(u8*)((u8*)this + 0xea) = *(u8*)(tempOption + 0x32);
    *(u8*)((u8*)this + 0xeb) = *(u8*)(tempOption + 0x33);
    *(u8*)((u8*)this + 0xec) = *(u8*)(tempOption + 0x34);

    __ct__UnkClass_8011C974((u8*)this + 0xf4, tempOption + 0x3c);
    *(u32*)((u8*)this + 0x104) = *(u32*)(tempOption + 0x4c);
    *(u32*)((u8*)this + 0x108) = *(u32*)(tempOption + 0x50);
    *(u32*)((u8*)this + 0x10c) = *(u32*)(tempOption + 0x54);
    *(u32*)((u8*)this + 0x110) = *(u32*)(tempOption + 0x58);
    *(u8*)((u8*)this + 0x114) = *(u8*)(tempOption + 0x5c);
    *(u8*)((u8*)this + 0x115) = *(u8*)(tempOption + 0x5d);
    *(u8*)((u8*)this + 0x116) = *(u8*)(tempOption + 0x5e);
    *(u8*)((u8*)this + 0x117) = *(u8*)(tempOption + 0x5f);
    *(f32*)((u8*)this + 0x118) = *(f32*)(tempOption + 0x60);
    *(f32*)((u8*)this + 0x11c) = *(f32*)(tempOption + 0x64);
    *(f32*)((u8*)this + 0x120) = *(f32*)(tempOption + 0x68);
    *(f32*)((u8*)this + 0x124) = *(f32*)(tempOption + 0x6c);
    *(f32*)((u8*)this + 0x128) = *(f32*)(tempOption + 0x70);
    *(u8*)((u8*)this + 0x12c) = *(u8*)(tempOption + 0x74);

    func_8018B0FC((u8*)this + 0x130, tempOption + 0x78);
    func_8018B0FC((u8*)this + 0x148, tempOption + 0x90);

    __ct__UnkClass_8011C974((u8*)this + 0x164, tempOption + 0xac);
    *(u32*)((u8*)this + 0x174) = *(u32*)(tempOption + 0xbc);
    *(u32*)((u8*)this + 0x178) = *(u32*)(tempOption + 0xc0);
    *(u32*)((u8*)this + 0x17c) = *(u32*)(tempOption + 0xc4);
    *(u32*)((u8*)this + 0x180) = *(u32*)(tempOption + 0xc8);
    *(u32*)((u8*)this + 0x184) = *(u32*)(tempOption + 0xcc);
    *(u8*)((u8*)this + 0x188) = *(u8*)(tempOption + 0xd0);
    *(u32*)((u8*)this + 0x18c) = *(u32*)(tempOption + 0xd4);
    *(u32*)((u8*)this + 0x190) = *(u32*)(tempOption + 0xd8);
    *(u8*)((u8*)this + 0x194) = *(u8*)(tempOption + 0xdc);
    *(u8*)((u8*)this + 0x195) = *(u8*)(tempOption + 0xdd);
    *(u8*)((u8*)this + 0x196) = *(u8*)(tempOption + 0xde);
    *(u8*)((u8*)this + 0x197) = *(u8*)(tempOption + 0xdf);
    *(u8*)((u8*)this + 0x198) = *(u8*)(tempOption + 0xe0);
    *(u8*)((u8*)this + 0x199) = *(u8*)(tempOption + 0xe1);

    func_8018B0FC((u8*)this + 0x19c, tempOption + 0xe4);
    *(u32*)((u8*)this + 0x1b4) = *(u32*)(tempOption + 0xfc);
    *(u32*)((u8*)this + 0x1b8) = *(u32*)(tempOption + 0x100);
    __dt__7COptionFv(reinterpret_cast<COption*>(tempOption), -1);

    COptionRebuildWidgets(&mOption);

    // Register the widget as an IScnRender render callback on its parent scene.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(renderCB, 0xd, 0);
}

void CMenuOption::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    COptionTeardown(&mOption);

    lbl_eu_80664A38 = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuOption::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0: advanceOptionPhase1(this); break;
    case 1: advanceOptionPhase2(this); break;
    case 2: func_8029BC78(this); break;
    case 3: flagOptionPhase1(this); break;
    }

    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    COptionTickState(&mOption);
}

void CMenuOption::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // (a C++ local would virtual-dispatch its scope-exit destructor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    BgTex_Draw_3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    COptionDraw(&mOption, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    drawHelp(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

/* Singleton factory (retail unmangled symbol). Returns 0 if an instance is
 * already live; otherwise allocates the 0x1C0-byte screen, constructs it,
 * stores it in the shared flag and registers it under `registParent`. */
CMenuOption* func_8029BB24(CProcess* registParent, CProcess* parent, u32 arg) {
    if (lbl_eu_80664A38 != 0) {
        return NULL;
    }
    CMenuOption* obj = (CMenuOption*)mtl::MemManager::allocate(
        0x1c0, CWorkThreadSystem::getWorkMem());
    if (obj != NULL) {
        obj = __ct__CMenuOption(obj, parent, arg);
    }
    lbl_eu_80664A38 = (u32)obj;
    Regist__8CProcessFP8CProcessb((CProcess*)obj, registParent, false);
    return (CMenuOption*)lbl_eu_80664A38;
}

/* Advance the option menu to phase 1 once the background texture, title bar
 * and option panel are all ready, then start the panel intro animations and
 * play the confirm sound effect (writes the state byte at 0x1BC). */
void advanceOptionPhase1(CMenuOption* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 && isInitialized(&self->mTitleAHelp) != 0 &&
        COptionIsWindowReady(&self->mOption) != 0) {
        func_801C412C(&self->mTitleAHelp);
        COptionBeginScrollSetup(&self->mOption);
        self->mState = 1;
        playUISound__FUl(0x6d);
    }
}

/* Advance the option menu to phase 2 once the title bar is idle and the
 * option panel has finished its intro (writes the state byte at 0x1BC). */
void advanceOptionPhase2(CMenuOption* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 && COptionGetLiveFlag(&self->mOption) != 0) {
        self->mState = 2;
    }
}

/* Option-menu input handler (retail func_8029BC78). Reads the current pad
 * state and dispatches to the embedded COption's directional/confirm handlers
 * depending on which control the player pressed this frame, then repaints the
 * title/help bar with the option's current index and advances the phase state
 * machine (mState / mField54). */
void func_8029BC78(CMenuOption* self) {
    if (COptionIsWindowReady(&self->mOption) == 0) {
        return;
    }

    CMenuOptionPadData* pad = (CMenuOptionPadData*)cf::CfGameManager::getCfPadData();

    // Classic controllers shift the directional/confirm trigger bits of the
    // pressed flags; Wiimote/Nunchuk use different positions. The four
    // directional buttons are read from the turbo-press flags.
    bool up;
    bool down;
    bool pageUp;
    bool pageDown;
    int left;
    int right;
    int pageLeft;
    int pageRight;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 21) & 1;
        right = (pressed >> 22) & 1;
        pageLeft = (pressed >> 28) & 1;
        pageRight = (pressed >> 23) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 4) & 1;
        right = (pressed >> 5) & 1;
        pageLeft = (pressed >> 7) & 1;
        pageRight = (pressed >> 10) & 1;
    }

    if (left) {
        COptionHandleLeft(&self->mOption);
    } else if (right) {
        COptionHandleRightAccept(&self->mOption, 0);
    } else if (up) {
        COptionCursorUp(&self->mOption);
    } else if (down) {
        COptionCursorDown(&self->mOption);
    } else if (pageUp) {
        COptionPageUp(&self->mOption);
    } else if (pageDown) {
        COptionPageDown(&self->mOption);
    } else if (pageLeft) {
        COptionOpenConfigDialog(&self->mOption);
    } else if (pageRight) {
        COptionHandleRightAccept(&self->mOption, 1);
    }

    func_801C41E8(&self->mTitleAHelp, (u8)COptionGetNavCode(&self->mOption));

    if (COptionGetConfirmGate(&self->mOption) != 0) {
        if (COptionGetSecondConfirm(&self->mOption) != 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
            }
            self->mState = 4;
            self->mField54 = 1;
        } else {
            beginClose(&self->mTitleAHelp);
            COptionConfirmSelection(&self->mOption);
            self->mState = 3;
        }
    }
}

/* Same idle+advance check as advanceOptionPhase2, but advances the option menu to
 * phase 1 (writes the state byte at offset 0x54). */
void flagOptionPhase1(CMenuOption* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 && COptionGetLiveFlag(&self->mOption) != 0) {
        self->mField54 = 1;
    }
}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * The IScnRender subobject sits at +0x58 within CMenuOption; vtable dispatch
 * through that subobject passes 'this' pointing at +0x58, so the thunk
 * subtracts it back and tail-calls the real implementation.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuOptionFv
 */
void CMenuOption_renderBeforeAdj58(void* self) {
    reinterpret_cast<CMenuOption*>(static_cast<char*>(self) - 0x58)->cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuOption.
 *
 * Same adjustment as CMenuOption_renderBeforeAdj58 but forwards to the destructor, leaving
 * r4 (delete flag) as caller leftover.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuOptionFv
 */
void CMenuOption_dtorAdj58(void* self) {
    ((void(*)(void*))__dt__11CMenuOptionFv)((char*)self - 0x58);
}

unsigned long hasOptionMenu(void) { return lbl_eu_80664A38 != 0; }

// --- restored from git history (base:gone); do not expand beyond these functions ---
// from commit 365650b84230 needle=sinit_8029E7D8
// LLM-HARNESS-END: us-802a0e58

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// LLM-HARNESS-BEGIN: us-802a1034
extern "C" void sinit_8029E7D8() {}


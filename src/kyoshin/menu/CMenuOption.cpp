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
    vtSlots->mScnRenderVt = (u32*)(lbl_eu_805392C8 + 9);
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

    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(&mBgTex.mMemRegion), reinterpret_cast<u8*>(&reinterpret_cast<CBgTex*>(tempBgTex)->mMemRegion));
    mBgTex.mFileHandle = reinterpret_cast<CBgTex*>(tempBgTex)->mFileHandle;
    mBgTex.mLayout = reinterpret_cast<CBgTex*>(tempBgTex)->mLayout;
    mBgTex.mLayoutReady = reinterpret_cast<CBgTex*>(tempBgTex)->mLayoutReady;
    mBgTex.mLoaded = reinterpret_cast<CBgTex*>(tempBgTex)->mLoaded;
    mBgTex.mPtmMode = reinterpret_cast<CBgTex*>(tempBgTex)->mPtmMode;
    __dt__6CBgTexFv(reinterpret_cast<CBgTex*>(tempBgTex), -1);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = BdatTouchStringCell(lbl_eu_805103C4, &lbl_eu_805103C4[9], 0x3b);

    u8 tempTitle[0x38];
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(tempTitle), name, 0x6c);

    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(&mTitleAHelp.unk4), reinterpret_cast<u8*>(&reinterpret_cast<CTitleAHelp*>(tempTitle)->unk4));
    mTitleAHelp.mFileHandle = reinterpret_cast<CTitleAHelp*>(tempTitle)->mFileHandle;
    mTitleAHelp.mArcResourceAccessor = reinterpret_cast<CTitleAHelp*>(tempTitle)->mArcResourceAccessor;
    // (per-statement temp views above keep the Init line count stable for -ipa file)
    mTitleAHelp.mLayout = reinterpret_cast<CTitleAHelp*>(tempTitle)->mLayout;
    mTitleAHelp.mAnimTrans20 = reinterpret_cast<CTitleAHelp*>(tempTitle)->mAnimTrans20;
    mTitleAHelp.mAnimTrans24 = reinterpret_cast<CTitleAHelp*>(tempTitle)->mAnimTrans24;
    mTitleAHelp.unk28 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk28;
    mTitleAHelp.unk2c = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk2c;
    mTitleAHelp.mName = reinterpret_cast<CTitleAHelp*>(tempTitle)->mName;
    mTitleAHelp.unk34 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk34;
    mTitleAHelp.unk35 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk35;
    mTitleAHelp.unk36 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk36;
    mTitleAHelp.unk37 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk37;
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(tempTitle), -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded COption via a temporary ---
    u8 tempOption[0x104];
    __ct__COption(reinterpret_cast<COption*>(tempOption), mArg);

    // Head of embedded COption matches COptionMem (region @+4), not class
    // COption's early members - same overlay OnFileEvent uses.
    __ct__UnkClass_8011C974(
        reinterpret_cast<u8*>(&reinterpret_cast<COptionMem*>(&mOption)->mRegion),
        reinterpret_cast<u8*>(&reinterpret_cast<COptionMem*>(tempOption)->mRegion));
    reinterpret_cast<COptionMem*>(&mOption)->mFHandle =
        reinterpret_cast<COptionMem*>(tempOption)->mFHandle;
    reinterpret_cast<COptionMem*>(&mOption)->mArcAcc =
        reinterpret_cast<COptionMem*>(tempOption)->mArcAcc;
    reinterpret_cast<COptionMem*>(&mOption)->mpLayout =
        reinterpret_cast<COptionMem*>(tempOption)->mpLayout;
    reinterpret_cast<COptionMem*>(&mOption)->mAnim0 =
        reinterpret_cast<COptionMem*>(tempOption)->mAnim0;
    reinterpret_cast<COptionMem*>(&mOption)->mAnim1 =
        reinterpret_cast<COptionMem*>(tempOption)->mAnim1;
    mOption.field_0x28 = reinterpret_cast<COption*>(tempOption)->field_0x28;
    mOption.field_0x29 = reinterpret_cast<COption*>(tempOption)->field_0x29;
    mOption.field_0x2A = reinterpret_cast<COption*>(tempOption)->field_0x2A;
    mOption.field_0x2B = reinterpret_cast<COption*>(tempOption)->field_0x2B;
    mOption.field_0x2C = reinterpret_cast<COption*>(tempOption)->field_0x2C;
    mOption.field_0x2D = reinterpret_cast<COption*>(tempOption)->field_0x2D;
    mOption.field_0x2E = reinterpret_cast<COption*>(tempOption)->field_0x2E;
    mOption.field_0x2F = reinterpret_cast<COption*>(tempOption)->field_0x2F;
    mOption.field_0x30 = reinterpret_cast<COption*>(tempOption)->field_0x30;
    mOption.field_0x31 = reinterpret_cast<COption*>(tempOption)->field_0x31;
    mOption.field_0x32 = reinterpret_cast<COption*>(tempOption)->field_0x32;
    mOption.field_0x33 = reinterpret_cast<COption*>(tempOption)->field_0x33;
    mOption.field_0x34 = reinterpret_cast<COption*>(tempOption)->field_0x34;

    // CScrollBar @ COption+0x38 (raw mScrollBar storage).
    __ct__UnkClass_8011C974(
        reinterpret_cast<u8*>(&reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mMemRegion),
        reinterpret_cast<u8*>(&reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mMemRegion));
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mFileHandle =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mFileHandle;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mAccessor =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mAccessor;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mLayout =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mLayout;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mAnimTransform =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mAnimTransform;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mReady = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mReady;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mVisible = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mVisible;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mState = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mState;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mActive = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mActive;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mAnimOffset = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mAnimOffset;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mScrollPosY = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mScrollPosY;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mScrollRatio = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mScrollRatio;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mThumbHeight = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mThumbHeight;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mContentHeight = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mContentHeight;
    reinterpret_cast<CScrollBar*>(mOption.mScrollBar)->mDirection = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mDirection;

    // CBaseCur sub-cursors @ +0x78 / +0x90.
    MenuShopSellCopyCursor(&mOption.mSubCur1, &reinterpret_cast<COption*>(tempOption)->mSubCur1);
    MenuShopSellCopyCursor(&mOption.mSubCur2, &reinterpret_cast<COption*>(tempOption)->mSubCur2);

    // CSysWin @ +0xA8 (mem region at +4); remaining fields still option-relative.
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(&reinterpret_cast<CSysWin*>(mOption.mSysWin)->mMemRegion), reinterpret_cast<u8*>(&reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mMemRegion));
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->mFileHandle = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mFileHandle;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->mTagProcessor = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mTagProcessor;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->mArcAccessor = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mArcAccessor;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->mLayout = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mLayout;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->mAnimTrans = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mAnimTrans;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_28 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_28;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_2C = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_2C;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_30 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_30;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_34 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_34;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_35 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_35;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_36 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_36;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_37 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_37;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_38 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_38;
    reinterpret_cast<CSysWin*>(mOption.mSysWin)->field_39 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_39;

    MenuShopSellCopyCursor(&mOption.mSubCur3, &reinterpret_cast<COption*>(tempOption)->mSubCur3);
    mOption.field_0xFC = reinterpret_cast<COption*>(tempOption)->field_0xFC;
    mOption.field_0x100 = reinterpret_cast<COption*>(tempOption)->field_0x100;
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

    lbl_eu_80664A38[0] = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuOption::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0: advanceOptionPhase1(this); break;
    case 1: advanceOptionPhase2(this); break;
    case 2: handleOptionInput(this); break;
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
    SetWideLocAdjust((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    BgTex_Draw_3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    COptionDraw(&mOption, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    drawHelp(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

/* Singleton factory (retail unmangled symbol). Returns 0 if an instance is
 * already live; otherwise allocates the 0x1C0-byte screen, constructs it,
 * stores it in the shared flag and registers it under `registParent`. */
CMenuOption* createOptionMenu(CProcess* registParent, CProcess* parent, u32 arg) {
    if (lbl_eu_80664A38[0] != 0) {
        return NULL;
    }
    CMenuOption* obj = (CMenuOption*)mtl::MemManager::allocate(
        0x1c0, CWorkThreadSystem::getWorkMem());
    if (obj != NULL) {
        obj = __ct__CMenuOption(obj, parent, arg);
    }
    lbl_eu_80664A38[0] = (u32)obj;
    Regist__8CProcessFP8CProcessb((CProcess*)obj, registParent, false);
    return (CMenuOption*)lbl_eu_80664A38[0];
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

/* Option-menu input handler (retail handleOptionInput). Reads the current pad
 * state and dispatches to the embedded COption's directional/confirm handlers
 * depending on which control the player pressed this frame, then repaints the
 * title/help bar with the option's current index and advances the phase state
 * machine (mState / mField54). */
void handleOptionInput(CMenuOption* self) {
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
void CMenuOption_renderBeforeAdj58(IScnRender* self) {
    reinterpret_cast<CMenuOption*>(reinterpret_cast<char*>(self) - 0x58)->cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuOption.
 *
 * Same adjustment as CMenuOption_renderBeforeAdj58 but forwards to the destructor, leaving
 * r4 (delete flag) as caller leftover.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuOptionFv
 */
void CMenuOption_dtorAdj58(IScnRender* self) {
    // Single-arg cast: retail thunk only adjusts this; delete flag stays in r4.
    ((void (*)(CMenuOption*))__dt__11CMenuOptionFv)(
        reinterpret_cast<CMenuOption*>(reinterpret_cast<char*>(self) - 0x58));
}

unsigned long hasOptionMenu(void) { return lbl_eu_80664A38[0] != 0; }

// Retail tables owned by this unit.
// .rodata (0x158): "CTutorial" head, the tutorial resource pool
// (lbl_eu_80510290, 17 strings), "CMenuOption" (lbl_eu_805103B8, read by the
// sdata locator below) and the title pool (lbl_eu_805103C4, used above at
// +0x144/+0x14D/+0x150). All boundaries are free so no alignment attributes
// (those flip codegen, cf. CBattery) - the head/Tex sizes absorb their pads.
// .sbss (0x8): singleton pointer + emitted zero word (retail section is 0x8;
// no code reads the second word).
// .sdata (0x10): two locator pairs {rodata-name, vtable-RTTI-slot}.
// .data (0x108): CTutorial vtable (34 slots: label lbl_eu_80539230),
// its RTTI trailer (label lbl_eu_805392B8), the CMenuOption vtable (14 slots:
// label lbl_eu_805392C8) and an anonymous RTTI trailer (14 words, static).
// Slot targets come from the retail rela.data (all addends +0). Real
// references where nameable (neither CTutorial nor IWorkEvent is in scope
// here, so plain extern "C" decls cannot clash; CMenuOption's own virtuals
// use canonical merging decls). Zero slots: Reset (inline stub danger),
// Draw (template-id), Tail (suffix mismatch), dtorAdj/renderAdj use their
// defined suffixed spellings, and CMenuOption's dtor uses its defined
// suffixed (deleting) spelling.
extern const char lbl_eu_80510280[0x10] = "CTutorial";
extern const char lbl_eu_80510290[0x128] = "menu/jp/Tutorial.arc\0" "/common/jp/bdat_menu_ttrl.bin\0" "menu/jp/tpl/c_ttrl_%03d.arc\0" "menu/jp/tpl/r_ttrl_%03d.arc\0" "MNU_ttrl\0" "page\0" "txt_pg00\0" "txt_pg01\0" "cttrl_i%03d_%02d.tpl\0" "rttrl_i%03d_%02d.tpl\0" "pic_cap\0" "CTutorial\0" "arc\0" "mf85_opt01_tut01.brlyt\0" "mf85_opt01_tut01_in.brlan\0" "mf85_opt01_tut01_info_in.brlan\0" "CTutorialTex";
extern const char lbl_eu_805103B8[0x0C] = "CMenuOption";
extern __declspec(section ".rodata") char lbl_eu_805103C4[0x14] =
    "MNU_main\0name\0\0\0\0\0\0";
u32 lbl_eu_80664A38[2] = { 0, 0 };
// .sdata (0x10): two locator pairs {rodata-name, vtable-RTTI-slot}.
extern u32 lbl_eu_805392B8[];
extern u32 lbl_eu_80539300[];
extern const char lbl_eu_805103B8[0x0C];
const void* lbl_eu_80662C50[2] = { lbl_eu_80510280, lbl_eu_805392B8 };
const void* lbl_eu_80662C58[2] = { lbl_eu_805103B8, lbl_eu_80539300 };
// .data (0x108): CTutorial vtable (34 slots: label lbl_eu_80539230), its RTTI
// trailer (label lbl_eu_805392B8), the CMenuOption vtable (14 slots: label
// lbl_eu_805392C8) and an anonymous RTTI trailer (14 words, static). The
// CMenuOption dtor uses its defined suffixed (deleting) spelling.
extern "C" void __dt__9CTutorialFv();
extern "C" void OnFileEvent__9CTutorialFP10CEventFile();
extern "C" void Init__11CMenuOptionFv(CMenuOption* self);
extern "C" void Term__11CMenuOptionFv(CMenuOption* self);
extern "C" void Move__11CMenuOptionFv(CMenuOption* self);
extern "C" void cbRenderBefore__11CMenuOptionFv(CMenuOption* self);
extern u32 lbl_eu_80661940[];
extern u32 lbl_eu_80661958[];
extern u32 lbl_eu_80661950[];
extern u32 lbl_eu_80661948[];
extern u32 lbl_eu_80661DE8[];
extern u32 lbl_eu_80661DE0[];
u32 lbl_eu_80539230[34] = {
    (u32)lbl_eu_80662C50, 0, (u32)__dt__9CTutorialFv,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
#pragma section data_type ".data" ".data"
__declspec(section ".data") u32 lbl_eu_805392B8[4] = { 0, 0, 0, 0 };
extern "C" void __dt__11CMenuOptionFv__FP11CMenuOptioni(CMenuOption* self, int flag);
u32 lbl_eu_805392C8[14] = {
    (u32)lbl_eu_80662C58, 0, (u32)__dt__11CMenuOptionFv__FP11CMenuOptioni,
    0, (u32)Init__11CMenuOptionFv, (u32)Term__11CMenuOptionFv,
    (u32)Move__11CMenuOptionFv, 0, 0,
    (u32)lbl_eu_80662C58, -88, (u32)CMenuOption_dtorAdj58, (u32)CMenuOption_renderBeforeAdj58,
    (u32)cbRenderBefore__11CMenuOptionFv,
};

u32 lbl_eu_80539300[14] = {
    (u32)lbl_eu_80661940, 88, (u32)lbl_eu_80661958, 0,
    (u32)lbl_eu_80661950, 0, (u32)lbl_eu_80661948, 0,
    (u32)lbl_eu_80661DE8, 0, (u32)lbl_eu_80661DE0, 0, 0, 0,
};

// FULL_MATCH: CMenuOption_renderBeforeAdj58, CMenuOption_dtorAdj58

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuOption.hpp"

// NOTE: kyoshin/harness_catalog.hpp deliberately omitted - its CScn clashes
// with monolib/scn.hpp (pulled in by CTaskGame.hpp below).
#include "kyoshin/CTaskGame.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"

void playUISound(u32);

// Option-menu helper entry points and imports (retail unmangled names; moved
// here from the header, which now carries only the class).
extern "C" CMenuOption* createOptionMenu(CProcess* registParent, CProcess* parent, u32 arg);
extern "C" u32 hasOptionMenu();

// Option-menu helper entry points (retail-unmangled callee names).
extern "C" void advanceOptionPhase2(CMenuOption* self);
extern "C" void flagOptionPhase1(CMenuOption* self);
extern "C" void advanceOptionPhase1(CMenuOption* self);
extern "C" void handleOptionInput(CMenuOption* self);

// Move()/cbRenderBefore() callees.
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void COptionTickState(COption* self);
extern "C" void COptionDraw(COption* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int IsMenuState621F0();
void SetWideLocAdjust(nw4r::lyt::DrawInfo* drawInfo);
extern u32 lbl_eu_80663E28;

// Ready/idle-check and animation helpers for the embedded sub-widgets
// (retail unmangles these member helpers; int returns so callers compare
// with cmpwi directly, no byte mask - matching retail).
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" int COptionIsWindowReady(COption* self);
extern "C" void COptionBeginScrollSetup(COption* self);

// Ctor/Init re-init helpers (retail-unmangled callee names - US strips
// mangling for these __ct__/func_ helpers).
extern "C" void __ct__6CBgTexFUc(CBgTex* self, u8 arg);
extern "C" void __ct__11CTitleAHelpFPcUc(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__COption(COption* self, u8 arg);
// __as__17UnkClass_8045F564FRC17UnkClass_8045F564 is declared by COption.hpp; BdatTouchStringCell by CFloorMap.hpp.
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void COptionRebuildWidgets(COption* self);

// Option-menu input-handler callees (retail-unmangled func_ names).
extern "C" void COptionHandleLeft(COption* self);
extern "C" void COptionHandleRightAccept(COption* self, int arg);
extern "C" void COptionCursorUp(COption* self);
extern "C" void COptionCursorDown(COption* self);
extern "C" void COptionPageUp(COption* self);
extern "C" void COptionPageDown(COption* self);
extern "C" void COptionOpenConfigDialog(COption* self);
extern "C" int COptionGetNavCode(COption* self);
extern "C" void COptionConfirmSelection(COption* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/COptionGetLiveFlag are declared int (not u8) so callers compare with
// cmpwi directly (no byte mask), matching retail.
extern "C" void COptionTeardown(COption* self);
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);
// CBgTex / CTitleAHelp helpers (retail unmangles these member helpers).
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void teardown(CTitleAHelp* self);

// D2-form subobject/base destructor helpers (explicit delete flags), used by
// the free-function form of ~CMenuOption. All are declared extern "C" so the
// literal retail symbol names are emitted (a plain C++ declaration would make
// retail symbol (0x800FF7F4 - the game-side D2 wrapper, distinct from the
// library D1 __dt__8CProcessFv at 0x804474D0).
extern "C" void __dt__7COptionFv(COption* _this, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* _this, int flags);
extern "C" void __dt__6CBgTexFv(CBgTex* _this, int flags);

// Shared singleton state flag (cleared by Term).
extern u32 lbl_eu_80664A38[2];

// Data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_805103C4[];  // option title/help string table base

/*
 * CfPadData overlay for the option-menu input handler (retail handleOptionInput).
 * Retail reads the current-pad pressed flags at +0x04 and the turbo-press
 * flags at +0x104 (the latter lives past CPad, in CfPadData's own flags
 * region; cf. CMenuMapSelect's CfPadDataLocal).
 */
struct CMenuOptionPadData {
    u32 mPadHeldFlags;           // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;        // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0x104 - 0x08];       // 0x08-0x103
    u32 mTurboPressButtonFlags;  // 0x104 - CfPadData::mTurboPressButtonFlags
};

/* Constructor (retail __ct__CMenuOption). The IUICf base builds the task header
 * and the IScnRender subobject sits at +0x58. CBgTex and CTitleAHelp are real
 * members built by the initialiser list; the COption bytes are constructed with
 * its flat retail ctor after the title bar, then the state bytes are stored. */
CMenuOption::CMenuOption(CProcess* parent, u32 arg)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(0, 0) {
    __ct__COption(reinterpret_cast<COption*>(mOption), 0);
    mState = 0;
    mArg = arg;
}

/* Destructor (retail __dt__11CMenuOptionFv). The COption bytes are torn down
 * here; the compiler then destroys mTitleAHelp, mBgTex and the bases. */
CMenuOption::~CMenuOption() {
    __dt__7COptionFv(reinterpret_cast<COption*>(mOption), -1);
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
    __ct__6CBgTexFUc(reinterpret_cast<CBgTex*>(tempBgTex), 0);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(reinterpret_cast<u8*>(&mBgTex.mMemRegion), reinterpret_cast<u8*>(&reinterpret_cast<CBgTex*>(tempBgTex)->mMemRegion));
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
    __ct__11CTitleAHelpFPcUc(reinterpret_cast<CTitleAHelp*>(tempTitle), name, 0x6c);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(reinterpret_cast<u8*>(&mTitleAHelp.unk4), reinterpret_cast<u8*>(&reinterpret_cast<CTitleAHelp*>(tempTitle)->unk4));
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

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded COption via a temporary ---
    u8 tempOption[0x104];
    __ct__COption(reinterpret_cast<COption*>(tempOption), mArg);

    // Head of embedded COption matches COptionMem (region @+4), not class
    // COption's early members - same overlay OnFileEvent uses.
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(
        reinterpret_cast<u8*>(&reinterpret_cast<COptionMem*>(mOption)->mRegion),
        reinterpret_cast<u8*>(&reinterpret_cast<COptionMem*>(tempOption)->mRegion));
    reinterpret_cast<COptionMem*>(mOption)->mFHandle =
        reinterpret_cast<COptionMem*>(tempOption)->mFHandle;
    reinterpret_cast<COptionMem*>(mOption)->mArcAcc =
        reinterpret_cast<COptionMem*>(tempOption)->mArcAcc;
    reinterpret_cast<COptionMem*>(mOption)->mpLayout =
        reinterpret_cast<COptionMem*>(tempOption)->mpLayout;
    reinterpret_cast<COptionMem*>(mOption)->mAnim0 =
        reinterpret_cast<COptionMem*>(tempOption)->mAnim0;
    reinterpret_cast<COptionMem*>(mOption)->mAnim1 =
        reinterpret_cast<COptionMem*>(tempOption)->mAnim1;
    reinterpret_cast<COption*>(mOption)->field_0x28 = reinterpret_cast<COption*>(tempOption)->field_0x28;
    reinterpret_cast<COption*>(mOption)->field_0x29 = reinterpret_cast<COption*>(tempOption)->field_0x29;
    reinterpret_cast<COption*>(mOption)->field_0x2A = reinterpret_cast<COption*>(tempOption)->field_0x2A;
    reinterpret_cast<COption*>(mOption)->field_0x2B = reinterpret_cast<COption*>(tempOption)->field_0x2B;
    reinterpret_cast<COption*>(mOption)->field_0x2C = reinterpret_cast<COption*>(tempOption)->field_0x2C;
    reinterpret_cast<COption*>(mOption)->field_0x2D = reinterpret_cast<COption*>(tempOption)->field_0x2D;
    reinterpret_cast<COption*>(mOption)->field_0x2E = reinterpret_cast<COption*>(tempOption)->field_0x2E;
    reinterpret_cast<COption*>(mOption)->field_0x2F = reinterpret_cast<COption*>(tempOption)->field_0x2F;
    reinterpret_cast<COption*>(mOption)->field_0x30 = reinterpret_cast<COption*>(tempOption)->field_0x30;
    reinterpret_cast<COption*>(mOption)->field_0x31 = reinterpret_cast<COption*>(tempOption)->field_0x31;
    reinterpret_cast<COption*>(mOption)->field_0x32 = reinterpret_cast<COption*>(tempOption)->field_0x32;
    reinterpret_cast<COption*>(mOption)->field_0x33 = reinterpret_cast<COption*>(tempOption)->field_0x33;
    reinterpret_cast<COption*>(mOption)->field_0x34 = reinterpret_cast<COption*>(tempOption)->field_0x34;

    // CScrollBar @ COption+0x38 (raw mScrollBar storage).
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(
        reinterpret_cast<u8*>(&reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mMemRegion),
        reinterpret_cast<u8*>(&reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mMemRegion));
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mFileHandle =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mFileHandle;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mAccessor =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mAccessor;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mLayout =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mLayout;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mAnimTransform =
        reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mAnimTransform;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mReady = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mReady;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mVisible = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mVisible;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mState = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mState;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mActive = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mActive;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mAnimOffset = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mAnimOffset;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mScrollPosY = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mScrollPosY;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mScrollRatio = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mScrollRatio;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mThumbHeight = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mThumbHeight;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mContentHeight = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mContentHeight;
    reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(mOption)->mScrollBar)->mDirection = reinterpret_cast<CScrollBar*>(reinterpret_cast<COption*>(tempOption)->mScrollBar)->mDirection;

    // CBaseCur sub-cursors @ +0x78 / +0x90.
    MenuShopSellCopyCursor(&reinterpret_cast<COption*>(mOption)->mSubCur1, &reinterpret_cast<COption*>(tempOption)->mSubCur1);
    MenuShopSellCopyCursor(&reinterpret_cast<COption*>(mOption)->mSubCur2, &reinterpret_cast<COption*>(tempOption)->mSubCur2);

    // CSysWin @ +0xA8 (mem region at +4); remaining fields still option-relative.
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(reinterpret_cast<u8*>(&reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->mMemRegion), reinterpret_cast<u8*>(&reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mMemRegion));
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->mFileHandle = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mFileHandle;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->mTagProcessor = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mTagProcessor;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->mArcAccessor = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mArcAccessor;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->mLayout = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mLayout;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->mAnimTrans = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->mAnimTrans;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_28 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_28;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_2C = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_2C;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_30 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_30;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_34 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_34;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_35 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_35;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_36 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_36;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_37 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_37;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_38 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_38;
    reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(mOption)->mSysWin)->field_39 = reinterpret_cast<CSysWin*>(reinterpret_cast<COption*>(tempOption)->mSysWin)->field_39;

    MenuShopSellCopyCursor(&reinterpret_cast<COption*>(mOption)->mSubCur3, &reinterpret_cast<COption*>(tempOption)->mSubCur3);
    reinterpret_cast<COption*>(mOption)->field_0xFC = reinterpret_cast<COption*>(tempOption)->field_0xFC;
    reinterpret_cast<COption*>(mOption)->field_0x100 = reinterpret_cast<COption*>(tempOption)->field_0x100;
    __dt__7COptionFv(reinterpret_cast<COption*>(tempOption), -1);

    COptionRebuildWidgets(reinterpret_cast<COption*>(mOption));

    // Register the widget as an IScnRender render callback on its parent scene.
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 0);
}

void CMenuOption::Term() {
    CDeviceVI::waitForDrawDone();

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    COptionTeardown(reinterpret_cast<COption*>(mOption));

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
    COptionTickState(reinterpret_cast<COption*>(mOption));
}

void CMenuOption::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    COptionDraw(reinterpret_cast<COption*>(mOption), &drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

/* Singleton factory (retail unmangled symbol, called from CUICfManager). Returns
 * 0 if an instance is already live; otherwise constructs the screen in work
 * memory, stores it in the shared flag and registers it under `registParent`. */
CMenuOption* createOptionMenu(CProcess* registParent, CProcess* parent, u32 arg) {
    if (lbl_eu_80664A38[0] != 0) {
        return NULL;
    }
    CMenuOption* obj = new (CWorkThreadSystem::getWorkMem()) CMenuOption(parent, arg);
    lbl_eu_80664A38[0] = (u32)obj;
    obj->Regist(registParent, false);
    return (CMenuOption*)lbl_eu_80664A38[0];
}

/* Advance the option menu to phase 1 once the background texture, title bar
 * and option panel are all ready, then start the panel intro animations and
 * play the confirm sound effect (writes the state byte at 0x1BC). */
void advanceOptionPhase1(CMenuOption* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 && isInitialized(&self->mTitleAHelp) != 0 &&
        COptionIsWindowReady(reinterpret_cast<COption*>(self->mOption)) != 0) {
        func_801C412C(&self->mTitleAHelp);
        COptionBeginScrollSetup(reinterpret_cast<COption*>(self->mOption));
        self->mState = 1;
        playUISound(0x6d);
    }
}

/* Advance the option menu to phase 2 once the title bar is idle and the
 * option panel has finished its intro (writes the state byte at 0x1BC). */
void advanceOptionPhase2(CMenuOption* self) {
    if (self->mTitleAHelp.isIdle() != 0 && reinterpret_cast<COption*>(self->mOption)->getLiveFlag() != 0) {
        self->mState = 2;
    }
}

/* Option-menu input handler (retail handleOptionInput). Reads the current pad
 * state and dispatches to the embedded COption's directional/confirm handlers
 * depending on which control the player pressed this frame, then repaints the
 * title/help bar with the option's current index and advances the phase state
 * machine (mState / mField54). */
void handleOptionInput(CMenuOption* self) {
    if (COptionIsWindowReady(reinterpret_cast<COption*>(self->mOption)) == 0) {
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
        COptionHandleLeft(reinterpret_cast<COption*>(self->mOption));
    } else if (right) {
        COptionHandleRightAccept(reinterpret_cast<COption*>(self->mOption), 0);
    } else if (up) {
        COptionCursorUp(reinterpret_cast<COption*>(self->mOption));
    } else if (down) {
        COptionCursorDown(reinterpret_cast<COption*>(self->mOption));
    } else if (pageUp) {
        COptionPageUp(reinterpret_cast<COption*>(self->mOption));
    } else if (pageDown) {
        COptionPageDown(reinterpret_cast<COption*>(self->mOption));
    } else if (pageLeft) {
        COptionOpenConfigDialog(reinterpret_cast<COption*>(self->mOption));
    } else if (pageRight) {
        COptionHandleRightAccept(reinterpret_cast<COption*>(self->mOption), 1);
    }

    func_801C41E8(&self->mTitleAHelp, (u8)COptionGetNavCode(reinterpret_cast<COption*>(self->mOption)));

    if (reinterpret_cast<COption*>(self->mOption)->getConfirmGate() != 0) {
        if (reinterpret_cast<COption*>(self->mOption)->getSecondConfirm() != 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
            }
            self->mState = 4;
            self->mField54 = 1;
        } else {
            beginClose(&self->mTitleAHelp);
            COptionConfirmSelection(reinterpret_cast<COption*>(self->mOption));
            self->mState = 3;
        }
    }
}

/* Same idle+advance check as advanceOptionPhase2, but advances the option menu to
 * phase 1 (writes the state byte at offset 0x54). */
void flagOptionPhase1(CMenuOption* self) {
    if (self->mTitleAHelp.isIdle() != 0 && reinterpret_cast<COption*>(self->mOption)->getLiveFlag() != 0) {
        self->mField54 = 1;
    }
}

unsigned long hasOptionMenu(void) { return lbl_eu_80664A38[0] != 0; }

// Retail tables owned by this unit.
// .rodata (0x20): the "CMenuOption" RTTI name head and the title pool
// (lbl_eu_805103C4, used above at +0x144/+0x14D/+0x150). The tutorial string
// pool that used to be defined here belongs to kyoshin/CTutorial (see the
// splits: CTutorial owns .rodata 0x80510280-0x805103B8).
// .sbss (0x8): singleton pointer + emitted zero word (retail section is 0x8;
// no code reads the second word).
extern __declspec(section ".rodata") char lbl_eu_805103C4[0x14] =
    "MNU_main\0name\0\0\0\0\0\0";
u32 lbl_eu_80664A38[2] = { 0, 0 };

// FULL_MATCH: CMenuGCItem_RenderThunk58, CMenuGCItem_DtorThunk58

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuGCItem.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80663E28; // render-gate mode bitfield (.sbss; bit 21 = busy)

// C-linkage callees (retail unmangled names for the func_ helpers).
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);

extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);

// DrawInfo helper - retail emits the mangled nw4r form
// (SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo), so it is a normal C++ global.

// Phase-2 helper defined below (phase-2 input dispatch; advances mState to 3).

CMenuGCItem* CMenuGCItem::sInstance;

/*
 * Constructor: the retail single function that lazily allocates the GC-item
 * instance (create below), then builds it: CTTask/IUICf bases, the parent
 * pointer at 0x5C, the BgTex and title/help members, the item-box grid, then
 * the state bytes.
 */
inline CMenuGCItem::CMenuGCItem(CProcess* parent, u32 arg)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(nullptr, 0), mItemBoxGrid(0, 0, 0, 1) {
    mState = 0;
    mArg = arg;
}

/*
 * Factory: refuse if an instance already exists, otherwise construct it in the
 * work memory, record it as the singleton, register it under `registParent`
 * and return the stored instance.
 */
CMenuGCItem* CMenuGCItem::create(CProcess* registParent, CProcess* parent, u32 arg) {
    if (sInstance != nullptr) {
        return nullptr;
    }
    CMenuGCItem* obj = new (CWorkThreadSystem::getWorkMem()) CMenuGCItem(parent, arg);
    sInstance = obj;
    obj->Regist(registParent, false);
    return sInstance;
}

/* Destructor: members and bases are destroyed by the compiler. */
CMenuGCItem::~CMenuGCItem() {}

void CMenuGCItem::Init() {}

/* Tear the GC-item screen down: wait for the draw callback to finish, remove
 * the render callback from the parent scene, release each embedded widget and
 * clear the singleton instance. */
void CMenuGCItem::Term() {
    CDeviceVI::waitForDrawDone();

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    mItemBoxGrid.UnloadItemBox();

    sInstance = nullptr;
}

/* Per-frame update of the GC-item screen. Gate on the task/busy flags, then
 * drive the 4-state FSM and refresh the bg / help bar / item grid each frame. */
void CMenuGCItem::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        // Once the bg texture, title bar and item grid are all ready, start
        // the panel intro animations and play the open cue (sound 0x6d).
        if (BgTex_IsLoaded_3E34(&mBgTex) != 0 && mTitleAHelp.isInitialized() != 0 &&
            mItemBoxGrid.IsItemBoxReady() != 0) {
            func_801C412C(&mTitleAHelp);
            mItemBoxGrid.InitItemDisplay();
            mState = 1;
            playUISound(0x6d);
        }
        break;
    case 1:
        // Advance to phase 2 once the title bar is idle and the grid is ready.
        if (mTitleAHelp.isIdle() != 0 && mItemBoxGrid.GetField61() != 0) {
            mState = 2;
        }
        break;
    case 2:
        this->HandlePhase2();
        break;
    case 3:
        // Same idle+ready check, but sets the phase flag at 0x54 instead.
        if (mTitleAHelp.isIdle() != 0 && mItemBoxGrid.GetField61() != 0) {
            mField54 = 1;
        }
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    mItemBoxGrid.UpdateItemBox();
}

/* Render the GC-item screen through a stack DrawInfo: gate on the task/busy
 * flags and the scene-active check, then draw bg, title/help bar and item
 * grid in that order. */
void CMenuGCItem::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
    mItemBoxGrid.DrawItemBoxGrid(&drawInfo);
}

extern "C" int CMenuGCItem_IsActive() {
    return CMenuGCItem::sInstance != nullptr;
}

void CMenuGCItem::HandlePhase2() {}

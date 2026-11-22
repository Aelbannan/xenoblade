#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/menu/CMenuBattleMode.hpp"
#include "kyoshin/cf/CfCamSymbols.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
// Conflicting free-function redeclarations between CAIAction.hpp (via
// CBattleManager.hpp -> CfObjectActor.hpp), plugin/ocBdat.hpp and
// chain/CChainActorList.hpp trip MWCC 10197 when they land in one TU.
// Pre-include the winning copies, then rename CAIAction's stray copies out
// of the way for the duration of the include (all three are idempotent).
#include "kyoshin/plugin/ocBdat.hpp"
// IdTable_SumValues: u32 form in CChainActorList.hpp wins; pre-include it, then
// rename CChainTimer's int-form copy out of the way across BOTH spans
// (CChain.hpp pulls CChainActorList then CChainTimer; UnkClass_800D8DBC.hpp
// re-includes CChainTimer inside the CBattleManager span).
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
// Pre-include CAIAction.hpp under the renames so its u16 CtrlObjectParam_GetCharEntry form
// wins; CVision's u32 copy is renamed away inside the CBattleManager span.
#include "kyoshin/cf/object/CAIAction.hpp"
#define CtrlObjectParam_GetCharEntry func_8009EC9C_cvision
#include "kyoshin/cf/CBattleManager.hpp"
// CtrlObjectParam_GetCharEntry stays defined until after the code_80135FDC.hpp include below.
#include "kyoshin/cf/CfGameManager.hpp"
// (findBattleStatusEntry: CAIAction.hpp / CtrlEnemy.hpp declare the identical
// void*(void*, u32) form - no guard needed.)
#include "kyoshin/cf/code_800F42AC.hpp"

// Imports from code_800F42AC.cpp (TU-local, as in the other callers).

extern "C" void* MapFx_ResolveOwner(void* src);

// CfObjectMove.hpp (via the CBattleManager.hpp include above) declares it
// const float. This TU uses neither copy.
// getReloadParam0: CAIAction.hpp's int form is already in; rename
// code_80135FDC.hpp's u16 copy out of the way (this TU never calls it).
// CtrlObjectParam_GetCharEntry stays renamed across this span too (CAIAction's u16 form
// wins; code_80135FDC.hpp carries another u32 copy).
#include "kyoshin/code_80135FDC.hpp"
#undef CtrlObjectParam_GetCharEntry
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt.h>

// Imports used by this unit (data/globals referenced at their retail names).
// Secondary-mode flags bitfield (bit 21 = 0x200000 gated in Move/cbRenderBefore).
extern u32 gmPauseFlags;
// sdata2 animation frame constants.
extern const f32 lbl_eu_80667C80;
extern const f32 lbl_eu_80667C84;
// Shared battle ARC resource accessor -- retail calls the unmangled name.

// TU-local mirror of reslist::size(); declaration order walks the counter
// between cursor and sentinel so MWCC colors cursor->r3, counter->r4,
// sentinel->r5 like retail (MWCC_CASES "reslist::size() regalloc").
static inline u32 ActorList1Count(reslist<cf::CfObjectActor*>* list) {
    // Holding the list pointer live across the sentinel load keeps MWCC from
    // reusing the call-result register for the sentinel, matching retail.
    u32 length = 0;
    _reslist_node<cf::CfObjectActor*>* curNode =
        list->mStartNodePtr->mNext;
    _reslist_node<cf::CfObjectActor*>* endNode = list->mStartNodePtr;
    while (curNode != endNode) {
        curNode = curNode->mNext;
        length++;
    }
    return length;
}

// CLEANUP: renamed all unk fields to descriptive names (see header).

CMenuBattleMode::~CMenuBattleMode() {}

/**
 * Initialises the battle mode layout.
 *
 * Allocates a 0xd00-byte MEM2 region via mLayoutMem, then builds the primary
 * layout (mLayout) and two animation layers (mAnimDefault, mAnimLabel) from the
 * shared battle ARC resource at string offsets 0x10, 0x29, 0x45 respectively.
 * mAnimLabel is disabled at init; mAnimDefault is enabled and its frame set to
 * the initial value. Registers this object as an IScnRender render callback at
 * layer 7 on the owning scene.
 */
void CMenuBattleMode::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mLayoutMem.createRegion(handle, 0xd00, "CMenuBattleMode", 0);
    // Scoped region guard - destructor releases the region when Init finishes
    Class_8045F858 regionGuard(&mLayoutMem);

    nw4r::lyt::ArcResourceAccessor* accessor = CUICfManager_getArcResourceAccessor();
    buildLayout(&mLayout, accessor, "mf71_battle40_mark.brlyt");

    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(mLayout, &mAnimDefault, accessor, "mf71_battle40_mark_in.brlan");

    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(mLayout, &mAnimLabel, accessor, "mf71_battle40_mark_roop.brlan");

    mLayout->SetAnimationEnable(mAnimLabel, false);
    mLayout->SetAnimationEnable(mAnimDefault, true);
    mAnimDefault->SetFrame(lbl_eu_80667C80);
    mLayout->Animate(0);

    IScnRender* cb = this;
    mScn->addRenderCB(cb, 7, 0);
    mLayoutMem.validateHeap();
}

void CMenuBattleMode::Term() {
    CDeviceVI::waitForDrawDone();

    mScn->removeRenderCB(this);

    if (mLayout) {
        delete mLayout;
        mLayout = 0;
    }

    mLayoutMem.deleteRegion();
    sInstance = nullptr;
}

void CMenuBattleMode::Move() {
    CTaskGame::getInstance();
    // Gate: skip when the task is busy or the global mode bit (0x200000) is
    // set. Exit label BEFORE the body label keeps the body off the fallthrough
    // so MWCC emits retail's branch-over-branch: `bne end` for the first
    // disjunct, `beq run; b end` for the second (CMenuGameClear idiom,
    // MWCC_CASES section on &&-gate branch-over-branch).
    if (CTaskGame::isFlag01Set() == 0 &&
        (gmPauseFlags & 0x200000) == 0) {
        goto run;
    }
    goto end;
end:
    return;
run:
    if (IsMenuState621F0() == 0) {
        return;
    }
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0xafa40000)) {
        return;
    }

    switch (mState) {
    case 0: {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        if (player == 0) {
            break;
        }
        MapFx_ResolveOwner(player);
        if (ActorList1Count(
                &cf::CBattleManager::getInstance()->mActorList1) == 0) {
            break;
        }
        mLayout->SetAnimationEnable(mAnimLabel, false);
        mLayout->SetAnimationEnable(mAnimDefault, true);
        mState = 1;
        break;
    }

    case 1:
        if (advanceAnimTransform(mAnimDefault, lbl_eu_80667C84)) {
            mLayout->SetAnimationEnable(mAnimDefault, false);
            mLayout->SetAnimationEnable(mAnimLabel, true);
            mAnimLabel->SetFrame(lbl_eu_80667C80);
            mState = 2;
        }
        break;

    case 2: {
        advanceAnimTransform(mAnimLabel, lbl_eu_80667C84);
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        if (player == 0) {
            break;
        }
        MapFx_ResolveOwner(player);
        if (ActorList1Count(
                &cf::CBattleManager::getInstance()->mActorList1) != 0) {
            break;
        }
        mLayout->SetAnimationEnable(mAnimLabel, false);
        mLayout->SetAnimationEnable(mAnimDefault, true);
        mState = 3;
        break;
    }

    case 3:
        if (AnimRewindFrame(mAnimDefault, lbl_eu_80667C84)) {
            mState = 0;
        }
        break;

    default:
        break;
    }

    if (mState != 0) {
        mLayout->Animate(0);
    }
}

void CMenuBattleMode::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    // Gate: skip when the task is busy or the global mode bit (0x200000) is
    // set. Exit label BEFORE the body label keeps the body off the fallthrough
    // so MWCC emits retail's branch-over-branch: `bne end` for the first
    // disjunct, `beq draw; b end` for the second (CMenuGameClear idiom,
    // MWCC_CASES section on &&-gate branch-over-branch).
    if (CTaskGame::isFlag01Set() == 0 &&
        (gmPauseFlags & 0x200000) == 0) {
        goto draw;
    }
    goto end;
end:
    return;
draw:
    if (IsMenuState621F0() == 0) {
        return;
    }
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0xafa40000)) {
        return;
    }
    if (mState == 0) {
        return;
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);  // disable Z compare, always pass
    {
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        drawLayout(mLayout, &drawInfo, 0, 1);
    }
}

CMenuBattleMode* CMenuBattleMode::sInstance;

inline CMenuBattleMode::CMenuBattleMode(CScn* scene)
    : mScn(scene), mLayout(nullptr), mAnimDefault(nullptr), mAnimLabel(nullptr), mState(0) {}

CMenuBattleMode* CMenuBattleMode::create(CProcess* parent, CScn* scene) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuBattleMode* menu = new (CWorkThreadSystem::getWorkMem()) CMenuBattleMode(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

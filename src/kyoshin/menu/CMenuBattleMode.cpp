#include "kyoshin/menu/CMenuBattleMode.hpp"

#include "kyoshin/CTaskGame.hpp"
// Conflicting free-function redeclarations between CAIAction.hpp (via
// CBattleManager.hpp -> CfObjectActor.hpp), plugin/ocBdat.hpp and
// chain/CChainActorList.hpp trip MWCC 10197 when they land in one TU.
// Pre-include the winning copies, then rename CAIAction's stray copies out
// of the way for the duration of the include (all three are idempotent).
#include "kyoshin/plugin/ocBdat.hpp"
// func_8025FB10: u32 form in CChainActorList.hpp wins; pre-include it, then
// rename CChainTimer's int-form copy out of the way across BOTH spans
// (CChain.hpp pulls CChainActorList then CChainTimer; UnkClass_800D8DBC.hpp
// re-includes CChainTimer inside the CBattleManager span).
#include "kyoshin/cf/chain/CChainActorList.hpp"
#define func_8025FB10 func_8025FB10_chainTimer
#include "kyoshin/cf/chain/CChain.hpp"
#define getBdatStringColumnValue getBdatStringColumnValue_caiaction2
#define getArtsSlotRC getArtsSlotRC_caiaction
#define getArtsParamRC2 getArtsParamRC2_caiaction
// Pre-include CAIAction.hpp under the renames so its u16 func_8009EC9C form
// wins; CVision's u32 copy is renamed away inside the CBattleManager span.
#include "kyoshin/cf/object/CAIAction.hpp"
#define func_8009EC9C func_8009EC9C_cvision
#include "kyoshin/cf/CBattleManager.hpp"
#undef getBdatStringColumnValue
#undef getArtsSlotRC
#undef getArtsParamRC2
#undef func_8025FB10
// func_8009EC9C stays defined until after the code_80135FDC.hpp include below.
#include "kyoshin/cf/CfGameManager.hpp"
// (func_80149154: code_800F42AC.hpp / CAIAction.hpp / CtrlEnemy.hpp now all
// declare the identical void*(void*, u32) form - no guard needed.)
#include "kyoshin/cf/code_800F42AC.hpp"
// code_80135FDC.hpp declares lbl_eu_8066A208 as u32 (line 188);
// CfObjectMove.hpp (via the CBattleManager.hpp include above) declares it
// const float. This TU uses neither copy.
// func_8016DF2C: CAIAction.hpp's int form is already in; rename
// code_80135FDC.hpp's u16 copy out of the way (this TU never calls it).
// func_8009EC9C stays renamed across this span too (CAIAction's u16 form
// wins; code_80135FDC.hpp carries another u32 copy).
#define func_8016DF2C func_8016DF2C_u16
#include "kyoshin/code_80135FDC.hpp"
#undef func_8016DF2C
#undef func_8009EC9C
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt.h>

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
    mLayoutMem.createRegion(handle, 0xd00, lbl_eu_80503D80, 0);
    // Scoped region guard - destructor releases the region when Init finishes
    Class_8045F858 regionGuard(&mLayoutMem);

    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
    buildLayout(&mLayout, accessor, lbl_eu_80503D80 + 0x10);

    accessor = func_801355F4();
    bindLayoutAnimTransform(mLayout, &mAnimDefault, accessor, lbl_eu_80503D80 + 0x29);

    accessor = func_801355F4();
    bindLayoutAnimTransform(mLayout, &mAnimLabel, accessor, lbl_eu_80503D80 + 0x45);

    mLayout->SetAnimationEnable(mAnimLabel, false);
    mLayout->SetAnimationEnable(mAnimDefault, true);
    mAnimDefault->SetFrame(lbl_eu_80667C80);
    mLayout->Animate(0);

    IScnRender* cb = this;
    mScn->addRenderCB(cb, 7, 0);
    mLayoutMem.func_8045F810();
}

// C-linkage declarations so MWCC emits the plain retail symbol names (the
// thunks jump through these unmangled addresses; C++ linkage would re-mangle).
extern "C" void cbRenderBefore__15CMenuBattleModeFv();
void func_801A0494(void* self) { ((void(*)(void*))cbRenderBefore__15CMenuBattleModeFv)((char*)self - 0x5c); }
extern "C" CMenuBattleMode* __dt__15CMenuBattleModeFv(CMenuBattleMode* self, int deleteFlag);
void func_801A049C(void* self) { ((void(*)(void*))__dt__15CMenuBattleModeFv)((char*)self - 0x5c); }

// Deleting virtual destructor (D1 shape): destroys the mLayoutMem member
// (delete flag -1), then the CProcess base (via IUICf/CTTask), then frees the
// backing block when the delete flag is nonzero. Written as a plain free C-ABI
// shim on the mangled symbol so MWCC does not re-install the class vtables at
// the top of the dtor (retail emits no vptr stores here). The redundant
// `if (self != 0)` re-check reproduces the retail's dead double-beq and the
// explicit __dt__8CProcessFv(self, 0) call fixes the delete flag to 0
// (CMenuSymbolMark/CMenuBattleChain idiom).
extern "C" void __dt__8CProcessFv(CProcess* self, int flags);
extern "C" void __dl__FPv(void* p);
extern "C" CMenuBattleMode* __dt__15CMenuBattleModeFv(CMenuBattleMode* self, int deleteFlag) {
    if (self == 0) goto end;
    self->mLayoutMem.~UnkClass_8045F564();
    if (self != 0) {
        if (self != 0) {
            __dt__8CProcessFv(self, 0);
        }
    }
    if (deleteFlag > 0) __dl__FPv(self);
end:
    return self;
}

/**
 * Factory-style constructor for the single battle-mode menu instance.
 *
 * If no instance exists yet (global is null), allocates a 0x84-byte block from
 * work memory, constructs the CProcess base + UnkClass_8045F564 member on top
 * of it, installs the three vtables (primary / IWorkEvent / IScnRender), then
 * stores the pointer in the singleton global and registers off the parent.
 */

// Byte-range shim over the retconned object so the factory ctor can write the
// vtables (+0x10, +0x58, +0x5C) and the PTMF callback slots (+0x3C..0x53)
// without raw pointer arithmetic. Mirrors CMenuBattleMode layout exactly.
struct CMenuBattleModeShim {
    u8 unk00[0x10];
    void* vtable;                    // 0x10
    u8 unk14[0x3C - 0x14];          // 0x14-0x3B (CProcess tail)
    u32 callbacks[6];               // 0x3C-0x53 (__ptmf_null copies)
    u8 field54;                     // 0x54
    u8 field55;                     // 0x55
    void* iweVtable;                // 0x58 (IWorkEvent)
    void* iscnVtable;               // 0x5C (IScnRender)
    UnkClass_8045F564 mLayoutMem;    // 0x60-0x6F
    CScn* mScn;                     // 0x70
    nw4r::lyt::Layout* mLayout;      // 0x74
    nw4r::lyt::AnimTransform* mAnimDefault; // 0x78
    nw4r::lyt::AnimTransform* mAnimLabel;   // 0x7C
    u8 mState;                      // 0x80
    u8 pad81[3];                    // 0x81-0x83
};
extern "C" CMenuBattleMode* __ct__CMenuBattleMode(CProcess* parent, CScn* scene) {
    if (lbl_eu_80664318 != 0) {
        return NULL;
    }

    mtl::ALLOC_HANDLE handle = CWorkThreadSystem::getWorkMem();
    CMenuBattleModeShim* shim =
        (CMenuBattleModeShim*)mtl::MemManager::allocate(0x84, handle);

    if (shim != NULL) {
        u32 zero = 0;
        CProcess* proc = (CProcess*)shim;
        __ct__8CProcessFv(proc);

        // Temp (CProcess) vtable first.
        shim->vtable = lbl_eu_8052BF70;

        // Word-exact copy of the two null-PMF slot groups (0x3C..0x53).
        u32* ptmf = __ptmf_null;
        u32 w1 = ptmf[1];
        u32 w0 = ptmf[0];
        shim->callbacks[0] = w0;
        shim->callbacks[1] = w1;
        shim->callbacks[2] = ptmf[2];

        w1 = ptmf[1];
        w0 = ptmf[0];
        shim->callbacks[3] = w0;
        shim->callbacks[4] = w1;
        shim->callbacks[5] = ptmf[2];

        shim->field54 = zero;
        shim->field55 = zero;

        // Composite vtable + IWorkEvent (+0x24) / IScnRender (+0xac)
        // sub-vtables, formed from one materialization of the final label.
        shim->vtable = lbl_eu_80532DD0;
        shim->iweVtable = lbl_eu_80532DD0 + 0x24;
        shim->iscnVtable = lbl_eu_80532DD0 + 0xac;

        ::new (&shim->mLayoutMem) UnkClass_8045F564();
        shim->mScn = scene;
        shim->mLayout = (nw4r::lyt::Layout*)zero;
        shim->mAnimDefault = (nw4r::lyt::AnimTransform*)zero;
        shim->mAnimLabel = (nw4r::lyt::AnimTransform*)zero;
        shim->mState = zero;
    }

    lbl_eu_80664318 = (u32)shim;
    ((CProcess*)shim)->Regist(parent, false);
    return (CMenuBattleMode*)lbl_eu_80664318;
}

void CMenuBattleMode::Term() {
    CDeviceVI::waitForDrawDone();

    mScn->removeRenderCB(this);

    if (mLayout) {
        delete mLayout;
        mLayout = 0;
    }

    mLayoutMem.func_8045F778();
    lbl_eu_80664318 = 0;
}

void CMenuBattleMode::Move() {
    CTaskGame::getInstance();
    // Gate: skip when the task is busy or the global mode bit (0x200000) is
    // set. Exit label BEFORE the body label keeps the body off the fallthrough
    // so MWCC emits retail's branch-over-branch: `bne end` for the first
    // disjunct, `beq run; b end` for the second (CMenuGameClear idiom,
    // MWCC_CASES section on &&-gate branch-over-branch).
    if (CTaskGame::isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto run;
    }
    goto end;
end:
    return;
run:
    if (func_8013BE50() == 0) {
        return;
    }
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(0xafa40000)) {
        return;
    }

    switch (mState) {
    case 0: {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        if (player == 0) {
            break;
        }
        func_8016FE34(player);
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
        func_8016FE34(player);
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
        if (func_80137510(mAnimDefault, lbl_eu_80667C84)) {
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

void CMenuBattleMode::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: skip when the task is busy or the global mode bit (0x200000) is
    // set. Exit label BEFORE the body label keeps the body off the fallthrough
    // so MWCC emits retail's branch-over-branch: `bne end` for the first
    // disjunct, `beq draw; b end` for the second (CMenuGameClear idiom,
    // MWCC_CASES section on &&-gate branch-over-branch).
    if (CTaskGame::isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto draw;
    }
    goto end;
end:
    return;
draw:
    if (func_8013BE50() == 0) {
        return;
    }
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(0xafa40000)) {
        return;
    }
    if (mState == 0) {
        return;
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);  // disable Z compare, always pass
    {
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        drawLayout(mLayout, &drawInfo, 0, 1);
    }
}
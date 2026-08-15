#include "kyoshin/menu/CMenuBattleMode.hpp"

// CTaskGame.hpp declares func_8004392C with a u32 third arg (line 486) and
// func_8049603C with a CScn* arg (line 559), which conflict with the void*
// copies in CVision.hpp / CSuddenCommu.hpp (reached via the
// CBattleManager.hpp include below); rename the CTaskGame.hpp copies out of
// the way (same scheme as CMenuBattlePlayerState.cpp). This TU uses none of
// them.
#define func_8004392C menuBmbCtaskGame4392CUnused
#define func_8049603C menuBmbCtaskGame9603CUnused
#include "kyoshin/CTaskGame.hpp"
#undef func_8049603C
#undef func_8004392C
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
// code_800F42AC.hpp declares func_80149154 with an int id arg; CAIAction.hpp
// (via CBattleManager.hpp -> CfObjectActor.hpp) declares the same name with a
// u32 arg -> MWCC overload conflict. This TU uses neither copy.
#define func_80149154 menuBmbCode800F42AC49154Unused
#include "kyoshin/cf/code_800F42AC.hpp"
#undef func_80149154
// code_80135FDC.hpp declares lbl_eu_8066A208 as u32 (line 188),
// func_8049603C with a CScn* arg (line 223), and getBdatStringColumnValue
// as void* (line 238); CfObjectMove.hpp (via the CBattleManager.hpp include
// above) declares lbl_eu_8066A208 const float, CSuddenCommu.hpp declares
// func_8049603C with a void* arg, and CfGimmick.hpp declares
// getBdatStringColumnValue as u32. This TU uses neither copy.
#define lbl_eu_8066A208 menuBmbCode35FDCepsilonUnused
#define func_8049603C menuBmbCode35FDC9603CUnused
#define getBdatStringColumnValue menuBmbCode35FDCBdatColUnused
#include "kyoshin/code_80135FDC.hpp"
#undef getBdatStringColumnValue
#undef func_8049603C
#undef lbl_eu_8066A208
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt.h>

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
    func_80136E84(&mLayout, accessor, lbl_eu_80503D80 + 0x10);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimDefault, accessor, lbl_eu_80503D80 + 0x29);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimLabel, accessor, lbl_eu_80503D80 + 0x45);

    mLayout->SetAnimationEnable(mAnimLabel, false);
    mLayout->SetAnimationEnable(mAnimDefault, true);
    mAnimDefault->SetFrame(lbl_eu_80667C80);
    mLayout->Animate(0);

    IScnRender* cb = this;
    mScn->addRenderCB(cb, 7, 0);
    mLayoutMem.func_8045F810();
}

void CMenuBattleMode::func_801A048C() {
    extern CMenuBattleMode* __dt__15CMenuBattleModeFv(CMenuBattleMode*, int);
    __dt__15CMenuBattleModeFv(this, 1);
}
extern void cbRenderBefore__15CMenuBattleModeFv();
void func_801A0494(void* self) { ((void(*)(void*))cbRenderBefore__15CMenuBattleModeFv)((char*)self - 0x5c); }
extern CMenuBattleMode* __dt__15CMenuBattleModeFv(void*, int);
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
        return 0;
    }

    u32 handle = CWorkThreadSystem::getWorkMem();
    CMenuBattleModeShim* shim =
        (CMenuBattleModeShim*)mtl::MemManager::allocate(0x84, handle);

    if (shim != 0) {
        __ct__8CProcessFv((CProcess*)shim);
        shim->vtable = lbl_eu_8052BF70;   // CProcess base vtable first

        u32* ptmf = __ptmf_null;
        char* vtFinal = lbl_eu_80532DD0;  // CMenuBattleMode primary vtable

        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        char* iweVtbl = vtFinal + 0x24;   // IWorkEvent secondary vtable

        shim->callbacks[0] = ptmf0;
        shim->callbacks[1] = ptmf1;

        u32 ptmf2 = ptmf[2];
        shim->callbacks[2] = ptmf2;

        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        shim->callbacks[3] = ptmf0;
        shim->callbacks[4] = ptmf1;
        ptmf2 = ptmf[2];
        shim->callbacks[5] = ptmf2;

        shim->field54 = 0;
        shim->field55 = 0;

        shim->vtable = vtFinal;
        shim->iweVtable = iweVtbl;
        shim->iscnVtable = vtFinal + 0xac;  // IScnRender secondary vtable

        ::new (&shim->mLayoutMem) UnkClass_8045F564();
        shim->mScn = scene;
        shim->mLayout = 0;
        shim->mAnimDefault = 0;
        shim->mAnimLabel = 0;
        shim->mState = 0;
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
    if (CTaskGame::func_800426F0()) {
        return;
    }
    if (lbl_eu_80663E28 & 0x200000) {  // bit 21: realtime event busy
        return;
    }
    if (!func_8013BE50()) {
        return;
    }
    cf::CfGameManager::getInstance();
    if (func_8006EF04__Fi(0xafa40000)) {
        return;
    }

    switch (mState) {
    case 0: {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        if (player != 0) {
            func_8016FE34(player);
            if (cf::CBattleManager::getInstance()->mActorList1.size() != 0) {
                mLayout->SetAnimationEnable(mAnimLabel, false);
                mLayout->SetAnimationEnable(mAnimDefault, true);
                mState = 1;
            }
        }
        break;
    }

    case 1:
        if (func_80137444(mAnimDefault, lbl_eu_80667C84)) {
            mLayout->SetAnimationEnable(mAnimLabel, false);
            mLayout->SetAnimationEnable(mAnimDefault, true);
            mAnimLabel->SetFrame(lbl_eu_80667C80);
            mState = 2;
        }
        break;

    case 2: {
        func_80137444(mAnimLabel, lbl_eu_80667C84);
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        if (player != 0) {
            func_8016FE34(player);
            if (cf::CBattleManager::getInstance()->mActorList1.size() == 0) {
                mLayout->SetAnimationEnable(mAnimLabel, false);
                mLayout->SetAnimationEnable(mAnimDefault, true);
                mState = 3;
            }
        }
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
    // MWCC_REFERENCE section on &&-gate branch-over-branch).
    if (CTaskGame::func_800426F0() == 0 &&
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
    if (func_8006EF04__Fi(0xafa40000)) {
        return;
    }
    if (mState == 0) {
        return;
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);  // disable Z compare, always pass
    {
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_80137038(mLayout, &drawInfo, 0, 1);
    }
}
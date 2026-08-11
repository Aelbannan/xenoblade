// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGameClear
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuGameClear.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>

// forward declarations for scaffold thunk references
void cbRenderBefore__14CMenuGameClearFv(void*);
void __dt__14CMenuGameClearFv(void*);

/*
 * Singleton factory constructor (retail unmangled symbol). Allocates the
 * 0x12E60-byte game-clear screen, builds it (CProcess ctor, temp vtable,
 * null PMF triple copy, composite vtable + IScnRender sub-vtable at +0x58,
 * then the embedded widgets and the CtrlObjectParam entry array), registers
 * it under `registParent` and returns the stored instance (0 if one exists).
 *
 * r3 = registration parent (Regist), r4 = owning scene (stored at 0x5C).
 */
extern "C" CMenuGameClear* __ct__CMenuGameClear(CProcess* registParent, CScn* scene) {
    if (lbl_eu_80664C08 != 0) {
        return 0;
    }

    CMenuGameClear* obj = (CMenuGameClear*)mtl::MemManager::allocate(
        sizeof(CMenuGameClear), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);

        // Temp (CProcess) vtable first, then the null PMF triple copy, then
        // the composite vtable and the IScnRender sub-vtable at +0x58.
        obj->mVtable10 = (u32)lbl_eu_8052BF70;
        u32* src = __ptmf_null;
        u32 pmf0_1 = src[1];
        u32 pmf0_0 = src[0];
        obj->mPtMf3C[0] = pmf0_0;
        obj->mPtMf3C[1] = pmf0_1;
        u32 pmf0_2 = src[2];
        obj->mPtMf3C[2] = pmf0_2;
        u32 pmf1_1 = src[1];
        u32 pmf1_0 = src[0];
        obj->mPtMf48[0] = pmf1_0;
        obj->mPtMf48[1] = pmf1_1;
        u32 pmf1_2 = src[2];
        obj->mPtMf48[2] = pmf1_2;

        obj->mField54 = 0;
        obj->mField55 = 0;

        obj->mVtable10 = (u32)lbl_eu_8053AE20;
        obj->mIScnRenderVtbl = (u32)lbl_eu_8053AE20 + 0x24;
        obj->mScene = scene;

        __ct__17UnkClass_8045F564Fv(
            reinterpret_cast<UnkClass_8045F564*>(&obj->mMemRegion));
        obj->mField70 = 0;
        __ct__CSysWin(&obj->mSysWin, 0);
        obj->mField71 = 0;
        __ct__CCur18(&obj->mCursor, 0);

        // Initialize the 14 CtrlObjectParam entries (0xCC stride each).
        cf::CtrlObjectParamInit* p = &obj->mParams[0];
        cf::CtrlObjectParamInit* end = p + 14;
        do {
            func_8009D764(p);
            p++;
        } while (p < end);
    }

    lbl_eu_80664C08 = obj;
    Regist__8CProcessFP8CProcessb((CProcess*)obj, registParent, false);
    return lbl_eu_80664C08;
}

/*
 * Complete-object destructor. Subobjects destroyed in reverse construction
 * order: CCur18@0xB0, CSysWin@0x74, UnkClass_8045F564@0x60, then the CProcess
 * base (guarded by the nested double null-check, an MWCC artifact), then
 * conditional operator delete when the flags parameter is positive.
 */
CMenuGameClear::~CMenuGameClear() {
    __dt__6CCur18Fv(&mCursor, -1);
    __dt__7CSysWinFv(&mSysWin, -1);
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->~UnkClass_8045F564();
    if (this) {
        if (this) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

void CMenuGameClear::Init() {}

/*
 * Tear the game-clear screen down: wait for the draw callback to finish,
 * remove the render callback from the owning scene, release the CSysWin /
 * CCur18 / region widgets, clear the singleton and re-enable the cf pad
 * flags.
 */
void CMenuGameClear::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi +0x58.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = reinterpret_cast<IScnRender*>(&mIScnRenderVtbl);
    }
    mScene->removeRenderCB(render);

    func_8022B7F4(&mSysWin);
    reinterpret_cast<CCursor18*>(&mCursor)->vf3();
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->func_8045F778();

    lbl_eu_80664C08 = 0;

    cf::CfGameManager::enablePadFlags(-1, false);
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

void CMenuGameClear::Move() {}

/*
 * Render the game-clear screen through a stack DrawInfo. The empty if/else-if
 * branches reproduce the retail `bne`/`beq + b` gate shape (task busy bit,
 * then the global flag word, then the scene-active check).
 */
void CMenuGameClear::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0) {
        // task busy: skip render
    } else if (lbl_eu_80663E28 & 0x200000) {
        // global gate bit set: skip render
    } else if (func_8013BE50() != 0) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_8022B7C8(&mSysWin, &drawInfo);
        func_801D20B0(&mCursor, &drawInfo);
        drawInfo.~DrawInfo();
    }
}

extern "C" unsigned long func_802B22E0() {
    return lbl_eu_80664C08 != 0;
}

void func_802B22F4(){}

void func_802B2488(){}

void func_802B261C(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuGameClearFv)((char*)self - 0x58); }

void func_802B2624(void* self) { ((void(*)(void*))__dt__14CMenuGameClearFv)((char*)self - 0x58); }

extern "C" u32 func_802B262C() {
    return lbl_eu_80664C10;
}

// FULL_MATCH: func_802B0F08, func_802B0F10

#include "kyoshin/menu/CMenuGCItem.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>

extern "C" void cbRenderBefore__11CMenuGCItemFv(void*);

// Phase-2 helper defined below (retail keeps the bare func_ name).
extern "C" void func_802B0D24(CMenuGCItem* self);

/*
 * Retail constructor symbol (unmangled global). Combines the factory and the
 * in-place construction: lazily allocate the single GC-item instance, build
 * it (CProcess base ctor, temp vtable store, null PMF data copy, composite
 * vtable + IScnRender sub-vtable at +0x58, then each embedded widget's ctor
 * and the final state bytes), register it under `registParent`, and return
 * the stored instance (or 0 if one already exists).
 *
 * r3 = registration parent (Regist), r4 = parent (stored at 0x5C),
 * r5 = constructor arg (stored at 0x4AC5).
 */
extern "C" __declspec(noinline) CMenuGCItem* __ct__CMenuGCItem(
    CProcess* registParent, CProcess* parent, u32 arg) {
    if (lbl_eu_80664C00 != 0) {
        return 0;
    }
    CMenuGCItem* obj = (CMenuGCItem*)mtl::MemManager::allocate(
        sizeof(CMenuGCItem), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);

        // vtable fixups: temp (CProcess) vtable first, then the composite
        // vtable and the IScnRender sub-vtable at +0x58. Both null-PMF triples
        // are copied through a single pinned base pointer (retail keeps one
        // materialization of &__ptmf_null live across both copies).
        *(u32*)((u8*)obj + 0x10) = (u32)lbl_eu_8052BF70;
        register u32* src = (u32*)__ptmf_null;
        u32 t1 = src[1];
        u32 t0 = src[0];
        obj->ptmf0[0] = t0;
        obj->ptmf0[1] = t1;
        obj->ptmf0[2] = src[2];
        t0 = src[0];
        t1 = src[1];
        obj->ptmf1[0] = t0;
        obj->ptmf1[1] = t1;
        obj->ptmf1[2] = src[2];
        obj->mField54 = 0;
        obj->mField55 = 0;

        *(u32*)((u8*)obj + 0x10) = (u32)lbl_eu_8053AD08;
        *(u32*)((u8*)obj + 0x58) = (u32)lbl_eu_8053AD08 + 0x24;
        obj->mParentRef = parent;

        __ct__CBgTex(&obj->mBgTex, 0);
        __ct__CTitleAHelp(&obj->mTitleAHelp, 0, 0);
        __ct__CItemBoxGrid(&obj->mItemBoxGrid, 0, 0, 0, 1);
        obj->mState = 0;
        obj->mArg = arg;
    }
    lbl_eu_80664C00 = (unsigned long)obj;
    obj->Regist(registParent, 0x0);
    return (CMenuGCItem*)lbl_eu_80664C00;
}

/*
 * Destructor (retail D2 form, symbol keeps its C++ mangling in US). Written
 * with an explicit flags parameter so the base-class destruction can target
 * the library D1 CProcess dtor symbol __dt__8CProcessFv (0x804474D0), which
 * is what retail calls here. Subobjects are destroyed in reverse construction
 * order, then conditional operator delete when flags > 0. The nested double
 * null-check before the base call is an MWCC artifact (same shape as
 * ~CSystemWindow / ~CSysWinSave).
 */
extern "C" CMenuGCItem* __dt__11CMenuGCItemFv(CMenuGCItem* _this, int flags) {
    if (_this != 0) {
        __dt__12CItemBoxGridFv(&_this->mItemBoxGrid, -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv((CProcess*)_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuGCItem::Init() {}

/* Tear the GC-item screen down: wait for the draw callback to finish, remove
 * the render callback from the parent scene, release each embedded widget and
 * clear the singleton instance flag. */
void CMenuGCItem::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_801CAE9C(&mItemBoxGrid);

    lbl_eu_80664C00 = 0;
}

/* Per-frame update of the GC-item screen. Gate on the task/busy flags, then
 * drive the 4-state FSM and refresh the bg / help bar / item grid each frame. */
void CMenuGCItem::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        // Once the bg texture, title bar and item grid are all ready, start
        // the panel intro animations and play the open cue (sound 0x6d).
        if (func_801C3E34(&mBgTex) != 0 && func_801C4114(&mTitleAHelp) != 0 &&
            func_801CB038(&mItemBoxGrid) != 0) {
            func_801C412C(&mTitleAHelp);
            func_801CB28C(&mItemBoxGrid);
            mState = 1;
            func_80138078__FUl(0x6d);
        }
        break;
    case 1:
        // Advance to phase 2 once the title bar is idle and the grid is ready.
        if (isIdle__11CTitleAHelpFv(&mTitleAHelp) != 0 && GetField61(&mItemBoxGrid) != 0) {
            mState = 2;
        }
        break;
    case 2:
        func_802B0D24(this);
        break;
    case 3:
        // Same idle+ready check, but sets the phase flag at 0x54 instead.
        if (isIdle__11CTitleAHelpFv(&mTitleAHelp) != 0 && GetField61(&mItemBoxGrid) != 0) {
            mField54 = 1;
        }
        break;
    }

    func_801C3D54(&mBgTex);
    func_801C3FF0(&mTitleAHelp);
    func_801CABC8(&mItemBoxGrid);
}

/* Render the GC-item screen through a stack DrawInfo: gate on the task/busy
 * flags and the scene-active check, then draw bg, title/help bar and item
 * grid in that order. */
void CMenuGCItem::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // (a C++ local would virtual-dispatch its scope-exit destructor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801CAD8C(&mItemBoxGrid, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary subobject at offset 0x58 within CMenuGCItem.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuGCItemFv
 */
extern "C" void func_802B0F08(void* self) {
    ((void(*)(void*))cbRenderBefore__11CMenuGCItemFv)((char*)self - 0x58);
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuGCItem.
 *
 * Same adjustment as func_802B0F08 but forwards to the destructor.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuGCItemFv
 */
extern "C" void func_802B0F10(void* self) {
    ((void(*)(void*))__dt__11CMenuGCItemFv)((char*)self - 0x58);
}

extern "C" int func_802B0D10() {
    return lbl_eu_80664C00 != 0;
}

extern "C" void func_802B0D24(CMenuGCItem* self) {}

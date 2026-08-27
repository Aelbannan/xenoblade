// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItem
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuItem.hpp"

#include "kyoshin/CTitleAHelp.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

unsigned long func_80167A18(void) { return *(unsigned long*)(&lbl_eu_80664258) != 0; }

/* Item-menu main input handler. Syncs the help bar to the grid mode, then
 * decodes the pad: turbo/pressed masks for cancel/page-turn/decide, and
 * pressed-bit positions that depend on the game-manager controller mode
 * (classic vs. others). Dispatches one grid action in priority order. */
void func_80167A2C(CMenuItem* self) {
    // Help bar follows the grid's current mode.
    u8 mode = func_801CDFB4(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->mTitleAHelp[0]), mode);

    CfMenuPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    bool up, down;
    int turboCancel, cancel, pageTurn, decide;
    bool leftTrigger, xHeld, yHeld;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // Classic controller bit layout.
        turboCancel = (pad->mTurboFlags & 0x8004) != 0;
        pageTurn = (pad->mTurboFlags & 0x10008) != 0;
        up = (pad->mPressedFlags & 0x00200000) != 0;
        down = (pad->mPressedFlags & 0x00400000) != 0;
        cancel = (pad->mTurboFlags & 0x2001) != 0;
        decide = (pad->mTurboFlags & 0x4002) != 0;
        leftTrigger = (pad->mPressedFlags & 0x01000000) != 0;
        xHeld = (pad->mPressedFlags & 0x00000200) != 0;
        yHeld = (pad->mPressedFlags & 0x00000400) != 0;
    } else {
        turboCancel = (pad->mTurboFlags & 0x8004) != 0;
        pageTurn = (pad->mTurboFlags & 0x10008) != 0;
        up = (pad->mPressedFlags & 0x00000010) != 0;
        down = (pad->mPressedFlags & 0x00000020) != 0;
        cancel = (pad->mTurboFlags & 0x2001) != 0;
        decide = (pad->mTurboFlags & 0x4002) != 0;
        leftTrigger = (pad->mPressedFlags & 0x00000800) != 0;
        xHeld = (pad->mPressedFlags & 0x00000200) != 0;
        yHeld = (pad->mPressedFlags & 0x00000040) != 0;
    }

    if (up != 0) {
        func_801CCAF0(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    } else if (down != 0) {
        if (func_801CB0FC(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0])) != 0) {
            func_801CC7B0(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]), 0);
        } else {
            // Scroll failed: hide the help prompt, refresh the grid and move
            // to state 3.
            func_801C414C(reinterpret_cast<CTitleAHelp*>(&self->mTitleAHelp[0]));
            func_801CB38C(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
            self->mState = 3;
        }
    } else if (turboCancel != 0) {
        func_801CB5F0(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    } else if (pageTurn != 0) {
        func_801CBA04(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    } else if (cancel != 0) {
        func_801CBDE8(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    } else if (decide != 0) {
        func_801CC0EC(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    } else if (leftTrigger != 0) {
        func_801CC5DC(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    } else if (xHeld != 0) {
        func_801CDC40(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    } else if (yHeld != 0) {
        func_801CDEE8(reinterpret_cast<CItemBoxGrid*>(&self->mItemBoxGrid[0]));
    }
}

void func_80167C30(void* self) { reinterpret_cast<CMenuItem*>((char*)self - 0x58)->cbRenderBefore(); }

extern "C" void __dt__9CMenuItemFv(void*, int);
void func_80167C38(void* self) { ((void(*)(void*))__dt__9CMenuItemFv)((char*)self - 0x58); }

/*
 * Menu item-screen singleton constructor (retail symbol keeps its unmangled
 * global name). If the singleton already exists, returns NULL. Otherwise
 * allocates the 0x4AC8-byte process from the work-memory region, builds the
 * CProcess base (abstract class, so the ctor is invoked via its C-ABI name),
 * installs the temp then composite vtables, copies the null PMF triple into
 * the Move/Draw callback slots, constructs the embedded widgets, registers
 * the process under `parent`, stores the singleton pointer and returns it.
 */
extern "C" CMenuItem* __ct__CMenuItem(CProcess* parent, CScn* scene,
                                      u8 flag5, u8 flag6) {
    if (lbl_eu_80664258 != 0) {
        return 0;
    }

    u32 handle = CWorkThreadSystem::getWorkMem();
    CMenuItem* obj = static_cast<CMenuItem*>(
        mtl::MemManager::allocate(0x4AC8, handle));

    if (obj != NULL) {
        __ct__8CProcessFv(reinterpret_cast<CProcess*>(obj));

        // vtable fixups: temp (CProcess) vtable first; composite vtable and
        // its IScnRender (+0x24) sub-vtable slot are installed after the PMF
        // copy (retail store order).
        obj->mVtable10 = reinterpret_cast<u32>(lbl_eu_8052BF70);

        u32* dst = &obj->ptmfCallbacks[0];
        u32* ptmf = __ptmf_null;
        char* vtFinal = lbl_eu_805309C0;

        u32 ptmf1 = ptmf[1];
        u32 ptmf0 = ptmf[0];
        char* iscnVt = vtFinal + 0x24;
        dst[0] = ptmf0;
        dst[1] = ptmf1;
        u32 ptmf2 = ptmf[2];
        dst[2] = ptmf2;

        ptmf1 = ptmf[1];
        ptmf0 = ptmf[0];
        dst[3] = ptmf0;
        dst[4] = ptmf1;
        ptmf2 = ptmf[2];
        dst[5] = ptmf2;

        obj->mField54 = 0;
        obj->mField55 = 0;

        obj->mVtable10 = reinterpret_cast<u32>(vtFinal);
        obj->mIScnRenderVt = reinterpret_cast<u32>(iscnVt);
        obj->mScene = scene;

        __ct__CBgTex(&obj->mBgTex, 0);
        __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(&obj->mTitleAHelp[0]), 0, 0);
        __ct__CItemBoxGrid(reinterpret_cast<CItemBoxGrid*>(&obj->mItemBoxGrid[0]),
                           0, 0, 0, 1);

        obj->mState = 0;
        obj->mField4AC5 = flag5;
        obj->mField4AC6 = flag6;
    }

    lbl_eu_80664258 = reinterpret_cast<u32>(obj);
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);

    // Retail reloads the singleton after Regist.
    return reinterpret_cast<CMenuItem*>(lbl_eu_80664258);
}

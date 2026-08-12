// Auto-scaffolded catalog TU for kyoshin/menu/CMenuBattleChain
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuBattleChain.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_animation.h>

// forward declarations for scaffold thunk references
void __dt__16CMenuBattleChainFv(void*);
void cbRenderBefore__16CMenuBattleChainFv(void*);

/*
 * Battle-chain menu constructor (retail unmangled name __ct__CMenuBattleChain,
 * 3 args: this, owning scene, chain-type byte). Builds the CProcess base,
 * installs the temp then composite vtables, copies the null PMF triple into
 * the Move/Draw callback slots, initialises the layout zone (+0x54..0x68),
 * stores the composite / IWorkEvent(+0x24) / IScnRender(+0xac) vtable slots,
 * constructs the scratch region at +0x78 and zeroes/initialises the tail
 * fields, keeping the chain-type byte at +0x9D.
 */
extern "C" CMenuBattleChain* __ct__CMenuBattleChain(CMenuBattleChain* obj,
                                                     CScn* parent, u8 arg) {
    __ct__8CProcessFv((CProcess*)obj);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IWorkEvent (+0x24) / IScnRender (+0xac) sub-vtables. The pmf
    // copy reads the null member-function-pointer triple through one base
    // register; post-increment derefs of a const local pointer fold the first
    // access into `lwzu` (btm_sco_init shape). Stores are [1],[0],[2] per
    // group (retail order 0x40,0x3C,0x44 / 0x4C,0x48,0x50).
    obj->mProcessVt = (u32)lbl_eu_8052D238;
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    obj->ptmfMove[1] = w1;
    obj->ptmfMove[0] = w0;
    u32 w2 = *src++;
    obj->ptmfMove[2] = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    obj->ptmfDraw[1] = w0;
    obj->ptmfDraw[0] = w1;
    w2 = *src++;
    obj->ptmfDraw[2] = w2;

    obj->mLayout = 0;
    obj->mField58 = 0;
    obj->mField5C = 0;
    obj->mField60 = -1;
    obj->mField64 = 0;
    obj->mField65 = 0;
    obj->mField66 = 0;
    obj->mField67 = 1;
    obj->mField68 = 0;

    obj->mProcessVt = (u32)lbl_eu_80539E48;
    obj->mIWorkEventVt = (u32)lbl_eu_80539E48 + 0x24;
    obj->mIScnRenderVt = (u32)lbl_eu_80539E48 + 0xac;
    obj->mScn = parent;

    __ct__17UnkClass_8045F564Fv(&obj->mRegion);

    obj->mAnim0 = 0;
    obj->mAnim1 = 0;
    obj->mAnim2 = 0;
    obj->mAnim3 = 0;
    obj->mAnim4 = 0;
    obj->mField9C = 0;
    obj->mField9D = arg;
    obj->mField9E = 0;
    return obj;
}

// Member/base dtors and operator delete (retail names). The redundant
// `if (self != 0)` re-check reproduces the retail's dead double-beq.
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);
extern "C" void __dt__8CProcessFv(void* self, int flags);
extern "C" void __dl__FPv(void* p);

extern "C" void* __dt__16CMenuBattleChainFv(void* self, int flags) {
    if (self == 0)
        goto end;
    __dt__17UnkClass_8045F564Fv((u8*)self + 0x78, -1);
    if (self != 0) {
        if (self != 0) {
            __dt__8CProcessFv(self, 0);
        }
    }
    if (flags > 0)
        __dl__FPv(self);
end:
    return self;
}

/*
 * Build the battle-chain menu: allocate a 0x4000 scratch region from MEM2,
 * load the layout + five anim transforms from the arc, bind the font onto the
 * root pane, bind and play the first anim, register the IScnRender callback
 * at +0x70 on the owning scene and release the region (the Class_8045F858
 * guard destructs at scope end, after func_8045F810).
 */
void CMenuBattleChain::Init() {
    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    char* base = lbl_eu_8051088C;
    mRegion.createRegion(mem2, 0x4000, base, 0);
    Class_8045F858 regionGuard(&mRegion);

    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &mLayout, func_801355F4(), &base[0x11]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mLayout, &mAnim0, func_801355F4(), &base[0x2a]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mLayout, &mAnim1, func_801355F4(), &base[0x46]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mLayout, &mAnim2, func_801355F4(), &base[0x64]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mLayout, &mAnim3, func_801355F4(), &base[0x87]);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        mLayout, &mAnim4, func_801355F4(), &base[0xaa]);

    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    func_8013676C(rootPane, func_801355A0());

    mLayout->UnbindAllAnimation();
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnim0);
    mLayout->SetAnimationEnable(mAnim0, true);
    mLayout->Animate(0);

    mField67 = 0;

    // MI adjust: IScnRender subobject at +0x70 (null-this safe).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRenderVt);
    }
    mScn->addRenderCB(cb, 0xa, 0);

    mRegion.func_8045F810();
}

void CMenuBattleChain::Term() {}

void CMenuBattleChain::Move() {
    // Skip while the game task is busy or the render-before flag is set.
    if (CTaskGame::getInstance()->func_800426F0() || (lbl_eu_80663E28 & 0x200000)) {
        return;
    }
    if (func_8013BE50() == 0) {
        return;
    }
    switch (mField9C) {
    case 0:
        // Start of the chain: reset the frame counter and play anim0.
        mField9C = 1;
        mField9E = 0;
        mAnim0->SetFrame(lbl_eu_80668CB0);
        mLayout->UnbindAllAnimation();
        mLayout->BindAnimation(mAnim0);
        mLayout->SetAnimationEnable(mAnim0, true);
        mLayout->Animate(0);
        func_802AAC78(this);
        func_802AA588(this);
        break;
    case 1:
        if (func_80137444(mAnim0, lbl_eu_80668CB4) != 0) {
            mField9C = 2;
            mAnim1->SetFrame(lbl_eu_80668CB0);
            mLayout->UnbindAllAnimation();
            mLayout->BindAnimation(mAnim1);
            mLayout->SetAnimationEnable(mAnim1, true);
            mLayout->Animate(0);
        }
        break;
    case 2:
        if (func_80137444(mAnim1, lbl_eu_80668CB4) != 0) {
            mField9C = 3;
        }
        break;
    case 3:
        break;
    case 4:
        if (func_80137444(mAnim2, lbl_eu_80668CB4) != 0) {
            mField9C = 5;
            mAnim3->SetFrame(lbl_eu_80668CB0);
            mLayout->UnbindAllAnimation();
            mLayout->BindAnimation(mAnim3);
            mLayout->SetAnimationEnable(mAnim3, true);
            mLayout->Animate(0);
            func_802AAC78(this);
            func_802AA588(this);
        }
        break;
    case 5:
        if (func_80137444(mAnim3, lbl_eu_80668CB4) != 0) {
            mField9C = 3;
        }
        break;
    case 6:
        if (func_80137444(mAnim4, lbl_eu_80668CB4) != 0) {
            mField9C = 0;
        }
        break;
    case 7:
        if (func_80137510(mAnim0, lbl_eu_80668CB4) != 0) {
            mField9C = 8;
        }
        break;
    case 8:
        mField64 = 1;
        break;
    }
    mLayout->Animate(0);
}

void CMenuBattleChain::cbRenderBefore() {}

void func_802AA2A0(){}

/*
 * Per-frame hook: if the singleton is live, switch to chain mode 7 and bind /
 * play the first anim transform on the layout.
 */
void func_802AA338() {
    CMenuBattleChain* menu = lbl_eu_80664A60;
    if (menu != NULL) {
        menu->mField9C = 7;
        menu->mLayout->UnbindAllAnimation();
        menu->mLayout->BindAnimation(menu->mAnim0);
        menu->mLayout->SetAnimationEnable(menu->mAnim0, true);
        menu->mLayout->Animate(0);
    }
}

/*
 * Switch the displayed chain type. If the requested type equals the current
 * one (or either is 8), replay the second anim transform; otherwise switch to
 * the fourth. The new type is stored back at +0x9D.
 */
extern "C" void func_802AA3D0(CMenuBattleChain* self, u8 arg) {
    if (self->mField9D == arg) {
        self->mField9C = 4;
        self->mAnim2->SetFrame(lbl_eu_80668CB0);
        self->mLayout->UnbindAllAnimation();
        self->mLayout->BindAnimation(self->mAnim2);
        self->mLayout->SetAnimationEnable(self->mAnim2, true);
        self->mLayout->Animate(0);
    } else if (self->mField9D == 8 || arg == 8) {
        self->mField9C = 4;
        self->mAnim2->SetFrame(lbl_eu_80668CB0);
        self->mLayout->UnbindAllAnimation();
        self->mLayout->BindAnimation(self->mAnim2);
        self->mLayout->SetAnimationEnable(self->mAnim2, true);
        self->mLayout->Animate(0);
    } else {
        self->mField9C = 6;
        self->mAnim4->SetFrame(lbl_eu_80668CB0);
        self->mLayout->UnbindAllAnimation();
        self->mLayout->BindAnimation(self->mAnim4);
        self->mLayout->SetAnimationEnable(self->mAnim4, true);
        self->mLayout->Animate(0);
    }
    self->mField9D = arg;
}

/*
 * Apply the battle-chain colour scheme for chain type +0x9D: set the chain
 * title colour pair, three fixed pane colours, the progress gauge position,
 * the gauge colour pair and the four bar colour pairs (all pane names live
 * in the lbl_eu_8051088C name table at +0xc9..0x133). Type 0 has no scheme.
 */
extern "C" void func_802AA588(CMenuBattleChain* self) {
    switch (self->mField9D) {
    case 0:
        break;
    case 1: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664A68, lbl_eu_80664A70);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D70);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D70);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D70);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662D90, lbl_eu_80662D94);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664AE8, lbl_eu_80664AF0);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664B68, lbl_eu_80664B70);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664B68, lbl_eu_80664B70);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664B68, lbl_eu_80664B70);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664B68, lbl_eu_80664B70);
        break;
    }
    case 2: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664A78, lbl_eu_80664A80);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D74);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D74);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D74);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662D98, lbl_eu_80662D9C);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664AF8, lbl_eu_80664B00);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664B78, lbl_eu_80664B80);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664B78, lbl_eu_80664B80);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664B78, lbl_eu_80664B80);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664B78, lbl_eu_80664B80);
        break;
    }
    case 3: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664A88, lbl_eu_80664A90);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D78);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D78);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D78);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662DA0, lbl_eu_80662DA4);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664B08, lbl_eu_80664B10);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664B88, lbl_eu_80664B90);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664B88, lbl_eu_80664B90);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664B88, lbl_eu_80664B90);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664B88, lbl_eu_80664B90);
        break;
    }
    case 4: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664A98, lbl_eu_80664AA0);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D7C);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D7C);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D7C);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662DA8, lbl_eu_80662DAC);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664B18, lbl_eu_80664B20);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664B98, lbl_eu_80664BA0);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664B98, lbl_eu_80664BA0);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664B98, lbl_eu_80664BA0);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664B98, lbl_eu_80664BA0);
        break;
    }
    case 5: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664AA8, lbl_eu_80664AB0);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D80);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D80);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D80);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662DB0, lbl_eu_80662DB4);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664B28, lbl_eu_80664B30);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664BA8, lbl_eu_80664BB0);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664BA8, lbl_eu_80664BB0);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664BA8, lbl_eu_80664BB0);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664BA8, lbl_eu_80664BB0);
        break;
    }
    case 6: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664AB8, lbl_eu_80664AC0);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D84);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D84);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D84);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662DB8, lbl_eu_80662DBC);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664B38, lbl_eu_80664B40);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664BB8, lbl_eu_80664BC0);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664BB8, lbl_eu_80664BC0);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664BB8, lbl_eu_80664BC0);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664BB8, lbl_eu_80664BC0);
        break;
    }
    case 7: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664AC8, lbl_eu_80664AD0);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D88);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D88);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D88);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662DC0, lbl_eu_80662DC4);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664B48, lbl_eu_80664B50);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664BC8, lbl_eu_80664BD0);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664BC8, lbl_eu_80664BD0);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664BC8, lbl_eu_80664BD0);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664BC8, lbl_eu_80664BD0);
        break;
    }
    case 8: {
        char* base = lbl_eu_8051088C;
        func_80139A18(self->mLayout, &base[0xc9], lbl_eu_80664AD8, lbl_eu_80664AE0);
        func_80137CD4(self->mLayout, &base[0xd0], -1, lbl_eu_80662D8C);
        func_80137CD4(self->mLayout, &base[0xdc], -1, lbl_eu_80662D8C);
        func_80137CD4(self->mLayout, &base[0xe8], -1, lbl_eu_80662D8C);
        func_80139BF4(self->mLayout, &base[0xf4], lbl_eu_80662DC8, lbl_eu_80662DCC);
        func_80139A18(self->mLayout, &base[0xfc], lbl_eu_80664B58, lbl_eu_80664B60);
        func_80139A18(self->mLayout, &base[0x106], lbl_eu_80664BD8, lbl_eu_80664BE0);
        func_80139A18(self->mLayout, &base[0x115], lbl_eu_80664BD8, lbl_eu_80664BE0);
        func_80139A18(self->mLayout, &base[0x124], lbl_eu_80664BD8, lbl_eu_80664BE0);
        func_80139A18(self->mLayout, &base[0x133], lbl_eu_80664BD8, lbl_eu_80664BE0);
        break;
    }
    }
}

/*
 * Chain-frame counter: bump the +0x9E counter each call. While the counter is
 * below 5, show the "progress" panes; once it reaches 5, clamp it and show the
 * "done" panes instead. Pane visibility is bit 0 of the byte at +0xBB.
 */
extern "C" void func_802AAC78(CMenuBattleChain* self) {
    u8 count = self->mField9E + 1;
    self->mField9E = count;
    if (count >= 5) {
        self->mField9E = 5;
        char* base = lbl_eu_8051088C;
        self->mLayout->GetRootPane()->FindPaneByName(&base[0xdc], true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0xe8], true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0xf4], true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0x115], true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0x124], true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0x133], true)->SetVisible(false);
    } else {
        char* base = lbl_eu_8051088C;
        self->mLayout->GetRootPane()->FindPaneByName(&base[0xdc], true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0xe8], true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0xf4], true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0x115], true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0x124], true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName(&base[0x133], true)->SetVisible(true);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout, &base[0xf4], self->mField9E);
        func_80136910__FPQ34nw4r3lyt6LayoutPcUc(self->mLayout, &base[0x133], self->mField9E);
    }
}

void func_802AB3B8(void* self) { ((void(*)(void*))__dt__16CMenuBattleChainFv)((char*)self - 0x6c); }

void func_802AB3C0(void* self) { ((void(*)(void*))cbRenderBefore__16CMenuBattleChainFv)((char*)self - 0x70); }

void func_802AB3C8(void* self) { ((void(*)(void*))__dt__16CMenuBattleChainFv)((char*)self - 0x70); }

// Track the current player and clear both pending toggle flags.
void func_802AB3D0(CBattleChainMenuState* self) {
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

// Consume the pending toggle flags once the arts-select menu is interactable.
void func_802AB410(CBattleChainMenuState* self) {
    if (CMenuArtsSelect_isInteractable()) {
        if (self->mFlag4 != 0) {
            func_8010433C();
            self->mFlag4 = 0;
        }
        if (self->mFlag5 != 0) {
            func_801043BC();
            self->mFlag5 = 0;
        }
    }
}

// Disable the arts-select menu, then (re)track the current player.
void func_802AB474(CBattleChainMenuState* self) {
    CMenuArtsSelect_setDisabled();
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

// Disable the arts-select menu only when the tracked player changed, then
// (re)track the current player.
void func_802AB4B8(CBattleChainMenuState* self) {
    if (self->mPlayer != cf::CfGameManager::getPlayer(0)) {
        CMenuArtsSelect_setDisabled();
    }
    self->mPlayer = cf::CfGameManager::getPlayer(0);
    self->mFlag4 = 0;
    self->mFlag5 = 0;
}

void func_802AB510(){}

extern "C" void func_802AB590(void* self) { *(u8*)((u8*)self + 4) = 1; }

// Whether the chain menu is busy (pending flag or arts-select not ready).
bool func_802AB59C(CBattleChainMenuState* self) {
    return self->mFlag4 != 0 || CMenuArtsSelect_isNotReady();
}

extern "C" void func_802AB5E4(void* self) { *(u8*)((u8*)self + 5) = 1; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Battle-chain pane colour records (zero-init .sbss; values filled at startup
// by sinit_802AAF24). Pairs are consumed by func_802AA588 per chain type.
u16 lbl_eu_80664A68[4];
u16 lbl_eu_80664A70[4];
u16 lbl_eu_80664A78[4];
u16 lbl_eu_80664A80[4];
u16 lbl_eu_80664A88[4];
u16 lbl_eu_80664A90[4];
u16 lbl_eu_80664A98[4];
u16 lbl_eu_80664AA0[4];
u16 lbl_eu_80664AA8[4];
u16 lbl_eu_80664AB0[4];
u16 lbl_eu_80664AB8[4];
u16 lbl_eu_80664AC0[4];
u16 lbl_eu_80664AC8[4];
u16 lbl_eu_80664AD0[4];
u16 lbl_eu_80664AD8[4];
u16 lbl_eu_80664AE0[4];
u16 lbl_eu_80664AE8[4];
u16 lbl_eu_80664AF0[4];
u16 lbl_eu_80664AF8[4];
u16 lbl_eu_80664B00[4];
u16 lbl_eu_80664B08[4];
u16 lbl_eu_80664B10[4];
u16 lbl_eu_80664B18[4];
u16 lbl_eu_80664B20[4];
u16 lbl_eu_80664B28[4];
u16 lbl_eu_80664B30[4];
u16 lbl_eu_80664B38[4];
u16 lbl_eu_80664B40[4];
u16 lbl_eu_80664B48[4];
u16 lbl_eu_80664B50[4];
u16 lbl_eu_80664B58[4];
u16 lbl_eu_80664B60[4];
u16 lbl_eu_80664B68[4];
u16 lbl_eu_80664B70[4];
u16 lbl_eu_80664B78[4];
u16 lbl_eu_80664B80[4];
u16 lbl_eu_80664B88[4];
u16 lbl_eu_80664B90[4];
u16 lbl_eu_80664B98[4];
u16 lbl_eu_80664BA0[4];
u16 lbl_eu_80664BA8[4];
u16 lbl_eu_80664BB0[4];
u16 lbl_eu_80664BB8[4];
u16 lbl_eu_80664BC0[4];
u16 lbl_eu_80664BC8[4];
u16 lbl_eu_80664BD0[4];
u16 lbl_eu_80664BD8[4];
u16 lbl_eu_80664BE0[4];

/*
 * Static initializer filling the 48 battle-chain colour records. Written in
 * retail's source order: records in address order, elements stored back-to-
 * front [3][2][1][0] (matches MWCC's register allocation: constants promoted
 * in first-use order 0,0x7f,0xff,0x69,0xf0,0x70,0x32,0xfe,0x0c,0x3a,0x4e,
 * 0x5a,0x1e,0x58; single-use constants re-loaded via r0).
 */
extern "C" void sinit_802AAF24() {
    lbl_eu_80664A68[3] = 0;
    lbl_eu_80664A68[2] = 0;
    lbl_eu_80664A68[1] = 0;
    lbl_eu_80664A68[0] = 0x7f;
    lbl_eu_80664A70[3] = 0xff;
    lbl_eu_80664A70[2] = 0x69;
    lbl_eu_80664A70[1] = 0x69;
    lbl_eu_80664A70[0] = 0xf0;
    lbl_eu_80664A78[3] = 0;
    lbl_eu_80664A78[2] = 0x70;
    lbl_eu_80664A78[1] = 0;
    lbl_eu_80664A78[0] = 0x32;
    lbl_eu_80664A80[3] = 0xff;
    lbl_eu_80664A80[2] = 0xfe;
    lbl_eu_80664A80[1] = 0x64;
    lbl_eu_80664A80[0] = 0x8a;
    lbl_eu_80664A88[3] = 0;
    lbl_eu_80664A88[2] = 0x0c;
    lbl_eu_80664A88[1] = 0x3a;
    lbl_eu_80664A88[0] = 0;
    lbl_eu_80664A90[3] = 0xff;
    lbl_eu_80664A90[2] = 0x2a;
    lbl_eu_80664A90[1] = 0xba;
    lbl_eu_80664A90[0] = 0x48;
    lbl_eu_80664A98[3] = 0;
    lbl_eu_80664A98[2] = 0;
    lbl_eu_80664A98[1] = 0x4e;
    lbl_eu_80664A98[0] = 0x5a;
    lbl_eu_80664AA0[3] = 0xff;
    lbl_eu_80664AA0[2] = 0x40;
    lbl_eu_80664AA0[1] = 0xdc;
    lbl_eu_80664AA0[0] = 0xf0;
    lbl_eu_80664AA8[3] = 0;
    lbl_eu_80664AA8[2] = 0;
    lbl_eu_80664AA8[1] = 0x1e;
    lbl_eu_80664AA8[0] = 0x58;
    lbl_eu_80664AB0[3] = 0xff;
    lbl_eu_80664AB0[2] = 0x2d;
    lbl_eu_80664AB0[1] = 0x7f;
    lbl_eu_80664AB0[0] = 0xea;
    lbl_eu_80664AB8[3] = 0;
    lbl_eu_80664AB8[2] = 0xa2;
    lbl_eu_80664AB8[1] = 0x3b;
    lbl_eu_80664AB8[0] = 0x0c;
    lbl_eu_80664AC0[3] = 0xff;
    lbl_eu_80664AC0[2] = 0xf2;
    lbl_eu_80664AC0[1] = 0x97;
    lbl_eu_80664AC0[0] = 0x4d;
    lbl_eu_80664AC8[3] = 0;
    lbl_eu_80664AC8[2] = 0x4c;
    lbl_eu_80664AC8[1] = 0;
    lbl_eu_80664AC8[0] = 0x5a;
    lbl_eu_80664AD0[3] = 0xff;
    lbl_eu_80664AD0[2] = 0xde;
    lbl_eu_80664AD0[1] = 0x72;
    lbl_eu_80664AD0[0] = 0xe6;
    lbl_eu_80664AD8[3] = 0;
    lbl_eu_80664AD8[2] = 0x46;
    lbl_eu_80664AD8[1] = 0x46;
    lbl_eu_80664AD8[0] = 0x46;
    lbl_eu_80664AE0[3] = 0xff;
    lbl_eu_80664AE0[2] = 0xf0;
    lbl_eu_80664AE0[1] = 0xf0;
    lbl_eu_80664AE0[0] = 0xf0;
    lbl_eu_80664AE8[3] = 0;
    lbl_eu_80664AE8[2] = 0;
    lbl_eu_80664AE8[1] = 0;
    lbl_eu_80664AE8[0] = 0x7f;
    lbl_eu_80664AF0[3] = 0xff;
    lbl_eu_80664AF0[2] = 0x69;
    lbl_eu_80664AF0[1] = 0x69;
    lbl_eu_80664AF0[0] = 0xf0;
    lbl_eu_80664AF8[3] = 0;
    lbl_eu_80664AF8[2] = 0x70;
    lbl_eu_80664AF8[1] = 0;
    lbl_eu_80664AF8[0] = 0x32;
    lbl_eu_80664B00[3] = 0xff;
    lbl_eu_80664B00[2] = 0xfe;
    lbl_eu_80664B00[1] = 0x64;
    lbl_eu_80664B00[0] = 0x8a;
    lbl_eu_80664B08[3] = 0;
    lbl_eu_80664B08[2] = 0x0c;
    lbl_eu_80664B08[1] = 0x3a;
    lbl_eu_80664B08[0] = 0;
    lbl_eu_80664B10[3] = 0xff;
    lbl_eu_80664B10[2] = 0x2a;
    lbl_eu_80664B10[1] = 0xba;
    lbl_eu_80664B10[0] = 0x48;
    lbl_eu_80664B18[3] = 0;
    lbl_eu_80664B18[2] = 0;
    lbl_eu_80664B18[1] = 0x4e;
    lbl_eu_80664B18[0] = 0x5a;
    lbl_eu_80664B20[3] = 0xff;
    lbl_eu_80664B20[2] = 0x40;
    lbl_eu_80664B20[1] = 0xdc;
    lbl_eu_80664B20[0] = 0xf0;
    lbl_eu_80664B28[3] = 0;
    lbl_eu_80664B28[2] = 0;
    lbl_eu_80664B28[1] = 0x1e;
    lbl_eu_80664B28[0] = 0x58;
    lbl_eu_80664B30[3] = 0xff;
    lbl_eu_80664B30[2] = 0x2d;
    lbl_eu_80664B30[1] = 0x7f;
    lbl_eu_80664B30[0] = 0xea;
    lbl_eu_80664B38[3] = 0;
    lbl_eu_80664B38[2] = 0xa2;
    lbl_eu_80664B38[1] = 0x3b;
    lbl_eu_80664B38[0] = 0x0c;
    lbl_eu_80664B40[3] = 0xff;
    lbl_eu_80664B40[2] = 0xf2;
    lbl_eu_80664B40[1] = 0x97;
    lbl_eu_80664B40[0] = 0x4d;
    lbl_eu_80664B48[3] = 0;
    lbl_eu_80664B48[2] = 0x4c;
    lbl_eu_80664B48[1] = 0;
    lbl_eu_80664B48[0] = 0x5a;
    lbl_eu_80664B50[3] = 0xff;
    lbl_eu_80664B50[2] = 0xde;
    lbl_eu_80664B50[1] = 0x72;
    lbl_eu_80664B50[0] = 0xe6;
    lbl_eu_80664B58[3] = 0;
    lbl_eu_80664B58[2] = 0x46;
    lbl_eu_80664B58[1] = 0x46;
    lbl_eu_80664B58[0] = 0x46;
    lbl_eu_80664B60[3] = 0xff;
    lbl_eu_80664B60[2] = 0xf0;
    lbl_eu_80664B60[1] = 0xf0;
    lbl_eu_80664B60[0] = 0xf0;
    lbl_eu_80664B68[3] = 0;
    lbl_eu_80664B68[2] = 0;
    lbl_eu_80664B68[1] = 0;
    lbl_eu_80664B68[0] = 0x7f;
    lbl_eu_80664B70[3] = 0xff;
    lbl_eu_80664B70[2] = 0xff;
    lbl_eu_80664B70[1] = 0xff;
    lbl_eu_80664B70[0] = 0xff;
    lbl_eu_80664B78[3] = 0;
    lbl_eu_80664B78[2] = 0x70;
    lbl_eu_80664B78[1] = 0;
    lbl_eu_80664B78[0] = 0x32;
    lbl_eu_80664B80[3] = 0xff;
    lbl_eu_80664B80[2] = 0xff;
    lbl_eu_80664B80[1] = 0xff;
    lbl_eu_80664B80[0] = 0xff;
    lbl_eu_80664B88[3] = 0;
    lbl_eu_80664B88[2] = 0x0c;
    lbl_eu_80664B88[1] = 0x3a;
    lbl_eu_80664B88[0] = 0;
    lbl_eu_80664B90[3] = 0xff;
    lbl_eu_80664B90[2] = 0xff;
    lbl_eu_80664B90[1] = 0xff;
    lbl_eu_80664B90[0] = 0xff;
    lbl_eu_80664B98[3] = 0;
    lbl_eu_80664B98[2] = 0;
    lbl_eu_80664B98[1] = 0x4e;
    lbl_eu_80664B98[0] = 0x5a;
    lbl_eu_80664BA0[3] = 0xff;
    lbl_eu_80664BA0[2] = 0xff;
    lbl_eu_80664BA0[1] = 0xff;
    lbl_eu_80664BA0[0] = 0xff;
    lbl_eu_80664BA8[3] = 0;
    lbl_eu_80664BA8[2] = 0;
    lbl_eu_80664BA8[1] = 0x1e;
    lbl_eu_80664BA8[0] = 0x58;
    lbl_eu_80664BB0[3] = 0xff;
    lbl_eu_80664BB0[2] = 0xff;
    lbl_eu_80664BB0[1] = 0xff;
    lbl_eu_80664BB0[0] = 0xff;
    lbl_eu_80664BB8[3] = 0;
    lbl_eu_80664BB8[2] = 0xa2;
    lbl_eu_80664BB8[1] = 0x3b;
    lbl_eu_80664BB8[0] = 0x0c;
    lbl_eu_80664BC0[3] = 0xff;
    lbl_eu_80664BC0[2] = 0xff;
    lbl_eu_80664BC0[1] = 0xff;
    lbl_eu_80664BC0[0] = 0xff;
    lbl_eu_80664BC8[3] = 0;
    lbl_eu_80664BC8[2] = 0x4c;
    lbl_eu_80664BC8[1] = 0;
    lbl_eu_80664BC8[0] = 0x5a;
    lbl_eu_80664BD0[3] = 0xff;
    lbl_eu_80664BD0[2] = 0xff;
    lbl_eu_80664BD0[1] = 0xff;
    lbl_eu_80664BD0[0] = 0xff;
    lbl_eu_80664BD8[3] = 0;
    lbl_eu_80664BD8[2] = 0x46;
    lbl_eu_80664BD8[1] = 0x46;
    lbl_eu_80664BD8[0] = 0x46;
    lbl_eu_80664BE0[3] = 0xff;
    lbl_eu_80664BE0[2] = 0xff;
    lbl_eu_80664BE0[1] = 0xff;
    lbl_eu_80664BE0[0] = 0xff;
}

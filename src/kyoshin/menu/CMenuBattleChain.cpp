// Auto-scaffolded catalog TU for kyoshin/menu/CMenuBattleChain
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/lyt/lyt_drawInfo.h>

// The retail CMenuBattleChain destructor calls __dt__8CProcessFv directly: the
// IUIWindow / CTTask<IUIWindow> / IWorkEvent destructor bodies are inlined into
// this TU (cs. CSysWinBuff / CTalkWindow).
#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuBattleChain.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <monolib/work/CProcess.hpp>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_animation.h>
#include "kyoshin/code_80135FDC_Lyt.hpp"

class IWorkEvent;
class IScnRender;

/*
 * Battle-chain resource names. Retail keeps them in MWCC's string pool as one
 * literal block: the layout/region name first, then the five anim transforms
 * (+0x11/+0x2a/+0x46/+0x64/+0x87/+0xaa) and the pane names at +0xc9..0x133 that
 * applyChainColorScheme indexes by chain type, plus six trailing zeros. That is
 * why the compiler's own "CMenuBattleChain" type-name string sits ahead of it at
 * .rodata:0x80510878 (the address __RTTI__16CMenuBattleChain points at) and the
 * block follows at 0x8051088C.
 */
// Thunk targets (retail mangled names). extern "C" keeps the exact symbol
// (no extra __FPv); GetItemMulti idiom.
extern "C" void* __dt__16CMenuBattleChainFv(CMenuBattleChain* self, int flags);
extern "C" void cbRenderBefore__16CMenuBattleChainFv(CMenuBattleChain* self);

// Term helpers (retail names; CDeviceVI static + UnkClass member).
namespace CDeviceVI {
void waitForDrawDone();
}

/*
 * CMenuBattleChain::CMenuBattleChain - CProcess base, then the inlined
 * CTTask<IUIWindow> / IUIWindow base constructors (base vptr, null Move/Draw
 * pmf hooks, layout/state fields), the composite and sub-vptrs, the embedded
 * layout-build region and the animation/state tail. The chain-type byte lands
 * at +0x9D.
 */
CMenuBattleChain::CMenuBattleChain(CScn* parent, u8 arg)
    : mScn(parent), mRegion(), mAnim0(0), mAnim1(0), mAnim2(0), mAnim3(0),
      mAnim4(0), mField9C(0), mField9D(arg), mField9E(0) {}

/*
 * Complete-object destructor. The embedded layout-build region is destroyed
 * (flags -1), then the CProcess base guarded by the nested double null-check
 * (an MWCC D2-inlined-into-D1 artifact; the IUIWindow / CTTask<IUIWindow> /
 * IWorkEvent base dtors are the inline-empty variants selected by the macros
 * at the top of this file), then the deleting path calls operator delete.
 * Same shape as ~CTalkWindow / ~CSysWinSave.
 */
CMenuBattleChain::~CMenuBattleChain() {}

/*
 * Build the battle-chain menu: allocate a 0x4000 scratch region from MEM2,
 * load the layout + five anim transforms from the arc, bind the font onto the
 * root pane, bind and play the first anim, register the IScnRender callback
 * at +0x70 on the owning scene and release the region (the Class_8045F858
 * guard destructs at scope end, after validateHeap).
 */
void CMenuBattleChain::Init() {
    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    mRegion.createRegion(mem2, 0x4000, "CMenuBattleChain", 0);
    Class_8045F858 regionGuard(&mRegion);

    buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(), "mf70_cf54_sideinfo.brlyt");
    bindLayoutAnimTransform(mLayout, &mAnim0, CUICfManager_getArcResourceAccessor(), "mf70_cf54_sideinfo_in.brlan");
    bindLayoutAnimTransform(mLayout, &mAnim1, CUICfManager_getArcResourceAccessor(), "mf70_cf54_sideinfo_glow.brlan");
    bindLayoutAnimTransform(mLayout, &mAnim2, CUICfManager_getArcResourceAccessor(), "mf70_cf54_sideinfo_chang_out.brlan");
    bindLayoutAnimTransform(mLayout, &mAnim3, CUICfManager_getArcResourceAccessor(), "mf70_cf54_sideinfo_change_in.brlan");
    bindLayoutAnimTransform(mLayout, &mAnim4, CUICfManager_getArcResourceAccessor(), "mf70_cf54_sideinfo_break.brlan");

    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)getPackedFont());

    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnim0);
    mLayout->SetAnimationEnable(mAnim0, true);
    mLayout->Animate(0);

    mField67 = 0;

    mScn->addRenderCB(this, 0xa, 0);

    mRegion.validateHeap();
}

void CMenuBattleChain::Term() {
    CDeviceVI::waitForDrawDone();
    mScn->removeRenderCB(this);
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }
    mRegion.deleteRegion();
    lbl_eu_80664A60 = 0;
}

void CMenuBattleChain::Move() {
    // Skip while the game task is busy or the render-before flag is set.
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000)) {
        return;
    }
    if (IsMenuState621F0() == 0) {
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
        tickChainCounter(this);
        applyChainColorScheme(this);
        break;
    case 1:
        if (advanceAnimTransform(mAnim0, lbl_eu_80668CB4) != 0) {
            mField9C = 2;
            mAnim1->SetFrame(lbl_eu_80668CB0);
            mLayout->UnbindAllAnimation();
            mLayout->BindAnimation(mAnim1);
            mLayout->SetAnimationEnable(mAnim1, true);
            mLayout->Animate(0);
        }
        break;
    case 2:
        if (advanceAnimTransform(mAnim1, lbl_eu_80668CB4) != 0) {
            mField9C = 3;
        }
        break;
    case 3:
        break;
    case 4:
        if (advanceAnimTransform(mAnim2, lbl_eu_80668CB4) != 0) {
            mField9C = 5;
            mAnim3->SetFrame(lbl_eu_80668CB0);
            mLayout->UnbindAllAnimation();
            mLayout->BindAnimation(mAnim3);
            mLayout->SetAnimationEnable(mAnim3, true);
            mLayout->Animate(0);
            tickChainCounter(this);
            applyChainColorScheme(this);
        }
        break;
    case 5:
        if (advanceAnimTransform(mAnim3, lbl_eu_80668CB4) != 0) {
            mField9C = 3;
        }
        break;
    case 6:
        if (advanceAnimTransform(mAnim4, lbl_eu_80668CB4) != 0) {
            mField9C = 0;
        }
        break;
    case 7:
        if (AnimRewindFrame(mAnim0, lbl_eu_80668CB4) != 0) {
            mField9C = 8;
        }
        break;
    case 8:
        mField64 = 1;
        break;
    }
    mLayout->Animate(0);
}

void CMenuBattleChain::cbRenderBefore() {
    // Gate: skip when the task game is busy or the global mode bit (0x200000)
    // is set. The if-&&-goto body / goto end / end: return chain keeps the
    // body off the fallthrough so MWCC emits retail's branch-over-branch:
    // `bne end` for the first disjunct, `beq body; b end` for the second
    // (same scheme as CSysWinSelect::cbRenderBefore).
    if (CTaskGame::getInstance()->isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (IsMenuState621F0() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mLayout, &drawInfo, 0, 1);
}

/*
 * Create or update the battle-chain menu singleton. If it already exists,
 * switch its chain type and return null; otherwise allocate 0xa0 bytes from
 * the work heap, construct the menu and register it under the parent process.
 */
// (memberized: switchChainType -> CMenuBattleChain::switchChainType; declaration moved into class CMenuBattleChain)
extern "C" CMenuBattleChain* createBattleChainMenu(CProcess* parent, CScn* scene, u8 chainType) {
    if (lbl_eu_80664A60 != NULL) {
        lbl_eu_80664A60->switchChainType(chainType);
        return NULL;
    }
    u32 heap = CWorkThreadSystem::getWorkMem();
    // Retail keeps obj live in r3 by chaining the ctor's `this` return
    // through the null-check, avoiding a fourth callee-saved register.
    CMenuBattleChain* obj = new (mtl::MemManager::allocate(0xa0, heap))
        CMenuBattleChain(scene, chainType);
    // Retail keeps obj live in r3 across the singleton store so Regist is
    // called directly on it; the global is re-read only for the return value.
    lbl_eu_80664A60 = obj;
    ((CProcess*)obj)->Regist(parent, false);
    return lbl_eu_80664A60;
}

/*
 * Per-frame hook: if the singleton is live, switch to chain mode 7 and bind /
 * play the first anim transform on the layout.
 */
void requestCancelChain() {
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
void CMenuBattleChain::switchChainType(u8 arg) {
    if (this->mField9D == arg) {
        this->mField9C = 4;
        this->mAnim2->SetFrame(lbl_eu_80668CB0);
        this->mLayout->UnbindAllAnimation();
        this->mLayout->BindAnimation(this->mAnim2);
        this->mLayout->SetAnimationEnable(this->mAnim2, true);
        this->mLayout->Animate(0);
    } else if (this->mField9D == 8 || arg == 8) {
        this->mField9C = 4;
        this->mAnim2->SetFrame(lbl_eu_80668CB0);
        this->mLayout->UnbindAllAnimation();
        this->mLayout->BindAnimation(this->mAnim2);
        this->mLayout->SetAnimationEnable(this->mAnim2, true);
        this->mLayout->Animate(0);
    } else {
        this->mField9C = 6;
        this->mAnim4->SetFrame(lbl_eu_80668CB0);
        this->mLayout->UnbindAllAnimation();
        this->mLayout->BindAnimation(this->mAnim4);
        this->mLayout->SetAnimationEnable(this->mAnim4, true);
        this->mLayout->Animate(0);
    }
    this->mField9D = arg;
}

/*
 * Apply the battle-chain colour scheme for chain type +0x9D: set the chain
 * title colour pair, three fixed pane colours, the progress gauge position,
 * the gauge colour pair and the four bar colour pairs (all pane names live
 * in the lbl_eu_8051088C name table at +0xc9..0x133). Type 0 has no scheme.
 */
extern "C" void applyChainColorScheme(CMenuBattleChain* self) {
    switch (self->mField9D) {
    case 0:
        break;
    case 1: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664A68, (const GXColorS10*)lbl_eu_80664A70);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D70);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D70);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D70);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662D90, lbl_eu_80662D94);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664AE8, (const GXColorS10*)lbl_eu_80664AF0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664B68, (const GXColorS10*)lbl_eu_80664B70);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664B68, (const GXColorS10*)lbl_eu_80664B70);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664B68, (const GXColorS10*)lbl_eu_80664B70);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664B68, (const GXColorS10*)lbl_eu_80664B70);
        break;
    }
    case 2: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664A78, (const GXColorS10*)lbl_eu_80664A80);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D74);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D74);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D74);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662D98, lbl_eu_80662D9C);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664AF8, (const GXColorS10*)lbl_eu_80664B00);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664B78, (const GXColorS10*)lbl_eu_80664B80);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664B78, (const GXColorS10*)lbl_eu_80664B80);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664B78, (const GXColorS10*)lbl_eu_80664B80);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664B78, (const GXColorS10*)lbl_eu_80664B80);
        break;
    }
    case 3: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664A88, (const GXColorS10*)lbl_eu_80664A90);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D78);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D78);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D78);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662DA0, lbl_eu_80662DA4);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664B08, (const GXColorS10*)lbl_eu_80664B10);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664B88, (const GXColorS10*)lbl_eu_80664B90);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664B88, (const GXColorS10*)lbl_eu_80664B90);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664B88, (const GXColorS10*)lbl_eu_80664B90);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664B88, (const GXColorS10*)lbl_eu_80664B90);
        break;
    }
    case 4: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664A98, (const GXColorS10*)lbl_eu_80664AA0);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D7C);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D7C);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D7C);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662DA8, lbl_eu_80662DAC);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664B18, (const GXColorS10*)lbl_eu_80664B20);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664B98, (const GXColorS10*)lbl_eu_80664BA0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664B98, (const GXColorS10*)lbl_eu_80664BA0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664B98, (const GXColorS10*)lbl_eu_80664BA0);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664B98, (const GXColorS10*)lbl_eu_80664BA0);
        break;
    }
    case 5: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664AA8, (const GXColorS10*)lbl_eu_80664AB0);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D80);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D80);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D80);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662DB0, lbl_eu_80662DB4);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664B28, (const GXColorS10*)lbl_eu_80664B30);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664BA8, (const GXColorS10*)lbl_eu_80664BB0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664BA8, (const GXColorS10*)lbl_eu_80664BB0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664BA8, (const GXColorS10*)lbl_eu_80664BB0);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664BA8, (const GXColorS10*)lbl_eu_80664BB0);
        break;
    }
    case 6: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664AB8, (const GXColorS10*)lbl_eu_80664AC0);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D84);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D84);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D84);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662DB8, lbl_eu_80662DBC);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664B38, (const GXColorS10*)lbl_eu_80664B40);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664BB8, (const GXColorS10*)lbl_eu_80664BC0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664BB8, (const GXColorS10*)lbl_eu_80664BC0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664BB8, (const GXColorS10*)lbl_eu_80664BC0);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664BB8, (const GXColorS10*)lbl_eu_80664BC0);
        break;
    }
    case 7: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664AC8, (const GXColorS10*)lbl_eu_80664AD0);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D88);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D88);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D88);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662DC0, lbl_eu_80662DC4);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664B48, (const GXColorS10*)lbl_eu_80664B50);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664BC8, (const GXColorS10*)lbl_eu_80664BD0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664BC8, (const GXColorS10*)lbl_eu_80664BD0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664BC8, (const GXColorS10*)lbl_eu_80664BD0);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664BC8, (const GXColorS10*)lbl_eu_80664BD0);
        break;
    }
    case 8: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)lbl_eu_80664AD8, (const GXColorS10*)lbl_eu_80664AE0);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, lbl_eu_80662D8C);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, lbl_eu_80662D8C);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, lbl_eu_80662D8C);
        PaneSetColorFieldPair(self->mLayout, "txt_num", lbl_eu_80662DC8, lbl_eu_80662DCC);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)lbl_eu_80664B58, (const GXColorS10*)lbl_eu_80664B60);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)lbl_eu_80664BD8, (const GXColorS10*)lbl_eu_80664BE0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)lbl_eu_80664BD8, (const GXColorS10*)lbl_eu_80664BE0);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)lbl_eu_80664BD8, (const GXColorS10*)lbl_eu_80664BE0);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)lbl_eu_80664BD8, (const GXColorS10*)lbl_eu_80664BE0);
        break;
    }
    }
}

/*
 * Chain-frame counter: bump the +0x9E counter each call. While the counter is
 * below 5, show the "progress" panes; once it reaches 5, clamp it and show the
 * "done" panes instead. Pane visibility is bit 0 of the byte at +0xBB.
 */
extern "C" void tickChainCounter(CMenuBattleChain* self) {
    u8 count = self->mField9E + 1;
    self->mField9E = count;
    if (count >= 5) {
        self->mField9E = 5;
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain01", true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain02", true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName("txt_num", true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain01_gl", true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain02_gl", true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName("txt_num_gl", true)->SetVisible(false);
    } else {
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain01", true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain02", true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName("txt_num", true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain01_gl", true)->SetVisible(true);
        self->mLayout->GetRootPane()->FindPaneByName("pic_chain02_gl", true)->SetVisible(false);
        self->mLayout->GetRootPane()->FindPaneByName("txt_num_gl", true)->SetVisible(true);
        setLayoutTextBoxNumber(self->mLayout, "txt_num", self->mField9E);
        setLayoutTextBoxNumber(self->mLayout, "txt_num_gl", self->mField9E);
    }
}

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

// Battle-chain singleton instance (retail .sbss head slot lbl_eu_80664A60;
// the 48 colour records follow it, matching the split range
// 0x80664A60-0x80664BE8).
CMenuBattleChain* lbl_eu_80664A60;
// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Battle-chain pane colour records (zero-init .sbss; values filled at startup
// by sinit_802AAF24). Pairs are consumed by applyChainColorScheme per chain type.
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

// (The resource-name block above is the 0x144-byte literal at retail
// .rodata:0x8051088C; the compiler emits it into the string pool.)

// Anim-frame constants (retail .sdata2 pool lbl_eu_80668CB0/B4: 0.0f frame
// reset, 1.0f "finished" threshold). Values verified against the retail
// object (build/us/obj/kyoshin/menu/CMenuBattleChain.o .sdata2).
extern const f32 lbl_eu_80668CB0 = 0.0f;
extern const f32 lbl_eu_80668CB4 = 1.0f;

// Chain-type colour/position constants (retail .sdata lbl_eu_80662D70..DCC).
// Values verified against the retail object
// (build/us/obj/kyoshin/menu/CMenuBattleChain.o .sdata): slots 0x70-0x8C
// are the eight per-type colours, slots 0x90-0xCC repeat them as
// (white, colour) pairs consumed by PaneSetColorFieldPair.
u32 lbl_eu_80662D70 = 0xF06969FF;
u32 lbl_eu_80662D74 = 0x8A64FEFF;
u32 lbl_eu_80662D78 = 0x48BA2AFF;
u32 lbl_eu_80662D7C = 0xF0DC40FF;
u32 lbl_eu_80662D80 = 0xEA7F2DFF;
u32 lbl_eu_80662D84 = 0x4D97F2FF;
u32 lbl_eu_80662D88 = 0xE672DEFF;
u32 lbl_eu_80662D8C = 0xA0A0A0FF;
u32 lbl_eu_80662D90 = 0xF0F0F0FF;
u32 lbl_eu_80662D94 = 0xF06969FF;
u32 lbl_eu_80662D98 = 0xF0F0F0FF;
u32 lbl_eu_80662D9C = 0x8A64FEFF;
u32 lbl_eu_80662DA0 = 0xF0F0F0FF;
u32 lbl_eu_80662DA4 = 0x48BA2AFF;
u32 lbl_eu_80662DA8 = 0xF0F0F0FF;
u32 lbl_eu_80662DAC = 0xF0DC40FF;
u32 lbl_eu_80662DB0 = 0xF0F0F0FF;
u32 lbl_eu_80662DB4 = 0xEA7F2DFF;
u32 lbl_eu_80662DB8 = 0xF0F0F0FF;
u32 lbl_eu_80662DBC = 0x4D97F2FF;
u32 lbl_eu_80662DC0 = 0xF0F0F0FF;
u32 lbl_eu_80662DC4 = 0xE672DEFF;
u32 lbl_eu_80662DC8 = 0xF0F0F0FF;
u32 lbl_eu_80662DCC = 0xA0A0A0FF;


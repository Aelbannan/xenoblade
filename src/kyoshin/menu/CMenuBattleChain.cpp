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
CMenuBattleChain::sInstance = 0;
}

void CMenuBattleChain::Move() {
    // Skip while the game task is busy or the render-before flag is set.
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000)) {
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
        (gmPauseFlags & 0x200000) == 0) {
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
if (CMenuBattleChain::sInstance != NULL) {
        CMenuBattleChain::sInstance->switchChainType(chainType);
        return NULL;
    }
    u32 heap = CWorkThreadSystem::getWorkMem();
    // Retail keeps obj live in r3 by chaining the ctor's `this` return
    // through the null-check, avoiding a fourth callee-saved register.
    CMenuBattleChain* obj = new (mtl::MemManager::allocate(0xa0, heap))
        CMenuBattleChain(scene, chainType);
    // Retail keeps obj live in r3 across the singleton store so Regist is
    // called directly on it; the global is re-read only for the return value.
CMenuBattleChain::sInstance = obj;
    ((CProcess*)obj)->Regist(parent, false);
return CMenuBattleChain::sInstance;
}

/*
 * Per-frame hook: if the singleton is live, switch to chain mode 7 and bind /
 * play the first anim transform on the layout.
 */
void requestCancelChain() {
CMenuBattleChain* menu = CMenuBattleChain::sInstance;
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
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT1A, (const GXColorS10*)chainIconColorT1B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT1);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT1);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT1);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT1, chainTextColorT1);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT1A, (const GXColorS10*)chainIconGlossColorT1B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT1A, (const GXColorS10*)chainBarColorT1B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT1A, (const GXColorS10*)chainBarColorT1B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT1A, (const GXColorS10*)chainBarColorT1B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT1A, (const GXColorS10*)chainBarColorT1B);
        break;
    }
    case 2: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT2A, (const GXColorS10*)chainIconColorT2B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT2);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT2);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT2);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT2, chainTextColorT2);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT2A, (const GXColorS10*)chainIconGlossColorT2B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT2A, (const GXColorS10*)chainBarColorT2B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT2A, (const GXColorS10*)chainBarColorT2B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT2A, (const GXColorS10*)chainBarColorT2B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT2A, (const GXColorS10*)chainBarColorT2B);
        break;
    }
    case 3: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT3A, (const GXColorS10*)chainIconColorT3B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT3);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT3);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT3);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT3, chainTextColorT3);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT3A, (const GXColorS10*)chainIconGlossColorT3B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT3A, (const GXColorS10*)chainBarColorT3B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT3A, (const GXColorS10*)chainBarColorT3B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT3A, (const GXColorS10*)chainBarColorT3B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT3A, (const GXColorS10*)chainBarColorT3B);
        break;
    }
    case 4: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT4A, (const GXColorS10*)chainIconColorT4B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT4);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT4);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT4);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT4, chainTextColorT4);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT4A, (const GXColorS10*)chainIconGlossColorT4B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT4A, (const GXColorS10*)chainBarColorT4B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT4A, (const GXColorS10*)chainBarColorT4B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT4A, (const GXColorS10*)chainBarColorT4B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT4A, (const GXColorS10*)chainBarColorT4B);
        break;
    }
    case 5: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT5A, (const GXColorS10*)chainIconColorT5B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT5);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT5);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT5);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT5, chainTextColorT5);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT5A, (const GXColorS10*)chainIconGlossColorT5B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT5A, (const GXColorS10*)chainBarColorT5B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT5A, (const GXColorS10*)chainBarColorT5B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT5A, (const GXColorS10*)chainBarColorT5B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT5A, (const GXColorS10*)chainBarColorT5B);
        break;
    }
    case 6: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT6A, (const GXColorS10*)chainIconColorT6B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT6);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT6);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT6);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT6, chainTextColorT6);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT6A, (const GXColorS10*)chainIconGlossColorT6B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT6A, (const GXColorS10*)chainBarColorT6B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT6A, (const GXColorS10*)chainBarColorT6B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT6A, (const GXColorS10*)chainBarColorT6B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT6A, (const GXColorS10*)chainBarColorT6B);
        break;
    }
    case 7: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT7A, (const GXColorS10*)chainIconColorT7B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT7);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT7);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT7);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT7, chainTextColorT7);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT7A, (const GXColorS10*)chainIconGlossColorT7B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT7A, (const GXColorS10*)chainBarColorT7B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT7A, (const GXColorS10*)chainBarColorT7B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT7A, (const GXColorS10*)chainBarColorT7B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT7A, (const GXColorS10*)chainBarColorT7B);
        break;
    }
    case 8: {
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic", (const GXColorS10*)chainIconColorT8A, (const GXColorS10*)chainIconColorT8B);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain00", -1, chainVtxColorT8);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain01", -1, chainVtxColorT8);
        PaneSetVtxColorPairsByName(self->mLayout, "pic_chain02", -1, chainVtxColorT8);
        PaneSetColorFieldPair(self->mLayout, "txt_num", chainTextWhiteT8, chainTextColorT8);
        PaneMatSetTevColorsByName(self->mLayout, "pic_ic_gl", (const GXColorS10*)chainIconGlossColorT8A, (const GXColorS10*)chainIconGlossColorT8B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain00_gl", (const GXColorS10*)chainBarColorT8A, (const GXColorS10*)chainBarColorT8B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain01_gl", (const GXColorS10*)chainBarColorT8A, (const GXColorS10*)chainBarColorT8B);
        PaneMatSetTevColorsByName(self->mLayout, "pic_chain02_gl", (const GXColorS10*)chainBarColorT8A, (const GXColorS10*)chainBarColorT8B);
        PaneMatSetTevColorsByName(self->mLayout, "txt_num_gl", (const GXColorS10*)chainBarColorT8A, (const GXColorS10*)chainBarColorT8B);
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
    chainIconColorT1A[3] = 0;
    chainIconColorT1A[2] = 0;
    chainIconColorT1A[1] = 0;
    chainIconColorT1A[0] = 0x7f;
    chainIconColorT1B[3] = 0xff;
    chainIconColorT1B[2] = 0x69;
    chainIconColorT1B[1] = 0x69;
    chainIconColorT1B[0] = 0xf0;
    chainIconColorT2A[3] = 0;
    chainIconColorT2A[2] = 0x70;
    chainIconColorT2A[1] = 0;
    chainIconColorT2A[0] = 0x32;
    chainIconColorT2B[3] = 0xff;
    chainIconColorT2B[2] = 0xfe;
    chainIconColorT2B[1] = 0x64;
    chainIconColorT2B[0] = 0x8a;
    chainIconColorT3A[3] = 0;
    chainIconColorT3A[2] = 0x0c;
    chainIconColorT3A[1] = 0x3a;
    chainIconColorT3A[0] = 0;
    chainIconColorT3B[3] = 0xff;
    chainIconColorT3B[2] = 0x2a;
    chainIconColorT3B[1] = 0xba;
    chainIconColorT3B[0] = 0x48;
    chainIconColorT4A[3] = 0;
    chainIconColorT4A[2] = 0;
    chainIconColorT4A[1] = 0x4e;
    chainIconColorT4A[0] = 0x5a;
    chainIconColorT4B[3] = 0xff;
    chainIconColorT4B[2] = 0x40;
    chainIconColorT4B[1] = 0xdc;
    chainIconColorT4B[0] = 0xf0;
    chainIconColorT5A[3] = 0;
    chainIconColorT5A[2] = 0;
    chainIconColorT5A[1] = 0x1e;
    chainIconColorT5A[0] = 0x58;
    chainIconColorT5B[3] = 0xff;
    chainIconColorT5B[2] = 0x2d;
    chainIconColorT5B[1] = 0x7f;
    chainIconColorT5B[0] = 0xea;
    chainIconColorT6A[3] = 0;
    chainIconColorT6A[2] = 0xa2;
    chainIconColorT6A[1] = 0x3b;
    chainIconColorT6A[0] = 0x0c;
    chainIconColorT6B[3] = 0xff;
    chainIconColorT6B[2] = 0xf2;
    chainIconColorT6B[1] = 0x97;
    chainIconColorT6B[0] = 0x4d;
    chainIconColorT7A[3] = 0;
    chainIconColorT7A[2] = 0x4c;
    chainIconColorT7A[1] = 0;
    chainIconColorT7A[0] = 0x5a;
    chainIconColorT7B[3] = 0xff;
    chainIconColorT7B[2] = 0xde;
    chainIconColorT7B[1] = 0x72;
    chainIconColorT7B[0] = 0xe6;
    chainIconColorT8A[3] = 0;
    chainIconColorT8A[2] = 0x46;
    chainIconColorT8A[1] = 0x46;
    chainIconColorT8A[0] = 0x46;
    chainIconColorT8B[3] = 0xff;
    chainIconColorT8B[2] = 0xf0;
    chainIconColorT8B[1] = 0xf0;
    chainIconColorT8B[0] = 0xf0;
    chainIconGlossColorT1A[3] = 0;
    chainIconGlossColorT1A[2] = 0;
    chainIconGlossColorT1A[1] = 0;
    chainIconGlossColorT1A[0] = 0x7f;
    chainIconGlossColorT1B[3] = 0xff;
    chainIconGlossColorT1B[2] = 0x69;
    chainIconGlossColorT1B[1] = 0x69;
    chainIconGlossColorT1B[0] = 0xf0;
    chainIconGlossColorT2A[3] = 0;
    chainIconGlossColorT2A[2] = 0x70;
    chainIconGlossColorT2A[1] = 0;
    chainIconGlossColorT2A[0] = 0x32;
    chainIconGlossColorT2B[3] = 0xff;
    chainIconGlossColorT2B[2] = 0xfe;
    chainIconGlossColorT2B[1] = 0x64;
    chainIconGlossColorT2B[0] = 0x8a;
    chainIconGlossColorT3A[3] = 0;
    chainIconGlossColorT3A[2] = 0x0c;
    chainIconGlossColorT3A[1] = 0x3a;
    chainIconGlossColorT3A[0] = 0;
    chainIconGlossColorT3B[3] = 0xff;
    chainIconGlossColorT3B[2] = 0x2a;
    chainIconGlossColorT3B[1] = 0xba;
    chainIconGlossColorT3B[0] = 0x48;
    chainIconGlossColorT4A[3] = 0;
    chainIconGlossColorT4A[2] = 0;
    chainIconGlossColorT4A[1] = 0x4e;
    chainIconGlossColorT4A[0] = 0x5a;
    chainIconGlossColorT4B[3] = 0xff;
    chainIconGlossColorT4B[2] = 0x40;
    chainIconGlossColorT4B[1] = 0xdc;
    chainIconGlossColorT4B[0] = 0xf0;
    chainIconGlossColorT5A[3] = 0;
    chainIconGlossColorT5A[2] = 0;
    chainIconGlossColorT5A[1] = 0x1e;
    chainIconGlossColorT5A[0] = 0x58;
    chainIconGlossColorT5B[3] = 0xff;
    chainIconGlossColorT5B[2] = 0x2d;
    chainIconGlossColorT5B[1] = 0x7f;
    chainIconGlossColorT5B[0] = 0xea;
    chainIconGlossColorT6A[3] = 0;
    chainIconGlossColorT6A[2] = 0xa2;
    chainIconGlossColorT6A[1] = 0x3b;
    chainIconGlossColorT6A[0] = 0x0c;
    chainIconGlossColorT6B[3] = 0xff;
    chainIconGlossColorT6B[2] = 0xf2;
    chainIconGlossColorT6B[1] = 0x97;
    chainIconGlossColorT6B[0] = 0x4d;
    chainIconGlossColorT7A[3] = 0;
    chainIconGlossColorT7A[2] = 0x4c;
    chainIconGlossColorT7A[1] = 0;
    chainIconGlossColorT7A[0] = 0x5a;
    chainIconGlossColorT7B[3] = 0xff;
    chainIconGlossColorT7B[2] = 0xde;
    chainIconGlossColorT7B[1] = 0x72;
    chainIconGlossColorT7B[0] = 0xe6;
    chainIconGlossColorT8A[3] = 0;
    chainIconGlossColorT8A[2] = 0x46;
    chainIconGlossColorT8A[1] = 0x46;
    chainIconGlossColorT8A[0] = 0x46;
    chainIconGlossColorT8B[3] = 0xff;
    chainIconGlossColorT8B[2] = 0xf0;
    chainIconGlossColorT8B[1] = 0xf0;
    chainIconGlossColorT8B[0] = 0xf0;
    chainBarColorT1A[3] = 0;
    chainBarColorT1A[2] = 0;
    chainBarColorT1A[1] = 0;
    chainBarColorT1A[0] = 0x7f;
    chainBarColorT1B[3] = 0xff;
    chainBarColorT1B[2] = 0xff;
    chainBarColorT1B[1] = 0xff;
    chainBarColorT1B[0] = 0xff;
    chainBarColorT2A[3] = 0;
    chainBarColorT2A[2] = 0x70;
    chainBarColorT2A[1] = 0;
    chainBarColorT2A[0] = 0x32;
    chainBarColorT2B[3] = 0xff;
    chainBarColorT2B[2] = 0xff;
    chainBarColorT2B[1] = 0xff;
    chainBarColorT2B[0] = 0xff;
    chainBarColorT3A[3] = 0;
    chainBarColorT3A[2] = 0x0c;
    chainBarColorT3A[1] = 0x3a;
    chainBarColorT3A[0] = 0;
    chainBarColorT3B[3] = 0xff;
    chainBarColorT3B[2] = 0xff;
    chainBarColorT3B[1] = 0xff;
    chainBarColorT3B[0] = 0xff;
    chainBarColorT4A[3] = 0;
    chainBarColorT4A[2] = 0;
    chainBarColorT4A[1] = 0x4e;
    chainBarColorT4A[0] = 0x5a;
    chainBarColorT4B[3] = 0xff;
    chainBarColorT4B[2] = 0xff;
    chainBarColorT4B[1] = 0xff;
    chainBarColorT4B[0] = 0xff;
    chainBarColorT5A[3] = 0;
    chainBarColorT5A[2] = 0;
    chainBarColorT5A[1] = 0x1e;
    chainBarColorT5A[0] = 0x58;
    chainBarColorT5B[3] = 0xff;
    chainBarColorT5B[2] = 0xff;
    chainBarColorT5B[1] = 0xff;
    chainBarColorT5B[0] = 0xff;
    chainBarColorT6A[3] = 0;
    chainBarColorT6A[2] = 0xa2;
    chainBarColorT6A[1] = 0x3b;
    chainBarColorT6A[0] = 0x0c;
    chainBarColorT6B[3] = 0xff;
    chainBarColorT6B[2] = 0xff;
    chainBarColorT6B[1] = 0xff;
    chainBarColorT6B[0] = 0xff;
    chainBarColorT7A[3] = 0;
    chainBarColorT7A[2] = 0x4c;
    chainBarColorT7A[1] = 0;
    chainBarColorT7A[0] = 0x5a;
    chainBarColorT7B[3] = 0xff;
    chainBarColorT7B[2] = 0xff;
    chainBarColorT7B[1] = 0xff;
    chainBarColorT7B[0] = 0xff;
    chainBarColorT8A[3] = 0;
    chainBarColorT8A[2] = 0x46;
    chainBarColorT8A[1] = 0x46;
    chainBarColorT8A[0] = 0x46;
    chainBarColorT8B[3] = 0xff;
    chainBarColorT8B[2] = 0xff;
    chainBarColorT8B[1] = 0xff;
    chainBarColorT8B[0] = 0xff;
}

// Battle-chain singleton instance (retail .sbss head slot;
// the 48 colour records follow it, matching the split range
// 0x80664A60-0x80664BE8).
CMenuBattleChain* CMenuBattleChain::sInstance;
// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Battle-chain pane colour records (zero-init .sbss; values filled at startup
// by sinit_802AAF24). Pairs are consumed by applyChainColorScheme per chain type.
u16 chainIconColorT1A[4];
u16 chainIconColorT1B[4];
u16 chainIconColorT2A[4];
u16 chainIconColorT2B[4];
u16 chainIconColorT3A[4];
u16 chainIconColorT3B[4];
u16 chainIconColorT4A[4];
u16 chainIconColorT4B[4];
u16 chainIconColorT5A[4];
u16 chainIconColorT5B[4];
u16 chainIconColorT6A[4];
u16 chainIconColorT6B[4];
u16 chainIconColorT7A[4];
u16 chainIconColorT7B[4];
u16 chainIconColorT8A[4];
u16 chainIconColorT8B[4];
u16 chainIconGlossColorT1A[4];
u16 chainIconGlossColorT1B[4];
u16 chainIconGlossColorT2A[4];
u16 chainIconGlossColorT2B[4];
u16 chainIconGlossColorT3A[4];
u16 chainIconGlossColorT3B[4];
u16 chainIconGlossColorT4A[4];
u16 chainIconGlossColorT4B[4];
u16 chainIconGlossColorT5A[4];
u16 chainIconGlossColorT5B[4];
u16 chainIconGlossColorT6A[4];
u16 chainIconGlossColorT6B[4];
u16 chainIconGlossColorT7A[4];
u16 chainIconGlossColorT7B[4];
u16 chainIconGlossColorT8A[4];
u16 chainIconGlossColorT8B[4];
u16 chainBarColorT1A[4];
u16 chainBarColorT1B[4];
u16 chainBarColorT2A[4];
u16 chainBarColorT2B[4];
u16 chainBarColorT3A[4];
u16 chainBarColorT3B[4];
u16 chainBarColorT4A[4];
u16 chainBarColorT4B[4];
u16 chainBarColorT5A[4];
u16 chainBarColorT5B[4];
u16 chainBarColorT6A[4];
u16 chainBarColorT6B[4];
u16 chainBarColorT7A[4];
u16 chainBarColorT7B[4];
u16 chainBarColorT8A[4];
u16 chainBarColorT8B[4];

// (The resource-name block above is the 0x144-byte literal at retail
// .rodata:0x8051088C; the compiler emits it into the string pool.)

// Anim-frame constants (retail .sdata2 pool lbl_eu_80668CB0/B4: 0.0f frame
// reset, 1.0f "finished" threshold). Values verified against the retail
// object (build/us/obj/kyoshin/menu/CMenuBattleChain.o .sdata2).
extern const f32 lbl_eu_80668CB0 = 0.0f;
extern const f32 lbl_eu_80668CB4 = 1.0f;

// Chain-type colour/position constants (retail .sdata chainVtxColorT1..DCC).
// Values verified against the retail object
// (build/us/obj/kyoshin/menu/CMenuBattleChain.o .sdata): slots 0x70-0x8C
// are the eight per-type colours, slots 0x90-0xCC repeat them as
// (white, colour) pairs consumed by PaneSetColorFieldPair.
u32 chainVtxColorT1 = 0xF06969FF;
u32 chainVtxColorT2 = 0x8A64FEFF;
u32 chainVtxColorT3 = 0x48BA2AFF;
u32 chainVtxColorT4 = 0xF0DC40FF;
u32 chainVtxColorT5 = 0xEA7F2DFF;
u32 chainVtxColorT6 = 0x4D97F2FF;
u32 chainVtxColorT7 = 0xE672DEFF;
u32 chainVtxColorT8 = 0xA0A0A0FF;
u32 chainTextWhiteT1 = 0xF0F0F0FF;
u32 chainTextColorT1 = 0xF06969FF;
u32 chainTextWhiteT2 = 0xF0F0F0FF;
u32 chainTextColorT2 = 0x8A64FEFF;
u32 chainTextWhiteT3 = 0xF0F0F0FF;
u32 chainTextColorT3 = 0x48BA2AFF;
u32 chainTextWhiteT4 = 0xF0F0F0FF;
u32 chainTextColorT4 = 0xF0DC40FF;
u32 chainTextWhiteT5 = 0xF0F0F0FF;
u32 chainTextColorT5 = 0xEA7F2DFF;
u32 chainTextWhiteT6 = 0xF0F0F0FF;
u32 chainTextColorT6 = 0x4D97F2FF;
u32 chainTextWhiteT7 = 0xF0F0F0FF;
u32 chainTextColorT7 = 0xE672DEFF;
u32 chainTextWhiteT8 = 0xF0F0F0FF;
u32 chainTextColorT8 = 0xA0A0A0FF;


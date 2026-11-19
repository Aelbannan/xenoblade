// Catalog TU for kyoshin/menu/CMenuBattleCommu
// Battle commu menu screen process reconstruction.

#include "kyoshin/menu/CMenuBattleCommu.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceSC.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <cstdio>
#include <nw4r/lyt.h>
#include <revolution/GX.h>

// Forward declarations for scaffold thunk references (member symbols emitted
// by the definitions in this TU). extern "C" keeps the `__dt__`-prefixed
// names unmangled so the thunk tail-call relocs match retail.
extern "C" void __dt__16CMenuBattleCommuFv(void*, int);
extern "C" void cbRenderBefore__16CMenuBattleCommuFv(void*);

CMenuBattleCommu* spMenuBattleCommuInstance = 0;

// ---------------------------------------------------------------------------
// Constructor
// CProcess-style region by hand (temp vtable, run-time ptmf hooks, dispatch
// sub-vtables, state fields) and constructs the embedded MEM2 layout region.
// r3 = this, r4 = owning scene (stored at +0x60).
// ---------------------------------------------------------------------------
extern "C" CMenuBattleCommu* __ct__CMenuBattleCommu(CMenuBattleCommu* self, CScn* scene) {
    __ct__8CProcessFv((CProcess*)self);

    self->mVtable10 = (u32)lbl_eu_8052C1C0;

    // Post-increment pointer form: MWCC folds the base @l into a single
    // `lwzu` for the first load and keeps the base register for the rest;
    // the second triple re-reads the same elements through the folded
    // negative offsets (retail re-loads ptmf[0..2] after the first stores).
    // q-locals declared first so MWCC colors q0 into r7 and the ptmf base
    // into r8 (retail allocation; declaration order drives Chaitin).
    u32 q0, q1, q2;
    const u32* ptmf = __ptmf_null;
    u32 p0, p1, p2;
    p0 = *ptmf++;
    p1 = *ptmf++;
    self->mPtMf3C[1] = p1;
    self->mPtMf3C[0] = p0;
    p2 = *ptmf++;
    self->mPtMf3C[2] = p2;
    q0 = ptmf[-3];
    q1 = ptmf[-2];
    self->mPtMf48[1] = q1;
    self->mPtMf48[0] = q0;
    q2 = ptmf[-1];
    self->mPtMf48[2] = q2;

    self->mField54 = 0;
    self->mField55 = 0;

    // Retail re-stores +0x10 with the composite vtable after the temp
    // (CProcess) vtable; +0x24/+0xac are the IWorkEvent / IScnRender
    // dispatch slots inside that cluster.
    self->mVtable10 = (u32)lbl_eu_80533638;
    self->mWorkEventVt = (u32)lbl_eu_80533638 + 0x24;
    self->mScnRenderVt = (u32)lbl_eu_80533638 + 0xac;
    self->mScene = scene;

    __ct__17UnkClass_8045F564Fv(&self->mMemRegion[0]);

    self->mField74 = 0;
    self->mField78 = 0;
    self->mField7C = 0;
    self->mField80 = 0;
    self->mField84 = 0;
    self->mField88 = 0;
    self->mField8C = 0;
    self->mField90 = 0;

    return self;
}

// ---------------------------------------------------------------------------
// Destructor. The embedded MEM2 region runs its member dtor (flag -1), then
// the CProcess subobject is destroyed OOL (flag 0) under the nested double
// null-check (MWCC keeps one beq per source-level if). The deleting wrapper
// (null check + conditional operator delete) is auto-generated for the
// virtual dtor.
// ---------------------------------------------------------------------------
CMenuBattleCommu::~CMenuBattleCommu() {
    __dt__17UnkClass_8045F564Fv(&mMemRegion[0], -1);
    if (this) {
        if (this) {
            __dt__8CProcessFv(reinterpret_cast<CProcess*>(this), 0);
        }
    }
}

// ---------------------------------------------------------------------------
// Init: allocate the MEM2 layout region, build the layout + six animation
// layers from the shared battle ARC, bind the font handle to the five info
// panes, enable the idle animation, then install the language-dependent tag
// text and register as an IScnRender callback (layer 10).
// ---------------------------------------------------------------------------
void CMenuBattleCommu::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->createRegion(
        handle, 0x14000, "MenuBattleCommu", 0);
    // Scoped region guard - destructor releases the region when Init finishes.
    Class_8045F858 regionGuard(reinterpret_cast<UnkClass_8045F564*>(&mMemRegion));

    nw4r::lyt::ArcResourceAccessor* accessor = CUICfManager_getArcResourceAccessor();
    buildLayout(reinterpret_cast<nw4r::lyt::Layout**>(&mField74), accessor,
                  "mf71_battle95_communi.brlyt");

    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(mField74), &mField78,
                  accessor, "mf71_battle95_communi_in.brlan");
    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(mField74), &mField7C,
                  accessor, "mf71_battle95_communi_ripple_in.brlan");
    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(mField74), &mField80,
                  accessor, "mf71_battle95_communi_ripple_roop.brlan");
    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(mField74), &mField84,
                  accessor, "mf71_battle95_communi_ripple_out.brlan");
    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(mField74), &mField88,
                  accessor, "mf71_battle95_communi_out.brlan");
    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(mField74), &mField8C,
                  accessor, "mf71_battle95_communi_bright_out.brlan");

    CBattleCommuFontObj* font = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1);
    u32 fontHandle = font->getFontHandle();

    setLayoutTextBoxFont(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_l", fontHandle);
    setLayoutTextBoxFont(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_r", fontHandle);
    setLayoutTextBoxFont(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_o", fontHandle);
    setLayoutTextBoxFont(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_u", fontHandle);
    setLayoutTextBoxFont(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess", fontHandle);

    mField74->SetAnimationEnable(mField7C, false);
    mField74->SetAnimationEnable(mField80, false);
    mField74->SetAnimationEnable(mField84, false);
    mField74->SetAnimationEnable(mField88, false);
    mField74->SetAnimationEnable(mField8C, false);
    mField74->SetAnimationEnable(mField78, true);
    mField78->SetFrame(lbl_eu_80667DE0);
    mField74->Animate();

    LayoutSetTextBoxFmtValue(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_l", "", 0);
    LayoutSetTextBoxFmtValue(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_r", "", 0);
    LayoutSetTextBoxFmtValue(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_o", "", 0);
    LayoutSetTextBoxFmtValue(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess_u", "", 0);
    LayoutSetTextBoxFmtValue(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                  "txt_mess", "", 0);

    // Language/party-dependent tag text: the 4CC key 0x74696D67 ("timg")
    // selects the tag resource; when found it is bound to the pane names at
    // 0x158 / 0x182+0x18f.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        char* res = static_cast<char*>(
            CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf71_btl90_chn_btn_c00.tpl", 0));
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mField74, "pic_button_r", (TPLPalette*)res);
        }
        res = static_cast<char*>(
            CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf71_btl90_chn_btngl_c00.tpl", 0));
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mField74, "pic_line_r00", (TPLPalette*)res);
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mField74, "pic_line_r01", (TPLPalette*)res);
        }
    } else {
        char* res = static_cast<char*>(
            CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf71_btl90_chn_btn_r00.tpl", 0));
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mField74, "pic_button_r", (TPLPalette*)res);
        }
        res = static_cast<char*>(
            CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf71_btl90_chn_btngl_r00.tpl", 0));
        if (res != 0) {
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mField74, "pic_line_r00", (TPLPalette*)res);
            PaneSetTexPaletteByName((nw4r::lyt::Layout*)mField74, "pic_line_r01", (TPLPalette*)res);
        }
    }

    // Register the IScnRender dispatch slot (+0x5C) as the draw callback.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = reinterpret_cast<IScnRender*>(&mScnRenderVt);
    }
    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, render, 10, 0);
}

// ---------------------------------------------------------------------------
// Tear the battle commu screen down: wait for the draw callback to finish,
// remove the render callback from the owning scene, release the +0x74 layout
// object, clear the scratch region and null the singleton.
// ---------------------------------------------------------------------------
void CMenuBattleCommu::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi +0x5c.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = reinterpret_cast<IScnRender*>(&mScnRenderVt);
    }
    removeRenderCB__4CScnFP10IScnRender(mScene, render);

    if (mField74 != 0) {
        if (mField74 != 0) {
            mField74->release(1);
        }
        mField74 = 0;
    }

    reinterpret_cast<UnkClass_8045F564*>(&mMemRegion)->deleteRegion();
    spMenuBattleCommuInstance = 0;
}

void CMenuBattleCommu::Move() {}

// ---------------------------------------------------------------------------
// Draw callback: guard against task/event/busy states, verify the player is
// in battle, then draw the layout with Z-testing disabled.
// ---------------------------------------------------------------------------
void CMenuBattleCommu::cbRenderBefore() {
    CTaskGame::getInstance();
    // OR-combined guard: MWCC emits the second disjunct as the unreduced
    // `beq cont; b exit` pair (retail shape, MWCC_CASES RFCOMM OR-guard).
    if (CTaskGame::isFlag01Set() != 0 ||
        (gmPauseFlags & 0x200000) != 0) {
        return;
    }
draw:
    if (IsMenuState621F0() == 0) {
        return;
    }
    if (gmGameFlags & 0xafa40000) {
        return;
    }
    if (mField90 == 0) {
        return;
    }

    CBattleCommuPlayer* player =
        reinterpret_cast<CBattleCommuPlayer*>(cf::CfGameManager::getPlayer(0));
    CBattleCommuActor2* actor2 = MapFx_ResolveOwner(player);
    if (actor2 != 0) {
        u32 id = *actor2->mSub04->getActorId();
        if (testActorStatus(actor2, &id, 0x803) != 0) {
            CBattleCommuBattleObj* battle = reinterpret_cast<CBattleCommuBattleObj*>(
                __dynamic_cast(player->mSub38, 0, &lbl_eu_80661C60,
                               &lbl_eu_80661BE8, 0));
            if (battle->mState36C == 0) {
                return;
            }
        }
    }

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);  // disable Z compare, always pass
    {
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        drawLayout(reinterpret_cast<nw4r::lyt::Layout*>(mField74),
                      &drawInfo, 0, 1);
    }
}

// ---------------------------------------------------------------------------
// Factory: if the singleton already exists, return 0; otherwise allocate
// 0x94 bytes from work memory, construct and register the screen under
// `parent`. The singleton store + Regist run even when the allocation failed
// (r3 stays NULL).
// ---------------------------------------------------------------------------
CMenuBattleCommu* battleCommuCreateRegister(CProcess* parent, CScn* scene) {
    if (spMenuBattleCommuInstance != 0) {
        return 0;
    }

    CMenuBattleCommu* obj = (CMenuBattleCommu*)mtl::MemManager::allocate(
        0x94, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // Reassign from the ctor return so obj stays in volatile r3 (retail
        // never spills it to a callee-saved register).
        obj = __ct__CMenuBattleCommu(obj, scene);
    }
    spMenuBattleCommuInstance = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return spMenuBattleCommuInstance;
}

// ---------------------------------------------------------------------------
// Publish the battle commu screen: once the singleton is up and not already
// in state 6, play through the six animation layers (0x78..0x88 disabled,
// then 0x8C enabled), reset the 0x8C animation frame, mark state 6 and play
// the "open" cue.
// ---------------------------------------------------------------------------
void battleCommuDisableAnims() {
    CMenuBattleCommu* self = spMenuBattleCommuInstance;
    if (self == 0) {
        return;
    }
    if (self->mField90 == 6) {
        return;
    }

    self->mField74->SetAnimationEnable(self->mField78, false);
    self->mField74->SetAnimationEnable(self->mField7C, false);
    self->mField74->SetAnimationEnable(self->mField80, false);
    self->mField74->SetAnimationEnable(self->mField84, false);
    self->mField74->SetAnimationEnable(self->mField88, false);
    self->mField74->SetAnimationEnable(self->mField8C, true);
    self->mField8C->SetFrame(lbl_eu_80667DE0);
    // The virtual calls above may alias the global, so re-read the singleton
    // for the final store (retail reloads it).
    spMenuBattleCommuInstance->mField90 = 6;
    playUISound(0x3f);
}

// Singleton active check: true when the instance exists and its +0x90 state
// word is set.
bool battleCommuIsActive() {
    bool ret = false;
    if (spMenuBattleCommuInstance != 0 && spMenuBattleCommuInstance->mField90 != 0) {
        ret = true;
    }
    return ret;
}

// ---------------------------------------------------------------------------
// Move state 0: show the battle commu. When the player is in battle, resolve
// the actor id and write the language-dependent tag text into the five info
// panes, then enable the entry animation and move to state 1.
// ---------------------------------------------------------------------------
void showBattleCommu(CMenuBattleCommu* self) {
    CBattleCommuPlayer* player =
        reinterpret_cast<CBattleCommuPlayer*>(cf::CfGameManager::getPlayer(0));
    CBattleCommuActor2* actor2 = MapFx_ResolveOwner(player);
    if (actor2 == 0) {
        return;
    }
    u32 id = *actor2->mSub04->getActorId();
    if (testActorStatus(actor2, &id, 0x803) == 0) {
        return;
    }

    // Reuse the dead player web so MWCC recycles its register (r28), and
    // initialize handled first (r0), count next (r5), state last (r4).
    int handled = 0;
    player = reinterpret_cast<CBattleCommuPlayer*>(player->mSub38);
    s32 count =
        reinterpret_cast<CBattleCommuBattleObj*>(player)->mCount370;
    if (count > 1) {
        count = 1;
    }
    switch (reinterpret_cast<CBattleCommuBattleObj*>(player)->mState36C) {
    case 1: {
        char buf[0x40];
        char* name = BdatTouchStringCell("MNU_kyeassign", "help",
                                   count + 0x1b);
        sprintf(buf, "%s", name);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_l", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_r", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_o", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_u", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess", buf, 0);
        handled = 1;
        break;
    }
    case 2: {
        char buf[0x40];
        char* name = BdatTouchStringCell("MNU_kyeassign", "help",
                                   count + 0x1c);
        sprintf(buf, "%s", name);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_l", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_r", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_o", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_u", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess", buf, 0);
        handled = 1;
        break;
    }
    case 3: {
        char buf[0x40];
        char* name = BdatTouchStringCell("MNU_kyeassign", "help",
                                   count + 0x1d);
        sprintf(buf, "%s", name);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_l", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_r", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_o", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_u", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess", buf, 0);
        handled = 1;
        break;
    }
    case 4: {
        CBattleCommuSub374* sub =
            reinterpret_cast<CBattleCommuBattleObj*>(player)->mSub374;
        if (count == 1) {
            char buf[0x40];
            u8 value = (u8)sub->mId3F28;
            char* name = BdatTouchStringCell("MNU_kyeassign", "help",
                                       0x1f);
            if (value != 0) {
                u8 lang = CDeviceSC::getLanguage();
                int jp = 0;
                if (lang == 2 || lang == 0) {
                    jp = 1;
                }
                if (jp != 0) {
                    sprintf(buf, "%s %s",
                            BdatGetPtrDirect(lbl_eu_80664090, "name",
                                          value),
                            name);
                } else {
                    sprintf(buf, "%s %s", name,
                            BdatGetPtrDirect(lbl_eu_80664090, "name",
                                          value));
                }
            } else {
                sprintf(buf, "%s", name);
            }
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_l", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_r", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_o", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_u", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess", buf, 0);
        } else {
            char buf[0x40];
            u8 value = (u8)sub->mId3F28;
            char* name = BdatTouchStringCell("MNU_kyeassign", "help",
                                       0x20);
            if (value != 0) {
                u8 lang = CDeviceSC::getLanguage();
                int jp = 0;
                if (lang == 2 || lang == 0) {
                    jp = 1;
                }
                if (jp != 0) {
                    sprintf(buf, "%s %s",
                            BdatGetPtrDirect(lbl_eu_80664090, "name",
                                          value),
                            name);
                } else {
                    sprintf(buf, "%s %s", name,
                            BdatGetPtrDirect(lbl_eu_80664090, "name",
                                          value));
                }
            } else {
                sprintf(buf, "%s", name);
            }
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_l", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_r", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_o", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess_u", buf, 0);
            LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                          "txt_mess", buf, 0);
        }
        handled = 1;
        break;
    }
    case 7: {
        char buf[0x40];
        char* name = BdatTouchStringCell("MNU_kyeassign", "help",
                                   count + 0x25);
        sprintf(buf, "%s", name);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_l", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_r", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_o", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_u", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess", buf, 0);
        handled = 1;
        break;
    }
    case 8: {
        char buf[0x40];
        char* name = BdatTouchStringCell("MNU_kyeassign", "help",
                                   count + 0x26);
        sprintf(buf, "%s", name);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_l", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_r", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_o", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess_u", buf, 0);
        LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField74),
                      "txt_mess", buf, 0);
        handled = 1;
        break;
    }
    default:
        break;
    }
    if (handled == 0) {
        return;
    }

    // Enable the entry animation and move to state 1.
    self->mField74->SetAnimationEnable(self->mField7C, false);
    self->mField74->SetAnimationEnable(self->mField80, false);
    self->mField74->SetAnimationEnable(self->mField84, false);
    self->mField74->SetAnimationEnable(self->mField88, false);
    self->mField74->SetAnimationEnable(self->mField8C, false);
    self->mField74->SetAnimationEnable(self->mField78, true);
    self->mField78->SetFrame(lbl_eu_80667DE0);
    playUISound(0x60);
    self->mField90 = 1;
}

// ---------------------------------------------------------------------------
// Move state 3: once the 0x80 animation has run its course, transition to
// state 4 unless the player is in a special battle state (1,2,3,4,7,8) that
// keeps the commu up.
// ---------------------------------------------------------------------------
void battleCommuAdvanceAnim80(CMenuBattleCommu* self) {
    if (advanceAnimTransform(self->mField80, lbl_eu_80667DE4) == 0) {
        return;
    }

    CBattleCommuPlayer* player =
        reinterpret_cast<CBattleCommuPlayer*>(cf::CfGameManager::getPlayer(0));
    CBattleCommuActor2* actor2 = MapFx_ResolveOwner(player);
    if (actor2 != 0) {
        u32 id = *actor2->mSub04->getActorId();
        if (testActorStatus(actor2, &id, 0x803) != 0) {
            int isSpecial = 0;
            CBattleCommuBattleObj* battle = reinterpret_cast<CBattleCommuBattleObj*>(
                __dynamic_cast(player->mSub38, 0, &lbl_eu_80661C60,
                               &lbl_eu_80661BE8, 0));
            s32 state = battle->mState36C;
            switch (state) {
            case 1: isSpecial = 1; break;
            case 2: isSpecial = 1; break;
            case 3: isSpecial = 1; break;
            case 4: isSpecial = 1; break;
            case 7: isSpecial = 1; break;
            case 8: isSpecial = 1; break;
            default: break;
            }
            if (isSpecial != 0) {
                return;
            }
            // Normal path: switch to the state-4 animation set.
            self->mField74->SetAnimationEnable(self->mField88, false);
            self->mField74->SetAnimationEnable(self->mField8C, false);
            self->mField74->SetAnimationEnable(self->mField78, false);
            self->mField74->SetAnimationEnable(self->mField7C, false);
            self->mField74->SetAnimationEnable(self->mField80, false);
            self->mField74->SetAnimationEnable(self->mField84, true);
            self->mField84->SetFrame(lbl_eu_80667DE0);
            self->mField90 = 4;
            return;
        }
    }
    // No valid actor (or not in battle): same transition.
    self->mField74->SetAnimationEnable(self->mField88, false);
    self->mField74->SetAnimationEnable(self->mField8C, false);
    self->mField74->SetAnimationEnable(self->mField78, false);
    self->mField74->SetAnimationEnable(self->mField7C, false);
    self->mField74->SetAnimationEnable(self->mField80, false);
    self->mField74->SetAnimationEnable(self->mField84, true);
    self->mField84->SetFrame(lbl_eu_80667DE0);
    self->mField90 = 4;
}

void battleCommuDtorThunk58(void* self) { ((void(*)(void*))__dt__16CMenuBattleCommuFv)((char*)self - 0x58); }

void battleCommuRenderBeforeThunk(void* self) { ((void(*)(void*))cbRenderBefore__16CMenuBattleCommuFv)((char*)self - 0x5c); }

void battleCommuDtorThunk5C(void* self) { ((void(*)(void*))__dt__16CMenuBattleCommuFv)((char*)self - 0x5c); }

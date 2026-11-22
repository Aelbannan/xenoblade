#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/menu/CMenuBattlePlayerState.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

#undef isSceneLoading__Q22cf13CfGameManagerFv
// code_80135FDC.hpp declares ml::epsilon as u32 (line 188);
// CfObjectMove.hpp (via the CBattleManager.hpp include above) declares it
// const float. This TU uses neither copy.

#define isSceneLoading__Q22cf13CfGameManagerFv menuBpsCfGameMgrCond5
// CfObjectModel.hpp (via CfObjectActor.hpp) declares this symbol as ml::CVec3;
// code_80135FDC.hpp re-types it as nw4r::math::VEC3 -> MWCC 10563. This TU
// never touches the zero vector, so rename the header-local decl away.
#define zero__Q22ml5CVec3 menuBpsZeroVecDecl5
#include "kyoshin/code_80135FDC.hpp"

#undef isSceneLoading__Q22cf13CfGameManagerFv
#undef zero__Q22ml5CVec3
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/work/CProcess.hpp"
#include "kyoshin/menu/CMenuZeal.hpp"
#include <stdio.h>
#include "decomp.h"
#include "functions.hpp"
#include <nw4r/math.h>
#include <revolution/GX.h>
#include "kyoshin/cf/chain/CChainImports.hpp"  // TU imports formerly reached via the fat CBattleManager/chain headers
#include "kyoshin/CfEnumListHolder.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Batch 2026-07-14g: menu-bps-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14h: menu-bps-move owns Move exclusively; do not touch
// Term / cbRenderBefore.
// Batch 2026-07-14k: menu-bps-ctor lives in CMenuBattlePlayerState_ct.cpp (-O4,s).

void BpsStateClearSlot(CMenuBattlePlayerStateSlot*);
extern "C" void* __dt__8010B444(CMenuBattlePlayerStateSlot*, s16);
extern "C" void* __dt__8010B444(CMenuBattlePlayerStateSlot* self, s16 del) {
    if (self && del > 0)
        operator delete(self);
    return self;
}
extern "C" {
extern CMenuBattlePlayerState* spBpsState;
extern u32 gmPauseFlags;
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void __dt__8CProcessFv(void* self, int flag);
void* __dt__22CMenuBattlePlayerStateFv(CMenuBattlePlayerState* self, int deleteFlag);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);


void __construct_array(void* ptr, void* ctor, void* dtor, u32 size, u32 n);
// findObjB28ById is declared (extern "C", s32) by CAIAction.hpp, included
// transitively via CfObjectActor.hpp -> CBattleManager.hpp.
// (memberized: BpsStateUpdateSlotArts -> CMenuBattlePlayerState::BpsStateUpdateSlotArts; declaration moved into class CMenuBattlePlayerState)
//
//
// (memberized: BpsStateActivateSlot -> CMenuBattlePlayerState::BpsStateActivateSlot; declaration moved into class CMenuBattlePlayerState)
//
// (memberized: BpsStateRefreshSlot -> CMenuBattlePlayerState::BpsStateRefreshSlot; declaration moved into class CMenuBattlePlayerState)
//
// Unmangled retail name (extern "C" in code_80135FDC.hpp).
extern char lbl_eu_804FD720[];
extern const f32 lbl_eu_80666F98; // 110.0f
extern const f32 lbl_eu_80666F9C; // 86.0f
extern const f32 lbl_eu_80666FA0; // -178.0f
}

extern "C" {
extern nw4r::lyt::ArcResourceAccessor* UIBattleGetWordEC();
}

extern void playUISound(u32);

GXColorS10 sBpsPaneColorWhite;
GXColorS10 sBpsPaneColorGold;
GXColorS10 sBpsPaneColorGray;

// Direct-store view
// SetTranslate here; MWCC refuses to inline the header body inside Move.
struct MenuBpsPaneXlate {
    u8 pad00[0x2c];
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
};





// Pane mTranslate views: retail inlines SetTranslate here as raw-word x/z
// copies plus a float Y update (out-of-line VEC3/SetTranslate calls do not
// match).
struct MenuBpsPaneWords {
    u8 pad00[0x2c];
    u32 field_2c;
    u32 field_30;
    u32 field_34;
};
struct MenuBpsPaneYf {
    u8 pad00[0x30];
    f32 field_30;
};

// Tail view of CMenuBattlePlayerStateSlot starting at +0x74: Init stages one
// of these on the stack per slot (head fields + a zero-filled 0x174 buffer)
// and block-assigns it over the slot tail.
struct MenuBpsSlotInit {
    void* unk74;                     // -> slot.unk74
    nw4r::lyt::Layout* unk78;        // -> slot.unk78
    nw4r::lyt::AnimTransform* unk7C; // -> slot.unk7C
    u8 unk80;                        // -> slot.unk80
    u8 pad81[3];
    void* unk84;                     // -> slot.unk84
    void* unk88;
    void* unk8C;
    u8 pad90[0x190 - 0x1C];          // -> slot.pad90..unk204
};

// Length-first decl so inlined size homes match retail (length r4, cur r3, end r5).
// Do not change shared reslist::size() - that regresses other units.
static inline u32 menuBpsActorListSize(const reslist<cf::CfObjectActor*>* list) {
    u32 length = 0;
    _reslist_node<cf::CfObjectActor*>* endNode = list->mStartNodePtr;
    _reslist_node<cf::CfObjectActor*>* curNode = list->mStartNodePtr->mNext;
    while (curNode != endNode) {
        length += 1;
        curNode = curNode->mNext;
    }
    return length;
}

void CMenuBattlePlayerState::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    nw4r::lyt::ArcResourceAccessor* accessor = UIBattleGetWordEC();
    unk64.createRegion(reinterpret_cast<int>(accessor), 0xE00,
                       lbl_eu_804FD720 + 0xF7, 0);

    {
        Class_8045F858 regionGuard(&unk64);
        char* tbl = lbl_eu_804FD720;
        // Loop invariants kept live across the whole pass (retail pins them
        // to nonvolatile f28-f31 / r24-r26).
        const f32 zeroF = 0.0f;
        const f32 yStep = lbl_eu_80666F9C;
        GXColorS10* colA = &sBpsPaneColorWhite;
        GXColorS10* colB = &sBpsPaneColorGold;
        GXColorS10* colC = &sBpsPaneColorGray;

        for (u8 i = 0; i < 3; i++) {
            CMenuBattlePlayerStateSlot& slot = mSlots[i];

            accessor = CUICfManager_getArcResourceAccessor();
            buildLayout(&slot.unk00, accessor, tbl + 0x10E);

            accessor = CUICfManager_getArcResourceAccessor();
            bindLayoutAnimTransform(slot.unk00, &slot.unk04, accessor, tbl + 0x12E);

            slot.unk00->SetAnimationEnable(slot.unk04, true);
            slot.unk04->SetFrame(0.0f);
            slot.unk00->Animate(0);

            accessor = CUICfManager_getArcResourceAccessor();
            buildLayout(&slot.unk08, accessor, tbl + 0x151);

            accessor = CUICfManager_getArcResourceAccessor();
            bindLayoutAnimTransform(slot.unk08, &slot.unk0C, accessor, tbl + 0x174);
            bindLayoutAnimTransform(slot.unk08, &slot.unk10, accessor, tbl + 0x19A);
            bindLayoutAnimTransform(slot.unk08, &slot.unk14, accessor, tbl + 0x1C5);

            slot.unk08->SetAnimationEnable(slot.unk10, false);
            slot.unk08->SetAnimationEnable(slot.unk14, false);
            slot.unk08->SetAnimationEnable(slot.unk0C, true);
            slot.unk0C->SetFrame(0.0f);
            slot.unk08->Animate(0);

            accessor = CUICfManager_getArcResourceAccessor();
            buildLayout(&slot.unk18, accessor, tbl + 0x1F0);

            accessor = CUICfManager_getArcResourceAccessor();
            bindLayoutAnimTransform(slot.unk18, &slot.unk1C, accessor, tbl + 0x210);

            {
                nw4r::lyt::Pane* rootPane = slot.unk18->GetRootPane();
                u32 fontHandle = (u32)CUICfManager_getPackedFont9C();
                ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontHandle);
            }
            {
                u32 fontAccessor = (u32)CUICfManager_getPackedFontD8();
                setLayoutTextBoxFont(slot.unk18, tbl + 0x233, fontAccessor);
                setLayoutTextBoxFont(slot.unk18, tbl + 0x23C, fontAccessor);
            }

            {
                // Bounding-rect query; the four halfwords fan out into the
                // shared color tables (A gets all four, B/C only the last).
                GXColorS10 rect = PaneMatGetS16QuadByName(slot.unk18, tbl + 0x245, 1);
                s16 v0 = rect.r;
                s16 v1 = rect.g;
                s16 v2 = rect.b;
                s16 v3 = rect.a;
                colA->r = v0;
                colA->g = v1;
                colA->b = v2;
                colA->a = v3;
                colB->a = v3;
                colC->a = v3;
            }

            {
                nw4r::lyt::Pane* rp = slot.unk18->GetRootPane();
                slot.unk44 = rp->FindPaneByName(tbl + 0x233, true);
                slot.unk48 = rp->FindPaneByName(tbl + 0x23C, true);
                slot.unk4C = rp->FindPaneByName(tbl + 0x245, true);
                slot.unk50 = rp->FindPaneByName(tbl + 0x24C, true);
                slot.mField54 = rp->FindPaneByName(tbl + 0x255, true);
                slot.unk58 = rp->FindPaneByName(tbl + 0x262, true);
                slot.unk5C = rp->FindPaneByName(tbl + 0x26F, true);
                slot.unk60 = rp->FindPaneByName(tbl + 0x27C, true);
                slot.unk64 = rp->FindPaneByName(tbl + 0x289, true);
                slot.unk68 = rp->FindPaneByName(tbl + 0x295, true);
                slot.unk6C = rp->FindPaneByName(tbl + 0x2A1, true);
                slot.unk70 = rp->FindPaneByName(tbl + 0x2AE, true);
            }

            slot.unk25C = 7;

            accessor = CUICfManager_getArcResourceAccessor();
            buildLayout(&slot.unk20, accessor, tbl + 0x2BB);

            accessor = CUICfManager_getArcResourceAccessor();
            bindLayoutAnimTransform(slot.unk20, &slot.unk24, accessor, tbl + 0x2DE);

            slot.unk20->SetAnimationEnable(slot.unk24, true);
            slot.unk24->SetFrame(0.0f);
            slot.unk20->Animate(0);

            accessor = CUICfManager_getArcResourceAccessor();
            buildLayout(&slot.unk28, accessor, tbl + 0x304);

            accessor = CUICfManager_getArcResourceAccessor();
            bindLayoutAnimTransform(slot.unk28, &slot.unk2C, accessor, tbl + 0x323);
            bindLayoutAnimTransform(slot.unk28, &slot.unk30, accessor, tbl + 0x347);
            bindLayoutAnimTransform(slot.unk28, &slot.unk34, accessor, tbl + 0x36D);
            bindLayoutAnimTransform(slot.unk28, &slot.unk38, accessor, tbl + 0x392);
            bindLayoutAnimTransform(slot.unk28, &slot.unk3C, accessor, tbl + 0x3B6);
            bindLayoutAnimTransform(slot.unk28, &slot.unk40, accessor, tbl + 0x3DC);

            slot.unk28->UnbindAllAnimation();
            slot.unk28->BindAnimation(slot.unk2C);
            slot.unk28->SetAnimationEnable(slot.unk2C, true);
            slot.unk28->Animate(0);
            slot.unk28->UnbindAllAnimation();

            {
                // Stage a zeroed slot-tail template on the stack, then block-
                // assign it over slot+0x74..0x204 (same clear shape as
                // BpsStateClearSlot: 0x60-stride body then 0xC-stride remainder).
                MenuBpsSlotInit init;
                init.unk74 = CUICfManager_getArcResourceAccessor();
                init.unk78 = NULL;
                init.unk7C = NULL;
                init.unk80 = 0;
                init.unk84 = NULL;
                init.unk88 = NULL;
                init.unk8C = NULL;

                u8* start = init.pad90;
                u8* end = reinterpret_cast<u8*>(&init) + sizeof(MenuBpsSlotInit);
                u8* lim = end - 0x60;
                u32 z = 0;
                u32 c;
                u32 ok;
                u32 ok2;

                ok = 0;
                ok2 = 0;
                if (!(start > end)) {
                    ok = 1;
                }
                if (ok != 0) {
                    ok2 = 1;
                }
                if (ok2 != 0) {
                    c = (u32)(lim + 0x5f - start) / 0x60;
                    if (start < lim) {
                        for (; c != 0; --c) {
                            u32* w = reinterpret_cast<u32*>(start);
                            w[0] = z;
                            w[1] = z;
                            w[2] = z;
                            w[3] = z;
                            w[4] = z;
                            w[5] = z;
                            w[6] = z;
                            w[7] = z;
                            w[8] = z;
                            w[9] = z;
                            w[10] = z;
                            w[11] = z;
                            w[12] = z;
                            w[13] = z;
                            w[14] = z;
                            w[15] = z;
                            w[16] = z;
                            w[17] = z;
                            w[18] = z;
                            w[19] = z;
                            w[20] = z;
                            w[21] = z;
                            w[22] = z;
                            w[23] = z;
                            start += 0x60;
                        }
                    }
                }
                c = (u32)(end + 0xb - start) / 0xc;
                if (start < end) {
                    for (; c != 0; --c) {
                        u32* w = reinterpret_cast<u32*>(start);
                        w[0] = z;
                        w[1] = z;
                        w[2] = z;
                        start += 0xc;
                    }
                }

                *reinterpret_cast<MenuBpsSlotInit*>(&slot.unk74) = init;
            }

            buildLayout(&slot.unk78,
                           static_cast<nw4r::lyt::ArcResourceAccessor*>(
                               slot.unk74),
                           tbl + 0xA2);
            bindLayoutAnimTransform(slot.unk78, &slot.unk7C,
                           static_cast<nw4r::lyt::ArcResourceAccessor*>(
                               slot.unk74),
                           tbl + 0xBC);

            {
                nw4r::lyt::Pane* subRoot = slot.unk78->GetRootPane();
                u32 fontHandle = (u32)CUICfManager_getPackedFont9C();
                ApplyFontToTxtPanes(subRoot, (const nw4r::ut::Font*)fontHandle);
            }

            slot.unk78->SetAnimationEnable(slot.unk7C, true);
            slot.unk7C->SetFrame(0.0f);
            slot.unk78->Animate(0);

            {
                nw4r::lyt::Pane* found =
                    slot.unk78->GetRootPane()->FindPaneByName(tbl + 0xD9, true);
                if (found != NULL) {
                    MenuBpsPaneWords* pw =
                        reinterpret_cast<MenuBpsPaneWords*>(found);
                    MenuBpsPaneYf* py = reinterpret_cast<MenuBpsPaneYf*>(found);
                    u32 xw = pw->field_2c;
                    u32 zw = pw->field_34;
                    py->field_30 = py->field_30 - yStep * i;
                    pw->field_2c = xw;
                    pw->field_34 = zw;
                }
            }

            // Drop each layout's root pane by the per-slot Y offset. Retail
            // inlines SetTranslate: raw-word x/z round-trip, float Y update.
            #define MENU_BPS_DROP_PANE(layout) \
            do { \
                nw4r::lyt::Pane* p_##__LINE__ = (layout)->GetRootPane(); \
                MenuBpsPaneWords* pw = reinterpret_cast<MenuBpsPaneWords*>(p_##__LINE__); \
                MenuBpsPaneYf* py = reinterpret_cast<MenuBpsPaneYf*>(p_##__LINE__); \
                u32 xw = pw->field_2c; \
                u32 zw = pw->field_34; \
                py->field_30 = py->field_30 - yStep * i; \
                pw->field_2c = xw; \
                pw->field_34 = zw; \
            } while (0)
            MENU_BPS_DROP_PANE(slot.unk00);
            MENU_BPS_DROP_PANE(slot.unk08);
            MENU_BPS_DROP_PANE(slot.unk18);
            MENU_BPS_DROP_PANE(slot.unk20);
            MENU_BPS_DROP_PANE(slot.unk28);
            #undef MENU_BPS_DROP_PANE
        }

        {
            UnkClass_8045F564 temp;
            void* e0 = NULL;
            nw4r::lyt::Layout* e4 = NULL;
            nw4r::lyt::AnimTransform* e8 = NULL;
            nw4r::lyt::AnimTransform* ec = NULL;
            nw4r::lyt::AnimTransform* f0v = NULL;
            u8 f4b = 1;
            u8 f5b;
            u32 f8b = 0;

            unk7D0 = temp;
            unk7E0 = e0;
            unk7E4 = e4;
            unk7E8 = e8;
            unk7EC = ec;
            unk7F0 = f0v;
            unk7F4 = f4b;
            unk7F5 = f5b;
            unk7F8 = f8b;
        }

        accessor = CUICfManager_getArcResourceAccessor();
        unk7E0 = accessor;

        handle = mtl::MemManager::getHandleMEM2();
        unk7D0.createRegion(static_cast<int>(handle), 0x2000,
                           lbl_eu_804FD720, 0);

        {
            Class_8045F858 scoped2(&unk7D0);
            mtl::MemManager::setMemInitFlag(false);

            buildLayout(&unk7E4, accessor, tbl + 0x12);
            bindLayoutAnimTransform(unk7E4, &unk7E8, accessor, tbl + 0x30);
            bindLayoutAnimTransform(unk7E4, &unk7EC, accessor, tbl + 0x51);
            bindLayoutAnimTransform(unk7E4, &unk7F0, accessor, tbl + 0x72);

            unk7E4->SetAnimationEnable(unk7EC, false);
            unk7E4->SetAnimationEnable(unk7F0, false);
            unk7E4->SetAnimationEnable(unk7E8, true);
            unk7E4->Animate(0);
        }

        void* actors[3];
        {
            int* party = CtrlObjectParam_GetSlotTableBase();
            actors[0] = findObjB28ById(party[1]);
            actors[1] = findObjB28ById(party[2]);
            actors[2] = findObjB28ById(party[3]);
        }

        {
            // Fresh NV set for this pass.
            const f32 zeroF2 = 0.0f;
            const f32 oneF = 1.0f;

            for (u8 i = 0; i < 3; i++) {
                cf::CfObjectActor* actor =
                    reinterpret_cast<cf::CfObjectActor*>(actors[i]);
                if (actor == NULL) {
                    continue;
                }
                CMenuBattlePlayerStateSlot& slot = mSlots[i];

                // Retail converts the +0x1E8 getter as signed here (no xoris
                // before the biased lfd); the shared header types the slot
                // u32, so hop through int to keep the signed conversion.
                slot.unk224 = static_cast<f32>(static_cast<int>(
                    actor->CActorParam_getSpentCurrency()));
                slot.unk228 = static_cast<f32>(
                    actor->CActorParam_getLevelExp());
                if (oneF == slot.unk228) {
                    slot.unk224 = zeroF2;
                }
                if (slot.unk22C < zeroF2) {
                    slot.unk22C = slot.unk224;
                }
                slot.unk218 = slot.unk21C = actor->CActorParam_getSecondCurrency();
                slot.unk210 = static_cast<u32>(
                    actor->CActorParam_getHp());
                slot.unk214 = static_cast<u32>(
                    actor->CActorParam_getDamageScale());
            }
        }

        mScn->addRenderCB(static_cast<IScnRender*>(this), 0xA, 0);

        unk64.validateHeap();
    }
}

void CMenuBattlePlayerState::Term() {
    CDeviceVI::waitForDrawDone();
    mScn->removeRenderCB(this);

    for (u8 i = 0; i < 3; i++) {
        CMenuBattlePlayerStateSlot& slot = mSlots[i];

        if (slot.unk00 != NULL) {
            delete slot.unk00;
            slot.unk00 = NULL;
        }
        if (slot.unk08 != NULL) {
            delete slot.unk08;
            slot.unk08 = NULL;
        }
        if (slot.unk18 != NULL) {
            delete slot.unk18;
            slot.unk18 = NULL;
        }
        if (slot.unk20 != NULL) {
            delete slot.unk20;
            slot.unk20 = NULL;
        }
        if (slot.unk28 != NULL) {
            delete slot.unk28;
            slot.unk28 = NULL;
        }
        if (slot.unk78 != NULL) {
            delete slot.unk78;
            slot.unk78 = NULL;
        }
    }

    if (unk7E4 != NULL) {
        delete unk7E4;
        unk7E4 = NULL;
    }

    unk7D0.deleteRegion();
    unk64.deleteRegion();
    spBpsState = NULL;
}

void CMenuBattlePlayerState::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto done;
    }
    if ((gmPauseFlags & (1u << 21)) != 0) {
        goto done;
    }
    if (!IsMenuState621F0()) {
        goto done;
    }
    if (unk7C9 != 0) {
        goto done;
    }
    if (gmGameFlags & 0xAFE40000u) {
        goto done;
    }
    if (cf::CfGameManager::isSceneLoading()) {
        goto done;
    }

    {
        u32 fullHpCount = 0;
        u32 aliveCount = 0;
        void* actors[3];
        actors[0] = NULL;
        actors[1] = NULL;
        actors[2] = NULL;

        int* party = CtrlObjectParam_GetSlotTableBase();
        u8 fi = 0;
        while (fi < 3) {
            // Retail: clrlslwi fi; add party; lwz 4(r3); ... stwx actors,same shift.
            int* p = party + fi;
            void* actor = findObjB28ById(p[1]);
            actors[fi] = actor;
            if (actor != NULL) {
                aliveCount += 1;
            }
            fi += 1;
        }

        // Gauge/bias NVs intentionally not hoisted as named locals: retail loads
        // int->float biases (lfd) before gauge floats; named f32 NVs force lfs-first.

        for (u8 i = 0; i < 3; i++) {
            cf::CfObjectActor* actor;
            CMenuBattlePlayerStateSlot* slot;
            u32 hp;
            u32 maxHp;
            f32 hpRatio;

            actor = reinterpret_cast<cf::CfObjectActor*>(actors[i]);
            if (actor == NULL) {
                continue;
            }

            // Retail: clrlwi; mulli 0x270; add this; addi +0x74 → &mSlots[i].
            slot = &mSlots[i];
            this->BpsStateUpdateSlotArts(actor, slot);

            hp = static_cast<u32>(actor->CActorParam_getHp());
            maxHp = static_cast<u32>(actor->CActorParam_getDamageScale());

            // Retail: lfs f26, zero pool -- not fmr from a zero NV.
            hpRatio = 0.0f;
            if (hp == 0) {
                goto skip_ratio;
            }
            if (maxHp == 0) {
                goto skip_ratio;
            }
            hpRatio =
                100.0f * (static_cast<f32>(hp) / static_cast<f32>(maxHp));
        skip_ratio:

            slot->unk204 = static_cast<u8>(
                reinterpret_cast<MenuBpsActorFields*>(actor)->unk3f28);
            slot->unk208 = reinterpret_cast<u32>(
                actor->CActorParam_getStatusTable());

            {
                u32 statusId = actor->CActorParam_getActorLevel();
                if (slot->unk20C != statusId) {
                    slot->unk25C |= 0x1;
                }
            }
            slot->unk20C = actor->CActorParam_getActorLevel();

            if (slot->unk210 != hp) {
                goto hp_dirty;
            }
            if (slot->unk214 != maxHp) {
                goto hp_dirty;
            }
            goto hp_clean;
        hp_dirty:
            slot->unk25C |= 0x2;
        hp_clean:
            slot->unk210 = hp;
            slot->unk214 = maxHp;
            slot->unk218 = actor->CActorParam_getSecondCurrency();
            slot->unk220 = hpRatio;

            {
                // Retail converts this getter as signed here (the pad
                // declared it int); hop through int to keep the flavor.
                f32 tB = static_cast<f32>(static_cast<int>(
                    actor->CActorParam_getLevelExp()));
                // Constant on the left → retail fcmpu cr0,fNV,f0 (not f0,fNV).
                if (1.0f == tB) {
                    f32 tB2 = static_cast<f32>(static_cast<int>(
                        actor->CActorParam_getLevelExp()));
                    if (slot->unk228 != tB2) {
                        slot->unk25C |= 0x4;
                    }
                    slot->unk224 = 0.0f;
                    slot->unk228 = 1.0f;
                    goto tension_done;
                }

                {
                    f32 tA = static_cast<f32>(actor->CActorParam_getSpentCurrency());
                    if (slot->unk224 != tA) {
                        goto tension_flag;
                    }
                    f32 tB2 = static_cast<f32>(static_cast<int>(
                        actor->CActorParam_getLevelExp()));
                    if (slot->unk228 == tB2) {
                        goto tension_store;
                    }
                }
            tension_flag:
                slot->unk25C |= 0x4;
            tension_store:
                slot->unk224 =
                    static_cast<f32>(actor->CActorParam_getSpentCurrency());
                slot->unk228 = static_cast<f32>(static_cast<int>(
                    actor->CActorParam_getLevelExp()));
            }
        tension_done:

            if (slot->unk22C < 0.0f) {
                slot->unk22C = slot->unk224;
            }

            {
                s16* pair = reinterpret_cast<s16*>(
                    actor->CActorParam_getTensionStatus());
                slot->unk230 = pair[1];
                slot->unk238 = pair[0];
            }

            {
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                u32 nonempty = menuBpsActorListSize(&bm->mActorList1) != 0;
                slot->unk240 = static_cast<u8>(nonempty);
                if (nonempty == 0) {
                    cf::CfObjectMove* player =
                        cf::CfGameManager::getPlayer(0);
                    if (player != NULL) {
                        int id = player->CObjectParam_getSelfObjectId();
                        if (id != 0) {
                            Func800B708C_Ret* handle = reinterpret_cast<Func800B708C_Ret*>(findObjectById(id));
                            if (handle != NULL) {
                                u32 bits = handle->unk64;
                                if (bits & 4) {
                                    slot->unk240 = 1;
                                }
                            }
                        }
                    }
                }
            }

            if (slot->unk21C != slot->unk218) {
                slot->unk21C = slot->unk218;
                slot->unk240 = 1;
            }
            if (slot->unk22C != slot->unk224) {
                slot->unk22C = slot->unk224;
                slot->unk240 = 1;
            }
            if (slot->unk210 != slot->unk214) {
                slot->unk240 = 1;
            }

            if (slot->unk240 == 0) {
                if (slot->unk210 == slot->unk214) {
                    fullHpCount += 1;
                }
            }

            switch (static_cast<s32>(slot->unk244)) {
            case 0:
                this->BpsStateActivateSlot(slot, i);
                break;
            case 1: {
                u32 a = advanceAnimTransform(slot->unk04, 1.0f);
                u32 b = advanceAnimTransform(slot->unk1C, 1.0f);
                slot->unk25C = (slot->unk25C & ~0x300u) | 0xC0u;
                if (a != 0 && b != 0) {
                    slot->unk244 = 2;
                }
                break;
            }
            case 2:
                this->BpsStateRefreshSlot(slot);
                break;
            case 3: {
                u32 a = AnimRewindFrame(slot->unk04,
                                      1.0f);
                u32 b = AnimRewindFrame(slot->unk1C,
                                      1.0f);
                // Retail: ori 0xc0; rlwinm clear PPC bits 21-23 (= 0x700)
                slot->unk25C = (slot->unk25C | 0xC0u) & ~0x700u;
                if (a != 0 && b != 0) {
                    slot->unk244 = 0;
                }
                break;
            }
            default:
                break;
            }

            if (slot->unk244 == 0) {
                continue;
            }

            if (slot->unk25C & 0x40) {
                slot->unk00->Animate(0);
            }
            if (slot->unk25C & 0x100) {
                slot->unk08->Animate(0);
            }
            if (slot->unk25C & 0x880) {
                slot->unk18->Animate(0);
            }
            if (slot->unk25C & 0x400) {
                slot->unk20->Animate(0);
            }
            if (slot->unk25C & 0x200) {
                slot->unk28->Animate(0);
            }
        }

        switch (static_cast<s32>(unk7F8)) {
        case 1:
            if (advanceAnimTransform(unk7E8, 1.0f) != 0) {
                unk7F4 = 1;
                unk7F8 = 2;
                unk7E4->SetAnimationEnable(unk7E8, false);
                unk7E4->SetAnimationEnable(unk7EC, false);
                unk7E4->SetAnimationEnable(unk7F0, true);
                unk7F0->SetFrame(0.0f);
            }
            break;
        case 2:
            advanceAnimTransform(unk7F0, 1.0f);
            break;
        case 3:
            if (AnimRewindFrame(unk7E8, 1.0f) !=
                0) {
                unk7F4 = 1;
                unk7F8 = 0;
            }
            break;
        case 4:
            if (advanceAnimTransform(unk7EC, 1.0f) != 0) {
                unk7EC->SetFrame(0.0f);
                unk7E4->Animate(0);
                unk7E4->SetAnimationEnable(unk7EC, false);
                unk7E4->SetAnimationEnable(unk7F0, false);
                unk7E4->SetAnimationEnable(unk7E8, true);
                unk7F8 = 3;
            }
            break;
        default:
            break;
        }

        if (unk7F8 != 0) {
            nw4r::lyt::Pane* pane =
                unk7E4->GetRootPane()->FindPaneByName(lbl_eu_804FD720 + 0x95,
                                                      true);
            // Retail: fully-inlined VEC3 temp (sp+8) copied into mTranslate;
            // hand-expanded member stores because MWCC will not inline the
            // header ctor/SetTranslate bodies inside this function.
            nw4r::math::VEC3 pos;
            pos.x = lbl_eu_80666FA0;
            pos.y =
                lbl_eu_80666F98 - lbl_eu_80666F9C * static_cast<f32>(unk7F5);
            pos.z = 0.0f;
            MenuBpsPaneXlate* px =
                reinterpret_cast<MenuBpsPaneXlate*>(pane);
            px->field_0x2c = pos.x;
            px->field_0x30 = pos.y;
            px->field_0x34 = pos.z;
        }

        unk7E4->Animate(0);

        if (static_cast<int>(fullHpCount) == static_cast<int>(aliveCount)) {
            f32 t = unk7C4 + 1.0f;
            unk7C4 = t;
            if (t >= 360.0f) {
                if (unk7C8 == 0) {
                    playUISound(0x9a);
                }
                unk7C8 = 1;
            }
        } else {
            unk7C4 = 0.0f;
            unk7C8 = 0;
        }
    }
done:
    ;
}

void CMenuBattlePlayerState::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto done;
    }
    if ((gmPauseFlags & (1u << 21)) != 0) {
        goto done;
    }
    if (!IsMenuState621F0()) {
        goto done;
    }
    if (unk7C9 != 0) {
        goto done;
    }
    if (gmGameFlags & 0xAFE40000u) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);

        for (u8 i = 0; i < 3; i++) {
            CMenuBattlePlayerStateSlot& slot = mSlots[i];

            if (slot.unk244 == 0) {
                continue;
            }

            drawLayout(slot.unk00, &drawInfo, 0, (slot.unk25C & 0x40) != 0);

            if (slot.unk25C & 0x100) {
                drawLayout(slot.unk08, &drawInfo, 0, 1);
            }
            if (slot.unk25C & 0x200) {
                drawLayout(slot.unk28, &drawInfo, 0, 1);
            }
            drawLayout(slot.unk18, &drawInfo, 0, (slot.unk25C & 0x880) != 0);
            if (slot.unk25C & 0x400) {
                drawLayout(slot.unk20, &drawInfo, 0, 1);
            }

            if (slot.unk78 != NULL && slot.unk80 != 0) {
                drawLayout(slot.unk78, &drawInfo, 0, 1);
            }
        }

        if (unk7F8 != 0) {
            drawLayout(unk7E4, &drawInfo, 0, 1);
        }
    }
done:
    ;
}

// Under stock -O4,p, /0x60 strength-reduces to mulhwu. Size opt keeps
// retail li/divwu/mtctr/bdnz (same as TU -O4,s). See MWCC_CASES.
#pragma optimize_for_size on
void BpsStateClearSlot(CMenuBattlePlayerStateSlot* slot){
    u32 z;
    u8* start;
    u8* end;
    u8* lim;
    u8* p;
    u32 c;
    u32 ok;
    u32 ok2;

    start = slot->pad90;
    end = reinterpret_cast<u8*>(&slot->unk204);

    slot->unk74 = NULL;
    slot->unk78 = NULL;
    slot->unk7C = NULL;
    slot->unk80 = 0;
    slot->unk84 = NULL;
    slot->unk88 = NULL;
    slot->unk8C = NULL;

    if (start >= end) {
        return;
    }

    // Retail bool gate before the 0x60 body (cmplw start,end → ok/ok2).
    lim = end - 0x60;
    ok = 0;
    ok2 = 0;
    if (!(start > end)) {
        ok = 1;
    }
    if (ok != 0) {
        ok2 = 1;
    }
    p = start;
    z = 0;
    if (ok2 != 0) {
        c = (u32)(lim + 0x5f - start) / 0x60;
        if (p < lim) {
            for (; c != 0; --c) {
                u32* w = reinterpret_cast<u32*>(p);
                w[0] = z;
                w[1] = z;
                w[2] = z;
                w[3] = z;
                w[4] = z;
                w[5] = z;
                w[6] = z;
                w[7] = z;
                w[8] = z;
                w[9] = z;
                w[10] = z;
                w[11] = z;
                w[12] = z;
                w[13] = z;
                w[14] = z;
                w[15] = z;
                w[16] = z;
                w[17] = z;
                w[18] = z;
                w[19] = z;
                w[20] = z;
                w[21] = z;
                w[22] = z;
                w[23] = z;
                p += 0x60;
            }
        }
    }
    c = (u32)(end + 0xb - p) / 0xc;
    if (p < end) {
        for (; c != 0; --c) {
            u32* w = reinterpret_cast<u32*>(p);
            w[0] = z;
            w[1] = z;
            w[2] = z;
            p += 0xc;
        }
    }
}
extern "C" int BpsStateSingletonToInt() { return (int)spBpsState; }
extern "C" void BpsStateFlag7C9Set(CMenuBattlePlayerState* p) {
    p->unk7C9 = 1;
}
extern "C" void sinit_8010E9F8() { sBpsPaneColorWhite.a = 0xff; sBpsPaneColorWhite.b = 0xff; sBpsPaneColorWhite.g = 0xff; sBpsPaneColorWhite.r = 0xff; sBpsPaneColorGold.a = 0xff; sBpsPaneColorGold.b = 0x5c; sBpsPaneColorGold.g = 0x92; sBpsPaneColorGold.r = 0xb9; sBpsPaneColorGray.a = 0xff; sBpsPaneColorGray.b = 0x50; sBpsPaneColorGray.g = 0x50; sBpsPaneColorGray.r = 0x50; }
// This-adjusting deleting-dtor thunk (secondary base at +0x58):
// retail emits `subi r3, r3, 0x58; b __dt__22CMenuBattlePlayerStateFv`.
// Forwarding deleteFlag untouched keeps the tail call (no bl).
// (thunk-recovery pilot: kept hand-written (named) so hexdiff pairs by name;
// the weak compiler @88@/@92@ twins pair only by offset, which needs exact
// tiling -- re-delete once this TU tiles exactly; padding keeps line count.)
extern "C" void* BpsStateDtorThunk58(void* self, int deleteFlag) {
    return __dt__22CMenuBattlePlayerStateFv(
        reinterpret_cast<CMenuBattlePlayerState*>((char*)self - 0x58),
        deleteFlag);
}
// IScnRender cbRenderBefore thunk (free-pointer call: member is now virtual,
// so a member call would bctrl-dispatch; retail is a direct bl). The fwd decl
// shares the thunk line so extern-C declaration counts are unchanged (pilot).
extern "C" void cbRenderBefore__22CMenuBattlePlayerStateFP4CScn(); extern "C" void BpsStateRenderThunk5C(void* self) { ((void(*)(void*))cbRenderBefore__22CMenuBattlePlayerStateFP4CScn)((char*)self - 0x5c); }
// This-adjusting deleting-dtor thunk (IScnRender base at +0x5c):
// retail emits `subi r3, r3, 0x5c; b __dt__22CMenuBattlePlayerStateFv`.
// Forwarding deleteFlag untouched keeps the tail call (no li/bl). Pad pad pad.
extern "C" void* BpsStateDtorThunk5C(void* self, int deleteFlag) {
    return __dt__22CMenuBattlePlayerStateFv(
        reinterpret_cast<CMenuBattlePlayerState*>((char*)self - 0x5c),
        deleteFlag);
}

// Runtime array-destruction helper (MWCC runtime library).
extern "C" void __destroy_arr(void* ptr, void* dtor, u32 size, u32 count);

// Battle-menu imports (retail unmangled names; see CUIBattleManager.hpp for
// UIBattleReleaseAll semantics - pane/material helpers shared by menu TUs).
extern "C" void* UIBattleGetTableB8();
extern "C" void UIBattleReleaseAll();
extern "C" void BtlDmg_QueueEnqueue(void* obj, u32 a, u32 b, u32 c);
extern "C" void* __ct__CMenuBattlePlayerState(void* self, CScn* scn);

#pragma optimize_for_size off
// ---------------------------------------------------------------------------
// Target us-8010d8a8: singleton factory. Allocates the player-state process
// from work memory, runs its constructor, stores the global pointer, and
// registers it as a child of `parent` (registration happens even when the
// allocation failed, matching retail). Returns NULL when already created.
// ---------------------------------------------------------------------------
CMenuBattlePlayerState* BpsStateCreateSingleton(CProcess* parent, CScn* scn) {
    if (spBpsState != NULL) {
        return NULL;
    }
    CMenuBattlePlayerState* obj = static_cast<CMenuBattlePlayerState*>(
        mtl::MemManager::allocate(0x7FC, CWorkThreadSystem::getWorkMem()));
    if (obj != NULL) {
        // The ctor returns `this` in r3, keeping obj live in r3 across the
        // call (no extra callee-saved spill), matching retail.
        obj = static_cast<CMenuBattlePlayerState*>(
            __ct__CMenuBattlePlayerState(obj, scn));
    }
    spBpsState = obj;
    obj->Regist(parent, false);
    return spBpsState;
}
#pragma optimize_for_size on

#pragma optimize_for_size off
// ---------------------------------------------------------------------------
// Target us-8010bf60: complete-object destructor (retail flag-carrying ABI).
// Order: unk7D0, slot array (__dt__8010B444 element dtor releases each
// individually allocated sub-layout set), unk64, CProcess base, then free.
// ---------------------------------------------------------------------------
extern "C" void* __dt__22CMenuBattlePlayerStateFv(
    CMenuBattlePlayerState* self, int deleteFlag) {
    if (self != NULL) {
        // Retail folds this guard into addic. r3,self,0x7CC (address of the
        // pre-unk7D0 word) rather than a fresh null test.
        char* preWord = (char*)self + 0x7CC;
        if (preWord != NULL) {
            // Member dtor call emits __dt__17UnkClass_8045F564Fv with r4=-1.
            reinterpret_cast<UnkClass_8045F564*>(preWord + 4)
                ->~UnkClass_8045F564();
        }
        __destroy_arr(self->mSlots, (void*)__dt__8010B444,
                      sizeof(CMenuBattlePlayerStateSlot), 3);
        self->unk64.~UnkClass_8045F564();
        // Retail emits the null test twice (single cmpwi, two beq).
        if (self != NULL && self != NULL) {
            __dt__8CProcessFv(self, 0);
        }
        if (deleteFlag > 0) {
            operator delete(self);
        }
    }
    return self;
}
// ---------------------------------------------------------------------------
// Target us-8010dbb0: per-slot animation select. Picks an entry from the
// shared battle-UI animation table indexed by the slot number, chosen by
// `mode`; a zero entry (or missing table) resets via UIBattleReleaseAll.
// ---------------------------------------------------------------------------
struct MenuBpsModeEntry {
    u32 unk00;
    u32 unk04;
    u32 unk08;
};

// Default mode: the BpsStateRefreshSlot caller passes no third argument (retail
// leaves r5 stale from an earlier callee), so the parameter defaults.
extern "C" void BpsStateSelectSlotAnim(CMenuBattlePlayerState* self,
                              CMenuBattlePlayerStateSlot* slot, s32 mode = 0) {
    MenuBpsModeEntry* table =
        reinterpret_cast<MenuBpsModeEntry*>(UIBattleGetTableB8());
    if (table != NULL) {
        // Retail lowers this as a switch: range tests 0/1 and 3/4 unsigned
        // with deferred case bodies in source order, equality test 2 last.
        u32 sel = 0;
        switch (mode) {
        case 0:
        case 1:
            sel = table[slot->unk258].unk08;
            break;
        case 2:
            sel = table[slot->unk258].unk00;
            break;
        case 3:
        case 4:
            sel = table[slot->unk258].unk04;
            break;
        }
        if (sel != 0) {
            PaneSetTexPalette(slot->unk5C, (TPLPalette*)sel);
            PaneSetTexPalette(slot->unk60, (TPLPalette*)sel);
            PaneSetTexPalette(slot->unk64, (TPLPalette*)sel);
            PaneSetTexPalette(slot->unk68, (TPLPalette*)sel);
        } else {
            UIBattleReleaseAll();
        }
    }
}

// ---------------------------------------------------------------------------
// Target us-8010d92c: routes a party-visual update for the acting actor.
// Resolves the actor's handle, requires bit 1 of its +0x64 flags, matches the
// id byte at handle+0x8C against the first three party-table words, and
// forwards to the per-slot updater at (state + i*0x270 + 0xE8).
// ---------------------------------------------------------------------------
void BpsStateQueueDamage(int id, u32 a, u32 b, u32 c) {
    CMenuBattlePlayerState* state = spBpsState;
    if (state == NULL) {
        return;
    }
    Func800B708C_Ret* handle =
        reinterpret_cast<Func800B708C_Ret*>(findObjectById(id));
    if (handle == NULL) {
        return;
    }
    if ((handle->unk64 & 2) == 0) {
        return;
    }
    int idxByte = handle->unk8C & 0xFF;
    struct PartyData {
        u32 w[12];
    };
    PartyData party = *(PartyData*)((u8*)CtrlObjectParam_GetSlotTableBase() + 4);

    for (u8 i = 0; i < 3; i++) {
        if (idxByte == static_cast<int>(party.w[i])) {
            char* slotView = reinterpret_cast<char*>(state) + static_cast<u32>(i) * 0x270;
            BtlDmg_QueueEnqueue(slotView + 0xE8, a, b, c);
            return;
        }
    }
}
#pragma optimize_for_size on

// Battle-party-slot per-frame update: finds the first free arts slot, derives
// an HP-gauge width ratio from a BDAT lookup, and writes it into the
// "N_HpGauge_Max" pane size.
extern "C" void BpsStateUpdateHpGauge(CMenuBattlePlayerState* self,
                              CMenuBattlePlayerStateSlot* slot) {
    // state load first: retail keeps it in r5 (result takes r6).
    MenuBpsActorState* state =
        reinterpret_cast<MenuBpsActorState*>(slot->unk208);
    u32 result = 0;
    // u32 (not u8) so the found-branch idx*4 keeps the 30-bit slwi mask;
    // the u8 range is re-derived at the unk888 index site.
    u32 idx = state->unk89C & 0xFF;
    MenuBpsPartyRow* row = &state->rows[idx];

    for (u32 i = 1; i < 6; i++) {
        // Named offset keeps the (i&0xFF)<<5 temp in its own register
        // (retail r5) instead of coalescing it with the lwzx dest r0.
        u32 off = (i & 0xFF) * 0x20;
        if (row->entries[off >> 5].unk00 != 0) {
            continue;
        }
        u8 c = slot->unk204;
        if (c <= 8) {
            // Single named base so MWCC shares one lis/addi(@l) across both
            // string args (retail keeps the table in r5).
            // Dependent-statement chain pins retail's temp birth order:
            // subi(c-1), lis(table), slwi(idx*4), mulli(*25), then adds.
            u32 k = c - 1;
            char* tbl = lbl_eu_804FD720;
            u32 iw = idx * 4;
            k *= 25;
            iw += idx;
            iw += i;
            k += iw;
            char* sA = tbl + 0x415;
            char* sB = tbl + 0x422;
            result = static_cast<u8>(BdatGetU8ByTableKey(sA, sB,
                                                   static_cast<u8>(k))) *
                     100;
        }
        break;
    }

    s32 maxHp = reinterpret_cast<MenuBpsActorState*>(slot->unk208)
                    ->unk888[static_cast<u8>(idx)];
    u32 ok = 0;
    f32 hpF = (f32)maxHp;
    f32 resF = (f32)result;
    f32 ratio;
    // Named-lbl zero/one so the SDA21 relocs keep the retail labels
    // (literals would fold into the TU-local float pool @N).
    if (hpF != 0.0f && resF != 0.0f) {
        ok = 1;
    }
    if (ok) {
        ratio = hpF / resF;
    } else {
        ratio = 0.0f;
    }

    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    if (slot->unk204 > 8) {
        ratio = 0.0f;
    }

    nw4r::lyt::Pane* pane = slot->mField54;
    if (pane != NULL) {
        pane->SetSize(nw4r::lyt::Size(82.0f * ratio,
                                      pane->GetSize().height));
    }
}
// Keep names distinct from Move's extern callees or MWCC DCE's the bl sites.

// Actor field holding the level icon id (fed in as arts-slot entry 0x20).
struct MenuBpsMoveActorFields {
    u8 pad00[0x1530];
    u32 unk1530; // +0x1530: level icon id (u16)
};

// ---------------------------------------------------------------------------
// Target us-8010dc90: per-actor arts-icon / auto-heal icon refresh for one
// battle-party slot. Advances a wrap-around cursor over the 32 icon slots,
// resolves each occupied slot's icon through the BDAT label table, binds it on
// the shared texture manager under the "timg" key, and lights the matching
// star pane's flag byte. Runs twice: once over the actor's live icon slots
// (unk268/unk6C pane), then over a snapshot plus the level icon at index 0x20
// (unk26C/unk70 pane).
// ---------------------------------------------------------------------------
void CMenuBattlePlayerState::BpsStateUpdateSlotArts(cf::CfObjectActor* actor, CMenuBattlePlayerStateSlot* slot) {
    nw4r::lyt::Pane* starPane;

    // Tension timer: raises flag bit 0x800 when it exceeds FDC.
    slot->unk264 = slot->unk264 + 1.0f;
    slot->unk25C = slot->unk25C & ~0x800u;
    if (slot->unk264 > 60.0f) {
        slot->unk25C = slot->unk25C | 0x800u;
        slot->unk264 = 0.0f;

        // Pass 1: scan the actor's live arts-icon slots from last match + 1,
        // binding the first hit's icon and lighting the level star pane.
        starPane = slot->unk6C;
        char* tbl = lbl_eu_804FD720;
        const u32 texKey = 0x74696D67;
        starPane->SetVisible(false);

        u32 cursor = (slot->unk268 + 1) & 0xFF;
        while (true) {
            if ((cursor & 0xFF) >= 0x20) {
                cursor = 0;
            }
            cf::CBattleStateEntry* rec =
                actor->CBattleState_getEventEntry(
                    static_cast<int>(cursor & 0xFF));
            if (rec->unk30 != 0x800 && rec->unk0C != 0 && rec->unk0C != 0xF &&
                rec->unk0C != 0x10 && rec->unk0C != 0x12) {
                u16 nameId = static_cast<u16>(
                    BdatGetU16Direct(::cf::CfBdat::spBtlBuffListFileData, tbl + 0x42B, rec->unk0C));
                if (nameId != 0) {
                    char* tex = MakeTplNameSysFile(nameId);
                    u32 bound = reinterpret_cast<u32>(
                        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                            CUICfManager_getArcResourceAccessor())
                            ->GetResource(texKey, tex, NULL));
                    if (bound != 0) {
                        starPane->SetVisible(true);
                        PaneSetTexPalette(starPane, (TPLPalette*)bound);
                        slot->unk268 = static_cast<u32>(static_cast<u8>(cursor));
                        break;
                    }
                }
            }
            if (static_cast<int>(static_cast<u8>(cursor)) ==
                static_cast<int>(slot->unk268)) {
                break;
            }
            cursor += 1;
        }

        // Pass 2: snapshot all 32 slots (empty-state slots read as 0), append
        // the actor's level icon as entry 0x20, then scan from last match + 1.
        starPane = slot->unk70;
        starPane->SetVisible(false);

        u16 ids[33];
        // u8 counter: retail zero-extends it for the Func21 arg and derives
        // the ids index as j*2 from the extended value.
        for (u8 j = 0; j < 32; j++) {
            cf::CBattleStateEntry* entry =
                actor->CBattleState_fetchStatusEntry(j);
            ids[j] = (entry->unk30 == 0x800u) ? 0 : entry->unk0C;
        }
        ids[32] = reinterpret_cast<MenuBpsMoveActorFields*>(actor)->unk1530;

        u32 cur2 = (slot->unk26C + 1) & 0xFF;
        char* tbl2 = lbl_eu_804FD720;
        const u32 texKey2 = 0x74696D67;
        while (true) {
            if ((cur2 & 0xFF) >= 0x21) {
                cur2 = 0;
            }
            u16 v = ids[cur2 & 0xFF];
            if (v != 0) {
                if ((cur2 & 0xFF) == 0x20) {
                    // Fixed "level up" icon id, no BDAT lookup for entry 0x20.
                    char* tex = MakeTplNameSysFile(0x13D);
                    u32 bound = reinterpret_cast<u32>(
                        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                            CUICfManager_getArcResourceAccessor())
                            ->GetResource(texKey2, tex, NULL));
                    if (bound != 0) {
                        starPane->SetVisible(true);
                        PaneSetTexPalette(starPane, (TPLPalette*)bound);
                        slot->unk26C =
                                static_cast<u32>(static_cast<u8>(cur2));
                        return;
                    }
                } else {
                    u16 nameId = static_cast<u16>(
                        BdatGetU16Direct(::cf::CfBdat::spBtlBuffListFileData, tbl2 + 0x42B, v));
                    if (nameId != 0) {
                        char* tex = MakeTplNameSysFile(nameId);
                        u32 bound = reinterpret_cast<u32>(
                            reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                                CUICfManager_getArcResourceAccessor())
                                ->GetResource(texKey2, tex, NULL));
                        if (bound != 0) {
                            starPane->SetVisible(true);
                            PaneSetTexPalette(starPane, (TPLPalette*)bound);
                            slot->unk26C =
                                static_cast<u32>(static_cast<u8>(cur2));
                            return;
                        }
                    }
                }
            }
            if (static_cast<int>(static_cast<u8>(cur2)) ==
                static_cast<int>(slot->unk26C)) {
                break;
            }
            cur2 += 1;
        }
    }
}
// ---------------------------------------------------------------------------
// Target us-8010df8c: per-slot activation pass. Refreshes the level text and
// name label, redraws HP digits / color set when flagged, updates the HP gauge
// width (quantized through sBpsHpGaugeWidths), advances mode/prev snapshots,
// toggles the party/tension star panes by HP state, resets the tension anims
// and re-arms the per-slot state machine. `index` nonzero suppresses the
// playUISound(0x99) jingle on first activation.
// ---------------------------------------------------------------------------
void CMenuBattlePlayerState::BpsStateActivateSlot(CMenuBattlePlayerStateSlot* slot, u32 index) {
    u8 gate = slot->unk240;
    slot->unk25C &= ~0x03E00000u;
    if (gate == 0 && slot->unk80 == 0) {
        return;
    }

    char buf[0x38];
    slot->unk25C = 0xC7;
    u32 prevStatus = slot->unk20C;
    slot->unk25C = 0xC6;
    sprintf(buf, lbl_eu_804FD720 + 0x401, prevStatus);
    SetLayoutSelfText(reinterpret_cast<nw4r::lyt::Layout*>(slot->unk44), buf, 0);
    SetLayoutSelfText(
        reinterpret_cast<nw4r::lyt::Layout*>(slot->unk48),
        BdatTouchStringCell(lbl_eu_804FD720 + 0x406, lbl_eu_804FD720 + 0x410, 2),
        0);

    if (slot->unk25C & 0x2) {
        slot->unk25C &= ~0x2u;
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)slot->unk4C, slot->unk210);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)slot->unk50, slot->unk214);
        // Color-set swap keyed on full-HP / dead / other.
        if (slot->unk210 == slot->unk214) {
            PaneMatSetS16Quad(slot->unk4C, &sBpsPaneColorGold, 1);
        } else if (slot->unk210 == 0) {
            PaneMatSetS16Quad(slot->unk4C, &sBpsPaneColorGray, 1);
        } else {
            PaneMatSetS16Quad(slot->unk4C, &sBpsPaneColorWhite, 1);
        }
    }

    BpsStateUpdateHpGauge(this, slot);

    if (slot->unk25C & 0x4) {
        slot->unk25C &= ~0x4u;
        f32 ratio = slot->unk224 / slot->unk228;
        nw4r::lyt::Pane* pane =
            reinterpret_cast<nw4r::lyt::Pane*>(slot->unk58);
        if (pane != NULL) {
            f32 scaled = 10.0f * ratio;
            f32 widthF = 0.5f * scaled;
            // Retail copies the width table onto the stack before indexing.
            f32 widths[5];
            widths[0] = sBpsHpGaugeWidths[0];
            widths[1] = sBpsHpGaugeWidths[1];
            widths[2] = sBpsHpGaugeWidths[2];
            widths[3] = sBpsHpGaugeWidths[3];
            widths[4] = sBpsHpGaugeWidths[4];
            // Signed int convert: MWCC emits inline fctiwz + signed-int->float
            // bias (lbl_eu_80666FB8 == 0x4330000080000000), not __cvt_fp2unsigned.
            s32 w = widthF;
            slot->unk24C = w;
            f32 prevMax = pane->GetSize().height;
            // u32 -> f32 goes through a biased double in MWCC.
            f32 frac = ratio - 0.2f * w;
            f32 q = frac / 0.2f;
            f32 val = 14.0f * q + widths[w];
            pane->SetSRTElement(8, val);
            pane->SetSRTElement(9, prevMax);
        }
    }

    slot->unk234 = slot->unk230;
    slot->unk23C = slot->unk238;
    BpsStateSelectSlotAnim(this, slot, slot->unk230);

    // Star-pane visibility follows the smoothed HP ratio.
    if (slot->unk220 == 0.0f) {
        slot->unk5C->SetVisible(true);
        slot->unk60->SetVisible(true);
        slot->unk64->SetVisible(false);
        slot->unk68->SetVisible(false);
        slot->unk08->GetRootPane()->SetVisible(false);
    } else {
        slot->unk5C->SetVisible(false);
        slot->unk60->SetVisible(false);
        slot->unk64->SetVisible(true);
        slot->unk68->SetVisible(true);
        slot->unk08->GetRootPane()->SetVisible(true);
    }

    slot->unk28->UnbindAllAnimation();
    slot->unk254 = 0xB;
    reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk24)->SetFrame(
0.0f);
    slot->unk20->GetRootPane()->SetVisible(true);
    slot->unk25C &= ~0x40000000u;
    reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk0C)->SetFrame(
0.0f);
    reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk10)->SetFrame(
0.0f);
    slot->unk08->SetAnimationEnable(slot->unk10, false);
    slot->unk08->SetAnimationEnable(slot->unk14, false);
    slot->unk08->SetAnimationEnable(slot->unk0C, true);
    reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk0C)->SetFrame(
0.0f);
    slot->unk08->Animate(0);
    slot->unk248 = 4;
    slot->unk24C = 0;
    slot->unk250 = 6;
    slot->unk18->SetAnimationEnable(slot->unk1C, true);
    slot->unk244 = 1;
    if (index == 0) {
        playUISound(0x99);
    }
}
// ---------------------------------------------------------------------------
// Target us-8010e3b0: per-slot battle-HUD update (state 2 of unk244). Refreshes
// level/name text when flagged, redraws HP digits/colors, updates the tension
// gauge, drives the star-pane visibility by HP ratio, advances the mode-change
// jingle, runs the tension state machine (unk250) and party-gauge machine
// (unk254), and steps the sub-layout (unk78) cursor.
// ---------------------------------------------------------------------------
void CMenuBattlePlayerState::BpsStateRefreshSlot(CMenuBattlePlayerStateSlot* slot) {
    u32 flags = slot->unk25C;
    u32 prevStatus = slot->unk20C;
    // Clear bits 6-10 up front; bit 0 only drops when the refresh runs.
    u32 masked = flags & ~0x7C0u;
    slot->unk25C = masked;
    char* tbl;
    if (flags & 1) {
        // Retail materializes the string table here (inside the branch) but
        // keeps the register live for the whole function.
        tbl = lbl_eu_804FD720;
        slot->unk25C = masked & ~1u;
        char buf[0x10];
        sprintf(buf, tbl + 0x401, prevStatus);
        SetLayoutSelfText(reinterpret_cast<nw4r::lyt::Layout*>(slot->unk44), buf, 0);
        SetLayoutSelfText(
            reinterpret_cast<nw4r::lyt::Layout*>(slot->unk48),
            BdatTouchStringCell(tbl + 0x406, tbl + 0x410, 2),
            0);
    }

    if (slot->unk25C & 2) {
        slot->unk25C &= ~2u;
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)slot->unk4C, slot->unk210);
        LayoutSetTextBoxInt((nw4r::lyt::Layout*)slot->unk50, slot->unk214);
        // Color-set swap keyed on full-HP / dead / other.
        if (slot->unk210 == slot->unk214) {
            PaneMatSetS16Quad(slot->unk4C, &sBpsPaneColorGold, 1);
        } else if (slot->unk210 == 0) {
            PaneMatSetS16Quad(slot->unk4C, &sBpsPaneColorGray, 1);
        } else {
            PaneMatSetS16Quad(slot->unk4C, &sBpsPaneColorWhite, 1);
        }
    }

    BpsStateUpdateHpGauge(this, slot);

    if (slot->unk25C & 4) {
        slot->unk25C &= ~4u;
        f32 ratio = slot->unk224 / slot->unk228;
        nw4r::lyt::Pane* pane =
            reinterpret_cast<nw4r::lyt::Pane*>(slot->unk58);
        if (pane != NULL) {
            f32 scaled = 10.0f * ratio;
            f32 widthF = 0.5f * scaled;
            // Retail copies the width table onto the stack before indexing.
            f32 widths[5];
            widths[0] = sBpsHpGaugeWidths[0];
            widths[1] = sBpsHpGaugeWidths[1];
            widths[2] = sBpsHpGaugeWidths[2];
            widths[3] = sBpsHpGaugeWidths[3];
            widths[4] = sBpsHpGaugeWidths[4];
            s32 w = widthF;
            slot->unk24C = w;
            f32 prevMax = pane->GetSize().height;
            f32 q = (ratio - 0.2f * w) / 0.2f;
            f32 val = 14.0f * q + widths[w];
            pane->SetSRTElement(8, val);
            pane->SetSRTElement(9, prevMax);
        }
    }

    if (slot->unk220 <= 0.0f) {
        // HP empty: light the party/tension stars, hide the arts/heal stars.
        slot->unk5C->SetVisible(true);
        slot->unk60->SetVisible(true);
        slot->unk64->SetVisible(false);
        slot->unk68->SetVisible(false);
        nw4r::lyt::Pane* mainRoot = slot->unk08->GetRootPane();
        mainRoot->SetVisible(false);

        u32 f4 = slot->unk25C;
        if ((f4 & 0x1000) == 0) {
            slot->unk25C = f4 | 0x80;
        }

        u32 mode = slot->unk250;
        slot->unk25C = slot->unk25C | 0x1000;
        if (mode != 6) {
            // Reset sequence: park every tension anim, re-arm unk0C, restart.
            slot->unk08->SetAnimationEnable(slot->unk0C,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk10,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk14,
                                                                   true);
            reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk14)->SetFrame(
0.0f);
            slot->unk08->Animate(0);
            slot->unk08->SetAnimationEnable(slot->unk14,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk10,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk10,
                                                                   true);
            reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk10)->SetFrame(
0.0f);
            slot->unk08->Animate(0);
            slot->unk08->SetAnimationEnable(slot->unk10,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk14,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk0C,
                                                                   true);
            reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk0C)->SetFrame(
0.0f);
            slot->unk08->Animate(0);
            slot->unk250 = 6;
        }
        slot->unk230 = 0;
        slot->unk238 = 0;
    } else {
        u32 f5 = slot->unk25C;
        slot->unk25C = f5 & ~0x1000u;
        slot->unk5C->SetVisible(false);
        slot->unk60->SetVisible(false);
        slot->unk64->SetVisible(true);
        slot->unk68->SetVisible(true);
        slot->unk08->GetRootPane()->SetVisible(true);
    }

    {
        u32 curMode = slot->unk230;
        u32 prevMode = slot->unk234;
        if (curMode != prevMode || slot->unk238 != slot->unk23C) {
            BpsStateSelectSlotAnim(this, slot);

            if (slot->unk260 == 0) {
                void* player = cf::CfGameManager::getPlayer(0);
                if (player != NULL) {
            MapFx_ResolveOwner(player);
            cf::CBattleManager* bm = cf::CBattleManager::getInstance();
            // Inline actor-list occupancy count (retail shape: head in r5,
            // counter r4, cursor r3).
            _reslist_node<cf::CfObjectActor*>* head =
                bm->mActorList1.mStartNodePtr;
            u32 n = 0;
            _reslist_node<cf::CfObjectActor*>* cur = head->mNext;
            while (cur != head) {
                n += 1;
                cur = cur->mNext;
            }
            if (n != 0) {
                    s32 delta;
                    if (slot->unk234 != slot->unk230) {
                        delta = slot->unk234 - slot->unk230;
                    } else {
                        delta = slot->unk23C - slot->unk238;
                    }
                    // Directional jingle: flash the gauge pane while playing
                    // one of two BDAT-named sub-anims, then a system sound.
                    // Pane::FindPaneByName (+0x3C); retail keeps the returned
                    // pane in r3 and toggles its +0xBB flag byte directly.
                    if (delta < 0) {
                        nw4r::lyt::Pane* flashA = slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x430, true);
                        flashA->SetVisible(false);
                        nw4r::lyt::Pane* flashB = slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x43b, true);
                        flashB->SetVisible(true);
                        if (!GetZealObjPtr()) {
                            playUISound(0x63);
                        }
                    } else if (delta > 0) {
                        nw4r::lyt::Pane* flashA = slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x430, true);
                        flashA->SetVisible(true);
                        nw4r::lyt::Pane* flashB = slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x43b, true);
                        flashB->SetVisible(false);
                        if (!GetZealObjPtr()) {
                            playUISound(0x64);
                        }
                    }
                    if (delta != 0) {
                        slot->unk25C |= 0x40000000;
                    }
                }
            }
            slot->unk234 = slot->unk230;
            slot->unk23C = slot->unk238;
            slot->unk260 = (gmGameFlags >> 26) & 1;
        }
    }}

    if (slot->unk25C & 0x40000000) {
        slot->unk25C = slot->unk25C | 0x400;
        if (advanceAnimTransform(slot->unk24, 1.0f) != 0) {
            reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk24)->SetFrame(
0.0f);
            slot->unk25C = slot->unk25C & ~0x40000000u;
        }
    }

    // Tension anim state machine (unk250).
    u32 changed1 = 0;
    switch (static_cast<s32>(slot->unk250)) {
    case 6:
        if (slot->unk220 <= 50.0f) {
            if (slot->unk220 > 0.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, true);
                slot->unk250 = 7;
                changed1 = 1;
            }
        }
        break;
    case 7:
        changed1 = 1;
        if (advanceAnimTransform(slot->unk0C, 1.0f) != 0) {
            if (slot->unk220 <= 15.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, true);
                slot->unk250 = 0xa;
            } else if (slot->unk220 <= 50.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk10, true);
                slot->unk250 = 8;
            } else {
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, true);
                slot->unk250 = 9;
            }
        }
        break;
    case 8:
        changed1 = 1;
        if (advanceAnimTransform(slot->unk10, 1.0f) != 0) {
            if (slot->unk220 <= 15.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, true);
                slot->unk250 = 0xa;
            } else if (slot->unk220 > 50.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, true);
                slot->unk250 = 9;
            }
        }
        break;
    case 9:
        changed1 = 1;
        if (AnimRewindFrame(slot->unk0C, 1.0f) != 0) {
            slot->unk250 = 6;
        }
        break;
    case 0xa:
        changed1 = 1;
        if (advanceAnimTransform(slot->unk14, 1.0f) != 0) {
            if (slot->unk220 > 50.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, true);
                slot->unk250 = 9;
            } else if (slot->unk220 > 15.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk10, true);
                slot->unk250 = 8;
            } else if (slot->unk220 <= 0.0f) {
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, true);
                slot->unk250 = 9;
            }
        }
        break;
    default:
        break;
    }
    if (changed1 != 0) {
        slot->unk25C |= 0x100;
    }

    // Party gauge in/out state machine (unk254). Each arm binds the next
    // transform on the shared unk28 layout: Animate, reset, BindAnimation,
    // SetAnimationEnable(true), rewind frame, Animate again.
    u32 changed2 = 0;
    switch (static_cast<s32>(slot->unk254)) {
    case 0xB:
        if (slot->unk220 > 0.0f) {
            if (slot->unk230 == 4) {
                nw4r::lyt::Layout* lay = slot->unk28;
                changed2 = 1;
                nw4r::lyt::AnimTransform* anm = slot->unk2C;
                lay->Animate(0);
                lay->UnbindAllAnimation();
                lay->BindAnimation(anm);
                lay->SetAnimationEnable(anm, true);
                reinterpret_cast<nw4r::lyt::AnimTransform*>(anm)->SetFrame(
0.0f);
                lay->Animate(0);
                slot->unk254 = 0xC;
            } else if (slot->unk230 == 0) {
                nw4r::lyt::Layout* lay = slot->unk28;
                changed2 = 1;
                nw4r::lyt::AnimTransform* anm = slot->unk38;
                lay->Animate(0);
                lay->UnbindAllAnimation();
                lay->BindAnimation(anm);
                lay->SetAnimationEnable(anm, true);
                reinterpret_cast<nw4r::lyt::AnimTransform*>(anm)->SetFrame(
0.0f);
                lay->Animate(0);
                slot->unk254 = 0xF;
            }
        }
        break;
    case 0xC:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk2C, 1.0f) != 0) {
            nw4r::lyt::Layout* lay = slot->unk28;
            nw4r::lyt::AnimTransform* anm = slot->unk30;
            lay->Animate(0);
            lay->UnbindAllAnimation();
            lay->BindAnimation(anm);
            lay->SetAnimationEnable(anm, true);
            reinterpret_cast<nw4r::lyt::AnimTransform*>(anm)->SetFrame(
0.0f);
            lay->Animate(0);
            slot->unk254 = 0xD;
        }
        break;
    case 0xD:
        changed2 = 1;
        advanceAnimTransform(slot->unk30, 1.0f);
        // Retail evaluates the gate as a skip-chain: any failed condition
        // falls straight through to the bind sequence; only all-pass plus
        // hp > 0 jumps past it.
        if (slot->unk230 == 4 && slot->unk240 == 0 && this->unk7C8 == 0) {
            if (slot->unk220 > 0.0f) {
                break;
            }
        }
        {
            nw4r::lyt::Layout* lay = slot->unk28;
            nw4r::lyt::AnimTransform* anm = slot->unk34;
            lay->Animate(0);
            lay->UnbindAllAnimation();
            lay->BindAnimation(anm);
            lay->SetAnimationEnable(anm, true);
            reinterpret_cast<nw4r::lyt::AnimTransform*>(anm)->SetFrame(
0.0f);
            lay->Animate(0);
            slot->unk254 = 0xE;
        }
        break;
    case 0xE:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk34, 1.0f) != 0) {
            slot->unk28->UnbindAllAnimation();
            slot->unk254 = 0xB;
        }
        break;
    case 0xF:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk38, 1.0f) != 0) {
            nw4r::lyt::Layout* lay = slot->unk28;
            nw4r::lyt::AnimTransform* anm = slot->unk3C;
            lay->Animate(0);
            lay->UnbindAllAnimation();
            lay->BindAnimation(anm);
            lay->SetAnimationEnable(anm, true);
            reinterpret_cast<nw4r::lyt::AnimTransform*>(anm)->SetFrame(
0.0f);
            lay->Animate(0);
            slot->unk254 = 0x10;
        }
        break;
    case 0x10:
        changed2 = 1;
        advanceAnimTransform(slot->unk3C, 1.0f);
        // Same skip-chain gate as case 0xD, keyed on mode 0.
        if (slot->unk230 == 0 && slot->unk240 == 0 && this->unk7C8 == 0) {
            if (slot->unk220 > 0.0f) {
                break;
            }
        }
        {
            nw4r::lyt::Layout* lay = slot->unk28;
            nw4r::lyt::AnimTransform* anm = slot->unk40;
            lay->Animate(0);
            lay->UnbindAllAnimation();
            lay->BindAnimation(anm);
            lay->SetAnimationEnable(anm, true);
            reinterpret_cast<nw4r::lyt::AnimTransform*>(anm)->SetFrame(
0.0f);
            lay->Animate(0);
            slot->unk254 = 0x11;
        }
        break;
    case 0x11:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk40, 1.0f) != 0) {
            slot->unk28->UnbindAllAnimation();
            slot->unk254 = 0xB;
        }
        break;
    default:
        break;
    }
    if (changed2 != 0) {
        slot->unk25C |= 0x200;
    }

    if (slot->unk78 != NULL) {
        if (slot->unk80 == 1) {
            if (advanceAnimTransform(slot->unk7C, 1.0f) != 0) {
                slot->unk80 = 2;
                reinterpret_cast<nw4r::lyt::AnimTransform*>(slot->unk7C)->SetFrame(
0.0f);
            }
        } else if (slot->unk80 == 2) {
            BtlDmg_QueueDraw(
                reinterpret_cast<CMenuBattleDamageQueue*>(&slot->unk74));
        }
        slot->unk78->Animate(0);
    }

    // Once the level-up cursor is armed, swap the gauge layout to its
    // alternate animation set and move the slot to state 3.
    if (slot->unk80 == 0 && slot->unk240 == 0 && this->unk7C8 != 0 &&
        slot->unk254 == 0xB) {
        nw4r::lyt::Pane* fp = slot->unk20->GetRootPane();
        fp->SetVisible(false);
        slot->unk18->SetAnimationEnable(slot->unk1C,
            true);
        slot->unk244 = 3;
    }
}

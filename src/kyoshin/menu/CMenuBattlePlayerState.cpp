#include "kyoshin/menu/CMenuBattlePlayerState.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#undef func_80043D90
#undef func_8017FD44
#undef isSceneLoading__Q22cf13CfGameManagerFv
#undef func_80043F18
// code_80135FDC.hpp declares lbl_eu_8066A208 as u32 (line 188);
// CfObjectMove.hpp (via the CBattleManager.hpp include above) declares it
// const float. This TU uses neither copy.
#define func_80043D90 menuBpsEnumListCtor5
#define func_8017FD44 menuBpsFd44Get5
#define isSceneLoading__Q22cf13CfGameManagerFv menuBpsCfGameMgrCond5
#define func_80043F18 menuBpsMoveEnumListGet5
// CfObjectModel.hpp (via CfObjectActor.hpp) declares this symbol as ml::CVec3;
// code_80135FDC.hpp re-types it as nw4r::math::VEC3 -> MWCC 10563. This TU
// never touches the zero vector, so rename the header-local decl away.
#define zero__Q22ml5CVec3 menuBpsZeroVecDecl5
#include "kyoshin/code_80135FDC.hpp"
#undef func_80043D90
#undef func_8017FD44
#undef isSceneLoading__Q22cf13CfGameManagerFv
#undef func_80043F18
#undef zero__Q22ml5CVec3
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/work/CProcess.hpp"

#include <stdio.h>
#include "decomp.h"
#include "functions.hpp"
#include <nw4r/math.h>
#include <revolution/GX.h>

// Batch 2026-07-14g: menu-bps-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14h: menu-bps-move owns Move exclusively; do not touch
// Term / cbRenderBefore.
// Batch 2026-07-14k: menu-bps-ctor lives in CMenuBattlePlayerState_ct.cpp (-O4,s).

struct CMenuBpsProcessShim {
    u8 unk00[0x10];
    void* vtable;
    u8 unk14[0x28];
    u32 callbacks[6]; // +0x3c
};

void func_8010B324(CMenuBattlePlayerStateSlot*);
extern "C" void* __dt__8010B444(CMenuBattlePlayerStateSlot*, s16);
extern "C" void __dl__FPv(void*);
extern "C" void* __dt__8010B444(CMenuBattlePlayerStateSlot* self, s16 del) {
    if (self && del > 0)
        __dl__FPv(self);
    return self;
}
extern "C" {
extern CMenuBattlePlayerState* lbl_eu_80663F48;
extern u32 lbl_eu_80663E28;
char lbl_eu_8052C1C0[];
char lbl_eu_8052C330[];
char lbl_eu_8052C42C[];
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void __dt__8CProcessFv(void* self, int flag);
void* __dt__22CMenuBattlePlayerStateFv(CMenuBattlePlayerState* self, int deleteFlag);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);


void __construct_array(void* ptr, void* ctor, void* dtor, u32 size, u32 n);
// func_800B8B94 is declared (extern "C", s32) by CAIAction.hpp, included
// transitively via CfObjectActor.hpp -> CBattleManager.hpp.
void func_8010D1B4(CMenuBattlePlayerState* self,
                   cf::CfObjectActor* actor,
                   CMenuBattlePlayerStateSlot* slot);
void func_8010D4B0(CMenuBattlePlayerState* self,
                   CMenuBattlePlayerStateSlot* slot, u32 index);
void func_8010D8D4(CMenuBattlePlayerState* self,
                   CMenuBattlePlayerStateSlot* slot);
// Unmangled retail name (extern "C" in code_80135FDC.hpp).
extern char lbl_eu_804FD720[];
extern const f32 lbl_eu_80666F90; // 1.0f
extern const f32 lbl_eu_80666F94; // 0.0f
extern const f32 lbl_eu_80666F98; // 110.0f
extern const f32 lbl_eu_80666F9C; // 86.0f
extern const double lbl_eu_80666FA8; // 0x4330000000000000 (int->double bias)
extern const f32 lbl_eu_80666FA0; // -178.0f
extern const f32 lbl_eu_80666FB0; // -1.0f
extern const f32 lbl_eu_80666FC0; // 100.0f
extern const f32 lbl_eu_80666FC4; // 360.0f
extern const f32 lbl_eu_80666FC8; // HP gauge width multiplier
}

extern "C" {
extern nw4r::lyt::ArcResourceAccessor* func_8012FDBC();
extern u32 func_801355BC();
extern u32 func_801355D8();
}

extern void playUISound(u32);

extern "C" {
extern s16 lbl_eu_80663F30[4];
extern s16 lbl_eu_80663F38[4];
extern s16 lbl_eu_80663F40[4];
}

typedef f32 (*GetF32Fn)(void*);
typedef u32 (*GetU32Fn)(void*);
typedef int (*GetIntFn)(void*);

template <typename Fn>
static inline Fn vslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
}

// Direct-store view of lyt::Pane::mTranslate (+0x2c) - retail inlines
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
    nw4r::lyt::ArcResourceAccessor* accessor = func_8012FDBC();
    unk64.createRegion(reinterpret_cast<int>(accessor), 0xE00,
                       lbl_eu_804FD720 + 0xF7, 0);

    {
        Class_8045F858 regionGuard(&unk64);
        char* tbl = lbl_eu_804FD720;
        // Loop invariants kept live across the whole pass (retail pins them
        // to nonvolatile f28-f31 / r24-r26).
        const f32 zeroF = lbl_eu_80666F94;
        const double intDbias = lbl_eu_80666FA8;
        const f32 yStep = lbl_eu_80666F9C;
        s16* colA = lbl_eu_80663F30;
        s16* colB = lbl_eu_80663F38;
        s16* colC = lbl_eu_80663F40;

        for (u8 i = 0; i < 3; i++) {
            CMenuBattlePlayerStateSlot& slot = mSlots[i];

            accessor = CUICfManager::func_801355F4();
            buildLayout(&slot.unk00, accessor, tbl + 0x10E);

            accessor = CUICfManager::func_801355F4();
            bindLayoutAnimTransform(slot.unk00, &slot.unk04, accessor, tbl + 0x12E);

            slot.unk00->SetAnimationEnable(slot.unk04, true);
            slot.unk04->SetFrame(0.0f);
            slot.unk00->Animate(0);

            accessor = CUICfManager::func_801355F4();
            buildLayout(&slot.unk08, accessor, tbl + 0x151);

            accessor = CUICfManager::func_801355F4();
            bindLayoutAnimTransform(slot.unk08, &slot.unk0C, accessor, tbl + 0x174);
            bindLayoutAnimTransform(slot.unk08, &slot.unk10, accessor, tbl + 0x19A);
            bindLayoutAnimTransform(slot.unk08, &slot.unk14, accessor, tbl + 0x1C5);

            slot.unk08->SetAnimationEnable(slot.unk10, false);
            slot.unk08->SetAnimationEnable(slot.unk14, false);
            slot.unk08->SetAnimationEnable(slot.unk0C, true);
            slot.unk0C->SetFrame(0.0f);
            slot.unk08->Animate(0);

            accessor = CUICfManager::func_801355F4();
            buildLayout(&slot.unk18, accessor, tbl + 0x1F0);

            accessor = CUICfManager::func_801355F4();
            bindLayoutAnimTransform(slot.unk18, &slot.unk1C, accessor, tbl + 0x210);

            {
                nw4r::lyt::Pane* rootPane = slot.unk18->GetRootPane();
                u32 fontHandle = func_801355BC();
                func_8013676C(rootPane, fontHandle);
            }
            {
                u32 fontAccessor = func_801355D8();
                setLayoutTextBoxFont(slot.unk18, tbl + 0x233, fontAccessor);
                setLayoutTextBoxFont(slot.unk18, tbl + 0x23C, fontAccessor);
            }

            {
                // Bounding-rect query; the four halfwords fan out into the
                // shared color tables (A gets all four, B/C only the last).
                unsigned long long rect = func_80139658(slot.unk18, tbl + 0x245, 1);
                s16 v0 = static_cast<s16>(rect);
                s16 v1 = static_cast<s16>(rect >> 16);
                s16 v2 = static_cast<s16>(rect >> 32);
                s16 v3 = static_cast<s16>(rect >> 48);
                colA[0] = v0;
                colA[1] = v1;
                colA[2] = v2;
                colA[3] = v3;
                colB[3] = v3;
                colC[3] = v3;
            }

            {
                nw4r::lyt::Pane* rp = slot.unk18->GetRootPane();
                slot.unk44 = rp->FindPaneByName(tbl + 0x233, true);
                slot.unk48 = rp->FindPaneByName(tbl + 0x23C, true);
                slot.unk4C = rp->FindPaneByName(tbl + 0x245, true);
                slot.unk50 = rp->FindPaneByName(tbl + 0x24C, true);
                slot.unk54 = rp->FindPaneByName(tbl + 0x255, true);
                slot.unk58 = rp->FindPaneByName(tbl + 0x262, true);
                slot.unk5C = rp->FindPaneByName(tbl + 0x26F, true);
                slot.unk60 = rp->FindPaneByName(tbl + 0x27C, true);
                slot.unk64 = rp->FindPaneByName(tbl + 0x289, true);
                slot.unk68 = rp->FindPaneByName(tbl + 0x295, true);
                slot.unk6C = rp->FindPaneByName(tbl + 0x2A1, true);
                slot.unk70 = rp->FindPaneByName(tbl + 0x2AE, true);
            }

            slot.unk25C = 7;

            accessor = CUICfManager::func_801355F4();
            buildLayout(&slot.unk20, accessor, tbl + 0x2BB);

            accessor = CUICfManager::func_801355F4();
            bindLayoutAnimTransform(slot.unk20, &slot.unk24, accessor, tbl + 0x2DE);

            slot.unk20->SetAnimationEnable(slot.unk24, true);
            slot.unk24->SetFrame(0.0f);
            slot.unk20->Animate(0);

            accessor = CUICfManager::func_801355F4();
            buildLayout(&slot.unk28, accessor, tbl + 0x304);

            accessor = CUICfManager::func_801355F4();
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
                // func_8010B324: 0x60-stride body then 0xC-stride remainder).
                MenuBpsSlotInit init;
                init.unk74 = CUICfManager::func_801355F4();
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
                u32 fontHandle = func_801355BC();
                func_8013676C(subRoot, fontHandle);
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

        accessor = CUICfManager::func_801355F4();
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
            int* party = func_8009ECB0();
            actors[0] = func_800B8B94(party[1]);
            actors[1] = func_800B8B94(party[2]);
            actors[2] = func_800B8B94(party[3]);
        }

        {
            // Fresh NV set for this pass (retail reloads FA8/F94/F90/FB8 here).
            const double intDbias2 = lbl_eu_80666FA8;
            const f32 zeroF2 = lbl_eu_80666F94;
            const double uIntBias = lbl_eu_80666FB8;
            const f32 oneF = lbl_eu_80666F90;

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
                    actor->CActorParam_UnkVirtualFunc85()));
                slot.unk228 = static_cast<f32>(
                    actor->CActorParam_UnkVirtualFunc87());
                if (oneF == slot.unk228) {
                    slot.unk224 = zeroF2;
                }
                if (slot.unk22C < zeroF2) {
                    slot.unk22C = slot.unk224;
                }
                slot.unk218 = slot.unk21C = actor->CActorParam_UnkVirtualFunc91();
                slot.unk210 = static_cast<u32>(
                    actor->CActorParam_UnkVirtualFunc37());
                slot.unk214 = static_cast<u32>(
                    actor->CActorParam_UnkVirtualFunc38());
            }
        }

        mScn->addRenderCB(static_cast<IScnRender*>(this), 0xA, 0);

        unk64.func_8045F810();
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

    unk7D0.func_8045F778();
    unk64.func_8045F778();
    lbl_eu_80663F48 = NULL;
}

void CMenuBattlePlayerState::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_CASES 8c9.
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }
    if (unk7C9 != 0) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFE40000u) {
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

        int* party = func_8009ECB0();
        u8 fi = 0;
        while (fi < 3) {
            // Retail: clrlslwi fi; add party; lwz 4(r3); ... stwx actors,same shift.
            int* p = party + fi;
            void* actor = func_800B8B94(p[1]);
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
            func_8010D1B4(this, actor, slot);

            hp = static_cast<u32>(actor->CActorParam_UnkVirtualFunc37());
            maxHp = static_cast<u32>(actor->CActorParam_UnkVirtualFunc38());

            // Retail: lfs f26, zero pool -- not fmr from a zero NV.
            hpRatio = lbl_eu_80666F94;
            if (hp == 0) {
                goto skip_ratio;
            }
            if (maxHp == 0) {
                goto skip_ratio;
            }
            hpRatio =
                lbl_eu_80666FC0 * (static_cast<f32>(hp) / static_cast<f32>(maxHp));
        skip_ratio:

            slot->unk204 = static_cast<u8>(
                reinterpret_cast<MenuBpsActorFields*>(actor)->unk3f28);
            slot->unk208 = reinterpret_cast<u32>(
                actor->CActorParam_UnkVirtualFunc127());

            {
                u32 statusId = actor->CActorParam_UnkVirtualFunc29();
                if (slot->unk20C != statusId) {
                    slot->unk25C |= 0x1;
                }
            }
            slot->unk20C = actor->CActorParam_UnkVirtualFunc29();

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
            slot->unk218 = actor->CActorParam_UnkVirtualFunc91();
            slot->unk220 = hpRatio;

            {
                // Retail converts this getter as signed here (the pad
                // declared it int); hop through int to keep the flavor.
                f32 tB = static_cast<f32>(static_cast<int>(
                    actor->CActorParam_UnkVirtualFunc87()));
                // Constant on the left → retail fcmpu cr0,fNV,f0 (not f0,fNV).
                if (lbl_eu_80666F90 == tB) {
                    f32 tB2 = static_cast<f32>(static_cast<int>(
                        actor->CActorParam_UnkVirtualFunc87()));
                    if (slot->unk228 != tB2) {
                        slot->unk25C |= 0x4;
                    }
                    slot->unk224 = lbl_eu_80666F94;
                    slot->unk228 = lbl_eu_80666F90;
                    goto tension_done;
                }

                {
                    f32 tA = static_cast<f32>(actor->CActorParam_UnkVirtualFunc85());
                    if (slot->unk224 != tA) {
                        goto tension_flag;
                    }
                    f32 tB2 = static_cast<f32>(static_cast<int>(
                        actor->CActorParam_UnkVirtualFunc87()));
                    if (slot->unk228 == tB2) {
                        goto tension_store;
                    }
                }
            tension_flag:
                slot->unk25C |= 0x4;
            tension_store:
                slot->unk224 =
                    static_cast<f32>(actor->CActorParam_UnkVirtualFunc85());
                slot->unk228 = static_cast<f32>(static_cast<int>(
                    actor->CActorParam_UnkVirtualFunc87()));
            }
        tension_done:

            if (slot->unk22C < lbl_eu_80666F94) {
                slot->unk22C = slot->unk224;
            }

            {
                s16* pair = reinterpret_cast<s16*>(
                    actor->CActorParam_UnkVirtualFunc152());
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
                        int id = player->CObjectParam_UnkVirtualFunc5();
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
                func_8010D4B0(this, slot, i);
                break;
            case 1: {
                u32 a = advanceAnimTransform(slot->unk04, lbl_eu_80666F90);
                u32 b = advanceAnimTransform(slot->unk1C, lbl_eu_80666F90);
                slot->unk25C = (slot->unk25C & ~0x300u) | 0xC0u;
                if (a != 0 && b != 0) {
                    slot->unk244 = 2;
                }
                break;
            }
            case 2:
                func_8010D8D4(this, slot);
                break;
            case 3: {
                u32 a = func_80137510(slot->unk04,
                                      lbl_eu_80666F90);
                u32 b = func_80137510(slot->unk1C,
                                      lbl_eu_80666F90);
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
            if (advanceAnimTransform(unk7E8, lbl_eu_80666F90) != 0) {
                unk7F4 = 1;
                unk7F8 = 2;
                unk7E4->SetAnimationEnable(unk7E8, false);
                unk7E4->SetAnimationEnable(unk7EC, false);
                unk7E4->SetAnimationEnable(unk7F0, true);
                unk7F0->SetFrame(lbl_eu_80666F94);
            }
            break;
        case 2:
            advanceAnimTransform(unk7F0, lbl_eu_80666F90);
            break;
        case 3:
            if (func_80137510(unk7E8, lbl_eu_80666F90) !=
                0) {
                unk7F4 = 1;
                unk7F8 = 0;
            }
            break;
        case 4:
            if (advanceAnimTransform(unk7EC, lbl_eu_80666F90) != 0) {
                unk7EC->SetFrame(lbl_eu_80666F94);
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
            pos.z = lbl_eu_80666F94;
            MenuBpsPaneXlate* px =
                reinterpret_cast<MenuBpsPaneXlate*>(pane);
            px->field_0x2c = pos.x;
            px->field_0x30 = pos.y;
            px->field_0x34 = pos.z;
        }

        unk7E4->Animate(0);

        if (static_cast<int>(fullHpCount) == static_cast<int>(aliveCount)) {
            f32 t = unk7C4 + lbl_eu_80666F90;
            unk7C4 = t;
            if (t >= lbl_eu_80666FC4) {
                if (unk7C8 == 0) {
                    playUISound(0x9a);
                }
                unk7C8 = 1;
            }
        } else {
            unk7C4 = lbl_eu_80666F94;
            unk7C8 = 0;
        }
    }
done:
    ;
}

void CMenuBattlePlayerState::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_CASES 8c9.
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }
    if (unk7C9 != 0) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFE40000u) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

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
void func_8010B324(CMenuBattlePlayerStateSlot* slot){
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
extern "C" int func_8010CE48() { return (int)lbl_eu_80663F48; }
extern "C" void func_8010CF5C(CMenuBattlePlayerState* p) {
    p->unk7C9 = 1;
}
extern "C" void sinit_8010E9F8() { lbl_eu_80663F30[3] = 0xff; lbl_eu_80663F30[2] = 0xff; lbl_eu_80663F30[1] = 0xff; lbl_eu_80663F30[0] = 0xff; lbl_eu_80663F38[3] = 0xff; lbl_eu_80663F38[2] = 0x5c; lbl_eu_80663F38[1] = 0x92; lbl_eu_80663F38[0] = 0xb9; lbl_eu_80663F40[3] = 0xff; lbl_eu_80663F40[2] = 0x50; lbl_eu_80663F40[1] = 0x50; lbl_eu_80663F40[0] = 0x50; }
// This-adjusting deleting-dtor thunk (secondary base at +0x58):
// retail emits `subi r3, r3, 0x58; b __dt__22CMenuBattlePlayerStateFv`.
// Forwarding deleteFlag untouched keeps the tail call (no bl).
extern "C" void* func_8010EA4C(void* self, int deleteFlag) {
    return __dt__22CMenuBattlePlayerStateFv(
        reinterpret_cast<CMenuBattlePlayerState*>((char*)self - 0x58),
        deleteFlag);
}
// IScnRender cbRenderBefore this-adjusting thunk (retail: subi r3,-0x5c; b cbRenderBefore__22CMenuBattlePlayerStateFv)
extern "C" void func_8010EA54(void* self) {
    reinterpret_cast<CMenuBattlePlayerState*>((char*)self - 0x5c)->cbRenderBefore();
}
// This-adjusting deleting-dtor thunk (IScnRender base at +0x5c):
// retail emits `subi r3, r3, 0x5c; b __dt__22CMenuBattlePlayerStateFv`.
// Forwarding deleteFlag untouched keeps the tail call (no li/bl).
extern "C" void* func_8010EA5C(void* self, int deleteFlag) {
    return __dt__22CMenuBattlePlayerStateFv(
        reinterpret_cast<CMenuBattlePlayerState*>((char*)self - 0x5c),
        deleteFlag);
}

// Runtime array-destruction helper (MWCC runtime library).
extern "C" void __destroy_arr(void* ptr, void* dtor, u32 size, u32 count);

// Battle-menu imports (retail unmangled names; see CUIBattleManager.hpp for
// func_8012FAA8 semantics - pane/material helpers shared by menu TUs).
extern "C" void* func_8012FA78();
extern "C" void func_8012FAA8();
extern "C" void func_80137F88(void* pane, u32 value);
extern "C" void func_8010A940(void* obj, u32 a, u32 b, u32 c);
extern "C" void* __ct__CMenuBattlePlayerState(void* self, CScn* scn);

#pragma optimize_for_size off
// ---------------------------------------------------------------------------
// Target us-8010d8a8: singleton factory. Allocates the player-state process
// from work memory, runs its constructor, stores the global pointer, and
// registers it as a child of `parent` (registration happens even when the
// allocation failed, matching retail). Returns NULL when already created.
// ---------------------------------------------------------------------------
CMenuBattlePlayerState* func_8010CDCC(CProcess* parent, CScn* scn) {
    if (lbl_eu_80663F48 != NULL) {
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
    lbl_eu_80663F48 = obj;
    obj->Regist(parent, false);
    return lbl_eu_80663F48;
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
            __dl__FPv(self);
        }
    }
    return self;
}
// ---------------------------------------------------------------------------
// Target us-8010dbb0: per-slot animation select. Picks an entry from the
// shared battle-UI animation table indexed by the slot number, chosen by
// `mode`; a zero entry (or missing table) resets via func_8012FAA8.
// ---------------------------------------------------------------------------
struct MenuBpsModeEntry {
    u32 unk00;
    u32 unk04;
    u32 unk08;
};

// Default mode: the func_8010D8D4 caller passes no third argument (retail
// leaves r5 stale from an earlier callee), so the parameter defaults.
extern "C" void func_8010D0D4(CMenuBattlePlayerState* self,
                              CMenuBattlePlayerStateSlot* slot, s32 mode = 0) {
    MenuBpsModeEntry* table =
        reinterpret_cast<MenuBpsModeEntry*>(func_8012FA78());
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
            func_80137F88(slot->unk5C, sel);
            func_80137F88(slot->unk60, sel);
            func_80137F88(slot->unk64, sel);
            func_80137F88(slot->unk68, sel);
        } else {
            func_8012FAA8();
        }
    }
}

// ---------------------------------------------------------------------------
// Target us-8010d92c: routes a party-visual update for the acting actor.
// Resolves the actor's handle, requires bit 1 of its +0x64 flags, matches the
// id byte at handle+0x8C against the first three party-table words, and
// forwards to the per-slot updater at (state + i*0x270 + 0xE8).
// ---------------------------------------------------------------------------
void func_8010CE50(int id, u32 a, u32 b, u32 c) {
    CMenuBattlePlayerState* state = lbl_eu_80663F48;
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
    PartyData party = *(PartyData*)((u8*)func_8009ECB0() + 4);

    for (u8 i = 0; i < 3; i++) {
        if (idxByte == static_cast<int>(party.w[i])) {
            char* slotView = reinterpret_cast<char*>(state) + static_cast<u32>(i) * 0x270;
            func_8010A940(slotView + 0xE8, a, b, c);
            return;
        }
    }
}
#pragma optimize_for_size on

// Battle-party-slot per-frame update: finds the first free arts slot, derives
// an HP-gauge width ratio from a BDAT lookup, and writes it into the
// "N_HpGauge_Max" pane size.
extern "C" void func_8010CF68(CMenuBattlePlayerState* self,
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
            result = static_cast<u8>(func_8013600C(sA, sB,
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
    if (hpF != lbl_eu_80666F94 && resF != lbl_eu_80666F94) {
        ok = 1;
    }
    if (ok) {
        ratio = hpF / resF;
    } else {
        ratio = lbl_eu_80666F94;
    }

    if (ratio > lbl_eu_80666F90) {
        ratio = lbl_eu_80666F90;
    }
    if (slot->unk204 > 8) {
        ratio = lbl_eu_80666F94;
    }

    nw4r::lyt::Pane* pane = slot->unk54;
    if (pane != NULL) {
        pane->SetSize(nw4r::lyt::Size(lbl_eu_80666FC8 * ratio,
                                      pane->GetSize().height));
    }
}
// Keep names distinct from Move's extern callees or MWCC DCE's the bl sites.

// Cast-only iface for the polymorphic member embedded at actor+0x8 (MWCC RTTI
// places two hidden slots before the first declared virtual). The icon-query
// virtuals return void and fill fields on the inner object itself (id at
// +0x0c, state at +0x30; 0x800 = empty slot); retail's caller keeps r3
// pointing at the inner object across the call.
// Battle-state head of the actor (cf::CBattleState base at +0x8): the
// UnkVirtualFunc21 icon query fills the state object in place (id at
// battle-state +0xC, state word at +0x30, 0x800 = empty slot); retail's
// caller reads both straight off the sub-object after the call.
struct MenuBpsActorBattleHead {
    u8 pad00[0x14];       // actor+0x14 == CBattleState+0x0C
    u16 unk14;
    u8 pad16[0x38 - 0x16];
    u32 unk38;            // actor+0x38 == CBattleState+0x30
};

// Actor field holding the level icon id (fed in as arts-slot entry 0x20).
struct MenuBpsMoveActorFields {
    u8 pad00[0x1530];
    u32 unk1530; // +0x1530: level icon id (u16)
};

// Pane flag byte view (layout-visible visible-gauge enable bit at +0xBB).
struct MenuBpsPaneFlagsView {
    u8 pad00[0xBB];
    u8 flagsBB;
};

// HP/tension gauge pane fields written directly.
struct MenuBpsGaugePaneView {
    u8 pad00[0x4C];
    f32 unk4C; // +0x4C: current gauge width
    f32 unk50; // +0x50: max gauge width
};

// AnimTransform frame float stored inline (SetFrame folds to this store).
struct MenuBpsAnimFrameView {
    u8 pad00[0x10];
    f32 frame;
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
void func_8010D1B4(CMenuBattlePlayerState* self,
                   cf::CfObjectActor* actor,
                   CMenuBattlePlayerStateSlot* slot) {
    MenuBpsPaneFlagsView* starPane;

    // Tension timer: raises flag bit 0x800 when it exceeds FDC.
    slot->unk264 = slot->unk264 + lbl_eu_80666F90;
    slot->unk25C = slot->unk25C & ~0x800u;
    if (slot->unk264 > lbl_eu_80666FDC) {
        slot->unk25C = slot->unk25C | 0x800u;
        slot->unk264 = lbl_eu_80666F94;

        // Pass 1: scan the actor's live arts-icon slots from last match + 1,
        // binding the first hit's icon and lighting the level star pane.
        starPane = reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk6C);
        char* tbl = lbl_eu_804FD720;
        const u32 texKey = 0x74696D67;
        starPane->flagsBB &= static_cast<u8>(~0x01);

        u32 cursor = (slot->unk268 + 1) & 0xFF;
        while (true) {
            if ((cursor & 0xFF) >= 0x20) {
                cursor = 0;
            }
            cf::CBattleStateEntry* rec =
                actor->CBattleState_UnkVirtualFunc22(
                    static_cast<int>(cursor & 0xFF));
            if (rec->unk30 != 0x800 && rec->unk0C != 0 && rec->unk0C != 0xF &&
                rec->unk0C != 0x10 && rec->unk0C != 0x12) {
                u16 nameId = static_cast<u16>(
                    func_80136254(lbl_eu_806640E0, tbl + 0x42B, rec->unk0C));
                if (nameId != 0) {
                    char* tex = func_80138F78(nameId);
                    u32 bound = reinterpret_cast<u32>(
                        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                            func_801355F4())
                            ->GetResource(texKey, tex, NULL));
                    if (bound != 0) {
                        starPane->flagsBB =
                            (starPane->flagsBB &
                             static_cast<u8>(~0x01)) |
                            0x01;
                        func_80137F88(starPane, bound);
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
        starPane = reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk70);
        starPane->flagsBB &= static_cast<u8>(~0x01);

        u16 ids[33];
        // u8 counter: retail zero-extends it for the Func21 arg and derives
        // the ids index as j*2 from the extended value.
        for (u8 j = 0; j < 32; j++) {
            actor->CBattleState_UnkVirtualFunc21(j);
            MenuBpsActorBattleHead* head =
                reinterpret_cast<MenuBpsActorBattleHead*>(actor);
            ids[j] = (head->unk38 == 0x800u) ? 0 : head->unk14;
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
                    char* tex = func_80138F78(0x13D);
                    u32 bound = reinterpret_cast<u32>(
                        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                            func_801355F4())
                            ->GetResource(texKey2, tex, NULL));
                    if (bound != 0) {
                        starPane->flagsBB =
                            (starPane->flagsBB &
                             static_cast<u8>(~0x01)) |
                            0x01;
                        func_80137F88(starPane, bound);
                        slot->unk26C =
                                static_cast<u32>(static_cast<u8>(cur2));
                        return;
                    }
                } else {
                    u16 nameId = static_cast<u16>(
                        func_80136254(lbl_eu_806640E0, tbl2 + 0x42B, v));
                    if (nameId != 0) {
                        char* tex = func_80138F78(nameId);
                        u32 bound = reinterpret_cast<u32>(
                            reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
                                func_801355F4())
                                ->GetResource(texKey2, tex, NULL));
                        if (bound != 0) {
                            starPane->flagsBB =
                            (starPane->flagsBB &
                             static_cast<u8>(~0x01)) |
                            0x01;
                            func_80137F88(starPane, bound);
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
// width (quantized through lbl_eu_804FD6E0), advances mode/prev snapshots,
// toggles the party/tension star panes by HP state, resets the tension anims
// and re-arms the per-slot state machine. `index` nonzero suppresses the
// playUISound(0x99) jingle on first activation.
// ---------------------------------------------------------------------------
void func_8010D4B0(CMenuBattlePlayerState* self,
                   CMenuBattlePlayerStateSlot* slot, u32 index) {
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
    func_80136D74(reinterpret_cast<nw4r::lyt::Layout*>(slot->unk44), buf, 0);
    func_80136D74(
        reinterpret_cast<nw4r::lyt::Layout*>(slot->unk48),
        func_80136190(lbl_eu_804FD720 + 0x406, lbl_eu_804FD720 + 0x410, 2),
        0);

    if (slot->unk25C & 0x2) {
        slot->unk25C &= ~0x2u;
        func_80136C98(slot->unk4C, slot->unk210);
        func_80136C98(slot->unk50, slot->unk214);
        // Color-set swap keyed on full-HP / dead / other.
        if (slot->unk210 == slot->unk214) {
            func_8013996C(slot->unk4C, lbl_eu_80663F38, 1);
        } else if (slot->unk210 == 0) {
            func_8013996C(slot->unk4C, lbl_eu_80663F40, 1);
        } else {
            func_8013996C(slot->unk4C, lbl_eu_80663F30, 1);
        }
    }

    func_8010CF68(self, slot);

    if (slot->unk25C & 0x4) {
        slot->unk25C &= ~0x4u;
        f32 ratio = slot->unk224 / slot->unk228;
        MenuBpsGaugePaneView* pane =
            reinterpret_cast<MenuBpsGaugePaneView*>(slot->unk58);
        if (pane != NULL) {
            f32 scaled = lbl_eu_80666FD0 * ratio;
            f32 widthF = lbl_eu_80666FCC * scaled;
            // Retail copies the width table onto the stack before indexing.
            f32 widths[5];
            widths[0] = lbl_eu_804FD6E0[0];
            widths[1] = lbl_eu_804FD6E0[1];
            widths[2] = lbl_eu_804FD6E0[2];
            widths[3] = lbl_eu_804FD6E0[3];
            widths[4] = lbl_eu_804FD6E0[4];
            // Signed int convert: MWCC emits inline fctiwz + signed-int->float
            // bias (lbl_eu_80666FB8 == 0x4330000080000000), not __cvt_fp2unsigned.
            s32 w = widthF;
            slot->unk24C = w;
            f32 prevMax = pane->unk50;
            // u32 -> f32 goes through a biased double in MWCC.
            f32 frac = ratio - lbl_eu_80666FD8 * w;
            f32 q = frac / lbl_eu_80666FD8;
            f32 val = lbl_eu_80666FD4 * q + widths[w];
            pane->unk4C = val;
            pane->unk50 = prevMax;
        }
    }

    slot->unk234 = slot->unk230;
    slot->unk23C = slot->unk238;
    func_8010D0D4(self, slot, slot->unk230);

    // Star-pane visibility follows the smoothed HP ratio.
    if (slot->unk220 == lbl_eu_80666F94) {
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB |= 0x01;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB |= 0x01;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB &=
            static_cast<u8>(~0x01);
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB &=
            static_cast<u8>(~0x01);
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk08->GetRootPane())
            ->flagsBB &= static_cast<u8>(~0x01);
    } else {
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB &=
            static_cast<u8>(~0x01);
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB &=
            static_cast<u8>(~0x01);
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB |= 0x01;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB |= 0x01;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk08->GetRootPane())
            ->flagsBB |= 0x01;
    }

    slot->unk28->UnbindAllAnimation();
    slot->unk254 = 0xB;
    reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk24)->frame =
        lbl_eu_80666F94;
    reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk20->GetRootPane())
        ->flagsBB |= 0x01;
    slot->unk25C &= ~0x40000000u;
    reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk0C)->frame =
        lbl_eu_80666F94;
    reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk10)->frame =
        lbl_eu_80666F94;
    slot->unk08->SetAnimationEnable(slot->unk10, false);
    slot->unk08->SetAnimationEnable(slot->unk14, false);
    slot->unk08->SetAnimationEnable(slot->unk0C, true);
    reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk0C)->frame =
        lbl_eu_80666F94;
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
extern "C" void func_8010D8D4(CMenuBattlePlayerState* self,
                              CMenuBattlePlayerStateSlot* slot) {
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
        func_80136D74(reinterpret_cast<nw4r::lyt::Layout*>(slot->unk44), buf, 0);
        func_80136D74(
            reinterpret_cast<nw4r::lyt::Layout*>(slot->unk48),
            func_80136190(tbl + 0x406, tbl + 0x410, 2),
            0);
    }

    if (slot->unk25C & 2) {
        slot->unk25C &= ~2u;
        func_80136C98(slot->unk4C, slot->unk210);
        func_80136C98(slot->unk50, slot->unk214);
        // Color-set swap keyed on full-HP / dead / other.
        if (slot->unk210 == slot->unk214) {
            func_8013996C(slot->unk4C, lbl_eu_80663F38, 1);
        } else if (slot->unk210 == 0) {
            func_8013996C(slot->unk4C, lbl_eu_80663F40, 1);
        } else {
            func_8013996C(slot->unk4C, lbl_eu_80663F30, 1);
        }
    }

    func_8010CF68(self, slot);

    if (slot->unk25C & 4) {
        slot->unk25C &= ~4u;
        f32 ratio = slot->unk224 / slot->unk228;
        MenuBpsGaugePaneView* pane =
            reinterpret_cast<MenuBpsGaugePaneView*>(slot->unk58);
        if (pane != NULL) {
            f32 scaled = lbl_eu_80666FD0 * ratio;
            f32 widthF = lbl_eu_80666FCC * scaled;
            // Retail copies the width table onto the stack before indexing.
            f32 widths[5];
            widths[0] = lbl_eu_804FD6E0[0];
            widths[1] = lbl_eu_804FD6E0[1];
            widths[2] = lbl_eu_804FD6E0[2];
            widths[3] = lbl_eu_804FD6E0[3];
            widths[4] = lbl_eu_804FD6E0[4];
            s32 w = widthF;
            slot->unk24C = w;
            f32 prevMax = pane->unk50;
            f32 q = (ratio - lbl_eu_80666FD8 * w) / lbl_eu_80666FD8;
            f32 val = lbl_eu_80666FD4 * q + widths[w];
            pane->unk4C = val;
            pane->unk50 = prevMax;
        }
    }

    if (slot->unk220 <= lbl_eu_80666F94) {
        // HP empty: light the party/tension stars, hide the arts/heal stars.
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB =
            (reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB &
             ~0x100u) |
            1u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB =
            (reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB &
             ~0x100u) |
            1u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB =
            reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB &
            ~0x100u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB =
            reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB &
            ~0x100u;
        MenuBpsPaneFlagsView* mainRoot = reinterpret_cast<MenuBpsPaneFlagsView*>(
            slot->unk08->GetRootPane());
        mainRoot->flagsBB = mainRoot->flagsBB & ~0x100u;

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
            reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk14)->frame =
                lbl_eu_80666F94;
            slot->unk08->Animate(0);
            slot->unk08->SetAnimationEnable(slot->unk14,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk10,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk10,
                                                                   true);
            reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk10)->frame =
                lbl_eu_80666F94;
            slot->unk08->Animate(0);
            slot->unk08->SetAnimationEnable(slot->unk10,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk14,
                                                                   false);
            slot->unk08->SetAnimationEnable(slot->unk0C,
                                                                   true);
            reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk0C)->frame =
                lbl_eu_80666F94;
            slot->unk08->Animate(0);
            slot->unk250 = 6;
        }
        slot->unk230 = 0;
        slot->unk238 = 0;
    } else {
        u32 f5 = slot->unk25C;
        slot->unk25C = f5 & ~0x1000u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB =
            reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB &
            ~0x100u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB =
            reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB &
            ~0x100u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB =
            (reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB &
             ~0x100u) |
            1u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB =
            (reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB &
             ~0x100u) |
            1u;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk08->GetRootPane())
            ->flagsBB = (reinterpret_cast<MenuBpsPaneFlagsView*>(
                             slot->unk08->GetRootPane())
                             ->flagsBB &
                         ~0x100u) |
            1u;
    }

    {
        u32 curMode = slot->unk230;
        u32 prevMode = slot->unk234;
        if (curMode != prevMode || slot->unk238 != slot->unk23C) {
            func_8010D0D4(self, slot);

            if (slot->unk260 == 0) {
                void* player = cf::CfGameManager::getPlayer(0);
                if (player != NULL) {
            func_8016FE34(player);
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
                        MenuBpsPaneFlagsView* flashA =
                            reinterpret_cast<MenuBpsPaneFlagsView*>(
                                slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x430, true));
                        flashA->flagsBB &= static_cast<u8>(~0x01);
                        MenuBpsPaneFlagsView* flashB =
                            reinterpret_cast<MenuBpsPaneFlagsView*>(
                                slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x43b, true));
                        flashB->flagsBB =
                            (flashB->flagsBB & static_cast<u8>(~0x01)) |
                            0x01;
                        if (!func_8017FD44()) {
                            playUISound(0x63);
                        }
                    } else if (delta > 0) {
                        MenuBpsPaneFlagsView* flashA =
                            reinterpret_cast<MenuBpsPaneFlagsView*>(
                                slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x430, true));
                        flashA->flagsBB =
                            (flashA->flagsBB & static_cast<u8>(~0x01)) |
                            0x01;
                        MenuBpsPaneFlagsView* flashB =
                            reinterpret_cast<MenuBpsPaneFlagsView*>(
                                slot->unk20->GetRootPane()->FindPaneByName(
                                    tbl + 0x43b, true));
                        flashB->flagsBB &= static_cast<u8>(~0x01);
                        if (!func_8017FD44()) {
                            playUISound(0x64);
                        }
                    }
                    if (delta != 0) {
                        slot->unk25C |= 0x40000000;
                    }
                }
                }
            }
            slot->unk234 = slot->unk230;
            slot->unk23C = slot->unk238;
            slot->unk260 = (lbl_eu_80663E24 >> 26) & 1;
        }
    }

    if (slot->unk25C & 0x40000000) {
        slot->unk25C = slot->unk25C | 0x400;
        if (advanceAnimTransform(slot->unk24, lbl_eu_80666F90) != 0) {
            reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk24)->frame =
                lbl_eu_80666F94;
            slot->unk25C = slot->unk25C & ~0x40000000u;
        }
    }

    // Tension anim state machine (unk250).
    u32 changed1 = 0;
    switch (static_cast<s32>(slot->unk250)) {
    case 6:
        if (slot->unk220 <= lbl_eu_80666FE0) {
            if (slot->unk220 > lbl_eu_80666F94) {
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
        if (advanceAnimTransform(slot->unk0C, lbl_eu_80666F90) != 0) {
            if (slot->unk220 <= lbl_eu_80666FE4) {
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, true);
                slot->unk250 = 0xa;
            } else if (slot->unk220 <= lbl_eu_80666FE0) {
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
        if (advanceAnimTransform(slot->unk10, lbl_eu_80666F90) != 0) {
            if (slot->unk220 <= lbl_eu_80666FE4) {
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, true);
                slot->unk250 = 0xa;
            } else if (slot->unk220 > lbl_eu_80666FE0) {
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
        if (func_80137510(slot->unk0C, lbl_eu_80666F90) != 0) {
            slot->unk250 = 6;
        }
        break;
    case 0xa:
        changed1 = 1;
        if (advanceAnimTransform(slot->unk14, lbl_eu_80666F90) != 0) {
            if (slot->unk220 > lbl_eu_80666FE0) {
                slot->unk08->SetAnimationEnable(
                    slot->unk10, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, true);
                slot->unk250 = 9;
            } else if (slot->unk220 > lbl_eu_80666FE4) {
                slot->unk08->SetAnimationEnable(
                    slot->unk14, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk0C, false);
                slot->unk08->SetAnimationEnable(
                    slot->unk10, true);
                slot->unk250 = 8;
            } else if (slot->unk220 <= lbl_eu_80666F94) {
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
        if (slot->unk220 > lbl_eu_80666F94) {
            if (slot->unk230 == 4) {
                nw4r::lyt::Layout* lay = slot->unk28;
                changed2 = 1;
                nw4r::lyt::AnimTransform* anm = slot->unk2C;
                lay->Animate(0);
                lay->UnbindAllAnimation();
                lay->BindAnimation(anm);
                lay->SetAnimationEnable(anm, true);
                reinterpret_cast<MenuBpsAnimFrameView*>(anm)->frame =
                    lbl_eu_80666F94;
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
                reinterpret_cast<MenuBpsAnimFrameView*>(anm)->frame =
                    lbl_eu_80666F94;
                lay->Animate(0);
                slot->unk254 = 0xF;
            }
        }
        break;
    case 0xC:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk2C, lbl_eu_80666F90) != 0) {
            nw4r::lyt::Layout* lay = slot->unk28;
            nw4r::lyt::AnimTransform* anm = slot->unk30;
            lay->Animate(0);
            lay->UnbindAllAnimation();
            lay->BindAnimation(anm);
            lay->SetAnimationEnable(anm, true);
            reinterpret_cast<MenuBpsAnimFrameView*>(anm)->frame =
                lbl_eu_80666F94;
            lay->Animate(0);
            slot->unk254 = 0xD;
        }
        break;
    case 0xD:
        changed2 = 1;
        advanceAnimTransform(slot->unk30, lbl_eu_80666F90);
        // Retail evaluates the gate as a skip-chain: any failed condition
        // falls straight through to the bind sequence; only all-pass plus
        // hp > 0 jumps past it.
        if (slot->unk230 == 4 && slot->unk240 == 0 && self->unk7C8 == 0) {
            if (slot->unk220 > lbl_eu_80666F94) {
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
            reinterpret_cast<MenuBpsAnimFrameView*>(anm)->frame =
                lbl_eu_80666F94;
            lay->Animate(0);
            slot->unk254 = 0xE;
        }
        break;
    case 0xE:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk34, lbl_eu_80666F90) != 0) {
            slot->unk28->UnbindAllAnimation();
            slot->unk254 = 0xB;
        }
        break;
    case 0xF:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk38, lbl_eu_80666F90) != 0) {
            nw4r::lyt::Layout* lay = slot->unk28;
            nw4r::lyt::AnimTransform* anm = slot->unk3C;
            lay->Animate(0);
            lay->UnbindAllAnimation();
            lay->BindAnimation(anm);
            lay->SetAnimationEnable(anm, true);
            reinterpret_cast<MenuBpsAnimFrameView*>(anm)->frame =
                lbl_eu_80666F94;
            lay->Animate(0);
            slot->unk254 = 0x10;
        }
        break;
    case 0x10:
        changed2 = 1;
        advanceAnimTransform(slot->unk3C, lbl_eu_80666F90);
        // Same skip-chain gate as case 0xD, keyed on mode 0.
        if (slot->unk230 == 0 && slot->unk240 == 0 && self->unk7C8 == 0) {
            if (slot->unk220 > lbl_eu_80666F94) {
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
            reinterpret_cast<MenuBpsAnimFrameView*>(anm)->frame =
                lbl_eu_80666F94;
            lay->Animate(0);
            slot->unk254 = 0x11;
        }
        break;
    case 0x11:
        changed2 = 1;
        if (advanceAnimTransform(slot->unk40, lbl_eu_80666F90) != 0) {
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
            if (advanceAnimTransform(slot->unk7C, lbl_eu_80666F90) != 0) {
                slot->unk80 = 2;
                reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk7C)->frame =
                    lbl_eu_80666F94;
            }
        } else if (slot->unk80 == 2) {
            func_8010ACC4(
                reinterpret_cast<CMenuBattleDamageQueue*>(&slot->unk74));
        }
        slot->unk78->Animate(0);
    }

    // Once the level-up cursor is armed, swap the gauge layout to its
    // alternate animation set and move the slot to state 3.
    if (slot->unk80 == 0 && slot->unk240 == 0 && self->unk7C8 != 0 &&
        slot->unk254 == 0xB) {
        MenuBpsPaneFlagsView* fp = reinterpret_cast<MenuBpsPaneFlagsView*>(
            slot->unk20->GetRootPane());
        fp->flagsBB = fp->flagsBB & ~0x100u;
        slot->unk18->SetAnimationEnable(slot->unk1C,
            true);
        slot->unk244 = 3;
    }
}

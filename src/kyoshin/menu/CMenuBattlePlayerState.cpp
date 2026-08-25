#include "kyoshin/menu/CMenuBattlePlayerState.hpp"

#include "kyoshin/CTaskGame.hpp"
// CUICfManager.hpp declares func_801B481C as int (line 54); CVision.hpp (via
// CBattleManager.hpp) declares it as u32. This TU uses neither copy.
#define func_801B481C menuBpsUiCfMgr481CUnused
#include "kyoshin/CUICfManager.hpp"
#undef func_801B481C
// CChainTimer.hpp (via CBattleManager.hpp) declares func_80174B4C(void*, u32)
// while CfObjectActor.hpp declares it with its full 5-arg ABI signature - an
// illegal overload when both are visible. Pre-include CfObjectActor.hpp so its
// declaration keeps the real name, then rename the CChainTimer/CVision copies
// out of the way (this TU uses neither).
#include "kyoshin/cf/object/CfObjectActor.hpp"
#define func_80174B4C menuBpsChainTimer74B4CUnused
// CfObjectImplMove.hpp declares the BM singleton as extern "C" void* while
// CSuddenCommu.hpp / CfGameManager.hpp use CBattleManagerView* - an illegal
// overload when both are visible in this TU (which only calls the member
// getInstance). Rename every free-function copy out of the way.
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#undef func_80174B4C
// code_80135FDC.hpp declares lbl_eu_8066A208 as u32 (line 188);
// CfObjectMove.hpp (via the CBattleManager.hpp include above) declares it
// const float. This TU uses neither copy.
#define lbl_eu_8066A208 menuBpsCode35FDCepsilonUnused
#include "kyoshin/code_80135FDC.hpp"
#undef lbl_eu_8066A208
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
extern u32 lbl_eu_80663E24;
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
void func_8010D1B4(CMenuBattlePlayerState* self, void* actor,
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

extern void func_80138078(u32);

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

// Cast-only ifaces: MWCC virtual dispatch uses r12 (retail), unlike function-pointer
// loads that color the vptr temp as r4. Never constructed.
struct MenuBpsActorIf {
    // MWCC (RTTI on) places two hidden slots before the first declared virtual;
    // omit _v000/_v004 so vf108 lands at retail 0x108, etc.
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual void _v0AC();
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void _v0C8();
    virtual void _v0CC();
    virtual void _v0D0();
    virtual void _v0D4();
    virtual void _v0D8();
    virtual void _v0DC();
    virtual void _v0E0();
    virtual void _v0E4();
    virtual void _v0E8();
    virtual void _v0EC();
    virtual void _v0F0();
    virtual void _v0F4();
    virtual void _v0F8();
    virtual void _v0FC();
    virtual void _v100();
    virtual void _v104();
    virtual u32 vf108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual f32 vf128();
    virtual f32 vf12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual void _v158();
    virtual void _v15C();
    virtual void _v160();
    virtual void _v164();
    virtual void _v168();
    virtual void _v16C();
    virtual void _v170();
    virtual void _v174();
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual void _v188();
    virtual void _v18C();
    virtual void _v190();
    virtual void _v194();
    virtual void _v198();
    virtual void _v19C();
    virtual void _v1A0();
    virtual void _v1A4();
    virtual void _v1A8();
    virtual void _v1AC();
    virtual void _v1B0();
    virtual void _v1B4();
    virtual void _v1B8();
    virtual void _v1BC();
    virtual void _v1C0();
    virtual void _v1C4();
    virtual void _v1C8();
    virtual void _v1CC();
    virtual void _v1D0();
    virtual void _v1D4();
    virtual void _v1D8();
    virtual void _v1DC();
    virtual void _v1E0();
    virtual void _v1E4();
    virtual int vf1E8();
    virtual void _v1EC();
    virtual int vf1F0();
    virtual void _v1F4();
    virtual void _v1F8();
    virtual void _v1FC();
    virtual u32 vf200();
    virtual void _v204();
    virtual void _v208();
    virtual void _v20C();
    virtual void _v210();
    virtual void _v214();
    virtual void _v218();
    virtual void _v21C();
    virtual void _v220();
    virtual void _v224();
    virtual void _v228();
    virtual void _v22C();
    virtual void _v230();
    virtual void _v234();
    virtual void _v238();
    virtual void _v23C();
    virtual void _v240();
    virtual void _v244();
    virtual void _v248();
    virtual void _v24C();
    virtual void _v250();
    virtual void _v254();
    virtual void _v258();
    virtual void _v25C();
    virtual void _v260();
    virtual void _v264();
    virtual void _v268();
    virtual void _v26C();
    virtual void _v270();
    virtual void _v274();
    virtual void _v278();
    virtual void _v27C();
    virtual void _v280();
    virtual void _v284();
    virtual void _v288();
    virtual void _v28C();
    virtual u32 vf290();
    virtual void _v294();
    virtual void _v298();
    virtual void _v29C();
    virtual void _v2A0();
    virtual void _v2A4();
    virtual void _v2A8();
    virtual void _v2AC();
    virtual void _v2B0();
    virtual void _v2B4();
    virtual void _v2B8();
    virtual void _v2BC();
    virtual void _v2C0();
    virtual void _v2C4();
    virtual void _v2C8();
    virtual void _v2CC();
    virtual void _v2D0();
    virtual void _v2D4();
    virtual void _v2D8();
    virtual void _v2DC();
    virtual void _v2E0();
    virtual void _v2E4();
    virtual void _v2E8();
    virtual void _v2EC();
    virtual void _v2F0();
    virtual s16* vf2F4();
};

struct MenuBpsMoveIf {
    // Same two-slot MWCC adjustment as MenuBpsActorIf.
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual int vf4C();
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
    mtl::ALLOC_HANDLE handle;
    nw4r::lyt::ArcResourceAccessor* accessor;
    void* actors[3];
    u32 z;
    int i;
    void* actor;
    int* party;
    u32 hp;
    u32 maxHp;
    f32 tA;
    int tB;
    IScnRender* cb;

    handle = mtl::MemManager::getHandleMEM2();
    accessor = func_8012FDBC();
    unk64.createRegion(reinterpret_cast<int>(accessor), 0xE00,
                       lbl_eu_804FD720 + 0xF7, 0);

    {
        Class_8045F858 regionGuard(&unk64);
        z = 0;
        actors[0] = NULL;
        actors[1] = NULL;
        actors[2] = NULL;

        for (i = 0; i < 3; i++) {
            CMenuBattlePlayerStateSlot& slot = mSlots[i];

            accessor = CUICfManager::func_801355F4();
            func_80136E84(&slot.unk00, accessor, lbl_eu_804FD720 + 0x10E);

            accessor = CUICfManager::func_801355F4();
            func_80136F08(slot.unk00, &slot.unk04, accessor,
                           lbl_eu_804FD720 + 0x12E);

            slot.unk00->SetAnimationEnable(slot.unk04, true);
            slot.unk04->SetFrame(0.0f);
            slot.unk00->Animate(0);

            accessor = CUICfManager::func_801355F4();
            func_80136E84(&slot.unk08, accessor, lbl_eu_804FD720 + 0x151);

            accessor = CUICfManager::func_801355F4();
            func_80136F08(slot.unk08, &slot.unk0C, accessor,
                           lbl_eu_804FD720 + 0x174);
            func_80136F08(slot.unk08, &slot.unk10, accessor,
                           lbl_eu_804FD720 + 0x19A);
            func_80136F08(slot.unk08, &slot.unk14, accessor,
                           lbl_eu_804FD720 + 0x1C5);

            slot.unk08->SetAnimationEnable(slot.unk10, false);
            slot.unk08->SetAnimationEnable(slot.unk14, false);
            slot.unk08->SetAnimationEnable(slot.unk0C, true);
            slot.unk0C->SetFrame(0.0f);
            slot.unk08->Animate(0);

            accessor = CUICfManager::func_801355F4();
            func_80136E84(&slot.unk18, accessor, lbl_eu_804FD720 + 0x1F0);

            accessor = CUICfManager::func_801355F4();
            func_80136F08(slot.unk18, &slot.unk1C, accessor,
                           lbl_eu_804FD720 + 0x210);

            {
                nw4r::lyt::Pane* rootPane = slot.unk18->GetRootPane();
                u32 fontHandle = func_801355BC();
                func_8013676C(rootPane, fontHandle);
            }
            {
                u32 fontAccessor = func_801355D8();
                func_801368C0(slot.unk18, lbl_eu_804FD720 + 0x233,
                              fontAccessor);
                func_801368C0(slot.unk18, lbl_eu_804FD720 + 0x23C,
                              fontAccessor);
            }

            {
                unsigned long long rect = func_80139658(
                    slot.unk18, lbl_eu_804FD720 + 0x245, 1);
                lbl_eu_80663F30[0] = static_cast<s16>(rect & 0xFFFF);
                lbl_eu_80663F30[1] = static_cast<s16>((rect >> 16) & 0xFFFF);
                lbl_eu_80663F30[2] = static_cast<s16>((rect >> 32) & 0xFFFF);
                lbl_eu_80663F30[3] = static_cast<s16>((rect >> 48) & 0xFFFF);
                lbl_eu_80663F38[3] = lbl_eu_80663F30[3];
                lbl_eu_80663F40[3] = lbl_eu_80663F30[3];
            }

            {
                nw4r::lyt::Pane* rp = slot.unk18->GetRootPane();
                slot.unk44 = rp->FindPaneByName(lbl_eu_804FD720 + 0x233, true);
                slot.unk48 = rp->FindPaneByName(lbl_eu_804FD720 + 0x23C, true);
                slot.unk4C = rp->FindPaneByName(lbl_eu_804FD720 + 0x245, true);
                slot.unk50 = rp->FindPaneByName(lbl_eu_804FD720 + 0x24C, true);
                slot.unk54 = rp->FindPaneByName(lbl_eu_804FD720 + 0x255, true);
                slot.unk58 = rp->FindPaneByName(lbl_eu_804FD720 + 0x262, true);
                slot.unk5C = rp->FindPaneByName(lbl_eu_804FD720 + 0x26F, true);
                slot.unk60 = rp->FindPaneByName(lbl_eu_804FD720 + 0x27C, true);
                slot.unk64 = rp->FindPaneByName(lbl_eu_804FD720 + 0x289, true);
                slot.unk68 = rp->FindPaneByName(lbl_eu_804FD720 + 0x295, true);
                slot.unk6C = rp->FindPaneByName(lbl_eu_804FD720 + 0x2A1, true);
                slot.unk70 = rp->FindPaneByName(lbl_eu_804FD720 + 0x2AE, true);
            }

            slot.unk18->SetAnimationEnable(slot.unk1C, true);
            slot.unk1C->SetFrame(0.0f);
            slot.unk18->Animate(0);

            {
                nw4r::lyt::Pane* rp = slot.unk18->GetRootPane();
                rp->FindPaneByName(lbl_eu_804FD720 + 0x233, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x23C, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x245, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x24C, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x255, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x262, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x26F, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x27C, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x289, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x295, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x2A1, true);
                rp->FindPaneByName(lbl_eu_804FD720 + 0x2AE, true);
            }

            slot.unk25C = 7;

            accessor = CUICfManager::func_801355F4();
            func_80136E84(&slot.unk20, accessor, lbl_eu_804FD720 + 0x2BB);

            accessor = CUICfManager::func_801355F4();
            func_80136F08(slot.unk20, &slot.unk24, accessor,
                           lbl_eu_804FD720 + 0x2DE);

            slot.unk20->SetAnimationEnable(slot.unk24, true);
            slot.unk24->SetFrame(0.0f);
            slot.unk20->Animate(0);

            accessor = CUICfManager::func_801355F4();
            func_80136E84(&slot.unk28, accessor, lbl_eu_804FD720 + 0x304);

            accessor = CUICfManager::func_801355F4();
            func_80136F08(slot.unk28, &slot.unk2C, accessor,
                           lbl_eu_804FD720 + 0x323);
            func_80136F08(slot.unk28, &slot.unk30, accessor,
                           lbl_eu_804FD720 + 0x347);
            func_80136F08(slot.unk28, &slot.unk34, accessor,
                           lbl_eu_804FD720 + 0x36D);
            func_80136F08(slot.unk28, &slot.unk38, accessor,
                           lbl_eu_804FD720 + 0x392);
            func_80136F08(slot.unk28, &slot.unk3C, accessor,
                           lbl_eu_804FD720 + 0x3B6);
            func_80136F08(slot.unk28, &slot.unk40, accessor,
                           lbl_eu_804FD720 + 0x3DC);

            slot.unk28->UnbindAllAnimation();
            slot.unk28->BindAnimation(slot.unk2C);
            slot.unk28->SetAnimationEnable(slot.unk2C, true);
            slot.unk28->Animate(0);
            slot.unk28->UnbindAllAnimation();

            {
                f32 yOff = 86.0f * static_cast<f32>(i);
                nw4r::lyt::Pane* p;

                p = slot.unk00->GetRootPane();
                p->SetTranslate(nw4r::math::VEC3(
                    p->GetTranslate().x, p->GetTranslate().y - yOff,
                    p->GetTranslate().z));

                p = slot.unk08->GetRootPane();
                p->SetTranslate(nw4r::math::VEC3(
                    p->GetTranslate().x, p->GetTranslate().y - yOff,
                    p->GetTranslate().z));

                p = slot.unk18->GetRootPane();
                p->SetTranslate(nw4r::math::VEC3(
                    p->GetTranslate().x, p->GetTranslate().y - yOff,
                    p->GetTranslate().z));

                p = slot.unk20->GetRootPane();
                p->SetTranslate(nw4r::math::VEC3(
                    p->GetTranslate().x, p->GetTranslate().y - yOff,
                    p->GetTranslate().z));

                p = slot.unk28->GetRootPane();
                p->SetTranslate(nw4r::math::VEC3(
                    p->GetTranslate().x, p->GetTranslate().y - yOff,
                    p->GetTranslate().z));
            }

            accessor = CUICfManager::func_801355F4();
            slot.unk74 = accessor;

            func_80136E84(&slot.unk78, accessor, lbl_eu_804FD720 + 0xA2);
            func_80136F08(slot.unk78, &slot.unk7C,
                           static_cast<nw4r::lyt::ArcResourceAccessor*>(
                               slot.unk74),
                           lbl_eu_804FD720 + 0xBC);

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
                    slot.unk78->GetRootPane()->FindPaneByName(
                        lbl_eu_804FD720 + 0xD9, true);
                if (found != NULL) {
                    f32 yOff2 = 86.0f * static_cast<f32>(i);
                    found->SetTranslate(nw4r::math::VEC3(
                        found->GetTranslate().x,
                        found->GetTranslate().y - yOff2,
                        found->GetTranslate().z));
                }
            }
        }

        {
            UnkClass_8045F564 temp;
            temp.unk0 = 0;
            temp.unk4 = 0;
            temp.unk8 = 0;
            temp.unkC = 0;
            unk7D0.unk0 = temp.unk0;
            unk7D0.unk4 = temp.unk4;
            unk7D0.unk8 = temp.unk8;
            unk7D0.unkC = temp.unkC;
        }
        unk7E0 = NULL;
        unk7E4 = NULL;
        unk7E8 = NULL;
        unk7EC = NULL;
        unk7F0 = NULL;
        unk7F4 = 1;
        unk7F5 = 0;
        unk7F8 = 0;

        accessor = CUICfManager::func_801355F4();
        unk7E0 = accessor;

        handle = mtl::MemManager::getHandleMEM2();
        unk7D0.createRegion(handle, 0x2000, lbl_eu_804FD720, 0);

        {
            Class_8045F858 scoped2(&unk7D0);
            mtl::MemManager::func_80434A4C(false);

            func_80136E84(&unk7E4,
                           static_cast<nw4r::lyt::ArcResourceAccessor*>(
                               unk7E0),
                           lbl_eu_804FD720 + 0x12);
            func_80136F08(unk7E4, &unk7E8,
                           static_cast<nw4r::lyt::ArcResourceAccessor*>(
                               unk7E0),
                           lbl_eu_804FD720 + 0x30);
            func_80136F08(unk7E4, &unk7EC,
                           static_cast<nw4r::lyt::ArcResourceAccessor*>(
                               unk7E0),
                           lbl_eu_804FD720 + 0x51);
            func_80136F08(unk7E4, &unk7F0,
                           static_cast<nw4r::lyt::ArcResourceAccessor*>(
                               unk7E0),
                           lbl_eu_804FD720 + 0x72);

            unk7E4->SetAnimationEnable(unk7EC, false);
            unk7E4->SetAnimationEnable(unk7F0, false);
            unk7E4->SetAnimationEnable(unk7E8, true);
            unk7E4->Animate(0);
        }

        party = func_8009ECB0();
        for (i = 0; i < 3; i++) {
            actor = func_800B8B94(party[i + 1]);
            actors[i] = actor;
        }

        for (i = 0; i < 3; i++) {
            actor = actors[i];
            if (actor == NULL) {
                continue;
            }

            CMenuBattlePlayerStateSlot& slot = mSlots[i];

            tA = static_cast<f32>(
                vslot<GetU32Fn>(actor, 0x1E8)(actor));
            slot.unk224 = tA;

            tB = vslot<GetIntFn>(actor, 0x1F0)(actor);
            slot.unk228 = static_cast<f32>(tB);
            if (slot.unk228 == 1.0f) {
                slot.unk224 = 0.0f;
            }

            if (slot.unk22C < 0.0f) {
                slot.unk22C = slot.unk224;
            }

            slot.unk218 = vslot<GetU32Fn>(actor, 0x200)(actor);
            slot.unk21C = slot.unk218;

            hp = static_cast<u32>(
                vslot<GetF32Fn>(actor, 0x128)(actor));
            maxHp = static_cast<u32>(
                vslot<GetF32Fn>(actor, 0x12C)(actor));
            slot.unk210 = hp;
            slot.unk214 = maxHp;
        }

        cb = static_cast<IScnRender*>(this);
        mScn->addRenderCB(cb, 0xA, 0);

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
    if (CTaskGame::func_800426F0()) {
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
    if (cf::CfGameManager::func_800829B8()) {
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
            MenuBpsActorIf* actor;
            CMenuBattlePlayerStateSlot* slot;
            u32 hp;
            u32 maxHp;
            f32 hpRatio;

            actor = reinterpret_cast<MenuBpsActorIf*>(actors[i]);
            if (actor == NULL) {
                continue;
            }

            // Retail: clrlwi; mulli 0x270; add this; addi +0x74 → &mSlots[i].
            slot = &mSlots[i];
            func_8010D1B4(this, actor, slot);

            hp = static_cast<u32>(actor->vf128());
            maxHp = static_cast<u32>(actor->vf12C());

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
            slot->unk208 = actor->vf290();

            {
                u32 statusId = actor->vf108();
                if (slot->unk20C != statusId) {
                    slot->unk25C |= 0x1;
                }
            }
            slot->unk20C = actor->vf108();

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
            slot->unk218 = actor->vf200();
            slot->unk220 = hpRatio;

            {
                f32 tB = static_cast<f32>(actor->vf1F0());
                // Constant on the left → retail fcmpu cr0,fNV,f0 (not f0,fNV).
                if (lbl_eu_80666F90 == tB) {
                    f32 tB2 = static_cast<f32>(actor->vf1F0());
                    if (slot->unk228 != tB2) {
                        slot->unk25C |= 0x4;
                    }
                    slot->unk224 = lbl_eu_80666F94;
                    slot->unk228 = lbl_eu_80666F90;
                    goto tension_done;
                }

                {
                    f32 tA = static_cast<f32>(static_cast<u32>(actor->vf1E8()));
                    if (slot->unk224 != tA) {
                        goto tension_flag;
                    }
                    f32 tB2 = static_cast<f32>(actor->vf1F0());
                    if (slot->unk228 == tB2) {
                        goto tension_store;
                    }
                }
            tension_flag:
                slot->unk25C |= 0x4;
            tension_store:
                slot->unk224 =
                    static_cast<f32>(static_cast<u32>(actor->vf1E8()));
                slot->unk228 = static_cast<f32>(actor->vf1F0());
            }
        tension_done:

            if (slot->unk22C < lbl_eu_80666F94) {
                slot->unk22C = slot->unk224;
            }

            {
                s16* pair = actor->vf2F4();
                slot->unk230 = pair[1];
                slot->unk238 = pair[0];
            }

            {
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                u32 nonempty = menuBpsActorListSize(&bm->mActorList1) != 0;
                slot->unk240 = static_cast<u8>(nonempty);
                if (nonempty == 0) {
                    MenuBpsMoveIf* move = reinterpret_cast<MenuBpsMoveIf*>(
                        cf::CfGameManager::getPlayer(0));
                    if (move != NULL) {
                        int id = move->vf4C();
                        if (id != 0) {
                            Func800B708C_Ret* handle = reinterpret_cast<Func800B708C_Ret*>(func_800B708C(id));
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
                u32 a = func_80137444(slot->unk04, lbl_eu_80666F90);
                u32 b = func_80137444(slot->unk1C, lbl_eu_80666F90);
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
            if (func_80137444(unk7E8, lbl_eu_80666F90) != 0) {
                unk7F4 = 1;
                unk7F8 = 2;
                unk7E4->SetAnimationEnable(unk7E8, false);
                unk7E4->SetAnimationEnable(unk7EC, false);
                unk7E4->SetAnimationEnable(unk7F0, true);
                unk7F0->SetFrame(lbl_eu_80666F94);
            }
            break;
        case 2:
            func_80137444(unk7F0, lbl_eu_80666F90);
            break;
        case 3:
            if (func_80137510(unk7E8, lbl_eu_80666F90) !=
                0) {
                unk7F4 = 1;
                unk7F8 = 0;
            }
            break;
        case 4:
            if (func_80137444(unk7EC, lbl_eu_80666F90) != 0) {
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
                    func_80138078(0x9a);
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
    if (CTaskGame::func_800426F0()) {
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

            func_80137038(slot.unk00, &drawInfo, 0, (slot.unk25C & 0x40) != 0);

            if (slot.unk25C & 0x100) {
                func_80137038(slot.unk08, &drawInfo, 0, 1);
            }
            if (slot.unk25C & 0x200) {
                func_80137038(slot.unk28, &drawInfo, 0, 1);
            }
            func_80137038(slot.unk18, &drawInfo, 0, (slot.unk25C & 0x880) != 0);
            if (slot.unk25C & 0x400) {
                func_80137038(slot.unk20, &drawInfo, 0, 1);
            }

            if (slot.unk78 != NULL && slot.unk80 != 0) {
                func_80137038(slot.unk78, &drawInfo, 0, 1);
            }
        }

        if (unk7F8 != 0) {
            func_80137038(unk7E4, &drawInfo, 0, 1);
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
    u32 big;
    u32 little;
    u32 c;
    u32 ok;
    u32 ok2;

    z = 0;
    big = 0x60;
    little = 0xc;
    start = slot->pad90;
    end = reinterpret_cast<u8*>(&slot->unk204);

    slot->unk74 = (void*)z;
    slot->unk78 = (nw4r::lyt::Layout*)z;
    slot->unk7C = (nw4r::lyt::AnimTransform*)z;
    slot->unk80 = (u8)z;
    slot->unk84 = (void*)z;
    slot->unk88 = (void*)z;
    slot->unk8C = (void*)z;

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
    if (ok2 != 0) {
        c = (u32)(lim + 0x5f - start) / big;
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
    c = (u32)(end + 0xb - p) / little;
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
extern "C" {
void func_8010EA4C(void* _this) {
    __dt__22CMenuBattlePlayerStateFv(
        reinterpret_cast<CMenuBattlePlayerState*>((char*)_this - 0x58), 1);
}
}
// IScnRender cbRenderBefore this-adjusting thunk (retail: subi r3,-0x5c; b cbRenderBefore__22CMenuBattlePlayerStateFv)
extern "C" void func_8010EA54(void* self) {
    reinterpret_cast<CMenuBattlePlayerState*>((char*)self - 0x5c)->cbRenderBefore();
}
extern "C" void func_8010EA5C(void* self) {
    __dt__22CMenuBattlePlayerStateFv(
        reinterpret_cast<CMenuBattlePlayerState*>((char*)self - 0x5c), 1);
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

void func_8010D0D4(CMenuBattlePlayerState* self, CMenuBattlePlayerStateSlot* slot,
                   u32 mode) {
    MenuBpsModeEntry* table =
        reinterpret_cast<MenuBpsModeEntry*>(func_8012FA78());
    u32 sel;
    if (table != NULL) {
        // Retail initializes the selector only after the table gate.
        sel = 0;
        if (mode <= 1) {
            sel = table[slot->unk258].unk08;
        } else if (mode - 3 <= 1) {
            sel = table[slot->unk258].unk04;
        } else if (mode == 2) {
            sel = table[slot->unk258].unk00;
        }
    }
    if (sel == 0) {
        func_8012FAA8();
        return;
    }
    func_80137F88(slot->unk5C, sel);
    func_80137F88(slot->unk60, sel);
    func_80137F88(slot->unk64, sel);
    func_80137F88(slot->unk68, sel);
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
        reinterpret_cast<Func800B708C_Ret*>(func_800B708C(id));
    if (handle == NULL) {
        return;
    }
    if ((handle->unk64 & 2) == 0) {
        return;
    }
    u8 idxByte = static_cast<u8>(handle->unk8C);

    struct PartyData {
        u32 w[12];
    };
    PartyData party = *(PartyData*)((u8*)func_8009ECB0() + 4);

    for (u8 i = 0; i < 3; i++) {
        if (idxByte == party.w[i]) {
            void* slotView =
                (u8*)state + static_cast<u32>(i) * 0x270 + 0xE8;
            func_8010A940(slotView, a, b, c);
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
            char* tbl = lbl_eu_804FD720;
            result = static_cast<u8>(func_8013600C(
                         tbl + 0x415,
                         tbl + 0x422,
                         static_cast<u8>((idx & 0xFF) * 4 + (idx & 0xFF) + i +
                                         (c - 1) * 25))) *
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
// Icon-result record filled by the actor's icon-query virtuals
// (id at +0x0c, state at +0x30; 0x800 = empty slot).
struct MenuBpsIconRecord {
    u8 pad00[0xC];
    u16 unk0C;
    u8 pad0E[0x30 - 0x0E];
    u32 unk30;
};

// Cast-only iface for the polymorphic member embedded at actor+0x8 (MWCC RTTI
// places two hidden slots before the first declared virtual). The icon queries
// fill a MenuBpsIconRecord; retail's caller passes the inner-object address as
// the record destination (r3), index in r4.
struct MenuBpsActorInnerIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual MenuBpsIconRecord* vf58(MenuBpsIconRecord* dest, int idx);
    virtual MenuBpsIconRecord* vf5C(MenuBpsIconRecord* dest, int idx);
};

struct MenuBpsMoveActorView {
    u8 pad00[8];
    MenuBpsActorInnerIf mInner; // +0x8 (member subobject, vptr at +0x8)
};

// Actor fields accessed past the inner object.
struct MenuBpsMoveActorFields {
    u8 pad00[0x1530];
    u32 unk1530; // +0x1530: level icon id (u16)
};

// Shared texture manager: binds a texture under the "timg" key (0x74696D67).
struct MenuBpsTexMgrIf {
    virtual void _v008();
    virtual u32 assignTexture(u32 key, char* tex, u32 arg); // +0xC
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

// Layout field at +0x10 holding a pane pointer whose +0xBB flag is toggled.
struct MenuBpsLytPaneRefView {
    u8 pad00[0x10];
    MenuBpsPaneFlagsView* paneRef;
};

// Layout method dispatched at vtable +0x24 (no args) - resets gauge anim state.
struct MenuBpsLytVt24If {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void vf24(); // +0x24
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
void func_8010D1B4(CMenuBattlePlayerState* self, void* actorPtr,
                   CMenuBattlePlayerStateSlot* slot) {
    MenuBpsMoveActorView* actor =
        reinterpret_cast<MenuBpsMoveActorView*>(actorPtr);

    // Tension timer: wraps and raises flag bit 0x800 when it exceeds FDC.
    slot->unk264 += lbl_eu_80666F90;
    slot->unk25C &= ~0x800u;
    if (slot->unk264 > lbl_eu_80666FDC) {
        slot->unk25C |= 0x800u;
        slot->unk264 = lbl_eu_80666F94;
    }

    // Pass 1: scan the actor's live arts-icon slots from last match + 1.
    MenuBpsIconRecord* res;
    {
    char* tbl = lbl_eu_804FD720;
    nw4r::lyt::Pane* lvlStarPane = slot->unk6C;
    reinterpret_cast<MenuBpsPaneFlagsView*>(lvlStarPane)->flagsBB &=
        static_cast<u8>(~0x01);

    u32 cursor = slot->unk268 + 1;
    while (true) {
        if ((cursor & 0xFF) >= 0x20) {
            cursor = 0;
        }
        MenuBpsIconRecord* res = actor->mInner.vf5C(
            reinterpret_cast<MenuBpsIconRecord*>(&actor->mInner),
            static_cast<int>(cursor & 0xFF));
        if (res->unk30 != 0x800 && res->unk0C != 0 && res->unk0C != 0xF &&
            res->unk0C != 0x10 && res->unk0C != 0x12) {
            u16 nameId = static_cast<u16>(
                func_80136254(lbl_eu_806640E0, tbl + 0x42B, res->unk0C));
            if (nameId != 0) {
                char* tex = func_80138F78(nameId);
                MenuBpsTexMgrIf* mgr =
                    reinterpret_cast<MenuBpsTexMgrIf*>(func_801355F4());
                u32 bound = mgr->assignTexture(0x74696D67, tex, 0);
                if (bound != 0) {
                    reinterpret_cast<MenuBpsPaneFlagsView*>(lvlStarPane)
                        ->flagsBB |= 0x01;
                    func_80137F88(lvlStarPane, bound);
                    slot->unk268 = cursor & 0xFF;
                    break;
                }
            }
        }
        if ((cursor & 0xFF) == slot->unk268) {
            break;
        }
        cursor += 1;
    }
    }

    reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk70)->flagsBB &=
        static_cast<u8>(~0x01);

    // Pass 2: snapshot all 32 slots (empty-state slots read as 0), append the
    // actor's level icon as entry 0x20, then scan from last match + 1.
    char* tbl = lbl_eu_804FD720;
    u16 ids[33];
    for (int j = 0; j < 32; j++) {
        actor->mInner.vf58(
            reinterpret_cast<MenuBpsIconRecord*>(&actor->mInner), j);
        ids[j] = (res->unk30 == 0x800) ? static_cast<u16>(0) : res->unk0C;
    }
    ids[32] = static_cast<u16>(
        reinterpret_cast<MenuBpsMoveActorFields*>(actorPtr)->unk1530);

    u32 cur2 = slot->unk26C + 1;
    while (true) {
        if ((cur2 & 0xFF) >= 0x21) {
            cur2 = 0;
        }
        u16 v = ids[cur2 & 0xFF];
        if (v != 0) {
            if ((cur2 & 0xFF) == 0x20) {
                // Fixed "level up" icon id, no BDAT lookup for entry 0x20.
                char* tex = func_80138F78(0x13D);
                MenuBpsTexMgrIf* mgr =
                    reinterpret_cast<MenuBpsTexMgrIf*>(func_801355F4());
                u32 bound = mgr->assignTexture(0x74696D67, tex, 0);
                if (bound != 0) {
                    reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk70)
                        ->flagsBB |= 0x01;
                    func_80137F88(slot->unk70, bound);
                    slot->unk26C = cur2 & 0xFF;
                    return;
                }
            } else {
                u16 nameId = static_cast<u16>(
                    func_80136254(lbl_eu_806640E0, tbl + 0x42B, v));
                if (nameId != 0) {
                    char* tex = func_80138F78(nameId);
                    MenuBpsTexMgrIf* mgr =
                        reinterpret_cast<MenuBpsTexMgrIf*>(func_801355F4());
                    u32 bound = mgr->assignTexture(0x74696D67, tex, 0);
                    if (bound != 0) {
                        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk70)
                            ->flagsBB |= 0x01;
                        func_80137F88(slot->unk70, bound);
                        slot->unk26C = cur2 & 0xFF;
                        return;
                    }
                }
            }
        }
        if ((cur2 & 0xFF) == slot->unk26C) {
            break;
        }
        cur2 += 1;
    }
}
// ---------------------------------------------------------------------------
// Target us-8010df8c: per-slot activation pass. Refreshes the level text and
// name label, redraws HP digits / color set when flagged, updates the HP gauge
// width (quantized through lbl_eu_804FD6E0), advances mode/prev snapshots,
// toggles the party/tension star panes by HP state, resets the tension anims
// and re-arms the per-slot state machine. `index` nonzero suppresses the
// func_80138078(0x99) jingle on first activation.
// ---------------------------------------------------------------------------
void func_8010D4B0(CMenuBattlePlayerState* self,
                   CMenuBattlePlayerStateSlot* slot, u32 index) {
    u8 gate = slot->unk240;
    slot->unk25C &= ~0x03E00000u;
    if (gate == 0 && slot->unk80 == 0) {
        return;
    }

    char buf[0x40];
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
            u32 w = static_cast<u32>(widthF); // fctiwz truncate
            slot->unk24C = w;
            f32 prevMax = pane->unk50;
            double dw = static_cast<double>(w);
            f32 frac = ratio - lbl_eu_80666FD8 * static_cast<f32>(dw);
            f32 q = frac / lbl_eu_80666FD8;
            f32 val = lbl_eu_80666FD4 * q + lbl_eu_804FD6E0[w];
            pane->unk4C = val;
            pane->unk50 = prevMax;
        }
    }

    slot->unk234 = slot->unk230;
    slot->unk23C = slot->unk238;
    func_8010D0D4(self, slot, slot->unk230);

    // Star-pane visibility follows the smoothed HP ratio.
    MenuBpsLytPaneRefView* mainRef =
        reinterpret_cast<MenuBpsLytPaneRefView*>(slot->unk08);
    if (slot->unk220 == lbl_eu_80666F94) {
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB |= 0x01;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB |= 0x01;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB &=
            static_cast<u8>(~0x01);
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB &=
            static_cast<u8>(~0x01);
        mainRef->paneRef->flagsBB &= static_cast<u8>(~0x01);
    } else {
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk5C)->flagsBB &=
            static_cast<u8>(~0x01);
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk60)->flagsBB &=
            static_cast<u8>(~0x01);
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk64)->flagsBB |= 0x01;
        reinterpret_cast<MenuBpsPaneFlagsView*>(slot->unk68)->flagsBB |= 0x01;
        mainRef->paneRef->flagsBB |= 0x01;
    }

    reinterpret_cast<MenuBpsLytVt24If*>(slot->unk28)->vf24();
    slot->unk254 = 0xB;
    reinterpret_cast<MenuBpsAnimFrameView*>(slot->unk24)->frame =
        lbl_eu_80666F94;
    reinterpret_cast<MenuBpsPaneFlagsView*>(
        reinterpret_cast<MenuBpsLytPaneRefView*>(slot->unk20)->paneRef)
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
        func_80138078(0x99);
    }
}
extern "C" void harness_stub_us_8010e3b0(CMenuBattlePlayerState* self,
                                         CMenuBattlePlayerStateSlot* slot) {}

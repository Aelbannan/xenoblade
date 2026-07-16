#include "kyoshin/menu/CMenuBattlePlayerState.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"

#include "decomp.h"
#include "functions.hpp"
#include <nw4r/math.h>
#include <revolution/GX.h>

// Batch 2026-07-14g: menu-bps-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14h: menu-bps-move owns Move exclusively; do not touch
// Term / cbRenderBefore.
// Batch 2026-07-14k: menu-bps-ctor owns __ct__CMenuBattlePlayerState only.

struct CMenuBpsProcessShim {
    u8 unk00[0x10];
    void* vtable;
    u8 unk14[0x28];
    u32 callbacks[6]; // +0x3c
};

extern "C" {
extern CMenuBattlePlayerState* lbl_eu_80663F48;
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
char lbl_eu_8052C1C0[];
char lbl_eu_8052C330[];
char lbl_eu_8052C42C[];
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
void func_8010B324(CMenuBattlePlayerStateSlot*);
void __dt__8010B444(CMenuBattlePlayerStateSlot*, s16);
void __construct_array(void* ptr, void* ctor, void* dtor, u32 size, u32 n);
// Unmangled retail name; int (not u8) avoids clrlwi before cmpwi.
int func_8013BE50();
void* func_800B8B94(int id);
void func_8010D1B4(CMenuBattlePlayerState* self, void* actor,
                   CMenuBattlePlayerStateSlot* slot);
void func_8010D4B0(CMenuBattlePlayerState* self,
                   CMenuBattlePlayerStateSlot* slot, u32 index);
void func_8010D8D4(CMenuBattlePlayerState* self,
                   CMenuBattlePlayerStateSlot* slot);
// Unmangled retail name (distinct from AnimTransform-mangled overload).
u32 func_80137510(void* anim, float frame);
extern char lbl_eu_804FD720[];
extern const f32 lbl_eu_80666F90; // 1.0f
extern const f32 lbl_eu_80666F94; // 0.0f
extern const f32 lbl_eu_80666F98; // 110.0f
extern const f32 lbl_eu_80666F9C; // 86.0f
extern const f32 lbl_eu_80666FA0; // -178.0f
extern const f32 lbl_eu_80666FB0; // -1.0f
extern const f32 lbl_eu_80666FC0; // 100.0f
extern const f32 lbl_eu_80666FC4; // 360.0f
}

extern "C" {
extern void* func_8012FDBC();
extern u32 func_801355BC();
extern u32 func_801355D8();
}

extern void func_80138078(u32);
extern "C" unsigned long long func_80139658(nw4r::lyt::Layout*, const char*, int);

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

// Retail linker name is untyped `__ct__CMenuBattlePlayerState` (takes CScn* in r4).
extern "C" CMenuBattlePlayerState*
__ct__CMenuBattlePlayerState(CMenuBattlePlayerState* self, CScn* scn) {
    CMenuBpsProcessShim* process;
    u8* ptmfBase;
    char* vtFinal;
    char* vtWork;
    char* vtRender;
    u32 ptmfWord0;
    u32 ptmfWord1;
    u32 ptmfWord2;
    u32 z;
    u32 v4;
    u32 v6;
    u32 vB;
    u8 i;
    UnkClass_8045F564* unk64p;

    process = reinterpret_cast<CMenuBpsProcessShim*>(self);
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));

    // Interim CProcess vtable, then final MI vtable + interface pieces.
    // Retail: lwzu of [0], then stw [1]@+0x40 before [0]@+0x3C (ArtsSelect order).
    process->vtable = lbl_eu_8052C1C0;
    vtFinal = lbl_eu_8052C330;
    ptmfBase = (u8*)__ptmf_null;
    ptmfWord0 = *(u32*)(ptmfBase + 0);
    vtWork = vtFinal + 0x24;
    vtRender = vtFinal + 0xac;
    z = 0;
    unk64p = &self->unk64;
    ptmfWord1 = *(u32*)(ptmfBase + 4);
    process->callbacks[1] = ptmfWord1;
    process->callbacks[0] = ptmfWord0;
    ptmfWord2 = *(u32*)(ptmfBase + 8);
    process->callbacks[2] = ptmfWord2;
    ptmfWord0 = *(u32*)(ptmfBase + 0);
    ptmfWord1 = *(u32*)(ptmfBase + 4);
    process->callbacks[4] = ptmfWord1;
    process->callbacks[3] = ptmfWord0;
    ptmfWord2 = *(u32*)(ptmfBase + 8);
    process->callbacks[5] = ptmfWord2;
    self->unk54 = (u8)z;
    self->unk55 = (u8)z;
    process->vtable = vtFinal;
    *reinterpret_cast<char**>(reinterpret_cast<u8*>(self) + 0x58) = vtWork;
    *reinterpret_cast<char**>(reinterpret_cast<u8*>(self) + 0x5c) = vtRender;
    self->mScn = scn;

    __ct__17UnkClass_8045F564Fv(unk64p);
    __construct_array(self->mSlots, reinterpret_cast<void*>(func_8010B324),
                      reinterpret_cast<void*>(__dt__8010B444), 0x270, 3);

    // Retail stores 0.0f then constructs unk7D0 before loading loop floats.
    self->unk7C4 = lbl_eu_80666F94;
    self->unk7C8 = (u8)z;
    self->unk7C9 = (u8)z;
    self->unk7CC = lbl_eu_8052C42C;
    __ct__17UnkClass_8045F564Fv(&self->unk7D0);

    self->unk7F4 = 1;
    self->unk7E0 = (void*)z;
    self->unk7E4 = (nw4r::lyt::Layout*)z;
    self->unk7E8 = (nw4r::lyt::AnimTransform*)z;
    self->unk7EC = (nw4r::lyt::AnimTransform*)z;
    self->unk7F0 = (nw4r::lyt::AnimTransform*)z;
    self->unk7F8 = z;

    {
        f32 zeroF;
        f32 neg1F;

        v4 = 4;
        v6 = 6;
        vB = 0xb;
        zeroF = lbl_eu_80666F94;
        neg1F = lbl_eu_80666FB0;
        i = 0;
        do {
            CMenuBattlePlayerStateSlot slot;
            u32* p;
            u32* end;

            // Inlined func_8010B324: clear +0x74..+0x8c then +0x90..+0x204.
            slot.unk74 = (void*)z;
            slot.unk78 = (nw4r::lyt::Layout*)z;
            slot.unk7C = (nw4r::lyt::AnimTransform*)z;
            slot.unk80 = (u8)z;
            slot.unk84 = (void*)z;
            slot.unk88 = (void*)z;
            slot.unk8C = (void*)z;
            p = reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x90);
            end = reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x204);
            while (p < end) {
                *p = z;
                p++;
            }

            slot.unk220 = zeroF;
            slot.unk224 = zeroF;
            slot.unk228 = zeroF;
            slot.unk22C = neg1F;
            slot.unk248 = v4;
            slot.unk250 = v6;
            slot.unk254 = vB;
            slot.unk258 = i;
            slot.unk264 = zeroF;

            // Post-float zeros: retail fills slot+0x00..+0x40 then sparse tail
            // before the aggregate copy into mSlots[i].
            {
                u32* hp = reinterpret_cast<u32*>(&slot);
                u32 left;
                for (left = 0x11; left != 0; left--) {
                    *hp = z;
                    hp++;
                }
            }
            slot.unk204 = (u8)z;
            slot.unk208 = z;
            slot.unk20C = z;
            slot.unk210 = z;
            slot.unk214 = z;
            slot.unk218 = z;
            slot.unk21C = z;
            slot.unk230 = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x234) = z;
            slot.unk238 = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x23c) = z;
            slot.unk25C = z;
            slot.unk240 = (u8)z;
            slot.unk244 = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x24c) = z;
            *reinterpret_cast<u8*>(reinterpret_cast<u8*>(&slot) + 0x260) = (u8)z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x268) = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x26c) = z;

            self->mSlots[i] = slot;
            i = (u8)(i + 1);
        } while (i < 3);
    }

    return self;
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
    accessor = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(
        func_8012FDBC());
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

        cb = this;
        if (this != NULL) {
            cb = reinterpret_cast<IScnRender*>(
                reinterpret_cast<u8*>(this) + 0x5C);
        }
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
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_REFERENCE 8c9.
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
            // Retail: clrlslwi fi; add party; lwz 4(r3); … stwx actors,same shift.
            int* p = party + fi;
            void* actor = func_800B8B94(p[1]);
            actors[fi] = actor;
            if (actor != NULL) {
                aliveCount += 1;
            }
            fi += 1;
        }

        // Gauge/bias NVs intentionally not hoisted as named locals: retail loads
        // int→float biases (lfd) before gauge floats; named f32 NVs force lfs-first.

        typedef s16* (*GetS16PairFn)(void*);

        for (u8 i = 0; i < 3; i++) {
            void* actor;
            CMenuBattlePlayerStateSlot* slot;
            u32 hp;
            u32 maxHp;
            f32 hpRatio;

            actor = actors[i];
            if (actor == NULL) {
                continue;
            }

            // Retail: clrlwi; mulli 0x270; add this; addi +0x74.
            {
                u8* row = reinterpret_cast<u8*>(this) + (i * 0x270);
                slot = reinterpret_cast<CMenuBattlePlayerStateSlot*>(row + 0x74);
            }
            func_8010D1B4(this, actor, slot);

            hp = static_cast<u32>(vslot<GetF32Fn>(actor, 0x128)(actor));
            maxHp = static_cast<u32>(vslot<GetF32Fn>(actor, 0x12C)(actor));

            // Retail: lfs f26, zero pool — not fmr from a zero NV.
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

            slot->unk204 = static_cast<u8>(*reinterpret_cast<u16*>(
                reinterpret_cast<u8*>(actor) + 0x3f28));
            slot->unk208 = vslot<GetU32Fn>(actor, 0x290)(actor);

            {
                u32 statusId = vslot<GetU32Fn>(actor, 0x108)(actor);
                if (slot->unk20C != statusId) {
                    slot->unk25C |= 0x1;
                }
            }
            slot->unk20C = vslot<GetU32Fn>(actor, 0x108)(actor);

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
            slot->unk218 = vslot<GetU32Fn>(actor, 0x200)(actor);
            slot->unk220 = hpRatio;

            {
                f32 tB = static_cast<f32>(vslot<GetIntFn>(actor, 0x1F0)(actor));
                if (tB == lbl_eu_80666F90) {
                    f32 tB2 =
                        static_cast<f32>(vslot<GetIntFn>(actor, 0x1F0)(actor));
                    if (slot->unk228 != tB2) {
                        slot->unk25C |= 0x4;
                    }
                    slot->unk224 = lbl_eu_80666F94;
                    slot->unk228 = lbl_eu_80666F90;
                    goto tension_done;
                }

                {
                    f32 tA = static_cast<f32>(static_cast<u32>(
                        vslot<GetIntFn>(actor, 0x1E8)(actor)));
                    if (slot->unk224 != tA) {
                        goto tension_flag;
                    }
                    f32 tB2 =
                        static_cast<f32>(vslot<GetIntFn>(actor, 0x1F0)(actor));
                    if (slot->unk228 == tB2) {
                        goto tension_store;
                    }
                }
            tension_flag:
                slot->unk25C |= 0x4;
            tension_store:
                slot->unk224 = static_cast<f32>(static_cast<u32>(
                    vslot<GetIntFn>(actor, 0x1E8)(actor)));
                slot->unk228 =
                    static_cast<f32>(vslot<GetIntFn>(actor, 0x1F0)(actor));
            }
        tension_done:

            if (slot->unk22C < lbl_eu_80666F94) {
                slot->unk22C = slot->unk224;
            }

            {
                s16* pair = vslot<GetS16PairFn>(actor, 0x2F4)(actor);
                slot->unk230 = pair[1];
                slot->unk238 = pair[0];
            }

            {
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                u32 nonempty = bm->mActorList1.size() != 0;
                slot->unk240 = static_cast<u8>(nonempty);
                if (nonempty == 0) {
                    void* move = cf::CfGameManager::func_80082D54(0);
                    if (move != NULL) {
                        int id = vslot<GetIntFn>(move, 0x4C)(move);
                        if (id != 0) {
                            void* handle = func_800B708C(id);
                            if (handle != NULL) {
                                u32 bits = *reinterpret_cast<u32*>(
                                    reinterpret_cast<u8*>(handle) + 0x64);
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
                u32 a = func_80137510(static_cast<void*>(slot->unk04),
                                      lbl_eu_80666F90);
                u32 b = func_80137510(static_cast<void*>(slot->unk1C),
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
            if (func_80137510(static_cast<void*>(unk7E8), lbl_eu_80666F90) !=
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
            f32 tx = lbl_eu_80666FA0;
            f32 ty =
                lbl_eu_80666F98 - lbl_eu_80666F9C * static_cast<f32>(unk7F5);
            f32 tz = lbl_eu_80666F94;
            pane->SetTranslate(nw4r::math::VEC3(tx, ty, tz));
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
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_REFERENCE 8c9.
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

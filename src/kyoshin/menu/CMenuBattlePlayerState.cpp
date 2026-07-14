#include "kyoshin/menu/CMenuBattlePlayerState.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"

#include "decomp.h"
#include "functions.hpp"
#include <nw4r/math.h>
#include <revolution/GX.h>

// Batch 2026-07-14g: menu-bps-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14h: menu-bps-move owns Move exclusively; do not touch
// Term / cbRenderBefore.

extern "C" {
extern CMenuBattlePlayerState* lbl_eu_80663F48;
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
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
extern const f32 lbl_eu_80666FC0; // 100.0f
extern const f32 lbl_eu_80666FC4; // 360.0f
}

extern void func_80138078(u32);

template <typename Fn>
static inline Fn vslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
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

        typedef f32 (*GetF32Fn)(void*);
        typedef u32 (*GetU32Fn)(void*);
        typedef int (*GetIntFn)(void*);
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

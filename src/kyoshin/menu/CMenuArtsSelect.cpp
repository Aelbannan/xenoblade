#include "kyoshin/menu/CMenuArtsSelect.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CPadManager.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include "decomp.h"
#include <revolution/GX.h>

extern "C" {
void* func_80110A70();
void* func_8010CE48();
void func_8010EDE4(void*);
void func_8010A8E4(void*);
void func_80133770();

// Batch 2026-07-14g: menu-arts-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14j: menu-arts-move owns Move exclusively.
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
// Unmangled retail names; int (not u8) avoids clrlwi before cmpwi.
int func_8013BE50();
int func_8018A608();
int func_80122448();

void* getInstance__11CSysWinBuffFv();
u8 func_8013BEB8();
void func_801080F8(CMenuArtsSelect* self);
int func_8012FA5C();
void func_80138078(u32);
u32 func_80137510(void* anim, float frame);
u32 func_80174C98(void* actor, u32* outVal, u32 flags);
void* func_8016FE34();
void* func_80153DB0(void* obj, s32 index);
int func_8010EDD4(void*);
int func_8010A840(void*);

void func_80104454(CMenuArtsSelect* self);
void func_80105A34(CMenuArtsSelect* self);
void func_80105D54(CMenuArtsSelect* self);
void func_80106450(CMenuArtsSelect* self);
void func_801065E4(CMenuArtsSelect* self);
void func_80106900(CMenuArtsSelect* self);
void func_80106C30(CMenuArtsSelect* self, s32 index);
void func_80106EC8(CMenuArtsSelect* self, s32 index);
void func_801071B8(CMenuArtsSelect* self, s32 index);
void func_801072E0(CMenuArtsSelect* self);
void func_80107580(CMenuArtsSelect* self);
int func_80107970(CMenuArtsSelect* self, s32 index);
int func_80107C54(CMenuArtsSelect* self, s32 index);

extern char lbl_eu_804FD1E0[];
extern const f32 lbl_eu_80666F28; // 0.0f
extern const f32 lbl_eu_80666F2C; // 1.0f
extern const f32 lbl_eu_80666F40; // -80.0f
extern const f32 lbl_eu_80666F44; // -1.0f
}

template <typename Fn>
static inline Fn artsVslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
}

void CMenuArtsSelect::Term() {
    // Retail clears lbl_eu_80663F24 (not a mangled spInstance).
    if (lbl_eu_80663F24 != NULL) {
        lbl_eu_80663F24->unkB0 = NULL;
        lbl_eu_80663F24->unk68 |= 0x40u;
        lbl_eu_80663F24 = NULL;
    }

    CDeviceVI::waitForDrawDone();

    // MI adjust: IScnRender at +0x5c (null-this safe).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(reinterpret_cast<u8*>(this) + 0x5c);
    }
    mScn->removeRenderCB(cb);

    // Sliding base matches retail r30 += 4 over 9 iterations.
    u8* base = reinterpret_cast<u8*>(this);
    s32 i = 0;
    do {
        nw4r::lyt::Layout*& a = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x1b8);
        if (a != NULL) {
            delete a;
            a = NULL;
        }
        nw4r::lyt::Layout*& b = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x170);
        if (b != NULL) {
            delete b;
            b = NULL;
        }
        nw4r::lyt::Layout*& c = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0x104);
        if (c != NULL) {
            delete c;
            c = NULL;
        }
        if (i < 8) {
            nw4r::lyt::Layout*& d = *reinterpret_cast<nw4r::lyt::Layout**>(base + 0xa4);
            if (d != NULL) {
                delete d;
                d = NULL;
            }
        }
        i++;
        base += 4;
    } while (i < 9);

    if (unk98 != NULL) {
        delete unk98;
        unk98 = NULL;
    }
    if (unk8C != NULL) {
        delete unk8C;
        unk8C = NULL;
    }
    if (unk80 != NULL) {
        delete unk80;
        unk80 = NULL;
    }
    if (unk294 != NULL) {
        delete unk294;
        unk294 = NULL;
    }

    unk6C.func_8045F778();

    lbl_eu_80663F20 = NULL;

    // Retail calls each getter twice when non-null.
    if (func_80110A70() != NULL) {
        func_8010EDE4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4);
    }
    if (func_8010CE48() != NULL) {
        func_8010A8E4(reinterpret_cast<u8*>(func_8010CE48()) + 0x7cc);
    }
    if (unk334 != 0) {
        func_80133770();
    }
}

void CMenuArtsSelect::Move() {
    // Extra early NV (live across the function) nudges this into r21 so
    // savegpr starts at r20 like retail.
    u32 one = 1;

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

    unk7C = -1;
    unk7D = -1;
    unk7E = -1;

    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }
    if (cf::CfGameManager::func_800829B8()) {
        goto done;
    }
    if (func_8018A608()) {
        goto done;
    }
    if (func_80122448()) {
        goto done;
    }
    if (getInstance__11CSysWinBuffFv() != NULL) {
        goto done;
    }
    if (func_80110A70() == NULL) {
        goto done;
    }
    // Same beq+8; b done shape as bit21 (PLAN.md section 17.6).
    if (func_8010CE48() != NULL) {
        goto after_ce48;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_ce48:

    func_801080F8(this);

    {
        u8 flag = func_8013BEB8();
        u8* b;
        b = &unk26C[0]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[1]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[2]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[3]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[4]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[5]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
        b = &unk26C[6]->unkBB;
        *b = static_cast<u8>((*b & 0xFEu) | flag);
    }

    if (unk298 >= 2) {
        void* move = cf::CfGameManager::func_80082D54(0);
        void* actor = move;
        if (move != NULL) {
            actor = reinterpret_cast<u8*>(move) - 0x3e9c;
        }
        if (actor != NULL) {
            typedef void* (*GetPtrFn)(void*);
            void* sub = *reinterpret_cast<void**>(reinterpret_cast<u8*>(actor) + 4);
            u32* pVal = reinterpret_cast<u32*>(artsVslot<GetPtrFn>(sub, 0x30)(sub));
            u32 localVal = pVal[0];
            if (func_80174C98(actor, &localVal, 0x803) != 0) {
                if (unk348 == 0) {
                    unk348 = 1;
                    unk328 = 4;
                    func_80107580(this);
                    if (unk324 == 4) {
                        func_801072E0(this);
                    }
                }
            } else {
                unk348 = 0;
            }
        }
    }

    switch (static_cast<s32>(unk298)) {
    case 0:
        unk308 |= 0x10u;
        if (func_8012FA5C() != 0) {
            func_80107580(this);
            func_80138078(0x42);
            unk298 = 1;
        }
        break;
    case 1:
        unk308 |= 0x10u;
        if (func_80137444(unk84, lbl_eu_80666F2C) != 0) {
            u8* flagByte =
                reinterpret_cast<u8*>(unk8C->GetRootPane()) + 0xBB;
            *flagByte = static_cast<u8>((*flagByte & 0xFEu) | 1u);
            char* nameStr =
                func_80136190(lbl_eu_804FD1E0 + 0x249, lbl_eu_804FD1E0 + 0x254, 1);
            char* helpStr =
                func_80136190(lbl_eu_804FD1E0 + 0x249, lbl_eu_804FD1E0 + 0x259, 1);
            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x69, nameStr, 0);
            func_80136B4C(unk80, lbl_eu_804FD1E0 + 0x5c, helpStr,
                          reinterpret_cast<u32>(unk294));
            unk298 = 2;
        }
        break;
    case 2:
        func_80104454(this);
        break;
    case 3:
        unk308 |= 0x90u;
        if (func_80137444(unk88, lbl_eu_80666F2C) != 0) {
            unk54 = 1;
        }
        if (unk308 & 0x2u) {
            // f32[5] stack homes → frame -0x70 with stfs at sp+0x10..0x20.
            f32 homes[5];
            homes[0] = lbl_eu_80666F28;
            homes[1] = lbl_eu_80666F40 * unk88->GetFrame();
            homes[2] = lbl_eu_80666F28;
            homes[3] = homes[1];
            homes[4] = lbl_eu_80666F28;
            f32* t = reinterpret_cast<f32*>(
                reinterpret_cast<u8*>(unk98->GetRootPane()) + 0x2C);
            t[0] = homes[0];
            t[1] = homes[1];
            t[2] = homes[2];
            unk308 |= 0x1u;
            if (homes[3] + homes[4] == 0.0f) {
                t[0] = homes[4];
            }
        }
        break;
    case 4:
        func_80105A34(this);
        break;
    case 5:
        if (func_80110A70() != NULL) {
            if (func_8010CE48() != NULL) {
                if (func_8010EDD4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4) !=
                    0) {
                    if (func_8010A840(reinterpret_cast<u8*>(func_8010CE48()) +
                                      0x7cc) != 0) {
                        unk298 = 6;
                    }
                }
            }
        }
        break;
    case 6:
        func_80105D54(this);
        break;
    case 7:
        if (func_80110A70() != NULL) {
            if (func_8010CE48() != NULL) {
                if (func_8010EDD4(reinterpret_cast<u8*>(func_80110A70()) + 0x7e4) !=
                    0) {
                    if (func_8010A840(reinterpret_cast<u8*>(func_8010CE48()) +
                                      0x7cc) != 0) {
                        u8* flagByte =
                            reinterpret_cast<u8*>(unk8C->GetRootPane()) + 0xBB;
                        *flagByte = static_cast<u8>((*flagByte & 0xFEu) | 1u);
                        unk298 = 2;
                    }
                }
            }
        }
        break;
    case 8:
        func_80106450(this);
        break;
    default:
        break;
    }

    if (unk298 != 0) {
        void* move = cf::CfGameManager::func_80082D54(0);
        void* actor = move;
        if (move != NULL) {
            actor = reinterpret_cast<u8*>(move) - 0x3e9c;
        }
        if (actor != NULL) {
            if (unk298 >= 2 && !(unk308 & 0x80u)) {
                if (unk328 == 4) {
                    switch (static_cast<s32>(unk29C)) {
                    case 9:
                        func_801065E4(this);
                        break;
                    case 10:
                        unk308 |= 0x3u;
                        if (func_80137444(unkA0, lbl_eu_80666F2C) != 0) {
                            unk29C = 0xb;
                        }
                        break;
                    case 11:
                        func_80106900(this);
                        break;
                    default:
                        break;
                    }
                }

                f32 zeroF = lbl_eu_80666F28;
                for (s32 i = 0; i < 8; i++) {
                    switch (static_cast<s32>(unk2A0[i])) {
                    case 9:
                        func_80106C30(this, i);
                        break;
                    case 10: {
                        unk318 |= (one << i) | (one << (i + 9));
                        cf::CfGameManager::func_80082D54(0);
                        void* skillSrc = func_8016FE34();
                        int ready = 0;
                        if (skillSrc != NULL) {
                            typedef u16* (*GetU16Fn)(void*);
                            u16* p =
                                artsVslot<GetU16Fn>(skillSrc, 0x27c)(skillSrc);
                            if (p[0] != 0) {
                                ready = 1;
                            }
                        }
                        typedef void* (*GetPtrFn)(void*);
                        void* skill =
                            artsVslot<GetPtrFn>(skillSrc, 0x278)(skillSrc);
                        void* info = func_80153DB0(skill, i);
                        if (ready == 0) {
                            if (*reinterpret_cast<u16*>(
                                    reinterpret_cast<u8*>(info) + 0x74) != 0) {
                                typedef f32 (*GetF32Fn)(void*);
                                void* table = *reinterpret_cast<void**>(
                                    reinterpret_cast<u8*>(info) + 0x84);
                                GetF32Fn getMax = reinterpret_cast<GetF32Fn>(
                                    *reinterpret_cast<void**>(
                                        reinterpret_cast<u8*>(table) + 0x14));
                                f32 denom = getMax(info);
                                f32 ratio;
                                if (denom != zeroF) {
                                    denom = getMax(info);
                                    ratio =
                                        *reinterpret_cast<f32*>(
                                            reinterpret_cast<u8*>(info) + 0x80) /
                                        denom;
                                } else {
                                    ratio = lbl_eu_80666F44;
                                }
                                if (ratio > zeroF) {
                                    ready = 1;
                                }
                            }
                        }
                        if (func_80137444(unkE4[i], lbl_eu_80666F2C) != 0 ||
                            ready != 0) {
                            unk2A0[i] = 0xb;
                            unkA4[i]->SetAnimationEnable(unkE4[i], false);
                            unkA4[i]->SetAnimationEnable(unkC4[i], true);
                        }
                        break;
                    }
                    case 11:
                        func_80106EC8(this, i);
                        break;
                    default:
                        break;
                    }
                }
            }

            {
                cf::CBattleManager* bm = cf::CBattleManager::getInstance();
                if (bm->mActorList1.size() == 0) {
                    CPad* pad = cf::CfGameManager::getCurrentPad();
                    u32 bit;
                    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
                        bit = (pad->mPressedButtonFlags >> 23) & 1;
                    } else {
                        bit = (pad->mPressedButtonFlags >> 10) & 1;
                    }
                    if (bit != 0) {
                        typedef f32 (*GetF32Fn)(void*);
                        f32 v = artsVslot<GetF32Fn>(actor, 0x128)(actor);
                        if (v > lbl_eu_80666F28) {
                            unk334 = 1;
                        }
                    }
                }
            }
        }

        if (unk298 >= 2 && !(unk308 & 0x80u)) {
            for (s32 i = 0; i < 9; i++) {
                switch (static_cast<s32>(unk2C0[i])) {
                case 0xc: {
                    s32 nextIdx = (i >= 8) ? 0 : (i + 1);
                    u32 mask = (1u << i) | (1u << (i + 9));
                    unk310 &= ~mask;
                    if (unk200[nextIdx]->unkBB & 1) {
                        if (func_80107C54(this, i) != 0) {
                            unk104[i]->SetAnimationEnable(unk14C[i], false);
                            unk104[i]->SetAnimationEnable(unk128[i], true);
                            unk128[i]->SetFrame(lbl_eu_80666F28);
                            unk2C0[i] = 0xd;
                            unk310 |= mask;
                        }
                    }
                    break;
                }
                case 0xd: {
                    s32 nextIdx = (i >= 8) ? 0 : (i + 1);
                    if (unk200[nextIdx]->unkBB & 1) {
                        unk310 |= (1u << i) | (1u << (i + 9));
                        if (func_80137444(unk128[i], lbl_eu_80666F2C) != 0) {
                            unk2C0[i] = 0xe;
                        }
                    } else {
                        unk310 &= ~((1u << i) | (1u << (i + 9)));
                        unk2C0[i] = 0xc;
                    }
                    break;
                }
                case 0xe:
                    func_801071B8(this, i);
                    break;
                case 0xf:
                    unk310 |= (1u << i) | (1u << (i + 9));
                    if (func_80137444(unk14C[i], lbl_eu_80666F2C) != 0) {
                        unk2C0[i] = 0xc;
                    }
                    break;
                default:
                    break;
                }

                switch (static_cast<s32>(unk2E4[i])) {
                case 0x10:
                    unk314 &= ~((1u << i) | (1u << (i + 9)));
                    if (func_80107970(this, i) != 0) {
                        unk2E4[i] = 0x11;
                    }
                    break;
                case 0x11:
                    unk314 |= (1u << i) | (1u << (i + 9));
                    if (func_80137444(unk194[i], lbl_eu_80666F2C) != 0) {
                        unk2E4[i] = 0x12;
                    }
                    break;
                case 0x12:
                    unk314 = (unk314 | (1u << i)) & ~(1u << (i + 9));
                    if (func_80107970(this, i) == 0) {
                        unk2E4[i] = 0x13;
                    }
                    break;
                case 0x13:
                    unk314 |= (1u << i) | (1u << (i + 9));
                    if (func_80137510(static_cast<void*>(unk194[i]),
                                      lbl_eu_80666F2C) != 0) {
                        unk2E4[i] = 0x10;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        for (s32 i = 0; i < 9; i++) {
            if (func_80137444(unk1DC[i], lbl_eu_80666F2C) != 0) {
                unk30C = static_cast<u16>(unk30C & ~static_cast<u16>(1u << i));
            } else {
                unk30C = static_cast<u16>(unk30C | static_cast<u16>(1u << i));
            }
        }
    }

    if (unk308 & 0x30u) {
        unk80->Animate(0);
    }
    unk8C->Animate(0);
    if (unk308 & 0x1u) {
        unk98->Animate(0);
    }

    // Sliding this-base (+4 / iter) matches retail r20 += 4; also keeps a
    // second NV so this stays in r21 (savegpr_20).
    {
        u8* base = reinterpret_cast<u8*>(this);
        s32 i = 0;
        do {
            if (i < 8) {
                if (unk318 & (one << (i + 9))) {
                    (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0xa4))
                        ->Animate(0);
                }
            }
            u32 bit9 = one << (i + 9);
            if (unk310 & bit9) {
                (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0x104))
                    ->Animate(0);
            }
            if (unk314 & bit9) {
                (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0x170))
                    ->Animate(0);
            }
            if (unk30C & (one << i)) {
                (*reinterpret_cast<nw4r::lyt::Layout**>(base + 0x1b8))
                    ->Animate(0);
            }
            i++;
            base += 4;
        } while (i < 9);
    }
done:
    ;
}

void CMenuArtsSelect::cbRenderBefore() {
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
    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }
    if (func_8018A608()) {
        goto done;
    }
    if (func_80122448()) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

        if (unk298 > 1 && !(unk308 & 0x80u)) {
            for (s32 i = 0; i < 9; i++) {
                if (unk30C & (1u << i)) {
                    func_80137038(unk1B8[i], &drawInfo, 0, 1);
                }
            }
        }

        {
            u32 flags = unk308;
            u32 notBit25 = !(flags & 0x40u);
            u32 merged = (flags & 0x30u) | notBit25;
            int visible = merged != 0;
            func_80137038(unk80, &drawInfo, 0, visible);
        }

        unk308 |= 0x40u;

        if (unk298 > 1) {
            if (unk328 == 4) {
                if (unk308 & 0x2u) {
                    u32 bit0 = unk308 & 0x1u;
                    u32 notBit28 = !(unk308 & 0x8u);
                    u32 merged = bit0 | notBit28;
                    int visible = merged != 0;
                    func_80137038(unk98, &drawInfo, 0, visible);
                    unk308 |= 0x8u;
                } else {
                    unk308 &= ~0x8u;
                }
            } else {
                unk308 &= ~0x8u;
            }

            if (!(unk308 & 0x80u)) {
                for (s32 i = 0; i < 8; i++) {
                    u32 v = unk318;
                    if (v & (1u << i)) {
                        u32 bit18 = 1u << (i + 18);
                        u32 notBit18 = !(v & bit18);
                        u32 merged = notBit18 | (v & (1u << (i + 9)));
                        int visible = merged != 0;
                        if (unk340 == 0) {
                            func_80137038(unkA4[i], &drawInfo, 0, visible);
                        }
                        unk318 |= bit18;
                    } else {
                        unk318 &= ~(1u << (i + 18));
                    }
                }

                for (s32 i = 0; i < 9; i++) {
                    s32 nextIdx = (i == 8) ? 0 : (i + 1);
                    if (unk200[nextIdx]->unkBB & 1) {
                        u32 v = unk310;
                        u32 bit18 = 1u << (i + 18);
                        u32 bitI = 1u << i;
                        if (v & bitI) {
                            u32 notBit18 = !(v & bit18);
                            u32 merged = notBit18 | (v & (1u << (i + 9)));
                            int visible = merged != 0;
                            func_80137038(unk104[i], &drawInfo, 0, visible);
                            unk310 |= bit18;
                        } else {
                            unk310 &= ~bit18;
                        }

                        if (unk2C0[i] == 0xC) {
                            u32 w = unk314;
                            if (w & bitI) {
                                u32 notBit18 = !(w & bit18);
                                u32 merged = notBit18 | (w & (1u << (i + 9)));
                                int visible = merged != 0;
                                func_80137038(unk170[i], &drawInfo, 0, visible);
                                unk314 |= bit18;
                            } else {
                                unk314 &= ~bit18;
                            }
                        } else {
                            unk314 &= ~bit18;
                        }
                    } else {
                            unk310 &= ~(1u << (i + 18));
                            unk314 &= ~(1u << (i + 18));
                    }
                }
            }
        }

        func_80137038(unk8C, &drawInfo, 0, 1);
    }
done:
    ;
}

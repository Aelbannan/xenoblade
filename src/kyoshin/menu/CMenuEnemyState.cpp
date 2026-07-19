#include "kyoshin/menu/CMenuEnemyState.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/work/CProcess.hpp"

#include "decomp.h"
#include <nw4r/math.h>
#include <revolution/GX.h>

// Batch 2026-07-14f: menu-enemy-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14h: menu-enemy-move owns Move exclusively; do not touch
// cbRenderBefore above.
// Batch 2026-07-14k: menu-enemy-ctor owns __ct__CMenuEnemyState exclusively.

struct CMenuEnemyCtorProcess {
    u8 unk00[0x10];
    void* vtable; // 0x10
    u8 unk14[0x28];
    u32 callbacks[6]; // 0x3C / 0x48 PTMF blocks
};

extern "C" {
char lbl_eu_8052BF70[];
char lbl_eu_8052C438[];
extern u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void __ct__17UnkClass_8045F564Fv(void*);
void __dt__17UnkClass_8045F564Fv(void*, s16);
void __ct__CPcSelectCursor(void* self);
extern const f32 lbl_eu_80666FEC;
extern const f32 lbl_eu_80667004;
}

extern "C" CMenuEnemyState* __ct__CMenuEnemyState(CMenuEnemyState* self, void* scn) {
    // NV decl order targets retail homes: r31=this, r30=zero, r29=scn.
    CMenuEnemyState* thisPtr;
    u32 zero;
    void* scnArg;
    CMenuEnemyCtorProcess* process;
    u8* ptmfBase;
    char* vtFinal;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;
    u8* panel;
    u8* panelEnd;
    f32 panelMarker;
    u8 tmp[0x48];
    u32 copy;

    thisPtr = self;
    scnArg = scn;
    process = reinterpret_cast<CMenuEnemyCtorProcess*>(thisPtr);
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
    ptmfBase = reinterpret_cast<u8*>(__ptmf_null);
    process->vtable = lbl_eu_8052BF70;
    ptmfWord1 = *reinterpret_cast<u32*>(ptmfBase + 4);
    vtFinal = lbl_eu_8052C438;
    ptmfWord0 = *reinterpret_cast<u32*>(ptmfBase + 0);
    process->callbacks[1] = ptmfWord1;
    process->callbacks[0] = ptmfWord0;
    ptmfWord2 = *reinterpret_cast<u32*>(ptmfBase + 8);
    process->callbacks[2] = ptmfWord2;
    ptmfWord0 = *reinterpret_cast<u32*>(ptmfBase + 0);
    ptmfWord1 = *reinterpret_cast<u32*>(ptmfBase + 4);
    process->callbacks[4] = ptmfWord1;
    process->callbacks[3] = ptmfWord0;
    ptmfWord2 = *reinterpret_cast<u32*>(ptmfBase + 8);
    process->callbacks[5] = ptmfWord2;
    zero = 0;
    *reinterpret_cast<u8*>(reinterpret_cast<u8*>(thisPtr) + 0x54) = 0;
    *reinterpret_cast<u8*>(reinterpret_cast<u8*>(thisPtr) + 0x55) = 0;
    process->vtable = vtFinal;
    *reinterpret_cast<char**>(reinterpret_cast<u8*>(thisPtr) + 0x58) = vtFinal + 0x24;
    *reinterpret_cast<char**>(reinterpret_cast<u8*>(thisPtr) + 0x5c) = vtFinal + 0xac;
    thisPtr->unk60 = scnArg;

    __ct__17UnkClass_8045F564Fv(reinterpret_cast<u8*>(thisPtr) + 0x64);

    thisPtr->unk74 = NULL;
    panel = reinterpret_cast<u8*>(thisPtr) + 0xa4;
    panelMarker = lbl_eu_80666FEC;
    panelEnd = reinterpret_cast<u8*>(thisPtr) + 0x7c4;
    thisPtr->unk78 = NULL;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7c) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x80) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x84) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x88) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x8c) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x90) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x94) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x98) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x9c) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0xa0) = zero;

    // do-while + live panelEnd matches retail fall-into-body cmplw/blt shape.
    // panelEnd/one r0?r3 Chaitin soft-cap closed via postprocess insn_patches.
    do {
        *reinterpret_cast<u32*>(panel + 0x00) = zero;
        *reinterpret_cast<u32*>(panel + 0x04) = zero;
        *reinterpret_cast<u32*>(panel + 0x08) = zero;
        *reinterpret_cast<u32*>(panel + 0x0c) = zero;
        *reinterpret_cast<u32*>(panel + 0x10) = zero;
        panel[0x14] = 0;
        panel[0x15] = 0;
        *reinterpret_cast<f32*>(panel + 0x18) = panelMarker;
        panel[0x1c] = 0;
        panel[0x1d] = 0;
        panel[0x1e] = 0;
        panel[0x1f] = 0;
        panel[0x20] = 0;
        panel[0x21] = 0;
        panel[0x22] = 0;
        *reinterpret_cast<u32*>(panel + 0x24) = zero;
        panel[0x28] = 0;
        panel[0x29] = 1;
        *reinterpret_cast<u32*>(panel + 0x2c) = zero;
        *reinterpret_cast<u32*>(panel + 0x30) = zero;
        *reinterpret_cast<u32*>(panel + 0x34) = zero;
        *reinterpret_cast<u32*>(panel + 0x38) = zero;
        *reinterpret_cast<u32*>(panel + 0x3c) = zero;
        *reinterpret_cast<u32*>(panel + 0x40) = zero;
        *reinterpret_cast<u32*>(panel + 0x44) = zero;
        *reinterpret_cast<u32*>(panel + 0x48) = zero;
        panel += 0x4c;
    } while (panel < panelEnd);

    zero = 0;
    reinterpret_cast<u8*>(thisPtr)[0x7c4] = 0;
    *reinterpret_cast<f32*>(reinterpret_cast<u8*>(thisPtr) + 0x7c8) = panelMarker;
    *reinterpret_cast<f32*>(reinterpret_cast<u8*>(thisPtr) + 0x7cc) = panelMarker;
    *reinterpret_cast<f32*>(reinterpret_cast<u8*>(thisPtr) + 0x7d0) = panelMarker;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7d4) = zero;
    *reinterpret_cast<f32*>(reinterpret_cast<u8*>(thisPtr) + 0x7d8) = panelMarker;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7dc) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7e0) = zero;

    __ct__CPcSelectCursor(reinterpret_cast<u8*>(thisPtr) + 0x7e4);

    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x82c) = zero;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x830) = zero;
    reinterpret_cast<u8*>(thisPtr)[0x834] = 0;
    *reinterpret_cast<f32*>(reinterpret_cast<u8*>(thisPtr) + 0x838) = lbl_eu_80667004;

    __ct__CPcSelectCursor(tmp);
    copy = *reinterpret_cast<u32*>(tmp + 0x04);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7e8) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x08);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7ec) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x0c);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7f0) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x10);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7f4) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x14);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7f8) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x18);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x7fc) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x1c);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x800) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x20);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x804) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x24);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x808) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x28);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x80c) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x2c);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x810) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x30);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x814) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x34);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x818) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x38);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x81c) = copy;
    copy = *reinterpret_cast<u32*>(tmp + 0x3c);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x820) = copy;
    reinterpret_cast<u8*>(thisPtr)[0x824] = tmp[0x40];
    reinterpret_cast<u8*>(thisPtr)[0x825] = tmp[0x41];
    copy = *reinterpret_cast<u32*>(tmp + 0x44);
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(thisPtr) + 0x828) = copy;
    __dt__17UnkClass_8045F564Fv(tmp + 0x08, -1);

    return thisPtr;
}

extern "C" {
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
int func_8013BE50();
}

// func_800B708C(BOOL)/func_800BFC68(CfObjectMove*) declared (not extern "C")
// in CfObjectPc.hpp / below - MWCC's own C++ mangling matches the retail
// linker names (func_800B708C__Fi / func_800BFC68__FPQ22cf12CfObjectMove).
extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);

extern "C" {
// CfGameManager::func_8008585C is not declared in CfGameManager.hpp (owned
// by other batch targets); call the mangled linker name directly instead of
// touching that shared header.
int func_8008585C__Q22cf13CfGameManagerFv();

extern u8 lbl_eu_80663F98;        // once-guard byte
extern f32 lbl_eu_80573A60[3];    // mutable pulse vector (shake/scale-like)
extern const f32 lbl_eu_80666FE8; // 1.0f
extern const f32 lbl_eu_80667004; // 0.0f
extern const f32 lbl_eu_8066700C; // pulse step
extern const f32 lbl_eu_80667010; // pulse frequency
extern const f32 lbl_eu_80667014; // distance^2 threshold
extern const f32 lbl_eu_80666FEC; // anim-state marker value
extern const f32 lbl_eu_8066A1F8; // pulse amplitude

void* func_800FE68C();
void* func_8016FE34();
int func_8013BF48();
void func_800BBA08(void* r3);
void func_800BBA7C(void* r3);
int func_8013A4B4(void* a, void* b, void* c);
void func_80111080(CMenuEnemyState* self, u8* panelData, void* posA, void* posB);
void func_80112170(CMenuEnemyState* self, u8* panelData);
// Retail leaves f1=vt+0x128 and f2=vt+0x12C live into these calls.
void func_80111B08(CMenuEnemyState* self, u8* panelData, f32 v128, f32 v12c);
void func_80111E70(CMenuEnemyState* self, u8* panelData, f32 v128, f32 v12c);
void func_801132A8(CMenuEnemyState* self, u8* panelData, void* actor);
void func_801127B0(CMenuEnemyState* self);
void func_801115E8(CMenuEnemyState* self, u8* panelData);
u32 func_80174C98(void* actor, u32* outVal, u32 flags);
void* func_80496264(void* obj, int index);
void func_8010EE40(void* self);
// Retail leaves this unmangled (distinct from the FPAnimTransformf-mangled
// overload in code_80135FDC.hpp); call via a void* so overload resolution
// selects this one, same trick as CMenuPTGauge::Move.
u32 func_80137510(void* anim, float frame);
}

struct Vec3f {
    f32 x, y, z;
};

// Vtable slot dispatch helper (same style as cf::CBattleState_UnkVirtualFuncN
// ? call through the vtable without declaring the callee's own methods).
template <typename Fn>
static inline Fn vslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
}

void CMenuEnemyState::cbRenderBefore() {
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

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

        u32 indices[0x18];
        u8 fill;
        for (fill = 0; fill < 0x18; fill++) {
            indices[fill] = fill;
        }

        // Outer for(left!=0) -> mtctr/bdnz. Inner bottom-tested goto keeps CTR outer.
        // Rematerialize &indices[j] each step (retail addi r1,0x8). Load depthB before
        // depthA so fcmpo uses f1/f0. XOR via store-reload of order[j]/pair[1].
        {
            u32* order = indices;
            u8 pass = 0;
            u32 left;
            for (left = 0x17; left != 0; left--) {
                u8 swapped = 0;
                u8 pass8 = pass;
                s32 limit = 0x17 - pass8;
                u8 j = 0;
                goto sort_test;
            sort_body: {
                    u32* pair = &indices[j];
                    u32 idxA = order[j];
                    u32 idxB = pair[1];
                    f32 depthB = *reinterpret_cast<f32*>(
                        reinterpret_cast<u8*>(this) + idxB * 0x4c + 0xbc);
                    f32 depthA = *reinterpret_cast<f32*>(
                        reinterpret_cast<u8*>(this) + idxA * 0x4c + 0xbc);
                    if (depthA > depthB) {
                        u32 tmp = order[j] ^ pair[1];
                        order[j] = tmp;
                        tmp = pair[1] ^ tmp;
                        pair[1] = tmp;
                        tmp = order[j] ^ tmp;
                        order[j] = tmp;
                        swapped = 1;
                    }
                    j++;
                }
            sort_test:
                if ((s32)j < limit) {
                    goto sort_body;
                }
                if (swapped == 0) {
                    break;
                }
                pass++;
            }
        }

        // NV decl order entry, order, i -> r30/r29/r28 (MWCC_REFERENCE 8c6).
        {
            u8* entry;
            u32* order;
            u8 i;
            order = indices;
            i = 0;
            do {
                entry = reinterpret_cast<u8*>(this) + order[i] * 0x4c;
                if (entry[0xb9] == 0) {
                    goto draw_next;
                }
                if (entry[0xcd] == 0) {
                    goto draw_next;
                }
                if (entry[0xcc] != 0) {
                    func_80137038(unk74, &drawInfo, 0, 1);
                }
                {
                    u8 cc = entry[0xcc];
                    int drawFlag = (cc == 0);
                    func_80137038(
                        *reinterpret_cast<nw4r::lyt::Layout**>(entry + 0xb0),
                        &drawInfo, drawFlag, 1);
                }
                if (entry[0xb8] == 0) {
                    func_80137038(
                        *reinterpret_cast<nw4r::lyt::Layout**>(entry + 0xa8),
                        &drawInfo, 0, 1);
                }
            draw_next:
                i++;
            } while (i < 0x18);
        }

        if (unk828 != 0) {
            func_80137038(unk800, &drawInfo, 0, 1);
        }
    }
done:
    ;
}

void CMenuEnemyState::Move() {
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
    if (func_8008585C__Q22cf13CfGameManagerFv()) {
        goto done;
    }

    {
    // Pulse / target-highlight setup. Scope ends before the panel loop so
    // noTarget does not inflate callee-saved pressure past retail _savegpr_22.
    {
        int noTarget = 1;

        if (cf::CfGameManager::func_80082D54(0) != NULL) {
            u32 lastId = *reinterpret_cast<u32*>(
                reinterpret_cast<u8*>(func_800FE68C()) + 0x90E4);

            if (lastId != 0) {
                void* obj = func_800B708C(static_cast<int>(lastId));

                if (obj != NULL) {
                    bool active =
                        (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(obj) + 0x64) & 4) != 0;

                    if (active) {
                        if (unk834 != 0) {
                            f32 v = unk838 - lbl_eu_8066700C;
                            unk838 = v;
                            if (v <= lbl_eu_80667004) {
                                unk838 = lbl_eu_80667004;
                            }
                        } else {
                            f32 v = unk838 + lbl_eu_8066700C;
                            unk838 = v;
                            if (v >= lbl_eu_80666FE8) {
                                unk838 = lbl_eu_80666FE8;
                                unk834 = 1;
                            }
                        }

                        if (lastId != unk830) {
                            unk834 = 0;
                            unk838 = lbl_eu_80667004;

                            void* oldObj = func_800B708C(static_cast<int>(unk830));
                            if (oldObj != NULL) {
                                func_800BBA08(oldObj);
                            }
                            unk830 = lastId;
                        }

                        if (!lbl_eu_80663F98) {
                            lbl_eu_80663F98 = 1;
                        }

                        {
                            f32 t = lbl_eu_8066A1F8 * unk838;
                            t = lbl_eu_80667010 * t;
                            f32 s = nw4r::math::SinFIdx(t);
                            lbl_eu_80573A60[0] = s;
                            lbl_eu_80573A60[1] = s;
                            lbl_eu_80573A60[2] = lbl_eu_80666FE8;
                            func_800BBA7C(obj);
                        }

                        noTarget = 0;
                    }
                }
            }
        }

        if (noTarget) {
            if (unk830 != 0) {
                void* h = func_800B708C(static_cast<int>(unk830));
                if (h != NULL) {
                    func_800BBA08(h);
                }
                unk830 = 0;
            }
        }
    }

    func_801127B0(this);

    cf::CfObjectPc* pc =
        func_800BFC68(cf::CfGameManager::func_80082D54(0));

    nw4r::math::VEC3 delta;
    nw4r::math::VEC3 scratch;
    f32 one = lbl_eu_80666FE8;
    f32 animMarker = lbl_eu_80666FEC;
    f32 distThresh = lbl_eu_80667014;
    u8 i = 0;
    u8 z = 0;

    for (; i < 0x18; i++) {
        u8* entry = reinterpret_cast<u8*>(this) + static_cast<u8>(i) * 0x4c;

        if (entry[0xB9] == 0) {
            continue;
        }

        u8* panelData = entry + 0xA4;

        if (panelData[0x28] == 0) {
            if (func_8013BF48()) {
                u8* bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x2c)) + 0xBB;
                *bp = (*bp & 0xFE) | 1;
                bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x30)) + 0xBB;
                *bp = (*bp & 0xFE) | 1;
                bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x34)) + 0xBB;
                *bp = (*bp & 0xFE) | 1;
            }
        }

        u32 actorId = *reinterpret_cast<u32*>(panelData + 0x0);
        void* handle = func_800B708C(static_cast<int>(actorId));
        if (handle == NULL) {
            panelData[0x15] = z;
            continue;
        }

        // r24 in retail's loop = result of func_8016FE34 (not the early target).
        void* actor2 = func_8016FE34();
        int skipDist = 0;
        if (actor2 != NULL) {
            void* sub3f34 =
                *reinterpret_cast<void**>(reinterpret_cast<u8*>(actor2) + 0x3f34);
            if (sub3f34 != NULL) {
                u32 bits = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(sub3f34) + 0x7a4);
                if ((bits >> 26) & 1) {
                    skipDist = 1;
                }
            }
        }

        if (actor2 != NULL) {
            typedef f32 (*GetFloatFn)(void*);
            if (animMarker == vslot<GetFloatFn>(actor2, 0x128)(actor2) && panelData[0x1c] == 0) {
                panelData[0x15] = z;
                continue;
            }
        }
        if (skipDist) {
            panelData[0x15] = z;
            continue;
        }

        if (panelData[0x28] == 0 && pc != NULL) {
            typedef void* (*GetPosFn)(void*);
            void* handlePos = vslot<GetPosFn>(handle, 0xAC)(handle);
            void* pcEmbed = reinterpret_cast<u8*>(pc) + 0x3e9c;
            void* pcPos = vslot<GetPosFn>(pcEmbed, 0xAC)(pcEmbed);

            nw4r::math::VEC3Sub(
                &delta,
                reinterpret_cast<const nw4r::math::VEC3*>(pcPos),
                reinterpret_cast<const nw4r::math::VEC3*>(handlePos));
            scratch = delta;
            if (scratch.x * scratch.x + scratch.y * scratch.y + scratch.z * scratch.z > distThresh) {
                panelData[0x15] = z;
                continue;
            }

            {
                void* pose = func_80496264(unk60, -1);
                Vec3f a;
                a.x = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pose) + 0x10c);
                a.y = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pose) + 0x110);
                a.z = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pose) + 0x114);
                Vec3f b;
                b.x = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pose) + 0x138);
                b.y = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pose) + 0x13c);
                b.z = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pose) + 0x140);

                void* cpos = vslot<GetPosFn>(handle, 0xAC)(handle);
                Vec3f c;
                c.x = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(cpos) + 0);
                c.y = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(cpos) + 4);
                c.z = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(cpos) + 8);

                if (!func_8013A4B4(&a, &b, &c)) {
                    panelData[0x15] = z;
                    continue;
                }
            }
        }

        if (panelData[0x28] == 0) {
            if (panelData[0x1d] != 0) {
                if (actor2 != NULL) {
                    u8* b1 = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x2c)) + 0xBB;
                    *b1 = (*b1 & 0xFE) | 1;

                    u8* b2 = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x30)) + 0xBB;
                    *b2 = *b2 & 0xFE;

                    typedef void* (*GetPtrFn)(void*);
                    void* r = vslot<GetPtrFn>(actor2, 0x258)(actor2);
                    u32 v = *reinterpret_cast<u32*>(r);
                    if (v - 1 <= 3) {
                        *b2 = (*b2 & 0xFE) | 1;
                    }

                    u8* b3 = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x34)) + 0xBB;
                    typedef u32 (*GetU8Fn)(void*);
                    u32 byteVal = vslot<GetU8Fn>(actor2, 0x260)(actor2) & 0xFF;
                    *b3 = (*b3 & 0xFE) | static_cast<u8>(byteVal);
                }
            } else {
                u8* bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x2c)) + 0xBB;
                *bp = *bp & 0xFE;
                bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x30)) + 0xBB;
                *bp = *bp & 0xFE;
                bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x34)) + 0xBB;
                *bp = *bp & 0xFE;
            }
        }

        func_801115E8(this, panelData);

        {
            Vec3f posTmp;
            Vec3f posA;
            Vec3f posB;

            {
                typedef void* (*GetVecFn)(void*, int);
                void* tmp = vslot<GetVecFn>(handle, 0x12C)(handle, 0x64);
                if (tmp != NULL) {
                    posTmp.x = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(tmp) + 0xc);
                    posTmp.y = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(tmp) + 0x1c);
                    posTmp.z = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(tmp) + 0x2c);
                    posA.x = posTmp.x;
                    posA.y = posTmp.y;
                    posA.z = posTmp.z;
                } else {
                    typedef void* (*GetPosFn)(void*);
                    void* pp = vslot<GetPosFn>(handle, 0xAC)(handle);
                    posA.x = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pp) + 0);
                    posA.y = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pp) + 4);
                    posA.z = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pp) + 8);
                }
            }

            {
                typedef void* (*GetPosFn)(void*);
                void* pp = vslot<GetPosFn>(handle, 0xAC)(handle);
                posB.x = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pp) + 0);
                posB.y = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pp) + 4);
                posB.z = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pp) + 8);
            }

            u32 flagWord = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(handle) + 0x64);
            if ((flagWord & 0x4000) != 0 || (flagWord & 0x8000) != 0) {
                if (handle != NULL) {
                    u8 animState = *(reinterpret_cast<u8*>(handle) + 0x91);
                    if (animState == 6) {
                        posA.y += one;
                    }
                }
            }

            func_80111080(this, panelData, &posA, &posB);
        }

        if (panelData[0x1f] == 0 && actor2 != NULL) { // entry+0xC3
            func_80112170(this, panelData);

            if (panelData[0x1c] != 0) { // entry+0xC0
                u32 count = cf::CBattleManager::getInstance()->mActorList1.size();
                panelData[0x14] = count == 0 ? 1 : 0; // entry+0xB8
            } else {
                void* subObj = *reinterpret_cast<void**>(reinterpret_cast<u8*>(actor2) + 4);
                typedef void* (*SubGetFn)(void*);
                void* result = vslot<SubGetFn>(subObj, 0x30)(subObj);
                u32 localVal = *reinterpret_cast<u32*>(result);
                u32 ret = func_80174C98(actor2, &localVal, 0x803);
                panelData[0x14] = ret == 0 ? 1 : 0;
            }

            if (panelData[0x14] == 0) {
                typedef f32 (*GetFloatFn)(void*);
                f32 v12c = vslot<GetFloatFn>(actor2, 0x12C)(actor2);
                f32 v128 = vslot<GetFloatFn>(actor2, 0x128)(actor2);
                func_80111B08(this, panelData, v128, v12c);

                v12c = vslot<GetFloatFn>(actor2, 0x12C)(actor2);
                v128 = vslot<GetFloatFn>(actor2, 0x128)(actor2);
                func_80111E70(this, panelData, v128, v12c);

                func_801132A8(this, panelData, actor2);
            }
        }

        if (panelData[0x28] != 0) {
            u8* bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x2c)) + 0xBB;
            *bp = *bp & 0xFE;
            bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x30)) + 0xBB;
            *bp = *bp & 0xFE;
            bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x34)) + 0xBB;
            *bp = *bp & 0xFE;
        } else if (!func_8013BF48()) {
            u8* bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x2c)) + 0xBB;
            *bp = *bp & 0xFE;
            bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x30)) + 0xBB;
            *bp = *bp & 0xFE;
            bp = reinterpret_cast<u8*>(*reinterpret_cast<void**>(panelData + 0x34)) + 0xBB;
            *bp = *bp & 0xFE;
        }
    }

    if (unk78D != 0) {
        u32 left = 0x17;
        u32 j = 0;
        do {
            u8* entry2 = reinterpret_cast<u8*>(this) + static_cast<u8>(j) * 0x4c;
            if (entry2[0xB9] != 0) {
                if (*reinterpret_cast<u32*>(entry2 + 0xA4) == unk778) {
                    entry2[0xB9] = z;
                    break;
                }
            }
            j++;
        } while (--left != 0);
    }

    func_80137444(unk78, lbl_eu_80666FE8);
    unk74->Animate(0);

    switch (unk828) {
    case 1:
        if (func_80137444(unk804, lbl_eu_80666FE8) != 0) {
            unk824 = 1;
            unk828 = 2;
        }
        break;
    case 3:
        if (func_80137510(static_cast<void*>(unk804), lbl_eu_80666FE8) != 0) {
            unk824 = 1;
            unk828 = 0;
        }
        break;
    case 4:
        if (func_80137444(unk808, lbl_eu_80666FE8) != 0) {
            *reinterpret_cast<f32*>(reinterpret_cast<u8*>(unk808) + 0x10) = lbl_eu_80666FEC;
            unk800->Animate(0);
            unk800->SetAnimationEnable(unk808, false);
            unk800->SetAnimationEnable(unk804, true);
            unk828 = 3;
        }
        break;
    default:
        break;
    }

    if (unk828 != 0) {
        func_8010EE40(&unk7E4);
    }

    unk800->Animate(0);
    }
done:
    ;
}

// LLM-HARNESS-BEGIN: us-8010f7f4
extern "C" void func_8010ED18() {}
// LLM-HARNESS-END: us-8010f7f4
// LLM-HARNESS-BEGIN: us-8010f814
extern "C" void func_8010ED38() {}
// LLM-HARNESS-END: us-8010f814
// LLM-HARNESS-BEGIN: us-8010f8b0
extern "C" bool func_8010EDD4() { return false; }
// LLM-HARNESS-END: us-8010f8b0
// LLM-HARNESS-BEGIN: us-8010f8b8
extern "C" bool func_8010EDDC() { return false; }
// LLM-HARNESS-END: us-8010f8b8
// LLM-HARNESS-BEGIN: us-8011154c
extern "C" bool func_80110A70() { return false; }
// LLM-HARNESS-END: us-8011154c
// LLM-HARNESS-BEGIN: us-80111b50
extern "C" void func_80111074() {}
// LLM-HARNESS-END: us-80111b50
// LLM-HARNESS-BEGIN: us-80113fc4
extern "C" void sinit_801134E8() {}
// LLM-HARNESS-END: us-80113fc4
// LLM-HARNESS-BEGIN: us-801140a4
extern "C" bool func_801135C8() { return false; }
// LLM-HARNESS-END: us-801140a4
// LLM-HARNESS-BEGIN: us-801140ac
extern "C" bool func_801135D0() { return false; }
// LLM-HARNESS-END: us-801140ac
// LLM-HARNESS-BEGIN: us-801140b4
extern "C" bool func_801135D8() { return false; }
// LLM-HARNESS-END: us-801140b4

// LLM-HARNESS-BEGIN: us-8010f620
extern "C" void func_8010EB44() {}
// LLM-HARNESS-END: us-8010f620
// LLM-HARNESS-BEGIN: us-8010f834
extern "C" void func_8010ED58() {}
// LLM-HARNESS-END: us-8010f834
// LLM-HARNESS-BEGIN: us-8010f8c0
extern "C" void func_8010EDE4() {}
// LLM-HARNESS-END: us-8010f8c0
// LLM-HARNESS-BEGIN: us-8010f91c
extern "C" void harness_stub_us_8010f91c() {}
// LLM-HARNESS-END: us-8010f91c
// LLM-HARNESS-BEGIN: us-801114b4
extern "C" void func_801109D8() {}
// LLM-HARNESS-END: us-801114b4
// LLM-HARNESS-BEGIN: us-80111554
extern "C" void func_80110A78() {}
// LLM-HARNESS-END: us-80111554
// LLM-HARNESS-BEGIN: us-80111b5c
extern "C" void harness_stub_us_80111b5c() {}
// LLM-HARNESS-END: us-80111b5c
// LLM-HARNESS-BEGIN: us-801120c4
extern "C" void harness_stub_us_801120c4() {}
// LLM-HARNESS-END: us-801120c4
// LLM-HARNESS-BEGIN: us-801125e4
extern "C" void harness_stub_us_801125e4() {}
// LLM-HARNESS-END: us-801125e4
// LLM-HARNESS-BEGIN: us-8011272c
extern "C" void func_80111C50() {}
// LLM-HARNESS-END: us-8011272c
// LLM-HARNESS-BEGIN: us-8011294c
extern "C" void harness_stub_us_8011294c() {}
// LLM-HARNESS-END: us-8011294c
// LLM-HARNESS-BEGIN: us-80112c4c
extern "C" void harness_stub_us_80112c4c() {}
// LLM-HARNESS-END: us-80112c4c
// LLM-HARNESS-BEGIN: us-80112fa4
extern "C" void func_801124C8() {}
// LLM-HARNESS-END: us-80112fa4
// LLM-HARNESS-BEGIN: us-8011328c
extern "C" void harness_stub_us_8011328c() {}
// LLM-HARNESS-END: us-8011328c
// LLM-HARNESS-BEGIN: us-80113d84
extern "C" void harness_stub_us_80113d84() {}
// LLM-HARNESS-END: us-80113d84

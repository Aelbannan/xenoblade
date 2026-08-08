#include "kyoshin/menu/CMenuEnemyState.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/work/CProcess.hpp"

#include "decomp.h"
#include <nw4r/math.h>
#include <revolution/GX.h>

// ---------------------------------------------------------------------------
// Local overlay structs for external objects accessed via pointer arithmetic.
// Each matches the retail layout of the accessed fields.
// ---------------------------------------------------------------------------

// Many rendering / actor objects have a flag byte at +0xBB.
struct ObjBBFlag {
    u8 gap00[0xBB];
    u8 flagBB;           // +0xBB, bit 0 = visible / highlight
};

// Object with flag word at +0x64 and anim-state byte at +0x91.
// (CfObjectMove / CfObjectPc base region.)
struct Obj64_91 {
    u8 gap00[0x64];
    u32 word64;           // +0x64  bitfield flags
    u8 gap68[0x91 - 0x68];
    u8 byte91;            // +0x91  animation state id
};

// Actor2 from func_8016FE34 - has a sub-object pointer at +0x04 and
// a larger sub-object pointer at +0x3F34.
struct Actor2Layout {
    u8 gap00[0x04];
    void* subObj4;        // +0x04
    u8 gap08[0x3F34 - 0x08];
    void* sub3F34;        // +0x3F34
};

// The object that actor2->sub3F34 points to.
struct Sub3F34Layout {
    u8 gap00[0x7A4];
    u32 bits7A4;          // +0x7A4  bitfield (bit 26 checked)
};

// CfObjectPc has an embedded sub-object at +0x3E9C with its own vtable.
struct PcEmbedLayout {
    void* vtable;         // +0x00 (at pc+0x3E9C)
};

// Pose data returned by func_80496264(unk60, -1).
// Two Vec3f clusters at +0x10C and +0x138.
struct PoseLayout {
    u8 gap00[0x10C];
    f32 vec10c_x;
    f32 vec10c_y;
    f32 vec10c_z;
    u8 gap118[0x138 - 0x118];
    f32 vec138_x;
    f32 vec138_y;
    f32 vec138_z;
};

// Object returned by vslot GetVecFn(handle, 0x12C).
// Three f32 components at 16-byte stride (0x0C / 0x1C / 0x2C).
struct RLayout {
    u8 gap00[0x0C];
    f32 val0C;
    u8 gap10[0x1C - 0x10];
    f32 val1C;
    u8 gap20[0x2C - 0x20];
    f32 val2C;
};

// Minimal overlay for nw4r::lyt::AnimTransform field at +0x10.
struct AnimTransformOverlay {
    u8 gap00[0x10];
    f32 field10;
};

// ---------------------------------------------------------------------------
// End of overlay structs
// ---------------------------------------------------------------------------

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

extern const f32 lbl_eu_80666FEC;
extern const f32 lbl_eu_80667004;
}

CMenuEnemyState::CMenuEnemyState(void* scn) : unk54(0), unk55(0){
    // NV decl order targets retail homes: r31=this, r30=zero, r29=scn.
    u32 zero;
    CMenuEnemyCtorProcess* process;
    u8* ptmfBase;
    char* vtFinal;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;
    MenuEnemyPanel* panel;
    MenuEnemyPanel* panelEnd;
    f32 panelMarker;
    u8 tmp[0x48];
    u32 copy;

    process = reinterpret_cast<CMenuEnemyCtorProcess*>(this);
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
    unk54 = 0;
    unk55 = 0;
    process->vtable = vtFinal;
    vtPtr1 = vtFinal + 0x24;
    vtPtr2 = vtFinal + 0xac;
    unk60 = scn;

    __ct__17UnkClass_8045F564Fv(unk64);

    unk74 = NULL;
    panel = panels;
    panelMarker = lbl_eu_80666FEC;
    panelEnd = &panels[24];
    unk78 = NULL;
    field7C = zero;
    field80 = zero;
    field84 = zero;
    field88 = zero;
    field8C = zero;
    field90 = zero;
    field94 = zero;
    field98 = zero;
    field9C = zero;
    fieldA0 = zero;

    // do-while + live panelEnd matches retail fall-into-body cmplw/blt shape.
    // panelEnd/one r0/r3 Chaitin soft-cap - keep iterating in high-level C.
    do {
        panel->actorId = zero;
        panel->layout1 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panel->unk08 = zero;
        panel->layout2 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panel->unk10 = zero;
        panel->drawLayout0Flag = 0;
        panel->visible = 0;
        panel->animMarker = panelMarker;
        panel->unk1C = 0;
        panel->unk1D = 0;
        panel->unk1E = 0;
        panel->unk1F = 0;
        panel->unk20 = 0;
        panel->unk21 = 0;
        panel->unk22 = 0;
        panel->unk24 = zero;
        panel->panelType = 0;
        panel->unk29 = 1;
        panel->obj1 = reinterpret_cast<void*>(zero);
        panel->obj2 = reinterpret_cast<void*>(zero);
        panel->obj3 = reinterpret_cast<void*>(zero);
        panel->unk38 = zero;
        panel->unk3C = zero;
        panel->unk40 = zero;
        panel->unk44 = zero;
        panel->unk48 = zero;
        panel++;
    } while (panel < panelEnd);

    zero = 0;
    field7C4 = 0;
    field7C8 = panelMarker;
    field7CC = panelMarker;
    field7D0 = panelMarker;
    field7D4 = zero;
    field7D8 = panelMarker;
    field7DC = zero;
    field7E0 = zero;

    __ct__CPcSelectCursor(&selectCursor);

    field82C = zero;
    unk830 = zero;
    unk834 = 0;
    unk838 = lbl_eu_80667004;

    __ct__CPcSelectCursor(tmp);
    {
        CPcSelectCursorLayout* tmpSel = reinterpret_cast<CPcSelectCursorLayout*>(tmp);
        copy = tmpSel->field04;
        selectCursor.field04 = copy;
        copy = tmpSel->field08;
        selectCursor.field08 = copy;
        copy = tmpSel->field0C;
        selectCursor.field0C = copy;
        copy = tmpSel->field10;
        selectCursor.field10 = copy;
        copy = tmpSel->field14;
        selectCursor.field14 = copy;
        copy = tmpSel->field18;
        selectCursor.field18 = copy;
        copy = reinterpret_cast<u32>(tmpSel->layout1C);
        selectCursor.layout1C = reinterpret_cast<nw4r::lyt::Layout*>(copy);
        copy = reinterpret_cast<u32>(tmpSel->anim20);
        selectCursor.anim20 = reinterpret_cast<nw4r::lyt::AnimTransform*>(copy);
        copy = reinterpret_cast<u32>(tmpSel->anim24);
        selectCursor.anim24 = reinterpret_cast<nw4r::lyt::AnimTransform*>(copy);
        copy = tmpSel->field28;
        selectCursor.field28 = copy;
        copy = tmpSel->field2C;
        selectCursor.field2C = copy;
        copy = tmpSel->field30;
        selectCursor.field30 = copy;
        copy = tmpSel->field34;
        selectCursor.field34 = copy;
        copy = tmpSel->field38;
        selectCursor.field38 = copy;
        copy = tmpSel->field3C;
        selectCursor.field3C = copy;
        selectCursor.byte40 = tmpSel->byte40;
        selectCursor.byte41 = tmpSel->byte41;
        copy = tmpSel->field44;
        selectCursor.field44 = copy;
    }
    __dt__17UnkClass_8045F564Fv(tmp + 0x08, -1);

    return;
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

void func_8010EE40(void* self);
void func_80111080(CMenuEnemyState* self, u8* panelData, void* posA, void* posB);
void func_801115E8(CMenuEnemyState* self, u8* panelData);
void func_80111B08(CMenuEnemyState* self, u8* panelData, f32 v128, f32 v12c);
void func_80111E70(CMenuEnemyState* self, u8* panelData, f32 v128, f32 v12c);
void func_80112170(CMenuEnemyState* self, u8* panelData);
void func_801127B0(CMenuEnemyState* self);
void func_801132A8(CMenuEnemyState* self, u8* panelData, void* actor);
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

extern "C" void* func_8016FE34(void* r3);
cf::CfObjectSelectorObj* func_800FE68C();
int func_8013BF48();
void func_800BBA08(void* r3);
void func_800BBA7C(void* r3);
int func_8013A4B4(void* a, void* b, void* c);


// Retail leaves f1=vt+0x128 and f2=vt+0x12C live into these calls.


int func_80174C98(void* actor, int* outVal, int flags);
void* func_80496264(void* obj, int index);

// Retail leaves func_80137510 unmangled (declared extern "C" in
// code_80135FDC.hpp); calls pass the AnimTransform* directly.
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
            // pass-before-order + block-scoped limit → order=r9, j=r12, swapped=r11
            // (retail). Peak 99.172%: pass/limit/pair still color r8/r10/r30 vs
            // retail r10/r5/r8; XOR operand/dest Chaitin follows from that.
            u8 pass = 0;
            u32* order = indices;
            s32 limit;
            u32 left;
            for (left = 0x17; left != 0; left--) {
                u8 swapped = 0;
                u8 pass8 = pass;
                limit = 0x17 - pass8;
                u8 j = 0;
                goto sort_test;
            sort_body: {
                    u32* pair = &indices[j];
                    u32 idxA = order[j];
                    u32 idxB = pair[1];
                    f32 depthB = panels[idxB].animMarker;
                    f32 depthA = panels[idxA].animMarker;
                    if (depthA > depthB) {
                        u32 tmp = order[j] ^ pair[1];
                        order[j] = tmp;
                        swapped = 1; // retail sets this before finishing the XOR swap
                        tmp = pair[1] ^ tmp;
                        pair[1] = tmp;
                        tmp = order[j] ^ tmp;
                        order[j] = tmp;
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
        // u32 counter; cast to u8 only when indexing. Compare stays cmpli (no
        // terminal clrlwi) so .text is retail 0x274.
        {
            u32 i;
            u32* order;
            order = indices;
            i = 0;
            do {
                MenuEnemyPanel& p = panels[order[static_cast<u8>(i)]];
                if (p.visible == 0) {
                    goto draw_next;
                }
                if (p.unk29 == 0) {
                    goto draw_next;
                }
                if (p.panelType != 0) {
                    func_80137038(unk74, &drawInfo, 0, 1);
                }
                {
                    int drawFlag = (p.panelType == 0) ? 0 : 1;
                    drawFlag = (drawFlag == 0) ? 1 : 0;
                    func_80137038(p.layout2, &drawInfo, drawFlag, 1);
                }
                if (p.drawLayout0Flag == 0) {
                    func_80137038(p.layout1, &drawInfo, 0, 1);
                }
            draw_next:
                i++;
            } while (i < 0x18);
        }

        if (selectCursor.field44 != 0) {
            func_80137038(selectCursor.layout1C, &drawInfo, 0, 1);
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

        if (cf::CfGameManager::getPlayer(0) != NULL) {
            // (Not in the 47-reinterpret_arith findings - reading offset
            //  0x90E4 from func_800FE68C().  Overlay struct with a u32 at
            //  that offset would be unwieldy; keep the original expression.)
            u32 lastId = *reinterpret_cast<u32*>(
                reinterpret_cast<u8*>(func_800FE68C()) + 0x90E4);

            if (lastId != 0) {
                void* obj = func_800B708C(static_cast<int>(lastId));

                if (obj != NULL) {
                    bool active =
                        (static_cast<Obj64_91*>(obj)->word64 & 4) != 0;

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

    // Retail pre-loop materialization (80110A88..AA4): pc call, then
    // f30/r30/f31/r29/f28/r28/r27/r31 - declare in that dependence order.
    cf::CfObjectPc* pc =
        func_800BFC68(cf::CfGameManager::getPlayer(0));
    f32 animMarker = lbl_eu_80666FEC;
    f32 distThresh = lbl_eu_80667014;
    nw4r::math::VEC3 scratch;
    nw4r::math::VEC3* pScratch = &scratch;
    f32 one = lbl_eu_80666FE8;
    nw4r::math::VEC3 delta;
    nw4r::math::VEC3* pDelta = &delta;
    u8 i = 0;
    u8 z = 0;

    for (; i < 0x18; i++) {
        MenuEnemyPanel& panel = panels[i];

        if (panel.visible == 0) {
            continue;
        }

        if (panel.panelType == 0) {
            if (func_8013BF48()) {
                u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
                u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
                u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
                b1 = (b1 & 0xFE) | 1;
                b2 = (b2 & 0xFE) | 1;
                b3 = (b3 & 0xFE) | 1;
            }
        }

        u32 actorId = panel.actorId;
        void* handle = func_800B708C(static_cast<int>(actorId));
        if (handle == NULL) {
            panel.visible = z;
            continue;
        }

        // r24 in retail's loop = result of func_8016FE34 (not the early target).
        void* actor2 = func_8016FE34(NULL);
        int skipDist = 0;
        int hasSub = 0;
        if (actor2 != NULL) {
            void* sub3f34 = static_cast<Actor2Layout*>(actor2)->sub3F34;
            if (sub3f34 != NULL) {
                hasSub = 1;
            }
        }
        if (hasSub) {
            void* sub3f34 = static_cast<Actor2Layout*>(actor2)->sub3F34;
            u32 bits = static_cast<Sub3F34Layout*>(sub3f34)->bits7A4;
            if ((bits >> 26) & 1) {
                skipDist = 1;
            }
        }

        if (actor2 != NULL) {
            typedef f32 (*GetFloatFn)(void*);
            f32 stateVal = vslot<GetFloatFn>(actor2, 0x128)(actor2);
            // Retail: state==FEC && panelData[0x1c]==0 → always cull.
            if (animMarker == stateVal) {
                if (panel.unk1C == 0) {
                    panel.visible = z;
                    continue;
                }
            }
        }
        if (skipDist) {
            panel.visible = z;
            continue;
        }

        // Distance cull + frustum test only when panelType==0 and pc!=NULL.
        // Retail skips both when either gate fails (falls through to flag work).
        if (panel.panelType == 0 && pc != NULL) {
            typedef void* (*GetPosFn)(void*);
            void* handlePos = vslot<GetPosFn>(handle, 0xAC)(handle);
            PcEmbedLayout* pcEmbed = reinterpret_cast<PcEmbedLayout*>(pc);
            void* pcPos = vslot<GetPosFn>(&pcEmbed->vtable, 0xAC)(&pcEmbed->vtable);

            nw4r::math::VEC3Sub(
                pDelta,
                reinterpret_cast<const nw4r::math::VEC3*>(pcPos),
                reinterpret_cast<const nw4r::math::VEC3*>(handlePos));
            scratch = delta;
            // Retail: interleaved assign + VEC3LenSq (ps_mul/ps_madd/ps_sum0).
            f32 distSq = nw4r::math::VEC3LenSq(pScratch);
            if (distSq > distThresh) {
                panel.visible = z;
                continue;
            }

            {
                PoseLayout* pose = static_cast<PoseLayout*>(func_80496264(unk60, -1));
                Vec3f a;
                a.x = pose->vec10c_x;
                a.y = pose->vec10c_y;
                a.z = pose->vec10c_z;
                Vec3f b;
                b.x = pose->vec138_x;
                b.y = pose->vec138_y;
                b.z = pose->vec138_z;

                void* qpos = vslot<GetPosFn>(handle, 0xAC)(handle);
                Vec3f c = *static_cast<const Vec3f*>(qpos);

                if (!func_8013A4B4(&a, &b, &c)) {
                    panel.visible = z;
                    continue;
                }
            }
        }

        if (panel.panelType == 0) {
            if (panel.unk1D != 0) {
                if (actor2 != NULL) {
                    u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
                    u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
                    u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;

                    b1 = (b1 & 0xFE) | 1;
                    b2 = b2 & 0xFE;

                    typedef void* (*GetPtrFn)(void*);
                    void* r = vslot<GetPtrFn>(actor2, 0x258)(actor2);
                    u32 v = *reinterpret_cast<u32*>(r);
                    if (v - 1 <= 3) {
                        b2 = (b2 & 0xFE) | 1;
                    }

                    typedef u32 (*GetU8Fn)(void*);
                    u32 byteVal = vslot<GetU8Fn>(actor2, 0x260)(actor2) & 0xFF;
                    b3 = (b3 & 0xFE) | static_cast<u8>(byteVal);
                }
            } else {
                u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
                u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
                u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
                b1 = b1 & 0xFE;
                b2 = b2 & 0xFE;
                b3 = b3 & 0xFE;
            }
        }

        func_801115E8(this, reinterpret_cast<u8*>(&panel));

        {
            Vec3f posTmp;
            Vec3f posA;
            Vec3f posB;
            Vec3f* posTmpPtr = &posTmp;

            typedef void* (*GetVecFn)(void*, int);
            void* r = vslot<GetVecFn>(handle, 0x12C)(handle, 0x64);
            if (r != NULL) {
                RLayout* rl = static_cast<RLayout*>(r);
                posTmpPtr->x = rl->val0C;
                posTmpPtr->y = rl->val1C;
                posTmpPtr->z = rl->val2C;
                posA = *posTmpPtr;
            } else {
                typedef void* (*GetPosFn)(void*);
                void* p = vslot<GetPosFn>(handle, 0xAC)(handle);
                posA = *static_cast<const Vec3f*>(p);
            }

            typedef void* (*GetPosFn)(void*);
            void* p2 = vslot<GetPosFn>(handle, 0xAC)(handle);
            posB = *static_cast<const Vec3f*>(p2);

            Obj64_91* hf = static_cast<Obj64_91*>(handle);
            u32 flagWord = hf->word64;
            if ((flagWord & 0x4000) != 0 || (flagWord & 0x8000) != 0) {
                if (handle != NULL) {
                    u8 animState = hf->byte91;
                    if (animState == 6) {
                        posA.y += one;
                    }
                }
            }

            func_80111080(this, reinterpret_cast<u8*>(&panel), &posA, &posB);
        }

        if (panel.unk1F == 0 && actor2 != NULL) {
            func_80112170(this, reinterpret_cast<u8*>(&panel));

            if (panel.unk1C != 0) {
                u32 count = cf::CBattleManager::getInstance()->mActorList1.size();
                panel.drawLayout0Flag = (count == 0) ? 1 : 0;
            } else {
                void* subObj = static_cast<Actor2Layout*>(actor2)->subObj4;
                typedef void* (*SubGetFn)(void*);
                void* result = vslot<SubGetFn>(subObj, 0x30)(subObj);
                int localVal = *reinterpret_cast<int*>(result);
                u32 ret = func_80174C98(actor2, &localVal, 0x803);
                panel.drawLayout0Flag = (ret == 0) ? 1 : 0;
            }

            if (panel.drawLayout0Flag == 0) {
                typedef f32 (*GetFloatFn)(void*);
                f32 v12c = vslot<GetFloatFn>(actor2, 0x12C)(actor2);
                f32 v128 = vslot<GetFloatFn>(actor2, 0x128)(actor2);
                func_80111B08(this, reinterpret_cast<u8*>(&panel), v128, v12c);

                v12c = vslot<GetFloatFn>(actor2, 0x12C)(actor2);
                v128 = vslot<GetFloatFn>(actor2, 0x128)(actor2);
                func_80111E70(this, reinterpret_cast<u8*>(&panel), v128, v12c);

                func_801132A8(this, reinterpret_cast<u8*>(&panel), actor2);
            }
        }

        if (panel.panelType != 0) {
            u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
            u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
            u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
            b1 = b1 & 0xFE;
            b2 = b2 & 0xFE;
            b3 = b3 & 0xFE;
        } else if (!func_8013BF48()) {
            u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
            u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
            u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
            b1 = b1 & 0xFE;
            b2 = b2 & 0xFE;
            b3 = b3 & 0xFE;
        }
    }

    if (panels[23].visible != 0) {
        u32 left = 0x17;
        u32 j = 0;
        do {
            MenuEnemyPanel& panel2 = panels[j];
            if (panel2.visible != 0) {
                if (panel2.actorId == panels[23].actorId) {
                    panel2.visible = z;
                    break;
                }
            }
            j++;
        } while (--left != 0);
    }

    func_80137444(unk78, lbl_eu_80666FE8);
    unk74->Animate(0);

    switch (selectCursor.field44) {
    case 1:
        if (func_80137444(selectCursor.anim20, lbl_eu_80666FE8) != 0) {
            selectCursor.byte40 = 1;
            selectCursor.field44 = 2;
        }
        break;
    case 3:
        if (func_80137510(selectCursor.anim20, lbl_eu_80666FE8) != 0) {
            selectCursor.byte40 = 1;
            selectCursor.field44 = 0;
        }
        break;
    case 4:
        if (func_80137444(selectCursor.anim24, lbl_eu_80666FE8) != 0) {
            reinterpret_cast<AnimTransformOverlay*>(selectCursor.anim24)->field10 = lbl_eu_80666FEC;
            selectCursor.layout1C->Animate(0);
            selectCursor.layout1C->SetAnimationEnable(selectCursor.anim24, false);
            selectCursor.layout1C->SetAnimationEnable(selectCursor.anim20, true);
            selectCursor.field44 = 3;
        }
        break;
    default:
        break;
    }

    if (selectCursor.field44 != 0) {
        func_8010EE40(&selectCursor);
    }

    selectCursor.layout1C->Animate(0);
    }
done:
    ;
}

extern "C" void func_8010EB44() {}
extern "C" void func_8010ED58() {}
extern "C" void func_8010EDE4() {}
extern "C" void func_801109D8() {}
extern "C" void func_80110A78() {}
extern "C" void func_80111C50() {}
extern "C" void func_801124C8() {}

extern "C" void Init__15CMenuEnemyStateFv() {}
extern "C" void Term__15CMenuEnemyStateFv() {}

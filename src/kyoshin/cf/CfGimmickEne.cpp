/**
 * @file CfGimmickEne.cpp
 * @brief Translation unit for cf::CfGimmickEne (enemy gimmick).
 *
 * All functions are currently auto-scaffolded stubs. Only FULL_MATCH
 * symbols have real bodies:
 *   func_8026E5BC -- empty virtual override (just blr, 4 bytes)
 *   func_8026F31C -- empty dispatch handler  (just blr, 4 bytes)
 */

#include "kyoshin/cf/CfGimmickEne.hpp"

#include <revolution/GX.h>
#include "monolib/math/CMat34.hpp"

using cf::CfGimmickEne;
using cf::CfGimmickEneActor;
using cf::CfGimmickEneActorVt;
using cf::CfGimmickEneScnProc;
using cf::CfGimmickEneSceneState;

// ---------------------------------------------------------------------------
// Constructor (cf::CfGimmickEne ctor, unmangled linker name).
//
// Populates every field from BDAT columns: the fixed column-name block
// (lbl_eu_8050E6A4), a runtime-patched 4-name table (lbl_eu_80537A68), and a
// runtime-patched color-name buffer (lbl_eu_806629B0). The two iteration
// loops read the per-cell columns for the marker's byte arrays (0x70/0x88)
// and the color RGB bytes (0x97..0x99).
// ---------------------------------------------------------------------------

void* __ct__cf_CfGimmickEne(cf::CfGimmickEne* self, u16 row) {
    __ct__cf_CfGimmick(self);
    self->vtable = (void*)lbl_eu_80537A78;
    self->typeId = 7;

    void* bdat = func_8003AA34();
    void* holder = lbl_eu_80664144;
    self->field_64 = row;
    func_80208F34(self, (float*)((u8*)self + 4), bdat, &holder);

    // Fixed BDAT columns. Each cell value is returned as a pointer/word; the
    // low 16/8 bits are stored into the field (the retail spills every result
    // to a stack slot and reloads the truncated width -- the pointer idiom
    // `*(u16*)&s` reproduces that layout).
    const char* s1 = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4, row);
    self->field_84 = *(u16*)&s1;
    const char* s2 = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x06, row);
    self->field_86 = *(u16*)&s2;
    const char* s3 = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x0C, row);
    // Runtime-patched column names: "a{r}{i}" for the 4-cell arrays and
    // "a{r}<suffix>" for the single-cell fields.
    char** names = lbl_eu_80537A68;
    self->field_9A = *(u8*)&s3;
    for (int r = 0; r < 2; r++) {
        u8* dst = (r == 0) ? &self->field_70 : self->field_88;
        names[3][0] = 'a' + r;
        names[2][0] = 'a' + r;
        names[1][0] = 'a' + r;
        lbl_eu_80537A68[0][0] = 'a' + r;
        for (int i = 0; i < 4; i++) {
            lbl_eu_80537A68[0][4] = '1' + i;
            const char* sv = (const char*)getBdatStringColumnValue(holder, lbl_eu_80537A68[0], row);
            dst[i] = *(u8*)&sv;
        }
        const char* s4 = (const char*)getBdatStringColumnValue(holder, names[1], row);
        self->field_8C[r] = *(u8*)&s4;
        const char* s5 = (const char*)getBdatStringColumnValue(holder, names[2], row);
        self->field_90[r] = *(u8*)&s5;
        const char* s6 = (const char*)getBdatStringColumnValue(holder, names[3], row);
        self->field_8E[r] = *(u8*)&s6;
    }

    const char* s7 = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x12, row);
    self->field_96 = *(u8*)&s7;
    const char* s8 = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x16, row);
    self->field_92 = *(u16*)&s8;
    const char* s9 = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x19, row);
    self->field_94 = *(u16*)&s9;
    const char* sA = (const char*)getBdatStringColumnValue(holder, *(char**)(lbl_eu_805357E8 + 0x2C), row);
    self->field_A6 = *(u16*)&sA;

    // Color RGB cells: column names built in the shared buffer "..{i}".
    for (int i = 0; i < 3; i++) {
        lbl_eu_806629B0[2] = '1' + i;
        const char* sc = (const char*)getBdatStringColumnValue(holder, lbl_eu_806629B0, row);
        (&self->colorR)[i] = *(u8*)&sc;
    }

    const char* sB = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x1F, row);
    self->field_9B = *(u8*)&sB;
    const char* sC = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x23, row);
    self->field_9C = *(u8*)&sC;
    const char* sD = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x28, row);
    self->field_A4 = *(u16*)&sD;
    const char* sE = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x2D, row);
    self->field_9E = *(u16*)&sE;
    const char* sF = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x34, row);
    self->field_A0 = *(u16*)&sF;
    const char* s10 = (const char*)getBdatStringColumnValue(holder, lbl_eu_8050E6A4 + 0x3A, row);
    self->field_A2 = *(u16*)&s10;

    // Behaviour-flag bit 2 selects the initial dispatch entry.
    if (self->field_9A & 0x4) {
        self->dispatchIdx = 1;
    } else {
        self->dispatchIdx = 0;
    }
    self->field_B0 = lbl_eu_80668968;
    self->field_BC = 0;
    self->field_B4 = 0;
    return self;
}

cf::CfGimmickEne::~CfGimmickEne() {
    // Restore the vtable, deactivate the base gimmick, then detach and
    // destroy the ScnProc marker node if one is bound.
    this->vtable = (void*)lbl_eu_80537A78;
    func_80208EE4(this);
    if (this->field_BC != 0) {
        nw4r::g3d::ScnGroup* group = func_8048EC14(lbl_eu_80663E14, 8);
        if (group->Size() != 0) {
            group->Remove(this->field_BC);
            this->field_BC->Destroy();
        }
        this->field_BC = 0;
        this->field_74 &= ~1;
    }
}

// ---------------------------------------------------------------------------
// func_8026E584 - dispatch through the per-state member-function-pointer
// table (lbl_eu_80537A20) selected by dispatchIdx.
// ---------------------------------------------------------------------------

void func_8026E584(CfGimmickEne* self) {
    (self->*lbl_eu_80537A20[self->dispatchIdx])();
}

// ---------------------------------------------------------------------------
// FULL_MATCH: Virtual function 6 override - no-op
//
// Retail:     blr
// Vtable slot: CfGimmickEne[+0x20] (overrides CfGimmick vfunc6)
// CfGimmick base impl at func_80208988 is non-empty; Ene does nothing.
// ---------------------------------------------------------------------------

void func_8026E5BC() {}

// ---------------------------------------------------------------------------
// func_8026E5C0 - bind an actor: verify the character id, then scan the
// actor's arts slots for the configured arts id and cache the match.
// ---------------------------------------------------------------------------

void func_8026E5C0(CfGimmickEne* self, CfGimmickEneActor* actor) {
    if (self->field_84 != actor->field_3F28) {
        return;
    }
    self->field_B4 = actor;
    self->field_B8 = 0;
    if (self->field_9A & 0x4) {
        return;
    }
    for (int i = 0; i < 8; i++) {
        u16 res = func_80153CAC(self->field_B4->getArtsSet(), (s16)i);
        if (res != 0 && res == self->field_86) {
            self->field_B8 = getArtsParamByIdx(self->field_B4->getArtsSet(), i);
        }
    }
}

// ---------------------------------------------------------------------------
// func_8026E678 - unbind the actor when arg is the bound one: clear the
// active flag and fire the outro sound if configured.
// ---------------------------------------------------------------------------

void func_8026E678(CfGimmickEne* self, CfGimmickEneActor* actor) {
    if (actor == self->field_B4) {
        self->field_B4 = 0;
        self->field_74 &= 0xFDFFFFFF;
        if (self->field_94 != 0 && lbl_eu_806646C0 != 0) {
            func_801BFED0(1, lbl_eu_806646C0, 0xa);
            lbl_eu_806646C0 = 0;
        }
    }
}

// ---------------------------------------------------------------------------
// Remaining functions (scaffolds - NOT YET DECOMPILED)
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// func_8026E6E4 / func_8026E7F8 (scaffolds - NOT YET DECOMPILED)
//
// __declspec(noinline) keeps MWCC's -inline auto / -ipa inliner from folding
// these empty bodies into their retail callers (func_8026EC80 / func_8026F320
// must emit a direct `bl` to them).
// ---------------------------------------------------------------------------

__declspec(noinline) void func_8026E6E4(CfGimmickEne* self) {
    (void)self;
}

__declspec(noinline) void func_8026E7F8(CfGimmickEne* self, int arg) {
    (void)arg;

    // Marker-quad render state: identity matrix, position-only vertex
    // format, no-depth blended TEV over the gimmick's color, then draw a
    // 4-vertex triangle fan.
    GXSetCurrentMtx(0);
    GXLoadPosMtxImm(*ml::CMat34::identity, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_LEQUAL, 0xff);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
                  GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                    GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                    GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

    // Material color is fixed; the ambient channel carries the gimmick's
    // configured color and alpha. One local is reused for both calls so the
    // retail's shared home slot + per-call argument copy is reproduced.
    GXColor color = lbl_eu_80668970;
    GXSetChanMatColor(GX_COLOR0A0, color);
    color.r = self->colorR;
    color.g = self->colorG;
    color.b = self->colorB;
    color.a = self->field_A9;
    GXSetChanAmbColor(GX_COLOR0A0, color);

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);
    GXPosition3f32(-1.0f, 1.0f, -1.0f);
    GXPosition3f32(1.0f, 1.0f, -1.0f);
    GXPosition3f32(1.0f, -1.0f, 1.0f);
    GXPosition3f32(-1.0f, -1.0f, -1.0f);
}

void func_8026EA20(){}

void func_8026EBA8(CfGimmickEne* self) {
    // Requires a bound actor; clear the 0x40 busy flag first.
    if (self->field_B4 == 0) {
        return;
    }
    self->field_74 &= 0xFFFFFFBF;

    // Compare the actor's current arts value (vtable 0x128) against the
    // arts id scaled by a constant and the actor's factor (vtable 0x12C).
    f32 factor = ((CfGimmickEneActorVt*)self->field_B4)->vf12C();
    f32 artsF = lbl_eu_8066897C * (f32)self->field_86;
    f32 limit = artsF * factor;
    f32 value = ((CfGimmickEneActorVt*)self->field_B4)->vf128();
    if (value > limit) {
        return;
    }

    // Either start the timed outro state or hand off to the spawn step.
    if (self->field_A4 != 0) {
        f32 dur = lbl_eu_8066896C;
        self->field_A8 = 0;
        self->dispatchIdx = 2;
        self->field_AC = dur;
    } else {
        func_8026E6E4(self);
    }
}

// ---------------------------------------------------------------------------
// func_8026EC80 - accumulate the frame delta into the running timer and
// advance the state machine once it reaches the configured duration.
// ---------------------------------------------------------------------------

void func_8026EC80(CfGimmickEne* self) {
    f32 dt = func_80496288(lbl_eu_80663E14);
    self->field_AC += dt;
    if (self->field_AC >= (f32)self->field_A4) {
        func_8026E6E4(self);
    }
}

void func_8026ECEC(CfGimmickEne* self) {
    u8 idx = self->field_A8;

    // Duration for the current state cell; any nonzero configured duration
    // is treated as one frame.
    f32 dur = (f32)(&self->field_9E)[idx];
    if (dur != lbl_eu_8066896C) {
        dur = lbl_eu_80668968;
    }

    // Accumulate the frame delta and clamp to the duration once reached.
    f32 dt = func_80496288(lbl_eu_80663E14);
    self->field_AC += dt;
    int reached = 0;
    if (self->field_AC >= dur) {
        self->field_AC = dur;
        reached = 1;
    }

    // Fade the marker alpha: state 1 snaps to opaque, states 0/2 ramp.
    if (self->field_74 & 1) {
        if (idx == 1) {
            self->field_A9 = 0xff;
        } else {
            f32 alpha;
            if (idx == 0) {
                alpha = lbl_eu_80668988 * (self->field_AC / dur);
            } else {
                alpha = lbl_eu_80668988 * ((dur - self->field_AC) / dur);
            }
            if (alpha < lbl_eu_8066896C) {
                alpha = lbl_eu_8066896C;
            }
            if (alpha > lbl_eu_80668988) {
                alpha = lbl_eu_80668988;
            }
            self->field_A9 = (u8)alpha;
        }
    }

    // Per-cell LOD dispatch: (8 << idx) selects the cell's state bit,
    // (2 << r) is the busy flag, bit 5 marks the cells processed this frame.
    u8 cellMask = 8 << idx;
    for (int r = 0; r < 2; r++) {
        u32 flags = self->field_74;
        if (flags & (8 << r)) {
            continue;
        }
        u8* p = (r == 0) ? &self->field_70 : self->field_88;
        u8 fe = self->field_8E[r];
        if (!(fe & cellMask)) {
            continue;
        }

        if (!(flags & 0x20)) {
            int flag = 1;
            if ((fe & 1) && self->field_AC < dur) {
                flag = 0;
                if (!(flags & (2 << r)) && (fe & 2)) {
                    for (int i = 0; i < 4; i++) {
                        if (p[i]) {
                            func_80462DB4__8CTaskLODFv(p[i], 1);
                            func_804BCC30(func_804BC9EC__Fv(), p[i]);
                        }
                    }
                }
            }

            // LOD level-ratio sweep for the cell's active tasks.
            f32 ratio = self->field_AC / dur;
            if (fe & 4) {
                ratio = lbl_eu_80668968 - ratio;
            }
            for (int i = 0; i < 4; i++) {
                if (p[i]) {
                    func_80462E3C__8CTaskLODFv(p[i], ratio);
                    func_804BCC30(func_804BC9EC__Fv(), p[i]);
                }
            }

            // Finish the cell: force the final LOD state.
            if (flag) {
                if (fe & 2) {
                    for (int i = 0; i < 4; i++) {
                        if (p[i]) {
                            func_80462DB4__8CTaskLODFv(p[i], 1);
                            func_80462E3C__8CTaskLODFv(p[i], lbl_eu_80668968);
                            func_804BCC30(func_804BC9EC__Fv(), p[i]);
                        }
                    }
                } else if (fe & 4) {
                    for (int i = 0; i < 4; i++) {
                        if (p[i]) {
                            func_80462DB4__8CTaskLODFv(p[i], 0);
                            func_80462E3C__8CTaskLODFv(p[i], lbl_eu_80668968);
                            func_804BCC3C(func_804BC9EC__Fv(), p[i]);
                        }
                    }
                }
                self->field_74 |= 0x20;
            }
        }

        // Marker-size cell: seed the base size from the global, then
        // interpolate the global toward the cell's configured value.
        if (self->field_90[r] != 0) {
            if (!(self->field_74 & (2 << r))) {
                self->field_B0 = lbl_eu_80662784;
            }
            f32 ratio = self->field_AC / dur;
            lbl_eu_80662784 = ratio * (lbl_eu_8066898C * (f32)self->field_90[r]) +
                               self->field_B0 * (lbl_eu_80668968 - ratio);
        }

        // Cell visibility flag set: apply the per-LOD filter modes.
        if (!(self->field_74 & (2 << r))) {
            u8 fc = self->field_8C[r];
            if (fc != 0) {
                int b2 = fc & 4;
                int b1 = fc & 2;
                int b0 = fc & 1;
                int b3 = fc & 8;
                for (int i = 0; i < 4; i++) {
                    u8 lod = p[i];
                    if (b2) {
                        if (b1) {
                            func_80462EF4__8CTaskLODFv(lod, lbl_eu_8066896C);
                        } else {
                            func_80462F10__8CTaskLODFv(lod);
                        }
                        func_80462F4C__8CTaskLODFv(lod, 0);
                    } else if (b0) {
                        func_80462F70__8CTaskLODFv(lod, 0);
                        func_80462F4C__8CTaskLODFv(lod, 1);
                    } else if (b1) {
                        func_80462F70__8CTaskLODFv(lod, 1);
                        func_80462F4C__8CTaskLODFv(lod, 1);
                    }
                    if (b3) {
                        func_80462ED0__8CTaskLODFv(lod, 0);
                    } else {
                        func_80462ED0__8CTaskLODFv(lod, 1);
                    }
                }
            }
            self->field_74 |= (2 << r);
        }
    }

    // Advance the state machine once the timer ran out.
    if (reached) {
        idx++;
        if (idx >= 3) {
            self->dispatchIdx = 4;
            idx = 0;
        } else {
            if (self->field_74 & 2) {
                self->field_74 |= 8;
            }
            if (self->field_74 & 4) {
                self->field_74 |= 0x10;
            }
            self->field_74 &= ~0x1C;
            if (idx == 2) {
                if (self->field_9B != 0) {
                    func_8020A068((int)self, 1, self->field_9B - 1);
                }
                if (self->field_94 != 0) {
                    if (lbl_eu_806646C0 != 0) {
                        func_801BFED0(1, lbl_eu_806646C0, 0xa);
                    }
                    lbl_eu_806646C0 = func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
                        1, self->field_94, 0, 0, lbl_eu_80668968);
                }
            }
        }
        self->field_A8 = idx;
        self->field_AC = lbl_eu_8066896C;
        self->field_B0 = lbl_eu_80668968;
    }
}

void func_8026F254(CfGimmickEne* self) {
    // Detach and destroy the ScnProc marker node if one is bound (same
    // teardown as the destructor).
    if (self->field_BC != 0) {
        nw4r::g3d::ScnGroup* group = func_8048EC14(lbl_eu_80663E14, 8);
        if (group->Size() != 0) {
            group->Remove(self->field_BC);
            self->field_BC->Destroy();
        }
        self->field_BC = 0;
        self->field_74 &= ~1;
    }

    // Fire the outro sound id if one is configured.
    u16 soundId = self->field_A6;
    if (soundId != 0) {
        func_801F4B68((int)getUnk80664658(), soundId);
    }

    // Behaviour-flag bit 2 selects the dispatch-table entry (5 vs 0).
    if (self->field_9A & 0x4) {
        self->dispatchIdx = 5;
    } else {
        self->dispatchIdx = 0;
    }
    self->field_74 &= 0xFFFFFFE1;
}

// ---------------------------------------------------------------------------
// FULL_MATCH: Dispatch table entry [5] - no-op
//
// Retail:     blr
// Table at:   0x80537A20 entry 5 (index 5 of 6, pattern {0, -1, func_ptr})
// Related:    func_8026E584 uses this dispatch table.
// ---------------------------------------------------------------------------

void func_8026F31C(){}

// ---------------------------------------------------------------------------
// func_8026F320 - ScnProc callback: render the marker quad only while the
// scene is active and the callback argument is zero.
// ---------------------------------------------------------------------------

void func_8026F320(CfGimmickEneScnProc* self, int arg) {
    CfGimmickEneSceneState* state = func_8048ECD0(func_8049698C());
    if (state->field_19 == 0 && arg == 0) {
        func_8026E7F8(self->field_F0, arg);
    }
}

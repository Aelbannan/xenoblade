// monolib/src/scn/CMdlMouth - mouth-animation helper object.
#include <harness_catalog.h>
#include <string.h>
#include <monolib/math.hpp>
#include <nw4r/math.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include "nw4r/g3d/g3d_scnmdl.h"
#include <nw4r/db/db_assert.h>
#include <monolib/core/code_804E36DC.hpp>
#include "libs/monolib/src/scn/CMdlMouth.hpp"

// Minimal typed view of the owner model (a CScnItemModelNw4r) fields used
// here. 0x146C is the model resource data, 0x147C the g3d scene object.
struct CScnItemModelNw4rOwner;  // full definition in CScnItemModelNw4r.hpp
struct CMdlOwnerCtx {
    u8    field_0x00[0x4];     // 0x00 vtable pointer region
    CScnItemModelNw4rOwner* field_04;      // 0x04 owner view (func_80496288 arg)
    u8    field_0x08[0x1464];  // reserved
    nw4r::g3d::ResMdlData* field_0x146C; // 0x146C model resource data
    u8    field_0x1470[0xC];   // reserved
    nw4r::g3d::ScnObj* field_0x147C;     // 0x147C g3d scene object (ScnMdl)
};

extern "C" void func_804E6898(u8* self, float val) {
    ((CMdlMouth*)self)->value2C = val;
}

int func_804E68A0(CMdlMouth* self, u32 arg2, nw4r::g3d::ChrAnmResult* res) {
    // Mismatched own-node id: callers may only animate the bound node.
    if ((u32)self->field_0x0C != arg2) {
        return 0;
    }
    // Closing-timer decrement: while it counts down the rest is skipped.
    if (self->field_0x08 != 0 && self->field_0x28 > 0) {
        self->field_0x28 -= 1;
        if (self->field_0x28 > 0) {
            return 0;
        }
    }
    if (self->field_0x08 == 0) {
        return 1;
    }
    // Integrate mouth-open angle from the owner view's delta.
    f32 dt = func_80496288(self->field_0x04->field_04);
    self->field_0x10 = self->field_0x14 * dt + self->field_0x10;
    if (self->field_0x10 > lbl_eu_8066A1F8) {
        // Reset to fully closed, then re-randomize open/close speeds.
        self->field_0x10 = lbl_eu_8066B310;
        self->field_0x14 = lbl_eu_8066B310;
        if (self->field_0x08 != 0) {
            self->field_0x14 = lbl_eu_8066B31C * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
            self->field_0x18 = lbl_eu_8066B320 * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
        }
    }
    nw4r::math::VEC3 v;
    f32 o = self->field_0x10;
    f32 s = nw4r::math::SinFIdx(lbl_eu_8066B330 * o);
    self->field_0x1C = self->field_0x18 * s * self->value2C;
    res->GetRotateDeg(&v);
    v.z = lbl_eu_8066B334 * self->field_0x1C + v.z;
    res->SetRotateDeg(&v);
    return 1;
}

CMdlMouth::~CMdlMouth() {}

void func_804E6A28(CMdlMouth* self) {
    // Per-frame mouth update: bail when no material is bound (id < 0) or the
    // closing timer still counts down; otherwise integrate the open angle and
    // push the SRT offset-table row into the copied material's texture SRT.
    if (self->field_0x24 < 0) {
        return;
    }
    if (self->field_0x08 != 0 && self->field_0x28 > 0) {
        self->field_0x28 -= 1;
        if (self->field_0x28 > 0) {
            return;
        }
    }
    f32 dt = func_80496288(self->field_0x04->field_04);
    self->field_0x10 = self->field_0x14 * dt + self->field_0x10;
    if (self->field_0x10 > lbl_eu_8066A1F8) {
        // Reset to fully closed, then re-randomize open/close speeds.
        self->field_0x10 = lbl_eu_8066B310;
        self->field_0x14 = lbl_eu_8066B310;
        if (self->field_0x08 != 0) {
            self->field_0x14 = lbl_eu_8066B31C * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
            self->field_0x18 = lbl_eu_8066B320 * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
        }
    }
    f32 o = self->field_0x10;
    f32 s = nw4r::math::SinFIdx(lbl_eu_8066B330 * o);
    self->field_0x1C = self->field_0x18 * s * self->value2C;
    // Pick the SRT offset-table row from the open amount, clamped to [0,2].
    s32 idx = (s32)(lbl_eu_8066B338 * self->field_0x1C);
    if ((u32)idx >= 3) {
        idx = 2;
    }
    nw4r::g3d::ScnMdl* scnMdl =
        (nw4r::g3d::ScnMdl*)self->field_0x04->field_0x147C;
    nw4r::g3d::ScnMdl::CopiedMatAccess cma(scnMdl, (u32)self->field_0x24);
    nw4r::g3d::ResTexSrt srt = cma.GetResTexSrt(false);
    if (srt.IsValid()) {
        // Reflection-scale SRT matrix; column 3 carries the per-index offset.
        // Load the per-index offset into a local first: its web must be born
        // before the two shared constants so the scratch colors come out
        // f0=one, f1=neg, f2=offset (retail order), while the _03 store stays
        // in offset order.
        f32 t = lbl_eu_805247B4[idx];
        nw4r::math::MTX34 m;
        m._00 = lbl_eu_8066B314;
        m._01 = lbl_eu_8066B310;
        m._02 = lbl_eu_8066B310;
        m._03 = t;
        m._10 = lbl_eu_8066B310;
        m._11 = lbl_eu_8066B314;
        m._12 = lbl_eu_8066B310;
        m._13 = lbl_eu_8066B310;
        m._20 = lbl_eu_8066B310;
        m._21 = lbl_eu_8066B310;
        m._22 = lbl_eu_8066B314;
        m._23 = lbl_eu_8066B310;
        srt.SetEffectMtx(0, &m);
        srt.SetEffectMtx(1, &m);
        srt.SetEffectMtx(2, &m);
    }
}

void func_804E65CC(CMdlMouth* self, CMdlOwnerCtx* model) {
    // Member field init (mirrors the retail ctor stores).
    self->field_0x04 = model;
    self->field_0x08 = 0;
    self->field_0x0C = -1;
    f32 z = lbl_eu_8066B310;
    self->field_0x1C = z;
    self->field_0x10 = z;
    self->field_0x14 = z;
    self->field_0x18 = z;
    self->field_0x24 = -1;
    self->field_0x28 = 0;
    self->value2C = lbl_eu_8066B314;

    // Bind the named node if present; the inner panics are DB_ASSERTs on the
    // same validity check, so MWCC reuses the outer compare (dead code).
    nw4r::g3d::ResNode node =
        nw4r::g3d::ResMdl(model->field_0x146C).GetResNode(lbl_eu_80663C58);
    if (node.IsValid()) {
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
        }
        self->field_0x0C = node.GetID();
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_80663C6C);
        }
        self->field_0x20 = node.ref().rot.z;
        // The model object is really a CScnItemModelNw4r; the view struct
        // above only exposes the fields this TU reads.
        func_80488F44((CScnItemModelNw4r*)model, self->field_0x0C);
    }

    // Scan materials for the eye-material name and stash its id. Each call
    // builds a fresh temporary ResMdl from the model resource (retail shape).
    u32 num =
        nw4r::g3d::ResMdl(model->field_0x146C).GetResMatNumEntries();
    for (u32 i = 0; i < num; i++) {
        nw4r::g3d::ResMat resMat =
            nw4r::g3d::ResMdl(model->field_0x146C).GetResMat(i);
        if (!resMat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C68);
        }
        const char* nm = resMat.ofs_to_ptr<char>(resMat.ref().name);
        if (strstr(nm, lbl_eu_80663C5C) != NULL) {
            if (!resMat.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                                lbl_eu_806638E8, lbl_eu_80663C68);
            }
            self->field_0x24 = resMat.ref().id;
            break;
        }
    }
}

void func_804E679C(CMdlMouth* self, int arg2, int arg3) {
    // All follow-up blocks are guarded by the mode-change condition: retail skips
    // the whole body with a single branch when field_0x08 already equals arg2.
    if (self->field_0x08 != arg2) {
        self->field_0x08 = arg2;
        if (arg2 != 0) {
            self->field_0x14 = lbl_eu_8066B31C * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
            self->field_0x18 = lbl_eu_8066B320 * (f32)ml::math::mtRand(100) + lbl_eu_8066B318;
        }
        if (arg2 != 0) {
            self->field_0x28 = 2;
        }
        if (arg3 != 0) {
            if (self->field_0x08 == 0) {
                f32 z = lbl_eu_8066B310;
                self->field_0x1C = z;
                self->field_0x10 = z;
                self->field_0x14 = z;
                self->field_0x18 = z;
            }
        }
    }
}

CMdlMouth::CMdlMouth() {
    field_0x04 = 0;
    field_0x08 = 0;
    field_0x0C = -1;
    f32 v = lbl_eu_8066B310;
    field_0x10 = v;
    field_0x14 = v;
    field_0x18 = v;
    field_0x1C = v;
    field_0x24 = -1;
    field_0x28 = 0;
    value2C = lbl_eu_8066B314;
}
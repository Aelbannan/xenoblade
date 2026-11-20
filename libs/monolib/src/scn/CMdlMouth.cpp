// monolib/src/scn/CMdlMouth - mouth-animation helper object.
#include <harness_catalog.h>
#include "monolib/scn/CScn.hpp"
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

// Retail fragments (see the data block at the bottom): the class ctor/dtor
// are defined as extern "C" functions so MWCC emits no auto vtable/RTTI.
extern "C" void* __dt__9CMdlMouthFv(CMdlMouth* ths, int flag);
extern "C" u32 lbl_eu_805701B0[];

// Minimal typed view of the owner model (a CScnItemModelNw4r) fields used
// here. 0x146C is the model resource data, 0x147C the g3d scene object.
struct CScnItemModelNw4rOwner;  // full definition in CScnItemModelNw4r.hpp
struct CMdlOwnerCtx {
    u8    field_0x00[0x4];     // 0x00 vtable pointer region
    CScnItemModelNw4rOwner* field_04;      // 0x04 owner view (Scn_GetFrameDelta arg)
    u8    field_0x08[0x1464];  // reserved
    nw4r::g3d::ResMdlData* field_0x146C; // 0x146C model resource data
    u8    field_0x1470[0xC];   // reserved
    nw4r::g3d::ScnMdl* field_0x147C;     // 0x147C g3d scene object (ScnMdl)
};


// Retail ctor fragment: stores the vtable pointer, then mirrors the retail
// field-init order (field_0x20 is left uninitialized, as in retail).
extern "C" void __ct__CMdlMouth(CMdlMouth* ths) {
    // Manual vptr store; typed as u32* to match lbl array.
    *reinterpret_cast<u32**>(ths) = lbl_eu_805701B0;
    ths->field_0x04 = 0;
    ths->field_0x08 = 0;
    ths->field_0x0C = -1;
    f32 z = 0.0f;
    ths->field_0x10 = z;
    ths->field_0x14 = z;
    ths->field_0x18 = z;
    ths->field_0x1C = z;
    ths->field_0x24 = -1;
    ths->field_0x28 = 0;
    ths->value2C = 1.0f;
}

// Retail dtor fragment (deleting-dtor form: flag > 0 frees the object).
extern "C" void* __dt__9CMdlMouthFv(CMdlMouth* ths, int flag) {
    if (ths != 0 && flag > 0) {
        operator delete(ths);
    }
    return ths;
}

extern "C" void MdlMouthBindModel(CMdlMouth* ths, CMdlOwnerCtx* model) {
    // Member field init (mirrors the retail ctor stores).
    ths->field_0x04 = model;
    ths->field_0x08 = 0;
    ths->field_0x0C = -1;
    f32 z = 0.0f;
    ths->field_0x1C = z;
    ths->field_0x10 = z;
    ths->field_0x14 = z;
    ths->field_0x18 = z;
    ths->field_0x24 = -1;
    ths->field_0x28 = 0;
    ths->value2C = 1.0f;

    // Bind the named node if present; the inner panics are DB_ASSERTs on the
    // same validity check, so MWCC reuses the outer compare (dead code).
    nw4r::g3d::ResNode node =
        nw4r::g3d::ResMdl(model->field_0x146C).GetResNode(lbl_eu_80663C58);
    if (node.IsValid()) {
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E1C8, 0x38, lbl_eu_8056E1A8);
        }
        ths->field_0x0C = node.GetID();
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 0x2c, lbl_eu_8056E178,
                            lbl_eu_80663910, lbl_eu_80663C6C);
        }
        ths->field_0x20 = node.ref().rot.z;
        // The model object is really a CScnItemModelNw4r; the view struct
        // above only exposes the fields this TU reads.
        scnImN4AddFrameId((CScnItemModelNw4r*)model, ths->field_0x0C);
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
            ths->field_0x24 = resMat.ref().id;
            break;
        }
    }
}

extern "C" void MdlMouthSetMode(CMdlMouth* ths, int arg2, int arg3) {
    // All follow-up blocks are guarded by the mode-change condition: retail skips
    // the whole body with a single branch when field_0x08 already equals arg2.
    if (ths->field_0x08 != arg2) {
        ths->field_0x08 = arg2;
        if (arg2 != 0) {
            ths->field_0x14 = 0.002f * (f32)ml::math::mtRand(100) + 0.2f;
            ths->field_0x18 = 0.006f * (f32)ml::math::mtRand(100) + 0.2f;
        }
        if (arg2 != 0) {
            ths->field_0x28 = 2;
        }
        if (arg3 != 0) {
            if (ths->field_0x08 == 0) {
                f32 z = 0.0f;
                ths->field_0x1C = z;
                ths->field_0x10 = z;
                ths->field_0x14 = z;
                ths->field_0x18 = z;
            }
        }
    }
}

extern "C" void MdlMouthSetValue(CMdlMouth* ths, float val) {
    ths->value2C = val;
}

extern "C" int MdlMouthPollAnim(CMdlMouth* ths, u32 arg2, nw4r::g3d::ChrAnmResult* res) {
    // Mismatched own-node id: callers may only animate the bound node.
    if ((u32)ths->field_0x0C != arg2) {
        return 0;
    }
    // Closing-timer decrement: while it counts down the rest is skipped.
    if (ths->field_0x08 != 0 && ths->field_0x28 > 0) {
        ths->field_0x28 -= 1;
        if (ths->field_0x28 > 0) {
            return 0;
        }
    }
    if (ths->field_0x08 == 0) {
        return 1;
    }
    // Integrate mouth-open angle from the owner view's delta.
    f32 dt = Scn_GetFrameDelta(ths->field_0x04->field_04);
    ths->field_0x10 = ths->field_0x14 * dt + ths->field_0x10;
    if (ths->field_0x10 > ml::pi) {
        // Reset to fully closed, then re-randomize open/close speeds.
        ths->field_0x10 = 0.0f;
        ths->field_0x14 = 0.0f;
        if (ths->field_0x08 != 0) {
            ths->field_0x14 = 0.002f * (f32)ml::math::mtRand(100) + 0.2f;
            ths->field_0x18 = 0.006f * (f32)ml::math::mtRand(100) + 0.2f;
        }
    }
    nw4r::math::VEC3 v;
    f32 o = ths->field_0x10;
    f32 s = nw4r::math::SinFIdx(40.743664f * o);
    ths->field_0x1C = ths->field_0x18 * s * ths->value2C;
    res->GetRotateDeg(&v);
    v.z = 33.0f * ths->field_0x1C + v.z;
    res->SetRotateDeg(&v);
    return 1;
}

extern "C" void MdlMouthTeardown(CMdlMouth* ths) {
    // Per-frame mouth update: bail when no material is bound (id < 0) or the
    // closing timer still counts down; otherwise integrate the open angle and
    // push the SRT offset-table row into the copied material's texture SRT.
    if (ths->field_0x24 < 0) {
        return;
    }
    if (ths->field_0x08 != 0 && ths->field_0x28 > 0) {
        ths->field_0x28 -= 1;
        if (ths->field_0x28 > 0) {
            return;
        }
    }
    f32 dt = Scn_GetFrameDelta(ths->field_0x04->field_04);
    ths->field_0x10 = ths->field_0x14 * dt + ths->field_0x10;
    if (ths->field_0x10 > ml::pi) {
        // Reset to fully closed, then re-randomize open/close speeds.
        ths->field_0x10 = 0.0f;
        ths->field_0x14 = 0.0f;
        if (ths->field_0x08 != 0) {
            ths->field_0x14 = 0.002f * (f32)ml::math::mtRand(100) + 0.2f;
            ths->field_0x18 = 0.006f * (f32)ml::math::mtRand(100) + 0.2f;
        }
    }
    f32 o = ths->field_0x10;
    f32 s = nw4r::math::SinFIdx(40.743664f * o);
    ths->field_0x1C = ths->field_0x18 * s * ths->value2C;
    // Pick the SRT offset-table row from the open amount, clamped to [0,2].
    s32 idx = (s32)(4.0f * ths->field_0x1C);
    if ((u32)idx >= 3) {
        idx = 2;
    }
    nw4r::g3d::ScnMdl* scnMdl =
        ths->field_0x04->field_0x147C;
    nw4r::g3d::ScnMdl::CopiedMatAccess cma(scnMdl, (u32)ths->field_0x24);
    nw4r::g3d::ResTexSrt srt = cma.GetResTexSrt(false);
    if (srt.IsValid()) {
        // Reflection-scale SRT matrix; column 3 carries the per-index offset.
        // Load the per-index offset into a local first: its web must be born
        // before the two shared constants so the scratch colors come out
        // f0=one, f1=neg, f2=offset (retail order), while the _03 store stays
        // in offset order.
        f32 t = lbl_eu_805247B4[idx];
        nw4r::math::MTX34 m;
        m._00 = 1.0f;
        m._01 = 0.0f;
        m._02 = 0.0f;
        m._03 = t;
        m._10 = 0.0f;
        m._11 = 1.0f;
        m._12 = 0.0f;
        m._13 = 0.0f;
        m._20 = 0.0f;
        m._21 = 0.0f;
        m._22 = 1.0f;
        m._23 = 0.0f;
        srt.SetEffectMtx(0, &m);
        srt.SetEffectMtx(1, &m);
        srt.SetEffectMtx(2, &m);
    }
}

// ===== Dissolved data owned by this TU =====
// [.rodata] 0x805247B4-0x805247D0 (28B): SRT offset table {0, 0.25f, 0.5f}
// indexed by the mouth-open stage, then the RTTI class-name string.
extern const f32 lbl_eu_805247B4[3] = { 0.0f, 0.25f, 0.5f };
extern const char lbl_eu_805247C0[0x10] = "CMdlMouth";
// [.sdata] 0x80663C58-0x80663C74 (28B): name pointers, RTTI locator
// {name, 0}, two "ref" Panic args, and a pointer into the shared pool.
extern const char lbl_eu_8066B308[];
extern const char lbl_eu_805247A8[];
extern const char lbl_eu_8066B340[];
const char* lbl_eu_80663C58 = lbl_eu_8066B308;
const char* lbl_eu_80663C5C = lbl_eu_805247A8;
extern "C" const void* __RTTI__9CMdlMouth[2] = { (const void*)lbl_eu_805247C0, 0 };
extern "C" __declspec(section ".sdata") const char lbl_eu_80663C68[4] = { 0x72,0x65,0x66,0x00 };
extern "C" __declspec(section ".sdata") const char lbl_eu_80663C6C[4] = { 0x72,0x65,0x66,0x00 };
const char* lbl_eu_80663C70 = lbl_eu_8066B340;
// [.data] 0x805701B0-0x805701C0 (16B): CMdlMouth vtable {RTTI locator
// __RTTI__9CMdlMouth, 0, dtor, 0}.
extern "C" u32 lbl_eu_805701B0[4] = {
    (u32)&__RTTI__9CMdlMouth, 0x00000000,
    (u32)&__dt__9CMdlMouthFv, 0x00000000,
};


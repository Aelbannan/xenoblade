// monolib/src/scn/CMdlAnmEye - eye-blink animation state machine for a
// material pair (two eyes). func_804E75B8 registers the eye materials by
// name prefix; func_804E77C4 advances the blink state machine and pushes a
// texture-SRT effect matrix per registered eye every frame.
#include <harness_catalog.h>
#include <string.h>
#include <monolib/math.hpp>
#include <monolib/core/code_804E36DC.hpp>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/db/db_assert.h>
#include "libs/monolib/src/scn/CMdlAnmEye.hpp"

// Minimal view of the owner model (CScnItemModelNw4r) fields used here.
// 0x146C is the model resource data, 0x147C the g3d scene object (ScnMdl).
struct CMdlAnmEyeModel {
    u8 pad_00[0x04];                 // 0x00..0x04 (vtable)
    void* field_04;                  // 0x04 owner (func_80496288 arg)
    u8 pad_08[0x7A8 - 0x08];
    u32 field_0x7A8;                 // 0x7A8 flag word (bit 2 = force scale 1.0)
    u8 pad_7AC[0x146C - 0x7AC];
    nw4r::g3d::ResMdlData* field_0x146C; // 0x146C model resource data
    u8 pad_1470[0x147C - 0x1470];
    void* field_0x147C;              // 0x147C g3d scene object (the ScnMdl)
};

CMdlAnmEye::CMdlAnmEye() {
    field_04 = 0;
    field_1C = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = lbl_eu_8066B360;
    value2C = 1;
}

CMdlAnmEye::~CMdlAnmEye() {}

// Register the left- and right-eye materials. Each scan stops at the first
// material whose name starts with the eye prefix (strstr == name).
void func_804E75B8(CMdlAnmEye* self, void* modelArg) {
    self->field_04 = modelArg;
    u32 num = nw4r::g3d::ResMdl(((CMdlAnmEyeModel*)modelArg)->field_0x146C)
                  .GetResMatNumEntries();
    u32 flag;
    for (u32 i = 0; i < num; i++) {
        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(((CMdlAnmEyeModel*)modelArg)->field_0x146C)
                .GetResMat(i);
        if (!mat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C98);
        }
        const char* nm = mat.ofs_to_ptr<char>(mat.ref().name);
        if (nm == strstr(nm, lbl_eu_80663C88)) {
            if (!mat.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                                lbl_eu_806638E8, lbl_eu_80663C98);
            }
            self->field_08[self->field_1C] = mat.ref().id;
            s32 n = self->field_1C + 1;
            self->field_18[self->field_1C] = 0;
            self->field_1C = n;
            break;
        }
    }
    flag = 1;
    for (u32 i = 0; i < num; i++) {
        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(((CMdlAnmEyeModel*)modelArg)->field_0x146C)
                .GetResMat(i);
        if (!mat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C98);
        }
        const char* nm = mat.ofs_to_ptr<char>(mat.ref().name);
        if (nm == strstr(nm, func_804E6C78())) {
            if (!mat.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                                lbl_eu_806638E8, lbl_eu_80663C98);
            }
            self->field_08[self->field_1C] = mat.ref().id;
            s32 n = self->field_1C + 1;
            self->field_18[self->field_1C] = flag;
            self->field_1C = n;
            if (n == 2) break;
        }
    }
}

void func_804E77BC(u8* self, u32 val) {
    ((CMdlAnmEye*)self)->value2C = val;
}

// Per-frame blink update: integrate the owner model's scale*delta into the
// anim offset, run the blink state machine, then push the resulting eye
// offset (per-eye angle table indexed by field_20) into each registered
// eye's texture SRT effect matrix.
void func_804E77C4(CMdlAnmEye* self) {
    if (self->value2C == 4) return;
    if (self->field_1C == 0) return;
    f32 scale = func_80484EB0(self->field_04);
    if (((CMdlAnmEyeModel*)self->field_04)->field_0x7A8 & 4) {
        scale = lbl_eu_8066B364;
    }
    f32 dt = func_80496288(((CMdlAnmEyeModel*)self->field_04)->field_04);
    f32 v = self->field_28 + scale * dt;
    self->field_28 = v;
    switch (self->field_20) {
    case 0:
        if (self->value2C == 1) return;
        if (v >= lbl_eu_8066B368 || self->value2C == 2 || self->value2C == 3) {
            self->field_28 = lbl_eu_8066B360;
            if (ml::math::mtRand(10) != 4 && self->value2C == 0) {
                s32 r = ml::math::mtRand(7) + 12;
                self->field_24 = self->field_24 + 1;
                if (self->field_24 < r) return;
            }
            self->field_24 = 0;
            self->field_20 = self->field_20 + 1;
        }
        break;
    case 1:
        if (self->value2C != 3 && v >= lbl_eu_8066B36C) {
            self->field_28 = lbl_eu_8066B360;
            self->field_20 = self->field_20 + 1;
        }
        break;
    case 2:
        if (v > lbl_eu_8066B36C && self->value2C != 2) {
            self->field_28 = lbl_eu_8066B360;
            self->field_20 = 0;
        }
        break;
    }
    for (s32 i = 0; i < self->field_1C; i++) {
        nw4r::g3d::ScnMdl::CopiedMatAccess cma(
            (nw4r::g3d::ScnMdl*)((CMdlAnmEyeModel*)self->field_04)->field_0x147C,
            self->field_08[i]);
        nw4r::g3d::ResTexSrt texSrt = cma.GetResTexSrt(false);
        if (!texSrt.IsValid()) continue;
        nw4r::math::MTX34 mtx;
        if (self->field_18[i] == 0) {
            f32 ang = lbl_eu_805247E8[self->field_20];
            mtx._00 = lbl_eu_8066B364; mtx._01 = lbl_eu_8066B360; mtx._02 = lbl_eu_8066B360;
            mtx._03 = lbl_eu_8066B360;
            mtx._10 = lbl_eu_8066B360; mtx._11 = lbl_eu_8066B364; mtx._12 = lbl_eu_8066B360;
            mtx._13 = ang;
            mtx._20 = lbl_eu_8066B360; mtx._21 = lbl_eu_8066B360; mtx._22 = lbl_eu_8066B364;
            mtx._23 = lbl_eu_8066B360;
        } else {
            f32 ang = lbl_eu_805247F4[self->field_20];
            mtx._00 = lbl_eu_8066B364; mtx._01 = lbl_eu_8066B360; mtx._02 = lbl_eu_8066B360;
            mtx._03 = -ang;
            mtx._10 = lbl_eu_8066B360; mtx._11 = lbl_eu_8066B364; mtx._12 = lbl_eu_8066B360;
            mtx._13 = lbl_eu_8066B360;
            mtx._20 = lbl_eu_8066B360; mtx._21 = lbl_eu_8066B360; mtx._22 = lbl_eu_8066B364;
            mtx._23 = lbl_eu_8066B360;
        }
        texSrt.SetEffectMtx(0, &mtx);
        texSrt.SetEffectMtx(1, &mtx);
        texSrt.SetEffectMtx(2, &mtx);
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.rodata] 0x805247E8-0x80524810 (40B)
extern const f32 lbl_eu_805247E8[3] = { 0.0f, 0.3375f, 0.675f };
extern const f32 lbl_eu_805247F4[3] = { 0.0f, -0.5f, -1.0f };
extern const char lbl_eu_80524800[0xB] = "CMdlAnmEye";
// [.sdata] 0x80663C88-0x80663CA0 (24B)
extern const char lbl_eu_8066B358[];
const char* lbl_eu_80663C88 = lbl_eu_8066B358;
extern "C" __declspec(section ".sdata") const char lbl_eu_80663C98[4] = { 0x72,0x65,0x66,0x00 };
DECOMP_FORCEACTIVE(CMdlAnmEye_cpp, lbl_eu_805247E8);
DECOMP_FORCEACTIVE(CMdlAnmEye_cpp, lbl_eu_805247F4);
DECOMP_FORCEACTIVE(CMdlAnmEye_cpp, lbl_eu_80524800);
DECOMP_FORCEACTIVE(CMdlAnmEye_cpp, lbl_eu_80663C88);
DECOMP_FORCEACTIVE(CMdlAnmEye_cpp, lbl_eu_80663C98);

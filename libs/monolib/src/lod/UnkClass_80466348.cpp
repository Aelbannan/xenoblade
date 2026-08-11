// Auto-scaffolded catalog TU for monolib/src/lod/UnkClass_80466348
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/GX.h>
#include "monolib/lod/LODMemMan.hpp"
#include "monolib/lod/code_804645CC.hpp"

namespace LOD {
struct UnkClass_80466348 {
    u8 field_0x0;   // ambient color R
    u8 field_0x1;   // ambient color G
    u8 field_0x2;   // ambient color B
    u8 field_0x3;
    u32 field_0x4;
    void func_80466348();
    void func_80466390();
    void func_804663D0();
    void func_80466418();
    void func_80466458();
    void func_804664A0();
    void func_804664E8();
    void func_80466520();
    void func_80466558();
    void func_80466590();
    void func_80466638();
    void func_804666E0();
    void func_80466794();
    void func_80466848();
    void func_804668F0();
    void func_80466998();
    void func_80466A4C();
    void func_80466AE8();
    void func_80466B84();
    void func_80466C38();
    void func_80466CEC();
    void func_80466D94();
    void func_80466E3C();
    void func_80466EE4();
    void func_804671E4();
    void func_8046734C();
    void func_804673D0();
    void func_80467454();
    void func_804674E4();
    void func_80467568();
    void func_804675F8();
    void func_80467610();
    void func_80467628();
    void func_80467640();
    void func_80467654();
    void func_80467668();
    void func_8046767C();
    void func_80467690();
    void func_804676A4();
    void func_804676BC();
    void func_804676D4();
    void func_804676EC();
    void func_8046774C();
    void func_80467758();
    void func_80467798();
    void func_80467898();
    void func_804678E8();
    void func_804679E8();
    void func_80467B04();
    void func_80467BD4();
    void func_80467CF0() const;
    void func_80467E14();
    void func_80467E94();
    void func_80467FB8();
    void func_804680DC();
    void func_804681B4();
    void func_804682D8();
    void func_80468354();
    void func_804683D0();
};
}

using namespace LOD;

// ---- shared LOD renderer data (.bss, non-small-data access) ----
extern u32 lbl_eu_80658368[3];
extern u32 lbl_eu_80658374[3];
extern f32 lbl_eu_80658380[12]; // LOD fog/color ramp, written by sinit_804683E8
extern f32 lbl_eu_806583B0[3][4]; // fixed LOD texture matrix

// ---- shared sdata2 float constants (small-data pool) ----
extern const float lbl_eu_8066A620;
extern const float lbl_eu_8066A624;
extern const float lbl_eu_8066A640;
extern const float lbl_eu_8066A644;

// LOD terrain state manager (sibling TU); only the fields referenced here.
struct UnkClass_8046368C {
    u8 pad_00[8];
    u16 field_0x8;   // 0x08 lookup-table index
    u8 pad_0A[2];
    f32 field_0xC;   // 0x0C
    u8 pad_10[0x1C - 0x10];
    f32 field_0x1C;  // 0x1C
};

// LOD record-pair dispatcher (sibling TU). extern "C" keeps the retail mangled
// name; the extra ABI args are explicit per the MWCC_REFERENCE "Fv ABI note".
extern "C" void func_804636AC__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, u16 index);

void LOD::UnkClass_80466348::func_80466348() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 1) {
        lbl_eu_806657DC = 1;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)lbl_eu_806657F0, (GXDiffuseFn)lbl_eu_80658374[1],
                      (GXAttnFn)lbl_eu_80658368[1]);
    }
}

void LOD::UnkClass_80466348::func_80466390() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 2) {
        lbl_eu_806657DC = 2;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)lbl_eu_806657EC, (GXDiffuseFn)lbl_eu_80658374[0],
                      (GXAttnFn)lbl_eu_80658368[0]);
    }
}

void LOD::UnkClass_80466348::func_804663D0() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 3) {
        lbl_eu_806657DC = 3;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)lbl_eu_806657F0, (GXDiffuseFn)lbl_eu_80658374[1],
                      (GXAttnFn)lbl_eu_80658368[1]);
    }
}

void LOD::UnkClass_80466348::func_80466418() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 4) {
        lbl_eu_806657DC = 4;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)lbl_eu_806657EC, (GXDiffuseFn)lbl_eu_80658374[0],
                      (GXAttnFn)lbl_eu_80658368[0]);
    }
}

void LOD::UnkClass_80466348::func_80466458() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 5) {
        lbl_eu_806657DC = 5;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)lbl_eu_806657F4, (GXDiffuseFn)lbl_eu_80658374[2],
                      (GXAttnFn)lbl_eu_80658368[2]);
    }
}

void LOD::UnkClass_80466348::func_804664A0() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 6) {
        lbl_eu_806657DC = 6;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)lbl_eu_806657F4, (GXDiffuseFn)lbl_eu_80658374[2],
                      (GXAttnFn)lbl_eu_80658368[2]);
    }
}

void LOD::UnkClass_80466348::func_804664E8() {
    // Latch the "diffuse+specular channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 7) {
        lbl_eu_806657DC = 7;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

void LOD::UnkClass_80466348::func_80466520() {
    // Latch the "alpha channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657E0 != 1) {
        lbl_eu_806657E0 = 1;
        GXSetChanCtrl(GX_ALPHA0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

void LOD::UnkClass_80466348::func_80466558() {
    // Latch the "alpha channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657E0 != 2) {
        lbl_eu_806657E0 = 2;
        GXSetChanCtrl(GX_ALPHA0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

void LOD::UnkClass_80466348::func_80466590() {}

void LOD::UnkClass_80466348::func_80466638() {}

void LOD::UnkClass_80466348::func_804666E0() {}

void LOD::UnkClass_80466348::func_80466794() {}

void LOD::UnkClass_80466348::func_80466848() {}

void LOD::UnkClass_80466348::func_804668F0() {}

void LOD::UnkClass_80466348::func_80466998() {}

void LOD::UnkClass_80466348::func_80466A4C() {}

void LOD::UnkClass_80466348::func_80466AE8() {}

void LOD::UnkClass_80466348::func_80466B84() {}

void LOD::UnkClass_80466348::func_80466C38() {}

void LOD::UnkClass_80466348::func_80466CEC() {}

void LOD::UnkClass_80466348::func_80466D94() {}

void LOD::UnkClass_80466348::func_80466E3C() {}

void LOD::UnkClass_80466348::func_80466EE4() {}

// Retail names are the shortened Fv form, but these entry points receive an
// extra vertex-attribute count argument in r4 (see MWCC_REFERENCE "Fv ABI
// note"); the vtx-desc "type" value is the self pointer itself.
extern "C" void func_80466F8C__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self, s32 count) {
    // Clear all vertex descriptors, then enable position + a run of texcoords,
    // all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, (GXAttrType)self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), (GXAttrType)self);
    }
}

extern "C" void func_80466FF8__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self, s32 count) {
    // Clear all vertex descriptors, then enable position, color0 and a run of
    // texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, (GXAttrType)self);
    GXSetVtxDesc(GX_VA_CLR0, (GXAttrType)self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), (GXAttrType)self);
    }
}

extern "C" void func_80467070__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self, s32 count) {
    // Clear all vertex descriptors, then enable position, normal and a run of
    // texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, (GXAttrType)self);
    GXSetVtxDesc(GX_VA_NRM, (GXAttrType)self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), (GXAttrType)self);
    }
}

// Retail name is the shortened Fv form; the entry point receives an extra
// vertex-attribute count argument in r4 (see MWCC_REFERENCE "Fv ABI note").
extern "C" void func_804670E8__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self, s32 count) {
    // Clear all vertex descriptors, then enable position, normal, color0 and a
    // run of texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, (GXAttrType)self);
    GXSetVtxDesc(GX_VA_NRM, (GXAttrType)self);
    GXSetVtxDesc(GX_VA_CLR0, (GXAttrType)self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), (GXAttrType)self);
    }
}

extern "C" void func_8046716C__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self, s32 count) {
    // Clear all vertex descriptors, then enable position, normal and a run of
    // texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, (GXAttrType)self);
    GXSetVtxDesc(GX_VA_NRM, (GXAttrType)self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), (GXAttrType)self);
    }
}

void LOD::UnkClass_80466348::func_804671E4() {}

extern "C" void func_80467268__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self, s32 count) {
    // Clear all vertex descriptors, then enable position + color0 with fixed
    // stream types and a run of indexed texcoords.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), GX_INDEX8);
    }
}

// Retail name is the shortened Fv form; the entry point receives an extra
// vertex-attribute count argument in r4 (see MWCC_REFERENCE "Fv ABI note").
extern "C" void func_804672D4__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self, s32 count) {
    // Clear all vertex descriptors, then enable the position/normal matrix
    // index (direct) plus position and a run of texcoords fed directly from
    // this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, (GXAttrType)self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), (GXAttrType)self);
    }
}

void LOD::UnkClass_80466348::func_8046734C() {}

void LOD::UnkClass_80466348::func_804673D0() {}

void LOD::UnkClass_80466348::func_80467454() {}

void LOD::UnkClass_80466348::func_804674E4() {}

void LOD::UnkClass_80466348::func_80467568() {}

void LOD::UnkClass_80466348::func_804675F8() {
    GXSetAlphaCompare(GX_ALWAYS, 0xff, GX_AOP_AND, GX_ALWAYS, 0xff);
}

void LOD::UnkClass_80466348::func_80467610() {
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_LEQUAL, 0xff);
}

void LOD::UnkClass_80466348::func_80467628() {
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_LEQUAL, 0xff);
}

void LOD::UnkClass_80466348::func_80467640() {
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
}

void LOD::UnkClass_80466348::func_80467654() {
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
}

void LOD::UnkClass_80466348::func_80467668() {
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
}

void LOD::UnkClass_80466348::func_8046767C() {
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_ZERO, GX_LO_NOOP);
}

void LOD::UnkClass_80466348::func_80467690() {
    GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
}

void LOD::UnkClass_80466348::func_804676A4() {
    // Pass the tex-gen source as a pointer-derived value (this + 4).
    GXSetTexCoordGen2((GXTexCoordID)this, GX_TG_MTX2x4, (GXTexGenSrc)&this->field_0x4, 0x3C, GX_DISABLE, 0x7D);
}

void LOD::UnkClass_80466348::func_804676BC() {
    GXSetTexCoordGen2((GXTexCoordID)this, GX_TG_MTX3x4, GX_TG_NRM, 0x1E, GX_ENABLE, 0x40);
}

void LOD::UnkClass_80466348::func_804676D4() {
    // Fixed tex-gen setup; the texcoord id is carried in via `this` (as in
    // func_804676A4/BC which pass `this` straight to GXSetTexCoordGen2).
    GXSetTexCoordGen2((GXTexCoordID)this, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_DISABLE, 0x7D);
}

// Retail name is the shortened Fv form, but the entry point receives an extra
// pointer in r4 (see MWCC_REFERENCE "Fv ABI note").
extern "C" void func_804676EC__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self,
                                                           const UnkClass_8046368C* arg) {
    // Build the LOD matrix in a scratch buffer, upload it, and bind tex-gen.
    s32 index = arg->field_0x8;
    Mtx mtx;
    func_804636AC__Q23LOD17UnkClass_8046368CFv((UnkClass_8046368C*)&mtx, index);
    GXLoadTexMtxImm(mtx, 0x21, GX_MTX_2x4);
    GXSetTexCoordGen2((GXTexCoordID)self, GX_TG_MTX2x4, (GXTexGenSrc)&self->field_0x4, 0x21,
                      GX_DISABLE, 0x7D);
}

void LOD::UnkClass_80466348::func_8046774C() { GXLoadTexMtxImm((const float(*)[4])this, 30, GX_MTX_3x4); }

void LOD::UnkClass_80466348::func_80467758() {
    // Concatenate the fixed LOD matrix with this object's matrix, then upload it.
    Mtx mtx;
    PSMTXConcat(lbl_eu_806583B0, (const f32(*)[4])this, mtx);
    GXLoadTexMtxImm(mtx, 30, GX_MTX_3x4);
}

void LOD::UnkClass_80466348::func_80467798() {}

void LOD::UnkClass_80466348::func_80467898() {
    // Build the color-and-alpha ambient color from this object's RGB bytes and
    // the shared alpha latch, then emit it.
    GXColor color = { field_0x0, field_0x1, field_0x2, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, color);
}

void LOD::UnkClass_80466348::func_804678E8() {}

void LOD::UnkClass_80466348::func_804679E8() {}

void LOD::UnkClass_80466348::func_80467B04() {}

void LOD::UnkClass_80466348::func_80467BD4() {}

void LOD::UnkClass_80466348::func_80467CF0() const {}

void LOD::UnkClass_80466348::func_80467E14() {
    // Build the ambient color from this object's RGB bytes and a scaled alpha:
    // the u32 alpha latch is converted through the s32->f32 path (MWCC's
    // 0x4330/xoris double trick) and scaled by the shared alpha scale.
    GXColor color = {
        field_0x0,
        field_0x1,
        field_0x2,
        (u8)(s32)((f32)((f64)(u32)(lbl_eu_80665814 ^ 0x80000000) - lbl_eu_8066A628) * lbl_eu_80665808),
    };
    GXSetChanAmbColor(GX_COLOR0A0, color);
}

void LOD::UnkClass_80466348::func_80467E94() {}

void LOD::UnkClass_80466348::func_80467FB8() {}

void LOD::UnkClass_80466348::func_804680DC() {}

void LOD::UnkClass_80466348::func_804681B4() {}

void LOD::UnkClass_80466348::func_804682D8() {
    // Identity swap tables: every channel maps to red except alpha.
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
}

void LOD::UnkClass_80466348::func_80468354() {
    // Identity swap tables: red->red, green->green, blue->blue, alpha->alpha.
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void LOD::UnkClass_80466348::func_804683D0() {
    // Cache the current-matrix value; only re-issue GXSetCurrentMtx when it changes.
    if ((s32)lbl_eu_80665818 != (s32)(u32)this) {
        lbl_eu_80665818 = (u32)this;
        GXSetCurrentMtx((u32)this);
    }
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// .ctors static initializer: fill the LOD fog/color ramp (lbl_eu_80658380)
// from the shared float constants.
extern "C" void sinit_804683E8() {
    float* t = lbl_eu_80658380;
    t[0] = lbl_eu_8066A640;
    t[1] = lbl_eu_8066A620;
    t[2] = lbl_eu_8066A620;
    t[3] = lbl_eu_8066A640;
    t[4] = lbl_eu_8066A620;
    t[5] = lbl_eu_8066A644;
    t[6] = lbl_eu_8066A620;
    t[7] = lbl_eu_8066A640;
    t[8] = lbl_eu_8066A620;
    t[9] = lbl_eu_8066A620;
    t[10] = lbl_eu_8066A620;
    t[11] = lbl_eu_8066A624;
}

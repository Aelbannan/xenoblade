// Decompilation of monolib/src/lod/UnkClass_80466348.
//
// LOD material/vertex-format state setters: GX channel/alpha setup, TEV
// register combiners, vertex-descriptor setup, alpha/blend modes, tex-gen
// and matrix upload, ambient-color emission, and the TU's .ctors static
// initializer (sinit_804683E8).
//
// The retail names are (shortened) "...Fv" annotations, but these entry
// points receive their real ABI arguments in the parameter registers (see
// docs/MWCC_CASES.md "Fv ABI note").  They are written as plain global
// free functions taking those arguments explicitly (same pattern as
// code_804645CC / CLODCacheManagerS); only loadTexMtxAndSetGen keeps its C-linkage
// retail name because it calls the sibling-record dispatcher by retail symbol.

#include <harness_catalog.h>
#include <revolution/GX.h>
#include "monolib/lod/LODMemMan.hpp"
#include "monolib/lod/code_804645CC.hpp"

namespace LOD {
struct UnkClass_80466348 {
    u8 field_0x0;   // ambient color R
    u8 field_0x1;   // ambient color G
    u8 field_0x2;   // ambient color B
    u8 field_0x3;   // unknown; unused by this TU
    u32 field_0x4;  // unknown; its address is used as the tex-gen source
    void setColorChanRegReg1();
    void setColorChanRegReg2();
    void setColorChanRegVtx1();
    void setColorChanRegVtx2();
    void setColorChanRegReg3();
    void setColorChanRegVtx3();
    void setColorChanDisable();
    void setAlphaChanVtx();
    void setAlphaChanReg();
    void tevStageTexRasc();
    void tevStageTexC0AlphaA0();
    void tevStageTexKonstC0();
    void tevStageTexKonstSubC0();
    void tevStageTexMulC0();
    void tevStageTexAlpha();
    void tevStageTexKonstOnly();
    void tevStageTexRascReg0();
    void tevStageTexRascReg1();
    void tevStageC0C1Tex();
    void tevStageC0C1TexAlpha();
    void tevStageC0TexAlphaBlend();
    void tevStageKonstTexReg1();
    void tevStageRascTexC0();
    void tevStageRascTexSubC0();
    void setupVtxDescPosNrmClrTex2();
    void setupVtxDescPnPosClrTex();
    void setupVtxDescPnPosNrmTex();
    void setupVtxDescPnPosNrmClrTex();
    void setupVtxDescPnPosNrmTex3();
    void setupVtxDescPnPosNrmClrTex3();
    void setAlphaCompareAlways();
    void setAlphaCompareGe80();
    void setAlphaCompareGe1();
    void setBlendModeNone();
    void setBlendModeBlend();
    void setBlendModeAdd();
    void setBlendModeReplace();
    void setBlendModeSubtract();
    void setTexGenMtx2x4Field();
    void setTexGenMtx3x4Nrm();
    void setTexGenMtx3x4Pos();
    void loadTexMtxAndSetGen();
    void loadTexMtxImmPos();
    void loadTexMtxConcat();
    void setAmbColorScaled1();
    void setAmbColorDirect();
    void setAmbColorScaled2();
    void setAmbColorScaledAlpha1();
    void setAmbColorAlphaSimple();
    void setAmbColorScaledAlpha2();
    void setAmbColorWithAlpha1();
    void func_80467E14();
    void setAmbColorWithAlpha2();
    void setAmbColorComposite1();
    void setAmbColorComposite2();
    void setAmbColorComposite3();
    void setTevSwapRedAlpha();
    void setTevSwapIdentity();
    void setCurrentMtxCached();
};
}

using namespace LOD;

// ---- shared LOD renderer data (.bss, non-small-data access) ----
extern u32 lbl_eu_80658368[3];
extern u32 lbl_eu_80658374[3];
extern f32 lbl_eu_80658348[3]; // per-channel ambient scale LUT
// per-channel ambient scale LUT (written by sinit)
extern f32 lbl_eu_80658358[3];
extern f32 lbl_eu_80658380[12]; // LOD fog/color ramp, written by sinit_804683E8
extern f32 lbl_eu_806583B0[3][4]; // fixed LOD texture matrix

// ---- shared sdata2 float constants (small-data pool) ----
extern const float lbl_eu_8066A620;
extern const float lbl_eu_8066A624;
extern const float lbl_eu_8066A640;
extern const float lbl_eu_8066A644;
extern const double lbl_eu_8066A628;  // s32->f32 magic (0x4330000080000000)
extern const double lbl_eu_8066A638;  // u8->f32 magic (0x4330000000000000)

// Builtin (f32) casts pool TU-local magic doubles; the retail object
// references the blob pool entries (lbl_eu_8066A628/A638) instead. Union
// helpers keep this TU's .sdata2 empty (retail shape).
inline f32 s32ToF_a628(s32 v) { return (f32)v; }
inline f32 u8ToF_a638(u8 v) { return (f32)v; }

// LOD terrain state manager (sibling TU); only the fields referenced here.
struct UnkClass_8046368C {
    u8 pad_00[8];
    u16 field_0x8;   // 0x08 lookup-table index
    u8 pad_0A[2];
    f32 field_0xC;   // 0x0C
    u8 pad_10[0x1C - 0x10];
    f32 field_0x1C;  // 0x1C
};

// LOD record-pair dispatcher (defined in CLODCacheManagerS.cpp). Declared
// with C linkage to keep the retail mangled name; the extra ABI args are
// explicit per the MWCC_CASES "Fv ABI note".
extern "C" void dispatchLodPair__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, u16 index);

void LOD::UnkClass_80466348::setColorChanRegReg1() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 1) {
        lbl_eu_806657DC = 1;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)lbl_eu_806657F0, (GXDiffuseFn)lbl_eu_80658374[1],
                      (GXAttnFn)lbl_eu_80658368[1]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegReg2() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 2) {
        lbl_eu_806657DC = 2;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)lbl_eu_806657EC, (GXDiffuseFn)lbl_eu_80658374[0],
                      (GXAttnFn)lbl_eu_80658368[0]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegVtx1() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 3) {
        lbl_eu_806657DC = 3;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)lbl_eu_806657F0, (GXDiffuseFn)lbl_eu_80658374[1],
                      (GXAttnFn)lbl_eu_80658368[1]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegVtx2() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 4) {
        lbl_eu_806657DC = 4;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)lbl_eu_806657EC, (GXDiffuseFn)lbl_eu_80658374[0],
                      (GXAttnFn)lbl_eu_80658368[0]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegReg3() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 5) {
        lbl_eu_806657DC = 5;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)lbl_eu_806657F4, (GXDiffuseFn)lbl_eu_80658374[2],
                      (GXAttnFn)lbl_eu_80658368[2]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegVtx3() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 6) {
        lbl_eu_806657DC = 6;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)lbl_eu_806657F4, (GXDiffuseFn)lbl_eu_80658374[2],
                      (GXAttnFn)lbl_eu_80658368[2]);
    }
}

void LOD::UnkClass_80466348::setColorChanDisable() {
    // Latch the "diffuse+specular channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657DC != 7) {
        lbl_eu_806657DC = 7;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

void LOD::UnkClass_80466348::setAlphaChanVtx() {
    // Latch the "alpha channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657E0 != 1) {
        lbl_eu_806657E0 = 1;
        GXSetChanCtrl(GX_ALPHA0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

void LOD::UnkClass_80466348::setAlphaChanReg() {
    // Latch the "alpha channel setup" state, then emit it once.
    if ((s32)lbl_eu_806657E0 != 2) {
        lbl_eu_806657E0 = 2;
        GXSetChanCtrl(GX_ALPHA0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

// TEV combiner: texel * raster colour and texel * raster alpha, written to
// the caller-supplied output register with the caller-supplied scale.
u32 tevStageTexRasc(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: (const-colour * texel) + texel-alpha colour, alpha = A0,
// written to the caller-supplied output register/scale.
u32 tevStageTexC0AlphaA0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, (texel * KONST) + C0 colour,
// alpha = A0; output register/scale from the caller.
u32 tevStageTexKonstC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, (texel * KONST) - C0 colour
// (SUB), (texel * KONST) + A0 alpha; output register/scale from the caller.
u32 tevStageTexKonstSubC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_SUB, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: texel * const-colour colour, texel-alpha + A0 alpha;
// output register/scale from the caller.
u32 tevStageTexMulC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: texel-alpha * texel colour, alpha = TEXA; output
// register/scale from the caller.
u32 tevStageTexAlpha(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXA, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, texel * KONST colour,
// KONST + texel-alpha alpha; output register/scale from the caller.
u32 tevStageTexKonstOnly(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: texel * raster colour/alpha written to TEVREG0; the scale
// comes from the caller (r4 is a dead slot in the retail ABI).
u32 tevStageTexRascReg0(GXTevStageID self, u32, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    return 1;
}

// TEV combiner: texel * raster colour to TEVREG1, alpha = A0 to TEVREG0;
// scale from the caller (r4 is a dead slot in the retail ABI).
u32 tevStageTexRascReg1(GXTevStageID self, u32, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, GX_TEVREG1);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    return 1;
}

// TEV combiner: select the constant colour, C0 * C1 + texel colour,
// raster-alpha + A0 alpha; output register/scale from the caller.
u32 tevStageC0C1Tex(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, C0 * C1 + texel-alpha colour,
// raster-alpha + A0 alpha; output register/scale from the caller.
u32 tevStageC0C1TexAlpha(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_C1, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: C0 * texel + texel-alpha + C0 colour, alpha = A0; output
// register/scale from the caller.
u32 tevStageC0TexAlphaBlend(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_TEXC, GX_CC_TEXA, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, KONST * texel colour to TEVREG1,
// alpha = A0 to TEVREG0; scale from the caller (r4 is a dead ABI slot).
u32 tevStageKonstTexReg1(GXTevStageID self, u32, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, GX_TEVREG1);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    return 1;
}

// TEV combiner: texel * raster + C0 colour, alpha = A0; output
// register/scale from the caller.
u32 tevStageRascTexC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: (texel * raster) - C0 colour (SUB), texel * raster + A0
// alpha; output register/scale from the caller.
u32 tevStageRascTexSubC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_SUB, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// Vertex-descriptor setup: clear, enable POS, then `count` texcoords fed
// directly from this object's attribute data (the vtx-desc type is the self
// pointer; count arrives in r4 per the retail ...Fv ABI).
void setupVtxDescPosTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position + a run of texcoords,
    // all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

void setupVtxDescPosClrTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position, color0 and a run of
    // texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_CLR0, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

void setupVtxDescPosNrmTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position, normal and a run of
    // texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

// Vertex-descriptor setup: clear, enable POS + NRM + CLR0, then `count`
// texcoords fed directly from this object's attribute data.
void setupVtxDescPosNrmClrTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position, normal, color0 and a
    // run of texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    GXSetVtxDesc(GX_VA_CLR0, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

void setupVtxDescPosNrmTex2(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position, normal and a run of
    // texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

// Vertex-descriptor setup: clear, enable POS + NRM + CLR0, then `count`
// texcoords fed directly from this object's attribute data.
void setupVtxDescPosNrmClrTex2(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position, normal, color0 and a
    // run of texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    GXSetVtxDesc(GX_VA_CLR0, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

void setupVtxDescPosClrIndexed(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position + color0 with fixed
    // stream types and a run of indexed texcoords.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), GX_INDEX8);
    }
}

// Vertex-descriptor setup: clear, enable PNMTXIDX + POS, then `count`
// texcoords fed directly from this object's attribute data.
void setupVtxDescPnPosTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable the position/normal matrix
    // index (direct) plus position and a run of texcoords fed directly from
    // this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

// Vertex-descriptor setup: clear, enable PNMTXIDX + POS + CLR0, then
// `count` texcoords fed directly from this object's attribute data.
// Vertex-descriptor setup: clear, enable PNMTXIDX + POS + CLR0, then
// `count` texcoords fed directly from this object's attribute data.
void setupVtxDescPnPosClrTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable the position/normal matrix
    // index (direct), position + color0 fed directly from this object's
    // attribute data, and a run of texcoords.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_CLR0, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

// Vertex-descriptor setup: clear, enable PNMTXIDX + POS + NRM, then
// `count` texcoords fed directly from this object's attribute data.
void setupVtxDescPnPosNrmTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable the position/normal matrix
    // index (direct), position + normal fed directly from this object's
    // attribute data, and a run of texcoords.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

// Vertex-descriptor setup: clear, enable PNMTXIDX + POS + NRM + CLR0, then
// `count` texcoords fed directly from this object's attribute data.
void setupVtxDescPnPosNrmClrTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable the position/normal matrix
    // index (direct), position, normal, color0 and a run of texcoords,
    // all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    GXSetVtxDesc(GX_VA_CLR0, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

// Vertex-descriptor setup: clear, enable PNMTXIDX + POS + NRM, then
// `count` texcoords fed directly from this object's attribute data.
void setupVtxDescPnPosNrmTex3(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable the position/normal matrix
    // index (direct), position + normal fed directly from this object's
    // attribute data, and a run of texcoords.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

// Vertex-descriptor setup: clear, enable PNMTXIDX + POS + NRM + CLR0, then
// `count` texcoords fed directly from this object's attribute data.
void setupVtxDescPnPosNrmClrTex3(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable the position/normal matrix
    // index (direct), position, normal, color0 and a run of texcoords, all
    // fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_NRM, self);
    GXSetVtxDesc(GX_VA_CLR0, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

void LOD::UnkClass_80466348::setAlphaCompareAlways() {
    // Alpha test: always pass.
    GXSetAlphaCompare(GX_ALWAYS, 0xff, GX_AOP_AND, GX_ALWAYS, 0xff);
}

void LOD::UnkClass_80466348::setAlphaCompareGe80() {
    // Alpha test: src alpha >= 0x80 AND <= 0xff.
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_LEQUAL, 0xff);
}

void LOD::UnkClass_80466348::setAlphaCompareGe1() {
    // Alpha test: src alpha >= 1 AND <= 0xff.
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_LEQUAL, 0xff);
}

void LOD::UnkClass_80466348::setBlendModeNone() {
    // Blend: disabled (opaque).
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
}

void LOD::UnkClass_80466348::setBlendModeBlend() {
    // Blend: standard alpha blend (src alpha over dst).
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
}

void LOD::UnkClass_80466348::setBlendModeAdd() {
    // Blend: additive (src * srcAlpha + dst).
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
}

void LOD::UnkClass_80466348::setBlendModeReplace() {
    // Blend: src colour replaces dst (src * srcColor).
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_ZERO, GX_LO_NOOP);
}

void LOD::UnkClass_80466348::setBlendModeSubtract() {
    // Blend: subtract (dst - src * srcAlpha).
    GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
}

void LOD::UnkClass_80466348::setTexGenMtx2x4Field() {
    // Tex-gen: mtx2x4; the texcoord id is `this` and the source is the
    // address of field_0x4.
    GXSetTexCoordGen2((GXTexCoordID)this, GX_TG_MTX2x4, (GXTexGenSrc)&this->field_0x4, 0x3C, GX_DISABLE, 0x7D);
}

void LOD::UnkClass_80466348::setTexGenMtx3x4Nrm() {
    // Tex-gen: normal source, mtx3x4, normalized (texcoord id in `this`).
    GXSetTexCoordGen2((GXTexCoordID)this, GX_TG_MTX3x4, GX_TG_NRM, 0x1E, GX_ENABLE, 0x40);
}

void LOD::UnkClass_80466348::setTexGenMtx3x4Pos() {
    // Fixed tex-gen setup; the texcoord id is carried in via `this` (as in
    // setTexGenMtx2x4Field/BC which pass `this` straight to GXSetTexCoordGen2).
    GXSetTexCoordGen2((GXTexCoordID)this, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_DISABLE, 0x7D);
}

// Matrix-upload helper: build the LOD pair matrix via the record dispatcher,
// upload it as tex-mtx 0x21, and bind tex-gen with the caller's texcoord id.
extern "C" void loadTexMtxAndSetGen__Q23LOD17UnkClass_80466348Fv(LOD::UnkClass_80466348* self,
                                                           const UnkClass_8046368C* arg) {
    // Build the LOD matrix in a scratch buffer, upload it, and bind tex-gen.
    s32 index = arg->field_0x8;
    Mtx mtx;
    dispatchLodPair__Q23LOD17UnkClass_8046368CFv((UnkClass_8046368C*)&mtx, index);
    GXLoadTexMtxImm(mtx, 0x21, GX_MTX_2x4);
    GXSetTexCoordGen2((GXTexCoordID)self, GX_TG_MTX2x4, (GXTexGenSrc)&self->field_0x4, 0x21,
                      GX_DISABLE, 0x7D);
}

void LOD::UnkClass_80466348::loadTexMtxImmPos() { GXLoadTexMtxImm((const float(*)[4])this, 30, GX_MTX_3x4); }

void LOD::UnkClass_80466348::loadTexMtxConcat() {
    // Concatenate the fixed LOD matrix with this object's matrix, then upload it.
    Mtx mtx;
    PSMTXConcat(lbl_eu_806583B0, (const f32(*)[4])this, mtx);
    GXLoadTexMtxImm(mtx, 30, GX_MTX_3x4);
}

// The object's leading bytes are the ambient colour (a GXColor view of the
// same memory the tex-gen/matrix uploaders use as a matrix); the retail
// ...Fv entry point receives it in r3 per the MWCC_CASES "Fv ABI note".
void setAmbColorScaled1(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its own LUT factor and the
    // shared s16-derived scale, then clamped to 255; alpha is the raw latch.
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658348[0] * lbl_eu_806657E4);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658348[1] * lbl_eu_806657E4);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658348[2] * lbl_eu_806657E4);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorDirect() {
    // Build the color-and-alpha ambient color from this object's RGB bytes and
    // the shared alpha latch, then emit it.
    GXColor color = { field_0x0, field_0x1, field_0x2, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, color);
}

void setAmbColorScaled2(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its own LUT factor and the
    // shared s16-derived scale, then clamped to 255; alpha is the raw latch.
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658358[0] * lbl_eu_806657E4);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658358[1] * lbl_eu_806657E4);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658358[2] * lbl_eu_806657E4);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void setAmbColorScaledAlpha1(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (A630 * latch * 806657E4), then clamped to 255;
    // alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)lbl_eu_80665814);
    scale = scale * lbl_eu_806657E4;
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658348[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658348[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658348[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void setAmbColorAlphaSimple(GXColor* color) {
    // Ambient color: each RGB byte is scaled by the alpha latch (converted
    // through s32->f32) times the shared A630 factor; alpha stays raw.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)lbl_eu_80665814);
    GXColor out = {
        (u8)(s32)(u8ToF_a638(color->r) * scale),
        (u8)(s32)(u8ToF_a638(color->g) * scale),
        (u8)(s32)(u8ToF_a638(color->b) * scale),
        (u8)lbl_eu_80665814,
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void setAmbColorScaledAlpha2(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (A630 * latch * 806657E4), then clamped to 255;
    // alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)lbl_eu_80665814);
    scale = scale * lbl_eu_806657E4;
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658358[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658358[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658358[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void setAmbColorWithAlpha1(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and 806657E4,
    // then clamped to 255; alpha is the latch scaled by 80665808.
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658348[0] * lbl_eu_806657E4);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658348[1] * lbl_eu_806657E4);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658348[2] * lbl_eu_806657E4);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = {
        (u8)r,
        (u8)g,
        (u8)b,
        (u8)(s32)(s32ToF_a628((s32)lbl_eu_80665814) * lbl_eu_80665808),
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::func_80467E14() {
    f32 conv = s32ToF_a628((s32)lbl_eu_80665814);
    f32 scaled = conv * lbl_eu_80665808;
    GXColor color = {
        field_0x0,
        field_0x1,
        field_0x2,
        (u8)(s32)scaled,
    };
    GXSetChanAmbColor(GX_COLOR0A0, color);
}

void setAmbColorWithAlpha2(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and 806657E4,
    // then clamped to 255; alpha is the latch scaled by 80665808.
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658358[0] * lbl_eu_806657E4);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658358[1] * lbl_eu_806657E4);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658358[2] * lbl_eu_806657E4);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = {
        (u8)r,
        (u8)g,
        (u8)b,
        (u8)(s32)(s32ToF_a628((s32)lbl_eu_80665814) * lbl_eu_80665808),
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void setAmbColorComposite1(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (80665808 * A630 * latch * 806657E4), then clamped
    // to 255; alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)lbl_eu_80665814);
    scale = scale * lbl_eu_806657E4;
    scale = lbl_eu_80665808 * scale;
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658348[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658348[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658348[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void setAmbColorComposite2(GXColor* color) {
    // Ambient color: each RGB byte is scaled by the alpha latch (converted
    // through s32->f32) times the A630 and 80665808 factors; alpha stays raw.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)lbl_eu_80665814);
    scale = scale * lbl_eu_80665808;
    GXColor out = {
        (u8)(s32)(u8ToF_a638(color->r) * scale),
        (u8)(s32)(u8ToF_a638(color->g) * scale),
        (u8)(s32)(u8ToF_a638(color->b) * scale),
        (u8)lbl_eu_80665814,
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void setAmbColorComposite3(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (80665808 * A630 * latch * 806657E4), then clamped
    // to 255; alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)lbl_eu_80665814);
    scale = scale * lbl_eu_806657E4;
    scale = lbl_eu_80665808 * scale;
    s32 r = (s32)(u8ToF_a638(color->r) * lbl_eu_80658358[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * lbl_eu_80658358[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * lbl_eu_80658358[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)lbl_eu_80665814 };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setTevSwapRedAlpha() {
    // Identity swap tables: every channel maps to red except alpha.
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
}

void LOD::UnkClass_80466348::setTevSwapIdentity() {
    // Identity swap tables: red->red, green->green, blue->blue, alpha->alpha.
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void LOD::UnkClass_80466348::setCurrentMtxCached() {
    // Cache the current-matrix value; only re-issue GXSetCurrentMtx when it changes.
    if ((s32)lbl_eu_80665818 != (s32)(u32)this) {
        lbl_eu_80665818 = (u32)this;
        GXSetCurrentMtx((u32)this);
    }
}

// --- static initializer (.ctors) ---
// .ctors static initializer: fill the LOD fog/color ramp (lbl_eu_80658380)
// from the shared sdata2 float constants.
//
// KNOWN CEILING: the retail .o registers sinit_804683E8 in .ctors, but as a
// plain C-linkage free function MWCC emits no .ctors entry, so the unit's
// single unmatched data slot (data 99.5%) is deferred (see CNReqtaskCheck).
extern "C" void sinit_804683E8() {
    float* ramp = lbl_eu_80658380;
    ramp[0] = lbl_eu_8066A640;
    ramp[1] = lbl_eu_8066A620;
    ramp[2] = lbl_eu_8066A620;
    ramp[3] = lbl_eu_8066A640;
    ramp[4] = lbl_eu_8066A620;
    ramp[5] = lbl_eu_8066A644;
    ramp[6] = lbl_eu_8066A620;
    ramp[7] = lbl_eu_8066A640;
    ramp[8] = lbl_eu_8066A620;
    ramp[9] = lbl_eu_8066A620;
    ramp[10] = lbl_eu_8066A620;
    ramp[11] = lbl_eu_8066A624;
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// unit monolib/src/lod/UnkClass_80466348
// foreign refs
extern "C" void setColorChanRegReg1__Q23LOD17UnkClass_80466348Fv();
extern "C" void setColorChanRegReg2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setColorChanRegVtx1__Q23LOD17UnkClass_80466348Fv();
extern "C" void setColorChanRegVtx2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setColorChanRegReg3__Q23LOD17UnkClass_80466348Fv();
extern "C" void setColorChanRegVtx3__Q23LOD17UnkClass_80466348Fv();
extern "C" void setColorChanDisable__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAlphaChanVtx__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAlphaChanReg__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexRasc__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexC0AlphaA0__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexKonstC0__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexKonstSubC0__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexMulC0__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexAlpha__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexKonstOnly__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexRascReg0__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageTexRascReg1__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageC0C1Tex__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageC0C1TexAlpha__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageC0TexAlphaBlend__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageKonstTexReg1__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageRascTexC0__Q23LOD17UnkClass_80466348Fv();
extern "C" void tevStageRascTexSubC0__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPosTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPosClrTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPosNrmTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPosNrmClrTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPosNrmTex2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPosNrmClrTex2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPosClrIndexed__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPnPosTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPnPosClrTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPnPosNrmTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPnPosNrmClrTex__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPnPosNrmTex3__Q23LOD17UnkClass_80466348Fv();
extern "C" void setupVtxDescPnPosNrmClrTex3__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAlphaCompareAlways__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAlphaCompareGe80__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAlphaCompareGe1__Q23LOD17UnkClass_80466348Fv();
extern "C" void setBlendModeNone__Q23LOD17UnkClass_80466348Fv();
extern "C" void setBlendModeBlend__Q23LOD17UnkClass_80466348Fv();
extern "C" void setBlendModeAdd__Q23LOD17UnkClass_80466348Fv();
extern "C" void setBlendModeReplace__Q23LOD17UnkClass_80466348Fv();
extern "C" void setBlendModeSubtract__Q23LOD17UnkClass_80466348Fv();
extern "C" void setTexGenMtx2x4Field__Q23LOD17UnkClass_80466348Fv();
extern "C" void setTexGenMtx3x4Nrm__Q23LOD17UnkClass_80466348Fv();
extern "C" void setTexGenMtx3x4Pos__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorScaled1__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorDirect__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorScaled2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorScaledAlpha1__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorAlphaSimple__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorScaledAlpha2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorWithAlpha1__Q23LOD17UnkClass_80466348Fv();
extern "C" void func_80467E14__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorWithAlpha2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorComposite1__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorComposite2__Q23LOD17UnkClass_80466348Fv();
extern "C" void setAmbColorComposite3__Q23LOD17UnkClass_80466348Fv();

extern u32 lbl_eu_8056D75C[19];
extern u32 lbl_eu_8056D7A8[118];

// [.data] 0x258 bytes align 8
extern "C" LodTexCoordSetup lbl_eu_8056D728[13] = { (LodTexCoordSetup)&setupVtxDescPosTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPosClrTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPosNrmTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPosNrmClrTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPosNrmTex2__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPosNrmClrTex2__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPosClrIndexed__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPnPosTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPnPosClrTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPnPosNrmTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPnPosNrmClrTex__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPnPosNrmTex3__Q23LOD17UnkClass_80466348Fv, (LodTexCoordSetup)&setupVtxDescPnPosNrmClrTex3__Q23LOD17UnkClass_80466348Fv };
u32 lbl_eu_8056D75C[19] = { (u32)&tevStageTexRasc__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexC0AlphaA0__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexKonstC0__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexKonstSubC0__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexMulC0__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexAlpha__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexKonstOnly__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexRascReg0__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageTexRascReg1__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageC0C1Tex__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageC0C1TexAlpha__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageC0TexAlphaBlend__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageKonstTexReg1__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageRascTexC0__Q23LOD17UnkClass_80466348Fv, (u32)&tevStageRascTexSubC0__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaCompareAlways__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaCompareGe80__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaCompareGe1__Q23LOD17UnkClass_80466348Fv, 0x00000000 };
u32 lbl_eu_8056D7A8[118] = { (u32)&setTexGenMtx2x4Field__Q23LOD17UnkClass_80466348Fv, (u32)&setTexGenMtx3x4Nrm__Q23LOD17UnkClass_80466348Fv, (u32)&setTexGenMtx3x4Nrm__Q23LOD17UnkClass_80466348Fv, (u32)&setTexGenMtx3x4Nrm__Q23LOD17UnkClass_80466348Fv, (u32)&setTexGenMtx3x4Pos__Q23LOD17UnkClass_80466348Fv, (u32)&loadTexMtxAndSetGen__Q23LOD17UnkClass_80466348Fv, (u32)&setBlendModeNone__Q23LOD17UnkClass_80466348Fv, (u32)&setBlendModeBlend__Q23LOD17UnkClass_80466348Fv, (u32)&setBlendModeAdd__Q23LOD17UnkClass_80466348Fv, (u32)&setBlendModeReplace__Q23LOD17UnkClass_80466348Fv, (u32)&setBlendModeSubtract__Q23LOD17UnkClass_80466348Fv, 0x00000000, (u32)&setAmbColorScaled1__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorDirect__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorScaled2__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorScaledAlpha1__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorAlphaSimple__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorScaledAlpha2__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorWithAlpha1__Q23LOD17UnkClass_80466348Fv, (u32)&func_80467E14__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorWithAlpha2__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorComposite1__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorComposite2__Q23LOD17UnkClass_80466348Fv, (u32)&setAmbColorComposite3__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegReg1__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegReg2__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegReg1__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegVtx1__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegVtx2__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegVtx1__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegReg3__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanRegVtx3__Q23LOD17UnkClass_80466348Fv, (u32)&setColorChanDisable__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanReg__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanReg__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanReg__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanVtx__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanVtx__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanVtx__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanReg__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanVtx__Q23LOD17UnkClass_80466348Fv, (u32)&setAlphaChanVtx__Q23LOD17UnkClass_80466348Fv, 0x00000001, 0x00000000, 0x00000001, 0x00000001, 0x00000000, 0x00000001, 0x00000002, 0x00000002, 0xFFFFFFFF, 0x4E573452, 0x3A466169, 0x6C656420, 0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307831, 0x66290000, 0x00000000, 0x6733645F, 0x72657374, 0x65785F61, 0x632E6800, 0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000, 0x00000000, 0x6733645F, 0x72657374, 0x65785F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420, 0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307831, 0x66290000, 0x6733645F, 0x72657370, 0x6C74745F, 0x61632E68, 0x00000000, 0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000, 0x6733645F, 0x72657370, 0x6C74745F, 0x61632E68, 0x00000000, 0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000, 0x6733645F, 0x72657370, 0x6C74745F, 0x61632E68, 0x00000000, 0x00000000 };


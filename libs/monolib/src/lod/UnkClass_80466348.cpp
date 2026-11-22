// Decompilation of monolib/src/lod/UnkClass_80466348.
//
// LOD material/vertex-format state setters: GX channel/alpha setup, TEV
// register combiners, vertex-descriptor setup, alpha/blend modes, tex-gen
// and matrix upload, ambient-color emission, and the TU's .ctors static
// initializer (sinit_804683E8).
//
// The shortened "...Fv" names in symbols.txt are placeholders: the entry
// points receive their real ABI arguments in the parameter registers (see
// docs/MWCC_CASES.md "Fv ABI note").  They are real static members of
// LOD::UnkClass_80466348 with those explicit parameters, so MWCC emits the
// accurate mangled names and the TU's .data dispatch tables link to them.

#include <harness_catalog.h>
#include <revolution/GX.h>
#include "monolib/lod/LODMemMan.hpp"
#include "monolib/lod/code_804645CC.hpp"

namespace LOD {
struct UnkClass_8046368C;
struct UnkClass_80466348 {
    u8 field_0x0;   // ambient color R
    u8 field_0x1;   // ambient color G
    u8 field_0x2;   // ambient color B
    u8 field_0x3;   // unknown; unused by this TU
    u32 field_0x4;  // unknown; its address is used as the tex-gen source
    static void setColorChanRegReg1();
    static void setColorChanRegReg2();
    static void setColorChanRegVtx1();
    static void setColorChanRegVtx2();
    static void setColorChanRegReg3();
    static void setColorChanRegVtx3();
    static void setColorChanDisable();
    static void setAlphaChanVtx();
    static void setAlphaChanReg();
    static u32 tevStageTexRasc(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale);
    static u32 tevStageTexC0AlphaA0(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale);
    static u32 tevStageTexKonstC0(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel);
    static u32 tevStageTexKonstSubC0(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel);
    static u32 tevStageTexMulC0(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale);
    static u32 tevStageTexAlpha(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale);
    static u32 tevStageTexKonstOnly(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel);
    static u32 tevStageTexRascReg0(GXTevStageID stage, u32, GXTevScale scale);
    static u32 tevStageTexRascReg1(GXTevStageID stage, u32, GXTevScale scale);
    static u32 tevStageC0C1Tex(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel);
    static u32 tevStageC0C1TexAlpha(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel);
    static u32 tevStageC0TexAlphaBlend(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale);
    static u32 tevStageKonstTexReg1(GXTevStageID stage, u32, GXTevScale scale, GXTevKColorSel sel);
    static u32 tevStageRascTexC0(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale);
    static u32 tevStageRascTexSubC0(GXTevStageID stage, GXTevRegID out_a, GXTevScale scale);
    static void setupVtxDescPosTex(GXAttrType self, s32 count);
    static void setupVtxDescPosClrTex(GXAttrType self, s32 count);
    static void setupVtxDescPosNrmTex(GXAttrType self, s32 count);
    static void setupVtxDescPosNrmClrTex(GXAttrType self, s32 count);
    static void setupVtxDescPosNrmTex2(GXAttrType self, s32 count);
    static void setupVtxDescPosNrmClrTex2(GXAttrType self, s32 count);
    static void setupVtxDescPosClrIndexed(GXAttrType self, s32 count);
    static void setupVtxDescPnPosTex(GXAttrType self, s32 count);
    static void setupVtxDescPnPosClrTex(GXAttrType self, s32 count);
    static void setupVtxDescPnPosNrmTex(GXAttrType self, s32 count);
    static void setupVtxDescPnPosNrmClrTex(GXAttrType self, s32 count);
    static void setupVtxDescPnPosNrmTex3(GXAttrType self, s32 count);
    static void setupVtxDescPnPosNrmClrTex3(GXAttrType self, s32 count);
    static void setAlphaCompareAlways();
    static void setAlphaCompareGe80();
    static void setAlphaCompareGe1();
    static void setBlendModeNone();
    static void setBlendModeBlend();
    static void setBlendModeAdd();
    static void setBlendModeReplace();
    static void setBlendModeSubtract();
    static void setTexGenMtx2x4Field(UnkClass_80466348* self);
    static void setTexGenMtx3x4Nrm(UnkClass_80466348* self);
    static void setTexGenMtx3x4Pos(UnkClass_80466348* self);
    static void loadTexMtxAndSetGen(UnkClass_80466348* self, const UnkClass_8046368C* arg);
    void loadTexMtxImmPos();
    void loadTexMtxConcat();
    static void setAmbColorScaled1(GXColor* color);
    static void setAmbColorDirect(GXColor* color);
    static void setAmbColorScaled2(GXColor* color);
    static void setAmbColorScaledAlpha1(GXColor* color);
    static void setAmbColorAlphaSimple(GXColor* color);
    static void setAmbColorScaledAlpha2(GXColor* color);
    static void setAmbColorWithAlpha1(UnkClass_80466348* self);
    static void setAmbColorWithAlpha0(UnkClass_80466348* self);
    static void setAmbColorWithAlpha2(UnkClass_80466348* self);
    static void setAmbColorComposite1(GXColor* color);
    static void setAmbColorComposite2(GXColor* color);
    static void setAmbColorComposite3(GXColor* color);
    void setTevSwapRedAlpha();
    void setTevSwapIdentity();
    void setCurrentMtxCached();
};
}

using namespace LOD;

// ---- shared LOD renderer data (.bss, non-small-data access) ----
extern u32 sLodAttnFnTable[3];
extern u32 sLodDiffuseFnTable[3];
extern f32 sAmbientScaleLut[3]; // per-channel ambient scale LUT
// per-channel ambient scale LUT (written by sinit)
extern f32 sAmbientAltScaleLut[3];
extern f32 sLodFogRamp[12]; // LOD fog/color ramp, written by sinit_804683E8
extern f32 sLodFixedTexMtx[3][4]; // fixed LOD texture matrix

// ---- shared sdata2 float constants (small-data pool) ----
extern const float lbl_eu_8066A620;
extern const float lbl_eu_8066A624;

// Builtin (f32) casts: retail's conversion idiom subtracts the 0x4330 magic
// with single rounding (fsubs), which only the builtin cast emits (a manual
// union emits double fsub + frsp and blows the schedule/size budget: tried,
// regressed the whole ambient family and broke the split budget). The builtin
// lowering pools TU-local copies of the magic doubles into this TU's .sdata2
// (0x10; retail references the value-equal blob entries lbl_eu_8066A628/A638
// instead) -- value-equal reloc drift, map-canonicalized per MWCC_CASES 7i /
// UnkVirtualFunc11 (retail fsubs => builtin cast). The named externs above are
// kept as documentation of the blob values.
inline f32 s32ToF_a628(s32 v) { return (f32)v; }
inline f32 u8ToF_a638(u8 v) { return (f32)v; }

// LOD terrain state manager (sibling TU); only the fields referenced here.
struct LOD::UnkClass_8046368C {
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
    if ((s32)sLastColorChanMode != 1) {
        sLastColorChanMode = 1;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)sLodLightMask1, (GXDiffuseFn)sLodDiffuseFnTable[1],
                      (GXAttnFn)sLodAttnFnTable[1]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegReg2() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)sLastColorChanMode != 2) {
        sLastColorChanMode = 2;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)sLodLightMask0, (GXDiffuseFn)sLodDiffuseFnTable[0],
                      (GXAttnFn)sLodAttnFnTable[0]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegVtx1() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)sLastColorChanMode != 3) {
        sLastColorChanMode = 3;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)sLodLightMask1, (GXDiffuseFn)sLodDiffuseFnTable[1],
                      (GXAttnFn)sLodAttnFnTable[1]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegVtx2() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)sLastColorChanMode != 4) {
        sLastColorChanMode = 4;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)sLodLightMask0, (GXDiffuseFn)sLodDiffuseFnTable[0],
                      (GXAttnFn)sLodAttnFnTable[0]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegReg3() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)sLastColorChanMode != 5) {
        sLastColorChanMode = 5;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG,
                      (GXLightID)sLodLightMask2, (GXDiffuseFn)sLodDiffuseFnTable[2],
                      (GXAttnFn)sLodAttnFnTable[2]);
    }
}

void LOD::UnkClass_80466348::setColorChanRegVtx3() {
    // Latch the "diffuse channel setup" state, then emit it once.
    if ((s32)sLastColorChanMode != 6) {
        sLastColorChanMode = 6;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX,
                      (GXLightID)sLodLightMask2, (GXDiffuseFn)sLodDiffuseFnTable[2],
                      (GXAttnFn)sLodAttnFnTable[2]);
    }
}

void LOD::UnkClass_80466348::setColorChanDisable() {
    // Latch the "diffuse+specular channel setup" state, then emit it once.
    if ((s32)sLastColorChanMode != 7) {
        sLastColorChanMode = 7;
        GXSetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

void LOD::UnkClass_80466348::setAlphaChanVtx() {
    // Latch the "alpha channel setup" state, then emit it once.
    if ((s32)sLastAlphaChanMode != 1) {
        sLastAlphaChanMode = 1;
        GXSetChanCtrl(GX_ALPHA0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

void LOD::UnkClass_80466348::setAlphaChanReg() {
    // Latch the "alpha channel setup" state, then emit it once.
    if ((s32)sLastAlphaChanMode != 2) {
        sLastAlphaChanMode = 2;
        GXSetChanCtrl(GX_ALPHA0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
}

// TEV combiner: texel * raster colour and texel * raster alpha, written to
// the caller-supplied output register with the caller-supplied scale.
u32 LOD::UnkClass_80466348::tevStageTexRasc(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: (const-colour * texel) + texel-alpha colour, alpha = A0,
// written to the caller-supplied output register/scale.
u32 LOD::UnkClass_80466348::tevStageTexC0AlphaA0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, (texel * KONST) + C0 colour,
// alpha = A0; output register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageTexKonstC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, (texel * KONST) - C0 colour
// (SUB), (texel * KONST) + A0 alpha; output register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageTexKonstSubC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_SUB, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: texel * const-colour colour, texel-alpha + A0 alpha;
// output register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageTexMulC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: texel-alpha * texel colour, alpha = TEXA; output
// register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageTexAlpha(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXA, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, texel * KONST colour,
// KONST + texel-alpha alpha; output register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageTexKonstOnly(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: texel * raster colour/alpha written to TEVREG0; the scale
// comes from the caller (r4 is a dead slot in the retail ABI).
u32 LOD::UnkClass_80466348::tevStageTexRascReg0(GXTevStageID self, u32, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    return 1;
}

// TEV combiner: texel * raster colour to TEVREG1, alpha = A0 to TEVREG0;
// scale from the caller (r4 is a dead slot in the retail ABI).
u32 LOD::UnkClass_80466348::tevStageTexRascReg1(GXTevStageID self, u32, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, GX_TEVREG1);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    return 1;
}

// TEV combiner: select the constant colour, C0 * C1 + texel colour,
// raster-alpha + A0 alpha; output register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageC0C1Tex(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, C0 * C1 + texel-alpha colour,
// raster-alpha + A0 alpha; output register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageC0C1TexAlpha(GXTevStageID self, GXTevRegID out_a, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_C1, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: C0 * texel + texel-alpha + C0 colour, alpha = A0; output
// register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageC0TexAlphaBlend(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_C0, GX_CC_TEXC, GX_CC_TEXA, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: select the constant colour, KONST * texel colour to TEVREG1,
// alpha = A0 to TEVREG0; scale from the caller (r4 is a dead ABI slot).
u32 LOD::UnkClass_80466348::tevStageKonstTexReg1(GXTevStageID self, u32, GXTevScale scale, GXTevKColorSel sel) {
    GXSetTevKColorSel(self, sel);
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, GX_TEVREG1);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    return 1;
}

// TEV combiner: texel * raster + C0 colour, alpha = A0; output
// register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageRascTexC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_ADD, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// TEV combiner: (texel * raster) - C0 colour (SUB), texel * raster + A0
// alpha; output register/scale from the caller.
u32 LOD::UnkClass_80466348::tevStageRascTexSubC0(GXTevStageID self, GXTevRegID out_a, GXTevScale scale) {
    GXSetTevColorIn(self, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_C0);
    GXSetTevColorOp(self, GX_TEV_SUB, GX_TB_ZERO, scale, GX_TRUE, out_a);
    GXSetTevAlphaIn(self, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_A0);
    GXSetTevAlphaOp(self, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, out_a);
    return 1;
}

// Vertex-descriptor setup: clear, enable POS, then `count` texcoords fed
// directly from this object's attribute data (the vtx-desc type is the self
// pointer; count arrives in r4 per the retail ...Fv ABI).
void LOD::UnkClass_80466348::setupVtxDescPosTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position + a run of texcoords,
    // all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

void LOD::UnkClass_80466348::setupVtxDescPosClrTex(GXAttrType self, s32 count) {
    // Clear all vertex descriptors, then enable position, color0 and a run of
    // texcoords, all fed directly from this object's attribute data.
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, self);
    GXSetVtxDesc(GX_VA_CLR0, self);
    for (s32 i = 0; i < count; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), self);
    }
}

void LOD::UnkClass_80466348::setupVtxDescPosNrmTex(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPosNrmClrTex(GXAttrType self, s32 count) {
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

void LOD::UnkClass_80466348::setupVtxDescPosNrmTex2(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPosNrmClrTex2(GXAttrType self, s32 count) {
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

void LOD::UnkClass_80466348::setupVtxDescPosClrIndexed(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPnPosTex(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPnPosClrTex(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPnPosNrmTex(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPnPosNrmClrTex(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPnPosNrmTex3(GXAttrType self, s32 count) {
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
void LOD::UnkClass_80466348::setupVtxDescPnPosNrmClrTex3(GXAttrType self, s32 count) {
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

void LOD::UnkClass_80466348::setTexGenMtx2x4Field(UnkClass_80466348* self) {
    // Tex-gen: mtx2x4; the texcoord id is `self` and the source is the
    // address of field_0x4.
    GXSetTexCoordGen2((GXTexCoordID)self, GX_TG_MTX2x4, (GXTexGenSrc)&self->field_0x4, 0x3C, GX_DISABLE, 0x7D);
}

void LOD::UnkClass_80466348::setTexGenMtx3x4Nrm(UnkClass_80466348* self) {
    // Tex-gen: normal source, mtx3x4, normalized (texcoord id in `self`).
    GXSetTexCoordGen2((GXTexCoordID)self, GX_TG_MTX3x4, GX_TG_NRM, 0x1E, GX_ENABLE, 0x40);
}

void LOD::UnkClass_80466348::setTexGenMtx3x4Pos(UnkClass_80466348* self) {
    // Fixed tex-gen setup; the texcoord id is carried in via `self` (as in
    // setTexGenMtx2x4Field/BC which pass `self` straight to GXSetTexCoordGen2).
    GXSetTexCoordGen2((GXTexCoordID)self, GX_TG_MTX3x4, GX_TG_POS, 0x1E, GX_DISABLE, 0x7D);
}

// Matrix-upload helper: build the LOD pair matrix via the record dispatcher,
// upload it as tex-mtx 0x21, and bind tex-gen with the caller's texcoord id.
void LOD::UnkClass_80466348::loadTexMtxAndSetGen(UnkClass_80466348* self,
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
    PSMTXConcat(sLodFixedTexMtx, (const f32(*)[4])this, mtx);
    GXLoadTexMtxImm(mtx, 30, GX_MTX_3x4);
}

// The object's leading bytes are the ambient colour (a GXColor view of the
// same memory the tex-gen/matrix uploaders use as a matrix); the retail
// ...Fv entry point receives it in r3 per the MWCC_CASES "Fv ABI note".
void LOD::UnkClass_80466348::setAmbColorScaled1(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its own LUT factor and the
    // shared s16-derived scale, then clamped to 255; alpha is the raw latch.
    s32 r = (s32)(u8ToF_a638(color->r) * sAmbientScaleLut[0] * sAmbientIntensityScale);
    s32 g = (s32)(u8ToF_a638(color->g) * sAmbientScaleLut[1] * sAmbientIntensityScale);
    s32 b = (s32)(u8ToF_a638(color->b) * sAmbientScaleLut[2] * sAmbientIntensityScale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)sAmbientAlpha };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorDirect(GXColor* color) {
    // Build the color-and-alpha ambient color from this object's RGB bytes and
    // the shared alpha latch, then emit it.
    GXColor out = { color->r, color->g, color->b, (u8)sAmbientAlpha };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorScaled2(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its own LUT factor and the
    // shared s16-derived scale, then clamped to 255; alpha is the raw latch.
    s32 r = (s32)(u8ToF_a638(color->r) * sAmbientAltScaleLut[0] * sAmbientIntensityScale);
    s32 g = (s32)(u8ToF_a638(color->g) * sAmbientAltScaleLut[1] * sAmbientIntensityScale);
    s32 b = (s32)(u8ToF_a638(color->b) * sAmbientAltScaleLut[2] * sAmbientIntensityScale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)sAmbientAlpha };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorScaledAlpha1(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (A630 * latch * 806657E4), then clamped to 255;
    // alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)sAmbientAlpha);
    scale = scale * sAmbientIntensityScale;
    s32 r = (s32)(u8ToF_a638(color->r) * sAmbientScaleLut[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * sAmbientScaleLut[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * sAmbientScaleLut[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)sAmbientAlpha };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorAlphaSimple(GXColor* color) {
    // Ambient color: each RGB byte is scaled by the alpha latch (converted
    // through s32->f32) times the shared A630 factor; alpha stays raw.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)sAmbientAlpha);
    GXColor out = {
        (u8)(s32)(u8ToF_a638(color->r) * scale),
        (u8)(s32)(u8ToF_a638(color->g) * scale),
        (u8)(s32)(u8ToF_a638(color->b) * scale),
        (u8)sAmbientAlpha,
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorScaledAlpha2(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (A630 * latch * 806657E4), then clamped to 255;
    // alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)sAmbientAlpha);
    scale = scale * sAmbientIntensityScale;
    s32 r = (s32)(u8ToF_a638(color->r) * sAmbientAltScaleLut[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * sAmbientAltScaleLut[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * sAmbientAltScaleLut[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)sAmbientAlpha };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorWithAlpha1(UnkClass_80466348* self) {
    // Ambient color: each RGB byte is scaled by its LUT factor and 806657E4,
    // then clamped to 255; alpha is the latch scaled by 80665808.
    s32 r = (s32)(u8ToF_a638(self->field_0x0) * sAmbientScaleLut[0] * sAmbientIntensityScale);
    s32 g = (s32)(u8ToF_a638(self->field_0x1) * sAmbientScaleLut[1] * sAmbientIntensityScale);
    s32 b = (s32)(u8ToF_a638(self->field_0x2) * sAmbientScaleLut[2] * sAmbientIntensityScale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = {
        (u8)r,
        (u8)g,
        (u8)b,
        (u8)(s32)(s32ToF_a628((s32)sAmbientAlpha) * sAmbientAlphaScale),
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorWithAlpha0(UnkClass_80466348* self) {
    f32 conv = s32ToF_a628((s32)sAmbientAlpha);
    f32 scaled = conv * sAmbientAlphaScale;
    GXColor color = {
        self->field_0x0,
        self->field_0x1,
        self->field_0x2,
        (u8)(s32)scaled,
    };
    GXSetChanAmbColor(GX_COLOR0A0, color);
}

void LOD::UnkClass_80466348::setAmbColorWithAlpha2(UnkClass_80466348* self) {
    // Ambient color: each RGB byte is scaled by its LUT factor and 806657E4,
    // then clamped to 255; alpha is the latch scaled by 80665808.
    s32 r = (s32)(u8ToF_a638(self->field_0x0) * sAmbientAltScaleLut[0] * sAmbientIntensityScale);
    s32 g = (s32)(u8ToF_a638(self->field_0x1) * sAmbientAltScaleLut[1] * sAmbientIntensityScale);
    s32 b = (s32)(u8ToF_a638(self->field_0x2) * sAmbientAltScaleLut[2] * sAmbientIntensityScale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = {
        (u8)r,
        (u8)g,
        (u8)b,
        (u8)(s32)(s32ToF_a628((s32)sAmbientAlpha) * sAmbientAlphaScale),
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorComposite1(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (80665808 * A630 * latch * 806657E4), then clamped
    // to 255; alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)sAmbientAlpha);
    scale = scale * sAmbientIntensityScale;
    scale = sAmbientAlphaScale * scale;
    s32 r = (s32)(u8ToF_a638(color->r) * sAmbientScaleLut[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * sAmbientScaleLut[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * sAmbientScaleLut[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)sAmbientAlpha };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorComposite2(GXColor* color) {
    // Ambient color: each RGB byte is scaled by the alpha latch (converted
    // through s32->f32) times the A630 and 80665808 factors; alpha stays raw.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)sAmbientAlpha);
    scale = scale * sAmbientAlphaScale;
    GXColor out = {
        (u8)(s32)(u8ToF_a638(color->r) * scale),
        (u8)(s32)(u8ToF_a638(color->g) * scale),
        (u8)(s32)(u8ToF_a638(color->b) * scale),
        (u8)sAmbientAlpha,
    };
    GXSetChanAmbColor(GX_COLOR0A0, out);
}

void LOD::UnkClass_80466348::setAmbColorComposite3(GXColor* color) {
    // Ambient color: each RGB byte is scaled by its LUT factor and the shared
    // alpha-derived scale (80665808 * A630 * latch * 806657E4), then clamped
    // to 255; alpha is the raw latch.
    f32 scale = lbl_eu_8066A630 * s32ToF_a628((s32)sAmbientAlpha);
    scale = scale * sAmbientIntensityScale;
    scale = sAmbientAlphaScale * scale;
    s32 r = (s32)(u8ToF_a638(color->r) * sAmbientAltScaleLut[0] * scale);
    s32 g = (s32)(u8ToF_a638(color->g) * sAmbientAltScaleLut[1] * scale);
    s32 b = (s32)(u8ToF_a638(color->b) * sAmbientAltScaleLut[2] * scale);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    GXColor out = { (u8)r, (u8)g, (u8)b, (u8)sAmbientAlpha };
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
    if ((s32)sCachedCurrentMtx != (s32)(u32)this) {
        sCachedCurrentMtx = (u32)this;
        GXSetCurrentMtx((u32)this);
    }
}

// --- static initializer (.ctors) ---
// .ctors static initializer: fill the LOD fog/color ramp (sLodFogRamp)
// from the shared sdata2 float constants.
//
// KNOWN CEILING: the retail .o registers sinit_804683E8 in .ctors, but as a
// plain C-linkage free function MWCC emits no .ctors entry, so the unit's
// single unmatched data slot (data 99.5%) is deferred (see CNReqtaskCheck).
extern "C" void sinit_804683E8() {
    float* ramp = sLodFogRamp;
    ramp[0] = 0.5f;
    ramp[1] = lbl_eu_8066A620;
    ramp[2] = lbl_eu_8066A620;
    ramp[3] = 0.5f;
    ramp[4] = lbl_eu_8066A620;
    ramp[5] = (-0.5f);
    ramp[6] = lbl_eu_8066A620;
    ramp[7] = 0.5f;
    ramp[8] = lbl_eu_8066A620;
    ramp[9] = lbl_eu_8066A620;
    ramp[10] = lbl_eu_8066A620;
    ramp[11] = lbl_eu_8066A624;
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// unit monolib/src/lod/UnkClass_80466348
// foreign refs

extern u32 sLodRenderStateDispatchTable[118];

// [.data] 0x258 bytes align 8
extern "C" LodTexCoordSetup sLodTexCoordSetupTable[13] = { (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPosTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPosClrTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPosNrmTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPosNrmClrTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPosNrmTex2, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPosNrmClrTex2, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPosClrIndexed, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPnPosTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPnPosClrTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPnPosNrmTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPnPosNrmClrTex, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPnPosNrmTex3, (LodTexCoordSetup)&LOD::UnkClass_80466348::setupVtxDescPnPosNrmClrTex3 };
u32 sLodTevStageSetupTable[19] = { (u32)&LOD::UnkClass_80466348::tevStageTexRasc, (u32)&LOD::UnkClass_80466348::tevStageTexC0AlphaA0, (u32)&LOD::UnkClass_80466348::tevStageTexKonstC0, (u32)&LOD::UnkClass_80466348::tevStageTexKonstSubC0, (u32)&LOD::UnkClass_80466348::tevStageTexMulC0, (u32)&LOD::UnkClass_80466348::tevStageTexAlpha, (u32)&LOD::UnkClass_80466348::tevStageTexKonstOnly, (u32)&LOD::UnkClass_80466348::tevStageTexRascReg0, (u32)&LOD::UnkClass_80466348::tevStageTexRascReg1, (u32)&LOD::UnkClass_80466348::tevStageC0C1Tex, (u32)&LOD::UnkClass_80466348::tevStageC0C1TexAlpha, (u32)&LOD::UnkClass_80466348::tevStageC0TexAlphaBlend, (u32)&LOD::UnkClass_80466348::tevStageKonstTexReg1, (u32)&LOD::UnkClass_80466348::tevStageRascTexC0, (u32)&LOD::UnkClass_80466348::tevStageRascTexSubC0, (u32)&LOD::UnkClass_80466348::setAlphaCompareAlways, (u32)&LOD::UnkClass_80466348::setAlphaCompareGe80, (u32)&LOD::UnkClass_80466348::setAlphaCompareGe1, 0x00000000 };
u32 sLodRenderStateDispatchTable[118] = { (u32)&LOD::UnkClass_80466348::setTexGenMtx2x4Field, (u32)&LOD::UnkClass_80466348::setTexGenMtx3x4Nrm, (u32)&LOD::UnkClass_80466348::setTexGenMtx3x4Nrm, (u32)&LOD::UnkClass_80466348::setTexGenMtx3x4Nrm, (u32)&LOD::UnkClass_80466348::setTexGenMtx3x4Pos, (u32)&LOD::UnkClass_80466348::loadTexMtxAndSetGen, (u32)&LOD::UnkClass_80466348::setBlendModeNone, (u32)&LOD::UnkClass_80466348::setBlendModeBlend, (u32)&LOD::UnkClass_80466348::setBlendModeAdd, (u32)&LOD::UnkClass_80466348::setBlendModeReplace, (u32)&LOD::UnkClass_80466348::setBlendModeSubtract, 0x00000000, (u32)&LOD::UnkClass_80466348::setAmbColorScaled1, (u32)&LOD::UnkClass_80466348::setAmbColorDirect, (u32)&LOD::UnkClass_80466348::setAmbColorScaled2, (u32)&LOD::UnkClass_80466348::setAmbColorScaledAlpha1, (u32)&LOD::UnkClass_80466348::setAmbColorAlphaSimple, (u32)&LOD::UnkClass_80466348::setAmbColorScaledAlpha2, (u32)&LOD::UnkClass_80466348::setAmbColorWithAlpha1, (u32)&LOD::UnkClass_80466348::setAmbColorWithAlpha0, (u32)&LOD::UnkClass_80466348::setAmbColorWithAlpha2, (u32)&LOD::UnkClass_80466348::setAmbColorComposite1, (u32)&LOD::UnkClass_80466348::setAmbColorComposite2, (u32)&LOD::UnkClass_80466348::setAmbColorComposite3, (u32)&LOD::UnkClass_80466348::setColorChanRegReg1, (u32)&LOD::UnkClass_80466348::setColorChanRegReg2, (u32)&LOD::UnkClass_80466348::setColorChanRegReg1, (u32)&LOD::UnkClass_80466348::setColorChanRegVtx1, (u32)&LOD::UnkClass_80466348::setColorChanRegVtx2, (u32)&LOD::UnkClass_80466348::setColorChanRegVtx1, (u32)&LOD::UnkClass_80466348::setColorChanRegReg3, (u32)&LOD::UnkClass_80466348::setColorChanRegVtx3, (u32)&LOD::UnkClass_80466348::setColorChanDisable, (u32)&LOD::UnkClass_80466348::setAlphaChanReg, (u32)&LOD::UnkClass_80466348::setAlphaChanReg, (u32)&LOD::UnkClass_80466348::setAlphaChanReg, (u32)&LOD::UnkClass_80466348::setAlphaChanVtx, (u32)&LOD::UnkClass_80466348::setAlphaChanVtx, (u32)&LOD::UnkClass_80466348::setAlphaChanVtx, (u32)&LOD::UnkClass_80466348::setAlphaChanReg, (u32)&LOD::UnkClass_80466348::setAlphaChanVtx, (u32)&LOD::UnkClass_80466348::setAlphaChanVtx, 0x00000001, 0x00000000, 0x00000001, 0x00000001, 0x00000000, 0x00000001, 0x00000002, 0x00000002, 0xFFFFFFFF, 0x4E573452, 0x3A466169, 0x6C656420, 0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307831, 0x66290000, 0x00000000, 0x6733645F, 0x72657374, 0x65785F61, 0x632E6800, 0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000, 0x00000000, 0x6733645F, 0x72657374, 0x65785F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420, 0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307831, 0x66290000, 0x6733645F, 0x72657370, 0x6C74745F, 0x61632E68, 0x00000000, 0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000, 0x6733645F, 0x72657370, 0x6C74745F, 0x61632E68, 0x00000000, 0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000, 0x6733645F, 0x72657370, 0x6C74745F, 0x61632E68, 0x00000000, 0x00000000 };


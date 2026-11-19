// Auto-scaffolded catalog TU for monolib/src/core/code_804F0258
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <decomp.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <math.h>
#include "monolib/effect/code_804CC2B8.hpp"
#include "monolib/effect/CERand.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CMat34.hpp"
namespace nw4r { namespace math { f32 FrSqrt(f32 x); } }

// === .sbss size=0x28 align=8 (zero-fill globals owned by this TU) ===
// A68-A74: last emitted marker screen position (written by recordMarkerPos).
extern "C" f32 sMarkerTailX = 0;
extern "C" f32 sMarkerTailY = 0;
extern "C" f32 sMarkerTrailX = 0;
extern "C" f32 sMarkerTrailY = 0;
// Retail object spans 8 bytes; only the leading flag byte is accessed.
extern "C" u8 sGradientRampInitFlag[8] = {0};
// Retail layout: A80 is an 8-byte align-8 object and A8A spans 6 bytes; the
// code only ever touches the leading flag byte.
extern "C" u8 sDefaultQuadInitFlag[8] __attribute__((aligned(8))) = {0}; // gradient table init flag (+retail tail pad)
extern "C" u8 sTexScaleInitFlag = 0; // tex-scale param table init flag
extern "C" u8 sDefaultMatrixInitFlag = 0;
// Retail A8A is a 6-byte object (flag + 5 anonymous tail bytes); MWCC
// force-aligns any >=2-byte .sbss object to 4, so the tail is modeled as
// separate 1-byte globals (CLODCacheManagerS retail-tail recipe).
extern "C" u8 sCacheViewInitFlag = 0;   // cache view init flag
extern "C" u8 lbl_eu_80665A8B_pad = 0;
extern "C" u8 lbl_eu_80665A8C_pad = 0;
extern "C" u8 lbl_eu_80665A8D_pad = 0;
extern "C" u8 lbl_eu_80665A8E_pad = 0;
extern "C" u8 lbl_eu_80665A8F_pad = 0;


// Screen-aligned quad payload drawn by drawGradQuad / drawGradBands.
struct DrawQuad {
    f32 x;         // 0x00
    f32 y;         // 0x04
    f32 z;         // 0x08
    f32 field_0xc; // 0x0c size-like
};

// Texture-matrix carrier passed as the last argument of drawTexQuad when a
// custom second texcoord transform is needed: six source floats followed by
// an in-place 3x4 output matrix.
struct QuadTexCtx {
    u8 pad_0x00[0x0c];
    f32 field_0x0c; // 0x0c
    f32 field_0x10; // 0x10
    f32 field_0x14; // 0x14
    f32 field_0x18; // 0x18
    f32 field_0x1c; // 0x1c
    f32 field_0x20; // 0x20
    f32 mtx[3][4];  // 0x24
};

// Draw-context helpers shared with code_804D854C.cpp / code_804EE558.cpp.
// Size-like draw parameter read by this entry point (float at +0xc).
struct TexDrawSize {
    u8 pad_0x00[0x0c];
    f32 field_0x0c;
};

extern "C" {
void snapTexCursor(void* desktop);
void restoreTexCursor(void* desktop);
void initEffectDrawCtx(void* draw);
int setupEffectDrawCtx(void* draw, void* desktop, void* material);
void releaseEffectDrawTex(void* draw);
void blitEffectViewRect(void* draw, int a, const void* b);
void SchedTexLoadCached(void* tex, int index, int cacheIndex, int wrap);

extern GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
extern void* cacheInstance__9CDeviceGX;
extern u32 getAdjustFlag__8CGXCacheFv(void* cache);
extern void* getSubRectPtr__8CGXCacheFv(void* cache);
void streamGradStrip(s32 texMap, void* drawCtx, const ml::CVec3* pos, const f32* rgba,
                   s32 boundFlag, const void* mtxSrc, f32 alpha, f32 alphaMax);
void drawTexQuad(s32 texMap, void* drawCtx, const DrawQuad* quad, s16 mode,
                   QuadTexCtx* ctx);
void getProjMatrix(Mtx44 mtx, s32 update, f32 bottom, f32 top);
void pushProjMtx(s32 update, f32 bottom, f32 top);
void DbgSetTevSwapMode(s32 mode);
// Gradient-shaded textured quad emitter worker (called from drawGradBands).
// Defined below, after its helpers.
struct CDrawCtxLocal;
void emitGradQuad(int texMap, CDrawCtxLocal* drawCtx, const ml::CVec3* pos,
                   const f32* gradDir, const f32* uvOrigin, const f32* color,
                   int boundFlag, int flag, QuadTexCtx* mtxCtx, u8 mode);
struct TexScaleParam;
ml::CMat34* getTexMtx(int update, TexScaleParam* params);
void DbgSetPerspectiveProj(const void* src);
f32 getWidthScale__9CDeviceVIFv();

// .bss tables shared with code_804EE558.cpp (defined there).
extern f32 sMarkerGradientRamp[12];
extern f32 sDefaultDrawQuad[4];
extern f32 sTexScaleParams[4];
extern f32 sTexScaleMatrix[12];
extern f32 sTexScaleOutput[4];
}

// Minimal draw-context object (see code_804D854C.cpp).
struct CDrawCtxLocal {
    void* field_0x00;
    s32 field_0x04;
    u8 field_0x08;
};

// Projection info fetched during the depth-sorted path of emitGradQuad;
// the perspective factor lives at +0x1e0.
struct ProjInfo {
    u8 pad_0x00[0x1e0];
    f32 field_0x1e0;
};

// GX cache device state: signed viewport extents at +0x4bc/+0x4be feed the
// gradient aspect-ratio computation in emitGradQuad.
struct CGXCacheDims {
    u8 pad_0x00[0x4bc];
    s16 field_0x4bc;
    s16 field_0x4be;
};

// Cached texture-matrix source: the GX cache viewport rect read by
// getProjMatrix / pushProjMtx / getTexMtx.
struct CGXCacheView {
    s16 field_0x0;
    s16 field_0x2;
    s16 field_0x4;
    s16 field_0x6;
};

// Texture-matrix source: bound texture, signed index guard, wrap mode.
struct TexMtxSrc {
    void* mTex;     // 0x00
    s8 mIndex;      // 0x04
    u8 pad_0x05[3];
    u32 mField08;   // 0x08
};

class CMarkerDistProvider;

// Marker trail renderer (see definition below).
extern "C" s32 drawMarkerTrail(s32 texMap, f32 alpha, CDrawCtxLocal* draw, const ml::CVec3* pos,
                  const f32* sizeF, s32 density, CMarkerDistProvider* prov,
                  QuadTexCtx* mtxSrc, s32 boundFlag);

// Marker/overlay draw entry point: sets up a draw context for `desktop` and
// blits the view into it, then emits the quad via drawMarkerTrail.
void drawMarkerEntry(void* desktop, f32 alpha, const ml::CVec3* pos, const TexDrawSize* size,
                   const f32* color, void* material, TexMtxSrc* mtxSrc, const void* clampInfo) {
    // Degenerate size or fully transparent: nothing to draw.
    if (size->field_0x0c <= 0.0f) return;
    if (alpha <= 0.0f) return;
    if (clampInfo == NULL) clampInfo = (const void*)&ceRandomizer;

    snapTexCursor(desktop);
    CDrawCtxLocal draw;
    initEffectDrawCtx(&draw);
    if (setupEffectDrawCtx(&draw, desktop, material) != 0) {
        if (mtxSrc == NULL || mtxSrc->mTex == NULL || mtxSrc->mIndex < 0) {
            blitEffectViewRect(&draw, 0, 0);
            drawMarkerTrail(0, alpha, &draw, pos, reinterpret_cast<const f32*>(size),
                          (s32)1, static_cast<CMarkerDistProvider*>(0),
                          static_cast<QuadTexCtx*>(0), (s32)-1);
        } else {
            blitEffectViewRect(&draw, 0, 0);
            if (mtxSrc->mTex != NULL) {
                SchedTexLoadCached(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            drawMarkerTrail(0, alpha, &draw, pos, reinterpret_cast<const f32*>(size),
                          (s32)1, static_cast<CMarkerDistProvider*>(0),
                          reinterpret_cast<QuadTexCtx*>(mtxSrc), (s32)1);
        }
        releaseEffectDrawTex(&draw);
    }
    restoreTexCursor(desktop);
}

class CMarkerDistProvider;
class CMarkerUser;
extern "C" void recordMarkerPos(CMarkerUser* user, f32 scale, ml::CVec3* origin, ml::CVec3* extent,
                   ml::CVec3* step, s32 count, u8* colorBytes,
                   CMarkerDistProvider* provider, s32 farFlag, bool saveFirst);

// Pack the four scaled size floats into the marker tint color bytes.
struct ColorBytes {
    u8 c[4];
};

// GX FIFO write window used by the direct vertex emitters here.
// 0xCC008000: MWCC encodes the store displacement as base 0xCC010000 with
// disp -0x8000 (s16 displacement range), matching retail lis rX, 0xcc01.
union FifoWord {
    f32 f;
    u8 b;
};

// Marker trail renderer: refreshes the ortho projection, configures the GX
// pipeline for a screen-space marker (flat when boundFlag<0, textured when
// >=0), normalizes the marker position against the framebuffer, then walks
// the four gradient-table segments emitting trail dots through
// recordMarkerPos. The triangle strip is bookended by two FIFO vertices: the
// scaled marker position up front and the first emitted trail point (saved
// in the saved marker coordinates via recordMarkerPos's saveFirst) at the end.
extern "C" s32 drawMarkerTrail(s32 texMap, f32 alpha, CDrawCtxLocal* draw, const ml::CVec3* pos,
                  const f32* sizeF, s32 density, CMarkerDistProvider* prov,
                  QuadTexCtx* mtxSrc, s32 boundFlag) {
    f32 scrH = (f32)getRenderModeObj__9CDeviceVIFv()->efbHeight;

    // Scale the four size floats and pack them to bytes (marker tint color).
    GXRenderModeObj* rm = getRenderModeObj__9CDeviceVIFv();
    f32 s0 = 255.0f * sizeF[0];
    f32 s1 = 255.0f * sizeF[1];
    f32 s2 = 255.0f * sizeF[2];
    f32 s3 = 255.0f * sizeF[3];
    u16 rawW = rm->fbWidth;
    ColorBytes col;
    col.c[0] = (u8)(s32)s0;
    col.c[1] = (u8)(s32)s1;
    col.c[2] = (u8)(s32)s2;
    col.c[3] = (u8)(s32)s3;
    ColorBytes colOut = col;
    f32 scrW1 = (f32)rawW;

    f32 scrW2 = (f32)getRenderModeObj__9CDeviceVIFv()->fbWidth;
    f32 scrH2 = (f32)getRenderModeObj__9CDeviceVIFv()->efbHeight;
    pushProjMtx(1, scrH2, scrW2);

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);
    GXTexObj* tex = reinterpret_cast<GXTexObj*>(draw->field_0x04);
    GXInitTexObjFilter(tex, GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(tex, (GXTexMapID)texMap);

    if (boundFlag < 0) {
        // Flat marker: single texgen fed by the shared texture matrix.
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* tm = getTexMtx(1, 0);
        GXLoadTexMtxImm(tm->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, (GXTexGenType)1, (GXTexGenSrc)4, 0x1e,
                          GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)0, (GXColorSrc)1,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        GXColor amb = { 0xFF, 0xFF, 0xFF, 0xFF };
        GXSetChanAmbColor(GX_COLOR0A0, amb);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap,
                      GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, (GXCompCnt)1, (GXCompType)4, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xb, (GXCompCnt)1, (GXCompType)5, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xd, (GXCompCnt)1, (GXCompType)4, 0);
        GXSetVtxDesc((GXAttr)9, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xb, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xd, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xe, GX_NONE);
    } else {
        // Textured marker: second texcoord uses the shared matrix or one
        // rebuilt in place from the caller's source floats.
        GXColor amb = { 0xFF, 0xFF, 0xFF, 0xFF };
        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        ml::CMat34* tm = getTexMtx(1, 0);
        GXLoadTexMtxImm(tm->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, (GXTexGenType)1, (GXTexGenSrc)4, 0x1e,
                          GX_FALSE, 0x7d);
        if (mtxSrc == 0) {
            GXSetTexCoordGen2(GX_TEXCOORD1, (GXTexGenType)1, (GXTexGenSrc)5,
                              0x3c, GX_FALSE, 0x7d);
        } else {
            const f32 zc = 0.0f;
            mtxSrc->mtx[0][0] = mtxSrc->field_0x18;
            mtxSrc->mtx[0][1] = zc;
            mtxSrc->mtx[0][2] = zc;
            mtxSrc->mtx[1][0] = zc;
            mtxSrc->mtx[1][1] = mtxSrc->field_0x1c;
            mtxSrc->mtx[1][2] = zc;
            mtxSrc->mtx[2][0] = zc;
            mtxSrc->mtx[2][1] = zc;
            mtxSrc->mtx[2][2] = mtxSrc->field_0x20;
            mtxSrc->mtx[0][3] = zc + mtxSrc->field_0x0c;
            mtxSrc->mtx[1][3] = zc + mtxSrc->field_0x10;
            mtxSrc->mtx[2][3] = zc + mtxSrc->field_0x14;
            GXLoadTexMtxImm(mtxSrc->mtx, 0x21, GX_MTX_2x4);
            GXSetTexCoordGen2(GX_TEXCOORD1, (GXTexGenType)1, (GXTexGenSrc)5,
                              0x21, GX_FALSE, 0x7d);
        }
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)0, (GXColorSrc)1,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        GXSetChanAmbColor(GX_COLOR0A0, amb);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap,
                      GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)0);
        // Second stage: texcoord 1 sampled from the bound map; color output
        // zeroed, alpha passes the rasterized alpha through.
        GXSetTevOrder((GXTevStageID)1, (GXTexCoordID)1, (GXTexMapID)boundFlag,
                      (GXChannelID)0xff);
        GXSetTevColorIn((GXTevStageID)1, (GXTevColorArg)0xf, (GXTevColorArg)0xf,
                        (GXTevColorArg)0xf, (GXTevColorArg)0x0);
        GXSetTevColorOp((GXTevStageID)1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaIn((GXTevStageID)1, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x0,
                        (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
        GXSetTevAlphaOp((GXTevStageID)1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_TRUE, GX_TEVPREV);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, (GXCompCnt)1, (GXCompType)4, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xb, (GXCompCnt)1, (GXCompType)5, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xd, (GXCompCnt)1, (GXCompType)4, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xe, (GXCompCnt)1, (GXCompType)4, 0);
        GXSetVtxDesc((GXAttr)9, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xb, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xd, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xe, GX_DIRECT);
    }

    // Lazy init of the gradient ramp corners (B488-scaled unit square).
    if (sGradientRampInitFlag[0] == 0) {
        f32 z = 0.0f;
        f32 gs = 1.0f;
        sMarkerGradientRamp[0] = z;
        sMarkerGradientRamp[1] = z;
        sMarkerGradientRamp[2] = z;
        sMarkerGradientRamp[3] = gs;
        sMarkerGradientRamp[4] = z;
        sMarkerGradientRamp[5] = z;
        sMarkerGradientRamp[6] = gs;
        sMarkerGradientRamp[7] = gs;
        sMarkerGradientRamp[8] = z;
        sMarkerGradientRamp[9] = z;
        sMarkerGradientRamp[10] = gs;
        sMarkerGradientRamp[11] = z;
        sGradientRampInitFlag[0] = 1;
    }

    // Normalize the marker position against the framebuffer and clamp to the
    // unit square; textured markers carry a second texcoord pair per vertex.
    bool textured = boundFlag >= 0;
    f32 v[3];
    v[2] = 0.0f;
    v[0] = pos->x / scrH;
    v[1] = pos->y / scrW1;
    if (v[0] < 0.0f) v[0] = 0.0f;
    if (1.0f < v[0]) v[0] = 1.0f;
    if (v[1] < 0.0f) v[1] = 0.0f;
    if (1.0f < v[1]) v[1] = 1.0f;

    // Angle from the normalized position to each ramp corner, wrapped into
    // [corner-PI, corner+PI]; each segment's dot count scales with the
    // wrapped angle magnitude relative to density/TAU.
    f32 ang[4];
    ang[0] = (f32)atan2(sMarkerGradientRamp[1] - v[1], sMarkerGradientRamp[0] - v[0]);
    ang[1] = (f32)atan2(sMarkerGradientRamp[4] - v[1], sMarkerGradientRamp[3] - v[0]);
    ang[2] = (f32)atan2(sMarkerGradientRamp[7] - v[1], sMarkerGradientRamp[6] - v[0]);
    ang[3] = (f32)atan2(sMarkerGradientRamp[10] - v[1], sMarkerGradientRamp[9] - v[0]);

    f32 a0 = ang[0];
    f32 lo0 = ang[0] - 3.14f;
    f32 hi0 = 3.14f + ang[0];
    while (a0 <= lo0) a0 += 6.28f;
    while (hi0 < a0) a0 -= 6.28f;
    s32 n0 = (s32)((f32)density * fabsf(a0 - ang[0]) /
                   6.24f);
    if (n0 <= 0) n0 = 1;

    f32 a1 = ang[1];
    f32 lo1 = ang[1] - 3.14f;
    f32 hi1 = 3.14f + ang[1];
    while (a1 <= lo1) a1 += 6.28f;
    while (hi1 < a1) a1 -= 6.28f;
    s32 n1 = (s32)((f32)density * fabsf(a1 - ang[1]) /
                   6.24f);
    if (n1 <= 0) n1 = 1;

    f32 a2 = ang[2];
    f32 lo2 = ang[2] - 3.14f;
    f32 hi2 = 3.14f + ang[2];
    while (a2 <= lo2) a2 += 6.28f;
    while (hi2 < a2) a2 -= 6.28f;
    s32 n2 = (s32)((f32)density * fabsf(a2 - ang[2]) /
                   6.24f);
    if (n2 <= 0) n2 = 1;

    f32 a3 = ang[3];
    f32 lo3 = ang[3] - 3.14f;
    f32 hi3 = 3.14f + ang[3];
    while (a3 <= lo3) a3 += 6.28f;
    while (hi3 < a3) a3 -= 6.28f;
    s32 n3 = (s32)((f32)density * fabsf(a3 - ang[3]) /
                   6.24f);
    if (n3 <= 0) n3 = 1;

    // One triangle strip: two bookend vertices plus one dot per step.
    GXBegin((GXPrimitive)0xa0, GX_VTXFMT0, (u16)((n3 + n2) + (n1 + n0) + 2));

    // Leading strip vertex: scaled marker position with transparent tint.
    f32 hh = (f32)getRenderModeObj__9CDeviceVIFv()->efbHeight;
    f32 ww = (f32)getRenderModeObj__9CDeviceVIFv()->fbWidth;
    FifoWord* fifo = reinterpret_cast<FifoWord*>(0xCC008000);
    fifo->f = v[0] * hh;
    fifo->f = v[1] * ww;
    fifo->f = 0.0f;
    fifo->b = colOut.c[0];
    fifo->b = colOut.c[1];
    fifo->b = colOut.c[2];
    fifo->b = 0;
    fifo->f = v[0];
    fifo->f = v[1];
    if (textured) {
        fifo->f = v[0];
        fifo->f = v[1];
    }

    // Emit the four trail segments; only the first call remembers its first
    // emitted point (reused as the trailing strip vertex below).
    ml::CVec3* t = reinterpret_cast<ml::CVec3*>(sMarkerGradientRamp);
    bool save = true;
    recordMarkerPos(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t,
                  t + 1, reinterpret_cast<ml::CVec3*>(v), n0, colOut.c, prov,
                  textured, save);
    save = false;
    recordMarkerPos(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t + 1,
                  t + 2, reinterpret_cast<ml::CVec3*>(v), n1, colOut.c, prov,
                  textured, save);
    save = false;
    recordMarkerPos(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t + 2,
                  t + 3, reinterpret_cast<ml::CVec3*>(v), n2, colOut.c, prov,
                  textured, save);
    save = false;
    recordMarkerPos(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t + 3,
                  t, reinterpret_cast<ml::CVec3*>(v), n3, colOut.c, prov,
                  textured, save);

    // Trailing strip vertex: first trail point saved by recordMarkerPos.
    fifo->f = sMarkerTailX;
    fifo->f = sMarkerTailY;
    fifo->f = 0.0f;
    fifo->b = colOut.c[0];
    fifo->b = colOut.c[1];
    fifo->b = colOut.c[2];
    fifo->b = colOut.c[3];
    fifo->f = sMarkerTrailX;
    fifo->f = sMarkerTrailY;
    if (textured) {
        fifo->f = sMarkerTrailX;
        fifo->f = sMarkerTrailY;
    }
}

// Callback object polled while emitting marker vertices; slot vptr+0xc
// returns a per-marker distance factor.
class CMarkerDistProvider {
public:
    virtual ~CMarkerDistProvider();
    virtual void field_0x08();
    virtual f32 getDistFactor() = 0; // vptr + 0xc
};

static volatile FifoWord* const s_altFifo = reinterpret_cast<volatile FifoWord*>(0xCC008000);

// Screen-space marker trail: walks from `origin` toward `extent` in `count`
// steps, normalizes each step offset against `step` and streams position /
// color / texcoord data straight into the alternate GX FIFO window.
// Leading `user` parameter is unused in the body but occupies r3 in retail.
extern "C" void recordMarkerPos(CMarkerUser* user, f32 scale, ml::CVec3* origin, ml::CVec3* extent,
                   ml::CVec3* step, s32 count, u8* colorBytes,
                   CMarkerDistProvider* provider, s32 farFlag, bool saveFirst) {
    (void)user;
    // Screen-space basis: x is scaled against the framebuffer height and y
    // against the width (retail converts height first, then the loop count,
    // then reads/converts the width).
    f32 scrX = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    // Width halfword is loaded before the vector setup but converted after it
    // (retail keeps the raw value live across the delta computation).
    u16 rawW = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    f32 fkCount = (f32)(count);

    ml::CVec3 delta;
    nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(&delta),
                        reinterpret_cast<const nw4r::math::VEC3*>(extent),
                        reinterpret_cast<const nw4r::math::VEC3*>(origin));
    ml::CVec3 dir = delta;
    f32 w = (f32)(rawW);
    f32 k = 1.0f / fkCount;
    ml::CVec3 inc;
    nw4r::math::VEC3Scale(reinterpret_cast<nw4r::math::VEC3*>(&inc),
                          reinterpret_cast<const nw4r::math::VEC3*>(&dir), k);
    ml::CVec3 incStep = inc;
    ml::CVec3 cur = *origin;

    for (s16 i = 0; i < count; i++) {
        ml::CVec3 diff;
        nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(&diff),
                            reinterpret_cast<const nw4r::math::VEC3*>(&cur),
                            reinterpret_cast<const nw4r::math::VEC3*>(step));
        ml::CVec3 nrm = diff;
        ml::CVec3 work = nrm;
        f32 mag = PSVECMag(work);
        if (0.5f >= mag) {
            // Far path: fixed falloff factor times the distance factor.
            nw4r::math::VEC3Scale(reinterpret_cast<nw4r::math::VEC3*>(&work),
                                  reinterpret_cast<const nw4r::math::VEC3*>(&work),
                                  2.0f * scale * provider->getDistFactor());
        } else {
            // Near path: normalize the offset (degenerate -> zero vector),
            // then scale by the distance factor alone.
            if (work.x * work.x + work.y * work.y + work.z * work.z == 0.0f) {
                work = ml::CVec3::zero;
            } else {
                PSVECNormalize(work, work);
            }
            nw4r::math::VEC3Scale(reinterpret_cast<nw4r::math::VEC3*>(&work),
                                  reinterpret_cast<const nw4r::math::VEC3*>(&work),
                                  scale * provider->getDistFactor());
        }

        f32 px = cur.x * scrX + work.x;
        f32 py = cur.y * w + work.y;
        s_altFifo->f = px;
        s_altFifo->f = py;
        s_altFifo->f = 0.0f;
        s_altFifo->b = colorBytes[0];
        s_altFifo->b = colorBytes[1];
        s_altFifo->b = colorBytes[2];
        s_altFifo->b = colorBytes[3];
        s_altFifo->f = cur.x;
        s_altFifo->f = cur.y;
        if (farFlag != 0) {
            // Far-flagged vertices carry a second texcoord pair.
            s_altFifo->f = cur.x;
            s_altFifo->f = cur.y;
        }
        if (saveFirst && i == 0) {
            // Remember the first emitted screen position for later reuse.
            sMarkerTailX = px;
            sMarkerTailY = py;
            sMarkerTrailX = cur.x;
            sMarkerTrailY = cur.y;
        }
        nw4r::math::VEC3Add(reinterpret_cast<nw4r::math::VEC3*>(&cur),
                            reinterpret_cast<const nw4r::math::VEC3*>(&cur),
                            reinterpret_cast<const nw4r::math::VEC3*>(&incStep));
    }
}

// Textured-quad blit entry: clamps the screen-space position to the render
// target, then emits the quad through the shared draw pipeline.
// Comment above the B498/B4B8 externs: MWCC unifies the internal (f32)u16
// magic with an identical extern; binding follows the LAST declaration.
void blitTexQuad(void* desktop, ml::CVec3* pos, TexDrawSize* size, f32 alpha,
                   f32 alphaMax, void* material, TexMtxSrc* mtxSrc) {
    // Bare declarations, then assignments in alpha/fadeMax order: allocation
    // still pins fadeMax to f31 and alpha to f30, and the split keeps retail's
    // fmr f30,f1 (alpha) before fmr f31,f2 (alphaMax) emission. Copy-initialized
    // declarations emit the two fmr in the opposite order (97.1% version).
    f32 fadeMax;
    f32 a;
    a = alpha;
    fadeMax = alphaMax;
    if (size->field_0x0c <= 0.0f) return;
    if (a <= 0.0f) return;
    if (fadeMax <= 0.0f) fadeMax = a;

    ml::CVec3 p = *pos;
    f32 fbH = (f32)getRenderModeObj__9CDeviceVIFv()->efbHeight;
    f32 fbW = (f32)getRenderModeObj__9CDeviceVIFv()->fbWidth;
    if (p.x < 0.0f) p.x = 0.0f;
    if (p.y < 0.0f) p.y = 0.0f;
    if (fbW < p.x) p.x = fbW;
    if (fbH < p.y) p.y = fbH;

    snapTexCursor(desktop);
    CDrawCtxLocal draw;
    initEffectDrawCtx(&draw);
    if (setupEffectDrawCtx(&draw, desktop, material) != 0) {
        if (mtxSrc == NULL || mtxSrc->mTex == NULL || mtxSrc->mIndex < 0) {
            blitEffectViewRect(&draw, 0, 0);
            streamGradStrip(0, &draw, &p, reinterpret_cast<const f32*>(size), -1, NULL,
                          a, fadeMax);
        } else {
            blitEffectViewRect(&draw, 0, 0);
            if (mtxSrc->mTex != NULL) {
                SchedTexLoadCached(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            streamGradStrip(0, &draw, &p, reinterpret_cast<const f32*>(size), 1, mtxSrc,
                          a, fadeMax);
        }
        releaseEffectDrawTex(&draw);
    }
    restoreTexCursor(desktop);
}

// Main FIFO window used by the direct vertex emitters (WGPIPE).
static volatile FifoWord* const s_fifo = reinterpret_cast<volatile FifoWord*>(0xCC008000);

// Pack a float into a 0-255 byte the way the retail emitters do: scale,
// convert to int via the 0x4330 magic double, and keep the low byte.
static inline u8 f32ToByte(f32 v) {
    s32 i = (s32)(v * 255.0f);
    return (u8)i;
}

// Float -> u32 bit copy. mwcc has no fpr->gpr move, so this spills the
// value through a stack slot (lfs/stfs/lwz).
static inline u32 floatBits(f32 f) {
    union {
        f32 f;
        u32 u;
    } c;
    c.f = f;
    return c.u;
}

// Textured quad blit worker (see blitTexQuad). Normalizes the quad
// position against the render-mode dimensions, refreshes the projection via
// pushProjMtx, configures the fixed-function pipeline and streams a
// gradient quad strip straight into the main GX FIFO. The strip's corner
// offsets walk by alphaMax-scaled steps each iteration and the color alpha
// byte halves per emitted quad until it reaches zero.
// The FIFO pointer is deliberately non-volatile: retail relies on MWCC being
// free to schedule the word/byte stores (they all target the same FIFO window
// and hardware consumes them in issue order).
void streamGradStrip(s32 texMap, void* drawCtx, const ml::CVec3* pos, const f32* rgba,
                   s32 boundFlag, const void* mtxSrc, f32 alpha, f32 alphaMax) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    const f32 zero = 0.0f;

    // Framebuffer dims: height first, then width, each via its own
    // getRenderModeObj fetch (matches retail fetch order).
    f32 x1 = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    f32 ratio = alpha / alphaMax;
    f32 y1 = (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth);

    f32 px = pos->x;
    f32 py = pos->y;

    // Normalize the quad position against the framebuffer size.
    f32 kx = 0.5f * x1;
    f32 tx = px / kx;
    f32 dx = (x1 - px) / kx;
    f32 ky = 0.5f * y1;
    f32 ty = py / ky;
    f32 dy = (y1 - py) / ky;

    // Gradient color packed to bytes for the indexed color attribute.
    // Kept as a 4-byte array so it lives in a stack slot like retail
    // (MWCC stages the four fctiwz results through spill slots and packs
    // them with a single word copy).
    s8 col[4];
    col[0] = (s8)(rgba[0] * 255.0f);
    col[1] = (s8)(rgba[1] * 255.0f);
    col[2] = (s8)(rgba[2] * 255.0f);
    col[3] = (s8)(rgba[3] * 255.0f);

    f32 u0 = px / x1;
    f32 v0 = py / y1;

    // Running corner offsets: two start at zero, two at the framebuffer
    // extents, and every emitted quad advances them by the scaled steps.
    f32 x0 = zero;
    f32 stepPx = alphaMax * tx;
    f32 stepDx = alphaMax * dx;
    f32 stepPy = alphaMax * ty;
    f32 stepDy = alphaMax * dy;
    f32 y0 = zero;

    // Projection refresh dims (retail reads efbHeight for both slots here).
    f32 mh = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    f32 mw = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    pushProjMtx(1, mw, mh);

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);
    GXInitTexObjFilter(reinterpret_cast<GXTexObj*>(draw->field_0x04), GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(reinterpret_cast<GXTexObj*>(draw->field_0x04), (GXTexMapID)texMap);

    FifoWord* fifo;

    if (boundFlag < 0) {
        // Untextured single-texgen gradient path.
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* texMtx = getTexMtx(1, NULL);
        GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0, GX_LIGHT_NULL,
                      GX_DF_NONE, (GXAttnFn)2);
        u32 amb = 0xFFFFFFFFu;
        GXSetChanAmbColor(GX_COLOR0A0, *reinterpret_cast<GXColor*>(&amb));
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xE, GX_NONE);

        fifo = reinterpret_cast<FifoWord*>(0xCC008000);
        s16 i = 0;
        while ((f64)i < ratio) {
            x0 -= stepPx;
            x1 += stepDx;
            col[3] >>= 1;
            y0 -= stepPy;
            y1 += stepDy;
            if (col[3] == 0) return;
            GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 6);
            fifo->f = px;
            fifo->f = py;
            fifo->f = zero;
            fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
            fifo->f = u0;
            fifo->f = v0;
            fifo->f = zero;
            fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
            fifo->f = zero; fifo->f = zero;
            fifo->f = x1; fifo->f = y0;
            fifo->f = zero;
            fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
            fifo->f = zero; fifo->f = zero;
            fifo->f = x1; fifo->f = y1;
            fifo->f = zero;
            fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
            fifo->f = 1.0f; fifo->f = zero;
            fifo->f = x1; fifo->f = y1;
            fifo->f = 1.0f; fifo->f = 1.0f;
            fifo->f = 1.0f;
            fifo->f = x0; fifo->f = y1;
            fifo->f = zero;
            fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
            fifo->f = zero; fifo->f = 1.0f;
            fifo->f = x0; fifo->f = y0;
            fifo->f = zero;
            fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
            fifo->f = zero; fifo->f = zero;
            fifo->f = zero;
            i++;
        }
        return;
    }

    // Textured path: second texcoord either uses the shared matrix or a
    // transform rebuilt in place from mtxSrc's source floats.
    u32 amb = 0xFFFFFFFFu;
    GXSetNumTexGens(2);
    GXSetNumTevStages(2);
    ml::CMat34* texMtx = getTexMtx(1, NULL);
    GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
    if (mtxSrc == NULL) {
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE, 0x7d);
    } else {
        QuadTexCtx* ctx = (QuadTexCtx*)mtxSrc;
        const f32 zc = 0.0f;
        ctx->mtx[0][0] = ctx->field_0x18;
        ctx->mtx[0][1] = zc;
        ctx->mtx[0][2] = zc;
        ctx->mtx[1][0] = zc;
        ctx->mtx[1][1] = ctx->field_0x1c;
        ctx->mtx[1][2] = zc;
        ctx->mtx[2][0] = zc;
        ctx->mtx[2][1] = zc;
        ctx->mtx[2][2] = ctx->field_0x20;
        ctx->mtx[0][3] = zc + ctx->field_0x0c;
        ctx->mtx[1][3] = zc + ctx->field_0x10;
        ctx->mtx[2][3] = zc + ctx->field_0x14;
        GXLoadTexMtxImm(ctx->mtx, 0x21, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, GX_FALSE, 0x7d);
    }
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0, GX_LIGHT_NULL,
                  GX_DF_NONE, (GXAttnFn)2);
    GXSetChanAmbColor(GX_COLOR0A0, *reinterpret_cast<GXColor*>(&amb));
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xE, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc((GXAttr)0xE, GX_DIRECT);

    fifo = reinterpret_cast<FifoWord*>(0xCC008000);
    s16 i = 0;
    while ((f64)i < ratio) {
        x0 -= stepPx;
        x1 += stepDx;
        col[3] >>= 1;
        y0 -= stepPy;
        y1 += stepDy;
        if (col[3] == 0) return;
        GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 6);
        fifo->f = px;
        fifo->f = py;
        fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = u0;
        fifo->f = v0;
        fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = zero; fifo->f = zero;
        fifo->f = x1; fifo->f = y0;
        fifo->f = zero; fifo->f = zero;
        fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = zero; fifo->f = zero;
        fifo->f = x1; fifo->f = y0;
        fifo->f = x1; fifo->f = y1;
        fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = 1.0f; fifo->f = zero;
        fifo->f = x1; fifo->f = y1;
        fifo->f = 1.0f; fifo->f = 1.0f;
        fifo->f = 1.0f;
        fifo->f = x0; fifo->f = y1;
        fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = zero; fifo->f = 1.0f;
        fifo->f = x0; fifo->f = y0;
        fifo->f = zero; fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = zero; fifo->f = zero;
        fifo->f = x0; fifo->f = y0;
        fifo->f = zero; fifo->f = zero;
        i++;
    }
}

extern "C" void DbgZeroTriple(void* self) {
    u32* words = (u32*)self;
    words[2] = 0;
    words[0] = 0;
    words[1] = 0;
}

// Animation-player handle: the bound anim object, its id, and the player
// (world matrix) that v13() fetches for that id.
struct CAnim;
// The player is a 4x4 row-major matrix; its translation column
// ([0][3], [1][3], [2][3]) is the position getPos() copies out.
struct CAnimPlayer {
    f32 m[4][4];
};

struct CAnimPlayerRef {
    CAnim* mAnim;         // 0x00
    u32 mId;              // 0x04
    CAnimPlayer* mPlayer; // 0x08
};

// Virtual dispatch target: v_i at vptr offset 8+4*i (MWCC RTTI header, the
// retail vptr points at the symbol start). Only slots 13 (0x3c) and 23
// (0x64) are actually called from this TU.
struct CAnim {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual CAnimPlayer* v13(u32 id) = 0; // vptr + 0x3c: fetch player for id
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual bool v22() = 0; // vptr + 0x60: anim update/validity check
    virtual bool v23() = 0; // vptr + 0x64: player availability check
};

extern "C" void DbgBindAnimPlayer(CAnimPlayerRef* self, CAnim* anim, u32 id) {
    self->mAnim = anim;
    self->mId = id;
    self->mPlayer = 0;
    if (anim != 0 && id != 0) {
        if (anim->v23()) {
            self->mPlayer = self->mAnim->v13(self->mId);
        }
    }
}

extern "C" void attachAnim(CAnimPlayerRef* self, CAnim* anim) {
    if (self->mAnim != anim) {
        u32 id = self->mId;
        self->mAnim = anim;
        self->mPlayer = 0;
        if (anim != 0 && id != 0 && anim->v23()) {
            self->mPlayer = self->mAnim->v13(self->mId);
        }
    }
    if (self->mPlayer == 0 && self->mAnim != 0 && self->mId != 0) {
        if (self->mAnim->v23()) {
            self->mPlayer = self->mAnim->v13(self->mId);
        }
    }
}

// nw4r math/db helpers (retail plain symbols).
extern "C" {
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
}
// The FSqrt warning strings live in CActParamAnim's retail .data pool; local
// string literals here would add a new .rodata section to this TU.
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

namespace nw4r {
namespace math {
struct MTX34;
MTX34* MTX34RotAxisFIdx(MTX34* pMtx, const VEC3* pAxis, f32 fidx);
} // namespace math
} // namespace nw4r

// Extract the rotation axis/angle of the bound player's world matrix into
// `out`, renormalizing degenerate basis columns first. The copy target is a
// flat 12-float block addressed by column (mat[i], mat[4+i], mat[8+i]).
extern "C" bool getAnimRotMtx(CAnimPlayerRef* self, nw4r::math::MTX34* out) {
    if (self->mPlayer == 0) return false;

    f32 angle;
    ml::CVec3 axis;
    if (self->mAnim->v22()) {
        f32 mat[12];
        CAnimPlayer* p = self->mPlayer;
        for (int i = 0; i < 3; i++) {
            ml::CVec3 col;
            col.x = p->m[0][i];
            col.y = p->m[1][i];
            col.z = p->m[2][i];
            f32 mag = col.x * col.x + col.y * col.y + col.z * col.z;
            if (mag == 0.0f) {
                mat[i] = p->m[0][i];
                mat[4 + i] = p->m[1][i];
                mat[8 + i] = p->m[2][i];
            } else {
                if (!(mag >= 0.0f)) {
Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 len;
                if (mag > 0.0f) {
                    len = mag * nw4r::math::FrSqrt(mag);
                } else {
                    len = 0.0f;
                }
                nw4r::math::VEC3Scale(col, col, 1.0f / len);
                mat[i] = col.x;
                mat[4 + i] = col.y;
                mat[8 + i] = col.z;
            }
            mat[3] = p->m[0][3];
            mat[7] = p->m[1][3];
            mat[11] = p->m[2][3];
        }
        reinterpret_cast<ml::CMat34*>(&mat)->getRotAxis(axis, &angle);
    } else {
        reinterpret_cast<ml::CMat34*>(self->mPlayer)->getRotAxis(axis, &angle);
    }
    nw4r::math::MTX34RotAxisFIdx(out, axis, 40.743664f * angle);
    return true;
}

extern "C" s32 getAnimPos(CAnimPlayerRef* self, ml::CVec3* out) {
    if (self->mPlayer == 0) return 0;
    CAnimPlayer* p = self->mPlayer;
    // Force z,y,x evaluation order so registers match retail.
    f32 z = p->m[2][3];
    f32 y = p->m[1][3];
    f32 x = p->m[0][3];
    *out = ml::CVec3(x, y, z);
    return 1;
}

// Gradient-shaded quad draw: binds the desktop draw context, lazily fills the
// default gradient table, then routes the quad through drawTexQuad either
// directly or via a nested sub-context when the quad sits outside the
// threshold band.
void drawGradQuad(void* desktop, DrawQuad* quad, s32 mode, void* material,
                   TexMtxSrc* mtxSrc) {
    if (quad->field_0xc <= 0.0f) return;
    snapTexCursor(desktop);
    CDrawCtxLocal draw;
    initEffectDrawCtx(&draw);
    if (setupEffectDrawCtx(&draw, desktop, material) != 0) {
        bool bound = mtxSrc != NULL && mtxSrc->mTex != NULL && mtxSrc->mIndex >= 0;
        bool inside = quad->x >= 1.0f && quad->y >= 1.0f &&
                      quad->z >= 1.0f;
        if (!bound) {
            if (sDefaultQuadInitFlag[0] == 0) {
                sDefaultDrawQuad[0] = 1.0f;
                sDefaultDrawQuad[1] = 1.0f;
                sDefaultDrawQuad[2] = 1.0f;
                sDefaultDrawQuad[3] = 1.0f;
                sDefaultQuadInitFlag[0] = 1;
            }
            if (inside) {
                blitEffectViewRect(&draw, 0, 0);
                DbgSetTevSwapMode(mode);
            } else if (quad->field_0xc >= 1.0f) {
                DbgSetTevSwapMode(mode);
                blitEffectViewRect(&draw, 0, 0);
                drawTexQuad(0, &draw,
                              reinterpret_cast<const DrawQuad*>(sDefaultDrawQuad), -1, NULL);
                blitEffectViewRect(&draw, 0, 0);
                DbgSetTevSwapMode(0);
            } else {
                CDrawCtxLocal sub;
                initEffectDrawCtx(&sub);
                if (setupEffectDrawCtx(&sub, draw.field_0x00, NULL) != 0) {
                    blitEffectViewRect(&sub, 0, 0);
                    DbgSetTevSwapMode(mode);
                    drawTexQuad(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(sDefaultDrawQuad), -1, NULL);
                    blitEffectViewRect(&draw, 0, 0);
                    DbgSetTevSwapMode(0);
                    drawTexQuad(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(sDefaultDrawQuad), -1, NULL);
                    releaseEffectDrawTex(&sub);
                }
            }
            drawTexQuad(0, &draw, quad, -1, NULL);
            DbgSetTevSwapMode(0);
        } else {
            if (sDefaultQuadInitFlag[0] == 0) {
                sDefaultDrawQuad[0] = 1.0f;
                sDefaultDrawQuad[1] = 1.0f;
                sDefaultDrawQuad[2] = 1.0f;
                sDefaultDrawQuad[3] = 1.0f;
                sDefaultQuadInitFlag[0] = 1;
            }
            if (inside) {
                blitEffectViewRect(&draw, 0, 0);
                DbgSetTevSwapMode(mode);
            } else if (quad->field_0xc >= 1.0f) {
                DbgSetTevSwapMode(mode);
                blitEffectViewRect(&draw, 0, 0);
                drawTexQuad(0, &draw,
                              reinterpret_cast<const DrawQuad*>(sDefaultDrawQuad), -1, NULL);
                blitEffectViewRect(&draw, 0, 0);
                DbgSetTevSwapMode(0);
            } else {
                CDrawCtxLocal sub;
                initEffectDrawCtx(&sub);
                if (setupEffectDrawCtx(&sub, draw.field_0x00, NULL) != 0) {
                    blitEffectViewRect(&sub, 0, 0);
                    DbgSetTevSwapMode(mode);
                    drawTexQuad(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(sDefaultDrawQuad), -1, NULL);
                    blitEffectViewRect(&draw, 0, 0);
                    DbgSetTevSwapMode(0);
                    drawTexQuad(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(sDefaultDrawQuad), -1, NULL);
                    releaseEffectDrawTex(&sub);
                }
            }
            if (mtxSrc->mTex != NULL) {
                SchedTexLoadCached(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            drawTexQuad(0, &draw, quad, 1,
                          reinterpret_cast<QuadTexCtx*>(mtxSrc));
            DbgSetTevSwapMode(0);
        }
        releaseEffectDrawTex(&draw);
    }
    restoreTexCursor(desktop);
}

// Gradient-shaded screen-space quad emitter (called from drawGradQuad).
// Refreshes the projection via pushProjMtx, configures the fixed-function
// pipeline and streams one full-screen quad strip straight into the main GX
// FIFO. mode < 0 draws an untextured gradient quad; mode >= 0 draws a
// textured quad whose second texcoord either uses the shared matrix (ctx ==
// NULL) or a transform rebuilt in place from ctx's source floats.
void drawTexQuad(s32 texMap, void* drawCtx, const DrawQuad* quad, s16 mode,
                   QuadTexCtx* ctx) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    // Two distinct zero locals: MWCC keeps both live in f31/f29 across the
    // whole body and uses them for the register-sourced FIFO stores.
    const f32 zA = 0.0f;
    f32 fbW = (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth);
    const f32 zB = 0.0f;
    f32 fbH = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    f32 fbH2 = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    f32 fbW2 = (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth);
    pushProjMtx(1, fbH2, fbW2);

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);

    GXTexObj* tex = reinterpret_cast<GXTexObj*>(draw->field_0x04);
    GXInitTexObjFilter(tex, GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, (GXCompare)7, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(tex, (GXTexMapID)texMap);

    volatile FifoWord* f = s_fifo;

    if (mode < 0) {
        // Untextured gradient path: tint comes from the quad position scaled
        // by the ramp constant, packed to bytes.
        GXColor col;
        col.r = (u8)(s32)(quad->x * 255.0f);
        col.g = (u8)(s32)(quad->y * 255.0f);
        col.b = (u8)(s32)(quad->z * 255.0f);
        col.a = (u8)(s32)(quad->field_0xc * 255.0f);

        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* texMtx = getTexMtx(1, NULL);
        GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0, GX_LIGHT_NULL,
                      GX_DF_NONE, (GXAttnFn)2);
        GXSetChanMatColor(GX_COLOR0A0, col);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)0);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_NONE);

        GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 5);
        // Full-screen strip: (0,0)-(W,0)-(W,H)-(0,H)-(0,0).
        f->f = zA; f->f = zB;
        f->f = 0.0f; f->f = 0.0f; f->f = 0.0f;
        f->f = fbW; f->f = zB;
        f->f = 0.0f; f->f = 1.0f; f->f = 0.0f;
        f->f = fbW; f->f = fbH;
        f->f = 0.0f; f->f = 1.0f; f->f = 1.0f;
        f->f = zA; f->f = fbH;
        f->f = 1.0f; f->f = 0.0f; f->f = 0.0f;
        f->f = zA; f->f = zB;
        f->f = 0.0f; f->f = 0.0f; f->f = 0.0f;
    } else {
        // Textured path: tint color packed from the quad position.
        GXColor col;
        col.r = (u8)(s32)(quad->x * 255.0f);
        col.g = (u8)(s32)(quad->y * 255.0f);
        col.b = (u8)(s32)(quad->z * 255.0f);
        col.a = (u8)(s32)(quad->field_0xc * 255.0f);

        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        ml::CMat34* texMtx = getTexMtx(1, NULL);
        GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        if (ctx == NULL) {
            // Second texcoord uses the shared identity-ish matrix.
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE, 0x7d);
        } else {
            // Rebuild the second texture matrix in place from ctx's source
            // floats: diagonal from 0x18/0x1c/0x20, translation from
            // 0x0c/0x10/0x14 added to zero.
            const f32 zc = 0.0f;
            ctx->mtx[0][0] = ctx->field_0x18;
            ctx->mtx[0][1] = zc;
            ctx->mtx[0][2] = zc;
            ctx->mtx[1][0] = zc;
            ctx->mtx[1][1] = ctx->field_0x1c;
            ctx->mtx[1][2] = zc;
            ctx->mtx[2][0] = zc;
            ctx->mtx[2][1] = zc;
            ctx->mtx[2][2] = ctx->field_0x20;
            ctx->mtx[0][3] = zc + ctx->field_0x0c;
            ctx->mtx[1][3] = zc + ctx->field_0x10;
            ctx->mtx[2][3] = zc + ctx->field_0x14;
            GXLoadTexMtxImm(ctx->mtx, 0x21, GX_MTX_2x4);
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, GX_FALSE, 0x7d);
        }
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0, GX_LIGHT_NULL,
                      GX_DF_NONE, (GXAttnFn)2);
        GXSetChanMatColor(GX_COLOR0A0, col);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)0);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);

        GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 5);
        f->f = zA; f->f = zB;
        f->f = 0.0f; f->f = 0.0f; f->f = 0.0f;
        f->f = 0.0f; f->f = 0.0f;
        f->f = fbW; f->f = zB;
        f->f = 0.0f; f->f = 1.0f; f->f = 0.0f;
        f->f = fbW; f->f = fbH;
        f->f = 0.0f; f->f = 1.0f; f->f = 1.0f;
        f->f = 1.0f; f->f = 1.0f;
        f->f = zA; f->f = fbH;
        f->f = 0.0f; f->f = 0.0f; f->f = 1.0f;
        f->f = zA; f->f = zB;
        f->f = 0.0f; f->f = 0.0f; f->f = 0.0f;
        if (ctx != NULL) {
            // The custom-matrix variant emits a wider texcoord tail.
            f->f = 1.0f; f->f = 1.0f; f->f = 1.0f;
            f->f = 0.0f; f->f = 0.0f; f->f = 0.0f;
            f->f = 0.0f;
        }
    }
}
void drawTexQuad(s32 texMap, void* drawCtx, const void* quadData, s32 mode,
                   const void* mtxSrc) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    const DrawQuad* quad = static_cast<const DrawQuad*>(quadData);
    const f32 zero = 0.0f;

    GXRenderModeObj* rmode = getRenderModeObj__9CDeviceVIFv();
    f32 fbH = (f32)(rmode->efbHeight);
    f32 fbW = (f32)(rmode->fbWidth);
    pushProjMtx(1, fbH, fbW);

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);

    GXTexObj* tex = reinterpret_cast<GXTexObj*>(draw->field_0x04);
    GXInitTexObjFilter(tex, GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, (GXCompare)7, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(tex, (GXTexMapID)texMap);

    if (mode < 0) {
        // Untextured gradient path: tint comes from the quad position scaled
        // by the ramp constant, packed to bytes.
        GXColor col;
        col.r = (u8)(s32)(quad->x * 255.0f);
        col.g = (u8)(s32)(quad->y * 255.0f);
        col.b = (u8)(s32)(quad->z * 255.0f);
        col.a = (u8)(s32)(quad->field_0xc * 255.0f);

        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* texMtx = getTexMtx(1, NULL);
        GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0, GX_LIGHT_NULL,
                      GX_DF_NONE, (GXAttnFn)2);
        GXSetChanMatColor(GX_COLOR0A0, col);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, (GXTexMapID)texMap, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)0);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_NONE);

        GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 5);
        volatile FifoWord* f = s_fifo;
        // Vertex 1
        f->f = zero; f->f = zero; f->f = zero; f->f = zero; f->f = zero;
        f->f = fbH; f->f = zero; f->f = zero; f->f = 1.0f;
        // Vertex 2
        f->f = zero; f->f = fbW; f->f = zero; f->f = zero; f->f = 1.0f;
        f->f = fbH; f->f = fbW; f->f = zero; f->f = zero;
        // Vertex 3
        f->f = 1.0f; f->f = zero; f->f = zero; f->f = zero; f->f = zero;
        f->f = fbH; f->f = zero;
        // Vertex 4
        f->f = 1.0f; f->f = 1.0f; f->f = zero; f->f = fbW;
        f->f = zero; f->f = fbH; f->f = zero;
        // Vertex 5
        f->f = 1.0f; f->f = zero; f->f = zero; f->f = zero; f->f = zero;
    }

    // Common/textured path: reconfigure and emit the textured quad strip.
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, mode < 0 ? GX_NONE : GX_DIRECT);

    GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 5);
    volatile FifoWord* f = s_fifo;
    f->f = zero; f->f = zero;
    f->f = zero; f->f = zero; f->f = zero;
    f->f = fbH; f->f = zero;
    f->f = 1.0f;
    f->f = zero; f->f = fbW;
    f->f = zero;
    f->f = 1.0f;
    f->f = fbH; f->f = fbW;
    f->f = 1.0f; f->f = 1.0f;
}

// Swap-mode tables indexed by mode: rows are swap table entries 0-3.
void DbgSetTevSwapMode(s32 mode) {
    switch (mode) {
    case 1:
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        break;
    case 2:
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
        break;
    case 3:
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
        break;
    default:
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        break;
    }
}

extern "C" void DbgCopyCamProj(Mtx mtx, const void* scene) {
    Scn_CopyCamProjMatrix(scene, mtx, -1);
}

// Variant of pushProjMtx that writes the projection into a caller-supplied
// matrix instead of pushing it straight to GX. When `update` is set and the
// GX cache has flushed, the shared texture-scale table is recomputed from the
// cache view rectangle against the render mode dimensions.
void getProjMatrix(Mtx44 mtx, s32 update, f32 bottom, f32 top) {
    if (update != 0 && getAdjustFlag__8CGXCacheFv(cacheInstance__9CDeviceGX) != 0) {
        if (sCacheViewInitFlag == 0) sCacheViewInitFlag = 1;
        const CGXCacheView* view = static_cast<const CGXCacheView*>(
            getSubRectPtr__8CGXCacheFv(cacheInstance__9CDeviceGX));
        f32 fbW = (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth);
        f32 scaleX = fbW / (f32)(view->field_0x4 - view->field_0x0);
        f32 fbH = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
        f32 scaleY = fbH / (f32)(view->field_0x6 - view->field_0x2);
        f32 ax = scaleX * ((f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth) /
                           (f32)(view->field_0x0));
        f32 ay = scaleY * ((f32)(view->field_0x2) /
                           (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight));
        f32 nx = -ax;
        f32 ny = -ay;
        (void)nx; (void)ny;
        sTexScaleOutput[0] = ax;
        sTexScaleOutput[2] = scaleX;
        sTexScaleOutput[3] = scaleY;
        sTexScaleOutput[1] = ay;
    }
    C_MTXOrtho(mtx, 0.0f, bottom, 0.0f, top, 0.0f,
               -16777216.0f);
}

// Cached texture-matrix builder: converts the GX cache viewport state into a
// 2x4 texture transform. Refreshed only when `update` is set and the GX cache
// has finished flushing; falls back to the identity matrix otherwise.
struct TexScaleParam {
    f32 x; // 0x00
    f32 y; // 0x04
};

ml::CMat34* getTexMtx(int update, TexScaleParam* params) {
    if (sDefaultMatrixInitFlag == 0) sDefaultMatrixInitFlag = 1;
    if (sTexScaleInitFlag == 0) {
        sTexScaleParams[0] = 1.0f;
        sTexScaleParams[1] = 1.0f;
        sTexScaleParams[2] = 1.0f;
        sTexScaleInitFlag = 1;
    }
    if (params == NULL) params = reinterpret_cast<TexScaleParam*>(sTexScaleParams);

    if (update == 0) {
        ml::CMat34* m = reinterpret_cast<ml::CMat34*>(sTexScaleMatrix);
        *m = ml::CMat34::identity;
        return m;
    }
    if (getAdjustFlag__8CGXCacheFv(cacheInstance__9CDeviceGX) == 0) {
        ml::CMat34* m = reinterpret_cast<ml::CMat34*>(sTexScaleMatrix);
        *m = ml::CMat34::identity;
        return m;
    }
    if (sCacheViewInitFlag == 0) sCacheViewInitFlag = 1;
    const CGXCacheView* view =
        static_cast<const CGXCacheView*>(getSubRectPtr__8CGXCacheFv(cacheInstance__9CDeviceGX));

    // Texture scale factors derived from the GX cache viewport rect.
    f32 scaleX = (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth) /
                 (f32)(view->field_0x4 - view->field_0x0);
    f32 scaleY = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight) /
                 (f32)(view->field_0x6 - view->field_0x2);
    f32 ax = scaleX *
             ((f32)(view->field_0x0) /
              (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth));
    sTexScaleOutput[0] = ax;
    f32 t = (f32)(view->field_0x2) /
            (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
    f32 ay = scaleY * t;
    f32 ny = -ay;
    f32 nx = -ax;
    sTexScaleOutput[2] = scaleX;
    sTexScaleOutput[3] = scaleY;
    ml::CMat34* m = reinterpret_cast<ml::CMat34*>(sTexScaleMatrix);
    m->m[0][0] = scaleX;
    m->m[0][1] = 0.0f;
    m->m[0][2] = 0.0f;
    m->m[0][3] = 0.0f;
    sTexScaleOutput[1] = ay;
    m->m[1][0] = 0.0f;
    m->m[1][1] = scaleY;
    m->m[1][2] = 0.0f;
    m->m[1][3] = 0.0f;
    m->m[2][0] = 0.0f;
    m->m[2][1] = 0.0f;
    m->m[2][2] = 1.0f;
    m->m[2][3] = 0.0f;
    m->m[2][3] = 1.0f;
    m->m[1][3] = 0.0f + ny * params->y;
    m->m[0][3] = 0.0f + nx * params->x;
    return m;
}

// Gradient-shaded textured quad emitter: clamps the direction vector into a
// unit band, then emits up to three gradient quads through emitGradQuad -
// a background pair via a nested sub-context and the main quad directly.
void drawGradBands(void* desktop, DrawQuad* quad, ml::CVec3* dir, TexDrawSize* size,
                   void* material, TexMtxSrc* mtxSrc) {
    if (size->field_0x0c <= 0.0f) return;
    snapTexCursor(desktop);

    // Clamp the direction per axis: up to the unit vector first, then down
    // to the B444 floor (retail order).
    ml::CVec3 v = *dir;
    if (ml::CVec3::unit.x < v.x) v.x = ml::CVec3::unit.x;
    if (ml::CVec3::unit.y < v.y) v.y = ml::CVec3::unit.y;
    if (ml::CVec3::unit.z < v.z) v.z = ml::CVec3::unit.z;
    if (v.x < 0.01f) v.x = 0.01f;
    if (v.y < 0.01f) v.y = 0.01f;
    if (v.z < 0.01f) v.z = 0.01f;

    CDrawCtxLocal draw;
    initEffectDrawCtx(&draw);
    if (setupEffectDrawCtx(&draw, desktop, material) != 0) {
        if (!(mtxSrc != NULL && mtxSrc->mTex != NULL && mtxSrc->mIndex >= 0)) {
            // Unbound path.
            if (1.0f == v.x && 1.0f == v.y) {
                blitEffectViewRect(&draw, 0, 0);
            } else {
                f32 hiU[3];
                f32 loU[3];
                CDrawCtxLocal subU;
                initEffectDrawCtx(&subU);
                if (setupEffectDrawCtx(&subU, draw.field_0x00, NULL) != 0) {
                    // Per-axis min/max band for the gradient falloff.
                    f32 t = v.x * 1.01f;
                    if (t < 1.0f) {
                        loU[0] = t;
                        hiU[0] = 1.01f;
                    } else {
                        loU[0] = v.x;
                        hiU[0] = 1.0f;
                    }
                    t = v.y * 1.01f;
                    if (t < 1.0f) {
                        loU[1] = t;
                        hiU[1] = 1.01f;
                    } else {
                        loU[1] = v.y;
                        hiU[1] = 1.0f;
                    }
                    loU[2] = v.z;
                    hiU[2] = 1.0f;

                    blitEffectViewRect(&subU, 0, 0);
                    emitGradQuad(0, &subU, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit, (const f32*)hiU,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    blitEffectViewRect(&draw, 0, loU);
                    emitGradQuad(0, &subU, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    releaseEffectDrawTex(&subU);
                }
            }
            emitGradQuad(0, &draw, reinterpret_cast<const ml::CVec3*>(quad),
                          (const f32*)&ml::CVec3::unit, (const f32*)&v,
                          reinterpret_cast<const f32*>(size), -1, 1,
                          (QuadTexCtx*)NULL, 1);
        } else {
            if (1.0f == v.x && 1.0f == v.y) {
                blitEffectViewRect(&draw, 0, 0);
            } else {
                CDrawCtxLocal subB;
                f32 loB[3];
                f32 hiB[3];
                initEffectDrawCtx(&subB);
                if (setupEffectDrawCtx(&subB, draw.field_0x00, NULL) != 0) {
                    f32 t = v.x * 1.01f;
                    if (t < 1.0f) {
                        loB[0] = t;
                        hiB[0] = 1.01f;
                    } else {
                        loB[0] = v.x;
                        hiB[0] = 1.0f;
                    }
                    t = v.y * 1.01f;
                    if (t < 1.0f) {
                        loB[1] = t;
                        hiB[1] = 1.01f;
                    } else {
                        loB[1] = v.y;
                        hiB[1] = 1.0f;
                    }
                    loB[2] = v.z;
                    hiB[2] = 1.0f;

                    blitEffectViewRect(&subB, 0, 0);
                    emitGradQuad(0, &subB, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit, (const f32*)hiB,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    blitEffectViewRect(&draw, 0, loB);
                    emitGradQuad(0, &subB, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    releaseEffectDrawTex(&subB);
                }
            }
            if (mtxSrc->mTex != NULL) {
                SchedTexLoadCached(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            emitGradQuad(0, &draw, reinterpret_cast<const ml::CVec3*>(quad),
                          (const f32*)&ml::CVec3::unit, (const f32*)&v,
                          reinterpret_cast<const f32*>(size), 1, 1,
                          reinterpret_cast<QuadTexCtx*>(mtxSrc), 1);
        }
        releaseEffectDrawTex(&draw);
    }
    restoreTexCursor(desktop);
}


// Gradient-shaded textured quad emitter worker (called from drawGradBands).
// Packs the RGBA color floats into bytes, picks a z-compare mode from the
// draw flags, computes a perspective split via tan() on the depth-sorted
// path, then streams a 4-vertex gradient quad into the main FIFO.
//
// flag==0 or degenerate z -> unsorted path: position normalized against the
// render target and offset by gradDir; otherwise a perspective split point
// derived from the projection info and GX cache aspect ratio positions the
// quad. boundFlag<0 emits one texture layer; otherwise two (with an optional
// custom second matrix rebuilt in place inside mtxCtx).
void emitGradQuad(int texMap, CDrawCtxLocal* drawCtx, const ml::CVec3* pos,
                   const f32* gradDir, const f32* uvOrigin, const f32* color,
                   int boundFlag, int flag, QuadTexCtx* mtxCtx, u8 mode) {
    // Pack the four RGBA floats into bytes, staged through integer storage
    // (byte assemble -> word copy -> byte extract) as in retail.
    struct ByteColor {
        u8 b[4];
    };
    ByteColor in;
    in.b[0] = (u8)(s32)(color[0] * 255.0f);
    in.b[1] = (u8)(s32)(color[1] * 255.0f);
    in.b[2] = (u8)(s32)(color[2] * 255.0f);
    in.b[3] = (u8)(s32)(color[3] * 255.0f);
    union {
        ByteColor bytes;
        u32 raw;
    } stage;
    stage.bytes = in;
    u32 raw = stage.raw;
    union {
        u32 raw;
        GXColor c;
    } out;
    out.raw = raw;
    GXColor col = out.c;

    // Quad corners: A = top-left, B = bottom-right; z = depth plane value.
    f32 ax, ay, bx, by, z;

    if (flag != 0 && pos->z != 0.0f) {
        // Depth-sorted path.
        ProjInfo* pi = static_cast<ProjInfo*>(Scn_FindCamItem(drawCtx->field_0x00, -1));
        f32 persp = 0.5f * pi->field_0x1e0;
        if (pos->z == 0.0f || 0.0f < pos->z) {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            z = pos->z;
        } else {
            GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
            z = -pos->z;
        }
        DbgSetPerspectiveProj(drawCtx->field_0x00);
        f32 depthTan = z * (f32)tan(3.14f * persp / 180.0f);

        // Aspect ratio of the GX cache viewport, scaled by device width scale.
        CGXCacheDims* gx = static_cast<CGXCacheDims*>(cacheInstance__9CDeviceGX);
        f32 aspect = 0.947f *
                     ((f32)(gx->field_0x4bc) /
                      (f32)(gx->field_0x4be));
        aspect *= getWidthScale__9CDeviceVIFv();

        // Split position: x measured against height, y against width.
        f32 splitX = 2.0f * depthTan * pos->x /
                     (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
        f32 negDT = -depthTan;
        ax = aspect * (splitX - depthTan);
        bx = aspect * (splitX + depthTan);
        f32 splitY = 2.0f * negDT * pos->y /
                     (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth);
        z = -depthTan;
        ay = splitY + depthTan;
        by = splitY - depthTan;
    } else {
        // Unsorted path.
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        pushProjMtx(mode, 1.0f, 1.0f);
        z = 0.0f;
        ax = pos->x;
        ay = pos->y;
        if (ax != 0.0f) {
            ax /= (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
        }
        if (ay != 0.0f) {
            ay /= (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth);
        }
        bx = ax + gradDir[0];
        by = ay + gradDir[1];
    }

    // Gradient direction extents; held in a stack array across the GX setup
    // calls below.
    f32 uv[2];
    uv[0] = uvOrigin[0];
    uv[1] = uvOrigin[1];

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);

    GXTexObj* tex = reinterpret_cast<GXTexObj*>(drawCtx->field_0x04);
    GXInitTexObjFilter(tex, GX_LINEAR, GX_LINEAR);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(tex, (GXTexMapID)texMap);

    if (boundFlag < 0) {
        // Single-texture path.
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* tm = getTexMtx(mode, (TexScaleParam*)uvOrigin);
        GXLoadTexMtxImm(tm->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        GXColor matCol = col;
        GXSetChanMatColor(GX_COLOR0A0, matCol);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xB, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xB, GX_NONE);
        GXSetVtxDesc((GXAttr)0xD, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xE, GX_NONE);

        GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 4);
        volatile FifoWord* f = s_fifo;
        f->f = ax; f->f = ay; f->f = z;
        f->f = 0.0f; f->f = 0.0f;
        f->f = bx; f->f = ay; f->f = z; f->f = uv[0]; f->f = 0.0f;
        f->f = bx; f->f = by; f->f = z; f->f = uv[0]; f->f = uv[1];
        f->f = ax; f->f = by; f->f = z; f->f = 0.0f; f->f = uv[1];
    } else {
        // Two-texture path with an optional custom second matrix.
        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        ml::CMat34* tm = getTexMtx(mode, (TexScaleParam*)uvOrigin);
        GXLoadTexMtxImm(tm->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        if (mtxCtx == NULL) {
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE, 0x7d);
        } else {
            // Rebuild the second texture matrix in place from the source
            // floats (diagonal at 0x18/0x1c/0x20, translation at 0x0c/0x10/0x14).
            mtxCtx->mtx[0][0] = mtxCtx->field_0x18;
            mtxCtx->mtx[0][1] = 0.0f;
            mtxCtx->mtx[0][2] = 0.0f;
            mtxCtx->mtx[0][3] = 0.0f + mtxCtx->field_0x0c;
            mtxCtx->mtx[1][0] = 0.0f;
            mtxCtx->mtx[1][1] = mtxCtx->field_0x1c;
            mtxCtx->mtx[1][2] = 0.0f;
            mtxCtx->mtx[1][3] = 0.0f + mtxCtx->field_0x10;
            mtxCtx->mtx[2][0] = 0.0f;
            mtxCtx->mtx[2][1] = 0.0f;
            mtxCtx->mtx[2][2] = mtxCtx->field_0x20;
            mtxCtx->mtx[2][3] = 0.0f + mtxCtx->field_0x14;
            GXLoadTexMtxImm(mtxCtx->mtx, 0x21, GX_MTX_2x4);
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x21, GX_FALSE, 0x7d);
        }
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        GXColor matCol = col;
        GXSetChanMatColor(GX_COLOR0A0, matCol);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, (GXTevMode)0);
        GXSetTevOrder((GXTevStageID)1, (GXTexCoordID)1, (GXTexMapID)boundFlag,
                      (GXChannelID)0xff);
        GXSetTevColorIn((GXTevStageID)1, (GXTevColorArg)0xf, (GXTevColorArg)0xf,
                        (GXTevColorArg)0xf, (GXTevColorArg)0x0);
        GXSetTevColorOp((GXTevStageID)1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaIn((GXTevStageID)1, (GXTevAlphaArg)7, (GXTevAlphaArg)0,
                        (GXTevAlphaArg)4, (GXTevAlphaArg)7);
        GXSetTevAlphaOp((GXTevStageID)1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                        GX_TRUE, GX_TEVPREV);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xB, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xE, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xB, GX_NONE);
        GXSetVtxDesc((GXAttr)0xD, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xE, GX_DIRECT);

        GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 4);
        volatile FifoWord* f = s_fifo;
        f->f = ax; f->f = ay; f->f = z;
        f->f = 0.0f; f->f = 0.0f;
        f->f = 0.0f; f->f = 0.0f;
        f->f = bx; f->f = ay; f->f = z; f->f = uv[0]; f->f = 0.0f;
        f->f = 0.0f; f->f = 0.0f;
        f->f = bx; f->f = by; f->f = z; f->f = uv[0]; f->f = uv[1];
        f->f = 1.0f; f->f = 1.0f;
        f->f = ax; f->f = by; f->f = z; f->f = 0.0f; f->f = uv[1];
        f->f = 0.0f; f->f = 1.0f;
    }
}

void DbgSetPerspectiveProj(const void* src) {
    Mtx44 m;
    Scn_CopyCamProjMatrix(src, m, -1);
    GXSetProjection(m, GX_PERSPECTIVE);
}

extern "C" void DbgPushPerspective(Mtx mtx){ GXSetProjection(mtx, GX_PERSPECTIVE); }

// Projection updater (see getProjMatrix): refreshes the texture-scale table
// when the cache view is valid, then builds and uploads an ortho projection
// box around +/- the given extents with 0.5 near/far planes.
void pushProjMtx(s32 update, f32 bottom, f32 top) {
    Mtx44 m;
    // Retail keeps three separate C_MTXOrtho call sites (one per path).
    if (update == 0) {
        C_MTXOrtho(m, 0.0f, top, 0.0f, bottom, 0.0f,
                   -16777216.0f);
    } else {
        if (getAdjustFlag__8CGXCacheFv(cacheInstance__9CDeviceGX) != 0) {
            if (sCacheViewInitFlag == 0) sCacheViewInitFlag = 1;
            const CGXCacheView* view = static_cast<const CGXCacheView*>(
                getSubRectPtr__8CGXCacheFv(cacheInstance__9CDeviceGX));
            // Texture scale from the cache view rect vs the render mode size.
            f32 fbW = (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth);
            f32 scaleX = fbW / (f32)(view->field_0x4 - view->field_0x0);
            f32 fbH = (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight);
            f32 scaleY = fbH / (f32)(view->field_0x6 - view->field_0x2);
            f32 ax = scaleX * ((f32)(view->field_0x0) /
                               (f32)(getRenderModeObj__9CDeviceVIFv()->fbWidth));
            f32 ay = scaleY * ((f32)(view->field_0x2) /
                               (f32)(getRenderModeObj__9CDeviceVIFv()->efbHeight));
            sTexScaleOutput[0] = ax;
            sTexScaleOutput[2] = scaleX;
            sTexScaleOutput[3] = scaleY;
            sTexScaleOutput[1] = ay;
            C_MTXOrtho(m, 0.0f, top, 0.0f, bottom, 0.0f,
                       -16777216.0f);
        } else {
            C_MTXOrtho(m, 0.0f, top, 0.0f, bottom, 0.0f,
                       -16777216.0f);
        }
    }
    GXSetProjection(m, GX_ORTHOGRAPHIC);
}

extern "C" void DbgPushOrtho(Mtx mtx){ GXSetProjection(mtx, GX_ORTHOGRAPHIC); }

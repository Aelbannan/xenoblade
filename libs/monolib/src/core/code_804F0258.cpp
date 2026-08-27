// Auto-scaffolded catalog TU for monolib/src/core/code_804F0258
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <decomp.h>
#include <revolution/GX.h>
#include <revolution/MTX.h>
#include <math.h>
#include "monolib/effect/code_804CC2B8.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CMat34.hpp"

// === .sbss size=0x28 align=8 (zero-fill globals owned by this TU) ===
// A68-A74: last emitted marker screen position (written by func_804F1B88).
extern "C" f32 lbl_eu_80665A68 = 0;
extern "C" f32 lbl_eu_80665A6C = 0;
extern "C" f32 lbl_eu_80665A70 = 0;
extern "C" f32 lbl_eu_80665A74 = 0;
// Retail object spans 8 bytes; only the leading flag byte is accessed.
extern "C" u8 lbl_eu_80665A78[8] = {0};
// Retail layout: A80 is an 8-byte align-8 object and A8A spans 6 bytes; the
// code only ever touches the leading flag byte.
extern "C" u8 lbl_eu_80665A80[8] __attribute__((aligned(8))) = {0}; // gradient table init flag (+retail tail pad)
extern "C" u8 lbl_eu_80665A88 = 0; // tex-scale param table init flag
extern "C" u8 lbl_eu_80665A89 = 0;
// Retail A8A is a 6-byte object (flag + 5 anonymous tail bytes); MWCC
// force-aligns any >=2-byte .sbss object to 4, so the tail is modeled as
// separate 1-byte globals (CLODCacheManagerS retail-tail recipe).
extern "C" u8 lbl_eu_80665A8A = 0;   // cache view init flag
extern "C" u8 lbl_eu_80665A8B_pad = 0;
extern "C" u8 lbl_eu_80665A8C_pad = 0;
extern "C" u8 lbl_eu_80665A8D_pad = 0;
extern "C" u8 lbl_eu_80665A8E_pad = 0;
extern "C" u8 lbl_eu_80665A8F_pad = 0;


// Screen-aligned quad payload drawn by func_804F2E44 / func_804F0258.
struct DrawQuad {
    f32 x;         // 0x00
    f32 y;         // 0x04
    f32 z;         // 0x08
    f32 field_0xc; // 0x0c size-like
};

// Texture-matrix carrier passed as the last argument of func_804F3258 when a
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
void func_804D8B28(void* desktop);
void func_804D8B30(void* desktop);
void func_804D8B38(void* draw);
int func_804D8B4C(void* draw, void* desktop, void* material);
void func_804D8C18(void* draw);
void func_804D8C68(void* draw, int a, const void* b);
void func_804DF164(void* tex, int index, int cacheIndex, int wrap);
extern u32 lbl_eu_80665A08; // default clamp-info blob
extern const f32 lbl_eu_8066B478; // draw-epsilon

extern GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
extern void* cacheInstance__9CDeviceGX;
extern u32 getAdjustFlag__8CGXCacheFv(void* cache);
extern void* getSubRectPtr__8CGXCacheFv(void* cache);
void func_804F213C(s32 texMap, void* drawCtx, const ml::CVec3* pos, const f32* rgba,
                   s32 boundFlag, const void* mtxSrc, f32 alpha, f32 alphaMax);
void func_804F3258(s32 texMap, void* drawCtx, const DrawQuad* quad, s16 mode,
                   QuadTexCtx* ctx);
void func_804F3B60(Mtx44 mtx, s32 update, f32 bottom, f32 top);
void func_804F4628(s32 update, f32 bottom, f32 top);
void func_804F3988(s32 mode);
// Gradient-shaded textured quad emitter worker (called from func_804F0258).
// Defined below, after its helpers.
struct CDrawCtxLocal;
void func_804F06C4(int texMap, CDrawCtxLocal* drawCtx, const ml::CVec3* pos,
                   const f32* gradDir, const f32* uvOrigin, const f32* color,
                   int boundFlag, int flag, QuadTexCtx* mtxCtx, u8 mode);
struct TexScaleParam;
ml::CMat34* func_804F42A0(int update, TexScaleParam* params);
void func_804F45EC(const void* src);
f32 getWidthScale__9CDeviceVIFv();

// Shared read-only literals (monolibdata2 .sdata2).
extern const f32 lbl_eu_8066B440; // size epsilon
extern const f32 lbl_eu_8066B444; // direction clamp min
extern const f32 lbl_eu_8066B448; // direction clamp max
extern const f32 lbl_eu_8066B44C; // direction scale
extern const f32 lbl_eu_8066B488; // spread scale
// B4B8 declared BEFORE B498: MWCC unifies its internal (f32)u16 2^52 constant
// with the first identical extern, and func_804F1F18's retail copy lives at
// B4B8 while the explicitly-passed conv helpers pin their own aliases.
extern const f64 lbl_eu_8066B498; // 0x4330000000000000 (u->f magic)
extern const f64 lbl_eu_8066B4B8; // 0x4330000000000000 (u->f magic)
extern const f64 lbl_eu_8066B4A0; // 0x4330000080000000 (s->f magic)
extern const f32 lbl_eu_8066B4A8; // near distance threshold
extern const f32 lbl_eu_8066B4AC; // far distance factor
extern const f32 lbl_eu_8066B4B0; // 0.0f
extern const f32 lbl_eu_8066B4F0; // 0.0f
extern const f32 lbl_eu_8066B4F4; // quad inside threshold
extern const f32 lbl_eu_8066B4F8;
extern const f32 lbl_eu_8066B450;
extern const f32 lbl_eu_8066B454;
extern const f32 lbl_eu_8066B458;
extern const f32 lbl_eu_8066B45C;
extern const f32 lbl_eu_8066B460;
extern const f64 lbl_eu_8066B468;
extern const f32 lbl_eu_8066B464;
extern const f64 lbl_eu_8066B470;
extern const f32 lbl_eu_8066B4C0;
extern const f32 lbl_eu_8066B4C4; // textured-path chan ambient
extern const f32 lbl_eu_8066B4C8;
extern const f32 lbl_eu_8066B4CC;
extern const f32 lbl_eu_8066B4D0;
extern const f64 lbl_eu_8066B4D8; // 0x4330000000000000 (i->f magic)
extern const f64 lbl_eu_8066B500; // 0x4330000000000000 (u->f magic)
extern const f32 lbl_eu_8066B508; // 0.5f
extern const f32 lbl_eu_8066B50C;
extern const f32 lbl_eu_8066B510; // 1.0f
extern const f64 lbl_eu_8066B518; // 0x4330000000000000 (u->f magic)
extern const f64 lbl_eu_8066B520; // 0x4330000080000000 (s->f magic)
extern const u32 lbl_eu_8066B47C; // F10A0 chan ambient (word-loaded)
extern const u32 lbl_eu_8066B480; // F10A0 textured-path chan ambient
extern const f32 lbl_eu_8066B484; // F10A0
extern const f32 lbl_eu_8066B488; // spread scale
extern const f32 lbl_eu_8066B48C; // F10A0 size-scale
extern const f32 lbl_eu_8066B490; // F10A0 chan ambient
extern const f32 lbl_eu_8066B494; // 6.24 (2pi wrap)

// .bss tables shared with code_804EE558.cpp (defined there).
extern f32 lbl_eu_806617C0[12];
extern f32 lbl_eu_806617F0[4];
extern f32 lbl_eu_80661800[4];
extern f32 lbl_eu_80661810[12];
extern f32 lbl_eu_80661840[4];
}

// Minimal draw-context object (see code_804D854C.cpp).
struct CDrawCtxLocal {
    void* field_0x00;
    s32 field_0x04;
    u8 field_0x08;
};

// Projection info fetched during the depth-sorted path of func_804F06C4;
// the perspective factor lives at +0x1e0.
struct ProjInfo {
    u8 pad_0x00[0x1e0];
    f32 field_0x1e0;
};

// GX cache device state: signed viewport extents at +0x4bc/+0x4be feed the
// gradient aspect-ratio computation in func_804F06C4.
struct CGXCacheDims {
    u8 pad_0x00[0x4bc];
    s16 field_0x4bc;
    s16 field_0x4be;
};

// Cached texture-matrix source: the GX cache viewport rect read by
// func_804F3B60 / func_804F4628 / func_804F42A0.
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

// Explicit 0x4330-magic integer -> float conversions so MWCC references the
// shared named .sdata2 magic doubles instead of synthesizing its own pool
// entries (retail relocs point at lbl_eu_* names).
static inline f32 convU16ToF(u16 v, f64 magic) {
    union {
        u32 w[2];
        f64 d;
    } c;
    c.w[1] = v;
    c.w[0] = 0x43300000;
    return (f32)(c.d - magic);
}

static inline f32 convS32ToF(s32 v, f64 magic) {
    union {
        u32 w[2];
        f64 d;
    } c;
    c.w[1] = (u32)v ^ 0x80000000;
    c.w[0] = 0x43300000;
    return (f32)(c.d - magic);
}

// Int -> double via the 0x4330 magic, kept in double precision (used for the
// gradient-step loop counter comparison; MWCC keeps the full double).
static inline f64 convS32ToD(s32 v, f64 magic) {
    union {
        u32 w[2];
        f64 d;
    } c;
    c.w[1] = (u32)v ^ 0x80000000;
    c.w[0] = 0x43300000;
    return c.d - magic;
}

class CMarkerDistProvider;

// Marker trail renderer (see definition below).
s32 func_804F10A0(s32 texMap, f32 alpha, CDrawCtxLocal* draw, const ml::CVec3* pos,
                  const f32* sizeF, s32 density, CMarkerDistProvider* prov,
                  QuadTexCtx* mtxSrc, s32 boundFlag);

// Marker/overlay draw entry point: sets up a draw context for `desktop` and
// blits the view into it, then emits the quad via func_804F10A0.
void func_804F0F2C(void* desktop, f32 alpha, const ml::CVec3* pos, const TexDrawSize* size,
                   const f32* color, void* material, TexMtxSrc* mtxSrc, const void* clampInfo) {
    // Degenerate size or fully transparent: nothing to draw.
    if (size->field_0x0c <= lbl_eu_8066B478) return;
    if (alpha <= lbl_eu_8066B478) return;
    if (clampInfo == NULL) clampInfo = (const void*)&lbl_eu_80665A08;

    func_804D8B28(desktop);
    CDrawCtxLocal draw;
    func_804D8B38(&draw);
    if (func_804D8B4C(&draw, desktop, material) != 0) {
        if (mtxSrc == NULL || mtxSrc->mTex == NULL || mtxSrc->mIndex < 0) {
            func_804D8C68(&draw, 0, 0);
            func_804F10A0(0, alpha, &draw, pos, reinterpret_cast<const f32*>(size),
                          (s32)1, static_cast<CMarkerDistProvider*>(0),
                          static_cast<QuadTexCtx*>(0), (s32)-1);
        } else {
            func_804D8C68(&draw, 0, 0);
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804F10A0(0, alpha, &draw, pos, reinterpret_cast<const f32*>(size),
                          (s32)1, static_cast<CMarkerDistProvider*>(0),
                          reinterpret_cast<QuadTexCtx*>(mtxSrc), (s32)1);
        }
        func_804D8C18(&draw);
    }
    func_804D8B30(desktop);
}

// Float -> u32 bit copy (spill through stack; see floatBits below).
static inline u32 f32bits10A0(f32 f) {
    union {
        f32 f;
        u32 u;
    } c;
    c.f = f;
    return c.u;
}

class CMarkerDistProvider;
class CMarkerUser;
void func_804F1B88(CMarkerUser* user, f32 scale, ml::CVec3* origin, ml::CVec3* extent,
                   ml::CVec3* step, s32 count, u8* colorBytes,
                   CMarkerDistProvider* provider, s32 farFlag, bool saveFirst);

// Pack the four scaled size floats into the marker tint color bytes.
struct ColorBytes {
    u8 c[4];
};

// Shared 0x4330-magic int->float conversion scratch (big-endian: w[0] holds
// the high word). Reusing one union keeps the composite in a single stack
// slot pair like retail instead of one spill slot per conversion site.
union IntFltCvt {
    u32 w[2];
    f64 d;
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
// func_804F1B88. The triangle strip is bookended by two FIFO vertices: the
// scaled marker position up front and the first emitted trail point (saved
// in lbl_eu_80665A68..74 via func_804F1B88's saveFirst) at the end.
s32 func_804F10A0(s32 texMap, f32 alpha, CDrawCtxLocal* draw, const ml::CVec3* pos,
                  const f32* sizeF, s32 density, CMarkerDistProvider* prov,
                  QuadTexCtx* mtxSrc, s32 boundFlag) {
    IntFltCvt cvt;
    cvt.w[0] = 0x43300000;
    cvt.w[1] = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    f32 scrH = (f32)(cvt.d - lbl_eu_8066B498);

    // Scale the four size floats and pack them to bytes (marker tint color).
    GXRenderModeObj* rm = getRenderModeObj__9CDeviceVIFv();
    f32 s0 = lbl_eu_8066B484 * sizeF[0];
    f32 s1 = lbl_eu_8066B484 * sizeF[1];
    f32 s2 = lbl_eu_8066B484 * sizeF[2];
    f32 s3 = lbl_eu_8066B484 * sizeF[3];
    u16 rawW = rm->fbWidth;
    ColorBytes col;
    col.c[0] = (u8)(s32)s0;
    col.c[1] = (u8)(s32)s1;
    col.c[2] = (u8)(s32)s2;
    col.c[3] = (u8)(s32)s3;
    ColorBytes colOut = col;
    cvt.w[1] = rawW;
    f32 scrW1 = (f32)(cvt.d - lbl_eu_8066B498);

    cvt.w[1] = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    f32 scrW2 = (f32)(cvt.d - lbl_eu_8066B498);
    cvt.w[1] = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    f32 scrH2 = (f32)(cvt.d - lbl_eu_8066B498);
    func_804F4628(1, scrH2, scrW2);

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
        ml::CMat34* tm = func_804F42A0(1, 0);
        GXLoadTexMtxImm(tm->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, (GXTexGenType)1, (GXTexGenSrc)4, 0x1e,
                          GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)0, (GXColorSrc)1,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        GXColor amb = *reinterpret_cast<const GXColor*>(&lbl_eu_8066B47C);
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
        GXColor amb = *reinterpret_cast<const GXColor*>(&lbl_eu_8066B480);
        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        ml::CMat34* tm = func_804F42A0(1, 0);
        GXLoadTexMtxImm(tm->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, (GXTexGenType)1, (GXTexGenSrc)4, 0x1e,
                          GX_FALSE, 0x7d);
        if (mtxSrc == 0) {
            GXSetTexCoordGen2(GX_TEXCOORD1, (GXTexGenType)1, (GXTexGenSrc)5,
                              0x3c, GX_FALSE, 0x7d);
        } else {
            const f32 zc = lbl_eu_8066B478;
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
    if (lbl_eu_80665A78[0] == 0) {
        f32 z = lbl_eu_8066B478;
        f32 gs = lbl_eu_8066B488;
        lbl_eu_806617C0[0] = z;
        lbl_eu_806617C0[1] = z;
        lbl_eu_806617C0[2] = z;
        lbl_eu_806617C0[3] = gs;
        lbl_eu_806617C0[4] = z;
        lbl_eu_806617C0[5] = z;
        lbl_eu_806617C0[6] = gs;
        lbl_eu_806617C0[7] = gs;
        lbl_eu_806617C0[8] = z;
        lbl_eu_806617C0[9] = z;
        lbl_eu_806617C0[10] = gs;
        lbl_eu_806617C0[11] = z;
        lbl_eu_80665A78[0] = 1;
    }

    // Normalize the marker position against the framebuffer and clamp to the
    // unit square; textured markers carry a second texcoord pair per vertex.
    bool textured = boundFlag >= 0;
    f32 v[3];
    v[2] = lbl_eu_8066B478;
    v[0] = pos->x / scrH;
    v[1] = pos->y / scrW1;
    if (v[0] < lbl_eu_8066B478) v[0] = lbl_eu_8066B478;
    if (lbl_eu_8066B488 < v[0]) v[0] = lbl_eu_8066B488;
    if (v[1] < lbl_eu_8066B478) v[1] = lbl_eu_8066B478;
    if (lbl_eu_8066B488 < v[1]) v[1] = lbl_eu_8066B488;

    // Angle from the normalized position to each ramp corner, wrapped into
    // [corner-PI, corner+PI]; each segment's dot count scales with the
    // wrapped angle magnitude relative to density/TAU.
    f32 ang[4];
    ang[0] = (f32)atan2(lbl_eu_806617C0[1] - v[1], lbl_eu_806617C0[0] - v[0]);
    ang[1] = (f32)atan2(lbl_eu_806617C0[4] - v[1], lbl_eu_806617C0[3] - v[0]);
    ang[2] = (f32)atan2(lbl_eu_806617C0[7] - v[1], lbl_eu_806617C0[6] - v[0]);
    ang[3] = (f32)atan2(lbl_eu_806617C0[10] - v[1], lbl_eu_806617C0[9] - v[0]);

    f32 a0 = ang[0];
    f32 lo0 = ang[0] - lbl_eu_8066B490;
    f32 hi0 = lbl_eu_8066B490 + ang[0];
    while (a0 <= lo0) a0 += lbl_eu_8066B48C;
    while (hi0 < a0) a0 -= lbl_eu_8066B48C;
    cvt.w[1] = (u32)density ^ 0x80000000u;
    s32 n0 = (s32)((f32)(cvt.d - lbl_eu_8066B4A0) * fabsf(a0 - ang[0]) /
                   lbl_eu_8066B494);
    if (n0 <= 0) n0 = 1;

    f32 a1 = ang[1];
    f32 lo1 = ang[1] - lbl_eu_8066B490;
    f32 hi1 = lbl_eu_8066B490 + ang[1];
    while (a1 <= lo1) a1 += lbl_eu_8066B48C;
    while (hi1 < a1) a1 -= lbl_eu_8066B48C;
    cvt.w[1] = (u32)density ^ 0x80000000u;
    s32 n1 = (s32)((f32)(cvt.d - lbl_eu_8066B4A0) * fabsf(a1 - ang[1]) /
                   lbl_eu_8066B494);
    if (n1 <= 0) n1 = 1;

    f32 a2 = ang[2];
    f32 lo2 = ang[2] - lbl_eu_8066B490;
    f32 hi2 = lbl_eu_8066B490 + ang[2];
    while (a2 <= lo2) a2 += lbl_eu_8066B48C;
    while (hi2 < a2) a2 -= lbl_eu_8066B48C;
    cvt.w[1] = (u32)density ^ 0x80000000u;
    s32 n2 = (s32)((f32)(cvt.d - lbl_eu_8066B4A0) * fabsf(a2 - ang[2]) /
                   lbl_eu_8066B494);
    if (n2 <= 0) n2 = 1;

    f32 a3 = ang[3];
    f32 lo3 = ang[3] - lbl_eu_8066B490;
    f32 hi3 = lbl_eu_8066B490 + ang[3];
    while (a3 <= lo3) a3 += lbl_eu_8066B48C;
    while (hi3 < a3) a3 -= lbl_eu_8066B48C;
    cvt.w[1] = (u32)density ^ 0x80000000u;
    s32 n3 = (s32)((f32)(cvt.d - lbl_eu_8066B4A0) * fabsf(a3 - ang[3]) /
                   lbl_eu_8066B494);
    if (n3 <= 0) n3 = 1;

    // One triangle strip: two bookend vertices plus one dot per step.
    GXBegin((GXPrimitive)0xa0, GX_VTXFMT0, (u16)((n3 + n2) + (n1 + n0) + 2));

    // Leading strip vertex: scaled marker position with transparent tint.
    cvt.w[1] = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    f32 hh = (f32)(cvt.d - lbl_eu_8066B498);
    cvt.w[1] = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    f32 ww = (f32)(cvt.d - lbl_eu_8066B498);
    FifoWord* fifo = reinterpret_cast<FifoWord*>(0xCC008000);
    fifo->f = v[0] * hh;
    fifo->f = v[1] * ww;
    fifo->f = lbl_eu_8066B478;
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
    ml::CVec3* t = reinterpret_cast<ml::CVec3*>(lbl_eu_806617C0);
    bool save = true;
    func_804F1B88(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t,
                  t + 1, reinterpret_cast<ml::CVec3*>(v), n0, colOut.c, prov,
                  textured, save);
    save = false;
    func_804F1B88(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t + 1,
                  t + 2, reinterpret_cast<ml::CVec3*>(v), n1, colOut.c, prov,
                  textured, save);
    save = false;
    func_804F1B88(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t + 2,
                  t + 3, reinterpret_cast<ml::CVec3*>(v), n2, colOut.c, prov,
                  textured, save);
    save = false;
    func_804F1B88(reinterpret_cast<CMarkerUser*>(draw->field_0x00), alpha, t + 3,
                  t, reinterpret_cast<ml::CVec3*>(v), n3, colOut.c, prov,
                  textured, save);

    // Trailing strip vertex: first trail point saved by func_804F1B88.
    fifo->f = lbl_eu_80665A68;
    fifo->f = lbl_eu_80665A6C;
    fifo->f = lbl_eu_8066B478;
    fifo->b = colOut.c[0];
    fifo->b = colOut.c[1];
    fifo->b = colOut.c[2];
    fifo->b = colOut.c[3];
    fifo->f = lbl_eu_80665A70;
    fifo->f = lbl_eu_80665A74;
    if (textured) {
        fifo->f = lbl_eu_80665A70;
        fifo->f = lbl_eu_80665A74;
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
void func_804F1B88(CMarkerUser* user, f32 scale, ml::CVec3* origin, ml::CVec3* extent,
                   ml::CVec3* step, s32 count, u8* colorBytes,
                   CMarkerDistProvider* provider, s32 farFlag, bool saveFirst) {
    (void)user;
    // Screen-space basis: x is scaled against the framebuffer height and y
    // against the width (retail converts height first, then the loop count,
    // then reads/converts the width).
    f32 scrX = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);
    // Width halfword is loaded before the vector setup but converted after it
    // (retail keeps the raw value live across the delta computation).
    u16 rawW = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    f32 fkCount = convS32ToF(count, lbl_eu_8066B4A0);

    ml::CVec3 delta;
    nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(&delta),
                        reinterpret_cast<const nw4r::math::VEC3*>(extent),
                        reinterpret_cast<const nw4r::math::VEC3*>(origin));
    ml::CVec3 dir = delta;
    f32 w = convU16ToF(rawW, lbl_eu_8066B498);
    f32 k = lbl_eu_8066B488 / fkCount;
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
        if (lbl_eu_8066B4A8 >= mag) {
            // Far path: fixed falloff factor times the distance factor.
            nw4r::math::VEC3Scale(reinterpret_cast<nw4r::math::VEC3*>(&work),
                                  reinterpret_cast<const nw4r::math::VEC3*>(&work),
                                  lbl_eu_8066B4AC * scale * provider->getDistFactor());
        } else {
            // Near path: normalize the offset (degenerate -> zero vector),
            // then scale by the distance factor alone.
            if (work.x * work.x + work.y * work.y + work.z * work.z == lbl_eu_8066B478) {
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
        s_altFifo->f = lbl_eu_8066B478;
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
            lbl_eu_80665A68 = px;
            lbl_eu_80665A6C = py;
            lbl_eu_80665A70 = cur.x;
            lbl_eu_80665A74 = cur.y;
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
void func_804F1F18(void* desktop, ml::CVec3* pos, TexDrawSize* size, f32 alpha,
                   f32 alphaMax, void* material, TexMtxSrc* mtxSrc) {
    // Declared in this order so MWCC pins fadeMax to f31 and alpha to f30
    // (descending-by-declaration allocation; matches retail).
    f32 fadeMax = alphaMax;
    f32 a = alpha;
    if (size->field_0x0c <= lbl_eu_8066B4B0) return;
    if (a <= lbl_eu_8066B4B0) return;
    if (fadeMax <= lbl_eu_8066B4B0) fadeMax = a;

    ml::CVec3 p = *pos;
    f32 fbH = (f32)getRenderModeObj__9CDeviceVIFv()->efbHeight;
    f32 fbW = (f32)getRenderModeObj__9CDeviceVIFv()->fbWidth;
    if (p.x < lbl_eu_8066B4B0) p.x = lbl_eu_8066B4B0;
    if (p.y < lbl_eu_8066B4B0) p.y = lbl_eu_8066B4B0;
    if (fbW < p.x) p.x = fbW;
    if (fbH < p.y) p.y = fbH;

    func_804D8B28(desktop);
    CDrawCtxLocal draw;
    func_804D8B38(&draw);
    if (func_804D8B4C(&draw, desktop, material) != 0) {
        if (mtxSrc == NULL || mtxSrc->mTex == NULL || mtxSrc->mIndex < 0) {
            func_804D8C68(&draw, 0, 0);
            func_804F213C(0, &draw, &p, reinterpret_cast<const f32*>(size), -1, NULL,
                          a, fadeMax);
        } else {
            func_804D8C68(&draw, 0, 0);
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804F213C(0, &draw, &p, reinterpret_cast<const f32*>(size), 1, mtxSrc,
                          a, fadeMax);
        }
        func_804D8C18(&draw);
    }
    func_804D8B30(desktop);
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

// Textured quad blit worker (see func_804F1F18). Normalizes the quad
// position against the render-mode dimensions, refreshes the projection via
// func_804F4628, configures the fixed-function pipeline and streams a
// gradient quad strip straight into the main GX FIFO. The strip's corner
// offsets walk by alphaMax-scaled steps each iteration and the color alpha
// byte halves per emitted quad until it reaches zero.
// The FIFO pointer is deliberately non-volatile: retail relies on MWCC being
// free to schedule the word/byte stores (they all target the same FIFO window
// and hardware consumes them in issue order).
void func_804F213C(s32 texMap, void* drawCtx, const ml::CVec3* pos, const f32* rgba,
                   s32 boundFlag, const void* mtxSrc, f32 alpha, f32 alphaMax) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    const f32 zero = lbl_eu_8066B4B0;

    // Framebuffer dims: height first, then width, each via its own
    // getRenderModeObj fetch (matches retail fetch order).
    f32 x1 = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);
    f32 ratio = alpha / alphaMax;
    f32 y1 = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B498);

    f32 px = pos->x;
    f32 py = pos->y;

    // Normalize the quad position against the framebuffer size.
    f32 kx = lbl_eu_8066B4C8 * x1;
    f32 tx = px / kx;
    f32 dx = (x1 - px) / kx;
    f32 ky = lbl_eu_8066B4C8 * y1;
    f32 ty = py / ky;
    f32 dy = (y1 - py) / ky;

    // Gradient color packed to bytes for the indexed color attribute.
    // Kept as a 4-byte array so it lives in a stack slot like retail
    // (MWCC stages the four fctiwz results through spill slots and packs
    // them with a single word copy).
    s8 col[4];
    col[0] = (s8)(rgba[0] * lbl_eu_8066B4CC);
    col[1] = (s8)(rgba[1] * lbl_eu_8066B4CC);
    col[2] = (s8)(rgba[2] * lbl_eu_8066B4CC);
    col[3] = (s8)(rgba[3] * lbl_eu_8066B4CC);

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
    f32 mh = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);
    f32 mw = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);
    func_804F4628(1, mw, mh);

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);
    GXInitTexObjFilter(reinterpret_cast<GXTexObj*>(draw->field_0x04), GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(reinterpret_cast<GXTexObj*>(draw->field_0x04), (GXTexMapID)texMap);

    FifoWord* fifo;
    const f64 i2fMagic = lbl_eu_8066B4D8;

    if (boundFlag < 0) {
        // Untextured single-texgen gradient path.
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* texMtx = func_804F42A0(1, NULL);
        GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0, GX_LIGHT_NULL,
                      GX_DF_NONE, (GXAttnFn)2);
        u32 amb = floatBits(lbl_eu_8066B4C0);
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
        while (convS32ToD(i, i2fMagic) < ratio) {
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
            fifo->f = lbl_eu_8066B4D0; fifo->f = zero;
            fifo->f = x1; fifo->f = y1;
            fifo->f = lbl_eu_8066B4D0; fifo->f = lbl_eu_8066B4D0;
            fifo->f = lbl_eu_8066B4D0;
            fifo->f = x0; fifo->f = y1;
            fifo->f = zero;
            fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
            fifo->f = zero; fifo->f = lbl_eu_8066B4D0;
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
    u32 amb = floatBits(lbl_eu_8066B4C4);
    GXSetNumTexGens(2);
    GXSetNumTevStages(2);
    ml::CMat34* texMtx = func_804F42A0(1, NULL);
    GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
    if (mtxSrc == NULL) {
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE, 0x7d);
    } else {
        QuadTexCtx* ctx = (QuadTexCtx*)mtxSrc;
        const f32 zc = lbl_eu_8066B4B0;
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
    while (convS32ToD(i, i2fMagic) < ratio) {
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
        fifo->f = lbl_eu_8066B4D0; fifo->f = zero;
        fifo->f = x1; fifo->f = y1;
        fifo->f = lbl_eu_8066B4D0; fifo->f = lbl_eu_8066B4D0;
        fifo->f = lbl_eu_8066B4D0;
        fifo->f = x0; fifo->f = y1;
        fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = zero; fifo->f = lbl_eu_8066B4D0;
        fifo->f = x0; fifo->f = y0;
        fifo->f = zero; fifo->f = zero;
        fifo->b = col[0]; fifo->b = col[1]; fifo->b = col[2]; fifo->b = col[3];
        fifo->f = zero; fifo->f = zero;
        fifo->f = x0; fifo->f = y0;
        fifo->f = zero; fifo->f = zero;
        i++;
    }
}

extern "C" void func_804F2A8C(void* self) {
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0) = 0;
    *(u32*)((u8*)self + 4) = 0;
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

void func_804F2AA0(CAnimPlayerRef* self, CAnim* anim, u32 id) {
    self->mAnim = anim;
    self->mId = id;
    self->mPlayer = 0;
    if (anim != 0 && id != 0) {
        if (anim->v23()) {
            self->mPlayer = self->mAnim->v13(self->mId);
        }
    }
}

void func_804F2B20(CAnimPlayerRef* self, CAnim* anim) {
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
float FrSqrt__Q24nw4r4mathFf(float);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
}
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];
extern const f32 lbl_eu_8066B4E0; // length epsilon
extern const f32 lbl_eu_8066B4E4; // 1.0f
extern const f32 lbl_eu_8066B4E8; // rad -> fidx scale

namespace nw4r {
namespace math {
struct MTX34;
MTX34* MTX34RotAxisFIdx(MTX34* pMtx, const VEC3* pAxis, f32 fidx);
} // namespace math
} // namespace nw4r

// Extract the rotation axis/angle of the bound player's world matrix into
// `out`, renormalizing degenerate basis columns first. The copy target is a
// flat 12-float block addressed by column (mat[i], mat[4+i], mat[8+i]).
bool func_804F2C04(CAnimPlayerRef* self, nw4r::math::MTX34* out) {
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
            if (mag == lbl_eu_8066B4E0) {
                mat[i] = p->m[0][i];
                mat[4 + i] = p->m[1][i];
                mat[8 + i] = p->m[2][i];
            } else {
                if (!(mag >= lbl_eu_8066B4E0)) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 len;
                if (mag > lbl_eu_8066B4E0) {
                    len = mag * FrSqrt__Q24nw4r4mathFf(mag);
                } else {
                    len = lbl_eu_8066B4E0;
                }
                nw4r::math::VEC3Scale(col, col, lbl_eu_8066B4E4 / len);
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
    nw4r::math::MTX34RotAxisFIdx(out, axis, lbl_eu_8066B4E8 * angle);
    return true;
}

s32 func_804F2DF0(CAnimPlayerRef* self, ml::CVec3* out) {
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
// default gradient table, then routes the quad through func_804F3258 either
// directly or via a nested sub-context when the quad sits outside the
// threshold band.
void func_804F2E44(void* desktop, DrawQuad* quad, s32 mode, void* material,
                   TexMtxSrc* mtxSrc) {
    if (quad->field_0xc <= lbl_eu_8066B4F0) return;
    func_804D8B28(desktop);
    CDrawCtxLocal draw;
    func_804D8B38(&draw);
    if (func_804D8B4C(&draw, desktop, material) != 0) {
        bool bound = mtxSrc != NULL && mtxSrc->mTex != NULL && mtxSrc->mIndex >= 0;
        bool inside = quad->x >= lbl_eu_8066B4F4 && quad->y >= lbl_eu_8066B4F4 &&
                      quad->z >= lbl_eu_8066B4F4;
        if (!bound) {
            if (lbl_eu_80665A80[0] == 0) {
                lbl_eu_806617F0[0] = lbl_eu_8066B4F4;
                lbl_eu_806617F0[1] = lbl_eu_8066B4F4;
                lbl_eu_806617F0[2] = lbl_eu_8066B4F4;
                lbl_eu_806617F0[3] = lbl_eu_8066B4F4;
                lbl_eu_80665A80[0] = 1;
            }
            if (inside) {
                func_804D8C68(&draw, 0, 0);
                func_804F3988(mode);
            } else if (quad->field_0xc >= lbl_eu_8066B4F4) {
                func_804F3988(mode);
                func_804D8C68(&draw, 0, 0);
                func_804F3258(0, &draw,
                              reinterpret_cast<const DrawQuad*>(lbl_eu_806617F0), -1, NULL);
                func_804D8C68(&draw, 0, 0);
                func_804F3988(0);
            } else {
                CDrawCtxLocal sub;
                func_804D8B38(&sub);
                if (func_804D8B4C(&sub, draw.field_0x00, NULL) != 0) {
                    func_804D8C68(&sub, 0, 0);
                    func_804F3988(mode);
                    func_804F3258(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(lbl_eu_806617F0), -1, NULL);
                    func_804D8C68(&draw, 0, 0);
                    func_804F3988(0);
                    func_804F3258(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(lbl_eu_806617F0), -1, NULL);
                    func_804D8C18(&sub);
                }
            }
            func_804F3258(0, &draw, quad, -1, NULL);
            func_804F3988(0);
        } else {
            if (lbl_eu_80665A80[0] == 0) {
                lbl_eu_806617F0[0] = lbl_eu_8066B4F4;
                lbl_eu_806617F0[1] = lbl_eu_8066B4F4;
                lbl_eu_806617F0[2] = lbl_eu_8066B4F4;
                lbl_eu_806617F0[3] = lbl_eu_8066B4F4;
                lbl_eu_80665A80[0] = 1;
            }
            if (inside) {
                func_804D8C68(&draw, 0, 0);
                func_804F3988(mode);
            } else if (quad->field_0xc >= lbl_eu_8066B4F4) {
                func_804F3988(mode);
                func_804D8C68(&draw, 0, 0);
                func_804F3258(0, &draw,
                              reinterpret_cast<const DrawQuad*>(lbl_eu_806617F0), -1, NULL);
                func_804D8C68(&draw, 0, 0);
                func_804F3988(0);
            } else {
                CDrawCtxLocal sub;
                func_804D8B38(&sub);
                if (func_804D8B4C(&sub, draw.field_0x00, NULL) != 0) {
                    func_804D8C68(&sub, 0, 0);
                    func_804F3988(mode);
                    func_804F3258(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(lbl_eu_806617F0), -1, NULL);
                    func_804D8C68(&draw, 0, 0);
                    func_804F3988(0);
                    func_804F3258(0, &sub,
                                  reinterpret_cast<const DrawQuad*>(lbl_eu_806617F0), -1, NULL);
                    func_804D8C18(&sub);
                }
            }
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804F3258(0, &draw, quad, 1,
                          reinterpret_cast<QuadTexCtx*>(mtxSrc));
            func_804F3988(0);
        }
        func_804D8C18(&draw);
    }
    func_804D8B30(desktop);
}

// Gradient-shaded screen-space quad emitter (called from func_804F2E44).
// Refreshes the projection via func_804F4628, configures the fixed-function
// pipeline and streams one full-screen quad strip straight into the main GX
// FIFO. mode < 0 draws an untextured gradient quad; mode >= 0 draws a
// textured quad whose second texcoord either uses the shared matrix (ctx ==
// NULL) or a transform rebuilt in place from ctx's source floats.
void func_804F3258(s32 texMap, void* drawCtx, const DrawQuad* quad, s16 mode,
                   QuadTexCtx* ctx) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    // Two distinct zero locals: MWCC keeps both live in f31/f29 across the
    // whole body and uses them for the register-sourced FIFO stores.
    const f32 zA = lbl_eu_8066B4F0;
    f32 fbW = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B500);
    const f32 zB = lbl_eu_8066B4F0;
    f32 fbH = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B500);
    f32 fbH2 = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B500);
    f32 fbW2 = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B500);
    func_804F4628(1, fbH2, fbW2);

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
        col.r = (u8)(s32)(quad->x * lbl_eu_8066B4F8);
        col.g = (u8)(s32)(quad->y * lbl_eu_8066B4F8);
        col.b = (u8)(s32)(quad->z * lbl_eu_8066B4F8);
        col.a = (u8)(s32)(quad->field_0xc * lbl_eu_8066B4F8);

        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* texMtx = func_804F42A0(1, NULL);
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
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0;
        f->f = fbW; f->f = zB;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F0;
        f->f = fbW; f->f = fbH;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F4;
        f->f = zA; f->f = fbH;
        f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0;
        f->f = zA; f->f = zB;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0;
    } else {
        // Textured path: tint color packed from the quad position.
        GXColor col;
        col.r = (u8)(s32)(quad->x * lbl_eu_8066B4F8);
        col.g = (u8)(s32)(quad->y * lbl_eu_8066B4F8);
        col.b = (u8)(s32)(quad->z * lbl_eu_8066B4F8);
        col.a = (u8)(s32)(quad->field_0xc * lbl_eu_8066B4F8);

        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        ml::CMat34* texMtx = func_804F42A0(1, NULL);
        GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        if (ctx == NULL) {
            // Second texcoord uses the shared identity-ish matrix.
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE, 0x7d);
        } else {
            // Rebuild the second texture matrix in place from ctx's source
            // floats: diagonal from 0x18/0x1c/0x20, translation from
            // 0x0c/0x10/0x14 added to zero.
            const f32 zc = lbl_eu_8066B4F0;
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
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0;
        f->f = fbW; f->f = zB;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F0;
        f->f = fbW; f->f = fbH;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F4;
        f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F4;
        f->f = zA; f->f = fbH;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F4;
        f->f = zA; f->f = zB;
        f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0;
        if (ctx != NULL) {
            // The custom-matrix variant emits a wider texcoord tail.
            f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F4;
            f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0; f->f = lbl_eu_8066B4F0;
            f->f = lbl_eu_8066B4F0;
        }
    }
}
void func_804F3258(s32 texMap, void* drawCtx, const void* quadData, s32 mode,
                   const void* mtxSrc) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    const DrawQuad* quad = static_cast<const DrawQuad*>(quadData);
    const f32 zero = lbl_eu_8066B4F0;

    GXRenderModeObj* rmode = getRenderModeObj__9CDeviceVIFv();
    f32 fbH = convU16ToF(rmode->efbHeight, lbl_eu_8066B500);
    f32 fbW = convU16ToF(rmode->fbWidth, lbl_eu_8066B500);
    func_804F4628(1, fbH, fbW);

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
        col.r = (u8)(s32)(quad->x * lbl_eu_8066B4F8);
        col.g = (u8)(s32)(quad->y * lbl_eu_8066B4F8);
        col.b = (u8)(s32)(quad->z * lbl_eu_8066B4F8);
        col.a = (u8)(s32)(quad->field_0xc * lbl_eu_8066B4F8);

        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* texMtx = func_804F42A0(1, NULL);
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
        f->f = fbH; f->f = zero; f->f = zero; f->f = lbl_eu_8066B4F4;
        // Vertex 2
        f->f = zero; f->f = fbW; f->f = zero; f->f = zero; f->f = lbl_eu_8066B4F4;
        f->f = fbH; f->f = fbW; f->f = zero; f->f = zero;
        // Vertex 3
        f->f = lbl_eu_8066B4F4; f->f = zero; f->f = zero; f->f = zero; f->f = zero;
        f->f = fbH; f->f = zero;
        // Vertex 4
        f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F4; f->f = zero; f->f = fbW;
        f->f = zero; f->f = fbH; f->f = zero;
        // Vertex 5
        f->f = lbl_eu_8066B4F4; f->f = zero; f->f = zero; f->f = zero; f->f = zero;
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
    f->f = lbl_eu_8066B4F4;
    f->f = zero; f->f = fbW;
    f->f = zero;
    f->f = lbl_eu_8066B4F4;
    f->f = fbH; f->f = fbW;
    f->f = lbl_eu_8066B4F4; f->f = lbl_eu_8066B4F4;
}

// Swap-mode tables indexed by mode: rows are swap table entries 0-3.
void func_804F3988(s32 mode) {
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

void func_804F3B4C(Mtx mtx, const void* scene) {
    func_80496120(scene, mtx, -1);
}

// Variant of func_804F4628 that writes the projection into a caller-supplied
// matrix instead of pushing it straight to GX. When `update` is set and the
// GX cache has flushed, the shared texture-scale table is recomputed from the
// cache view rectangle against the render mode dimensions.
void func_804F3B60(Mtx44 mtx, s32 update, f32 bottom, f32 top) {
    if (update != 0 && getAdjustFlag__8CGXCacheFv(cacheInstance__9CDeviceGX) != 0) {
        if (lbl_eu_80665A8A == 0) lbl_eu_80665A8A = 1;
        const CGXCacheView* view = static_cast<const CGXCacheView*>(
            getSubRectPtr__8CGXCacheFv(cacheInstance__9CDeviceGX));
        f32 fbW = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B518);
        f32 scaleX = fbW / convS32ToF(view->field_0x4 - view->field_0x0, lbl_eu_8066B520);
        f32 fbH = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B518);
        f32 scaleY = fbH / convS32ToF(view->field_0x6 - view->field_0x2, lbl_eu_8066B520);
        f32 ax = scaleX * (convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B518) /
                           convS32ToF(view->field_0x0, lbl_eu_8066B520));
        f32 ay = scaleY * (convS32ToF(view->field_0x2, lbl_eu_8066B520) /
                           convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B518));
        f32 nx = -ax;
        f32 ny = -ay;
        (void)nx; (void)ny;
        lbl_eu_80661840[0] = ax;
        lbl_eu_80661840[2] = scaleX;
        lbl_eu_80661840[3] = scaleY;
        lbl_eu_80661840[1] = ay;
    }
    C_MTXOrtho(mtx, lbl_eu_8066B508, bottom, lbl_eu_8066B508, top, lbl_eu_8066B508,
               lbl_eu_8066B50C);
}

// Cached texture-matrix builder: converts the GX cache viewport state into a
// 2x4 texture transform. Refreshed only when `update` is set and the GX cache
// has finished flushing; falls back to the identity matrix otherwise.
struct TexScaleParam {
    f32 x; // 0x00
    f32 y; // 0x04
};

ml::CMat34* func_804F42A0(int update, TexScaleParam* params) {
    if (lbl_eu_80665A89 == 0) lbl_eu_80665A89 = 1;
    if (lbl_eu_80665A88 == 0) {
        lbl_eu_80661800[0] = lbl_eu_8066B510;
        lbl_eu_80661800[1] = lbl_eu_8066B510;
        lbl_eu_80661800[2] = lbl_eu_8066B510;
        lbl_eu_80665A88 = 1;
    }
    if (params == NULL) params = reinterpret_cast<TexScaleParam*>(lbl_eu_80661800);

    if (update == 0) {
        ml::CMat34* m = reinterpret_cast<ml::CMat34*>(lbl_eu_80661810);
        *m = ml::CMat34::identity;
        return m;
    }
    if (getAdjustFlag__8CGXCacheFv(cacheInstance__9CDeviceGX) == 0) {
        ml::CMat34* m = reinterpret_cast<ml::CMat34*>(lbl_eu_80661810);
        *m = ml::CMat34::identity;
        return m;
    }
    if (lbl_eu_80665A8A == 0) lbl_eu_80665A8A = 1;
    const CGXCacheView* view =
        static_cast<const CGXCacheView*>(getSubRectPtr__8CGXCacheFv(cacheInstance__9CDeviceGX));

    // Texture scale factors derived from the GX cache viewport rect.
    f32 scaleX = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B518) /
                 convS32ToF(view->field_0x4 - view->field_0x0, lbl_eu_8066B520);
    f32 scaleY = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B518) /
                 convS32ToF(view->field_0x6 - view->field_0x2, lbl_eu_8066B520);
    f32 ax = scaleX *
             (convS32ToF(view->field_0x0, lbl_eu_8066B520) /
              convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B518));
    lbl_eu_80661840[0] = ax;
    f32 t = convS32ToF(view->field_0x2, lbl_eu_8066B520) /
            convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B518);
    f32 ay = scaleY * t;
    f32 ny = -ay;
    f32 nx = -ax;
    lbl_eu_80661840[2] = scaleX;
    lbl_eu_80661840[3] = scaleY;
    ml::CMat34* m = reinterpret_cast<ml::CMat34*>(lbl_eu_80661810);
    m->m[0][0] = scaleX;
    m->m[0][1] = lbl_eu_8066B508;
    m->m[0][2] = lbl_eu_8066B508;
    m->m[0][3] = lbl_eu_8066B508;
    lbl_eu_80661840[1] = ay;
    m->m[1][0] = lbl_eu_8066B508;
    m->m[1][1] = scaleY;
    m->m[1][2] = lbl_eu_8066B508;
    m->m[1][3] = lbl_eu_8066B508;
    m->m[2][0] = lbl_eu_8066B508;
    m->m[2][1] = lbl_eu_8066B508;
    m->m[2][2] = lbl_eu_8066B510;
    m->m[2][3] = lbl_eu_8066B508;
    m->m[2][3] = lbl_eu_8066B510;
    m->m[1][3] = lbl_eu_8066B508 + ny * params->y;
    m->m[0][3] = lbl_eu_8066B508 + nx * params->x;
    return m;
}

// Gradient-shaded textured quad emitter: clamps the direction vector into a
// unit band, then emits up to three gradient quads through func_804F06C4 -
// a background pair via a nested sub-context and the main quad directly.
void func_804F0258(void* desktop, DrawQuad* quad, ml::CVec3* dir, TexDrawSize* size,
                   void* material, TexMtxSrc* mtxSrc) {
    if (size->field_0x0c <= lbl_eu_8066B440) return;
    func_804D8B28(desktop);

    // Clamp the direction per axis: up to the unit vector first, then down
    // to the B444 floor (retail order).
    ml::CVec3 v = *dir;
    if (ml::CVec3::unit.x < v.x) v.x = ml::CVec3::unit.x;
    if (ml::CVec3::unit.y < v.y) v.y = ml::CVec3::unit.y;
    if (ml::CVec3::unit.z < v.z) v.z = ml::CVec3::unit.z;
    if (v.x < lbl_eu_8066B444) v.x = lbl_eu_8066B444;
    if (v.y < lbl_eu_8066B444) v.y = lbl_eu_8066B444;
    if (v.z < lbl_eu_8066B444) v.z = lbl_eu_8066B444;

    CDrawCtxLocal draw;
    func_804D8B38(&draw);
    if (func_804D8B4C(&draw, desktop, material) != 0) {
        if (!(mtxSrc != NULL && mtxSrc->mTex != NULL && mtxSrc->mIndex >= 0)) {
            // Unbound path.
            if (lbl_eu_8066B448 == v.x && lbl_eu_8066B448 == v.y) {
                func_804D8C68(&draw, 0, 0);
            } else {
                f32 hiU[3];
                f32 loU[3];
                CDrawCtxLocal subU;
                func_804D8B38(&subU);
                if (func_804D8B4C(&subU, draw.field_0x00, NULL) != 0) {
                    // Per-axis min/max band for the gradient falloff.
                    f32 t = v.x * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        loU[0] = t;
                        hiU[0] = lbl_eu_8066B44C;
                    } else {
                        loU[0] = v.x;
                        hiU[0] = lbl_eu_8066B448;
                    }
                    t = v.y * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        loU[1] = t;
                        hiU[1] = lbl_eu_8066B44C;
                    } else {
                        loU[1] = v.y;
                        hiU[1] = lbl_eu_8066B448;
                    }
                    loU[2] = v.z;
                    hiU[2] = lbl_eu_8066B448;

                    func_804D8C68(&subU, 0, 0);
                    func_804F06C4(0, &subU, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit, (const f32*)hiU,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    func_804D8C68(&draw, 0, loU);
                    func_804F06C4(0, &subU, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    func_804D8C18(&subU);
                }
            }
            func_804F06C4(0, &draw, reinterpret_cast<const ml::CVec3*>(quad),
                          (const f32*)&ml::CVec3::unit, (const f32*)&v,
                          reinterpret_cast<const f32*>(size), -1, 1,
                          (QuadTexCtx*)NULL, 1);
        } else {
            if (lbl_eu_8066B448 == v.x && lbl_eu_8066B448 == v.y) {
                func_804D8C68(&draw, 0, 0);
            } else {
                CDrawCtxLocal subB;
                f32 loB[3];
                f32 hiB[3];
                func_804D8B38(&subB);
                if (func_804D8B4C(&subB, draw.field_0x00, NULL) != 0) {
                    f32 t = v.x * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        loB[0] = t;
                        hiB[0] = lbl_eu_8066B44C;
                    } else {
                        loB[0] = v.x;
                        hiB[0] = lbl_eu_8066B448;
                    }
                    t = v.y * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        loB[1] = t;
                        hiB[1] = lbl_eu_8066B44C;
                    } else {
                        loB[1] = v.y;
                        hiB[1] = lbl_eu_8066B448;
                    }
                    loB[2] = v.z;
                    hiB[2] = lbl_eu_8066B448;

                    func_804D8C68(&subB, 0, 0);
                    func_804F06C4(0, &subB, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit, (const f32*)hiB,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    func_804D8C68(&draw, 0, loB);
                    func_804F06C4(0, &subB, (const ml::CVec3*)&ml::CVec3::zero,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CVec3::unit,
                                  (const f32*)&ml::CCol4::white, -1, 0,
                                  (QuadTexCtx*)NULL, 0);
                    func_804D8C18(&subB);
                }
            }
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804F06C4(0, &draw, reinterpret_cast<const ml::CVec3*>(quad),
                          (const f32*)&ml::CVec3::unit, (const f32*)&v,
                          reinterpret_cast<const f32*>(size), 1, 1,
                          reinterpret_cast<QuadTexCtx*>(mtxSrc), 1);
        }
        func_804D8C18(&draw);
    }
    func_804D8B30(desktop);
}


// Gradient-shaded textured quad emitter worker (called from func_804F0258).
// Packs the RGBA color floats into bytes, picks a z-compare mode from the
// draw flags, computes a perspective split via tan() on the depth-sorted
// path, then streams a 4-vertex gradient quad into the main FIFO.
//
// flag==0 or degenerate z -> unsorted path: position normalized against the
// render target and offset by gradDir; otherwise a perspective split point
// derived from the projection info and GX cache aspect ratio positions the
// quad. boundFlag<0 emits one texture layer; otherwise two (with an optional
// custom second matrix rebuilt in place inside mtxCtx).
void func_804F06C4(int texMap, CDrawCtxLocal* drawCtx, const ml::CVec3* pos,
                   const f32* gradDir, const f32* uvOrigin, const f32* color,
                   int boundFlag, int flag, QuadTexCtx* mtxCtx, u8 mode) {
    // Pack the four RGBA floats into bytes, staged through integer storage
    // (byte assemble -> word copy -> byte extract) as in retail.
    struct ByteColor {
        u8 b[4];
    };
    ByteColor in;
    in.b[0] = (u8)(s32)(color[0] * lbl_eu_8066B450);
    in.b[1] = (u8)(s32)(color[1] * lbl_eu_8066B450);
    in.b[2] = (u8)(s32)(color[2] * lbl_eu_8066B450);
    in.b[3] = (u8)(s32)(color[3] * lbl_eu_8066B450);
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

    if (flag != 0 && pos->z != lbl_eu_8066B440) {
        // Depth-sorted path.
        ProjInfo* pi = static_cast<ProjInfo*>(func_80496264(drawCtx->field_0x00, -1));
        f32 persp = lbl_eu_8066B454 * pi->field_0x1e0;
        if (pos->z == lbl_eu_8066B440 || lbl_eu_8066B440 < pos->z) {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            z = pos->z;
        } else {
            GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
            z = -pos->z;
        }
        func_804F45EC(drawCtx->field_0x00);
        f32 depthTan = z * (f32)tan(lbl_eu_8066B458 * persp / lbl_eu_8066B45C);

        // Aspect ratio of the GX cache viewport, scaled by device width scale.
        CGXCacheDims* gx = static_cast<CGXCacheDims*>(cacheInstance__9CDeviceGX);
        f32 aspect = lbl_eu_8066B460 *
                     (convS32ToF(gx->field_0x4bc, lbl_eu_8066B468) /
                      convS32ToF(gx->field_0x4be, lbl_eu_8066B468));
        aspect *= getWidthScale__9CDeviceVIFv();

        // Split position: x measured against height, y against width.
        f32 splitX = lbl_eu_8066B464 * depthTan * pos->x /
                     convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight,
                                lbl_eu_8066B470);
        f32 negDT = -depthTan;
        ax = aspect * (splitX - depthTan);
        bx = aspect * (splitX + depthTan);
        f32 splitY = lbl_eu_8066B464 * negDT * pos->y /
                     convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth,
                                lbl_eu_8066B470);
        z = -depthTan;
        ay = splitY + depthTan;
        by = splitY - depthTan;
    } else {
        // Unsorted path.
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        func_804F4628(mode, lbl_eu_8066B448, lbl_eu_8066B448);
        z = lbl_eu_8066B440;
        ax = pos->x;
        ay = pos->y;
        if (ax != lbl_eu_8066B440) {
            ax /= convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight,
                             lbl_eu_8066B470);
        }
        if (ay != lbl_eu_8066B440) {
            ay /= convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth,
                             lbl_eu_8066B470);
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
        ml::CMat34* tm = func_804F42A0(mode, (TexScaleParam*)uvOrigin);
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
        f->f = lbl_eu_8066B440; f->f = lbl_eu_8066B440;
        f->f = bx; f->f = ay; f->f = z; f->f = uv[0]; f->f = lbl_eu_8066B440;
        f->f = bx; f->f = by; f->f = z; f->f = uv[0]; f->f = uv[1];
        f->f = ax; f->f = by; f->f = z; f->f = lbl_eu_8066B440; f->f = uv[1];
    } else {
        // Two-texture path with an optional custom second matrix.
        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        ml::CMat34* tm = func_804F42A0(mode, (TexScaleParam*)uvOrigin);
        GXLoadTexMtxImm(tm->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        if (mtxCtx == NULL) {
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE, 0x7d);
        } else {
            // Rebuild the second texture matrix in place from the source
            // floats (diagonal at 0x18/0x1c/0x20, translation at 0x0c/0x10/0x14).
            mtxCtx->mtx[0][0] = mtxCtx->field_0x18;
            mtxCtx->mtx[0][1] = lbl_eu_8066B440;
            mtxCtx->mtx[0][2] = lbl_eu_8066B440;
            mtxCtx->mtx[0][3] = lbl_eu_8066B440 + mtxCtx->field_0x0c;
            mtxCtx->mtx[1][0] = lbl_eu_8066B440;
            mtxCtx->mtx[1][1] = mtxCtx->field_0x1c;
            mtxCtx->mtx[1][2] = lbl_eu_8066B440;
            mtxCtx->mtx[1][3] = lbl_eu_8066B440 + mtxCtx->field_0x10;
            mtxCtx->mtx[2][0] = lbl_eu_8066B440;
            mtxCtx->mtx[2][1] = lbl_eu_8066B440;
            mtxCtx->mtx[2][2] = mtxCtx->field_0x20;
            mtxCtx->mtx[2][3] = lbl_eu_8066B440 + mtxCtx->field_0x14;
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
        f->f = lbl_eu_8066B440; f->f = lbl_eu_8066B440;
        f->f = lbl_eu_8066B440; f->f = lbl_eu_8066B440;
        f->f = bx; f->f = ay; f->f = z; f->f = uv[0]; f->f = lbl_eu_8066B440;
        f->f = lbl_eu_8066B440; f->f = lbl_eu_8066B440;
        f->f = bx; f->f = by; f->f = z; f->f = uv[0]; f->f = uv[1];
        f->f = lbl_eu_8066B448; f->f = lbl_eu_8066B448;
        f->f = ax; f->f = by; f->f = z; f->f = lbl_eu_8066B440; f->f = uv[1];
        f->f = lbl_eu_8066B440; f->f = lbl_eu_8066B448;
    }
}

void func_804F45EC(const void* src) {
    Mtx44 m;
    func_80496120(src, m, -1);
    GXSetProjection(m, GX_PERSPECTIVE);
}

void func_804F4620(Mtx mtx){ GXSetProjection(mtx, GX_PERSPECTIVE); }

// Projection updater (see func_804F3B60): refreshes the texture-scale table
// when the cache view is valid, then builds and uploads an ortho projection
// box around +/- the given extents with 0.5 near/far planes.
void func_804F4628(s32 update, f32 bottom, f32 top) {
    Mtx44 m;
    // Retail keeps three separate C_MTXOrtho call sites (one per path).
    if (update == 0) {
        C_MTXOrtho(m, lbl_eu_8066B508, top, lbl_eu_8066B508, bottom, lbl_eu_8066B508,
                   lbl_eu_8066B50C);
    } else {
        if (getAdjustFlag__8CGXCacheFv(cacheInstance__9CDeviceGX) != 0) {
            if (lbl_eu_80665A8A == 0) lbl_eu_80665A8A = 1;
            const CGXCacheView* view = static_cast<const CGXCacheView*>(
                getSubRectPtr__8CGXCacheFv(cacheInstance__9CDeviceGX));
            // Texture scale from the cache view rect vs the render mode size.
            f32 fbW = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B518);
            f32 scaleX = fbW / convS32ToF(view->field_0x4 - view->field_0x0, lbl_eu_8066B520);
            f32 fbH = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B518);
            f32 scaleY = fbH / convS32ToF(view->field_0x6 - view->field_0x2, lbl_eu_8066B520);
            f32 ax = scaleX * (convS32ToF(view->field_0x0, lbl_eu_8066B520) /
                               convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B518));
            f32 ay = scaleY * (convS32ToF(view->field_0x2, lbl_eu_8066B520) /
                               convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B518));
            lbl_eu_80661840[0] = ax;
            lbl_eu_80661840[2] = scaleX;
            lbl_eu_80661840[3] = scaleY;
            lbl_eu_80661840[1] = ay;
            C_MTXOrtho(m, lbl_eu_8066B508, top, lbl_eu_8066B508, bottom, lbl_eu_8066B508,
                       lbl_eu_8066B50C);
        } else {
            C_MTXOrtho(m, lbl_eu_8066B508, top, lbl_eu_8066B508, bottom, lbl_eu_8066B508,
                       lbl_eu_8066B50C);
        }
    }
    GXSetProjection(m, GX_ORTHOGRAPHIC);
}

void func_804F4D74(Mtx mtx){ GXSetProjection(mtx, GX_ORTHOGRAPHIC); }

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
extern "C" u64 lbl_eu_80665A78 = 0;
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
DECOMP_FORCEACTIVE(f0258, lbl_eu_80665A68, lbl_eu_80665A6C, lbl_eu_80665A70,
    lbl_eu_80665A74, lbl_eu_80665A78, lbl_eu_80665A80, lbl_eu_80665A88,
    lbl_eu_80665A89, lbl_eu_80665A8A, lbl_eu_80665A8B_pad, lbl_eu_80665A8C_pad,
    lbl_eu_80665A8D_pad, lbl_eu_80665A8E_pad, lbl_eu_80665A8F_pad);


// Screen-aligned quad payload drawn by func_804F2E44 / func_804F0258.
struct DrawQuad {
    f32 x;         // 0x00
    f32 y;         // 0x04
    f32 z;         // 0x08
    f32 field_0xc; // 0x0c size-like
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
s32 func_804F10A0(s32, f32 alpha, void* draw, const ml::CVec3* pos, const TexDrawSize* size,
                  const f32* color, const void* clampInfo, s32 flag, const void* mtxSrc);
extern u32 lbl_eu_80665A08; // default clamp-info blob
extern const f32 lbl_eu_8066B478; // draw-epsilon

extern GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
extern void* cacheInstance__9CDeviceGX;
extern u32 func_8044BE24__8CGXCacheFv(void* cache);
extern void* func_8044BE1C__8CGXCacheFv(void* cache);
void func_804F213C(s32 texMap, void* drawCtx, const ml::CVec3* pos, const f32* rgba,
                   s32 boundFlag, const void* mtxSrc, f32 alpha, f32 alphaMax);
void func_804F3258(s32 texMap, void* drawCtx, const void* quadData, s32 mode,
                   const void* mtxSrc);
void func_804F3B60(Mtx44 mtx, s32 update, f32 bottom, f32 top);
void func_804F4628(s32 update, f32 bottom, f32 top);
void func_804F3988(s32 mode);
// Gradient-shaded textured quad emitter worker (called from func_804F0258).
// Defined below, after its helpers.
void func_804F06C4(int texMap, void* drawCtx, const ml::CVec3* pos, const void* dirA,
                   const void* dirB, const f32* color, int boundFlag, int flag,
                   int colorId, int mode);
struct TexScaleParam;
ml::CMat34* func_804F42A0(int update, TexScaleParam* params);
void func_804F45EC(const void* src);

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
extern const f32 lbl_eu_8066B4C8;
extern const f32 lbl_eu_8066B4CC;
extern const f32 lbl_eu_8066B4D0;
extern const f64 lbl_eu_8066B500; // 0x4330000000000000 (u->f magic)
extern const f32 lbl_eu_8066B508; // 0.5f
extern const f32 lbl_eu_8066B50C;
extern const f32 lbl_eu_8066B510; // 1.0f
extern const f64 lbl_eu_8066B518; // 0x4330000000000000 (u->f magic)
extern const f64 lbl_eu_8066B520; // 0x4330000080000000 (s->f magic)
extern const f32 lbl_eu_8066B47C; // F10A0 size-scale
extern const f32 lbl_eu_8066B480; // F10A0
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
            func_804F10A0(0, alpha, &draw, pos, size, color, clampInfo, -1, NULL);
        } else {
            func_804D8C68(&draw, 0, 0);
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804F10A0(0, alpha, &draw, pos, size, color, clampInfo, 1, mtxSrc);
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
void func_804F1B88(f32 scale, ml::CVec3* origin, ml::CVec3* extent, ml::CVec3* step,
                   s32 count, u8* colorBytes, CMarkerDistProvider* provider, s32 farFlag,
                   bool saveFirst);

// Marker/arrow draw driver (reconstruction draft v1 - structure per retail
// F10A0 call map; approximated points marked TODO).
struct TrailCtx {
    u8 pad_00[8];
    u8 bytes4;
    u8 pad_09[3];
    f32 dirs[12];
};

extern "C" s32 func_804F10A0(s32 texMap, f32 alpha, void* drawCtx,
                             const ml::CVec3* pos, const TexDrawSize* size,
                             const f32* color, const void* clampInfo,
                             s32 boundFlag, const void* mtxSrc) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    f32 fbH = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);

    // Retail +0xec..+0xf4: second rmode fetch feeds both the fbWidth read and
    // the scale block; all four size floats scaled by B484 as a group.
    GXRenderModeObj* rmode2 = getRenderModeObj__9CDeviceVIFv();
    const f32* szf = reinterpret_cast<const f32*>(size);
    f32 sc0 = lbl_eu_8066B484 * szf[0];
    f32 sc1 = lbl_eu_8066B484 * szf[1];
    f32 sc2 = lbl_eu_8066B484 * szf[2];
    f32 sc3 = lbl_eu_8066B484 * szf[3];
    u8 b0 = (u8)(s32)sc0;
    u8 b1 = (u8)(s32)sc1;
    u8 b2 = (u8)(s32)sc2;
    u8 b3 = (u8)(s32)sc3;
    f32 fbW = convU16ToF(rmode2->fbWidth, lbl_eu_8066B498);

    f32 fbH2 = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);

    func_804F4628(1, fbW, fbH2);

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);
    GXInitTexObjFilter(reinterpret_cast<GXTexObj*>(draw->field_0x04), GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(reinterpret_cast<GXTexObj*>(draw->field_0x04), (GXTexMapID)texMap);

    if (boundFlag == 0 && mtxSrc == NULL) {
        // Untextured single-marker path.
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        ml::CMat34* texMtx = func_804F42A0(1, NULL);
        GXLoadTexMtxImm(texMtx->m, 0x1e, GX_MTX_2x4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, GX_FALSE, 0x7d);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        u32 amb = f32bits10A0(lbl_eu_8066B490);
        GXSetChanAmbColor(GX_COLOR0A0, *reinterpret_cast<GXColor*>(&amb));
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, (GXTexMapID)texMap, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xB, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xB, GX_INDEX8);
        GXSetVtxDesc((GXAttr)0xD, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xE, GX_NONE);
    } else if (mtxSrc != NULL) {
        // Textured path A: marker quad written into the caller vertex buffer.
        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xB, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xE, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xB, GX_INDEX8);
        GXSetVtxDesc((GXAttr)0xD, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xE, GX_DIRECT);
        // TODO(draft): exact 125-basis TexCoordGen2 pair + vertex fill
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 125, GX_FALSE, 125);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 125, GX_FALSE, 125);
    } else {
        // Textured path B.
        GXSetNumTexGens(2);
        GXSetNumTevStages(2);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0,
                      GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 125, GX_FALSE, 125);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 125, GX_FALSE, 125);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xB, GX_CLR_RGBA, GX_RGB8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xE, GX_TEX_ST, GX_F32, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xB, GX_INDEX8);
        GXSetVtxDesc((GXAttr)0xD, GX_DIRECT);
        GXSetVtxDesc((GXAttr)0xE, GX_DIRECT);
    }

    // Common tail state.
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, (GXColorSrc)1, (GXColorSrc)0,
                  GX_LIGHT_NULL, GX_DF_NONE, (GXAttnFn)2);
    u32 amb2 = f32bits10A0(lbl_eu_8066B490);
    GXSetChanAmbColor(GX_COLOR0A0, *reinterpret_cast<GXColor*>(&amb2));
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, (GXTexMapID)texMap, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASA);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)9, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xB, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xE, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc((GXAttr)0xB, GX_INDEX8);
    GXSetVtxDesc((GXAttr)0xD, GX_DIRECT);
    GXSetVtxDesc((GXAttr)0xE, GX_DIRECT);

    // Gradient table lazy init (12 entries).
    if (lbl_eu_806617C0[0] == 0.0f) {
        // TODO(draft): exact constant pattern from retail +0x3a0 block
        for (int i = 0; i < 12; i++) {
            lbl_eu_806617C0[i] = (i % 3 == 1) ? 1.0f : 0.0f;
        }
    }

    // Four trail angles with wrap normalization (TODO(draft): exact sources).
    f32 ang[4];
    ang[0] = atan2(fbH2 - alpha, fbH - alpha);
    ang[1] = atan2(alpha - fbW, fbW - alpha);
    ang[2] = atan2(pos->y - fbH2, pos->x - fbH);
    ang[3] = atan2(size->field_0x0c - fbW, alpha - fbH);
    for (int i = 0; i < 4; i++) {
        while (ang[i] > lbl_eu_8066B494) {
            ang[i] -= lbl_eu_8066B494;
        }
        while (ang[i] < -lbl_eu_8066B494) {
            ang[i] += lbl_eu_8066B494;
        }
    }

    // Emit the four trails through the shared emitter.
    u8 col[4];
    col[0] = b0;
    col[1] = b1;
    col[2] = b2;
    col[3] = b3;
    func_804F1B88(1.0f, const_cast<ml::CVec3*>(pos), const_cast<ml::CVec3*>(pos),
                  const_cast<ml::CVec3*>(pos), 1, col, NULL, boundFlag != 0, false);
    func_804F1B88(1.0f, const_cast<ml::CVec3*>(pos), const_cast<ml::CVec3*>(pos),
                  const_cast<ml::CVec3*>(pos), 1, col, NULL, boundFlag != 0, false);
    func_804F1B88(1.0f, const_cast<ml::CVec3*>(pos), const_cast<ml::CVec3*>(pos),
                  const_cast<ml::CVec3*>(pos), 1, col, NULL, boundFlag != 0, false);
    func_804F1B88(1.0f, const_cast<ml::CVec3*>(pos), const_cast<ml::CVec3*>(pos),
                  const_cast<ml::CVec3*>(pos), 1, col, NULL, boundFlag != 0, false);
    return 1;
}

// Callback object polled while emitting marker vertices; slot vptr+0xc
// returns a per-marker distance factor.
class CMarkerDistProvider {
public:
    virtual ~CMarkerDistProvider();
    virtual void field_0x08();
    virtual f32 getDistFactor() = 0; // vptr + 0xc
};

// Alternate GX FIFO write window used by the direct vertex emitters here.
// 0xCC008000: MWCC encodes the store displacement as base 0xCC010000 with
// disp -0x8000 (s16 displacement range), matching retail lis rX, 0xcc01.
union FifoWord {
    f32 f;
    u8 b;
};
static volatile FifoWord* const s_altFifo = reinterpret_cast<volatile FifoWord*>(0xCC008000);

// Screen-space marker trail: walks from `origin` toward `extent` in `count`
// steps, normalizes each step offset against `step` and streams position /
// color / texcoord data straight into the alternate GX FIFO window.
void func_804F1B88(f32 scale, ml::CVec3* origin, ml::CVec3* extent, ml::CVec3* step,
                   s32 count, u8* colorBytes, CMarkerDistProvider* provider, s32 farFlag,
                   bool saveFirst) {
    // Screen-space basis: x is scaled against the framebuffer height and y
    // against the width (retail converts height first, then the loop count,
    // then reads/converts the width).
    f32 scrX = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);
    f32 fkCount = convS32ToF(count, lbl_eu_8066B4A0);
    GXRenderModeObj* rmode = getRenderModeObj__9CDeviceVIFv();

    ml::CVec3 dir = *extent - *origin;
    f32 w = convU16ToF(rmode->fbWidth, lbl_eu_8066B498);
    f32 k = lbl_eu_8066B488 / fkCount;
    ml::CVec3 inc = dir * k;
    ml::CVec3 cur = *origin;
    ml::CVec3 incStep = inc;

    for (s16 i = 0; i < count; i++) {
        ml::CVec3 diff = cur - *step;
        ml::CVec3 nrm = diff;
        ml::CVec3 work = nrm;
        f32 mag = PSVECMag(work);
        if (lbl_eu_8066B4A8 >= mag) {
            // Far path: fixed falloff factor times the distance factor.
            work *= lbl_eu_8066B4AC * scale * provider->getDistFactor();
        } else {
            // Near path: normalize the offset (degenerate -> zero vector),
            // then scale by the distance factor alone.
            if (work.x * work.x + work.y * work.y + work.z * work.z == lbl_eu_8066B478) {
                work = ml::CVec3::zero;
            } else {
                PSVECNormalize(work, work);
            }
            work *= scale * provider->getDistFactor();
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
        cur += incStep;
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
// func_804F4628, configures the fixed-function pipeline and streams one quad
// per gradient step straight into the main GX FIFO.
void func_804F213C(s32 texMap, void* drawCtx, const ml::CVec3* pos, const f32* rgba,
                   s32 boundFlag, const void* mtxSrc, f32 alpha, f32 alphaMax) {
    CDrawCtxLocal* draw = static_cast<CDrawCtxLocal*>(drawCtx);
    const f32 zero = lbl_eu_8066B4B0;

    f32 fbH = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);
    f32 ratio = alpha / alphaMax;
    f32 fbW = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B498);

    // Normalize the quad position against the framebuffer size.
    f32 px = pos->x;
    f32 py = pos->y;
    f32 kx = lbl_eu_8066B4C8 * fbH;
    f32 ky = lbl_eu_8066B4C8 * fbW;
    f32 tx = px / kx;
    f32 dx = (fbH - px) / kx;
    f32 dy = (fbW - py) / ky;
    f32 ty = py / ky;

    // Gradient color packed to bytes for the indexed color attribute.
    u8 cr = (u8)(s32)(rgba[0] * lbl_eu_8066B4CC);
    u8 cg = (u8)(s32)(rgba[1] * lbl_eu_8066B4CC);
    u8 cb = (u8)(s32)(rgba[2] * lbl_eu_8066B4CC);
    u8 ca = (u8)(s32)(rgba[3] * lbl_eu_8066B4CC);

    f32 f23 = px / fbH;
    f32 f22 = py / fbW;

    // Per-step offsets scaled by the fade ceiling.
    f32 sx0 = alphaMax * tx;
    f32 sx1 = alphaMax * dx;
    f32 sy0 = alphaMax * dy;
    f32 sy1 = alphaMax * ty;

    f32 vh = convU16ToF(getRenderModeObj__9CDeviceVIFv()->efbHeight, lbl_eu_8066B498);
    f32 vw = convU16ToF(getRenderModeObj__9CDeviceVIFv()->fbWidth, lbl_eu_8066B498);
    func_804F4628(1, vh, vw);

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);
    GXInitTexObjFilter(reinterpret_cast<GXTexObj*>(draw->field_0x04), GX_LINEAR, GX_LINEAR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, (GXBlendFactor)4, (GXBlendFactor)5, GX_LO_CLEAR);
    GXLoadTexObj(reinterpret_cast<GXTexObj*>(draw->field_0x04), (GXTexMapID)texMap);

    if (boundFlag < 0) {
        // Untextured gradient path.
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
    }

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    // Emit one quad per gradient step until the fade ratio is exhausted.
    volatile FifoWord* f = s_fifo;
    s32 i = 0;
    do {
        GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 4);
        f->f = zero; f->f = zero;
        f->b = cr; f->b = cg; f->b = cb; f->b = ca;
        f->f = lbl_eu_8066B4D0; f->f = lbl_eu_8066B4D0;
        f->f = f23 + sx0; f->f = zero;
        f->b = cr; f->b = cg; f->b = cb; f->b = ca;
        f->f = lbl_eu_8066B4D0; f->f = f22 + sy0;
        f->f = f23 + sx1; f->f = f22 + sy1;
        f->b = cr; f->b = cg; f->b = cb; f->b = ca;
        f->f = zero; f->f = f22;
        i++;
    } while ((f32)i < ratio);
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
                func_804F3258(0, &draw, lbl_eu_806617F0, -1, NULL);
                func_804D8C68(&draw, 0, 0);
                func_804F3988(0);
            } else {
                CDrawCtxLocal sub;
                func_804D8B38(&sub);
                if (func_804D8B4C(&sub, draw.field_0x00, NULL) != 0) {
                    func_804D8C68(&sub, 0, 0);
                    func_804F3988(mode);
                    func_804F3258(0, &sub, lbl_eu_806617F0, -1, NULL);
                    func_804D8C68(&draw, 0, 0);
                    func_804F3988(0);
                    func_804F3258(0, &sub, lbl_eu_806617F0, -1, NULL);
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
                func_804F3258(0, &draw, lbl_eu_806617F0, -1, NULL);
                func_804D8C68(&draw, 0, 0);
                func_804F3988(0);
            } else {
                CDrawCtxLocal sub;
                func_804D8B38(&sub);
                if (func_804D8B4C(&sub, draw.field_0x00, NULL) != 0) {
                    func_804D8C68(&sub, 0, 0);
                    func_804F3988(mode);
                    func_804F3258(0, &sub, lbl_eu_806617F0, -1, NULL);
                    func_804D8C68(&draw, 0, 0);
                    func_804F3988(0);
                    func_804F3258(0, &sub, lbl_eu_806617F0, -1, NULL);
                    func_804D8C18(&sub);
                }
            }
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804F3258(0, &draw, quad, 1, mtxSrc);
            func_804F3988(0);
        }
        func_804D8C18(&draw);
    }
    func_804D8B30(desktop);
}

// Gradient-shaded screen-space quad emitter (called from func_804F2E44).
// Refreshes the projection via func_804F4628, configures the fixed-function
// pipeline for a single textured/gradient quad and streams its vertices
// straight into the main GX FIFO window.
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
    if (update != 0 && func_8044BE24__8CGXCacheFv(cacheInstance__9CDeviceGX) != 0) {
        if (lbl_eu_80665A8A == 0) lbl_eu_80665A8A = 1;
        const CGXCacheView* view = static_cast<const CGXCacheView*>(
            func_8044BE1C__8CGXCacheFv(cacheInstance__9CDeviceGX));
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
    if (func_8044BE24__8CGXCacheFv(cacheInstance__9CDeviceGX) == 0) {
        ml::CMat34* m = reinterpret_cast<ml::CMat34*>(lbl_eu_80661810);
        *m = ml::CMat34::identity;
        return m;
    }
    if (lbl_eu_80665A8A == 0) lbl_eu_80665A8A = 1;
    const CGXCacheView* view =
        static_cast<const CGXCacheView*>(func_8044BE1C__8CGXCacheFv(cacheInstance__9CDeviceGX));

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
    const ml::CVec3& zero = ml::CVec3::zero;
    const ml::CVec3& unit = ml::CVec3::unit;
    const ml::CCol4& white = ml::CCol4::white;
    func_804D8B28(desktop);

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
        bool bound = mtxSrc != NULL && mtxSrc->mTex != NULL && mtxSrc->mIndex >= 0;
        if (!bound) {
            if (v.x == lbl_eu_8066B448 && v.y == lbl_eu_8066B448) {
                func_804D8C68(&draw, 0, 0);
            } else {
                CDrawCtxLocal sub;
                func_804D8B38(&sub);
                if (func_804D8B4C(&sub, draw.field_0x00, NULL) != 0) {
                    // Per-axis min/max band for the gradient falloff.
                    f32 lo[3];
                    f32 hi[3];
                    f32 t = v.x * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        lo[0] = t;
                        hi[0] = lbl_eu_8066B44C;
                    } else {
                        lo[0] = v.x;
                        hi[0] = lbl_eu_8066B448;
                    }
                    t = v.y * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        lo[1] = t;
                        hi[1] = lbl_eu_8066B44C;
                    } else {
                        lo[1] = v.y;
                        hi[1] = lbl_eu_8066B448;
                    }
                    lo[2] = v.z;
                    hi[2] = lbl_eu_8066B448;

                    func_804D8C68(&sub, 0, 0);
                    func_804F06C4(0, (void*)&sub, &zero, (const void*)lo,
                                  (const void*)hi, (const f32*)&white, -1, 0, 0, 0);
                    func_804D8C68(&draw, 0, lo);
                    func_804F06C4(0, (void*)&sub, &zero, (const void*)&unit,
                                  (const void*)&unit, (const f32*)&white, -1, 0, 0, 0);
                    func_804D8C18(&sub);
                }
            }
            func_804F06C4(0, (void*)&draw, reinterpret_cast<const ml::CVec3*>(quad),
                          (const void*)&v, (const void*)&unit,
                          reinterpret_cast<const f32*>(size), -1, 1, 0, 1);
        } else {
            if (v.x == lbl_eu_8066B448 && v.y == lbl_eu_8066B448) {
                func_804D8C68(&draw, 0, 0);
            } else {
                CDrawCtxLocal sub;
                func_804D8B38(&sub);
                if (func_804D8B4C(&sub, draw.field_0x00, NULL) != 0) {
                    f32 lo[3];
                    f32 hi[3];
                    f32 t = v.x * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        lo[0] = t;
                        hi[0] = lbl_eu_8066B44C;
                    } else {
                        lo[0] = v.x;
                        hi[0] = lbl_eu_8066B448;
                    }
                    t = v.y * lbl_eu_8066B44C;
                    if (t < lbl_eu_8066B448) {
                        lo[1] = t;
                        hi[1] = lbl_eu_8066B44C;
                    } else {
                        lo[1] = v.y;
                        hi[1] = lbl_eu_8066B448;
                    }
                    lo[2] = v.z;
                    hi[2] = lbl_eu_8066B448;

                    func_804D8C68(&sub, 0, 0);
                    func_804F06C4(0, (void*)&sub, &zero, (const void*)lo,
                                  (const void*)hi, (const f32*)&white, -1, 0, 0, 0);
                    func_804D8C68(&draw, 0, lo);
                    func_804F06C4(0, (void*)&sub, &zero, (const void*)&unit,
                                  (const void*)&unit, (const f32*)&white, -1, 0, 0, 0);
                    func_804D8C18(&sub);
                }
            }
            if (mtxSrc->mTex != NULL) {
                func_804DF164(mtxSrc->mTex, mtxSrc->mIndex, 1, mtxSrc->mField08);
            }
            func_804F06C4(0, (void*)&draw, reinterpret_cast<const ml::CVec3*>(quad),
                          (const void*)&v, (const void*)&unit,
                          reinterpret_cast<const f32*>(size), 1, 1, 0, 1);
        }
        func_804D8C18(&draw);
    }
    func_804D8B30(desktop);
}


// Gradient-shaded textured quad emitter worker (called from func_804F0258).
// Packs the RGBA color floats into bytes, picks a z-compare mode from the
// draw flags, computes a perspective split via tan() on the depth-sorted
// path, then streams a 4-vertex gradient quad into the main FIFO.
void func_804F06C4(int texMap, void* drawCtx, const ml::CVec3* pos, const void* dirA,
                   const void* dirB, const f32* color, int boundFlag, int flag,
                   int colorId, int mode) {
    // Pack the four RGBA floats into bytes.
    GXColor col;
    col.r = (u8)(s32)(color[0] * lbl_eu_8066B450);
    col.g = (u8)(s32)(color[1] * lbl_eu_8066B450);
    col.b = (u8)(s32)(color[2] * lbl_eu_8066B450);
    col.a = (u8)(s32)(color[3] * lbl_eu_8066B450);

    f32 depth;
    if (flag == 0 || pos->z == lbl_eu_8066B440) {
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        func_804F4628(colorId, lbl_eu_8066B448, lbl_eu_8066B448);
        depth = lbl_eu_8066B444;
    } else {
        // Depth-sorted path: pick a z-compare from the sign of the extent and
        // derive a perspective scale from the view angle.
        if (pos->z < lbl_eu_8066B440 && pos->z != lbl_eu_8066B440) {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            depth = pos->z;
        } else {
            GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
            depth = -pos->z;
        }
        func_804F45EC(*(const void**)dirA);
        f32 t = lbl_eu_8066B458 * depth / lbl_eu_8066B45C;
        tan(t);
    }

    Mtx mtxId;
    PSMTXIdentity(mtxId);
    GXLoadPosMtxImm(mtxId, 0);
    GXSetCurrentMtx(0);

    f32 lo0 = pos->x;
    f32 hi0 = pos->y;
    f32 lo1 = depth;
    f32 hi1 = depth;

    GXBegin((GXPrimitive)0xA0, GX_VTXFMT0, 4);
    volatile FifoWord* f = s_fifo;
    f->f = lo1; f->f = hi1; f->f = lo0; f->f = hi0; f->f = lo1; f->f = hi1;
    f->f = lbl_eu_8066B444; f->f = lbl_eu_8066B444;
    f->f = lo1; f->f = hi1; f->f = lo0; f->f = lbl_eu_8066B448;
    f->f = lbl_eu_8066B444; f->f = lbl_eu_8066B444;
    f->f = lo1; f->f = lbl_eu_8066B448; f->f = lo0; f->f = hi0;
    f->f = lbl_eu_8066B448; f->f = lbl_eu_8066B444;
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
    if (update != 0 && func_8044BE24__8CGXCacheFv(cacheInstance__9CDeviceGX) != 0) {
        if (lbl_eu_80665A8A == 0) lbl_eu_80665A8A = 1;
        const CGXCacheView* view = static_cast<const CGXCacheView*>(
            func_8044BE1C__8CGXCacheFv(cacheInstance__9CDeviceGX));
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
    C_MTXOrtho(m, lbl_eu_8066B508, bottom, lbl_eu_8066B508, top, lbl_eu_8066B508,
               lbl_eu_8066B50C);
    GXSetProjection(m, GX_PERSPECTIVE);
}

void func_804F4D74(Mtx mtx){ GXSetProjection(mtx, GX_ORTHOGRAPHIC); }

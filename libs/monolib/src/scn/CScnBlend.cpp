// CScnBlend full-screen color-blend filter effect.
// Inherits CScnFilter (scene filter base) and adds a blend-rect region plus
// GX blend/colour settings. Decompiled for monolib/src/scn/CScnBlend.

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>
#include <monolib/core/CView.hpp>
#include <monolib/device/CDeviceGX.hpp>
#include <monolib/math/CMat34.hpp>
#include "libs/monolib/src/scn/CScnBlend.hpp"

// Global operator delete -- MWCC mangles this to operator delete(defined in MemManager.cpp).
void operator delete(void* p) throw();

// Opaque render-work object (CScnTexWorkMan instance), only forwarded.
struct CTexWorkObj;
extern "C" GXTexObj* TexMan_AllocBuffer_0208(CTexWorkObj* obj, u16 w, u16 h, u32 fmt);
extern "C" void TexMan_ReleaseArg_02D8(CTexWorkObj* obj, GXTexObj* tex);
extern "C" void setupLightVtxFormat(int fmt, int mode);
extern "C" void setupLightTevColor(int a, int b, int c);
extern "C" void setupLightTevAlpha(int a, int b, int c);
extern "C" void TexObj_BlitViewRect(GXTexObj* tex, int p2, int p3);
struct RectRegion;
extern "C" void drawTexturedQuad(ml::CRect* rect, RectRegion* region,
                              ml::CCol4* col);
extern "C" void drawFlatQuad(ml::CRect* rect, RectRegion* region);
extern "C" void updateOrthoGX__8CGXCacheFv(CGXCache* cache);
extern "C" void updateCmd1Words__8CGXCacheFv(CGXCache* cache, u32 a, u32 b,
                                           u32 c, u32 d, u32 e);
extern "C" void setAlphaBlend__8CGXCacheFv(CGXCache* cache, u32 a, u32 b);

// Owning scene view as seen here: only field 0x6C (texture work manager) read.
struct CScnBlendView {
    u8 field_0x00[0x6C];
    CTexWorkObj* mTexWork;
};

// Retail __ct__CScnBlend: fills the default blend rect/tint region and the
// blend-mode fields; the compiler emits the base ctor call and the vptr store.
CScnBlend::CScnBlend() {
    // First 0-store is consumed by the flags OR (store-to-load forward); the
    // second (redundant) store keeps the physical stw at the end of the block.
    field_0x68 = 0;

    RectRegion tmp;
    tmp.x = 1.0f;
    tmp.y = 1.0f;
    tmp.w = 1.0f;
    tmp.h = 0.5f;
    tmp.tR = 0.0f;
    tmp.tG = 0.0f;
    tmp.tB = 0.0f;
    mRect = tmp;

    mFlags = (u8)((u32)field_0x68 | 3);
    field_0x58 = 4;
    field_0x5C = 1;
    field_0x60 = 4;
    field_0x64 = 1;
    field_0x68 = 0;
}

// MWCC destructors return the object pointer (see trailing mr r3, r30 in retail).
CScnBlend::~CScnBlend() {
}

extern "C" void func_80498D98(CScnBlend* blend, int enable) {
    if (enable != 0) {
        blend->mFlags |= 1;
    } else {
        blend->mFlags &= 0xFE;
    }
}

extern "C" void Blend_SetFlag2(CScnBlend* ths, u32 enable) {
    if (enable != 0) {
        ths->mFlags |= 2;
    } else {
        ths->mFlags &= ~2;
    }
}


// ============================================================================
// Blend_DrawFilter - full-screen blend filter draw (CScnBlend vtable slot).
// Skips when the region height (= tint alpha) matches the pool constant.
// With flag bit0 clear the quad is drawn straight into the frame; with bit0
// set the current view is first rendered into a texture (halved when bit1 is
// set) and drawn back with a TEV stage modulated by the negated tint color.
// ============================================================================
void CScnBlend::drawFilter(CScnFilterHost* param) {
    CScnBlend* ths = this;
    CScnBlendView* view = reinterpret_cast<CScnBlendView*>(param);
    ml::CRect rect;
    CView::insetRectToView(rect, CView::getCurrentView());

    if (ths->mRect.h) {
        GXLoadPosMtxImm(ml::CMat34::identity.m, 0);
        GXSetCurrentMtx(0);
        updateOrthoGX__8CGXCacheFv(CDeviceGX::getCacheInstance());
        CDeviceGX::getCacheInstance()->setZCompareMD(0, 0);
        CDeviceGX::getCacheInstance()->setZWriteMode(0, 0);
        setAlphaBlend__8CGXCacheFv(CDeviceGX::getCacheInstance(), 0, 0);

        if (!(ths->mFlags & 1)) {
            // Direct path: no texture, no texture matrix.
            setupLightVtxFormat(0, 0);
            updateCmd1Words__8CGXCacheFv(CDeviceGX::getCacheInstance(),
                                       ths->field_0x5C,
                                       ths->field_0x60, ths->field_0x64,
                                       ths->field_0x68, 0);
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                          GX_COLOR0A0);
            if (ths->field_0x5C != 3) {
                setupLightTevColor(0, 0, 0);
                setupLightTevAlpha(0, 0, 0);
            } else {
                setupLightTevColor(0, 3, 0);
                setupLightTevAlpha(0, 0, 0);
            }
            drawFlatQuad(&rect, &ths->mRect);
        } else {
            // Texture path: render the view into a work texture first.
            CTexWorkObj* texWork = view->mTexWork;
            u16 tw;
            u16 th;
            if (ths->mFlags & 2) {
                // Halve the rect size (signed divide by two).
                s16 w = rect.mSize.x;
                s16 h = rect.mSize.y;
                tw = (u16)(w / 2);
                th = (u16)(h / 2);
            } else {
                tw = (u16)rect.mSize.x;
                th = (u16)rect.mSize.y;
            }
            GXTexObj* tex = TexMan_AllocBuffer_0208(texWork, tw, th, ths->field_0x58);
            if (tex == NULL) {
                // Allocation failure: retail exits without the cache restore.
                return;
            }
            TexObj_BlitViewRect(tex, (ths->mFlags >> 1) & 1, 0);
            setupLightVtxFormat(0, 1);
            updateCmd1Words__8CGXCacheFv(CDeviceGX::getCacheInstance(),
                                       ths->field_0x5C,
                                       ths->field_0x60,
                                       ths->field_0x64,
                                       ths->field_0x68, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
                              GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
            if (ths->field_0x5C != 3) {
                // Single TEV stage: raster color * texture * (-tint*255)
                // held in TEV register 3.
                GXColorS10 tevColor = {0, 0, 0, 0};
                tevColor.r = (s16)(255.0f * -ths->mRect.tR);
                tevColor.g = (s16)(255.0f * -ths->mRect.tG);
                tevColor.b = (s16)(255.0f * -ths->mRect.tB);
                GXSetTevColorS10((GXTevRegID)3, tevColor);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0,
                              GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
                                GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC,
                                GX_CC_TEXC, GX_CC_C2);
                setupLightTevAlpha(0, 0, 0);
            } else {
                GXSetNumTevStages(2);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0,
                              GX_COLOR0A0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0,
                              GX_COLOR0A0);
                setupLightTevColor(0, 2, 1);
                setupLightTevColor(1, 4, 0);
                setupLightTevAlpha(0, 0, 0);
                setupLightTevAlpha(1, 0, 0);
            }
            GXInitTexObjFilter(tex, GX_LINEAR, GX_LINEAR);
            GXLoadTexObj(tex, GX_TEXMAP0);
            ml::CCol4 blendColor(0.0f, 0.0f,
                                 1.0f, 1.0f);
            drawTexturedQuad(&rect, &ths->mRect, &blendColor);
            TexMan_ReleaseArg_02D8(texWork, tex);
        }

        CDeviceGX::getCacheInstance()->setZCompareMD(1, 0);
        CDeviceGX::getCacheInstance()->setZWriteMode(1, 0);
        setAlphaBlend__8CGXCacheFv(CDeviceGX::getCacheInstance(), 1, 0);
    }
}

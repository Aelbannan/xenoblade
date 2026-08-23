// CScnBloom - Bloom post-processing effect.
// The bloom object is created by CScnRoot (0x94 bytes): base params at 0x00
// (init by func_80499718 via func_804996E8), view pointer at 0x48, then
// per-frame render state (func_80499FD0 is the second-stage constructor).

#include "libs/monolib/src/scn/CScnBloom.hpp"
#include "monolib/scn/CScnFilter.hpp"
extern "C" void __ct__CScnFilter(void*);
#include "monolib/core/CView.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/math/CMat34.hpp"
#include <harness_catalog.h>
#include <stdint.h>
#include <new>
#include <string.h>

// Imports not yet declared in their owning headers (retail C-linkage names).
extern "C" {
// Deleting destructor (retail __dt__804997D0): free self when mode > 0,
// return self.
extern "C" __declspec(noinline) void* __dt__804997D0(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self != 0 && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}
void func_8043E928__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect, CView* view);
void func_80490314(CTexWorkObj* obj);
GXTexObj* func_80490208(CTexWorkObj* obj, u16 w, u16 h, u32 fmt);
void func_804902D8(CTexWorkObj* obj, GXTexObj* tex);
void func_804943E0(GXTexObj* tex, int p2, int p3);
void __dt__10CScnFilterFv(CScnFilter* self, s32 flags);
void func_804944DC(GXTexObj* tex, ml::CRect* rect, u8 p3, u8 p4);
u32 func_80499810(CScnBloom* self, CTexWorkObj* work, GXTexObj** bufs,
                  int maxBufs, ml::CRect* rect, u32 flag);
void func_80494A64(int a, int b, int c);
void func_80494C30(int a, int b, int c);
void func_804948F4(int a, int b);
void func_80494F10(ml::CRect* rect, ml::CCol4* p2, ml::CCol4* p3);
void func_8044A578__8CGXCacheFv(CGXCache* cache, ml::CCol4* color, int flag);
void func_8044A7F8__8CGXCacheFv(CGXCache* cache, u32 a, u32 b, u32 c, u32 d, u32 e);
void func_8044B5C0__8CGXCacheFv(CGXCache* cache);
void func_8044ABAC__8CGXCacheFv(CGXCache* cache, u32 a, u32 b);
}

// Extern declarations for functions not yet decompiled
extern "C" {
void func_8049928C(CScnBloom* self, u32 param);
void* __dt__9CScnBloomFv(CScnBloom* self, s32 flags);
CScnBloom* func_804996E8(CScnBloom* self);
void func_80499718(CScnBloom* self);
void func_8049A128(CScnBloom* self);
void func_8049A850(CScnBloom* self, GXTexObj* tex, ml::CRect* rect);
void func_8049AA74(CScnBloom* self);
}

// Retail data labels referenced by the bloom vtable/params (foreign pool).
extern "C" u32 lbl_eu_806639F8;
extern "C" u32 lbl_eu_806639F0;
extern "C" u32 lbl_eu_80663A00;
// Foreign function referenced by the dispatch table.
extern "C" void func_8049C868();
// Forward declarations for in-TU extern "C" functions referenced below.
extern "C" void func_804996E0(CScnBloom* self, s32 flags);
extern "C" void func_804996D8(CScnBloom* self, u32 param);

// === .data size=0x38 align=8 ===
// CScnBloom dispatch table (36B): typeinfo locators + virtual thunks.
extern "C" u32 lbl_eu_8056EA08[9] = {
    (u32)&lbl_eu_806639F8, 0x00000000, (u32)&lbl_eu_806639F8, 0xFFFFFFB8,
    (u32)&func_804996E0, (u32)&func_8049C868, (u32)&func_804996D8,
    (u32)&__dt__9CScnBloomFv, (u32)&func_8049928C,
};
// Bloom param block (20B).
extern "C" u32 lbl_eu_8056EA2C[5] = {
    (u32)&lbl_eu_806639F0, 0x00000048, (u32)&lbl_eu_80663A00, 0x00000000, 0x00000000,
};

// ============================================================================
// FULL_MATCH: us-8049d7bc (func_804996D8)
// Virtual thunk for CScnFilter::renderBloom override.
// When called through the CScnFilter vtable, 'this' points to the
// CScnFilter subobject at offset 0x48 from the CScnBloom base.
// This thunk adjusts 'this' back to the CScnBloom base and tail-calls
// the implementation (func_8049928C).
// ============================================================================
extern "C" void func_804996D8(CScnBloom* self, u32 param) {
    // Adjust this from CScnFilter subobject (+0x48) to CScnBloom base
    CScnBloom* base = reinterpret_cast<CScnBloom*>(
        reinterpret_cast<uintptr_t>(self) - 0x48
    );
    // Tail-call to implementation
    typedef void (*RenderFn)(CScnBloom*, u32);
    RenderFn render = reinterpret_cast<RenderFn>(&func_8049928C);
    render(base, param);
}

// ============================================================================
// FULL_MATCH: us-8049d7c4 (func_804996E0)
// Virtual thunk for CScnFilter::destroyBloom override.
// When called through the CScnFilter vtable, 'this' points to the
// CScnFilter subobject at offset 0x48 from the CScnBloom base.
// This thunk adjusts 'this' back to the CScnBloom base and tail-calls
// the CScnBloom destructor.
// ============================================================================
extern "C" void func_804996E0(CScnBloom* self, s32 flags) {
    // Adjust this from CScnFilter subobject (+0x48) to CScnBloom base
    CScnBloom* base = reinterpret_cast<CScnBloom*>(
        reinterpret_cast<uintptr_t>(self) - 0x48
    );
    // Tail-call to destructor
    typedef void (*DtorFn)(CScnBloom*, s32);
    DtorFn dtor = reinterpret_cast<DtorFn>(&__dt__9CScnBloomFv);
    dtor(base, flags);
}

// ============================================================================
// func_804996E8 (0x8049D7CC) - base-object initializer wrapper.
// Calls func_80499718 to fill the CScnBloomBase params (0x00-0x47) and returns
// the object (r3 is preserved across the call, so the result is self).
// dont_inline: retail callers (__ct__CScnBloom, func_80499FD0) issue a real
// `bl func_804996E8`; -inline auto would fold this wrapper into the call site.
// ============================================================================
#pragma dont_inline on
extern "C" CScnBloom* func_804996E8(CScnBloom* self) {
    func_80499718(self);
    return self;
}
#pragma dont_inline off

// ============================================================================
// func_80499718 (0x8049D7FC) - initialize the CScnBloomBase params.
// Fills the color (1.0, 1.0, 1.0, 0.5), blend/filter modes, pass count, blur
// samples and viewport divisors at 0x00-0x47.
// ============================================================================
extern "C" void func_80499718(CScnBloom* self) {
    ml::CCol4 color(lbl_eu_8066AB00, lbl_eu_8066AB00, lbl_eu_8066AB00, lbl_eu_8066AB04);
    self->mColor = color;
    self->mBlendMode = 0x10;
    self->mFilterType = 0xeb;
    self->mTexFormat = 6;
    self->mUnk18 = 1;
    self->mUnk1C = 1;
    self->mEnabled = 1;
    self->mRenderMode = 3;
    self->mThreshold = lbl_eu_8066AB08;
    self->mBlurSamples = 4;
    self->mUnk2C = 1;
    self->mUnk30 = 1;
    self->mUnk34 = 1;
    self->mUnk38 = 4;
    self->mUnk3C = 1;
    self->mUnk40 = 0;
    self->mUnk44 = 8;
}

// ============================================================================
// func_80499FD0 (0x8049E0B4) - second-stage bloom constructor (CScnRoot).
// Inits the base params, stores the owning view, default blur colors/scales
// and viewport, then overrides the color with CCol4::white. Returns the object.
// ============================================================================
extern "C" CScnBloom* func_80499FD0(CScnBloom* self, CView* view) {
    func_804996E8(self);
    self->mView = view;
    self->mTexture = 0;
    self->mUnk50 = 1;
    self->mFlags = (u8)((u32)self->mTexture | 1);
    ml::CCol4 blurColor(lbl_eu_8066AB28, lbl_eu_8066AB28, lbl_eu_8066AB28, lbl_eu_8066AB2C);
    ml::CVec3 blurScale(lbl_eu_8066AB30, lbl_eu_8066AB30, lbl_eu_8066AB30);
    self->mBlurColor = blurColor;
    self->mBlurScale = blurScale;
    self->mViewportX = 1;
    self->mViewportY = 4;
    self->mViewportW = 1;
    self->mViewportH = 0;
    self->mColor = ml::CCol4::white;
    return self;
}

// ============================================================================
// func_8049A128 (0x8049E20C) - bloom render at full screen-res texture.
// Clamps the current view rect, creates the screen texture (format from
// mTexFormat), copies the cache clear color and pushes the texture.
// ============================================================================
extern "C" void func_8049A128(CScnBloom* self) {
    ml::CRect rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(&rect, CView::getCurrentView());

    GXRenderModeObj* rm = CDeviceVI::getRenderModeObj();
    s16 x = rect.mPos.x;
    s16 maxX = (s16)(rm->fbWidth - 1);
    if (x < 0) {
        rect.mPos.x = 0;
    } else if (x > maxX) {
        rect.mPos.x = maxX;
    }
    rm = CDeviceVI::getRenderModeObj();
    s16 y = rect.mPos.y;
    s16 maxY = (s16)(rm->efbHeight - 1);
    if (y < 0) {
        rect.mPos.y = 0;
    } else if (y > maxY) {
        rect.mPos.y = maxY;
    }
    if (rect.mPos.x + rect.mSize.x > CDeviceVI::getRenderModeObj()->fbWidth) {
        rect.mSize.x = CDeviceVI::getRenderModeObj()->fbWidth - rect.mPos.x;
    }
    if (rect.mPos.y + rect.mSize.y > CDeviceVI::getRenderModeObj()->efbHeight) {
        rect.mSize.y = CDeviceVI::getRenderModeObj()->efbHeight - rect.mPos.y;
    }
    if (rect.mPos.x & 1) {
        rect.mPos.x += 1;
        rect.mSize.x -= 1;
    }
    if (rect.mPos.y & 1) {
        rect.mPos.y += 1;
        rect.mSize.y -= 1;
    }
    if (rect.mSize.x & 1) {
        rect.mSize.x -= 1;
    }
    if (rect.mSize.y & 1) {
        rect.mSize.y -= 1;
    }

    CTexWorkObj* texWork = ((CScnBloomView*)self->mView)->mTexWork;
    func_80490314(texWork);
    self->mTexture = func_80490208(texWork, (u16)rect.mSize.x, (u16)rect.mSize.y, self->mTexFormat);
    if (self->mTexture != 0) {
        ml::CCol4* cacheColor = CDeviceGX::getCacheInstance()->func_8044B5B4();
        self->mClearColor = *cacheColor;
        func_8044A578__8CGXCacheFv(CDeviceGX::getCacheInstance(), &ml::CCol4::black, 0);
        func_804944DC(self->mTexture, &rect, 0, 1);
    }
}

// ============================================================================
// func_8049A850 (0x8049E934) - configure TEV stages and draw the bloom quad.
// The mUnk34==3 path uses two TEV stages (gaussian), otherwise a single
// constant-color stage. The blur scale (mBlurScale, negated * 255) feeds the
// TEV color; the blur tint color is passed to func_80494F10.
// ============================================================================
extern "C" void func_8049A850(CScnBloom* self, GXTexObj* tex, ml::CRect* rect) {
    CGXCache* cache = CDeviceGX::getCacheInstance();
    func_8044A7F8__8CGXCacheFv(cache, self->mViewportX, self->mViewportY, self->mViewportW, self->mViewportH, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    if (self->mUnk34 != 3) {
        GXColorS10 tevColor = {0, 0, 0, 0};
        tevColor.r = (s16)(lbl_eu_8066AB40 * -self->mBlurScale.x);
        tevColor.g = (s16)(lbl_eu_8066AB40 * -self->mBlurScale.y);
        tevColor.b = (s16)(lbl_eu_8066AB40 * -self->mBlurScale.z);
        GXSetTevColorS10((GXTevRegID)3, tevColor);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_C2);
        func_80494C30(0, 0, 0);
    } else {
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        func_80494A64(0, 2, 1);
        func_80494A64(1, 4, 0);
        func_80494C30(0, 0, 0);
        func_80494C30(1, 0, 0);
    }
    GXInitTexObjFilter(tex, GX_LINEAR, GX_LINEAR);
    GXLoadTexObj(tex, GX_TEXMAP0);
    ml::CCol4 blendColor(lbl_eu_8066AB30, lbl_eu_8066AB30, lbl_eu_8066AB28, lbl_eu_8066AB28);
    func_80494F10(rect, &self->mBlurColor, &blendColor);
}

// ============================================================================
// func_8049AA74 (0x8049EB58) - bloom render at half screen-res texture.
// Same rect clamp as func_8049A128 but halves the texture size, resets the
// GX cache state, loads the identity matrix and draws via func_8049A850.
// ============================================================================
extern "C" void func_8049AA74(CScnBloom* self) {
    ml::CRect rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(&rect, CView::getCurrentView());

    GXRenderModeObj* rm = CDeviceVI::getRenderModeObj();
    s16 x = rect.mPos.x;
    s16 maxX = (s16)(rm->fbWidth - 1);
    if (x < 0) {
        rect.mPos.x = 0;
    } else if (x > maxX) {
        rect.mPos.x = maxX;
    }
    rm = CDeviceVI::getRenderModeObj();
    s16 y = rect.mPos.y;
    s16 maxY = (s16)(rm->efbHeight - 1);
    if (y < 0) {
        rect.mPos.y = 0;
    } else if (y > maxY) {
        rect.mPos.y = maxY;
    }
    if (rect.mPos.x + rect.mSize.x > CDeviceVI::getRenderModeObj()->fbWidth) {
        rect.mSize.x = CDeviceVI::getRenderModeObj()->fbWidth - rect.mPos.x;
    }
    if (rect.mPos.y + rect.mSize.y > CDeviceVI::getRenderModeObj()->efbHeight) {
        rect.mSize.y = CDeviceVI::getRenderModeObj()->efbHeight - rect.mPos.y;
    }
    if (rect.mPos.x & 1) {
        rect.mPos.x += 1;
        rect.mSize.x -= 1;
    }
    if (rect.mPos.y & 1) {
        rect.mPos.y += 1;
        rect.mSize.y -= 1;
    }
    if (rect.mSize.x & 1) {
        rect.mSize.x -= 1;
    }
    if (rect.mSize.y & 1) {
        rect.mSize.y -= 1;
    }

    CTexWorkObj* texWork = ((CScnBloomView*)self->mView)->mTexWork;
    u32 w = (u32)rect.mSize.x;
    u32 h = (u32)rect.mSize.y;
    u32 fmt = self->mTexFormat;
    GXTexObj* tex = func_80490208(texWork, (u16)((w + (w >> 31)) >> 1), (u16)((h + (h >> 31)) >> 1), fmt);
    if (tex != 0) {
        func_804944DC(tex, &rect, 1, 0);
        CDeviceGX::getCacheInstance()->func_8044BE38();
        func_8044B5C0__8CGXCacheFv(CDeviceGX::getCacheInstance());
        CDeviceGX::getCacheInstance()->func_8044A94C(0, 0);
        CDeviceGX::getCacheInstance()->func_8044AA7C(0, 0);
        func_8044ABAC__8CGXCacheFv(CDeviceGX::getCacheInstance(), 0, 0);
        GXLoadPosMtxImm(ml::CMat34::identity.m, 0);
        GXSetCurrentMtx(0);
        func_804948F4(0, 1);
        func_8049A850(self, tex, &rect);
        func_804902D8(texWork, tex);
    }
}

// Stub functions for other symbols in this TU (not yet decompiled)
extern "C" void func_804996D8_stub() {}

extern "C" void func_804996D8_stub2() {}

extern "C" void func_804996D8_stub3() {}

extern "C" void func_804996D8_stub4() {}

extern "C" void func_804996D8_stub5() {}

extern "C" void func_804996D8_stub6() {}

extern "C" void func_804996D8_stub7() {}

extern "C" void func_804996D8_stub8() {}

// Delete-flag dtor: tear down the base (+0xC), free when the flag is
// positive, and return self (retail stmw r30 frame).
extern "C" void* __dt__804997D0(void*, int);
extern "C" void* __dl__FPv(void*);
extern "C" void* __dt__8049A0D0(void* self, int flag) {
    if (self) {
        __dt__804997D0(self, 0);
        if (flag > 0)
            __dl__FPv(self);
    }
    return self;
}

// ============================================================================
// func_8049A318 (0x8049A318) - bloom render using the cached screen texture
// (mTexture). Clamps/halves the view rect, re-renders it into tiles, extracts
// bright-pass textures via func_80499810, blurs them back, then clears the
// cached texture and restores the cache background color.
// ============================================================================
extern "C" void func_8049A318(CScnBloom* self) {
    CView* view = CView::getCurrentView();
    if (self->mTexture == NULL)
        return;
    CDeviceGX::getCacheInstance()->func_8044BE38();

    ml::CRect rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(&rect, view);

    GXRenderModeObj* rm = CDeviceVI::getRenderModeObj();
    s16 x = rect.mPos.x;
    s16 maxX = (s16)(rm->fbWidth - 1);
    if (x < 0) {
        rect.mPos.x = 0;
    } else if (x > maxX) {
        rect.mPos.x = maxX;
    }
    rm = CDeviceVI::getRenderModeObj();
    s16 y = rect.mPos.y;
    s16 maxY = (s16)(rm->efbHeight - 1);
    if (y < 0) {
        rect.mPos.y = 0;
    } else if (y > maxY) {
        rect.mPos.y = maxY;
    }
    if (rect.mPos.x + rect.mSize.x > CDeviceVI::getRenderModeObj()->fbWidth) {
        rect.mSize.x = CDeviceVI::getRenderModeObj()->fbWidth - rect.mPos.x;
    }
    if (rect.mPos.y + rect.mSize.y > CDeviceVI::getRenderModeObj()->efbHeight) {
        rect.mSize.y = CDeviceVI::getRenderModeObj()->efbHeight - rect.mPos.y;
    }
    if (rect.mPos.x & 1) {
        rect.mPos.x += 1;
        rect.mSize.x -= 1;
    }
    if (rect.mPos.y & 1) {
        rect.mPos.y += 1;
        rect.mSize.y -= 1;
    }
    if (rect.mSize.x & 1) {
        rect.mSize.x -= 1;
    }
    if (rect.mSize.y & 1) {
        rect.mSize.y -= 1;
    }

    CTexWorkObj* texWork = ((CScnBloomView*)self->mView)->mTexWork;
    ml::CCol4 col2(lbl_eu_8066AB28, lbl_eu_8066AB28, lbl_eu_8066AB28, lbl_eu_8066AB28);
    ml::CCol4 col3(lbl_eu_8066AB30, lbl_eu_8066AB30, lbl_eu_8066AB28, lbl_eu_8066AB28);

    // Tile counts: viewport / mUnk44 rounded up to even, as in func_8049928C.
    s32 div = self->mUnk44;
    u16 tileW = rect.mSize.x / div;
    if (tileW & 1)
        tileW += 1;
    u16 tileH = rect.mSize.y / div;
    if (tileH & 1)
        tileH += 1;

    // Half-resolution screen texture (format 4).
    u32 w = (u32)rect.mSize.x;
    u32 h = (u32)rect.mSize.y;
    GXTexObj* tex = func_80490208(texWork, (u16)((w + (w >> 31)) >> 1),
                                  (u16)((h + (h >> 31)) >> 1), 4);
    if (tex == NULL)
        return;

    CGXCache* cache = CDeviceGX::getCacheInstance();
    GXInitTexObjFilter(tex, (GXTexFilter)1, (GXTexFilter)1);
    func_804944DC(tex, &rect, 1, 0);
    func_804948F4(0, 1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE,
                      GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    func_80494A64(0, 2, 0);
    func_80494C30(0, 0, 0);
    func_8044B5C0__8CGXCacheFv(cache);
    cache->func_8044A94C(0, 0);
    cache->func_8044AA7C(0, 0);
    func_8044ABAC__8CGXCacheFv(cache, 0, 0);
    func_8044A7F8__8CGXCacheFv(cache, 0, 4, 5, 0, 0);
    GXLoadPosMtxImm(ml::CMat34::identity.m, 0);
    GXSetCurrentMtx(0);
    GXLoadTexObj(tex, GX_TEXMAP0);

    ml::CRect rect2;
    rect2.mPos = rect.mPos;
    rect2.mSize.x = tileW;
    rect2.mSize.y = tileH;
    func_80494F10(&rect2, &col2, &col3);
    GXPixModeSync();
    func_804902D8(texWork, tex);

    GXTexObj* bufs[10];
    memset(bufs, 0, sizeof(bufs));
    ml::CRect rc;
    rc.mPos = rect.mPos;
    rc.mSize.x = tileW;
    rc.mSize.y = tileH;
    u32 count = func_80499810(self, texWork, bufs, 10, &rc, 0);

    func_804948F4(0, 1);
    GXSetNumTevStages(1);
    func_80494A64(0, 2, 0);
    func_80494C30(0, 0, 0);
    func_8044A7F8__8CGXCacheFv(cache, 0, 4, 5, 0, 0);
    GXInitTexObjFilter(self->mTexture, (GXTexFilter)1, (GXTexFilter)1);
    GXLoadTexObj(self->mTexture, GX_TEXMAP0);
    func_80494F10(&rect, &col2, &col3);
    func_8044A7F8__8CGXCacheFv(cache, self->mUnk34, self->mUnk38, self->mUnk3C,
                               self->mUnk40, 0);

    ml::CCol4 col;
    col.r = self->mColor.r;
    col.g = self->mColor.g;
    col.b = self->mColor.b;
    col.a = self->mColor.a / (f64)count;

    for (u32 i = 0; i < count; i++) {
        GXInitTexObjFilter(bufs[i], (GXTexFilter)self->mUnk2C, (GXTexFilter)self->mUnk30);
        GXLoadTexObj(bufs[i], GX_TEXMAP0);
        func_80494F10(&rect, &col, &col3);
    }
    if (self->mFlags & 4) {
        func_8049A850(self, self->mTexture, &rect);
    }
    for (u32 i = 0; i < count; i++) {
        func_804902D8(texWork, bufs[i]);
    }
    func_804902D8(texWork, self->mTexture);
    func_80490314(texWork);
    self->mTexture = NULL;
    func_8044A578__8CGXCacheFv(cache, &self->mClearColor, 0);
}

// ============================================================================
// func_80499810 (0x8049D8F4) - bright-pass extraction over halving tiles.
// Renders the screen texture into successively halved tiles, capturing up to
// 'maxBufs' bright-pass textures. Each tile is sampled with per-axis step
// sizes mThreshold / (size - 0.5); when mEnabled is set, four diagonal taps
// are drawn per tile instead of a single offset tap. Returns the count.
// ============================================================================
u32 func_80499810(CScnBloom* self, CTexWorkObj* work, GXTexObj** bufs,
                  int maxBufs, ml::CRect* rect, u32 flag) {
    ml::CRect rc;
    s16 x = rect->mPos.x;
    s16 y = rect->mPos.y;
    s16 w = rect->mSize.x;
    s16 h = rect->mSize.y;
    rc.mPos.y = y;
    rc.mPos.x = x;
    rc.mSize.x = w;
    rc.mSize.y = h;

    GXTexObj* tex = func_80490208(work, (u16)w, (u16)h, 1);
    if (work == NULL)
        return 0;

    GXInitTexObjFilter(tex, (GXTexFilter)1, (GXTexFilter)1);
    func_804944DC(tex, &rc, 0, 0);

    if (flag == 0) {
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        func_80494A64(0, 1, 0);
    } else {
        // Two constant TEV colors built from the blend/filter mode params.
        GXColorS10 blendCol = {0, 0, 0, 0};
        GXColorS10 filterCol = {0, 0, 0, 0};
        blendCol.r = blendCol.g = blendCol.b = self->mBlendMode;
        filterCol.r = filterCol.g = filterCol.b = self->mFilterType;
        GXSetNumTevStages(3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0xe, (GXTevBias)0,
                        (GXTevScale)0, GX_TRUE, (GXTevRegID)1);
        GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)8, (GXTevColorArg)4,
                        (GXTevColorArg)8, (GXTevColorArg)0xf);
        GXSetTevColorS10((GXTevRegID)2, blendCol);
        GXSetTevColorOp((GXTevStageID)1, (GXTevOp)0xe, (GXTevBias)0,
                        (GXTevScale)0, GX_TRUE, (GXTevRegID)1);
        GXSetTevColorIn((GXTevStageID)1, (GXTevColorArg)6, (GXTevColorArg)2,
                        (GXTevColorArg)2, (GXTevColorArg)0xf);
        GXSetTevColorS10((GXTevRegID)3, filterCol);
        GXSetTevColorOp((GXTevStageID)2, (GXTevOp)0xe, (GXTevBias)0,
                        (GXTevScale)0, GX_TRUE, (GXTevRegID)0);
        GXSetTevColorIn((GXTevStageID)2, (GXTevColorArg)2, (GXTevColorArg)0xf,
                        (GXTevColorArg)0xc, (GXTevColorArg)0xf);
    }

    func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(), 1, 0, 2, 0, 0);
    GXInitTexObjFilter(tex, (GXTexFilter)1, (GXTexFilter)1);
    GXLoadTexObj(tex, GX_TEXMAP0);

    ml::CCol4 colA(lbl_eu_8066AB0C, lbl_eu_8066AB0C, lbl_eu_8066AB00,
                   lbl_eu_8066AB00);
    ml::CCol4 colB(lbl_eu_8066AB00, lbl_eu_8066AB00, lbl_eu_8066AB00,
                   lbl_eu_8066AB00);
    func_80494F10(&rc, &colA, &colB);

    func_804948F4(0, 1);
    GXSetNumTevStages(1);
    func_80494A64(0, 2, 0);
    func_80494C30(0, 0, 0);

    f32 scale = self->mEnabled ? lbl_eu_8066AB10 : lbl_eu_8066AB18;
    const f32 one = lbl_eu_8066AB00;
    const f32 zero = lbl_eu_8066AB0C;
    GXTexObj** pBuf = bufs;
    u32 count = 0;
    while (count < self->mRenderMode && count < (u32)maxBufs) {
        s16 tw = rc.mSize.x;
        s16 th = rc.mSize.y;
        if (tw < 1 || th < 1)
            break;
        // Per-axis blur step: threshold / (tile size - 0.5).
        f32 sx = self->mThreshold / ((f64)(u32)(u16)tw - lbl_eu_8066AB20);
        f32 sy = self->mThreshold / ((f64)(u32)(u16)th - lbl_eu_8066AB20);

        *pBuf = func_80490208(work, (u16)tw, (u16)th, self->mBlurSamples);
        if (*pBuf == NULL)
            break;

        if (count == 0) {
            func_804944DC(*pBuf, &rc, 0, 0);
            func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(), 0, 4, 5,
                                       0, 0);
            GXInitTexObjFilter(*pBuf, (GXTexFilter)self->mUnk2C,
                               (GXTexFilter)self->mUnk30);
            GXLoadTexObj(*pBuf, GX_TEXMAP0);
            func_80494F10(&rc, &ml::CCol4(zero, zero, one, one),
                          &ml::CCol4(one, one, one, scale));
            func_804944DC(*pBuf, &rc, 0, 0);
        } else {
            func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(), 0, 4, 5,
                                       0, 0);
            // Draw the previous capture back over the tile before sampling.
            GXInitTexObjFilter(pBuf[-1], (GXTexFilter)self->mUnk2C,
                               (GXTexFilter)self->mUnk30);
            GXLoadTexObj(pBuf[-1], GX_TEXMAP0);
            func_80494F10(&rc, &ml::CCol4(zero, zero, one, one),
                          &ml::CCol4(one, one, one, scale));
        }

        func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(), 1, 4, 1, 0, 0);
        GXInitTexObjFilter(*pBuf, (GXTexFilter)self->mUnk2C,
                           (GXTexFilter)self->mUnk30);
        GXLoadTexObj(*pBuf, GX_TEXMAP0);
        if (self->mEnabled == 0) {
            func_80494F10(&rc, &ml::CCol4(sx, sy, one - sx, one - sy),
                          &ml::CCol4(one, one, one, scale));
            func_804944DC(*pBuf, &rc, 0, 0);
        } else {
            // Center tap plus four diagonal taps around it.
            ml::CCol4 cb2(one, one, one, scale);
            func_80494F10(&rc, &ml::CCol4(-sx, -sy, one - sx, one - sy), &cb2);
            func_804944DC(*pBuf, &rc, 0, 0);

            func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(), 1, 4, 1,
                                       0, 0);
            ml::CCol4 c1(sx, -sy, one + sx, one - sy);
            func_80494F10(&rc, &c1, &cb2);            func_804944DC(*pBuf, &rc, 0, 0);

            func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(), 1, 4, 1,
                                       0, 0);
            ml::CCol4 c2(-sx, sy, one - sx, one + sy);
            func_80494F10(&rc, &c2, &cb2);            func_804944DC(*pBuf, &rc, 0, 0);

            func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(), 1, 4, 1,
                                       0, 0);
            ml::CCol4 c3(sx, sy, one + sx, one + sy);
            func_80494F10(&rc, &c3, &cb2);            func_804944DC(*pBuf, &rc, 0, 0);
        }

        // Halve the tile size each pass, rounding up to keep sizes even.
        s16 nw = rc.mSize.x;
        s16 nh = rc.mSize.y;
        s16 hw = nw / 2;
        s16 hh = nh / 2;
        if (nw % 2 != 0)
            hw += 1;
        if (nh % 2 != 0)
            hh += 1;
        rc.mSize.x = hw;
        rc.mSize.y = hh;
        ++pBuf;
        count++;
    }

    func_804902D8(work, tex);
    return count;
}

// ============================================================================
// func_8049928C (0x8049928C) - bloom render through the CScnFilter vtable.
// Skips when the base color alpha matches the pool constant, otherwise renders
// the view into tiles, extracts up to 10 bright-pass textures via
// func_80499810 and blurs them back over the scene.
// ============================================================================
extern "C" void func_8049928C(CScnBloom* self, u32 param) {
    CView* view = CView::getCurrentView();
    ml::CRect rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(&rect, view);

    if (self->mColor.a != lbl_eu_8066AAF0) {
        // 'param' is the owning scene-view object; its +0x6C holds the texture
        // work manager.
        CTexWorkObj* texWork = ((CScnBloomView*)param)->mTexWork;
        GXTexObj* tex = func_80490208(texWork, (u16)rect.mSize.x, (u16)rect.mSize.y,
                                      self->mTexFormat);
        if (tex != NULL) {
            CGXCache* cache = CDeviceGX::getCacheInstance();
            ml::CCol4 col2(lbl_eu_8066AAF4, lbl_eu_8066AAF4, lbl_eu_8066AAF4,
                           lbl_eu_8066AAF4);
            ml::CCol4 col3(lbl_eu_8066AAF0, lbl_eu_8066AAF0, lbl_eu_8066AAF4,
                           lbl_eu_8066AAF4);
            GXLoadPosMtxImm(ml::CMat34::identity.m, 0);
            GXSetCurrentMtx(0);
            func_8044B5C0__8CGXCacheFv(cache);
            cache->func_8044A94C(0, 0);
            cache->func_8044AA7C(0, 0);
            func_8044ABAC__8CGXCacheFv(cache, 0, 0);
            func_804948F4(0, 1);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
                              GX_FALSE, GX_PTIDENTITY);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            func_80494A64(0, 2, 0);
            func_80494C30(0, 0, 0);
            func_804943E0(tex, 0, 1);

            // Tile sizes: viewport divided by mUnk44, rounded up to an even
            // count (odd quotient gets incremented).
            s32 div = self->mUnk44;
            u16 tileW = rect.mSize.x / div;
            if (tileW & 1)
                tileW += 1;
            u16 tileH = rect.mSize.y / div;
            if (tileH & 1)
                tileH += 1;

            GXInitTexObjFilter(tex, (GXTexFilter)self->mUnk18, (GXTexFilter)self->mUnk1C);
            GXLoadTexObj(tex, GX_TEXMAP0);

            ml::CRect rect2;
            rect2.mPos = rect.mPos;
            rect2.mSize.x = tileW;
            rect2.mSize.y = tileH;
            func_80494F10(&rect2, &col2, &col3);

            GXTexObj* bufs[10];
            memset(bufs, 0, sizeof(bufs));
            ml::CRect rc;
            rc.mPos = rect.mPos;
            rc.mSize.x = tileW;
            rc.mSize.y = tileH;
            u32 count = func_80499810(self, texWork, bufs, 10, &rc, 1);

            func_8044A7F8__8CGXCacheFv(cache, 0, 4, 5, 0, 0);
            GXLoadTexObj(tex, GX_TEXMAP0);
            func_80494F10(&rect, &col2, &col3);
            func_8044A7F8__8CGXCacheFv(cache, self->mUnk34, self->mUnk38,
                                       self->mUnk3C, self->mUnk40, 0);

            // Blur step size: color alpha divided by the extracted-texture
            // count (converted through f64).
            ml::CCol4 col;
            col.r = self->mColor.r;
            col.g = self->mColor.g;
            col.b = self->mColor.b;
            col.a = self->mColor.a / (f64)count;

            if (self->mUnk50 & 4) {
                GXSetNumTexGens(2);
                GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
                                  GX_FALSE, GX_PTIDENTITY);
                GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
                                  GX_FALSE, GX_PTIDENTITY);
                GXSetNumTevStages(2);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
                func_80494A64(0, 2, 0);
                func_80494A64(1, 5, 0);
                func_80494C30(0, 0, 0);
                func_80494C30(1, 4, 0);
                GXLoadTexObj((GXTexObj*)((u8*)&self->mClearColor + 0xC), GX_TEXMAP1);
            }
            for (u32 i = 0; i < count; i++) {
                GXInitTexObjFilter(bufs[i], (GXTexFilter)self->mUnk2C,
                                   (GXTexFilter)self->mUnk30);
                GXLoadTexObj(bufs[i], GX_TEXMAP0);
                func_80494F10(&rect, &col, &col3);
            }
            for (u32 i = 0; i < count; i++) {
                func_804902D8(texWork, bufs[i]);
            }
            func_804902D8(texWork, tex);
            func_80490314(texWork);
            cache->func_8044A94C(1, 0);
            cache->func_8044AA7C(1, 0);
            func_8044ABAC__8CGXCacheFv(cache, 0, 0);
        }
    }
}

// Retail __ct__CScnBloom (flat name): runs the base-object initializer, then
// constructs the CScnFilter sub-object at +0x48 and installs both vtables
// (main at +0x80, i.e. over the blur-scale z slot which is written later by
// func_80499FD0; the derived CScnFilter entry (+8) goes to +0x48).
extern "C" CScnBloom* __ct__CScnBloom(CScnBloom* self) {
    func_804996E8(self);
    __ct__CScnFilter((void*)&self->mView);
    void* vtbl = (void*)lbl_eu_8056EA08;
    *(void**)&self->mBlurScale.z = vtbl;
    self->mView = (CView*)((u8*)vtbl + 8);
    return self;
}

// ============================================================================
// __dt__9CScnBloomFv - complete-object destructor.
// With self != 0: destroys the CScnFilter subobject at +0x48 and the
// non-polymorphic base fragment, frees the storage when flags > 0.
// Returns self in every case (retail keeps self in r30, flags in r31).
// ============================================================================
extern "C" void* __dt__9CScnBloomFv(CScnBloom* self, s32 flags) {
    if (self != NULL) {
        __dt__10CScnFilterFv((CScnFilter*)(void*)&self->mView, 0);
        __dt__804997D0(self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

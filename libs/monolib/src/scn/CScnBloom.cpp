// CScnBloom - Bloom post-processing effect.
// The bloom object is created by CScnRoot (0x94 bytes): base params at 0x00
// (init by func_80499718 via func_804996E8), view pointer at 0x48, then
// per-frame render state (func_80499FD0 is the second-stage constructor).

#include "libs/monolib/src/scn/CScnBloom.hpp"
extern "C" void __ct__CScnFilter(void*);
#include "monolib/core/CView.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/math/CMat34.hpp"
#include <harness_catalog.h>
#include <stdint.h>

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
void func_804944DC(GXTexObj* tex, ml::CRect* rect, u8 p3, u8 p4);
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
void __dt__9CScnBloomFv(CScnBloom* self, s32 flags);
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

void func_8049A318(){}

extern "C" void func_80499810() {}

// Retail __ct__CScnBloom (flat name): runs the base-object initializer, then
// constructs the CScnFilter sub-object at +0x48 and installs both vtables
// (main at +0x80, the derived CScnFilter sub-object at +0x48 = main + 8).
extern "C" CScnBloom* __ct__CScnBloom(CScnBloom* self) {
    func_804996E8(self);
    __ct__CScnFilter((void*)((u8*)self + 0x48));
    *(void**)((u8*)self + 0x80) = (void*)lbl_eu_8056EA08;
    *(void**)((u8*)self + 0x48) = (void*)((u8*)lbl_eu_8056EA08 + 8);
    return self;
}

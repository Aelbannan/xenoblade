// CScnBlend full-screen color-blend filter effect.
// Inherits CScnFilter (scene filter base) and adds a blend-rect region plus
// GX blend/colour settings. Decompiled for monolib/src/scn/CScnBlend.

#include <types.h>
#include <monolib/scn/CScnFilter.hpp>
#include <monolib/core/CView.hpp>
#include <monolib/device/CDeviceGX.hpp>
#include <monolib/math/CMat34.hpp>

// --- Cross-TU imports (declared here; .cpp-only TU, CScnFilter.hpp is read-only) ---
extern "C" void __ct__CScnFilter(CScnFilter* self);
extern "C" void __dt__10CScnFilterFv(CScnFilter* self, int flag);
// Global operator delete — MWCC mangles this to __dl__FPv (defined in MemManager.cpp).
void operator delete(void* p) throw();
extern const f32 lbl_eu_8066AAE0;
extern const f32 lbl_eu_8066AAE4;
extern const f32 lbl_eu_8066AAE8;
extern "C" void func_8049C868();   // CScnBloom (foreign TU)

// Opaque render-work object (CScnTexWorkMan instance), only forwarded.
struct CTexWorkObj;
extern "C" GXTexObj* func_80490208(CTexWorkObj* obj, u16 w, u16 h, u32 fmt);
extern "C" void func_804902D8(CTexWorkObj* obj, GXTexObj* tex);
extern "C" void func_8043E928__5CViewFRQ22ml5CRectP5CView(ml::CRect* rect,
                                                           CView* view);
extern "C" void func_804948F4(int fmt, int mode);
extern "C" void func_80494A64(int a, int b, int c);
extern "C" void func_80494C30(int a, int b, int c);
extern "C" void func_804943E0(GXTexObj* tex, int p2, int p3);
struct RectRegion;
extern "C" void func_80494F10(ml::CRect* rect, RectRegion* region,
                              ml::CCol4* col);
extern "C" void func_80494D84(ml::CRect* rect, RectRegion* region);
extern "C" void func_8044B5C0__8CGXCacheFv(CGXCache* cache);
extern "C" void func_8044A7F8__8CGXCacheFv(CGXCache* cache, u32 a, u32 b,
                                           u32 c, u32 d, u32 e);
extern "C" void func_8044ABAC__8CGXCacheFv(CGXCache* cache, u32 a, u32 b);
extern const f32 lbl_eu_8066AAEC;
extern u32 lbl_eu_806639E8;    // CScnBlend RTTI locator (owned by CScnItemCamera TU)
extern u32 lbl_eu_806639F0;    // CScnFilter RTTI locator (owned by CScnItemCamera TU)

// Region + tint as one anonymous aggregate (MWCC struct-copies via lwz/stw).
// x/y/w/h describe the blend quad in normalised screen space; tR/tG/tB are the
// per-channel tint factors (h doubles as the "effect active" marker: it equals
// the pool constant when the filter is disabled).
struct RectRegion {
    struct {
        f32 x;      // 0x38
        f32 y;      // 0x3C
        f32 w;      // 0x40
        f32 h;      // 0x44
        f32 tR;     // 0x48
        f32 tG;     // 0x4C
        f32 tB;     // 0x50
    };
};

struct CScnBlend : public CScnFilter {
    virtual ~CScnBlend();

    u8 pad_18[0x38 - 0x18];
    RectRegion mRect;   // 0x38
    u8 mFlags;      // 0x54 bit0: render-to-texture, bit1: halve target size
    u8 pad_55[3];
    u32 field_0x58; // texture format passed to the tex-work allocator
    s32 field_0x5C; // blend mode (compared against 3 for the two-stage path)
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6C;
};

extern "C" void func_80498DE8(CScnBlend* self, void* param);  // defined below

extern "C" CScnBlend* __dt__9CScnBlendFv(CScnBlend* self, int flag); // defined below

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056E9E8-0x8056EA08 (32B): CScnBlend vtable (20B) + RTTI chain (12B).
// The vtable is installed by __ct__CScnBlend; writing the dtor as an extern "C"
// free function keeps MWCC from auto-emitting __vt__/__RTTI__ here (the retail
// RTTI locators lbl_eu_806639E8/lbl_eu_806639F0 live in the CScnItemCamera TU).
u32 lbl_eu_8056E9E8[5] = {
    (u32)&lbl_eu_806639E8, 0x00000000, (u32)&__dt__9CScnBlendFv,
    (u32)&func_8049C868, (u32)&func_80498DE8,
};
u32 lbl_eu_8056E9FC[3] = {
    (u32)&lbl_eu_806639F0, 0x00000000, 0x00000000,
};

// [.rodata] 0x805240A0-0x805240AC (12B): "CScnBlend"
extern "C" const u32 lbl_eu_805240A0[3] = {
    0x4353636E, 0x426C656E, 0x64000000,
};

// extern "C" free-function dtor (CFontLayer pattern): keeps MWCC from
// auto-emitting __vt__/__RTTI__ for CScnBlend in this TU (the retail vtable
// is the dissolved .data blob above).
// MWCC destructors return the object pointer (see trailing mr r3, r30 in retail).
extern "C" CScnBlend* __dt__9CScnBlendFv(CScnBlend* self, int flag) {
    if (self != 0) {
        __dt__10CScnFilterFv((CScnFilter*)self, 0);
        if (flag > 0) {
            operator delete(self);
        }
    }
    return self;
}

// Retail __ct__CScnBlend (flat name): calls the base ctor, installs the
// CScnBlend vtable, fills the default blend rect/tint region and the
// blend-mode fields. Returns the object.
extern "C" CScnBlend* __ct__CScnBlend(CScnBlend* self) {
    __ct__CScnFilter(self);

    *(void**)self = (void*)lbl_eu_8056E9E8;

    // First 0-store is consumed by the flags OR (store-to-load forward); the
    // second (redundant) store keeps the physical stw at the end of the block.
    self->field_0x68 = 0;

    RectRegion tmp;
    tmp.x = lbl_eu_8066AAE0;
    tmp.y = lbl_eu_8066AAE0;
    tmp.w = lbl_eu_8066AAE0;
    tmp.h = lbl_eu_8066AAE4;
    tmp.tR = lbl_eu_8066AAE8;
    tmp.tG = lbl_eu_8066AAE8;
    tmp.tB = lbl_eu_8066AAE8;
    self->mRect = tmp;

    self->mFlags = (u8)((u32)self->field_0x68 | 3);
    self->field_0x58 = 4;
    self->field_0x5C = 1;
    self->field_0x60 = 4;
    self->field_0x64 = 1;
    self->field_0x68 = 0;
    return self;
}

struct CScnBlendState {
    u8 _00[0x54];
    u8 flags;
};

void func_80498D98(void* obj, int enable) {
    CScnBlendState* state = (CScnBlendState*)obj;
    if (enable != 0) {
        state->flags |= 1;
    } else {
        state->flags &= 0xFE;
    }
}

extern "C" void func_80498DC0(u8* self, u32 enable) {
    if (enable != 0) {
        *(u8*)((u8*)self + 0x54) |= 2;
    } else {
        *(u8*)((u8*)self + 0x54) &= ~2;
    }
}

// Owning scene view as seen here: only field 0x6C (texture work manager) read.
struct CScnBlendView {
    u8 field_0x00[0x6C];
    CTexWorkObj* mTexWork;
};

// ============================================================================
// func_80498DE8 - full-screen blend filter draw (CScnBlend vtable slot).
// Skips when the region height (= tint alpha) matches the pool constant.
// With flag bit0 clear the quad is drawn straight into the frame; with bit0
// set the current view is first rendered into a texture (halved when bit1 is
// set) and drawn back with a TEV stage modulated by the negated tint color.
// ============================================================================
extern "C" void func_80498DE8(CScnBlend* self, void* param) {
    ml::CRect rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(&rect, CView::getCurrentView());

    if (self->mRect.h != lbl_eu_8066AAE8) {
        GXLoadPosMtxImm(ml::CMat34::identity.m, 0);
        GXSetCurrentMtx(0);
        func_8044B5C0__8CGXCacheFv(CDeviceGX::getCacheInstance());
        CDeviceGX::getCacheInstance()->func_8044A94C(0, 0);
        CDeviceGX::getCacheInstance()->func_8044AA7C(0, 0);
        func_8044ABAC__8CGXCacheFv(CDeviceGX::getCacheInstance(), 0, 0);

        if (!(self->mFlags & 1)) {
            // Direct path: no texture, no texture matrix.
            func_804948F4(0, 0);
            func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(),
                                       self->field_0x5C,
                                       self->field_0x60, self->field_0x64,
                                       self->field_0x68, 0);
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                          GX_COLOR0A0);
            if (self->field_0x5C != 3) {
                func_80494A64(0, 0, 0);
                func_80494C30(0, 0, 0);
            } else {
                func_80494A64(0, 3, 0);
                func_80494C30(0, 0, 0);
            }
            func_80494D84(&rect, &self->mRect);
        } else {
            // Texture path: render the view into a work texture first.
            CTexWorkObj* texWork = ((CScnBlendView*)param)->mTexWork;
            u16 tw;
            u16 th;
            if (self->mFlags & 2) {
                // Halve the rect size (signed divide by two).
                s16 w = rect.mSize.x;
                s16 h = rect.mSize.y;
                tw = (u16)(w / 2);
                th = (u16)(h / 2);
            } else {
                tw = (u16)rect.mSize.x;
                th = (u16)rect.mSize.y;
            }
            GXTexObj* tex = func_80490208(texWork, tw, th, self->field_0x58);
            if (tex == NULL) {
                // Allocation failure: retail exits without the cache restore.
                return;
            }
            func_804943E0(tex, (self->mFlags >> 1) & 1, 0);
            func_804948F4(0, 1);
            func_8044A7F8__8CGXCacheFv(CDeviceGX::getCacheInstance(),
                                       self->field_0x5C,
                                       self->field_0x60,
                                       self->field_0x64,
                                       self->field_0x68, 0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
                              GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
            if (self->field_0x5C != 3) {
                // Single TEV stage: raster color * texture * (-tint*255)
                // held in TEV register 3.
                GXColorS10 tevColor = {0, 0, 0, 0};
                tevColor.r = (s16)(lbl_eu_8066AAEC * -self->mRect.tR);
                tevColor.g = (s16)(lbl_eu_8066AAEC * -self->mRect.tG);
                tevColor.b = (s16)(lbl_eu_8066AAEC * -self->mRect.tB);
                GXSetTevColorS10((GXTevRegID)3, tevColor);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0,
                              GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
                                GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC,
                                GX_CC_TEXC, GX_CC_C2);
                func_80494C30(0, 0, 0);
            } else {
                GXSetNumTevStages(2);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0,
                              GX_COLOR0A0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0,
                              GX_COLOR0A0);
                func_80494A64(0, 2, 1);
                func_80494A64(1, 4, 0);
                func_80494C30(0, 0, 0);
                func_80494C30(1, 0, 0);
            }
            GXInitTexObjFilter(tex, GX_LINEAR, GX_LINEAR);
            GXLoadTexObj(tex, GX_TEXMAP0);
            ml::CCol4 blendColor(lbl_eu_8066AAE8, lbl_eu_8066AAE8,
                                 lbl_eu_8066AAE0, lbl_eu_8066AAE0);
            func_80494F10(&rect, &self->mRect, &blendColor);
            func_804902D8(texWork, tex);
        }

        CDeviceGX::getCacheInstance()->func_8044A94C(1, 0);
        CDeviceGX::getCacheInstance()->func_8044AA7C(1, 0);
        func_8044ABAC__8CGXCacheFv(CDeviceGX::getCacheInstance(), 1, 0);
    }
}

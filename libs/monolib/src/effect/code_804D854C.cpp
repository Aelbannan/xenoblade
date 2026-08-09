// Decompiled for monolib/src/effect/code_804D854C
//
// Fog setup + draw-context helpers shared with the CScnTexWorkMan / marker
// draw pipeline (also referenced from monolib/src/core/code_804EE558.cpp).

#include <harness_catalog.h>
#include <revolution/gx.h>
#include <monolib/math.hpp>
#include <monolib/core/CViewFrame.hpp>

// Shared BSS fog descriptor: written by func_804D854C, read by func_804D8AA4.
struct FogDesc {
    u32 type;   // 0x00 GXFogType
    f32 f4;     // 0x04 start
    f32 f8;     // 0x08 end
    f32 fc;     // 0x0c near
    f32 f10;    // 0x10 far
    u32 color;  // 0x14 GXColor packed
};

extern FogDesc lbl_eu_8065FCD0;
extern u8    lbl_eu_80663B3C;
extern f32   lbl_eu_8066B1D8;

// The retail GXSetFog takes the color by pointer (repo header passes by value).
typedef void (*GXSetFogPtr)(GXFogType, const GXColor*, f32, f32, f32, f32);

// Adjacent helpers in CScnTexWorkMan.
extern "C" {
void func_804902D8(const void* p);
void* func_80490208(void* p, s32 w, s32 h, s32 n);
void func_8043E928__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, void* view);
void func_804944DC(GXTexObj* self, ml::CRect* rect, u8 p3, u8 p4);
}

// Scn object returned by func_80496264 (field_0x0c selects a view).
struct ScnObj {
    u8 pad_0x00[0x0c];
    void* field_0x0c;
};
extern "C" ScnObj* func_80496264(void* src, s32 index);
extern "C" void* func_8049627C(void* self, s32 index);

// Source object owning a texture-work pointer at +0x6c.
struct TexSrc {
    u8 pad_0x00[0x6c];
    void* field_0x6c;
};

// Minimal draw-context object (CDrawGX draw state).
struct CDrawCtx {
    TexSrc* field_0x00;    // 0x00 owning tex-work source (has a +0x6c ptr)
    s32   field_0x04;   // 0x04 texture-object handle / guard
    u8    field_0x08;   // 0x08 copy flag
};

extern "C" void func_8048ECD8(void* r3);
extern "C" void* GetFog__Q34nw4r3g3d7ScnRootFi(void* self, int idx);

// func_804D8AA4: push fog state (or a cleared/default fog).
void func_804D8AA4(int p1, int UNUSED_p2) {
    if (lbl_eu_80663B3C != 0 && p1 != 0) {
        GXColor color;
        *(u32*)&color = lbl_eu_8065FCD0.color;
        ((GXSetFogPtr)GXSetFog)((GXFogType)lbl_eu_8065FCD0.type, &color,
                                lbl_eu_8065FCD0.f4, lbl_eu_8065FCD0.f8,
                                lbl_eu_8065FCD0.fc, lbl_eu_8065FCD0.f10);
    } else {
        GXColor color;
        *(u32*)&color = 0;
        f32 c = lbl_eu_8066B1D8;
        ((GXSetFogPtr)GXSetFog)((GXFogType)0, &color, c, c, c, c);
    }
}

// func_804D8C18: release the draw-context texture object and clear the owner.
void func_804D8C18(CDrawCtx* self) {
    if (0 != self->field_0x04) {
        func_804902D8(self->field_0x00->field_0x6c);
        self->field_0x04 = 0;
    }
    self->field_0x00 = 0;
}

// func_804D8B38: init a draw context.
extern "C" void func_804D8B38(CDrawCtx* self) {
    self->field_0x00 = 0;
    self->field_0x04 = 0;
    self->field_0x08 = 0;
}

// func_804D8B4C: establish the draw context for a desktop/material, computing
//       the target texture size from the owning view's rect.
int func_804D8B4C(CDrawCtx* draw, TexSrc* desktop, void* material) {
    ScnObj* obj = func_80496264(desktop, -1);
    void* view = func_8049627C(desktop, (s32)obj->field_0x0c);
    ml::CRect rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(rect, view);
    draw->field_0x00 = (TexSrc*)desktop;
    draw->field_0x08 = (u8)(u32)material;
    if (material == 0) {
        draw->field_0x04 = (s32)func_80490208(desktop->field_0x6c,
                                               (u16)rect.mSize.x,
                                               (u16)rect.mSize.y, 6);
    } else {
        draw->field_0x04 = (s32)func_80490208(desktop->field_0x6c,
                                               (u16)((u32)(s32)rect.mSize.x >> 1),
                                               (u16)((u32)(s32)rect.mSize.y >> 1), 6);
    }
    if (draw->field_0x04 == 0) {
        return 0;
    }
    return 1;
}

// func_804D8C68: load the draw-context texture and blit the view rect into it.
extern "C" void func_804D8C68(CDrawCtx* draw, GXTexMapID p2, const ml::CVec2* p3) {
    ScnObj* obj = func_80496264(draw->field_0x00, -1);
    void* view = func_8049627C(draw->field_0x00, (s32)obj->field_0x0c);
    ml::CRect rect;
    func_8043E928__5CViewFRQ22ml5CRectP5CView(rect, view);
    s32 w = rect.mSize.x;
    s32 h = rect.mSize.y;
    if (p3 != 0) {
        w = (s32)((f32)rect.mSize.x * p3->x);
        if (w & 1) w = (s16)(w + 1);
        h = (s32)((f32)rect.mSize.y * p3->y);
        if (h & 1) h = (s16)(h + 1);
    }
    GXLoadTexObj((GXTexObj*)draw->field_0x04, p2);
    ml::CRect local;
    local.mPos.x = rect.mPos.x;
    local.mPos.y = rect.mPos.y;
    local.mSize.x = (s16)w;
    local.mSize.y = (s16)h;
    func_804944DC((GXTexObj*)draw->field_0x04, &local, (u8)draw->field_0x08, 0);
}

void func_804D854C(){}
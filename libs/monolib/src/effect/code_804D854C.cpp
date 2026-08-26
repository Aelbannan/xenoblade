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
void func_804902D8(const void* p, s32 handle);
void* func_80490208(void* p, s32 w, s32 h, s32 n);
void func_8043E928__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, void* view);
void func_804944DC(GXTexObj* self, ml::CRect* rect, u8 p3, u8 p4);
}

// Fog fade globals shared with CETrail.cpp (func_804D82DC / func_804D83D0).
extern s32 lbl_eu_806659BC;   // frames remaining in the fog fade
extern u8  lbl_eu_806659C0[8]; // override fog color used while a fog is disabled
struct CScnRootEnv;
extern CScnRootEnv* lbl_eu_806659B8; // active scene
extern s32 lbl_eu_80663B38;   // fog index used for the scene lookup

// .sdata2 constants used by the blend math.
extern f32 lbl_eu_8066B1B8;  // 1.0 target weight
extern f32 lbl_eu_8066B1BC;  // fade duration divisor
extern f32 lbl_eu_8066B1C0;  // 255.0f color scale
extern f32 lbl_eu_8066B1C4;  // 0.5f selection threshold

// Leading 0x18 bytes of a fog record as returned by ScnRoot::GetFog.
struct FogData {
    u32 m_type; // 0x00
    f32 m_startz;
    f32 m_endz;
    f32 m_nearz;
    f32 m_farz; // 0x10
    u8 m_r, m_g, m_b, m_a;
};

// Full 0x30-byte fog record; GetFog only fills the leading FogData (0x18).
struct FogState {
    u32 m_type; // 0x00
    f32 m_startz;
    f32 m_endz;
    f32 m_nearz;
    f32 m_farz;  // 0x10
    u8 m_r;      // 0x14
    u8 m_g;
    u8 m_b;
    u8 m_a;
    u8 m_18;
    u8 m_19;
    u16 m_1A;
    u32 m_1C;
    u32 m_20;
    u32 m_24;
    u32 m_28;
    u32 m_2C;
};
extern FogState lbl_eu_8065FCA0;

namespace nw4r {
namespace g3d {
struct ScnRoot {
    FogData* GetFog(int index);
};
} // namespace g3d
} // namespace nw4r

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
enum DrawCtxState { DCTX_EMPTY = 0 };
struct CDrawCtx {
    TexSrc* field_0x00;    // 0x00 owning tex-work source (has a +0x6c ptr)
    s32   field_0x04;   // 0x04 texture-object handle / guard
    u8    field_0x08;   // 0x08 copy flag
};

extern "C" void* func_8048ECD8(void* r3);
extern "C" void* GetFog__Q34nw4r3g3d7ScnRootFi(void* self, int idx);

// Retail: lwz r3,0x6C(r3); b func_80490314 / func_8049032C — load the
// desktop's tex-work pointer at +0x6C and forward it (callee ignores r3).
extern "C" void func_80490314(void* x);
extern "C" void func_8049032C(void* x);
extern "C" void func_804D8B28(void* desktop) { func_80490314(*(void**)((u8*)desktop + 0x6C)); }
extern "C" void func_804D8B30(void* desktop) { func_8049032C(*(void**)((u8*)desktop + 0x6C)); }


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
    if (self->field_0x04 != 0) {
        // handle passed through as a dead 2nd arg; its load is CSE'd with the
        // guard compare, which is what puts the tested value in r4 like retail.
        func_804902D8(self->field_0x00->field_0x6c, self->field_0x04);
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
        w = (s32)(p3->x * (f32)rect.mSize.x);
        if (w & 1) w = (s16)(w + 1);
        h = (s32)(p3->y * (f32)rect.mSize.y);
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

// func_804D854C: resolve the active fog into lbl_eu_8065FCD0. While a fade is
// running (lbl_eu_806659BC > 0) the saved fog (lbl_eu_8065FCA0) is blended
// toward the scene fog by the remaining-frame ratio.
void func_804D854C() {
    if (lbl_eu_806659BC <= 0) {
        nw4r::g3d::ScnRoot* root = (nw4r::g3d::ScnRoot*)func_8048ECD8(lbl_eu_806659B8);
        *(FogData*)&lbl_eu_8065FCD0 = *root->GetFog(lbl_eu_80663B38);
        return;
    }

    nw4r::g3d::ScnRoot* root = (nw4r::g3d::ScnRoot*)func_8048ECD8(lbl_eu_806659B8);

    // GetFog only fills the leading 0x18 bytes; widening tmp to a full
    // FogState leaves its tail fields untouched (matching retail).
    FogState tmp;
    *(FogData*)&tmp = *root->GetFog(lbl_eu_80663B38);

    // Fade weight derived from the frames-remaining counter.
    f32 blend = lbl_eu_8066B1B8 - lbl_eu_806659BC / lbl_eu_8066B1BC;
    f32 w = lbl_eu_8066B1B8 - blend;

    FogState work[2];
    work[0] = *(FogState*)&lbl_eu_8065FCA0;
    work[1] = tmp;

    if (tmp.m_type == 0) {
        work[1] = *(FogState*)&lbl_eu_8065FCA0;
        work[1].m_r = lbl_eu_806659C0[0];
        work[1].m_g = lbl_eu_806659C0[1];
        work[1].m_b = lbl_eu_806659C0[2];
        work[1].m_a = lbl_eu_806659C0[3];
    }
    if (work[0].m_type == 0) {
        work[0] = tmp;
        work[0].m_r = lbl_eu_806659C0[0];
        work[0].m_g = lbl_eu_806659C0[1];
        work[0].m_b = lbl_eu_806659C0[2];
        work[0].m_a = lbl_eu_806659C0[3];
    }

    // Past halfway the blend snaps to the scene fog record entirely.
    FogState* sel = (blend >= lbl_eu_8066B1C4) ? &work[1] : &work[0];

    // Colors are normalized, blended, then rescaled for the output record.
    f32 base[4];
    base[0] = work[0].m_r / lbl_eu_8066B1C0;
    base[1] = work[0].m_g / lbl_eu_8066B1C0;
    base[2] = work[0].m_b / lbl_eu_8066B1C0;
    base[3] = work[0].m_a / lbl_eu_8066B1C0;

    f32 tgt[4];
    tgt[0] = work[1].m_r / lbl_eu_8066B1C0;
    tgt[1] = work[1].m_g / lbl_eu_8066B1C0;
    tgt[2] = work[1].m_b / lbl_eu_8066B1C0;
    tgt[3] = work[1].m_a / lbl_eu_8066B1C0;

    f32 out[4];
    out[0] = tgt[0] * blend + base[0] * w;
    out[1] = tgt[1] * blend + base[1] * w;
    out[2] = tgt[2] * blend + base[2] * w;
    out[3] = tgt[3] * blend + base[3] * w;

    FogDesc* dst = &lbl_eu_8065FCD0;
    dst->type = sel->m_type;
    dst->f4 = blend * (work[1].m_startz - work[0].m_startz) + work[0].m_startz;
    dst->f8 = blend * (work[1].m_endz - work[0].m_endz) + work[0].m_endz;
    dst->fc = blend * (work[1].m_nearz - work[0].m_nearz) + work[0].m_nearz;
    dst->f10 = blend * (work[1].m_farz - work[0].m_farz) + work[0].m_farz;

    GXColor color;
    *(u32*)&color = 0;
    color.r = (u8)(out[0] * lbl_eu_8066B1C0);
    color.g = (u8)(out[1] * lbl_eu_8066B1C0);
    color.b = (u8)(out[2] * lbl_eu_8066B1C0);
    color.a = (u8)(lbl_eu_8066B1B8 * lbl_eu_8066B1C0);
    dst->color = *(u32*)&color;
}
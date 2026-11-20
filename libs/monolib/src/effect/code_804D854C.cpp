// Decompiled for monolib/src/effect/code_804D854C
//
// Fog setup + draw-context helpers shared with the CScnTexWorkMan / marker
// draw pipeline (also referenced from monolib/src/core/code_804EE558.cpp).

#include <harness_catalog.h>
#include <revolution/gx.h>
#include <monolib/math.hpp>
#include <monolib/core/CViewFrame.hpp>

// Shared BSS fog descriptor: written by resolveEffectFog, read by pushEffectFog.
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

// The retail GXSetFog takes the color by pointer (repo header passes by value).
typedef void (*GXSetFogPtr)(GXFogType, const GXColor*, f32, f32, f32, f32);

// Adjacent helpers in CScnTexWorkMan.
extern "C" {
void TexMan_ReleaseArg_02D8(const void* p, s32 handle);
void* TexMan_AllocBuffer_0208(void* p, s32 w, s32 h, s32 n);
void insetRectToView__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, void* view);
void TexObj_BlitRect(GXTexObj* self, ml::CRect* rect, u8 p3, u8 p4);
}

// Fog fade globals shared with CETrail.cpp (Trail_SnapFog / Trail_RestoreFog).
extern s32 lbl_eu_806659BC;   // frames remaining in the fog fade
extern u8  lbl_eu_806659C0[8]; // override fog color used while a fog is disabled
struct CScnRootEnv;
extern CScnRootEnv* sActiveSceneRoot; // active scene
extern s32 lbl_eu_80663B38;   // fog index used for the scene lookup

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

// Scn object returned by Scn_FindCamItem (field_0x0c selects a view).
struct ScnObj {
    u8 pad_0x00[0x0c];
    void* field_0x0c;
};
extern "C" ScnObj* Scn_FindCamItem(void* src, s32 index);
extern "C" void* Scn_SetCamIndex(void* self, s32 index);

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

extern "C" void* getScnRootSlot10(void* sceneEnv);
extern "C" void* GetFog__Q34nw4r3g3d7ScnRootFi(void* self, int idx);

// resolveEffectFog: resolve the active fog into lbl_eu_8065FCD0. While a fade is
// running (lbl_eu_806659BC > 0) the saved fog (lbl_eu_8065FCA0) is blended
// toward the scene fog by the remaining-frame ratio.
void resolveEffectFog() {
    if (lbl_eu_806659BC <= 0) {
        s32 idx = lbl_eu_80663B38;
        nw4r::g3d::ScnRoot* root = (nw4r::g3d::ScnRoot*)getScnRootSlot10(sActiveSceneRoot);
        *(FogData*)&lbl_eu_8065FCD0 = *root->GetFog(idx);
        return;
    }

    s32 idx = lbl_eu_80663B38;
    nw4r::g3d::ScnRoot* root = (nw4r::g3d::ScnRoot*)getScnRootSlot10(sActiveSceneRoot);

    // GetFog only fills the leading 0x18 bytes; widening tmp to a full
    // FogState leaves its tail fields untouched (matching retail).
    FogState tmp;
    *(FogData*)&tmp = *root->GetFog(idx);

    // Fade weight derived from the frames-remaining counter.
    f32 blend = 1.0f - (f32)lbl_eu_806659BC / 30.0f;

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

    // Colors are normalized, blended, then rescaled for the output record.
    f32 base[4];
    base[0] = work[0].m_r / 255.0f;
    base[1] = work[0].m_g / 255.0f;
    base[2] = work[0].m_b / 255.0f;
    base[3] = work[0].m_a / 255.0f;

    f32 tgt[4];
    tgt[0] = work[1].m_r / 255.0f;
    tgt[1] = work[1].m_g / 255.0f;
    tgt[2] = work[1].m_b / 255.0f;
    tgt[3] = work[1].m_a / 255.0f;

    // Past halfway the blend snaps to the scene fog record entirely.
    u32 selType;
    if (blend < 0.5f) {
        selType = work[0].m_type;
    } else {
        selType = work[1].m_type;
    }

    f32 w = 1.0f - blend;

    f32 out[4];
    out[0] = tgt[0] * blend + base[0] * w;
    out[1] = tgt[1] * blend + base[1] * w;
    out[2] = tgt[2] * blend + base[2] * w;
    out[3] = tgt[3] * blend + base[3] * w;

    FogDesc* dst = &lbl_eu_8065FCD0;
    dst->type = selType;

    GXColor color;
    *(u32*)&color = 0;
    color.r = (u8)(out[0] * 255.0f);
    color.g = (u8)(out[1] * 255.0f);
    color.b = (u8)(out[2] * 255.0f);
    color.a = (u8)(1.0f * 255.0f);

    dst->f4 = blend * (work[1].m_startz - work[0].m_startz) + work[0].m_startz;
    dst->f8 = blend * (work[1].m_endz - work[0].m_endz) + work[0].m_endz;
    dst->fc = blend * (work[1].m_nearz - work[0].m_nearz) + work[0].m_nearz;
    dst->f10 = blend * (work[1].m_farz - work[0].m_farz) + work[0].m_farz;
    dst->color = *(u32*)&color;
}

// Retail: lwz r3,0x6C(r3); b TexMan_SnapCursor_0314 / TexMan_RestoreCursor_032C -- load the
// desktop's tex-work pointer at +0x6C and forward it (callee ignores r3).
extern "C" void TexMan_SnapCursor_0314(void* x);
extern "C" void TexMan_RestoreCursor_032C(void* x);
extern "C" void snapTexCursor(void* desktop) { TexMan_SnapCursor_0314(*(void**)((u8*)desktop + 0x6C)); }
extern "C" void restoreTexCursor(void* desktop) { TexMan_RestoreCursor_032C(*(void**)((u8*)desktop + 0x6C)); }


// pushEffectFog: push fog state (or a cleared/default fog).
void pushEffectFog(int p1, int UNUSED_p2) {
    if (lbl_eu_80663B3C != 0 && p1 != 0) {
        GXColor color;
        *(u32*)&color = lbl_eu_8065FCD0.color;
        ((GXSetFogPtr)GXSetFog)((GXFogType)lbl_eu_8065FCD0.type, &color,  // DEBT(callcast): SDK header passes GXColor by value; retail view passes &color (pointer) in r4
                                lbl_eu_8065FCD0.f4, lbl_eu_8065FCD0.f8,
                                lbl_eu_8065FCD0.fc, lbl_eu_8065FCD0.f10);
    } else {
        GXColor color;
        *(u32*)&color = 0;
        f32 c = 0.0f;
        ((GXSetFogPtr)GXSetFog)((GXFogType)0, &color, c, c, c, c);  // DEBT(callcast): SDK header passes GXColor by value; retail view passes &color (pointer) in r4
    }
}

// releaseEffectDrawTex: release the draw-context texture object and clear the owner.
void releaseEffectDrawTex(CDrawCtx* self) {
    if (self->field_0x04 != 0) {
        // handle passed through as a dead 2nd arg; its load is CSE'd with the
        // guard compare, which is what puts the tested value in r4 like retail.
        TexMan_ReleaseArg_02D8(self->field_0x00->field_0x6c, self->field_0x04);
        self->field_0x04 = 0;
    }
    self->field_0x00 = 0;
}

// initEffectDrawCtx: init a draw context.
extern "C" void initEffectDrawCtx(CDrawCtx* self) {
    self->field_0x00 = 0;
    self->field_0x04 = 0;
    self->field_0x08 = 0;
}

// setupEffectDrawCtx: establish the draw context for a desktop/material, computing
//       the target texture size from the owning view's rect.
int setupEffectDrawCtx(CDrawCtx* draw, TexSrc* desktop, void* material) {
    ScnObj* obj = Scn_FindCamItem(desktop, -1);
    void* view = Scn_SetCamIndex(desktop, (s32)obj->field_0x0c);
    ml::CRect rect;
    insetRectToView__5CViewFRQ22ml5CRectP5CView(rect, view);
    draw->field_0x00 = (TexSrc*)desktop;
    draw->field_0x08 = (u8)(u32)material;
    if (material == 0) {
        draw->field_0x04 = (s32)TexMan_AllocBuffer_0208(desktop->field_0x6c,
                                               (u16)rect.mSize.x,
                                               (u16)rect.mSize.y, 6);
    } else {
        draw->field_0x04 = (s32)TexMan_AllocBuffer_0208(desktop->field_0x6c,
                                               (u16)((u32)(s32)rect.mSize.x >> 1),
                                               (u16)((u32)(s32)rect.mSize.y >> 1), 6);
    }
    if (draw->field_0x04 == 0) {
        return 0;
    }
    return 1;
}

// blitEffectViewRect: load the draw-context texture and blit the view rect into it.
extern "C" void blitEffectViewRect(CDrawCtx* draw, GXTexMapID p2, const ml::CVec2* p3) {
    ScnObj* obj = Scn_FindCamItem(draw->field_0x00, -1);
    void* view = Scn_SetCamIndex(draw->field_0x00, (s32)obj->field_0x0c);
    ml::CRect rect;
    insetRectToView__5CViewFRQ22ml5CRectP5CView(rect, view);
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
    TexObj_BlitRect((GXTexObj*)draw->field_0x04, &local, (u8)draw->field_0x08, 0);
}

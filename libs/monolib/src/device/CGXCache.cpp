// Auto-scaffolded catalog TU for monolib/src/device/CGXCache
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/GX.h>
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CMat34.hpp"

// LLM-HARNESS-BEGIN: us-8044bbd4
extern "C" void __ct__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044bbd4

// LLM-HARNESS-BEGIN: us-8044be7c
extern "C" void __dt__11IStateCacheFv() {}
// LLM-HARNESS-END: us-8044be7c

// LLM-HARNESS-BEGIN: us-8044bebc
extern "C" void __ct__CMsgParam_32() {}
// LLM-HARNESS-END: us-8044bebc

// LLM-HARNESS-BEGIN: us-8044bef0
extern "C" void __dt__CMsgParam_32() {}
// LLM-HARNESS-END: us-8044bef0

// LLM-HARNESS-BEGIN: us-8044bf48
extern "C" void __dt__804494D8() {}
// LLM-HARNESS-END: us-8044bf48

// LLM-HARNESS-BEGIN: us-8044bfa4
extern "C" void __ct__80449534() {}
// LLM-HARNESS-END: us-8044bfa4

// LLM-HARNESS-BEGIN: us-8044bfb8
extern "C" void __ct__80449548(void) {}
// LLM-HARNESS-END: us-8044bfb8

// LLM-HARNESS-BEGIN: us-8044bfbc
extern "C" void func_8044954C(void) {}
// LLM-HARNESS-END: us-8044bfbc

// LLM-HARNESS-BEGIN: us-8044bfc0
extern "C" void func_80449550__FRQ22ml5CCol4() {}
// LLM-HARNESS-END: us-8044bfc0

// LLM-HARNESS-BEGIN: us-8044c764
extern "C" void __ct__IStateCache() {}
// LLM-HARNESS-END: us-8044c764

// LLM-HARNESS-BEGIN: us-8044c774
extern "C" void __dt__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044c774

// LLM-HARNESS-BEGIN: us-8044c7d8
extern "C" void func_80449D68__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044c7d8

// LLM-HARNESS-BEGIN: us-8044cfe8
extern "C" void func_8044A578__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044cfe8

// LLM-HARNESS-BEGIN: us-8044d138
extern "C" void func_8044A6C8__8CGXCacheFii() {}
// LLM-HARNESS-END: us-8044d138

// LLM-HARNESS-BEGIN: us-8044d268
extern "C" void func_8044A7F8__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044d268

// LLM-HARNESS-BEGIN: us-8044d3bc
extern "C" void func_8044A94C__8CGXCacheFii() {}
// LLM-HARNESS-END: us-8044d3bc

// LLM-HARNESS-BEGIN: us-8044d4ec
extern "C" void func_8044AA7C__8CGXCacheFii() {}
// LLM-HARNESS-END: us-8044d4ec

// LLM-HARNESS-BEGIN: us-8044d61c
extern "C" void func_8044ABAC__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044d61c

// LLM-HARNESS-BEGIN: us-8044d74c
extern "C" void func_8044ACDC__8CGXCacheFRCQ22ml5CCol4i() {}
// LLM-HARNESS-END: us-8044d74c

// LLM-HARNESS-BEGIN: us-8044d8fc
extern "C" void func_8044AE8C__8CGXCacheFRCQ22ml5CCol4i() {}
// LLM-HARNESS-END: us-8044d8fc

// LLM-HARNESS-BEGIN: us-8044daac
extern "C" void func_8044B03C__8CGXCacheFi() {}
// LLM-HARNESS-END: us-8044daac

// LLM-HARNESS-BEGIN: us-8044dbd8
extern "C" void func_8044B168__8CGXCacheFi() {}
// LLM-HARNESS-END: us-8044dbd8

// LLM-HARNESS-BEGIN: us-8044dd04
extern "C" void func_8044B294__8CGXCacheFUl(void) {}
// LLM-HARNESS-END: us-8044dd04

extern "C" void func_8044CE68__8CGXCacheFv(void* self, u32 cmd);

struct C1FCMsgEntry {
    u32 command;
    u32 wid;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

struct C1FCCacheLayout {
    u8 pad0[0x488];
    C1FCMsgEntry* mArrayPtr;
    u32 mFront;
    u32 mSize;
    u32 mCapacity;
    u32 field6;
    u32 field7;
    u8 pad1[8]; // 0x4A0..0x4A7 so rect4A8 lands at 0x4A8
    s16 rect4A8[4];
    s16 rect4B0[4];
    u8 pad2[0x50C - 0x4B8];
    u32 unk50C;
};

// LLM-HARNESS-BEGIN: us-8044dd08
// Optional ring pair pointers in r4–r6 update cached scissor rects, then enqueue
// GX commands for tags 0xB / 0xC via func_8044CE68 (this+4).
extern "C" void func_8044B298__8CGXCacheFv(void* self, void* a, void* b, void* c) {
    C1FCCacheLayout* cache = (C1FCCacheLayout*)self;
    u32* insetPair;
    s16 stack[4];
    u32 i;
    u32 idx;
    u32 slot;
    C1FCMsgEntry* entry;

    // Keep b in r31 across the function like retail (null → this+0x4A8).
    insetPair = (u32*)b;
    if (a != 0) {
        u32 w0 = ((u32*)a)[0];
        u32 w1 = ((u32*)a)[1];
        *(u32*)&cache->rect4A8[2] = w1;
        *(u32*)&cache->rect4A8[0] = w0;
    }
    if (c != 0) {
        u32 w0 = ((u32*)c)[0];
        u32 w1 = ((u32*)c)[1];
        *(u32*)&cache->rect4B0[2] = w1;
        *(u32*)&cache->rect4B0[0] = w0;
    }
    if (insetPair == 0) {
        insetPair = (u32*)((u8*)cache + 0x4a8);
    }

    stack[0] = (s16)(cache->rect4A8[0] - cache->rect4B0[0]);
    stack[1] = (s16)(cache->rect4A8[1] - cache->rect4B0[1]);
    stack[2] = cache->rect4B0[2];
    stack[3] = cache->rect4B0[3];
    *(s16*)((u8*)cache + 0x4bc) = stack[2];
    *(s16*)((u8*)cache + 0x4be) = stack[3];

    i = 0;
    for (u32 n = cache->mSize; n != 0; n--) {
        idx = cache->mFront + i;
        slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
        if (cache->mArrayPtr[slot].command == 0xb) {
            goto found_b;
        }
        i++;
    }
    i = (u32)-1;
found_b:
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    cache->mArrayPtr[slot].unk23 = 0xd;
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    entry = &cache->mArrayPtr[slot];
    entry->wid = *(u32*)&stack[0];
    entry->unk8 = *(u32*)&stack[2];
    {
        // Force a fresh this+4 for each call (no CSE into saved r31).
        void* msg = (u8*)self + 4;
        func_8044CE68__8CGXCacheFv(msg, 0xb);
    }

    i = 0;
    for (u32 n = cache->mSize; n != 0; n--) {
        idx = cache->mFront + i;
        slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
        if (cache->mArrayPtr[slot].command == 0xc) {
            goto found_c;
        }
        i++;
    }
    i = (u32)-1;
found_c:
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    cache->mArrayPtr[slot].unk23 = 0xd;
    idx = cache->mFront + i;
    slot = idx - (idx / cache->mCapacity) * cache->mCapacity;
    entry = &cache->mArrayPtr[slot];
    entry->wid = insetPair[0];
    entry->unk8 = insetPair[1];
    {
        void* msg = (u8*)self + 4;
        func_8044CE68__8CGXCacheFv(msg, 0xc);
    }
}
// LLM-HARNESS-END: us-8044dd08

// LLM-HARNESS-BEGIN: us-8044df28
extern "C" void func_8044B4B8__8CGXCacheFP9_GXTexObjUsUs() {}
// LLM-HARNESS-END: us-8044df28

// LLM-HARNESS-BEGIN: us-8044e024
extern "C" void func_8044B5B4__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044e024

// LLM-HARNESS-BEGIN: us-8044e030
extern "C" void func_8044B5C0__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044e030

// LLM-HARNESS-BEGIN: us-8044e0d0
extern "C" void func_8044B660__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044e0d0

// LLM-HARNESS-BEGIN: us-8044e33c
extern "C" void func_8044B8CC__8CGXCacheFfff() {}
// LLM-HARNESS-END: us-8044e33c

// LLM-HARNESS-BEGIN: us-8044e590
extern "C" void func_8044BB20__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044e590

// LLM-HARNESS-BEGIN: us-8044e7e4
extern "C" void func_8044BD74__8CGXCacheFi() {}
// LLM-HARNESS-END: us-8044e7e4

// LLM-HARNESS-BEGIN: us-8044e880
extern "C" void func_8044BE10__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044e880

// LLM-HARNESS-BEGIN: us-8044e88c
extern "C" void* func_8044BE1C__8CGXCacheFv(void* self) { return (void*)((u8*)self + 0x510); }
// LLM-HARNESS-END: us-8044e88c

// LLM-HARNESS-BEGIN: us-8044e894
extern "C" u8 func_8044BE24__8CGXCacheFv(void* self) { return ((u8*)self)[0x518]; }
// LLM-HARNESS-END: us-8044e894

// LLM-HARNESS-BEGIN: us-8044e89c
extern "C" void func_8044BE2C__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044e89c

// LLM-HARNESS-BEGIN: us-8044e8a8
// Retail is a pure tail to func_8044C1FC. Keep callee undefined here so MWCC
// cannot inline an empty same-TU stub into a lone blr.
extern "C" void func_8044C1FC__8CGXCacheFv(void* self);
extern "C" void func_8044BE38__8CGXCacheFv(void* self) {
    func_8044C1FC__8CGXCacheFv(self);
}
// LLM-HARNESS-END: us-8044e8a8

// LLM-HARNESS-BEGIN: us-8044e8ac
extern "C" void func_8044BE3C__8CGXCacheFv(void* self) {
    (void)self;
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX4, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX5, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX6, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX7, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX2x4, (GXTexGenSrc)8, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD5, GX_TG_MTX2x4, (GXTexGenSrc)9, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD6, GX_TG_MTX2x4, (GXTexGenSrc)10, 0x3C, GX_DISABLE, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD7, GX_TG_MTX2x4, (GXTexGenSrc)11, 0x3C, GX_DISABLE, 0x7D);
    GXSetNumTexGens(1);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD1, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD2, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD3, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD4, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD5, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD6, GX_DISABLE, GX_DISABLE);
    GXEnableTexOffsets(GX_TEXCOORD7, GX_DISABLE, GX_DISABLE);
}
// LLM-HARNESS-END: us-8044e8ac

// LLM-HARNESS-BEGIN: us-8044ea30
extern "C" void func_8044BFC0__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044ea30

// LLM-HARNESS-BEGIN: us-8044eaa4
extern "C" void func_8044C034__8CGXCacheFv(void* self) {
    (void)self;
    u32 tevStage;
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    tevStage = 0;
    while (tevStage < 0x10) {
        GXSetTevKColorSel((GXTevStageID)tevStage, (GXTevKColorSel)6);
        GXSetTevKAlphaSel((GXTevStageID)tevStage, GX_TEV_KASEL_1);
        GXSetTevSwapMode((GXTevStageID)tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        tevStage++;
    }
    tevStage = 0;
    while (tevStage < 0x10) {
        GXSetTevDirect((GXTevStageID)tevStage);
        tevStage++;
    }
}
// LLM-HARNESS-END: us-8044eaa4

// LLM-HARNESS-BEGIN: us-8044ec6c
extern "C" {
GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
extern const f32 lbl_eu_8066A37C;
extern const f32 lbl_eu_8066A3A0;
extern const f32 lbl_eu_8066A3A4;
extern u32 lbl_eu_80663644;
void func_8044BE3C__8CGXCacheFv(void* self);
void func_8044C034__8CGXCacheFv(void* self);
void func_8044CE68__8CGXCacheFv(void* self, u32 cmd);
void* func_8044CEF8__8CGXCacheFv(void* self, u32 cmd);
}

#pragma dont_inline on
extern "C" void func_8044C1FC__8CGXCacheFv(void* self) {
    C1FCCacheLayout* cache = (C1FCCacheLayout*)self;
    void* msgSelf = (u8*)self + 4;
    GXColor gxCol;
    GXRenderModeObj* rmo;
    f32 yScale;
    u16 halfVi;
    u32 fieldMode;
    ml::CCol4* pCol;
    u32 zTex;
    s16 d0;
    s16 d1;
    s16 stack4A8[4];
    s16* stackScissor;
    f32 scale;

    GXSetCoPlanar(GX_DISABLE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    func_8044BE3C__8CGXCacheFv(self);
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
    GXClearVtxDesc();

    GXLoadPosMtxImm(*ml::CMat34::identity, GX_PNMTX0);
    GXLoadNrmMtxImm(*ml::CMat34::identity, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadTexMtxImm(*ml::CMat34::identity, 0x3C, GX_MTX_3x4);
    GXLoadTexMtxImm(*ml::CMat34::identity, 0x7D, GX_MTX_3x4);

    scale = lbl_eu_8066A37C;
    gxCol.r = (u8)(s32)(scale * ml::CCol4::red.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::red.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::red.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::red.a);
    GXSetFog(GX_FOG_NONE, gxCol, lbl_eu_8066A3A0, lbl_eu_8066A3A4, lbl_eu_8066A37C, lbl_eu_8066A3A4);

    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);

    gxCol.r = (u8)(s32)(scale * ml::CCol4::black.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::black.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::black.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::black.a);
    GXSetChanAmbColor(GX_COLOR0A0, gxCol);
    gxCol.r = (u8)(s32)(scale * ml::CCol4::white.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::white.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::white.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::white.a);
    GXSetChanMatColor(GX_COLOR0A0, gxCol);
    gxCol.r = (u8)(s32)(scale * ml::CCol4::black.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::black.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::black.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::black.a);
    GXSetChanAmbColor(GX_COLOR1A1, gxCol);
    gxCol.r = (u8)(s32)(scale * ml::CCol4::white.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::white.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::white.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::white.a);
    GXSetChanMatColor(GX_COLOR1A1, gxCol);

    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

    func_8044C034__8CGXCacheFv(self);

    GXSetNumIndStages(0);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE2, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE3, GX_ITS_1, GX_ITS_1);

    rmo = getRenderModeObj__9CDeviceVIFv();
    yScale = GXGetYScaleFactor(rmo->efbHeight, rmo->xfbHeight);
    GXSetDispCopyYScale(yScale);
    halfVi = (u16)(rmo->xfbHeight << 1);
    fieldMode = rmo->viHeight - halfVi;
    fieldMode = fieldMode ? 1 : 0;
    rmo = getRenderModeObj__9CDeviceVIFv();
    GXSetFieldMode(rmo->field_rendering, fieldMode);
    rmo = getRenderModeObj__9CDeviceVIFv();
    GXSetDispCopySrc(0, 0, rmo->fbWidth, rmo->efbHeight);
    rmo = getRenderModeObj__9CDeviceVIFv();
    GXSetDispCopyDst(rmo->fbWidth, (u16)yScale);
    GXSetDispCopyGamma(cache->unk50C);

    pCol = (ml::CCol4*)func_8044CEF8__8CGXCacheFv(msgSelf, 3);
    zTex = *(u32*)func_8044CEF8__8CGXCacheFv(msgSelf, 2);
    gxCol.r = (u8)(s32)(scale * pCol->r);
    gxCol.g = (u8)(s32)(scale * pCol->g);
    gxCol.b = (u8)(s32)(scale * pCol->b);
    gxCol.a = (u8)(s32)(scale * pCol->a);
    GXSetCopyClear(gxCol, zTex);

    rmo = getRenderModeObj__9CDeviceVIFv();
    stack4A8[0] = 0;
    stack4A8[1] = 0;
    stack4A8[2] = (s16)rmo->fbWidth;
    stack4A8[3] = (s16)rmo->efbHeight;
    *(u32*)&cache->rect4A8[0] = *(u32*)&stack4A8[0];
    *(u32*)&cache->rect4A8[2] = *(u32*)&stack4A8[2];
    *(u32*)&cache->rect4B0[0] = *(u32*)&stack4A8[0];
    *(u32*)&cache->rect4B0[2] = *(u32*)&stack4A8[2];
    stackScissor = stack4A8;
    d0 = cache->rect4A8[0] - cache->rect4B0[0];
    d1 = cache->rect4A8[1] - cache->rect4B0[1];
    cache->rect4B0[2] = stack4A8[2];
    cache->rect4B0[3] = stack4A8[3];

    {
        s32 found_B = -1;
        u32 i_B;
        for (i_B = 0; i_B < cache->mSize; i_B++) {
            u32 idx_B = cache->mFront + i_B;
            u32 slot_B = idx_B - (idx_B / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_B].command == 0xB) {
                found_B = (s32)i_B;
                break;
            }
        }
        {
            u32 idx2_B = cache->mFront + (u32)found_B;
            u32 slot2_B = idx2_B - (idx2_B / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_B = &cache->mArrayPtr[slot2_B];
            e_B->unk23 = 0x0D;
            e_B->unk8 = (u32)d0;
            e_B->unk10 = (u32)d1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0xB);
    }

    {
        s32 found_C = -1;
        u32 i_C;
        for (i_C = 0; i_C < cache->mSize; i_C++) {
            u32 idx_C = cache->mFront + i_C;
            u32 slot_C = idx_C - (idx_C / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_C].command == 0xC) {
                found_C = (s32)i_C;
                break;
            }
        }
        {
            u32 idx2_C = cache->mFront + (u32)found_C;
            u32 slot2_C = idx2_C - (idx2_C / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_C = &cache->mArrayPtr[slot2_C];
            e_C->unk23 = 0x0D;
            e_C->unk8 = *(u32*)&stackScissor[0];
            e_C->unk10 = *(u32*)&stackScissor[2];
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0xC);
    }

    {
        s32 found_4 = -1;
        u32 i_4;
        for (i_4 = 0; i_4 < cache->mSize; i_4++) {
            u32 idx_4 = cache->mFront + i_4;
            u32 slot_4 = idx_4 - (idx_4 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_4].command == 0x4) {
                found_4 = (s32)i_4;
                break;
            }
        }
        {
            u32 idx2_4 = cache->mFront + (u32)found_4;
            u32 slot2_4 = idx2_4 - (idx2_4 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_4 = &cache->mArrayPtr[slot2_4];
            e_4->unk23 = 0x03;
            e_4->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x4);
    }

    {
        s32 found_5 = -1;
        u32 i_5;
        for (i_5 = 0; i_5 < cache->mSize; i_5++) {
            u32 idx_5 = cache->mFront + i_5;
            u32 slot_5 = idx_5 - (idx_5 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_5].command == 0x5) {
                found_5 = (s32)i_5;
                break;
            }
        }
        {
            u32 idx2_5 = cache->mFront + (u32)found_5;
            u32 slot2_5 = idx2_5 - (idx2_5 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_5 = &cache->mArrayPtr[slot2_5];
            e_5->unk23 = 0x04;
            *(u8*)&e_5->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x5);
    }

    {
        s32 found_6 = -1;
        u32 i_6;
        for (i_6 = 0; i_6 < cache->mSize; i_6++) {
            u32 idx_6 = cache->mFront + i_6;
            u32 slot_6 = idx_6 - (idx_6 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_6].command == 0x6) {
                found_6 = (s32)i_6;
                break;
            }
        }
        {
            u32 idx2_6 = cache->mFront + (u32)found_6;
            u32 slot2_6 = idx2_6 - (idx2_6 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_6 = &cache->mArrayPtr[slot2_6];
            e_6->unk23 = 0x04;
            *(u8*)&e_6->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x6);
    }

    {
        s32 found_7 = -1;
        u32 i_7;
        for (i_7 = 0; i_7 < cache->mSize; i_7++) {
            u32 idx_7 = cache->mFront + i_7;
            u32 slot_7 = idx_7 - (idx_7 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_7].command == 0x7) {
                found_7 = (s32)i_7;
                break;
            }
        }
        {
            u32 idx2_7 = cache->mFront + (u32)found_7;
            u32 slot2_7 = idx2_7 - (idx2_7 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_7 = &cache->mArrayPtr[slot2_7];
            e_7->unk23 = 0x04;
            *(u8*)&e_7->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x7);
    }

    {
        s32 found_8 = -1;
        u32 i_8;
        for (i_8 = 0; i_8 < cache->mSize; i_8++) {
            u32 idx_8 = cache->mFront + i_8;
            u32 slot_8 = idx_8 - (idx_8 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_8].command == 0x8) {
                found_8 = (s32)i_8;
                break;
            }
        }
        {
            u32 idx2_8 = cache->mFront + (u32)found_8;
            u32 slot2_8 = idx2_8 - (idx2_8 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_8 = &cache->mArrayPtr[slot2_8];
            e_8->unk23 = 0x04;
            *(u8*)&e_8->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x8);
    }

    {
        s32 found_0 = -1;
        u32 i_0;
        for (i_0 = 0; i_0 < cache->mSize; i_0++) {
            u32 idx_0 = cache->mFront + i_0;
            u32 slot_0 = idx_0 - (idx_0 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_0].command == 0x0) {
                found_0 = (s32)i_0;
                break;
            }
        }
        {
            u32 idx2_0 = cache->mFront + (u32)found_0;
            u32 slot2_0 = idx2_0 - (idx2_0 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_0 = &cache->mArrayPtr[slot2_0];
            e_0->unk23 = 0x03;
            e_0->unk8 = 1;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x0);
    }

    {
        s32 found_9 = -1;
        u32 i_9;
        for (i_9 = 0; i_9 < cache->mSize; i_9++) {
            u32 idx_9 = cache->mFront + i_9;
            u32 slot_9 = idx_9 - (idx_9 / cache->mCapacity) * cache->mCapacity;
            if (cache->mArrayPtr[slot_9].command == 0x9) {
                found_9 = (s32)i_9;
                break;
            }
        }
        {
            u32 idx2_9 = cache->mFront + (u32)found_9;
            u32 slot2_9 = idx2_9 - (idx2_9 / cache->mCapacity) * cache->mCapacity;
            C1FCMsgEntry* e_9 = &cache->mArrayPtr[slot2_9];
            e_9->unk23 = 0x03;
            e_9->unk8 = lbl_eu_80663644;
        }
        func_8044CE68__8CGXCacheFv(msgSelf, 0x9);
    }

    gxCol.r = (u8)(s32)(scale * ml::CCol4::white.r);
    gxCol.g = (u8)(s32)(scale * ml::CCol4::white.g);
    gxCol.b = (u8)(s32)(scale * ml::CCol4::white.b);
    gxCol.a = (u8)(s32)(scale * ml::CCol4::white.a);
    GXSetTevColor(GX_TEVREG1, gxCol);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8044ec6c

// LLM-HARNESS-BEGIN: us-8044f8d8
struct MsgParam32Entry {
    u32 command;
    u32 wid;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

struct MsgParam32Ring {
    u8 pad[0x484];
    MsgParam32Entry* mArrayPtr;
    u32 mFront;
    u32 mSize;
    u32 mCapacity;
    u32 field6;
    void* field7;
};

extern "C" void func_8044CE68__8CGXCacheFv(void* self, u32 cmd) {
    MsgParam32Ring* ring = (MsgParam32Ring*)self;
    u32 i = 0;
    u32 n;

    // `for (n = size; n != 0; n--)` emits retail mtctr/bdnz (~86%).
    for (n = ring->mSize; n != 0; n--) {
        u32 idx = ring->mFront + i;
        u32 slot = idx - (idx / ring->mCapacity) * ring->mCapacity;
        if (ring->mArrayPtr[slot].command == cmd) {
            goto dispatch;
        }
        i++;
    }
    i = (u32)-1;
dispatch:
    {
        void* obj = ring->field7;
        void** vtbl = *(void***)obj;
        u32 idx = ring->mFront + i;
        u32 slot = idx - (idx / ring->mCapacity) * ring->mCapacity;
        // Retail: r3=obj, r4=cmd, r5=entry+4, then bctr vtbl[3].
        ((void (*)(void*, u32, void*))vtbl[3])(obj, cmd, (u8*)&ring->mArrayPtr[slot] + 4);
    }
}
// LLM-HARNESS-END: us-8044f8d8

// LLM-HARNESS-BEGIN: us-8044f968
extern "C" void* func_8044CEF8__8CGXCacheFv(void* self, u32 cmd) {
    MsgParam32Ring* ring = (MsgParam32Ring*)self;
    u32 i = 0;
    u32 n;

    for (n = ring->mSize; n != 0; n--) {
        u32 idx = ring->mFront + i;
        u32 slot = idx - (idx / ring->mCapacity) * ring->mCapacity;
        if (ring->mArrayPtr[slot].command == cmd) {
            goto found_entry;
        }
        i++;
    }
    i = (u32)-1;
found_entry:
    {
        u32 idx = ring->mFront + i;
        u32 slot = idx - (idx / ring->mCapacity) * ring->mCapacity;
        return (u8*)&ring->mArrayPtr[slot] + 4;
    }
}
// LLM-HARNESS-END: us-8044f968
extern "C" void func_8044CF74__8CGXCacheFv() {}
// LLM-HARNESS-END: us-8044f9e4

// --- hard-symbol stubs (scaffold_hard_symbols) ---
template <int N> class CMsgParam {
public:
    void func_804495C4(unsigned long);
    void func_8044972C(unsigned long);
    void func_804498A4(unsigned long);
    void func_80449A1C();
    void func_80449B94(unsigned long);
};
// LLM-HARNESS-BEGIN: us-8044c034
template <> void CMsgParam<32>::func_804495C4(unsigned long) {}
// LLM-HARNESS-END: us-8044c034
// LLM-HARNESS-BEGIN: us-8044c19c
template <> void CMsgParam<32>::func_8044972C(unsigned long) {}
// LLM-HARNESS-END: us-8044c19c
// LLM-HARNESS-BEGIN: us-8044c314
template <> void CMsgParam<32>::func_804498A4(unsigned long) {}
// LLM-HARNESS-END: us-8044c314
// LLM-HARNESS-BEGIN: us-8044c48c
template <> void CMsgParam<32>::func_80449A1C() {}
// LLM-HARNESS-END: us-8044c48c
// LLM-HARNESS-BEGIN: us-8044c604
template <> void CMsgParam<32>::func_80449B94(unsigned long) {}
// LLM-HARNESS-END: us-8044c604

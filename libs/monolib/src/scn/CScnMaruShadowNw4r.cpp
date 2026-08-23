// CScnMaruShadowNw4r -- scene shadow rendering (nw4r framework)
//
// Translation unit contains:
//   - CScnMaruShadowNw4r constructor/destructor
//   - Shadow property accessors and configuration helpers
//   - Static initializer registered in .ctors
//
// FULL_MATCH functions:
//   func_8048EA9C -- returns 1 (capability check / stub)
//   sinit_8048EAA4 -- static initializer (empty, .ctors registration)

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnMaruShadowNw4r.hpp"

#include <cmath>
#include <nw4r/math/math_arithmetic.h>
#include <nw4r/math/math_triangular.h>

void func_8048DD18(CScnMaruShadowNw4r* self, ShadowCtx* ctx, float f1, float f2, u32 unk);

// func_8048D264 support: static-data loader + nw4r g3d texture accessors.
extern "C" int getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
    const char* name, void* handle, u32* size);
extern "C" void Init__Q34nw4r3g3d7ResFileFv(void* res);
extern "C" void* GetResTex__Q34nw4r3g3d7ResFileCFPCc(void* res, const char* name);
extern "C" void GetTexObjParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP9_GXTexFmtPfPfPUc(
    void* tex, void** imgPtr, u16* width, u16* height, s32* fmt, float* minLod,
    float* maxLod, u8* lodBias);
extern "C" void func_80496120(void* self, void* mtx, int id);

extern const char lbl_eu_80523F94[];  // shadow static-data resource name
extern const char lbl_eu_8052637C[];  // panic site file
extern const char lbl_eu_80526354[];  // panic site fmt
extern const char* lbl_eu_80663958;   // texture resource name

// func_8048D264 local storage: StaticDataHandle is the CLibStaticData handle
// (first word = loaded data pointer).
struct ShadowStaticDataHandle {
    void* data;
};

extern "C" u32 lbl_eu_8056E598[8];  // CScnMaruShadowNw4r vtable (defined below)


// us-80491238: __ct__CScnMaruShadowNw4r (constructor, 0x60 bytes)
// OPEN ITEM (75%, scheduling-only residual): retail order is
//   [li r5,0][lfs f1,A944][lfs f0,A948][lis vtbl@ha][lfs f2,A940][addi vtbl@lo]
//   [ori r0,r5,1][li r4,128]
// vs decomp [lfs f0,A948][lis][li r5,0][lfs f1,A944][lfs f2,A940][addi]
//   [li r4,128][li r0,1]. Same instruction multiset; differs in load emission
// order/colors (A944 emitted first but colored f1), li r5 hoist position, and
// ori-vs-li for the flags constant. Tried: flags-var-first + `flags|1`
// (58.3%), zero-stores-first (16.7%) - both worse, reverted. Emission order
// follows declaration order, so A944-first+f1-color is unreachable via decl
// steering alone; likely needs the true original statement interleaving.
void __ct__CScnMaruShadowNw4r(CScnMaruShadowNw4r* self) {
    const float b = lbl_eu_8066A948;
    const float a = lbl_eu_8066A944;
    const float c = lbl_eu_8066A940;

    self->vtable = (u32)&lbl_eu_8056E598;
    self->field_0x08 = c;
    self->field_0x18 = a;
    self->field_0x1c = b;
    self->field_0x20 = a;
    self->field_0x24 = b;
    self->field_0x2a = 0;
    self->field_0x2b = 0;
    self->field_0x4c = 0;
    self->field_0x50 = 0;
    self->field_0x06 = 0;
    self->field_0x05 = 0;
    self->field_0x04 = 0;
    self->field_0x07 = 0x80;
    self->field_0x28 = 1;
}

// us-80491298: __dt__4CScnFvMaruShadowNw4r (destructor, 0x40 bytes)
extern "C" CScn* __dt__4CScnFvMaruShadowNw4r(CScn* self, int flags) {
    if (self != 0) {
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// us-804912d8: func_8048D264 (0x29C bytes)
// Shadow-texture GX pipeline setup: loads the shadow texture from static data,
// builds a GXTexObj, configures the fixed-function pipeline (1 TEV stage,
// alpha blend, texcoord gen), and returns 1 when the path is ready.
int func_8048D264(void* self) {
    // NOTE: MWCC allocates stack homes per size-class in REVERSE declaration
    // order; this order reproduces the retail frame layout
    // (bias@8, w@10, h@12, resTex@16, img@20, minLod@24, maxLod@28,
    //  fmt@32, resFile@36, handle@40, texObj@48, projMtx@80).
    u8 lodBias;
    u16 texH;
    u16 texW;
    ShadowStaticDataHandle handle;
    void* resFile;
    s32 texFmt;
    float maxLod;
    float minLod;
    void* imgPtr;
    void* resTex;
    GXTexObj texObj;
    Mtx44 projMtx;

    if (getStaticFileData__14CLibStaticDataFPCcP16StaticDataHandlePUl(
            lbl_eu_80523F94, &handle, NULL) != 0) {
        resFile = handle.data;
        if ((u32)resFile & 0x1F) {
            Panic__Q24nw4r2dbFPCciPCce(lbl_eu_8052637C, 60, lbl_eu_80526354);
        }
        Init__Q34nw4r3g3d7ResFileFv(&resFile);
        resTex = GetResTex__Q34nw4r3g3d7ResFileCFPCc(&resFile, lbl_eu_80663958);
        GetTexObjParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP9_GXTexFmtPfPfPUc(
            &resTex, &imgPtr, &texW, &texH, &texFmt, &minLod, &maxLod, &lodBias);

        GXInitTexObj(&texObj, imgPtr, texW, texH, (GXTexFmt)texFmt,
                     (GXTexWrapMode)0, (GXTexWrapMode)0, lodBias);
        GXInitTexObjLOD(&texObj, (GXTexFilter)1, (GXTexFilter)1, lbl_eu_8066A948,
                        lbl_eu_8066A948, lbl_eu_8066A948, (u8)0, (u8)0,
                        (GXAnisotropy)0);

        GXClearVtxDesc();
        GXSetVtxDesc((GXAttr)9, (GXAttrType)1);
        GXSetVtxDesc((GXAttr)13, (GXAttrType)1);
        GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)9, (GXCompCnt)1, (GXCompType)4, 0);
        GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)13, (GXCompCnt)1, (GXCompType)0, 0);
        GXSetCullMode((GXCullMode)0);
        GXSetZMode(1, (GXCompare)3, 0);
        GXSetAlphaCompare((GXCompare)4, 0, (GXAlphaOp)0, (GXCompare)3, 255);
        GXSetAlphaUpdate(0);
        GXSetBlendMode((GXBlendMode)1, (GXBlendFactor)4, (GXBlendFactor)5,
                       (GXLogicOp)0);
        GXSetChanCtrl((GXChannelID)4, (u8)0, (GXColorSrc)0, (GXColorSrc)0,
                      (GXLightID)0, (GXDiffuseFn)0, (GXAttnFn)2);
        GXSetNumTevStages(1);
        GXSetNumIndStages(0);
        GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)0,
                      (GXChannelID)4);
        GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)15, (GXTevColorArg)15,
                        (GXTevColorArg)15, (GXTevColorArg)10);
        GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 1,
                        (GXTevRegID)0);
        GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)5, (GXTevAlphaArg)7,
                        (GXTevAlphaArg)4, (GXTevAlphaArg)7);
        GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, 1,
                        (GXTevRegID)0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2((GXTexCoordID)0, (GXTexGenType)1, (GXTexGenSrc)4, 60, 0,
                          125);
        GXLoadTexObj(&texObj, (GXTexMapID)0);

        func_80496120(self, projMtx, -1);
        GXSetProjection(projMtx, (GXProjectionType)0);
        GXSetCurrentMtx(0);
        return 1;
    }
    return 0;
}


// us-80491574: func_8048D500 (0x658 bytes)
// Per-frame maru-shadow update/draw driver.
void func_8048D500(CScnMaruShadowNw4r* self, ShadowCtx* ctx) {
    u16 flags = self->field_0x28;
    if ((flags & 1) == 0 || (flags & 0x40) != 0) {
        return;
    }
    func_804BC9EC();
    if (func_804BCC10() == 0 || ctx == NULL) {
        return;
    }

    u32 cfg = ctx->field_0x7a4;
    bool cond1 = ((cfg & 2) == 0 && (cfg & 0x8000) == 0);
    bool cond2 = cond1 && (cfg & 0x4000) == 0;
    if (!cond2) {
        return;
    }

    mlCVec3 pos;
    pos.x = ctx->field_0x2DC;
    pos.y = ctx->field_0x2E0 + lbl_eu_8066A994;
    pos.z = ctx->field_0x2E4;
    float fscale = lbl_eu_8066A990 * ctx->field_0x2E8;

    float anchorY = 0.0f;
    if ((self->field_0x28 & 0x10) != 0) {
        fscale = lbl_eu_8066A944 + ctx->field_0x2E8;
        anchorY = self->field_0x14;
        self->field_0x28 &= ~0x40;
    } else {
        u32 cfg2 = ctx->field_0x7a4;
        if ((cfg2 & 0x20) == 0 && (cfg2 & 0x100) == 0) {
            void* p = ctx->field_0x14AC;
            fscale = lbl_eu_8066A998;
            if (p != NULL) {
                pos.x = *(float*)((char*)p + 0xC);
                pos.y = *(float*)((char*)p + 0x1C);
                pos.z = *(float*)((char*)p + 0x2C);
            }
            pos.y += lbl_eu_8066A994;
        } else {
            if (!func_804BE398(&pos, -fscale, lbl_eu_8066A948, 0, 0)) {
                return;
            }
            void* hit = func_804BE50C(0);
            anchorY = *(float*)((char*)hit + 4);
        }
    }

    // Ground offset fades the shadow with height above the anchor plane.
    self->field_0x20 = lbl_eu_8066A944;
    float lim = lbl_eu_8066A99C * fscale;
    float d = ctx->field_0x2E0 - anchorY;
    if (d > lim) {
        float t = (d - lbl_eu_8066A99C * fscale) / (lbl_eu_8066A9A0 * fscale);
        self->field_0x20 = lbl_eu_8066A944 - t;
        if (self->field_0x20 < 0.0f) {
            self->field_0x20 = 0.0f;
        }
    }

    // Vertical velocity integration with wrap-around clamps.
    float vy = self->field_0x1c;
    if (vy > 0.0f) {
        self->field_0x18 += vy;
        if ((double)self->field_0x18 >= lbl_eu_8066A9A8) {
            self->field_0x18 = lbl_eu_8066A944;
            self->field_0x1c = lbl_eu_8066A948;
        }
    } else if (vy < 0.0f) {
        self->field_0x18 += vy;
        if ((double)self->field_0x18 <= lbl_eu_8066A9B0) {
            self->field_0x18 = lbl_eu_8066A948;
            self->field_0x1c = lbl_eu_8066A948;
        }
    }

    // Build the ambient/material color with animated alpha.
    u8 rgba[4];
    rgba[0] = self->field_0x04;
    rgba[1] = self->field_0x05;
    rgba[2] = self->field_0x06;
    rgba[3] = (u8)(s32)((double)self->field_0x07 * (double)self->field_0x18 *
                        (double)self->field_0x20);

    bool draw = true;
    if ((self->field_0x28 & 2) != 0) {
        void* rec = func_804961D4(ctx->field_0x04, -1);
        float cx = *(float*)((char*)rec + 0xC);
        float cy = *(float*)((char*)rec + 0x1C);
        float cz = *(float*)((char*)rec + 0x2C);
        void* obj = func_8048315C(ctx);
        float dx = *(float*)((char*)obj + 0xB8) - cx;
        float dy = *(float*)((char*)obj + 0xBC) - cy;
        float dz = *(float*)((char*)obj + 0xC0) - cz;
        float dist = (float)sqrt((double)(dx * dx + dy * dy + dz * dz));
        if (dist > self->field_0x0C) {
            rgba[3] = 0;
        } else if (dist > self->field_0x10) {
            float t = (dist - self->field_0x0C) / (self->field_0x10 - self->field_0x0C);
            rgba[3] = (u8)(s32)((double)rgba[3] * (1.0 - (double)t));
        }
    }
    if (rgba[3] != 0) {
        GXColor col;
        col.r = rgba[0]; col.g = rgba[1]; col.b = rgba[2]; col.a = rgba[3];
        GXSetChanAmbColor(GX_COLOR0, col);
        GXSetChanMatColor(GX_COLOR0, col);
    } else {
        draw = false;
    }
    if (!draw) {
        return;
    }

    float baseScale = self->field_0x08 * ctx->field_0x304;
    if ((self->field_0x28 & 0x10) != 0) {
        // One quad per entry in the per-node table.
        u8 n = self->field_0x2b;
        MaruShadowEntry* e = self->entries;
        for (int j = 0; j < n; j++) {
            double raw = (double)e[j].scaleQ;
            float s = lbl_eu_8066A9B8 * (float)(baseScale * (raw));
            func_8048DD18(self, ctx, s, fscale, e[j].nodeId);
        }
        return;
    }

    nw4r::g3d::ResMdl mdl = ctx->resMdl;
    const char** names = (const char**)(&lbl_eu_8056E578);
    if (self->field_0x2a == 2) {
        nw4r::g3d::ResNode node = GetResNode__Q34nw4r3g3d6ResMdlCFPCc(mdl, names[5]);
        if (node.ptr == NULL) {
            Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80529678, 0x53, lbl_eu_80529658);
        }
        if (node.ptr != NULL) {
            func_8048DD18(self, ctx, baseScale, fscale, *(u32*)((char*)node.ptr + 0x10));
        }
        node = GetResNode__Q34nw4r3g3d6ResMdlCFPCc(mdl, names[6]);
        if (node.ptr == NULL) {
            Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80529678, 0x53, lbl_eu_80529658);
        }
        if (node.ptr != NULL) {
            func_8048DD18(self, ctx, baseScale, fscale, *(u32*)((char*)node.ptr + 0x10));
        }
        return;
    }
    if (self->field_0x2a == 1 && (self->field_0x28 & 4) != 0) {
        func_8048E2F8(self, ctx, baseScale * lbl_eu_80523F78[0]);
        return;
    }
    int n = (self->field_0x2a == 1) ? 1 : 7;
    for (int i = 0; i < n; i++) {
        nw4r::g3d::ResNode node = GetResNode__Q34nw4r3g3d6ResMdlCFPCc(mdl, names[i]);
        if (node.ptr == NULL) {
            Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80529678, 0x53, lbl_eu_80529658);
        }
        if (node.ptr != NULL) {
            func_8048DD18(self, ctx, baseScale * lbl_eu_80523F78[i], fscale,
                          *(u32*)((char*)node.ptr + 0x10));
        }
    }
}

// us-80491bcc: func_8048DB58 (0x18 bytes)
extern "C" void func_8048DB58(u8* self, const void* other) {
    *(u16*)((u8*)self + 0x28) |= 0x40;
    *(float*)((u8*)self + 0x14) = *(float*)((u8*)other + 4);
}

// us-80491be4: func_8048DB70 (0x1A8 bytes)
void func_8048DB70(){}

// us-80491d8c: func_8048DD18 (0x5E0 bytes)
// Draws one maru-shadow quad: either the projected shadow-map path or the
// billboard path depending on self->field_0x28 bit 0x10.
void func_8048DD18(CScnMaruShadowNw4r* self, ShadowCtx* ctx, float f1, float f2, u32 unk) {
    u32 flags = self->field_0x28;
    float shadowScale = lbl_eu_80663964;
    u8* recBase = *(u8**)((char*)ctx->field_0x147C + 0xEC);
    u8* rec = recBase + (flags & 0x10);

    if ((flags & 0x10) != 0) {
        // Projected texture path: scale/offset from the render record.
        float sx = *(float*)(rec + 0xC);
        float ty = *(float*)(rec + 0x2C);
        float tzOff = *(float*)(rec + 0x1C);
        float* gvec = (float*)lbl_eu_80658638;
        float depthScale = lbl_eu_8066A944 / (lbl_eu_8066A990 * f1);
        gvec[0] = sx;
        gvec[1] = ty;
        gvec[2] = tzOff - lbl_eu_8066A9C0 * f2;
        lbl_eu_806658E0 = depthScale;

        const float k = lbl_eu_8066A944;
        const float zero = lbl_eu_8066A948;
        float mtx[3][4];
        mtx[0][0] = k; mtx[0][1] = zero; mtx[0][2] = zero; mtx[0][3] = sx;
        mtx[1][0] = zero; mtx[1][1] = k; mtx[1][2] = zero; mtx[1][3] = ty;
        mtx[2][0] = zero; mtx[2][1] = zero; mtx[2][2] = k; mtx[2][3] = tzOff - lbl_eu_8066A9C0 * f2;

        void* posMtx = func_8049621C(ctx->field_0x04, -1);
        GXLoadPosMtxImm((const float (*)[4])posMtx, GX_PNMTX0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, (GXCompType)4, 0);

        float smtx[3][4];
        smtx[0][0] = k; smtx[0][1] = zero; smtx[0][2] = zero;
        smtx[1][0] = zero; smtx[1][1] = k; smtx[1][2] = zero;
        smtx[2][0] = zero; smtx[2][1] = zero; smtx[2][2] = k;
        func_804BE3E0(&func_8048DB70, smtx, 0x00044A05, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)0xD, GX_TEX_ST, (GXCompType)0, 0);
        return;
    }

    // Billboard path.
    float vx = *(float*)(rec + 0xC);
    float vy = *(float*)(rec + 0x1C);
    float vz = *(float*)(rec + 0x2C);
    float topY = vy + lbl_eu_8066A994;
    float v1[3] = { vx, vy, vz };
    float v2[3] = { vx, topY, vz };

    mlCVec3 up;
    mlCVec3 dir;
    if ((flags & 0x100) != 0) {
        self->field_0x28 &= ~0x100;
        dir.x = v2[0]; dir.y = v2[1]; dir.z = v2[2];
        up = unitY__Q22ml5CVec3;
    } else {
        mlCVec3 vv;
        vv.x = v2[0]; vv.y = v2[1]; vv.z = v2[2];
        if (!func_804BE398(&vv, -f1, lbl_eu_8066A948, 0, 0)) {
            return;
        }
        func_804BE4E0(&up, 0);
        if (((flags & 0x8000) != 0) && !(up.y >= self->field_0x24)) {
            return;
        }
        func_804BE4B4(&dir, 0);
        dir.x = v1[0];
    }

    float lenSq = up.x * up.x + up.z * up.z;
    if (lenSq <= lbl_eu_8066A948) {
        Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    float len = 0.0f;
    if (lenSq > 0.0f) {
        float inv = nw4r::math::FrSqrt(lenSq);
        len = lenSq * inv;
    }

    float ang1 = nw4r::math::Atan2FIdx(up.y, 0.0f) * lbl_eu_8066A9C4;
    float ang2 = nw4r::math::Atan2FIdx(up.x, ang1) * lbl_eu_8066A9C4;
    (void)ang2;
    float s1 = nw4r::math::SinFIdx(-ang1 * lbl_eu_8066A9C8);
    float c1 = nw4r::math::CosFIdx(-ang1 * lbl_eu_8066A9C8);
    (void)s1; (void)c1;
    float s2 = nw4r::math::SinFIdx(ang1 * lbl_eu_8066A9C8);
    float c2 = nw4r::math::CosFIdx(ang1 * lbl_eu_8066A9C8);

    float rot1[3][4];
    rot1[0][0] = c1; rot1[0][1] = -s1; rot1[0][2] = 0.0f; rot1[0][3] = 0.0f;
    rot1[1][0] = s1; rot1[1][1] = c1; rot1[1][2] = 0.0f; rot1[1][3] = 0.0f;
    rot1[2][0] = 0.0f; rot1[2][1] = 0.0f; rot1[2][2] = 1.0f; rot1[2][3] = 0.0f;
    float tmp[3][4];
    PSMTXConcat(rot1, rot1, tmp);

    float s3 = nw4r::math::SinFIdx(ang2 * lbl_eu_8066A9C8);
    float c3 = nw4r::math::CosFIdx(ang2 * lbl_eu_8066A9C8);
    float rot2[3][4];
    rot2[0][0] = c3; rot2[0][1] = 0.0f; rot2[0][2] = s3; rot2[0][3] = 0.0f;
    rot2[1][0] = 0.0f; rot2[1][1] = 1.0f; rot2[1][2] = 0.0f; rot2[1][3] = 0.0f;
    rot2[2][0] = -s3; rot2[2][1] = 0.0f; rot2[2][2] = c3; rot2[2][3] = 0.0f;
    PSMTXConcat(rot2, tmp, tmp);

    // Scale by the shared shadow scale and translate to the anchor point.
    mlCVec3 scaled;
    scaled.x = dir.x * shadowScale;
    scaled.y = up.y * shadowScale;
    scaled.z = dir.z * shadowScale;
    scaled.x += v1[0]; scaled.y += v1[1]; scaled.z += v1[2];
    (void)scaled;

    void* posMtx = func_8049621C(ctx->field_0x04, 0);
    PSMTXConcat(tmp, tmp, (float (*)[4])posMtx);
    GXLoadPosMtxImm((const float (*)[4])posMtx, GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    // Four vertices: position + two attribute bytes each (matches retail pipe writes).
    GXPosition3f32(-f1, f1, lbl_eu_8066A948); GXColor1x8(0); GXColor1x8(0);
    GXPosition3f32(f1, f1, lbl_eu_8066A948); GXColor1x8(1); GXColor1x8(0);
    GXPosition3f32(f1, -f1, lbl_eu_8066A948); GXColor1x8(1); GXColor1x8(1);
    GXPosition3f32(-f1, -f1, lbl_eu_8066A948); GXColor1x8(0); GXColor1x8(1);
}

// us-8049236c: func_8048E2F8 (0x384 bytes)
void func_8048E2F8(){}

// us-8049236c: func_8048E2F8 (0x384 bytes)
// Flat upright billboard path (field_0x2a==1 && flags&4): builds pitch/yaw
// rotations from the anchor direction vector, concatenates them in place,
// stamps the anchor world position into the combined matrix's translation
// column, concatenates with the camera matrix and streams one textured quad.
struct ShadowMtx {
    float m[3][4];
};

void func_8048E2F8(CScnMaruShadowNw4r* shadow, ShadowCtx* ctx, float scale) {
    // Frame layout (scalars reverse-per-class ascending, aggregates forward):
    // posX@8 posY@12 posZ@16 angPitch@20 angYaw@24 pitch@28,
    // concatOut@32 rotTilt@80 rotYaw@128 copyDst@176 rotPitch@224.
    float scl = lbl_eu_80663964;  // first FP local -> f31 (Rule A)
    mlCVec3* dir = (mlCVec3*)(void*)shadow->field_0x50;
    mlCVec3* anchor = (mlCVec3*)(void*)shadow->field_0x4c;

    float pitch;
    float angYaw;
    float angPitch;
    float posZ;
    float posY;
    float posX;
    ShadowMtx concatOut;
    ShadowMtx rotTilt;
    ShadowMtx rotYaw;
    ShadowMtx copyDst;
    ShadowMtx rotPitch;

    float lenSq = dir->x * dir->x + dir->z * dir->z;
    if (lenSq <= lbl_eu_8066A948) {
        Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    if (lenSq > lbl_eu_8066A948) {
        // len scoped inside the guard: dies before the matrix builds, so it
        // never claims a callee-saved FP register.
        float len = lenSq * nw4r::math::FrSqrt(lenSq);
        angPitch = nw4r::math::Atan2FIdx(dir->y, len) * lbl_eu_8066A9C4;
        angYaw = nw4r::math::Atan2FIdx(dir->x, dir->z) * lbl_eu_8066A9C4;
    }

    // Y-rotation by pitch (identity: flat mode carries no pitch).
    pitch = 0.0f;
    float sP = nw4r::math::SinFIdx(pitch * lbl_eu_8066A9C8);
    float cP = nw4r::math::CosFIdx(pitch * lbl_eu_8066A9C8);
    rotPitch.m[0][0] = cP; rotPitch.m[0][1] = -sP;
    rotPitch.m[0][2] = lbl_eu_8066A948; rotPitch.m[0][3] = lbl_eu_8066A948;
    rotPitch.m[1][0] = sP; rotPitch.m[1][1] = cP;
    rotPitch.m[1][2] = lbl_eu_8066A948; rotPitch.m[1][3] = lbl_eu_8066A948;
    rotPitch.m[2][0] = lbl_eu_8066A948; rotPitch.m[2][1] = lbl_eu_8066A948;
    rotPitch.m[2][2] = lbl_eu_8066A944; rotPitch.m[2][3] = lbl_eu_8066A948;

    // X-rotation by -angPitch.
    float sT = nw4r::math::SinFIdx(-angPitch * lbl_eu_8066A9C8);
    float cT = nw4r::math::CosFIdx(-angPitch * lbl_eu_8066A9C8);
    rotTilt.m[0][0] = lbl_eu_8066A944; rotTilt.m[0][1] = lbl_eu_8066A948;
    rotTilt.m[0][2] = lbl_eu_8066A948; rotTilt.m[0][3] = lbl_eu_8066A948;
    rotTilt.m[1][0] = lbl_eu_8066A948; rotTilt.m[1][1] = cT;
    rotTilt.m[1][2] = -sT; rotTilt.m[1][3] = lbl_eu_8066A948;
    rotTilt.m[2][0] = lbl_eu_8066A948; rotTilt.m[2][1] = sT;
    rotTilt.m[2][2] = cT; rotTilt.m[2][3] = lbl_eu_8066A948;

    // Y-rotation by angYaw.
    float sY = nw4r::math::SinFIdx(angYaw * lbl_eu_8066A9C8);
    float cY = nw4r::math::CosFIdx(angYaw * lbl_eu_8066A9C8);
    rotYaw.m[0][0] = cY; rotYaw.m[0][1] = lbl_eu_8066A948;
    rotYaw.m[0][2] = sY; rotYaw.m[0][3] = lbl_eu_8066A948;
    rotYaw.m[1][0] = lbl_eu_8066A948; rotYaw.m[1][1] = lbl_eu_8066A944;
    rotYaw.m[1][2] = lbl_eu_8066A948; rotYaw.m[1][3] = lbl_eu_8066A948;
    rotYaw.m[2][0] = -sY; rotYaw.m[2][1] = lbl_eu_8066A948;
    rotYaw.m[2][2] = cY; rotYaw.m[2][3] = lbl_eu_8066A948;

    // Rotations preserve the translation column, so concatenate in place
    // first, then stamp the anchor world position.
    PSMTXConcat(rotTilt.m, rotPitch.m, rotPitch.m);
    PSMTXConcat(rotYaw.m, rotPitch.m, rotPitch.m);

    posX = anchor->x;
    posY = anchor->y + scl;
    posZ = anchor->z;
    rotPitch.m[0][3] = posX;
    rotPitch.m[1][3] = posY;
    rotPitch.m[2][3] = posZ;

    PSMTXConcat((float (*)[4])func_8049621C(ctx->field_0x04, -1), rotPitch.m,
                concatOut.m);
    copyDst = concatOut;
    GXLoadPosMtxImm(copyDst.m, GX_PNMTX0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-scale, scale, lbl_eu_8066A948); GXColor1x8(0); GXColor1x8(0);
    GXPosition3f32(scale, scale, lbl_eu_8066A948); GXColor1x8(1); GXColor1x8(0);
    GXPosition3f32(scale, -scale, lbl_eu_8066A948); GXColor1x8(1); GXColor1x8(1);
    GXPosition3f32(-scale, -scale, lbl_eu_8066A948); GXColor1x8(0); GXColor1x8(1);
}

// us-804926f0: func_8048E67C (0x20 bytes)
extern const float lbl_eu_8066A994;  // foreign .sdata2
extern const float lbl_eu_8066A9CC;  // foreign .sdata2

void func_8048E67C(void* obj, int condition) {
    if (condition != 0) {
        *(float*)((char*)obj + 0x1c) = lbl_eu_8066A994;
        return;
    }
    *(float*)((char*)obj + 0x1c) = lbl_eu_8066A9CC;
}

// us-80492710: func_8048E69C (0x39C bytes)
// Walks every ResNode of the model, reads each node's user-data dictionary
// and fills the per-node shadow scale table (up to 8 entries) plus the
// shadow flag accumulator.
void func_8048E69C(CScnMaruShadowNw4r* shadow, u32* flagAcc, ShadowCtx* ctx) {
    int count = 0;
    *flagAcc = 0;
    nw4r::g3d::ResMdl mdl = ctx->resMdl;
    int numNodes = GetResNodeNumEntries__Q34nw4r3g3d6ResMdlCFv(mdl);
    const float baseScale = lbl_eu_8066A9D0;
    MaruShadowEntry* cur = shadow->entries;

    // Panic-site strings hoisted out of the loop by MWCC (lis/addi pairs).
    const char* f57File = lbl_eu_80530D18;
    const char* f57Fmt = lbl_eu_80530CFC;
    const char* f57Func = lbl_eu_80530CF0;
    const char* alignFile = lbl_eu_80530F08;
    const char* alignFmt = lbl_eu_80530EE0;
    const char* dicFile = lbl_eu_80530D54;
    const char* dicFmt = lbl_eu_80530D2C;
    const char* assertFmt = lbl_eu_80530D68;

    for (u32 i = 0; i < numNodes; i++) {
        nw4r::g3d::ResNode node = GetResNode__Q34nw4r3g3d6ResMdlCFUl(mdl, i);
        void* ud = GetResUserData__Q34nw4r3g3d7ResNodeFv(node);
        if (ud == NULL) {
            continue;
        }

        // First lookup: per-node scale entry under key A.
        const char* key = lbl_eu_8066395C;
        if (key == NULL) {
            Panic__Q24nw4r2dbFPCciPCce(f57File, 0x57, f57Fmt, f57Func, lbl_eu_80663968);
        }
        nw4r::g3d::ResDic dic;
        dic.ptr = (char*)ud + 4;
        if (((u32)dic.ptr & 3) != 0) {
            Panic__Q24nw4r2dbFPCciPCce(alignFile, 0x54, alignFmt);
        }
        void* entry = __vc__Q34nw4r3g3d6ResDicCFPCc(dic, key);
        if (((u32)entry & 3) != 0) {
            Panic__Q24nw4r2dbFPCciPCce(dicFile, 0x26, dicFmt);
        }
        if (entry != NULL) {
            if (node.ptr == NULL) {
                Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80529678, 0x53, lbl_eu_80529658);
            }
            cur->nodeId = node.ptr != NULL ? *(u16*)((char*)node.ptr + 0x10) : 0;
            if (entry == NULL) {
                Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8, assertFmt,
                                           lbl_eu_80663970);
            }
            if (*(u32*)((char*)entry + 0xC) != 1) {
                Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530E74, 0x3D, lbl_eu_80530E30);
            }
            if (entry == NULL) {
                Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530D94, 0x26, lbl_eu_80530D78, assertFmt,
                                           lbl_eu_8066396C);
            }
            u32 off = *(u32*)((char*)entry + 4);
            void* data = off != 0 ? (char*)entry + off : 0;
            float f = *(float*)data;
            count++;
            cur->scaleQ = (u16)(s32)(baseScale * f);
            cur++;
            if (count >= 8) {
                goto done;
            }
        }

        // Second lookup: per-node flag bits under key B (always executed).
        if (ud == NULL) {
            Panic__Q24nw4r2dbFPCciPCce(f57File, 0x57, f57Fmt, f57Func, lbl_eu_80663968);
        }
        nw4r::g3d::ResDic dic2;
        dic2.ptr = (char*)ud + 4;
        if (((u32)dic2.ptr & 3) != 0) {
            Panic__Q24nw4r2dbFPCciPCce(alignFile, 0x54, alignFmt);
        }
        void* entry2 = __vc__Q34nw4r3g3d6ResDicCFPCc(dic2, lbl_eu_80663960);
        if (((u32)entry2 & 3) != 0) {
            Panic__Q24nw4r2dbFPCciPCce(dicFile, 0x26, dicFmt);
        }
        if (entry2 != NULL) {
            if (entry2 == NULL) {
                Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8, assertFmt,
                                           lbl_eu_80663970);
            }
            if (*(u32*)((char*)entry2 + 0xC) != 0) {
                Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
            }
            if (entry2 == NULL) {
                Panic__Q24nw4r2dbFPCciPCce(lbl_eu_80530D94, 0x26, lbl_eu_80530D78, assertFmt,
                                           lbl_eu_8066396C);
            }
            u32 off2 = *(u32*)((char*)entry2 + 4);
            void* data2 = off2 != 0 ? (char*)entry2 + off2 : 0;
            u32 v = *(u32*)data2;
            if ((v & 1) != 0) {
                shadow->field_0x28 |= 0x20;
            }
            *flagAcc |= (v & ~3u);
        }
    }
done:
    if (count != 0) {
        shadow->field_0x2b = (u8)count;
        shadow->field_0x28 |= 8;
    }
}

// us-80492aac: func_8048EA38 (0x8 bytes) -- stores float to sda21 global
extern "C" void func_8048EA38(float v){
    lbl_eu_80663964 = v;
}

// us-80492ab4: func_8048EA40 (0x8 bytes) -- loads float from sda21 global
float func_8048EA40()
{
    return lbl_eu_80663964;
}

// us-80492abc: func_8048EA48 (0x2C bytes) -- sets/clears flag 0x80 in halfword + stores float
extern "C" void func_8048EA48(u8* self, u32 enable, float v) {
    if (enable != 0) {
        *(u16*)((u8*)self + 0x28) |= 0x80;
    } else {
        *(u16*)((u8*)self + 0x28) &= ~0x80;
    }
    *(float*)((u8*)self + 0x24) = v;
}

// us-80492ae8: func_8048EA74 (0x28 bytes) -- sets/clears flag 0x100 in halfword
extern "C" void func_8048EA74(u8* self, u32 enable) {
    if (enable != 0) {
        *(u16*)((u8*)self + 0x28) |= 0x100;
    } else {
        *(u16*)((u8*)self + 0x28) &= ~0x100;
    }
}

// --- FULL_MATCH functions ---

// func_8048EA9C -- capability check stub, always returns true.
// Retail: li r3, 1; blr  (8 bytes)
extern "C" int func_8048EA9C() { return 1; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---

// sinit_8048EAA4 -- static initializer registered in .ctors.
// Empty body; used as constructor hook for the TU.
// Retail: blr  (4 bytes)
extern "C" void sinit_8048EAA4() {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace MSBlob {
extern "C" void func_8048E67C();
extern "C" void func_8048EA9C();
extern "C" void func_8048D500();
extern "C" void func_8048DB58();
extern "C" void func_8048EA74();
extern "C" void func_8048EA48();
}
extern "C" u32 lbl_eu_806623F8;   // foreign .sdata

// [.data] 0x8056E598-0x8056E5B8 (32B): CScnMaruShadowNw4r vtable
extern "C" u32 lbl_eu_8056E598[8] = {
    (u32)&lbl_eu_806623F8, 0x00000000,
    (u32)&MSBlob::func_8048E67C,
    (u32)&MSBlob::func_8048EA9C,
    (u32)&MSBlob::func_8048D500,
    (u32)&MSBlob::func_8048DB58,
    (u32)&MSBlob::func_8048EA74,
    (u32)&MSBlob::func_8048EA48,
};

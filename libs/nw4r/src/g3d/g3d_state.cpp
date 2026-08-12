// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_state
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/math.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>

namespace nw4r {
namespace g3d {
namespace detail {

#include <nw4r/g3d/detail/g3d_transform_ps.inl>

} // namespace detail
} // namespace g3d
} // namespace nw4r

struct FogState {
    u32 flags;                  // at 0x0
    s32 curFogID;               // at 0x4
    nw4r::g3d::FogData fogArray[0x20]; // at 0x8
};

// TLUT cache (retail .bss at 8061A6C0): last-uploaded TLUT objects + flag.
struct TlutCacheState {
    GXTlutObj tlutObj[8]; // at 0x0
    u16 flag;             // at 0x60
};
extern TlutCacheState lbl_eu_8061A6C0;

// Tex cache (retail .bss at 8061A5A0): the byte at 0x100 tracks which TLUTs
// must be re-uploaded after a texture edit.
struct TexCacheState {
    u8 data[0x100]; // at 0x0
    u8 flag;        // at 0x100
};
extern TexCacheState lbl_eu_8061A5A0;

// Per-texcoord scale pair of the texCoordScale cache.
struct TexCoordScaleEntry {
    u16 scaleS; // at 0x0
    u16 scaleT; // at 0x2
};

// Texcoord-scale cache at 8061A52C (offset 0xC inside the state blob).
struct TexCoordScaleCache {
    u32 flag;                  // at 0x0  (bit0 = scales re-emitted, bit1 = tex invalidated)
    TexCoordScaleEntry src[8]; // at 0x4  per-source scale pairs
    TexCoordScaleEntry dst[8]; // at 0x24 cached pairs (8-byte stride)
    u8 dirty[8];               // at 0x64 0xFF = clean, else source texcoord index
};

// G3DState persistent cache blob (retail .bss at 8061A520). One struct over
// the whole region so MWCC materializes a single base register and reaches
// every sub-cache via a constant offset (the retail Invalidate /
// LoadResShpPrePrimitive addressing pattern).
struct G3DStateCache {
    // genMode cache
    u8 nTexGens;                 // at 0x0
    u8 nChans;                   // at 0x1
    u8 nTevs;                    // at 0x2
    u8 nInds;                    // at 0x3
    GXCullMode cullMode;         // at 0x4
    u32 flag;                    // at 0x8

    TexCoordScaleCache texCoordScale;      // at 0xC
    u8 PADDING_0x78[0x80 - 0x78];          // at 0x78

    TexCacheState texCache;                // at 0x80
    u8 PADDING_0x181[0x1A0 - 0x181];       // at 0x181

    TlutCacheState tlutCache;              // at 0x1A0
    u8 PADDING_0x202[0x204 - 0x202];       // at 0x202

    u32 vtxDescv[3];                       // at 0x204  cached GX vtx desc list
    u32 curMtx[8];                         // at 0x210  current-matrix id cache
    const nw4r::math::MTX34* pViewPosMtxArray;    // at 0x230
    const nw4r::math::MTX33* pViewNrmMtxArray;    // at 0x234
    const nw4r::math::MTX34* pViewEnvTexMtxArray; // at 0x238
    u8 PADDING_0x23C[0xA40 - 0x23C];       // at 0x23C

    u32 fogFlag;                           // at 0xA40  (FogState.flags)
    u8 PADDING_0xA44[0x1064 - 0xA44];      // at 0xA44  (LightState.mSetting at 0x1054..0x1064)

    u32 lightField_0x10;                   // at 0x1064  (LightState.field_0x10)
    u32 lightDiffColorMask;                // at 0x1068
    u32 lightDiffAlphaMask;                // at 0x106C
    u32 lightSpecColorMask;                // at 0x1070
    u32 lightSpecAlphaMask;                // at 0x1074
    s8 lightObjIndex[8];                   // at 0x1078
};
extern G3DStateCache lbl_eu_8061A520;

// "sync" flag: cleared before every display-list upload.
extern bool lbl_eu_80665448;

// Small-data flags in g3d_state.o's retail .sbss.
extern u8 lbl_eu_8066544C;
extern u32 lbl_eu_80665450;
extern u32 lbl_eu_80665454;
extern u32 lbl_eu_80665460;

// Float constants in g3d_state.o's retail .sdata2 (IndMtxOpStd::SetNrmMapMtx).
extern const float lbl_eu_80669BEC;  // 0.0f
extern const float lbl_eu_80669BF8;  // 1.0f
extern const float lbl_eu_80669BFC;  // 0.5f (ind normal-map scale)
extern const float lbl_eu_80669C00;  // 1.0f/255.0f (channel-color scale)
extern const double lbl_eu_80669C08; // 2^52 + 2^31 (u32 -> double magic)

// Cull-mode -> genMode bit remap table (retail .sdata2 at 80669BE8).
extern const float lbl_eu_80669BE8;

// Camera matrix state blob layout (retail .bss at 8061DFA0).
struct CameraMtxStateData {
    u16 mFlag;                            // at 0x0
    u16 mCurCameraID;                     // at 0x2
    nw4r::math::MTX34 mViewMtx;           // at 0x4
    nw4r::math::MTX34 mViewMtxArray[0x20]; // at 0x34
    nw4r::math::MTX44 mProjMtxArray[0x20]; // at 0x634
    nw4r::math::MTX34 mNrmMtxArray[0x20];  // at 0xE34
    nw4r::math::MTX34 mEnvMtxArray[0x20];  // at 0x1434
};

namespace nw4r {
namespace g3d {

// Retail camera-state blob (8061DFA0). C linkage keeps the name unmangled
// inside this namespace (used by detail::ScnDependentMtxFunc below).
extern "C" CameraMtxStateData lbl_eu_8061DFA0;

LightObj::LightObj() : mFlag(0) {}

const GXRenderModeObj* G3DState::GetRenderModeObj() {
    return &lbl_eu_8061F9D4;
}

const math::MTX33* G3DState::GetViewNrmMtxPtr(u32 idx) {
    math::MTX33* pNrmMtx =
        *reinterpret_cast<math::MTX33**>(lbl_eu_8061A750 + 4);

    if (pNrmMtx != NULL) {
        return &pNrmMtx[idx];
    }

    if (!lbl_eu_80665458[0]) {
        lbl_eu_80665458[0] = 1;
    }

    math::MTX34* pViewMtx =
        *reinterpret_cast<math::MTX34**>(lbl_eu_8061A750);

    detail::CalcViewNrmMtx(reinterpret_cast<math::MTX33*>(lbl_eu_8061FAB8),
                           pViewMtx != NULL ? &pViewMtx[idx] : NULL);

    return reinterpret_cast<const math::MTX33*>(lbl_eu_8061FAB8);
}

void G3DState::IndMtxOpStd::Reset() {
    PSMTXIdentity(mIndMtx[0]);
    PSMTXIdentity(mIndMtx[1]);
    PSMTXIdentity(mIndMtx[2]);
    mIsValidMtx[2] = false;
    mIsValidMtx[1] = false;
    mIsValidMtx[0] = false;
}

void G3DState::IndMtxOpStd::SetNrmMapMtx(GXIndTexMtxID id,
                                         const math::VEC3* pLightVec,
                                         const math::MTX34* pNrmMtx,
                                         ResMatMiscData::IndirectMethod method) {
    if (id < GX_ITM_0 || id > GX_ITM_2) {
        return;
    }
    if (method == ResMatMiscData::WARP || method == ResMatMiscData::FUR) {
        return;
    }

    u32 i = id - GX_ITM_0;
    mIsValidMtx[i] = true;

    if (pLightVec != NULL) {
        // Row 0 = negated light vector scaled into texture space.
        mIndMtx[i].m[0][0] = -pLightVec->x * lbl_eu_80669BFC;
        mIndMtx[i].m[0][1] = -pLightVec->y * lbl_eu_80669BFC;
        mIndMtx[i].m[0][2] = -pLightVec->z * lbl_eu_80669BFC;

        if (method == ResMatMiscData::NORMAL_MAP_SPECULAR) {
            // Row 1 = normalized (x, y, z - 1) light vector.
            math::VEC3 v;
            v.x = pLightVec->x;
            v.y = pLightVec->y;
            v.z = pLightVec->z - lbl_eu_80669BF8;
            PSVECNormalize(v, v);
            mIndMtx[i].m[1][0] = -v.x * lbl_eu_80669BFC;
            mIndMtx[i].m[1][1] = -v.y * lbl_eu_80669BFC;
            mIndMtx[i].m[1][2] = -v.z * lbl_eu_80669BFC;
        } else {
            mIndMtx[i].m[1][0] = lbl_eu_80669BEC;
            mIndMtx[i].m[1][1] = lbl_eu_80669BEC;
            mIndMtx[i].m[1][2] = lbl_eu_80669BEC;
        }

        PSMTXConcat(mIndMtx[i], *pNrmMtx, mIndMtx[i]);
    } else {
        math::MTX34Zero(&mIndMtx[i]);
    }
}

void G3DState::IndMtxOpStd::operator()(IndTexMtxInfo* pInfo) {
    if (mIsValidMtx[0]) {
        pInfo->offset_mtx[0] = mIndMtx[0];
        pInfo->flag |= 1;
    }
    if (mIsValidMtx[1]) {
        pInfo->offset_mtx[1] = mIndMtx[1];
        pInfo->flag |= 2;
    }
    if (mIsValidMtx[2]) {
        pInfo->offset_mtx[2] = mIndMtx[2];
        pInfo->flag |= 4;
    }
}

void G3DState::SetRenderModeObj(const GXRenderModeObj& rObj) {
    lbl_eu_8061F9D4 = rObj;
}

void G3DState::SetViewPosNrmMtxArray(const math::MTX34* pViewPosMtxArray,
                                     const math::MTX33* pViewNrmMtxArray,
                                     const math::MTX34* pViewEnvTexMtxArray) {
    *reinterpret_cast<const math::MTX34**>(lbl_eu_8061A750) = pViewPosMtxArray;
    *reinterpret_cast<const math::MTX33**>(lbl_eu_8061A750 + 4) = pViewNrmMtxArray;
    *reinterpret_cast<const math::MTX34**>(lbl_eu_8061A750 + 8) = pViewEnvTexMtxArray;

    if (pViewPosMtxArray != NULL) {
        GXSetArray(GX_POS_MTX_ARRAY, pViewPosMtxArray, 0x30);
    }

    math::MTX33* pNrm =
        *reinterpret_cast<math::MTX33**>(lbl_eu_8061A750 + 4);

    if (pNrm != NULL) {
        GXSetArray(GX_NRM_MTX_ARRAY, pNrm, 0x24);
    }

    math::MTX34* pEnv =
        *reinterpret_cast<math::MTX34**>(lbl_eu_8061A750 + 8);

    if (pEnv != NULL) {
        GXSetArray(GX_TEX_MTX_ARRAY, pEnv, 0x30);
    }
}

void G3DState::SetFog(Fog fog, int id) {
    FogState* pState = &lbl_eu_8061AF60;

    if (id >= 0 && id < 0x20 && fog.ptr() != NULL) {
        if (pState->curFogID != id ||
            memcmp(fog.ptr(), &pState->fogArray[id], 0x30) != 0) {
            pState->flags = 0;
            fog.CopyTo(&pState->fogArray[id]);
        }
    }
}

void G3DState::LoadFog(int id) {
    if ((lbl_eu_8061AF60.flags & 1) && (lbl_eu_8061AF60.flags & 2) &&
        id == lbl_eu_8061AF60.curFogID) {
        return;
    }

    if (id < 0 || id >= 0x20) {
        GXColor color = {0, 0, 0, 0};
        GXSetFog(GX_FOG_NONE, color, 0.0f, 0.0f, 0.0f, 0.0f);
    } else {
        Fog fog(&lbl_eu_8061AF60.fogArray[id]);
        fog.SetGP();
    }

    lbl_eu_8061AF60.curFogID = id;
    lbl_eu_8061AF60.flags |= 3;
}

void G3DState::LoadResMatTexCoordGen(ResMatTexCoordGen rObj) {
    if (rObj.IsValid()) {
        bool sync = lbl_eu_80665448;
        lbl_eu_80665448 = 0;
        rObj.CallDisplayList(lbl_eu_8061A520.nTexGens, sync);

        u32 flag = lbl_eu_8061A520.flag;
        if ((flag & 0x4) && (flag & 3) != 3) {
            fifo::GDSetGenMode2(lbl_eu_8061A520.nTexGens, lbl_eu_8061A520.nChans,
                                lbl_eu_8061A520.nTevs, lbl_eu_8061A520.nInds,
                                lbl_eu_8061A520.cullMode);
            lbl_eu_8061A520.flag |= 3;
        }
    }
}

void G3DState::LoadResMatPix(ResMatPix rObj) {
    if (!rObj.IsValid()) {
        return;
    }

    u32 flag = lbl_eu_8061A520.flag;
    if ((flag & 0x4) && (flag & 3) == 2) {
        // Re-emit the genMode BP word (pix/tev caches share the dirty
        // genMode state with LoadResMatTexCoordGen, but only the BP
        // register write is repeated here).
        volatile u8* fifo = (volatile u8*)0xCC008000;
        const u8* cull = (const u8*)&lbl_eu_80669BE8;
        GXCullMode cullMode = lbl_eu_8061A520.cullMode;

        *fifo = 0x61;
        *(volatile u32*)fifo = 0xFE07FC3F;

        *fifo = 0x61;
        *(volatile u32*)fifo = lbl_eu_8061A520.nTexGens |
                               (lbl_eu_8061A520.nChans << 4) |
                               ((lbl_eu_8061A520.nTevs - 1) << 10) |
                               (cull[cullMode] << 14) |
                               (lbl_eu_8061A520.nInds << 16);

        lbl_eu_8061A520.flag |= 1;
    }

    bool sync = lbl_eu_80665448;
    lbl_eu_80665448 = 0;
    rObj.CallDisplayList(sync);
}

void G3DState::LoadResMatTevColor(ResMatTevColor rObj) {
    if (!rObj.IsValid()) {
        return;
    }

    u32 flag = lbl_eu_8061A520.flag;
    if ((flag & 0x4) && (flag & 3) == 2) {
        volatile u8* fifo = (volatile u8*)0xCC008000;
        const u8* cull = (const u8*)&lbl_eu_80669BE8;
        GXCullMode cullMode = lbl_eu_8061A520.cullMode;

        *fifo = 0x61;
        *(volatile u32*)fifo = 0xFE07FC3F;

        *fifo = 0x61;
        *(volatile u32*)fifo = lbl_eu_8061A520.nTexGens |
                               (lbl_eu_8061A520.nChans << 4) |
                               ((lbl_eu_8061A520.nTevs - 1) << 10) |
                               (cull[cullMode] << 14) |
                               (lbl_eu_8061A520.nInds << 16);

        lbl_eu_8061A520.flag |= 1;
    }

    bool sync = lbl_eu_80665448;
    lbl_eu_80665448 = 0;
    rObj.CallDisplayList(sync);
}

void G3DState::LoadResMatIndMtxAndScale(const ResMatIndMtxAndScale ind) {
    if (!ind.IsValid()) {
        return;
    }

    u32 flag = lbl_eu_8061A520.flag;
    if ((flag & 0x4) && (flag & 3) == 2) {
        // Re-emit the genMode BP word (ind-mtx uploads share the dirty
        // genMode state with LoadResMatTexCoordGen; only the BP register
        // write is repeated here).
        volatile u8* fifo = (volatile u8*)0xCC008000;
        const u8* cull = (const u8*)&lbl_eu_80669BE8;
        GXCullMode cullMode = lbl_eu_8061A520.cullMode;

        *fifo = 0x61;
        *(volatile u32*)fifo = 0xFE07FC3F;

        *fifo = 0x61;
        *(volatile u32*)fifo = lbl_eu_8061A520.nTexGens |
                               (lbl_eu_8061A520.nChans << 4) |
                               ((lbl_eu_8061A520.nTevs - 1) << 10) |
                               (cull[cullMode] << 14) |
                               (lbl_eu_8061A520.nInds << 16);

        lbl_eu_8061A520.flag |= 1;
    }

    bool sync = lbl_eu_80665448;
    lbl_eu_80665448 = 0;
    ind.CallDisplayList(lbl_eu_8061A520.nInds, sync);
}

void G3DState::Invalidate(u32 flag) {
    G3DStateCache* pState = &lbl_eu_8061A520;

    if (flag & INVALIDATE_TEX) {
        pState->texCache.flag = 0;
        pState->texCoordScale.flag = 0;
    }

    if (flag & INVALIDATE_TLUT) {
        pState->tlutCache.flag = 0;
    }

    if (flag & INVALIDATE_TEV) {
        pState->texCoordScale.flag = 0;
        lbl_eu_80665454 = 0;
    }

    if (flag & INVALIDATE_GENMODE) {
        pState->nTexGens = 1;
        pState->nChans = 0;
        pState->nTevs = 1;
        pState->nInds = 0;
        pState->cullMode = GX_CULL_BACK;
        pState->flag = 0;
    }

    if (flag & INVALIDATE_SHP) {
        pState->vtxDescv[0] = 0;
        pState->vtxDescv[1] = 0;
        pState->vtxDescv[2] = 0;
    }

    if (flag & INVALIDATE_CURRMTX) {
        lbl_eu_8066544C = 0;
    }

    if (flag & INVALIDATE_TEXMTX) {
        for (int i = 0; i < 8; i++) {
            pState->curMtx[i] = 0;
        }
        lbl_eu_80665450 = 0;
    }

    if (flag & INVALIDATE_MISC) {
        lbl_eu_80665460 &= ~1;
    }

    if (flag & INVALIDATE_FOG) {
        pState->fogFlag = 0;
    }

    if (flag & INVALIDATE_LIGHT) {
        pState->lightField_0x10 = 0xFFFFFFFF;
        pState->lightDiffColorMask = 0;
        pState->lightDiffAlphaMask = 0;
        pState->lightSpecColorMask = 0;
        pState->lightSpecAlphaMask = 0;
        for (int i = 0; i < 8; i++) {
            pState->lightObjIndex[i] = -1;
        }
    }

    if (flag & INVALIDATE_POSMTX) {
        pState->pViewPosMtxArray = NULL;
        pState->pViewNrmMtxArray = NULL;
        pState->pViewEnvTexMtxArray = NULL;
    }

    lbl_eu_80665448 = 1;
}

void G3DState::LoadResGenMode(ResGenMode mode) {
    if (!mode.IsValid()) {
        return;
    }

    if (lbl_eu_8061A520.nTexGens != mode.GXGetNumTexGens()) {
        lbl_eu_8061A520.nTexGens = mode.GXGetNumTexGens();
        lbl_eu_8061A520.flag &= ~3;
    }

    if (lbl_eu_8061A520.nChans != mode.GXGetNumChans()) {
        lbl_eu_8061A520.nChans = mode.GXGetNumChans();
        lbl_eu_8061A520.flag &= ~3;
    }

    if (lbl_eu_8061A520.nTevs != mode.GXGetNumTevStages()) {
        lbl_eu_8061A520.nTevs = mode.GXGetNumTevStages();
        lbl_eu_8061A520.flag &= ~1;
    }

    if (lbl_eu_8061A520.nInds != mode.GXGetNumIndStages()) {
        lbl_eu_8061A520.nInds = mode.GXGetNumIndStages();
        lbl_eu_8061A520.flag &= ~1;
    }

    if (lbl_eu_8061A520.cullMode != mode.GXGetCullMode()) {
        lbl_eu_8061A520.cullMode = mode.GXGetCullMode();
        lbl_eu_8061A520.flag &= ~1;
    }

    if (!(lbl_eu_8061A520.flag & 4)) {
        lbl_eu_8061A520.flag = (lbl_eu_8061A520.flag & ~3) | 4;
    }
}

void G3DState::LoadResShpPrePrimitive(ResShp shp) {
    if (!shp.IsValid()) {
        return;
    }

    G3DStateCache* pState = &lbl_eu_8061A520;

    // Re-emit texcoord scales when the texture cache was invalidated since
    // the last shape upload and the cached scales have not been re-emitted.
    if ((pState->texCoordScale.flag & 2) &&
        (pState->texCoordScale.flag & 1) == 0 &&
        pState->nTexGens != 0) {
        for (u8 i = 0; i < pState->nTexGens; i++) {
            u8 b = pState->texCoordScale.dirty[i];
            if (b != 0xFF) {
                pState->texCoordScale.dst[i].scaleS =
                    pState->texCoordScale.src[b].scaleS;
                pState->texCoordScale.dst[i].scaleT =
                    pState->texCoordScale.src[b].scaleT;
                fifo::GDSetTexCoordScale2(
                    static_cast<GXTexCoordID>(i),
                    pState->texCoordScale.src[b].scaleS, false, false,
                    pState->texCoordScale.src[b].scaleT, false, false);
            }
        }
        pState->texCoordScale.flag |= 1;
    }

    // Re-emit the genMode BP word when the pix/tev caches dirtied it.
    if ((pState->flag & 4) && (pState->flag & 3) != 3) {
        fifo::GDSetGenMode2(pState->nTexGens, pState->nChans,
                            pState->nTevs, pState->nInds, pState->cullMode);
        pState->flag |= 3;
    }

    ResShpData* pData = shp.ptr();
    bool cacheIsSame;
    if (pData->cache.data_u32[0] == pState->vtxDescv[0] &&
        pData->cache.data_u32[1] == pState->vtxDescv[1] &&
        pData->cache.data_u32[2] == pState->vtxDescv[2]) {
        cacheIsSame = true;
    } else {
        pState->vtxDescv[0] = pData->cache.data_u32[0];
        pState->vtxDescv[1] = pData->cache.data_u32[1];
        pState->vtxDescv[2] = pData->cache.data_u32[2];
        cacheIsSame = false;
    }

    bool sync = lbl_eu_80665448;
    lbl_eu_80665448 = 0;
    shp.CallPrePrimitiveDisplayList(sync, cacheIsSame);
}

void G3DState::LoadResTlutObj(const ResTlutObj tlutObj) {
    if (tlutObj.ptr() != NULL) {
        TlutCacheState* pTlutCache = &lbl_eu_8061A6C0;
        TexCacheState* pTexCache = &lbl_eu_8061A5A0;
        for (u32 i = 0; i < 8; i++) {
            GXTlut tlut = static_cast<GXTlut>(i);
            if (tlutObj.IsValidTlut(tlut)) {
                const GXTlutObj* pObj = tlutObj.GetTlut(tlut);
                u16 bit = static_cast<u16>(1 << tlut);

                if (pTlutCache->flag & bit) {
                    // Skip the upload when the cached TLUT still matches.
                    bool b1 = pObj->dummy[0] == pTlutCache->tlutObj[i].dummy[0] &&
                              pObj->dummy[1] == pTlutCache->tlutObj[i].dummy[1];
                    bool b2 = b1 && pObj->dummy[2] == pTlutCache->tlutObj[i].dummy[2];
                    if (b2) {
                        continue;
                    }
                }

                pTlutCache->flag |= bit;
                pTlutCache->tlutObj[i] = *pObj;
                GXLoadTlut(const_cast<GXTlutObj*>(pObj), tlut);
                pTexCache->flag &= static_cast<u8>(~(1 << tlut));
            }
        }

        lbl_eu_80665448 = 1;
    }
}

} // namespace g3d
} // namespace nw4r

void EnvironmentMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc(){}

void EnvironmentSpecularMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc(){}

namespace nw4r {
namespace g3d {
namespace detail {
namespace ScnDependentMtxFunc {

void DefaultMapping(math::MTX34* pMtx, s8 camRef, s8 lightRef) {
    if (pMtx != NULL) {
        PSMTXIdentity(*pMtx);
    }
}

void ProjectionMapping(math::MTX34* pMtx, s8 camRef, s8 lightRef) {
    if (pMtx != NULL) {
        if (camRef >= 0 && camRef < 0x20) {
            math::MTX34* pCamMtx;
            if (camRef < 0x20 && camRef >= 0) {
                pCamMtx = &lbl_eu_8061DFA0.mViewMtxArray[camRef];
            } else {
                pCamMtx = NULL;
            }

            if (!(lbl_eu_8061DFA0.mFlag & 1)) {
                // Lazily compute the inverse of the current camera's view
                // matrix once per camera switch.
                PSMTXInverse(
                    lbl_eu_8061DFA0.mViewMtxArray[lbl_eu_8061DFA0.mCurCameraID],
                    lbl_eu_8061DFA0.mViewMtx);
                lbl_eu_8061DFA0.mFlag |= 1;
            }

            PSMTXConcat(*pCamMtx, lbl_eu_8061DFA0.mViewMtx, *pMtx);

            math::MTX34* pProjTexMtx;
            if (camRef < 0x20 && camRef >= 0) {
                pProjTexMtx = &lbl_eu_8061DFA0.mNrmMtxArray[camRef];
            } else {
                pProjTexMtx = NULL;
            }
            PSMTXConcat(*pProjTexMtx, *pMtx, *pMtx);
        } else {
            PSMTXCopy(
                lbl_eu_8061DFA0.mNrmMtxArray[lbl_eu_8061DFA0.mCurCameraID],
                *pMtx);
        }
    }
}

} // namespace ScnDependentMtxFunc
} // namespace detail
} // namespace g3d
} // namespace nw4r

void FifoSend__Q44nw4r3g3d8G3DState13IndTexMtxInfoCFv(){}

void LoadResMatMisc__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResMatMisc(){}

void LoadResTexObj__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResTexObj(){}

void LoadResTev__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResTev(){}


void LoadResMatIndMtxAndScale__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d20ResMatIndMtxAndScaleRQ44nw4r3g3d8G3DState8IndMtxOp(){}

namespace nw4r {
namespace g3d {

void G3DState::LoadResMatChan(const ResMatChan chan, u32 maskDiffColor,
                              u32 maskDiffAlpha, u32 maskSpecColor,
                              u32 maskSpecAlpha, GXColor amb, bool lightOff) {
    const ResChanData* pData = chan.ptr();
    if (pData == NULL) {
        return;
    }

    u32 flag = pData->chan[0].flag;

    // Mat color 0 (XF 0x100C): upload the resource color whenever either the
    // color or alpha enable bit is set (all three bit combinations write the
    // same register).
    if (flag & Chan::FLAG_MAT_COLOR_ENABLE) {
        if (flag & Chan::FLAG_MAT_ALPHA_ENABLE) {
            fifo::GDSetChanMatColor(GX_COLOR0, pData->chan[0].matColor);
        } else {
            fifo::GDSetChanMatColor(GX_COLOR0, pData->chan[0].matColor);
        }
    } else if (flag & Chan::FLAG_MAT_ALPHA_ENABLE) {
        fifo::GDSetChanMatColor(GX_COLOR0, pData->chan[0].matColor);
    }

    // Amb color 0 (XF 0x100A): the runtime color is scaled component-wise by
    // the resource's ambColor bytes (value * scale / 255, rounded). The u32 ->
    // f64 cast makes MWCC emit its shared 2^52 magic-double conversion
    // (xoris/stw/lfd/fsubs), hoisting the 0x43300000 stores into the prologue.
    u32 ambFlag = pData->chan[0].flag;
    GXColor scaled;
    scaled.r = (u8)(lbl_eu_80669BFC +
                    (f32)((f64)(u32)((u32)amb.r * (u32)pData->chan[0].ambColor.r)) *
                        lbl_eu_80669C00);
    scaled.g = (u8)(lbl_eu_80669BFC +
                    (f32)((f64)(u32)((u32)amb.g * (u32)pData->chan[0].ambColor.g)) *
                        lbl_eu_80669C00);
    scaled.b = (u8)(lbl_eu_80669BFC +
                    (f32)((f64)(u32)((u32)amb.b * (u32)pData->chan[0].ambColor.b)) *
                        lbl_eu_80669C00);
    scaled.a = (u8)(lbl_eu_80669BFC +
                    (f32)((f64)(u32)((u32)amb.a * (u32)pData->chan[0].ambColor.a)) *
                        lbl_eu_80669C00);

    if (ambFlag & Chan::FLAG_AMB_COLOR_ENABLE) {
        if (ambFlag & Chan::FLAG_AMB_ALPHA_ENABLE) {
            fifo::GDSetChanAmbColor(GX_COLOR0, scaled);
        } else {
            fifo::GDSetChanAmbColor(GX_COLOR0, scaled);
        }
    } else if (ambFlag & Chan::FLAG_AMB_ALPHA_ENABLE) {
        fifo::GDSetChanAmbColor(GX_COLOR0, scaled);
    }

    // Channel-control registers (XF 0x100E / 0x1010): pack the light masks
    // into the resource ctrl words. lightOff additionally clears the LIGHT bit.
    if (lightOff) {
        if (pData->chan[0].flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
            fifo::GDSetChanCtrlLightOff(GX_COLOR0, pData->chan[0].paramChanCtrlC,
                                        maskDiffColor);
        }
        if (pData->chan[0].flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
            fifo::GDSetChanCtrlLightOff(GX_ALPHA0, pData->chan[0].paramChanCtrlA,
                                        maskDiffAlpha);
        }
    } else {
        if (pData->chan[0].flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
            fifo::GDSetChanCtrl(GX_COLOR0, pData->chan[0].paramChanCtrlC,
                                maskDiffColor);
        }
        if (pData->chan[0].flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
            fifo::GDSetChanCtrl(GX_ALPHA0, pData->chan[0].paramChanCtrlA,
                                maskDiffAlpha);
        }
    }

    // Second channel: only uploaded when the genMode cache says two channels
    // are active. Its registers are XF 0x100D/0x100B (mat/amb) and 0x100F/0x1011
    // (ctrl); on the one-channel path the specular ctrl registers are cleared.
    if (lbl_eu_8061A520.nChans == 2) {
        const ResChanData* pChan1Data = chan.ptr();
        u32 flag1 = pChan1Data->chan[1].flag;

        if (flag1 & Chan::FLAG_MAT_COLOR_ENABLE) {
            if (flag1 & Chan::FLAG_MAT_ALPHA_ENABLE) {
                fifo::GDSetChanMatColor(GX_COLOR1, pChan1Data->chan[1].matColor);
            } else {
                fifo::GDSetChanMatColor(GX_COLOR1, pChan1Data->chan[1].matColor);
            }
        } else if (flag1 & Chan::FLAG_MAT_ALPHA_ENABLE) {
            fifo::GDSetChanMatColor(GX_COLOR1, pChan1Data->chan[1].matColor);
        }

        if (pChan1Data->chan[1].flag & Chan::FLAG_AMB_COLOR_ENABLE) {
            if (pChan1Data->chan[1].flag & Chan::FLAG_AMB_ALPHA_ENABLE) {
                fifo::GDSetChanAmbColor(GX_COLOR1, pChan1Data->chan[1].ambColor);
            } else {
                fifo::GDSetChanAmbColor(GX_COLOR1, pChan1Data->chan[1].ambColor);
            }
        } else if (pChan1Data->chan[1].flag & Chan::FLAG_AMB_ALPHA_ENABLE) {
            fifo::GDSetChanAmbColor(GX_COLOR1, pChan1Data->chan[1].ambColor);
        }

        if (lightOff) {
            if (pChan1Data->chan[1].flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
                fifo::GDSetChanCtrlLightOff(GX_COLOR1,
                                            pChan1Data->chan[1].paramChanCtrlC,
                                            maskSpecColor);
            }
            if (pChan1Data->chan[1].flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
                fifo::GDSetChanCtrlLightOff(GX_ALPHA1,
                                            pChan1Data->chan[1].paramChanCtrlA,
                                            maskSpecAlpha);
            }
        } else {
            if (pChan1Data->chan[1].flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
                fifo::GDSetChanCtrl(GX_COLOR1,
                                    pChan1Data->chan[1].paramChanCtrlC,
                                    maskSpecColor);
            }
            if (pChan1Data->chan[1].flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
                fifo::GDSetChanCtrl(GX_ALPHA1,
                                    pChan1Data->chan[1].paramChanCtrlA,
                                    maskSpecAlpha);
            }
        }
    } else {
        fifo::GDSetChanCtrl(GX_COLOR1, 0, 0);
        fifo::GDSetChanCtrl(GX_ALPHA1, 0, 0);
    }

    // Re-emit the genMode BP word if a pix/tev cache dirtied it.
    u32 genFlag = lbl_eu_8061A520.flag;
    if ((genFlag & 0x4) && (genFlag & 3) != 3) {
        fifo::GDSetGenMode2(lbl_eu_8061A520.nTexGens, lbl_eu_8061A520.nChans,
                            lbl_eu_8061A520.nTevs, lbl_eu_8061A520.nInds,
                            lbl_eu_8061A520.cullMode);
        lbl_eu_8061A520.flag |= 3;
    }
}

} // namespace g3d
} // namespace nw4r


void LoadResTexSrt__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResTexSrt(){}

void LoadResShpPrimitive__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResShpPCQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34(){}

void SetViewPosNrmMtxArray__Q34nw4r3g3d8G3DStateFPCQ34nw4r4math5MTX34PCQ34nw4r4math5MTX33PCQ34nw4r4math5MTX34(){}

void GetViewNrmMtxPtr__Q34nw4r3g3d8G3DStateFUl(){}

u32 GetIndMtxOp__Q34nw4r3g3d8G3DStateFv(void) {
    extern u32 lbl_eu_8066346C;
    return lbl_eu_8066346C;
}


void LoadFog__Q34nw4r3g3d8G3DStateFi(){}



// Light object array (retail global at 8061B574).
extern char lbl_eu_8061B574[];

void* GetLightObj__Q34nw4r3g3d8G3DStateFi(int index) {
    if (index < 0 || index >= 128) return nullptr;
    return &lbl_eu_8061B574[index * 0x44 + 0x2c];
}

namespace nw4r {
namespace g3d {

void G3DState::SetAmbLightObj(const AmbLightObj& rObj, int idx) {
    if (idx < 0) {
        return;
    }
    if (idx >= 0x80) {
        return;
    }
    AmbLightObj* dst = reinterpret_cast<AmbLightObj*>(lbl_eu_8061B574 + idx * 4 + 0x222c);
    *dst = rObj;
}

} // namespace g3d
} // namespace nw4r

void LoadLightSet__Q34nw4r3g3d8G3DStateFiPUlPUlPUlPUlPQ34nw4r3g3d11AmbLightObj(){}

extern const unsigned char lbl_eu_8061DFA0[];

const void* GetCameraMtxPtr__Q34nw4r3g3d8G3DStateFv() {
    const unsigned char* base = lbl_eu_8061DFA0;
    unsigned short index = *(const unsigned short*)(base + 2);
    return base + index * 0x30 + 0x34;
}

void SetRenderModeObj__Q34nw4r3g3d8G3DStateFRC16_GXRenderModeObj(){}



// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace nw4r { namespace g3d { class LightSetting; }}
namespace nw4r { namespace g3d { class LightObj; }}
namespace nw4r { namespace g3d { class AmbLightObj; }}
namespace nw4r {
    namespace g3d {
        namespace G3DState {
            namespace {
                struct CameraMtxState {
                    u16 mFlag;                 // at 0x0
                    u16 mCurCameraID;          // at 0x2
                    math::MTX34 mViewMtx;      // at 0x4
                    math::MTX34 mViewMtxArray[0x20];  // at 0x34
                    math::MTX44 mProjMtxArray[0x20];  // at 0x634
                    math::MTX34 mNrmMtxArray[0x20];   // at 0xE34
                    math::MTX34 mEnvMtxArray[0x20];   // at 0x1434

                    CameraMtxState();
                };

                CameraMtxState::CameraMtxState() {
                    mFlag = 0;
                    mCurCameraID = 0;
                    PSMTXIdentity(mViewMtx);

                    for (u32 i = 0; i < 0x20; i++) {
                        PSMTXIdentity(mViewMtxArray[i]);
                        MTX44Identity(&mProjMtxArray[i]);
                        PSMTXIdentity(mNrmMtxArray[i]);
                        PSMTXIdentity(mEnvMtxArray[i]);
                    }
                }

                // Retail camera-state blob (8061DFA0). C linkage keeps the
                // symbol name unmangled inside this namespace.
                extern "C" CameraMtxState lbl_eu_8061DFA0;

                // File-scope instance so MWCC emits the ctor body (the retail
                // __sinit calls it on the camera-state blob at 8061DFA0).
                static CameraMtxState g_CameraMtxState;
            }
        }
    }
}
namespace nw4r {
    namespace g3d {
        namespace G3DState {
            namespace {
                class LightState {
                    public:
                    nw4r::g3d::LightSetting mSetting;      // at 0x0
                    u32 field_0x10;                        // at 0x10
                    u32 mDiffColorMask;                    // at 0x14
                    u32 mDiffAlphaMask;                    // at 0x18
                    u32 mSpecColorMask;                    // at 0x1C
                    u32 mSpecAlphaMask;                    // at 0x20
                    s8 mLightObjIndex[8];                  // at 0x24
                    nw4r::g3d::LightObj mLightObj[0x80];   // at 0x2C
                    nw4r::g3d::AmbLightObj mAmbLightObj[0x80]; // at 0x222C
                    nw4r::g3d::LightSetData mLightSetData[0x80]; // at 0x242C

                    void SetLightSetting(const nw4r::g3d::LightSetting&);
                    void SetLightObj(const nw4r::g3d::LightObj&, int);
                    void LoadLightSet(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*, nw4r::g3d::AmbLightObj*);
                    ~LightState();
                };
            } // anonymous namespace

            void SetLightSetting(const LightSetting& rSetting) {
                reinterpret_cast<LightState*>(lbl_eu_8061B574)->SetLightSetting(
                    rSetting);
            }

            void SetLightObj(const LightObj& rObj, int idx) {
                reinterpret_cast<LightState*>(lbl_eu_8061B574)->SetLightObj(
                    rObj, idx);
            }

            // LightState::SetLightSetting / SetLightObj are separate retail
            // functions (not session targets); they are implemented here so
            // the G3DState thunks above emit a real tail call instead of
            // MWCC inlining an empty stub.
            namespace {
                void LightState::SetLightSetting(const nw4r::g3d::LightSetting& rSetting) {
                    mSetting.Import(rSetting);

                    mSetting.ApplyViewMtx(
                        lbl_eu_8061DFA0.mViewMtxArray[lbl_eu_8061DFA0.mCurCameraID],
                        rSetting.GetNumLightObj());

                    field_0x10 = -1;
                    mDiffColorMask = 0;
                    mDiffAlphaMask = 0;
                    mSpecColorMask = 0;
                    mSpecAlphaMask = 0;

                    u32 numLight = mSetting.GetNumLightObj();
                    u32 loaded = 0;

                    for (int i = 0; i < 8; i++) {
                        mLightObjIndex[7 - i] = -1;
                    }

                    for (u32 i = 0; loaded < 8 && i < numLight; i++) {
                        LightObj& rObj = mSetting.GetLightObjArray()[i];
                        if (rObj.IsEnable()) {
                            mLightObjIndex[loaded] = static_cast<s8>(i);
                            GXLoadLightObjImm(static_cast<GXLightObj*>(rObj),
                                              static_cast<GXLightID>(1 << loaded));
                            loaded++;
                        }
                    }
                }

                void LightState::SetLightObj(const nw4r::g3d::LightObj& rObj,
                                             int idx) {
                    if (idx < 0 || idx >= 0x80) {
                        return;
                    }

                    LightObj tmp(rObj);

                    const unsigned char* pCamBase = ::lbl_eu_8061DFA0;
                    u16 camID = *reinterpret_cast<const u16*>(pCamBase + 2);
                    const math::MTX34* pViewMtx =
                        reinterpret_cast<const math::MTX34*>(
                            pCamBase + camID * 0x30 + 0x34);
                    tmp.ApplyViewMtx(*pViewMtx);

                    LightObj* pDst = &mLightObj[idx];
                    if (tmp != *pDst) {
                        *pDst = tmp;

                        for (u32 i = 0; i < 8; i++) {
                            if (mLightObjIndex[i] != idx) {
                                continue;
                            }

                            if (tmp.IsEnable()) {
                                u32 bit = 1 << i;
                                GXLoadLightObjImm(static_cast<GXLightObj*>(tmp),
                                                  static_cast<GXLightID>(bit));

                                if (bit & ((mSpecAlphaMask | mSpecColorMask) |
                                           (mDiffColorMask | mDiffAlphaMask))) {
                                    if (tmp.IsSpecularLight()) {
                                        if (tmp.IsColorEnable()) {
                                            mSpecColorMask |= bit;
                                        } else {
                                            mSpecColorMask &= ~bit;
                                        }
                                        if (tmp.IsAlphaEnable()) {
                                            mSpecAlphaMask |= bit;
                                        } else {
                                            mSpecAlphaMask &= ~bit;
                                        }
                                        mDiffColorMask &= ~bit;
                                        mDiffAlphaMask &= ~bit;
                                    } else {
                                        if (tmp.IsColorEnable()) {
                                            mDiffColorMask |= bit;
                                        } else {
                                            mDiffColorMask &= ~bit;
                                        }
                                        if (tmp.IsAlphaEnable()) {
                                            mDiffAlphaMask |= bit;
                                        } else {
                                            mDiffAlphaMask &= ~bit;
                                        }
                                        mSpecColorMask &= ~bit;
                                        mSpecAlphaMask &= ~bit;
                                    }
                                }
                            } else {
                                mLightObjIndex[i] = -1;
                                u32 notBit = ~(1 << i);
                                mSpecColorMask &= notBit;
                                mSpecAlphaMask &= notBit;
                                mDiffColorMask &= notBit;
                                mDiffAlphaMask &= notBit;
                            }
                        }
                    }
                }
                void LightState::LoadLightSet(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*, nw4r::g3d::AmbLightObj*) {}
                LightState::~LightState() {
                    extern void __dt__Q34nw4r3g3d8LightObjFv(void*, int);

                    __destroy_arr(mLightObj,
                                  reinterpret_cast<ConstructorDestructor*>(
                                      const_cast<void*>(reinterpret_cast<
                                          const void*>(
                                          &__dt__Q34nw4r3g3d8LightObjFv))),
                                  sizeof(nw4r::g3d::LightObj), 0x80);
                }
            } // anonymous namespace

            void SetCameraProjMtx(const Camera& rCam, int id, bool view) {
                if (id < NUM_CAMERA && id >= 0) {
                    rCam.GetCameraMtx(&lbl_eu_8061DFA0.mViewMtxArray[id]);
                    rCam.GetProjectionMtx(&lbl_eu_8061DFA0.mProjMtxArray[id]);
                    rCam.GetProjectionTexMtx(&lbl_eu_8061DFA0.mNrmMtxArray[id]);
                    rCam.GetEnvironmentTexMtx(&lbl_eu_8061DFA0.mEnvMtxArray[id]);

                    if (view) {
                        lbl_eu_8061DFA0.mCurCameraID = static_cast<u16>(id);
                        lbl_eu_8061DFA0.mFlag &= ~1;
                    }
                }
            }
        }
    }
}

// The retail TU emits the LightSetting/LightObj destructors out-of-line
// (__dt__Q34nw4r3g3d12LightSettingFv / __dt__Q34nw4r3g3d8LightObjFv) even
// though g3d_light.h declares them inline. File-scope instances make MWCC
// register their destructors in __sinit, forcing the standalone bodies to be
// emitted (other TUs link against these __dt__ symbols). The instances
// themselves are never referenced.
static nw4r::g3d::LightObj g_dummyLightObj;
static nw4r::g3d::LightSetting g_dummyLightSetting(0, 0, 0, 0, 0);

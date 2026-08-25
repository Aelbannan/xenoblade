// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_state
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/g3d/res/g3d_resmat.h>
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

// Tex cache (retail .bss at 8061A5A0): one cached GXTexObj per texmap ID;
// the byte at 0x100 tracks which must be re-uploaded after an edit.
struct TexCacheState {
    GXTexObj texObj[8]; // at 0x0 (8 x 0x20-byte objects)
    u8 flag;            // at 0x100
};
extern TexCacheState lbl_eu_8061A5A0;

// Per-texcoord scale pair of the texCoordScale cache.
struct TexCoordScaleEntry {
    u16 scaleS; // at 0x0
    u16 scaleT; // at 0x2
};

// Cached-dst entries are 8 bytes wide (extra fields beyond the scale pair),
// which puts dirty[] at cache+0x64 like retail.
struct TexCoordScaleDstEntry {
    u16 scaleS;    // at 0x0
    u16 scaleT;    // at 0x2
    u16 field_0x4; // at 0x4
    u16 field_0x6; // at 0x6
};

// Texcoord-scale cache at 8061A52C (offset 0xC inside the state blob).
// Retail also emits this address as its own .bss symbol, so the source
// keeps a standalone extern for it.
struct TexCoordScaleCache {
    u32 flag;                  // at 0x0  (bit0 = scales re-emitted, bit1 = tex invalidated)
    TexCoordScaleEntry src[8]; // at 0x4  per-source scale pairs (4-byte stride)
    TexCoordScaleDstEntry dst[8]; // at 0x24 cached pairs (8-byte stride)
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

    TexCoordScaleCache texCoordScale;      // at 0xC (ends 0x58)
    u8 PADDING_0x58[0x80 - 0x58];          // at 0x58

    // Wrapper depth reproduces retail's split addressing (addi rN,r4,off;
    // stb 0xNN(rN)) for the far byte/half caches.
    struct TexCacheHolder { TexCacheState cache; };
    struct TlutCacheHolder { TlutCacheState cache; };

    TexCacheHolder texCache;               // at 0x80
    u8 PADDING_0x181[0x1A0 - 0x181];       // at 0x181

    TlutCacheHolder tlutCache;             // at 0x1A0
    u8 PADDING_0x202[0x204 - 0x202];       // at 0x202

    u32 vtxDescv[3];                       // at 0x204  cached GX vtx desc list
    u32 curMtx[8];                         // at 0x210  current-matrix id cache

    // View-matrix pointer triple (retail 0x230..0x23C).
    struct ViewMtxPtrs {
        const nw4r::math::MTX34* pos; // at 0x0
        const nw4r::math::MTX33* nrm; // at 0x4
        const nw4r::math::MTX34* env; // at 0x8
    };
    ViewMtxPtrs viewMtxPtrs;               // at 0x230
    u8 PADDING_0x23C[0xA40 - 0x23C];       // at 0x23C

    u32 fogFlag;                           // at 0xA40  (FogState.flags)
    u8 PADDING_0xA44[0x1054 - 0xA44];      // at 0xA44  (LightState.mSetting at 0x1054..)

    // Light-invalidate block (retail base for the LIGHT case is blob+0x1054).
    struct LightInvalBlock {
        u8 pad[0x10];                  // at 0x0   (LightState.mSetting tail)
        u32 field_0x10;                // at 0x10
        u32 diffColorMask;             // at 0x14
        u32 diffAlphaMask;             // at 0x18
        u32 specColorMask;             // at 0x1C
        u32 specAlphaMask;             // at 0x20
        s8 objIndex[8];                // at 0x24
    };
    LightInvalBlock lightInval;            // at 0x1054
};
extern G3DStateCache lbl_eu_8061A520;
extern TexCoordScaleCache lbl_eu_8061A52C;

// "sync" flag: cleared before every display-list upload.
extern bool lbl_eu_80665448;

// Small-data flags in g3d_state.o's retail .sbss.
extern u8 lbl_eu_8066544C;
extern u32 lbl_eu_80665450;
extern u32 lbl_eu_80665454;
// Z-comploc cache (retail small-data at 80665460): bit0 of the flag word
// marks the cached byte valid.
struct ZCompLocState {
    u32 flag;   // at 0x0
    u8 compLoc; // at 0x4
};
extern ZCompLocState lbl_eu_80665460;

// Float constants in g3d_state.o's retail .sdata2 (IndMtxOpStd::SetNrmMapMtx
// and ScnDependentMtxFunc::EnvironmentMapping).
extern const float lbl_eu_80669BEC;  // 0.0f
extern const float lbl_eu_80669BF0; // 1.0f (environment-mapping up vector)
extern const float lbl_eu_80669BF4; // epsilon (environment-mapping dir test)
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
            // Retail stores the normalized components in reverse column order.
            mIndMtx[i].m[1][0] = -v.z * lbl_eu_80669BFC;
            mIndMtx[i].m[1][1] = -v.y * lbl_eu_80669BFC;
            mIndMtx[i].m[1][2] = -v.x * lbl_eu_80669BFC;
        } else {
            mIndMtx[i].m[1][2] = lbl_eu_80669BEC;
            mIndMtx[i].m[1][1] = lbl_eu_80669BEC;
            mIndMtx[i].m[1][0] = lbl_eu_80669BEC;
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
    // Local copy: MWCC materializes it as load-member/store-slot at entry,
    // freeing the parameter register - matches retail's early homing.
    Fog f = fog;

    if (id >= 0 && id < 0x20 && f.ptr() != NULL) {
        FogState* pState = &lbl_eu_8061AF60;

        // Skip when the same fog was already uploaded.
        if (lbl_eu_8061AF60.curFogID == id &&
            memcmp(f.ptr(), &pState->fogArray[id],
                   sizeof(FogData)) == 0) {
            return;
        }

        lbl_eu_8061AF60.flags = 0;
        f.CopyTo(&pState->fogArray[id]);
    }
}

void G3DState::LoadFog(int id) {
    if ((lbl_eu_8061AF60.flags & 1) && (lbl_eu_8061AF60.flags & 2) &&
        id == lbl_eu_8061AF60.curFogID) {
        return;
    }

    if (id < 0 || id >= 0x20) {
        GXColor color = {0, 0, 0, 0};
        GXSetFog(GX_FOG_NONE, color, lbl_eu_80669BEC, lbl_eu_80669BEC,
                 lbl_eu_80669BEC, lbl_eu_80669BEC);
    } else {
        // Chained temporary keeps the ctor result in r3 for SetGP.
        Fog(&lbl_eu_8061AF60.fogArray[id]).SetGP();
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

// Body runs on a pointer parameter: after -inline auto folds it into
// Invalidate, the parameter stays an opaque base register, reproducing
// retail's per-block subobject addressing.
namespace {

using G3DState::INVALIDATE_TEX;
using G3DState::INVALIDATE_TLUT;
using G3DState::INVALIDATE_TEV;
using G3DState::INVALIDATE_GENMODE;
using G3DState::INVALIDATE_SHP;
using G3DState::INVALIDATE_CURRMTX;
using G3DState::INVALIDATE_TEXMTX;
using G3DState::INVALIDATE_MISC;
using G3DState::INVALIDATE_FOG;
using G3DState::INVALIDATE_LIGHT;
using G3DState::INVALIDATE_POSMTX;

void InvalidateBody(G3DStateCache* pState, u32 flag) {
    if (flag & INVALIDATE_TEX) {
        pState->texCache.cache.flag = 0;
        pState->texCoordScale.flag = 0;
    }

    if (flag & INVALIDATE_TLUT) {
        pState->tlutCache.cache.flag = 0;
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
        pState->vtxDescv[2] = 0;
        pState->vtxDescv[1] = 0;
        pState->vtxDescv[0] = 0;
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
        lbl_eu_80665460.flag &= ~1;
    }

    if (flag & INVALIDATE_FOG) {
        pState->fogFlag = 0;
    }

    if (flag & INVALIDATE_LIGHT) {
        pState->lightInval.field_0x10 = -1;
        pState->lightInval.diffColorMask = 0;
        pState->lightInval.diffAlphaMask = 0;
        pState->lightInval.specColorMask = 0;
        pState->lightInval.specAlphaMask = 0;
        for (int i = 7; i >= 0; i--) {
            pState->lightInval.objIndex[i] = -1;
        }
    }

    if (flag & INVALIDATE_POSMTX) {
        pState->viewMtxPtrs.pos = NULL;
        pState->viewMtxPtrs.nrm = NULL;
        pState->viewMtxPtrs.env = NULL;
    }
}

} // namespace

void G3DState::Invalidate(u32 flag) {
    InvalidateBody(&lbl_eu_8061A520, flag);
    lbl_eu_80665448 = 1;
}

// The resource pointer is cached once; each getter re-tests it exactly like
// the inline ResGenMode accessors so MWCC keeps it in a single register.
void G3DState::LoadResGenMode(ResGenMode mode) {
    ResGenModeData* pRes = mode.ptr();
    if (pRes == NULL) {
        return;
    }

    if (lbl_eu_8061A520.nTexGens !=
        (u8)(pRes != NULL ? pRes->nTexGens : 0)) {
        lbl_eu_8061A520.nTexGens = (u8)(pRes != NULL ? pRes->nTexGens : 0);
        lbl_eu_8061A520.flag &= ~3;
    }

    if (lbl_eu_8061A520.nChans != (u8)(pRes != NULL ? pRes->nChans : 0)) {
        lbl_eu_8061A520.nChans = (u8)(pRes != NULL ? pRes->nChans : 0);
        lbl_eu_8061A520.flag &= ~3;
    }

    if (lbl_eu_8061A520.nTevs != (u8)(pRes != NULL ? pRes->nTevs : 0)) {
        lbl_eu_8061A520.nTevs = (u8)(pRes != NULL ? pRes->nTevs : 0);
        lbl_eu_8061A520.flag &= ~1;
    }

    if (lbl_eu_8061A520.nInds != (u8)(pRes != NULL ? pRes->nInds : 0)) {
        lbl_eu_8061A520.nInds = (u8)(pRes != NULL ? pRes->nInds : 0);
        lbl_eu_8061A520.flag &= ~1;
    }

    // Evaluated accessor-first to match MWCC's scheduling of the word compare.
    GXCullMode cull =
        pRes != NULL ? pRes->cullMode : GX_CULL_ALL;
    if (lbl_eu_8061A520.cullMode != cull) {
        lbl_eu_8061A520.cullMode = pRes != NULL ? pRes->cullMode : GX_CULL_ALL;
        lbl_eu_8061A520.flag &= ~1;
    }

    G3DStateCache* pCache = &lbl_eu_8061A520;
    if (!(pCache->flag & 4)) {
        pCache->flag = (pCache->flag & ~3) | 4;
    }
}

void G3DState::LoadResShpPrePrimitive(ResShp shp) {
    // Declared before the validity test so MWCC schedules the cache base
    // address setup ahead of the null-check branch, like retail.
    G3DStateCache* pState = &lbl_eu_8061A520;

    if (shp.ptr() == NULL) {
        return;
    }

    // Declaration order drives MWCC's creation-order callee-saved
    // coloring: pScale -> i -> texgen count; pScale is assigned lazily so
    // its address computation is emitted inside the branch like retail.
    TexCoordScaleCache* pScale;
    u8 i;
    u8 nTexGens = pState->nTexGens;

    // Re-emit texcoord scales when the texture cache was invalidated since
    // the last shape upload and the cached scales have not been re-emitted.
    if ((pState->texCoordScale.flag & 2) &&
        (pState->texCoordScale.flag & 1) == 0 && nTexGens != 0) {
        pScale = &pState->texCoordScale;
        for (i = 0; i < nTexGens; i++) {
            // The dst-cache updates are assignment expressions inside the
            // call arguments; evaluating them in-place reproduces retail's
            // exact argument-setup interleaving.
            if (pScale->dirty[i] != 0xFF) {
                fifo::GDSetTexCoordScale2(
                    static_cast<GXTexCoordID>(i),
                    (pScale->dst[i].scaleS =
                         pScale->src[pScale->dirty[i]].scaleS),
                    false, false,
                    (pScale->dst[i].scaleT =
                         pScale->src[pScale->dirty[i]].scaleT),
                    false, false);
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
    // Materialized comparison result plus per-arm reassignment reproduces
    // retail's default-zero / set-on-match / re-test sequence.
    bool cacheIsSame =
        pData->cache.data_u32[0] == pState->vtxDescv[0] &&
        pData->cache.data_u32[1] == pState->vtxDescv[1] &&
        pData->cache.data_u32[2] == pState->vtxDescv[2];

    bool uploadDesc;
    if (cacheIsSame) {
        uploadDesc = true;
    } else {
        pState->vtxDescv[0] = pData->cache.data_u32[0];
        pState->vtxDescv[1] = pData->cache.data_u32[1];
        pState->vtxDescv[2] = pData->cache.data_u32[2];
        uploadDesc = false;
    }

    bool sync = lbl_eu_80665448;
    lbl_eu_80665448 = 0;
    shp.CallPrePrimitiveDisplayList(sync, uploadDesc);
}

void G3DState::LoadResTlutObj(const ResTlutObj tlutObj) {
    // Explicit bool-normalize reproduces MWCC's neg/or/srwi test.
    const ResTlutObjData* pData = tlutObj.ptr();
    if ((((u32) - (s32)pData | (u32)pData) >> 31) != 0) {
        // Copy rebuilt from the already-loaded pointer so MWCC CSEs the
        // single mPtr load for both the test and the stack home.
        const ResTlutObj obj(const_cast<void*>((const void*)pData));

        TexCacheState* pTexCache = &lbl_eu_8061A5A0;
        TlutCacheState* pTlutCache = &lbl_eu_8061A6C0;

        for (u32 i = 0; i < 8; i++) {
            if (!obj.IsValidTlut(static_cast<GXTlut>(i))) {
                continue;
            }

            const GXTlutObj* pObj = obj.GetTlut(static_cast<GXTlut>(i));
            u16 bit = static_cast<u16>(1 << i);

            // Skip the upload when this TLUT was cached and still matches.
            if (pTlutCache->flag & bit) {
                bool b1 = pObj->dummy[0] == pTlutCache->tlutObj[i].dummy[0];
                bool b2 = false;
                if (b1 && pObj->dummy[1] == pTlutCache->tlutObj[i].dummy[1]) {
                    b2 = pObj->dummy[2] == pTlutCache->tlutObj[i].dummy[2];
                }
                if (b2) {
                    continue;
                }
            }

            pTlutCache->flag |= bit;
            pTlutCache->tlutObj[i] = *pObj;
            GXLoadTlut(const_cast<GXTlutObj*>(pObj), static_cast<GXTlut>(i));
            pTexCache->flag &= ~(1 << i);
        }

        lbl_eu_80665448 = true;
    }
}

} // namespace g3d
} // namespace nw4r


// Static projection/texmtx blob folded into environment matrices by
// ScnDependentMtxFunc::EnvironmentMapping (retail .data at 8051D660).
extern const unsigned char lbl_eu_8051D660[];

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

// G3DState::IndTexMtxInfo::FifoSend: uploads each cached indirect matrix
// whose dirty bit (flag bits 0..2) is set, using the retail matrix IDs
// (0, 3, 6 = ITM stages with identity scale).
void FifoSend__Q44nw4r3g3d8G3DState13IndTexMtxInfoCFv(
    const nw4r::g3d::G3DState::IndTexMtxInfo* pThis) {
    if (pThis->flag & 1) {
        nw4r::g3d::fifo::GDSetIndTexMtx(0, pThis->offset_mtx[0]);
    }
    if (pThis->flag & 2) {
        nw4r::g3d::fifo::GDSetIndTexMtx(3, pThis->offset_mtx[1]);
    }
    if (pThis->flag & 4) {
        nw4r::g3d::fifo::GDSetIndTexMtx(6, pThis->offset_mtx[2]);
    }
}




namespace nw4r {
namespace g3d {
namespace G3DState {

// LoadResTexObj: per-texmap upload with an 8-word cache compare.
void LoadResTexObj(const ResTexObj texObj) {
    // Local copy + explicit bool-normalize reproduces MWCC's neg/or/srwi
    // null test and stack home of the resource handle.
    const ResTexObjData* pData = texObj.ptr();
    if ((((u32) - (s32)pData | (u32)pData) >> 31) != 0) {
        const ResTexObj obj(const_cast<void*>((const void*)pData));

        TexCacheState* pTexCache = &lbl_eu_8061A5A0;

        for (u32 i = 0; i < 8; i++) {
            if (!obj.IsValidTexObj(static_cast<GXTexMapID>(i))) {
                continue;
            }

            const GXTexObj* pObj = obj.GetTexObj(static_cast<GXTexMapID>(i));
            u32 bit = 1 << i;

            // Skip the upload when this texmap was cached and all 8 words of
            // the cached GXTexObj still match the resource's copy.
            if (pTexCache->flag & bit) {
                // Grouped materialized-bool chain: each stage is guarded by
                // the previous stage's bool and assigns a fresh one.
                bool b01 = false;
                if (pObj->dummy[0] == pTexCache->texObj[i].dummy[0] &&
                    pObj->dummy[1] == pTexCache->texObj[i].dummy[1]) {
                    b01 = true;
                }
                bool b012 = false;
                if (b01 && pObj->dummy[2] == pTexCache->texObj[i].dummy[2]) {
                    b012 = true;
                }
                bool b0123 = false;
                if (b012 && pObj->dummy[3] == pTexCache->texObj[i].dummy[3]) {
                    b0123 = true;
                }
                bool b01234 = false;
                if (b0123 && pObj->dummy[4] == pTexCache->texObj[i].dummy[4]) {
                    b01234 = true;
                }
                bool b012345 = false;
                if (b01234 && pObj->dummy[5] == pTexCache->texObj[i].dummy[5]) {
                    b012345 = true;
                }
                bool b0123456 = false;
                if (b012345 && pObj->dummy[6] == pTexCache->texObj[i].dummy[6]) {
                    b0123456 = true;
                }
                bool bAll = false;
                if (b0123456 && pObj->dummy[7] == pTexCache->texObj[i].dummy[7]) {
                    bAll = true;
                }
                if (bAll) {
                    continue;
                }
            }

            pTexCache->flag |= static_cast<u8>(bit);
            pTexCache->texObj[i] = *pObj;
            GXLoadTexObj(pObj, static_cast<GXTexMapID>(i));

            // Remember the uploaded texture dimensions in the scale cache.
            u16 height = GXGetTexObjHeight(pObj);
            lbl_eu_8061A52C.src[i].scaleS = GXGetTexObjWidth(pObj);
            lbl_eu_8061A52C.src[i].scaleT = height;

            lbl_eu_8061A52C.flag = (lbl_eu_8061A52C.flag | 2) & ~1u;
        }

        lbl_eu_80665448 = true;
    }
}

} // namespace G3DState
} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

// G3DState::LoadResMatIndMtxAndScale (two-arg form): uploads the indirect
// matrix display list, collects the three resource indirect matrices into a
// stack IndTexMtxInfo, lets the caller's IndMtxOp override them virtually,
// then emits whichever matrices remain flagged.
void G3DState::LoadResMatIndMtxAndScale(const ResMatIndMtxAndScale ind,
                                        IndMtxOp& rOp) {
    if (!ind.IsValid()) {
        return;
    }

    u32 flag = lbl_eu_8061A520.flag;
    if ((flag & 0x4) && (flag & 3) == 2) {
        // Re-emit the genMode BP word (shared dirty genMode state).
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

    IndTexMtxInfo info;
    if (ind.GXGetIndTexMtx(static_cast<GXIndTexMtxID>(1),
                           &info.offset_mtx[0])) {
        info.flag |= 1;
    }
    if (ind.GXGetIndTexMtx(static_cast<GXIndTexMtxID>(2),
                           &info.offset_mtx[1])) {
        info.flag |= 2;
    }
    if (ind.GXGetIndTexMtx(static_cast<GXIndTexMtxID>(3),
                           &info.offset_mtx[2])) {
        info.flag |= 4;
    }

    rOp(&info);

    // Inline equivalent of IndTexMtxInfo::FifoSend.
    if (info.flag & 1) {
        fifo::GDSetIndTexMtx(0, info.offset_mtx[0]);
    }
    if (info.flag & 2) {
        fifo::GDSetIndTexMtx(3, info.offset_mtx[1]);
    }
    if (info.flag & 4) {
        fifo::GDSetIndTexMtx(6, info.offset_mtx[2]);
    }
}

} // namespace g3d
} // namespace nw4r


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
    // (xoris/stw/lfd/fsub vs the pooled magic constant, retail .sdata2
    // lbl_eu_80669C08).
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

void G3DState::LoadResTev(ResTev tev) {
    // Local copy of the by-value resource handle (best-known shape).
    // NOTE: the true retail body continues past the scale loop with
    // additional tev-dependent work not present here; see open-item notes.
    ResTev res = tev;

    if (res.ptr() == NULL) {
        return;
    }

    // Materialized comparison result plus per-arm reassignment reproduces
    // retail's default-cached / set-on-change / re-test sequence: the TEV
    // display list is only uploaded when the resource pointer changed.
    // Cache-on-left flips the compare operand order to match retail.
    bool cacheIsSame;
    if (lbl_eu_80665454 == reinterpret_cast<u32>(res.ptr())) {
        cacheIsSame = true;
    } else {
        lbl_eu_80665454 = reinterpret_cast<u32>(res.ptr());
        cacheIsSame = false;
    }

    if (!cacheIsSame) {
        // Re-emit the genMode BP word when the pix/tev caches dirtied it.
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
        res.CallDisplayList(sync);
    }

    // Declaration order drives MWCC's callee-saved coloring: the scale-flag
    // word and the texgen count are read ahead of the branch, while
    // pScale/i are assigned lazily inside it.
    // Declared ahead of the branch so MWCC retains the high half of the
    // cache address across it; the pointer itself is only materialized at
    // its first use inside the loop (retail addi-from-shared-base shape).
    TexCoordScaleCache* pScale = &lbl_eu_8061A52C;
    u8 i;
    u8 nTexGens = lbl_eu_8061A520.nTexGens;

    // Re-emit texcoord scales when the texture cache was invalidated since
    // the last upload and the cached scales have not been re-emitted yet.
    if ((pScale->flag & 2) && (pScale->flag & 1) == 0 && nTexGens != 0) {
        for (i = 0; i < nTexGens; i++) {
            // The dst-cache updates are assignment expressions inside the
            // call arguments; evaluating them in-place reproduces retail's
            // exact argument-setup interleaving.
            if (pScale->dirty[i] != 0xFF) {
                fifo::GDSetTexCoordScale2(
                    static_cast<GXTexCoordID>(i),
                    (pScale->dst[i].scaleS =
                         pScale->src[pScale->dirty[i]].scaleS),
                    false, false,
                    (pScale->dst[i].scaleT =
                         pScale->src[pScale->dirty[i]].scaleT),
                    false, false);
            }
        }
        lbl_eu_8061A52C.flag |= 1;
    }
}

} // namespace g3d
} // namespace nw4r

// Scene-dependent texmtx dispatch table (retail .bss at 8061A760): one
// {function,type} pair per TexMtxEffect::map_mode, plus the per-texture
// cache (8061A730) of the function type last used for each texcoord.
struct ScnDependentTexMtxEntry {
    nw4r::g3d::G3DState::ScnDependentTexMtxFuncPtr func; // at 0x0
    unsigned long type;                                  // at 0x4
};
extern ScnDependentTexMtxEntry lbl_eu_8061A760[nw4r::g3d::G3DState::NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE];
extern unsigned long lbl_eu_8061A730[8];

namespace nw4r {
namespace g3d {
namespace G3DState {

// G3DState::LoadResTexSrt: rebuild and upload each texcoord matrix.
// Per texture i: start from identity, optionally apply the scene-dependent
// mapping function selected by effect[i].map_mode, apply the resource
// effect matrix, fold in the animated TexSrt via CalcTexMtx, then upload
// either the result or an identity matrix (tracking validity in the
// lbl_eu_80665450 dirty-flag word).
void LoadResTexSrt(const ResTexSrt srt) {
    const ResTexSrtData* pData = srt.ptr();
    u32 texMtxId = GX_PTTEXMTX0;

    for (u32 i = 0; i < ResTexSrtData::NUM_OF_TEXTURE; i++) {
        const ResTexSrtData* p = srt.ptr();
        u32 shift = i * TexSrt::NUM_OF_FLAGS;

        if (p != NULL && (p->flag & (1 << shift))) {
            const TexMtxEffect* pEffect = &p->effect[i];
            math::MTX34 mtx;
            PSMTXIdentity(mtx);
            bool bSet = true;

            u32 funcIdx = pEffect->map_mode;
            if (funcIdx >= NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE) {
                funcIdx = 0;
            }

            if (pEffect->map_mode != 0) {
                (*lbl_eu_8061A760[funcIdx].func)(
                    &mtx,
                    pEffect->ref_camera >= NUM_CAMERA ? -1 : pEffect->ref_camera,
                    pEffect->ref_light >= NUM_LIGHT ? -1 : pEffect->ref_light);
                lbl_eu_8061A730[i] = lbl_eu_8061A760[funcIdx].type;
                bSet = false;
            } else {
                lbl_eu_8061A730[i] = 0;
            }

            // Only rebuild when the resource requests a full transform
            // (all four TexSrt flags set means "identity allowed").
            bool isIdentity = false;
            if (((p->flag >> shift) & 0xF) == 0xF &&
                (pEffect->misc_flag & TexMtxEffect::FLAG_IDENT)) {
                isIdentity = true;
            }

            if (!isIdentity) {
                if (bSet) {
                    if (!(pEffect->misc_flag & TexMtxEffect::FLAG_IDENT)) {
                        PSMTXCopy(
                            *reinterpret_cast<const math::MTX34*>(
                                &pEffect->effectMtx),
                            mtx);
                        bSet = false;
                    }
                } else {
                    PSMTXConcat(
                        *reinterpret_cast<const math::MTX34*>(
                            &pEffect->effectMtx),
                        mtx, mtx);
                }

                TexSrtTypedef::TexMatrixMode mode =
                    static_cast<TexSrtTypedef::TexMatrixMode>(
                        (p->flag >> shift) & 0xF);

                if (pEffect->map_mode == 0) {
                    CalcTexMtx(&mtx, bSet, p->texSrt[i],
                               static_cast<TexSrt::Flag>(p->texMtxMode), mode);
                } else {
                    // Environment-style matrices are built in transposed
                    // column order; swap columns 1/2 of rows 0/2 afterwards.
                    math::MTX34 envMtx;
                    CalcTexMtx(&envMtx, true, p->texSrt[i],
                               static_cast<TexSrt::Flag>(p->texMtxMode), mode);

                    f32 swap1 = envMtx.m[0][1];
                    f32 swap2 = envMtx.m[2][1];
                    envMtx.m[0][1] = envMtx.m[0][2];
                    envMtx.m[2][1] = envMtx.m[2][2];
                    envMtx.m[0][2] = swap1;
                    envMtx.m[2][2] = swap2;

                    PSMTXConcat(envMtx, mtx, mtx);
                }
                bSet = false;
            }

            if (bSet) {
                // Matrix unchanged: only upload a fresh identity once.
                u32 bit = 1 << i;
                if (!(lbl_eu_80665450 & bit)) {
                    lbl_eu_80665450 |= bit;
                    math::MTX34 idMtx;
                    PSMTXIdentity(idMtx);
                    GXLoadTexMtxImm(idMtx, texMtxId, GX_MTX_3x4);
                }
            } else {
                lbl_eu_80665450 &= ~(1 << i);
                GXLoadTexMtxImm(mtx, texMtxId, GX_MTX_3x4);
            }
        } else {
            lbl_eu_8061A730[i] = 0;
        }

        texMtxId += 3;
    }
}

} // namespace G3DState
} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void G3DState::LoadResShpPrimitive(const ResShp shp, const math::MTX34* pViewPos,
                                   const math::MTX34* pViewNrm) {
    G3DStateCache* pState = &lbl_eu_8061A520;
    const ResShpData* pData = shp.ptr();

    if (pData == NULL) {
        return;
    }

    // Re-emit texcoord scales when the texture cache was invalidated since
    // the last upload and the cached scales have not been re-emitted yet.
    TexCoordScaleCache* pScale;
    u8 i;
    u8 nTexGens = pState->nTexGens;
    if ((pState->texCoordScale.flag & 2) &&
        (pState->texCoordScale.flag & 1) == 0 && nTexGens != 0) {
        pScale = &pState->texCoordScale;
        for (i = 0; i < nTexGens; i++) {
            if (pScale->dirty[i] != 0xFF) {
                fifo::GDSetTexCoordScale2(
                    static_cast<GXTexCoordID>(i),
                    (pScale->dst[i].scaleS =
                         pScale->src[pScale->dirty[i]].scaleS),
                    false, false,
                    (pScale->dst[i].scaleT =
                         pScale->src[pScale->dirty[i]].scaleT),
                    false, false);
            }
        }
        pState->texCoordScale.flag |= 1;
    }

    s32 curMtxIdx = static_cast<s32>(pData->curMtxIdx);
    bool envelope = curMtxIdx < 0;

    if (!envelope) {
        if (pViewPos != NULL) {
            GXLoadPosMtxImm(*pViewPos, 0);
            if (pData->vcdBitmap & 0x400) {
                // Shapes without a dedicated normal matrix reuse the view
                // position matrix.
                if (pViewNrm != NULL) {
                    GXLoadNrmMtxImm(*pViewNrm, 0);
                } else {
                    GXLoadNrmMtxImm(*pViewPos, 0);
                }
            }
        } else {
            u32 mtxIdx = static_cast<u32>(curMtxIdx) & 0xFFFF;
            GXLoadPosMtxIndx(mtxIdx, 0);
            if (pData->vcdBitmap & 0x400) {
                GXLoadNrmMtxIndx3x3(mtxIdx, 0);
            }
        }
    }

    // TexMtx upload: per-texcoord cached matrix state decides whether to
    // recompute the view normal matrix (1), upload identity (2), or leave
    // the default light-channel matrix (default case).
    if (curMtxIdx >= 0) {
        bool bResetCurrentMtx = true;
        u32 mtxIds[8];
        u32 mtxId = 0x1E;

        for (u32 i = 0; i < 8; i++) {
            u32 cache = pState->curMtx[i];

            if (cache == 1) {
                mtxIds[i] = mtxId;
                const math::MTX33* pNrmArray =
                    pState->viewMtxPtrs.nrm;

                if (pNrmArray != NULL) {
                    fifo::GDLoadTexMtxImm3x3(pNrmArray[curMtxIdx], mtxIds[i]);
                } else {
                    if (!lbl_eu_80665458[0]) {
                        lbl_eu_80665458[0] = 1;
                    }
                    detail::CalcViewNrmMtx(
                        reinterpret_cast<math::MTX33*>(lbl_eu_8061FAB8),
                        pState->viewMtxPtrs.pos != NULL
                            ? &pState->viewMtxPtrs.pos[curMtxIdx]
                            : NULL);
                    fifo::GDLoadTexMtxImm3x3(
                        *reinterpret_cast<const math::MTX33*>(
                            lbl_eu_8061FAB8),
                        mtxIds[i]);
                }
                bResetCurrentMtx = false;
            } else if (cache == 2) {
                mtxIds[i] = 0;
                bResetCurrentMtx = false;
            } else {
                mtxIds[i] = 0x3C;
            }

            mtxId += 3;
        }

        if (bResetCurrentMtx) {
            // No texcoord matrix was touched: only reset the XF current
            // matrices once after an invalidation.
            if (!lbl_eu_8066544C) {
                fifo::GDResetCurrentMtx();
                lbl_eu_8066544C = 1;
            }
        } else {
            fifo::GDSetCurrentMtx(mtxIds);
            lbl_eu_8066544C = 0;
        }
    } else if (!lbl_eu_8066544C) {
        fifo::GDResetCurrentMtx();
        lbl_eu_8066544C = 1;
    }

    // Re-emit the genMode BP word when the pix/tev caches dirtied it.
    u32 flag = pState->flag;
    if ((flag & 0x4) && (flag & 3) != 3) {
        fifo::GDSetGenMode2(pState->nTexGens, pState->nChans, pState->nTevs,
                            pState->nInds, pState->cullMode);
        pState->flag |= 3;
    }

    bool sync = lbl_eu_80665448;
    lbl_eu_80665448 = 0;
    shp.CallPrimitiveDisplayList(sync);

    if (envelope) {
        lbl_eu_8066544C = 0;
    }
}

} // namespace g3d
} // namespace nw4r

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


// G3DState::LoadResMatMisc: keeps the GX z-comploc in sync with the
// material resource through the one-entry cache at lbl_eu_80665460.
void LoadResMatMisc__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResMatMisc(
    nw4r::g3d::ResMatMisc misc) {
    if (misc.ptr() != NULL) {
        GXBool compLoc = misc.GXGetZCompLoc();
        if ((lbl_eu_80665460.flag & 1) == 0 ||
            lbl_eu_80665460.compLoc != compLoc) {
            lbl_eu_80665460.compLoc = compLoc;
            lbl_eu_80665460.flag |= 1;
            GXSetZCompLoc(compLoc);
            lbl_eu_80665448 = true;
        }
    }
}

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
                    LightState();
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
                // Empty body: automatic member cleanup (__destroy_arr over
                // mLightObj plus the flag-gated operator delete) reproduces
                // the retail out-of-line dtor. Referenced by __sinit through
                // the static instance below.
                LightState::LightState()
                    : mSetting(NULL, NULL, 0, NULL, 0) {}

                LightState::~LightState() {}


            // Retail emits a standalone LightState::LoadLightSet body; not
            // itself a session target, but it must stay out-of-line so the
            // global thunk keeps its retail bl-forwarder shape.
            void LightState::LoadLightSet(int idx, unsigned long* pDiffColorMask,
                                          unsigned long* pDiffAlphaMask,
                                          unsigned long* pSpecColorMask,
                                          unsigned long* pSpecAlphaMask,
                                          nw4r::g3d::AmbLightObj* pAmbLightObj) {
                *pDiffColorMask = 0;
                *pDiffAlphaMask = 0;
                *pSpecColorMask = 0;
                *pSpecAlphaMask = 0;

                for (int i = 0; i < 8; i++) {
                    s8 lightIdx = mLightObjIndex[i];
                    if (lightIdx < 0) {
                        continue;
                    }

                    LightObj& rObj = mLightObj[lightIdx];
                    if (!rObj.IsEnable()) {
                        continue;
                    }

                    u32 bit = 1 << i;
                    GXLoadLightObjImm(static_cast<GXLightObj*>(rObj),
                                      static_cast<GXLightID>(bit));

                    if (rObj.IsSpecularLight()) {
                        if (rObj.IsColorEnable()) {
                            *pSpecColorMask |= bit;
                        }
                        if (rObj.IsAlphaEnable()) {
                            *pSpecAlphaMask |= bit;
                        }
                    } else {
                        if (rObj.IsColorEnable()) {
                            *pDiffColorMask |= bit;
                        }
                        if (rObj.IsAlphaEnable()) {
                            *pDiffAlphaMask |= bit;
                        }
                    }
                }

                if (idx >= 0 && idx < 0x80 && pAmbLightObj != NULL) {
                    *pAmbLightObj = mAmbLightObj[idx];
                }
            }
            } // anonymous namespace

            // Alias so the global-scope retail thunk can name the
            // anonymous-namespace LightState type.
            typedef LightState LoadLightSetState;

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

namespace nw4r {
namespace g3d {
namespace G3DState {
// File-scope instance so MWCC registers the LightState blob for static
// destruction, emitting the compiler-generated dtor out-of-line.
static LightState g_LightStateInstance;
} // namespace G3DState
} // namespace g3d
} // namespace nw4r

// G3DState::LoadLightSet: thin forwarder into the persistent LightState
// blob at lbl_eu_8061B574.
void LoadLightSet__Q34nw4r3g3d8G3DStateFiPUlPUlPUlPUlPQ34nw4r3g3d11AmbLightObj(
    int id, unsigned long* pDiffColorMask, unsigned long* pDiffAlphaMask,
    unsigned long* pSpecColorMask, unsigned long* pSpecAlphaMask,
    nw4r::g3d::AmbLightObj* pAmbLightObj) {
    reinterpret_cast<nw4r::g3d::G3DState::LoadLightSetState*>(lbl_eu_8061B574)
        ->LoadLightSet(id, pDiffColorMask, pDiffAlphaMask, pSpecColorMask,
                       pSpecAlphaMask, pAmbLightObj);
}

namespace nw4r {
namespace g3d {
namespace detail {
namespace ScnDependentMtxFunc {

// Builds an environment-reflection texgen matrix. Two sources: the camera
// view matrix (ref_camera path) or a scene light orientation (ref_light
// path); when neither reference is usable, copies the current environment
// matrix.
void EnvironmentMapping(math::MTX34* pMtx, s8 camRef, s8 lightRef) {
    if (pMtx != NULL) {
        if (camRef >= 0 && camRef < 0x20) {
            if (camRef >= 0 && camRef < 0x20) {
                // Lazily refresh the cached inverse of the current view
                // matrix.
                if (!(lbl_eu_8061DFA0.mFlag & 1)) {
                    PSMTXInverse(
                        lbl_eu_8061DFA0
                            .mViewMtxArray[lbl_eu_8061DFA0.mCurCameraID],
                        lbl_eu_8061DFA0.mViewMtx);
                    lbl_eu_8061DFA0.mFlag |= 1;
                }

                PSMTXConcat(lbl_eu_8061DFA0.mViewMtxArray[camRef],
                            lbl_eu_8061DFA0.mViewMtx, *pMtx);

                f32 zero = lbl_eu_80669BEC;
                pMtx->m[0][3] = pMtx->m[1][3] = pMtx->m[2][3] = zero;

                PSMTXConcat(
                    lbl_eu_8061DFA0.mEnvMtxArray[lbl_eu_8061DFA0.mCurCameraID],
                    *pMtx, *pMtx);
                return;
            }
        } else if (lightRef >= 0 && lightRef < 0x80 &&
                   reinterpret_cast<LightObj*>(
                       GetLightObj__Q34nw4r3g3d8G3DStateFi(lightRef))
                       ->IsEnable()) {
            LightObj* pLight = reinterpret_cast<LightObj*>(
                GetLightObj__Q34nw4r3g3d8G3DStateFi(lightRef));

            math::VEC3 dir;
            pLight->GetLightDir(&dir);

            // Lazily refresh the cached inverse of the current view matrix.
            if (!(lbl_eu_8061DFA0.mFlag & 1)) {
                PSMTXInverse(
                    lbl_eu_8061DFA0.mViewMtxArray[lbl_eu_8061DFA0.mCurCameraID],
                    lbl_eu_8061DFA0.mViewMtx);
                lbl_eu_8061DFA0.mFlag |= 1;
            }

                // Diffuse lights drive the matrix from their position;
                // spot/specular lights from their direction (falling back to
                // the position when the direction is degenerate).
                bool bUsePos = !pLight->IsSpotLight() &&
                               !pLight->IsSpecularLight();

                if (bUsePos || (dir.x == lbl_eu_80669BEC &&
                                dir.z == lbl_eu_80669BEC &&
                                dir.y == lbl_eu_80669BEC)) {
                    pLight->GetLightPos(&dir);
                    VEC3TransformNormal(&dir, &lbl_eu_8061DFA0.mViewMtx,
                                        &dir);
                    dir.x = -dir.x;
                    dir.y = -dir.y;
                    dir.z = -dir.z;

                    if (dir.x != lbl_eu_80669BEC ||
                        dir.z != lbl_eu_80669BEC ||
                        dir.y != lbl_eu_80669BEC) {
                        // Retail quirk: the already-transformed direction is
                        // transformed a second time on this path.
                        VEC3TransformNormal(&dir, &lbl_eu_8061DFA0.mViewMtx,
                                            &dir);
                    } else {
                        dir.y = lbl_eu_80669BF0;
                    }
                } else {
                    VEC3TransformNormal(&dir, &lbl_eu_8061DFA0.mViewMtx,
                                        &dir);
                }

            // Orthonormal basis: pick an up vector not parallel to the
            // direction and cross out sideways/vertical axes.
            math::VEC3 up;
            if (__fabs(dir.x) >= lbl_eu_80669BF4 ||
                __fabs(dir.z) >= lbl_eu_80669BF4) {
                up.z = lbl_eu_80669BEC;
                up.x = lbl_eu_80669BEC;
                up.y = lbl_eu_80669BF8;
            } else {
                up.x = lbl_eu_80669BEC;
                up.y = lbl_eu_80669BEC;
                up.z =
                    (dir.y <= lbl_eu_80669BEC) ? lbl_eu_80669BF0 : lbl_eu_80669BF8;
            }

            math::VEC3 side;
            PSVECCrossProduct(dir, up, side);
            PSVECNormalize(side, side);
            math::VEC3 axis;
            PSVECCrossProduct(side, dir, axis);

            math::MTX34& rMtx = *pMtx;
            rMtx.m[0][0] = side.x;
            rMtx.m[0][1] = side.y;
            rMtx.m[0][2] = side.z;
            rMtx.m[1][0] = axis.x;
            rMtx.m[1][1] = axis.y;
            rMtx.m[1][2] = axis.z;
            rMtx.m[2][0] = -dir.x;
            rMtx.m[2][1] = -dir.y;
            rMtx.m[2][2] = -dir.z;

            PSMTXConcat(*pMtx, lbl_eu_8061DFA0.mViewMtx, *pMtx);

            f32 zero = lbl_eu_80669BEC;
            rMtx.m[0][3] = rMtx.m[1][3] = rMtx.m[2][3] = zero;

            PSMTXConcat(
                *reinterpret_cast<const math::MTX34*>(lbl_eu_8051D660),
                *pMtx, *pMtx);
            return;
        }
    }

    PSMTXCopy(
        lbl_eu_8061DFA0.mEnvMtxArray[lbl_eu_8061DFA0.mCurCameraID], *pMtx);
}

} // namespace ScnDependentMtxFunc
} // namespace detail
} // namespace g3d
} // namespace nw4r

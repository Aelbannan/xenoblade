// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmtexsrt
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_anmtexsrt.h>

namespace nw4r {
namespace g3d {

bool AnmObjTexSrt::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_INVALID | BINDING_UNDEFINED)) == 0;
}

f32 AnmObjTexSrtRes::GetFrame() const {
    return GetFrm();
}

f32 AnmObjTexSrtRes::GetUpdateRate() const {
    return GetRate();
}

NW4R_G3D_RTTI_DEF(AnmObjTexSrt);
NW4R_G3D_RTTI_DEF(AnmObjTexSrtRes);

// AnmObj::IsDerivedFrom body (retail inlines the base chain into every
// derived class; the 0x650/0x640 checks come from this helper's boundary).
static bool anmObjBaseIsDerivedFrom(G3dObj::TypeObj other) {
    if (other == AnmObj::GetTypeObjStatic()) {
        return true;
    }
    if (other == G3dObj::GetTypeObjStatic()) {
        return true;
    }
    return false;
}

bool AnmObjTexSrt::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(TYPE_NAME)) {
        return true;
    }
    return anmObjBaseIsDerivedFrom(other);
}

bool AnmObjTexSrtRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(TYPE_NAME)) {
        return true;
    }
    return AnmObjTexSrt::IsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjTexSrtRes::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}

const char* AnmObjTexSrtRes::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

const char* AnmObjTexSrtOverride::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmObjTexSrt::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

AnmObjTexSrtRes* AnmObjTexSrt::Attach(int idx, AnmObjTexSrtRes* pRes) {
    return NULL;
}

AnmObjTexSrtRes* AnmObjTexSrt::Detach(int idx) {
    return NULL;
}

void AnmObjTexSrt::DetachAll() {}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

// u16 -> f32 through a signed int, matching retail's biased-magic
// signed-int->double sequence (0x4330000080000000).
static inline f32 AnmTexSrtConvU16ToF(u16 v) {
    return static_cast<f32>(static_cast<int>(v));
}

inline AnmObjTexSrt::AnmObjTexSrt(MEMAllocator* pAllocator, u16* pBindingBuf,
                                  int numBinding)
    : AnmObj(pAllocator, NULL), mNumBinding(numBinding),
      mpBinding(pBindingBuf) {
    Release();
}

inline AnmObjTexSrtRes::AnmObjTexSrtRes(MEMAllocator* pAllocator,
                                        ResAnmTexSrt srt, u16* pBindingBuf,
                                        int numBinding,
                                        TexSrtAnmResult* pCacheBuf)
    : AnmObjTexSrt(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f, AnmTexSrtConvU16ToF(srt.ref().info.numFrame),
                GetAnmPlayPolicy(srt.GetAnmPolicy())),
      mRes(srt), mpResultCache(pCacheBuf) {}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

AnmObjTexSrtRes* AnmObjTexSrtRes::Construct(MEMAllocator* pAllocator,
                                            u32* pSize, ResAnmTexSrt srt,
                                            ResMdl mdl, bool cache) {
    if (!srt.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    u16 numMat = srt.ref().info.numMaterial;
    int numBinding = mdl.GetResMatNumEntries();

    // Pointer locals declared first (MWCC birth-order register coloring).
    void* pBuffer;
    char* pBase;
    TexSrtAnmResult* pCacheBuf;
    u16* pBindingBuf;
    AnmObjTexSrtRes* pAnm;

    // Branchless "cache ? numMat : 0" select, matching retail's neg/or/srawi/and
    // nonzero-test idiom.
    u32 numCache = numMat & ((-cache | cache) >> 31);

    u32 resultOfs = numCache * sizeof(TexSrtAnmResult);
    u32 size =
        resultOfs + (u32)numBinding * sizeof(u16) + sizeof(AnmObjTexSrtRes);

    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    pBuffer = MEMAllocFromAllocator(pAllocator, size);

    if (pBuffer == NULL) {
        return NULL;
    }

    pBase = static_cast<char*>(pBuffer);

    pCacheBuf =
        cache ? reinterpret_cast<TexSrtAnmResult*>(pBase +
                                                   sizeof(AnmObjTexSrtRes))
              : NULL;

    pBindingBuf = reinterpret_cast<u16*>(pBase + resultOfs +
                                         sizeof(AnmObjTexSrtRes));

    pAnm = new (pBase) AnmObjTexSrtRes(pAllocator, srt, pBindingBuf, numBinding,
                                       pCacheBuf);

    // Fill the freshly allocated cache: every defined binding gets its
    // TexSrtAnmResult evaluated once at the start frame.
    if (pCacheBuf != NULL) {
        f32 f = pAnm->GetFrm();
        for (u32 i = 0, ofs = 0; i < (u32)pAnm->mNumBinding;
             ofs += (u32)sizeof(u16), i++) {
            u16 binding =
                *(const u16*)(reinterpret_cast<const u8*>(pAnm->mpBinding) + ofs);
            if (!(binding & BINDING_UNDEFINED)) {
                u32 id = binding & BINDING_ID_MASK;
                pAnm->mRes.GetAnmResult(&pAnm->mpResultCache[id], id, f);
            }
        }
    }

    return pAnm;
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmObjTexSrtRes::UpdateCache() {
    if (mpResultCache != NULL) {
        f32 f = GetFrm();
        for (u32 i = 0; i < (u32)mNumBinding; i++) {
            u16 binding = mpBinding[i];
            if (!(binding & BINDING_UNDEFINED)) {
                u32 id = binding & BINDING_ID_MASK;
                mRes.GetAnmResult(&mpResultCache[id], id, f);
            }
        }
    }
}

void AnmObjTexSrtRes::SetFrame(f32 frame) {
    SetFrm(frame);
    UpdateCache();
}

void AnmObjTexSrtRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    if (0.0f == rate) {
        UpdateCache();
    }
}

void AnmObjTexSrtRes::UpdateFrame() {
    if (0.0f != GetRate()) {
        UpdateFrm();
        UpdateCache();
    }
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

bool AnmObjTexSrtRes::Bind(ResMdl mdl) {
    int numMat = mRes.ref().info.numMaterial;
    bool bound = false;

    for (u16 i = 0; i < numMat; i++) {
        // Inlined GetMatAnm(): dictionary lookup may fail at every offset
        // level, hence the nested null checks in retail.
        const ResAnmTexSrtMatData* pData = mRes.GetMatAnm(i);
        const char* pName = reinterpret_cast<const char*>(
            static_cast<ptrdiff_t>(pData->name) +
            reinterpret_cast<ptrdiff_t>(pData));

        ResMat mat = mdl.GetResMat(ResName(
            reinterpret_cast<const ResNameData*>(
                reinterpret_cast<const char*>(pName) - 4)));

        if (mat.IsValid()) {
            mpBinding[mat.ref().id] = i;
            bound = true;
        }
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return bound;
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

const TexSrtAnmResult* AnmObjTexSrtRes::GetResult(TexSrtAnmResult* pResult,
                                                  u32 idx) {
    u16 binding = mpBinding[idx];

    if (binding & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->flags = 0;
        return pResult;
    }

    if (mpResultCache != NULL) {
        return &mpResultCache[binding];
    }

    mRes.GetAnmResult(pResult, binding, GetFrm());
    return pResult;
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

void AnmObjTexSrtRes::G3dProc(u32 task, u32 arg, void* pArg) {
    switch (task) {
    case G3DPROC_UPDATEFRAME:
        UpdateFrame();
        break;
    case G3DPROC_DETACH_PARENT:
        SetParent(NULL);
        break;
    case G3DPROC_ATTACH_PARENT:
        SetParent(static_cast<G3dObj*>(pArg));
        break;
    default:
        break;
    }
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

void ApplyTexSrtAnmResult(ResTexSrt texSrt, const TexSrtAnmResult* pResult) {
    ResTexSrtData& data = texSrt.ref();
    u32 texSrtFlags = data.flag;
    u32 resultFlags = pResult->flags;
    u32 mask = 0xF;

    for (u32 i = 0; texSrtFlags != 0 && resultFlags != 0; i++) {
        if ((texSrtFlags & 1) != 0 && (resultFlags & 1) != 0) {
            data.texMtxMode = pResult->texMtxMode;

            data.texSrt[i] = pResult->srt[i];

            data.flag =
                (data.flag & ~mask) | (pResult->flags & mask);
        }

        texSrtFlags >>= 4;
        resultFlags >>= 4;
        mask <<= 4;
    }
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

void ApplyTexSrtAnmResult(ResTexSrt texSrt, ResMatIndMtxAndScale indMtxAndScale,
                          const TexSrtAnmResult* pResult) {
    ApplyTexSrtAnmResult(texSrt, pResult);

    u32 flags = pResult->indFlags;
    const TexSrt* pSrt = &pResult->srt[TexSrtAnmResult::NUM_OF_MAT_TEX_MTX];
    u32 i = 8;

    while (flags != 0) {
        if (flags & 1) {
            math::MTX34 mtx;

            CalcTexMtx(&mtx, true, *pSrt,
                       static_cast<TexSrt::Flag>(flags & 0xF));

            f32 scale = 1.0f;
            f32 v = math::FAbs(mtx._00);
            if (v < scale) scale = v;
            v = math::FAbs(mtx._01);
            if (v < scale) scale = v;
            v = math::FAbs(mtx._10);
            if (v < scale) scale = v;
            v = math::FAbs(mtx._11);
            if (v < scale) scale = v;
            v = math::FAbs(mtx._20);
            if (v < scale) scale = v;
            v = math::FAbs(mtx._22);
            if (v < scale) scale = v;

            s8 scaleExp = static_cast<s8>(
                ((reinterpret_cast<u32&>(scale) >> 23) & 0xFF) - 0x7E);

            f32 factor = static_cast<f32>(ldexp(1.0f, -scaleExp));

            mtx._00 *= factor;
            mtx._01 *= factor;
            mtx._02 *= factor;
            mtx._10 *= factor;
            mtx._11 *= factor;
            mtx._12 *= factor;
            mtx._20 *= factor;
            mtx._21 *= factor;
            mtx._22 *= factor;

            indMtxAndScale.GXSetIndTexMtx(
                static_cast<GXIndTexMtxID>(i - 7), mtx, scaleExp);
        }

        flags >>= 4;
        pSrt++;
        i++;
    }
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

const G3dObj::TypeObj AnmObjTexSrtOverride::GetTypeObj() const {
    // The overrider is never instantiated by the game, so retail carries no
    // type-name block for it; it reports the base class's type object.
    return AnmObjTexSrt::GetTypeObjStatic();
}

} // namespace g3d
} // namespace nw4r

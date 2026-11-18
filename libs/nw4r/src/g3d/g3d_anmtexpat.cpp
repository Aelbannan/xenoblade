// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmtexpat
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_anmtexpat.h>

// TexPat play-policy table (shared with the other anm units, small-data).
extern const nw4r::g3d::PlayPolicyFunc lbl_eu_80663458[2];

namespace nw4r {
namespace g3d {

bool AnmObjTexPat::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_INVALID | BINDING_UNDEFINED)) == 0;
}

f32 AnmObjTexPatRes::GetFrame() const {
    return GetFrm();
}

NW4R_G3D_RTTI_DEF(AnmObjTexPat);
NW4R_G3D_RTTI_DEF(AnmObjTexPatRes);

// AnmObj::IsDerivedFrom body (retail inlines the base chain into every
// derived class; the 0x650/0x640 checks come from this helper's boundary).
static bool anmObjBaseIsDerivedFrom(G3dObj::TypeObj other) {
    if (other == G3dObj::TypeObj(lbl_eu_8051D650)) {
        return true;
    }
    if (other == G3dObj::TypeObj(lbl_eu_8051D640)) {
        return true;
    }
    return false;
}

bool AnmObjTexPat::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(TYPE_NAME)) {
        return true;
    }
    return anmObjBaseIsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjTexPat::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}

const char* AnmObjTexPat::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

bool AnmObjTexPatRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(TYPE_NAME)) {
        return true;
    }
    return AnmObjTexPat::IsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjTexPatRes::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}

const char* AnmObjTexPatRes::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmObjTexPat::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

AnmObjTexPatRes* AnmObjTexPat::Attach(int idx, AnmObjTexPatRes* pRes) {
    return NULL;
}

AnmObjTexPatRes* AnmObjTexPat::Detach(int idx) {
    return NULL;
}

void AnmObjTexPat::DetachAll() {}

} // namespace g3d
} // namespace nw4r



// The resource's u16 frame count is promoted through `int` before the float
// conversion: MWCC then emits retail's signed-int magic (value ^ 0x8000 in the
// low word, `lfd` 0x4330000080000000, `fsubs`) and pools that double.
static inline f32 TexPatConvU16ToF(u16 v) {
    return static_cast<f32>(static_cast<int>(v));
}

namespace nw4r {
namespace g3d {

// Constructor chain, defined inline so MWCC expands it fully inside Construct
// (retail has no out-of-line ctors for these classes): base fields, then a
// *virtual* Release() during AnmObjTexPat construction (initializes every
// binding to UNDEFINED), then the FrameCtrl bases and resource members.
inline AnmObjTexPat::AnmObjTexPat(MEMAllocator* pAllocator, u16* pBindingBuf,
                                  int numBinding)
    : AnmObj(pAllocator, NULL), mNumBinding(numBinding),
      mpBinding(pBindingBuf) {
    Release();
}

inline AnmObjTexPatRes::AnmObjTexPatRes(MEMAllocator* pAllocator,
                                        ResAnmTexPat pat, u16* pBindingBuf,
                                        int numBinding,
                                        TexPatAnmResult* pCacheBuf)
    : AnmObjTexPat(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f,
                TexPatConvU16ToF(pat.ref().info.numFrame),
                lbl_eu_80663458[pat.ref().info.policy]),
      mRes(pat), mpResultCache(pCacheBuf) {}

// Layout of the allocation: [object][result cache][binding table]. The cache
// region only exists when `cache` is set, sized by the texpat material count.
AnmObjTexPatRes* AnmObjTexPatRes::Construct(MEMAllocator* pAllocator,
                                            u32* pSize, ResAnmTexPat pat,
                                            ResMdl mdl, bool cache) {
    if (!pat.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    // Pointer locals declared first (MWCC birth-order register coloring).
    void* pBuffer;
    char* pBase;
    TexPatAnmResult* pCacheBuf;
    u16* pBindingBuf;
    AnmObjTexPatRes* pAnm;

    u16 numMat = pat.ref().info.numMaterial;

    int numBinding = mdl.GetResMatNumEntries();

    // Branchless "cache ? numMat : 0" select, matching retail's neg/or/srawi/and
    // nonzero-test idiom.
    u32 numCache = numMat & ((-cache | cache) >> 31);

    u32 resultOfs = numCache * sizeof(TexPatAnmResult);
    u32 size = resultOfs + (u32)numBinding * sizeof(u16) + sizeof(AnmObjTexPatRes);

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
        cache ? reinterpret_cast<TexPatAnmResult*>(pBase + sizeof(AnmObjTexPatRes))
              : NULL;

    pBindingBuf = reinterpret_cast<u16*>(pBase + resultOfs +
                                         sizeof(AnmObjTexPatRes));

    pAnm = new (pBase)
        AnmObjTexPatRes(pAllocator, pat, pBindingBuf, numBinding, pCacheBuf);

    // Fill the freshly allocated cache: every defined binding gets its
    // TexPatAnmResult evaluated once at the start frame.
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

void AnmObjTexPatRes::UpdateCache() {
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

void AnmObjTexPatRes::SetFrame(f32 frame) {
    SetFrm(frame);
    UpdateCache();
}

void AnmObjTexPatRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    if (0.0f == rate) {
        UpdateCache();
    }
}

void AnmObjTexPatRes::UpdateFrame() {
    if (0.0f != GetRate()) {
        UpdateFrm();
        UpdateCache();
    }
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

bool AnmObjTexPatRes::Bind(ResMdl mdl) {
    int numMat = mRes.ref().info.numMaterial;
    bool bound = false;

    for (u16 i = 0; i < numMat; i++) {
        // Inlined GetMatAnm(): dictionary lookup may fail at every offset
        // level, hence the nested null checks in retail.
        const char* pName;
        const ResAnmTexPatMatData* pData = mRes.GetMatAnm(i);
        const char* pName2 = reinterpret_cast<const char*>(
            static_cast<ptrdiff_t>(pData->name) +
            reinterpret_cast<ptrdiff_t>(pData));

        ResName name(reinterpret_cast<const ResNameData*>(pName2 - sizeof(u32)));
        ResMat mat = mdl.GetResMat(name);

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

const TexPatAnmResult* AnmObjTexPatRes::GetResult(TexPatAnmResult* pResult,
                                                  u32 idx) {
    u16 binding = mpBinding[idx];

    if (binding & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->bTexExist = 0;
        pResult->bPlttExist = 0;
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

void AnmObjTexPatRes::G3dProc(u32 task, u32 arg, void* pArg) {
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

// Replays each animated texture/palette result back into the shared ResTexObj
// / ResTlutObj banks: bit i of the existence masks gates slot i.
void ApplyTexPatAnmResult(ResTexObj texObj, ResTlutObj tlutObj,
                          const TexPatAnmResult* pResult) {
    const TexPatAnmResult* p = pResult;
    int i;
    // Walks the tex[] array: starts at the result header, so entry i is
    // pTex[1] on iteration i.
    const ResTex* pTex;
    u32 plttMask;
    u32 texMask = p->bTexExist;
    for (i = 0, pTex = reinterpret_cast<const ResTex*>(p); texMask != 0;
         texMask >>= 1) {
        if (texMask & 1) {
            ResTex tex = pTex[1];

            GXTexObj* pObj = texObj.GetTexObj(static_cast<GXTexMapID>(i));

            GXTexFilter filterMin;
            GXTexFilter filterMag;
            f32 minLod;
            f32 maxLod;
            f32 lodBias;
            GXBool biasClamp;
            GXBool edgeLod;
            GXBool mipMap;
            GXAnisotropy aniso;

            GXGetTexObjLODAll(pObj, &filterMin, &filterMag, &minLod, &maxLod,
                              &lodBias, &biasClamp, &edgeLod, &aniso);

            GXTexWrapMode wrapS = GXGetTexObjWrapS(pObj);
            GXTexWrapMode wrapT = GXGetTexObjWrapT(pObj);

            void* pImg;
            u16 width;
            u16 height;
            GXTexFmt fmt;

            if (tex.IsCIFmt()) {
                GXCITexFmt ciFmt;

                tex.GetTexObjCIParam(&pImg, &width, &height, &ciFmt, &minLod,
                                     &maxLod, &mipMap);

                GXInitTexObjCI(pObj, pImg, width, height,
                               static_cast<GXTexFmt>(ciFmt), wrapS, wrapT,
                               mipMap, static_cast<u32>(i));
            } else {
                tex.GetTexObjParam(&pImg, &width, &height, &fmt, &minLod,
                                   &maxLod, &mipMap);

                GXInitTexObj(pObj, pImg, width, height, fmt, wrapS, wrapT,
                             mipMap);
            }

            GXInitTexObjLOD(pObj, filterMin, filterMag, minLod, maxLod, lodBias,
                            biasClamp, edgeLod, aniso);
        }

        pTex++;
        i++;
    }

    int j = 0;
    for (plttMask = p->bPlttExist; plttMask != 0; plttMask >>= 1) {
        if (plttMask & 1) {
            ResPltt pltt = p->pltt[j];

            u16* pPlttData = pltt.GetPlttData();
            GXTlutFmt fmt = pltt.ref().fmt;
            u16 entries = pltt.ref().numEntries;

            GXTlutObj* pTlutObj = tlutObj.GetTlut(static_cast<GXTlut>(j));

            GXInitTlutObj(pTlutObj, pPlttData, fmt, entries);
        }

        j++;
    }
}

} // namespace g3d
} // namespace nw4r


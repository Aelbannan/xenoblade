// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmtexpat
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_anmtexpat.h>

// Float constants in the retail .sdata2 pool (named so the sda21 relocs
// reference the retail symbols instead of TU-local pool labels).
extern const f32 lbl_eu_80669B58; // 0.0f (rate==0 triggers the cache refresh)
extern const f32 lbl_eu_80669B5C; // 1.0f (FrameCtrl::mUpdateRate init)
extern const f64 lbl_eu_80669B60; // 0x4330000000000000 (u16->double bias)
extern const f32 lbl_eu_80669B5C; // 1.0f (FrameCtrl::mUpdateRate init)
extern const f64 lbl_eu_80669B60; // 0x4330000080000000 (signed int->double bias)
extern const f32 lbl_eu_80663460; // 1.0f (FrameCtrl::smBaseUpdateRate)

extern char lbl_eu_80569258[]; // AnmObjTexPat vtable (construction stage)
extern char lbl_eu_80569210[]; // AnmObjTexPatRes final vtable
// TexPat play-policy table (shared with the other anm units, small-data).
extern const nw4r::g3d::PlayPolicyFunc lbl_eu_80663458[2];

// Retail ships FrameCtrl's static smBaseUpdateRate as the named .sdata object
// lbl_eu_80663460; redirect the inline UpdateFrm() body to that name.
namespace nw4r {
namespace g3d {

bool AnmObjTexPat::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_INVALID | BINDING_UNDEFINED)) == 0;
}

f32 AnmObjTexPatRes::GetFrame() const {
    return GetFrm();
}

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjTexPat")> lbl_eu_8051D560 = {sizeof("AnmObjTexPat"), "AnmObjTexPat"};

bool AnmObjTexPat::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D560) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjTexPat::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D560);
}

const char* AnmObjTexPat::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjTexPatRes")> lbl_eu_8051D578 = {sizeof("AnmObjTexPatRes"), "AnmObjTexPatRes"};

bool AnmObjTexPatRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D578) ? true
         : other == TypeObj(lbl_eu_8051D560) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjTexPatRes::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D578);
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

} // namespace g3d
} // namespace nw4r



int Attach__Q34nw4r3g3d12AnmObjTexPatFiPQ34nw4r3g3d15AnmObjTexPatRes(void) { return 0x0; }

int Detach__Q34nw4r3g3d12AnmObjTexPatFi(void) { return 0x0; }

void DetachAll__Q34nw4r3g3d12AnmObjTexPatFv(void) {}




// u16 -> f32 widening matching retail's biased-magic signed-int->double
// sequence: flip the sign bit, scaffold 0x43300000xxxxxxxx on the stack (the
// value word is stored FIRST), and subtract the shared sdata2 bias
// lbl_eu_80669B60 (= 0x4330000080000000).
static inline f32 TexPatConvU16ToF(u16 v) {
    union {
        u32 w[2];
        f64 d;
    } t;
    t.w[1] = (u32)v ^ 0x80000000u;
    t.w[0] = 0x43300000u;
    return static_cast<f32>(t.d - lbl_eu_80669B60);
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
    *(void**)this = (void*)lbl_eu_80569258;
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
      mRes(pat), mpResultCache(pCacheBuf) {
    *(void**)this = (void*)lbl_eu_80569210;
}

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

    if (lbl_eu_80669B58 == rate) {
        UpdateCache();
    }
}

void AnmObjTexPatRes::UpdateFrame() {
    if (lbl_eu_80669B58 != GetRate()) {
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

        i++;
        pTex++;
    }

    plttMask = p->bPlttExist;
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

void IsDerivedFrom__Q34nw4r3g3d15AnmObjTexPatResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d12AnmObjTexPatCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d15AnmObjTexPatResCFv(){}

extern const char lbl_eu_8051D578[];
void* GetTypeObj__Q34nw4r3g3d15AnmObjTexPatResCFv(void) { return (void*)lbl_eu_8051D578; }

void __dt__Q34nw4r3g3d15AnmObjTexPatResFv(){}

void GetTypeName__Q34nw4r3g3d12AnmObjTexPatCFv(){}

extern "C" void* GetTypeObj__Q34nw4r3g3d12AnmObjTexPatCFv(void) { return (void*)lbl_eu_8051D560; }

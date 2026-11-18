#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_anmclr.h>
#include <nw4r/ut.h>

// Shared animation play-policy table (owned by g3d_anmobj's small-data slice).
extern const nw4r::g3d::PlayPolicyFunc lbl_eu_80663458[2];

namespace nw4r {
namespace g3d {

// The resource's u16 frame count is promoted through `int` before the float
// conversion: MWCC then emits retail's signed-int magic (value ^ 0x8000 in the
// low word, `lfd` 0x4330000080000000, `fsubs`) and pools that double. A direct
// u16 cast emits the unsigned magic (0x4330000000000000) instead.
static inline f32 AnmClrConvU16ToF(u16 v) {
    return static_cast<f32>(static_cast<int>(v));
}

NW4R_G3D_RTTI_DEF(AnmObjMatClr);
NW4R_G3D_RTTI_DEF(AnmObjMatClrRes);

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

bool AnmObjMatClr::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_INVALID | BINDING_UNDEFINED)) == 0;
}

bool AnmObjMatClr::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(TYPE_NAME)) {
        return true;
    }
    return anmObjBaseIsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjMatClr::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}

const char* AnmObjMatClr::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

bool AnmObjMatClrRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(TYPE_NAME)) {
        return true;
    }
    return AnmObjMatClr::IsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjMatClrRes::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}

const char* AnmObjMatClrRes::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

AnmObjMatClrRes* AnmObjMatClr::Attach(int idx, AnmObjMatClrRes* pRes) {
    return NULL;
}

AnmObjMatClrRes* AnmObjMatClr::Detach(int idx) {
    return NULL;
}

void AnmObjMatClr::DetachAll() {}

void AnmObjMatClr::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

f32 AnmObjMatClrRes::GetFrame() const {
    return GetFrm();
}

f32 AnmObjMatClrRes::GetUpdateRate() const {
    return GetRate();
}

// Constructor chain, defined inline so MWCC expands it fully inside Construct
// (retail has no out-of-line ctors for these classes): base fields, then a
// *virtual* Release() during AnmObjMatClr construction (initializes every
// binding to UNDEFINED), then the FrameCtrl bases and resource members.
inline AnmObjMatClr::AnmObjMatClr(MEMAllocator* pAllocator, u16* pBindingBuf,
                                  int numBinding)
    : AnmObj(pAllocator, NULL), mNumBinding(numBinding),
      mpBinding(pBindingBuf) {
    Release();
}

inline AnmObjMatClrRes::AnmObjMatClrRes(MEMAllocator* pAllocator,
                                        ResAnmClr clr, u16* pBindingBuf,
                                        int numBinding,
                                        ClrAnmResult* pCacheBuf)
    : AnmObjMatClr(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f, AnmClrConvU16ToF(clr.ref().info.numFrame),
                lbl_eu_80663458[clr.ref().info.policy]),
      mRes(clr), mpResultCache(pCacheBuf) {}

// Layout of the allocation: [object][result cache][binding table]. The cache
// region only exists when `cache` is set, sized by the material count.
AnmObjMatClrRes* AnmObjMatClrRes::Construct(MEMAllocator* pAllocator,
                                            u32* pSize, ResAnmClr clr,
                                            ResMdl mdl, bool cache) {
    if (!clr.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    u16 numMat = clr.ref().info.numMaterial;
    int numBinding = mdl.GetResMatNumEntries();

    // Pointer locals declared first (MWCC birth-order register coloring).
    void* pBuffer;
    char* pBase;
    ClrAnmResult* pCacheBuf;
    u16* pBindingBuf;
    AnmObjMatClrRes* pAnm;

    u32 numCache = cache ? numMat : 0;

    u32 resultOfs = numCache * sizeof(ClrAnmResult);

    u32 size =
        sizeof(AnmObjMatClrRes) + resultOfs + numBinding * sizeof(u16);

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
        cache ? reinterpret_cast<ClrAnmResult*>(pBase + sizeof(AnmObjMatClrRes))
              : NULL;

    pBindingBuf = reinterpret_cast<u16*>(pBase + resultOfs +
                                         sizeof(AnmObjMatClrRes));

    pAnm = new (pBase) AnmObjMatClrRes(pAllocator, clr, pBindingBuf, numBinding,
                                       pCacheBuf);

    // Fill the freshly allocated cache: every defined binding gets its
    // ClrAnmResult evaluated once at the start frame.
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

void AnmObjMatClrRes::UpdateCache() {
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

void AnmObjMatClrRes::SetFrame(f32 frame) {
    SetFrm(frame);
    UpdateCache();
}

void AnmObjMatClrRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    if (0.0f == rate) {
        UpdateCache();
    }
}

void AnmObjMatClrRes::UpdateFrame() {
    if (0.0f != GetRate()) {
        UpdateFrm();
        UpdateCache();
    }
}

bool AnmObjMatClrRes::Bind(ResMdl mdl) {
    int numMat = mRes.ref().info.numMaterial;
    bool bound = false;

    for (u16 i = 0; i < numMat; i++) {
        // Inlined GetMatAnm(): dictionary lookup may fail at every offset
        // level, hence the nested null checks in retail.
        const ResAnmClrMatData* pData = mRes.GetMatAnm(i);
        const char* pName = reinterpret_cast<const char*>(
            static_cast<ptrdiff_t>(pData->name) +
            reinterpret_cast<ptrdiff_t>(pData));

        ResName name(reinterpret_cast<const ResNameData*>(pName - sizeof(u32)));
        ResMat mat = mdl.GetResMat(name);

        if (mat.IsValid()) {
            mpBinding[mat.ref().id] = i;
            bound = true;
        }
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return bound;
}

const ClrAnmResult* AnmObjMatClrRes::GetResult(ClrAnmResult* pResult,
                                               u32 idx) {
    u16 binding = mpBinding[idx];

    if (binding & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->bRgbaExist = 0;
        return pResult;
    }

    if (mpResultCache != NULL) {
        return &mpResultCache[binding];
    }

    mRes.GetAnmResult(pResult, binding, GetFrm());
    return pResult;
}

void AnmObjMatClrRes::G3dProc(u32 task, u32 arg, void* pArg) {
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
namespace ut {

Color Color::operator&(u32 color) const {
    return Color(ToU32() & color);
}

Color Color::operator|(u32 color) const {
    return Color(ToU32() | color);
}

} // namespace ut
} // namespace nw4r

namespace nw4r {
namespace g3d {

void ApplyClrAnmResult(ResMatChan chan, ResMatTevColor tev,
                       const ClrAnmResult* pResult) {
    ut::Color color;

    if (pResult->bRgbaExist & 0x0000000F) {
        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_CLR0)) {
            Chan& chanData = chan.ref().chan[0];
            color = chanData.matColor;
            chanData.matColor = (color & pResult->rgbaMask[ClrAnmResult::CLA_CLR0]) |
                          pResult->rgba[ClrAnmResult::CLA_CLR0];
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_AMB0)) {
            Chan& chanData = chan.ref().chan[0];
            color = chanData.ambColor;
            chanData.ambColor = (color & pResult->rgbaMask[ClrAnmResult::CLA_AMB0]) |
                          pResult->rgba[ClrAnmResult::CLA_AMB0];
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_CLR1)) {
            Chan& chanData = chan.ref().chan[1];
            color = chanData.matColor;
            chanData.matColor = (color & pResult->rgbaMask[ClrAnmResult::CLA_CLR1]) |
                          pResult->rgba[ClrAnmResult::CLA_CLR1];
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_AMB1)) {
            Chan& chanData = chan.ref().chan[1];
            color = chanData.ambColor;
            chanData.ambColor = (color & pResult->rgbaMask[ClrAnmResult::CLA_AMB1]) |
                          pResult->rgba[ClrAnmResult::CLA_AMB1];
        }

    }

    if (pResult->bRgbaExist & 0x000007F0) {
        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEV0)) {
            if (!tev.GXGetTevColor(GX_TEVREG0, &color)) {
                color = ut::Color::BLACK;
            }

            tev.GXSetTevColor(GX_TEVREG0, (color & pResult->rgbaMask[ClrAnmResult::CLA_TEV0]) |
                                 pResult->rgba[ClrAnmResult::CLA_TEV0]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEV1)) {
            if (!tev.GXGetTevColor(GX_TEVREG1, &color)) {
                color = ut::Color::BLACK;
            }

            tev.GXSetTevColor(GX_TEVREG1, (color & pResult->rgbaMask[ClrAnmResult::CLA_TEV1]) |
                                 pResult->rgba[ClrAnmResult::CLA_TEV1]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEV2)) {
            if (!tev.GXGetTevColor(GX_TEVREG2, &color)) {
                color = ut::Color::BLACK;
            }

            tev.GXSetTevColor(GX_TEVREG2, (color & pResult->rgbaMask[ClrAnmResult::CLA_TEV2]) |
                                 pResult->rgba[ClrAnmResult::CLA_TEV2]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK0)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR0, &color)) {
                color = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR0, (color & pResult->rgbaMask[ClrAnmResult::CLA_TEVK0]) |
                                 pResult->rgba[ClrAnmResult::CLA_TEVK0]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK1)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR1, &color)) {
                color = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR1, (color & pResult->rgbaMask[ClrAnmResult::CLA_TEVK1]) |
                                 pResult->rgba[ClrAnmResult::CLA_TEVK1]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK2)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR2, &color)) {
                color = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR2, (color & pResult->rgbaMask[ClrAnmResult::CLA_TEVK2]) |
                                 pResult->rgba[ClrAnmResult::CLA_TEVK2]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK3)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR3, &color)) {
                color = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR3, (color & pResult->rgbaMask[ClrAnmResult::CLA_TEVK3]) |
                                 pResult->rgba[ClrAnmResult::CLA_TEVK3]);
        }

        tev.DCStore(false);
    }
}

} // namespace g3d
} // namespace nw4r

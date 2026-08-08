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

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjTexSrt")> lbl_eu_8051D590 = {sizeof("AnmObjTexSrt"), "AnmObjTexSrt"};

bool AnmObjTexSrt::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D590) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjTexSrtRes")> lbl_eu_8051D5A8 = {sizeof("AnmObjTexSrtRes"), "AnmObjTexSrtRes"};

bool AnmObjTexSrtRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D5A8) ? true
         : other == TypeObj(lbl_eu_8051D590) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjTexSrtRes::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5A8);
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

} // namespace g3d
} // namespace nw4r



int Attach__Q34nw4r3g3d12AnmObjTexSrtFiPQ34nw4r3g3d15AnmObjTexSrtRes(void) { return 0x0; }

int Detach__Q34nw4r3g3d12AnmObjTexSrtFi(void) { return 0x0; }

void DetachAll__Q34nw4r3g3d12AnmObjTexSrtFv(void) {}



void Construct__Q34nw4r3g3d15AnmObjTexSrtResFP12MEMAllocatorPUlQ34nw4r3g3d12ResAnmTexSrtQ34nw4r3g3d6ResMdlb(){}

namespace nw4r {
namespace g3d {

void AnmObjTexSrtRes::SetFrame(f32 frame) {
    SetFrm(frame);

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

void AnmObjTexSrtRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    if (rate == 1.0f) {
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
}

void AnmObjTexSrtRes::UpdateFrame() {
    if (GetRate() != 1.0f) {
        UpdateFrm();

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
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

bool AnmObjTexSrtRes::Bind(ResMdl mdl) {
    u16 numMat = mRes.ref().info.numMaterial;
    bool bound = false;

    for (u16 i = 0; i < numMat; i++) {
        ResDic dic = ResDic(mRes.ofs_to_ptr<ResDicData>(mRes.ref().toTexSrtDataDic));
        const ResAnmTexSrtMatData* pData =
            reinterpret_cast<const ResAnmTexSrtMatData*>(dic[i]);
        const char* pName =
            reinterpret_cast<const char*>(pData) + pData->name;

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
    u32 texSrtFlags = texSrt.ref().flag;
    u32 resultFlags = pResult->flags;
    u32 mask = 0xF;
    TexSrt* pDst = texSrt.ref().texSrt;
    const TexSrt* pSrc = pResult->srt;

    while (texSrtFlags != 0 && resultFlags != 0) {
        if ((texSrtFlags & 1) != 0 && (resultFlags & 1) != 0) {
            texSrt.ref().texMtxMode = pResult->texMtxMode;

            *pDst = *pSrc;

            texSrt.ref().flag =
                (texSrt.ref().flag & ~mask) | (resultFlags & mask);
        }

        texSrtFlags >>= 4;
        resultFlags >>= 4;
        mask <<= 4;
        pDst++;
        pSrc++;
    }
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

void ApplyTexSrtAnmResult(ResTexSrt texSrt, ResMatIndMtxAndScale indMtxAndScale,
                          const TexSrtAnmResult* pResult) {
    ApplyTexSrtAnmResult(texSrt, pResult);

    u32 flags = *reinterpret_cast<u32*>(
        reinterpret_cast<u8*>(indMtxAndScale.ptr()) + 4);
    const TexSrt* pSrt = texSrt.ref().texSrt;
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



void IsDerivedFrom__Q34nw4r3g3d15AnmObjTexSrtResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d12AnmObjTexSrtCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d15AnmObjTexSrtResCFv(){}

extern const char lbl_eu_8051D5A8[];
void* GetTypeObj__Q34nw4r3g3d15AnmObjTexSrtResCFv(void) { return (void*)lbl_eu_8051D5A8; }

void __dt__Q34nw4r3g3d15AnmObjTexSrtResFv(){}

extern "C" void* GetTypeObj__Q34nw4r3g3d20AnmObjTexSrtOverrideCFv(void) { return (void*)lbl_eu_8051D590; }

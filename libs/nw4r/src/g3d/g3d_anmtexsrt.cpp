// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmtexsrt
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObj")> lbl_eu_8051D650;
extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("G3dObj")> lbl_eu_8051D640;

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

void Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdl(){}

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



void G3dProc__Q34nw4r3g3d15AnmObjTexSrtResFUlUlPv(){}

void ApplyTexSrtAnmResult__Q24nw4r3g3dFQ34nw4r3g3d9ResTexSrtPCQ34nw4r3g3d15TexSrtAnmResult(){}

void ApplyTexSrtAnmResult__Q24nw4r3g3dFQ34nw4r3g3d9ResTexSrtQ34nw4r3g3d20ResMatIndMtxAndScalePCQ34nw4r3g3d15TexSrtAnmResult(){}

void IsDerivedFrom__Q34nw4r3g3d15AnmObjTexSrtResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d12AnmObjTexSrtCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d15AnmObjTexSrtResCFv(){}

extern const char lbl_eu_8051D5A8[];
void* GetTypeObj__Q34nw4r3g3d15AnmObjTexSrtResCFv(void) { return (void*)lbl_eu_8051D5A8; }

void __dt__Q34nw4r3g3d15AnmObjTexSrtResFv(){}

extern "C" const char lbl_eu_8051D590[];
extern "C" void* GetTypeObj__Q34nw4r3g3d20AnmObjTexSrtOverrideCFv(void) { return (void*)lbl_eu_8051D590; }

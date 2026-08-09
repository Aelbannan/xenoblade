// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnroot
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/g3d.h>
#include <harness_catalog.h>
using nw4r::g3d::ScnRoot;
using nw4r::g3d::ScnObjGather;

void ScnRoot::CalcMaterial() {
    G3dProc(G3DPROC_CALC_MAT, 0, NULL);
}

void ScnObjGather::Clear() {
    mNumScnObjXlu = 0;
    mNumScnObjOpa = 0;
}

void Construct__Q34nw4r3g3d7ScnRootFP12MEMAllocatorPUlUlUlUlUl(){}

void G3dProc__Q34nw4r3g3d7ScnRootFUlUlPv(){}

void GetCamera__Q34nw4r3g3d7ScnRootFi(){}

void GetCurrentCamera__Q34nw4r3g3d7ScnRootFv(){}

void SetCurrentCamera__Q34nw4r3g3d7ScnRootFi(ScnRoot* self, int idx) {
    self->mCurrentCameraID = static_cast<u8>(idx);
}

void GetFog__Q34nw4r3g3d7ScnRootFi(){}

void SetAnmScn__Q34nw4r3g3d7ScnRootFPQ34nw4r3g3d6AnmScn(){}

void RemoveAnmScn__Q34nw4r3g3d7ScnRootFv(){}

void UpdateFrame__Q34nw4r3g3d7ScnRootFv(){}

void SetGlbSettings__Q34nw4r3g3d7ScnRootFv(){}

void CalcAnmScn__Q34nw4r3g3d7ScnRootFv(){}

void CalcWorld__Q34nw4r3g3d7ScnRootFv(){}

void CalcView__Q34nw4r3g3d7ScnRootFv(){}

void GatherDrawScnObj__Q34nw4r3g3d7ScnRootFv(){}

void DrawOpa__Q34nw4r3g3d7ScnRootFv(){}

void DrawXlu__Q34nw4r3g3d7ScnRootFv(){}

void __ct__Q34nw4r3g3d7ScnRootFP12MEMAllocatorPQ34nw4r3g3d13IScnObjGatherPPQ34nw4r3g3d6ScnObjUlUlUlPQ34nw4r3g3d8LightObjPQ34nw4r3g3d11AmbLightObjPQ34nw4r3g3d12LightSetData(){}

void __dt__Q34nw4r3g3d7ScnRootFv(){}

void Add__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d6ScnObjbb(){}

void ZSort__Q34nw4r3g3d12ScnObjGatherFv(){}

void Sort__Q34nw4r3g3d12ScnObjGatherFv(){}

void Sort__Q34nw4r3g3d12ScnObjGatherFPFPCQ34nw4r3g3d6ScnObjPCQ34nw4r3g3d6ScnObj_bPFPCQ34nw4r3g3d6ScnObjPCQ34nw4r3g3d6ScnObj_b() {}

void DrawOpa__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d14ResMdlDrawMode(){}

void DrawXlu__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d14ResMdlDrawMode(){}

int CheckScnObj__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d6ScnObj(void) { return 0x0; }

void __dt__Q34nw4r3g3d12ScnObjGatherFv(){}

void IsDerivedFrom__Q34nw4r3g3d7ScnRootCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d7ScnRootCFv(){}

extern "C" void* GetTypeObj__Q34nw4r3g3d7ScnRootCFv(void) { return (void*)lbl_eu_8051D798; }

namespace nw4r { namespace g3d { class ScnObj; }}
namespace nw4r {
    namespace g3d {
        namespace {
            void LessZSortOpa(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) {}
        }
    }
}
namespace nw4r {
    namespace g3d {
        namespace {
            void LessZSortXlu(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) {}
        }
    }
}
namespace nw4r {
    namespace g3d {
        namespace {
            void LessByGetValueForSortOpa(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) {}
        }
    }
}
namespace nw4r {
    namespace g3d {
        namespace {
            void LessByGetValueForSortXlu(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) {}
        }
    }
}


namespace nw4r {
namespace g3d {

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("ScnRoot")> lbl_eu_8051D798 = {sizeof("ScnRoot"), "ScnRoot"};

bool ScnRoot::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D798) ? true
         : other == TypeObj(lbl_eu_8051D788) ? true
         : other == TypeObj(lbl_eu_8051D768) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj ScnRoot::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D798);
}

const char* ScnRoot::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r

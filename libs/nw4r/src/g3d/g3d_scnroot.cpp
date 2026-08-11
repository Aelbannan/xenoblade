// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnroot
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/g3d.h>
#include <harness_catalog.h>
using nw4r::g3d::ScnRoot;
using nw4r::g3d::ScnObjGather;
using nw4r::g3d::Camera;
using nw4r::g3d::Fog;

// MWCC's <algorithm> doesn't declare std::sort (same pattern as g3d_draw.cpp)
namespace std {
template <typename T, typename Compare>
void sort(T first, T last, Compare comp);
}

namespace nw4r {
namespace g3d {
namespace {

bool LessZSortOpa(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) { return false; }
bool LessZSortXlu(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) { return false; }
bool LessByGetValueForSortOpa(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) { return false; }
bool LessByGetValueForSortXlu(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*) { return false; }

} // namespace
} // namespace g3d
} // namespace nw4r

void ScnRoot::CalcMaterial() {
    G3dProc(G3DPROC_CALC_MAT, 0, NULL);
}

void ScnObjGather::Clear() {
    mNumScnObjXlu = 0;
    mNumScnObjOpa = 0;
}

void Construct__Q34nw4r3g3d7ScnRootFP12MEMAllocatorPUlUlUlUlUl(){}

void G3dProc__Q34nw4r3g3d7ScnRootFUlUlPv(){}

Camera ScnRoot::GetCamera(int idx) {
    if (0 <= idx && idx < G3DState::NUM_CAMERA) {
        return Camera(&mCamera[idx]);
    }

    return Camera(NULL);
}

void GetCurrentCamera__Q34nw4r3g3d7ScnRootFv(){}

void SetCurrentCamera__Q34nw4r3g3d7ScnRootFi(ScnRoot* self, int idx) {
    self->mCurrentCameraID = static_cast<u8>(idx);
}

Fog ScnRoot::GetFog(int idx) {
    if (0 <= idx && idx < G3DState::NUM_FOG) {
        return Fog(&mFog[idx]);
    }

    return Fog(NULL);
}

void SetAnmScn__Q34nw4r3g3d7ScnRootFPQ34nw4r3g3d6AnmScn(){}

// NOTE: ScnRoot::RemoveAnmScn is not declared in g3d_scnroot.h; the retail
// symbol returns the detached AnmScn* (see `mr r3, r31` epilogue).
nw4r::g3d::AnmScn* RemoveAnmScn__Q34nw4r3g3d7ScnRootFv(ScnRoot* self) {
    nw4r::g3d::AnmScn* pAnmScn = self->mpAnmScn;

    if (pAnmScn != NULL) {
        pAnmScn->G3dProc(nw4r::g3d::G3dObj::G3DPROC_DETACH_PARENT, 0, self);
        self->mpAnmScn = NULL;
    }

    return pAnmScn;
}

void ScnRoot::UpdateFrame() {
    if (mpAnmScn != NULL) {
        mpAnmScn->UpdateFrame();
    }

    G3dProc(G3DPROC_UPDATEFRAME, 0, NULL);
}

void SetGlbSettings__Q34nw4r3g3d7ScnRootFv(){}

void CalcAnmScn__Q34nw4r3g3d7ScnRootFv(){}

void CalcWorld__Q34nw4r3g3d7ScnRootFv(){}

void ScnRoot::CalcView() {
    GXInvalidateVtxCache();

    Camera camera(&mCamera[mCurrentCameraID]);

    nw4r::math::MTX34 mtx;
    Camera(camera).GetCameraMtx(&mtx);

    G3dProc(G3DPROC_CALC_VIEW, 0, &mtx);
}

void GatherDrawScnObj__Q34nw4r3g3d7ScnRootFv(){}

void ScnRoot::DrawOpa() {
    SetGlbSettings();

    if (TestScnRootFlag(SCNROOTFLAG_FORCE_RESMDLDRAWMODE)) {
        mpCollection->DrawOpa(&mDrawMode);
    } else {
        mpCollection->DrawOpa(NULL);
    }

    G3DState::Invalidate(G3DState::INVALIDATE_TEV);
}

void ScnRoot::DrawXlu() {
    SetGlbSettings();

    if (TestScnRootFlag(SCNROOTFLAG_FORCE_RESMDLDRAWMODE)) {
        mpCollection->DrawXlu(&mDrawMode);
    } else {
        mpCollection->DrawXlu(NULL);
    }

    G3DState::Invalidate(G3DState::INVALIDATE_TEV);
}

void __ct__Q34nw4r3g3d7ScnRootFP12MEMAllocatorPQ34nw4r3g3d13IScnObjGatherPPQ34nw4r3g3d6ScnObjUlUlUlPQ34nw4r3g3d8LightObjPQ34nw4r3g3d11AmbLightObjPQ34nw4r3g3d12LightSetData(){}

void __dt__Q34nw4r3g3d7ScnRootFv(){}

void Add__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d6ScnObjbb(){}

void ScnObjGather::ZSort() {
    std::sort(mpArrayOpa, mpArrayOpa + mNumScnObjOpa, LessZSortOpa);
    std::sort(mpArrayXlu, mpArrayXlu + mNumScnObjXlu, LessZSortXlu);
}

void ScnObjGather::Sort() {
    std::sort(mpArrayOpa, mpArrayOpa + mNumScnObjOpa,
              LessByGetValueForSortOpa);
    std::sort(mpArrayXlu, mpArrayXlu + mNumScnObjXlu,
              LessByGetValueForSortXlu);
}

void ScnObjGather::Sort(LessThanFunc pOpaFunc, LessThanFunc pXluFunc) {
    std::sort(mpArrayOpa, mpArrayOpa + mNumScnObjOpa, pOpaFunc);
    std::sort(mpArrayXlu, mpArrayXlu + mNumScnObjXlu, pXluFunc);
}

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

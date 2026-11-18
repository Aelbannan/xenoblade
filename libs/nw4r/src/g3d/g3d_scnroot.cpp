// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnroot
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/g3d.h>
#include <harness_catalog.h>

#include <algorithm>

#include <revolution/GX.h>
using nw4r::g3d::ScnRoot;
using nw4r::g3d::ScnObjGather;
using nw4r::g3d::Camera;
using nw4r::g3d::Fog;

// The scene-culling frustum pointer (nw4r::g3d::gpCullingFrustum) is defined
// in this TU (retail sbss 0x80665468); g3d_scnobj.h declares it extern, and
// g3d_scnobj.cpp reads it during child gathering.
//
// MWCC's <algorithm> doesn't declare std::sort (same pattern as g3d_draw.cpp);
// the retail nw4r build instantiated the full MSL implementation in this TU.
// The per-instantiation `static int shuffle` pivot-rotation counter is emitted
// by MWCC into .sdata after the CalcAnmScn std::min odr-use statics
// (G3DState::NUM_FOG / G3DState::NUM_CAMERA, the retail sdata slots
// 0x80663480 / 0x80663484), giving the two -4 counters at the retail slots
// 0x80663488 / 0x8066348C.

namespace std {

template <typename TIt, typename TCompare>
inline TIt min_element(TIt first, TIt last, TCompare compare) {
    TIt it = first;

    if (first != last) {
        for (++first; first != last; ++first) {
            if (compare(*first, *it)) {
                it = first;
            }
        }
    }

    return it;
}

template <typename TCompare, typename TIt>
inline void __selection_sort(TIt first, TIt last, TCompare compare) {
    if (first == last) {
        return;
    }

    TIt j = last;
    for (--j; first != j; ++first) {
        TIt i = min_element<TIt, TCompare&>(first, last, compare);

        if (i != first) {
            swap(*i, *first);
        }
    }
}

// Order the three candidate pivot elements (a1, a2, a3): written with the
// if/else shape (not the early-return shape) so MWCC's auto-inliner keeps the
// call out-of-line, as in retail.
template <typename TCompare, typename TIt>
void __sort132(TIt a1, TIt a2, TIt a3, TCompare compare) {
    int i1 = compare(*a3, *a1);
    bool n1 = !i1;
    int i2 = compare(*a2, *a3);
    bool n2 = !i2;

    if (n1 && n2) {
        // both comparisons returned false: ordering already fine
    } else if (!n1 && !n2) {
        swap(*a1, *a2);
    } else {
        // exactly one comparison held: reorder around the third candidate
        if (compare(*a2, *a1)) {
            swap(*a1, *a2);
        }
        if (n1) {
            swap(*a2, *a3);
        } else {
            swap(*a1, *a3);
        }
    }
}

static const int SHUFFLE_MIN = -4;
static const int SHUFFLE_MAX = 5;
static const int SELECTION_SORT_MIN = 20;

template <typename TIt, typename TCompare>
void sort(TIt first, TIt last, TCompare compare) {
    while (true) {
        long len = static_cast<long>(last - first);
        if (len <= 1) {
            return;
        }

        if (len <= SELECTION_SORT_MIN) {
            __selection_sort<TCompare&, TIt>(first, last, compare);
            return;
        }

        static int shuffle = -4;
        TIt m = first + (len / 4 + shuffle % SHUFFLE_MAX);

        if (++shuffle >= SHUFFLE_MAX) {
            shuffle = SHUFFLE_MIN;
        }

        TIt i1 = first + (len * 3 / 4 + shuffle % SHUFFLE_MAX);
        if (++shuffle >= SHUFFLE_MAX) {
            shuffle = SHUFFLE_MIN;
        }

        TIt j = last - 1;
        __sort132<TCompare&, TIt>(m, i1, j, compare);

        m = first;
        i1 = j;
        while (compare(*m, *j)) {
            m++;
        }

        do {
            i1--;

            if (m == i1) {
                break;
            }
        } while (!compare(*i1, *j));

        if (m < i1) {
            swap(*m, *i1);
            m++;

            while (true) {
                while (compare(*m, *j)) {
                    m++;
                }

                while (!compare(*--i1, *j)) {
                    ;
                }

                if (m < i1) {
                    swap(*m, *i1);
                    m++;
                } else {
                    break;
                }
            }
        }

        if (m == first) {
            swap(*m, *j);
            m++;

            i1 = last;
            if (!compare(*first, *--i1)) {
                while (m != last && !compare(*first, *m)) {
                    m++;
                }

                if (m < i1) {
                    swap(*m, *i1);
                }
            }

            if (m < i1) {
                while (true) {
                    while (!compare(*first, *m)) {
                        m++;
                    }

                    while (compare(*first, *--i1)) {
                        ;
                    }

                    if (m < i1) {
                        swap(*m, *i1);
                        m++;
                    } else {
                        break;
                    }
                }
            }

            first = m;
        } else if (m - first < last - m) {
            sort<TIt, TCompare&>(first, m, compare);
            first = m;
        } else {
            sort<TIt, TCompare&>(m, last, compare);
            last = m;
        }
    }
}

} // namespace std

// Explicitly instantiate the median-of-3 helper so it is emitted as its own
// symbol (retail __sort132<...>); MWCC otherwise auto-inlines it into sort.
namespace std {
template void __sort132<bool (*&)(const nw4r::g3d::ScnObj*,
                                  const nw4r::g3d::ScnObj*),
                        nw4r::g3d::ScnObj**>(
    nw4r::g3d::ScnObj**, nw4r::g3d::ScnObj**, nw4r::g3d::ScnObj**,
    bool (*&)(const nw4r::g3d::ScnObj*, const nw4r::g3d::ScnObj*));
} // namespace std

namespace nw4r {
namespace g3d {

const math::FRUSTUM* gpCullingFrustum = NULL;

namespace {

// Z-priority sort comparators for the scene-object arrays. When priorities
// tie, the object farther from the view (larger view-space Z at +0x98) sorts
// first; otherwise lower priority sorts first.
bool LessZSortOpa(const nw4r::g3d::ScnObj* a, const nw4r::g3d::ScnObj* b) {
    int pa = a->GetPriorityDrawOpa();
    int pb = b->GetPriorityDrawOpa();
    if (pa == pb) {
        return a->GetMtxPtr(nw4r::g3d::ScnObj::MTX_VIEW)->m[2][3] >
               b->GetMtxPtr(nw4r::g3d::ScnObj::MTX_VIEW)->m[2][3];
    }
    return pa < pb;
}
bool LessZSortXlu(const nw4r::g3d::ScnObj* a, const nw4r::g3d::ScnObj* b) {
    int pa = a->GetPriorityDrawXlu();
    int pb = b->GetPriorityDrawXlu();
    if (pa == pb) {
        return a->GetMtxPtr(nw4r::g3d::ScnObj::MTX_VIEW)->m[2][3] <
               b->GetMtxPtr(nw4r::g3d::ScnObj::MTX_VIEW)->m[2][3];
    }
    return pa < pb;
}
bool LessByGetValueForSortOpa(const nw4r::g3d::ScnObj* pLhs,
                              const nw4r::g3d::ScnObj* pRhs) {
    if (pLhs->GetPriorityDrawOpa() == pRhs->GetPriorityDrawOpa()) {
        return pLhs->GetValueForSortOpa() < pRhs->GetValueForSortOpa();
    }
    return pLhs->GetPriorityDrawOpa() < pRhs->GetPriorityDrawOpa();
}

bool LessByGetValueForSortXlu(const nw4r::g3d::ScnObj* pLhs,
                              const nw4r::g3d::ScnObj* pRhs) {
    if (pLhs->GetPriorityDrawXlu() == pRhs->GetPriorityDrawXlu()) {
        return pLhs->GetValueForSortXlu() < pRhs->GetValueForSortXlu();
    }
    return pLhs->GetPriorityDrawXlu() < pRhs->GetPriorityDrawXlu();
}

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

// Defined inline here so placement-new inside ScnRoot::Construct inlines the
// vtable + field stores (retail has no out-of-line ScnObjGather ctor symbol).
inline ScnObjGather::ScnObjGather(ScnObj** ppBufOpa, ScnObj** ppBufXlu,
                                  u32 capacity)
    : mpArrayOpa(ppBufOpa), mpArrayXlu(ppBufXlu), mSizeScnObj(capacity),
      mNumScnObjOpa(0), mNumScnObjXlu(0) {}

ScnRoot* ScnRoot::Construct(MEMAllocator* pAllocator, u32* pSize,
                            u32 maxChildren, u32 maxScnObj, u32 numLightObj,
                            u32 numLightSet) {
    ScnRoot* pObj = NULL;
    u32 scnRootSize = sizeof(ScnRoot);
    u32 scnObjGatherSize = sizeof(ScnObjGather);

    u32 childrenSize = maxChildren * sizeof(ScnObj*);
    u32 scnObjBufSize =
        maxScnObj * sizeof(ScnObj*) + maxScnObj * sizeof(ScnObj*);

    u32 lightObjSize = numLightObj * sizeof(LightObj);
    u32 ambLightObjSize = numLightObj * sizeof(AmbLightObj);
    u32 lightSetDataSize = numLightSet * sizeof(LightSetData);

    u32 scnObjGatherOfs = ut::RoundUp<u32>(scnRootSize, 4);
    u32 childrenOfs =
        ut::RoundUp<u32>(scnObjGatherOfs + scnObjGatherSize, 4);
    u32 scnObjBufOfs = ut::RoundUp<u32>(childrenOfs + childrenSize, 4);
    u32 lightObjOfs = ut::RoundUp<u32>(scnObjBufOfs + scnObjBufSize, 4);
    u32 ambLightOfs = ut::RoundUp<u32>(lightObjOfs + lightObjSize, 4);
    u32 lightSetDataOfs = ut::RoundUp<u32>(ambLightOfs + ambLightObjSize, 4);

    u32 size = ut::RoundUp<u32>(lightSetDataOfs + lightSetDataSize, 4);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        u8* buf = static_cast<u8*>(MEMAllocFromAllocator(pAllocator, size));
        if (buf == NULL) {
            return NULL;
        }

        ScnObj** ppScnObjBuf =
            reinterpret_cast<ScnObj**>(buf + scnObjBufOfs);

        IScnObjGather* pGather = new (buf + scnObjGatherOfs)
            ScnObjGather(ppScnObjBuf, ppScnObjBuf + maxScnObj, maxScnObj);

        LightObj* pLightObjBuf =
            reinterpret_cast<LightObj*>(buf + lightObjOfs);

        AmbLightObj* pAmbObjBuf =
            reinterpret_cast<AmbLightObj*>(buf + ambLightOfs);

        LightSetData* pLightSetBuf =
            reinterpret_cast<LightSetData*>(buf + lightSetDataOfs);

        pObj = new (buf) ScnRoot(
            pAllocator, pGather,
            reinterpret_cast<ScnObj**>(buf + childrenOfs), maxChildren,
            numLightObj, numLightSet, pLightObjBuf, pAmbObjBuf, pLightSetBuf);
    }

    return pObj;
}

void ScnRoot::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    // If a child was detached and it was the attached animation, detach it
    // from this root as well; otherwise defer to the group default.
    switch (task) {
    case G3DPROC_CHILD_DETACHED:
        if (mpAnmScn == pInfo) {
            mpAnmScn->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
            mpAnmScn = NULL;
            break;
        }
        // fall through
    default:
        DefG3dProcScnGroup(task, param, pInfo);
        break;
    }
}

Camera ScnRoot::GetCamera(int idx) {
    if (0 <= idx && idx < G3DState::NUM_CAMERA) {
        return Camera(&mCamera[idx]);
    }

    return Camera(NULL);
}

Camera ScnRoot::GetCurrentCamera() {
    return Camera(&mCamera[mCurrentCameraID]);
}

void ScnRoot::SetCurrentCamera(int idx) {
    mCurrentCameraID = static_cast<u8>(idx);
}

Fog ScnRoot::GetFog(int idx) {
    if (0 <= idx && idx < G3DState::NUM_FOG) {
        return Fog(&mFog[idx]);
    }

    return Fog(NULL);
}

// Attach a scene animation to the root: detach any previous animation first,
// then attach the new one (returns true, as retail always does).
bool ScnRoot::SetAnmScn(nw4r::g3d::AnmScn* pAnmScn) {
    if (mpAnmScn != NULL) {
        if (mpAnmScn != NULL) {
            mpAnmScn->G3dProc(nw4r::g3d::G3dObj::G3DPROC_DETACH_PARENT, 0,
                               this);
            mpAnmScn = NULL;
        }
    }

    mpAnmScn = pAnmScn;
    pAnmScn->G3dProc(nw4r::g3d::G3dObj::G3DPROC_ATTACH_PARENT, 0, this);
    return true;
}

// NOTE: the retail ScnRoot::RemoveAnmScn returns the detached AnmScn*
// (see `mr r3, r31` epilogue).
nw4r::g3d::AnmScn* ScnRoot::RemoveAnmScn() {
    nw4r::g3d::AnmScn* pAnmScn = mpAnmScn;

    if (pAnmScn != NULL) {
        pAnmScn->G3dProc(nw4r::g3d::G3dObj::G3DPROC_DETACH_PARENT, 0, this);
        mpAnmScn = NULL;
    }

    return pAnmScn;
}

void ScnRoot::UpdateFrame() {
    if (mpAnmScn != NULL) {
        mpAnmScn->UpdateFrame();
    }

    G3dProc(G3DPROC_UPDATEFRAME, 0, NULL);
}

void ScnRoot::SetGlbSettings() {
    int i;

    Camera cam = GetCurrentCamera();
    cam.GXSetViewport();
    cam.GXSetProjection();
    cam.GXSetScissor();
    cam.GXSetScissorBoxOffset();

    for (i = 0; i < G3DState::NUM_CAMERA; i++) {
        G3DState::SetCameraProjMtx(Camera(&mCamera[i]), i,
                                   i == mCurrentCameraID);
    }

    bool persp = cam.GetProjectionType() != GX_ORTHOGRAPHIC;
    u32 projOrthoBit = persp ? 0 : GX_ORTHOGRAPHIC << 3;

    f32 near = cam.ref().projNear;
    f32 far = cam.ref().projFar;

    u16 width = 0;
    u16 center = 0;

    math::MTX44 proj;
    bool fogRangeAdj = false;

    for (i = 0; i < G3DState::NUM_FOG; i++) {
        Fog fog(&mFog[i]);
        fog.SetNearFar(near, far);

        if (fog.ref().type != GX_FOG_NONE) {
            fog.ref().type = static_cast<GXFogType>(
                (fog.ref().type & ~(GX_ORTHOGRAPHIC << 3)) | projOrthoBit);
        }

        if (fog.IsFogRangeAdjEnable()) {
            if (!fogRangeAdj) {
                cam.GetProjectionMtx(&proj);

                f32 f_width, x;
                cam.GetViewport(&x, NULL, &f_width, NULL, NULL, NULL);

                width = math::F32ToU16(f_width);
                center = math::F32ToU16(x + f_width * 0.5f);
                fogRangeAdj = true;
            }

            fog.SetFogRangeAdjParam(width, center, proj);
        }

        G3DState::SetFog(fog, i);
    }

    G3DState::SetLightSetting(mLightSetting);
}

void ScnRoot::CalcAnmScn() {
    if (mpAnmScn == NULL) {
        return;
    }

    const u32 numLightSet = mpAnmScn->GetLightSetMaxRefNumber();
    const u32 numFog = mpAnmScn->GetFogMaxRefNumber();
    const u32 numCamera = mpAnmScn->GetCameraMaxRefNumber();

    if (numLightSet > 0) {
        mpAnmScn->GetLightSetting(&mLightSetting);
    }

    if (numFog > 0) {
        const u32 numLoadableFog = std::min<u32>(G3DState::NUM_FOG, numFog);

        for (u32 i = 0; i < numLoadableFog; i++) {
            Fog fog(&mFog[i]);
            mpAnmScn->GetFog(fog, i);
        }
    }

    if (numCamera > 0) {
        const u32 numLoadableCamera =
            std::min<u32>(G3DState::NUM_CAMERA, numCamera);

        for (u32 i = 0; i < numLoadableCamera; i++) {
            Camera cam(&mCamera[i]);
            mpAnmScn->GetCamera(cam, i);
        }
    }
}

void ScnRoot::CalcWorld() {
    CalcAnmScn();
    G3dProc(nw4r::g3d::G3dObj::G3DPROC_CALC_WORLD, 0, 0);
}

namespace {

} // namespace

void ScnRoot::CalcView() {
    GXInvalidateVtxCache();

    nw4r::math::MTX34 mtx;
    Camera camera = GetCurrentCamera();

    camera.GetCameraMtx(&mtx);

    G3dProc(nw4r::g3d::G3dObj::G3DPROC_CALC_VIEW, 0, &mtx);
}

void ScnRoot::GatherDrawScnObj() {
    mpCollection->Clear();

    math::FRUSTUM frustum;

    Camera cam = GetCurrentCamera();

    math::MTX34 mtx;
    cam.GetCameraMtx(&mtx);

    if (cam.ref().flags & CameraData::FLAG_PROJ_PERSP) {
        frustum.Set(cam.ref().projFovy, cam.ref().projAspect,
                    cam.ref().projNear, cam.ref().projFar, mtx);
    } else {
        frustum.Set(cam.ref().projTop, cam.ref().projBottom,
                    cam.ref().projLeft, cam.ref().projRight,
                    cam.ref().projNear, cam.ref().projFar, mtx);
    }

    gpCullingFrustum = &frustum;
    G3dProc(G3DPROC_GATHER_SCNOBJ, 0, mpCollection);
    gpCullingFrustum = NULL;
}

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

ScnRoot::ScnRoot(MEMAllocator* pAllocator, IScnObjGather* pGather,
                 ScnObj** ppChildrenBuf, u32 maxChildren, u32 numLight,
                 u32 numLightSet, LightObj* pLightObjBuf,
                 AmbLightObj* pAmbObjBuf, LightSetData* pLightSetBuf)
    : ScnGroup(pAllocator, ppChildrenBuf, maxChildren),
      mpCollection(pGather),
      mDrawMode(RESMDL_DRAWMODE_SORT_XLU_Z),
      mScnRootFlags(0),
      mCurrentCameraID(0),
      // Declared after the camera/fog arrays, so it initializes after them.
      mLightSetting(pLightObjBuf, pAmbObjBuf, numLight, pLightSetBuf,
                    numLightSet),
      mpAnmScn(NULL) {
    for (u32 i = 0; i < G3DState::NUM_CAMERA; i++) {
        Camera camera(&mCamera[i]);
        camera.Init();
    }

    for (u32 i = 0; i < G3DState::NUM_FOG; i++) {
        Fog fog(&mFog[i]);
        fog.Init();
    }
}

ScnRoot::~ScnRoot() {
    if (mpAnmScn != NULL) {
        mpAnmScn->G3dProc(nw4r::g3d::G3dObj::G3DPROC_DETACH_PARENT, 0, this);
    }
}

nw4r::g3d::IScnObjGather::CullingStatus ScnObjGather::Add(ScnObj* pObj, bool opa, bool xlu) {
    CullingStatus status = CULLINGSTATUS_INTERSECT;

    // If culling is active, cull against the global frustum; an object that
    // is fully outside is rejected outright, fully inside is recorded so the
    // caller can skip per-object tests later.
    if (gpCullingFrustum != NULL) {
        u32 option;
        pObj->GetScnObjOption(ScnObj::OPTID_ENABLE_CULLING, &option);
        if (option != 0) {
            math::AABB aabb;
            pObj->GetBoundingVolume(ScnObj::BOUNDINGVOLUME_AABB_WORLD, &aabb);

            math::IntersectionResult result =
                gpCullingFrustum->IntersectAABB_Ex(&aabb);
            if (result == math::INTERSECTION_OUTSIDE) {
                return CULLINGSTATUS_OUTSIDE;
            }
            if (result == math::INTERSECTION_INSIDE) {
                status = CULLINGSTATUS_INSIDE;
            }
        }
    }

    if (opa) {
        if (mNumScnObjOpa < mSizeScnObj) {
            mpArrayOpa[mNumScnObjOpa++] = pObj;
        } else {
            return status;
        }
    }

    if (xlu) {
        if (mNumScnObjXlu < mSizeScnObj) {
            mpArrayXlu[mNumScnObjXlu++] = pObj;
        } else {
            return status;
        }
    }

    return status;
}

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

void ScnObjGather::DrawOpa(ResMdlDrawMode* pForceMode) {
    // Counter incremented before the virtual call, matching retail scheduling.
    u32 i = 0;
    while (i != mNumScnObjOpa) {
        ScnObj* pObj = mpArrayOpa[i++];
        pObj->G3dProc(nw4r::g3d::G3dObj::G3DPROC_DRAW_OPA, 0, pForceMode);
    }
}

void ScnObjGather::DrawXlu(ResMdlDrawMode* pForceMode) {
    // Counter incremented before the virtual call, matching retail scheduling.
    u32 i = 0;
    while (i != mNumScnObjXlu) {
        ScnObj* pObj = mpArrayXlu[i++];
        pObj->G3dProc(nw4r::g3d::G3dObj::G3DPROC_DRAW_XLU, 0, pForceMode);
    }
}

u32 ScnObjGather::CheckScnObj(ScnObj* pObj) { return 0; }

namespace nw4r { namespace g3d { class ScnObj; }}

namespace nw4r {
namespace g3d {


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("ScnRoot")> lbl_eu_8051D798 = {sizeof("ScnRoot"), "ScnRoot"};

// ScnRoot's base-chain check (ScnGroup, ScnObj, G3dObj) as a static helper,
// same convention as scnObjBaseIsDerivedFrom / anmObjBaseIsDerivedFrom: the
// inlined by-value helper boundary makes MWCC reload other at the ScnGroup
// link and CSE the remaining comparisons, as retail does.
static bool scnRootBaseIsDerivedFrom(G3dObj::TypeObj other) {
    return other == G3dObj::TypeObj(lbl_eu_8051D788) ? true
         : other == G3dObj::TypeObj(lbl_eu_8051D768) ? true
         : (other == G3dObj::TypeObj(lbl_eu_8051D640));
}

bool ScnRoot::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(lbl_eu_8051D798)) {
        return true;
    }
    return scnRootBaseIsDerivedFrom(other);
}

const G3dObj::TypeObj ScnRoot::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D798);
}

const char* ScnRoot::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r}

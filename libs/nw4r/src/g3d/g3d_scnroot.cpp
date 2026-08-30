// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnroot
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/g3d.h>
#include <harness_catalog.h>
using nw4r::g3d::ScnRoot;
using nw4r::g3d::ScnObjGather;
using nw4r::g3d::Camera;
using nw4r::g3d::Fog;

// gpCullingFrustum (declared in g3d_scnobj.h) is defined in g3d_scnobj.cpp;
// the retail linker name is lbl_eu_80665468 (sbss). Reference the retail name
// directly so the SDA21 relocs match retail.


// MWCC's <algorithm> doesn't declare std::sort (same pattern as g3d_draw.cpp);
// the retail nw4r build instantiated the full MSL implementation in this TU.
// Reconstructed from the retail object:
//   - small ranges (<= 20 elements) are sorted by selection sort,
//   - larger ranges are quicksorted with a pivot picked from two pseudo-random
//     positions near the 1/4 and 3/4 marks (a static `shuffle` counter rotates
//     the offset so repeated sorts of similar data don't hit the same pivot),
//   - after partitioning, the smaller side is recursed into and the larger
//     side is looped on, keeping the recursion depth bounded.
// Pivot rotation counter shared by the ScnObj sort instantiations in this TU
// (retail linker name lbl_eu_8066348C).
extern "C" {
__declspec(section ".sdata") __attribute__((aligned(8))) int lbl_eu_80663480 = 0x20;
__declspec(section ".sdata") __attribute__((aligned(4))) int lbl_eu_80663484 = 0x20;
__declspec(section ".sdata") __attribute__((aligned(8))) int lbl_eu_80663488 = -4;
__declspec(section ".sdata") __attribute__((aligned(4))) int lbl_eu_8066348C = -4;
}
extern "C" __attribute__((aligned(8))) unsigned char lbl_eu_80665468[8] = {0};

namespace std {

template <typename T>
inline void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// Order the three candidate pivot elements over *_F, *_L, *_P.
template <typename _Pr, typename _RI>
void __sort132(_RI _F, _RI _L, _RI _P, _Pr _C) {
    int i1 = _C(*_P, *_F);
    bool n1 = !i1;
    int i2 = _C(*_L, *_P);
    bool n2 = !i2;

    if (n1 && n2) {
        // both comparisons returned false: ordering already fine
    } else if (!n1 && !n2) {
        swap(*_F, *_L);
    } else {
        // exactly one comparison held: reorder around the third candidate
        if (_C(*_L, *_F)) {
            swap(*_F, *_L);
        }
        if (n1) {
            swap(*_L, *_P);
        } else {
            swap(*_F, *_P);
        }
    }
}

template <typename _RI, typename _Pr>
void sort(_RI _F, _RI _L, _Pr _P) {

    for (;;) {
        int count = _L - _F;
        if (count <= 1) {
            return;
        }

        if (count <= 20) {
            if (_F != _L) {
                // Selection sort: for each position, find the smallest
                // remaining element and swap it into place.
                for (_RI i = _F; i != _L - 1; ++i) {
                    _RI min = i;
                    if (i != _L) {
                        for (_RI j = i + 1; j != _L; ++j) {
                            if (_P(*j, *min)) {
                                min = j;
                            }
                        }
                    }
                    if (min != i) {
                        swap(*min, *i);
                    }
                }
            }
            return;
        }

        // count > 20: quicksort path
        {
            int c = lbl_eu_8066348C;
            _RI p1 = _F + (count / 4 + c % 5);
            int c1 = c + 1;
            if (c1 >= 5) {
                c1 = -4;
            }
            int s = c1 + 1;
            lbl_eu_8066348C = s;
            _RI p2 = _F + (((count << 2) - count) / 4 + c1 % 5);
            if (s >= 5) {
                // Only reachable when c == 4 (s == 5), so this stores -4.
                lbl_eu_8066348C = s - 9;
            }
            // Force the reference comparator instantiation (as retail does).
            _RI pivot = _L - 1;
            __sort132<_Pr&, _RI>(p1, p2, pivot, _P);

            // Partition [_F, _L) around the pivot kept at _L - 1: advance i
            // past elements below the pivot and j down past elements above it.
            _RI i = _F;
            _RI j = pivot;
            while (_P(*i, *pivot)) {
                ++i;
            }
            do {
                --j;
                if (i == j) {
                    goto partition_end;
                }
            } while (!_P(*j, *pivot));
            if (i < j) {
                // Rotated loop: the first swap is peeled before the scan loop
                // (matches retail layout).
                swap(*i, *j);
                ++i;
                for (;;) {
                    while (_P(*i, *pivot)) {
                        ++i;
                    }
                    do {
                        --j;
                    } while (!_P(*j, *pivot));
                    if (i >= j) {
                        break;
                    }
                    swap(*i, *j);
                    ++i;
                }
            }
        partition_end:;

            if (i == _F) {
                // The pivot is the smallest element: move it to the front and
                // partition the remainder around it with the comparisons
                // reversed.
                swap(*i, *pivot);
                ++i;
                j = _L - 1;
                if (!_P(*_F, *(_L - 1))) {
                    while (i != _L && !_P(*_F, *i)) {
                        ++i;
                    }
                    if (i < j) {
                        swap(*i, *j);
                    }
                }
                while (i < j) {
                    while (!_P(*_F, *i)) {
                        ++i;
                    }
                    do {
                        --j;
                    } while (_P(*_F, *j));
                    if (i >= j) {
                        break;
                    }
                    swap(*i, *j);
                    ++i;
                }
                _F = i;
            } else {
                // Recurse on the smaller side and loop on the larger side.
                // The recursion always uses the reference comparator form.
                int left = i - _F;
                int right = _L - i;
                if (left < right) {
                    sort<_RI, _Pr&>(_F, i, _P);
                    _F = i;
                } else {
                    sort<_RI, _Pr&>(i, _L, _P);
                    _L = i;
                }
            }
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

    // Lay out the single backing allocation: root object, gather, child
    // pointer array, opaque/xlu object arrays, light objects, ambient light
    // objects and light set data - each segment 4-byte aligned.
    u32 ofsOpa = ut::RoundUp<u32>(sizeof(ScnRoot) + sizeof(ScnObjGather) +
                                      maxChildren * sizeof(ScnObj*),
                                  4);
    u32 ofsLightObj =
        ut::RoundUp<u32>(ofsOpa + maxScnObj * sizeof(ScnObj*) * 2, 4);
    u32 ofsAmb =
        ut::RoundUp<u32>(ofsLightObj + numLightObj * sizeof(LightObj), 4);
    u32 ofsLightSet =
        ut::RoundUp<u32>(ofsAmb + numLightObj * sizeof(AmbLightObj), 4);
    u32 ofsXlu = maxScnObj * sizeof(ScnObj*);
    u32 size =
        ut::RoundUp<u32>(ofsLightSet + numLightSet * sizeof(LightSetData), 4);

    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        u8* buf = static_cast<u8*>(MEMAllocFromAllocator(pAllocator, size));

        if (buf == NULL) {
            return NULL;
        }

        ScnObjGather* pGather = new (buf + sizeof(ScnRoot)) ScnObjGather(
            reinterpret_cast<ScnObj**>(buf + ofsOpa),
            reinterpret_cast<ScnObj**>(reinterpret_cast<u8*>(buf + ofsOpa) +
                                       ofsXlu),
            maxScnObj);

        pObj = new (buf) ScnRoot(
            pAllocator, pGather,
            reinterpret_cast<ScnObj**>(buf + sizeof(ScnRoot) +
                                       sizeof(ScnObjGather)),
            maxChildren, numLightObj, numLightSet,
            reinterpret_cast<LightObj*>(buf + ofsLightObj),
            reinterpret_cast<AmbLightObj*>(buf + ofsAmb),
            reinterpret_cast<LightSetData*>(buf + ofsLightSet));
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

void SetCurrentCamera__Q34nw4r3g3d7ScnRootFi(ScnRoot* self, int idx) {
    self->mCurrentCameraID = static_cast<u8>(idx);
}

Fog ScnRoot::GetFog(int idx) {
    if (0 <= idx && idx < G3DState::NUM_FOG) {
        return Fog(&mFog[idx]);
    }

    return Fog(NULL);
}

// Attach a scene animation to the root: detach any previous animation first,
// then attach the new one (returns true, as retail always does).
bool SetAnmScn__Q34nw4r3g3d7ScnRootFPQ34nw4r3g3d6AnmScn(ScnRoot* self, nw4r::g3d::AnmScn* pAnmScn) {
    if (self->mpAnmScn != NULL) {
        if (self->mpAnmScn != NULL) {
            self->mpAnmScn->G3dProc(
                nw4r::g3d::G3dObj::G3DPROC_DETACH_PARENT, 0, self);
            self->mpAnmScn = NULL;
        }
    }

    self->mpAnmScn = pAnmScn;
    pAnmScn->G3dProc(nw4r::g3d::G3dObj::G3DPROC_ATTACH_PARENT, 0, self);
    return true;
}

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

void CalcWorld__Q34nw4r3g3d7ScnRootFv(ScnRoot* self) {
    self->CalcAnmScn();
    self->G3dProc(nw4r::g3d::G3dObj::G3DPROC_CALC_WORLD, 0, 0);
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

    Camera camera = GetCurrentCamera();

    math::MTX34 mtx;
    camera.GetCameraMtx(&mtx);

    // Set up the culling frustum from the current camera's projection.
    math::FRUSTUM fr;
    if (camera.ref().flags & CameraData::FLAG_PROJ_PERSP) {
        fr.Set(camera.ref().projFovy, camera.ref().projAspect,
               camera.ref().projNear, camera.ref().projFar, mtx);
    } else {
        fr.Set(camera.ref().projTop, camera.ref().projBottom,
               camera.ref().projLeft, camera.ref().projRight,
               camera.ref().projNear, camera.ref().projFar, mtx);
    }

    *(::nw4r::math::FRUSTUM**)lbl_eu_80665468 = &fr;
    G3dProc(G3DPROC_GATHER_SCNOBJ, 0, mpCollection);
    *(::nw4r::math::FRUSTUM**)lbl_eu_80665468 = NULL;
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
    if (*(::nw4r::math::FRUSTUM**)lbl_eu_80665468 != NULL) {
        u32 option;
        pObj->GetScnObjOption(ScnObj::OPTID_ENABLE_CULLING, &option);
        if (option != 0) {
            math::AABB aabb;
            pObj->GetBoundingVolume(ScnObj::BOUNDINGVOLUME_AABB_WORLD, &aabb);

            math::IntersectionResult result =
                (*(::nw4r::math::FRUSTUM**)lbl_eu_80665468)->IntersectAABB_Ex(&aabb);
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
} // namespace nw4r}

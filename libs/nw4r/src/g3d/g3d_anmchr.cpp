// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmchr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Widened for the retail-symbol Bind below: AnmObjTexSrt's binding buffer is
// protected and AnmObjTexSrtRes::mRes is private, and g3d_anmtexsrt.h is
// outside this session's writable scope.
#define private public
#define protected public
#include <nw4r/g3d.h>
#undef private
#undef protected

namespace nw4r {
namespace g3d {

f32 AnmObjChrRes::GetFrame() const {
    return GetFrm();
}

// Out-of-line dtor: the retail emits the three nested null-guards and the
// G3dObj base-dtor call (delete flag 0); the inline header body would
// short-circuit the chain.
nw4r::g3d::AnmObjChrRes::~AnmObjChrRes() {}

void AnmObjChrBlend::SetWeight(int idx, f32 weight) {
    mpWeightArray[idx] = weight;
}

f32 AnmObjChrBlend::GetWeight(int idx) const {
    return mpWeightArray[idx];
}

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChr")> lbl_eu_8051D5C0 = {sizeof("AnmObjChr"), "AnmObjChr"};

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

bool AnmObjChr::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(lbl_eu_8051D5C0)) {
        return true;
    }
    return anmObjBaseIsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjChr::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5C0);
}

const char* AnmObjChr::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChrNode")> lbl_eu_8051D5D0 = {sizeof("AnmObjChrNode"), "AnmObjChrNode"};

bool AnmObjChrNode::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(lbl_eu_8051D5D0)) {
        return true;
    }
    return AnmObjChr::IsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjChrNode::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5D0);
}

const char* AnmObjChrNode::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChrBlend")> lbl_eu_8051D5E4 = {sizeof("AnmObjChrBlend"), "AnmObjChrBlend"};

const G3dObj::TypeObj AnmObjChrBlend::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5E4);
}

const char* AnmObjChrBlend::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChrRes")> lbl_eu_8051D5F8 = {sizeof("AnmObjChrRes"), "AnmObjChrRes"};

bool AnmObjChrRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(lbl_eu_8051D5F8)) {
        return true;
    }
    return AnmObjChr::IsDerivedFrom(other);
}

const G3dObj::TypeObj AnmObjChrRes::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5F8);
}

const char* AnmObjChrRes::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmObjChr::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

} // namespace g3d
} // namespace nw4r



int Attach__Q34nw4r3g3d9AnmObjChrFiPQ34nw4r3g3d12AnmObjChrRes(void) { return 0x0; }

int Detach__Q34nw4r3g3d9AnmObjChrFi(void) { return 0x0; }

void SetWeight__Q34nw4r3g3d9AnmObjChrFif(void) {}

float GetWeight__Q34nw4r3g3d9AnmObjChrCFi() { return lbl_eu_80669B88; }

void DetachAll__Q34nw4r3g3d9AnmObjChrFv(void) {}


namespace nw4r {
namespace g3d {

AnmObjChrNode::~AnmObjChrNode() {
    DetachAll();
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

// Branch order mirrors retail: ATTACH stores the parent; CHILD_DETACHED
// detaches the matching child; DETACH_PARENT clears the parent.
void AnmObjChrNode::G3dProc(u32 task, u32 param, void* pInfo) {
    switch ((int)task) {
    case G3DPROC_CHILD_DETACHED:
        for (int i = 0; i < mChildrenArraySize; i++) {
            if (mpChildrenArray[i] == pInfo) {
                Detach(i);
                return;
            }
        }
        return;
    case G3DPROC_DETACH_PARENT:
        SetParent(NULL);
        break;
    case G3DPROC_ATTACH_PARENT:
        SetParent(static_cast<G3dObj*>(pInfo));
        break;
    default:
        break;
    }
}

AnmObjChrRes* AnmObjChrNode::Attach(int idx, AnmObjChrRes* pRes) {
    AnmObjChrRes* pOld = Detach(idx);

    bool found = false;

    for (u32 i = 0; i < static_cast<u32>(mNumBinding); i++) {
        if (pRes->TestDefined(i)) {
            found = true;
            mpBinding[i] = 0;
        }
    }

    if (found) {
        SetAnmFlag(FLAG_ANM_BOUND, true);
    }

    mpChildrenArray[idx] = pRes;
    pRes->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);

    return pOld;
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

AnmObjChrRes* AnmObjChrNode::Detach(int index) {
    AnmObjChrRes* pOld = mpChildrenArray[index];

    if (pOld != NULL) {
        pOld->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpChildrenArray[index] = NULL;

        bool bFound = false;

        // Unsigned compare against mNumBinding matches retail cmplw.
        for (u32 j = 0; j < static_cast<u32>(mNumBinding); j++) {
            u16 binding = BINDING_UNDEFINED;

            for (int i = 0; i < mChildrenArraySize; i++) {
                AnmObjChrRes* p = mpChildrenArray[i];

                if (p != NULL && p->TestDefined(j)) {
                    bFound = true;
                    binding = 0;
                    break;
                }
            }

            mpBinding[j] = binding;
        }

        if (!bFound) {
            SetAnmFlag(FLAG_ANM_BOUND, false);
        }
    }

    return pOld;
}

} // namespace g3d
} // namespace nw4r




namespace nw4r {
namespace g3d {

void AnmObjChrNode::SetFrame(f32 frame) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetFrame(frame);
        }
    }
}

f32 AnmObjChrNode::GetFrame() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetFrame();
        }
    }
    return lbl_eu_80669B88;
}

void AnmObjChrNode::SetUpdateRate(f32 rate) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetUpdateRate(rate);
        }
    }
}

f32 AnmObjChrNode::GetUpdateRate() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetUpdateRate();
        }
    }
    return lbl_eu_80669B8C;
}

void AnmObjChrNode::DetachAll() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        Detach(i);
    }
}

void AnmObjChrNode::UpdateFrame() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->UpdateFrame();
        }
    }
}

} // namespace g3d
} // namespace nw4r

// Propagate Bind down every attached child; we are bound if any child is.
// The byte-offset induction variables mirror retail's strength reduction.
namespace nw4r {
namespace g3d {

bool AnmObjChrNode::Bind(const ResMdl mdl) {
    bool bound = false;

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjChrRes* pRes = mpChildrenArray[i];

        if (pRes != NULL) {
            bool result = pRes->Bind(mdl);
            bound = bound || result;

            // Merge the child's defined bindings into our table.
            for (u32 j = 0; j < (u32)mNumBinding; j++) {
                if (pRes->TestDefined(j)) {
                    mpBinding[j] = 0;
                }
            }
        }
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return bound;
}

// Two-target variant: identical propagation, forwarding mdl/target/option.
bool AnmObjChrNode::Bind(const ResMdl mdl, u32 target, BindOption option) {
    bool bound = false;

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjChrRes* pRes = mpChildrenArray[i];

        if (pRes != NULL) {
            bool result = pRes->Bind(mdl, target, option);
            bound = bound || result;

            for (u32 j = 0; j < (u32)mNumBinding; j++) {
                if (pRes->TestDefined(j)) {
                    mpBinding[j] = 0;
                }
            }
        }
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return bound;
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmObjChrNode::Release() {
    // Release every bound child, clear all bindings to UNDEFINED, drop the
    // bound flag.
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->Release();
        }
    }

    // Explicit byte-offset IV keeps MWCC from deriving its own temp,
    // which would swap the counter/offset registers vs retail.
    for (int i = 0, ofs = 0; i < mNumBinding; ofs += (int)sizeof(u16), i++) {
        *(u16*)(reinterpret_cast<u8*>(mpBinding) + ofs) = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

} // namespace g3d
} // namespace nw4r

// Release every child, mark everything UNDEFINED, then re-mark entries
// whose children still define their own bindings.
namespace nw4r {
namespace g3d {

void AnmObjChrNode::Release(const ResMdl mdl, u32 target, BindOption option) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjChrRes* pRes = mpChildrenArray[i];

        if (pRes != NULL) {
            pRes->Release(mdl, target, option);
        }
    }

    for (int j = 0, ofs = 0; j < mNumBinding; j++, ofs += (int)sizeof(u16)) {
        *(u16*)(reinterpret_cast<u8*>(mpBinding) + ofs) = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjChrRes* pRes = mpChildrenArray[i];

        if (pRes != NULL) {
            for (u32 j = 0; j < (u32)mNumBinding; j++) {
                if (pRes->TestDefined(j)) {
                    mpBinding[j] = 0;
                }
            }
        }
    }
}

} // namespace g3d
} // namespace nw4r

// Retail-owned construction vtables (nw4r_data.s)
extern char lbl_eu_80569438[]; // AnmObjChr construction vtable
extern char lbl_eu_805693E0[]; // AnmObjChrNode vtable
extern char lbl_eu_80569388[]; // AnmObjChrBlend vtable
extern char lbl_eu_80569330[]; // AnmObjChrRes vtable
// Complete const array type sized to the retail .sdata table (2 entries,
// 8 bytes) keeps it small-data eligible (retail: li r4, tbl@sda21).
extern const nw4r::g3d::PlayPolicyFunc lbl_eu_80663458[2]; // anm play-policy table

namespace nw4r {
namespace g3d {

// Constructor chain for AnmObjChrBlend. Defined inline in this TU so MWCC
// inlines the whole chain into Construct below, matching retail's expansion:
// base fields, then a *virtual* Release() during AnmObjChr construction
// (initializes every binding to UNDEFINED), then zeroing the child
// pointer array, then filling the weight array with 1.0f.
inline AnmObjChr::AnmObjChr(MEMAllocator* pAllocator, u16* pBindingBuf,
                            int numBinding)
    : AnmObj(pAllocator, NULL), mNumBinding(numBinding),
      mpBinding(pBindingBuf) {
    *(void**)this = (void*)lbl_eu_80569438;
    Release();
}

inline AnmObjChrNode::AnmObjChrNode(MEMAllocator* pAllocator, u16* pBindingBuf,
                                    int numBinding,
                                    AnmObjChrRes** ppChildrenBuf,
                                    int numChildren)
    : AnmObjChr(pAllocator, pBindingBuf, numBinding),
      mChildrenArraySize(numChildren), mpChildrenArray(ppChildrenBuf) {
    *(void**)this = (void*)lbl_eu_805693E0;
    for (int i = 0; i < mChildrenArraySize; i++) {
        mpChildrenArray[i] = NULL;
    }
}

inline AnmObjChrBlend::AnmObjChrBlend(MEMAllocator* pAllocator,
                                      u16* pBindingBuf, int numBinding,
                                      AnmObjChrRes** ppChildrenBuf,
                                      int numChildren, f32* pWeightBuf)
    : AnmObjChrNode(pAllocator, pBindingBuf, numBinding, ppChildrenBuf,
                    numChildren),
      mpWeightArray(pWeightBuf) {
    *(void**)this = (void*)lbl_eu_80569388;

    // All weights start fully bound (1.0f, the shared sdata2 constant).
    for (int i = 0; i < mChildrenArraySize; i++) {
        mpWeightArray[i] = lbl_eu_80669B8C;
    }
}

AnmObjChrBlend* AnmObjChrBlend::Construct(MEMAllocator* pAllocator,
                                          u32* pSize, ResMdl mdl,
                                          int numChildren) {
    if (!mdl.IsValid()) {
        return NULL;
    }

    int numBinding = mdl.GetResNodeNumEntries();

    // Layout: [object][u16 bindings][child ptrs][f32 weights], each region
    // 4-byte aligned after the previous one.
    u32 childrenOfs =
        (sizeof(AnmObjChrBlend) + numBinding * sizeof(u16) + 3) & ~3u;
    u32 childSpan = numChildren * (u32)sizeof(AnmObjChrRes*);
    u32 weightOfs = (childrenOfs + childSpan + 3) & ~3u;
    u32 totalSize = (weightOfs + childSpan + 3) & ~3u;

    if (pSize != NULL) {
        *pSize = totalSize;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    void* pMem = MEMAllocFromAllocator(pAllocator, totalSize);

    if (pMem == NULL) {
        return NULL;
    }

    return new (pMem) AnmObjChrBlend(
        pAllocator,
        reinterpret_cast<u16*>(static_cast<char*>(pMem) + sizeof(AnmObjChrBlend)),
        numBinding,
        reinterpret_cast<AnmObjChrRes**>(static_cast<char*>(pMem) + childrenOfs),
        numChildren,
        reinterpret_cast<f32*>(static_cast<char*>(pMem) + weightOfs));
}

} // namespace g3d
} // namespace nw4r

// Shared .sdata2 quiet-NaN constant (0x7FC00000, defined in port/data_defs.cpp);
// retail's inlined FLog domain-guard loads NaN from this slot.
extern const f32 lbl_eu_80669B90;

namespace nw4r {
namespace g3d {

// Blended GetResult: weights every attached child's ChrAnmResult by
// mpWeightArray[idx]. One contributing child forwards directly; none clears
// the result. Scale blending uses logs/exps under FLAG_USE_ACCURATE_SCALE_BLEND,
// rotation blends via quaternions under FLAG_USE_QUATERNION_ROTATION_BLEND,
// otherwise rotation rows and the translation column blend linearly.
const ChrAnmResult* AnmObjChrBlend::GetResult(ChrAnmResult* pResult, u32 idx) {
    // First pass: collect contributing children and the total weight.
    AnmObjChrRes* pFirst = NULL;
    int count = 0;
    f32 totalWeight = lbl_eu_80669B88;

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjChrRes* pRes = mpChildrenArray[i];
        f32 weight = mpWeightArray[i];

        if (pRes == NULL || weight == lbl_eu_80669B88 ||
            !pRes->TestExistence(idx)) {
            continue;
        }

        if (pFirst == NULL) {
            pFirst = pRes;
        }

        totalWeight += weight;
        count++;
    }

    if (count == 0) {
        pResult->flags = 0;
        return pResult;
    }

    if (count == 1) {
        return pFirst->GetResult(pResult, idx);
    }

    bool bQuatBlend = TestAnmFlag(FLAG_USE_QUATERNION_ROTATION_BLEND);
    bool bAccurateScale = TestAnmFlag(FLAG_USE_ACCURATE_SCALE_BLEND);

    f32 invTotal = math::FInv(totalWeight);

    // Start from "everything": all flags kept, zero scale/matrix, identity quat.
    pResult->flags = ~0u;
    pResult->s.x = lbl_eu_80669B88;
    pResult->s.y = lbl_eu_80669B88;
    pResult->s.z = lbl_eu_80669B88;

    math::MTX34Zero(&pResult->rt);

    math::QUAT accQuat(lbl_eu_80669B88, lbl_eu_80669B88, lbl_eu_80669B88,
                       lbl_eu_80669B8C);
    math::MTX33 firstRot;
    math::VEC3 logScale;
    logScale.x = lbl_eu_80669B88;
    logScale.y = lbl_eu_80669B88;
    logScale.z = lbl_eu_80669B88;
    f32 quatWeightSum = lbl_eu_80669B88;

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjChrRes* pRes = mpChildrenArray[i];
        f32 weight = mpWeightArray[i];

        if (pRes == NULL || weight == lbl_eu_80669B88 ||
            !pRes->TestExistence(idx)) {
            continue;
        }

        ChrAnmResult childResult;
        const ChrAnmResult* pChild = pRes->GetResult(&childResult, idx);
        u32 flags = pChild->flags;

        f32 w = weight * invTotal;

        if (!bAccurateScale) {
            if (!(flags & ChrAnmResult::FLAG_SCALE_ONE)) {
                pResult->s.x += pChild->s.x * w;
                pResult->s.y += pChild->s.y * w;
                pResult->s.z += pChild->s.z * w;
            } else {
                pResult->s.x += w;
                pResult->s.y += w;
                pResult->s.z += w;
            }
        } else if (!(flags & ChrAnmResult::FLAG_SCALE_ONE)) {
            // Domain-guarded log: non-positive child scales contribute NaN,
            // loaded from the shared sdata2 slot.
            logScale.x +=
                w * (pChild->s.x > lbl_eu_80669B88
                         ? math::detail::FLog(pChild->s.x)
                         : lbl_eu_80669B90);
            logScale.y +=
                w * (pChild->s.y > lbl_eu_80669B88
                         ? math::detail::FLog(pChild->s.y)
                         : lbl_eu_80669B90);
            logScale.z +=
                w * (pChild->s.z > lbl_eu_80669B88
                         ? math::detail::FLog(pChild->s.z)
                         : lbl_eu_80669B90);
        }

        if (bQuatBlend) {
            math::QUAT q;

            if (flags & ChrAnmResult::FLAG_ROT_ZERO) {
                q.x = lbl_eu_80669B88;
                q.y = lbl_eu_80669B88;
                q.z = lbl_eu_80669B88;
                q.w = lbl_eu_80669B8C;
            } else {
                math::MTX34ToQUAT(&q, &pChild->rt);
            }

            quatWeightSum += weight;
            C_QUATSlerp(&accQuat, &q, &accQuat,
                        weight * math::FInv(quatWeightSum));
        } else {
            if (!(flags & ChrAnmResult::FLAG_ROT_ZERO)) {
                if (i == 0) {
                    firstRot._00 = pChild->rt._00;
                    firstRot._01 = pChild->rt._01;
                    firstRot._02 = pChild->rt._02;
                    firstRot._10 = pChild->rt._10;
                    firstRot._11 = pChild->rt._11;
                    firstRot._12 = pChild->rt._12;
                    firstRot._20 = pChild->rt._20;
                    firstRot._21 = pChild->rt._21;
                    firstRot._22 = pChild->rt._22;
                }

                pResult->rt._00 += pChild->rt._00 * w;
                pResult->rt._01 += pChild->rt._01 * w;
                pResult->rt._02 += pChild->rt._02 * w;
                pResult->rt._10 += pChild->rt._10 * w;
                pResult->rt._11 += pChild->rt._11 * w;
                pResult->rt._12 += pChild->rt._12 * w;
            } else {
                if (i == 0) {
                    math::MTX33Identity(&firstRot);
                }

                pResult->rt._00 += w;
                pResult->rt._11 += w;
            }
        }

        if (!(flags & ChrAnmResult::FLAG_TRANS_ZERO)) {
            pResult->rt._03 += pChild->rt._03 * w;
            pResult->rt._13 += pChild->rt._13 * w;
            pResult->rt._23 += pChild->rt._23 * w;
        }

        pResult->flags &= flags;
    }

    if (bAccurateScale) {
        pResult->s.x = math::FExp(logScale.x);
        pResult->s.y = math::FExp(logScale.y);
        pResult->s.z = math::FExp(logScale.z);
    }

    if (bQuatBlend) {
        // Rebuild the rotation from the blended quaternion, keeping translation.
        math::VEC3 trans(pResult->rt._03, pResult->rt._13, pResult->rt._23);
        math::QUATToMTX34(&pResult->rt, &accQuat);
        pResult->rt._03 = trans.x;
        pResult->rt._13 = trans.y;
        pResult->rt._23 = trans.z;
    } else {
        // Re-orthogonalize the blended rotation rows; fall back to the first
        // child's raw rotation when a row degenerated to zero length.
        Vec* pRow0 = reinterpret_cast<Vec*>(&pResult->rt._00);
        Vec* pRow1 = reinterpret_cast<Vec*>(&pResult->rt._10);
        Vec* pRow2 = reinterpret_cast<Vec*>(&pResult->rt._20);

        PSVECCrossProduct(pRow0, pRow1, pRow2);

        f32 lenSq0 = pRow0->x * pRow0->x + pRow0->y * pRow0->y +
                     pRow0->z * pRow0->z;
        f32 lenSq2 = pRow2->x * pRow2->x + pRow2->y * pRow2->y +
                     pRow2->z * pRow2->z;

        if (lenSq0 == lbl_eu_80669B88 || lenSq2 == lbl_eu_80669B88) {
            pResult->rt._00 = firstRot._00;
            pResult->rt._01 = firstRot._01;
            pResult->rt._02 = firstRot._02;
            pResult->rt._10 = firstRot._10;
            pResult->rt._11 = firstRot._11;
            pResult->rt._12 = firstRot._12;
            pResult->rt._20 = firstRot._20;
            pResult->rt._21 = firstRot._21;
            pResult->rt._22 = firstRot._22;
        } else {
            PSVECNormalize(pRow0, pRow0);
            PSVECNormalize(pRow2, pRow2);
            PSVECCrossProduct(pRow2, pRow0, pRow1);
        }
    }

    pResult->flags &= ~ChrAnmResult::FLAG_ROT_RAW_FMT;
    return pResult;
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

// Constructor chain: AnmObjChr base installs the construction vtable and runs
// a *virtual* Release() (clears every binding entry); FrameCtrl picks up
// [0, numFrame) driven by the resource's play-policy table.
inline AnmObjChrRes::AnmObjChrRes(MEMAllocator* pAllocator, ResAnmChr chr,
                                  u16* pBindingBuf, int numBinding,
                                  ChrAnmResult* pCacheBuf)
    : AnmObjChr(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f, chr.ref().info.numFrame,
                lbl_eu_80663458[chr.ref().info.policy]),
      mRes(chr), mpResultCache(pCacheBuf) {
    *(void**)this = (void*)lbl_eu_80569330;
}

// Layout of the allocation: [object][result cache][binding table]. The cache
// region only exists when `cache` is set, and its entry count comes from the
// chr resource's node count.
AnmObjChrRes* AnmObjChrRes::Construct(MEMAllocator* pAllocator, u32* pSize,
                                      ResAnmChr chr, ResMdl mdl, bool cache) {
    if (!chr.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    // Result cache exists only when requested; sized by the chr node count.
    u16 numNode = chr.ref().info.numNode;

    int numBinding = mdl.GetResNodeNumEntries();

    // Branchless "cache ? numNode : 0" select, matching retail's neg/or/srawi/and
    // nonzero-test idiom.
    u32 numCache = numNode & ((-cache | cache) >> 31);

    u32 resultOfs = numCache * sizeof(ChrAnmResult);
    u32 size = resultOfs + numBinding * sizeof(u16) + sizeof(AnmObjChrRes);

    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    void* pBuffer = MEMAllocFromAllocator(pAllocator, size);

    if (pBuffer == NULL) {
        return NULL;
    }

    char* pBase = static_cast<char*>(pBuffer);

    ChrAnmResult* pCacheBuf =
        cache ? reinterpret_cast<ChrAnmResult*>(pBase + sizeof(AnmObjChrRes))
              : NULL;

    char* pBindOfs = pBase + resultOfs;
    u16* pBindingBuf = reinterpret_cast<u16*>(pBindOfs + sizeof(AnmObjChrRes));

    AnmObjChrRes* pAnm = new (pBase)
        AnmObjChrRes(pAllocator, chr, pBindingBuf, numBinding, pCacheBuf);

    // Fill the freshly allocated cache: every defined binding gets its
    // ChrAnmResult evaluated once at the start frame.
    if (pAnm->mpResultCache != NULL) {
        f32 f = pAnm->GetFrm();
        for (u32 i = 0, ofs = 0; i < (u32)pAnm->mNumBinding; ofs += (u32)sizeof(u16), i++) {
            u16 binding = *(const u16*)(reinterpret_cast<const u8*>(pAnm->mpBinding) + ofs);
            if (!(binding & BINDING_UNDEFINED)) {
                u32 id = binding & BINDING_ID_MASK;
                pAnm->mRes.GetAnmResult(&pAnm->mpResultCache[id], id, f);
            }
        }
    }

    return pAnm;
}

// Refresh every cached ChrAnmResult from the current frame.
void AnmObjChrRes::UpdateCache() {
    if (mpResultCache != NULL) {
        f32 f = GetFrm();
        // Explicit byte-offset IV keeps MWCC from deriving its own temp,
        // which would steal the first callee-saved register.
        for (u32 i = 0, ofs = 0; i < (u32)mNumBinding; ofs += (u32)sizeof(u16), i++) {
            u16 binding = *(const u16*)(reinterpret_cast<const u8*>(mpBinding) + ofs);
            if (!(binding & BINDING_UNDEFINED)) {
                u32 id = binding & BINDING_ID_MASK;
                mRes.GetAnmResult(&mpResultCache[id], id, f);
            }
        }
    }
}

} // namespace g3d
} // namespace nw4r

// Aliases FrameCtrl::smBaseUpdateRate (sdata2).
extern const f32 lbl_eu_80663460;

namespace nw4r {
namespace g3d {

// Stand-in for FrameCtrl::UpdateFrm() kept as a real function so MWCC's
// early CSE cannot merge the mUpdateRate reloads across the call boundary;
// smBaseUpdateRate resolves to the shared sdata2 slot lbl_eu_80663460.
void AnmObjChrResUpdateFrm(FrameCtrl* fc) {
    fc->SetFrm(fc->GetRate() * lbl_eu_80663460 + fc->GetFrm());
}

// G3dProc: ATTACH stores the parent, UPDATEFRAME tail-dispatches the
// vtable slot, DETACH_PARENT clears the parent.
void AnmObjChrRes::G3dProc(u32 task, u32 param, void* pInfo) {
    switch ((int)task) {
    case G3DPROC_UPDATEFRAME:
        UpdateFrame();
        break;
    case G3DPROC_DETACH_PARENT:
        SetParent(NULL);
        break;
    case G3DPROC_ATTACH_PARENT:
        SetParent(static_cast<G3dObj*>(pInfo));
        break;
    default:
        break;
    }
}

void AnmObjChrRes::SetFrame(f32 frame) {
    SetFrm(frame);

    if (mpResultCache != NULL) {
        f32 f = GetFrm();
        // Explicit byte-offset IV keeps MWCC from deriving its own temp,
        // which would steal the first callee-saved register.
        for (u32 i = 0, ofs = 0; i < (u32)mNumBinding; ofs += (u32)sizeof(u16), i++) {
            u16 binding = *(const u16*)(reinterpret_cast<const u8*>(mpBinding) + ofs);
            if (!(binding & BINDING_UNDEFINED)) {
                u32 id = binding & BINDING_ID_MASK;
                mRes.GetAnmResult(&mpResultCache[id], id, f);
            }
        }
    }
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

void AnmObjChrRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    // Only refresh the cache when the rate returns to exactly 1.0f.
    if (lbl_eu_80669B88 == rate) {
        if (mpResultCache != NULL) {
            f32 f = GetFrm();
            // Explicit byte-offset IV keeps MWCC from deriving its own temp,
            // which would steal the first callee-saved register.
            for (u32 i = 0, ofs = 0; i < (u32)mNumBinding; ofs += (u32)sizeof(u16), i++) {
                u16 binding = *(const u16*)(reinterpret_cast<const u8*>(mpBinding) + ofs);
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



float GetUpdateRate__Q34nw4r3g3d12AnmObjChrResCFv(const nw4r::g3d::AnmObjChrRes* self) { return *(float *)((char *)self + 28); }

namespace nw4r {
namespace g3d {

void AnmObjChrRes::UpdateFrame() {
    if (lbl_eu_80669B88 != GetRate()) {
        UpdateFrm();

        if (mpResultCache != NULL) {
            f32 f = GetFrm();
            // Explicit byte-offset IV keeps MWCC from deriving its own temp,
            // which would steal the first callee-saved register.
            for (u32 i = 0, ofs = 0; i < (u32)mNumBinding; ofs += (u32)sizeof(u16), i++) {
                u16 binding = *(const u16*)(reinterpret_cast<const u8*>(mpBinding) + ofs);
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

bool AnmObjChrRes::Bind(ResMdl mdl) {
    int numNode = mRes.ref().info.numNode;
    bool bound = false;
    u16 i = 0;

    // Map chr-anm node i to the model node sharing its name; the model
    // node's id becomes our binding entry.
    if (numNode > 0) {
    for (; i < numNode; i++) {
        const ResDic dic = mRes.ofs_to_obj<ResDic>(mRes.ref().toChrDataDic);
        const ResAnmChrNodeData* pData = static_cast<const ResAnmChrNodeData*>(dic[i]);

        // Operand order in this add matters for byte-identity: retail emits
        // add r5, r0, r5 (offset term first).
        ResName name((char*)(pData->name + (s32)pData - sizeof(u32)));

        ResNode node = mdl.GetResNode(name);

        if (node.IsValid()) {
            mpBinding[node.GetID()] = i;
            bound = true;
        }
    }
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return bound;
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

// Mirrors the retail inline accessor shape (cf. ResAnmChr::GetNodeAnmIndex):
// evaluating the name argument before the inlined dictionary lookup reproduces
// retail's temp evaluation/home ordering.
inline s32 GetTexSrtMatAnmIndex(const ResAnmTexSrt& srt, const ResName name) {
    return srt.ofs_to_obj<ResDic>(srt.ref().toTexSrtDataDic).GetIndex(name);
}

} // namespace g3d
} // namespace nw4r

// Retail defines this as a member of AnmObjTexSrtRes, but g3d_anmtexsrt.h
// (outside our writable scope) declares only the 1-arg overload. Identifiers
// containing "__" are emitted verbatim by MWCC, so this free function defines
// the exact retail symbol with an ABI identical to the member (this, mdl,
// target, option).
bool Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption(
    nw4r::g3d::AnmObjTexSrtRes* pThis, const nw4r::g3d::ResMdl mdl, u32 target,
    nw4r::g3d::AnmObjChr::BindOption option) {
    using namespace nw4r::g3d;

    bool bound = false;

    switch (option) {
    case AnmObjChr::BIND_PARTIAL: {
        const ResMdl res = mdl;

        // Bind range extends up to (excluding) the depth-first successor of
        // node `target`; the successor is found by walking up the parent
        // chain until some ancestor has a next sibling.
        ResNode node = res.GetResNode(target);
        ResNode next = node.ofs_to_obj<ResNode>(node.ref().toNextSibling);

        while (!next.IsValid()) {
            node = node.ofs_to_obj<ResNode>(node.ref().toParentNode);

            if (!node.IsValid()) {
                break;
            }

            next = node.ofs_to_obj<ResNode>(node.ref().toNextSibling);
        }

        u32 num = next.IsValid() ? next.GetID() : res.GetResNodeNumEntries();

        // Byte-offset induction variables mirror retail's strength reduction
        // (separate halfword-offset counter kept alongside i).
        for (u32 i = target, ofs = target * sizeof(u16); i < num;
             ofs += sizeof(u16), i++) {
            ResNode node = mdl.GetResNode(i);
            const void* pRaw = node.GetResName().ptr();

            s32 idx = GetTexSrtMatAnmIndex(pThis->mRes, node.GetResName());

            if (idx != -1) {
                bound = true;
                *(u16*)(reinterpret_cast<u8*>(pThis->mpBinding) + ofs) =
                    (u16)idx;
            }
        }
        break;
    }

    case AnmObjChr::BIND_ONE: {
        ResNode node = mdl.GetResNode(target);
        const void* pRaw = node.GetResName().ptr();

        s32 idx = GetTexSrtMatAnmIndex(pThis->mRes, node.GetResName());

        if (idx != -1) {
            bound = true;
            *(u16*)(reinterpret_cast<u8*>(pThis->mpBinding) +
                    target * sizeof(u16)) = (u16)idx;
        }
        break;
    }
    }

    pThis->SetAnmFlag(AnmObj::FLAG_ANM_BOUND, true);
    return bound;
}

namespace nw4r {
namespace g3d {

// BIND_ONE clears the single entry `target`; BIND_PARTIAL clears every entry
// from `target` up to (excluding) the depth-first successor of node `target`
// in the model hierarchy. The successor is found by walking up the parent
// chain until some ancestor has a next sibling.
void AnmObjChrRes::Release(const ResMdl mdl, u32 target, BindOption option) {
    switch (option) {
    case AnmObjChr::BIND_PARTIAL: {
        const ResMdl res = mdl;

        ResNode node = res.GetResNode(target);
        ResNode next = node.ofs_to_obj<ResNode>(node.ref().toNextSibling);

        while (!next.IsValid()) {
            node = node.ofs_to_obj<ResNode>(node.ref().toParentNode);

            if (!node.IsValid()) {
                break;
            }

            next = node.ofs_to_obj<ResNode>(node.ref().toNextSibling);
        }

        u32 num = next.IsValid() ? next.GetID() : res.GetResNodeNumEntries();

        for (u32 i = target; i < num; i++) {
            mpBinding[i] = BINDING_UNDEFINED;
        }
        break;
    }

    case AnmObjChr::BIND_ONE:
        mpBinding[target] = BINDING_UNDEFINED;
        break;
    }
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

const ChrAnmResult* AnmObjChrRes::GetResult(ChrAnmResult* pResult,
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




void IsDerivedFrom__Q34nw4r3g3d12AnmObjChrResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d9AnmObjChrCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d12AnmObjChrResCFv(){}

extern const char lbl_eu_8051D5F8[];
void* GetTypeObj__Q34nw4r3g3d12AnmObjChrResCFv(void) { return (void*)lbl_eu_8051D5F8; }

void __dt__Q34nw4r3g3d12AnmObjChrResFv(){}


void IsDerivedFrom__Q34nw4r3g3d13AnmObjChrNodeCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d14AnmObjChrBlendCFv(){}

extern const char lbl_eu_8051D5E4[];
void* GetTypeObj__Q34nw4r3g3d14AnmObjChrBlendCFv(void) { return (void*)lbl_eu_8051D5E4; }


void GetTypeName__Q34nw4r3g3d13AnmObjChrNodeCFv(){}

extern const char lbl_eu_8051D5D0[];
extern "C" void* GetTypeObj__Q34nw4r3g3d13AnmObjChrNodeCFv(void) { return (void*)lbl_eu_8051D5D0; }

void GetTypeName__Q34nw4r3g3d9AnmObjChrCFv(){}

void* GetTypeObj__Q34nw4r3g3d9AnmObjChrCFv(void) { return (void*)lbl_eu_8051D5C0; }

namespace nw4r {
namespace g3d {

// Retail inlines the full base chain into this function; calling the base
// member reproduces the staggered register allocation of the inline expansion.
// The first comparison is written constant-first so MWCC colors `other` into
// r0 and the type-object address into r3, matching the inline expansion.
bool AnmObjChrBlend::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (TypeObj(lbl_eu_8051D5E4) == other) {
        return true;
    }
    return AnmObjChrNode::IsDerivedFrom(other);
}

// AnmObjChrBlend dtor: retail inlines the AnmObjChrNode base-dtor chain
// (vtable store, DetachAll, G3dObj base dtor) into the out-of-line copy of
// the header's inline empty dtor. No legal out-of-line redefinition exists
// while the header declares the body inline, so the mangled stub below
// keeps the symbol linkable. See the GetResult stub note for the one-line
// postprocess rule that would unblock byte-identical matching.
void __dt__Q34nw4r3g3d14AnmObjChrBlendFv(void) {}


} // namespace g3d
} // namespace nw4r

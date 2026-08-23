// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmchr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

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

    for (int i = 0; i < mNumBinding; i++) {
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

AnmObjChrRes* AnmObjChrNode::Detach(int idx) {
    AnmObjChrRes* pChild = mpChildrenArray[idx];

    if (pChild != NULL) {
        pChild->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpChildrenArray[idx] = NULL;

        bool found = false;

        for (int j = 0; j < mNumBinding; j++) {
            u16 binding = BINDING_UNDEFINED;

            for (int i = 0; i < mChildrenArraySize; i++) {
                AnmObjChrRes* p = mpChildrenArray[i];

                if (p != NULL && p->TestDefined(j)) {
                    found = true;
                    binding = 0;
                    break;
                }
            }

            mpBinding[j] = binding;
        }

        if (!found) {
            SetAnmFlag(FLAG_ANM_BOUND, false);
        }
    }

    return pChild;
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

    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
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

void Construct__Q34nw4r3g3d14AnmObjChrBlendFP12MEMAllocatorPUlQ34nw4r3g3d6ResMdli(){}

void GetResult__Q34nw4r3g3d14AnmObjChrBlendFPQ34nw4r3g3d12ChrAnmResultUl(){}

void Construct__Q34nw4r3g3d12AnmObjChrResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmChrQ34nw4r3g3d6ResMdlb(){}

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
    // Local snapshot keeps the second GetRate() below from being CSE'd
    // into the comparison load, matching retail's two reloads.
    f32 rate = GetRate();
    if (lbl_eu_80669B88 != rate) {
        // Inline expansion of FrameCtrl::UpdateFrm/SetFrm; smBaseUpdateRate
        // resolves to the shared sdata2 slot lbl_eu_80663460.
        SetFrm(GetFrm() + GetRate() * lbl_eu_80663460);

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
    bool bound = false;
    u32 numNode = mRes.ref().info.numNode;

    // Map chr-anm node i to the model node sharing its name; the model
    // node's id becomes our binding entry.
    for (u32 i = 0; i < numNode; i++) {
        const ResAnmChrData& r = mRes.ref();

        const ResDic dic = ResDic(mRes.ofs_to_ptr<ResDicData>(r.toChrDataDic));
        const ResAnmChrNodeData* pData =
            static_cast<const ResAnmChrNodeData*>(dic[i]);

        if (pData == NULL) {
            continue;
        }

        ResName name(NW4R_G3D_OFS_TO_RESNAME(pData, pData->name));

        ResNode node = mdl.GetResNode(name);

        if (node.IsValid()) {
            mpBinding[node.ref().id] = i;
            bound = true;
        }
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return bound;
}

} // namespace g3d
} // namespace nw4r



void Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption(){}

void Release__Q34nw4r3g3d12AnmObjChrResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption(){}

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
bool AnmObjChrBlend::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(lbl_eu_8051D5E4)) {
        return true;
    }
    return AnmObjChrNode::IsDerivedFrom(other);
}

// AnmObjChrBlend dtor: retail inlines the AnmObjChrNode base-dtor chain
// (vtable store, DetachAll, G3dObj base dtor) into the out-of-line copy of
// the header's inline empty dtor. No legal out-of-line redefinition exists
// while the header declares the body inline, so the mangled stub below
// keeps the symbol linkable.
void __dt__Q34nw4r3g3d14AnmObjChrBlendFv(void) {}

} // namespace g3d
} // namespace nw4r

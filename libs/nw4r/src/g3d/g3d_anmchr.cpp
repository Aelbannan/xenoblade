// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmchr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObj")> lbl_eu_8051D650;
extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("G3dObj")> lbl_eu_8051D640;

namespace nw4r {
namespace g3d {

f32 AnmObjChrRes::GetFrame() const {
    return GetFrm();
}

void AnmObjChrBlend::SetWeight(int idx, f32 weight) {
    mpWeightArray[idx] = weight;
}

f32 AnmObjChrBlend::GetWeight(int idx) const {
    return mpWeightArray[idx];
}

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChr")> lbl_eu_8051D5C0 = {sizeof("AnmObjChr"), "AnmObjChr"};

bool AnmObjChr::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D5C0) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjChr::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5C0);
}

const char* AnmObjChr::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChrNode")> lbl_eu_8051D5D0 = {sizeof("AnmObjChrNode"), "AnmObjChrNode"};

bool AnmObjChrNode::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D5D0) ? true
         : other == TypeObj(lbl_eu_8051D5C0) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjChrNode::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5D0);
}

const char* AnmObjChrNode::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChrBlend")> lbl_eu_8051D5E4 = {sizeof("AnmObjChrBlend"), "AnmObjChrBlend"};

bool AnmObjChrBlend::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D5E4) ? true
         : other == TypeObj(lbl_eu_8051D5D0) ? true
         : other == TypeObj(lbl_eu_8051D5C0) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjChrBlend::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D5E4);
}

const char* AnmObjChrBlend::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjChrRes")> lbl_eu_8051D5F8 = {sizeof("AnmObjChrRes"), "AnmObjChrRes"};

bool AnmObjChrRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D5F8) ? true
         : other == TypeObj(lbl_eu_8051D5C0) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
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

extern float lbl_eu_80669B88;
float GetWeight__Q34nw4r3g3d9AnmObjChrCFi() { return lbl_eu_80669B88; }

void DetachAll__Q34nw4r3g3d9AnmObjChrFv(void) {}


namespace nw4r {
namespace g3d {

AnmObjChrNode::~AnmObjChrNode() {
    if (this != NULL) {
        DetachAll();
    }
}

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

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

// Shared sdata2 0.0f / 1.0f slots used by the node-loop accessor defaults.
extern "C" const float lbl_eu_80669B88; // 0.0f
extern "C" const float lbl_eu_80669B8C; // 1.0f

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

void Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdl(){}

void Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption(){}

void Release__Q34nw4r3g3d13AnmObjChrNodeFv(){}

void Release__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption(){}

void G3dProc__Q34nw4r3g3d13AnmObjChrNodeFUlUlPv(){}

void Construct__Q34nw4r3g3d14AnmObjChrBlendFP12MEMAllocatorPUlQ34nw4r3g3d6ResMdli(){}

void GetResult__Q34nw4r3g3d14AnmObjChrBlendFPQ34nw4r3g3d12ChrAnmResultUl(){}

void Construct__Q34nw4r3g3d12AnmObjChrResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmChrQ34nw4r3g3d6ResMdlb(){}

namespace nw4r {
namespace g3d {

void AnmObjChrRes::SetFrame(f32 frame) {
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

} // namespace g3d
} // namespace nw4r



namespace nw4r {
namespace g3d {

void AnmObjChrRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    if (rate == 1.0f) {
        if (mpResultCache != NULL) {
            f32 f = GetFrm();
            u32 i = 0;
            while (i < (u32)mNumBinding) {
                u16 binding = mpBinding[i];
                if (!(binding & BINDING_UNDEFINED)) {
                    u32 id = binding & BINDING_ID_MASK;
                    mRes.GetAnmResult(&mpResultCache[id], id, f);
                }
                i++;
            }
        }
    }
}

} // namespace g3d
} // namespace nw4r



float GetUpdateRate__Q34nw4r3g3d12AnmObjChrResCFv(void *self) { return *(float *)((char *)self + 28); }

namespace nw4r {
namespace g3d {

void AnmObjChrRes::UpdateFrame() {
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

bool AnmObjChrRes::Bind(ResMdl mdl) {
    u16 numNode = mRes.ref().info.numNode;
    bool bound = false;

    for (u16 i = 0; i < numNode; i++) {
        ResDic dic = ResDic(mRes.ofs_to_ptr<ResDicData>(mRes.ref().toChrDataDic));
        const ResAnmChrNodeData* pData =
            reinterpret_cast<const ResAnmChrNodeData*>(dic[i]);
        const char* pName =
            reinterpret_cast<const char*>(pData) + pData->name;

        ResNode node = mdl.GetResNode(ResName(
            reinterpret_cast<const ResNameData*>(
                reinterpret_cast<const char*>(pName) - 4)));

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

void GetResult__Q34nw4r3g3d12AnmObjChrResFPQ34nw4r3g3d12ChrAnmResultUl(){}

void G3dProc__Q34nw4r3g3d12AnmObjChrResFUlUlPv(){}

void IsDerivedFrom__Q34nw4r3g3d12AnmObjChrResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d9AnmObjChrCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d12AnmObjChrResCFv(){}

extern const char lbl_eu_8051D5F8[];
void* GetTypeObj__Q34nw4r3g3d12AnmObjChrResCFv(void) { return (void*)lbl_eu_8051D5F8; }

void __dt__Q34nw4r3g3d12AnmObjChrResFv(){}

void IsDerivedFrom__Q34nw4r3g3d14AnmObjChrBlendCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d13AnmObjChrNodeCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d14AnmObjChrBlendCFv(){}

extern const char lbl_eu_8051D5E4[];
void* GetTypeObj__Q34nw4r3g3d14AnmObjChrBlendCFv(void) { return (void*)lbl_eu_8051D5E4; }

void __dt__Q34nw4r3g3d14AnmObjChrBlendFv(){}

void GetTypeName__Q34nw4r3g3d13AnmObjChrNodeCFv(){}

extern const char lbl_eu_8051D5D0[];
extern "C" void* GetTypeObj__Q34nw4r3g3d13AnmObjChrNodeCFv(void) { return (void*)lbl_eu_8051D5D0; }

void GetTypeName__Q34nw4r3g3d9AnmObjChrCFv(){}

extern "C" const char lbl_eu_8051D5C0[];
extern "C" void* GetTypeObj__Q34nw4r3g3d9AnmObjChrCFv(void) { return (void*)lbl_eu_8051D5C0; }

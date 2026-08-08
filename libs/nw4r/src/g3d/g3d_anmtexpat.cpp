// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmtexpat
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_anmtexpat.h>

namespace nw4r {
namespace g3d {

bool AnmObjTexPat::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_INVALID | BINDING_UNDEFINED)) == 0;
}

f32 AnmObjTexPatRes::GetFrame() const {
    return GetFrm();
}

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjTexPat")> lbl_eu_8051D560 = {sizeof("AnmObjTexPat"), "AnmObjTexPat"};

bool AnmObjTexPat::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D560) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjTexPat::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D560);
}

const char* AnmObjTexPat::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjTexPatRes")> lbl_eu_8051D578 = {sizeof("AnmObjTexPatRes"), "AnmObjTexPatRes"};

bool AnmObjTexPatRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D578) ? true
         : other == TypeObj(lbl_eu_8051D560) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjTexPatRes::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D578);
}

const char* AnmObjTexPatRes::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmObjTexPat::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

} // namespace g3d
} // namespace nw4r



int Attach__Q34nw4r3g3d12AnmObjTexPatFiPQ34nw4r3g3d15AnmObjTexPatRes(void) { return 0x0; }

int Detach__Q34nw4r3g3d12AnmObjTexPatFi(void) { return 0x0; }

void DetachAll__Q34nw4r3g3d12AnmObjTexPatFv(void) {}



void Construct__Q34nw4r3g3d15AnmObjTexPatResFP12MEMAllocatorPUlQ34nw4r3g3d12ResAnmTexPatQ34nw4r3g3d6ResMdlb(){}

namespace nw4r {
namespace g3d {

void AnmObjTexPatRes::SetFrame(f32 frame) {
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

void AnmObjTexPatRes::SetUpdateRate(f32 rate) {
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

void AnmObjTexPatRes::UpdateFrame() {
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

bool AnmObjTexPatRes::Bind(ResMdl mdl) {
    u16 numMat = mRes.ref().info.numMaterial;
    bool bound = false;

    for (u16 i = 0; i < numMat; i++) {
        ResDic dic = ResDic(mRes.ofs_to_ptr<ResDicData>(mRes.ref().toTexPatDataDic));
        const ResAnmTexPatMatData* pData =
            reinterpret_cast<const ResAnmTexPatMatData*>(dic[i]);
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

const TexPatAnmResult* AnmObjTexPatRes::GetResult(TexPatAnmResult* pResult,
                                                  u32 idx) {
    u16 binding = mpBinding[idx];

    if (binding & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->bTexExist = 0;
        pResult->bPlttExist = 0;
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

void AnmObjTexPatRes::G3dProc(u32 task, u32 arg, void* pArg) {
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



void ApplyTexPatAnmResult__Q24nw4r3g3dFQ34nw4r3g3d9ResTexObjQ34nw4r3g3d10ResTlutObjPCQ34nw4r3g3d15TexPatAnmResult(){}

void IsDerivedFrom__Q34nw4r3g3d15AnmObjTexPatResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d12AnmObjTexPatCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d15AnmObjTexPatResCFv(){}

extern const char lbl_eu_8051D578[];
void* GetTypeObj__Q34nw4r3g3d15AnmObjTexPatResCFv(void) { return (void*)lbl_eu_8051D578; }

void __dt__Q34nw4r3g3d15AnmObjTexPatResFv(){}

void GetTypeName__Q34nw4r3g3d12AnmObjTexPatCFv(){}

extern "C" void* GetTypeObj__Q34nw4r3g3d12AnmObjTexPatCFv(void) { return (void*)lbl_eu_8051D560; }

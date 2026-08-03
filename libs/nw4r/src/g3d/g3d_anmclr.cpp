// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmclr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObj")> lbl_eu_8051D650;
extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("G3dObj")> lbl_eu_8051D640;

namespace nw4r {
namespace g3d {

bool AnmObjMatClr::TestExistence(u32 idx) const {
    return (mpBinding[idx] & (BINDING_INVALID | BINDING_UNDEFINED)) == 0;
}

f32 AnmObjMatClrRes::GetFrame() const {
    return GetFrm();
}

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjMatClr")> lbl_eu_8051D530 = {sizeof("AnmObjMatClr"), "AnmObjMatClr"};

bool AnmObjMatClr::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D530) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjMatClr::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D530);
}

const char* AnmObjMatClr::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjMatClrRes")> lbl_eu_8051D548 = {sizeof("AnmObjMatClrRes"), "AnmObjMatClrRes"};

bool AnmObjMatClrRes::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D548) ? true
         : other == TypeObj(lbl_eu_8051D530) ? true
         : other == TypeObj(lbl_eu_8051D650) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj AnmObjMatClrRes::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D548);
}

const char* AnmObjMatClrRes::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
namespace ut {

Color Color::operator&(u32 color) const {
    return Color(ToU32() & color);
}

Color Color::operator|(u32 color) const {
    return Color(ToU32() | color);
}

} // namespace ut
} // namespace nw4r

namespace nw4r {
namespace g3d {

void AnmObjMatClr::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

} // namespace g3d
} // namespace nw4r



int Attach__Q34nw4r3g3d12AnmObjMatClrFiPQ34nw4r3g3d15AnmObjMatClrRes(void) { return 0x0; }

int Detach__Q34nw4r3g3d12AnmObjMatClrFi(void) { return 0x0; }

void DetachAll__Q34nw4r3g3d12AnmObjMatClrFv(void) {}



void Construct__Q34nw4r3g3d15AnmObjMatClrResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmClrQ34nw4r3g3d6ResMdlb(){}

namespace nw4r {
namespace g3d {

void AnmObjMatClrRes::SetFrame(f32 frame) {
    SetFrm(frame);

    if (mpResultCache != NULL) {
        f32 f = GetFrm();
        for (u32 i = 0; i < mNumBinding; i++) {
            u16 binding = mpBinding[i];
            if (!(binding & BINDING_UNDEFINED)) {
                u32 id = binding & BINDING_ID_MASK;
                mRes.GetAnmResult(&mpResultCache[id], id, f);
            }
        }
    }
}

void AnmObjMatClrRes::SetUpdateRate(f32 rate) {
    SetRate(rate);

    if (rate == 1.0f) {
        if (mpResultCache != NULL) {
            f32 f = GetFrm();
            for (u32 i = 0; i < mNumBinding; i++) {
                u16 binding = mpBinding[i];
                if (!(binding & BINDING_UNDEFINED)) {
                    u32 id = binding & BINDING_ID_MASK;
                    mRes.GetAnmResult(&mpResultCache[id], id, f);
                }
            }
        }
    }
}

void AnmObjMatClrRes::UpdateFrame() {
    if (GetRate() != 1.0f) {
        UpdateFrm();

        if (mpResultCache != NULL) {
            f32 f = GetFrm();
            for (u32 i = 0; i < mNumBinding; i++) {
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



void SetUpdateRate__Q34nw4r3g3d15AnmObjMatClrResFf(){}

float GetUpdateRate__Q34nw4r3g3d15AnmObjMatClrResCFv(void *self) { return *(float *)((char *)self + 28); }

void UpdateFrame__Q34nw4r3g3d15AnmObjMatClrResFv(){}

void Bind__Q34nw4r3g3d15AnmObjMatClrResFQ34nw4r3g3d6ResMdl(){}

namespace nw4r {
namespace g3d {

const ClrAnmResult* AnmObjMatClrRes::GetResult(ClrAnmResult* pResult,
                                               u32 idx) {
    u16 binding = mpBinding[idx];

    if (binding & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->bRgbaExist = 0;
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



void G3dProc__Q34nw4r3g3d15AnmObjMatClrResFUlUlPv(){}

void ApplyClrAnmResult__Q24nw4r3g3dFQ34nw4r3g3d10ResMatChanQ34nw4r3g3d14ResMatTevColorPCQ34nw4r3g3d12ClrAnmResult(){}

void IsDerivedFrom__Q34nw4r3g3d15AnmObjMatClrResCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void IsDerivedFrom__Q34nw4r3g3d12AnmObjMatClrCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d15AnmObjMatClrResCFv(){}

extern const char lbl_eu_8051D548[];
void* GetTypeObj__Q34nw4r3g3d15AnmObjMatClrResCFv(void) { return (void*)lbl_eu_8051D548; }

void __dt__Q34nw4r3g3d15AnmObjMatClrResFv(){}

void GetTypeName__Q34nw4r3g3d12AnmObjMatClrCFv(){}

extern "C" const char lbl_eu_8051D530[];
extern "C" void* GetTypeObj__Q34nw4r3g3d12AnmObjMatClrCFv(void) { return (void*)lbl_eu_8051D530; }

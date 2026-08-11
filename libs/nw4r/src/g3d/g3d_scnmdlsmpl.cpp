// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlsmpl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

void ScnMdlSmpl_CalcPosture__Q34nw4r3g3d12ScnMdlSimpleFUlPCQ34nw4r4math5MTX34(){}


void ScnMdlSmpl_G3DPROC_CALC_VIEW__Q34nw4r3g3d12ScnMdlSimpleFUlPCQ34nw4r4math5MTX34(){}

void ExecCallback_DRAW_OPA__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}

void ExecCallback_DRAW_XLU__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}

void G3dProc__Q34nw4r3g3d12ScnMdlSimpleFUlUlPv(){}


void GetScnMtxPos__Q34nw4r3g3d12ScnMdlSimpleCFPQ34nw4r4math5MTX34Q44nw4r3g3d6ScnObj13ScnObjMtxTypeUl(){}

void SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObj(){}

void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

namespace nw4r {
namespace g3d {

AnmObj* ScnMdlSimple::GetAnmObj(AnmObjType type) {
    switch (type) {
    case ANMOBJTYPE_CHR: return mpAnmObjChr;
    case ANMOBJTYPE_VIS: return mpAnmObjVis;
    case ANMOBJTYPE_MATCLR: return mpAnmObjMatClr;
    case ANMOBJTYPE_TEXPAT: return mpAnmObjTexPat;
    case ANMOBJTYPE_TEXSRT: return mpAnmObjTexSrt;
    case ANMOBJTYPE_SHP:
    case ANMOBJTYPE_NOT_SPECIFIED:
    default: return NULL;
    }
}

const AnmObj* ScnMdlSimple::GetAnmObj(AnmObjType type) const {
    switch (type) {
    case ANMOBJTYPE_CHR: return mpAnmObjChr;
    case ANMOBJTYPE_VIS: return mpAnmObjVis;
    case ANMOBJTYPE_MATCLR: return mpAnmObjMatClr;
    case ANMOBJTYPE_TEXPAT: return mpAnmObjTexPat;
    case ANMOBJTYPE_TEXSRT: return mpAnmObjTexSrt;
    case ANMOBJTYPE_SHP:
    case ANMOBJTYPE_NOT_SPECIFIED:
    default: return NULL;
    }
}

math::MTX34* ScnMdlSimple::GetViewPosMtxArray() {
    u32 aligned = (mNumViewMtx * 0x30 + 0x1f) & ~0x1f;
    return reinterpret_cast<math::MTX34*>(reinterpret_cast<u8*>(mpViewPosMtxArray) +
                                          mCurView * aligned);
}

math::MTX34* ScnMdlSimple::GetViewTexMtxArray() {
    if (mpViewTexMtxArray != NULL) {
        u32 aligned = (mNumViewMtx * 0x30 + 0x1f) & ~0x1f;
        return reinterpret_cast<math::MTX34*>(
            reinterpret_cast<u8*>(mpViewTexMtxArray) + mCurView * aligned);
    }
    return NULL;
}

math::MTX33* ScnMdlSimple::GetViewNrmMtxArray() {
    if (mpViewNrmMtxArray != NULL) {
        u32 aligned = (mNumViewMtx * 0x24 + 0x1f) & ~0x1f;
        return reinterpret_cast<math::MTX33*>(
            reinterpret_cast<u8*>(mpViewNrmMtxArray) + mCurView * aligned);
    }
    return NULL;
}

void ScnMdlSimple::EnableScnMdlCallbackTiming(Timing timing) {
    if (timing & ScnObj::CALLBACK_TIMING_A) {
        mCwcbTiming |= CALLBACK_TIMING_A;
    }
    if (timing & ScnObj::CALLBACK_TIMING_B) {
        mCwcbTiming |= CALLBACK_TIMING_B;
    }
    if (timing & ScnObj::CALLBACK_TIMING_C) {
        mCwcbTiming |= CALLBACK_TIMING_C;
    }
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(
    u32, IScnObjGather* pCollection) {
    // Add self to the collection. A set NOT_GATHER_* flag excludes this
    // object from the matching draw pass.
    pCollection->Add(this, !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_OPA),
                     !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_XLU));
}

bool ScnMdlSimple::SetScnObjOption(u32 option, u32 value) {
    switch (option) {
    case OPTID_IGNORE_ANMCHR_TRANS:
        SetScnObjFlag(SCNOBJFLAG_IGNORE_ANMCHR_TRANS, value);
        break;
    default:
        return ScnLeaf::SetScnObjOption(option, value);
    }
    return true;
}

bool ScnMdlSimple::GetScnObjOption(u32 option, u32* pValue) const {
    if (pValue == NULL) {
        return false;
    }

    switch (option) {
    case OPTID_IGNORE_ANMCHR_TRANS:
        *pValue = TestScnObjFlag(SCNOBJFLAG_IGNORE_ANMCHR_TRANS);
        break;
    default:
        return ScnLeaf::GetScnObjOption(option, pValue);
    }
    return true;
}

void ScnMdlSimple::UpdateFrame() {
    if (mpAnmObjChr != NULL) {
        mpAnmObjChr->UpdateFrame();
    }
    if (mpAnmObjVis != NULL) {
        mpAnmObjVis->UpdateFrame();
    }
    if (mpAnmObjMatClr != NULL) {
        mpAnmObjMatClr->UpdateFrame();
    }
    if (mpAnmObjTexPat != NULL) {
        mpAnmObjTexPat->UpdateFrame();
    }
    if (mpAnmObjTexSrt != NULL) {
        mpAnmObjTexSrt->UpdateFrame();
    }
}

ICalcWorldCallback::~ICalcWorldCallback() {}

} // namespace g3d
} // namespace nw4r




void __ct__Q34nw4r3g3d12ScnMdlSimpleFP12MEMAllocatorQ34nw4r3g3d6ResMdlPQ34nw4r4math5MTX34PUlPQ34nw4r4math5MTX34PQ34nw4r4math5MTX33PQ34nw4r4math5MTX34ii(){}

void __dt__Q34nw4r3g3d12ScnMdlSimpleFv(){}


void IsDerivedFrom__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d12ScnMdlSimpleCFv(){}

extern "C" void* GetTypeObj__Q34nw4r3g3d12ScnMdlSimpleCFv(void) { return (void*)&lbl_eu_8051D7A8; }

namespace nw4r {
namespace g3d {

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("ScnMdlSimple")> lbl_eu_8051D7A8 = {sizeof("ScnMdlSimple"), "ScnMdlSimple"};

bool ScnMdlSimple::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D7A8) ? true
         : other == TypeObj(lbl_eu_8051D778) ? true
         : other == TypeObj(lbl_eu_8051D768) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj ScnMdlSimple::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D7A8);
}

const char* ScnMdlSimple::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r

// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlsmpl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/ut.h>

// AnmObjVis's TypeName data symbol is not declared in any header; declare it
// here (global-scope variable names are not mangled).
extern const char lbl_eu_8051D520[];

void ScnMdlSmpl_CalcPosture__Q34nw4r3g3d12ScnMdlSimpleFUlPCQ34nw4r4math5MTX34(){}


void ExecCallback_DRAW_OPA__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}

void ExecCallback_DRAW_XLU__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}


void GetScnMtxPos__Q34nw4r3g3d12ScnMdlSimpleCFPQ34nw4r4math5MTX34Q44nw4r3g3d6ScnObj13ScnObjMtxTypeUl(){}

void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObj(){}

void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

namespace nw4r {
namespace g3d {
namespace {

// The animation TypeName data symbols are declared in the public headers as
// raw char arrays, but TypeObj's constructor binds a typed ResNameDataT view.
// Rebind the same address through the typed reference; only the address is
// stored, so the view width is irrelevant. Always inlined by MWCC.
inline G3dObj::TypeObj MakeTypeObj(const char* pTypeName) {
    return G3dObj::TypeObj(
        *reinterpret_cast<const G3dObj::ResNameDataT<32>*>(pTypeName));
}

} // namespace

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

void ScnMdlSimple::ScnMdlSmpl_CalcPosture(u32 param, const math::MTX34* pParent) {
    CheckCallback_CALC_WORLD(CALLBACK_TIMING_A, param,
                             const_cast<void*>(static_cast<const void*>(pParent)));

    CalcWorldMtx(pParent, &param);

    CheckCallback_CALC_WORLD(CALLBACK_TIMING_B, param,
                             const_cast<void*>(static_cast<const void*>(pParent)));

    u32 posNrmMtxNum = mResMdl.GetResMdlInfo().GetNumPosNrmMtx();

    // If the model matrix array fits in the locked cache (LC), DMA the
    // results through the cache base address instead of main memory.
    u32 lock = 0;
    math::MTX34* pWorldMtx;
    if (posNrmMtxNum > MTX_CACHE_MIN && posNrmMtxNum < MTX_CACHE_MAX) {
        lock = ut::LC::Lock();
        if (lock != 1) {
            goto no_lc; // lock failed: fall back to main memory
        }
        mFlagScnMdlSimple |= SCNMDLSMPLFLAG_LC_DMA;
        DCInvalidateRange(mpWorldMtxArray,
                          posNrmMtxNum * sizeof(math::MTX34));
        pWorldMtx = reinterpret_cast<math::MTX34*>(OS_CACHE_BASE);
        goto lc_done;
    }
no_lc:
    mFlagScnMdlSimple &= ~SCNMDLSMPLFLAG_LC_DMA;
    pWorldMtx = mpWorldMtxArray;
lc_done:

    ScaleProperty scale = GetScaleProperty();

    // Root node attribute for CalcWorld: encodes the root scaling mode and
    // whether the animated transform is ignored.
    u32 rootAttrib = 0xF0000000;
    u32 ignoreAnmChrTrans;
    GetScnObjOption(OPTID_IGNORE_ANMCHR_TRANS, &ignoreAnmChrTrans);
    if (scale == UNIFORM_SCALED) {
        rootAttrib = 0x30000000;
    } else if (scale == NONUNIFORM_SCALED) {
        rootAttrib = 0;
    }
    if (ignoreAnmChrTrans != 0) {
        rootAttrib |= 0x08000000;
    }

    if (mpCalcWorldCallback != NULL) {
        FuncObjCalcWorld funcObj(mpCalcWorldCallback, mCwcbTiming, mCwcbNodeID);
        AnmObjChr* pAnmChr = mpAnmObjChr;
        CalcWorld(pWorldMtx, mpWorldMtxAttribArray, mpByteCodeCalc,
                  &mMtxArray[MTX_WORLD], mResMdl, pAnmChr, &funcObj,
                  rootAttrib);
    } else {
        AnmObjChr* pAnmChr = mpAnmObjChr;
        CalcWorld(pWorldMtx, mpWorldMtxAttribArray, mpByteCodeCalc,
                  &mMtxArray[MTX_WORLD], mResMdl, pAnmChr, NULL, rootAttrib);
    }

    const u8* pByteCodeMix = mpByteCodeMix;
    if (pByteCodeMix != NULL) {
        CalcSkinning(pWorldMtx, mpWorldMtxAttribArray, mResMdl, pByteCodeMix);
    }

    if (lock != 0) {
        ut::LC::StoreData(mpWorldMtxArray,
                          reinterpret_cast<void*>(OS_CACHE_BASE),
                          posNrmMtxNum * sizeof(math::MTX34));
        ut::LC::Unlock();
    }
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_CALC_VIEW(u32 param,
                                                const math::MTX34* pCamera) {
    mCurView = (mCurView + 1) % mNumView;

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_A, param,
                            const_cast<void*>(static_cast<const void*>(pCamera)));
    CalcViewMtx(pCamera);
    CheckCallback_CALC_VIEW(CALLBACK_TIMING_B, param,
                            const_cast<void*>(static_cast<const void*>(pCamera)));

    if (ut::LC::Lock() != 0) {
        if (mFlagScnMdlSimple & SCNMDLSMPLFLAG_LC_DMA) {
            DCStoreRange(mpWorldMtxArray, mNumViewMtx * sizeof(math::MTX34));

            CalcView_LC_DMA_ModelMtx(GetViewPosMtxArray(), GetViewNrmMtxArray(),
                                     mpWorldMtxArray, mpWorldMtxAttribArray,
                                     mNumViewMtx, pCamera, mResMdl,
                                     GetViewTexMtxArray());
        } else {
            CalcView_LC(GetViewPosMtxArray(), GetViewNrmMtxArray(),
                        mpWorldMtxArray, mpWorldMtxAttribArray, mNumViewMtx,
                        pCamera, mResMdl, GetViewTexMtxArray());
        }
        ut::LC::Unlock();
    } else {
        CalcView(GetViewPosMtxArray(), GetViewNrmMtxArray(), mpWorldMtxArray,
                 mpWorldMtxAttribArray, mNumViewMtx, pCamera, mResMdl,
                 GetViewTexMtxArray());
    }

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_C, param,
                            const_cast<void*>(static_cast<const void*>(pCamera)));
}

bool ScnMdlSimple::SetAnmObj(AnmObj* pObj, AnmObjType type) {
    if (pObj == NULL || pObj->GetParent() != NULL) {
        return false;
    }

    switch (type) {
    case ANMOBJTYPE_CHR: {
        AnmObj* pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D5C0))) {
            pAnmObj = pObj;
        }
        if (pAnmObj == NULL) {
            return false;
        }
        if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
            return false;
        }
        if (mpAnmObjChr != NULL) {
            RemoveAnmObj(mpAnmObjChr);
        }
        mpAnmObjChr = static_cast<AnmObjChr*>(pAnmObj);
        pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
        return true;
    }
    case ANMOBJTYPE_VIS: {
        AnmObj* pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D520))) {
            pAnmObj = pObj;
        }
        if (pAnmObj == NULL) {
            return false;
        }
        if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
            return false;
        }
        if (mpAnmObjVis != NULL) {
            RemoveAnmObj(mpAnmObjVis);
        }
        mpAnmObjVis = static_cast<AnmObjVis*>(pAnmObj);
        pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
        return true;
    }
    case ANMOBJTYPE_MATCLR: {
        AnmObj* pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D530))) {
            pAnmObj = pObj;
        }
        if (pAnmObj == NULL) {
            return false;
        }
        if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
            return false;
        }
        if (mpAnmObjMatClr != NULL) {
            RemoveAnmObj(mpAnmObjMatClr);
        }
        mpAnmObjMatClr = static_cast<AnmObjMatClr*>(pAnmObj);
        pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
        return true;
    }
    case ANMOBJTYPE_TEXPAT: {
        AnmObj* pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D560))) {
            pAnmObj = pObj;
        }
        if (pAnmObj == NULL) {
            return false;
        }
        if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
            return false;
        }
        if (mpAnmObjTexPat != NULL) {
            RemoveAnmObj(mpAnmObjTexPat);
        }
        mpAnmObjTexPat = static_cast<AnmObjTexPat*>(pAnmObj);
        pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
        return true;
    }
    case ANMOBJTYPE_TEXSRT: {
        AnmObj* pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D590))) {
            pAnmObj = pObj;
        }
        if (pAnmObj == NULL) {
            return false;
        }
        if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
            return false;
        }
        if (mpAnmObjTexSrt != NULL) {
            RemoveAnmObj(mpAnmObjTexSrt);
        }
        mpAnmObjTexSrt = static_cast<AnmObjTexSrt*>(pAnmObj);
        pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
        return true;
    }
    case ANMOBJTYPE_SHP: {
        // Catch-all type: accept the first concrete animation type the object
        // derives from.
        AnmObj* pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D5C0))) {
            pAnmObj = pObj;
        }
        if (pAnmObj != NULL) {
            if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
                return false;
            }
            if (mpAnmObjChr != NULL) {
                RemoveAnmObj(mpAnmObjChr);
            }
            mpAnmObjChr = static_cast<AnmObjChr*>(pAnmObj);
            pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
            return true;
        }
        pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D520))) {
            pAnmObj = pObj;
        }
        if (pAnmObj != NULL) {
            if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
                return false;
            }
            if (mpAnmObjVis != NULL) {
                RemoveAnmObj(mpAnmObjVis);
            }
            mpAnmObjVis = static_cast<AnmObjVis*>(pAnmObj);
            pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
            return true;
        }
        pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D530))) {
            pAnmObj = pObj;
        }
        if (pAnmObj != NULL) {
            if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
                return false;
            }
            if (mpAnmObjMatClr != NULL) {
                RemoveAnmObj(mpAnmObjMatClr);
            }
            mpAnmObjMatClr = static_cast<AnmObjMatClr*>(pAnmObj);
            pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
            return true;
        }
        pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D560))) {
            pAnmObj = pObj;
        }
        if (pAnmObj != NULL) {
            if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
                return false;
            }
            if (mpAnmObjTexPat != NULL) {
                RemoveAnmObj(mpAnmObjTexPat);
            }
            mpAnmObjTexPat = static_cast<AnmObjTexPat*>(pAnmObj);
            pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
            return true;
        }
        pAnmObj = NULL;
        if (pObj != NULL && pObj->IsDerivedFrom(MakeTypeObj(lbl_eu_8051D590))) {
            pAnmObj = pObj;
        }
        if (pAnmObj != NULL) {
            if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
                return false;
            }
            if (mpAnmObjTexSrt != NULL) {
                RemoveAnmObj(mpAnmObjTexSrt);
            }
            mpAnmObjTexSrt = static_cast<AnmObjTexSrt*>(pAnmObj);
            pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
            return true;
        }
        break;
    }
    default:
        break;
    }

    return false;
}

void ScnMdlSimple::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3DPROC_CALC_WORLD:
        ScnMdlSmpl_CalcPosture(param, static_cast<const math::MTX34*>(pInfo));
        if (mpAnmObjVis != NULL) {
            ApplyVisAnmResult(mResMdl, mpAnmObjVis);
        }
        CheckCallback_CALC_WORLD(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_CALC_MAT:
        CheckCallback_CALC_MAT(CALLBACK_TIMING_A, param, pInfo);
        if (mpAnmObjTexPat != NULL || mpAnmObjTexSrt != NULL ||
            mpAnmObjMatClr != NULL) {
            CalcMaterialDirectly(mResMdl, mpAnmObjTexPat, mpAnmObjTexSrt,
                                 mpAnmObjMatClr);
        }
        CheckCallback_CALC_MAT(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_CALC_VIEW:
        ScnMdlSmpl_G3DPROC_CALC_VIEW(param,
                                     static_cast<const math::MTX34*>(pInfo));
        break;

    case G3DPROC_GATHER_SCNOBJ:
        ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(param,
                                         static_cast<IScnObjGather*>(pInfo));
        break;

    case G3DPROC_DRAW_OPA:
        CheckCallback_DRAW_OPA(CALLBACK_TIMING_A, param, pInfo);
        DrawResMdlDirectly(
            mResMdl, GetViewPosMtxArray(), GetViewNrmMtxArray(),
            GetViewTexMtxArray(), mpByteCodeDrawOpa, NULL, NULL,
            (pInfo != NULL) ? *static_cast<const u32*>(pInfo) : mDrawMode);
        CheckCallback_DRAW_OPA(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_DRAW_XLU:
        CheckCallback_DRAW_XLU(CALLBACK_TIMING_A, param, pInfo);
        DrawResMdlDirectly(
            mResMdl, GetViewPosMtxArray(), GetViewNrmMtxArray(),
            GetViewTexMtxArray(), mpByteCodeDrawXlu, NULL, NULL,
            (pInfo != NULL) ? *static_cast<const u32*>(pInfo) : mDrawMode);
        CheckCallback_DRAW_XLU(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_UPDATEFRAME:
        UpdateFrame();
        break;

    case G3DPROC_CHILD_DETACHED:
        RemoveAnmObj(static_cast<AnmObj*>(pInfo));
        break;

    default:
        DefG3dProcScnLeaf(task, param, pInfo);
        break;
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

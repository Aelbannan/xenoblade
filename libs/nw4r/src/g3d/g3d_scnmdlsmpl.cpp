// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlsmpl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/ut.h>

void ExecCallback_DRAW_XLU__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv(void) {}


namespace nw4r {
namespace g3d {

bool ScnMdlSimple::GetScnMtxPos(math::MTX34* pMtx, ScnObj::ScnObjMtxType type,
                                u32 index) const {
    if (pMtx != NULL) {
        const ResMdl mdl = mResMdl;
        const ResNode node = mdl.GetResNode(index);

        if (node.IsValid()) {
            // The node's matrix ID indexes into both matrix arrays.
            u32 mtxID = node.GetMtxID();

            switch (type) {
            case ScnObj::MTX_WORLD:
                PSMTXCopy(mpWorldMtxArray[mtxID], *pMtx);
                return true;
            case ScnObj::MTX_VIEW: {
                u32 aligned =
                    (mNumViewMtx * sizeof(math::MTX34) + 0x1F) & ~0x1Fu;
                math::MTX34* pView = reinterpret_cast<math::MTX34*>(
                    reinterpret_cast<u8*>(mpViewPosMtxArray) +
                    mCurView * aligned);
                PSMTXCopy(pView[mtxID], *pMtx);
                return true;
            }
            default:
                break;
            }
        }
    }

    return false;
}

bool ScnMdlSimple::RemoveAnmObj(AnmObj* pObj) {
    if (pObj == NULL) {
        return false;
    }

    if (pObj == mpAnmObjChr) {
        mpAnmObjChr->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjChr = NULL;
        return true;
    }
    if (pObj == mpAnmObjVis) {
        mpAnmObjVis->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjVis = NULL;
        return true;
    }
    if (pObj == mpAnmObjMatClr) {
        mpAnmObjMatClr->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjMatClr = NULL;
        return true;
    }
    if (pObj == mpAnmObjTexPat) {
        mpAnmObjTexPat->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjTexPat = NULL;
        return true;
    }
    if (pObj == mpAnmObjTexSrt) {
        mpAnmObjTexSrt->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjTexSrt = NULL;
        return true;
    }

    return false;
}

// Detaches the animation object of the given type. Each case forwards to the
// AnmObj* overload through the virtual table.
AnmObj* ScnMdlSimple::RemoveAnmObj(ScnMdlSimple::AnmObjType type) {
    AnmObj* pAnmObj = NULL;

    switch (type) {
    case ANMOBJTYPE_CHR:
        pAnmObj = mpAnmObjChr;
        RemoveAnmObj(pAnmObj);
        break;
    case ANMOBJTYPE_VIS:
        pAnmObj = mpAnmObjVis;
        RemoveAnmObj(pAnmObj);
        break;
    case ANMOBJTYPE_MATCLR:
        pAnmObj = mpAnmObjMatClr;
        RemoveAnmObj(pAnmObj);
        break;
    case ANMOBJTYPE_TEXPAT:
        pAnmObj = mpAnmObjTexPat;
        RemoveAnmObj(pAnmObj);
        break;
    case ANMOBJTYPE_TEXSRT:
        pAnmObj = mpAnmObjTexSrt;
        RemoveAnmObj(pAnmObj);
        break;
    case ANMOBJTYPE_SHP:
    case ANMOBJTYPE_NOT_SPECIFIED:
    default:
        break;
    }

    return pAnmObj;
}

} // namespace g3d
} // namespace nw4r



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

// NOTE: the trailing two arguments intentionally mirror the banked draft:
// MWCC routes pCamera through the callback's pointer slot.
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

            // Retail computes the normal-array pointer first, then the texture
            // array pointer; the position array is evaluated inline at the
            // call.
            math::MTX33* pNrm = GetViewNrmMtxArray();
            math::MTX34* pTex = GetViewTexMtxArray();

            CalcView_LC_DMA_ModelMtx(GetViewPosMtxArray(), pNrm,
                                     mpWorldMtxArray, mpWorldMtxAttribArray,
                                     mNumViewMtx, pCamera, mResMdl, pTex);
        } else {
            math::MTX33* pNrm = GetViewNrmMtxArray();
            math::MTX34* pTex = GetViewTexMtxArray();

            CalcView_LC(GetViewPosMtxArray(), pNrm, mpWorldMtxArray,
                        mpWorldMtxAttribArray, mNumViewMtx, pCamera, mResMdl,
                        pTex);
        }
        ut::LC::Unlock();
    } else {
        math::MTX33* pNrm = GetViewNrmMtxArray();
        math::MTX34* pTex = GetViewTexMtxArray();

        CalcView(GetViewPosMtxArray(), pNrm, mpWorldMtxArray,
                 mpWorldMtxAttribArray, mNumViewMtx, pCamera, mResMdl, pTex);
    }

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_C, param,
                            const_cast<void*>(static_cast<const void*>(pCamera)));
}

// Detach-and-replace goes through the AnmObj* overload; each case forwards
// through it after the type check.
bool ScnMdlSimple::SetAnmObj(AnmObj* pObj, AnmObjType type) {
    if (pObj == NULL || pObj->GetParent() != NULL) {
        return false;
    }

    // The unspecified type selects the concrete animation type from the
    // object's RTTI, then uses the same attach path as an explicit type.
    if (type == ANMOBJTYPE_NOT_SPECIFIED) {
        if (G3dObj::DynamicCast<AnmObjChr>(pObj) != NULL) {
            type = ANMOBJTYPE_CHR;
        } else if (G3dObj::DynamicCast<AnmObjVis>(pObj) != NULL) {
            type = ANMOBJTYPE_VIS;
        } else if (G3dObj::DynamicCast<AnmObjMatClr>(pObj) != NULL) {
            type = ANMOBJTYPE_MATCLR;
        } else if (G3dObj::DynamicCast<AnmObjTexPat>(pObj) != NULL) {
            type = ANMOBJTYPE_TEXPAT;
        } else if (G3dObj::DynamicCast<AnmObjTexSrt>(pObj) != NULL) {
            type = ANMOBJTYPE_TEXSRT;
        }
    }

    switch (type) {
    case ANMOBJTYPE_CHR: {
        AnmObj* pAnmObj = G3dObj::DynamicCast<AnmObjChr>(pObj);
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
        AnmObj* pAnmObj = G3dObj::DynamicCast<AnmObjVis>(pObj);
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
        AnmObj* pAnmObj = G3dObj::DynamicCast<AnmObjMatClr>(pObj);
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
        AnmObj* pAnmObj = G3dObj::DynamicCast<AnmObjTexPat>(pObj);
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
        AnmObj* pAnmObj = G3dObj::DynamicCast<AnmObjTexSrt>(pObj);
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
    case ANMOBJTYPE_SHP:
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

    case G3DPROC_CALC_VIEW: {
        // Retail inlines the whole CALC_VIEW routine here (no helper call).
        const math::MTX34* pCamera = static_cast<const math::MTX34*>(pInfo);

        mCurView = (mCurView + 1) % mNumView;

        CheckCallback_CALC_VIEW(CALLBACK_TIMING_A, param,
                                const_cast<void*>(
                                    static_cast<const void*>(pCamera)));
        CalcViewMtx(pCamera);
        CheckCallback_CALC_VIEW(CALLBACK_TIMING_B, param,
                                const_cast<void*>(
                                    static_cast<const void*>(pCamera)));

        if (ut::LC::Lock() != 0) {
            if (mFlagScnMdlSimple & SCNMDLSMPLFLAG_LC_DMA) {
                DCStoreRange(mpWorldMtxArray,
                             mNumViewMtx * sizeof(math::MTX34));

                math::MTX33* pNrm = GetViewNrmMtxArray();
                math::MTX34* pTex = GetViewTexMtxArray();

                CalcView_LC_DMA_ModelMtx(GetViewPosMtxArray(), pNrm,
                                         mpWorldMtxArray, mpWorldMtxAttribArray,
                                         mNumViewMtx, pCamera, mResMdl, pTex);
            } else {
                math::MTX33* pNrm = GetViewNrmMtxArray();
                math::MTX34* pTex = GetViewTexMtxArray();

                CalcView_LC(GetViewPosMtxArray(), pNrm, mpWorldMtxArray,
                            mpWorldMtxAttribArray, mNumViewMtx, pCamera,
                            mResMdl, pTex);
            }
            ut::LC::Unlock();
        } else {
            math::MTX33* pNrm = GetViewNrmMtxArray();
            math::MTX34* pTex = GetViewTexMtxArray();

            CalcView(GetViewPosMtxArray(), pNrm, mpWorldMtxArray,
                     mpWorldMtxAttribArray, mNumViewMtx, pCamera, mResMdl,
                     pTex);
        }

        CheckCallback_CALC_VIEW(CALLBACK_TIMING_C, param,
                                const_cast<void*>(
                                    static_cast<const void*>(pCamera)));
        break;
    }

    case G3DPROC_GATHER_SCNOBJ:
        // Retail inlines the gather helper.
        static_cast<IScnObjGather*>(pInfo)->Add(
            this, !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_OPA),
            !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_XLU));
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

namespace nw4r {
namespace g3d {

ScnMdlSimple::ScnMdlSimple(MEMAllocator* pAllocator, ResMdl mdl,
                           math::MTX34* pWorldMtxArray,
                           u32* pWorldMtxAttribArray,
                           math::MTX34* pViewPosMtxArray,
                           math::MTX33* pViewNrmMtxArray,
                           math::MTX34* pViewTexMtxArray, int numView,
                           int numViewMtx)
    : ScnLeaf(pAllocator) {
    mResMdl = mdl;

    mpWorldMtxArray = pWorldMtxArray;
    mpWorldMtxAttribArray = pWorldMtxAttribArray;
    mpViewPosMtxArray = pViewPosMtxArray;
    mpViewNrmMtxArray = pViewNrmMtxArray;
    mpViewTexMtxArray = pViewTexMtxArray;

    mNumView = static_cast<u8>(numView);
    mCurView = 0;
    mNumViewMtx = static_cast<u16>(numViewMtx);
    mFlagScnMdlSimple = 0;

    mpByteCodeCalc = mdl.GetResByteCode("NodeTree");
    mpByteCodeMix = mdl.GetResByteCode("NodeMix");
    mpByteCodeDrawOpa = mdl.GetResByteCode("DrawOpa");
    mpByteCodeDrawXlu = mdl.GetResByteCode("DrawXlu");

    mDrawMode = RESMDL_DRAWMODE_DEFAULT;

    mpCalcWorldCallback = NULL;
    mCwcbTiming = 0;
    mCwcbDeleteOption = 0;
    mCwcbNodeID = 0;

    mpAnmObjChr = NULL;
    mpAnmObjVis = NULL;
    mpAnmObjMatClr = NULL;
    mpAnmObjTexPat = NULL;
    mpAnmObjTexSrt = NULL;

    // Record which bytecode streams are missing so later passes can skip them.
    if (mpByteCodeCalc == NULL) {
        mFlagScnMdlSimple |= 0x2000; // NOT_CALC_BYTECODE
    } else {
        mFlagScnMdlSimple &= ~0x2000u;
    }
    if (mpByteCodeDrawOpa == NULL) {
        mFlagScnMdlSimple |= 0x4000; // NOT_DRAW_OPA_BYTECODE
    } else {
        mFlagScnMdlSimple &= ~0x4000u;
    }

    // The per-view matrix arrays live in one contiguous allocation; invalidate
    // the cache for every view's slice before first use.
    if (mpViewPosMtxArray != NULL) {
        u32 aligned = (numViewMtx * sizeof(math::MTX34) + 0x1F) & ~0x1F;
        DCInvalidateRange(mpViewPosMtxArray, numView * aligned);
    }
    if (mpViewNrmMtxArray != NULL) {
        u32 aligned = (numViewMtx * sizeof(math::MTX33) + 0x1F) & ~0x1F;
        DCInvalidateRange(mpViewNrmMtxArray, numView * aligned);
    }
    if (mpViewTexMtxArray != NULL) {
        u32 aligned = (numViewMtx * sizeof(math::MTX34) + 0x1F) & ~0x1F;
        DCInvalidateRange(mpViewTexMtxArray, numView * aligned);
    }

    // Use the model's stored local bounding volume when it is valid.
    const ResMdlData& rData = mdl.ref();
    if (rData.info.is_valid_volume != 0) {
        math::AABB aabb;
        aabb.min = rData.info.volume_min;
        aabb.max = rData.info.volume_max;
        SetBoundingVolume(BOUNDINGVOLUME_AABB_LOCAL, &aabb);
    }
}

ScnMdlSimple::~ScnMdlSimple() {
    if (static_cast<s8>(mCwcbDeleteOption) == 1 &&
        mpCalcWorldCallback != NULL) {
        delete mpCalcWorldCallback;
    } else {
        // no other delete options
    }

    if (mpAnmObjChr != NULL) {
        RemoveAnmObj(mpAnmObjChr);
    }
    if (mpAnmObjVis != NULL) {
        RemoveAnmObj(mpAnmObjVis);
    }
    if (mpAnmObjMatClr != NULL) {
        RemoveAnmObj(mpAnmObjMatClr);
    }
    if (mpAnmObjTexPat != NULL) {
        RemoveAnmObj(mpAnmObjTexPat);
    }
    if (mpAnmObjTexSrt != NULL) {
        RemoveAnmObj(mpAnmObjTexSrt);
    }
}

NW4R_G3D_RTTI_DEF(ScnMdlSimple);

bool ScnMdlSimple::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(TYPE_NAME) ? true
         : other == ScnLeaf::GetTypeObjStatic() ? true
         : other == ScnObj::GetTypeObjStatic() ? true
         : (other == G3dObj::GetTypeObjStatic());
}

const G3dObj::TypeObj ScnMdlSimple::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}

const char* ScnMdlSimple::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r


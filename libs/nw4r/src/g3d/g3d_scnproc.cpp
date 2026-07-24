#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(ScnProc);

ScnProc* ScnProc::Construct(MEMAllocator* pAllocator, u32* pSize,
                            DrawProc pProc, bool opa, bool xlu,
                            u32 userDataSize) {
    ScnProc* pScnProc = NULL;
    u32 scnProcSize = sizeof(ScnProc);

    userDataSize = align4(userDataSize);
    u32 userDataOfs = align4(scnProcSize);

    u32 size = align4(userDataOfs + userDataSize);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        u8* pBuffer = reinterpret_cast<u8*>(Alloc(pAllocator, size));

        if (pBuffer != NULL) {
            void* pUserData = userDataSize != 0 ? pBuffer + userDataOfs : NULL;

            pScnProc =
                new (pBuffer) ScnProc(pAllocator, pProc, pUserData, opa, xlu);
        }
    }

    return pScnProc;
}

void ScnProc::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3DPROC_GATHER_SCNOBJ: {
        IScnObjGather* pCollection = static_cast<IScnObjGather*>(pInfo);
        pCollection->Add(this, (mFlag & SCNPROCFLAG_DRAW_OPA) ? true : false,
                         (mFlag & SCNPROCFLAG_DRAW_XLU) ? true : false);
        break;
    }

    case G3DPROC_DRAW_OPA: {
        if (mpDrawProc != NULL) {
            G3DState::Invalidate();
            mpDrawProc(this, true);
        }
        break;
    }

    case G3DPROC_DRAW_XLU: {
        if (mpDrawProc != NULL) {
            G3DState::Invalidate();
            mpDrawProc(this, false);
        }
        break;
    }

    default: {
        DefG3dProcScnLeaf(task, param, pInfo);
    }
    }
}

} // namespace g3d
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-803ff3a8
extern "C" void IsDerivedFrom__Q34nw4r3g3d7ScnProcCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803ff3a8
// LLM-HARNESS-BEGIN: us-803ff410
extern "C" void GetTypeName__Q34nw4r3g3d7ScnProcCFv() {}
// LLM-HARNESS-END: us-803ff410
// LLM-HARNESS-BEGIN: us-803ff440
extern "C" void* GetTypeObj__Q34nw4r3g3d7ScnProcCFv(void) { return 0; }
// LLM-HARNESS-END: us-803ff440

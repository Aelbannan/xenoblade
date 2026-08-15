#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_scnproc.h>

namespace nw4r {
namespace g3d {

// Allocates a ScnProc (plus an optional caller-owned user-data area) out of
// the given MEM allocator. The user-data area follows the object header:
// both the object size and the user-data size are rounded up to 4 bytes, and
// the layout is size-aligned so `pSize` reports the exact allocation size.
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
            u8* pUserData = userDataSize != 0 ? pBuffer + userDataOfs : NULL;

            pScnProc =
                new (pBuffer) ScnProc(pAllocator, pProc, pUserData, opa, xlu);
        }
    }

    return pScnProc;
}

// Scene-graph message dispatch. Callback tasks (GATHER_SCNOBJ / DRAW_OPA /
// DRAW_XLU) are serviced directly; everything else falls through to the
// ScnLeaf default handler.
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

// Empty body: ScnProc owns no resources of its own. The compiler still emits
// the implicit ScnLeaf base destruction, which devirtualises straight to
// ScnObj::~ScnObj (retail's double `this`-null-check + `li r4,0; bl` shape).
ScnProc::~ScnProc() {}

// Retail RTTI name record for ScnProc ("ScnProc", 8 bytes including NUL)
// lives in the shared nw4r_data.s unit, outside this split slice; the
// GetTypeObj / IsDerivedFrom relocations reference it by name only, so keep
// it as an extern declaration (no local .rodata). `extern "C"` is required:
// the declaration sits inside namespace nw4r::g3d, where C++ linkage would
// mangle the global variable name.
extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("ScnProc")> lbl_eu_8051D7E8;

// Base-chain check mirroring ScnLeaf/ScnObj/G3dObj::IsDerivedFrom. Kept as a
// static helper (not a virtual-call chain) so MWCC inlines it into
// ScnProc::IsDerivedFrom and emits the retail parameter-reload pattern
// (a fresh lwz for the first two comparisons, then register reuse).
static bool scnLeafIsDerivedFrom(G3dObj::TypeObj other) {
    if (other == G3dObj::TypeObj(lbl_eu_8051D778)) {
        return true;
    }
    if (other == G3dObj::TypeObj(lbl_eu_8051D768)) {
        return true;
    }
    return other == G3dObj::TypeObj(lbl_eu_8051D640);
}

// ScnProc is derived from ScnLeaf -> ScnObj -> G3dObj; the RTTI chain is
// flattened here so MWCC emits the retail straight-line comparison sequence.
bool ScnProc::IsDerivedFrom(G3dObj::TypeObj other) const {
    if (other == TypeObj(lbl_eu_8051D7E8)) {
        return true;
    }
    return scnLeafIsDerivedFrom(other);
}

const G3dObj::TypeObj ScnProc::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D7E8);
}

const char* ScnProc::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

} // namespace g3d
} // namespace nw4r

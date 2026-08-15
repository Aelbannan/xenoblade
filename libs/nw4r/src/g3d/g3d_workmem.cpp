#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace workmem {

// Retail work-memory arena (nw4r_data.s lbl_eu_8061FB00, .bss 0x18000).
// Reference the shared retail buffer instead of defining a TU-local arena so
// the unit emits no .bss; the getters keep the retail lis/addi relocs.
extern "C" union {
    u8 mem[WORKMEM_SIZE]; // at 0x0
    struct {
        math::_VEC3 tmpScale[WORKMEM_NUMTMPSCALE]; // at 0x0
        u32 mtxID[WORKMEM_NUMMTXID];               // at 0x6000
    };
    u8 byteCode[WORKMEM_NUMBYTECODE];                 // at 0x0
    MdlZ mdlZ[WORKMEM_NUMMDLZ];                       // at 0x0
    math::_MTX34 skinningMtx[WORKMEM_NUMSKINNINGMTX]; // at 0x0
    math::_MTX34 bbMtx[WORKMEM_NUMBBMTX];             // at 0x0
} lbl_eu_8061FB00;

math::VEC3* GetScaleTemporary() {
    return static_cast<math::VEC3*>(lbl_eu_8061FB00.tmpScale);
}

u32* GetMtxIDTemporary() {
    return lbl_eu_8061FB00.mtxID;
}

MdlZ* GetMdlZTemporary() {
    return lbl_eu_8061FB00.mdlZ;
}

math::MTX34* GetSkinningMtxTemporary() {
    return static_cast<math::MTX34*>(lbl_eu_8061FB00.skinningMtx);
}

math::MTX34* GetBillboardMtxTemporary() {
    return static_cast<math::MTX34*>(lbl_eu_8061FB00.bbMtx);
}

} // namespace workmem
} // namespace detail
} // namespace g3d
} // namespace nw4r

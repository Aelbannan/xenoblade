#ifndef NW4R_G3D_MAYA_H
#define NW4R_G3D_MAYA_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanmtexsrt.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" const float lbl_eu_80669C98; // 0.0f
extern "C" const float lbl_eu_80669C9C; // 1.0f
extern "C" const float lbl_eu_80669CA0; // 256.0f/360.0f (deg_to_fidx)
extern "C" const float lbl_eu_80669CA4; // 0.5f

typedef void (*TexSrtMtxFunc)(math::MTX34* pMtx, const TexSrt& rSrt);
extern "C" const TexSrtMtxFunc lbl_eu_8051D6C0[];
extern "C" const TexSrtMtxFunc lbl_eu_8051D6DC[];

bool CalcTexMtx_Maya(math::MTX34* pMtx, bool set, const TexSrt& rSrt,
                     TexSrt::Flag flag);

u32 CalcWorldMtx_Maya_SSC_Apply(math::MTX34* pW, math::VEC3* pS,
                                const math::MTX34* pW1, const math::VEC3* pS1,
                                u32 attr, const ChrAnmResult* pResult);

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif

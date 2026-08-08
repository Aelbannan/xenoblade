#ifndef NW4R_G3D_3DSMAX_H
#define NW4R_G3D_3DSMAX_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanmtexsrt.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {

bool CalcTexMtx_3dsmax(math::MTX34* pMtx, bool set, const TexSrt& rSrt,
                       TexSrt::Flag flag);

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
typedef void (*TexSrtMtxFunc)(nw4r::math::MTX34* pMtx,
                              const nw4r::g3d::TexSrt& rSrt);
extern "C" const float lbl_eu_80669CC0; // 0.0f
extern "C" const float lbl_eu_80669CC4; // 0.5f
extern "C" const float lbl_eu_80669CC8; // 1.0f
extern "C" const float lbl_eu_80669CCC; // 256.0f/360.0f (deg_to_fidx)
extern "C" const float lbl_eu_80669CD0; // -0.5f
extern "C" const TexSrtMtxFunc lbl_eu_8051D730[];
extern "C" const TexSrtMtxFunc lbl_eu_8051D74C[];

#endif

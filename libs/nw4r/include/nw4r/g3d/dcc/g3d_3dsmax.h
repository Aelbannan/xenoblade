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
extern "C" const TexSrtMtxFunc TexSrtMtxSetDispatch[];
extern "C" const TexSrtMtxFunc TexSrtMtxProductDispatch[];

#endif

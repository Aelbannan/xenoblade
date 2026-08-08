#ifndef NW4R_G3D_XSI_H
#define NW4R_G3D_XSI_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanmtexsrt.h>

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
// XSI DCC float constants (SDA21)
extern "C" const float lbl_eu_80669CB0; // 0.0f
extern "C" const float lbl_eu_80669CB4; // 1.0f
extern "C" const float lbl_eu_80669CB8; // 256.0f/360.0f (deg_to_fidx)

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {

bool CalcTexMtx_Xsi(math::MTX34* pMtx, bool set, const TexSrt& rSrt,
                    TexSrt::Flag flag);

u32 CalcWorldMtx_Xsi(math::MTX34* pW, math::VEC3* pS, const math::MTX34* pW1,
                     const math::VEC3* pS1, u32 attr,
                     const g3d::ChrAnmResult* pResult);

// TexSrtMtxFunc dispatch tables (retail symbol names)
typedef void (*TexSrtMtxFunc)(math::MTX34* pMtx, const TexSrt& rSrt);

extern "C" const TexSrtMtxFunc lbl_eu_8051D6F8[];
extern "C" const TexSrtMtxFunc lbl_eu_8051D714[];

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif

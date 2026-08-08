#include <nw4r/g3d.h>
#include <nw4r/g3d/dcc/g3d_3dsmax.h>
#include <decomp.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {
namespace {

#pragma dont_inline on
void MakeTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] = (lbl_eu_80669CC8 - rSrt.Su) * lbl_eu_80669CC4;
    pMtx->m[1][3] = (lbl_eu_80669CC8 - rSrt.Sv) * lbl_eu_80669CC4;
    pMtx->m[0][0] = rSrt.Su;
    pMtx->m[0][1] = lbl_eu_80669CC0;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[1][0] = lbl_eu_80669CC0;
    pMtx->m[1][1] = rSrt.Sv;
    pMtx->m[1][2] = lbl_eu_80669CC0;
}

void MakeTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CD0 * (cosR + sinR - lbl_eu_80669CC8);
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = lbl_eu_80669CD0 * (-cosR + sinR - lbl_eu_80669CC8);
}

void MakeTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = lbl_eu_80669CC8;
    pMtx->m[0][1] = lbl_eu_80669CC0;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = -rSrt.Tu;
    pMtx->m[1][0] = lbl_eu_80669CC0;
    pMtx->m[1][1] = lbl_eu_80669CC8;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = rSrt.Tv;
}

void MakeTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][1] = su * sinR;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CD0 * (su * (cosR - sinR) - lbl_eu_80669CC8);
    pMtx->m[1][0] = -sv * sinR;
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = lbl_eu_80669CD0 * (sv * (cosR - sinR) - lbl_eu_80669CC8);
}

void MakeTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CC4 - lbl_eu_80669CC4 * cosR - lbl_eu_80669CC4 * sinR +
                    tu * (-cosR) - tv * sinR + lbl_eu_80669CC4;
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = lbl_eu_80669CC4 + lbl_eu_80669CC4 * sinR - lbl_eu_80669CC4 * cosR +
                    tu * sinR + tv * (-cosR) + lbl_eu_80669CC4;
}

void MakeTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su;
    pMtx->m[0][1] = lbl_eu_80669CC0;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CC4 * (lbl_eu_80669CC8 - su) - su * tu + lbl_eu_80669CC4;
    pMtx->m[1][0] = lbl_eu_80669CC0;
    pMtx->m[1][1] = sv;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = lbl_eu_80669CC4 * (lbl_eu_80669CC8 - sv) + sv * tv + lbl_eu_80669CC4;
}

void MakeTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][1] = su * sinR;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CC4 - lbl_eu_80669CC4 * su * cosR + lbl_eu_80669CC4 * su * sinR +
                    tu * (-su * cosR) + tv * (-sv * sinR) + lbl_eu_80669CC4;
    pMtx->m[1][0] = -sv * sinR;
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = lbl_eu_80669CC4 - lbl_eu_80669CC4 * sv * cosR - lbl_eu_80669CC4 * sv * sinR +
                    tu * sv * sinR + tv * (-sv * cosR) + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sv = rSrt.Sv;
    f32 su = rSrt.Su;

    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] = su * m03 + lbl_eu_80669CC4;
    pMtx->m[1][0] *= sv;
    pMtx->m[1][1] *= sv;
    pMtx->m[1][2] *= sv;
    pMtx->m[1][3] = sv * m13 + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] = m00 * cosR - m01 * sinR;
    pMtx->m[0][1] = m00 * sinR + m01 * cosR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = m03 * cosR + m13 * sinR + lbl_eu_80669CC4;
    pMtx->m[1][0] = m10 * cosR - m11 * sinR;
    pMtx->m[1][1] = m10 * sinR + m11 * cosR;
    pMtx->m[1][2] = pMtx->m[1][2];
    pMtx->m[1][3] = m13 * cosR - m03 * sinR + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] -= rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] = m00 * su * cosR - m01 * sv * sinR;
    pMtx->m[0][1] = m00 * su * sinR + m01 * sv * cosR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = m03 * cosR + m13 * sinR + lbl_eu_80669CC4;
    pMtx->m[1][0] = m10 * su * cosR - m11 * sv * sinR;
    pMtx->m[1][1] = m10 * su * sinR + m11 * sv * cosR;
    pMtx->m[1][2] = pMtx->m[1][2];
    pMtx->m[1][3] = m13 * cosR - m03 * sinR + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] = m00 * cosR - m01 * sinR;
    pMtx->m[0][1] = m00 * sinR + m01 * cosR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = m03 * cosR + m13 * sinR - tu + lbl_eu_80669CC4;
    pMtx->m[1][0] = m10 * cosR - m11 * sinR;
    pMtx->m[1][1] = m10 * sinR + m11 * cosR;
    pMtx->m[1][2] = pMtx->m[1][2];
    pMtx->m[1][3] = m13 * cosR - m03 * sinR + tv + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] = su * m03 - tu + lbl_eu_80669CC4;
    pMtx->m[1][0] *= sv;
    pMtx->m[1][1] *= sv;
    pMtx->m[1][2] *= sv;
    pMtx->m[1][3] = sv * m13 + tv + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] = m00 * su * cosR - m01 * sv * sinR;
    pMtx->m[0][1] = m00 * su * sinR + m01 * sv * cosR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = m03 * cosR + m13 * sinR - tu + lbl_eu_80669CC4;
    pMtx->m[1][0] = m10 * su * cosR - m11 * sv * sinR;
    pMtx->m[1][1] = m10 * su * sinR + m11 * sv * cosR;
    pMtx->m[1][2] = pMtx->m[1][2];
    pMtx->m[1][3] = m13 * cosR - m03 * sinR + tv + lbl_eu_80669CC4;
}
#pragma dont_inline reset

typedef void (*TexSrtMtxFunc)(math::MTX34* pMtx, const TexSrt& rSrt);

} // namespace



bool CalcTexMtx_3dsmax(math::MTX34* pMtx, bool bSet, const TexSrt& rSrt, TexSrt::Flag flag) {
    u32 idx = DECOMP_PPC_RLWINM(flag, 31, 29, 31);

    if (idx == 0x7) {
        return false;
    }

    if (bSet) {
        lbl_eu_8051D730[idx](pMtx, rSrt);
    } else {
        lbl_eu_8051D74C[idx](pMtx, rSrt);
    }

    pMtx->m[2][0] = lbl_eu_80669CC0;
    pMtx->m[2][1] = lbl_eu_80669CC0;
    pMtx->m[2][2] = lbl_eu_80669CC8;
    pMtx->m[2][3] = lbl_eu_80669CC0;

    return true;
}

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r

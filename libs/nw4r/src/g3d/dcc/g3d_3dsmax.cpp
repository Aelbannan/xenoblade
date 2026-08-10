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
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    pMtx->m[0][0] = su;
    pMtx->m[0][1] = lbl_eu_80669CC0;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CC4 * (lbl_eu_80669CC8 - su);
    pMtx->m[1][0] = lbl_eu_80669CC0;
    pMtx->m[1][1] = sv;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = lbl_eu_80669CC4 * (lbl_eu_80669CC8 - sv);
}

void MakeTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 cosR, sinR;
    f32 fidx = lbl_eu_80669CCC * rSrt.R;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    pMtx->m[0][0] = sinR;
    pMtx->m[0][1] = cosR;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CD0 * (sinR + cosR - lbl_eu_80669CC8);
    pMtx->m[1][0] = -cosR;
    pMtx->m[1][1] = sinR;
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
    pMtx->m[0][3] = lbl_eu_80669CD0 * (su * cosR + su * sinR - lbl_eu_80669CC8);
    pMtx->m[1][0] = -(sv * sinR);
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] =
        lbl_eu_80669CD0 * (-(sv * sinR) + sv * cosR - lbl_eu_80669CC8);
}

void MakeTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = lbl_eu_80669CCC * rSrt.R;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = -cosR * (lbl_eu_80669CC4 + rSrt.Tu) +
                    sinR * (rSrt.Tv - lbl_eu_80669CC4) + lbl_eu_80669CC4;
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = sinR * (lbl_eu_80669CC4 + rSrt.Tu) +
                    cosR * (rSrt.Tv - lbl_eu_80669CC4) + lbl_eu_80669CC4;
}

void MakeTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su;
    pMtx->m[0][1] = lbl_eu_80669CC0;
    pMtx->m[0][2] = lbl_eu_80669CC0;
    pMtx->m[0][3] = lbl_eu_80669CC4 + (-su) * (lbl_eu_80669CC4 + tu);
    pMtx->m[1][0] = lbl_eu_80669CC0;
    pMtx->m[1][1] = sv;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = lbl_eu_80669CC4 + sv * (tv - lbl_eu_80669CC4);
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
    pMtx->m[0][3] = -(su * cosR) * (lbl_eu_80669CC4 + tu) +
                    su * sinR * (tv - lbl_eu_80669CC4) + lbl_eu_80669CC4;
    pMtx->m[1][0] = -(sv * sinR);
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = lbl_eu_80669CC0;
    pMtx->m[1][3] = sv * sinR * (lbl_eu_80669CC4 + tu) +
                    sv * cosR * (tv - lbl_eu_80669CC4) + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] = lbl_eu_80669CC4 + su * m03;
    pMtx->m[1][0] *= sv;
    pMtx->m[1][1] *= sv;
    pMtx->m[1][2] *= sv;
    pMtx->m[1][3] = lbl_eu_80669CC4 + sv * m13;
}

void ProductTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3] - lbl_eu_80669CC4;
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3] - lbl_eu_80669CC4;

    pMtx->m[0][0] = m00 * cosR + m10 * sinR;
    pMtx->m[1][0] = -sinR * m00 + cosR * m10;
    pMtx->m[0][1] = m01 * cosR + m11 * sinR;
    pMtx->m[1][1] = -sinR * m01 + cosR * m11;
    pMtx->m[0][2] = m02 * cosR + m12 * sinR;
    pMtx->m[1][2] = -sinR * m02 + cosR * m12;
    pMtx->m[0][3] = m03 * cosR + m13 * sinR + lbl_eu_80669CC4;
    pMtx->m[1][3] = -sinR * m03 + cosR * m13 + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] -= rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m03 = pMtx->m[0][3];
    f32 m13 = pMtx->m[1][3];
    f32 m00 = pMtx->m[0][0];
    f32 m10 = pMtx->m[1][0];
    f32 m01 = pMtx->m[0][1];
    f32 m11 = pMtx->m[1][1];
    f32 m02 = pMtx->m[0][2];
    f32 m12 = pMtx->m[1][2];

    f32 svs = rSrt.Sv * sinR;
    f32 sus = rSrt.Su * sinR;
    f32 suc = rSrt.Su * cosR;
    f32 nsvs = -svs;
    f32 m03c = m03 - lbl_eu_80669CC4;
    f32 m13c = m13 - lbl_eu_80669CC4;
    f32 svc = rSrt.Sv * cosR;

    // result = TexSrt(SR) * mtx (column-vector convention, translate row first)
    pMtx->m[0][3] = suc * m03c + sus * m13c + lbl_eu_80669CC4;
    pMtx->m[1][3] = nsvs * m03c + svc * m13c + lbl_eu_80669CC4;
    pMtx->m[0][0] = suc * m00 + sus * m10;
    pMtx->m[1][0] = nsvs * m00 + svc * m10;
    pMtx->m[0][1] = suc * m01 + sus * m11;
    pMtx->m[1][1] = nsvs * m01 + svc * m11;
    pMtx->m[0][2] = suc * m02 + sus * m12;
    pMtx->m[1][2] = nsvs * m02 + svc * m12;
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CCC;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3] - rSrt.Tu;
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3] + rSrt.Tv;

    f32 m03c = m03 - lbl_eu_80669CC4;
    f32 m13c = m13 - lbl_eu_80669CC4;

    pMtx->m[0][0] = m00 * cosR + m10 * sinR;
    pMtx->m[1][0] = -sinR * m00 + cosR * m10;
    pMtx->m[0][1] = m01 * cosR + m11 * sinR;
    pMtx->m[1][1] = -sinR * m01 + cosR * m11;
    pMtx->m[0][2] = m02 * cosR + m12 * sinR;
    pMtx->m[1][2] = -sinR * m02 + cosR * m12;
    pMtx->m[0][3] = m03c * cosR + m13c * sinR + lbl_eu_80669CC4;
    pMtx->m[1][3] = -sinR * m03c + cosR * m13c + lbl_eu_80669CC4;
}

void ProductTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m03 = pMtx->m[0][3];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] = lbl_eu_80669CC4 + su * (m03 - tu - lbl_eu_80669CC4);
    pMtx->m[1][0] *= sv;
    pMtx->m[1][1] *= sv;
    pMtx->m[1][2] *= sv;
    pMtx->m[1][3] = m13 * (lbl_eu_80669CC4 + sv * (m13 + tv - lbl_eu_80669CC4));
}

void ProductTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = lbl_eu_80669CCC * rSrt.R;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 sv = rSrt.Sv;
    f32 su = rSrt.Su;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m03 = pMtx->m[0][3] - tu;
    f32 m13 = pMtx->m[1][3] + tv;
    f32 m00 = pMtx->m[0][0];
    f32 m10 = pMtx->m[1][0];
    f32 m01 = pMtx->m[0][1];
    f32 m11 = pMtx->m[1][1];
    f32 m02 = pMtx->m[0][2];
    f32 m12 = pMtx->m[1][2];

    f32 svs = sv * sinR;
    f32 sus = su * sinR;
    f32 suc = su * cosR;
    f32 nsvs = -svs;
    f32 svc = sv * cosR;

    // result = TexSrt(SRT) * mtx (column-vector convention, translate row first)
    pMtx->m[0][0] = suc * m00 + sus * m10;
    pMtx->m[0][3] = suc * m03 + sus * m13 + lbl_eu_80669CC4;
    pMtx->m[1][3] = nsvs * m03 + svc * m13 + lbl_eu_80669CC4;
    pMtx->m[1][0] = nsvs * m00 + svc * m10;
    pMtx->m[0][1] = suc * m01 + sus * m11;
    pMtx->m[1][1] = nsvs * m01 + svc * m11;
    pMtx->m[0][2] = suc * m02 + sus * m12;
    pMtx->m[1][2] = nsvs * m02 + svc * m12;
}
#pragma dont_inline reset

typedef void (*TexSrtMtxFunc)(math::MTX34* pMtx, const TexSrt& rSrt);

} // namespace

// These TexSrt matrix builders are only referenced through the retail
// dispatch tables (lbl_eu_8051D730 / lbl_eu_8051D74C), which the compiler
// cannot see as in-TU references. Force-emit them so they survive DCE.
DECOMP_FORCEACTIVE(g3d_3dsmax_cpp,
                   MakeTexSrtMtx_S, MakeTexSrtMtx_R, MakeTexSrtMtx_T,
                   MakeTexSrtMtx_SR, MakeTexSrtMtx_RT, MakeTexSrtMtx_ST,
                   MakeTexSrtMtx_SRT, ProductTexSrtMtx_S, ProductTexSrtMtx_R,
                   ProductTexSrtMtx_T, ProductTexSrtMtx_SR, ProductTexSrtMtx_RT,
                   ProductTexSrtMtx_ST, ProductTexSrtMtx_SRT);


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

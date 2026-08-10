#include <nw4r/g3d.h>
#include <nw4r/g3d/dcc/g3d_maya.h>
#include <decomp.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {

// -0.5f (RT translation constant, retail .sdata2 pool symbol)
extern "C" const float lbl_eu_80669CA8;

namespace {

#pragma dont_inline on
void MakeTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = rSrt.Su;
    pMtx->m[0][1] = lbl_eu_80669C98;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = lbl_eu_80669C98;
    pMtx->m[1][0] = lbl_eu_80669C98;
    pMtx->m[1][1] = rSrt.Sv;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = lbl_eu_80669C9C - rSrt.Sv;
}

void MakeTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = lbl_eu_80669CA4 * (lbl_eu_80669C9C - cosR - sinR);
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = lbl_eu_80669CA4 * (lbl_eu_80669C9C - cosR + sinR);
}

void MakeTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = lbl_eu_80669C9C;
    pMtx->m[0][1] = lbl_eu_80669C98;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = -rSrt.Tu;
    pMtx->m[1][0] = lbl_eu_80669C98;
    pMtx->m[1][1] = lbl_eu_80669C9C;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = rSrt.Tv;
}

void MakeTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][1] = su * sinR;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = lbl_eu_80669CA4 * su * (lbl_eu_80669C9C - cosR - sinR);
    pMtx->m[1][0] = -sv * sinR;
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = lbl_eu_80669C9C - lbl_eu_80669CA4 * sv * (lbl_eu_80669C9C - cosR + sinR);
}

void MakeTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = lbl_eu_80669CA4 * (lbl_eu_80669C9C - cosR - sinR) - rSrt.Tu;
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = rSrt.Tv + lbl_eu_80669CA4 * (lbl_eu_80669C9C - cosR + sinR);
}

void MakeTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su;
    pMtx->m[0][1] = lbl_eu_80669C98;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = -su * tu;
    pMtx->m[1][0] = lbl_eu_80669C98;
    pMtx->m[1][1] = sv;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = tv + sv * (lbl_eu_80669C9C - tv);
}

void MakeTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][1] = su * sinR;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = lbl_eu_80669CA4 * su * (lbl_eu_80669C9C - cosR - sinR) - tu;
    pMtx->m[1][0] = -sv * sinR;
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = tv - lbl_eu_80669CA4 * sv * (lbl_eu_80669C9C - cosR + sinR);
}

void ProductTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sv = rSrt.Sv;
    f32 su = rSrt.Su;

    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] = m03 * su;
    pMtx->m[1][0] = m10 * sv;
    pMtx->m[1][1] = m11 * sv;
    pMtx->m[1][2] = m12 * sv;
    pMtx->m[1][3] = m13 * sv + (lbl_eu_80669C9C - sv);
}

void ProductTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] = m00 * cosR - m01 * sinR;
    pMtx->m[0][1] = m00 * sinR + m01 * cosR;
    pMtx->m[0][2] = m02;
    pMtx->m[0][3] = m03;
    pMtx->m[1][0] = m10 * cosR - m11 * sinR;
    pMtx->m[1][1] = m10 * sinR + m11 * cosR;
    pMtx->m[1][2] = m12;
    pMtx->m[1][3] = m13;
}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] += -rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    f32 sv_sin = sv * sinR;
    f32 sv_cos = sv * cosR;
    f32 sv_half = 0.5f * sv;
    f32 m13_5 = m13 - 0.5f;
    f32 m03_5 = m03 - 0.5f;
    f32 su_cos = su * cosR;
    f32 su_sin = su * sinR;

    pMtx->m[1][3] = 1.0f + (-sv_sin * m03_5 + sv_cos * m13_5 - sv_half);
    pMtx->m[1][0] = -sv_sin * m00 + sv_cos * m10;
    pMtx->m[0][0] = su_cos * m00 + su_sin * m10;
    pMtx->m[0][1] = su_cos * m01 + su_sin * m11;
    pMtx->m[1][1] = -sv_sin * m01 + sv_cos * m11;
    pMtx->m[0][2] = su_cos * m02 + su_sin * m12;
    pMtx->m[1][2] = -sv_sin * m02 + sv_cos * m12;
    pMtx->m[0][3] = su_sin * m12 + (su_cos * m03_5 + su_sin * m13_5);
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    // pMtx = T(-Tu, Tv) * R_maya * pMtx; R_maya is the transposed Maya
    // rotation (sin/cos swapped rows) with 0.5*(1-cos-sin) translation.
    pMtx->m[0][0] = sinR * m10 + cosR * m00;
    pMtx->m[1][0] = -sinR * m00 + cosR * m10;
    pMtx->m[0][1] = sinR * m11 + cosR * m01;
    pMtx->m[1][1] = -sinR * m01 + cosR * m11;
    pMtx->m[0][2] = sinR * m12 + cosR * m02;
    pMtx->m[1][2] = -sinR * m02 + cosR * m12;
    pMtx->m[0][3] = (lbl_eu_80669CA8 * cosR + (sinR * m13 + cosR * m03) -
                     (lbl_eu_80669CA4 * sinR - lbl_eu_80669CA4)) -
                    rSrt.Tu;
    pMtx->m[1][3] = rSrt.Tv +
                    ((lbl_eu_80669CA4 * sinR - lbl_eu_80669CA4) +
                     (lbl_eu_80669CA8 * cosR + (-sinR * m03 + cosR * m13)));
}

void ProductTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] = pMtx->m[0][3] * su - tu;
    pMtx->m[1][0] *= sv;
    pMtx->m[1][1] *= sv;
    pMtx->m[1][2] *= sv;
    pMtx->m[1][3] = pMtx->m[1][3] * sv + tv;
}

void ProductTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CA0;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    f32 sv_sin = sv * sinR;
    f32 m13_5 = m13 - 0.5f;
    f32 tv_5 = 0.5f - rSrt.Tv;
    f32 tu_5 = 0.5f - rSrt.Tu;
    f32 sv_cos = sv * cosR;
    f32 su_cos = su * cosR;
    f32 m03_5 = m03 - 0.5f;
    f32 m03_p5 = 0.5f + m03;
    f32 sv_tv = tv_5 * sv;
    f32 su_sin = su * sinR;

    pMtx->m[0][0] = su_cos * m00 + su_sin * m10;
    pMtx->m[1][0] = -sv_sin * m00 + sv_cos * m10;
    pMtx->m[1][3] = 1.0f + (-sv_sin * m03_p5 + sv_cos * m13_5 - sv_tv);
    pMtx->m[0][1] = su_cos * m01 + su_sin * m11;
    pMtx->m[0][2] = su_cos * m02 + su_sin * m12;
    pMtx->m[1][1] = -sv_sin * m01 + sv_cos * m11;
    pMtx->m[1][2] = -sv_sin * m02 + sv_cos * m12;
    pMtx->m[0][3] = tu_5 * su + (su_cos * m03_5 + su_sin * m13_5);
}
#pragma dont_inline reset

} // namespace

DECOMP_FORCEACTIVE(g3d_maya_cpp,
                   MakeTexSrtMtx_S, MakeTexSrtMtx_R, MakeTexSrtMtx_T,
                   MakeTexSrtMtx_SR, MakeTexSrtMtx_RT, MakeTexSrtMtx_ST,
                   MakeTexSrtMtx_SRT, ProductTexSrtMtx_S, ProductTexSrtMtx_R,
                   ProductTexSrtMtx_T, ProductTexSrtMtx_SR, ProductTexSrtMtx_RT,
                   ProductTexSrtMtx_ST, ProductTexSrtMtx_SRT);

bool CalcTexMtx_Maya(math::MTX34* pMtx, bool bSet, const TexSrt& rSrt, TexSrt::Flag flag) {
    u32 idx = DECOMP_PPC_RLWINM(flag, 31, 29, 31);

    if (idx == 0x7) {
        return false;
    }

    if (bSet) {
        lbl_eu_8051D6C0[idx](pMtx, rSrt);
    } else {
        lbl_eu_8051D6DC[idx](pMtx, rSrt);
    }

    pMtx->m[2][0] = lbl_eu_80669C98;
    pMtx->m[2][1] = lbl_eu_80669C98;
    pMtx->m[2][2] = lbl_eu_80669C9C;
    pMtx->m[2][3] = lbl_eu_80669C98;

    return true;
}

u32 CalcWorldMtx_Maya_SSC_Apply(math::MTX34* pW, math::VEC3* pS, const math::MTX34* pW1,
                                 const math::VEC3* pS1, u32 attr,
                                 const ChrAnmResult* pResult) {
    u32 flags = pResult->flags;
    u32 newAttr = attr;

    if ((flags & ChrAnmResult::FLAG_ROT_TRANS_ZERO) ||
        (flags & ChrAnmResult::FLAG_MTX_IDENT)) {
        math::MTX34Copy(pW, pW1);
    } else if (flags & ChrAnmResult::FLAG_ROT_ZERO) {
        if (detail::WorldMtxAttr::IsScaleOne(attr)) {
            math::VEC3 trans(pResult->rt._03, pResult->rt._13, pResult->rt._23);
            math::MTX34Trans(pW, pW1, &trans);
        } else {
            math::VEC3 trans(pS1->x * pResult->rt._03, pS1->y * pResult->rt._13,
                             pS1->z * pResult->rt._23);
            math::MTX34Trans(pW, pW1, &trans);
        }
    } else if (detail::WorldMtxAttr::IsScaleOne(attr)) {
        math::MTX34Mult(pW, pW1, &pResult->rt);
    } else {
        math::MTX34 temp;
        math::MTX34Copy(&temp, &pResult->rt);
        temp._03 *= pS1->x;
        temp._13 *= pS1->y;
        temp._23 *= pS1->z;
        math::MTX34Mult(pW, pW1, &temp);
    }

    if (flags & ChrAnmResult::FLAG_SCALE_ONE) {
        newAttr = detail::WorldMtxAttr::AnmScaleOne(newAttr);
        pS->x = pS->y = pS->z = 1.0f;
    } else {
        newAttr = detail::WorldMtxAttr::AnmNotScaleOne(newAttr);
        *pS = pResult->s;
    }

    if (flags & ChrAnmResult::FLAG_SCALE_UNIFORM) {
        newAttr = detail::WorldMtxAttr::AnmScaleUniform(newAttr);
    } else {
        newAttr = detail::WorldMtxAttr::AnmNotScaleUniform(newAttr);
    }

    return newAttr;
}

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r

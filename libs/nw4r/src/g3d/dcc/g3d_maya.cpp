#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {
namespace {

void MakeTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    pMtx->m[0][0] = su;
    pMtx->m[0][1] = 0.0f;
    pMtx->m[0][2] = 0.0f;
    pMtx->m[0][3] = 0.0f;
    pMtx->m[1][0] = 0.0f;
    pMtx->m[1][1] = sv;
    pMtx->m[1][2] = 0.0f;
    pMtx->m[1][3] = 1.0f - sv;
}

void MakeTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = 0.0f;
    pMtx->m[0][3] = 0.5f * (1.0f - cosR - sinR);
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = 0.0f;
    pMtx->m[1][3] = 0.5f * (1.0f - cosR + sinR);
}

void MakeTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = 1.0f;
    pMtx->m[0][1] = 0.0f;
    pMtx->m[0][2] = 0.0f;
    pMtx->m[0][3] = -rSrt.Tu;
    pMtx->m[1][0] = 0.0f;
    pMtx->m[1][1] = 1.0f;
    pMtx->m[1][2] = 0.0f;
    pMtx->m[1][3] = rSrt.Tv;
}

void MakeTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][1] = su * sinR;
    pMtx->m[0][2] = 0.0f;
    pMtx->m[0][3] = 0.5f * su * (1.0f - cosR - sinR);
    pMtx->m[1][0] = -sv * sinR;
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = 0.0f;
    pMtx->m[1][3] = 1.0f - 0.5f * sv * (1.0f - cosR + sinR);
}

void MakeTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = 0.0f;
    pMtx->m[0][3] = 0.5f * (1.0f - cosR - sinR) - tu;
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = 0.0f;
    pMtx->m[1][3] = tv + 0.5f * (1.0f - cosR + sinR);
}

void MakeTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su;
    pMtx->m[0][1] = 0.0f;
    pMtx->m[0][2] = 0.0f;
    pMtx->m[0][3] = -su * tu;
    pMtx->m[1][0] = 0.0f;
    pMtx->m[1][1] = sv;
    pMtx->m[1][2] = 0.0f;
    pMtx->m[1][3] = tv + sv * (1.0f - tv);
}

void MakeTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][1] = su * sinR;
    pMtx->m[0][2] = 0.0f;
    pMtx->m[0][3] = 0.5f * su * (1.0f - cosR - sinR) - tu;
    pMtx->m[1][0] = -sv * sinR;
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = 0.0f;
    pMtx->m[1][3] = tv - 0.5f * sv * (1.0f - cosR + sinR);
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
    pMtx->m[1][3] = m13 * sv + (1.0f - sv);
}

void ProductTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] = m00 * cosR + m01 * (-sinR);
    pMtx->m[0][1] = m00 * sinR + m01 * cosR;
    pMtx->m[0][2] = m02;
    pMtx->m[0][3] = m03;
    pMtx->m[1][0] = m10 * cosR + m11 * (-sinR);
    pMtx->m[1][1] = m10 * sinR + m11 * cosR;
    pMtx->m[1][2] = m12;
    pMtx->m[1][3] = m13;
}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] -= rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

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

    f32 cosR_su = cosR * su;
    f32 sinR_su = sinR * su;
    f32 cosR_sv = cosR * sv;
    f32 sinR_sv = sinR * sv;

    pMtx->m[0][0] = m00 * cosR_su + m01 * (-sinR_sv);
    pMtx->m[0][1] = m00 * sinR_su + m01 * cosR_sv;
    pMtx->m[0][2] = m02;
    pMtx->m[0][3] = m03;
    pMtx->m[1][0] = m10 * cosR_su + m11 * (-sinR_sv);
    pMtx->m[1][1] = m10 * sinR_su + m11 * cosR_sv;
    pMtx->m[1][2] = m12;
    pMtx->m[1][3] = m13;
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] = m00 * cosR + m01 * (-sinR);
    pMtx->m[0][1] = m00 * sinR + m01 * cosR;
    pMtx->m[0][2] = m02;
    pMtx->m[0][3] = m03 - tu;
    pMtx->m[1][0] = m10 * cosR + m11 * (-sinR);
    pMtx->m[1][1] = m10 * sinR + m11 * cosR;
    pMtx->m[1][2] = m12;
    pMtx->m[1][3] = m13 + tv;
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
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    f32 cosR_su = cosR * su;
    f32 sinR_su = sinR * su;
    f32 cosR_sv = cosR * sv;
    f32 sinR_sv = sinR * sv;

    pMtx->m[0][0] = m00 * cosR_su + m01 * (-sinR_sv);
    pMtx->m[0][1] = m00 * sinR_su + m01 * cosR_sv;
    pMtx->m[0][2] = m02;
    pMtx->m[0][3] = m03 - tu;
    pMtx->m[1][0] = m10 * cosR_su + m11 * (-sinR_sv);
    pMtx->m[1][1] = m10 * sinR_su + m11 * cosR_sv;
    pMtx->m[1][2] = m12;
    pMtx->m[1][3] = m13 + tv;
}

} // namespace

bool CalcTexMtx_Maya(math::MTX34* pMtx, bool bSet, const TexSrt& rSrt, TexSrt::Flag flag) {
    u32 idx = (flag >> 28) & 0x7;

    if (idx == 0x7) {
        return false;
    }

    switch (idx) {
    case 0:
        if (bSet) {
            MakeTexSrtMtx_SRT(pMtx, rSrt);
        } else {
            ProductTexSrtMtx_SRT(pMtx, rSrt);
        }
        break;
    case 1:
        if (bSet) {
            MakeTexSrtMtx_RT(pMtx, rSrt);
        } else {
            ProductTexSrtMtx_RT(pMtx, rSrt);
        }
        break;
    case 2:
        if (bSet) {
            MakeTexSrtMtx_ST(pMtx, rSrt);
        } else {
            ProductTexSrtMtx_ST(pMtx, rSrt);
        }
        break;
    case 3:
        if (bSet) {
            MakeTexSrtMtx_T(pMtx, rSrt);
        } else {
            ProductTexSrtMtx_T(pMtx, rSrt);
        }
        break;
    case 4:
        if (bSet) {
            MakeTexSrtMtx_SR(pMtx, rSrt);
        } else {
            ProductTexSrtMtx_SR(pMtx, rSrt);
        }
        break;
    case 5:
        if (bSet) {
            MakeTexSrtMtx_R(pMtx, rSrt);
        } else {
            ProductTexSrtMtx_R(pMtx, rSrt);
        }
        break;
    case 6:
        if (bSet) {
            MakeTexSrtMtx_S(pMtx, rSrt);
        } else {
            ProductTexSrtMtx_S(pMtx, rSrt);
        }
        break;
    }

    pMtx->m[2][0] = 0.0f;
    pMtx->m[2][1] = 0.0f;
    pMtx->m[2][2] = 1.0f;
    pMtx->m[2][3] = 0.0f;

    return true;
}

u32 CalcWorldMtx_Maya_SSC_Apply(math::MTX34* pW, math::VEC3* pS, const math::MTX34* pW1,
                                 const math::VEC3* pS1, u32 attr,
                                 const ChrAnmResult* pResult) {
    u32 flags = pResult->flags;
    u32 newAttr = attr;

    if ((flags & ChrAnmResult::FLAG_MTX_IDENT) ||
        (flags & ChrAnmResult::FLAG_ROT_TRANS_ZERO)) {
        if (detail::WorldMtxAttr::IsScaleOne(attr)) {
            math::MTX34Copy(pW, pW1);
        } else {
            math::MTX34Scale(pW, pW1, pS1);
        }
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

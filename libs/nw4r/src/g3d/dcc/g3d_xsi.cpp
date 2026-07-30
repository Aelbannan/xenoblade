#include <nw4r/g3d.h>

// XSI DCC float constants (SDA21)
extern "C" const float lbl_eu_80669CB0; // 0.0f
extern "C" const float lbl_eu_80669CB4; // 1.0f
extern "C" const float lbl_eu_80669CB8; // 256.0f/360.0f (deg_to_fidx)

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {
namespace {

#pragma dont_inline on
void MakeTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = rSrt.Su;
    pMtx->m[0][1] = lbl_eu_80669CB0;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = lbl_eu_80669CB0;
    pMtx->m[1][0] = lbl_eu_80669CB0;
    pMtx->m[1][1] = rSrt.Sv;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 - rSrt.Sv;
}

void MakeTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    pMtx->m[0][0] = sinR;
    pMtx->m[0][1] = -cosR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = cosR;
    pMtx->m[1][0] = cosR;
    pMtx->m[1][1] = sinR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 - sinR;
}

void MakeTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = lbl_eu_80669CB4;
    pMtx->m[0][1] = lbl_eu_80669CB0;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = -rSrt.Tu;
    pMtx->m[1][0] = lbl_eu_80669CB0;
    pMtx->m[1][1] = lbl_eu_80669CB4;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = rSrt.Tv;
}

void MakeTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    pMtx->m[0][0] = su * sinR;
    pMtx->m[0][1] = -su * cosR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = su * cosR;
    pMtx->m[1][0] = sv * cosR;
    pMtx->m[1][1] = sv * sinR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 - sv * sinR;
}

void MakeTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = sinR;
    pMtx->m[0][1] = -cosR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = cosR * (lbl_eu_80669CB4 - tu) - sinR * tv;
    pMtx->m[1][0] = cosR;
    pMtx->m[1][1] = sinR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 - sinR + cosR * tu - sinR * tv;
}

void MakeTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su;
    pMtx->m[0][1] = lbl_eu_80669CB0;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = -su * tu;
    pMtx->m[1][0] = lbl_eu_80669CB0;
    pMtx->m[1][1] = sv;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = sv * (lbl_eu_80669CB4 - tv) + tv;
}

void MakeTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su * sinR;
    pMtx->m[0][1] = -su * cosR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = -su * cosR * tu - sv * sinR * tv + su * cosR;
    pMtx->m[1][0] = sv * cosR;
    pMtx->m[1][1] = sv * sinR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 - sv * sinR - sv * cosR * tu + su * sinR * tv;
}

void ProductTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sv = rSrt.Sv;
    f32 su = rSrt.Su;

    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] *= su;
    pMtx->m[1][0] *= sv;
    pMtx->m[1][1] *= sv;
    pMtx->m[1][2] *= sv;
    pMtx->m[1][3] = sv * (m13 - lbl_eu_80669CB4) + lbl_eu_80669CB4;
}

void ProductTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] = m00 * sinR + m01 * cosR;
    pMtx->m[0][1] = m00 * (-cosR) + m01 * sinR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = pMtx->m[0][3];
    pMtx->m[1][0] = m10 * sinR + m11 * cosR;
    pMtx->m[1][1] = m10 * (-cosR) + m11 * sinR;
    pMtx->m[1][2] = pMtx->m[1][2];
    pMtx->m[1][3] = m13 + sinR - lbl_eu_80669CB4;
}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] -= rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] = m00 * su * sinR + m01 * sv * cosR;
    pMtx->m[0][1] = m00 * (-su * cosR) + m01 * sv * sinR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = m03 + cosR * sv;
    pMtx->m[1][0] = m10 * su * sinR + m11 * sv * cosR;
    pMtx->m[1][1] = m10 * (-su * cosR) + m11 * sv * sinR;
    pMtx->m[1][2] = pMtx->m[1][2];
    pMtx->m[1][3] = m13 + lbl_eu_80669CB4 - sv * sinR;
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] = m00 * sinR + m01 * cosR;
    pMtx->m[0][1] = m00 * (-cosR) + m01 * sinR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = m03 - tu;
    pMtx->m[1][0] = m10 * sinR + m11 * cosR;
    pMtx->m[1][1] = m10 * (-cosR) + m11 * sinR;
    pMtx->m[1][2] = pMtx->m[1][2];
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
    pMtx->m[1][3] = pMtx->m[1][3] * sv + sv * (lbl_eu_80669CB4 - tv) + tv - sv;
}

void ProductTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = rSrt.R * lbl_eu_80669CB8;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m13 = pMtx->m[1][3];

    pMtx->m[0][0] = m00 * su * sinR + m01 * sv * cosR;
    pMtx->m[0][1] = m00 * (-su * cosR) + m01 * sv * sinR;
    pMtx->m[0][2] = pMtx->m[0][2];
    pMtx->m[0][3] = m03 - tu;
    pMtx->m[1][0] = m10 * su * sinR + m11 * sv * cosR;
    pMtx->m[1][1] = m10 * (-su * cosR) + m11 * sv * sinR;
    pMtx->m[1][2] = pMtx->m[1][2];
    pMtx->m[1][3] = m13 + sv * (tv - lbl_eu_80669CB4);
}
#pragma dont_inline reset

typedef void (*TexSrtMtxFunc)(math::MTX34* pMtx, const TexSrt& rSrt);

static const TexSrtMtxFunc MakeTexSrtMtxTable[] = {
    MakeTexSrtMtx_SRT, MakeTexSrtMtx_RT,  MakeTexSrtMtx_ST, MakeTexSrtMtx_T,
    MakeTexSrtMtx_SR,  MakeTexSrtMtx_R,   MakeTexSrtMtx_S,
};

static const TexSrtMtxFunc ProductTexSrtMtxTable[] = {
    ProductTexSrtMtx_SRT, ProductTexSrtMtx_RT,  ProductTexSrtMtx_ST, ProductTexSrtMtx_T,
    ProductTexSrtMtx_SR,  ProductTexSrtMtx_R,   ProductTexSrtMtx_S,
};

} // namespace

bool CalcTexMtx_Xsi(math::MTX34* pMtx, bool bSet, const TexSrt& rSrt, TexSrt::Flag flag) {
    u32 idx = (flag >> 28) & 0x7;

    if (idx == 0x7) {
        return false;
    }

    if (bSet) {
        MakeTexSrtMtxTable[idx](pMtx, rSrt);
    } else {
        ProductTexSrtMtxTable[idx](pMtx, rSrt);
    }

    pMtx->m[2][0] = lbl_eu_80669CB0;
    pMtx->m[2][1] = lbl_eu_80669CB0;
    pMtx->m[2][2] = lbl_eu_80669CB4;
    pMtx->m[2][3] = lbl_eu_80669CB0;

    return true;
}

u32 CalcWorldMtx_Xsi(math::MTX34* pW, math::VEC3* pS, const math::MTX34* pW1,
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
        *pS = *pS1;
    } else {
        newAttr = detail::WorldMtxAttr::AnmNotScaleOne(newAttr);
        pS->x = pS1->x * pResult->s.x;
        pS->y = pS1->y * pResult->s.y;
        pS->z = pS1->z * pResult->s.z;
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

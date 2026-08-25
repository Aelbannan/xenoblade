#include <nw4r/g3d.h>
#include <decomp.h>

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

    // XSI rotation: cos on the diagonal, translation = (sin, 1 - cos)
    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = -sinR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = sinR;
    pMtx->m[1][0] = sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 - cosR;
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
    f32 fidx = lbl_eu_80669CB8 * rSrt.R;
    f32 cosR, sinR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = cosR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][1] = -sinR;
    pMtx->m[0][3] = sinR - cosR * tu - sinR * tv;
    pMtx->m[1][0] = sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 + (-cosR - sinR * tu + cosR * tv);
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
    pMtx->m[1][3] = lbl_eu_80669CB4 + sv * (tv - lbl_eu_80669CB4);
}

void MakeTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = lbl_eu_80669CB8 * rSrt.R;
    f32 cosR, sinR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][1] = -(su * sinR);
    pMtx->m[0][3] = su * sinR - su * cosR * tu - su * sinR * tv;
    pMtx->m[1][0] = sv * sinR;
    pMtx->m[1][1] = sv * cosR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 + (-sv * cosR - sv * sinR * tu + sv * cosR * tv);
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
    f32 fidx = lbl_eu_80669CB8 * rSrt.R;
    f32 cosR, sinR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m00 = pMtx->m[0][0];
    f32 m10 = pMtx->m[1][0];
    f32 m01 = pMtx->m[0][1];
    f32 m11 = pMtx->m[1][1];
    f32 m02 = pMtx->m[0][2];
    f32 m12 = pMtx->m[1][2];
    f32 m03 = pMtx->m[0][3];
    f32 m13 = pMtx->m[1][3];

    // Rotate columns 0-2 about Z; translation column mixes in the rotation terms
    pMtx->m[0][0] = cosR * m00 - sinR * m10;
    pMtx->m[1][0] = sinR * m00 + cosR * m10;
    pMtx->m[0][1] = cosR * m01 - sinR * m11;
    pMtx->m[1][1] = sinR * m01 + cosR * m11;
    pMtx->m[0][2] = cosR * m02 - sinR * m12;
    pMtx->m[1][2] = sinR * m02 + cosR * m12;
    pMtx->m[0][3] = sinR + (cosR * m03 - sinR * m13);
    pMtx->m[1][3] = lbl_eu_80669CB4 + (sinR * m03 + cosR * m13 - cosR);
}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] -= rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = lbl_eu_80669CB8 * rSrt.R;
    f32 cosR, sinR;
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

    // Row 0 scaled by (su rotated), row 1 by (sv rotated)
    pMtx->m[1][3] = lbl_eu_80669CB4 + (sv * sinR * m03 + sv * cosR * m13 - sv * cosR);
    pMtx->m[0][0] = su * cosR * m00 - su * sinR * m10;
    pMtx->m[0][3] = su * sinR + su * cosR * m03 - su * sinR * m13;
    pMtx->m[0][1] = su * cosR * m01 - su * sinR * m11;
    pMtx->m[1][0] = sv * sinR * m00 + sv * cosR * m10;
    pMtx->m[1][1] = sv * sinR * m01 + sv * cosR * m11;
    pMtx->m[0][2] = su * cosR * m02 - su * sinR * m12;
    pMtx->m[1][2] = sv * sinR * m02 + sv * cosR * m12;
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 fidx = lbl_eu_80669CB8 * rSrt.R;
    f32 cosR, sinR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 m00 = pMtx->m[0][0];
    f32 m10 = pMtx->m[1][0];
    f32 m01 = pMtx->m[0][1];
    f32 m11 = pMtx->m[1][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    // Rotate columns 0-2 about Z, then apply the translation offsets
    pMtx->m[0][0] = cosR * m00 - sinR * m10;
    pMtx->m[1][0] = sinR * m00 + cosR * m10;
    pMtx->m[0][1] = cosR * m01 - sinR * m11;
    pMtx->m[1][1] = sinR * m01 + cosR * m11;
    pMtx->m[0][2] = cosR * m02 - sinR * m12;
    pMtx->m[1][2] = sinR * m02 + cosR * m12;
    pMtx->m[0][3] = sinR + (cosR * (m03 - tu) - sinR * (m13 + tv));
    pMtx->m[1][3] = lbl_eu_80669CB4 + (sinR * (m03 - tu) + cosR * (m13 + tv) - cosR);
}

void ProductTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sv = rSrt.Sv;
    f32 su = rSrt.Su;

    f32 m13 = pMtx->m[1][3];
    f32 m03 = pMtx->m[0][3];

    pMtx->m[0][0] *= su;
    pMtx->m[0][1] *= su;
    pMtx->m[0][2] *= su;
    pMtx->m[0][3] = su * (m03 - rSrt.Tu);
    pMtx->m[1][0] *= sv;
    pMtx->m[1][1] *= sv;
    pMtx->m[1][2] *= sv;
    pMtx->m[1][3] = lbl_eu_80669CB4 + sv * (m13 + rSrt.Tv - lbl_eu_80669CB4);
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

} // namespace

// Load-bearing keep-alive: these functions are referenced only through the
// retail dispatch tables lbl_eu_8051D6F8/lbl_eu_8051D714 (nw4r_data.s), which
// are invisible to the compiler; without this emitter -ipa file dead-strips
// all 14 retail-present functions (attempts.jsonl forceactive-cleanup-g3d-xsi).
DECOMP_FORCEACTIVE(g3d_xsi_cpp,
                   MakeTexSrtMtx_S, MakeTexSrtMtx_R, MakeTexSrtMtx_T,
                   MakeTexSrtMtx_SR, MakeTexSrtMtx_RT, MakeTexSrtMtx_ST,
                   MakeTexSrtMtx_SRT, ProductTexSrtMtx_S, ProductTexSrtMtx_R,
                   ProductTexSrtMtx_T, ProductTexSrtMtx_SR, ProductTexSrtMtx_RT,
                   ProductTexSrtMtx_ST, ProductTexSrtMtx_SRT);

bool CalcTexMtx_Xsi(math::MTX34* pMtx, bool bSet, const TexSrt& rSrt,
                    TexSrt::Flag flag) {
    u32 idx = DECOMP_PPC_RLWINM(flag, 31, 29, 31);

    if (idx == 0x7) {
        return false;
    }

    if (bSet) {
        lbl_eu_8051D6F8[idx](pMtx, rSrt);
    } else {
        lbl_eu_8051D714[idx](pMtx, rSrt);
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

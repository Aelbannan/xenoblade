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
    f32 r = rSrt.R;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, lbl_eu_80669CA0 * r);

    // translation terms: t = 0.5*cos, h = 0.5*sin, base = 0.5 - t
    f32 t = lbl_eu_80669CA4 * cosR;
    f32 h = lbl_eu_80669CA4 * sinR;
    f32 base = lbl_eu_80669CA4 - t;

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = base - h;
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = base + h;
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
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CA0 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    // Split decl/assign: declaration order fixes temp register colors,
    // assignment order fixes emission order.
    // Split decl/assign: declaration order fixes temp register colors,
    // assignment order fixes emission order.
    f32 sv, su, svs, suc, sus, svc;
    su = rSrt.Su;
    sv = rSrt.Sv;
    svc = sv * cosR;
    svs = sv * sinR;
    suc = su * cosR;
    sus = su * sinR;

    pMtx->m[0][0] = suc;
    pMtx->m[0][1] = sus;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = lbl_eu_80669CA8 * (sus + suc - su);
    pMtx->m[1][0] = -svs;
    pMtx->m[1][1] = svc;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = lbl_eu_80669C9C + lbl_eu_80669CA4 * (svs - svc - sv);
}

void MakeTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CA0 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    // t = 0.5*cos and h = 0.5*sin feed both translation terms;
    // base = 0.5 - t. Named temporaries are required: MWCC does not CSE
    // duplicated float subproducts across statements in this TU.
    f32 t = lbl_eu_80669CA4 * cosR;
    f32 h = lbl_eu_80669CA4 * sinR;
    f32 base = lbl_eu_80669CA4 - t;

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = sinR;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = (base - h) - rSrt.Tu;
    pMtx->m[1][0] = -sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = rSrt.Tv + (base + h);
}

void MakeTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = rSrt.Su;
    pMtx->m[0][1] = lbl_eu_80669C98;
    pMtx->m[0][2] = lbl_eu_80669C98;
    pMtx->m[0][3] = -rSrt.Su * rSrt.Tu;
    pMtx->m[1][0] = lbl_eu_80669C98;
    pMtx->m[1][1] = rSrt.Sv;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = lbl_eu_80669C9C + rSrt.Sv * (rSrt.Tv - lbl_eu_80669C9C);
}

void MakeTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CA0 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;

    // hs = 0.5*sin, u = -0.5*cos; the 0.5 subtraction comes after both products
    f32 hs = lbl_eu_80669CA4 * sinR;
    f32 u = lbl_eu_80669CA8 * cosR;
    f32 t = hs - lbl_eu_80669CA4;

    // read late so the loads schedule where retail has them
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    pMtx->m[0][0] = su * cosR;
    pMtx->m[0][2] = lbl_eu_80669C98;

    // both translation offsets share the (u -/+ t) forms
    f32 lo = u - t;
    f32 hi = u + t;

    pMtx->m[0][1] = su * sinR;
    pMtx->m[0][3] = su * (lo - tu);
    pMtx->m[1][0] = -sv * sinR;
    pMtx->m[1][2] = lbl_eu_80669C98;
    pMtx->m[1][3] = lbl_eu_80669C9C + sv * (tv + hi);
    pMtx->m[1][1] = sv * cosR;
}
void ProductTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] *= rSrt.Su;
    pMtx->m[0][1] *= rSrt.Su;
    pMtx->m[0][2] *= rSrt.Su;
    pMtx->m[0][3] *= rSrt.Su;
    pMtx->m[1][0] *= rSrt.Sv;
    pMtx->m[1][1] *= rSrt.Sv;
    pMtx->m[1][2] *= rSrt.Sv;
    pMtx->m[1][3] = (lbl_eu_80669C9C + pMtx->m[1][3] * rSrt.Sv) - rSrt.Sv;
}

void ProductTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CA0 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 h = lbl_eu_80669CA4;

    // Rotate each column pair in place; the translation column also folds in
    // the Maya half-texel pivot correction h*(1-cos-sin). Column results are
    // held in temps so every matrix element is read before any write.
    f32 a0 = pMtx->m[0][0];
    f32 b0 = pMtx->m[1][0];
    f32 t00 = cosR * a0 + sinR * b0;
    f32 t10 = -sinR * a0 + cosR * b0;

    f32 hc = h * cosR;
    f32 hs = h * sinR;
    f32 base = h - hc;

    pMtx->m[0][0] = t00;
    pMtx->m[1][0] = t10;

    f32 a1 = pMtx->m[0][1];
    f32 b1 = pMtx->m[1][1];
    pMtx->m[0][1] = cosR * a1 + sinR * b1;
    pMtx->m[1][1] = -sinR * a1 + cosR * b1;

    f32 a2 = pMtx->m[0][2];
    f32 b2 = pMtx->m[1][2];
    pMtx->m[0][2] = cosR * a2 + sinR * b2;
    pMtx->m[1][2] = -sinR * a2 + cosR * b2;

    f32 a3 = pMtx->m[0][3];
    f32 b3 = pMtx->m[1][3];
    pMtx->m[0][3] = base + (cosR * a3 + sinR * b3) - hs;
    pMtx->m[1][3] = hs + (base + (-sinR * a3 + cosR * b3));
}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] += -rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CA0 * r;
    // Split decl/assign: declaration order fixes temp register colors,
    // assignment order fixes emission order.
    f32 svs, svc, half, m13, m03, svh, su, m13h, m00, m03h, m10, suc, m01,
        sus, m11, m02, m12;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    // Row 0 is scaled by Su, row 1 by Sv; the translation column folds in
    // the Maya half-texel pivot correction before rotating.

    svs = sinR * rSrt.Sv;
    svc = cosR * rSrt.Sv;
    half = lbl_eu_80669CA4;
    m13 = pMtx->m[1][3];
    m03 = pMtx->m[0][3];
    svh = half * rSrt.Sv;
    su = rSrt.Su;
    m13h = m13 - half;
    m00 = pMtx->m[0][0];
    m03h = m03 - half;
    m10 = pMtx->m[1][0];
    suc = cosR * su;
    m01 = pMtx->m[0][1];
    sus = sinR * su;
    m11 = pMtx->m[1][1];
    m02 = pMtx->m[0][2];
    m12 = pMtx->m[1][2];

    pMtx->m[1][3] = (-svs * m03h + svc * m13h - svh) + lbl_eu_80669C9C;
    pMtx->m[1][0] = -svs * m00 + svc * m10;
    pMtx->m[0][0] = suc * m00 + sus * m10;
    pMtx->m[0][1] = suc * m01 + sus * m11;
    pMtx->m[1][1] = -svs * m01 + svc * m11;
    pMtx->m[0][2] = suc * m02 + sus * m12;
    pMtx->m[1][2] = -svs * m02 + svc * m12;
    pMtx->m[0][3] = half * su + (suc * m03h + sus * m13h);
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, lbl_eu_80669CA0 * rSrt.R);

    // pMtx = T(-Tu, Tv) * R_maya * pMtx; R_maya is the transposed Maya
    // rotation with the Maya half-texel pivot correction.
    f32 m00 = pMtx->m[0][0];
    f32 m10 = pMtx->m[1][0];
    f32 m01 = pMtx->m[0][1];
    f32 m11 = pMtx->m[1][1];
    f32 m02 = pMtx->m[0][2];
    f32 m12 = pMtx->m[1][2];
    f32 m03 = pMtx->m[0][3];
    f32 m13 = pMtx->m[1][3];

    // One syntactic mention per pooled constant -> one SDA reloc each.
    f32 half = lbl_eu_80669CA4;
    f32 neghalf = lbl_eu_80669CA8;

    pMtx->m[0][0] = cosR * m00 + sinR * m10;
    pMtx->m[1][0] = -sinR * m00 + cosR * m10;

    f32 h = half * sinR;
    f32 hcos = neghalf * cosR;
    f32 base = h - half;

    pMtx->m[0][1] = cosR * m01 + sinR * m11;
    pMtx->m[1][1] = -sinR * m01 + cosR * m11;
    pMtx->m[0][2] = cosR * m02 + sinR * m12;
    pMtx->m[1][2] = -sinR * m02 + cosR * m12;
    pMtx->m[0][3] = hcos + (cosR * m03 + sinR * m13) - base - rSrt.Tu;
    pMtx->m[1][3] = rSrt.Tv + (base + (hcos + (-sinR * m03 + cosR * m13)));
}

void ProductTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 b = pMtx->m[0][3] - rSrt.Tu;

    f32 n10 = pMtx->m[1][0] * rSrt.Sv;
    pMtx->m[0][0] *= rSrt.Su;
    pMtx->m[0][3] = rSrt.Su * b;
    pMtx->m[1][0] = n10;
    pMtx->m[0][1] *= rSrt.Su;
    pMtx->m[0][2] *= rSrt.Su;
    pMtx->m[1][1] *= rSrt.Sv;
    pMtx->m[1][2] *= rSrt.Sv;
    pMtx->m[1][3] =
        (pMtx->m[1][3] + rSrt.Tv - lbl_eu_80669C9C) * rSrt.Sv + lbl_eu_80669C9C;
}

void ProductTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 r = rSrt.R;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, lbl_eu_80669CA0 * r);

    // Split decl/assign: declaration order fixes temp register colors,
    // assignment order fixes emission order (same idiom as ProductTexSrtMtx_SR).
    f32 half, sv_sin, m13, m13_5, tv, su, tv_5, tu, m03, sv_neg, m10,
        tu_5, sv_cos, m00, su_cos, m03_5, m11, m03_p5, su_sin, m12, m02,
        one, sv_tv, sv, m01;

    sv_sin = sinR * rSrt.Sv;
    sv = rSrt.Sv;
    half = lbl_eu_80669CA4;
    m13 = pMtx->m[1][3];
    m13_5 = m13 - half;
    tv = rSrt.Tv;
    su = rSrt.Su;
    tv_5 = half - tv;
    tu = rSrt.Tu;
    m03 = pMtx->m[0][3];
    sv_neg = -sv_sin;
    m10 = pMtx->m[1][0];
    tu_5 = half - tu;
    sv_cos = sv * cosR;
    m00 = pMtx->m[0][0];
    su_cos = su * cosR;
    m03_5 = m03 - half;
    m11 = pMtx->m[1][1];
    m03_p5 = half + m03;
    sv_tv = tv_5 * sv;
    m01 = pMtx->m[0][1];
    su_sin = su * sinR;
    one = lbl_eu_80669C9C;

    f32 t_a = sv_neg * m03_p5;
    f32 t_b = sv_cos * m13_5;
    f32 t_c = su_cos * m00;

    m12 = pMtx->m[1][2];
    m02 = pMtx->m[0][2];

    f32 t_d = t_a + t_b;
    f32 t_e = sv_neg * m00;
    f32 t_f = su_sin * m10;
    f32 t_g = t_d - sv_tv;
    f32 t_h = sv_cos * m10;

    pMtx->m[0][0] = t_c + t_f;
    pMtx->m[1][0] = t_e + t_h;

    f32 t_i = su_cos * m01;
    f32 t_j = su_sin * m11;
    f32 t_k = su_cos * m03_5;
    f32 t_l = su_sin * m13_5;

    pMtx->m[1][3] = one + t_g;
    pMtx->m[0][1] = t_i + t_j;

    f32 t_m = su_cos * m02;
    f32 t_n = su_sin * m12;
    f32 t_o = sv_neg * m01;

    pMtx->m[0][2] = t_m + t_n;

    f32 t_p = sv_cos * m11;
    f32 t_q = sv_cos * m12;
    f32 t_r = t_o + t_p;

    f32 t_s = sv_neg * m02;
    f32 t_t = tu_5 * su;

    pMtx->m[1][1] = t_r;
    pMtx->m[1][2] = t_s + t_q;
    pMtx->m[0][3] = t_t + (t_k + t_l);
}
#pragma dont_inline reset

} // namespace

// Retail dispatch tables (nw4r_data.s .rodata 0x8051D6C0/0x8051D6DC, 7
// entries each in reverse declaration order). Defining them here gives
// -ipa file real references to the 14 TexSrtMtx functions above, replacing
// the former DECOMP_FORCEACTIVE emitter entirely (attempts 61552/61553 and
// forceactive-retire/*: ((used)) and #pragma force_active are both ignored
// by -ipa file DCE, and external linkage would break the @unnamed@g3d_maya_cpp@
// mangled names). The tables must also be removed from nw4r_data.s to avoid
// duplicate globals at link.
extern "C" const TexSrtMtxFunc lbl_eu_8051D6C0[7] = {
    MakeTexSrtMtx_SRT, MakeTexSrtMtx_RT, MakeTexSrtMtx_ST,
    MakeTexSrtMtx_T, MakeTexSrtMtx_SR, MakeTexSrtMtx_R, MakeTexSrtMtx_S,
};
extern "C" const TexSrtMtxFunc lbl_eu_8051D6DC[7] = {
    ProductTexSrtMtx_SRT, ProductTexSrtMtx_RT, ProductTexSrtMtx_ST,
    ProductTexSrtMtx_T, ProductTexSrtMtx_SR, ProductTexSrtMtx_R,
    ProductTexSrtMtx_S,
};

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

    if ((flags & ChrAnmResult::FLAG_MTX_IDENT) ||
        (flags & ChrAnmResult::FLAG_ROT_TRANS_ZERO)) {
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
        pS->x = pS->y = pS->z = lbl_eu_80669C9C;
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

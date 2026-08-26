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
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CB8 * r;
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
    // Declaration order fixes the stack slots: retail passes &sin at r1+0xC
    // and &cos at r1+0x8, so cosR must sit below sinR.
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CB8 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;

    // Statement order drives MWCC's temp register allocation to match retail:
    // Su products first; Sv is read inline so its load schedules after cos.
    f32 su_cos = su * cosR;
    f32 su_sin = su * sinR;
    f32 sv_cos = rSrt.Sv * cosR;
    f32 sv_sin = rSrt.Sv * sinR;

    pMtx->m[0][0] = su_cos;
    pMtx->m[0][1] = -su_sin;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = su_sin;
    pMtx->m[1][0] = sv_sin;
    pMtx->m[1][1] = sv_cos;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 + (-sv_cos);
}

void MakeTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    // Same shape as the matched MakeTexSrtMtx_SR: intermediate r local fixes
    // load scheduling, and declaration order puts cosR at sp+0x8 / sinR at
    // sp+0xC as retail passes them to SinCosFIdx.
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CB8 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    // reverse declaration order mirrors MWCC's register allocation
    f32 tv = rSrt.Tv;
    f32 tu = rSrt.Tu;

    pMtx->m[0][0] = cosR;
    pMtx->m[0][1] = -sinR;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = sinR - cosR * tu - sinR * tv;
    pMtx->m[1][0] = sinR;
    pMtx->m[1][1] = cosR;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 + (-cosR - sinR * tu + cosR * tv);
}

void MakeTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] = rSrt.Su;
    pMtx->m[0][1] = lbl_eu_80669CB0;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[0][3] = (-rSrt.Su) * rSrt.Tu;
    pMtx->m[1][0] = lbl_eu_80669CB0;
    pMtx->m[1][1] = rSrt.Sv;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[1][3] = lbl_eu_80669CB4 + rSrt.Sv * (rSrt.Tv - lbl_eu_80669CB4);
}

void MakeTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CB8 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;

    // Product temps declared in retail creation order.
    f32 sv_cos = sv * cosR;
    f32 sv_sin = sv * sinR;
    f32 su_cos = su * cosR;
    f32 su_sin = su * sinR;
    f32 sv_sin_tu = sv_sin * tu;
    f32 su_cos_tu = su_cos * tu;
    f32 sv_cos_tv = sv_cos * tv;
    f32 su_sin_tv = su_sin * tv;

    pMtx->m[0][0] = su_cos;
    pMtx->m[0][2] = lbl_eu_80669CB0;
    pMtx->m[1][0] = sv_sin;
    pMtx->m[1][1] = sv_cos;
    pMtx->m[1][2] = lbl_eu_80669CB0;
    pMtx->m[0][1] = -su_sin;
    pMtx->m[0][3] = su_sin - su_cos_tu - su_sin_tv;
    pMtx->m[1][3] = lbl_eu_80669CB4 + (-sv_cos - sv_sin_tu + sv_cos_tv);
}

void ProductTexSrtMtx_S(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][0] *= rSrt.Su;
    pMtx->m[0][1] *= rSrt.Su;
    pMtx->m[0][2] *= rSrt.Su;
    pMtx->m[0][3] *= rSrt.Su;
    pMtx->m[1][0] *= rSrt.Sv;
    pMtx->m[1][1] *= rSrt.Sv;
    pMtx->m[1][2] *= rSrt.Sv;
    pMtx->m[1][3] = rSrt.Sv * (pMtx->m[1][3] - lbl_eu_80669CB4) + lbl_eu_80669CB4;
}

void ProductTexSrtMtx_R(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CB8 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    // All elements are read up front; each column pair is rotated and written
    // back before the next.
    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m03 = pMtx->m[0][3];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];
    f32 m13 = pMtx->m[1][3];

    {
        // Product temps force MWCC's multiply order: c*x0, s*x0, s*y0, c*y0.
        f32 p1 = m00 * cosR;
        f32 p2 = m00 * sinR;
        f32 p3 = m10 * sinR;
        f32 p4 = m10 * cosR;
        f32 s1 = p2 + p4;
        f32 s0 = p1 - p3;
        pMtx->m[0][0] = s0;
        pMtx->m[1][0] = s1;
    }
    {
        f32 p1 = m01 * cosR;
        f32 p2 = m01 * sinR;
        f32 p3 = m11 * sinR;
        f32 p4 = m11 * cosR;
        f32 s1 = p2 + p4;
        f32 s0 = p1 - p3;
        pMtx->m[0][1] = s0;
        pMtx->m[1][1] = s1;
    }
    {
        f32 p1 = m02 * cosR;
        f32 p2 = m02 * sinR;
        f32 p3 = m12 * sinR;
        f32 p4 = m12 * cosR;
        f32 s1 = p2 + p4;
        f32 s0 = p1 - p3;
        pMtx->m[0][2] = s0;
        pMtx->m[1][2] = s1;
    }
    {
        // Tail product order: c*m13, s*m03, c*m03, s*m13.
        f32 q1 = m13 * cosR;
        f32 q2 = m03 * sinR;
        f32 q3 = m03 * cosR;
        f32 q4 = m13 * sinR;
        f32 ad = q2 + q1 - cosR;
        f32 sb = q3 - q4;
        f32 b = sinR + sb;
        f32 a = lbl_eu_80669CB4 + ad;
        pMtx->m[0][3] = b;
        pMtx->m[1][3] = a;
    }}

void ProductTexSrtMtx_T(math::MTX34* pMtx, const TexSrt& rSrt) {
    pMtx->m[0][3] -= rSrt.Tu;
    pMtx->m[1][3] += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, rSrt.R * lbl_eu_80669CB8);

    // Creation order mirrors retail's value sequence to steer MWCC register
    // coloring.
    f32 sv = rSrt.Sv;
    f32 svc = sv * cosR;
    f32 su = rSrt.Su;
    f32 svs = sv * sinR;
    f32 m03 = pMtx->m[0][3];
    f32 sus = su * sinR;
    f32 m13 = pMtx->m[1][3];
    f32 suc = su * cosR;

    // All matrix inputs cached before the first store (aliasing barrier,
    // matches retail's single load per element).
    f32 m00 = pMtx->m[0][0];
    f32 m01 = pMtx->m[0][1];
    f32 m02 = pMtx->m[0][2];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m12 = pMtx->m[1][2];

    // Store order matches retail: m13, m00, m03, m10, m01, m11, m02, m12.
    pMtx->m[1][3] = lbl_eu_80669CB4 + (svs * m03 + svc * m13 - svc);
    pMtx->m[0][0] = suc * m00 - sus * m10;
    pMtx->m[0][3] = sus + (suc * m03 - sus * m13);
    pMtx->m[1][0] = svs * m00 + svc * m10;
    pMtx->m[0][1] = suc * m01 - sus * m11;
    pMtx->m[1][1] = svs * m01 + svc * m11;
    pMtx->m[0][2] = suc * m02 - sus * m12;
    pMtx->m[1][2] = svs * m02 + svc * m12;
}

void ProductTexSrtMtx_RT(math::MTX34* pMtx, const TexSrt& rSrt) {
    // Intermediate r local fixes load scheduling of R vs the constant.
    f32 r = rSrt.R;
    f32 fidx = lbl_eu_80669CB8 * r;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    // Each column pair is rotated in place; product temps over direct field
    // reads keep the loads interleaved with multiplies like retail.
    {
        f32 p1 = cosR * pMtx->m[0][0];
        f32 p2 = sinR * pMtx->m[0][0];
        f32 p3 = sinR * pMtx->m[1][0];
        f32 p4 = cosR * pMtx->m[1][0];
        pMtx->m[0][0] = p1 - p3;
        pMtx->m[1][0] = p2 + p4;
    }
    {
        f32 p1 = cosR * pMtx->m[0][1];
        f32 p2 = sinR * pMtx->m[0][1];
        f32 p3 = sinR * pMtx->m[1][1];
        f32 p4 = cosR * pMtx->m[1][1];
        pMtx->m[0][1] = p1 - p3;
        pMtx->m[1][1] = p2 + p4;
    }
    {
        f32 p1 = cosR * pMtx->m[0][2];
        f32 p2 = sinR * pMtx->m[0][2];
        f32 p3 = sinR * pMtx->m[1][2];
        f32 p4 = cosR * pMtx->m[1][2];
        pMtx->m[0][2] = p1 - p3;
        pMtx->m[1][2] = p2 + p4;
    }
    {
        // Column 3: rotate the pivot-relative translation and fold it back.
        f32 a = pMtx->m[0][3] - rSrt.Tu;
        f32 b = pMtx->m[1][3] + rSrt.Tv;
        f32 q1 = cosR * b;
        f32 q2 = sinR * a;
        f32 q3 = cosR * a;
        f32 q4 = sinR * b;
        // Compute the m13 sum first (retail order); stores stay column-ordered
        f32 t13 = lbl_eu_80669CB4 + (q2 + q1 - cosR);
        pMtx->m[0][3] = sinR + (q3 - q4);
        pMtx->m[1][3] = t13;
    }
}

void ProductTexSrtMtx_ST(math::MTX34* pMtx, const TexSrt& rSrt) {
    // Load/eval order mirrors retail: m13, Tv, const, m03, Tu, Sv, then scale
    // column pairs bottom-row/top-row so the scheduler interleaves identically.
    f32 m13 = pMtx->m[1][3];
    f32 tv = rSrt.Tv;
    f32 m03 = pMtx->m[0][3];
    f32 tu = rSrt.Tu;
    f32 sv = rSrt.Sv;

    pMtx->m[1][3] = lbl_eu_80669CB4 + sv * (m13 + tv - lbl_eu_80669CB4);
    pMtx->m[0][3] = rSrt.Su * (m03 - tu);
    pMtx->m[1][0] *= sv;
    pMtx->m[0][0] *= rSrt.Su;
    pMtx->m[1][1] *= sv;
    pMtx->m[0][1] *= rSrt.Su;
    pMtx->m[1][2] *= sv;
    pMtx->m[0][2] *= rSrt.Su;
}

void ProductTexSrtMtx_SRT(math::MTX34* pMtx, const TexSrt& rSrt) {
    f32 rot = rSrt.R;
    f32 su = rSrt.Su;
    f32 sv = rSrt.Sv;
    f32 tu = rSrt.Tu;
    f32 tv = rSrt.Tv;
    f32 fidx = lbl_eu_80669CB8 * rot;
    f32 sinR, cosR;
    math::SinCosFIdx(&sinR, &cosR, fidx);

    // Statement order mirrors the retail schedule: each load/multiply sits
    // where retail emits it, so the scheduler has nothing to reorder.
    f32 c = cosR;
    f32 s = sinR;

    f32 svCos = sv * c;
    f32 svSin = sv * s;

    f32 m03 = pMtx->m[0][3];
    f32 suCos = su * c;
    f32 m13 = pMtx->m[1][3];
    f32 d03 = m03 - tu;
    f32 suSin = su * s;
    f32 m00 = pMtx->m[0][0];
    f32 t13 = m13 + tv;
    f32 m01 = pMtx->m[0][1];
    f32 m10 = pMtx->m[1][0];
    f32 m11 = pMtx->m[1][1];
    f32 m02 = pMtx->m[0][2];
    f32 m12 = pMtx->m[1][2];

    pMtx->m[0][3] = suSin + (suCos * d03 - suSin * t13);
    pMtx->m[0][0] = suCos * m00 - suSin * m01;
    pMtx->m[1][3] = lbl_eu_80669CB4 + ((svSin * d03 + svCos * t13) - svCos);
    pMtx->m[1][0] = svSin * m00 + svCos * m01;
    pMtx->m[0][1] = suCos * m10 - suSin * m11;
    pMtx->m[1][1] = svSin * m10 + svCos * m11;
    pMtx->m[0][2] = suCos * m02 - suSin * m12;
    pMtx->m[1][2] = svSin * m02 + svCos * m12;
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
// The emitter function itself is retail-absent; tools/postprocess_reloc_names.py
// UNIT_RULES drops it from the link (drop_text_symbols, trailing-* prefix match).

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

// XSI dcc path keys scale-one behavior off ATTR_ALL_S_ONE (attr bit 31).
u32 CalcWorldMtx_Xsi(math::MTX34* pW, math::VEC3* pS, const math::MTX34* pW1,
                     const math::VEC3* pS1, u32 attr,
                     const ChrAnmResult* pResult) {
    u32 flags = pResult->flags;
    u32 newAttr = attr;

    // high-bit condition masks set by the XSI result builder
    if ((flags & ChrAnmResult::FLAG_MTX_IDENT) ||
        (flags & ChrAnmResult::FLAG_ROT_TRANS_ZERO)) {
        math::MTX34Copy(pW, pW1);
    } else if (flags & ChrAnmResult::FLAG_ROT_ZERO) {
        if (detail::WorldMtxAttr::IsAllScaleOne(attr)) {
            math::VEC3 trans(pResult->rt._03, pResult->rt._13, pResult->rt._23);
            math::MTX34Trans(pW, pW1, &trans);
        } else {
            math::VEC3 trans(pS1->x * pResult->rt._03, pS1->y * pResult->rt._13,
                             pS1->z * pResult->rt._23);
            math::MTX34Trans(pW, pW1, &trans);
        }
    } else if (detail::WorldMtxAttr::IsAllScaleOne(attr)) {
        math::MTX34Mult(pW, pW1, &pResult->rt);
    } else {
        // Parent scale applied to the local translation before concat.
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

// The literal-pool constants lbl_eu_80669CB0/B4/B8 are owned by the shared
// nw4r_data.s blob in retail (.sdata2 0x248-0x250); this TU references them
// through the extern "C" declarations in include/nw4r/g3d/dcc/g3d_xsi.h.
// Do not define them here: local .sdata2 copies fail the unit data gate
// against the retail split (retail size 0x0 != decomp 0xC).

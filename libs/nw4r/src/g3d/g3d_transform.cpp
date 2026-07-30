#include <nw4r/g3d.h>
#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {

/******************************************************************************
 *
 * 3x3 inverse kernels shared by CalcViewNrmMtx / CalcViewTexMtx /
 * CalcInvWorldMtx.
 *
 * Retail implements these with Gekko paired-single SIMD (psq_l, ps_merge10,
 * ps_mul, ps_msub, ps_madd, fres, ps_nmsub, ps_muls0, ps_cmpo0). The scalar
 * C++ below computes the identical function and is portable to any host
 * toolchain (PC port). It will not byte-match the retail PS instruction
 * stream -- the acceptance target is EQUIVALENT_MATCH, not FULL_MATCH.
 *
 * Two deliberate precision notes versus retail:
 *   - Retail's reciprocal is fres (~6-bit estimate) + one Newton-Raphson step
 *     (~12-bit). We use an exact IEEE divide (1.0f/det, ~24-bit): strictly
 *     more accurate, invisible in-game.
 *   - Retail's determinant/cofactors use fused ps_madd (single rounding); we
 *     use separate fmul/fadd. The singularity epsilon (~1e-36) is so small
 *     that this only ever matters for (near-)singular matrices.
 *
 ******************************************************************************/

namespace {

/**
 * Singularity threshold. Retail literal at lbl_eu_80669B40 = 0x03AA2425
 * (~1e-36). A denormal guard: any meaningful determinant passes; only
 * (near-)zero / denormal determinants are treated as singular.
 */
const f32 INVERSE_EPSILON = 1.0e-36f;

/**
 * Adjugate (transpose of the cofactor matrix) of the upper-left 3x3 of `m`,
 * stored row-major so that, with det = return value:
 *
 *     true inverse       M^-1 [R][C] = adj[3*R + C] / det   (CalcInvWorldMtx)
 *     inverse-transpose  M^-T [R][C] = adj[3*C + R] / det   (Nrm/Tex matrices)
 *
 * @return determinant via first-column cofactor expansion
 *         (det = a*C00 + d*C10 + g*C20).
 */
inline __attribute__((always_inline))
f32 CalcAdjugate(const math::MTX34* m, f32 adj[9]) {
    const f32 a = m->_00, b = m->_01, c = m->_02;
    const f32 d = m->_10, e = m->_11, f = m->_12;
    const f32 g = m->_20, h = m->_21, i = m->_22;

    adj[0] =  (e*i - f*h);  // adjugate[0][0] = C00
    adj[1] =  (c*h - b*i);  // adjugate[0][1] = C10
    adj[2] =  (b*f - e*c);  // adjugate[0][2] = C20
    adj[3] =  (f*g - i*d);  // adjugate[1][0] = C01
    adj[4] =  (a*i - c*g);  // adjugate[1][1] = C11
    adj[5] =  (c*d - f*a);  // adjugate[1][2] = C21
    adj[6] =  (d*h - e*g);  // adjugate[2][0] = C02
    adj[7] =  (b*g - a*h);  // adjugate[2][1] = C12
    adj[8] =  (a*e - b*d);  // adjugate[2][2] = C22

    return a*adj[0] + d*adj[1] + g*adj[2];
}

/**
 * Retail singularity test: ps_abs(det) then ps_cmpo0 (ordered) vs epsilon,
 * branch-to-success on >=. For the branch decision this is identical to
 * |det| >= epsilon, including NaN and +-0 (all classify as singular).
 */
inline __attribute__((always_inline))
bool IsInvertible(f32 det) {
    const f32 absDet = det < 0.0f ? -det : det;
    return absDet >= INVERSE_EPSILON;
}

} // anonymous namespace

/******************************************************************************
 *
 * CalcViewNrmMtx
 *
 * Inverts the 3x3 rotation/scale submatrix of a MTX34 as a NORMAL matrix
 * (M^-T) and writes it to a MTX33.
 *
 * Retail: 0x803E3B14, size 0xE4.
 * On singular input retail copies the input 3x3 into pOut and returns false.
 *
 ******************************************************************************/
bool CalcViewNrmMtx(math::MTX33* pOut, const math::MTX34* pMtx) {
    f32 adj[9];
    const f32 det = CalcAdjugate(pMtx, adj);

    if (!IsInvertible(det)) {
        // Retail singular path: copy input 3x3 -> pOut. Read everything
        // before writing so pOut/pMtx overlap is safe (retail uses the
        // pre-loaded registers for the same reason).
        const f32 a = pMtx->_00, b = pMtx->_01, c = pMtx->_02;
        const f32 d = pMtx->_10, e = pMtx->_11, f = pMtx->_12;
        const f32 g = pMtx->_20, h = pMtx->_21, i = pMtx->_22;
        pOut->_00 = a; pOut->_01 = b; pOut->_02 = c;
        pOut->_10 = d; pOut->_11 = e; pOut->_12 = f;
        pOut->_20 = g; pOut->_21 = h; pOut->_22 = i;
        return false;
    }

    const f32 inv = 1.0f / det;
    // Normal matrix = M^-T: out[R][C] = adj[3*C + R] * inv.
    pOut->_00 = adj[0]*inv; pOut->_01 = adj[3]*inv; pOut->_02 = adj[6]*inv;
    pOut->_10 = adj[1]*inv; pOut->_11 = adj[4]*inv; pOut->_12 = adj[7]*inv;
    pOut->_20 = adj[2]*inv; pOut->_21 = adj[5]*inv; pOut->_22 = adj[8]*inv;
    return true;
}

/******************************************************************************
 *
 * CalcViewTexMtx
 *
 * Inverts the 3x3 submatrix as M^-T into a full MTX34 with the translation
 * column zeroed. Used for view-space texture matrices.
 *
 * Retail: 0x803E3BF8, size 0x108.
 * On singular input: if pOut==pMtx the matrix is left untouched; otherwise
 * the input 3x3 is copied and the translation column zeroed. Returns false.
 *
 ******************************************************************************/
bool CalcViewTexMtx(math::MTX34* pOut, const math::MTX34* pMtx) {
    f32 adj[9];
    const f32 det = CalcAdjugate(pMtx, adj);

    if (!IsInvertible(det)) {
        if (pOut != pMtx) {
            const f32 a = pMtx->_00, b = pMtx->_01, c = pMtx->_02;
            const f32 d = pMtx->_10, e = pMtx->_11, f = pMtx->_12;
            const f32 g = pMtx->_20, h = pMtx->_21, i = pMtx->_22;
            pOut->_00 = a; pOut->_01 = b; pOut->_02 = c; pOut->_03 = 0.0f;
            pOut->_10 = d; pOut->_11 = e; pOut->_12 = f; pOut->_13 = 0.0f;
            pOut->_20 = g; pOut->_21 = h; pOut->_22 = i; pOut->_23 = 0.0f;
        }
        return false;
    }

    const f32 inv = 1.0f / det;
    // M^-T into the 3x3, translation column = 0.
    pOut->_00 = adj[0]*inv; pOut->_01 = adj[3]*inv; pOut->_02 = adj[6]*inv; pOut->_03 = 0.0f;
    pOut->_10 = adj[1]*inv; pOut->_11 = adj[4]*inv; pOut->_12 = adj[7]*inv; pOut->_13 = 0.0f;
    pOut->_20 = adj[2]*inv; pOut->_21 = adj[5]*inv; pOut->_22 = adj[8]*inv; pOut->_23 = 0.0f;
    return true;
}

/******************************************************************************
 *
 * CalcInvWorldMtx
 *
 * Full MTX34 inverse: the TRUE inverse M^-1 of the 3x3 submatrix (NOT
 * transposed) plus the affine translation column -M^-1_3x3 * t.
 *
 * Retail: 0x803E3D00, size 0x104.
 * On singular input retail performs no stores and returns false.
 *
 ******************************************************************************/
bool CalcInvWorldMtx(math::MTX34* pOut, const math::MTX34* pMtx) {
    f32 adj[9];
    const f32 det = CalcAdjugate(pMtx, adj);

    if (!IsInvertible(det)) {
        return false;
    }

    const f32 inv = 1.0f / det;
    // Read translation first (alias-safe even if pOut==pMtx; the 3x3 stores
    // below never touch the _03/_13/_23 slots).
    const f32 t0 = pMtx->_03, t1 = pMtx->_13, t2 = pMtx->_23;

    // True inverse M^-1: out[R][C] = adj[3*R + C] * inv (no transpose).
    const f32 i00 = adj[0]*inv, i01 = adj[1]*inv, i02 = adj[2]*inv;
    const f32 i10 = adj[3]*inv, i11 = adj[4]*inv, i12 = adj[5]*inv;
    const f32 i20 = adj[6]*inv, i21 = adj[7]*inv, i22 = adj[8]*inv;

    pOut->_00 = i00; pOut->_01 = i01; pOut->_02 = i02;
    pOut->_10 = i10; pOut->_11 = i11; pOut->_12 = i12;
    pOut->_20 = i20; pOut->_21 = i21; pOut->_22 = i22;

    // Affine translation column: -M^-1_3x3 * t (retail: ps_mul/ps_madd/ps_nmadd).
    pOut->_03 = -(i00*t0 + i01*t1 + i02*t2);
    pOut->_13 = -(i10*t0 + i11*t1 + i12*t2);
    pOut->_23 = -(i20*t0 + i21*t1 + i22*t2);
    return true;
}

} // namespace detail
} // namespace g3d
} // namespace nw4r

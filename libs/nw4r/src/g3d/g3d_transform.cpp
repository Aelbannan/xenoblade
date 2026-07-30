#include <nw4r/g3d.h>
#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {

/**
 * Singularity threshold for 3x3 matrix inverse.
 * Retail value at lbl_eu_80669B40: 0x03AA2425 (~1e-36).
 * Essentially a denormal guard — any meaningful determinant passes.
 */
static const f32 INVERSE_EPSILON = 1.0e-36f;

/**
 * Shared 3x3 inverse kernel.
 *
 * Computes the inverse of the upper-left 3x3 submatrix of a MTX34
 * using cofactor expansion + determinant check.
 *
 * Retail implements this with paired-single SIMD (psq_l, ps_merge10,
 * ps_mul, ps_msub, ps_madd, fres, ps_nmsub, ps_muls0, ps_cmpo0).
 * The scalar C++ below is mathematically identical.
 *
 * @param cofactors  Output: 9 cofactors (row-major 3x3), unscaled
 * @param det        Output: determinant of the 3x3 submatrix
 * @param pMtx       Input: 4x3 matrix whose 3x3 submatrix is inverted
 * @return true if |det| >= epsilon (inverse exists)
 */
static bool CalcCofactorsAndDet(f32 cofactors[9], f32* det,
                                const math::MTX34* pMtx) {
    const f32 a = pMtx->_00, b = pMtx->_01, c = pMtx->_02;
    const f32 d = pMtx->_10, e = pMtx->_11, f = pMtx->_12;
    const f32 g = pMtx->_20, h = pMtx->_21, i = pMtx->_22;

    // Cofactors (retail: ps_mul / ps_msub pairs)
    // Row 0 of cofactor matrix
    cofactors[0] = e * i - f * h;  // C00
    cofactors[1] = c * h - b * i;  // C10 (note: transposed storage)
    cofactors[2] = b * f - e * c;  // C20

    // Row 1
    cofactors[3] = f * g - i * d;  // C01
    cofactors[4] = a * i - c * g;  // C11
    cofactors[5] = c * d - f * a;  // C21

    // Row 2
    cofactors[6] = d * h - e * g;  // C02
    cofactors[7] = b * g - a * h;  // C12
    cofactors[8] = a * e - b * d;  // C22

    // Determinant via first-column expansion
    // (retail: ps_mul f7,f0,f13 / ps_madd f7,f2,f12,f7 / ps_madd f7,f4,f11,f7)
    *det = a * cofactors[0] + d * cofactors[1] + g * cofactors[2];

    // Singularity check (retail: ps_abs + ps_cmpo0 vs epsilon)
    f32 absDet = *det < 0.0f ? -*det : *det;
    return absDet >= INVERSE_EPSILON;
}

/**
 * Newton-Raphson refined reciprocal.
 * Retail uses fres (hardware estimate, ~6 bits) + one NR step (~12 bits).
 * On PC, 1.0f/x gives full 24-bit precision — strictly more accurate.
 */
static inline f32 FastReciprocal(f32 x) {
#if defined(__MWERKS__) && !defined(NONMATCHING)
    // Match retail: fres + one Newton-Raphson iteration
    register f32 work0, work1, work2;
    ASM (
        fres     work0, x
        ps_add   work1, work0, work0
        ps_mul   work2, work0, work0
        ps_nmsub work0, x, work2, work1
    )
    return work0;
#else
    return 1.0f / x;
#endif
}

/******************************************************************************
 *
 * CalcViewNrmMtx
 *
 * Inverts the 3x3 rotation/scale submatrix of a MTX34 and writes
 * the result to a MTX33. Used for normal-matrix computation.
 *
 * Retail: 0x803E3B14, size 0xE4
 *
 ******************************************************************************/
bool CalcViewNrmMtx(math::MTX33* pOut, const math::MTX34* pMtx) {
    f32 cof[9];
    f32 det;

    if (!CalcCofactorsAndDet(cof, &det, pMtx)) {
        return false;
    }

    f32 invDet = FastReciprocal(det);

    // Scale cofactors → inverse (retail: ps_muls0 sequence)
    // MTX33 layout: _00 _01 _02 / _10 _11 _12 / _20 _21 _22
    pOut->_00 = cof[0] * invDet;
    pOut->_01 = cof[3] * invDet;
    pOut->_02 = cof[6] * invDet;
    pOut->_10 = cof[1] * invDet;
    pOut->_11 = cof[4] * invDet;
    pOut->_12 = cof[7] * invDet;
    pOut->_20 = cof[2] * invDet;
    pOut->_21 = cof[5] * invDet;
    pOut->_22 = cof[8] * invDet;

    return true;
}

/******************************************************************************
 *
 * CalcViewTexMtx
 *
 * Inverts the 3x3 submatrix and writes a full MTX34 with the
 * translation column zeroed. Used for view-space texture matrices.
 *
 * Retail: 0x803E3BF8, size 0x108
 *
 ******************************************************************************/
bool CalcViewTexMtx(math::MTX34* pOut, const math::MTX34* pMtx) {
    f32 cof[9];
    f32 det;

    if (!CalcCofactorsAndDet(cof, &det, pMtx)) {
        // Singular: copy input as-is, zero translation column
        if (pOut != pMtx) {
            math::MTX34Copy(pOut, pMtx);
        }
        pOut->_03 = 0.0f;
        pOut->_13 = 0.0f;
        pOut->_23 = 0.0f;
        return false;
    }

    f32 invDet = FastReciprocal(det);

    pOut->_00 = cof[0] * invDet;
    pOut->_01 = cof[3] * invDet;
    pOut->_02 = cof[6] * invDet;
    pOut->_03 = 0.0f;

    pOut->_10 = cof[1] * invDet;
    pOut->_11 = cof[4] * invDet;
    pOut->_12 = cof[7] * invDet;
    pOut->_13 = 0.0f;

    pOut->_20 = cof[2] * invDet;
    pOut->_21 = cof[5] * invDet;
    pOut->_22 = cof[8] * invDet;
    pOut->_23 = 0.0f;

    return true;
}

/******************************************************************************
 *
 * CalcInvWorldMtx
 *
 * Full MTX34 inverse: inverts the 3x3 submatrix AND computes the
 * translation column as -inv3x3 * t. Used for world→view transforms.
 *
 * Retail: 0x803E3D00, size 0x104
 *
 ******************************************************************************/
bool CalcInvWorldMtx(math::MTX34* pOut, const math::MTX34* pMtx) {
    f32 cof[9];
    f32 det;

    if (!CalcCofactorsAndDet(cof, &det, pMtx)) {
        return false;
    }

    f32 invDet = FastReciprocal(det);

    // Scaled inverse 3x3
    f32 inv00 = cof[0] * invDet;
    f32 inv01 = cof[3] * invDet;
    f32 inv02 = cof[6] * invDet;
    f32 inv10 = cof[1] * invDet;
    f32 inv11 = cof[4] * invDet;
    f32 inv12 = cof[7] * invDet;
    f32 inv20 = cof[2] * invDet;
    f32 inv21 = cof[5] * invDet;
    f32 inv22 = cof[8] * invDet;

    pOut->_00 = inv00;
    pOut->_01 = inv01;
    pOut->_02 = inv02;

    pOut->_10 = inv10;
    pOut->_11 = inv11;
    pOut->_12 = inv12;

    pOut->_20 = inv20;
    pOut->_21 = inv21;
    pOut->_22 = inv22;

    // Translation: -inv3x3 * t  (retail: lfs + ps_mul/ps_madd/ps_nmadd)
    f32 t0 = pMtx->_03;
    f32 t1 = pMtx->_13;
    f32 t2 = pMtx->_23;

    pOut->_03 = -(inv00 * t0 + inv01 * t1 + inv02 * t2);
    pOut->_13 = -(inv10 * t0 + inv11 * t1 + inv12 * t2);
    pOut->_23 = -(inv20 * t0 + inv21 * t1 + inv22 * t2);

    return true;
}

} // namespace detail
} // namespace g3d
} // namespace nw4r

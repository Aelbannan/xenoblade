#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/g3d/g3d_calcview.h>
#include <nw4r/g3d/g3d_workmem.h>
#include <nw4r/ut/ut_LockedCache.h>

// Retail nw4r data-pool constants referenced by the billboard kernels.  Named
// after the retail symbols so the generated sda21 float relocations match
// retail exactly (instead of TU-local pool labels).
extern const f32 lbl_eu_80669C30;  // 1.0f
extern const f32 lbl_eu_80669C34;  // billboard Y-axis degeneracy epsilon
extern const f32 lbl_eu_80669C38;  // adjugate singularity epsilon (f32 view)

namespace nw4r {
namespace g3d {
namespace detail {

#include <nw4r/g3d/detail/g3d_transform_ps.inl>

} // namespace detail

// Maximum number of view matrices
static const u32 G3D_CALCVIEW_MAX_MTX = 32;

namespace {

// Forward declarations
void GetModelLocalAxisY2(math::VEC3*, const math::MTX34*, const math::MTX34*);
void GetModelLocalAxisY3(math::VEC3*, const math::MTX34*, const math::MTX34*);
void Calc_BILLBOARD_STD(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, ResMdl, unsigned long);
void Calc_BILLBOARD_PERSP_STD(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, ResMdl, unsigned long);
void Calc_BILLBOARD_ROT(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, ResMdl, unsigned long);
void Calc_BILLBOARD_PERSP_ROT(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, ResMdl, unsigned long);
void Calc_BILLBOARD_Y(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, ResMdl, unsigned long);
void Calc_BILLBOARD_PERSP_Y(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, ResMdl, unsigned long);

/******************************************************************************
 *
 * GetModelLocalAxisY2
 *
 * Computes the direction of the parent's local Y axis (column 1) expressed
 * through the node's matrix.  The parent's axes are recovered from the
 * adjugate of its upper-left 3x3 (cross products of the other two columns),
 * which stays well-defined under non-uniform scale; the adjugate row 1 is
 * then transformed by the node matrix and divided by the determinant.
 * Returns the 2D result (z = 0); used by the non-perspective billboards.
 *
 ******************************************************************************/
// Isolated Gekko paired-single backend (PLAN.md §17.6): retail compiled
// GetModelLocalAxisY2/Y3 as paired-single kernels (same adjugate prologue as
// the g3d_transform_ps.inl family), so the Wii/MWCC matching build uses the
// SDK kernel bodies verbatim; other builds take the scalar fallback below.
#if defined(__MWERKS__) && !defined(NONMATCHING)

// clang-format off
asm void GetModelLocalAxisY2(register math::VEC3* pOut,
                             register const math::MTX34* pMtx,
                             register const math::MTX34* pParentMtx) {
    nofralloc
    psq_l      f0, 0x0(pParentMtx), 1, 0
    psq_l      f1, 0x4(pParentMtx), 0, 0
    psq_l      f2, 0x10(pParentMtx), 1, 0
    ps_merge10 f6, f1, f0
    psq_l      f3, 0x14(pParentMtx), 0, 0
    psq_l      f4, 0x20(pParentMtx), 1, 0
    ps_merge10 f7, f3, f2
    psq_l      f5, 0x24(pParentMtx), 0, 0
    ps_mul     f11, f3, f6
    ps_mul     f13, f5, f7
    ps_merge10 f8, f5, f4
    ps_msub    f11, f1, f7, f11
    ps_mul     f12, f1, f8
    ps_msub    f13, f3, f8, f13
    lis        r12, lbl_eu_80669C38@ha
    psq_l      f3, 0x18(pMtx), 1, 0
    ps_msub    f12, f5, f6, f12
    addi       r12, r12, lbl_eu_80669C38@l
    ps_mul     f7, f0, f13
    ps_mul     f9, f0, f5
    psq_l      f6, 0x0(r12), 1, 0
    ps_madd    f7, f2, f12, f7
    ps_sub     f13, f13, f13
    psq_l      f2, 0x8(pMtx), 1, 0
    ps_madd    f7, f4, f11, f7
    ps_msub    f9, f1, f4, f9
    psq_l      f1, 0x10(pMtx), 0, 0
    ps_abs     f8, f7
    psq_st     f13, 0x8(pOut), 1, 0
    ps_cmpo0   cr0, f8, f6
    bge        axisy2_invertible
    psq_st     f13, 0x0(pOut), 0, 0
    blr

axisy2_invertible:
    fres       f8, f7
    psq_l      f0, 0x0(pMtx), 0, 0
    ps_add     f11, f8, f8
    ps_mul     f10, f7, f8
    ps_merge00 f4, f0, f1
    ps_nmsub   f8, f8, f10, f11
    ps_merge11 f5, f0, f1
    ps_merge00 f6, f2, f3
    ps_muls0   f12, f12, f8
    ps_muls0   f9, f9, f8
    ps_muls0   f0, f4, f12
    ps_madds1  f0, f5, f12, f0
    ps_madds0  f0, f6, f9, f0
    psq_st     f0, 0x0(pOut), 0, 0
    blr
}
// clang-format on

#else

void GetModelLocalAxisY2(math::VEC3* pOut, const math::MTX34* pMtx,
                         const math::MTX34* pParentMtx) {
    // Parent matrix columns: right (col 0), up (col 1), fwd (col 2).
    f32 r0 = pParentMtx->_00, r1 = pParentMtx->_10, r2 = pParentMtx->_20;
    f32 u0 = pParentMtx->_01, u1 = pParentMtx->_11, u2 = pParentMtx->_21;
    f32 f0 = pParentMtx->_02, f1 = pParentMtx->_12, f2 = pParentMtx->_22;

    // Adjugate columns: X = up x fwd, Y = fwd x right, Z = right x up.
    f32 ix0 = u1 * f2 - u2 * f1;
    f32 ix1 = u2 * f0 - u0 * f2;
    f32 ix2 = u0 * f1 - u1 * f0;
    f32 iy0 = f1 * r2 - f2 * r1;
    f32 iy1 = f2 * r0 - f0 * r2;
    f32 iy2 = f0 * r1 - f1 * r0;
    f32 iz0 = r1 * u2 - r2 * u1;
    f32 iz1 = r2 * u0 - r0 * u2;
    f32 iz2 = r0 * u1 - r1 * u0;

    // Determinant = right . X.  A (near-)singular parent yields no axis.
    f32 det = r0 * ix0 + r1 * ix1 + r2 * ix2;
    f32 absDet = det >= 0.0f ? det : -det;
    pOut->z = 0.0f;
    if (absDet < lbl_eu_80669C38) {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        return;
    }

    // 1/det via fres + Newton-Raphson refinement (retail's lane-0 sequence:
    // fres, fadds, fmuls, fnmsubs).
    f32 y0 = __fres(det);
    f32 t = det * y0;
    f32 two = y0 + y0;
    f32 invDet = __fnmsubs(y0, t, two);

    // out = M_node . (X.y, Y.y, Z.y) / det  --  parent Y axis through node.
    pOut->x = (pMtx->_00 * ix1 + pMtx->_01 * iy1 + pMtx->_02 * iz1) * invDet;
    pOut->y = (pMtx->_10 * ix1 + pMtx->_11 * iy1 + pMtx->_12 * iz1) * invDet;
}

#endif // __MWERKS__ && !NONMATCHING

/******************************************************************************
 *
 * GetModelLocalAxisY3
 *
 * Same as GetModelLocalAxisY2 but keeps all three rows of pMtx in the result
 * (3D output); used by the perspective billboard variants.
 *
 ******************************************************************************/

// Isolated PS backend, same arrangement as GetModelLocalAxisY2 above.
#if defined(__MWERKS__) && !defined(NONMATCHING)

// clang-format off
asm void GetModelLocalAxisY3(register math::VEC3* pOut,
                             register const math::MTX34* pMtx,
                             register const math::MTX34* pParentMtx) {
    nofralloc
    psq_l      f0, 0x0(pParentMtx), 1, 0
    psq_l      f1, 0x4(pParentMtx), 0, 0
    psq_l      f2, 0x10(pParentMtx), 1, 0
    ps_merge10 f6, f1, f0
    psq_l      f3, 0x14(pParentMtx), 0, 0
    psq_l      f4, 0x20(pParentMtx), 1, 0
    ps_merge10 f7, f3, f2
    psq_l      f5, 0x24(pParentMtx), 0, 0
    ps_mul     f11, f3, f6
    ps_mul     f13, f5, f7
    ps_merge10 f8, f5, f4
    ps_msub    f11, f1, f7, f11
    ps_mul     f12, f1, f8
    ps_msub    f13, f3, f8, f13
    lis        r12, lbl_eu_80669C38@ha
    psq_l      f3, 0x18(pMtx), 1, 0
    ps_msub    f12, f5, f6, f12
    addi       r12, r12, lbl_eu_80669C38@l
    ps_mul     f7, f0, f13
    ps_mul     f9, f0, f5
    psq_l      f6, 0x0(r12), 1, 0
    ps_madd    f7, f2, f12, f7
    ps_sub     f13, f13, f13
    psq_l      f2, 0x8(pMtx), 1, 0
    ps_madd    f7, f4, f11, f7
    ps_msub    f9, f1, f4, f9
    psq_l      f1, 0x10(pMtx), 0, 0
    ps_abs     f8, f7
    psq_st     f13, 0x8(pOut), 1, 0
    ps_cmpo0   cr0, f8, f6
    bge        axisy3_invertible
    psq_st     f13, 0x0(pOut), 0, 0
    blr

axisy3_invertible:
    fres       f8, f7
    psq_l      f0, 0x0(pMtx), 0, 0
    ps_add     f11, f8, f8
    ps_mul     f10, f7, f8
    ps_merge00 f4, f0, f1
    ps_nmsub   f8, f8, f10, f11
    ps_merge11 f5, f0, f1
    ps_merge00 f6, f2, f3
    ps_muls0   f12, f12, f8
    psq_l      f1, 0x20(pMtx), 0, 0
    ps_muls0   f9, f9, f8
    psq_l      f2, 0x28(pMtx), 1, 0
    ps_muls0   f0, f4, f12
    ps_mul     f1, f1, f9
    ps_madds1  f0, f5, f12, f0
    ps_sum0    f1, f1, f1, f1
    ps_madds0  f0, f6, f9, f0
    fmadds     f1, f2, f9, f1
    psq_st     f0, 0x0(pOut), 0, 0
    psq_st     f1, 0x8(pOut), 1, 0
    blr
}
// clang-format on

#else

void GetModelLocalAxisY3(math::VEC3* pOut, const math::MTX34* pMtx,
                         const math::MTX34* pParentMtx) {
    // Parent matrix columns: right (col 0), up (col 1), fwd (col 2).
    f32 r0 = pParentMtx->_00, r1 = pParentMtx->_10, r2 = pParentMtx->_20;
    f32 u0 = pParentMtx->_01, u1 = pParentMtx->_11, u2 = pParentMtx->_21;
    f32 f0 = pParentMtx->_02, f1 = pParentMtx->_12, f2 = pParentMtx->_22;

    // Adjugate columns: X = up x fwd, Y = fwd x right, Z = right x up.
    f32 ix0 = u1 * f2 - u2 * f1;
    f32 ix1 = u2 * f0 - u0 * f2;
    f32 ix2 = u0 * f1 - u1 * f0;
    f32 iy0 = f1 * r2 - f2 * r1;
    f32 iy1 = f2 * r0 - f0 * r2;
    f32 iy2 = f0 * r1 - f1 * r0;
    f32 iz0 = r1 * u2 - r2 * u1;
    f32 iz1 = r2 * u0 - r0 * u2;
    f32 iz2 = r0 * u1 - r1 * u0;

    // Determinant = right . X.  A (near-)singular parent yields no axis.
    f32 det = r0 * ix0 + r1 * ix1 + r2 * ix2;
    f32 absDet = det >= 0.0f ? det : -det;
    if (absDet < lbl_eu_80669C38) {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 0.0f;
        return;
    }

    // 1/det via fres + Newton-Raphson refinement (retail's lane-0 sequence).
    f32 y0 = __fres(det);
    f32 t = det * y0;
    f32 two = y0 + y0;
    f32 invDet = __fnmsubs(y0, t, two);

    // out = M_node . (X.y, Y.y, Z.y) / det  --  parent Y axis through node.
    pOut->x = (pMtx->_00 * ix1 + pMtx->_01 * iy1 + pMtx->_02 * iz1) * invDet;
    pOut->y = (pMtx->_10 * ix1 + pMtx->_11 * iy1 + pMtx->_12 * iz1) * invDet;
    pOut->z = (pMtx->_20 * ix1 + pMtx->_21 * iy1 + pMtx->_22 * iz1) * invDet;
}

#endif // __MWERKS__ && !NONMATCHING

/******************************************************************************
 *
 * Calc_BILLBOARD_STD
 *
 * Standard billboard: always faces the camera.  The billboard's local Z axis
 * points toward the camera.  The X/Y axes are built from the current matrix's
 * Y column (normalized, with unit Z) and scaled by the model matrix column
 * lengths.
 *
 ******************************************************************************/
void Calc_BILLBOARD_STD(math::MTX34* pOut, const math::MTX34* pMtxArray,
                         bool useParent, const math::MTX34* pViewMtx,
                         ResMdl mdl, unsigned long mtxId) {
    // The billboard's local Y axis comes from the current matrix's Y column
    // (X, Y components) with a unit Z component.
    math::VEC3 localY;
    localY.x = pOut->_01;
    localY.y = pOut->_11;
    localY.z = lbl_eu_80669C30;

    // Degenerate Y axis (both X and Y components significant) -> zero it.
    if (__fabs(localY.x) > lbl_eu_80669C34 &&
        __fabs(localY.y) > lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&localY, &localY);

    const math::MTX34* pMtx = &pMtxArray[mtxId];

    if (useParent) {
        // Parent-scaled billboard: scale everything by the first column
        // length of the model matrix.
        f32 lenSq = pMtx->_00 * pMtx->_00 + pMtx->_10 * pMtx->_10 +
                    pMtx->_20 * pMtx->_20;
        f32 len;
        if (lenSq > lbl_eu_80669C30) {
            len = lenSq * math::FrSqrt(lenSq);
        } else {
            len = lbl_eu_80669C30;
        }

        pOut->_00 = localY.x * len;
        pOut->_01 = localY.y * len;
        pOut->_02 = lbl_eu_80669C30;
        pOut->_10 = -localY.y * len;
        pOut->_11 = localY.x * len;
        pOut->_12 = lbl_eu_80669C30;
        pOut->_20 = lbl_eu_80669C30;
        pOut->_21 = lbl_eu_80669C30;
        pOut->_22 = len;
    } else {
        // Scale each output column by the model matrix's column length.
        f32 lenSq0 = pMtx->_00 * pMtx->_00 + pMtx->_10 * pMtx->_10 +
                     pMtx->_20 * pMtx->_20;
        f32 len0;
        if (lenSq0 > lbl_eu_80669C30) {
            len0 = lenSq0 * math::FrSqrt(lenSq0);
        } else {
            len0 = lbl_eu_80669C30;
        }

        f32 lenSq1 = pMtx->_01 * pMtx->_01 + pMtx->_11 * pMtx->_11 +
                     pMtx->_21 * pMtx->_21;
        f32 len1;
        if (lenSq1 > lbl_eu_80669C30) {
            len1 = lenSq1 * math::FrSqrt(lenSq1);
        } else {
            len1 = lbl_eu_80669C30;
        }

        f32 lenSq2 = pMtx->_02 * pMtx->_02 + pMtx->_12 * pMtx->_12 +
                     pMtx->_22 * pMtx->_22;
        f32 len2;
        if (lenSq2 > lbl_eu_80669C30) {
            len2 = lenSq2 * math::FrSqrt(lenSq2);
        } else {
            len2 = lbl_eu_80669C30;
        }

        pOut->_00 = localY.x * len0;
        pOut->_01 = localY.y * len1;
        pOut->_02 = lbl_eu_80669C30;
        pOut->_10 = -localY.y * len0;
        pOut->_11 = localY.x * len1;
        pOut->_12 = lbl_eu_80669C30;
        pOut->_20 = lbl_eu_80669C30;
        pOut->_21 = lbl_eu_80669C30;
        pOut->_22 = len2;
    }
}

/******************************************************************************
 *
 * Calc_BILLBOARD_PERSP_STD
 *
 * Perspective standard billboard.
 *
 ******************************************************************************/
void Calc_BILLBOARD_PERSP_STD(math::MTX34* pOut, const math::MTX34* pMtxArray,
                               bool useParent, const math::MTX34* pViewMtx,
                               ResMdl mdl, unsigned long mtxId) {
    // Billboard basis: up = the output matrix's Y column, dir = negated Z
    // column (toward the camera).  Output columns are right, up, dir.
    math::VEC3 right;
    math::VEC3 up(pOut->_01, pOut->_11, pOut->_21);
    math::VEC3 dir(-pOut->_03, -pOut->_13, -pOut->_23);

    // Degenerate facing direction (all components ~0) -> zero matrix.
    if (math::FAbs(dir.x) < lbl_eu_80669C34 &&
        math::FAbs(dir.y) < lbl_eu_80669C34 &&
        math::FAbs(dir.z) < lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&dir, &dir);

    math::VEC3Cross(&right, &up, &dir);

    if (math::FAbs(right.x) < lbl_eu_80669C34 &&
        math::FAbs(right.y) < lbl_eu_80669C34 &&
        math::FAbs(right.z) < lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&right, &right);
    math::VEC3Cross(&up, &dir, &right);

    if (useParent) {
        // Parent-scaled: one scale from the model matrix's X column length.
        f32 lenSq = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                    pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                    pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
        f32 len = lbl_eu_80669C30;
        if (lenSq > lbl_eu_80669C30) {
            len = lenSq * math::FrSqrt(lenSq);
        }

        pOut->_00 = right.x * len;
        pOut->_01 = up.x * len;
        pOut->_02 = dir.x * len;
        pOut->_10 = right.y * len;
        pOut->_11 = up.y * len;
        pOut->_12 = dir.y * len;
        pOut->_20 = right.z * len;
        pOut->_21 = up.z * len;
        pOut->_22 = dir.z * len;
    } else {
        // Per-column scaling from the model matrix column lengths.
        f32 len0Sq = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                     pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                     pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
        f32 len0 = lbl_eu_80669C30;
        if (len0Sq > lbl_eu_80669C30) {
            len0 = len0Sq * math::FrSqrt(len0Sq);
        }

        f32 len1Sq = pMtxArray[mtxId]._01 * pMtxArray[mtxId]._01 +
                     pMtxArray[mtxId]._11 * pMtxArray[mtxId]._11 +
                     pMtxArray[mtxId]._21 * pMtxArray[mtxId]._21;
        f32 len1 = lbl_eu_80669C30;
        if (len1Sq > lbl_eu_80669C30) {
            len1 = len1Sq * math::FrSqrt(len1Sq);
        }

        f32 len2Sq = pMtxArray[mtxId]._02 * pMtxArray[mtxId]._02 +
                     pMtxArray[mtxId]._12 * pMtxArray[mtxId]._12 +
                     pMtxArray[mtxId]._22 * pMtxArray[mtxId]._22;
        f32 len2 = lbl_eu_80669C30;
        if (len2Sq > lbl_eu_80669C30) {
            len2 = len2Sq * math::FrSqrt(len2Sq);
        }

        pOut->_00 = right.x * len0;
        pOut->_01 = up.x * len1;
        pOut->_02 = dir.x * len2;
        pOut->_10 = right.y * len0;
        pOut->_11 = up.y * len1;
        pOut->_12 = dir.y * len2;
        pOut->_20 = right.z * len0;
        pOut->_21 = up.z * len1;
        pOut->_22 = dir.z * len2;
    }
}

/******************************************************************************
 *
 * Calc_BILLBOARD_ROT
 *
 * Rotational billboard: rotates around the model's local Y axis to face the
 * camera.  The local Y axis comes from the parent node's matrix (via
 * GetModelLocalAxisY2), or from the node's own Y column when no parent
 * applies.
 *
 ******************************************************************************/
void Calc_BILLBOARD_ROT(math::MTX34* pOut, const math::MTX34* pMtxArray,
                         bool useParent, const math::MTX34* pViewMtx,
                         ResMdl mdl, unsigned long mtxId) {
    math::VEC3 localY;

    // Look up the node that owns this matrix and its parent node; the local
    // Y axis is derived from the parent's matrix when available.
    s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(mtxId);
    if (nodeId >= 0) {
        ResNode node = mdl.GetResNode(static_cast<int>(nodeId));
        ResNode parent = node.GetParentNode();
        if (parent.IsValid()) {
            GetModelLocalAxisY2(&localY, &pMtxArray[mtxId],
                                &pMtxArray[parent.GetMtxID()]);
        } else {
            localY.x = pMtxArray[mtxId]._01;
            localY.y = pMtxArray[mtxId]._11;
            localY.z = lbl_eu_80669C30;
        }
    } else {
        localY.x = pMtxArray[mtxId]._01;
        localY.y = pMtxArray[mtxId]._11;
        localY.z = lbl_eu_80669C30;
    }

    // Degenerate local Y axis (both X and Y components significant) -> zero.
    if (__fabs(localY.x) > lbl_eu_80669C34 &&
        __fabs(localY.y) > lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&localY, &localY);

    const math::MTX34* pMtx = &pMtxArray[mtxId];

    if (useParent) {
        // Parent-scaled: one length from the first model-matrix column.
        f32 lenSq = pMtx->_00 * pMtx->_00 + pMtx->_10 * pMtx->_10 +
                    pMtx->_20 * pMtx->_20;
        f32 len;
        if (lenSq > lbl_eu_80669C30) {
            len = lenSq * math::FrSqrt(lenSq);
        } else {
            len = lbl_eu_80669C30;
        }

        pOut->_00 = localY.x * len;
        pOut->_01 = localY.y * len;
        pOut->_02 = lbl_eu_80669C30;
        pOut->_10 = -localY.y * len;
        pOut->_11 = localY.x * len;
        pOut->_12 = lbl_eu_80669C30;
        pOut->_20 = lbl_eu_80669C30;
        pOut->_21 = lbl_eu_80669C30;
        pOut->_22 = len;
    } else {
        // Per-column scaling from the model matrix column lengths.
        f32 lenSq0 = pMtx->_00 * pMtx->_00 + pMtx->_10 * pMtx->_10 +
                     pMtx->_20 * pMtx->_20;
        f32 len0;
        if (lenSq0 > lbl_eu_80669C30) {
            len0 = lenSq0 * math::FrSqrt(lenSq0);
        } else {
            len0 = lbl_eu_80669C30;
        }

        f32 lenSq1 = pMtx->_01 * pMtx->_01 + pMtx->_11 * pMtx->_11 +
                     pMtx->_21 * pMtx->_21;
        f32 len1;
        if (lenSq1 > lbl_eu_80669C30) {
            len1 = lenSq1 * math::FrSqrt(lenSq1);
        } else {
            len1 = lbl_eu_80669C30;
        }

        f32 lenSq2 = pMtx->_02 * pMtx->_02 + pMtx->_12 * pMtx->_12 +
                     pMtx->_22 * pMtx->_22;
        f32 len2;
        if (lenSq2 > lbl_eu_80669C30) {
            len2 = lenSq2 * math::FrSqrt(lenSq2);
        } else {
            len2 = lbl_eu_80669C30;
        }

        pOut->_00 = localY.x * len0;
        pOut->_01 = localY.y * len1;
        pOut->_02 = lbl_eu_80669C30;
        pOut->_10 = -localY.y * len0;
        pOut->_11 = localY.x * len1;
        pOut->_12 = lbl_eu_80669C30;
        pOut->_20 = lbl_eu_80669C30;
        pOut->_21 = lbl_eu_80669C30;
        pOut->_22 = len2;
    }
}

/******************************************************************************
 *
 * Calc_BILLBOARD_PERSP_ROT
 *
 * Perspective rotational billboard.
 *
 ******************************************************************************/
void Calc_BILLBOARD_PERSP_ROT(math::MTX34* pOut, const math::MTX34* pMtxArray,
                               bool useParent, const math::MTX34* pViewMtx,
                               ResMdl mdl, unsigned long mtxId) {
    math::VEC3 dir(-pOut->_03, -pOut->_13, -pOut->_23);

    ResMdlInfo info = mdl.GetResMdlInfo();
    s32 nodeId = info.GetNodeIDFromMtxID(mtxId);

    math::VEC3 localY;
    localY.x = 0.0f;
    localY.y = 0.0f;
    localY.z = 0.0f;
    const math::MTX34* pNodeMtx = NULL;
    const math::MTX34* pParentMtx = NULL;

    if (nodeId >= 0) {
        ResNode node = mdl.GetResNode(static_cast<u32>(nodeId));
        ResNode parentNode = node.GetParentNode();
        u32 parentMtxId = mtxId;

        if (parentNode.IsValid()) {
            parentMtxId = parentNode.GetMtxID();
        }

        const u8* base = reinterpret_cast<const u8*>(pMtxArray);

        pNodeMtx = reinterpret_cast<const math::MTX34*>(
            base + mtxId * sizeof(math::MTX34));
        pParentMtx = reinterpret_cast<const math::MTX34*>(
            base + parentMtxId * sizeof(math::MTX34));

        GetModelLocalAxisY3(&localY, pNodeMtx, pParentMtx);
    } else {
        const u8* base = reinterpret_cast<const u8*>(pMtxArray);

        pNodeMtx = reinterpret_cast<const math::MTX34*>(
            base + mtxId * sizeof(math::MTX34));

        localY.x = pNodeMtx->_01;
        localY.y = pNodeMtx->_11;
        localY.z = pNodeMtx->_21;
    }

    if (math::FAbs(dir.x) < 1.0e-18f && math::FAbs(dir.y) < 1.0e-18f &&
        math::FAbs(dir.z) < 1.0e-18f) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&dir, &dir);

    math::VEC3 right;
    math::VEC3Cross(&right, &localY, &dir);

    if (math::FAbs(right.x) < 1.0e-18f && math::FAbs(right.y) < 1.0e-18f &&
        math::FAbs(right.z) < 1.0e-18f) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&right, &right);
    math::VEC3Cross(&localY, &dir, &right);

    f32 len = math::FrSqrt(pNodeMtx->_00 * pNodeMtx->_00 +
                           pNodeMtx->_10 * pNodeMtx->_10 +
                           pNodeMtx->_20 * pNodeMtx->_20);
    f32 len1 = len * (pNodeMtx->_00 * pNodeMtx->_00 +
                      pNodeMtx->_10 * pNodeMtx->_10 +
                      pNodeMtx->_20 * pNodeMtx->_20);

    pOut->_00 = right.x * len1;
    pOut->_01 = localY.x * len1;
    pOut->_02 = dir.x * len1;
    pOut->_10 = right.y * len1;
    pOut->_11 = localY.y * len1;
    pOut->_12 = dir.y * len1;
    pOut->_20 = right.z * len1;
    pOut->_21 = localY.z * len1;
    pOut->_22 = dir.z * len1;
}

/******************************************************************************
 *
 * Calc_BILLBOARD_Y
 *
 * Y-axis billboard: faces the camera while keeping the model's Y axis fixed
 * (world up).  The up direction is derived from the current matrix's Y
 * column; the camera-facing axes come from up x Y and the normalized up.
 *
 ******************************************************************************/
void Calc_BILLBOARD_Y(math::MTX34* pOut, const math::MTX34* pMtxArray,
                       bool useParent, const math::MTX34* pViewMtx,
                       ResMdl mdl, unsigned long mtxId) {
    // up = the world-up direction implied by the current matrix's Y column;
    // yAxis = that Y column itself (normalized below).
    math::VEC3 up;
    up.x = pOut->_11;
    up.y = -pOut->_01;
    up.z = lbl_eu_80669C30;
    math::VEC3 yAxis;
    yAxis.x = pOut->_01;
    yAxis.y = pOut->_11;
    yAxis.z = pOut->_21;

    // Degenerate Y column (all three components significant) -> zero it.
    if (__fabs(yAxis.x) > lbl_eu_80669C34 &&
        __fabs(yAxis.y) > lbl_eu_80669C34 &&
        __fabs(yAxis.z) > lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    const math::MTX34* pMtx = &pMtxArray[mtxId];

    // Normalize the Y column by its own length.
    f32 lenSq1 = pMtx->_01 * pMtx->_01 + pMtx->_11 * pMtx->_11 +
                 pMtx->_21 * pMtx->_21;
    f32 len1;
    if (lenSq1 > lbl_eu_80669C30) {
        len1 = lenSq1 * math::FrSqrt(lenSq1);
    } else {
        len1 = lbl_eu_80669C30;
    }

    // 1/len1 via fres + Newton-Raphson refinement (retail's lane-0 sequence).
    f32 y0 = __fres(len1);
    f32 t = len1 * y0;
    f32 two = y0 + y0;
    f32 invLen1 = __fnmsubs(y0, t, two);

    yAxis.x *= invLen1;
    yAxis.y *= invLen1;
    yAxis.z *= invLen1;

    // The implied up direction must stay near-vertical; otherwise no
    // billboard can be formed.
    if (__fabs(up.x) > lbl_eu_80669C34 && __fabs(up.y) > lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&up, &up);

    math::VEC3 right;
    math::VEC3Cross(&right, &up, &yAxis);

    if (useParent) {
        // Parent-scaled: everything scaled by the Y column length.
        pOut->_00 = up.x * len1;
        pOut->_01 = yAxis.x * len1;
        pOut->_02 = right.x * len1;
        pOut->_10 = up.y * len1;
        pOut->_11 = yAxis.y * len1;
        pOut->_12 = right.y * len1;
        pOut->_20 = up.z * len1;
        pOut->_21 = yAxis.z * len1;
        pOut->_22 = right.z * len1;
    } else {
        // Per-column scaling from the model matrix column lengths.
        f32 lenSq0 = pMtx->_00 * pMtx->_00 + pMtx->_10 * pMtx->_10 +
                     pMtx->_20 * pMtx->_20;
        f32 len0;
        if (lenSq0 > lbl_eu_80669C30) {
            len0 = lenSq0 * math::FrSqrt(lenSq0);
        } else {
            len0 = lbl_eu_80669C30;
        }

        f32 lenSq2 = pMtx->_02 * pMtx->_02 + pMtx->_12 * pMtx->_12 +
                     pMtx->_22 * pMtx->_22;
        f32 len2;
        if (lenSq2 > lbl_eu_80669C30) {
            len2 = lenSq2 * math::FrSqrt(lenSq2);
        } else {
            len2 = lbl_eu_80669C30;
        }

        pOut->_00 = up.x * len0;
        pOut->_01 = yAxis.x * len1;
        pOut->_02 = right.x * len2;
        pOut->_10 = up.y * len0;
        pOut->_11 = yAxis.y * len1;
        pOut->_12 = right.y * len2;
        pOut->_20 = up.z * len0;
        pOut->_21 = yAxis.z * len1;
        pOut->_22 = right.z * len2;
    }
}

/******************************************************************************
 *
 * Calc_BILLBOARD_PERSP_Y
 *
 * Perspective Y-axis billboard.
 *
 ******************************************************************************/
void Calc_BILLBOARD_PERSP_Y(math::MTX34* pOut, const math::MTX34* pMtxArray,
                             bool useParent, const math::MTX34* pViewMtx,
                             ResMdl mdl, unsigned long mtxId) {
    // Billboard basis: up = the output matrix's Y column, dir = negated Z
    // column (toward the camera).  Output columns are right, up, dir.
    math::VEC3 right;
    math::VEC3 up(pOut->_01, pOut->_11, pOut->_21);
    math::VEC3 dir(-pOut->_03, -pOut->_13, -pOut->_23);

    // Degenerate up vector (all components ~0) -> zero matrix.
    if (math::FAbs(up.x) < lbl_eu_80669C34 &&
        math::FAbs(up.y) < lbl_eu_80669C34 &&
        math::FAbs(up.z) < lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    // Normalize up by its own (clamped) length via fres + Newton reciprocal.
    f32 upLenSq = pMtxArray[mtxId]._01 * pMtxArray[mtxId]._01 +
                  pMtxArray[mtxId]._11 * pMtxArray[mtxId]._11 +
                  pMtxArray[mtxId]._21 * pMtxArray[mtxId]._21;
    f32 upLen = lbl_eu_80669C30;
    if (upLenSq > lbl_eu_80669C30) {
        upLen = upLenSq * math::FrSqrt(upLenSq);
    }
    math::VEC3Scale(&up, &up, math::FInv(upLen));

    math::VEC3Cross(&right, &up, &dir);

    if (math::FAbs(right.x) < lbl_eu_80669C34 &&
        math::FAbs(right.y) < lbl_eu_80669C34 &&
        math::FAbs(right.z) < lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&right, &right);
    math::VEC3Cross(&dir, &right, &up);

    if (useParent) {
        // Parent-scaled: one scale (the up length) for all three columns.
        pOut->_00 = right.x * upLen;
        pOut->_01 = up.x * upLen;
        pOut->_02 = dir.x * upLen;
        pOut->_10 = right.y * upLen;
        pOut->_11 = up.y * upLen;
        pOut->_12 = dir.y * upLen;
        pOut->_20 = right.z * upLen;
        pOut->_21 = up.z * upLen;
        pOut->_22 = dir.z * upLen;
    } else {
        // Per-column scaling from the model matrix column lengths; the up
        // column keeps the length computed for the normalize above.
        f32 len0Sq = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                     pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                     pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
        f32 len0;
        if (len0Sq > lbl_eu_80669C30) {
            len0 = len0Sq * math::FrSqrt(len0Sq);
        } else {
            len0 = lbl_eu_80669C30;
        }

        f32 len2Sq = pMtxArray[mtxId]._02 * pMtxArray[mtxId]._02 +
                     pMtxArray[mtxId]._12 * pMtxArray[mtxId]._12 +
                     pMtxArray[mtxId]._22 * pMtxArray[mtxId]._22;
        f32 len2;
        if (len2Sq > lbl_eu_80669C30) {
            len2 = len2Sq * math::FrSqrt(len2Sq);
        } else {
            len2 = lbl_eu_80669C30;
        }

        pOut->_00 = right.x * len0;
        pOut->_01 = up.x * upLen;
        pOut->_02 = dir.x * len2;
        pOut->_10 = right.y * len0;
        pOut->_11 = up.y * upLen;
        pOut->_12 = dir.y * len2;
        pOut->_20 = right.z * len0;
        pOut->_21 = up.z * upLen;
        pOut->_22 = dir.z * len2;
    }
}

} // anonymous namespace

/******************************************************************************
 *
 * CalcView
 *
 * Computes view-position, view-normal, and view-tex matrices from model
 * matrices. Iterates over all model matrices and applies the view matrix
 * to compute view-space position, normal, and texture matrices.
 *
 ******************************************************************************/
namespace {

typedef void (*CalcBillboardFunc)(math::MTX34*, const math::MTX34*, bool,
                                  const math::MTX34*, ResMdl, u32);

// Local copy of the dispatch table; postprocess renames it to the retail
// pool symbol lbl_eu_8051D6A0 (nw4r_data.s) and strips the section, keeping
// the Calc_BILLBOARD_* kernels referenced/emitted.
const CalcBillboardFunc gCalcBillboardFuncTable[] = {
    NULL,
    Calc_BILLBOARD_STD,
    Calc_BILLBOARD_PERSP_STD,
    Calc_BILLBOARD_ROT,
    Calc_BILLBOARD_PERSP_ROT,
    Calc_BILLBOARD_Y,
    Calc_BILLBOARD_PERSP_Y,
    NULL,
};

} // namespace

void CalcView(math::MTX34* pViewPosArray, math::MTX33* pViewNrmArray,
              const math::MTX34* pModelMtxArray,
              const u32* pModelMtxAttribArray, u32 numMtx,
              const math::MTX34* pViewMtx, const ResMdl mdl,
              math::MTX34* pViewTexMtxArray) {
    if (numMtx == 0) {
        return;
    }

    if (numMtx > 1) {
        math::MTX34MultArray(pViewPosArray, pViewMtx, pModelMtxArray,
                             numMtx);
    } else {
        math::MTX34Mult(&pViewPosArray[0], pViewMtx,
                        &pModelMtxArray[0]);
    }

    math::MTX34* pWorkMtx = nw4r::g3d::detail::workmem::GetBillboardMtxTemporary();

    u32 i = 0;
    const u32* pAttrib = pModelMtxAttribArray;
    math::MTX34* pViewPos = pViewPosArray;
    math::MTX34* pViewTex = pViewTexMtxArray;

    while (i < numMtx) {
        u32 attrib = *pAttrib++;
        u32 billboardIdx = attrib & 0xFF;

        if (billboardIdx != 0) {
            gCalcBillboardFuncTable[billboardIdx](pViewPos, pModelMtxArray,
                                                  (attrib >> 2) & 1, pViewMtx,
                                                  mdl, i);

            s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(i);
            ResNode node = mdl.GetResNode(static_cast<int>(nodeId));
            u8* pData = NULL;

            if (node.IsValid()) {
                s32 toData = node.ref().toResUserData;
                pData = (toData != 0)
                            ? reinterpret_cast<u8*>(&node.ref()) + toData
                            : NULL;
            }

            if (pData != NULL) {
                math::MTX34 inv;

                if (detail::CalcInvWorldMtx(&inv, pViewPos) == 1) {
                    math::MTX34Mult(pViewTex, &inv, pViewTex);
                } else {
                    math::MTX34Identity(pViewTex);
                    pViewTex->_02 = pViewMtx->_02;
                    pViewTex->_12 = pViewMtx->_12;
                    pViewTex->_22 = pViewMtx->_22;
                }
            }
        } else {
            s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(i);

            if (nodeId >= 0) {
                ResNode node = mdl.GetResNode(static_cast<int>(nodeId));

                if (node.IsValid()) {
                    if (node.ref().flags & 0x400) {
                        s32 parentId = node.ref().bbref_nodeid;
                        ResNode parent =
                            mdl.GetResNode(static_cast<int>(parentId));
                        u32 parentMtxId =
                            parent.IsValid() ? parent.ref().mtxID : 0;

                        math::MTX34Mult(&pWorkMtx[parentMtxId], pViewPos,
                                        &pModelMtxArray[i]);
                    }
                }
            }
        }

        pViewPos++;
        pViewTex++;
        i++;
    }

    u32 j = 0;
    math::MTX33* pViewNrm = pViewNrmArray;
    math::MTX34* pViewPos2 = pViewPosArray;
    math::MTX34* pViewTex2 = pViewTexMtxArray;

    while (j < numMtx) {
        if (pViewTexMtxArray != NULL) {
            detail::CalcViewTexMtx(pViewTex2, pViewPos2);
            math::MTX34ToMTX33(pViewNrm, pViewPos2);
        } else {
            detail::CalcViewNrmMtx(pViewNrm, pViewPos2);
        }

        pViewNrm++;
        pViewPos2++;
        pViewTex2++;
        j++;
    }
}

/******************************************************************************
 *
 * CalcView_LC
 *
 * Same as CalcView but uses locked cache (LC) for matrix data.
 *
 ******************************************************************************/
void CalcView_LC(math::MTX34* pViewPosArray, math::MTX33* pViewNrmArray,
                 const math::MTX34* pModelMtxArray,
                 const u32* pModelMtxAttribArray, u32 numMtx,
                 const math::MTX34* pViewMtx, const ResMdl mdl,
                 math::MTX34* pViewTexMtxArray) {
    if (numMtx == 0) {
        return;
    }

    // Locked-cache base address for the position-matrix store: written
    // in-place below as a direct literal (0xE0001000). Pooling this table
    // into .rodata would leave the split with local data retail does not
    // have (retail materializes it on the stack).

    u32 posSize = (numMtx * sizeof(math::MTX34) + 0x1F) & ~0x1F;
    u32 nrmSize = (numMtx * sizeof(math::MTX33) + 0x1F) & ~0x1F;

    DCInvalidateRange(pViewNrmArray, nrmSize);
    DCInvalidateRange(pViewTexMtxArray, posSize);
    DCInvalidateRange(pViewPosArray, posSize);

    math::MTX34* pWorkMtx = nw4r::g3d::detail::workmem::GetBillboardMtxTemporary();

    u32 processed = 0;

    while (processed < numMtx) {
        u32 chunk = numMtx - processed;

        if (chunk > 0x28) {
            chunk = 0x28;
        }

        LCQueueWait(0);

        if (chunk > 1) {
            math::MTX34MultArray(&pViewPosArray[processed], pViewMtx,
                                 &pModelMtxArray[processed], chunk);
        } else {
            math::MTX34Mult(&pViewPosArray[processed], pViewMtx,
                            &pModelMtxArray[processed]);
        }

        for (u32 i = 0; i < chunk; i++) {
            u32 idx = processed + i;
            u32 attrib = pModelMtxAttribArray[idx];
            u32 billboardIdx = attrib & 0xFF;

            if (billboardIdx != 0) {
                gCalcBillboardFuncTable[billboardIdx](
                    &pViewPosArray[idx], pModelMtxArray, (attrib >> 2) & 1,
                    pViewMtx, mdl, idx);

                s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(idx);
                ResNode node = mdl.GetResNode(static_cast<int>(nodeId));
                u8* pData = NULL;

                if (node.IsValid()) {
                    s32 toData = node.ref().toResUserData;
                    pData = (toData != 0)
                                ? reinterpret_cast<u8*>(&node.ref()) + toData
                                : NULL;
                }

                if (pData != NULL) {
                    math::MTX34 inv;

                    if (detail::CalcInvWorldMtx(&inv, &pViewPosArray[idx]) ==
                        1) {
                        math::MTX34Mult(&pViewTexMtxArray[idx], &inv,
                                        &pViewTexMtxArray[idx]);
                    } else {
                        math::MTX34Identity(&pViewTexMtxArray[idx]);
                        pViewTexMtxArray[idx]._02 = pViewMtx->_02;
                        pViewTexMtxArray[idx]._12 = pViewMtx->_12;
                        pViewTexMtxArray[idx]._22 = pViewMtx->_22;
                    }
                }
            } else {
                s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(idx);

                if (nodeId >= 0) {
                    ResNode node =
                        mdl.GetResNode(static_cast<int>(nodeId));

                    if (node.IsValid()) {
                        if (node.ref().flags & 0x400) {
                            s32 parentId = node.ref().bbref_nodeid;
                            ResNode parent =
                                mdl.GetResNode(static_cast<int>(parentId));
                            u32 parentMtxId =
                                parent.IsValid() ? parent.ref().mtxID : 0;

                            math::MTX34Mult(&pWorkMtx[parentMtxId],
                                            &pViewPosArray[idx],
                                            &pModelMtxArray[idx]);
                        }
                    }
                }
            }
        }

        processed += chunk;
    }

    nw4r::ut::LC::StoreBlocks(pViewPosArray,
                        reinterpret_cast<void*>(0xE0001000),
                        posSize / 32);

    if (pViewNrmArray != NULL) {
        for (u32 i = 0; i < numMtx; i++) {
            u32 attrib = pModelMtxAttribArray[i];

            if (attrib & 1) {
                if (pViewTexMtxArray != NULL) {
                    math::MTX34Copy(&pViewTexMtxArray[i], &pViewPosArray[i]);
                    pViewTexMtxArray[i]._03 = 0.0f;
                    pViewTexMtxArray[i]._13 = 0.0f;
                    pViewTexMtxArray[i]._23 = 0.0f;
                }

                math::MTX34ToMTX33(&pViewNrmArray[i], &pViewPosArray[i]);
            } else {
                if (pViewTexMtxArray != NULL) {
                    detail::CalcViewTexMtx(&pViewTexMtxArray[i],
                                           &pViewPosArray[i]);
                }

                detail::CalcViewNrmMtx(&pViewNrmArray[i], &pViewPosArray[i]);
            }
        }
    }
}

// Local copy of the dispatch table (renamed to the retail pool symbol
// lbl_eu_8051D6A0 from nw4r_data.s by postprocess; section stripped).

void CalcView_LC_DMA_ModelMtx(math::MTX34* pViewPosArray,
                              math::MTX33* pViewNrmArray,
                              const math::MTX34* pModelMtxArray,
                              const u32* pModelMtxAttribArray, u32 numMtx,
                              const math::MTX34* pViewMtx, const ResMdl mdl,
                              math::MTX34* pViewTexMtxArray) {
    if (numMtx == 0) {
        return;
    }

    // Locked-cache region ring: 8 slots rotating by pairwise swaps
    // (region, region+2nd-half) each 0x28-matrix iteration.  "pre" is the
    // DMA prefetch target for the next chunk.
    u32 posSize = (numMtx * sizeof(math::MTX34) + 0x1F) & ~0x1F;

    u32 lcDma = 0xE0000000;
    u32 lcNrm = 0xE0001000;
    u32 lcTex = 0xE0001800;
    u32 lcPre = 0xE0002000;
    u32 lcPos2 = 0xE0002800;
    u32 lcNrm2 = 0xE0003000;
    u32 lcTex2 = 0xE0003800;
    u32 lcPos = 0xE0000800;

    DCInvalidateRange(pViewPosArray, posSize);

    // Number of LC buffers to drain before overwriting a region.
    u32 lcState = 1;
    if (pViewNrmArray != NULL) {
        lcState = 2;
        u32 nrmSize = (numMtx * sizeof(math::MTX33) + 0x1F) & ~0x1F;
        DCInvalidateRange(pViewNrmArray, nrmSize);

        if (pViewTexMtxArray != NULL) {
            lcState = 3;
            DCInvalidateRange(pViewTexMtxArray, posSize);
        }
    }

    math::MTX34* pWorkMtx =
        nw4r::g3d::detail::workmem::GetBillboardMtxTemporary();

    LCQueueWait(0);

    // Refresh the (stale) main-memory model matrices from the locked cache,
    // where the previous pipeline stage DMA'd them.
    nw4r::ut::LC::LoadBlocks(
        reinterpret_cast<void*>(0xE0000000),
        const_cast<void*>(reinterpret_cast<const void*>(pModelMtxArray)),
        (numMtx > 0x28) ? 0x3C : (posSize / 32));

    if (numMtx > 0x14) {
        while (LCQueueLength() != 0) {
            OSYieldThread();
        }
    } else {
        LCQueueWait(0);
    }

    u32 processed = 0;         // current matrix index
    u32 attribByteOffset = 0;  // == processed * 4
    u32 dmaByteOffset = 0;     // == processed * 0x30
    u32 workByteOffset = 0;    // == processed * 0x30

    // Advancing copies of the output pointers; the originals are kept for
    // the per-chunk null checks.
    math::MTX33* pViewNrm = pViewNrmArray;
    math::MTX34* pViewTex = pViewTexMtxArray;

    while (processed < numMtx) {
        u32 chunk = numMtx - processed;
        u32 posBlocks, nrmBlocks;

        if (chunk > 0x28) {
            chunk = 0x28;
            posBlocks = 0x3C;
            nrmBlocks = 0x2D;
        } else {
            posBlocks = (chunk * sizeof(math::MTX34) + 0x1F) / 32;
            nrmBlocks = (chunk * sizeof(math::MTX33) + 0x1F) / 32;
        }

        LCQueueWait(lcState);

        // Prefetch the next 0x28 matrices into the pre region while this
        // chunk is still being processed.
        if (processed + 0x28 < numMtx) {
            u32 preChunk = numMtx - processed - 0x28;
            u32 preBlocks = (preChunk > 0x28)
                                ? 0x3C
                                : ((preChunk * sizeof(math::MTX34) + 0x1F) /
                                   32);

            nw4r::ut::LC::LoadBlocks(
                reinterpret_cast<void*>(lcPre),
                const_cast<void*>(reinterpret_cast<const void*>(
                    reinterpret_cast<const u8*>(pModelMtxArray) +
                    dmaByteOffset + 0x780)),
                preBlocks);
        }

        // viewPos = viewMtx * modelMtx, both operands in the locked cache.
        if (chunk > 1) {
            math::MTX34MultArray(reinterpret_cast<math::MTX34*>(lcPos),
                                 pViewMtx,
                                 reinterpret_cast<math::MTX34*>(lcDma),
                                 chunk);
        } else {
            math::MTX34Mult(reinterpret_cast<math::MTX34*>(lcPos), pViewMtx,
                            reinterpret_cast<math::MTX34*>(lcDma));
        }

        for (u32 i = 0; i < chunk; i++) {
            u32 attrib = pModelMtxAttribArray[processed + i];
            u32 billboardIdx = attrib & 0xFF;

            if (billboardIdx != 0) {
                gCalcBillboardFuncTable[billboardIdx](
                    reinterpret_cast<math::MTX34*>(lcPos) + i, pModelMtxArray,
                    (attrib >> 2) & 1, pViewMtx, mdl, processed + i);

                // Per-node billboard data hangs off the node's child link.
                s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(
                    processed + i);
                ResNode node = mdl.GetResNode(static_cast<u32>(nodeId));
                u8* pData = NULL;

                if (node.IsValid()) {
                    s32 toData = node.ref().toChildNode;
                    if (toData != 0) {
                        pData = reinterpret_cast<u8*>(&node.ref()) + toData;
                    }
                }

                if (pData != NULL) {
                    math::MTX34 inv;

                    if (detail::CalcInvWorldMtx(
                            &inv, &pModelMtxArray[processed + i]) == 1) {
                        // workMtx[mtxId] = viewPos * inv(worldMtx): the
                        // billboard texture matrix for the renderer.
                        math::MTX34Mult(&pWorkMtx[processed + i],
                                        reinterpret_cast<math::MTX34*>(lcPos) +
                                            i,
                                        &inv);
                    } else {
                        math::MTX34Identity(&pWorkMtx[processed + i]);
                        pWorkMtx[processed + i]._02 = pViewMtx->_02;
                        pWorkMtx[processed + i]._12 = pViewMtx->_12;
                        pWorkMtx[processed + i]._22 = pViewMtx->_22;
                    }
                }
            } else {
                s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(
                    processed + i);

                if (nodeId >= 0) {
                    ResNode node = mdl.GetResNode(static_cast<u32>(nodeId));

                    if (node.IsValid()) {
                        if (node.ref().flags & 0x400) {
                            ResNode parent =
                                mdl.GetResNode(node.ref().bbref_nodeid);
                            u32 parentMtxId =
                                parent.IsValid() ? parent.ref().mtxID : 0;

                            // Child of a billboard parent: combine the
                            // parent's work matrix with this node's model.
                            math::MTX34Mult(
                                reinterpret_cast<math::MTX34*>(lcPos) + i,
                                &pWorkMtx[parentMtxId],
                                &pModelMtxArray[processed + i]);
                        }
                    }
                }
            }
        }

        nw4r::ut::LC::StoreBlocks(pViewPosArray,
                                  reinterpret_cast<void*>(lcPos), posBlocks);

        if (pViewNrmArray) {
            for (u32 i = 0; i < chunk; i++) {
                u32 attrib = pModelMtxAttribArray[processed + i];

                if (attrib & 4) {
                    // Billboard matrix: tex is a copy with unit translation.
                    if (pViewTexMtxArray) {
                        math::MTX34Copy(
                            reinterpret_cast<math::MTX34*>(lcTex) + i,
                            reinterpret_cast<math::MTX34*>(lcPos) + i);
                        reinterpret_cast<math::MTX34*>(lcTex)[i]._23 =
                            lbl_eu_80669C30;
                        reinterpret_cast<math::MTX34*>(lcTex)[i]._13 =
                            lbl_eu_80669C30;
                        reinterpret_cast<math::MTX34*>(lcTex)[i]._03 =
                            lbl_eu_80669C30;
                    }

                    math::MTX34ToMTX33(reinterpret_cast<math::MTX33*>(lcNrm) +
                                           i,
                                       reinterpret_cast<math::MTX34*>(lcPos) +
                                           i);
                } else {
                    if (pViewTexMtxArray) {
                        detail::CalcViewTexMtx(
                            reinterpret_cast<math::MTX34*>(lcTex) + i,
                            reinterpret_cast<math::MTX34*>(lcPos) + i);
                        math::MTX34ToMTX33(
                            reinterpret_cast<math::MTX33*>(lcNrm) + i,
                            reinterpret_cast<math::MTX34*>(lcTex) + i);
                    } else {
                        detail::CalcViewNrmMtx(
                            reinterpret_cast<math::MTX33*>(lcNrm) + i,
                            reinterpret_cast<math::MTX34*>(lcPos) + i);
                    }
                }
            }

            nw4r::ut::LC::StoreBlocks(pViewNrm,
                                      reinterpret_cast<void*>(lcNrm),
                                      nrmBlocks);

            if (pViewTexMtxArray) {
                nw4r::ut::LC::StoreBlocks(pViewTex,
                                          reinterpret_cast<void*>(lcTex),
                                          posBlocks);
            }
        }

        // Rotate the LC region ring (advance each slot by 2 of 8).
        u32 t = lcDma;
        lcDma = lcPre;
        lcPre = t;
        t = lcPos;
        lcPos = lcPos2;
        lcPos2 = t;
        t = lcNrm;
        lcNrm = lcNrm2;
        lcNrm2 = t;
        t = lcTex;
        lcTex = lcTex2;
        lcTex2 = t;

        processed += 0x28;
        attribByteOffset += 0xa0;
        dmaByteOffset += 0x780;
        workByteOffset += 0x780;
        pViewPosArray += 0x28;
        pViewNrm += 0x28;
        pViewTex += 0x28;
    }
}

} // namespace g3d
} // namespace nw4r
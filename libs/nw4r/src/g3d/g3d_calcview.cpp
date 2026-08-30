#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/g3d/g3d_calcview.h>
#include <nw4r/g3d/g3d_workmem.h>
#include <nw4r/ut/ut_LockedCache.h>
#include <revolution/MTX.h>
#include <revolution/os/OSCache.h>

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
void Calc_BILLBOARD_STD(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, const ResMdl*, unsigned long);
void Calc_BILLBOARD_PERSP_STD(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, const ResMdl*, unsigned long);
void Calc_BILLBOARD_ROT(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, const ResMdl*, unsigned long);
void Calc_BILLBOARD_PERSP_ROT(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, const ResMdl, unsigned long);
void Calc_BILLBOARD_Y(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, const ResMdl*, unsigned long);
void Calc_BILLBOARD_PERSP_Y(math::MTX34*, const math::MTX34*, bool, const math::MTX34*, const ResMdl*, unsigned long);

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
                         const ResMdl* pMdl, unsigned long mtxId) {
    // The billboard's local Y axis comes from the current matrix's Y column
    // (X, Y components) with a unit Z component.
    math::VEC3 localY(pOut->_01, pOut->_11, lbl_eu_80669C30);

    // Degenerate Y axis: if both X and Y components are negligible the
    // column points straight down Z (at the camera) and no billboard
    // rotation can be built.
    if (__fabs(localY.x) >= lbl_eu_80669C34 ||
        __fabs(localY.y) >= lbl_eu_80669C34) {
        math::VEC3Normalize(&localY, &localY);

        const math::MTX34* pMtx = &pMtxArray[mtxId];

        // Output rows form a 2D rotation of the normalized Y axis, scaled
        // by model-matrix column lengths; retail pairs the row stores.
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

            pOut->_22 = len;
            pOut->_02 = lbl_eu_80669C30;
            pOut->_12 = lbl_eu_80669C30;
            pOut->_00 = localY.y * len;
            pOut->_01 = localY.x * len;
            pOut->_20 = lbl_eu_80669C30;
            pOut->_21 = lbl_eu_80669C30;
            pOut->_10 = -localY.x * len;
            pOut->_11 = localY.y * len;
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

            pOut->_22 = len2;
            pOut->_02 = lbl_eu_80669C30;
            pOut->_12 = lbl_eu_80669C30;
            pOut->_00 = localY.y * len0;
            pOut->_01 = localY.x * len1;
            pOut->_20 = lbl_eu_80669C30;
            pOut->_21 = lbl_eu_80669C30;
            pOut->_10 = -localY.x * len0;
            pOut->_11 = localY.y * len1;
        }
    } else {
        math::MTX34Zero(pOut);
        return;
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
                               const ResMdl* pMdl, unsigned long mtxId) {
    // Billboard basis: up = the output matrix's Y column, dir = negated Z
    // column (toward the camera).  Output columns are right, up, dir.
    math::VEC3 right;
    math::VEC3 up(pOut->_01, pOut->_11, pOut->_21);
    math::VEC3 dir(-pOut->_03, -pOut->_13, -pOut->_23);

    if (__fabs(dir.x) >= lbl_eu_80669C34 ||
        __fabs(dir.y) >= lbl_eu_80669C34 ||
        __fabs(dir.z) >= lbl_eu_80669C34) {
        PSVECNormalize(reinterpret_cast<const Vec*>(&dir),
                       reinterpret_cast<Vec*>(&dir));

        PSVECCrossProduct(reinterpret_cast<const Vec*>(&up),
                          reinterpret_cast<const Vec*>(&dir),
                          reinterpret_cast<Vec*>(&right));

        if (__fabs(right.x) >= lbl_eu_80669C34 ||
            __fabs(right.y) >= lbl_eu_80669C34 ||
            __fabs(right.z) >= lbl_eu_80669C34) {
            PSVECNormalize(reinterpret_cast<const Vec*>(&right),
                           reinterpret_cast<Vec*>(&right));
            PSVECCrossProduct(reinterpret_cast<const Vec*>(&dir),
                              reinterpret_cast<const Vec*>(&right),
                              reinterpret_cast<Vec*>(&up));

            if (useParent) {
        // Parent-scaled: one scale from the model matrix's X column length.
        f32 lenSq = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                    pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                    pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
        f32 len = lbl_eu_80669C30;
        if (lenSq > lbl_eu_80669C30) {
            len = lenSq * math::FrSqrt(lenSq);
        }

        // NOTE: retail never scales _12 (upstream nw4r quirk).
        pOut->_00 = right.x * len;
        pOut->_01 = up.x * len;
        pOut->_02 = dir.x * len;
        pOut->_10 = right.y * len;
        pOut->_11 = up.y * len;
        pOut->_12 = dir.y;
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

        // NOTE: retail never scales _12 (upstream nw4r quirk).
        pOut->_00 = right.x * len0;
        pOut->_01 = up.x * len1;
        pOut->_02 = dir.x * len2;
        pOut->_10 = right.y * len0;
        pOut->_11 = up.y * len1;
        pOut->_12 = dir.y;
        pOut->_20 = right.z * len0;
        pOut->_21 = up.z * len1;
        pOut->_22 = dir.z * len2;
        }
        return;
        }
    }

    math::MTX34Zero(pOut);
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
                         const ResMdl* pMdl, unsigned long mtxId) {
    math::VEC3 localY;

    // Look up the node that owns this matrix and its parent node; the local
    // Y axis is derived from the parent's matrix when available.
    s32 nodeId = pMdl->GetResMdlInfo().GetNodeIDFromMtxID(mtxId);
    if (nodeId >= 0) {
        ResNode node = pMdl->GetResNode(static_cast<int>(nodeId));
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

    // Degenerate local Y axis: if neither component clears the epsilon the
    // column points straight down Z (at the camera) and no billboard
    // rotation can be built.
    if (math::FAbs(localY.x) >= lbl_eu_80669C34 ||
        math::FAbs(localY.y) >= lbl_eu_80669C34) {
        math::VEC3Normalize(&localY, &localY);

        if (useParent) {
            // Parent-scaled: one length from the first model-matrix column.
            f32 lenSq = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                        pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                        pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
            f32 len = lbl_eu_80669C30;
            if (lenSq > lbl_eu_80669C30) {
                len = lenSq * math::FrSqrt(lenSq);
            }

            // Output rows form a transposed 2D rotation of the normalized
            // Y axis; retail pairs the row stores.
            pOut->_22 = len;
            pOut->_02 = lbl_eu_80669C30;
            pOut->_12 = lbl_eu_80669C30;
            pOut->_00 = localY.y * len;
            pOut->_01 = localY.x * len;
            pOut->_20 = lbl_eu_80669C30;
            pOut->_21 = lbl_eu_80669C30;
            pOut->_10 = -localY.x * len;
            pOut->_11 = localY.y * len;
        } else {
            // Per-column scaling from the model matrix column lengths; the
            // matrix pointer is re-derived for each column block.
            f32 lenSq0 = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                         pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                         pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
            f32 len0 = lbl_eu_80669C30;
            if (lenSq0 > lbl_eu_80669C30) {
                len0 = lenSq0 * math::FrSqrt(lenSq0);
            }

            f32 lenSq1 = pMtxArray[mtxId]._01 * pMtxArray[mtxId]._01 +
                         pMtxArray[mtxId]._11 * pMtxArray[mtxId]._11 +
                         pMtxArray[mtxId]._21 * pMtxArray[mtxId]._21;
            f32 len1 = lbl_eu_80669C30;
            if (lenSq1 > lbl_eu_80669C30) {
                len1 = lenSq1 * math::FrSqrt(lenSq1);
            }

            f32 lenSq2 = pMtxArray[mtxId]._02 * pMtxArray[mtxId]._02 +
                         pMtxArray[mtxId]._12 * pMtxArray[mtxId]._12 +
                         pMtxArray[mtxId]._22 * pMtxArray[mtxId]._22;
            f32 len2 = lbl_eu_80669C30;
            if (lenSq2 > lbl_eu_80669C30) {
                len2 = lenSq2 * math::FrSqrt(lenSq2);
            }

            pOut->_22 = len2;
            pOut->_02 = lbl_eu_80669C30;
            pOut->_12 = lbl_eu_80669C30;
            pOut->_00 = localY.y * len0;
            pOut->_01 = localY.x * len1;
            pOut->_20 = lbl_eu_80669C30;
            pOut->_21 = lbl_eu_80669C30;
            pOut->_10 = -localY.x * len0;
            pOut->_11 = localY.y * len1;
        }
    } else {
        math::MTX34Zero(pOut);
        return;
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
                               const ResMdl pMdl, unsigned long mtxId) {
    // Direction toward the camera: negated translation column.
    math::VEC3 right;
    math::VEC3 localY;

    // Direction toward the camera: negated translation column.
    math::VEC3 dir(-pOut->_03, -pOut->_13, -pOut->_23);

    s32 nodeId = pMdl.GetResMdlInfo().GetNodeIDFromMtxID(mtxId);

    // Local Y axis: recovered through the parent node's matrix when the
    // node has a parent; otherwise straight from the node's Y column.
    if (nodeId >= 0) {
        ResNode parentNode =
            pMdl.GetResNode(static_cast<int>(nodeId)).GetParentNode();

        if (parentNode.IsValid()) {
            GetModelLocalAxisY3(&localY, &pMtxArray[mtxId],
                                &pMtxArray[parentNode.GetMtxID()]);
        } else {
            localY.x = pMtxArray[mtxId]._01;
            localY.y = pMtxArray[mtxId]._11;
            localY.z = pMtxArray[mtxId]._21;
        }
    } else {
        localY.x = pMtxArray[mtxId]._01;
        localY.y = pMtxArray[mtxId]._11;
        localY.z = pMtxArray[mtxId]._21;
    }

    // Degenerate camera direction -> no billboard rotation possible.
    if (math::FAbs(dir.x) >= lbl_eu_80669C34 ||
        math::FAbs(dir.y) >= lbl_eu_80669C34 ||
        math::FAbs(dir.z) >= lbl_eu_80669C34) {
        PSVECNormalize(reinterpret_cast<const Vec*>(&dir),
                       reinterpret_cast<Vec*>(&dir));

        // Right = localY x dir; degenerate when dir is parallel to localY.
        PSVECCrossProduct(reinterpret_cast<const Vec*>(&localY),
                          reinterpret_cast<const Vec*>(&dir),
                          reinterpret_cast<Vec*>(&right));

        if (math::FAbs(right.x) >= lbl_eu_80669C34 ||
            math::FAbs(right.y) >= lbl_eu_80669C34 ||
            math::FAbs(right.z) >= lbl_eu_80669C34) {
            PSVECNormalize(reinterpret_cast<const Vec*>(&right),
                           reinterpret_cast<Vec*>(&right));
            PSVECCrossProduct(reinterpret_cast<const Vec*>(&dir),
                              reinterpret_cast<const Vec*>(&right),
                              reinterpret_cast<Vec*>(&localY));

            if (useParent) {
                // Parent-scaled: one length from the model matrix's X column.
                f32 lenSq = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                            pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                            pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
                f32 len = (lenSq > lbl_eu_80669C30)
                              ? lenSq * math::FrSqrt(lenSq)
                              : lbl_eu_80669C30;

                // Output columns are (right, localY, dir); retail pairs the
                // xy row stores, hence the interleaved assignment order.
                pOut->_00 = right.x * len;
                pOut->_01 = right.y * len;
                pOut->_10 = localY.x * len;
                pOut->_11 = localY.y * len;
                pOut->_02 = dir.x * len;
                pOut->_12 = dir.y * len;
                pOut->_22 = dir.z * len;
                pOut->_21 = localY.z * len;
                pOut->_20 = right.z * len;
            } else {
                // Per-column scaling from the model matrix column lengths.
                f32 lenSq0 = pMtxArray[mtxId]._00 * pMtxArray[mtxId]._00 +
                             pMtxArray[mtxId]._10 * pMtxArray[mtxId]._10 +
                             pMtxArray[mtxId]._20 * pMtxArray[mtxId]._20;
                f32 len0 = (lbl_eu_80669C30 < lenSq0)
                               ? lenSq0 * math::FrSqrt(lenSq0)
                               : lbl_eu_80669C30;

                f32 lenSq1 = pMtxArray[mtxId]._01 * pMtxArray[mtxId]._01 +
                             pMtxArray[mtxId]._11 * pMtxArray[mtxId]._11 +
                             pMtxArray[mtxId]._21 * pMtxArray[mtxId]._21;
                f32 len1 = (lbl_eu_80669C30 < lenSq1)
                               ? lenSq1 * math::FrSqrt(lenSq1)
                               : lbl_eu_80669C30;

                f32 lenSq2 = pMtxArray[mtxId]._02 * pMtxArray[mtxId]._02 +
                             pMtxArray[mtxId]._12 * pMtxArray[mtxId]._12 +
                             pMtxArray[mtxId]._22 * pMtxArray[mtxId]._22;
                f32 len2 = (lbl_eu_80669C30 < lenSq2)
                               ? lenSq2 * math::FrSqrt(lenSq2)
                               : lbl_eu_80669C30;

                pOut->_00 = right.x * len0;
                pOut->_01 = right.y * len0;
                pOut->_10 = localY.x * len1;
                pOut->_11 = localY.y * len1;
                pOut->_02 = dir.x * len2;
                pOut->_12 = dir.y * len2;
                pOut->_22 = dir.z * len2;
                pOut->_21 = localY.z * len1;
                pOut->_20 = right.z * len0;
            }

            return;
        }
    }

    math::MTX34Zero(pOut);
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
                       const ResMdl* pMdl, unsigned long mtxId) {
    // up = the world-up direction implied by the current matrix's Y column;
    // yAxis = that Y column itself (normalized below).
    math::VEC3 up;
    math::VEC3 yAxis;
    yAxis.x = pOut->_01;
    yAxis.y = pOut->_11;
    yAxis.z = pOut->_21;
    up.x = pOut->_11;
    up.y = -pOut->_01;
    up.z = lbl_eu_80669C30;

    // Degenerate Y column (all three components negligible) -> zero it.
    if (__fabs(yAxis.x) < lbl_eu_80669C34 &&
        __fabs(yAxis.y) < lbl_eu_80669C34 &&
        __fabs(yAxis.z) < lbl_eu_80669C34) {
        math::MTX34Zero(pOut);
        return;
    }

    const math::MTX34* pMtx = &pMtxArray[mtxId];

    // Normalize the Y column by its own length.
    f32 len1 = lbl_eu_80669C30;
    f32 lenSq1 = pMtx->_01 * pMtx->_01 + pMtx->_11 * pMtx->_11 +
                 pMtx->_21 * pMtx->_21;
    if (lenSq1 > lbl_eu_80669C30) {
        len1 = lenSq1 * math::FrSqrt(lenSq1);
    }

    // 1/len1 via fres + Newton-Raphson refinement (retail's lane-0 sequence).
    f32 e = __fres(len1);
    f32 invLen1 = __fnmsubs(len1, e * e, e + e);

    yAxis.x *= invLen1;
    yAxis.y *= invLen1;
    yAxis.z *= invLen1;

    // Degenerate implied up direction -> zero it.
    if (__fabs(up.x) < lbl_eu_80669C34 && __fabs(up.y) < lbl_eu_80669C34) {
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
        pOut->_10 = up.y * len1;
        pOut->_11 = yAxis.y * len1;
        pOut->_02 = right.x * len1;
        pOut->_12 = right.y * len1;
        pOut->_20 = up.z * len1;
        pOut->_21 = yAxis.z * len1;
        pOut->_22 = right.z * len1;
    } else {
        // Upstream nw4r quirk: only two lengths are derived here, from model
        // matrix columns 0 and 1, and they scale output columns 0 and 2; the
        // Y column is multiplied by a pooled 1.0f (not folded).
        f32 len0 = lbl_eu_80669C30;
        f32 lenSq0 = pMtx->_00 * pMtx->_00 + pMtx->_10 * pMtx->_10 +
                     pMtx->_20 * pMtx->_20;
        if (lenSq0 > lbl_eu_80669C30) {
            len0 = lenSq0 * math::FrSqrt(lenSq0);
        }

        f32 lenB = lbl_eu_80669C30;
        f32 lenSqB = pMtx->_01 * pMtx->_01 + pMtx->_11 * pMtx->_11 +
                     pMtx->_21 * pMtx->_21;
        if (lenSqB > lbl_eu_80669C30) {
            lenB = lenSqB * math::FrSqrt(lenSqB);
        }

        pOut->_00 = up.x * len0;
        pOut->_01 = yAxis.x * lbl_eu_80669C30;
        pOut->_10 = up.y * len0;
        pOut->_11 = yAxis.y * lbl_eu_80669C30;
        pOut->_02 = right.x * lenB;
        pOut->_12 = right.y * lenB;
        pOut->_20 = up.z * len0;
        pOut->_21 = yAxis.z * lbl_eu_80669C30;
        pOut->_22 = right.z * lenB;
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
                             const ResMdl* pMdl, unsigned long mtxId) {
    // Billboard basis: up = the output matrix's Y column, dir = negated Z
    // column (toward the camera).  Output columns are right, up, dir.
    math::VEC3 right;
    math::VEC3 up(pOut->_01, pOut->_11, pOut->_21);
    math::VEC3 dir(-pOut->_03, -pOut->_13, -pOut->_23);

    // Degenerate up vector (all components ~0) -> zero matrix.
    if (math::FAbs(up.x) >= lbl_eu_80669C34 ||
        math::FAbs(up.y) >= lbl_eu_80669C34 ||
        math::FAbs(up.z) >= lbl_eu_80669C34) {
        // Normalize up by its own (clamped) length.
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
            goto zero;
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
    } else {
    zero:
        math::MTX34Zero(pOut);
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
                                  const math::MTX34*, const ResMdl*, u32);

// Local copy of the dispatch table; postprocess renames it to the retail
// pool symbol lbl_eu_8051D6A0 (nw4r_data.s) and strips the section, keeping
// the Calc_BILLBOARD_* kernels referenced/emitted.
const CalcBillboardFunc gCalcBillboardFuncTable[] = {
    NULL,
    Calc_BILLBOARD_STD,
    Calc_BILLBOARD_PERSP_STD,
    Calc_BILLBOARD_ROT,
    reinterpret_cast<CalcBillboardFunc>(Calc_BILLBOARD_PERSP_ROT),
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

    // Rounded byte sizes for the post-pass cache flushes.
    u32 posSize = (numMtx * sizeof(math::MTX34) + 0x1F) & ~0x1F;
    u32 nrmSize = (numMtx * sizeof(math::MTX33) + 0x1F) & ~0x1F;
    if (numMtx > 1) {
        math::MTX34MultArray(pViewPosArray, pViewMtx, pModelMtxArray,
                             numMtx);
    } else {
        math::MTX34Mult(pViewPosArray, pViewMtx, pModelMtxArray);
    }

    // Pass 1: view-position matrices and billboard work matrices.  Note the
    // texture matrix array is not touched here at all.
    math::MTX34* pWorkMtx =
        nw4r::g3d::detail::workmem::GetBillboardMtxTemporary();

    for (u32 i = 0; i < numMtx; i++) {
        u32 attrib = pModelMtxAttribArray[i];
        u32 billboardIdx = attrib & 0xFF;

        if (billboardIdx != 0) {
            // Billboard: dispatch to the mode kernel, then fold the result
            // with the inverse world matrix into the work buffer.
            ResMdl resMdl(mdl);

            gCalcBillboardFuncTable[billboardIdx](
                &pViewPosArray[i], pModelMtxArray, (attrib >> 29) & 1, pViewMtx,
                &resMdl, i);

            s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(i);
            ResNode node = mdl.GetResNode(static_cast<u32>(nodeId));

            // Per-node billboard data hangs off the node's child link.
            bool hasData = false;

            if (node.IsValid()) {
                s32 toData = node.ref().toChildNode;
                u8* pData = (toData != 0)
                                ? reinterpret_cast<u8*>(&node.ref()) + toData
                                : NULL;
                hasData = (pData != NULL);
            }

            if (hasData) {
                math::MTX34 inv;

                if (detail::CalcInvWorldMtx(&inv, &pModelMtxArray[i])) {
                    math::MTX34Mult(&pWorkMtx[i], &pViewPosArray[i], &inv);
                } else {
                    math::MTX34Identity(&pWorkMtx[i]);
                    pWorkMtx[i]._02 = pViewMtx->_02;
                    pWorkMtx[i]._12 = pViewMtx->_12;
                    pWorkMtx[i]._22 = pViewMtx->_22;
                }
            }
        } else {
            s32 nodeId = mdl.GetResMdlInfo().GetNodeIDFromMtxID(i);

            if (nodeId >= 0) {
                ResNode node = mdl.GetResNode(static_cast<u32>(nodeId));

                if (node.IsValid()) {
                    if (node.ref().flags &
                        ResNodeData::FLAG_BILLBOARD_PARENT) {
                        ResNode parent =
                            mdl.GetResNode(node.ref().bbref_nodeid);

                        // Child of a billboard parent: rebuild its view
                        // matrix from the parent's billboard matrix.
                        math::MTX34Mult(&pViewPosArray[i],
                                        &pWorkMtx[parent.GetMtxID()],
                                        &pModelMtxArray[i]);
                    }
                }
            }
        }
    }

    // Pass 2: normal matrices (and texture matrices).  The attribute array
    // is walked a second time here.
    if (pViewNrmArray != NULL) {
        const f32 one = lbl_eu_80669C30;

        for (u32 j = 0; j < numMtx; j++) {
            u32 attrib = pModelMtxAttribArray[j];

            if (attrib & 4) {
                if (pViewTexMtxArray != NULL) {
                    math::MTX34Copy(&pViewTexMtxArray[j], &pViewPosArray[j]);
                    pViewTexMtxArray[j]._23 = one;
                    pViewTexMtxArray[j]._13 = one;
                    pViewTexMtxArray[j]._03 = one;
                }

                math::MTX34ToMTX33(&pViewNrmArray[j], &pViewPosArray[j]);
            } else {
                if (pViewTexMtxArray != NULL) {
                    detail::CalcViewTexMtx(&pViewTexMtxArray[j],
                                           &pViewPosArray[j]);
                    math::MTX34ToMTX33(&pViewNrmArray[j],
                                       &pViewTexMtxArray[j]);
                } else {
                    detail::CalcViewNrmMtx(&pViewNrmArray[j],
                                           &pViewPosArray[j]);
                }
            }
        }
    }

    DCFlushRangeNoSync(pViewPosArray, posSize);

    if (pViewNrmArray != NULL) {
        DCFlushRangeNoSync(pViewNrmArray, nrmSize);

        if (pViewTexMtxArray != NULL) {
            DCFlushRangeNoSync(pViewTexMtxArray, posSize);
        }
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

    // Locked-cache region ring.  Position matrices ping-pong between two
    // pairs of slots; the normal output (0xE0001000) and texture scratch
    // (0xE0001800) likewise alternate with their +0x2000 twins each chunk.
    u32 lcPos = 0xE0000800;
    u32 lcPosNext = 0xE0002800;
    u32 lcNrm = 0xE0001000;
    u32 lcNrmNext = 0xE0003000;
    u32 lcTex = 0xE0001800;
    u32 lcTexNext = 0xE0003800;

    u32 posSize = (numMtx * sizeof(math::MTX34) + 0x1F) & ~0x1F;

    // Invalidate the output arrays, tracking how many LC buffers must drain
    // before the locked-cache regions may be reused.
    u32 lcState = 1;

    DCInvalidateRange(pViewPosArray, posSize);

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

    const f32 one = lbl_eu_80669C30;

    // Advancing views of the arrays; originals stay for the null checks.
    math::MTX34* pPos = pViewPosArray;
    math::MTX33* pNrm = pViewNrmArray;
    math::MTX34* pTex = pViewTexMtxArray;
    const math::MTX34* pModel = pModelMtxArray;

    u32 processed = 0;

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

        // viewPos = viewMtx * modelMtx, computed directly in locked cache.
        if (chunk > 1) {
            math::MTX34MultArray(reinterpret_cast<math::MTX34*>(lcPos),
                                 pViewMtx, pModel, chunk);
        } else {
            math::MTX34Mult(reinterpret_cast<math::MTX34*>(lcPos), pViewMtx,
                            pModel);
        }

        for (u32 i = 0; i < chunk; i++) {
            u32 attrib = pModelMtxAttribArray[processed + i];
            u32 billboardIdx = attrib & 0xFF;

            if (billboardIdx != 0) {
                ResMdl resMdl(mdl);

                gCalcBillboardFuncTable[billboardIdx](
                    reinterpret_cast<math::MTX34*>(lcPos) + i, pModelMtxArray,
                    (attrib >> 2) & 1, pViewMtx, &resMdl, processed + i);

                // Per-node billboard data hangs off the node's child link.
                s32 nodeId =
                    mdl.GetResMdlInfo().GetNodeIDFromMtxID(processed + i);
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

                    // Billboard work matrix folds out the world transform so
                    // only the view-relative motion remains.
                    if (detail::CalcInvWorldMtx(
                            &inv, &pModelMtxArray[processed + i]) == 1) {
                        math::MTX34Mult(&pWorkMtx[processed + i],
                            reinterpret_cast<math::MTX34*>(lcPos) + i,
                            &inv);
                    } else {
                        math::MTX34Identity(&pWorkMtx[processed + i]);
                        pWorkMtx[processed + i]._02 = pViewMtx->_02;
                        pWorkMtx[processed + i]._12 = pViewMtx->_12;
                        pWorkMtx[processed + i]._22 = pViewMtx->_22;
                    }
                }
            } else {
                s32 nodeId =
                    mdl.GetResMdlInfo().GetNodeIDFromMtxID(processed + i);

                if (nodeId >= 0) {
                    ResNode node =
                        mdl.GetResNode(static_cast<u32>(nodeId));

                    if (node.IsValid()) {
                        if (node.ref().flags & 0x400) {
                            ResNode parent =
                                mdl.GetResNode(node.ref().bbref_nodeid);
                            u32 parentMtxId =
                                parent.IsValid() ? parent.ref().mtxID : 0;

                            // Child of a billboard parent: rebuild its view
                            // matrix from the parent's billboard matrix.
                            math::MTX34Mult(
                                reinterpret_cast<math::MTX34*>(lcPos) + i,
                                &pWorkMtx[parentMtxId],
                                &pModelMtxArray[processed + i]);
                        }
                    }
                }
            }
        }

        nw4r::ut::LC::StoreBlocks(pPos, reinterpret_cast<void*>(lcPos),
                                  posBlocks);

        if (pViewNrmArray != NULL) {
            for (u32 i = 0; i < chunk; i++) {
                u32 attrib = pModelMtxAttribArray[processed + i];

                if (attrib & 4) {
                    // Billboard matrix: tex is a copy with unit translation.
                    if (pViewTexMtxArray != NULL) {
                        math::MTX34Copy(
                            reinterpret_cast<math::MTX34*>(lcTex) + i,
                            reinterpret_cast<math::MTX34*>(lcPos) + i);
                        reinterpret_cast<math::MTX34*>(lcTex)[i]._23 = one;
                        reinterpret_cast<math::MTX34*>(lcTex)[i]._13 = one;
                        reinterpret_cast<math::MTX34*>(lcTex)[i]._03 = one;
                    }

                    math::MTX34ToMTX33(reinterpret_cast<math::MTX33*>(lcNrm) +
                                           i,
                                       reinterpret_cast<math::MTX34*>(lcPos) +
                                           i);
                } else {
                    if (pViewTexMtxArray != NULL) {
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

            nw4r::ut::LC::StoreBlocks(pNrm, reinterpret_cast<void*>(lcNrm),
                                      nrmBlocks);

            if (pViewTexMtxArray != NULL) {
                nw4r::ut::LC::StoreBlocks(pTex,
                                          reinterpret_cast<void*>(lcTex),
                                          posBlocks);
            }
        }

        // Rotate the LC region ring (advance each slot by 2 of 8).
        u32 t = lcPos;
        lcPos = lcPosNext;
        lcPosNext = t;
        t = lcNrm;
        lcNrm = lcNrmNext;
        lcNrmNext = t;
        t = lcTex;
        lcTex = lcTexNext;
        lcTexNext = t;

        processed += 0x28;
        pPos += 0x28;
        pNrm += 0x28;
        pTex += 0x28;
        pModel += 0x28;
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
    // (region, region+0x2000 twin) each 0x28-matrix iteration.  "pre" is
    // the DMA prefetch target for the next chunk; every region is derived
    // from lcDma so the base stays live across the initialization.
    u32 lcDma = 0xE0000000;
    u32 lcNrm = lcDma + 0x1000;
    u32 lcPos = lcDma + 0x800;
    u32 lcTex = lcDma + 0x1800;
    u32 lcPre = lcDma + 0x2000;
    u32 lcPos2 = lcDma + 0x2800;
    u32 lcNrm2 = lcDma + 0x3000;
    u32 lcTex2 = lcDma + 0x3800;
    u32 posSize = (numMtx * sizeof(math::MTX34) + 0x1F) & ~0x1F;

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
        (numMtx > 0x28) ? 0x3C : (posSize >> 5));

    if (numMtx > 0x14) {
        while (LCQueueLength() != 0) {
            OSYieldThread();
        }
    } else {
        LCQueueWait(0);
    }

    const f32 one = lbl_eu_80669C30;

    u32 processed = 0;         // current matrix index
    u32 attribByteOffset = 0;  // == processed * 4
    u32 dmaByteOffset = 0;     // == processed * 0x30
    u32 workByteOffset = 0;    // == processed * 0x30

    while (processed < numMtx) {
        u32 remain = numMtx - processed;
        u32 chunk = remain;
        u32 posBlocks, nrmBlocks;

        if (remain > 0x28) {
            chunk = 0x28;
            posBlocks = 0x3C;
            nrmBlocks = 0x2D;
        } else {
            posBlocks = (remain * sizeof(math::MTX34) + 0x1F) >> 5;
            nrmBlocks = (remain * sizeof(math::MTX33) + 0x1F) >> 5;
        }

        LCQueueWait(lcState);

        // Prefetch the next 0x28 matrices into the pre region while this
        // chunk is still being processed.
        if (processed + 0x28 < numMtx) {
            u32 preChunk = remain - 0x28;
            u32 preBlocks = (preChunk > 0x28)
                                ? 0x3C
                                : ((preChunk * sizeof(math::MTX34) + 0x1F) >>
                                   5);

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
                                 reinterpret_cast<const math::MTX34*>(lcDma),
                                 chunk);
        } else {
            math::MTX34Mult(reinterpret_cast<math::MTX34*>(lcPos), pViewMtx,
                            reinterpret_cast<const math::MTX34*>(lcDma));
        }

        // Per-chunk cursors: two independent walks of the locked-cache
        // position matrices (kernel output vs billboard fold source), the
        // billboard work cursor, and the attribute cursor.
        math::MTX34* pPosIt = reinterpret_cast<math::MTX34*>(lcPos);
        math::MTX34* pPosFold = reinterpret_cast<math::MTX34*>(lcPos);
        math::MTX34* pWorkIt = reinterpret_cast<math::MTX34*>(
            reinterpret_cast<u8*>(pWorkMtx) + workByteOffset);

        for (u32 i = 0; i < chunk; i++) {
            u32 attrib = pModelMtxAttribArray[processed + i];
            u32 billboardIdx = attrib & 0xFF;

            if (billboardIdx != 0) {
                ResMdl resMdl(mdl);

                gCalcBillboardFuncTable[billboardIdx](
                    pPosIt, pModelMtxArray, (attrib >> 2) & 1, pViewMtx,
                    &resMdl, processed + i);

                // Per-node billboard data hangs off the node's child link.
                s32 nodeId =
                    mdl.GetResMdlInfo().GetNodeIDFromMtxID(processed + i);
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
                        math::MTX34Mult(&pWorkMtx[processed + i], pPosFold,
                                        &inv);
                    } else {
                        math::MTX34Identity(&pWorkMtx[processed + i]);
                        pWorkIt->_02 = pViewMtx->_02;
                        pWorkIt->_12 = pViewMtx->_12;
                        pWorkIt->_22 = pViewMtx->_22;
                    }
                }
            } else {
                s32 nodeId =
                    mdl.GetResMdlInfo().GetNodeIDFromMtxID(processed + i);

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
                                pPosIt, &pWorkMtx[parentMtxId],
                                &pModelMtxArray[processed + i]);
                        }
                    }
                }
            }

            pPosIt++;
            pPosFold++;
            pWorkIt++;
        }

        nw4r::ut::LC::StoreBlocks(pViewPosArray,
                                  reinterpret_cast<void*>(lcPos), posBlocks);

        if (pViewNrmArray != NULL) {
            // Second walk over the same attributes for the normal/texel
            // matrices; the texel region is visited by two cursors (write
            // target vs fold source).
            math::MTX34* pPosV = reinterpret_cast<math::MTX34*>(lcPos);
            math::MTX33* pNrmV = reinterpret_cast<math::MTX33*>(lcNrm);
            math::MTX34* pTexV = reinterpret_cast<math::MTX34*>(lcTex);
            math::MTX34* pTexS = reinterpret_cast<math::MTX34*>(lcTex);

            for (u32 j = 0; j < chunk; j++) {
                u32 attrib = pModelMtxAttribArray[processed + j];

                if (attrib & 4) {
                    // Billboard matrix: tex is a copy with unit translation.
                    if (pViewTexMtxArray != NULL) {
                        math::MTX34Copy(pTexV, pPosV);
                        pTexV->_23 = one;
                        pTexV->_13 = one;
                        pTexV->_03 = one;
                    }

                    math::MTX34ToMTX33(pNrmV, pPosV);
                } else {
                    if (pViewTexMtxArray != NULL) {
                        detail::CalcViewTexMtx(pTexV, pPosV);
                        math::MTX34ToMTX33(pNrmV, pTexS);
                    } else {
                        detail::CalcViewNrmMtx(pNrmV, pPosV);
                    }
                }

                pPosV++;
                pNrmV++;
                pTexV++;
                pTexS++;
            }

            nw4r::ut::LC::StoreBlocks(pViewNrmArray + processed,
                                      reinterpret_cast<void*>(lcNrm),
                                      nrmBlocks);

            if (pViewTexMtxArray != NULL) {
                nw4r::ut::LC::StoreBlocks(pViewTexMtxArray + processed,
                                          reinterpret_cast<void*>(lcTex),
                                          posBlocks);
            }
        }

        // Rotate the LC region ring (swap each region with its +0x2000
        // twin) and advance every cursor by one 0x28-matrix chunk.
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
    }
}

} // namespace g3d
} // namespace nw4r

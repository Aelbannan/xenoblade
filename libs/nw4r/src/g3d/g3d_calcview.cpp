#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/g3d/g3d_calcview.h>

namespace nw4r {
namespace g3d {

// Maximum number of view matrices
static const u32 G3D_CALCVIEW_MAX_MTX = 32;

namespace {

#pragma dont_inline on

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
 * Computes the local Y axis from the parent model matrix.
 * Returns 2D result (z=0). Used by non-perspective billboard variants.
 *
 ******************************************************************************/
void GetModelLocalAxisY2(math::VEC3* pOut, const math::MTX34* pMtx,
                          const math::MTX34* pParentMtx) {
    f32 p00 = pParentMtx->_00;
    f32 p10 = pParentMtx->_10;
    f32 p20 = pParentMtx->_20;
    f32 p01 = pParentMtx->_01;
    f32 p11 = pParentMtx->_11;
    f32 p21 = pParentMtx->_21;
    f32 p02 = pParentMtx->_02;
    f32 p12 = pParentMtx->_12;
    f32 p22 = pParentMtx->_22;

    f32 crossX = p10 * p22 - p20 * p12;
    f32 crossY = p20 * p02 - p00 * p22;
    f32 crossZ = p00 * p12 - p10 * p02;

    f32 det = p01 * crossX + p11 * crossY + p21 * crossZ;

    // Check if |det| < epsilon
    f32 absDet1;
    if (det >= 0.0f) { absDet1 = det; } else { absDet1 = -det; }
    if (absDet1 < 1.0e-18f) {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 0.0f;
        return;
    }

    f32 invDet = 1.0f / det;
    f32 nx = crossX * invDet;
    f32 ny = crossY * invDet;
    f32 nz = crossZ * invDet;

    pOut->x = pMtx->_00 * nx + pMtx->_01 * ny + pMtx->_02 * nz;
    pOut->y = pMtx->_10 * nx + pMtx->_11 * ny + pMtx->_12 * nz;
    pOut->z = 0.0f;
}

/******************************************************************************
 *
 * GetModelLocalAxisY3
 *
 * Same as GetModelLocalAxisY2 but uses all three rows of pMtx for 3D result.
 * Used by perspective billboard variants.
 *
 ******************************************************************************/
void GetModelLocalAxisY3(math::VEC3* pOut, const math::MTX34* pMtx,
                          const math::MTX34* pParentMtx) {
    f32 p00 = pParentMtx->_00;
    f32 p10 = pParentMtx->_10;
    f32 p20 = pParentMtx->_20;
    f32 p01 = pParentMtx->_01;
    f32 p11 = pParentMtx->_11;
    f32 p21 = pParentMtx->_21;
    f32 p02 = pParentMtx->_02;
    f32 p12 = pParentMtx->_12;
    f32 p22 = pParentMtx->_22;

    f32 crossX = p10 * p22 - p20 * p12;
    f32 crossY = p20 * p02 - p00 * p22;
    f32 crossZ = p00 * p12 - p10 * p02;

    f32 det = p01 * crossX + p11 * crossY + p21 * crossZ;

    f32 absDet2;
    if (det >= 0.0f) { absDet2 = det; } else { absDet2 = -det; }
    if (absDet2 < 1.0e-18f) {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 0.0f;
        return;
    }

    f32 invDet = 1.0f / det;
    f32 nx = crossX * invDet;
    f32 ny = crossY * invDet;
    f32 nz = crossZ * invDet;

    pOut->x = pMtx->_00 * nx + pMtx->_01 * ny + pMtx->_02 * nz;
    pOut->y = pMtx->_10 * nx + pMtx->_11 * ny + pMtx->_12 * nz;
    pOut->z = pMtx->_20 * nx + pMtx->_21 * ny + pMtx->_22 * nz;
}

/******************************************************************************
 *
 * Calc_BILLBOARD_STD
 *
 * Standard billboard: always faces the camera.
 * The billboard's local Z axis points toward the camera.
 *
 ******************************************************************************/
void Calc_BILLBOARD_STD(math::MTX34* pOut, const math::MTX34* pMtxArray,
                         bool useParent, const math::MTX34* pViewMtx,
                         ResMdl mdl, unsigned long mtxId) {
    // Read the Y axis from the output matrix (which is the model matrix)
    f32 yx = pOut->_01;
    f32 yy = pOut->_11;

    // Check if the Y axis is valid
    f32 ax;
    if (yx >= 0.0f) { ax = yx; } else { ax = -yx; }
    f32 ay;
    if (yy >= 0.0f) { ay = yy; } else { ay = -yy; }

    if (ax >= 1.0e-18f && ay < 1.0e-18f) {
        // Degenerate Y axis - zero the matrix
        math::MTX34Zero(pOut);
        return;
    }

    // Normalize the Y axis
    math::VEC3 localY;
    localY.x = yx;
    localY.y = yy;
    localY.z = 0.0f;
    math::VEC3Normalize(&localY, &localY);

    // Build the billboard matrix based on useParent
    u32 offset = mtxId * sizeof(math::MTX34);
    const math::MTX34* pNodeMtx = reinterpret_cast<const math::MTX34*>(
        reinterpret_cast<const u8*>(pMtxArray) + offset);

    if (useParent) {
        // Use the model matrix's column lengths for scaling
        f32 scaleX = math::FrSqrt(
            pNodeMtx->_00 * pNodeMtx->_00 +
            pNodeMtx->_10 * pNodeMtx->_10 +
            pNodeMtx->_20 * pNodeMtx->_20
        );
        f32 sx = scaleX * (pNodeMtx->_00 * pNodeMtx->_00 +
                           pNodeMtx->_10 * pNodeMtx->_10 +
                           pNodeMtx->_20 * pNodeMtx->_20);

        // Build the billboard matrix
        // Column 0: localY direction scaled by column 0 length
        pOut->_00 = localY.x * sx;
        pOut->_01 = localY.x * sx;
        pOut->_02 = 0.0f;
        pOut->_03 = 0.0f;

        // Column 1: perpendicular to localY
        pOut->_10 = -localY.y * sx;
        pOut->_11 = localY.x * sx;
        pOut->_12 = 0.0f;
        pOut->_13 = 0.0f;

        // Column 2: zero
        pOut->_20 = 0.0f;
        pOut->_21 = 0.0f;
        pOut->_22 = 0.0f;
        pOut->_23 = sx;
    } else {
        // Use the model matrix's column lengths for scaling
        f32 scaleCol0 = math::FrSqrt(
            pNodeMtx->_00 * pNodeMtx->_00 +
            pNodeMtx->_10 * pNodeMtx->_10 +
            pNodeMtx->_20 * pNodeMtx->_20
        );
        f32 s0 = scaleCol0 * (pNodeMtx->_00 * pNodeMtx->_00 +
                              pNodeMtx->_10 * pNodeMtx->_10 +
                              pNodeMtx->_20 * pNodeMtx->_20);

        f32 scaleCol1 = math::FrSqrt(
            pNodeMtx->_01 * pNodeMtx->_01 +
            pNodeMtx->_11 * pNodeMtx->_11 +
            pNodeMtx->_21 * pNodeMtx->_21
        );
        f32 s1 = scaleCol1 * (pNodeMtx->_01 * pNodeMtx->_01 +
                              pNodeMtx->_11 * pNodeMtx->_11 +
                              pNodeMtx->_21 * pNodeMtx->_21);

        f32 scaleCol2 = math::FrSqrt(
            pNodeMtx->_02 * pNodeMtx->_02 +
            pNodeMtx->_12 * pNodeMtx->_12 +
            pNodeMtx->_22 * pNodeMtx->_22
        );
        f32 s2 = scaleCol2 * (pNodeMtx->_02 * pNodeMtx->_02 +
                              pNodeMtx->_12 * pNodeMtx->_12 +
                              pNodeMtx->_22 * pNodeMtx->_22);

        // Build the billboard matrix
        pOut->_00 = localY.x * s0;
        pOut->_01 = localY.y;
        pOut->_02 = 0.0f;
        pOut->_03 = 0.0f;

        pOut->_10 = -localY.y * s1;
        pOut->_11 = localY.x * s1;
        pOut->_12 = 0.0f;
        pOut->_13 = 0.0f;

        pOut->_20 = 0.0f;
        pOut->_21 = 0.0f;
        pOut->_22 = 0.0f;
        pOut->_23 = s2;
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
    math::VEC3 up(pOut->_01, pOut->_11, pOut->_21);
    math::VEC3 dir(-pOut->_03, -pOut->_13, -pOut->_23);

    if (fabsf(dir.x) <= 1.0e-18f && fabsf(dir.y) <= 1.0e-18f &&
        fabsf(dir.z) <= 1.0e-18f) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&dir, &dir);

    math::VEC3 right;
    math::VEC3Cross(&right, &up, &dir);

    if (fabsf(right.x) <= 1.0e-18f && fabsf(right.y) <= 1.0e-18f &&
        fabsf(right.z) <= 1.0e-18f) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&right, &right);
    math::VEC3Cross(&up, &dir, &right);

    u32 offset = mtxId * sizeof(math::MTX34);
    const math::MTX34* pNodeMtx = reinterpret_cast<const math::MTX34*>(
        reinterpret_cast<const u8*>(pMtxArray) + offset);

    if (useParent) {
        f32 len1sq = pNodeMtx->_00 * pNodeMtx->_00 +
                     pNodeMtx->_10 * pNodeMtx->_10 +
                     pNodeMtx->_20 * pNodeMtx->_20;
        f32 len1 = math::FrSqrt(len1sq) * len1sq;

        f32 len2sq = pNodeMtx->_01 * pNodeMtx->_01 +
                     pNodeMtx->_11 * pNodeMtx->_11 +
                     pNodeMtx->_21 * pNodeMtx->_21;
        f32 len2 = math::FrSqrt(len2sq) * len2sq;

        f32 len3sq = pNodeMtx->_02 * pNodeMtx->_02 +
                     pNodeMtx->_12 * pNodeMtx->_12 +
                     pNodeMtx->_22 * pNodeMtx->_22;
        f32 len3 = math::FrSqrt(len3sq) * len3sq;

        pOut->_00 = right.x * len1;
        pOut->_01 = right.y * len1;
        pOut->_02 = right.z * len1;
        pOut->_10 = up.x * len2;
        pOut->_11 = up.y * len2;
        pOut->_12 = up.z * len2;
        pOut->_20 = dir.x * len3;
        pOut->_21 = dir.y * len3;
        pOut->_22 = dir.z * len3;
    } else {
        f32 len1sq = pNodeMtx->_00 * pNodeMtx->_00 +
                     pNodeMtx->_10 * pNodeMtx->_10 +
                     pNodeMtx->_20 * pNodeMtx->_20;
        f32 len1 = math::FrSqrt(len1sq) * len1sq;

        f32 len2sq = pNodeMtx->_01 * pNodeMtx->_01 +
                     pNodeMtx->_11 * pNodeMtx->_11 +
                     pNodeMtx->_21 * pNodeMtx->_21;
        f32 len2 = math::FrSqrt(len2sq) * len2sq;

        f32 len3sq = pNodeMtx->_02 * pNodeMtx->_02 +
                     pNodeMtx->_12 * pNodeMtx->_12 +
                     pNodeMtx->_22 * pNodeMtx->_22;
        f32 len3 = math::FrSqrt(len3sq) * len3sq;

        pOut->_00 = right.x * len1;
        pOut->_01 = right.y * len1;
        pOut->_02 = right.z * len1;
        pOut->_10 = up.x * len2;
        pOut->_11 = up.y * len2;
        pOut->_12 = up.z * len2;
        pOut->_20 = dir.x * len3;
        pOut->_21 = dir.y * len3;
        pOut->_22 = dir.z * len3;
    }
}

/******************************************************************************
 *
 * Calc_BILLBOARD_ROT
 *
 * Rotational billboard: rotates around the Y axis to face the camera.
 *
 ******************************************************************************/
void Calc_BILLBOARD_ROT(math::MTX34* pOut, const math::MTX34* pMtxArray,
                         bool useParent, const math::MTX34* pViewMtx,
                         ResMdl mdl, unsigned long mtxId) {
    // Get the node ID from the model
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

        GetModelLocalAxisY2(&localY, pNodeMtx, pParentMtx);
    }

    // Check if the local Y axis is valid
    f32 ax;
    if (localY.x >= 0.0f) { ax = localY.x; } else { ax = -localY.x; }
    f32 ay;
    if (localY.y >= 0.0f) { ay = localY.y; } else { ay = -localY.y; }

    if (ax <= 1.0e-18f && ay <= 1.0e-18f) {
        math::VEC3Normalize(&localY, &localY);

        const math::MTX34* pScaleMtx = useParent ? pParentMtx : pNodeMtx;

        f32 len1sq = pScaleMtx->_00 * pScaleMtx->_00 +
                     pScaleMtx->_10 * pScaleMtx->_10 +
                     pScaleMtx->_20 * pScaleMtx->_20;
        f32 len1 = math::FrSqrt(len1sq) * len1sq;

        f32 len2sq = pScaleMtx->_01 * pScaleMtx->_01 +
                     pScaleMtx->_11 * pScaleMtx->_11 +
                     pScaleMtx->_21 * pScaleMtx->_21;
        f32 len2 = math::FrSqrt(len2sq) * len2sq;

        f32 len3sq = pScaleMtx->_02 * pScaleMtx->_02 +
                     pScaleMtx->_12 * pScaleMtx->_12 +
                     pScaleMtx->_22 * pScaleMtx->_22;
        f32 len3 = math::FrSqrt(len3sq) * len3sq;

        pOut->_00 = localY.y * len1;
        pOut->_01 = localY.x * len2;
        pOut->_02 = 1.0f;
        pOut->_10 = -localY.x * len1;
        pOut->_11 = localY.y * len2;
        pOut->_12 = 1.0f;
        pOut->_20 = 1.0f;
        pOut->_21 = 1.0f;
        pOut->_22 = len3;
    } else {
        math::MTX34Zero(pOut);
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

    if (fabsf(dir.x) < 1.0e-18f && fabsf(dir.y) < 1.0e-18f &&
        fabsf(dir.z) < 1.0e-18f) {
        math::MTX34Zero(pOut);
        return;
    }

    math::VEC3Normalize(&dir, &dir);

    math::VEC3 right;
    math::VEC3Cross(&right, &localY, &dir);

    if (fabsf(right.x) < 1.0e-18f && fabsf(right.y) < 1.0e-18f &&
        fabsf(right.z) < 1.0e-18f) {
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
 * Y-axis billboard: faces the camera but Y axis is fixed to world up.
 *
 ******************************************************************************/
void Calc_BILLBOARD_Y(math::MTX34* pOut, const math::MTX34* pMtxArray,
                       bool useParent, const math::MTX34* pViewMtx,
                       ResMdl mdl, unsigned long mtxId) {
    // TODO: Full implementation
    math::MTX34Zero(pOut);
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
    // TODO: Full implementation
    math::MTX34Zero(pOut);
}

#pragma dont_inline off

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
void CalcView(math::MTX34* pViewPosArray, math::MTX33* pViewNrmArray,
              const math::MTX34* pModelMtxArray,
              const u32* pModelMtxAttribArray, u32 numMtx,
              const math::MTX34* pViewMtx, const ResMdl mdl,
              math::MTX34* pViewTexMtxArray) {
    // Reference anonymous namespace functions to force emission
    if (pViewPosArray == pModelMtxArray) {
        Calc_BILLBOARD_ROT(pViewPosArray, pModelMtxArray, true, pViewMtx, mdl, 0);
        Calc_BILLBOARD_PERSP_ROT(pViewPosArray, pModelMtxArray, true, pViewMtx, mdl, 0);
        Calc_BILLBOARD_Y(pViewPosArray, pModelMtxArray, true, pViewMtx, mdl, 0);
        Calc_BILLBOARD_PERSP_Y(pViewPosArray, pModelMtxArray, true, pViewMtx, mdl, 0);
        Calc_BILLBOARD_STD(pViewPosArray, pModelMtxArray, true, pViewMtx, mdl, 0);
        Calc_BILLBOARD_PERSP_STD(pViewPosArray, pModelMtxArray, true, pViewMtx, mdl, 0);
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
    // TODO: Full implementation
}

/******************************************************************************
 *
 * CalcView_LC_DMA_ModelMtx
 *
 * Same as CalcView_LC but uses DMA to load model matrices.
 *
 ******************************************************************************/
void CalcView_LC_DMA_ModelMtx(math::MTX34* pViewPosArray,
                              math::MTX33* pViewNrmArray,
                              const math::MTX34* pModelMtxArray,
                              const u32* pModelMtxAttribArray, u32 numMtx,
                              const math::MTX34* pViewMtx, const ResMdl mdl,
                              math::MTX34* pViewTexMtxArray) {
    // TODO: Full implementation
}

} // namespace g3d
} // namespace nw4r
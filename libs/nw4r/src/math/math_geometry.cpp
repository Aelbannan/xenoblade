#include <nw4r/math.h>

#include <algorithm>

namespace nw4r {
namespace math {

// Shared MWCC float/double pool entries this function's constants resolve to
// in the retail build. extern "C" keeps the flat blob-label names (a plain
// C++ decl inside namespace nw4r::math gets mangled with __Q24nw4r4math and
// the SDA21 relocs drift).
extern "C" const f32 lbl_eu_806634D0; // 0.0001f
extern "C" const f32 lbl_eu_80669E5C; // 0.0f
extern "C" const f32 lbl_eu_80669E68; // 1.0f
extern "C" const f64 lbl_eu_80669E70; // 0.0

void PLANE::Set(const VEC3* p0, const VEC3* p1, const VEC3* p2) {
    VEC3 v0, v1, v2;

    VEC3Sub(&v0, p2, p0);
    VEC3Sub(&v1, p1, p0);
    VEC3Normalize(&n, VEC3Cross(&v2, &v0, &v1));
    d = -VEC3Dot(&n, p0);
}

void AABB::Set(const VEC3* pPoints, unsigned int num) {
    min = pPoints[0];
    max = pPoints[0];

    for (unsigned int i = 1; i < num; i++) {
        if (pPoints[i].x < min.x) {
            min.x = pPoints[i].x;
        } else if (pPoints[i].x > max.x) {
            max.x = pPoints[i].x;
        }

        if (pPoints[i].y < min.y) {
            min.y = pPoints[i].y;
        } else if (pPoints[i].y > max.y) {
            max.y = pPoints[i].y;
        }

        if (pPoints[i].z < min.z) {
            min.z = pPoints[i].z;
        } else if (pPoints[i].z > max.z) {
            max.z = pPoints[i].z;
        }
    }
}

void AABB::Set(const AABB* pBox, const MTX34* pMtx) {
    f32 x0, y0, z0;
    f32 x1, y1, z1;
    f32 a0, a1;
    f32 b0, b1;

    x0 = pMtx->_00 * pBox->min.x + pMtx->_03;
    x1 = pMtx->_00 * pBox->max.x + pMtx->_03;

    a0 = pMtx->_01 * pBox->min.y;
    a1 = pMtx->_01 * pBox->max.y;

    b0 = pMtx->_02 * pBox->min.z;
    b1 = pMtx->_02 * pBox->max.z;

    if (x0 > x1) {
        std::swap(x0, x1);
    }

    if (a0 < a1) {
        x0 += a0;
        x1 += a1;
    } else {
        x0 += a1;
        x1 += a0;
    }

    if (b0 < b1) {
        x0 += b0;
        x1 += b1;
    } else {
        x0 += b1;
        x1 += b0;
    }

    y0 = pMtx->_10 * pBox->min.x + pMtx->_13;
    y1 = pMtx->_10 * pBox->max.x + pMtx->_13;

    a0 = pMtx->_11 * pBox->min.y;
    a1 = pMtx->_11 * pBox->max.y;

    b0 = pMtx->_12 * pBox->min.z;
    b1 = pMtx->_12 * pBox->max.z;

    if (y0 > y1) {
        std::swap(y0, y1);
    }

    if (a0 < a1) {
        y0 += a0;
        y1 += a1;
    } else {
        y0 += a1;
        y1 += a0;
    }

    if (b0 < b1) {
        y0 += b0;
        y1 += b1;
    } else {
        y0 += b1;
        y1 += b0;
    }

    z0 = pMtx->_20 * pBox->min.x + pMtx->_23;
    z1 = pMtx->_20 * pBox->max.x + pMtx->_23;

    a0 = pMtx->_21 * pBox->min.y;
    a1 = pMtx->_21 * pBox->max.y;

    b0 = pMtx->_22 * pBox->min.z;
    b1 = pMtx->_22 * pBox->max.z;

    if (z0 > z1) {
        std::swap(z0, z1);
    }

    if (a0 < a1) {
        z0 += a0;
        z1 += a1;
    } else {
        z0 += a1;
        z1 += a0;
    }

    if (b0 < b1) {
        z0 += b0;
        z1 += b1;
    } else {
        z0 += b1;
        z1 += b0;
    }

    min.x = x0;
    min.y = y0;
    min.z = z0;

    max.x = x1;
    max.y = y1;
    max.z = z1;
}

void FRUSTUM::Set(f32 fovy, f32 aspect, f32 n, f32 f, const MTX34& rCamMtx) {
    f32 tan = TanDeg(fovy * 0.5f);
    f32 ny = tan * n;
    f32 nx = ny * aspect;
    Set(ny, -ny, -nx, nx, n, f, rCamMtx);
}

void FRUSTUM::Set(f32 t, f32 b, f32 l, f32 r, f32 n, f32 f,
                  const MTX34& rCamMtx) {
    MTX34 invcam;
    MTX34Inv(&invcam, &rCamMtx);
    MTX34Copy(&mCamMtx, &rCamMtx);

    VEC3 p0(0.0f, 0.0f, 0.0f);
    VEC3 p[POINT_MAX];

    f32 f_n = f / n;

    p[POINT_NEAR_TL].x = l;
    p[POINT_NEAR_TL].y = t;
    p[POINT_NEAR_TL].z = -n;

    p[POINT_NEAR_TR].x = r;
    p[POINT_NEAR_TR].y = t;
    p[POINT_NEAR_TR].z = -n;

    p[POINT_NEAR_BR].x = r;
    p[POINT_NEAR_BR].y = b;
    p[POINT_NEAR_BR].z = -n;

    p[POINT_NEAR_BL].x = l;
    p[POINT_NEAR_BL].y = b;
    p[POINT_NEAR_BL].z = -n;

    p[POINT_FAR_TL].x = f_n * l;
    p[POINT_FAR_TL].y = f_n * t;
    p[POINT_FAR_TL].z = -f;

    p[POINT_FAR_TR].x = f_n * r;
    p[POINT_FAR_TR].y = f_n * t;
    p[POINT_FAR_TR].z = -f;

    p[POINT_FAR_BR].x = f_n * r;
    p[POINT_FAR_BR].y = f_n * b;
    p[POINT_FAR_BR].z = -f;

    p[POINT_FAR_BL].x = f_n * l;
    p[POINT_FAR_BL].y = f_n * b;
    p[POINT_FAR_BL].z = -f;

    mNearZ = -n;
    mFarZ = -f;

    mPlaneL.Set(&p0, &p[POINT_NEAR_BL], &p[POINT_NEAR_TL]);
    mPlaneR.Set(&p0, &p[POINT_NEAR_TR], &p[POINT_NEAR_BR]);
    mPlaneT.Set(&p0, &p[POINT_NEAR_TL], &p[POINT_NEAR_TR]);
    mPlaneB.Set(&p0, &p[POINT_NEAR_BR], &p[POINT_NEAR_BL]);

    for (int i = 0; i < POINT_MAX; i++) {
        VEC3TransformCoord(&p[i], &invcam, &p[i]);
    }

    VEC3TransformCoord(&p0, &invcam, &p0);
    mBox.Set(p, POINT_MAX);

    // clang-format off
    mPlanes[PLANE_L].Set(&p0,               &p[POINT_NEAR_BL], &p[POINT_NEAR_TL]);
    mPlanes[PLANE_R].Set(&p0,               &p[POINT_NEAR_TR], &p[POINT_NEAR_BR]);
    mPlanes[PLANE_N].Set(&p[POINT_NEAR_TL], &p[POINT_NEAR_TR], &p[POINT_NEAR_BR]);
    mPlanes[PLANE_F].Set(&p[POINT_FAR_TL],  &p[POINT_FAR_BL],  &p[POINT_FAR_BR]);
    mPlanes[PLANE_T].Set(&p0,               &p[POINT_NEAR_TL], &p[POINT_NEAR_TR]);
    mPlanes[PLANE_B].Set(&p0,               &p[POINT_NEAR_BR], &p[POINT_NEAR_BL]);
    // clang-format on
}

bool IntersectionAABB(const AABB* pA, const AABB* pB) {
    if (pA->min.x > pB->max.x || pB->min.x > pA->max.x ||
        pA->min.y > pB->max.y || pB->min.y > pA->max.y ||
        pA->min.z > pB->max.z || pB->min.z > pA->max.z) {
        return false;
    }

    return true;
}

IntersectionResult FRUSTUM::IntersectAABB_Ex(const AABB* pBox) const {
    if (!IntersectionAABB(pBox, &mBox)) {
        return INTERSECTION_OUTSIDE;
    }

    IntersectionResult result = INTERSECTION_INSIDE;
    VEC3 p, n;

    for (int i = 0; i < PLANE_MAX; i++) {
        if (mPlanes[i].n.x >= 0.0f) {
            p.x = pBox->min.x;
            n.x = pBox->max.x;
        } else {
            p.x = pBox->max.x;
            n.x = pBox->min.x;
        }

        if (mPlanes[i].n.y >= 0.0f) {
            p.y = pBox->min.y;
            n.y = pBox->max.y;
        } else {
            p.y = pBox->max.y;
            n.y = pBox->min.y;
        }

        if (mPlanes[i].n.z >= 0.0f) {
            p.z = pBox->min.z;
            n.z = pBox->max.z;
        } else {
            p.z = pBox->max.z;
            n.z = pBox->min.z;
        }

        if (mPlanes[i].Test(p) > 0.0f) {
            return INTERSECTION_NONE;
        }

        if (mPlanes[i].Test(n) > 0.0f) {
            result = INTERSECTION_INTERSECT;
        }
    }

    return result;
}

struct SEGMENT3 {
    VEC3 a;
    VEC3 b;
};

struct CAPSULE {
    SEGMENT3 segment;
    f32 radius;
};

struct SPHERE {
    VEC3 center; // at 0x0
    f32 radius;  // at 0xC
};

f32 DistSqSegment3ToSegment3(const SEGMENT3* pSegment0,
                             const SEGMENT3* pSegment1, f32* pOut0,
                             f32* pOut1) {
    VEC3 d0 = pSegment0->b - pSegment0->a;
    VEC3 d1 = pSegment1->b - pSegment1->a;
    VEC3 diff = pSegment0->a - pSegment1->a;

    f32 a = VEC3LenSq(&d0);
    f32 b = VEC3Dot(&d0, &d1);
    f32 c = VEC3LenSq(&d1);
    f32 d = VEC3Dot(&d0, &diff);
    f32 e = VEC3Dot(&d1, &diff);
    f32 determinant = a * c - b * b;

    f32 sNumerator;
    f32 sDenominator = determinant;
    f32 tNumerator;
    f32 tDenominator = determinant;

    if (determinant < lbl_eu_806634D0) {
        sNumerator = 0.0f;
        sDenominator = 1.0f;
        tNumerator = e;
        tDenominator = c;
    } else {
        sNumerator = b * e - c * d;
        tNumerator = a * e - b * d;

        if (sNumerator < 0.0f) {
            sNumerator = 0.0f;
            tNumerator = e;
            tDenominator = c;
        } else if (sNumerator > sDenominator) {
            sNumerator = sDenominator;
            tNumerator = e + b;
            tDenominator = c;
        }
    }

    // Double-pool zero: retail loads this constant via lfd, so compare
    // against the extern double, not a 0.0f float literal.
    if (tNumerator < lbl_eu_80669E70) {
        tNumerator = lbl_eu_80669E5C;

        if (-d < lbl_eu_80669E5C) {
            sNumerator = lbl_eu_80669E5C;
        } else if (-d > a) {
            sNumerator = sDenominator;
        } else {
            sNumerator = -d;
            sDenominator = a;
        }
    } else if (tNumerator > tDenominator) {
        tNumerator = tDenominator;

        if (-d + b < lbl_eu_80669E5C) {
            sNumerator = lbl_eu_80669E5C;
        } else if (-d + b > a) {
            sNumerator = sDenominator;
        } else {
            sNumerator = -d + b;
            sDenominator = a;
        }
    }

    f32 s = FAbs(sNumerator) < lbl_eu_806634D0 ? lbl_eu_80669E5C
                                               : sNumerator / sDenominator;
    f32 t = FAbs(tNumerator) < lbl_eu_806634D0 ? lbl_eu_80669E5C
                                               : tNumerator / tDenominator;

    if (pOut0 != 0) {
        *pOut0 = s;
    }
    if (pOut1 != 0) {
        *pOut1 = t;
    }

    VEC3 closest = diff + d0 * s - d1 * t;
    return VEC3LenSq(&closest);
}

bool IntersectionCapsule(const CAPSULE* pCapsule0,
                         const CAPSULE* pCapsule1) {
    f32 radius = pCapsule0->radius + pCapsule1->radius;
    return DistSqSegment3ToSegment3(&pCapsule0->segment,
                                    &pCapsule1->segment, 0, 0) <=
           radius * radius;
}

} // namespace math
} // namespace nw4r

// Layout mirror of nw4r::math::FRUSTUM's private members. math_geometry.h is
// outside this session's writable scope, so FRUSTUM cannot gain the
// IntersectSphere member declaration here; the mangled-name free function
// below walks the mirror instead. Only the fields this function touches are
// listed (offsets must match the real class up to mFarZ at 0x74).
struct FRUSTUMLayout {
    nw4r::math::MTX34 mCamMtx; // at 0x0
    nw4r::math::PLANE mPlaneL; // at 0x30
    nw4r::math::PLANE mPlaneR; // at 0x40
    nw4r::math::PLANE mPlaneT; // at 0x50
    nw4r::math::PLANE mPlaneB; // at 0x60
    f32 mNearZ;                // at 0x70
    f32 mFarZ;                 // at 0x74
};

bool IntersectSphere__Q34nw4r4math7FRUSTUMCFPCQ34nw4r4math6SPHERE(
    const FRUSTUMLayout* self, const nw4r::math::SPHERE* pSphere) {
    const FRUSTUMLayout* pThis = self;

    nw4r::math::VEC3 p;

    // Transform the sphere center into view space. The near/far tests need z
    // first, so compute the rows lazily in the order they are used.
    p.z = pThis->mCamMtx._20 * pSphere->center.x +
          pThis->mCamMtx._21 * pSphere->center.y +
          pThis->mCamMtx._22 * pSphere->center.z + pThis->mCamMtx._23;

    if (p.z - pSphere->radius > pThis->mNearZ) {
        return false;
    }
    if (p.z + pSphere->radius < pThis->mFarZ) {
        return false;
    }

    // The side/top/bottom planes all pass through the view origin, so their
    // d terms vanish and only the n.x/n.z (sides) or n.y/n.z (top/bottom)
    // components of the plane normals are needed.
    p.x = pThis->mCamMtx._00 * pSphere->center.x +
          pThis->mCamMtx._01 * pSphere->center.y +
          pThis->mCamMtx._02 * pSphere->center.z + pThis->mCamMtx._03;

    if (p.x * pThis->mPlaneL.n.x + p.z * pThis->mPlaneL.n.z > pSphere->radius) {
        return false;
    }
    if (p.x * pThis->mPlaneR.n.x + p.z * pThis->mPlaneR.n.z > pSphere->radius) {
        return false;
    }

    p.y = pThis->mCamMtx._10 * pSphere->center.x +
          pThis->mCamMtx._11 * pSphere->center.y +
          pThis->mCamMtx._12 * pSphere->center.z + pThis->mCamMtx._13;

    if (p.y * pThis->mPlaneT.n.y + p.z * pThis->mPlaneT.n.z > pSphere->radius) {
        return false;
    }
    return !(p.y * pThis->mPlaneB.n.y + p.z * pThis->mPlaneB.n.z >
             pSphere->radius);
}

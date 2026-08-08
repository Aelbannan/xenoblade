#include "types.h"
#include "kyoshin/cf/CfCollAABBImpl.hpp"
#include "monolib/math.hpp"
#include <nw4r/math.h>

void __ct__7CDrawGXFv(void* self);
void __dt__7CDrawGXFv(void* self, int param);

// Render an AABB debug box: builds a Y-axis rotation matrix from the shape's
// angle plus its world position, then draws the min/max cube through CDrawGX.
void func_800AAE24(cf::CfCollAABBImpl* aabb) {
    float fidx = 40.743664f * aabb->mAngle;
    float sinVal = nw4r::math::SinFIdx(fidx);
    float cosVal = nw4r::math::CosFIdx(fidx);

    ml::CMat34 mat;
    mat.m[0][0] = cosVal;
    mat.m[0][1] = 0.0f;
    mat.m[0][2] = sinVal;
    mat.m[0][3] = 0.0f;
    mat.m[1][0] = 0.0f;
    mat.m[1][1] = 1.0f;
    mat.m[1][2] = 0.0f;
    mat.m[1][3] = 0.0f;
    mat.m[2][0] = -sinVal;
    mat.m[2][1] = 0.0f;
    mat.m[2][2] = cosVal;
    mat.m[2][3] = 0.0f;

    // world position via virtual func at vtable 0xAC
    mat.replaceTranslation(*aabb->GetPos());

    // line color chosen by mode flag at 0x94
    ml::CCol4 lineCol;
    if (aabb->field_94 == 1) {
        ml::CCol4 tmp(0.0f, 1.0f, 1.0f, 0.1f);
        lineCol = tmp;
    } else {
        ml::CCol4 tmp(0.0f, 0.5f, 1.0f, 0.15f);
        lineCol = tmp;
    }

    char gx[0xF0];
    __ct__7CDrawGXFv(gx);
    func_8049034C(lbl_eu_80663E14, gx, 0);

    ml::CCol4 col(0.0f, 1.0f, 1.0f, 0.3f);
    setCol__7CDrawGXFRCQ22ml5CCol4(gx, &col);
    setMatrix__7CDrawGXFRCQ22ml6CMat34(gx, &mat);
    setCol__7CDrawGXFRCQ22ml5CCol4(gx, &lineCol);
    renderCube__7CDrawGXFRCQ22ml5CVec3RCQ22ml5CVec3(gx, &aabb->mMin, &aabb->mMax);
    __dt__7CDrawGXFv(gx, -1);
}

// Render AABB collision shape into a collision query context.
// r4 is the AABB collision shape; r3 (unused) and r5/r6 pass through to func_800A5FE8.
extern "C" void func_800AAFF4(void* r3, cf::CfCollAABBImpl* aabb, void* query, void* result) {
    func_800A5FE8(query, &aabb->mMin, &aabb->mMax, &aabb->mCenter, result);
}
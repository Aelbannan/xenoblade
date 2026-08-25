#include "types.h"
#include "kyoshin/cf/CfCollAABBImpl.hpp"
#include "monolib/math.hpp"
#include "monolib/core/CDrawGX.hpp"
#include <nw4r/math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Render an AABB debug box: builds a Y-axis rotation matrix from the shape's
// angle plus its world position, then draws the min/max cube through CDrawGX.
// Unused opaque context parameter is left unnamed (retail mangles this
// function's parameter types into its symbol name, so the type must stay).
void func_800AAE24(void*, cf::CfCollAABBImpl* aabb) {
    // Retail keeps the raw angle live in f30 across both trig calls.
    float angle = aabb->mAngle;
    float sinVal = nw4r::math::SinFIdx(lbl_eu_80666928 * angle);
    float cosVal = nw4r::math::CosFIdx(lbl_eu_80666928 * angle);

    ml::CMat34 mat;
    mat.m[0][0] = cosVal;
    mat.m[0][1] = lbl_eu_80666910;
    mat.m[0][2] = sinVal;
    mat.m[0][3] = lbl_eu_80666910;
    mat.m[1][0] = lbl_eu_80666910;
    mat.m[1][1] = lbl_eu_80666914;
    mat.m[1][2] = lbl_eu_80666910;
    mat.m[1][3] = lbl_eu_80666910;
    mat.m[2][0] = -sinVal;
    mat.m[2][1] = lbl_eu_80666910;
    mat.m[2][2] = cosVal;
    mat.m[2][3] = lbl_eu_80666910;

    // world position via virtual func at vtable 0xAC
    mat.replaceTranslation(*aabb->GetPos());

    // line color chosen by mode flag at 0x94
    ml::CCol4 lineCol;
    if (aabb->field_94 == 1) {
        ml::CCol4 tmp;
        tmp.set(lbl_eu_80666910, lbl_eu_80666914, lbl_eu_80666914, lbl_eu_8066692C);
        lineCol = tmp;
    } else {
        ml::CCol4 tmp;
        tmp.set(lbl_eu_80666910, lbl_eu_80666930, lbl_eu_80666914, lbl_eu_80666934);
        lineCol = tmp;
    }

    CDrawGX gx;
    func_8049034C(lbl_eu_80663E14, &gx, 0);

    ml::CCol4 col(lbl_eu_80666910, lbl_eu_80666914, lbl_eu_80666914, lbl_eu_80666918);
    gx.setCol(col);
    gx.setMatrix(mat);
    gx.setCol(lineCol);
    gx.renderCube(aabb->mMin, aabb->mMax);
}

// Render AABB collision shape into a collision query context.
// r4 is the AABB collision shape; r3 (unused) and r5/r6 pass through to func_800A5FE8.
extern "C" void func_800AAFF4(void* r3, cf::CfCollAABBImpl* aabb, void* query, void* result) {
    func_800A5FE8(query, &aabb->mMin, &aabb->mMax, &aabb->mCenter, result);
}
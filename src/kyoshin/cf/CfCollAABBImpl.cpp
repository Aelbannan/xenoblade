#include "types.h"
#include "kyoshin/cf/CfCollAABBImpl.hpp"
#include "kyoshin/cf/object/CfObjectColl.hpp"
#include "monolib/math.hpp"
#include "monolib/core/CDrawGX.hpp"
#include <nw4r/math.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Render an AABB debug box: builds a Y-axis rotation matrix from the shape's
// angle plus its world position, then draws the min/max cube through CDrawGX.
// The AABB data (angle at 0x14C, flag at 0x94, vectors at 0xD8/0xE4/0xF0) lives
// in the CfObjectColl union; the CfCollAABBImpl struct is a view onto that
// memory. Position is fetched via cf::CfObject::GetPosition (virtual at +0xAC,
// lbl_eu_80528600) on the CfObjectColl.

extern "C" void func_800AAE24(void* /*ctx*/, cf::CfObjectColl* coll) {
    // View coll memory as AABB data.
    cf::CfCollAABBImpl* aabb = reinterpret_cast<cf::CfCollAABBImpl*>(coll);

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

    // world position via CfObject virtual at +0xAC
    mat.replaceTranslation(*coll->CfObject_UnkVirtualFunc23());

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
// r4 is the CfObjectColl carrying AABB data; r3 (unused, the CfCollImpl this)
// is ignored. Tail-calls func_800A5FE8.
extern "C" void func_800AAFF4(void* /*ctx*/, cf::CfObjectColl* coll, void* query, void* result) {
    cf::CfCollAABBImpl* aabb = reinterpret_cast<cf::CfCollAABBImpl*>(coll);
    func_800A5FE8(query, &aabb->mMin, &aabb->mMax, &aabb->mCenter, result);
}

#include "types.h"
#include "monolib/core/CDrawGX.hpp"
#include "kyoshin/cf/CfCollCircleImpl.hpp"
#include "kyoshin/cf/object/CfObjectColl.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Shared debug color constant (sdata2). lbl_eu_80666910 (0.0f) and
// lbl_eu_80666914 (1.0f) come from kyoshin/cf/object/CfObjectColl.hpp;
// only 0.3f is declared here. Referencing the named globals keeps
// the reloc names identical to retail instead of literal pool constants.
// 0.3f is lbl_eu_80666918.
extern float lbl_eu_80666918;

// func_800AABD4: debug draw for the circle collision shape.
// Builds a stack CDrawGX, sets a cyan-ish color, then renders a filled
// circle at the shape's fetched position with the shape's radius.
// The shape is a CfObject-family coll object (radius at 0xB8); position
// comes from the real cf::CfObject virtual at +0xAC
// (CfObject_UnkVirtualFunc23, cf. CfCollSphereImpl).
void func_800AABD4(void* context, cf::CfObjectColl* shape){
    CDrawGX gx;
    func_8049034C(lbl_eu_80663E14, &gx, 0);

    ml::CCol4 col;
    col.r = lbl_eu_80666910;
    col.g = lbl_eu_80666914;
    col.b = lbl_eu_80666914;
    col.a = lbl_eu_80666918;
    gx.setCol(col);

    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    gx.renderCircle(*pos, 0x24, static_cast<float>(uval));
}

// func_800AAC98: debug draw helper that forwards position + radius to
// func_800A50AC after going through the shape's virtual getter.
void func_800AAC98(void* context, cf::CfObjectColl* shape, void* c, void* d){
    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    func_800A50AC(c, pos, static_cast<float>(uval), d);
}

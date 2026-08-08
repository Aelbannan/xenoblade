#include "types.h"
#include "monolib/core/CDrawGX.hpp"
#include "kyoshin/cf/CfCollCircleImpl.hpp"

// Shared debug color constants (sdata2). Referencing the named globals keeps
// the reloc names identical to retail instead of literal pool constants.
extern float lbl_eu_80666910; // 0.0f
// 1.0f is lbl_eu_80666914; 0.3f is lbl_eu_80666918.
extern float lbl_eu_80666914;
extern float lbl_eu_80666918;

// func_800AABD4: debug draw for the circle collision shape.
// Builds a stack CDrawGX, sets a cyan-ish color, then renders a filled
// circle at the shape's fetched position with the shape's radius.
void func_800AABD4(void* context, cf::CfCollCircleImpl* shape){
    CDrawGX gx;
    func_8049034C(lbl_eu_80663E14, &gx, 0);

    ml::CCol4 col;
    col.r = lbl_eu_80666910;
    col.g = lbl_eu_80666914;
    col.b = lbl_eu_80666914;
    col.a = lbl_eu_80666918;
    gx.setCol(col);

    u32 uval = static_cast<u32>(shape->field_B8);
    void* pos = reinterpret_cast<cf::CfCollCircleVTableIf*>(shape)->_v0AC();
    gx.renderCircle(*reinterpret_cast<ml::CVec3*>(pos), 0x24, static_cast<float>(uval));
}

// func_800AAC98: debug draw helper that forwards position + radius to
// func_800A50AC after going through the shape's virtual getter.
void func_800AAC98(void* context, cf::CfCollCircleImpl* shape, void* c, void* d){
    u32 uval = static_cast<u32>(shape->field_B8);
    void* mgr = reinterpret_cast<cf::CfCollCircleVTableIf*>(shape)->_v0AC();
    func_800A50AC(c, mgr, static_cast<float>(uval), d);
}
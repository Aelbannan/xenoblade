#pragma once

#include <types.h>
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (ml::epsilon)

// .sdata pointer constants referenced by UVAnm_RegisterPairs (node/material name
// prefixes for the UV-animation registration scan).
extern const char* lbl_eu_80663C70;
extern u32 lbl_eu_80663C74;

// UV-animation helper object. Binds a set of nodes/materials of the owner
// model into parallel id arrays so an animated texture SRT can be applied to
// them every frame.
class CMdlAnmUV {
public:
    CMdlAnmUV();
    virtual ~CMdlAnmUV();

    /* 0x04 */ void* field_04;   // owner model object (CScnItemModelNw4r*)
    /* 0x08 */ u16 field_08[8];  // bound node ids
    /* 0x18 */ u16 field_18[8];  // bound material ids
    /* 0x28 */ u16 field_28[8];  // stage values (-1 = unset)
    /* 0x38 */ u32 field_38;     // number of registered node/material pairs
    /* 0x3C */ u8 field_3c;      // registered-flag
    /* 0x3D */ u8 field_3d;      // name-based mode flag ("pc06"/"np705011")
};
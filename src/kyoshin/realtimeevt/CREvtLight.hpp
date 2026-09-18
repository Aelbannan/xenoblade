#pragma once

#include <types.h>
#include <nw4r/math.h>
#include <revolution/MTX.h>

// CREvtLight - realtime event light
// Inherits from cf::CREvtObj (vtable at 0x80532320, size 0x14)
// Own vtable at 0x80533D90 (size 0x28 = 10 entries)
// Total size: 0x50
struct CREvtLight {
    // Inherited from cf::CREvtObj (size 0x14)
    /* 0x00 */ u32* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 ptmf[3];

    // CREvtLight fields
    /* 0x14 */ u32 field_14;  // initialized to 0
    /* 0x18 */ u32 field_18;  // constructor arg
    /* 0x1C */ u32 field_1C;  // set by CREvtLight_SetResource
    /* 0x20 */ void* field_20; // resource handle (nullptr initially)

    // Position/orientation of the light (used by CREvtLight_DistToTarget / CREvtLight_AngleState)
    /* 0x24 */ u8 pad_24[0x3C - 0x24];
    /* 0x3C */ nw4r::math::VEC3 mPos;
    /* 0x48 */ u8 pad_48[0x4C - 0x48];
    /* 0x4C */ f32 mAngle;

    nw4r::math::VEC3* pos() { return &mPos; }
};


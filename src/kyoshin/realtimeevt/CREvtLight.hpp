#pragma once

#include <types.h>
#include <nw4r/math.h>
#include <revolution/MTX.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

namespace cf {

// CREvtLight - realtime event light (RTTI name "cf::CREvtLight").
// Derives from cf::CREvtObj (type 2); vtable at 0x80533D90.
// Total size: 0x24 (allocated from the CREvtMem arena).
class CREvtLight : public CREvtObj {
public:
    CREvtLight(u32 arg);
    virtual ~CREvtLight();
    virtual void vfunc_04();  // empty override of the pure slot
    virtual void vfunc_1C();  // empty override

    /* 0x14 */ u32 field_14;  // initialized to 0
    /* 0x18 */ u32 field_18;  // constructor arg
    /* 0x1C */ u32 field_1C;  // set by CREvtLight_SetResource
    /* 0x20 */ void* field_20; // resource handle (nullptr initially)
};

} // namespace cf

using cf::CREvtLight;

// Position/orientation view used by CREvtLight_DistToTarget / CREvtLight_AngleState.
// Callers pass an AI-action block (+0x3E9C of the owner), not a CREvtLight.
struct CREvtLightPose {
    /* 0x00 */ u8 pad_00[0x3C];
    /* 0x3C */ nw4r::math::VEC3 mPos;
    /* 0x48 */ u8 pad_48[0x4C - 0x48];
    /* 0x4C */ f32 mAngle;

    nw4r::math::VEC3* pos() { return &mPos; }
};

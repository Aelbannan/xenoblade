#pragma once

#include <types.h>

// IScnPolyShadow - polymorphic shadow interface held by CScnItemModel
// (+0x7EC). RTTI "IScnPolyShadow" (no bases).
//
// CScnMaruShadowNw4r is the round ("maru") shadow implementation; its vtable
// (0x8056E598, six slots, no destructor slot) lives in
// monolib/src/scn/CScnMaruShadowNw4r.cpp. Only the members used through
// dynamic_cast by cf::CREvtModel::setupShadow are named here.
//
// NOTE: monolib/src/scn/CScnMaruShadowNw4r.hpp still carries a flattened
// (non-polymorphic) view of the same class for its own TU; do not include
// both headers in one translation unit.
class IScnPolyShadow {
public:
    virtual void pickScale() = 0;     // vtable +0x08 (MaruShadowPickScale)
    virtual bool isEnabled() = 0;     // vtable +0x0C (MaruShadowStubTrue)
    virtual void update() = 0;        // vtable +0x10 (MaruShadowUpdate)
    virtual void setFlag40() = 0;     // vtable +0x14 (MaruShadowSetFlag40Store)
    virtual void setFlag100() = 0;    // vtable +0x18 (MaruShadowSetFlag100)
    virtual void setFlag80() = 0;     // vtable +0x1C (MaruShadowSetFlag80Store)
};

class CScnMaruShadowNw4r : public IScnPolyShadow {
public:
    virtual void pickScale();
    virtual bool isEnabled();
    virtual void update();
    virtual void setFlag40();
    virtual void setFlag100();
    virtual void setFlag80();

    /* 0x04 */ u8 mColorR;
    /* 0x05 */ u8 mColorG;
    /* 0x06 */ u8 mColorB;
    /* 0x07 */ u8 mColorA;
    /* 0x08 */ f32 mRadiusScale;
    /* 0x0C */ u8 _pad0C[0x2A - 0x0C];
    /* 0x2A */ u8 mMono;
};

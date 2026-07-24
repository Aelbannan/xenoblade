#pragma once

#include <types.h>
#include "monolib/math.hpp"
#include <nw4r/g3d/g3d_light.h>

class CLight{
public:
    CLight();
    virtual ~CLight(){}

    ml::CVec3 unk4;
    ml::CVec3 unk10;
    ml::CVec3 unk1C;
    float unk28;
    nw4r::g3d::LightObj* mpLightObj;
    u32 mFlags;
    u32 unk34;
    float unk38;
    float unk3C;
};

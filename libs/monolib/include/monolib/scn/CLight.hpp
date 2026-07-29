#pragma once

#include <types.h>
#include "monolib/math.hpp"
#include <nw4r/g3d/g3d_light.h>
#include <nw4r/math/math_types.h>

class CLight{
public:
    CLight();
    virtual ~CLight(){}

    void func_804C0484(const float* dir);
    void func_804C07F0(const float* color);
    void func_804C09E8(GXLightObj* outLight, const Mtx matrix);

    u32 mFlags;                         // 0x00
    ml::CVec3 unk4;                     // 0x04 - position
    ml::CVec3 unk10;                    // 0x10 - color RGB
    ml::CVec3 unk1C;                    // 0x1C - alpha + direction.xy
    float unk28;                        // 0x28 - direction.z
    nw4r::g3d::LightObj* mpLightObj;    // 0x2C
    u32 unk30;                          // 0x30 - padding
    u32 unk34;                          // 0x34 - light type
    float unk38;                        // 0x38 - intensity
    float unk3C;                        // 0x3C - attenuation end
};

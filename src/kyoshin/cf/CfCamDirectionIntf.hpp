#pragma once

#include <types.h>
#include "monolib/math.hpp"

namespace cf {

class CfCamEvent {
public:
    u8 unk0[0x28];
    ml::CVec3 unk28;
    u8 unk34[0x25C - 0x34];
    ml::CVec3 unk25C;
    ml::CVec3 mLookat;
    ml::CVec3 unk274;
    u8 unk280[0x284 - 0x280];
    f32 unk284;
};

} // namespace cf

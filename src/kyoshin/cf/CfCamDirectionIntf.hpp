#pragma once

#include <types.h>
#include "monolib/math.hpp"

namespace cf {

// Forward declarations for camera interface types.
// These are vtable-only interfaces whose methods are extern "C" free functions
// (C-linkage names for retail symbol matching).
struct CfCamDirectionIntf;
struct CfCamLookatIntf;
struct CfCamTargetIntf;

// Camera event state - used as a shared state block by camera interface vtable functions
// (CfCamDirectionIntf, CfCamLookatIntf, CfCamTargetIntf). Functions receive a
// typed interface pointer (unused self, vtable compat) and a CfCamEvent* for state.
class CfCamEvent {
public:
    u8 unk0[0x28];               // 0x00: vtable pointers + unknown
    ml::CVec3 unk28;              // 0x28: position copy (set from pos argument)
    u8 unk34[0x25C - 0x34];       // 0x34: gap
    ml::CVec3 unk25C;             // 0x25C: direction vector (from setDir/last lookat result)
    ml::CVec3 mLookat;            // 0x268: current lookat target point
    ml::CVec3 unk274;             // 0x274: camera base position / orbit pivot
    u8 unk280[0x284 - 0x280];     // 0x280: padding
    f32 unk284;                   // 0x284: camera speed/factor parameter
};

} // namespace cf

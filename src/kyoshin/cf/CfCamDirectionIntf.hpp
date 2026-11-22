#pragma once

#include <types.h>
#include "monolib/math.hpp"
#include "kyoshin/cf/CfCamEvent.hpp"

namespace cf {

// Forward declarations for camera interface types.
// These are vtable-only interfaces whose methods are extern "C" free functions
// (C-linkage names for retail symbol matching).
struct CfCamDirectionIntf;
struct CfCamLookatIntf;
struct CfCamTargetIntf;

} // namespace cf

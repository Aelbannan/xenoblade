#pragma once

#include <types.h>
#include <nw4r/g3d/g3d_scnroot.h>
#include "libs/monolib/src/scn/CScnRootNw4r.hpp"

// Host for __ct__CScnItemLightNw4r (scene object), not the allocated light.
struct CScnLightItemHost {
    u8 _00[0x60];
    u8* mPool;                   // +0x60 scene-item pool
    u8 _64[0x8C - 0x64];
    CScnRootNw4r* mRoot;          // +0x8C scene root wrapper
};

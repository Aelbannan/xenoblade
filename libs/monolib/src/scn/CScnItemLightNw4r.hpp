#pragma once

#include <types.h>
#include <nw4r/g3d/g3d_scnroot.h>

// Object at host +0x8C; +0x10 is copied onto the new light's +0x1220.
struct CScnLightRootView {
    u8 _00[0x10];
    nw4r::g3d::ScnRoot* mScnRoot;  // +0x10
};

// Host for __ct__CScnItemLightNw4r (scene object), not the allocated light.
struct CScnLightItemHost {
    u8 _00[0x60];
    u8* mPool;                   // +0x60 scene-item pool
    u8 _64[0x8C - 0x64];
    CScnLightRootView* mRoot;    // +0x8C
};

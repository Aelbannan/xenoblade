#pragma once

#include <types.h>

// Forward declarations
class CScn;

/**
 * Tutorial list menu widget.
 *
 * MI layout: CProcess at 0x00, IScnRender (secondary base) at 0x58.
 * The thunk functions func_802ACBCC / func_802ACBD4 adjust this by -0x58
 * when entering through the IScnRender vtable.
 *
 * Recovered member offsets from constructor:
 *   0x00-0x57: CProcess base + pre-IScnRender fields
 *   0x58:      IScnRender subobject (vtable ptr)
 *   0x5C:      CScn* mScene
 *   0x60:      CBgTex (size 0x20)
 *   0x80:      CTitleAHelp (size 0x38)
 *   0xB8:      CTutorialList (size 0x288)
 *   0x340:     CTutorial (size 0x54)
 *   0x394:     f32
 *   0x398:     u8
 *   0x39A:     u16
 *   0x39C:     total
 */
class CMenuTutorialList {
public:
    // 0x00-0x57: CProcess base + pre-IScnRender members (opaque)
    u8 unk00[0x5C];

    // 0x5C: owning scene for render callbacks
    CScn* mScene;

    // Member sub-objects (opaque until their types get headers)
    u8 mBgTex[0x20];         // 0x60 -- CBgTex
    u8 mTitleAHelp[0x38];    // 0x80 -- CTitleAHelp
    u8 mTutorialList[0x288]; // 0xB8 -- CTutorialList
    u8 mTutorial[0x54];      // 0x340 -- CTutorial

    f32 mTimer;              // 0x394
    u8 mFlag;                // 0x398
    u16 mChoice;             // 0x39A
};
// Total size: 0x39C

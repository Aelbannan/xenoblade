#pragma once

#include <types.h>

namespace cf {

    // Base class for move-control objects (CCtrlMovePC, CCtrlMoveEne, CCtrlMoveNpc, etc.)
    // Manages position, velocity, and movement state for controllable entities.
    //
    // Layout (from constructor at 0x800892DC):
    //   0x00: parent vtable / first field
    //   0x0C: mPosition (CVec3, 12 bytes)
    //   0x18: mVelocity (CVec3, 12 bytes)
    //   0x24: padding/unknown (12 bytes)
    //   0x30: mpSomePtr (initialized to NULL)
    //   0x34: mpDataPtr (set from constructor param)
    //   0x38: mFloatParam1 (from lbl_80666598)
    //   0x3C: mFloatParam2 (from lbl_8066659C)
    //   0x40: mFlagsU16_1
    //   0x42: mFlagsU16_2
    //   0x44: mByte1
    //   0x45: mByte2
    //   0x48: secondary vtable (interface or adjusted base)
    // Minimum size: 0x4C
    class CCtrlMoveBase {
    public:
        CCtrlMoveBase();
        virtual ~CCtrlMoveBase();

        // Empty virtual stub — no-op override for interface slot.
        virtual void func_80089628();

        // Empty virtual stub — no-op override for interface slot.
        virtual void func_8008A0C4();
    };

} // namespace cf

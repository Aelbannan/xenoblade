#pragma once

#include <types.h>

namespace cf {

// Resource reload implementation.
// Manages reloading of resources with a timer and state tracking.
// Inherits from CfResImpl (base class defines fields 0x00-0x13).
struct CfResReloadImpl {
    // 0x00: parent/reference pointer (set from ctor arg)
    // 0x04: timer or delay float
    // 0x08: u16 type/category
    // 0x0A: u16 state/param (-1 = invalid)
    // 0x0C: u16 counter
    // 0x0E: u16 state/param (-1 = invalid)
    // 0x10: vtable pointer
    // 0x14: u8[8] work buffer (zeroed)
    // 0x1C: u16 reload count/state
    // 0x1E: u8 flags
    // 0x1F: u8 flags
    u8 _00[0x20];

    // vtable slot 6: returns a resource type identifier (12 for reload impl)
    int func_8016CF1C();

    // vtable slot 23: returns a resource sub-type identifier (1 for reload impl)
    int func_8016DECC();
};

} // namespace cf

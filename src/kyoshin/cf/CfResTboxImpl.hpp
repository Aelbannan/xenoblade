#pragma once

#include <types.h>

namespace cf {

// Resource textbox implementation.
// Inherits from CfResObjImpl (base class handles offsets 0x00-0x0F).
struct CfResTboxImpl {
    // 0x00: pointer to associated object (set from ctor arg)
    // 0x08: u16 open/active counter
    // 0x10: vtable pointer (set to lbl_eu_80535204)
    u8 _00[0x14];
};

} // namespace cf

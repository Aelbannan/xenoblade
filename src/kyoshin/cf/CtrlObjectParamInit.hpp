#pragma once

#include <types.h>

namespace cf {

    // -- Opaque struct for CtrlObjectParam_InitEquipSlots -------------------------------------
    //   6 s16s (set to -1) + 192-byte zeroed area.
    struct CtrlObjectParamInit {
        s16 field_00;
        s16 field_02;
        s16 field_04;
        s16 field_06;
        s16 field_08;
        s16 field_0A;
        u8  blob[192];
    };

} // namespace cf

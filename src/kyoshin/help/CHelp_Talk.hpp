#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* func_8016FE34(void* source);
extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void* func_800BF324(void* objParam);

namespace cf {

// CHelp_Talk inherits from CHelpSwitch (which adds mFlag at +0xC).
// Additional fields at +0x10..+0x16.
class CHelp_Talk : public CHelpSwitch {
public:
    void func_802B86BC();
    bool func_802B86F0();

    u32 field_0x10;  // 0x10 - compared against a u16 from another object
    u8 field_0x14;   // 0x14 - flag checked before func_8009CF8C call
    u8 field_0x15;   // 0x15 - flag checked before final block
    u8 field_0x16;   // 0x16 - main state/result byte, read and written throughout
};

} // namespace cf

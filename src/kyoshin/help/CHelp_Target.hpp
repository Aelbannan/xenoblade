#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0x10
class CHelp_Target : public CHelp {
public:
    bool CHelp_UnkVirtualFunc3();

    s32 unkC; // 0xC
};
} // namespace cf

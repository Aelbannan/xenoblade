#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0x10
class CHelp_ArtsSet : public CHelp {
public:
    bool func_802B7D9C();

    s32 unkC; // 0xC
};
} // namespace cf

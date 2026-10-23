#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_GameOver : public CHelp {
public:
    bool isAvailable();

    s32 unkC; // 0xC
    u8 mFlag10; // 0x10
};
} // namespace cf

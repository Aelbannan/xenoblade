#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_GameOver : public CHelp {
public:
    bool func_802B80F4();

    s32 unkC; // 0xC
    u8 mFlag10; // 0x10
};
} // namespace cf

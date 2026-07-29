#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0xF
class CHelp_LearnArts : public CHelp {
public:
    bool func_802B8398();
    void func_802B84E0();

    s16 mWpnTypeCount; // 0xC
    u8 mLearnArtsFlag; // 0xE
};
} // namespace cf

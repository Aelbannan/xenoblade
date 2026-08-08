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

// Character-data object returned by func_8009EC9C. Fields beyond +0xE8
// are the learn-arts flag grid read by func_801F9268.
struct CHelpLearnArtsCharData {
    u8 pad[0xC];            // 0x0..0xB
    u16 mWpnType;           // +0xC
    u8 pad2[0xE8 - 0xE];    // 0xE..0xE7
    u8 mArts[1];            // +0xE8
};

} // namespace cf

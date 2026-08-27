#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_Sp : public CHelp {
public:
    bool isSpThresholdMet();

    f32 mThreshold; // 0xC
};
} // namespace cf

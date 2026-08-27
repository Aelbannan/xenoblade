#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_Pg : public CHelp {
public:
    bool isThresholdMet();

    f32 mThreshold; // 0xC
};
} // namespace cf

#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_Sp : public CHelp {
public:
    bool func_802B8654();

    f32 mThreshold; // 0xC
};
} // namespace cf

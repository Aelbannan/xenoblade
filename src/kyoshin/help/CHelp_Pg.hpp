#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_Pg : public CHelp {
public:
    bool func_802B85A4();

    f32 mThreshold; // 0xC
};
} // namespace cf

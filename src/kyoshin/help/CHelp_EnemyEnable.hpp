#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
// size: 0xC (no extra members beyond CHelp).
class CHelp_EnemyEnable : public CHelp {
public:
    bool func_802B8028();
};
}

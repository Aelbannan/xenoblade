#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_OpenPartyMenu : public CHelp {
public:
    bool isPartyMenuReady();
};
} // namespace cf

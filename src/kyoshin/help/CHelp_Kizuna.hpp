#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"


namespace cf {
class __declspec(novtable) CHelp_Kizuna : public CHelp {
public:
    u8 isKizunaFlag();
};
} // namespace cf

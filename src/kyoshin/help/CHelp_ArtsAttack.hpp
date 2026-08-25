#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {

class __declspec(novtable) CHelp_ArtsAttack : public CHelpSwitch {
public:
    u32 func_802B7D00();
};

} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

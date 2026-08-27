#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_ClosePartyMenu : public CHelpSwitch {
public:
    void checkHelpCondition();
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 func_80293C10();
extern "C" u32 func_80192BD0();

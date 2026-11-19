#pragma once

#include "kyoshin/CSystemWindowApi.hpp"
#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_CloseSysMenu : public CHelpSwitch {
public:
    void updateCloseSysHelp();
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

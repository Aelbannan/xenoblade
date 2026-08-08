#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_CloseSysMenu : public CHelp {
public:
    void func_802B7E7C();
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" UNKWORD func_80124B78();

#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_CloseItemMenu : public CHelp {
public:
    void func_802B7E34();
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// func_801B481C is a retail global helper ("any block condition active"),
// defined in kyoshin/menu/CMenuGetItemMulti.cpp. Its retail symbol is
// unmangled, so a C-linkage declaration is real ABI here, not scaffolding:
// removing the extern "C" would mangle the call and fail to link.
extern "C" u32 func_801B481C();

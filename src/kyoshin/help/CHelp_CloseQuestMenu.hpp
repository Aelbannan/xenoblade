#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_CloseQuestMenu : public CHelp {
public:
    void func_802B8604();
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" UNKWORD func_80122450();

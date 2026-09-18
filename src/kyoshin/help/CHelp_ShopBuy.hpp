#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void ShopBuy_HasInstance();

namespace cf {
class __declspec(novtable) CHelp_ShopBuy : public CHelp {
public:
    void checkHelpCondition();
};
} // namespace cf

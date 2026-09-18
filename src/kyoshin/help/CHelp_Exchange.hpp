#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void ItemEx_HasInstance();

namespace cf {
class __declspec(novtable) CHelp_Exchange : public CHelp {
public:
    void checkExchangeState();
};
} // namespace cf

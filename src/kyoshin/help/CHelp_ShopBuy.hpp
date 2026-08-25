#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void func_8018C180();

namespace cf {
class __declspec(novtable) CHelp_ShopBuy : public CHelp {
public:
    void func_802B864C();
};
} // namespace cf

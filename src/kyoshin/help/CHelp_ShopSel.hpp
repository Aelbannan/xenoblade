#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void func_8018B398();

namespace cf {
class __declspec(novtable) CHelp_ShopSel : public CHelp {
public:
    void checkShopSel();
};
} // namespace cf

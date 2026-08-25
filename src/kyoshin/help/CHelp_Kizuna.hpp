#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
struct CHelpFlagBag;
extern "C" CHelpFlagBag* lbl_eu_80664A10;

namespace cf {
class __declspec(novtable) CHelp_Kizuna : public CHelp {
public:
    u8 func_802B818C();
};
} // namespace cf

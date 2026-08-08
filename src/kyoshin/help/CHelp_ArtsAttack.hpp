#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {

// size: 0xC (no extra members beyond CHelp).
class CHelp_ArtsAttack : public CHelp {
public:
    bool func_802B7D00();
};

} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);
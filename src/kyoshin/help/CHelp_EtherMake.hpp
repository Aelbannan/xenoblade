#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class CHelp_EtherMake : public CHelp {
public:
    bool func_802B80A4();
    void func_802B80EC();
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" UNKWORD func_80212480();
extern "C" UNKWORD func_80212490();

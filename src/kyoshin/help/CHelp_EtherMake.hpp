#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_EtherMake : public CHelp {
public:
    bool isEtherReady();
    void notifyEther();
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" UNKWORD func_80212480();
extern "C" UNKWORD func_80212490();

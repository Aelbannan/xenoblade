#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {
class __declspec(novtable) CHelp_CkKizuna : public CHelpSwitch {
public:
    void checkKizunaTalkClosed(); // was func_802B7DEC (retail 0x802BA85C)
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" UNKWORD func_801BCF38();

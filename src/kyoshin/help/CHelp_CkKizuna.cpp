#include "kyoshin/help/CHelp_CkKizuna.hpp"

namespace cf {

void CHelp_CkKizuna::func_802B7DEC() {
    // Inline the comparison so MWCC keeps func_801BCF38's return in r3 for the
    // cntlzw/srwi boolean conversion (matching retail's scheduling exactly).
    this->func_802B7CBC(func_801BCF38() == 0);
}

} // namespace cf

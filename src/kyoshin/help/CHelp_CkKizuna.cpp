#include "kyoshin/help/CHelp_CkKizuna.hpp"

namespace cf {

// Sets the CHelpSwitch flag while the kizuna-talk menu (CMenuKizunaTalk
// singleton via func_801BCF38) is absent — i.e. closes this help once the
// talk ends.
void CHelp_CkKizuna::checkKizunaTalkClosed() {
    // Inline the comparison so MWCC keeps func_801BCF38's return in r3 for the
    // cntlzw/srwi boolean conversion (matching retail's scheduling exactly).
    this->func_802B7CBC(func_801BCF38() == 0);
}

} // namespace cf

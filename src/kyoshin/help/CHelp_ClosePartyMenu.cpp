// Decompiled TU for kyoshin/help/CHelp_ClosePartyMenu.

#include "kyoshin/help/CHelp_ClosePartyMenu.hpp"
#include "kyoshin/help/CHelp_OpenPartyMenu.hpp"

namespace cf {

// us-802bafa4
// Calls vtbl slot 7 with 1 (true) only if both func_80293C10 and func_80192BD0
// return zero; otherwise 0.
void CHelp_ClosePartyMenu::func_802B8534() {
    u32 flag = (!func_80293C10() && !func_80192BD0()) ? 1 : 0;
    this->func_802B7CBC(flag);
}

} // namespace cf

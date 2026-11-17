// Decompiled TU for kyoshin/help/CHelp_ClosePartyMenu.

#include "kyoshin/help/CHelp_ClosePartyMenu.hpp"

namespace cf {

// us-802baf5c
// Returns true if either PTNotice or the party-menu state is active.
bool CHelp_ClosePartyMenu::isPartyMenuReady() {
    return PTNotice_IsActive_3C10() || menuPTStateIsActive();
}

// us-802bafa4
// Calls vtbl slot 7 with 1 (true) only if both PTNotice_IsActive_3C10 and menuPTStateIsActive
// return zero; otherwise 0.
void CHelp_ClosePartyMenu::checkHelpCondition() {
    u32 flag = (!PTNotice_IsActive_3C10() && !menuPTStateIsActive()) ? 1 : 0;
    this->func_802B7CBC(flag);
}

} // namespace cf

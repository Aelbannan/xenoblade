#include "kyoshin/help/CHelp_CloseItemMenu.hpp"

namespace cf {

// Arms the close-item-menu help switch (CHelpSwitch::func_802B7CBC) with
// "item-get menu not open": func_801B481C reports the live get-item-multi
// object, so the switch is set while no block condition is active.
void CHelp_CloseItemMenu::checkCloseItemMenu() {
    this->func_802B7CBC(func_801B481C() == 0);
}

} // namespace cf

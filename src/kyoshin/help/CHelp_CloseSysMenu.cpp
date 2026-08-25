#include "kyoshin/help/CHelp_CloseSysMenu.hpp"

namespace cf {

void CHelp_CloseSysMenu::func_802B7E7C() {
    this->func_802B7CBC(func_80124B78() == 0);
}

} // namespace cf
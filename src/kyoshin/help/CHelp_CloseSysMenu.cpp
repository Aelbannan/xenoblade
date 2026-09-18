#include "kyoshin/help/CHelp_CloseSysMenu.hpp"

namespace cf {

void CHelp_CloseSysMenu::updateCloseSysHelp() {
    this->func_802B7CBC(SysWinGetSingleton() == 0);
}

} // namespace cf
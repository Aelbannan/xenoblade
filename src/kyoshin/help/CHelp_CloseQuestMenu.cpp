#include "kyoshin/help/CHelp_CloseQuestMenu.hpp"

namespace cf {

void CHelp_CloseQuestMenu::checkCloseQuestMenu() {
    this->func_802B7CBC(hasQuestWindow() == 0);
}

} // namespace cf
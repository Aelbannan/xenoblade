#include "kyoshin/help/CHelp_GameOver.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CHelpManager.hpp"

namespace cf {
bool CHelp_GameOver::isAvailable() {
    if (isHelpPopupReady(lbl_eu_80664A10) == 0) {
        return false;
    }
    if (unkC != 0 && CfGameManager::getQueuedFileEventCount() < unkC) {
        return false;
    }
    if (mFlag10 != 0 && lbl_eu_80664A10->mField18 == 0) {
        return false;
    }
    return true;
}
} // namespace cf

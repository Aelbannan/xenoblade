#include "kyoshin/help/CHelp_ArtsSet.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

namespace cf {
bool CHelp_ArtsSet::isHelpAvailable() {
    if (unkC != 0 && CfGameManager::getQueuedFileEventCount() < unkC) {
        return false;
    }
    return CMenuArtsSet_isCreated();
}
} // namespace cf

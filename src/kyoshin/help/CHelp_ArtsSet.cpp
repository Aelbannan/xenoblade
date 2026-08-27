#include "kyoshin/help/CHelp_ArtsSet.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

namespace cf {
bool CHelp_ArtsSet::isHelpAvailable() {
    if (unkC != 0 && CfGameManager::func_800822F4() < unkC) {
        return false;
    }
    return func_8022F530();
}
} // namespace cf

#include "kyoshin/help/CHelp_ItemCole.hpp"

#include "kyoshin/cf/CHelpManager.hpp"

namespace cf {
u8 CHelp_ItemCole::getItemCole() {
    return cf::CHelpManager::sInstance->mField14;
}
} // namespace cf

#include "kyoshin/help/CHelp_Kizuna.hpp"

#include "kyoshin/cf/CHelpManager.hpp"

namespace cf {
u8 CHelp_Kizuna::isKizunaFlag() {
    return cf::CHelpManager::sInstance->mField15;
}
} // namespace cf

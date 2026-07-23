#include "kyoshin/help/CHelp_Pg.hpp"
#include "kyoshin/cf/CBattleManager.hpp"

namespace cf {
bool CHelp_Pg::func_802B85A4() {
    s32 value = *reinterpret_cast<s32*>(reinterpret_cast<u8*>(CBattleManager::getInstance()) + 0x194);
    return mThreshold <= static_cast<f32>(value);
}
} // namespace cf

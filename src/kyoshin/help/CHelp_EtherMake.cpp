#include "kyoshin/help/CHelp_EtherMake.hpp"

namespace cf {
bool CHelp_EtherMake::isEtherReady() {
    u32 result = 0;
    if (MakeCrystalIsCreated() != 0) {
        if (MakeCrystalGetWinData() != 0) {
            result = 1;
        }
    }
    return result != 0;
}

void CHelp_EtherMake::notifyEther() {
    MakeCrystalGetWinData();
}
} // namespace cf

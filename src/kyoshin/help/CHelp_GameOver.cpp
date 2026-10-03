#include "kyoshin/help/CHelp_GameOver.hpp"

struct CHelpFlagBag {
    u8 pad[0x18];
    u8 mByte18; // +0x18
};

// CfGameManager::getQueuedFileEventCount forward (avoid including CfGameManager.hpp which
// redeclares lbl_eu_80664A10 as CHelpManager* and conflicts with the
// CHelpFlagBag* view used by this TU).
namespace cf {
class CfGameManager {
public:
    static s32 getQueuedFileEventCount();
};
}

namespace cf {
bool CHelp_GameOver::isAvailable() {
    if (func_80295D30(lbl_eu_80664A10) == 0) {
        return false;
    }
    if (unkC != 0 && CfGameManager::getQueuedFileEventCount() < unkC) {
        return false;
    }
    if (mFlag10 != 0 && lbl_eu_80664A10->mByte18 == 0) {
        return false;
    }
    return true;
}
} // namespace cf

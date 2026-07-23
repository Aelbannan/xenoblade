#include "kyoshin/help/CHelp_GameOver.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

struct CHelpFlagBag {
    u8 pad[0x18];
    u8 mByte18; // +0x18
};

extern "C" CHelpFlagBag* lbl_eu_80664A10;
extern "C" UNKWORD func_80295D30(CHelpFlagBag* bag);

namespace cf {
bool CHelp_GameOver::func_802B80F4() {
    if (func_80295D30(lbl_eu_80664A10) == 0) {
        return false;
    }
    if (unkC != 0 && CfGameManager::func_800822F4() < unkC) {
        return false;
    }
    if (mFlag10 != 0 && lbl_eu_80664A10->mByte18 == 0) {
        return false;
    }
    return true;
}
} // namespace cf

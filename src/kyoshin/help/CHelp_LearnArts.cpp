#include "kyoshin/help/CHelp_LearnArts.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CHelpManager.hpp"
#include "kyoshin/cf/object/CfObjectPoint.hpp"

namespace cf {

// Trivial flag reset; kept before func_802B8398 to mirror retail TU order.
void CHelp_LearnArts::isHelpAvailable() {
    mLearnArtsFlag = 0;
}

bool CHelp_LearnArts::func_802B8398() {
    int bound;
    const char* colName;
    s16 threshold;
    u8* artsBase;
    u8 byteVal;
    int counter;
    union {
        u32 v;   // full-word BDAT cell value
        u8 b[4]; // byte view for the low-byte art-id extraction
    } col;

    if (cf::CfGameManager::isSceneLoading()) {
        return false;
    }
    if (lbl_eu_80664A10->mLearnArtsFlag == 0) {
        return false;
    }
    if (func_80122450() != 0) {
        // Arts-state helper; copy its result byte into our flag.
        mLearnArtsFlag = func_8012246C();
        return false;
    }
    if (func_801B481C() != 0) {
        mLearnArtsFlag = 0;
        return false;
    }
    if (mLearnArtsFlag != 0) {
        return false;
    }
    // Clear the manager-side flag, then scan every party weapon-type row:
    // character 1 only checks its first 8 art slots, the rest check all 16.
    // Returns true as soon as some character knows >= mWpnTypeCount arts.
    lbl_eu_80664A10->mLearnArtsFlag = 0;
    threshold = mWpnTypeCount;
    for (int character = 1; character <= 13; ++character) {
        colName = (const char*)lbl_eu_805138AC;
        cf::CHelpLearnArtsCharData* charData = (cf::CHelpLearnArtsCharData*)func_8009EC9C((u16)character);
        artsBase = charData->mArts;
        col.v = getBdatStringColumnValue(lbl_eu_806640F4, colName, charData->mWpnType);
        byteVal = col.b[0];
        bound = ((s16)character == 1) ? 8 : 16;
        counter = 0;
        for (int j = 0; j < bound; ++j) {
            if (func_801F9268(artsBase, byteVal, j)) {
                ++counter;
            }
        }
        if (threshold > counter) {
            continue;
        }
        return true;
    }
    return false;
}
} // namespace cf

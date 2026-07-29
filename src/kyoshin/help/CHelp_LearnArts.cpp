#include "kyoshin/help/CHelp_LearnArts.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

struct CHelpFlagBag {
    u8 pad[0x16];
    u8 mLearnArtsFlag; // +0x16
};

extern "C" CHelpFlagBag* lbl_eu_80664A10;
extern "C" void* lbl_eu_806640F4;
extern "C" UNKWORD func_80122450();
extern "C" u8 func_8012246C();
extern "C" u32 func_801B481C();
extern "C" void* func_8009EC9C(u16 index);
extern "C" bool func_801F9268(unsigned char* p, int i, int j);

namespace cf {
bool CHelp_LearnArts::func_802B8398() {
    if (cf::CfGameManager::func_800829B8()) {
        return false;
    }
    if (lbl_eu_80664A10->mLearnArtsFlag == 0) {
        return false;
    }
    if (func_80122450() != 0) {
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
    lbl_eu_80664A10->mLearnArtsFlag = 0;
    const char* columnName = "wpn_type";
    s16 threshold = mWpnTypeCount;
    for (s16 character = 1; character <= 12; character++) {
        void* charData = func_8009EC9C((u16)character);
        u16 wpnType = *(u16*)((u8*)charData + 0xC);
        const char* strResult = CBdat::getBdatStringColumnValue(lbl_eu_806640F4, columnName, wpnType);
        u8 byteVal = (u8)(u32)strResult;
        u8* artsBase = (u8*)charData + 0xE8;
        int bound = (character == 1) ? 8 : 16;
        int counter = 0;
        for (int j = 0; j < bound; j++) {
            if (func_801F9268(artsBase, byteVal, j)) {
                counter++;
            }
        }
        if (threshold > counter) {
            return true;
        }
    }
    return false;
}

void CHelp_LearnArts::func_802B84E0() {
    mLearnArtsFlag = 0;
}
} // namespace cf

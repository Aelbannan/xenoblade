#include "kyoshin/help/CHelp_LearnArts.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerUnityHelpers.hpp"
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
    s16 threshold = mWpnTypeCount;
    for (int character = 1; character <= 12; character++) {
        UnkClass_8009EC9C* charData = func_8009EC9C(static_cast<u16>(character));
        u16 wpnType = charData->field_0x0[0xC]; // FIXME: proper field offset
        const char* strResult = CBdat::getBdatStringColumnValue(lbl_eu_806640F4, "wpn_type", wpnType);
        u8 byteFromStack = reinterpret_cast<u32>(strResult); // truncated to byte
        int bound = (character == 1) ? 8 : 16;
        int counter = 0;
        for (int j = 0; j < bound; j++) {
            if (func_801F9268(reinterpret_cast<unsigned char*>(charData) + 0xE8, byteFromStack, j)) {
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

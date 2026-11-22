#include "kyoshin/help/CHelp_LandMark.hpp"
#include "kyoshin/cf/CtrlRemoteApi.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

namespace cf {

CHelp_LandMark::CHelp_LandMark(u32 owner, u32 param, s32 landmarkId) : CHelp(owner, param) {
    mLandmarkId = landmarkId;
    mTimer = 0;
    CtrlRemote_ResetSlotArrayObj(static_cast<IFlagEvent*>(this));
}

CHelp_LandMark::~CHelp_LandMark() {
    CtrlRemote_ResetSlotArrayByIndex((u32)static_cast<IFlagEvent*>(this));
}

void CHelp_LandMark::CHelp_UnkVirtualFunc1() {
    mTimer = 0;
    mSavedFlags = 0;
}

void CHelp_LandMark::FlagEvent1(int eventId, int arg2, int arg3) {
    bool touched = CtrlRemote_TouchBitByArg(mOwner) != 0;
    if (touched) return;
    if (mLandmarkId + 0x20C8 != eventId) return;
    if (arg2 == 0) return;

    mSavedFlags = CfGameManager::getEnabledInputFlags();
    CfGameManager::enablePadFlags(-1, false);
    gmGameFlags |= 0x2000;
    mTimer = 75;
}

UNKWORD CHelp_LandMark::f10() {
    if (mTimer <= 0) {
        return 0;
    }
    if (--mTimer > 0) {
        return 0;
    }
    CfGameManager::enablePadFlags(mSavedFlags, true);
    gmGameFlags &= ~0x2000;
    return 1;
}

} // namespace cf

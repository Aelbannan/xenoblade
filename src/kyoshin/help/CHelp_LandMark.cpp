#include "kyoshin/help/CHelp_LandMark.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" int func_8009CF8C(void*);
extern "C" void* func_8009D414(void*);
extern "C" void func_8009D514(void*);
extern u32 lbl_eu_80663E24;

namespace cf {

CHelp_LandMark::CHelp_LandMark() : CHelp() {
    // Subobject at +0x0C is the second base class.
    // Using pointer arithmetic is required because there's no
    // named type for this subobject in single-inheritance C++.
    func_8009D414(reinterpret_cast<char*>(this) + 0x0C);
}

CHelp_LandMark::~CHelp_LandMark() {
    func_8009D514(reinterpret_cast<char*>(this) + 0x0C);
}

void CHelp_LandMark::func_802B8280() {
    mTimer = 0;
    mSavedFlags = 0;
}

void CHelp_LandMark::func_802B8290(u32 param1, u32 param2) {
    int result = func_8009CF8C(mOwner);
    if (result == 0 && (field_10 + 0x20c8u == param1) && param2 != 0) {
        mSavedFlags = CfGameManager::getEnabledInputFlags();
        CfGameManager::enablePadFlags(-1, false);
        lbl_eu_80663E24 |= 0x2000;
        mTimer = 0x4B;
    }
}

u32 CHelp_LandMark::func_802B8328() {
    if (mTimer <= 0) {
        return 0;
    }
    s32 newVal = mTimer - 1;
    mTimer = newVal;
    if (newVal > 0) {
        return 0;
    }
    CfGameManager::enablePadFlags(mSavedFlags, true);
    lbl_eu_80663E24 &= ~0x2000;
    return 1;
}

void CHelp_LandMark::func_802B8388(u32 param1, u32 param2) {
    func_802B8290(param1, param2);
}

void CHelp_LandMark::func_802B8390() {
    this->~CHelp_LandMark();
}

} // namespace cf

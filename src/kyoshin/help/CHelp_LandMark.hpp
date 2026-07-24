#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {

class CHelp_LandMark : public CHelp {
public:
    CHelp_LandMark();
    ~CHelp_LandMark();

    void func_802B8280();
    void func_802B8290(u32 param1, u32 param2);
    u32 func_802B8328();
    void func_802B8388(u32 param1, u32 param2);
    void func_802B8390();

    // CHelp at +0x00..+0x0B (mOwner, mParam, mVtbl)
    u32 field_10;      // +0x10
    s32 mTimer;        // +0x14
    u32 mSavedFlags;   // +0x18
};

} // namespace cf

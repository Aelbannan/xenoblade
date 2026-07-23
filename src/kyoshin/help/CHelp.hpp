#pragma once

#include <types.h>

namespace cf {

// Manual interface table at CHelp+0x8 (not a C++ vptr at +0).
struct CHelpVtbl {
    void* mSlots[8]; // +0x00..+0x1C
};

// Retail ctor writes: owner@0, param@4, vtbl@8. Base size is 0xC.
// Construction uses retail symbol __ct__Q22cf5CHelpFv(self, owner, param).
class CHelp {
public:
    void CHelp_UnkVirtualFunc2();

    void* mOwner; // 0x0
    u32 mParam; // 0x4 (low byte used by UnkVirtualFunc2)
    CHelpVtbl* mVtbl; // 0x8
};

// Flag helper sharing the CHelp prefix; flag byte at +0xC.
// CBC/CE4 keep Fv symbols via extern "C" in CHelp.cpp (arg in r4).
class CHelpSwitch : public CHelp {
public:
    void func_802B7CB0();

    u8 mFlag; // 0xC
};

} // namespace cf

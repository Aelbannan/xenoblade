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
    void func_802B7C68();

    void* mOwner; // 0x0
    u32 mParam; // 0x4 (low byte used by UnkVirtualFunc2)
    CHelpVtbl* mVtbl; // 0x8
    CHelp(void* owner, u32 param);
};

// Flag helper sharing the CHelp prefix; flag byte at +0xC.
// CBC/CE4 keep Fv symbols via extern "C" in CHelp.cpp (arg in r4).
class CHelpSwitch : public CHelp {
public:
    void func_802B7CB0();

    u8 mFlag; // 0xC
    u32 func_802B7CBC(u32 flag);
    u32 func_802B7CE4(u8 flag);
};

} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" cf::CHelpVtbl lbl_eu_8053B3A0;
extern "C" void func_80134D18(u32 param0, UNKWORD param1, UNKWORD param2);
extern "C" void func_8009D018(void* owner, u32 flag);
extern "C" void func_8013DB6C(u32 mode, u32 param, u32 a, u32 b);
extern "C" void func_8029A658();

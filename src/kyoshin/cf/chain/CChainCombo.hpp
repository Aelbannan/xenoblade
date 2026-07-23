#pragma once

#include <types.h>

// Gauge pair helpers (CSysWinSave.cpp) - C++ linkage -> func_80294824__FPv.
void func_80294824(void* gauge);
void func_80294834(void* gauge);
void func_802AA338();

namespace cf {

// Two-float chain gauge at CChainCombo+0xC (written by func_80294824/34/44).
struct CChainGauge {
    float mVal0; // 0x0
    float mVal1; // 0x4
};

// Retail vtable lbl_eu_80538994 lives in split1 (dtor only); not emitted here.
extern "C" void* lbl_eu_80538994[];

/* Chain arts combo tracker. Size 0x18.
   Manual vptr @0x14 (not a normal C++ vptr-at-0 class) to match retail and
   avoid a weak local dtor / __vt__ reloc name mismatch. */
struct CChainCombo {
    int mArtsType; // 0x0 - last arts category byte (0..8)
    int mComboCount; // 0x4 - steps 0..5
    bool mPending; // 0x8 - set externally; consumed by func_80293EEC
    u8 pad9[3];
    CChainGauge mGauge; // 0xC
    void* mVtbl; // 0x14 - lbl_eu_80538994

    CChainCombo();
    void func1();
};

} // namespace cf

// Opaque objects that only expose a C++-style vptr at +0 (forces r12-style loads).
struct CChainVObj {
    void** mVtbl;
};

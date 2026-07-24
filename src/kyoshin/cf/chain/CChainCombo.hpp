#pragma once

#include <types.h>

namespace cf {

// Two-float chain gauge at CChainCombo+0xC (written by func_80294824/34/44).
struct CChainGauge {
    float mVal0; // 0x0 first gauge value (initialized by func_80294824)
    float mVal1; // 0x4 second gauge value (written by func_80294834/44)
};

} // namespace cf

// Gauge pair helpers (CSysWinSave.cpp).
// Declared as extern "C" with explicit mangled names so callers in this TU
// reference the correct retail symbols without re-mangling when the parameter
// type is not void*.  The gauges these operate on live at CChainCombo+0xC.
extern "C" void func_80294824__FPv(cf::CChainGauge* gauge);
extern "C" void func_80294834__FPv(cf::CChainGauge* gauge);

// Resets/respawns chain combo state (CMenuBattleChain.cpp).
extern "C" void func_802AA338__Fv();

// Retail vtable lbl_eu_80538994 lives in split1 (dtor only); not emitted here.
extern "C" void* lbl_eu_80538994[];

namespace cf {

/* Chain arts combo tracker. Size 0x18.
   Manual vptr @0x14 (not a normal C++ vptr-at-0 class) to match retail and
   avoid a weak local dtor / __vt__ reloc name mismatch. */
struct CChainCombo {
    int mArtsType;   // 0x0 - last arts category byte (0..8)
    int mComboCount; // 0x4 - steps 0..5
    bool mPending;   // 0x8 - set externally; consumed by func_80293EEC
    u8 pad9[3];
    CChainGauge mGauge; // 0xC - chain gauge pair
    void* mVtbl;        // 0x14 - lbl_eu_80538994

    CChainCombo();
    void func1();
};

} // namespace cf

// Opaque objects that only expose a C++-style vptr at +0 (forces r12-style loads).
struct CChainVObj {
    void** mVtbl;
};

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
extern "C" void func_80294844(cf::CChainGauge* gauge, float value);

// Resets/respawns chain combo state (CMenuBattleChain.cpp).
extern "C" void func_802AA338__Fv();

// Retail vtable lbl_eu_80538994 lives in split1 (dtor only); not emitted here.
extern "C" void* lbl_eu_80538994[];

// 3-entry table indexed by probability thresholds in func_80293EEC.
extern "C" int lbl_eu_80538988[3];

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
    ~CChainCombo();
    void func1();
};

} // namespace cf

// Object returned by CActorParam_UnkVirtualFunc132 (vtable[0x2A4]).
// Has a pointer at +0x50 to an arts category struct.
struct CChainCombo_ArtsCategoryHolder {
    u8 pad[0x50];
    struct CChainCombo_ArtsCategory* mArtsCategory; // 0x50
};

// Object with arts category byte at +0x3E.
struct CChainCombo_ArtsCategory {
    u8 pad[0x3e];
    u8 mArtsCategory; // 0x3e
};

// Opaque object that only exposes a C++-style vptr at +0 (forces r12-style loads).
struct CChainVObj {
    void** mVtbl;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_8013EAB0();
extern "C" void func_802A07F4(int, void*);

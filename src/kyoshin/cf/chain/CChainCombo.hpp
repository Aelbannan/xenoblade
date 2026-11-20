#pragma once

#include <types.h>

namespace cf {

// Two-float chain gauge at CChainCombo+0xC (written by initChainGauge/34/44).
struct CChainGauge {
    float mVal0; // 0x0 first gauge value (initialized by initChainGauge)
    float mVal1; // 0x4 second gauge value (written by resetChainGauge/44)
};

} // namespace cf

// Gauge pair helpers (CSysWinSave.cpp).
// Declared as extern "C" with explicit mangled names so callers in this TU
// reference the correct retail symbols without re-mangling when the parameter
// type is not void*.  The gauges these operate on live at CChainCombo+0xC.
extern "C" void initChainGauge__FPv(cf::CChainGauge* gauge);
extern "C" void resetChainGauge__FPv(cf::CChainGauge* gauge);
extern "C" void setChainGauge(cf::CChainGauge* gauge, float value);

// Resets/respawns chain combo state (CMenuBattleChain.cpp).
extern "C" void requestCancelChain__Fv();

// __vt__Q22cf11CChainCombo (retail .data:0x80538994, dtor-only): emitted here
// by the compiler, next to the out-of-line constructor in CChainCombo.cpp.

// 3-entry table indexed by probability thresholds in payoutChainCombo.
extern "C" int lbl_eu_80538988[3];

namespace cf {

/* Chain arts combo tracker. Size 0x18. Polymorphic after its members, so the
   vptr sits at 0x14; the dtor is inline (retail keeps only a weak copy) and
   the 0xC-byte vtable is emitted here, next to the out-of-line constructor. */
struct CChainCombo {
    int mArtsType;   // 0x0 - last arts category byte (0..8)
    int mComboCount; // 0x4 - steps 0..5
    bool mPending;   // 0x8 - set externally; consumed by payoutChainCombo
    u8 pad9[3];
    CChainGauge mGauge; // 0xC - chain gauge pair

    CChainCombo();
    virtual ~CChainCombo() {}
    void func1();
};

} // namespace cf

// Object returned by CActorParam_getMoveRecord (vtable[0x2A4]).
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

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)


// TU-private imports for kyoshin/cf/chain/CChainCombo.cpp (moved out of the public
// CChainCombo.hpp, which keeps the classes).

extern "C" void UIWin_CreateAA2A0Win();

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
extern "C" void func_80294844(cf::CChainGauge* gauge, float value);

// Resets/respawns chain combo state (CMenuBattleChain.cpp).
extern "C" void requestCancelChain__Fv();

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
    void** mVtbl;       // 0x14 - lbl_eu_80538994

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

// View of CfObjectActor's CfObjectMove sub-object at absolute +0x3E9C
// (the headers' C++ base layout places it 8 bytes off, so reach it raw).
struct CChainCombo_MoveSub {
    u8 _pad[0x3E9C];
    void** mVtbl; // 0x3E9C
};

// Vtable proxies (dummy virtuals pin slot offsets; only ever used through
// pointer casts, so no vtable is emitted and calls take the r12 dispatch).
class CChainCombo_Vt4CIf {
public:
    virtual void _d008(); virtual void _d00C(); virtual void _d010();
    virtual void _d014(); virtual void _d018(); virtual void _d01C();
    virtual void _d020(); virtual void _d024(); virtual void _d028();
    virtual void _d02C(); virtual void _d030(); virtual void _d034();
    virtual void _d038(); virtual void _d03C(); virtual void _d040();
    virtual void _d044(); virtual void _d048();
    virtual int m4C(); // vtable +0x4C (CObjectParam_UnkVirtualFunc5)
};

class CChainCombo_Vt184If {
public:
    virtual void _d008(); virtual void _d00C(); virtual void _d010();
    virtual void _d014(); virtual void _d018(); virtual void _d01C();
    virtual void _d020(); virtual void _d024(); virtual void _d028();
    virtual void _d02C(); virtual void _d030(); virtual void _d034();
    virtual void _d038(); virtual void _d03C(); virtual void _d040();
    virtual void _d044(); virtual void _d048(); virtual void _d04C();
    virtual void _d050(); virtual void _d054(); virtual void _d058();
    virtual void _d05C(); virtual void _d060(); virtual void _d064();
    virtual void _d068(); virtual void _d06C(); virtual void _d070();
    virtual void _d074(); virtual void _d078(); virtual void _d07C();
    virtual void _d080(); virtual void _d084(); virtual void _d088();
    virtual void _d08C(); virtual void _d090(); virtual void _d094();
    virtual void _d098(); virtual void _d09C(); virtual void _d0A0();
    virtual void _d0A4(); virtual void _d0A8(); virtual void _d0AC();
    virtual void _d0B0(); virtual void _d0B4(); virtual void _d0B8();
    virtual void _d0BC(); virtual void _d0C0(); virtual void _d0C4();
    virtual void _d0C8(); virtual void _d0CC(); virtual void _d0D0();
    virtual void _d0D4(); virtual void _d0D8(); virtual void _d0DC();
    virtual void _d0E0(); virtual void _d0E4(); virtual void _d0E8();
    virtual void _d0EC(); virtual void _d0F0(); virtual void _d0F4();
    virtual void _d0F8(); virtual void _d0FC(); virtual void _d100();
    virtual void _d104(); virtual void _d108(); virtual void _d10C();
    virtual void _d110(); virtual void _d114(); virtual void _d118();
    virtual void _d11C(); virtual void _d120(); virtual void _d124();
    virtual void _d128(); virtual void _d12C(); virtual void _d130();
    virtual void _d134(); virtual void _d138(); virtual void _d13C();
    virtual void _d140(); virtual void _d144(); virtual void _d148();
    virtual void _d14C(); virtual void _d150(); virtual void _d154();
    virtual void _d158(); virtual void _d15C(); virtual void _d160();
    virtual void _d164(); virtual void _d168(); virtual void _d16C();
    virtual void _d170(); virtual void _d174(); virtual void _d178();
    virtual void _d17C(); virtual void _d180();
    virtual void m184(int value); // vtable +0x184 (CfObjectModel_UnkVirtualFunc4)
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_8013EAB0();
extern "C" void func_802A07F4(int, void*);

// CObjectParam vtable slot +0x4C dispatched through the CChainCombo_Vt4CIf
// proxy; CfObjectModel_UnkVirtualFunc4 goes through CChainCombo_Vt184If.

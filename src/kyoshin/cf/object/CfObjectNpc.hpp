#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

// C-ABI helper imports used by CfObjectNpc (retail-unmangled symbols).
extern "C" void* func_8003AA34();
extern "C" void* getFP__FPCc(const char* path);
extern "C" void func_800BE33C(void* obj, int flag);
extern "C" void func_800BE824(void* obj, int flag);
extern "C" void func_804B0AD4(void* obj, int param, float a, float b);
extern "C" u32 func_80061FFC();
extern "C" int func_8013EB90(int v);

// Retail data labels referenced by this unit. lbl_eu_80663E24 and
// lbl_eu_80663E14 come from CfObjectMove.hpp.
extern u8 lbl_eu_804FC580[];   // BDAT column-name string table base
extern void* lbl_eu_805298B8[];   // CfObjectNpc vtable (stored by the ctor)
extern float lbl_eu_80666AE0;
extern float lbl_eu_80666AE4;
extern float lbl_eu_80666AE8;
extern float lbl_eu_80666AEC;
extern float lbl_eu_80666AF0;
extern float lbl_eu_80666AF4;
extern float lbl_eu_80666AF8;
extern float lbl_eu_80666AFC;
extern double lbl_eu_80666B00;   // int->float conversion bias (2^52, sdata2)

// ---------------------------------------------------------------------------
// Retail constructor (hidden-parameter form)
// ---------------------------------------------------------------------------
namespace cf {
    class CfObjectNpc;
}

// The retail symbol __ct__Q22cf11CfObjectNpcFv is mangled as a parameter-less
// member ctor, but the ABI carries a second argument in r4: a heap-select
// flag written by the spawner in code_800B06A4 (1 => CfResPcImpl child,
// 0 => CfResReloadImpl child). See MWCC_PATTERNS.md "Fv-with-hidden-params".
// A real member ctor taking the flag would mangle to __ct__Q22cf11CfObjectNpcFi
// and break the caller relocs, so the body is a global function whose
// leading-"__" name MWCC emits verbatim (same precedent as __ct__cf_CfObjectEne).
// Returns the object pointer (MWCC ctor ABI keeps this in r3).
cf::CfObjectNpc* __ct__Q22cf11CfObjectNpcFv(cf::CfObjectNpc* self, int heapFlag);

// Base ctor chain import (defined in CfObjectMove.cpp as the real member
// ctor; referenced under its retail mangled name).
extern "C" void __ct__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self);

// Resource-impl child ctors (legacy flat retail names; defined in
// CfResPcImpl.cpp / CfResReloadImpl.cpp). Only forward declarations here -
// including CfResReloadImpl.hpp would collide its typed func_800BE824
// declaration with the void* form above.
extern "C" u8* __ct__cf_CfResPcImpl(u8* obj, cf::CfObjectMove* parent);
extern "C" void* __ct__cf_CfResReloadImpl(void* self, void* parent);

// Vtable-layout proxy for calling cf-chain vtable slots that carry
// arguments or return values the base header declares as plain void slots.
// Dummy virtuals pin every offset so the named slots land exactly where the
// retail CfObjectNpc vtable has them (same shape as CfObjectModel.hpp's
// CfObjectVt14C proxies).
class CfObjectNpcVt {
public:
    // NB: MWCC implicitly reserves vtable +0x0/+0x4 for the dtor pair.
    virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
    virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
    virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
    virtual void m38();
    virtual void setName(const char* name);   // +0x3C (CfObject_UnkVirtualFunc15)
    // +0x40 .. +0xD8
    virtual void m40(); virtual void m44(); virtual void m48(); virtual void m4C();
    virtual void m50(u32 flag); virtual void m54(); virtual void m58(); virtual void m5C();
    virtual void m60(); virtual void m64(); virtual void m68(); virtual void m6C();
    virtual void m70(); virtual void m74(); virtual void m78(); virtual void m7C();
    virtual void m80(); virtual void m84(); virtual void m88();
    // +0x8C leaves a scaled float in f1 despite the void base declaration.
    virtual f32 getScale8C();
    virtual void m90(); virtual void m94(); virtual void m98(); virtual void m9C();
    virtual void mA0(); virtual void mA4(); virtual void mA8(); virtual void mAC();
    virtual void mB0(); virtual void mB4(); virtual void mB8(); virtual void mBC();
    virtual void mC0(); virtual void mC4(); virtual void mC8(); virtual void mCC();
    virtual void mD0(); virtual void mD4(); virtual void mD8();
    virtual void setFade(float value);        // +0xDC (CfObject_UnkVirtualFunc35)
    // +0xE0 .. +0x130
    virtual void mE0(); virtual void mE4(); virtual void mE8(); virtual void mEC();
    virtual void mF0(); virtual void mF4(); virtual void mF8(); virtual void mFC();
    virtual void m100(); virtual void m104(); virtual void m108(); virtual void m10C();
    virtual void m110(); virtual void m114(); virtual void m118(); virtual void m11C();
    virtual void m120(); virtual void m124(); virtual void m128(); virtual void m12C();
    virtual void m130();
    virtual void setTime(float value);        // +0x134 (CfObject_UnkVirtualFunc57)
    virtual u32* getScalePtr();               // +0x138 (CfObject_UnkVirtualFunc58)
    virtual void setSpeed(float value);       // +0x13C (CfObject_UnkVirtualFunc59)
    // +0x140 .. +0x220
    virtual void m140(); virtual void m144(); virtual void m148(); virtual void m14C();
    virtual void m150(); virtual void m154(); virtual void m158(); virtual void m15C();
    virtual void m160(); virtual void m164(); virtual void m168(); virtual void m16C();
    virtual void m170(); virtual void m174(); virtual void m178(); virtual void m17C();
    virtual void m180(); virtual void m184(); virtual void m188(); virtual void m18C();
    virtual void m190(); virtual void m194(); virtual void m198(); virtual void m19C();
    virtual void m1A0(); virtual void m1A4(); virtual void m1A8();
    virtual void m1AC(u32 param, const char* table); // +0x1AC BDAT column loader
    virtual void m1B0(); virtual void m1B4(); virtual void m1B8(); virtual void m1BC();
    virtual void m1C0(); virtual void m1C4(); virtual void m1C8(); virtual void m1CC();
    virtual void m1D0();
    virtual void moveTime(float value);       // +0x1D4 (CfObjectMove_UnkVirtualFunc4)
    virtual void m1D8(); virtual void m1DC();
    virtual void m1E0(); virtual void m1E4(); virtual void m1E8(); virtual void m1EC();
    virtual void m1F0(); virtual void m1F4(); virtual void m1F8(); virtual void m1FC();
    virtual void m200(); virtual void m204(); virtual void m208(); virtual void m20C();
    virtual void m210(); virtual void m214(); virtual void m218(); virtual void m21C();
    virtual void m220();
    virtual void applyParams();               // +0x224 (slot after CfObjectMove_UnkVirtualFunc23)
};

namespace cf {
    // size: 0x724
    class CfObjectNpc : public CfObjectMove {
    public:
        // NB: constructed only through the hidden-param free function
        // __ct__Q22cf11CfObjectNpcFv (declared above); no member ctor.
        virtual ~CfObjectNpc();

        // NPC-specific member functions
        bool initNpcFlags();
        void updateNpcDialog();
        void func_800BF764();
        void loadIconType();
        s16 getRltMeet();
        u8 getIconType();

        // Reads the f32 returned by vtable slot 0x8C (declared void in the
        // base header, but retail leaves a scaled float in f1).
        float readSlot8c();

        // 0x0: vtable
        // 0x0-718: CfObjectMove

        // NPC-specific fields
        u8  mIconType;    // 0x71C - NPC icon type (from BDAT icon_type column)
        u8  pad_71D;      // 0x71D - alignment padding
        s16 mRltMeet;     // 0x71E - relationship meet value, -1 = uninitialized
        f32 mTimer;       // 0x720 - timer for NPC dialogue trigger
    };
}

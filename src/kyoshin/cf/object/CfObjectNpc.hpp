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

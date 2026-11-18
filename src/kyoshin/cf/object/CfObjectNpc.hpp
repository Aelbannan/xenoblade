#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

// C-ABI helper imports used by CfObjectNpc (retail-unmangled symbols).
extern "C" void* Bdat_GetTable_AA34();
extern "C" void CfObjectMove_setModelDisplayFlag(void* obj, int flag);
extern "C" void CfObjectMove_setRegionAttached(void* obj, int flag);
extern "C" void ColiSetMoveVec2(void* obj, int param, float a, float b);
extern "C" u32 CfRes_getAllocHandle();
extern "C" int UIWin_HasPendingUpdate(int v);

// Retail data labels referenced by this unit. lbl_eu_80663E24 and
// lbl_eu_80663E14 come from CfObjectMove.hpp.
extern const float lbl_eu_80666AE0;
extern const float lbl_eu_80666AE4;
extern const float lbl_eu_80666AE8;
extern const float lbl_eu_80666AEC;
extern const float lbl_eu_80666AF0;
extern const float lbl_eu_80666AF4;
extern const float lbl_eu_80666AF8;
extern const float lbl_eu_80666AFC;
extern const double lbl_eu_80666B00;   // int->float conversion bias (2^52, sdata2)

// Resource-impl child ctors (legacy flat retail names; defined in
// CfResPcImpl.cpp / CfResReloadImpl.cpp). Only forward declarations here -
// including CfResReloadImpl.hpp would collide its typed CfObjectMove_setRegionAttached
// declaration with the void* form above.
extern "C" u8* __ct__cf_CfResPcImpl(u8* obj, cf::CfObjectMove* parent);
extern "C" void* __ct__cf_CfResReloadImpl(void* self, void* parent);



namespace cf {
    // size: 0x724
    class CfObjectNpc : public CfObjectMove {
    public:
        // heapFlag selects the resource-impl child allocated into +0xB0
        // (1 => CfResPcImpl, 0 => CfResReloadImpl); passed by the spawner.
        CfObjectNpc(int heapFlag);
        virtual ~CfObjectNpc();                       //0x54

        virtual int CfObject_UnkVirtualFunc2();       //0x58 (initialise NPC flags)
        virtual void CfObject_UnkVirtualFunc4();      //0x60 (per-frame dialogue update)
        virtual void CObjectParam_UnkVirtualFunc4();  //0x48 (load NPC BDAT params)

        virtual void loadIconType();                  //0x224
        virtual u8 getIconType();                     //0x228
        virtual s16 getRltMeet();                     //0x22C

        // 0x0-718: CfObjectMove

        u8  mIconType;    // 0x71C - NPC icon type (from BDAT icon_type column)
        u8  pad_71D;      // 0x71D - alignment padding
        s16 mRltMeet;     // 0x71E - relationship meet value, -1 = uninitialized
        f32 mTimer;       // 0x720 - timer for NPC dialogue trigger
    };
}

#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CfObjectMoveHelpers.hpp"

#include "kyoshin/plugin/ocBdat.hpp"

// C-ABI helper imports used by CfObjectNpc (retail-unmangled symbols).
extern "C" void* Bdat_GetTable_AA34();
extern "C" void CfObjectMove_setRegionAttached(void* obj, int flag);
extern "C" void ColiSetMoveVec2(void* obj, int param, float a, float b);
extern "C" u32 CfRes_getAllocHandle();
extern "C" int UIWin_HasPendingUpdate(int v);


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

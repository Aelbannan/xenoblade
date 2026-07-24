#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {
    // size: 0x724
    class CfObjectNpc : public CfObjectMove {
    public:
        CfObjectNpc();
        virtual ~CfObjectNpc();

        // NPC-specific member functions
        bool func_800BF46C();
        void func_800BF4DC();
        void func_800BF764();
        void func_800BF8CC();
        s16 func_800BF920();
        u8 func_800BF984();

        // 0x0: vtable
        // 0x0-718: CfObjectMove

        // NPC-specific fields
        u8  pad_718[4];   // 0x718 - unknown
        u8  mIconType;    // 0x71C - NPC icon type (from BDAT icon_type column)
        u8  pad_71D;      // 0x71D - alignment padding
        s16 mRltMeet;     // 0x71E - relationship meet value, -1 = uninitialized
        f32 mTimer;       // 0x720 - timer for NPC dialogue trigger
    };
}

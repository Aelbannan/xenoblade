#pragma once

#include <types.h>

namespace cf {
    class CChainChance {
    public:
        // Fields initialized to 0 by ChainChance_Reset (partial init function)
        u16 mChainCount;      //0x0 - Number of successful chain links/extensions
        u8 unk2[2];           //0x2
        u32 unk4;             //0x4 - Possibly a pointer or timer
        u16 mField08;         //0x8 - Partially initialized by ChainChance_Reset
        u16 mField0A;         //0xA - Partially initialized by ChainChance_Reset
        u8 mField0C;          //0xC - Partially initialized by ChainChance_Reset
        u8 mPadding0D[3];     //0xD
        //0x10: vtable

        virtual ~CChainChance();

        void ChainChance_Reset() {
            mChainCount = 0;
            mField08 = 0;
            mField0A = 0;
            mField0C = 0;
        }

        u8 unk14[4];          //0x14
    };
}

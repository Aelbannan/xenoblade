#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainTemp.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"
#include <cstring>

namespace cf {
    //size: 0x80
    // Vtable at 0x70 is manually managed; no implicit C++ vtable at 0x00.
    class CChainActor {
    public:
        u32 unk0;
        CChainTemp mChainTemp; //0x4
        u16 unk6C;              //0x6C
        u8 _pad6E[2];           //0x6E
        u32 mVTable;            //0x70: vtable pointer (manually managed, non-standard ABI)

        CChainActor() : unk6C(0) {
            unk0 = 0;
        }
        ~CChainActor();

        CChainEffect mChainEffect; //0x74
    };
}

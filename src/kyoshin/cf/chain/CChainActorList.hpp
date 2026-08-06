#pragma once

#include <types.h>
#include "kyoshin/cf/chain/UnkClass_8027AD70.hpp"
#include "monolib/util.hpp"

namespace cf {
    //size: 0x1DB0
    class CChainActorList {
    public:
        UnkClass_8027AD70 unk0; //0x0
        u8 unk1D80[8];
        reslist<CChainActor*> mChainActorList; //0x1D88
        u8 unk1DA8[4];
        //0x1DAC: vtable

        CChainActorList();
        virtual ~CChainActorList();
    };

    // size 0xE8: fixed array of CChainActor* with an element count
    // (field at 0xE0) and a byte flag (field at 0xE4).
    class CChainList {
    public:
        CChainActor* mActors[0x38]; //0x0
        u32 mCount;                 //0xE0
        u8 mFlag;                   //0xE4
    };

    // Minimal object manipulated by func_8027C154 (u16 field at 0x0).
    class CChainMsg {
    public:
        u16 field_0x0; //0x0
    };

    // Minimal object manipulated by func_8027CBE8 (s32 field at 0x0).
    class CChainCounter {
    public:
        s32 field_0x0; //0x0
    };

    // Large object manipulated by func_8027C040 (u32 flag at 0x3F00).
    class CChainFlag {
    public:
        u8 field_0[0x3F00]; //0x0
        u32 field_0x3F00;   //0x3F00
    };
}

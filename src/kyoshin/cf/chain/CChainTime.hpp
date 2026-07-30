#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainEffect.hpp"

namespace cf {
    //size: 0x18
    class CChainTime {
    public:
        // 0x00: mTimer (float)
        // 0x04: mEnabled (u8)
        // 0x05: mPaused (u8)
        // 0x06: mLoop (u8)
        // 0x07: padding
        f32 mTimer;          // 0x00
        u8 mEnabled;         // 0x04
        u8 mPaused;          // 0x05
        u8 mLoop;            // 0x06
        u8 _pad07;           // 0x07
        CChainEffect mChainEffect; //0x08 (size 0xC)
        //0x14: vtable

        CChainTime();
        virtual ~CChainTime();
        
        void func_8027CE30();
    };
}

void func_8027CEB0(cf::CChainTime*, u8);
void func_8027CF3C(cf::CChainTime*);

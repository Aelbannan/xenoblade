#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

namespace cf {
    class __declspec(novtable) CChainActorEne : public CChainActor {
    public:
        CChainActorEne();
        virtual ~CChainActorEne();
        // overrides of CChainActor's first 16 virtuals (0x0C..0x48)
        virtual void _vf0C(int val) override; // 0x0C func_80281308
        virtual void _vf14() override; // 0x14 func_8028133C
        virtual int _vf20(int arg) override; // 0x20 func_80281384
        virtual void _vf30(int p1,int p2,int p3) override; // 0x30 func_80281438
        virtual void* _vf34() override; // 0x34 func_80281460
        virtual int _vf40() override; // 0x40 func_8028146C
        virtual int _vf44(void* arg) override; // 0x44 func_802818E4 (tail to 0x48)
        virtual int vf48(void* arg) override; // 0x48 func_802814E4
        // new slots beyond base's 0x48 (0x4C..0x74)
        virtual int _vf4C(); // 0x4C func_802815B8
        virtual void _vf50(); // 0x50 func_802816FC
        virtual int _vf54(); // 0x54 func_8028183C
        virtual int _vf58(); // 0x58 CChain_getChainCount
        virtual int _vf5C(); // 0x5C CChain_getZero_A584
        virtual int _vf60(); // 0x60 func_802818DC (int)
        virtual void _vf64(); // 0x64 func_802818D4 (void)
        void func_802818D4();
        s32 func_802818DC();
    };
}


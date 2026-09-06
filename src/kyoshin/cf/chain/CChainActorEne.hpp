#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

extern "C" cf::CChainActorVtbl lbl_eu_80538458;

namespace cf {
    class __declspec(novtable) CChainActorEne : public CChainActor {
    public:
        CChainActorEne();
        virtual ~CChainActorEne();
        void func_802818D4();
        s32 func_802818DC();
    };
}


#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

extern "C" cf::CChainActorVtbl lbl_eu_80538458;

namespace cf {
    class __declspec(novtable) CChainActorEne : public CChainActor {
    public:
        CChainActorEne();
        virtual ~CChainActorEne();
        void ChainEne_ReleaseEffect_18D4();
        s32 ChainEne_AlwaysTrue_18DC();
    };
}


#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

extern "C" cf::CChainActorVtbl lbl_eu_80538458;

struct EneChainObj;
struct BattleActor;

namespace cf {
    class __declspec(novtable) CChainActorEne : public CChainActor {
    public:
        CChainActorEne();
        virtual ~CChainActorEne();
        void ChainEne_ReleaseEffect_18D4();
        s32 ChainEne_AlwaysTrue_18DC();
        // Inline members used by the enemy-chain virtuals. Definitions live
        // in CChainActorEne.cpp next to the view types.
        static inline bool isChainEneSpecial(EneChainObj* obj);
        static inline EneChainObj* findChainEneTimeline();
        static inline BattleActor* findChainEneActor(EneChainObj* obj);
    };
}

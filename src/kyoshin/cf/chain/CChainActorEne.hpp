#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

namespace cf {
    class CChainActorEne : public CChainActor {
    public:
        // No additional members beyond CChainActor
        // Class methods use extern "C" linkage to match retail symbol names
    };
}

// Forwarding: passes &self->mChainEffect to func_802A0AA0
// Returns whether the effect matches the given parameter
extern "C" void func_802818D4(cf::CChainActorEne* self);

// Returns whether this enemy chain actor is active
extern "C" s32 func_802818DC(cf::CChainActorEne* self);


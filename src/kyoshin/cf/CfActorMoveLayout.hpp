#pragma once

#include <types.h>
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {
    // Canonical base adjustment for the embedded CfObjectMove of a
    // CfObjectActor. CfObjectActor.hpp's declared base sequence currently
    // puts CfObjectMove at +0x3EA4, 8 bytes past its retail +0x3E9C offset
    // (the constructor's vptr stores and the 0x45C0 actor size agree on
    // +0x3E9C). This composition spells the retail layout explicitly: the
    // CActorParam base occupies +0x0..+0x3380, the actor's CAIAction trailer
    // fills +0x3380..+0x3E9C, then the CfObjectMove base begins. A
    // static_cast from CfObjectMove* to this class therefore recovers the
    // actor base without a byte-offset cast at each call site.
    struct CfActorMoveTrailer {
        u8 field_0x0[0xB1C];
    };

    class CfActorMoveLayout : public CActorParam,
                              public CfActorMoveTrailer,
                              public CfObjectMove {
    public:
    };
}

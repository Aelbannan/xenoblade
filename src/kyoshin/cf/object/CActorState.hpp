#pragma once

#include <types.h>

namespace cf {
    class CObjectState;  // Move subobject pointed at by unk4

    class CActorState {
    public:
        virtual void CActorState_UnkVirtualFunc1(); //0x8

        // r4 = owning actor's CfObjectMove subobject (typically this+0x3E9C).
        CActorState(UNKTYPE* r4){
            unk4 = r4;
        }

        UNKTYPE* unk4;
    };
}

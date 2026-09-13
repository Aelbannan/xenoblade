#pragma once

#include <types.h>

namespace cf {
    class CObjectState;  // Move subobject pointed at by unk4

    class CActorState {
    public:
        virtual void CActorState_setObjStateBit3(); //0x8 (was UnkVirtualFunc1)
        void CActorState_UnkVirtualFunc1() { CActorState_setObjStateBit3(); } // legacy Unk spelling

        // r4 = owning actor's CfObjectMove subobject (typically this+0x3E9C).
        CActorState(UNKTYPE* r4){
            unk4 = r4;
        }

        UNKTYPE* unk4;
    };
}

#pragma once

#include <types.h>

namespace cf {
    class CObjectState;

    class CActorState {
    public:
        // Retail: inline (weak copy in CfObjectEne.cpp, vtable __vt__Q22cf11CActorState
        // emitted at the end of CActorParam.cpp .data). Kept out-of-line for now:
        // plugin/pluginEve.cpp declares `extern u32 __RTTI__Q22cf11CActorState[]`,
        // which collides (10322) once the inline vtable/RTTI is emitted there.
        virtual void CActorState_setObjStateBit3(); //0x8
        void CActorState_UnkVirtualFunc1() { CActorState_setObjStateBit3(); } // legacy Unk spelling

        // r4 = owning actor's CfObjectMove subobject (typically this+0x3E9C).
        CActorState(UNKTYPE* r4){
            unk4 = r4;
        }

        UNKTYPE* unk4;
    };
}

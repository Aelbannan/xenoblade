#pragma once

#include <types.h>

namespace cf {
    class IBattleEvent {
    public:
        virtual ~IBattleEvent();
        virtual void onActorRegister(void* actor); // 0xC
        virtual void IBattleEvent_UnkVirtualFunc2(void* actor); // 0x10
    };
}

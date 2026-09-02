#pragma once

#include <types.h>

namespace cf {
    class IBattleEvent {
    public:
        virtual ~IBattleEvent();
        virtual void onActorRegister(void* actor); // 0xC
    };
}

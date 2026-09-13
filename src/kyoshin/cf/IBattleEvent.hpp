#pragma once

#include <types.h>

namespace cf {
    class IBattleEvent {
    public:
        virtual ~IBattleEvent();
        virtual void onActorRegister(void* actor); // 0xC
        virtual void onActorUnregister(void* actor); // 0x10 - remove-path notify (func_800D9CA0), mirrors onActorRegister
    };
}

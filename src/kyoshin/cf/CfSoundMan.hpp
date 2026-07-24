#pragma once

#include <types.h>

namespace cf {
    class CfSoundMan {
    public:
        static void func_801BFC38(u32 r3, u32 r4, u32 r5, u32 r6, float f1);
    };
}

// No-op notification handler. Called from func_801C0DC4 when
// nw4r::snd::detail::SoundStartable::detail_StartSound succeeds.
// Left intentionally empty in the retail binary (single blr).
extern "C" void func_801C0DC0();

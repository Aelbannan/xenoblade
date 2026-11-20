#pragma once

#include <types.h>

struct CScnFrame {
    CScnFrame(u32 param);
    virtual ~CScnFrame();
    // CScn tears down every scene manager through ScnTermRelease::release;
    // the frame's fourth vtable word is this unimplemented slot.
    virtual void release(int flag) = 0;
    void update();

    u32 mParam;
    f32 mElapsed;
    f32 mStep;
    u8 mFlag;
};

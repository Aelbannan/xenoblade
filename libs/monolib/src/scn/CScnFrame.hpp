#pragma once

#include <types.h>

struct __declspec(novtable) CScnFrame {
    void* vtable;
    u32 mParam;
    f32 mElapsed;
    f32 mStep;
    u8 mFlag;

    virtual ~CScnFrame();
    void update();
};

#pragma once

#include <types.h>

class CTaskEnvironment {
public:
    CTaskEnvironment();
    virtual ~CTaskEnvironment();
    void Init();
    void Term();
    void Move();

    // TODO: add fields
    void Draw();
    u32 getField68() const { return *(u32*)((u8*)this + 0x68); }
};


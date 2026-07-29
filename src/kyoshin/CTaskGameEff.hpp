#pragma once

#include <types.h>

class CTaskGameEff {
public:
    CTaskGameEff();
    virtual ~CTaskGameEff();
    void Init();
    void Term();

    // Nested class at offset 0x58 (size and layout unknown)
    class CEffRenderHighPrio {
    public:
        ~CEffRenderHighPrio();
    };

    // TODO: add fields
    void Move();
    void cbRenderBefore();
    void Draw();
};


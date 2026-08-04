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
};


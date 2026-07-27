#pragma once

#include <types.h>

namespace cf {

class CfTaskMain {
public:
    virtual ~CfTaskMain();
    void Move();
    void Draw();
    void Tail();

    // TODO: add fields
    void Init();
    void Term();
};
} // namespace cf


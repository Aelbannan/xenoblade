#pragma once

#include <types.h>

namespace cf {

class CfRes {
public:
    virtual ~CfRes();
    CfRes();
    void OnFileEvent();

    // TODO: add fields
};
} // namespace cf

namespace cf {

class CfResTask {
public:
    virtual ~CfResTask();
    void Term();

    // TODO: add fields
    void Init();
    void Move();
    void Draw();
};
} // namespace cf


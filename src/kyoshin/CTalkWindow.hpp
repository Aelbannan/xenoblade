#pragma once

#include <types.h>

class CTalkWindow {
public:
    CTalkWindow();
    virtual ~CTalkWindow();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
    void Draw();
};


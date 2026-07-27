#pragma once

#include <types.h>

class CMenuSymbolMark {
public:
    CMenuSymbolMark();
    virtual ~CMenuSymbolMark();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

class CArrow3D {
public:
    virtual ~CArrow3D();
    void cbRenderBefore();
    void Term();
    void Init();

    // TODO: add fields
    void Move();
};


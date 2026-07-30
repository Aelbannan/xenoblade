#pragma once

#include <types.h>

class CMMClock {
public:
    virtual ~CMMClock();
    void OnFileEvent();

    // TODO: add fields
};

class CMMTex {
public:
    virtual ~CMMTex() {}
    void OnFileEvent();

    // TODO: add fields
};

class CMiniMap {
public:
    CMiniMap();
    virtual ~CMiniMap();
    void OnFileEvent();

    // TODO: add fields
};

class CMenuMiniMap2 {
public:
    virtual ~CMenuMiniMap2();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};


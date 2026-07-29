#pragma once

#include <types.h>

class CCol6CheckBat {
public:
    virtual ~CCol6CheckBat();
    CCol6CheckBat();
    void Term();
    void Move();

    // TODO: add fields
    void Init();
};

class CCol6Hint {
public:
    CCol6Hint();
    virtual ~CCol6Hint();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

class CCol6System {
public:
    CCol6System();
    virtual ~CCol6System();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

class CCol6Invite {
public:
    virtual ~CCol6Invite();
    void Init();
    void Move();

    // TODO: add fields
    void Term();
};


#pragma once

#include <types.h>
#include "kyoshin/CCol6CheckBat.hpp"
#include "kyoshin/CCol6Invite.hpp"

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

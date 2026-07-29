#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CScrollBar.hpp"

class CCol6Hint : public CProcess {
public:
    CCol6Hint(void*);
    virtual ~CCol6Hint();
    void Init() override;
    void Term() override;
    void Move() override;
    void cbRenderBefore();

    /* 0x3C */ u8 _pad3C[0x74 - 0x3C];
    /* 0x74 */ u8 mMemRegion[0x10];               // UnkClass_8045F564 scratch region
    /* 0x84 */ u8 _pad84[0x124 - 0x84];
    /* 0x124 */ u8 mCur[0x18];                      // CCur18 cursor widget
    /* 0x13C */ u8 mScrollBarBuf[0x40];             // CScrollBar widget (0x40 bytes)
};

class CCol6System : public CProcess {
public:
    CCol6System(void*);
    virtual ~CCol6System();
    void Init() override;
    void Term() override;
    void Move() override;
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


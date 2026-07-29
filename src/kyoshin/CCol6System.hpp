#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
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

class CCol6System : public CProcess {
public:
    CCol6System();
    virtual ~CCol6System();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x3C */ u8 _pad3C[0x74 - 0x3C];
    /* 0x74 */ u8 mMemRegion[0x10];       // UnkClass_8045F564
    /* 0x84 */ u8 _pad84[0xA8 - 0x84];
    /* 0xA8 */ u8 mCur1[0x18];            // CCur18
    /* 0xC0 */ u8 mCur2[0x18];            // CCur18
    /* 0xD8 */ u8 mSysWin1[0x3C];         // CSysWin
    /* 0x114 */ u8 mSysWin2[0x3C];        // CSysWin
};

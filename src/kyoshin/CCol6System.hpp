#pragma once

#include <types.h>

class __declspec(novtable) CCol6Hint {
public:
    CCol6Hint(void*);
    virtual ~CCol6Hint();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x04 */ u8 _pad04[0x74 - 0x04];
    /* 0x74 */ u8 mMemRegion[0x10];               // UnkClass_8045F564 scratch region
    /* 0x84 */ u8 _pad84[0x124 - 0x84];
    /* 0x124 */ u8 mCur[0x18];                      // CCur18 cursor widget
    /* 0x13C */ u8 mScrollBarBuf[0x40];             // CScrollBar widget (0x40 bytes)
};

class CCol6System {
public:
    CCol6System(void*);
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


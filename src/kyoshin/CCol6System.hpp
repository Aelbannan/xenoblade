#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CBaseCur.hpp"

class CCol6CheckBat {
public:
    virtual ~CCol6CheckBat();
    CCol6CheckBat();
    void Term();
    void Move();

    // TODO: add fields
    void Init();
};

/* CCur18: cursor widget 18 (0x18 bytes). Extends CBaseCur by 2 bytes.
   No standalone header yet; minimal declaration for CCol6Hint layout. */
class CCur18 : public CBaseCur {
public:
    CCur18(void*);
    virtual ~CCur18();
    u8 _pad18[2]; // 0x16 -> 0x18
};

class CCol6Hint : public CProcess {
public:
    CCol6Hint(void*);
    virtual ~CCol6Hint();
    void Init() override;
    void Term() override;
    void Move() override;
    void cbRenderBefore();

    /* 0x3C */ u8 _pad3C[0x74 - 0x3C];
    /* 0x74 */ UnkClass_8045F564 mMemRegion; // layout scratch region
    /* 0x84 */ u8 _pad84[0x124 - 0x84];
    /* 0x124 */ CCur18 mCur; // cursor widget
    /* 0x13C */ CScrollBar mScrollBar; // scroll bar widget
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


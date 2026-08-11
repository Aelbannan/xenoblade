#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void cbRenderBefore__12CUIErrMesWinFv();
extern "C" void __dt__12CUIErrMesWinFv();

// CSysWin helper imports (retail C-linkage; same declarations as CSaveLoad.hpp).
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CSysWin_isActive(void*);

// Sub-object embedded at CErrMesEntry +0x09, managed by the func_802B5148 /
// func_802B515C / func_802B58A4 family. Its first byte is an active flag;
// the rest of the layout is opaque to this TU.
struct CErrMesSub {
    u8 field_0;  // +0x00 - active flag
};

// Error-message record (func_802B48A0 / func_802B48B8 family).
struct CErrMesEntry {
    /* 0x00 */ u32 field_0;
    /* 0x04 */ u32 field_4;
    /* 0x08 */ u8 field_8;
    /* 0x09 */ CErrMesSub sub;  // embedded sub-object
};

class CUIErrMesWin {
public:
    virtual ~CUIErrMesWin();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    /* 0x04 */ u8 field_0x04[0x5C];  // +0x04..+0x60 - unknown region
    /* 0x60 */ CSysWin mSysWin;      // system message window
    /* 0x9C */ u8 field_0x9C;        // window state byte (2=ready, 4=active)
};

class CMenuTitle {
public:
    virtual ~CMenuTitle();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};

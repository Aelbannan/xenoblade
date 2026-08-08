#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void cbRenderBefore__12CUIErrMesWinFv();
extern "C" void __dt__12CUIErrMesWinFv();

class CUIErrMesWin {
public:
    virtual ~CUIErrMesWin();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
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


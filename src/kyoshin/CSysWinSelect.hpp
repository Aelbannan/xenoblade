#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void cbRenderBefore__13CSystemWindowFv(void* self);

class CSysWinSelect {
public:
    CSysWinSelect();
    virtual ~CSysWinSelect();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // TODO: add fields
};


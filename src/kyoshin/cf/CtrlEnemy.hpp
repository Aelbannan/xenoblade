#pragma once

#include <types.h>

namespace cf {

class CtrlBase {
public:
    virtual ~CtrlBase();
};

class CtrlEnemy {
public:
    virtual ~CtrlEnemy();

    // delegate helpers
    void delegateTo80089B24();
    void delegateTo80089E88();
    void delegateTo80089F68();

    // return-0 stubs
    int ret0_80088870();
    int ret0_80088878();
    int ret0_80088880();
    int ret0_80088888();
    int ret0_80088890();
    int ret0_80088898();
    int ret0_800888A0();
    int ret0_800888A8();
    int ret0_800888B0();
    int ret0_800888B8();
    int ret0_800888C0();
    int ret0_800888C8();
    int ret0_800888D0();
    int ret0_800888D8();
    int ret0_800888E0();
    int ret0_800888E8();

    // return-1 stubs
    int ret1_800888F4();
    int ret1_800888FC();

    // TODO: add fields
};
} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80089B24(void* self);
extern "C" void func_80089E88(void* self);
extern "C" void func_80089F68(void* self);


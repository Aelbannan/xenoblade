#pragma once

#include <types.h>

namespace cf {

class CfGimmick {
public:
    // Temporary: vtable placeholder
    u8 gap00[4];
    virtual ~CfGimmick();

    void func_8020896C(void* other);
    void func_80208988();
    int func_8020A8AC();
};

} // namespace cf

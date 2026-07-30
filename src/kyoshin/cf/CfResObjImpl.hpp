#pragma once

#include <types.h>

namespace cf {

struct CfResObjImpl {
    CfResObjImpl();
    ~CfResObjImpl();

    u8 _00[0x10];
};

} // namespace cf

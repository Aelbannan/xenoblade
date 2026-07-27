#pragma once

#include <types.h>

namespace cf {

class CfGimmick {
public:
    // Temporary: vtable placeholder
    u8 gap00[4];
    virtual ~CfGimmick();
};

} // namespace cf

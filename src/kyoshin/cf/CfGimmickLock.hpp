#pragma once

#include <types.h>

namespace cf {

class CfGimmickLock {
public:
    CfGimmickLock();
    virtual ~CfGimmickLock();

    u8 _00[0x20];
};

} // namespace cf
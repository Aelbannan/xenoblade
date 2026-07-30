#pragma once

#include <types.h>

namespace cf {

class CfGimmickSaveOff {
public:
    CfGimmickSaveOff();
    virtual ~CfGimmickSaveOff();

    u8 _00[0x20];
};

} // namespace cf
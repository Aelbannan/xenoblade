#pragma once

#include <types.h>

namespace cf {

class CfObjectImplNpc {
public:
    virtual ~CfObjectImplNpc();

    u8 _pad04[0x64];
    u8 mUnk_0x68[4];
};
} // namespace cf

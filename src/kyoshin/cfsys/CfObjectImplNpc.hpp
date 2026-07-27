#pragma once

#include <types.h>

namespace cf {

class CfObjectImplNpc {
public:
    virtual ~CfObjectImplNpc();

    u8* func_800CA924();
    void func_800CA938();
    void func_800CA940();

    u8 _pad04[0x64];
    u8 mUnk_0x68[4];
};
} // namespace cf

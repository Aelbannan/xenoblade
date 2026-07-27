#pragma once

#include <types.h>

namespace cf {

class CfObjectImplWalker {
public:
    virtual ~CfObjectImplWalker();

    void* func_800C22C4();
    u32 func_800C5970();

    // TODO: add fields
};
} // namespace cf


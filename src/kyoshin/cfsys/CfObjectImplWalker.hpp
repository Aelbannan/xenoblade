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

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_800C6EC0(void* self);
extern "C" void func_800CFFA0(void* self);


#pragma once

#include <types.h>

namespace cf {

class CfObjEnumList {
public:
    CfObjEnumList();
    virtual ~CfObjEnumList();

    void* func_800F6E98(int index);
    void* func_800F6EAC(unsigned long idx);
    void* func_800F6EC0(int index);

    // +0x00: vtable
    u8 _pad_04[0x20 - 0x04]; // 0x04-0x1F
    void* mPtrArray[1];       // 0x20+ - variable-length pointer array
};

} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_800F4798(void* self);

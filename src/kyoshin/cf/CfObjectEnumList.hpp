#pragma once

#include <types.h>

namespace cf {

class CfObjEnumList {
public:
    CfObjEnumList();
    virtual ~CfObjEnumList();

    // +0x00: vtable
    u8 _pad_04[0x20 - 0x04]; // 0x04-0x1F
    void* mPtrArray[1];       // 0x20+ — variable-length pointer array
};

} // namespace cf

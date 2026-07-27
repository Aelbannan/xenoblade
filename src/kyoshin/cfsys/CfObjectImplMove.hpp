#pragma once

#include <types.h>

namespace cf {

class CfObjectImplMove {
public:
    virtual ~CfObjectImplMove();

    void func_800CFFA0(unsigned int* param);
    void func_800CFFBC();
    void func_800CFFC4();
};

} // namespace cf

// Internal driver struct for CfObjectImplMove.
// Function func_800CD5C0 accesses a sub-object at +0x14 and reads a uint at +0xc4.
struct CfObjectImplMoveData {
    u8 _00_13[0x14];              // 0x00-0x13
    void* mSubObj;                // 0x14, sub-object with field at +0xc4

    void func_800CD5C0(unsigned int a, unsigned int b);
};

struct CfObjectImplMoveSubObj {
    u8 _00_C3[0xc4];              // 0x00-0xc3
    unsigned int mSomeId;         // 0xc4
};

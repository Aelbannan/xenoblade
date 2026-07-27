#pragma once

#include <types.h>

// Internal driver struct for CfObjectImplPc.
// Function func_800CA274 accesses a sub-object at +0x18 and reads/writes an int at +0x3e98.
struct CfObjectImplPcData {
    u8 _00_17[0x18];              // 0x00-0x17
    void* mSubObj;                // 0x18, sub-object with field at +0x3e98
};

struct CfObjectImplPcSubObj {
    u8 _00_3E97[0x3e98];          // 0x00-0x3e97
    int mSomeValue;               // 0x3e98
};

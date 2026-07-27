#pragma once

#include <types.h>

class CActParamData {
public:
    static void func_80057CDC();
    CActParamData();
    virtual ~CActParamData();

    // +0x00: vtable
    char _pad_04[0x260 - 0x04]; // 0x04-0x25F
    // Sub-object A at 0x260
    char mSubObj260[0x2D4 - 0x260]; // 0x260-0x2D3
    u32 mField2D4;                  // 0x2D4
    char _pad_2D8[0x2E0 - 0x2D8]; // 0x2D8-0x2DF
    // Sub-object B at 0x2E0
    char mSubObj2E0[0x2F4 - 0x2E0]; // 0x2E0-0x2F3
    void* mPtr2F4;                  // 0x2F4
    char _pad_2F8[0x354 - 0x2F8]; // 0x2F8-0x353
    u32 mField354;                  // 0x354
    char _pad_358[0x358 - 0x354 + 4]; // padding to cover ptr at 0x358
};

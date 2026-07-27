#pragma once

#include <types.h>

class CSortMenu {
public:
    CSortMenu();
    virtual ~CSortMenu();
    void OnFileEvent();
    u8 func_801D3320();
    u8 func_801D3328();
    void func_801D3518(int value);
    u8 func_801D37F4();
    u8 func_801D3808();
    u8 func_801D3810();

    u8 _00[0x28];
    u8 field_0x28;
    u8 _29[0x2B - 0x29];
    u8 field_0x2B;
    u8 _2C[0x6C - 0x2C];
    int mArray[32];    // 0x6C-0xEB (32 * 4 = 128 bytes)
    u8 field_0xEC;     // count / flag
    u8 field_0xED;
    u8 field_0xEE;
};

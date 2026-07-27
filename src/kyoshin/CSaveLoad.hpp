#pragma once

#include <types.h>

class CSLCur {
public:
    CSLCur();
    void func_8028EA74();

    // No vtable (no virtual functions)
    int mField0;    // 0x00
    int mField4;    // 0x04
    void* mField8;  // 0x08
    u8 mFieldC;     // 0x0C
    u8 mFieldD;     // 0x0D
    u8 mFieldE;     // 0x0E
};

class CSaveLoad {
public:
    CSaveLoad();
    virtual ~CSaveLoad();
    void func_8028F23C();
    void OnFileEvent();

    // +0x00: vtable
    char _pad_04[0x11E - 0x04]; // 0x04-0x11D
    u8 mField11E;                // 0x11E
    char _pad_11F[0x121 - 0x11F];// 0x11F-0x120
    u8 mField121;                // 0x121
    char _pad_122[0x123 - 0x122];// 0x122
    u8 mField123;                // 0x123
    char _pad_124[0x12A - 0x124];// 0x124-0x129
    u8 mField12A;                // 0x12A
};

namespace cf {

class CfAward {
public:
    virtual ~CfAward();
    void func_80291B18();

    // TODO: add fields
};
} // namespace cf

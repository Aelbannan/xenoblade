#pragma once

#include <types.h>

namespace cf {
class CActParamAnimGame;
}

// Sub-object struct within CModelDisp, stride 0xFF0
struct CModelDispSub {
    u8 _00[0x08];
    void* mpController;             // 0x08 - pointer to controller (vcalled)
    u8 _0C[0x08];
    u8 mSubObj[0x544];              // 0x14 - sub-object region (base for getNextChainObj)
    u8 mBuffer[0x4B4];              // 0x558 - buffer region (base for func_8004B9D4)
    void* mResultA;                 // 0xA0C - getNextChainObj result
    u8 _A10[0x84];
    u8 mBuffer2[0x4B4];             // 0xA94 - second buffer region (base for func_8004B9D4)
    void* mResultB;                 // 0xF48 - getNextChainObj result
    u8 _F4C[0x84];
    u32 mFlagFD0;                   // 0xFD0 - flag
    u32 mFlagFD4;                   // 0xFD4 - flag
    u8 _FD8[0x18];                  // padding to 0xFF0
};

class CModelDisp {
public:
    CModelDisp();
    virtual ~CModelDisp();

    u8 _pad0[0x2FD4];
    u8 field_2FD8;          // 0x2FD8
    u8 _pad2FD9[0x2FDC - 0x2FD9];
    f32 field_2FDC;         // 0x2FDC - display alpha/transition value
    f32 field_2FE0;         // 0x2FE0 - accumulator timer
    u8 field_2FE4;          // 0x2FE4
};


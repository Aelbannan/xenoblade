#pragma once

#include <types.h>

namespace cf {

class CtrlRemote {
public:
    virtual ~CtrlRemote();
};

class CtrlPad {
public:
    virtual ~CtrlPad();
};

class CtrlPc {
public:
    CtrlPc();
    virtual ~CtrlPc();

    // +0x00: vtable
    char _pad_04[0x2C - 0x04];   // 0x04-0x2B
    u32 mPadFlags;                // 0x2C (bit flags: bits 8-20 tested)
    char _pad_30[0x8C - 0x30];   // 0x30-0x8B
    char mSubObj8C[0x1D4 - 0x8C];// 0x8C-0x1D3 (delegation subobject)
    u32 mField1D4;                // 0x1D4
    char _pad_1D8[0x380 - 0x1D8];// 0x1D8-0x37F
    u32 mField380;                // 0x380
};

} // namespace cf

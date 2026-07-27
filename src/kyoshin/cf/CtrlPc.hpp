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

    // bit testers
    u32 testBit20();
    u32 testBit19();
    u32 testBit18();
    u32 testBit17();
    u32 testBit16();
    u32 testBit15();
    u32 testBit14();
    u32 testBit13();
    u32 testBit12();
    u32 testBit11();
    u32 testBit10();
    u32 testBit9();
    u32 testBit8();

    // setters/getters
    void setField380(u32 val);
    u32 getField1D4();

    // delegate helpers
    void delegateTo899C0();
    void delegateTo89B24();
    void delegateTo89E88();
    void delegateTo89F68();

    int returnTrue();

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

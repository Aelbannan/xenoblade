#pragma once

#include <types.h>

namespace cf {

class CREvtObj {
public:
    CREvtObj(int arg);
    virtual ~CREvtObj();

    void func_80185700();
    void func_80185748();

    // vtable (0x80532320)
    // +0x00: vtable ptr
    // +0x04: int field (from constructor arg)
    // +0x08: __ptmf (12 bytes = 3 x u32)
    // Total size: 0x14
    u32 mField04;
    u32 mPtmf[3];
};

} // namespace cf
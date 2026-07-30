#pragma once

#include <types.h>

namespace cf {

class CfGimmickSaveOff {
public:
    CfGimmickSaveOff(s32 param);
    virtual ~CfGimmickSaveOff();

    // CfGimmick base class fields (layout unknown) occupy 0x00-0x5F
    u8 _00[0x60];       // 0x00-0x5F: base class
    u32 mState;         // 0x60: jump table index
    u16 mParam;         // 0x64: saved constructor param
    u8 _66[6];          // 0x66-0x6B
    u16 mMinVal;        // 0x6C: lower bound
    u16 mMaxVal;        // 0x6E: upper bound
    u8 _70[0x12];       // 0x70-0x81
    u16 mFlag;          // 0x82: = 8
    u32 mType;          // 0x84: type (byte-extended)
};

} // namespace cf

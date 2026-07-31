#pragma once

#include <types.h>
#include "kyoshin/cf/object/CObjectState.hpp"

namespace cf {
    //min size: 0x38
    class CObjectParam : public CObjectState {
    public:
        virtual void CObjectParam_UnkVirtualFunc1(u32 a, u8 b); //0x3C
        virtual void* CObjectParam_UnkVirtualFunc2(); //0x40
        virtual int CObjectParam_UnkVirtualFunc3(); //0x44
        virtual void CObjectParam_UnkVirtualFunc4(); //0x48
        virtual BOOL CObjectParam_UnkVirtualFunc5(); //0x4C
        virtual void CObjectParam_UnkVirtualFunc6(); //0x50

        //0x0: vtable
        //0x0-10: CObjectState
        void* mPtr10;          // 0x10-0x13 (pointer stored at offset 0x10)
        u8 unk14[0x20 - 0x14]; // 0x14-0x2F
        u32 field_30;          // 0x30  — checked for non-zero by UnkVirtualFunc3
        u8  unk34[4];          // 0x34..0x37  (remainder of old unk10_3[0x28])
    };
}

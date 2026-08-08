#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectObj.hpp"

namespace cf {
    //size: 0x738
    class CfObjectTbox : public CfObjectObj {
    public:
        //0x0: vtable
        //0x0-718: CfObjectObj
        u8 _718[0x8];       // 0x718-0x71F
        u32 field_720;      // 0x720
        u8 _724[0x10];      // 0x724-0x733
        u32 field_734;      // 0x734
        u16 field_738;      // 0x738
        u8 _73A[2];         // 0x73A-0x73B
        u32 field_73C;      // 0x73C
    CfObjectTbox();
    virtual ~CfObjectTbox();
    void func_801F8DD0();
    };
}

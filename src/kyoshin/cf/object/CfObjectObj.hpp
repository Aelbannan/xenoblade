#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"

namespace cf {
    //size 0x718
    class CfObjectObj : public CfObjectMove {
    public:
        //0x0: vtable 1
        //0x0-718: CfObjectMove

        CfObjectObj();
        virtual ~CfObjectObj();

        void func_800BFAAC();
        int func_800BFA88();
        void func_800BFB90();

        //0x718-0x71B
        u8 _718[4];
        u16 field_71C;      //0x71C helper field
    };
}
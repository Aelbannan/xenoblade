#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"

namespace cf {
    //size: 0x2F50
    class CfObjectMap : public CfObjectModel {
    public:
        //0x0: vtable
        //0x0-BE: CfObjectModel
        u8 unkBE[0x2E92];
    void func_800B9A70();
    void func_800B9AB4();
    void func_800B9B78();
    void func_800B9E3C();
    void func_800BA610();
    void func_800BA66C();
    virtual ~CfObjectMap();
    };
}

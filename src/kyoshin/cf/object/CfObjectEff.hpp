#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"

namespace cf {
    //size: 0xC0
    class CfObjectEff : public CfObject {
    public:
        //0x0: vtable
        //0x0-70: CfObject
        u8 unk70[0x50];
    CfObjectEff();
    void func_800AC7CC();
    void func_800AC7FC();
    void func_800AC810();
    void func_800AC86C();
    void func_800AC990();
    void func_800ACA58();
    void func_800ACAE8();
    void func_800ACB08();
    void func_800ACBA4();
    void func_800ACBCC();
    void func_800ACC3C();
    void func_800ACC94();
    void func_800ACCD4();
    void func_800ACCE4();
    void func_800ACD4C();
    void func_800ACD5C();
    void func_800ACDA0();
    void func_800ACDE0();
    void func_800ACDFC();
    void func_800ACE60();
    void func_800ACE78();
    void func_800ACEAC();
    void func_800ACF50() const;
    void func_800AD3A4();
    void func_800AD4A4();
    void func_800AD68C();
    virtual ~CfObjectEff();
    void func_800AD818();
    void func_800AD830();
    void func_800AD840();
    };
}

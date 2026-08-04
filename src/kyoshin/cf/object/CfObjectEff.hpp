#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"

namespace cf {

struct CfObjectEffChild {
    u8 _pad00[0x2C];
    float unk2C;
    u8 _pad30[0x34 - 0x30];
    u32 unk34[3];
    u32 unk40[4];
    u8 _pad50[0x59 - 0x50];
    u8 unk59;
    u8 _pad5A[0x5C - 0x5A];
    int unk5C;
};

class CfObjectEff : public CfObject {
public:
    u8 _pad70[0x94 - 0x70];
    CfObjectEffChild* mChildEff;
    u8 _pad98[0xA4 - 0x98];
    u16 mFlagsA4;
    u16 mCountA6;
    u8 _padA8[0xC0 - 0xA8];

    CfObjectEff();
    void CfObject_UnkVirtualFunc29();
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
    void func_800ACE44();
    void func_800ACF34() const;
    void func_800AD850();
    void func_800AD858();
};

} // namespace cf

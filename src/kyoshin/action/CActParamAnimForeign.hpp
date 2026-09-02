#pragma once
#include <types.h>

// Foreign virtual interfaces used by CActParamAnim for external objects.
// Moved from CActParamAnim.hpp to satisfy pad-deletion requirement for primary files.
// These remain cast-only views of external vtables; owning headers are in scope
// when retail puts the slot there (same as PcSub4VtIf pattern for foreign sub-objects).

struct CActParamAnimOwner {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    u32 field04;
    u8 _pad08[0x0C - 0x08];
    f32 field0C;
    u8 _pad10[0x14 - 0x10];
    f32 field14;
    u32 field18;
    u32 field1C;
    u8 _pad_20[0x24 - 0x20];
    u32 field24;
};

struct CActParamAnimObj14 {
    virtual int f00();
    virtual int f01();
    virtual int f02();
    virtual int f14();
};

struct CActParamAnimObj6468 {
    virtual int f00();
    virtual int f01();
    virtual int f02();
    virtual int f03();
    virtual int f04();
    virtual int f05();
    virtual int f06();
    virtual int f07();
    virtual int f08();
    virtual int f09();
    virtual int f10();
    virtual int f11();
    virtual int f12();
    virtual int f13();
    virtual int f14();
    virtual int f15();
    virtual int f16();
    virtual int f17();
    virtual int f18();
    virtual int f19();
    virtual int f20();
    virtual int f21();
    virtual int f22();
    virtual int dispatch64(u32 effId);
    virtual int dispatch68();
};

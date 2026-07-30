#pragma once

#include <types.h>

class UnkClass_8047CA88 {
public:
    static UnkClass_8047CA88* getInstance();
    void func_8047CAA8();
    void func_8047CC4C();

    // Fields discovered from asm
    /* 0x00 */ u8 _00[4];         // vtable or padding
    /* 0x04 */ void* field_04;    // pointer to data
    /* 0x08 */ f32 field_08;      // scalar
    /* 0x0C */ f32 field_0C;      // paired with 0x10?
    /* 0x10 */ f32 field_10;      // paired with 0x0C?
    /* 0x14 */ f32 field_14;      // scalar (inv length)
    /* 0x18 */ f32 field_18;      // scalar
    /* 0x1C */ void* field_1C;    // pointer (child data)
    /* 0x20 */ void* field_20;    // pointer
    /* 0x24 */ void* field_24;    // pointer
};

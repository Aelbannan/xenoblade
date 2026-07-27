#pragma once

#include <types.h>

class UnkClass_800B0AD8 {
public:
    void clearCounters();
    u32 getCount();
    u32 getSize();
    void* getRingElem(u32 index);
};

class UnkClass_805764CC {
public:
    UnkClass_805764CC();
    virtual ~UnkClass_805764CC();

    u32 get_u32_18();
    void set_u32_00(u32 val);
    void fwdTo_2DB0(int param);
    u32 get_u32_04();
    u32 get_u32_74();
    u32 get_u32_70();
    void clear_380();
    void* getNull();
    u32 get_u32_380();
    u32 get_u32_98();
    u16 get_u16_45C0();
    void appendToBuffer(void* other);
    void* getPtr_1A8();
    void clear_700();
    void* getPtr_720();
    void maskField_6C(u32 mask, int enable);
    void set_u32_734(u32 val);
    void set_u8_73A(u8 a, u8 b);
    void set_u32_720(u32 val);
    void set_u16_738(u16 val);
    void set_float_730(float val);
    u32 get_u32_620();
    void fwdTo_99EC(int param);

    // TODO: add fields
};


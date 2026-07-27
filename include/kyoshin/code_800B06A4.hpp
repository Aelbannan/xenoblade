#ifndef KYOSHIN_CODE_800B06A4_HPP
#define KYOSHIN_CODE_800B06A4_HPP

#include <types.h>

namespace cf {
struct CfObject {
    char _pad[4];
};
struct TboxInfo {
    char _pad[4];
};
struct IFactoryEvent {
    char _pad[4];
};
}

namespace ml {
template <int N>
struct FixStr {
    char data[N];
    FixStr();
};
}

template <typename T>
class _reslist_base {
public:
    ~_reslist_base();
    char _pad[0x1F];
};

template <typename T>
class reslist : public _reslist_base<T> {
public:
    ~reslist();
    char _pad2[0x20 - sizeof(_reslist_base<T>)];
};

class UnkClass_800B0AD8 {
public:
    UnkClass_800B0AD8();
    void clearCounters();
    u32 getCount();
    u32 getSize();
    void* getRingElem(u32 index);
    u8 unk0[0xAF8];                // 0x00
    UnkClass_800B0AD8* unkAF8;     // 0xAF8
    u32 unkAFC;                     // 0xAFC
    u32 unkB00;                     // 0xB00
    u32 unkB04;                     // 0xB04
    // total size: 0xB08
};

class UnkClass_805764CC : public reslist<cf::CfObject> {
public:
    UnkClass_805764CC();
    ~UnkClass_805764CC() {}
    static UnkClass_805764CC* func_800B07E8();

    u32 get_u32_18();
    u32 get_u32_04();
    u32 get_u32_74();
    u32 get_u32_70();
    u32 get_u32_380();
    u32 get_u32_98();
    u32 get_u32_620();
    u16 get_u16_45C0();
    void appendToBuffer(void* other);
    void clear_380();
    void clear_700();
    void fwdTo_2DB0(int param);
    void fwdTo_99EC(int param);
    void maskField_6C(u32 mask, int enable);
    void set_float_730(float val);
    void set_u16_738(u16 val);
    void set_u32_00(u32 val);
    void set_u32_720(u32 val);
    void set_u32_734(u32 val);
    void set_u8_73A(u8 a, u8 b);
    void* getNull();
    void* getPtr_1A8();
    void* getPtr_720();
    
    // 0x20 - 0xB27
    UnkClass_800B0AD8 field_0x20;
    // 0xB28 - 0xB47
    reslist<cf::CfObject> field_0xB28;
    // 0xB48 - 0xB67
    reslist<cf::CfObject> field_0xB48;
    // 0xB68 - 0xB87
    reslist<cf::CfObject> field_0xB68;
    // 0xB88 - 0xBA7
    reslist<cf::CfObject> field_0xB88;
    // 0xBA8 - 0xBC7
    reslist<cf::CfObject> field_0xBA8;
    // 0xBC8 - 0xBE7
    reslist<cf::CfObject> field_0xBC8;
    // 0xBE8 - 0xC07
    reslist<cf::CfObject> field_0xBE8;
    // 0xC08 - 0xC27
    reslist<cf::CfObject> field_0xC08;
    // 0xC28 - 0xC47
    reslist<cf::CfObject> field_0xC28;
    // 0xC48 - 0xC7F
    reslist<cf::TboxInfo> field_0xC48;
    // 0xC80 - 0xC9F
    reslist<cf::IFactoryEvent*> field_0xC80;
    // 0xCA0 - 0xCAF
    u32 field_0xCA0;
    u32 field_0xCA4;
    u32 field_0xCA8;
    u32 field_0xCAC;
    // 0xCB0 - 0xCFB
    ml::FixStr<64> field_0xCB0;
    // 0xCFC - 0xD07
    u32 field_0xCFC;
    u32 field_0xD00;
    u32 field_0xD04;
    // 0xD08 - 0xD11
    u16 field_0xD0E;
    u16 field_0xD10;
};

#endif

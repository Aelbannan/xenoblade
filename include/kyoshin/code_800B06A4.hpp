#ifndef KYOSHIN_CODE_800B06A4_HPP
#define KYOSHIN_CODE_800B06A4_HPP
#include <types.h>
namespace cf {
struct CfObject { char _pad[4]; };
struct TboxInfo { char _pad[4]; };
struct IFactoryEvent { char _pad[4]; };
}
namespace ml {
template <int N>
struct FixStr {
    char mString[N];
    int mLength;
    u8 _pad[8];
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
    u8 unk0[0xAF8];
    UnkClass_800B0AD8* unkAF8;
    u32 unkAFC;
    u32 unkB00;
    u32 unkB04;
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
    UnkClass_800B0AD8 field_0x20;
    reslist<cf::CfObject> field_0xB28;
    reslist<cf::CfObject> field_0xB48;
    reslist<cf::CfObject> field_0xB68;
    reslist<cf::CfObject> field_0xB88;
    reslist<cf::CfObject> field_0xBA8;
    reslist<cf::CfObject> field_0xBC8;
    reslist<cf::CfObject> field_0xBE8;
    reslist<cf::CfObject> field_0xC08;
    reslist<cf::CfObject> field_0xC28;
    u8 field_0xC48[0x38];
    reslist<cf::IFactoryEvent*> field_0xC80;
    u32 field_0xCA0;
    u32 field_0xCA4;
    u32 field_0xCA8;
    u32 field_0xCAC;
    ml::FixStr<64> field_0xCB0;
    u32 field_0xCFC;
    u32 field_0xD00;
    u32 field_0xD04;
    u16 field_0xD0E;
    u16 field_0xD10;
    u8 _pad_D12_to_15EF[0x15F0 - 0xD12];
    u32 field_0x15F0;
};
#endif

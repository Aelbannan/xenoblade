#ifndef KYOSHIN_CODE_800B06A4_HPP
#define KYOSHIN_CODE_800B06A4_HPP
#include <types.h>
namespace cf {
struct CfObject { char _pad[4]; };
struct TboxInfo { char _pad[4]; };
struct IFactoryEvent { char _pad[4]; };
}

// Fixed-size pool node used by reslist<cf::IFactoryEvent*> (0xc bytes per entry).
// node[0]=next, [4]=prev, [8]=data pointer. An empty slot has next == 0.
struct CFactoryEventPoolNode {
    CFactoryEventPoolNode* next; // +0x0
    CFactoryEventPoolNode* prev; // +0x4
    cf::IFactoryEvent* data;     // +0x8
};

// Argument to func_800B4A24: an embedded sub-object (parent offset -0x3E9C).
// First word is a function table (callable entry 0x80 = vtable+0x200), and
// +0x64 holds status flags (bit 26 = 0x04000000 tested by the caller).
struct CEvtTypeArg;
typedef int (*CEvtTypeFn)(CEvtTypeArg*);
struct CEvtTypeArg {
    CEvtTypeFn* fnTable;    // +0x0
    u8 _pad04[0x64 - 0x4];
    u32 flags;              // +0x64 (bit 26 => 0x04000000)
};
namespace ml {
template <int N>
struct FixStr {
    char mString[N];
    int mLength;
    FixStr();
};
}
template <typename T>
class _reslist_base {
public:
    _reslist_base();
    ~_reslist_base();
    u32 field_0x00;        // 0x00: vtable pointer
    u32 field_0x04;        // 0x04: head pointer (this+8)
    u32 sentinel_prev;     // 0x08: sentinel node prev
    u32 sentinel_next;     // 0x0C: sentinel node next
    // total 0x10
};

// Generic reslist template - adds 0x10 bytes of padding (total 0x20)
template <typename T>
class reslist : public _reslist_base<T> {
public:
    ~reslist();
    u8 _pad_10[0x10];   // 0x10-0x1f
};

// Specialization for cf::CfObject - replaces padding with named fields (0x20)
template <>
class reslist<cf::CfObject> : public _reslist_base<cf::CfObject> {
public:
    reslist();
    ~reslist();
    u32 field_0x10;    // 0x10
    u32 field_0x14;    // 0x14
    u32 field_0x18;    // 0x18
    u8 field_0x1c;     // 0x1c
    u8 _pad_1d[3];     // 0x1d-0x1f
    // total 0x20
};

// Specialization for cf::TboxInfo - adds fields at 0x10-0x37 (total 0x38)
template <>
class reslist<cf::TboxInfo> : public _reslist_base<cf::TboxInfo> {
public:
    reslist();
    ~reslist();
    u8 _pad_10[0x1c];   // 0x10-0x2b
    u32 field_0x2c;     // 0x2c
    u32 field_0x30;     // 0x30
    u8 field_0x34;      // 0x34
    u8 _pad_35[3];      // 0x35-0x37
    // total 0x38
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
    u32 field_0xCF4;
    float field_0xCF8;
    u32 field_0xCFC;
    u32 field_0xD00;
    u32 field_0xD04;
    u16 field_0xD0E;
    u16 field_0xD10;
    u8 _pad_D12_to_15EF[0x15F0 - 0xD12];
    u32 field_0x15F0;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" unsigned long func_800B1C00();
extern "C" s32 func_800B1C0C(u32 mask);
extern "C" void func_800B6DD0(void* reslist, void* obj);
extern "C" void func_800B1B2C(void* obj);
extern "C" void func_800B2D88(void* self, void* arg);
extern "C" void func_800B2DB0(void* self, void* arg);
extern "C" void func_800B15A4(UnkClass_805764CC* self);
extern "C" void func_800B1E18(UnkClass_805764CC* self, void* arg);
extern "C" void func_800B7058(void* obj);
extern "C" void func_800B93D0(void* obj);
extern "C" void func_800B9404(void* obj);
extern "C" void* func_800B6EC0(UnkClass_805764CC* self, int arg);
extern "C" void func_800B1C24(int arg, void* ptr);
extern "C" void* func_800B39C0(void* arg);
extern "C" void func_800B68A8(void* self, void* a, void* b, int c, float f);
extern "C" void* func_800B708C__Fi(int arg);
extern "C" void* func_80193CD0(void* a, void* b);
extern "C" unsigned long func_80061FFC();
extern "C" void __dl__FPv(void*);
extern "C" void __dt__8047BDA8(void*);
extern "C" void func_800B0894(UnkClass_805764CC* self, unsigned long handle, unsigned long count);
extern "C" void func_800B4278(void* object, u32 arg);
extern "C" void func_800B42E8(void* object, u32 arg);
extern "C" unsigned long func_800B0FEC(void* self);
extern "C" void func_800B0FF4(void* self, unsigned long handle, unsigned long count);
extern "C" void* func_800B20B4(void* self, unsigned long arg1, void* arg2, unsigned long arg3);
extern "C" void func_800B084C(UnkClass_805764CC* self, unsigned long count);
#endif
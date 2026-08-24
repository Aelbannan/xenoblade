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

// Sentinal node stored inline in every reslist (0xc bytes: next/prev/item).
// Links are typed so chained derefs coalesce into one vreg (retail reuses
// the same register for the address temp and the loaded value).
struct CfReslistNode {
    CfReslistNode* mNext;  // +0x0
    CfReslistNode* mPrev;  // +0x4
    void* mItem;           // +0x8
};

// Mirror of the reslist<cf::IFactoryEvent*> pool list (0x20 bytes) walked by
// the flattened dtors (__dt__800B151C/__dt__800B18CC) and the pool-insert
// helpers (func_800B2DB0/func_800B39C8).
struct FactoryPoolList {
    void* mVtable;                // 0x00
    CfReslistNode* mStartNodePtr; // 0x04 inline sentinel node
    CfReslistNode mStartNode;     // 0x08 (0xc bytes)
    void* mList;                  // 0x14 pool array base
    u32 mCapacity;                // 0x18 entry count
    u8 field_0x1C;                // 0x1C owns-pool-array flag
};

// One 0xC-stride pool entry claimed by the insert helpers.
struct FactoryPoolEntry {
    void* next; // 0x00
    void* prev; // 0x04
    void* data; // 0x08 payload copied from the source's first word
};

// Flat mirror of the reslist<cf::TboxInfo> layout (0x38 bytes) used by its
// flattened constructor (struct-typed sentinel access reproduces the retail
// store scheduling; see __ct__reslist_cf_TboxInfo).
struct TboxInfoReslistLayout {
    void* mVtable;               // 0x00
    CfReslistNode* mStartNodePtr; // 0x04
    CfReslistNode mStartNode;     // 0x08 (0xc bytes)
    u8 _pad14[0x2c - 0x14];       // 0x14..0x2b
    u32 field_0x2c;               // 0x2c
    u32 field_0x30;               // 0x30
    u8 field_0x34;                // 0x34
};

// Argument to func_800B4A24: an embedded sub-object (parent offset -0x3E9C).
// First word is a real vtable (virtual slot 0x80 = +0x200 called by
// func_800B4A24), and +0x64 holds status flags (bit 2 = 0x4).
class CEvtTypeVt {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1A();
    virtual void unk1B();
    virtual void unk1C();
    virtual void unk1D();
    virtual void unk1E();
    virtual void unk1F();
    virtual void unk20();
    virtual void unk21();
    virtual void unk22();
    virtual void unk23();
    virtual void unk24();
    virtual void unk25();
    virtual void unk26();
    virtual void unk27();
    virtual void unk28();
    virtual void unk29();
    virtual void unk2A();
    virtual void unk2B();
    virtual void unk2C();
    virtual void unk2D();
    virtual void unk2E();
    virtual void unk2F();
    virtual void unk30();
    virtual void unk31();
    virtual void unk32();
    virtual void unk33();
    virtual void unk34();
    virtual void unk35();
    virtual void unk36();
    virtual void unk37();
    virtual void unk38();
    virtual void unk39();
    virtual void unk3A();
    virtual void unk3B();
    virtual void unk3C();
    virtual void unk3D();
    virtual void unk3E();
    virtual void unk3F();
    virtual void unk40();
    virtual void unk41();
    virtual void unk42();
    virtual void unk43();
    virtual void unk44();
    virtual void unk45();
    virtual void unk46();
    virtual void unk47();
    virtual void unk48();
    virtual void unk49();
    virtual void unk4A();
    virtual void unk4B();
    virtual void unk4C();
    virtual void unk4D();
    virtual void unk4E();
    virtual void unk4F();
    virtual void unk50();
    virtual void unk51();
    virtual void unk52();
    virtual void unk53();
    virtual void unk54();
    virtual void unk55();
    virtual void unk56();
    virtual void unk57();
    virtual void unk58();
    virtual void unk59();
    virtual void unk5A();
    virtual void unk5B();
    virtual void unk5C();
    virtual void unk5D();
    virtual void unk5E();
    virtual void unk5F();
    virtual void unk60();
    virtual void unk61();
    virtual void unk62();
    virtual void unk63();
    virtual void unk64();
    virtual void unk65();
    virtual void unk66();
    virtual void unk67();
    virtual void unk68();
    virtual void unk69();
    virtual void unk6A();
    virtual void unk6B();
    virtual void unk6C();
    virtual void unk6D();
    virtual void unk6E();
    virtual void unk6F();
    virtual void unk70();
    virtual void unk71();
    virtual void unk72();
    virtual void unk73();
    virtual void unk74();
    virtual void unk75();
    virtual void unk76();
    virtual void unk77();
    virtual void unk78();
    virtual void unk79();
    virtual void unk7A();
    virtual void unk7B();
    virtual void unk7C();
    virtual void unk7D();
    virtual s32 unk7E();   // func_800B4A24 dispatches here (vtable +0x200)
    virtual void unk7F();
    virtual s32 unk80();
};

struct CEvtTypeArg : public CEvtTypeVt {
    u8 _pad04[0x64 - 0x4];
    u32 flags;              // +0x64 (bit 2 => 0x4)
};

// --- func_800B4CA0 support ---
// Battle/HUD controller walked by func_800B4CA0: one shared vtable serves two
// dispatch sites (slot 0x46 at +0x118 takes a float; slot 0xAF at +0x2BC
// returns a status int), plus plain fields at 0x3F00/0x3F08/0x3F60.
class IDispB4CA0 {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1A();
    virtual void unk1B();
    virtual void unk1C();
    virtual void unk1D();
    virtual void unk1E();
    virtual void unk1F();
    virtual void unk20();
    virtual void unk21();
    virtual void unk22();
    virtual void unk23();
    virtual void unk24();
    virtual void unk25();
    virtual void unk26();
    virtual void unk27();
    virtual void unk28();
    virtual void unk29();
    virtual void unk2A();
    virtual void unk2B();
    virtual void unk2C();
    virtual void unk2D();
    virtual void unk2E();
    virtual void unk2F();
    virtual void unk30();
    virtual void unk31();
    virtual void unk32();
    virtual void unk33();
    virtual void unk34();
    virtual void unk35();
    virtual void unk36();
    virtual void unk37();
    virtual void unk38();
    virtual void unk39();
    virtual void unk3A();
    virtual void unk3B();
    virtual void unk3C();
    virtual void unk3D();
    virtual void unk3E();
    virtual void unk3F();
    virtual void unk40();
    virtual void unk41();
    virtual void unk42();
    virtual void unk43();
    virtual void unk44();
    virtual void unk45();
    virtual void unk46(float val);   // vtable +0x118
    virtual void unk47();
    virtual void unk48();
    virtual void unk49();
    virtual void unk4A();
    virtual void unk4B();
    virtual void unk4C();
    virtual void unk4D();
    virtual void unk4E();
    virtual void unk4F();
    virtual void unk50();
    virtual void unk51();
    virtual void unk52();
    virtual void unk53();
    virtual void unk54();
    virtual void unk55();
    virtual void unk56();
    virtual void unk57();
    virtual void unk58();
    virtual void unk59();
    virtual void unk5A();
    virtual void unk5B();
    virtual void unk5C();
    virtual void unk5D();
    virtual void unk5E();
    virtual void unk5F();
    virtual void unk60();
    virtual void unk61();
    virtual void unk62();
    virtual void unk63();
    virtual void unk64();
    virtual void unk65();
    virtual void unk66();
    virtual void unk67();
    virtual void unk68();
    virtual void unk69();
    virtual void unk6A();
    virtual void unk6B();
    virtual void unk6C();
    virtual void unk6D();
    virtual void unk6E();
    virtual void unk6F();
    virtual void unk70();
    virtual void unk71();
    virtual void unk72();
    virtual void unk73();
    virtual void unk74();
    virtual void unk75();
    virtual void unk76();
    virtual void unk77();
    virtual void unk78();
    virtual void unk79();
    virtual void unk7A();
    virtual void unk7B();
    virtual void unk7C();
    virtual void unk7D();
    virtual void unk7E();
    virtual void unk7F();
    virtual void unk80();
    virtual void unk81();
    virtual void unk82();
    virtual void unk83();
    virtual void unk84();
    virtual void unk85();
    virtual void unk86();
    virtual void unk87();
    virtual void unk88();
    virtual void unk89();
    virtual void unk8A();
    virtual void unk8B();
    virtual void unk8C();
    virtual void unk8D();
    virtual void unk8E();
    virtual void unk8F();
    virtual void unk90();
    virtual void unk91();
    virtual void unk92();
    virtual void unk93();
    virtual void unk94();
    virtual void unk95();
    virtual void unk96();
    virtual void unk97();
    virtual void unk98();
    virtual void unk99();
    virtual void unk9A();
    virtual void unk9B();
    virtual void unk9C();
    virtual void unk9D();
    virtual void unk9E();
    virtual void unk9F();
    virtual void unkA0();
    virtual void unkA1();
    virtual void unkA2();
    virtual void unkA3();
    virtual void unkA4();
    virtual void unkA5();
    virtual void unkA6();
    virtual void unkA7();
    virtual void unkA8();
    virtual void unkA9();
    virtual void unkAA();
    virtual void unkAB();
    virtual void unkAC();
    virtual void unkAD();
    virtual void unkAE();
    virtual s32 unkAF();             // vtable +0x2BC
};

// Object layout walked by func_800B4CA0. Standalone class (v2).
// Slots 0x00-0xAF mirror the shared dispatch table: slot 0x46 at +0x118 takes
// a float; slot 0xAF at +0x2BC returns a status int.
class Func4CA0Obj {
public:
    /* Slot map: retail tests slot 0x46 (+0x118, float arg) and slot 0xAF
     * (+0x2BC). MWCC emits each entry two slots higher than declared for
     * this class shape, so two leading stubs are omitted to compensate. */
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1A();
    virtual void unk1B();
    virtual void unk1C();
    virtual void unk1D();
    virtual void unk1E();
    virtual void unk1F();
    virtual void unk20();
    virtual void unk21();
    virtual void unk22();
    virtual void unk23();
    virtual void unk24();
    virtual void unk25();
    virtual void unk26();
    virtual void unk27();
    virtual void unk28();
    virtual void unk29();
    virtual void unk2A();
    virtual void unk2B();
    virtual void unk2C();
    virtual void unk2D();
    virtual void unk2E();
    virtual void unk2F();
    virtual void unk30();
    virtual void unk31();
    virtual void unk32();
    virtual void unk33();
    virtual void unk34();
    virtual void unk35();
    virtual void unk36();
    virtual void unk37();
    virtual void unk38();
    virtual void unk39();
    virtual void unk3A();
    virtual void unk3B();
    virtual void unk3C();
    virtual void unk3D();
    virtual void unk3E();
    virtual void unk3F();
    virtual void unk40();
    virtual void unk41();
    virtual void unk42();
    virtual void unk43();
    virtual void unk44();
    virtual void unk45();
    virtual void unk46(float val);   // vtable +0x118
    virtual void unk47();
    virtual void unk48();
    virtual void unk49();
    virtual void unk4A();
    virtual void unk4B();
    virtual void unk4C();
    virtual void unk4D();
    virtual void unk4E();
    virtual void unk4F();
    virtual void unk50();
    virtual void unk51();
    virtual void unk52();
    virtual void unk53();
    virtual void unk54();
    virtual void unk55();
    virtual void unk56();
    virtual void unk57();
    virtual void unk58();
    virtual void unk59();
    virtual void unk5A();
    virtual void unk5B();
    virtual void unk5C();
    virtual void unk5D();
    virtual void unk5E();
    virtual void unk5F();
    virtual void unk60();
    virtual void unk61();
    virtual void unk62();
    virtual void unk63();
    virtual void unk64();
    virtual void unk65();
    virtual void unk66();
    virtual void unk67();
    virtual void unk68();
    virtual void unk69();
    virtual void unk6A();
    virtual void unk6B();
    virtual void unk6C();
    virtual void unk6D();
    virtual void unk6E();
    virtual void unk6F();
    virtual void unk70();
    virtual void unk71();
    virtual void unk72();
    virtual void unk73();
    virtual void unk74();
    virtual void unk75();
    virtual void unk76();
    virtual void unk77();
    virtual void unk78();
    virtual void unk79();
    virtual void unk7A();
    virtual void unk7B();
    virtual void unk7C();
    virtual void unk7D();
    virtual void unk7E();
    virtual void unk7F();
    virtual void unk80();
    virtual void unk81();
    virtual void unk82();
    virtual void unk83();
    virtual void unk84();
    virtual void unk85();
    virtual void unk86();
    virtual void unk87();
    virtual void unk88();
    virtual void unk89();
    virtual void unk8A();
    virtual void unk8B();
    virtual void unk8C();
    virtual void unk8D();
    virtual void unk8E();
    virtual void unk8F();
    virtual void unk90();
    virtual void unk91();
    virtual void unk92();
    virtual void unk93();
    virtual void unk94();
    virtual void unk95();
    virtual void unk96();
    virtual void unk97();
    virtual void unk98();
    virtual void unk99();
    virtual void unk9A();
    virtual void unk9B();
    virtual void unk9C();
    virtual void unk9D();
    virtual void unk9E();
    virtual void unk9F();
    virtual void unkA0();
    virtual void unkA1();
    virtual void unkA2();
    virtual void unkA3();
    virtual void unkA4();
    virtual void unkA5();
    virtual void unkA6();
    virtual void unkA7();
    virtual void unkA8();
    virtual void unkA9();
    virtual void unkAA();
    virtual void unkAB();
    virtual void unkAC();
    virtual void unkAD();
    virtual void unkAE();
    virtual s32 unkAF();             // vtable +0x2BC
    u8 _pad04[0x3F00 - 0x4];
    u32 field_3F00;                  // +0x3F00 (bit 27 tested)
    u8 _pad08[0x3F08 - 0x3F04];
    u32 field_3F08;                  // +0x3F08 (bit 27 tested)
    u8 _pad0C[0x3F60 - 0x3F0C];
    void* field_3F60;                // +0x3F60 -> sub-record
};

// Sub-record reached through Func4CA0Obj::field_3F60.
struct Func4CA0Sub {
    u8 _pad00[0x4EC];
    u32 field_4EC;                   // bit 18 tested ((v >> 18) & 1)
};

// Object walked by func_800B8B94 / func_800B8C78 (u16 id at +0x8C).
struct B8B94Obj {
    u8 _pad00[0x8C];
    u16 field_8C;
};

// NOTE: the reslist member destructors are invoked through proper member
// syntax in ~UnkClass_805764CC so MWCC emits the exact retail flat symbols:
//   reslist<cf::IFactoryEvent*> -> __dt__30reslist<PQ22cf13IFactoryEvent>Fv (local)
//   reslist<cf::TboxInfo>       -> __dt__23reslist<Q22cf8TboxInfo>Fv (local)
//   reslist<cf::CfObject*>      -> __dt__24reslist<PQ22cf8CfObject>Fv
//                                  (import; defined in CTaskGame.cpp)
class UnkClass_800B0AD8;
extern "C" UnkClass_800B0AD8* __dt__800B0AF4(void* self, int flags);
extern "C" void* func_800B76A4();      // head of the B28 object list
extern "C" void* func_800B76CC();      // head of the B48 object list
extern "C" void* func_8016FE34(void* source);
extern "C" int func_800DA06C(void* bm, void* obj);
extern "C" void* getInstance__Q22cf14CBattleManagerFv();
#include "monolib/util/FixStr.hpp"
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

// Generic reslist template - adds 0x10 bytes of padding (total 0x20). No ctor
// is declared: the retail reslist ctors are flattened free functions
// (__ct__reslist_cf_*), and member-ctor mangling would emit __ct__23reslist<..>
// (wrong reloc names), so the members must not auto-construct.
template <typename T>
class reslist : public _reslist_base<T> {
public:
    ~reslist();
    u32 field_0x10;    // 0x10
    u32 field_0x14;    // 0x14
    u32 field_0x18;    // 0x18
    u8 field_0x1c;     // 0x1c
    u8 _pad_1d[3];     // 0x1d-0x1f
};

// Specialization for cf::CfObject - replaces padding with named fields (0x20)
template <>
class reslist<cf::CfObject> : public _reslist_base<cf::CfObject> {
public:
    ~reslist();
    u32 field_0x10;    // 0x10
    u32 field_0x14;    // 0x14
    u32 field_0x18;    // 0x18
    u8 field_0x1c;     // 0x1c
    u8 _pad_1d[3];     // 0x1d-0x1f
    // total 0x20
};

// Specialization for cf::TboxInfo - adds fields at 0x10-0x37 (total 0x38).
// NOT derived from _reslist_base: deriving makes MWCC emit a call to the base
// dtor instead of the standalone body retail uses.
template <>
class reslist<cf::TboxInfo> {
public:
    ~reslist();
    void* mVtbl;            // 0x00
    CfReslistNode* mStartNodePtr; // 0x04
    CfReslistNode mStartNode;     // 0x08 (0xc bytes)
    u8 _pad_14[0x2c - 0x14];   // 0x14-0x2b
    u32 field_0x2c;     // 0x2c
    u32 field_0x30;     // 0x30
    u8 field_0x34;      // 0x34
    u8 _pad_35[3];      // 0x35-0x37
    // total 0x38
};

// Specialization for cf::IFactoryEvent* - standalone (NOT derived from
// _reslist_base, mirroring the cf::TboxInfo trick): deriving makes the
// derived dtor emit a trailing ~_reslist_base call + delete tail, which
// retail's __dt__30reslist<PQ22cf13IFactoryEvent>Fv copy does not have.
template <>
class reslist<cf::IFactoryEvent*> {
public:
    ~reslist();
    void* mVtbl;                  // 0x00
    CfReslistNode* mStartNodePtr; // 0x04
    CfReslistNode mStartNode;     // 0x08 (0xc bytes)
    u32 field_0x14;               // 0x14 pool array base
    u32 field_0x18;               // 0x18 entry count
    u8 field_0x1c;                // 0x1c owns-pool-array flag
    u8 _pad_1d[3];                // 0x1d-0x1f
    // total 0x20
};
// Field accessor view over a CfObject payload walked by the reslist search
// helpers (func_800B4278 / func_800B42E8).
struct CfObjFieldView {
    u8 _pad00[0x64];
    u32 field_0x64;   // mask bits tested by func_800B4278
    u8 _pad68[0x94 - 0x68];
    s32 field_0x94;   // type id compared by func_800B42E8
    u8 _pad98[0x9C - 0x98];
    u32 field_0x9C;
};

// Virtual-dispatch interfaces matching the retail vtable shapes:
// IDispB1F6C: callback objects with the target method at slot 2.
// IDispB79A4: iteration nodes with the target method at slot 0x67.
class IDispB1F6C {
public:
    virtual void unk02();
};
class IDispB79A4 {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1A();
    virtual void unk1B();
    virtual void unk1C();
    virtual void unk1D();
    virtual void unk1E();
    virtual void unk1F();
    virtual void unk20();
    virtual void unk21();
    virtual void unk22();
    virtual void unk23();
    virtual void unk24();
    virtual void unk25();
    virtual void unk26();
    virtual void unk27();
    virtual void unk28();
    virtual void unk29();
    virtual void unk2A();
    virtual void unk2B();
    virtual void unk2C();
    virtual void unk2D();
    virtual void unk2E();
    virtual void unk2F();
    virtual void unk30();
    virtual void unk31();
    virtual void unk32();
    virtual void unk33();
    virtual void unk34();
    virtual void unk35();
    virtual void unk36();
    virtual void unk37();
    virtual void unk38();
    virtual void unk39();
    virtual void unk3A();
    virtual void unk3B();
    virtual void unk3C();
    virtual void unk3D();
    virtual void unk3E();
    virtual void unk3F();
    virtual void unk40();
    virtual void unk41();
    virtual void unk42();
    virtual void unk43();
    virtual void unk44();
    virtual void unk45();
    virtual void unk46();
    virtual void unk47();
    virtual void unk48();
    virtual void unk49();
    virtual void unk4A();
    virtual void unk4B();
    virtual void unk4C();
    virtual void unk4D();
    virtual void unk4E();
    virtual void unk4F();
    virtual void unk50();
    virtual void unk51();
    virtual void unk52();
    virtual void unk53();
    virtual void unk54();
    virtual void unk55();
    virtual void unk56();
    virtual void unk57();
    virtual void unk58();
    virtual void unk59();
    virtual void unk5A();
    virtual void unk5B();
    virtual void unk5C();
    virtual void unk5D();
    virtual void unk5E();
    virtual void unk5F();
    virtual void unk60();
    virtual void unk61();
    virtual void unk62();
    virtual void unk63();
    virtual void unk64();
    virtual void unk67(void* arg); // +0x19C
};

class UnkClass_800B0AD8 {
public:
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
class AD8Wrapper : public UnkClass_800B0AD8 {
public:
    ~AD8Wrapper();  // defined out-of-line in code_800B06A4.cpp after
                    // ~UnkClass_805764CC (auto_inline off): forwards to the
                    // flat __dt__800B0AF4 teardown
};
class UnkClass_805764CC : public reslist<cf::CfObject*> {
public:
    UnkClass_805764CC();
    ~UnkClass_805764CC();
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
    /* 0x20 - AD8 teardown wrapper; its dtor forwards to flat __dt__800B0AF4 */
    AD8Wrapper field_0x20;
    // Retail element type is cf::CfObject* (PQ22cf8CfObject); the pointer
    // specialization keeps the flat dtor symbol importable.
    reslist<cf::CfObject*> field_0xB28;
    reslist<cf::CfObject*> field_0xB48;
    reslist<cf::CfObject*> field_0xB68;
    reslist<cf::CfObject*> field_0xB88;
    reslist<cf::CfObject*> field_0xBA8;
    reslist<cf::CfObject*> field_0xBC8;
    reslist<cf::CfObject*> field_0xBE8;
    reslist<cf::CfObject*> field_0xC08;
    reslist<cf::CfObject*> field_0xC28;
    reslist<cf::TboxInfo> field_0xC48;      // 0xC48 (0x38 bytes)
    reslist<cf::IFactoryEvent*> field_0xC80;
    u32 field_0xCA0;
    u32 field_0xCA4;
    u32 field_0xCA8;
    u32 field_0xCAC;
    // raw 0x44 buffer for the FixStr<64> at 0xCB0; kept as bytes so the ctor
    // can placement-new it in body order (a real FixStr member would
    // auto-construct before the manual reslist ctor calls, breaking retail order)
    u8 field_0xCB0[0x44];
    u32 field_0xCF4;
    float field_0xCF8;
    u32 field_0xCFC;
    u32 field_0xD00;
    u32 field_0xD04;
    u8 _pad_D08[6];     // 0xD08..0xD0E (unknown fields)
    u16 field_0xD0E;    // 0xD0E
    u16 field_0xD10;    // 0xD10
    u8 _pad_D12_to_15EF[0x15F0 - 0xD12];   // 0xD12 .. 0x15F0
    u32 field_0x15F0;   // exactly 0x15F0
};

// --- func_800B7A18 support: circular object-list iteration ---
// Mirror of retail UnkF8C0Node/UnkF8C0Source (CfGameManager.cpp). The
// iterator node holds the current ItemListNode pointer; the list header is
// the shared { +0x00 unused, +0x04 head-sentinel } shape used by
// CfGimmickList / CFloorMapObjList / ItemListManager alike.
struct F8C0IteratorNode {
    u32 field_0x0;                // +0x00 current list node
};
struct F8C0ListSource {
    u32 field_0x0;                // +0x00
    F8C0IteratorNode* field_0x4;  // +0x04 head sentinel
};

// CfGameManager item-list traversal primitives (retail mangled names).
// The retail symbols are C-ABI-named (extern "C" suppresses MWCC's C++
// param mangling, which would append __FP.. to the identifier).
extern "C" void func_8007F8C0__Q22cf13CfGameManagerFv(F8C0IteratorNode* destination, const F8C0ListSource* source);
extern "C" void** func_8007F8D0__Q22cf13CfGameManagerFv(F8C0IteratorNode* iterator);
extern "C" void func_8007F8DC__Q22cf13CfGameManagerFv(F8C0IteratorNode* destination, F8C0IteratorNode* source, u32 unused);
extern "C" void func_8007F8F4__Q22cf13CfGameManagerFv(F8C0IteratorNode* destination, const F8C0ListSource* source);
extern "C" bool func_8007F900__Q22cf13CfGameManagerFv(const u32* first, const u32* second);
extern "C" void* func_800B182C(void* node);
extern "C" u16 func_8007F8B8__Q22cf13CfGameManagerFv(void* object);

// Imports used by func_800B70FC / func_800B7214 (retail symbol names).
extern "C" void func_800B71C4();
extern "C" void* func_800B6BA4__Fv();
extern "C" void func_800BFDE0(void* obj, u32 flag);

// Payload record stored inside each 0x24-byte pool slot of the TboxInfo
// reslist (copied wholesale by func_800B7214).
struct TboxPayload {   // 0x1C bytes
    u32 field_00;
    float field_04;
    float field_08;
    float field_0C;
    float field_10;
    u32 field_14;
    u16 field_18;
    u8 field_1A;
    u8 field_1B;
};

// One free-list pool slot: two link pointers followed by the payload.
struct TboxPoolSlot {  // 0x24 bytes
    CfReslistNode* mLink0;  // +0x00
    CfReslistNode* mLink1;  // +0x04
    TboxPayload mInfo;      // +0x08
};

// Slot-array view over the TboxInfo reslist used by func_800B7214.
struct TboxInfoReslistPoolView {
    void* mVtable;                // 0x00
    CfReslistNode* mStartNodePtr; // 0x04
    CfReslistNode mStartNode;     // 0x08
    u8 _pad14[0x2c - 0x14];
    TboxPoolSlot* mSlots;         // 0x2c pool array base
    s32 mSlotCount;               // 0x30
    u8 field_0x34;
};

// Selection-sort entry: 8-byte {pointer, sort-value} pair.
struct SortEntry {
    void* mPointer;  // +0x00
    float mValue;    // +0x04
};
typedef int (*SortEntryCompare)(SortEntry*, SortEntry*);

// --- batch targets: introsort family / ring buffer (us-800b58a8 etc.) ---
extern "C" void func_800B535C(void** first, void** last, void** cmp);
extern "C" void func_800B587C(void** first, void** last, void** cmp);
extern "C" void func_800B570C(void** a, void** b, void** c, void** cmp);
extern "C" s32 func_800B3D64(UnkClass_800B0AD8* self, cf::CfObject** itemPtr);

// Pivot-jitter statics rotated by the sort drivers (retail .data).
extern int lbl_eu_80661CD8;
extern int lbl_eu_80661CDC;

struct B47Vec3;

// Globals used by func_800B8FC4.
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;

// Float/double pool constants shared by the cf object scan routines.
extern const float lbl_eu_806669D8;
extern const float lbl_eu_806669EC;
extern const float lbl_eu_806669DC;
extern const float lbl_eu_806669E0;
extern const float lbl_eu_806669E4;
extern const float lbl_eu_806669F4;
extern const float lbl_eu_806669F8;
extern const float lbl_eu_806669FC;
extern const float lbl_eu_80666A00;
extern const float lbl_eu_80666A04;
extern const float lbl_eu_80666A0C;
extern const float lbl_eu_80666A14;
extern const float lbl_eu_80666A18;
extern const double lbl_eu_80666A20;

// ml::CVec3 static zero (retail symbol zero__Q22ml5CVec3).
namespace ml {
class CVec3 {
public:
    float x, y, z;
    static CVec3 zero;
};
} // namespace ml

// CfGameManager imports (retail mangled names).
extern "C" void* getInstance__Q22cf13CfGameManagerFv();
extern "C" bool func_8008585C__Q22cf13CfGameManagerFv(void* gm);
extern "C" bool func_80085840__Q22cf13CfGameManagerFv(void* gm);
extern "C" u32 func_80082FE4__Q22cf13CfGameManagerFv(void* gm);
extern "C" void func_8008360C__Q22cf13CfGameManagerFv(void* gm);
extern "C" void func_8007F9B4__Q22cf13CfGameManagerFv(void* gm);
extern "C" void func_80084C10__Q22cf13CfGameManagerFv(void* gm);
extern "C" void func_80083DEC__Q22cf13CfGameManagerFv(void* gm, void* a, s32 b,
                                                          B47Vec3* c, const B47Vec3* d,
                                                          u16 e, u16 f, u16 g, u16 h);
extern "C" void* func_800B6C58();
extern "C" int func_800AB580(void* obj, void* arg, int flag, float range);
extern "C" void* getInstance__Q22cf14CBattleManagerFv();
extern const float lbl_eu_80666A28;
extern const float lbl_eu_80666A2C;
extern const float lbl_eu_80666A30;

// View of a ring-buffer element's sort key (offset 0x74).
struct RingSortKeyView {
    u8 _pad00[0x74];
    u32 sortKey;   // +0x74
};

// Object record walked by func_800B7AF0 (field view).
struct B7AF0Obj {
    u8 _pad00[0x64];
    u32 flags64;                 // +0x64 (bit17 tested)
    u32 flags68;                 // +0x68 (bits 13/15 tested)
    u8 _pad6C[0x94 - 0x6C];
    s32 type94;                  // +0x94 type id
    u8 _pad98[0x9C - 0x98];
    u32 field9C;                 // +0x9C (low halfword tested)
    u8 _padA0[0x148 - 0xA0];
    float field148;              // +0x148
    u8 _pad14C[0x15A - 0x14C];
    u16 h15A;                    // +0x15A
    u16 h15C;                    // +0x15C
    u16 h15E;                    // +0x15E
    u16 h160;                    // +0x160
};

// Payload record whose first word feeds the 0x803-byte refresh copy.
class IB7ArgChild {
public:
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void p09();
    virtual void p0A();
    virtual void p0B();
    virtual void* p0C();       // +0x30
};

// Second argument of func_800B7AF0: readiness probe at slot 29 (+0x74),
// position getter at slot 42 (+0xA8).
class IB7Arg : public IB7ArgChild {
public:
    virtual void p0D();
    virtual void p0E();
    virtual void p0F();
    virtual void p10();
    virtual void p11();
    virtual void p12();
    virtual void p13();
    virtual void p14();
    virtual void p15();
    virtual void p16();
    virtual void p17();
    virtual void p18();
    virtual void p19();
    virtual void p1A();
    virtual void p1B();
    virtual void p1C();
    virtual bool p1D();        // +0x74
    virtual void p1E();
    virtual void p1F();
    virtual void p20();
    virtual void p21();
    virtual void p22();
    virtual void p23();
    virtual void p24();
    virtual void p25();
    virtual void p26();
    virtual void p27();
    virtual void p28();
    virtual B47Vec3* unk2A();  // +0xA8
};

// Same object viewed at slot 22 (+0x58).
class IB20B4Vt58 {
public:
    virtual void q00();
    virtual void q01();
    virtual void q02();
    virtual void q03();
    virtual void q04();
    virtual void q05();
    virtual void q06();
    virtual void q07();
    virtual void q08();
    virtual void q09();
    virtual void q0A();
    virtual void q0B();
    virtual void q0C();
    virtual void q0D();
    virtual void q0E();
    virtual void q0F();
    virtual void q10();
    virtual void q11();
    virtual void q12();
    virtual void q13();
    virtual void q14();
    virtual void unk22(u32 arg);               // +0x58
};

// Factory-event queue entry: notification at slot 3 (+0xC).
class IB20B4Event {
public:
    virtual void e00();
    virtual void e01();
    virtual void e02();
    virtual void e03(void* obj);               // +0xC
};

// Payload record consumed by func_800B20B4.
struct B20B4Payload {
    u32 w00;   // +0x00
    u32 w04;   // +0x04
    u32 w08;   // +0x08
    u32 w0C;   // +0x0C
    u32 w10;   // +0x10
    u32 w14;   // +0x14
    u32 w18;   // +0x18
    u16 h1C;   // +0x1C
    u16 h1E;   // +0x1E
    u16 h20;   // +0x20
    u16 h22;   // +0x22
    u16 h24;   // +0x24 npc/pc selector
};

// Imports used by func_800B20B4 (retail symbol names).
extern "C" void* func_80061FE8();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(unsigned long size, unsigned long heap);
extern "C" void __ct__Q22cf13CfObjectModelFv(void* self);
extern "C" void __ct__Q22cf11CfObjectNpcFv(void* self, int flag);
extern "C" void __ct__Q22cf10CfObjectPcFv(void* self);
extern "C" void func_800BE948(void* obj, u16 id);
extern "C" void func_800BE960(void* obj, s32 a);
extern "C" void func_800BE978(void* obj, s32 a);
extern "C" s32 func_800822F4__Q22cf13CfGameManagerFv();
extern "C" s32 func_80063560(s32 kind, s32 a, s32 b);
extern "C" s32 func_8006398C(s32 a);
extern "C" u32 func_80063310(void* a);
extern "C" s32 func_80063394(void* a);
extern "C" s32 func_8006846C(void* res, s32 id);
extern "C" void* func_80062C88(s32 id);
extern "C" void* func_80062E04(s32 id);
extern "C" void* func_80062E64(s32 id);
extern "C" void* func_80062CE4(s32 id);
extern "C" void* func_80062D44(s32 id);
extern "C" void func_80066714(int flag);
extern void* lbl_eu_8052A3B0[];
extern void* lbl_eu_805294E0[];
extern "C" void* func_800B20B4(UnkClass_805764CC* self, u32 mask,
                                const B20B4Payload* payload, u32 arg);

// Imports used by func_800B5994 (retail symbol names).
extern "C" void func_8004B0B0(void* dst);
extern "C" void func_8004B3F0(void* dst, const B47Vec3* src);
extern "C" int testResInfoFlag(unsigned long mask);
extern "C" void* getUnk80664658();
extern "C" void* func_800B6494();
extern "C" int CfRes_getE24Bit22();
extern const float lbl_eu_80661CD4;

// Player object returned by getPlayer__Q22cf13CfGameManagerFi:
// position getter at slot 43 (+0xAC).
class IB8FC4Player {
public:
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void p09();
    virtual void p0A();
    virtual void p0B();
    virtual void p0C();
    virtual void p0D();
    virtual void p0E();
    virtual void p0F();
    virtual void p10();
    virtual void p11();
    virtual void p12();
    virtual void p13();
    virtual void p14();
    virtual void p15();
    virtual void p16();
    virtual void p17();
    virtual void p18();
    virtual void p19();
    virtual void p1A();
    virtual void p1B();
    virtual void p1C();
    virtual void p1D();
    virtual void p1E();
    virtual void p1F();
    virtual void p20();
    virtual void p21();
    virtual void p22();
    virtual void p23();
    virtual void p24();
    virtual void p25();
    virtual void p26();
    virtual void p27();
    virtual void p28();
    virtual void p29();
    virtual void p2A();
    virtual B47Vec3* unk2B();   // +0xAC
};

// Spawned-object views used by func_800B20B4.
// Full object: point-setter at slot 81 (+0x144), spawn hook at slot 86 (+0x158).
class IB20B4Obj : public IB8FC4Player {
public:
    virtual void p2C();
    virtual void p2D();
    virtual void p2E();
    virtual void p2F();
    virtual void p30();
    virtual void p31();
    virtual void p32();
    virtual void p33();
    virtual void p34();
    virtual void p35();
    virtual void p36();
    virtual void p37();
    virtual void p38();
    virtual void p39();
    virtual void p3A();
    virtual void p3B();
    virtual void p3C();
    virtual void p3D();
    virtual void p3E();
    virtual void p3F();
    virtual void p40();
    virtual void p41();
    virtual void p42();
    virtual void p43();
    virtual void p44();
    virtual void p45();
    virtual void p46();
    virtual void p47();
    virtual void p48();
    virtual void p49();
    virtual void p4A();
    virtual void p4B();
    virtual void p4C();
    virtual void p4D();
    virtual void p4E();
    virtual void p4F();
    virtual void p50();
    virtual void setPoint(s32 idx, u32 val);   // +0x144
    virtual void p52();
    virtual void p53();
    virtual void p54();
    virtual void p55();
    virtual void unk56(s32 flag);              // +0x158
};

// Object type-id / trigger helpers (CfObjectMove.cpp).
extern "C" u16 func_800BE93C(void* self);
extern "C" void func_800BF2E0(void* self);

// Circular object-list accessors (defined in this unit's retail span).
extern "C" void* func_800B6BC8();
extern "C" void* func_800B6BEC();

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" unsigned long func_800B1C00();
extern "C" s32 func_800B1C0C(u32 mask);
extern "C" void* func_800B6DD0(void* reslist, void* obj);
extern "C" void func_800B1B2C(void* obj);
extern "C" void func_800B2D88(void* self, void* arg);
extern "C" void func_800B2DB0(FactoryPoolList* self, void* payloadSrc);
extern "C" void func_800B39C8(FactoryPoolList* self, void* payloadSrc);
extern "C" void func_800B1E2C(const char* name);
extern "C" void func_800B1EB8(int mask);
extern "C" void func_80068A30(void* dst, const char* src);
extern "C" void func_800B9A30(void* dst);
extern "C" void func_800B1368(int mask);
// Split1 .rodata string table; the compared name sits at +0x35.
extern char lbl_eu_804FC4D8[];
extern "C" void func_800B15A4(UnkClass_805764CC* self);
extern "C" void __dt__800B151C(FactoryPoolList* self);
extern "C" void __dt__800B18CC(FactoryPoolList* self);
extern "C" void func_800B1E18(UnkClass_805764CC* self, void* arg);
extern "C" void func_800B7058(void* obj);
extern "C" void func_800B93D0(void* obj);
extern "C" void func_800B9404(void* obj);
extern "C" void* func_800B6EC0(UnkClass_805764CC* self, int arg);
extern "C" void func_800B1C24(int arg, void* ptr);
extern "C" void* func_800B39C0(void* arg);
extern "C" void func_800BC3B0(void* obj, float val);
extern "C" void* func_800B708C__Fi(int arg);
// Enum-list holder helpers (cf::CfObjEnumList) used by func_800B98C8.
extern "C" void func_80043D90(void* holder);
extern "C" void* func_80043F18(void* holder);
extern "C" void __dt__80043E88(void* holder, int flag);
extern "C" void func_800F4A98(void* list, u32 type, u32 filter);
extern "C" void* func_800F6E98(void* list, int index);
extern "C" int func_800B9984(void* list);
extern "C" void* __dynamic_cast(void* obj, long offset, const void* srcType,
                                const void* dstType, void* src2dst);
// RTTI typeinfo pair for the actor dynamic_cast.
extern const void* lbl_eu_806618E8;
extern const void* lbl_eu_806618F0;
extern "C" void* func_80193CD0(void* a, void* b);
extern "C" unsigned long func_80061FFC();
extern "C" void __dl__FPv(void*);
extern "C" void __dla__FPv(void*);
extern "C" void* func_800B1AC0(void* a, void* b);
extern "C" void func_800B73E8(void* a, void* b, void* c);
extern "C" void __dt__8047BDA8(void*);
extern "C" void func_800B0894(UnkClass_805764CC* self, unsigned long handle, unsigned long count);
extern "C" void func_800B4278(void* object, u32 arg);
extern "C" void func_800B42E8(void* object, u32 arg);
extern "C" unsigned long func_800B0FEC(void* self);
extern "C" void func_800B0FF4(void* self, unsigned long handle, unsigned long count);
extern "C" void* func_800B20B4(UnkClass_805764CC* self, u32 arg1,
                                const B20B4Payload* arg2, u32 arg3);
// CfRes.cpp import (retail exports the unmangled name).
extern "C" u32 CfRes_extractBits27_5(void* self);
extern "C" void func_800B084C(UnkClass_805764CC* self, unsigned long count);
extern "C" void func_801742D4(void* obj);
extern "C" void func_80173C6C(void* obj, void* arg);
extern "C" void func_802B2A08(void* obj);
extern "C" void func_800B1F6C(UnkClass_805764CC* self);
extern "C" void func_800B1F2C(UnkClass_805764CC* self, void* obj);
extern "C" void func_800B205C(UnkClass_805764CC* self);
extern "C" void func_800B2034(UnkClass_805764CC* self, void* obj);
extern "C" void func_800B1FD8(UnkClass_805764CC* self);
extern "C" void func_800B1EC8(UnkClass_805764CC* self);
void func_800B1EC8(UnkClass_805764CC* self);
void func_800B1C78(UnkClass_805764CC* self);

// --- imports for func_800B655C (us-800b6e58) ---
// Returns the raw flag word so callers can use the value-context !=0 idiom.
extern "C" int testResInfoFlag(unsigned long mask);
extern "C" void* func_8006A6D0();
extern "C" u8 func_800AF7E4(void* container, const char* name);
extern "C" int func_801949E0(void* ctx);
extern "C" int func_8006DBD4(void* container, u16 flag);
extern "C" void func_800BE824(void* data, u32 flag);
extern "C" void func_800B66AC(void* data);
extern "C" void func_800B64F8(void* data);

// --- imports for func_800B4120 (us-800b4a1c) ---
extern "C" int func_8006C1B0(void* obj);
extern "C" int func_8006DF9C(void* obj);
extern "C" int func_800B31BC(void* obj);
extern "C" int func_800B31C8(void* obj);
extern "C" int func_800B31E0(void* obj);
extern "C" int func_800B31EC(void* obj);
extern "C" int func_800B31F8(void* obj);
extern "C" int func_800B3204(void* obj);
extern "C" int func_800B31B0(void* obj);
extern "C" void func_800B3A60(void* list, void* obj);

// --- imports for func_800B83AC (us-800b8cc8) ---
extern "C" int func_8007F91C__Q22cf13CfGameManagerFv();
extern "C" void* func_8009ECB0();
extern "C" void* func_8009EC9C(unsigned long index);
extern "C" int func_80174C98(void* dst, void* src, unsigned long size);
extern "C" int func_80148778(void* obj, int arg);
extern "C" void CfRes_getD80Flag();
extern "C" void func_80496288();
extern "C" void func_801765A4(void* obj, int arg);

// --- imports for func_800B7410 (us-800b7d30) ---
extern "C" void func_800B75C4(void* obj, unsigned long mask, int flag);
extern float lbl_eu_80666A10;
extern float lbl_eu_80666A08;

// --- func_800B83AC support types ---
// Embedded vtable-pointer sub-object at char+0x17c (status controller).
class B83ACSub17c {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1A();
    virtual void unk1B();
    virtual void unk1C();
    virtual void unk1D();
    virtual void unk1E();
    virtual void unk1F();
    virtual void unk20();
    virtual void unk21();
    virtual void unk22();
    virtual void unk23();
    virtual void unk24();
    virtual void unk25();
    virtual void unk26();
    virtual void unk27();
    virtual void unk28();
    virtual void unk29();
    virtual void unk2A();
    virtual void unk2B();
    virtual void unk2C();
    // slot 0xac: reset with a flag argument
    virtual void unk2D(int flag);
    // slot 0xb0
    virtual void unk2E();
    // slot 0xb4
    virtual void unk2F();
    // slot 0xb8
    virtual void unk30();
    virtual void unk31();
    virtual void unk32();
    virtual void unk33();
    virtual void unk34();
    virtual void unk35();
    virtual void unk36();
    virtual void unk37();
    virtual void unk38();
    virtual void unk39();
    virtual void unk3A();
    virtual void unk3B();
    virtual void unk3C();
    virtual void unk3D();
    virtual void unk3E();
    virtual void unk3F();
    virtual void unk40();
    virtual void unk41();
    virtual void unk42();
    virtual void unk43();
    virtual void unk44();
    virtual void unk45();
    virtual void unk46();
    virtual void unk47();
    virtual void unk48();
    virtual void unk49();
    virtual void unk4A();
    virtual void unk4B();
    virtual void unk4C();
    virtual void unk4D();
    virtual void unk4E();
    virtual void unk4F();
    virtual void unk50();
    virtual void unk51();
    virtual void unk52();
    virtual void unk53();
    virtual void unk54();
    virtual void unk55();
    virtual void unk56();
    virtual void unk57();
    virtual void unk58();
    virtual void unk59();
    virtual void unk5A();
    virtual void unk5B();
    virtual void unk5C();
    virtual void unk5D();
    virtual void unk5E();
    virtual void unk5F();
    virtual void unk60();
    virtual void unk61();
    virtual void unk62();
    virtual void unk63();
    virtual void unk64();
    virtual void unk65();
    virtual void unk66();
    virtual void unk67();
    virtual void unk68();
    virtual void unk69();
    virtual void unk6A();
    virtual void unk6B();
    virtual void unk6C();
    virtual void unk6D();
    virtual void unk6E();
    virtual void unk6F();
    virtual void unk70();
    virtual void unk71();
    virtual void unk72();
    virtual void unk73();
    // slot 0x128: read a progress value compared against a threshold
    virtual float unk74();
};

// Embedded vtable-pointer sub-object at char+0x184.
class B83ACSub184 {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    // slot 0x14: triggered with an id argument
    virtual void unk05(int arg);
};

// Object behind the pointer at char+0x180; its slot-0x30 getter returns the
// record whose first word feeds func_80174C98's copy.
class B83ACSrc180 {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    // slot 0x30: returns a pointer to the copy source record
    virtual void* unk0C();
};

// Party member record walked by func_800B83AC (base from func_8009ECB0;
// the walker starts at base+4 and reads slots at +0xc from there).
struct B83ACPartyBase {
    u8 _pad00[0x10];
    s32 memberSlot[6];   // +0x10..0x24, values 1..13 are valid ids
};

// Character record returned by func_8009EC9C.
struct B83ACCharObj {
    u8 _pad00[0x17c];
    B83ACSub17c m17c;    // +0x17c embedded status controller
    void* field_180;     // +0x180 source object for the 0x1c-byte copy
    B83ACSub184 m184;    // +0x184 trigger sub-object
};

// --- batch targets: us-800b5680 / us-800b4e9c / us-800b19ec / us-800b1e70 / us-800b9678 ---

// Scratch entry-array accessors over the temp buffer built by func_800B5994
// (retail names; begin/end/iterator primitives used by func_800B4D84).
extern "C" void** func_800B5944(void* buf);
extern "C" void** func_800B5948(void* buf);
extern "C" void func_800B5958(void* it);
extern "C" u32* func_800B5964(void* it, u32 index);
extern "C" u32 func_800B5970(void* it);
extern "C" int func_800B6544(const void* a, const void* b);
extern "C" void func_800B4FAC(void** first, void** last, int (*cmp)(const void*, const void*));
extern "C" void* getUnk80664658();
extern "C" void* func_800B4F58(void* p);
extern "C" unsigned short func_800B4F64();
extern "C" u32 func_800B4FA4(void* obj);
extern "C" void func_800B4F6C(void* obj);
extern "C" void func_800B4F90(void* obj);
extern "C" int func_800B3D4C(void* obj, u32 flag);
extern "C" int func_80082FCC__Q22cf13CfGameManagerFv(void* obj);
extern "C" u32 func_eu_800BFC7C(void* obj);
extern "C" u32 func_800A8C84();
extern "C" void CfRes_callFunc_68110(u32 value);
extern "C" void CfRes_callFunc_68078();

// Extra list heads / misc imports for func_800B45A0 / func_800B1120.
extern "C" void* func_800B6C34();
extern "C" void func_80063160(s32 arg);
extern "C" void func_800631FC(s32 arg);
extern "C" void func_800B4588(void* self);

// Factory-event queue iterator primitives (retail names).
extern "C" void func_800B14C4(void* dst, void* list);
extern "C" void** func_800B14D4(void* it);
extern "C" void func_800B14E0(void* it);
extern "C" void func_800B14F0(void* dst, void* list);
extern "C" int func_800B14FC(void* a, void* b);

// Sub-object ctors/dtors driven by func_800B1120 / func_800B15A4.
extern "C" void __dt__800B183C(void* self);
extern "C" void __dt__80206170(void* self, int flags);
extern "C" void __dt__801732F8(void* self, int flags);
extern "C" void __dt__80193538(void* self, int flags);
extern "C" void __dt__801F3C08(void* self, int flags);
extern "C" void* __ct__80193270(void* self);
extern "C" void* __ct__801F3BE8(void* self);
extern "C" void* __ct__80205A7C(void* self);
extern "C" void* __ct__cf_CfHikariItemManager(void* self);
extern "C" void* func_800AD860__FPv(void* data);
extern "C" void CfRes_callFunc_68254();

// Payload objects walked by func_800B45A0: status callback at slot 0x1D
// (+0x74) plus a flag word at +0x64 (bit 15 tested).
class IEvB45A0 {
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual void vf0A();
    virtual void vf0B();
    virtual void vf0C();
    virtual void vf0D();
    virtual void vf0E();
    virtual void vf0F();
    virtual void vf10();
    virtual void vf11();
    virtual void vf12();
    virtual void vf13();
    virtual void vf14();
    virtual void vf15();
    virtual void vf16();
    virtual void vf17();
    virtual void vf18();
    virtual void vf19();
    virtual void vf1A();
    virtual bool vf1D();   // vtable +0x74 (compiler pads 2 slots)
};
struct B45A0ObjView : public IEvB45A0 {
    u8 _pad04[0x64 - 0x4];
    u32 field_64;          // bit 15 gates the vf1D check
};

// Factory-event payloads drained by func_800B1120 (slot 5) and
// func_800B15A4 (slot 6). MWCC pads two hidden vtable slots ahead of the
// declared virtuals, so the callbacks sit two entries early here.
class IFeEventBase {
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf05();   // compiled slot 5 (+0x14)
    virtual void vf06();   // compiled slot 6 (+0x18)
};

// Object dispatched at vtable slot 21 (+0x54) by func_800B15A4's
// game-manager walk.
class IDispB15A4 {
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual void vf0A();
    virtual void vf0B();
    virtual void vf0C();
    virtual void vf0D();
    virtual void vf0E();
    virtual void vf0F();
    virtual void vf10();
    virtual void vf11();
    virtual void vf12();
    virtual void vf15(int flag);   // compiled slot 21 (+0x54)
};

// Object behind field_0xD00: reset dispatch at slot 2 (+0x8).
class IDispD00 {
public:
    virtual void vf02(int flag);   // compiled slot 2 (+0x8)
};

// Embedded sub-object at container+0x3E9C used by func_800B8D5C: status
// queries at slots 0x1D/0x1E (+0x74/+0x78), a validator at slot 0x58
// (+0x160) and a float getter at slot 0x5B (+0x16C).
class B8D5CSub {
public:
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual void vf0A();
    virtual void vf0B();
    virtual void vf0C();
    virtual void vf0D();
    virtual void vf0E();
    virtual void vf0F();
    virtual void vf10();
    virtual void vf11();
    virtual void vf12();
    virtual void vf13();
    virtual void vf14();
    virtual void vf15();
    virtual void vf16();
    virtual void vf17();
    virtual void vf18();
    virtual void vf19();
    virtual void vf1A();
    virtual s32 vf1C();     // compiled slot 29 (+0x74)
    virtual s32 vf1D();     // +0x78
    virtual void vf1E();
    virtual void vf1F();
    virtual void vf20();
    virtual void vf21();
    virtual void vf22();
    virtual void vf23();
    virtual void vf24();
    virtual void vf25();
    virtual void vf26();
    virtual void vf27();
    virtual void vf28();
    virtual void vf29();
    virtual void vf2A();
    virtual void* unk2B();   // compiled slot 43 (+0xAC): position getter
    virtual void vf2C();
    virtual void vf2D();
    virtual void vf2E();
    virtual void vf2F();
    virtual void vf30();
    virtual void vf31();
    virtual void vf32();
    virtual void vf33();
    virtual void vf34();
    virtual void vf35();
    virtual void vf36();
    virtual void vf37();
    virtual void vf38();
    virtual void vf39();
    virtual void vf3A();
    virtual void vf3B();
    virtual void vf3C();
    virtual void vf3D();
    virtual void vf3E();
    virtual void vf3F();
    virtual void vf40();
    virtual void vf41();
    virtual void vf42();
    virtual void vf43();
    virtual void vf44();
    virtual void vf45();
    virtual void vf46();
    virtual void vf47();
    virtual void vf48();
    virtual void vf49();
    virtual void vf4A();
    virtual void vf4B();
    virtual void vf4C();
    virtual void vf4D();
    virtual void vf4E();
    virtual void vf4F();
    virtual void vf50();
    virtual void vf51();
    virtual void vf52();
    virtual void vf53();
    virtual void vf54();
    virtual void vf55();
    virtual s32 vf58();     // compiled slot 88 (+0x160)
    virtual void vf59();
    virtual void vf5A();
    virtual float vf5B();   // compiled slot 91 (+0x16C)
};

// Container resolved from a node payload via func_800AD860__FPV.
struct B8D5CObj {
    u8 _pad00[0x15F0];
    u32 field_15F0;                    // type id, cascade-tested against 4..8
    u8 _padF4[0x3E9C - 0x15F4];
    B8D5CSub field_3E9C;               // embedded sub-object (vtable @ +0x3E9C)
    u8 _padEA0[0x3F04 - 0x3EA0];
    u32 field_3F04;                    // bits 11/25/26 tested
    u32 field_3F08;                    // bits 0/30 tested
    u8 _padF10[0x3F60 - 0x3F10];
    void* field_3F60;                  // must be null on the success path
};

// --- func_800B7410 support types ---
// TboxInfo reslist payload view used while spawning voice objects.
struct B7410Node {
    u32 field_00;          // +0x00 occupied guard (0 == free to spawn)
    u8 _pad04[0x10 - 0x04];
    float field_10;        // +0x10 volume/position float
    u32 field_14;          // +0x14 resource tag
    u16 field_18;          // +0x18 id argument
    u8 field_1A;           // +0x1A language A match byte
    u8 field_1B;           // +0x1B language B match byte
};

// --- batch targets: us-800b71a4 / us-800b50a4 / us-800b8e40 / us-800b4354 / us-800b379c ---

// Shared sdata2/sdata float constants.
extern const float lbl_eu_806669DC;
extern const float lbl_eu_806669E4;
extern const float lbl_eu_806669E8;
extern const float lbl_eu_806669EC;
extern const float lbl_eu_806669F0;
extern const float lbl_eu_80666A0C;
// Runtime-tuned globals.
extern float lbl_eu_80663ED0;
extern void* lbl_eu_80663E14;

// nw4r imports used by func_800B47A8 (retail C-ABI names).
namespace nw4r {
namespace db {
extern "C" void Warning(const char* file, int line, const char* msg, ...);
}
namespace math {
extern "C" float FrSqrt(float x);
}
}
extern char lbl_eu_80526300[];
extern char lbl_eu_80526324[];

// Simple 3-float vector manipulated by func_800B47A8.
struct B47Vec3 {
    float x;
    float y;
    float z;
};

// Listener/dispatch object behind *(lbl_eu_80663E14 + 0xB4): slot 5 (+0x14)
// receives the shifted position vectors from func_800B47A8 and returns a hit id.
class IBvt14 {
public:
    virtual void pad00();
    virtual void pad01();
    virtual void pad02();
    virtual void pad03();
    virtual void pad04();
    virtual int unk05(void* a, void* b, void* c, int mode);
};

// Object walked by func_800B68A8: position getter at slot 0x2B (+0xAC),
// volume setter at slot 0x5A (+0x168), volume getter at slot 0x5B (+0x16C),
// level getter at slot 0x5D (+0x174).
class IB68Obj {
public:
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void p09();
    virtual void p0A();
    virtual void p0B();
    virtual void p0C();
    virtual void p0D();
    virtual void p0E();
    virtual void p0F();
    virtual void p10();
    virtual void p11();
    virtual void p12();
    virtual void p13();
    virtual void p14();
    virtual void p15();
    virtual void p16();
    virtual void p17();
    virtual void p18();
    virtual void p19();
    virtual void p1A();
    virtual void p1B();
    virtual void p1C();
    virtual void p1D();
    virtual void p1E();
    virtual void p1F();
    virtual void p20();
    virtual void p21();
    virtual void p22();
    virtual void p23();
    virtual void p24();
    virtual void p25();
    virtual void p26();
    virtual void p27();
    virtual void p28();
    virtual void p29();
    virtual void p2A();
    virtual void* unk2B();  // +0xAC
    virtual void p2C();
    virtual void p2D();
    virtual void p2E();
    virtual void p2F();
    virtual void p30();
    virtual void p31();
    virtual void p32();
    virtual void p33();
    virtual void p34();
    virtual void p35();
    virtual void p36();
    virtual void p37();
    virtual void p38();
    virtual void p39();
    virtual void p3A();
    virtual void p3B();
    virtual void p3C();
    virtual void p3D();
    virtual void p3E();
    virtual void p3F();
    virtual void p40();
    virtual void p41();
    virtual void p42();
    virtual void p43();
    virtual void p44();
    virtual void p45();
    virtual void p46();
    virtual void p47();
    virtual void p48();
    virtual void p49();
    virtual void p4A();
    virtual void p4B();
    virtual void p4C();
    virtual void p4D();
    virtual void p4E();
    virtual void p4F();
    virtual void p50();
    virtual void p51();
    virtual void p52();
    virtual void p53();
    virtual void p54();
    virtual void p55();
    virtual void p56();
    virtual void p57();
    virtual void p58();
    virtual void p59();
    virtual void unk5A(float val);  // +0x168
    virtual float unk5B();          // +0x16C
    virtual void p5C();
    virtual float unk5D();          // +0x174
};

// Factory-event queue payload whose slot 4 (+0x10) receives a removed object
// (used by func_800B8524 / func_800B3A88 queue drains).
class IQItem4 {
public:
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void unk04(void* arg);
};

// Object removed by func_800B3A88: slot 21 (+0x54) fires with a flag.
class IDispObj54 {
public:
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void p09();
    virtual void p0A();
    virtual void p0B();
    virtual void p0C();
    virtual void p0D();
    virtual void p0E();
    virtual void p0F();
    virtual void p10();
    virtual void p11();
    virtual void p12();
    virtual void p13();
    virtual void p14();
    virtual void unk15(int flag);
};

// Game-manager-like holder polled by func_800B3A88: current-object getter at
// slot 24 (+0x60) and reset at slot 23 (+0x5C).
class IDispB3A88Mgr {
public:
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void p09();
    virtual void p0A();
    virtual void p0B();
    virtual void p0C();
    virtual void p0D();
    virtual void p0E();
    virtual void p0F();
    virtual void p10();
    virtual void p11();
    virtual void p12();
    virtual void p13();
    virtual void p14();
    virtual void p15();
    virtual void p16();
    virtual void unk17(int flag);
    virtual void* unk18();
};

// Vtable view of the payload records walked by func_800B8524 (slots 0x60/0x64).
class IB8524DataVt {
public:
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void p09();
    virtual void p0A();
    virtual void p0B();
    virtual void p0C();
    virtual void p0D();
    virtual void p0E();
    virtual void p0F();
    virtual void p10();
    virtual void p11();
    virtual void p12();
    virtual void p13();
    virtual void p14();
    virtual void p15();
    virtual void p16();
    virtual void p17();
    virtual void unk18();  // +0x60
    virtual void unk19();  // +0x64
};

// Field view of the func_800B8524 payloads (flag words at +0x64/+0x68, +0xC4).
struct B8524Data : public IB8524DataVt {
    u8 _pad04[0x64 - 0x4];
    u32 flags64;                 // bit 2/8/9 tested
    u32 flags68;                 // bit 5/6 tested
    u8 _pad6C[0xC4 - 0x6C];
    u32 field_C4;                // guard for func_8004CF00
};

// Record returned by func_800BBC0C inside func_800B8524.
struct B8524Sub {
    u8 _pad00[0x68];
    u32 field68;                 // bit 11 tested
    u8 _pad6C[0x98 - 0x6C];
    void* field98;               // must be non-null
};

// Imports used by the batch targets (retail symbol names).
extern "C" int func_800B4594(void* obj);
extern "C" int func_8006EF04(u32 mask);
extern "C" int func_800B64B8(void* obj, u32 mask);
extern "C" void func_8004CB80(void* dst, void* a, void* b);
extern "C" float func_80073F88(void* p);
extern "C" void func_800B6AF4(void* self);
extern "C" void func_80496264(int arg);
extern "C" void* CfRes_getInstanceField();
extern "C" void func_80067DB4();
extern "C" void* func_800BBC0C(void* data);
extern "C" void func_800BC3D8(void* obj, float val);
extern "C" void func_8004CF00();
extern "C" void* func_800B77BC();
extern "C" void func_800B7AF0(UnkClass_805764CC* self, class IB7Arg* arg);
extern "C" void func_800B9C14(void* obj);
extern "C" void func_80068358(void* obj);
extern "C" IDispB3A88Mgr* func_800821F8__Q22cf13CfGameManagerFv();
extern "C" void* func_8006E59C();
extern "C" void* func_800755B0(void* mgr, int index);
extern "C" s32 func_800BE96C(void* obj);
extern "C" void func_80063900(u32 id);
extern "C" void func_800638B4(s32 id);
extern "C" s32 func_800BF2CC(void* obj);
extern "C" void func_eu_801C17FC(u32 a, u32 b, int c);
extern "C" s32 func_80198400(void* voice, u32 key);
extern "C" void func_80197B4C(void* container, int flag);
extern "C" void func_80197BA4(void* container, int a, int b);

extern "C" u32* func_800B1818(void* iter, int arg);
extern "C" void* func_800B1820(void* dst, void* src);
extern "C" void func_800B3A54(void* dst, void* src);
extern "C" void func_800B3A88(UnkClass_805764CC* self, void* obj);
extern "C" void func_800B8524(UnkClass_805764CC* self, void* arg, void* other);
extern "C" void func_800B68A8(UnkClass_805764CC* self, void* partner,
                              const void* listv, int flag, float limit);
extern "C" u32 func_800B47A8(int modeScale, float scale, const B47Vec3* posA,
                             const B47Vec3* posB, int modeFade, int modeNear,
                             float* outDist, u8* outHit);
extern "C" u32 func_800B2ED0(UnkClass_805764CC* self, cf::CfObject* obj);

// Spawned object interface used by func_800B7410.
class IDispB7410 {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1A();
    virtual void unk1B();
    virtual void unk1C();
    virtual void unk1D();
    virtual void unk1E();
    virtual void unk1F();
    virtual void unk20();
    virtual void unk21();
    virtual void unk22();
    virtual void unk23();
    virtual void unk24();
    virtual void unk25();
    virtual void unk26();
    // slot 0x9c (index 39): bind a payload record
    virtual void unk39(void* arg);
    virtual void unk3A();
    virtual void unk3B();
    virtual void unk3C();
    virtual void unk3D();
    virtual void unk3E();
    virtual void unk3F();
    virtual void unk40();
    virtual void unk41();
    virtual void unk42();
    virtual void unk43();
    virtual void unk44();
    virtual void unk45();
    virtual void unk46();
    virtual void unk47();
    virtual void unk48();
    // slot 0xc4 (index 49): set the node float
    virtual void unk49(float val);
    virtual void unk4A();
    virtual void unk4B();
    virtual void unk4C();
    virtual void unk4D();
    virtual void unk4E();
    // slot 0xdc (index 55): apply the shared constant
    virtual void unk55(float val);
};
#endif
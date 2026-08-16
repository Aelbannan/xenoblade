#pragma once

#include <types.h>

namespace cf {

class CfObjectImplMove {
public:
    virtual ~CfObjectImplMove();

    void func_800CFFA0(unsigned int* param);
    void func_800CFFBC();
    void func_800CFFC4();
};

} // namespace cf

// Internal driver struct for CfObjectImplMove.
// Function func_800CD5C0 accesses a sub-object at +0x14 and reads a uint at +0xc4.
struct CfObjectImplMoveSubObj;
struct CfObjectImplMoveData {
    u8 _00_13[0x14];              // 0x00-0x13
    CfObjectImplMoveSubObj* mSubObj;                // 0x14, sub-object with field at +0xc4

    void func_800CD5C0(unsigned int a, unsigned int b);
};

// Vtable-view replacement for the driver sub-object dispatch: genuine virtual
// calls reproduce retail's r12 dispatch (lwz r12,0(rX); lwz r12,slot(r12)).
// Slots 0xAC/0x128 return position objects, slot 0x220 takes the move id.
class CfObjectImplMoveSubObj {
public:
    virtual void* f00(); virtual void* f01(); virtual void* f02(); virtual void* f03();
    virtual void* f04(); virtual void* f05(); virtual void* f06(); virtual void* f07();
    virtual void* f08(); virtual void* f09(); virtual void* f10(); virtual void* f11();
    virtual void* f12(); virtual void* f13(); virtual void* f14(); virtual void* f15();
    virtual void* f16(); virtual void* f17(); virtual void* f18(); virtual void* f19();
    virtual void* f20(); virtual void* f21(); virtual void* f22(); virtual void* f23();
    virtual void* f24(); virtual void* f25(); virtual void* f26(); virtual void* f27();
    virtual void* f28(); virtual void* f29(); virtual void* f30(); virtual void* f31();
    virtual void* f32(); virtual void* f33(); virtual void* f34(); virtual void* f35();
    virtual void* f36(); virtual void* f37(); virtual void* f38(); virtual void* f39();
    virtual void* f40();
    virtual void* vfnAC();            // index 41 -> vtable 0xac
    virtual void* f42(); virtual void* f43(); virtual void* f44(); virtual void* f45();
    virtual void* f46(); virtual void* f47(); virtual void* f48(); virtual void* f49();
    virtual void* f50(); virtual void* f51(); virtual void* f52(); virtual void* f53();
    virtual void* f54(); virtual void* f55(); virtual void* f56(); virtual void* f57();
    virtual void* f58(); virtual void* f59(); virtual void* f60(); virtual void* f61();
    virtual void* f62(); virtual void* f63(); virtual void* f64(); virtual void* f65();
    virtual void* f66(); virtual void* f67(); virtual void* f68(); virtual void* f69();
    virtual void* f70(); virtual void* f71();
    virtual void* vfn128();           // index 72 -> vtable 0x128
    virtual void* f73(); virtual void* f74(); virtual void* f75(); virtual void* f76();
    virtual void* f77(); virtual void* f78(); virtual void* f79(); virtual void* f80();
    virtual void* f81(); virtual void* f82(); virtual void* f83(); virtual void* f84();
    virtual void* f85(); virtual void* f86(); virtual void* f87(); virtual void* f88();
    virtual void* f89(); virtual void* f90(); virtual void* f91(); virtual void* f92();
    virtual void* f93(); virtual void* f94(); virtual void* f95(); virtual void* f96();
    virtual void* f97(); virtual void* f98(); virtual void* f99(); virtual void* f100();
    virtual void* f101(); virtual void* f102(); virtual void* f103(); virtual void* f104();
    virtual void* f105(); virtual void* f106(); virtual void* f107(); virtual void* f108();
    virtual void* f109(); virtual void* f110(); virtual void* f111(); virtual void* f112();
    virtual void* f113(); virtual void* f114(); virtual void* f115(); virtual void* f116();
    virtual void* f117(); virtual void* f118(); virtual void* f119(); virtual void* f120();
    virtual void* f121(); virtual void* f122(); virtual void* f123(); virtual void* f124();
    virtual void* f125(); virtual void* f126(); virtual void* f127(); virtual void* f128();
    virtual void* f129(); virtual void* f130(); virtual void* f131(); virtual void* f132();
    virtual void* f133();
    virtual void* vfn220(u16 id);     // index 134 -> vtable 0x220

    // vptr occupies 0x00-0x03; fields keep their absolute retail offsets.
    u8 _04_63[0x60];              // 0x04-0x63
    u32 field_0x64;               // 0x64 (flags: bit 1 = 0x2, bit 7 = 0x80)
    u8 _68_8F[0x90 - 0x68];       // 0x68-0x8f
    u32 field_0x90;               // 0x90 (status word)
    u8 _94_97[0x98 - 0x94];       // 0x94-0x97
    unsigned int field_0x98;      // 0x98 (event / handle id; also an object ptr in func_800CED64)
    u8 _9C_C3[0x28];              // 0x9c-0xc3
    unsigned int mSomeId;         // 0xc4
    u8 _C8_6F7[0x6f8 - 0xc8];     // 0xc8-0x6f7
    void* field_0x6F8;            // 0x6f8 (partner object, vtable slot 0xa8)
    void* field_0x6FC;            // 0x6fc (partner object, vtable slot 0xa8)
};

// Partner objects at driver +0x6F8 / +0x6FC: slot 0xA8 returns the object
// handed to func_800ACF78.
class CfMoveDriverSubObj {
public:
    virtual void* f00(); virtual void* f01(); virtual void* f02(); virtual void* f03();
    virtual void* f04(); virtual void* f05(); virtual void* f06(); virtual void* f07();
    virtual void* f08(); virtual void* f09(); virtual void* f10(); virtual void* f11();
    virtual void* f12(); virtual void* f13(); virtual void* f14(); virtual void* f15();
    virtual void* f16(); virtual void* f17(); virtual void* f18(); virtual void* f19();
    virtual void* f20(); virtual void* f21(); virtual void* f22(); virtual void* f23();
    virtual void* f24(); virtual void* f25(); virtual void* f26(); virtual void* f27();
    virtual void* f28(); virtual void* f29(); virtual void* f30(); virtual void* f31();
    virtual void* f32(); virtual void* f33(); virtual void* f34(); virtual void* f35();
    virtual void* f36(); virtual void* f37(); virtual void* f38(); virtual void* f39();
    virtual void* vfnA8();            // index 40 -> vtable 0xa8
};

// Move/effect object returned by the driver dispatch (CfObjectEff family):
// slot 0x9C receives a position vector, slot 0x194 a flag word; +0x94 holds
// the child-effect pointer fed to func_804E3CDC.
class CfMoveEffObj {
public:
    virtual void* f00(); virtual void* f01(); virtual void* f02(); virtual void* f03();
    virtual void* f04(); virtual void* f05(); virtual void* f06(); virtual void* f07();
    virtual void* f08(); virtual void* f09(); virtual void* f10(); virtual void* f11();
    virtual void* f12(); virtual void* f13(); virtual void* f14(); virtual void* f15();
    virtual void* f16(); virtual void* f17(); virtual void* f18(); virtual void* f19();
    virtual void* f20(); virtual void* f21(); virtual void* f22(); virtual void* f23();
    virtual void* f24(); virtual void* f25(); virtual void* f26(); virtual void* f27();
    virtual void* f28(); virtual void* f29(); virtual void* f30(); virtual void* f31();
    virtual void* f32(); virtual void* f33(); virtual void* f34(); virtual void* f35();
    virtual void* f36();
    virtual void vfn9C(struct CfMoveVec3f* vec);  // index 37 -> vtable 0x9c
    virtual void* f38(); virtual void* f39(); virtual void* f40(); virtual void* f41();
    virtual void* f42(); virtual void* f43(); virtual void* f44(); virtual void* f45();
    virtual void* f46(); virtual void* f47(); virtual void* f48(); virtual void* f49();
    virtual void* f50(); virtual void* f51(); virtual void* f52(); virtual void* f53();
    virtual void* f54(); virtual void* f55(); virtual void* f56(); virtual void* f57();
    virtual void* f58(); virtual void* f59(); virtual void* f60(); virtual void* f61();
    virtual void* f62(); virtual void* f63(); virtual void* f64(); virtual void* f65();
    virtual void* f66(); virtual void* f67(); virtual void* f68(); virtual void* f69();
    virtual void* f70(); virtual void* f71(); virtual void* f72(); virtual void* f73();
    virtual void* f74(); virtual void* f75(); virtual void* f76(); virtual void* f77();
    virtual void* f78(); virtual void* f79(); virtual void* f80(); virtual void* f81();
    virtual void* f82(); virtual void* f83(); virtual void* f84(); virtual void* f85();
    virtual void* f86(); virtual void* f87(); virtual void* f88(); virtual void* f89();
    virtual void* f90(); virtual void* f91(); virtual void* f92(); virtual void* f93();
    virtual void* f94(); virtual void* f95(); virtual void* f96(); virtual void* f97();
    virtual void* f98();
    virtual void vfn194(u32 a);           // index 99 -> vtable 0x194

    u8 _04_93[0x90];               // 0x04-0x93
    void* field_0x94;              // 0x94 (child effect)
};

// Event object reached via CfObjectImplMoveSubObj::field_0x98 (func_800CED64):
// vtable slot 0x88 is dispatched with a u32 argument; slot 0x4c (f17) returns
// a float compared against lbl_eu_80666C88 by func_800CC638.
class CfMoveEventObj {
public:
    virtual void* f00();   virtual void* f01();   virtual void* f02();   virtual void* f03();
    virtual void* f04();   virtual void* f05();   virtual void* f06();   virtual void* f07();
    virtual void* f08();   virtual void* f09();   virtual void* f10();   virtual void* f11();
    virtual void* f12();   virtual void* f13();   virtual void* f14();   virtual void* f15();
    virtual void* f16();   virtual f32 f17();    // index 17 -> vtable 0x4c (returns float)
    virtual void* f18();   virtual void* f19();   virtual void* f20();   virtual void* f21();
    virtual void* f22();   virtual void* f23();   virtual void* f24();   virtual void* f25();
    virtual void* f26();   virtual void* f27();   virtual void* f28();   virtual void* f29();
    virtual void* f30();   virtual void* f31();
    virtual void vfn88(u32 a);    // index 32 -> vtable 0x88
};

// Embedded sub-object (own vtable) at +0x3e9c of CfActorObj. With -RTTI on,
// declared index N sits at vtable offset (N+2)*4, so index 17 is 0x4c and
// index 18 is 0x50. vfn00 (index 0, 0x08) is called with a u32 from
// func_800CE8E4; vfn13 (index 17) returns the sub-result used by func_800CE8AC
// / func_800CEA34 / func_800CE8E4 (retail lwzu r12/lwz r12,0x4c shape);
// vfn14 (index 18) is dispatched with a pointer argument from func_800CE8E4.
struct CfEmbeddedSubObj_3E9C {
    virtual void* vfn00(u32 a);   // index 0  -> vtable 0x08
    virtual void* f01();          // index 1
    virtual void* f02();          // index 2
    virtual void* f03();          // index 3
    virtual void* f04();          // index 4
    virtual void* f05();          // index 5
    virtual void* f06();          // index 6
    virtual void* f07();          // index 7
    virtual void* f08();          // index 8
    virtual void* f09();          // index 9
    virtual void* f10();          // index 10
    virtual void* f11();          // index 11
    virtual void* f12();          // index 12
    virtual void* f13();          // index 13
    virtual void* f14();          // index 14
    virtual void* f15();          // index 15
    virtual void* f16();          // index 16
    virtual void* vfn13();        // index 17 -> vtable 0x4c (returns sub-result)
    virtual void* vfn14(void* a); // index 18 -> vtable 0x50
    virtual void* f19();          // index 19
    // Filler slots 0x54-0x200 (indices 20-126): opaque vtable padding so the
    // dispatched slots below sit at their retail offsets.
    virtual void* f20(); virtual void* f21(); virtual void* f22(); virtual void* f23();
    virtual void* f24(); virtual void* f25(); virtual void* f26(); virtual void* f27();
    virtual void* f28(); virtual void* f29(); virtual void* f30(); virtual void* f31();
    virtual void* f32(); virtual void* f33(); virtual void* f34(); virtual void* f35();
    virtual void* f36(); virtual void* f37(); virtual void* f38(); virtual void* f39();
    virtual void* f40(); virtual void* f41(); virtual void* f42(); virtual void* f43();
    virtual void* f44(); virtual void* f45(); virtual void* f46(); virtual void* f47();
    virtual void* f48(); virtual void* f49(); virtual void* f50(); virtual void* f51();
    virtual void* f52(); virtual void* f53(); virtual void* f54(); virtual void* f55();
    virtual void* f56(); virtual void* f57(); virtual void* f58(); virtual void* f59();
    virtual void* f60(); virtual void* f61(); virtual void* f62(); virtual void* f63();
    virtual void* f64(); virtual void* f65(); virtual void* f66(); virtual void* f67();
    virtual void* f68(); virtual void* f69(); virtual void* f70(); virtual void* f71();
    virtual void* f72(); virtual void* f73(); virtual void* f74(); virtual void* f75();
    virtual void* f76(); virtual void* f77(); virtual void* f78(); virtual void* f79();
    virtual void* f80(); virtual void* f81(); virtual void* f82(); virtual void* f83();
    virtual void* f84(); virtual void* f85(); virtual void* f86(); virtual void* f87();
    virtual void* f88(); virtual void* f89(); virtual void* f90(); virtual void* f91();
    virtual void* f92(); virtual void* f93(); virtual void* f94(); virtual void* f95();
    virtual void* f96(); virtual void* f97(); virtual void* f98(); virtual void* f99();
    virtual void* f100(); virtual void* f101(); virtual void* f102(); virtual void* f103();
    virtual void* f104(); virtual void* f105(); virtual void* f106(); virtual void* f107();
    virtual void* f108(); virtual void* f109(); virtual void* f110(); virtual void* f111();
    virtual void* f112(); virtual void* f113(); virtual void* f114(); virtual void* f115();
    virtual void* f116(); virtual void* f117(); virtual void* f118(); virtual void* f119();
    virtual void* f120(); virtual void* f121(); virtual void* f122(); virtual void* f123();
    virtual void* f124(); virtual void* f125(); virtual void* f126();
    virtual void* vfn204(u32 a, int b, int c, int d, int e);  // index 127 -> vtable 0x204
    virtual void* f128();         // index 128 -> vtable 0x208
    virtual void* vfn20C(u32 a);  // index 129 -> vtable 0x20c
};

// Result of CfActorObj::vf298() (vtable 0x298): +0x4 feeds func_800B708C's
// actor-id lookup (func_800CC638); +0x50 is a battle-state block pointer
// (func_800CEA34 reads its kind/count bytes at +0x43/+0x44).
struct CfMoveVf298Result {
    u8 _00_03[0x4];               // 0x00-0x03
    u32 field_0x4;                // 0x04 (actor id)
    u8 _08_4F[0x50 - 0x08];       // 0x08-0x4f
    void* field_0x50;             // 0x50
};

// Battle-state block behind CfMoveVf298Result::field_0x50: kind byte at +0x43,
// count byte at +0x44.
struct CfMoveBattleState {
    u8 _00_42[0x43];              // 0x00-0x42
    u8 field_0x43;                // 0x43 (kind: 1 -> count from +0x44, 2 -> 1)
    u8 field_0x44;                // 0x44 (count value)
};

// Item returned by CfActorObj::vf29C() (vtable 0x29c): word written at +0x4
// by func_800CEA34's loop.
struct CfMoveVf29CItem {
    u8 _00_03[0x4];               // 0x00-0x03
    void* field_0x4;              // 0x4
};

// Object at CfActorObj::field_04: vtable slot 0x20 (index 6) called with a
// u32 mask, slot 0x30 (index 10) returns a word block (func_800CE8E4).
struct CfMoveB30Result {
    u32 field_0;                  // 0x00
};

class CfActorObj4 {
public:
    virtual void b00();           // index 0
    virtual void b04();           // index 1
    virtual void b08();           // index 2
    virtual void b0C();           // index 3
    virtual void b10();           // index 4
    virtual void b14();           // index 5
    virtual void b20(u32 a);      // index 6  -> vtable 0x20
    virtual void b24();           // index 7
    virtual void b28();           // index 8
    virtual void b2C();           // index 9
    virtual CfMoveB30Result* b30();  // index 10 -> vtable 0x30
};

// Result of CfActorObj::vfE4() (vtable 0xe4): vtable slot 0x10 (index 2)
// called with no args (func_800CA964).
class CfMoveFieldE4 {
public:
    virtual void e00();           // index 0
    virtual void e04();           // index 1
    virtual void e10();           // index 2 -> vtable 0x10
    virtual void e14();           // index 3 -> vtable 0x14
};

// Request parameter consumed by func_800CC638: mode byte at +0xA, move id at
// +0xC, flag/type byte at +0xE (bit 7 = flag, low 7 bits = effect type), and
// a count byte at +0xF.
struct CfMoveParam {
    u8 _00_09[0xa];               // 0x00-0x09
    u8 field_0xA;                 // 0xa (mode: 0 = manager lookup, 1 = driver dispatch, 2 = manager pair)
    u8 _pad0B;                    // 0xb
    u16 field_0xC;                // 0xc (move id)
    u8 field_0xE;                 // 0xe (bit 7 = flag, low 7 bits = effect type)
    u8 field_0xF;                 // 0xf (count)
};

// 3-component float vector.
struct CfMoveVec3f {
    f32 x, y, z;
};

// Float/word bit-cast pair for word-copying float values (stfs->lwz->stw).
union CfMoveFloatBits {
    f32 f;
    u32 w;
};

// u32 word-pair / f64 view for MWCC's 0x43300000 int->float conversion
// (CfMapEffectManager.hpp convention): subtract the named sdata2 magic double
// lbl_eu_80666C90 so the pool reloc matches retail instead of an MWCC @N.
union CfMoveF64Conv {
    u32 w[2];
    f64 d;
};

// Position object returned by driver vtable slot 0x128 (func_800CC638 reads
// float fields at +0xC, +0x1C, +0x2C).
struct CfMovePosObj {
    u8 _00_0B[0xc];               // 0x00-0x0b
    f32 field_0xC;                // 0xc
    u8 _10_1B[0x10];              // 0x10-0x1b
    f32 field_0x1C;               // 0x1c
    u8 _20_2B[0x10];              // 0x20-0x2b
    f32 field_0x2C;               // 0x2c
};

// Actor object reached via CfObjectImplMoveObj::field_0x18; carries the
// field_04 sub-object (b20/b30) and the embedded move sub-object at +0x3e9c.
// vtable slots 0x298 (index 164) / 0x29c (index 165) / 0x2c4 (index 175) are
// dispatched from func_800CEA34 / func_800CE544. Never instantiated, so no
// vtable is emitted.
class CfActorObj {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();   virtual void f9();   virtual void f10();  virtual void f11();
    virtual void f12();  virtual void f13();  virtual void f14();  virtual void f15();
    virtual void f16();  virtual void f17();  virtual void f18();  virtual void f19();
    virtual void f20();  virtual void f21();  virtual void f22();  virtual void f23();
    virtual void f24();  virtual void f25();  virtual void f26();  virtual void f27();
    virtual void f28();  virtual void f29();  virtual void f30();  virtual void f31();
    virtual void f32_(); virtual void f33();  virtual void f34();  virtual void f35();
    virtual void f36();  virtual void f37();  virtual void f38();  virtual void f39();
    virtual void f40();  virtual void f41();  virtual void f42();  virtual void f43();
    virtual void f44();  virtual void f45();  virtual void f46();  virtual void f47();
    virtual void f48();  virtual void f49();  virtual void f50();  virtual void f51();
    virtual void f52();  virtual void f53();  virtual void f54();  virtual void f55();
    virtual void f56();  virtual void f57();  virtual void f58();  virtual void f59();
    virtual void f60();  virtual void f61();  virtual void f62();  virtual void f63();
    virtual void f64();  virtual void f65();  virtual void f66();  virtual void f67();
    virtual void f68();  virtual void f69();  virtual void f70();  virtual void f71();
    virtual void f72();  virtual void f73();  virtual void f74();  virtual void f75();
    virtual void f76();  virtual void f77();  virtual void f78();  virtual void f79();
    virtual void f80();  virtual void f81();  virtual void f82();  virtual void f83();
    virtual void f84();  virtual void f85();  virtual void f86();  virtual void f87();
    virtual void f88();  virtual void f89();  virtual void f90();  virtual void f91();
    virtual void f92();  virtual void f93();  virtual void f94();  virtual void f95();
    virtual void f96();  virtual void f97();  virtual void f98();  virtual void f99();
    virtual void f100(); virtual void f101(); virtual void f102(); virtual void f103();
    virtual void f104(); virtual void f105(); virtual void f106(); virtual void f107();
    virtual void f108(); virtual void f109(); virtual void f110(); virtual void f111();
    virtual void f112(); virtual void f113(); virtual void f114(); virtual void f115();
    virtual void f116(); virtual void f117(); virtual void f118(); virtual void f119();
    virtual void f120(); virtual void f121(); virtual void f122(); virtual void f123();
    virtual void f124(); virtual void f125(); virtual void f126(); virtual void f127();
    virtual void f128(); virtual void f129(); virtual void f130(); virtual void f131();
    virtual void f132(); virtual void f133(); virtual void f134(); virtual void f135();
    virtual void f136(); virtual void f137(); virtual void f138(); virtual void f139();
    virtual void f140(); virtual void f141(); virtual void f142(); virtual void f143();
    virtual void f144(); virtual void f145(); virtual void f146(); virtual void f147();
    virtual void f148(); virtual void f149(); virtual void f150(); virtual void f151();
    virtual void f152(); virtual void f153(); virtual void f154(); virtual void f155();
    virtual void f156(); virtual void f157(); virtual void f158(); virtual void f159();
    virtual void f160(); virtual void f161(); virtual void f162(); virtual void f163();
    virtual CfMoveVf298Result* vf298();   // index 164 -> vtable 0x298
    virtual CfMoveVf29CItem* vf29C(u32 idx);  // index 165 -> vtable 0x29c
    virtual void f166(); virtual void f167(); virtual void f168(); virtual void f169();
    virtual void f170(); virtual void f171(); virtual void f172(); virtual void f173();
    virtual void f174();
    virtual void vf2C4(void* a, f32 x, f32 y, f32 z);  // index 175 -> vtable 0x2c4

    CfActorObj4* field_04;                 // 0x04
    u8 _pad08[0x3E9C - 0x08];              // 0x08-0x3E9B
    CfEmbeddedSubObj_3E9C sub;             // 0x3E9C (embedded move sub-object)
};

// Main object manipulated by this TU's func_* helpers: vtable at 0x00
// (implicit), embedded callback source at 0x0c, driver sub-object at 0x14,
// actor object at 0x18, init fields at 0x1c-0x24, opaque buffer at 0x28
// (address taken by func_800CA964). Never instantiated, so no vtable is
// emitted. With -RTTI on, declared index N -> vtable offset (N+2)*4.
class CfObjectImplMoveObj {
public:
    virtual ~CfObjectImplMoveObj();         // index 0  -> vtable 0x08
    virtual void vf0C();                    // index 1
    virtual void vf10();                    // index 2
    virtual void vf14();                    // index 3
    virtual void vf18();                    // index 4
    virtual void vf1C();                    // index 5
    virtual void vf20();                    // index 6
    virtual void vf24();                    // index 7
    virtual void vf28();                    // index 8
    virtual void vf2C();                    // index 9
    virtual void vf30(u32 a, u32 b);        // index 10 -> vtable 0x30
    virtual void vf34();                    // index 11
    virtual void vf38();                    // index 12
    virtual void vf3C();                    // index 13
    virtual u32 vf40(u32 a);                // index 14 -> vtable 0x40
    virtual void vf44();                    // index 15
    virtual int vf48();                     // index 16 -> vtable 0x48
    virtual void vf4C();                    // index 17
    virtual void vf50();                    // index 18
    virtual void vf54();                    // index 19
    virtual void vf58();                    // index 20
    virtual void vf5C();                    // index 21
    virtual void vf60();                    // index 22
    virtual void vf64();                    // index 23
    virtual void vf68();                    // index 24
    virtual void vf6C();                    // index 25
    virtual void vf70(void* a);             // index 26 -> vtable 0x70
    virtual void vf74();                    // index 27
    virtual void vf78();                    // index 28
    virtual void vf7C();                    // index 29
    virtual void vf80();                    // index 30 -> vtable 0x80
    virtual void vf84(void* a);             // index 31 -> vtable 0x84
    virtual void vf88();                    // index 32
    virtual void vf8C();                    // index 33
    virtual void vf90();                    // index 34
    virtual void vf94();                    // index 35
    virtual void vf98();                    // index 36
    virtual void vf9C();                    // index 37
    virtual void vfA0();                    // index 38
    virtual void vfA4();                    // index 39
    virtual void vfA8();                    // index 40
    virtual void vfAC();                    // index 41
    virtual void vfB0();                    // index 42
    virtual void vfB4();                    // index 43
    virtual void vfB8();                    // index 44
    virtual void vfBC();                    // index 45
    virtual void vfC0();                    // index 46
    virtual void vfC4();                    // index 47
    virtual void vfC8();                    // index 48
    virtual void vfCC();                    // index 49
    virtual void vfD0();                    // index 50
    virtual void vfD4();                    // index 51 -> vtable 0xd4
    virtual void vfD8();                    // index 52 -> vtable 0xd8
    virtual void vfDC();                    // index 53 -> vtable 0xdc
    virtual void vfE0();                    // index 54 -> vtable 0xe0
    virtual CfMoveFieldE4* vfE4();          // index 55 -> vtable 0xe4

    // Layout fields (vptr implicit at 0x00 from the virtuals above)
    u8 _04_0B[0x8];               // 0x04-0x0b
    u8 field_0x0C[0x8];           // 0x0c-0x13 embedded callback source
    CfObjectImplMoveSubObj* mSubObj;  // 0x14
    CfActorObj* field_0x18;       // 0x18
    u8* field_0x1C;               // 0x1c (opaque object pointer)
    u32 field_0x20;               // 0x20
    f32 field_0x24;               // 0x24
    u8 field_0x28[0x10];          // 0x28 (opaque; address taken)
};

// Stack-allocated enum-list holder (func_80043D90 ctor / __dt__80043E88 dtor)
// and the list returned by func_80043F18 (element count at +0x620).
struct CfMoveEnumHolder {
    void* list;   // 0x0
    u32 handle;   // 0x4
};

struct CfMoveEnumList {
    u8 _pad00[0x620];                          // 0x00-0x61F
    u32 field_620;                             // 0x620
};

// First word of the CBattleManager::func_800EA444 result (func_800CEA34).
struct CfMoveBMId {
    u32 field_0;                               // 0x00
};

// --- imports referenced by this TU (retail symbol names) ---

extern const f32 lbl_eu_80666C60;       // float constant, SDA21 (r2)

// C++-mangled retail helper func_800B708C__Fi (actor id -> source).
extern void* func_800B708C(int id);

// func_8006EF04__Fi: presentation/event flag-bit probe (mangled C++ match).
extern bool func_8006EF04(int mask);

// C-linkage imports (retail symbol names are unmangled - keep verbatim).
extern "C" {
void* func_8016FE34(void* source);
void func_80482AB8(u32 id, void* source);
void* func_800EA444(void* bm);
void* getInstance__Q22cf14CBattleManagerFv();
void* getInstance__Q22cf13CfGameManagerFv();
void func_802A0E08(void* self);
void func_802A31AC(void* a, void* b, void* c);
u32 func_80174C98(void* actor, u32* outVal, u32 flags);
void func_80043D90(CfMoveEnumHolder* holder);
CfMoveEnumList* func_80043F18(CfMoveEnumHolder* holder);
void func_800F4A98(CfMoveEnumList* list, u32 type, u32 filter);
void func_800F6ED0(CfMoveEnumList* list, u32 value);
void* func_800F6EAC(CfMoveEnumList* list, u32 idx);
void* func_800F6E08(CfMoveEnumList* list);
void __dt__80043E88(CfMoveEnumHolder* holder, int flags);

// The CfObjectImplMove destructor, called directly by the func_800CFFBC /
// func_800CFFC4 adjusting thunks. The C++ member form would emit the 2-arg
// dtor ABI (li r4, delete-flag) plus __FPv mangling; the retail thunks are
// 1-arg tail calls to the unmangled symbol, so keep C linkage here.
void __dt__Q22cf16CfObjectImplMoveFv(void* self);
int func_80148778(void* obj, int id);
void* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);
void func_800ACFD8(void* obj, void* target);
void func_804E3CDC(void* effect, f32 f1, f32 f2);
}

// Retail float constants (sdata2 pool) used by func_800CC638.
extern const f32 lbl_eu_80666C88;   // event-object f17() comparison threshold
// sdata2 int->float magic (2^52 = 0x4330000000000000) for the 0x43300000
// conversion (CfMapEffectManager.hpp convention).
extern const f64 lbl_eu_80666C90;
extern const f32 lbl_eu_80666C64;   // func_804E3CDC second argument

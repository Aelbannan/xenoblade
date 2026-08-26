#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

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

class CfMoveFieldE4;
// Forward views used by the class declarations below.
struct CfVf218Result;
struct CfVf2A048Result;
struct CfBoundObj;

// Vtable-view replacement for the driver sub-object dispatch: genuine virtual
// calls reproduce retail's r12 dispatch (lwz r12,0(rX); lwz r12,slot(r12)).
// Slots 0xAC/0x128 return position objects, slot 0x220 takes the move id.
class CfObjectImplMoveSubObj {
public:
    virtual void* f00(); virtual u32 f01(u32 a); virtual void* f02(u32 a); virtual void* f03();
    virtual void* f04(u32 a); virtual void* f05(); virtual void* f06(u32 a);
    virtual void* f07(u32 a);
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
    virtual void* f54();
    virtual CfMoveFieldE4* vfnE4();   // index 55 -> vtable 0xe4 (returns the field-E4 object)
    virtual void* f56(); virtual void* f57();
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
    virtual void* f125();
    virtual void* vfn200();           // index 126 -> vtable 0x200
    virtual void* f127(); virtual void* f128();
    virtual void* f129(); virtual void* f130(); virtual void* f131(); virtual void* f132();
    virtual void* f133();
    virtual void* vfn220(u16 id);     // index 134 -> vtable 0x220

    // vptr occupies 0x00-0x03; fields keep their absolute retail offsets.
    u8 _04_63[0x60];              // 0x04-0x63
    u32 field_0x64;               // 0x64 (flags: bit 1 = 0x2, bit 7 = 0x80)
    u8 _68_73[0x74 - 0x68];       // 0x68-0x73
    u32 field_0x74;               // 0x74 (sound-arg handle word)
    u8 _78_8B[0x8c - 0x78];       // 0x78-0x8b
    u16 field_0x8C;               // 0x8c (halfword compared against 6)
    u8 _8E_8F[2];                 // 0x8e-0x8f
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
    virtual u32 f01(u32 a);       // index 1  -> vtable 0x0c
    virtual void* f02(u32 a);     // index 2  -> vtable 0x10
    virtual void* f03();          // index 3
    virtual void* f04(u32 a);     // index 4  -> vtable 0x18
    virtual void* f05();          // index 5
    virtual void* f06(u32 a);     // index 6  -> vtable 0x20
    virtual void* f07(u32 a);     // index 7  -> vtable 0x24
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
    virtual void* f32(); virtual void* f33();
    virtual void vfn90(float v);  // index 34 -> vtable 0x90 (presentation float)
    virtual float vfn94();        // index 35 -> vtable 0x94 (float query)
    virtual void* f36(); virtual void* f37(); virtual void* f38(); virtual void* f39();
    virtual void* f40(); virtual void* f41(); virtual void* f42(); virtual void* f43();
    virtual void* f44(); virtual void* f45(); virtual void* f46(); virtual void* f47();
    virtual void* f48(); virtual void* f49(); virtual void* f50(); virtual void* f51();
    virtual void* f52(); virtual void* f53(); virtual void* f54(); virtual void* f55();
    virtual void* f56(); virtual void* f57(); virtual void* f58(); virtual void* f59();
    virtual void* f60(); virtual void* f61(); virtual void* f62(); virtual void* f63();
    virtual void* f64(); virtual void* f65();
    virtual CfBoundObj* vfn110(); // index 66 -> vtable 0x110 (bound-object fetch)
    virtual void* f67(); virtual void* f68(); virtual void* f69(); virtual void* f70();
    virtual void* f71();
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
    virtual void* f128(u32 a);    // index 128 -> vtable 0x208
    virtual void* vfn20C(u32 a);  // index 129 -> vtable 0x20c
    virtual u32 vfn210(u32 a);    // index 130 -> vtable 0x210

    // Data view over the embedded copy inside CfActorObj (offsets below the
    // vptr are relative to 0x3e9c): event-callback registration at +0x98
    // (actor 0x3F34) and battle id at +0xc4 (actor 0x3F60).
    u8 _04_63[0x60];              // 0x04-0x63
    u32 field_64;                 // +0x64 (flag word; bit 1 = 0x2)
    u8 _68_71[0x74 - 0x68];       // 0x68-0x71
    u32 field_74;                 // +0x74 (actor 0x3F10)
    u8 _78_8B[0x8c - 0x78];       // 0x78-0x8b
    u16 field_0x8C;               // +0x8c (actor 0x3F28 state halfword)
    u8 _8E_97[0x98 - 0x8e];       // 0x8e-0x97
    void* field_98;               // +0x98 (actor 0x3F34, func_80482AD4 target)
    u8 _9C_C3[0x28];              // 0x9c-0xc3
    u32 field_C4;                 // +0xc4 (actor 0x3F60, battle id)
    u8 _C8_6F7[0x6f8 - 0xc8];     // 0xc8-0x6f7
    void* field_6F8;              // +0x6f8 (partner object)
    void* field_6FC;              // +0x6fc (partner object)
    u8 _700_71B[0x71c - 0x700];   // 0x700-0x71b
    u32 field_71C;                // +0x71c (actor 0x45b8 partner-object link)
};

// Result of CfActorObj::vf298() (vtable 0x298): +0x4 feeds func_800B708C's
// actor-id lookup (func_800CC638); +0x50 is a battle-state block pointer
// (func_800CEA34 reads its kind/count bytes at +0x43/+0x44).
struct CfMoveVf298Result {
    u8 _00_03[0x4];               // 0x00-0x03
    u32 field_0x4;                // 0x04 (actor id)
    u8 _08_47[0x48 - 0x08];       // 0x08-0x47
    u32 field_48;                 // 0x48 (knockback strength, func_800CD5DC)
    u8 _4C_4F[0x50 - 0x4c];       // 0x4c-0x4f
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
    u8 _08_47[0x48 - 0x8];
    u16 field_0x48;               // 0x48 (state halfword)
    u8 _4A_4F[0x50 - 0x4a];
    void* field_0x50;             // 0x50 (chained item pointer)
    u8 _54_77[0x78 - 0x54];
    u32 field_0x78;               // 0x78 (flag word)
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

// Object attached at CfObjectImplMoveObj::field_0x1C (func_800CAA44 clears a
// flag word at +0xb0 and sets bit 0x40 of the word at +0x68).
struct CfMoveField1CObj {
    u8 _00_67[0x68];              // 0x00-0x67
    u32 field_0x68;               // 0x68 (flag word)
    u8 _6C_AF[0xb0 - 0x6c];       // 0x6c-0xaf
    u32 field_0xB0;               // 0xb0 (cleared on reset)
};

// Event/effect state block bound at actor +0x3F60 (embedded sub-object's
// +0xc4 pointer). Fields read by func_800CC020 / func_800CF810.
struct CfMoveEvt60 {
    u8 _00_0B[0xc];               // 0x00-0x0b
    u32 field_0xC;                // 0xc (flag word; bit 30 probed)
    u8 _10_E7[0x2e8 - 0x10];
    f32 field_2E8;                // 0x2e8 (effect scale source)
    u8 _2EC_97[0x39c - 0x2ec];
    f32 field_39C;                // 0x39c
    u8 _3A0_C3[0x3c4 - 0x3a0];
    f32 field_3C4;                // 0x3c4
    u8 _3C8_43[0x444 - 0x3c8];
    f32 field_444;                // 0x444
    u8 _448_B[0x4ac - 0x448];
    u32 field_4AC;                // 0x4ac (state id)
    u8 _4B0_B[0x4ec - 0x4b0];
    u32 field_4EC;                // 0x4ec (flag word)
    f32 field_4F8;                // 0x4f8 (elapsed timer)
    f32 field_4FC;                // 0x4fc
    u8 _500_5F[0x760 - 0x500];
    f32 field_760;                // 0x760 (presentation position)
    f32 field_764;                // 0x764
    f32 field_768;                // 0x768
};

// Game-manager effect-object view: slot 0x9c takes a position vector, slot
// 0xbc takes a 3-float angle triple.
class CfMoveMgrEfView {
public:
    virtual void* g00(); virtual void* g01(); virtual void* g02(); virtual void* g03();
    virtual void* g04(); virtual void* g05(); virtual void* g06(); virtual void* g07();
    virtual void* g08(); virtual void* g09(); virtual void* g10(); virtual void* g11();
    virtual void* g12(); virtual void* g13(); virtual void* g14(); virtual void* g15();
    virtual void* g16(); virtual void* g17(); virtual void* g18(); virtual void* g19();
    virtual void* g20(); virtual void* g21(); virtual void* g22(); virtual void* g23();
    virtual void* g24(); virtual void* g25(); virtual void* g26(); virtual void* g27();
    virtual void* g28(); virtual void* g29(); virtual void* g30(); virtual void* g31();
    virtual void* g32(); virtual void* g33(); virtual void* g34(); virtual void* g35();
    virtual void* g36();
    virtual void vfn9C(CfMoveVec3f* vec);      // offset 0x9c
    virtual void* g38(); virtual void* g39(); virtual void* g40(); virtual void* g41();
    virtual void* g42(); virtual void* g43(); virtual void* g44();
    virtual void vfBC(f32* angs);               // offset 0xbc
};

// Result object behind CfActorObj::f139() (vtable 0x234): float at +0x00.
struct CfMoveF139Result {
    f32 field_0;
};

// Object referenced by the embedded sub-object's +0xc4 pointer
// (func_800CB454 probes bit 0x2 of its +0x4ec word).
struct CfMoveC4Obj {
    u8 _00_73[0x374];
    u32 field_374;                // 0x374 (compared against 5)
    u8 _378_E8[0x4ec - 0x378];
    u32 field_4EC;
};

// Event object behind the embedded sub-object's +0x98 registration slot
// (actor +0x3f34), as dispatched by func_800CEE80: slots 0x40/0x44 manage the
// cached effect handle, the +0x7a4 flag word gates the query, +0x14ac is a
// fallback result object, and +0x760..0x768 is a presentation position triple.
class CfMoveEvt98 {
public:
    virtual void* p00(); virtual void* p01(); virtual void* p02(); virtual void* p03();
    virtual void* p04(); virtual void* p05(); virtual void* p06(); virtual void* p07();
    virtual void* p08(); virtual void* p09(); virtual void* p10(); virtual void* p11();
    virtual void* p12(); virtual void* p13();
    virtual void* vf40();             // index 14 -> vtable 0x40
    virtual u32 vf44();               // index 15 -> vtable 0x44

    u8 _04_75F[0x760 - 0x04];         // 0x04-0x75f
    f32 field_760;                    // 0x760 (presentation position)
    f32 field_764;                    // 0x764
    f32 field_768;                    // 0x768
    u8 _76C_7A3[0x7a4 - 0x76c];       // 0x76c-0x7a3
    u32 field_0x7A4;                  // 0x7a4 (flag word; bit 30 probed)
    u8 _7A8_14AB[0x14ac - 0x7a8];     // 0x7a8-0x14ab
    u32 field_0x14AC;                 // 0x14ac (fallback result object)
};

// Contact parameter block handed to func_800CF810.
struct CfMoveContact {
    u8 _00_09[0xa];
    u8 field_0xA;                 // 0xa (request mode byte)
    u8 _0B_0F[5];
    f32 field_10[3];              // 0x10 local-space contact point
    u8 field_1C;                  // 0x1c flag byte
    u8 _1D_20[3];
};

// Driver sub-object view for slot 0x120 (index 70).
class CfDriverSlot120 {
public:
    virtual void* d00(); virtual void* d01(); virtual void* d02(); virtual void* d03();
    virtual void* d04(); virtual void* d05(); virtual void* d06(); virtual void* d07();
    virtual void* d08(); virtual void* d09(); virtual void* d10(); virtual void* d11();
    virtual void* d12(); virtual void* d13(); virtual void* d14(); virtual void* d15();
    virtual void* d16(); virtual void* d17(); virtual void* d18(); virtual void* d19();
    virtual void* d20(); virtual void* d21(); virtual void* d22(); virtual void* d23();
    virtual void* d24(); virtual void* d25(); virtual void* d26(); virtual void* d27();
    virtual void* d28(); virtual void* d29(); virtual void* d30(); virtual void* d31();
    virtual void* d32(); virtual void* d33(); virtual void* d34(); virtual void* d35();
    virtual void* d36(); virtual void* d37(); virtual void* d38(); virtual void* d39();
    virtual void* d40(); virtual void* d41(); virtual void* d42(); virtual void* d43();
    virtual void* d44(); virtual void* d45(); virtual void* d46(); virtual void* d47();
    virtual void* d48(); virtual void* d49(); virtual void* d50(); virtual void* d51();
    virtual void* d52(); virtual void* d53(); virtual void* d54(); virtual void* d55();
    virtual void* d56(); virtual void* d57(); virtual void* d58(); virtual void* d59();
    virtual void* d60(); virtual void* d61(); virtual void* d62(); virtual void* d63();
    virtual void* d64(); virtual void* d65(); virtual void* d66(); virtual void* d67();
    virtual void* d68(); virtual void* d69();
    virtual void* vfn120(void* a);   // index 70 -> vtable 0x120
};

// Wide view of the handler object embedded at actor +0x08 (used by
// func_800CAB30 case bodies): flag word at +0x1528 gates a slot-0x20
// dispatch whose argument is that same word.
class CfMoveHandlerWide {
public:
    virtual void* w00();
    virtual void* w04();
    virtual void* w08();
    virtual void* w0C();
    virtual void* w10();
    virtual void* w14();
    virtual void* w20(u32 id);   // index 6 -> vtable 0x20
    virtual void* w24();         // index 7

    u8 _04_1527[0x1528 - 0x04];
    u32 field_0x1528;            // +0x1528 (dispatch gate / argument)
};

// Handler object embedded at actor +0x08 (slot 0x20 takes a handler id).
class CfMoveHandler8 {
public:
    virtual void* h00();
    virtual void* h04();
    virtual void* h08();
    virtual void* h0C();
    virtual void* h10();
    virtual void* h14();
    virtual void* h20(u32 id);   // index 6 -> vtable 0x20
    virtual void* h24();         // index 7
    virtual void* h28();         // index 8
    virtual void* h2C();         // index 9
    virtual void* h30(u32 id);   // index 10 -> vtable 0x30
};

// Event parameter block handed to func_800CB454 / func_800CAB30:
// event id halfword at +0xc, presentation halfword at +0x2e, flag word +0x30.
struct CfMoveEvtParam {
    u8 _00_0C[0xc];
    u16 field_C;
    u8 _0E_2E[0x20];
    u16 field_2E;
    u16 _pad30;
    u32 field_30;
};

// 0x20-byte request buffer built on the stack by func_800CB454 and handed to
// func_8014AC38. Bytes 0x04-0x11 are cleared by one memset, then individual
// fields are stored.
struct CfMoveAcReqBody {
    u8 _04_05[2];                 // 0x04-0x05
    u8 field_6;                   // 0x06
    u8 _07_0C[6];                 // 0x07-0x0c
    u8 field_D;                   // 0x0d
    u8 _0E_0F[2];                 // 0x0e-0x0f
    u16 field_10;                 // 0x10
};

struct CfMoveAcReq {
    union {
        CfMoveAcReqBody body;     // named-field view of 0x04..0x11
        u8 raw[0xe];              // memset target (0x04-0x11)
    } at4;
    u16 field_12;                 // 0x12
    f32 field_14;                 // 0x14 (float constant)
    u8 _18_1F[8];
};

// Wide view of the vf29C() item (func_800CB454 clears bit groups of +0x74).
struct CfMoveVf29CX74 {
    u8 _00_73[0x74];
    u32 field_74;
};

// Move-state bookkeeping block embedded in CfActorObj at +0x3380.
struct CfActorMstBlock {
    u8 buf[4];                    // 0x3380 (effect-list buffer)
    u32 field_4;                  // 0x3384
    u16 field_8;                  // 0x3388
    u8 _padA[0x210 - 0xa];
    u32 field_210;                // 0x3590
    u32 field_214;                // 0x3594
    u8 _pad218[0xafc - 0x218];
    u32 field_afc;                // 0x3E7C
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
    virtual void vf98(u32 id);  virtual void f37();  virtual void f38();  virtual void f39();
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
    virtual CfVf218Result* vf218(); virtual void f133(); virtual void f134(); virtual void f135();
    virtual void f136(); virtual void f137(); virtual void* f138(); virtual CfMoveF139Result* f139();
    virtual void f140(); virtual void f141(); virtual void f142(); virtual void f143();
    virtual void f144(); virtual void f145(); virtual void f146(); virtual void f147();
    virtual void f148(); virtual void f149(); virtual void f150(); virtual void f151();
    virtual void f152(); virtual void f153(); virtual void f154(); virtual void f155();
    virtual void f156(); virtual void f157(); virtual void f158(); virtual void f159();
    virtual void f160(); virtual void f161(); virtual void f162(); virtual void f163();
    virtual CfMoveVf298Result* vf298();   // index 164 -> vtable 0x298
    virtual CfMoveVf29CItem* vf29C(u32 idx);  // index 165 -> vtable 0x29c
    virtual CfVf2A048Result* vf2A0();         // index 166 -> vtable 0x2a0
    virtual CfVf2A048Result* vf2A4();         // index 167 -> vtable 0x2a4
    virtual void* vf2A8();                    // index 168 -> vtable 0x2a8
    virtual void* vf2AC();                    // index 169 -> vtable 0x2ac
    virtual void* vf2B0();                    // index 170 -> vtable 0x2b0
    virtual void* f171(); virtual void* f172(); virtual void* f173();
    virtual void f174();
    virtual void vf2C4(void* a, f32 x, f32 y, f32 z);  // index 175 -> vtable 0x2c4
    // Fillers up to the slots dispatched by func_800CD5DC's partner tail.
    virtual void* f176(); virtual void* f177(); virtual void* f178(); virtual void* f179();
    virtual void* f180(); virtual void* f181(); virtual void* f182(); virtual void* f183();
    virtual void* f184(); virtual void* f185(); virtual void* f186(); virtual void* f187();
    virtual void* f188(); virtual void* f189(); virtual void* f190(); virtual void* f191();
    virtual void* f192(); virtual void* f193(); virtual void* f194(); virtual void* f195();
    virtual void* f196(); virtual void* f197(); virtual void* f198(); virtual void* f199();
    virtual void* f200(); virtual void* f201(); virtual void* f202(); virtual void* f203();
    virtual void* f204(); virtual void* f205(); virtual void* f206(); virtual void* f207();
    virtual void* f208(); virtual void* f209(); virtual void* f210(); virtual void* f211();
    virtual void* f212(); virtual void* f213(); virtual void* f214(); virtual void* f215();
    virtual void* f216(); virtual void* f217(); virtual void* f218(); virtual void* f219();
    virtual void* f220(); virtual void* f221(); virtual void* f222(); virtual void* f223();
    virtual void* f224(); virtual void* f225(); virtual void* f226(); virtual void* f227();
    virtual void* f228(); virtual void* f229(); virtual void* f230(); virtual void* f231();
    virtual void* f232(); virtual void* f233(); virtual void* f234(); virtual void* f235();
    virtual void* f236(); virtual void* f237(); virtual void* f238(); virtual void* f239();
    virtual void* f240(); virtual void* f241(); virtual void* f242(); virtual void* f243();
    virtual void* f244(); virtual void* f245(); virtual void* f246(); virtual void* f247();
    virtual void* f248(); virtual void* f249(); virtual void* f250(); virtual void* f251();
    virtual void* f252(); virtual void* f253(); virtual void* f254(); virtual void* f255();
    virtual void* f256(); virtual void* f257(); virtual void* f258(); virtual void* f259();
    virtual void* f260(); virtual void* f261(); virtual void* f262(); virtual void* f263();
    virtual void* f264(); virtual void* f265(); virtual void* f266(); virtual void* f267();
    virtual void* f268(); virtual void* f269(); virtual void* f270(); virtual void* f271();
    virtual void* f272(); virtual void* f273(); virtual void* f274(); virtual void* f275();
    virtual void* f276(); virtual void* f277(); virtual void* f278(); virtual void* f279();
    virtual void* f280(); virtual void* f281(); virtual void* f282(); virtual void* f283();
    virtual void* f284(); virtual void* f285(); virtual void* f286(); virtual void* f287();
    virtual void* f288(); virtual void* f289(); virtual void* f290(); virtual void* f291();
    virtual void* f292(); virtual void* f293(); virtual void* f294(); virtual void* f295();
    virtual void* f296(); virtual void* f297(); virtual void* f298(); virtual void* f299();
    virtual void* f300(); virtual void* f301(); virtual void* f302(); virtual void* f303();
    virtual void* f304(); virtual void* f305(); virtual void* f306(); virtual void* f307();
    virtual void* f308(); virtual void* f309(); virtual void* f310(); virtual void* f311();
    virtual void* f312(); virtual void* f313(); virtual void* f314(); virtual void* f315();
    virtual void* f316(); virtual void* f317(); virtual void* f318(); virtual void* f319();
    virtual void* f320(); virtual void* f321(); virtual void* f322(); virtual void* f323();
    virtual void* f324(); virtual void* f325(); virtual void* f326(); virtual void* f327();
    virtual void* f328(); virtual void* f329(); virtual void* f330(); virtual void* f331();
    virtual void* f332(); virtual void* f333(); virtual void* f334(); virtual void* f335();
    virtual void* f336(); virtual void* f337(); virtual void* f338(); virtual void* f339();
    virtual void* f340(); virtual void* f341(); virtual void* f342(); virtual void* f343();
    virtual void* f344(); virtual void* f345(); virtual void* f346(); virtual void* f347();
    virtual void* f348(); virtual void* f349(); virtual void* f350(); virtual void* f351();
    virtual void* f352(); virtual void* f353(); virtual void* f354(); virtual void* f355();
    virtual void* f356(); virtual void* f357(); virtual void* f358(); virtual void* f359();
    virtual void* f360(); virtual void* f361(); virtual void* f362(); virtual void* f363();
    virtual void* f364(); virtual void* f365(); virtual void* f366(); virtual void* f367();
    virtual void* vf5c8(void* a);              // index 368 -> vtable 0x5c8
    virtual void* vf5cc();                     // index 369 -> vtable 0x5cc
    virtual void* f370();
    virtual void* vf5d0();                     // index 371 -> vtable 0x5d0

    CfActorObj4* field_04;                 // 0x04
    u8 field_08[8];                        // 0x08 (handler block probed by func_80148778)
    u8 _pad10[0x74 - 0x10];
    u32 field_74;                          // 0x74 (bit-group clear target)
    u8 _pad78b[0x3374 - 0x78];
    u32 field_3374;                        // 0x3374
    u8 _pad78[0x3380 - 0x3378];
    CfActorMstBlock mst;                   // 0x3380 (move-state bookkeeping)
    u8 _padE80[0x3E98 - 0x3E80];
    u32 field_3E98;                        // 0x3E98
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
    virtual void vf70(void* a = 0);         // index 26 -> vtable 0x70
    virtual void vf74();                    // index 27
    virtual void vf78();                    // index 28
    virtual void vf7C();                    // index 29
    virtual void vf80();                    // index 30 -> vtable 0x80
    virtual void vf84(void* a);             // index 31 -> vtable 0x84
    virtual void vf88();                    // index 32
    virtual void vf8C();                    // index 33
    virtual void vf90();                    // index 34
    virtual void vf94(u32 a);               // index 35 -> vtable 0x94
    virtual void vf98(u32 a);               // index 36 -> vtable 0x98
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
    void* field_0x04;             // 0x04 (request word stored by func_800CEB68)
    u8 _08_0B[4];                 // 0x08-0x0b
    u8 field_0x0C[0x8];           // 0x0c-0x13 embedded callback source
    CfObjectImplMoveSubObj* mSubObj;  // 0x14
    CfActorObj* field_0x18;       // 0x18
    CfMoveField1CObj* field_0x1C; // 0x1c (attached flag-word object)
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

// CBattleManager::func_800EA444 result view reading the flag word at +0x824
// (func_800CD5DC probes bit 11).
struct CfMoveBM824 {
    u8 _00_823[0x824];                         // 0x00-0x823
    u32 field_824;                             // 0x824 (flag word; bit 11)
};

// Result of the embedded sub-object's slot 0x218 (func_800CD5DC knockback
// scaling): signed halfword at +0x28.
struct CfVf218Result {
    u8 _00_27[0x28];                           // 0x00-0x27
    s16 field_28;                              // 0x28
};

// Result view shared by CfActorObj::vf2A0 / vf2A4: compared words at +0x48.
struct CfVf2A048Result {
    u8 _00_47[0x48];                           // 0x00-0x47
    u32 field_48;                              // 0x48
};

// Object returned by the embedded sub-object's slot 0x110: its +0x18 is the
// bound move-driver object swapped by func_800CD5DC's partner tail.
struct CfBoundObj {
    u8 _00_17[0x18];                           // 0x00-0x17
    void* field_0x18;                          // 0x18 (bound move object)
};

// Battle-manager gate view for func_800CC964: battle-intro counter halfword
// at +0x20c8 suppresses the 0x27 request while non-zero.
struct CBattleManagerCcGate {
    u8 _00_20C7[0x20c8];
    s16 field_20C8;                            // 0x20c8 (battle intro counter)
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
void func_8014B2DC(void* buf); // canonical void* form (CtrlAct.hpp/ImplPc.hpp)
void func_80482AB8(u32 id, void* source);
void* func_800EA444(void* bm);
#include "kyoshin/cf/CfMapItemManager.hpp" // func_80174C98 (owner decl)
void* getInstance__Q22cf13CfGameManagerFv();
void func_802A0E08(void* self);
void func_802A31AC(void* a, void* b, void* c);
// Enum-list helper family: canonical extern "C" void* forms (CVision.hpp).
void func_80043D90(void* holder);
void* func_80043F18(void* holder);
void func_800F4A98(void* list, u32 type, u32 filter);
void func_800F6ED0(void* list, void* value); // canonical (void*,void*) form (CAIAction/ImplPc/ImplWalker)
void* func_800F6EAC(void* list, u32 idx);
void* func_800F6E08(void* list);
void __dt__80043E88(void* holder, int flags);

// The CfObjectImplMove destructor, called directly by the func_800CFFBC /
// func_800CFFC4 adjusting thunks. The C++ member form would emit the 2-arg
// dtor ABI (li r4, delete-flag) plus __FPv mangling; the retail thunks are
// 1-arg tail calls to the unmangled symbol, so keep C linkage here.
void __dt__Q22cf16CfObjectImplMoveFv(void* self);
int func_80148778(void* obj, int id);
void func_8014AE00(void* buf);   // move-state buffer init (actor +0x3380)
void* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);
void func_800ACFD8(void* obj, void* target);
void func_804E3CDC(void* effect, f32 f1, f32 f2);
void func_80482AD4(void* handler, void* source);
void func_8015BD94(void* effect);
void func_802A0FE8(void* self);
void* func_80496264(void* scene, int index);  // scene pose lookup (func_800CD460)
void func_8007B044(void* shake, int flag);    // camera-shake dispatch (func_800CD460)
}

// Talk-source getter (cf::CfObjectModel.cpp) and battle-entry helper.
// Retail symbol is unmangled - keep C linkage.
extern "C" void func_800CB21C(CfObjectImplMoveObj* self, u32 id);

// Battle-manager helpers dispatched by func_800CD5DC (retail symbols are
// unmangled; owners: CfObjectImplEne.cpp / CBattleManager.cpp).
extern "C" int func_800DA06C(void* mgr, void* obj);
extern "C" void func_800D9978(void* mgr, void* obj);
extern "C" void func_800D9CA0(void* mgr, void* target);
extern "C" void func_800DA0A4(void* mgr, void* actor, u32 param);
// Voice-manager hook fired at the end of func_800CD5DC's case 8 body.
extern "C" void func_802A2D0C(void* actor);
// Vision-system refresh (code_800F42AC.cpp), called by func_800CD5DC.
extern "C" void func_800F449C(void* obj);
// Battle-status add helper (CActParamAnimGame.cpp), used by func_800CD5DC.
extern "C" void func_8004CEF8(void* obj, u32 param);

// Event dispatcher defined below in this TU; retail symbol is unmangled, so
// declare it with C linkage here (the definition below inherits it).
extern "C" void func_800CB9AC(void* self, u32 id); // canonical void* form (shared with CfObjectImplPc.hpp)
// (CBattleManager.cpp): retail symbols are unmangled, so keep C linkage
// (same pattern as the other imports above).
extern "C" void* func_800BBC0C(void* objParam);
extern "C" void func_800E1B5C(void* mgr, void* battleObj);

// getInstance__Q22cf14CBattleManagerFv: the one shared declaration lives in
// kyoshin/cf/CBattleManagerApi.hpp (included at the top of this header).
// Camera-shake parameter block
// vectors at +0x04 / +0x10 are scaled by the distance falloff factor before
// the call.
struct CfMoveCd460Shake {
    f32 field_00;                   // 0x00 intensity
    CfMoveVec3f vecA;               // 0x04
    CfMoveVec3f vecB;               // 0x10
    u8 _1C_34[0x34 - 0x1c];
};

// Parameter block for func_800CD460: embedded shake data plus a distance
// threshold (0 means derive it from the event object's effect scale).
struct CfMoveCd460Arg {
    u8 _00_0B[0xc];
    CfMoveCd460Shake shake;         // 0x0c-0x3f
    f32 field_40;                   // 0x40 threshold override
};

// Move-target object view (position at +0x3a8, cf/object/CfObjectMove.hpp).
struct CfMoveCd460Target {
    u8 _00_3A7[0x3a8];
    CfMoveVec3f pos;                // 0x3a8
};

// Scene pose block returned by func_80496264(scene, -1) (position at +0x10c).
struct CfMoveCd460Pose {
    u8 _00_10B[0x10c];
    CfMoveVec3f pos;                // 0x10c
};


// Retail float constants (sdata2 pool) used by func_800CC638.
extern const f32 lbl_eu_80666C88;   // event-object f17() comparison threshold
extern const f32 lbl_eu_80666C68;   // move-distance base threshold
// sdata2 int->float magic (2^52 = 0x4330000000000000) for the 0x43300000
// conversion (CfMapEffectManager.hpp convention).
extern const f64 lbl_eu_80666C90;
extern const f32 lbl_eu_80666C64;   // func_804E3CDC second argument
extern const f32 lbl_eu_80666C78;   // func_800CD5DC knockback divisor
// sdata2 magic paired with the 0x43300000 int->float conversion.
extern const f64 lbl_eu_80666C80;

extern u32 lbl_eu_80663EF0;
class CScn;  // monolib scene (canonical decl: cf/object/CfObjectMove.hpp)

// float-returning probe over the lbl_eu_80663E14 object (retail symbol is
// unmangled - keep C linkage, declared above).
extern const f32 lbl_eu_80666C6C;
extern const f32 lbl_eu_80666C70;
extern const f32 lbl_eu_80666C74;
extern const f32 lbl_eu_80666C7C;
extern const f32 lbl_eu_80666CA8;
extern const f32 lbl_eu_80666C98;
extern const f32 lbl_eu_80666CCC;
extern const f32 lbl_eu_80666CD0;
extern const f32 lbl_eu_80666CD4;
extern const f32 lbl_eu_80666C9C;
extern const f32 lbl_eu_80666CA0;   // func_800CEE80 timer threshold / vec.y seed
extern const f32 lbl_eu_80666CA4;   // func_800CEE80 final field_0x24 store
extern const f32 lbl_eu_80666CAC;
extern const f32 lbl_eu_80666CB0;
extern const f32 lbl_eu_80666CB4;
extern const f32 lbl_eu_80666CB8;
extern const f32 lbl_eu_80666CBC;
extern const f32 lbl_eu_80666CC0;
extern const f32 lbl_eu_80666CC8;
extern const f32 lbl_eu_8066A200;   // pi/2 clamp (sdata2)
extern const char lbl_eu_8052ADB0[]; // nw4r::db::Warning source file
extern const char lbl_eu_8052AD88[]; // nw4r::db::Warning message
extern const char lbl_eu_80526300[]; // nw4r::db::Warning message
extern const char lbl_eu_80526324[]; // nw4r::db::Warning source file
extern const f32 lbl_eu_80666CC4;    // dot-product threshold

// nw4r math FIdx trig helpers (retail symbols unmangled).
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" f32 CosFIdx__Q24nw4r4mathFf(f32);
extern "C" f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32 value);

namespace nw4r {
namespace db {
void Warning(const char* file, int line, const char* fmt, ...);
} // namespace db
} // namespace nw4r

// cf::CfGameManager::getPlayer(int) - extern "C" keeps MWCC from
// re-mangling the pre-mangled retail name (CPartsChange.hpp convention).
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);

// Volatile here: func_800CEE80 performs a dead reload of this word (retail
// keeps the load), so the compiler must not elide it.
extern volatile u32 lbl_eu_80661D40;

// Presentation / sound / effect helpers used by the move-event dispatchers.
extern "C" void func_801A891C(void* actor, int param); // canonical (void*,int) form (CfObjectActor.hpp)
extern "C" void func_80174C24(void* actor, u32 mask);
extern "C" void* func_800F477C(void);
extern "C" void func_8014AC38(void* buf, void* req); // canonical void* form (CVision.hpp/CtrlPc.hpp)
#include <string.h>
// func_8049603C is declared once in libs/monolib/src/scn/CScn_8049603C.hpp;
// do not redeclare it here.
// Single shared flat-name form (CfGimmickEne/CfGimmickObject/CVision/
// CfMapMineManager/CPartsChange convention).
extern "C" u16 func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(
    u32 soundMan, u32 a, u32 b, u32 c, f32 e);
// Same form as CfResPcImpl/CfResReloadImpl/CfResObjImpl (single shared
// signature; retail function returns the sound id).
extern "C" int func_801BFE20(int a, int b, u8* c, float f1, float f2);
extern "C" void func_800ACC64(void* a, void* b);
extern "C" void func_801BFDE8(u32 mode, u32 value, u32 playerValue, f32 first,
    f32 second); // single shared uint form (CfGimmick.hpp / UnityHelpers)
extern "C" void func_801BFE8C(u32 a, u32 b, u32 c); // canonical u32 form (CVision/CfResReload/ImplPc/ImplEne)
extern "C" void* func_8048315C(void);
// func_800CF810 shape: ground/screen probe writing the adjusted position and
// taking the source vector plus a float constant.
// Canonical form (matches CPartsChange.hpp / CtrlMoveBase.hpp / CfCam.cpp).
// Call sites needing another ABI cast through a local fn-pointer typedef
// (CfCam BE398Fn convention).
extern "C" int func_804BE398(void* vec, int a, int b, int c, f32 d, f32 e); // int (not u32) to match CtrlMoveBase.hpp - u32/int here made the two extern "C" decls distinct signatures (illegal overloading) in every TU seeing both
extern "C" void func_804BE4B4(void* a, int b);
extern "C" void func_804BE4E0(void* a, int b);
// (remaining helpers are declared by their owning headers)

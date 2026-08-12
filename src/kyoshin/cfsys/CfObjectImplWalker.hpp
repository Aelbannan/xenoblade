#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>

namespace cf {

// Walker interface object. Only the vtable slots and fields this TU touches
// are declared; with -RTTI on MWCC reserves two leading vtable slots
// (offset-to-top + typeinfo), so declared index N sits at vtable offset
// (N+2)*4. The class is never instantiated here, so no vtable is emitted.
class CfObjectImplWalker {
public:
    virtual ~CfObjectImplWalker();              // index 0 -> vtable 0x8

    virtual void vf0C();                        // index 1
    virtual void vf10();                        // index 2
    virtual void vf14();                        // index 3
    virtual void vf18();                        // index 4
    virtual void vf1C();                        // index 5
    virtual void vf20();                        // index 6
    virtual void vf24();                        // index 7
    virtual void vf28();                        // index 8
    virtual void vf2C();                        // index 9
    virtual void vf30(u32 a, u32 b);            // index 10 -> vtable 0x30
    virtual void vf34(u32 a);                    // index 11 -> vtable 0x34
    virtual void vf38();                        // index 12
    virtual void vf3C();                        // index 13
    virtual u32 vf40(u32 a);                    // index 14 -> vtable 0x40
    virtual void vf44();                        // index 15
    virtual void* vf48();                       // index 16 -> vtable 0x48
    virtual void vf4C();                        // index 17
    virtual void vf50();                        // index 18
    virtual void vf54();                        // index 19
    virtual void vf58();                        // index 20
    virtual void vf5C();                        // index 21
    virtual void vf60();                        // index 22
    virtual void vf64();                        // index 23 -> vtable 0x64
    virtual void vf68();                        // index 24
    virtual void vf6C();                        // index 25
    virtual void vf70(u32 a);                    // index 26
    virtual void vf74();                        // index 27
    virtual void vf78();                        // index 28
    virtual void vf7C();                        // index 29
    virtual void vf80();                        // index 30
    virtual void vf84(u32 arg);                 // index 31 -> vtable 0x84
    virtual void vf88();                        // index 32
    virtual void vf8C();                        // index 33
    virtual void vf90();                        // index 34
    virtual void vf94();                        // index 35
    virtual void vf98();                        // index 36
    virtual u32 vf9C();                        // index 37 -> vtable 0x9C
    virtual void vfA0();                        // index 38
    virtual void vfA4();                        // index 39
    virtual void vfA8();                        // index 40
    virtual void vfAC();                        // index 41
    virtual void vfB0();                        // index 42
    virtual void vfB4();                        // index 43
    virtual void vfB8();                        // index 44
    virtual void vfBC();                        // index 45
    virtual void vfC0();                        // index 46
    virtual void vfC4();                        // index 47
    virtual void vfC8();                        // index 48
    virtual void vfCC();                        // index 49
    virtual void vfD0();                        // index 50
    virtual void vfD4();                        // index 51 -> vtable 0xD4
    virtual void vfD8();                        // index 52 -> vtable 0xD8
    virtual void vfDC();                        // index 53 -> vtable 0xDC
    virtual void vfE0();                        // index 54
    virtual class CfWalkFieldE4* vfE4();       // index 55 -> vtable 0xE4
    virtual void vfE8();                        // index 56
    virtual void vfEC();                        // index 57
    virtual void vfF0();                        // index 58
    virtual void vfF4();                        // index 59
    virtual void vfF8();                        // index 60
    virtual void vfFC();                        // index 61
    virtual void vf100();                       // index 62 -> vtable 0x100
    virtual void vf104();                       // index 63 -> vtable 0x104

    void* func_800C22C4();
    u32 func_800C5970();

    // Layout fields (vptr implicit at 0x00 from the virtuals above)
    class CfWalkField04* field_04;             // 0x04
    u8 _pad08[0x14 - 0x08];                    // 0x08-0x13
    class CfWalkField14* field_14;             // 0x14
    class CfWalkBattleObj* field_18;           // 0x18 (battle object base)
    u8 _pad1C[0x28 - 0x1C];                    // 0x1C-0x27
    u8 field_28[0x344];                        // 0x28-0x36B (opaque; address taken)
    u8 _pad36C[0x380 - 0x36C];                 // 0x36C-0x37F
    u32 field_380;                             // 0x380
    u32 field_384;                             // 0x384
    u32 field_388;                             // 0x388
    u32 field_38C;                             // 0x38C
    u32 field_390;                             // 0x390
    u32 field_394;                             // 0x394
    u32 field_398;                             // 0x398
    u8  field_39C;                             // 0x39C
    u8  _pad39D[0x3A0 - 0x39D];                // 0x39D-0x39F
    f32 field_3A0;                             // 0x3A0
};

// ---------------------------------------------------------------------------
// vtable dispatch views (never instantiated; no vtables emitted).
// ---------------------------------------------------------------------------

// Move sub-object embedded at +0x3E9C of the battle object (secondary vtable).
class CfWalkMoveSub {
public:
    virtual void m08(u32 a);                   // index 0 -> vtable 0x8
    virtual u32 m0C(u32 a);                    // index 1 -> vtable 0xC
    virtual void m10(u32 a);                   // index 2 -> vtable 0x10
    virtual void m14();                        // index 3
    virtual void m18();                        // index 4
    virtual void m1C();                        // index 5
    virtual void m20();                        // index 6
    virtual void m24();                        // index 7
    virtual void m28();                        // index 8
    virtual void m2C();                        // index 9
    virtual void m30();                        // index 10
    virtual void m34();                        // index 11
    virtual void m38();                        // index 12
    virtual void m3C();                        // index 13
    virtual void m40();                        // index 14
    virtual void m44();                        // index 15
    virtual void m48();                        // index 16
    virtual u32 m4C();                        // index 17 -> vtable 0x4c
    virtual void m50(u32 a);                   // index 18 -> vtable 0x50
    virtual void m54();                        // index 19
    virtual void m58();                        // index 20
    virtual void m5C();                        // index 21
    virtual void m60();                        // index 22
    virtual void m64();                        // index 23
    virtual void m68();                        // index 24
    virtual void m6C();                        // index 25
    virtual void m70();                        // index 26
    virtual void m74();                        // index 27
    virtual void m78();                        // index 28
    virtual void m7C();                        // index 29
    virtual void m80();                        // index 30
    virtual void m84();                        // index 31
    virtual void m88();                        // index 32
    virtual void m8C();                        // index 33
    virtual void m90();                        // index 34
    virtual void m94();                        // index 35
    virtual void m98();                        // index 36
    virtual void m9C(void* a);                 // index 37 -> vtable 0x9C
    virtual void mA0();                        // index 38
    virtual void mA4();                        // index 39
    virtual void mA8();                        // index 40
    virtual void* mAC();                       // index 41 -> vtable 0xAC
    virtual void mB0();                        // index 42
    virtual void mB4();                        // index 43
    virtual void mB8();                        // index 44
    virtual void mBC();                        // index 45
    virtual void mC0();                        // index 46
    virtual void mC4();                        // index 47
    virtual void mC8();                        // index 48
    virtual void mCC();                        // index 49
    virtual void mD0();                        // index 50
    virtual void mD4();                        // index 51
    virtual void mD8();                        // index 52
    virtual void mDC();                        // index 53
    virtual void mE0();                        // index 54
    virtual void mE4();                        // index 55
    virtual void mE8();                        // index 56
    virtual void mEC();                        // index 57
    virtual void mF0();                        // index 58
    virtual void mF4();                        // index 59
    virtual void mF8();                        // index 60
    virtual void mFC();                        // index 61
    virtual void m100();                       // index 62
    virtual void m104();                       // index 63
    virtual void m108();                       // index 64
    virtual void m10C(u32 a);                  // index 65 -> vtable 0x10c
    virtual void* m110();                      // index 66 -> vtable 0x110 (returns this)
    virtual void m114();                       // index 67
    virtual void m118();                       // index 68
    virtual void m11C();                       // index 69
    virtual void m120();                       // index 70
    virtual void m124();                       // index 71
    virtual void m128();                       // index 72
    virtual void m12C();                       // index 73
    virtual void m130();                       // index 74
    virtual void m134(f32 a);                  // index 75 -> vtable 0x134
    virtual struct CfWalkMovePos* m138();      // index 76 -> vtable 0x138 (returns pos block)
    virtual void m13C();                       // index 77
    virtual void m140();                       // index 78
    virtual void m144();                       // index 79
    virtual void m148();                       // index 80
    virtual void m14C();                       // index 81
    virtual void m150();                       // index 82
    virtual void m154();                       // index 83
    virtual void m158();                       // index 84
    virtual void m15C();                       // index 85
    virtual void m160();                       // index 86
    virtual void m164();                       // index 87
    virtual void m168();                       // index 88
    virtual void m16C();                       // index 89
    virtual void m170();                       // index 90
    virtual void m174();                       // index 91
    virtual void m178();                       // index 92
    virtual void m17C();                       // index 93
    virtual void m180();                       // index 94
    virtual void m184();                       // index 95
    virtual void m188();                       // index 96
    virtual void m18C();                       // index 97
    virtual void m190();                       // index 98
    virtual void m194();                       // index 99
    virtual void m198();                       // index 100
    virtual void m19C();                       // index 101
    virtual void m1A0();                       // index 102
    virtual void m1A4();                       // index 103
    virtual void m1A8();                       // index 104
    virtual void m1AC(u32 a, const char* b);   // index 105 -> vtable 0x1AC
    virtual void m1B0();                       // index 106
    virtual void m1B4();                       // index 107
    virtual void m1B8();                       // index 108
    virtual void m1BC();                       // index 109
    virtual void m1C0();                       // index 110
    virtual void m1C4();                       // index 111
    virtual void m1C8();                       // index 112
    virtual void m1CC();                       // index 113
    virtual void m1D0();                       // index 114
    virtual void m1D4(f32 a);                  // index 115 -> vtable 0x1D4

    u8 _pad04[0x38 - 0x04];                    // 0x04-0x37
    void* field_38;                            // 0x38 (battle-object alias at +0x3ED4)
    u8 _pad3C[0x68 - 0x3C];                    // 0x3C-0x67
    u32 field_68;                              // 0x68 (battle-object view alias at +0x3F04)
    u8 _pad6C[0x74 - 0x6C];                    // 0x6C-0x73
    u32 field_74;                              // 0x74
    u8 _pad78[0x8C - 0x78];                    // 0x78-0x8B
    u16 field_8C;                              // 0x8C
    u8 _pad8E[0xC4 - 0x8E];                    // 0x8E-0xC3 (deeper fields undeclared)
};

// Position block returned by the move sub-object's m138() (vtable 0x138).
struct CfWalkMovePos {
    f32 field_0;                               // 0x0
};

// Deep view over the move sub-object for fields past +0x78 that the base
// CfWalkMoveSub does not declare (same vptr; only used via cast).
struct CfWalkMoveSubDeep {
    u8 _pad00[0xC4];                           // 0x00-0xC3
    class CfWalkSubC4_3B4* field_C4;           // 0xC4 (alias of battle object +0x3F60)
};

// Target of the move sub-object's field_C4 pointer: word at +0x3B4.
struct CfWalkSubC4_3B4 {
    u8 _pad00[0x3B4];                          // 0x00-0x3B3
    u32 field_3B4;                             // 0x3B4
};

// b30() result: actor-id word at +0.
struct CfWalkB30Result {
    u32 field_0;                               // 0x0
};

// Object at battleObj->field_04: vtable slot 0x20 called with (u32).
class CfWalkBattleObj4 {
public:
    virtual void b00();                        // index 0
    virtual void b04();                        // index 1
    virtual void b08();                        // index 2
    virtual void b0C();                        // index 3
    virtual void b10();                        // index 4
    virtual void b14();                        // index 5
    virtual void b20(u32 a);                   // index 6 -> vtable 0x20
    virtual void b24();                        // index 7
    virtual void b28();                        // index 8
    virtual void b2C();                        // index 9
    virtual CfWalkB30Result* b30();            // index 10 -> vtable 0x30
};

// Walker field_04 target: vtable slot 0x74 returns a status word.
class CfWalkField04 {
public:
    virtual void f00();                        // index 0
    virtual void f04();                        // index 1
    virtual void f08();                        // index 2
    virtual void f0C();                        // index 3
    virtual void f10();                        // index 4
    virtual void f14();                        // index 5
    virtual void f18();                        // index 6
    virtual void f1C();                        // index 7
    virtual void f20();                        // index 8
    virtual void f24();                        // index 9
    virtual void f28();                        // index 10
    virtual void f2C();                        // index 11
    virtual void f30();                        // index 12
    virtual void f34();                        // index 13
    virtual void f38();                        // index 14
    virtual void f3C();                        // index 15
    virtual void f40();                        // index 16
    virtual void f44();                        // index 17
    virtual void f48();                        // index 18
    virtual void f4C();                        // index 19
    virtual void f50();                        // index 20
    virtual void f54();                        // index 21
    virtual void f58();                        // index 22
    virtual void f5C();                        // index 23
    virtual void f60();                        // index 24
    virtual void f64();                        // index 25
    virtual void f68();                        // index 26
    virtual u32 vf74();                        // index 27 -> vtable 0x74
};

// Walker field_14 target: status word at +0x90.
struct CfWalkField14 {
    u8 _pad00[0x90];                           // 0x00-0x8F
    u32 field_90;                              // 0x90
};

// vfE4() result: vtable slot 0x10 called with no args.
class CfWalkFieldE4 {
public:
    virtual void e00();                        // index 0
    virtual void e04();                        // index 1
    virtual void e10();                        // index 2 -> vtable 0x10
};

// Result of the battle object's vf298() (vtable 0x298): status word at +0x4.
struct CfWalkVf298Result {
    u8 _00[0x4];                             // 0x00-0x03
    u32 field_4;                             // 0x4
};

// Battle object base at field_18: embedded move at +0x3E9C, id at +0x3F60.
class CfWalkBattleObj {
public:
    virtual void bf08();                       // index 0
    virtual void bf0C();                       // index 1
    virtual void bf10();                       // index 2
    virtual void bf14();                       // index 3
    virtual void bf18();                       // index 4
    virtual void bf1C();                       // index 5
    virtual void bf20();                       // index 6
    virtual void bf24();                       // index 7
    virtual void bf28();                       // index 8
    virtual void bf2C();                       // index 9
    virtual void bf30();                       // index 10
    virtual void bf34();                       // index 11
    virtual void bf38();                       // index 12
    virtual void bf3C();                       // index 13
    virtual void bf40();                       // index 14
    virtual void bf44();                       // index 15
    virtual void bf48();                       // index 16
    virtual void bf4C();                       // index 17
    virtual void bf50();                       // index 18
    virtual void bf54();                       // index 19
    virtual void bf58();                       // index 20
    virtual void bf5C();                       // index 21
    virtual void bf60();                       // index 22
    virtual void bf64();                       // index 23
    virtual void bf68();                       // index 24
    virtual void bf6C();                       // index 25
    virtual void bf70();                       // index 26
    virtual void bf74();                       // index 27
    virtual void bf78();                       // index 28
    virtual void bf7C();                       // index 29
    virtual void bf80();                       // index 30
    virtual void bf84();                       // index 31
    virtual void bf88();                       // index 32
    virtual void bf8C();                       // index 33
    virtual void bf90();                       // index 34
    virtual void bf94();                       // index 35
    virtual void bf98();                       // index 36
    virtual void bf9C();                       // index 37
    virtual void bfA0();                       // index 38
    virtual void bfA4();                       // index 39
    virtual void bfA8();                       // index 40
    virtual void bfAC();                       // index 41
    virtual void bfB0();                       // index 42
    virtual void bfB4();                       // index 43
    virtual void bfB8();                       // index 44
    virtual void bfBC();                       // index 45
    virtual void bfC0();                       // index 46
    virtual void bfC4();                       // index 47
    virtual void bfC8();                       // index 48
    virtual void bfCC();                       // index 49
    virtual void bfD0();                       // index 50
    virtual void bfD4();                       // index 51
    virtual void bfD8();                       // index 52
    virtual void bfDC();                       // index 53
    virtual void bfE0();                       // index 54
    virtual void bfE4();                       // index 55
    virtual void bfE8();                       // index 56
    virtual void bfEC();                       // index 57
    virtual void bfF0();                       // index 58
    virtual void bfF4();                       // index 59
    virtual void bfF8();                       // index 60
    virtual void bfFC();                       // index 61
    virtual void bg00();                       // index 62
    virtual void bg04();                       // index 63
    virtual void bg08();                       // index 64
    virtual void bg0C();                       // index 65
    virtual void bg10();                       // index 66
    virtual void bg14();                       // index 67
    virtual void bg18();                       // index 68
    virtual void bg1C();                       // index 69
    virtual void bg20();                       // index 70
    virtual void bg24();                       // index 71
    virtual void bg28();                       // index 72
    virtual f32 vf12C();                       // index 73 -> vtable 0x12C (returns move value)
    virtual void bg30();                       // index 74
    virtual void bg34();                       // index 75
    virtual void bg38();                       // index 76
    virtual void bg3C();                       // index 77
    virtual void bg40();                       // index 78
    virtual void bg44();                       // index 79
    virtual void bg48();                       // index 80
    virtual void bg4C();                       // index 81
    virtual void bg50();                       // index 82
    virtual void bg54();                       // index 83
    virtual void bg58();                       // index 84
    virtual void bg5C();                       // index 85
    virtual void bg60();                       // index 86
    virtual void bg64();                       // index 87
    virtual void bg68();                       // index 88
    virtual void bg6C();                       // index 89
    virtual void bg70();                       // index 90
    virtual void bg74();                       // index 91
    virtual void bg78();                       // index 92
    virtual void bg7C();                       // index 93
    virtual void bg80();                       // index 94
    virtual void bg84();                       // index 95
    virtual void bg88();                       // index 96
    virtual void bg8C();                       // index 97
    virtual void bg90();                       // index 98
    virtual void bg94();                       // index 99
    virtual void bg98();                       // index 100
    virtual void bg9C();                       // index 101
    virtual void bgA0();                       // index 102
    virtual void bgA4();                       // index 103
    virtual void bgA8();                       // index 104
    virtual void bgAC();                       // index 105
    virtual void bgB0();                       // index 106
    virtual void bgB4();                       // index 107
    virtual void bgB8();                       // index 108
    virtual void bgBC();                       // index 109
    virtual void bgC0();                       // index 110
    virtual void bgC4();                       // index 111
    virtual void bgC8();                       // index 112
    virtual void bgCC();                       // index 113
    virtual void bgD0();                       // index 114
    virtual void bgD4();                       // index 115
    virtual void bgD8();                       // index 116
    virtual void bgDC();                       // index 117
    virtual void bgE0();                       // index 118
    virtual void bgE4();                       // index 119
    virtual void bgE8();                       // index 120
    virtual void bgEC();                       // index 121
    virtual void bgF0();                       // index 122
    virtual void bgF4();                       // index 123
    virtual void bgF8();                       // index 124
    virtual void bgFC();                       // index 125
    virtual void bh00();                       // index 126
    virtual void bh04();                       // index 127
    virtual void bh08();                       // index 128
    virtual void bh0C();                       // index 129
    virtual void bh10();                       // index 130
    virtual void bh14();                       // index 131
    virtual void bh18();                       // index 132
    virtual void bh1C();                       // index 133
    virtual void bh20();                       // index 134
    virtual void bh24();                       // index 135
    virtual void bh28();                       // index 136
    virtual void bh2C();                       // index 137
    virtual void vf230();                      // index 138 -> vtable 0x230
    virtual void* vf234();                     // index 139 -> vtable 0x234 (returns value block)
    virtual void vf238();                      // index 140
    virtual void vf23C();                      // index 141
    virtual void vf240();                      // index 142
    virtual void vf244();                      // index 143
    virtual void vf248();                      // index 144
    virtual void vf24C();                      // index 145
    virtual void vf250();                      // index 146
    virtual void vf254();                      // index 147
    virtual void vf258();                      // index 148
    virtual void vf25C();                      // index 149
    virtual void vf260();                      // index 150
    virtual void vf264();                      // index 151
    virtual void vf268();                      // index 152
    virtual void vf26C();                      // index 153
    virtual void vf270();                      // index 154
    virtual void vf274();                      // index 155
    virtual void vf278();                      // index 156
    virtual void vf27C();                      // index 157
    virtual void vf280();                      // index 158
    virtual void vf284();                      // index 159
    virtual void vf288();                      // index 160
    virtual void vf28C();                      // index 161
    virtual void vf290();                      // index 162
    virtual void vf294();                      // index 163
    virtual CfWalkVf298Result* vf298();        // index 164 -> vtable 0x298

    CfWalkBattleObj4* field_04;                // 0x04
    u8 _pad08[0x3374 - 0x08];                  // 0x08-0x3373
    u32 field_3374;                            // 0x3374 (flags)
    u8 _pad3378[0x3E9C - 0x3378];              // 0x3378-0x3E9B
    CfWalkMoveSub mSub;                        // 0x3E9C (0xC4 bytes incl. vptr)
    u32 field_3F60;                            // 0x3F60 (battle id / battle-state pointer)
};

// ---------------------------------------------------------------------------
// Phantom vtable views / record types for this unit's call sites. Never
// instantiated; only cast + call a slot, so no vtable is emitted.
// ---------------------------------------------------------------------------

// func_8016FE34 result (player move record): phantom vtable with the
// battle-active check at 0x2BC (index 173), embedded move sub-object at
// +0x3E9C, battle-object pointer at +0x3F60.
class CfWalkPlayerRec {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();   virtual void f9();   virtual void f10();  virtual void f11();
    virtual void f12();  virtual void f13();  virtual void f14();  virtual void f15();
    virtual void f16();  virtual void f17();  virtual void f18();  virtual void f19();
    virtual void f20();  virtual void f21();  virtual void f22();  virtual void f23();
    virtual void f24();  virtual void f25();  virtual void f26();  virtual void f27();
    virtual void f28();  virtual void f29();  virtual void f30();  virtual void f31();
    virtual void f32();  virtual void f33();  virtual void f34();  virtual void f35();
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
    virtual void f164(); virtual void f165(); virtual void f166(); virtual void f167();
    virtual void f168(); virtual void f169(); virtual void f170(); virtual void f171();
    virtual void f172();
    virtual u32 vf2BC();                     // index 173 -> vtable 0x2BC (battle-active)

    u8 _pad04[0x3E9C - 0x04];                // 0x04-0x3E9B
    CfWalkMoveSub mSub;                      // 0x3E9C (embedded move sub-object)
    u8 _pad3F14[0x3F60 - 0x3F14];            // 0x3F14-0x3F5F
    void* field_3F60;                        // 0x3F60 (battle-object pointer)
};

// Battle object (from player record / walker field_18): flag word at +0x4EC.
struct CfWalkBattleFlags {
    u8 _00[0x4EC];                           // 0x00-0x4EB
    u32 field_4EC;                           // 0x4EC flags
    u8 _4F0[0x530 - 0x4F0];                  // 0x4F0-0x52F
    u16 field_530;                           // 0x530 flags
};

// Battle sub-object (CfWalkActorObj::field_C4): position query virtuals.
class CfWalkSubC4 {
public:
    virtual void f0();                       // index 0
    virtual f32 vf0C();                      // index 1 -> vtable 0xC
    virtual void f2();                       // index 2
    virtual f32 vf14();                      // index 3 -> vtable 0x14
};

// Actor object from func_800B708C (id -> object): flags at +0x64, move handle
// at +0x74, sub-object pointer at +0x98, battle sub-object at +0xC4, area
// id/step at +0x734/+0x738.
class CfWalkActorObj {
public:
    u8 _pad00[0x64];                       // 0x00-0x63 (vtable at 0)
    u32 field_64;                          // 0x64 flags
    u8 _pad68[0x74 - 0x68];                // 0x68-0x73
    void* field_74;                        // 0x74
    u8 _pad78[0x98 - 0x78];                // 0x78-0x97
    void* field_98;                        // 0x98
    u8 _pad9C[0xC4 - 0x9C];                // 0x9C-0xC3
    CfWalkSubC4* field_C4;                 // 0xC4 battle sub-object
    u8 _padC8[0x734 - 0xC8];               // 0xC8-0x733
    s32 field_734;                         // 0x734 area id
    u16 field_738;                         // 0x738 step
};

// __dynamic_cast target (typeinfo lbl_eu_80661C10): battle-flag query slots.
class CfWalkBattleSub {
public:
    virtual void f0();   virtual void f1();  virtual void f2();  virtual void f3();
    virtual u32 vf18();                      // index 4 -> vtable 0x18
    virtual u32 vf1C();                      // index 5 -> vtable 0x1C
    virtual u32 vf20();                      // index 6 -> vtable 0x20
    virtual void f7();   virtual void f8();  virtual void f9();
    virtual u32 vf30();                      // index 10 -> vtable 0x30
    virtual u32 vf34();                      // index 11 -> vtable 0x34
    virtual u32 vf38();                      // index 12 -> vtable 0x38
    virtual u32 vf3C();                      // index 13 -> vtable 0x3C
    virtual u32 vf40();                      // index 14 -> vtable 0x40
    virtual u32 vf44();                      // index 15 -> vtable 0x44
    virtual u32 vf48();                      // index 16 -> vtable 0x48
    virtual u32 vf4C();                      // index 17 -> vtable 0x4C
    virtual u32 vf50();                      // index 18 -> vtable 0x50
    virtual u32 vf54();                      // index 19 -> vtable 0x54
};

// Phantom vtable for actor sub-objects: slot 0x88 called with a flag.
class CfWalkSub88 {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();   virtual void f9();   virtual void f10();  virtual void f11();
    virtual void f12();  virtual void f13();  virtual void f14();  virtual void f15();
    virtual void f16();  virtual void f17();  virtual void f18();  virtual void f19();
    virtual void f20();  virtual void f21();  virtual void f22();  virtual void f23();
    virtual void f24();  virtual void f25();  virtual void f26();  virtual void f27();
    virtual void f28();  virtual void f29();  virtual void f30();  virtual void f31();
    virtual void vf88(u32 a);                // index 32 -> vtable 0x88
};

// Phantom vtable for actor objects: slot 0xAC returns the arts/action id.
class CfWalkObjAC {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();   virtual void f9();   virtual void f10();  virtual void f11();
    virtual void f12();  virtual void f13();  virtual void f14();  virtual void f15();
    virtual void f16();  virtual void f17();  virtual void f18();  virtual void f19();
    virtual void f20();  virtual void f21();  virtual void f22();  virtual void f23();
    virtual void f24();  virtual void f25();  virtual void f26();  virtual void f27();
    virtual void f28();  virtual void f29();  virtual void f30();  virtual void f31();
    virtual void f32();  virtual void f33();  virtual void f34();  virtual void f35();
    virtual void f36();  virtual void f37();  virtual void f38();  virtual void f39();
    virtual void f40();
    virtual void* vfAC();                    // index 41 -> vtable 0xAC
};

// Phantom vtable for the walker's battle object: slot 0x2A4 returns the
// battle-status record.
class CfWalkBattleObjVtbl {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();   virtual void f9();   virtual void f10();  virtual void f11();
    virtual void f12();  virtual void f13();  virtual void f14();  virtual void f15();
    virtual void f16();  virtual void f17();  virtual void f18();  virtual void f19();
    virtual void f20();  virtual void f21();  virtual void f22();  virtual void f23();
    virtual void f24();  virtual void f25();  virtual void f26();  virtual void f27();
    virtual void f28();  virtual void f29();  virtual void f30();  virtual void f31();
    virtual void f32();  virtual void f33();  virtual void f34();  virtual void f35();
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
    virtual void f164(); virtual void f165(); virtual void f166();
    virtual void* vf2A4();                   // index 167 -> vtable 0x2A4
};

// vf2A4() result: battle-status flags at +0x78.
struct CfWalkVf2A4Result {
    u8 _00[0x78];                            // 0x00-0x77
    u32 field_78;                            // 0x78 flags
};

// Phantom vtable for func_800821F8 result (dynamic manager): slot 0x2C.
class CfWalkDynMgr {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();
    virtual void vf2C(void* arg);            // index 9 -> vtable 0x2C
};

// Talk-source object (func_800BBC0C result): vtable slot 0x160, flags at +0x64.
class CfWalkTalkSrc {
public:
    virtual void t00(u32 a);                   // index 0 -> vtable 0x8
    virtual u32 t04(u32 a);                    // index 1 -> vtable 0xC
    virtual void t08(u32 a);                   // index 2 -> vtable 0x10
    virtual void t0C();                        // index 3
    virtual void t10(u32 a);                   // index 4 -> vtable 0x18
    virtual void t14();                        // index 5
    virtual void t18();                        // index 6
    virtual u32 t1C(u32 a);                    // index 7 -> vtable 0x24
    virtual void t20();                        // index 8
    virtual void t24();                        // index 9
    virtual void t28();                        // index 10
    virtual void t2C();                        // index 11
    virtual void t30();                        // index 12
    virtual void t34();                        // index 13
    virtual void t38();                        // index 14
    virtual void t3C();                        // index 15
    virtual void t40();                        // index 16
    virtual void t44();                        // index 17
    virtual void t48(u32 a);                   // index 18 -> vtable 0x50
    virtual void t4C();                        // index 19
    virtual void t50();                        // index 20
    virtual void t54();                        // index 21
    virtual void t58();                        // index 22
    virtual void t5C();                        // index 23
    virtual void t60();                        // index 24
    virtual void t64();                        // index 25
    virtual void t68();                        // index 26
    virtual void t6C();                        // index 27
    virtual void t70();                        // index 28
    virtual void t74();                        // index 29
    virtual void t78();                        // index 30
    virtual void t7C();                        // index 31
    virtual void t80();                        // index 32
    virtual void t84();                        // index 33
    virtual void t88();                        // index 34
    virtual void t8C();                        // index 35
    virtual void t90();                        // index 36
    virtual void t94();                        // index 37
    virtual void t98();                        // index 38
    virtual void t9C();                        // index 39
    virtual void tA0();                        // index 40
    virtual void tA4();                        // index 41
    virtual void tA8();                        // index 42
    virtual void tAC();                        // index 43
    virtual void tB0();                        // index 44
    virtual void tB4();                        // index 45
    virtual void tB8();                        // index 46
    virtual void tBC();                        // index 47
    virtual void tC0();                        // index 48
    virtual void tC4();                        // index 49
    virtual void tC8();                        // index 50
    virtual void tCC();                        // index 51
    virtual void tD0();                        // index 52
    virtual void tD4();                        // index 53
    virtual void tD8();                        // index 54
    virtual void tDC();                        // index 55
    virtual void tE0();                        // index 56
    virtual void tE4();                        // index 57
    virtual void tE8();                        // index 58
    virtual void tEC();                        // index 59
    virtual void tF0();                        // index 60
    virtual void tF4();                        // index 61
    virtual void tF8();                        // index 62
    virtual void tFC();                        // index 63
    virtual void t100();                       // index 64
    virtual void t104();                       // index 65
    virtual void t108();                       // index 66
    virtual void t10C();                       // index 67
    virtual void t110();                       // index 68
    virtual void t114();                       // index 69
    virtual void t118();                       // index 70
    virtual void t11C();                       // index 71
    virtual void t120();                       // index 72
    virtual void t124();                       // index 73
    virtual void t128();                       // index 74
    virtual void t12C();                       // index 75
    virtual void t130();                       // index 76
    virtual void t134();                       // index 77
    virtual void t138();                       // index 78
    virtual void t13C();                       // index 79
    virtual void t140();                       // index 80
    virtual void t144();                       // index 81
    virtual void t148();                       // index 82
    virtual void t14C();                       // index 83
    virtual void t150();                       // index 84
    virtual void t154();                       // index 85
    virtual void* t160();                      // index 86 -> vtable 0x160
    virtual u32 t164();                        // index 87 -> vtable 0x164
    virtual void t168();                       // index 88
    virtual f32 t16C();                        // index 89 -> vtable 0x16C
    virtual void t170();                       // index 90
    virtual f32 vf174();                       // index 91 -> vtable 0x174 (battle progress)
    virtual void t178();                       // index 92
    virtual void t17C();                       // index 93
    virtual void t180();                       // index 94
    virtual void t184();                       // index 95
    virtual void t188();                       // index 96
    virtual void t18C();                       // index 97
    virtual void t190();                       // index 98
    virtual void t194();                       // index 99
    virtual void t198();                       // index 100
    virtual void t19C();                       // index 101
    virtual void t1A0();                       // index 102
    virtual void t1A4();                       // index 103
    virtual void t1A8();                       // index 104
    virtual void t1AC(u32 a, const char* b);   // index 105 -> vtable 0x1AC
    virtual void t1B0();                       // index 106
    virtual void t1B4();                       // index 107
    virtual void t1B8();                       // index 108
    virtual void t1BC();                       // index 109
    virtual void t1C0();                       // index 110
    virtual void t1C4();                       // index 111
    virtual void t1C8();                       // index 112
    virtual void t1CC();                       // index 113
    virtual void t1D0();                       // index 114
    virtual void t1D4();                       // index 115
    virtual void t1D8();                       // index 116
    virtual void t1DC();                       // index 117
    virtual void t1E0();                       // index 118
    virtual void t1E4();                       // index 119
    virtual void t1E8();                       // index 120
    virtual void t1EC();                       // index 121
    virtual void t1F0();                       // index 122
    virtual void t1F4();                       // index 123
    virtual void t1F8();                       // index 124
    virtual void t1FC();                       // index 125
    virtual void t200();                       // index 126
    virtual void t204();                       // index 127
    virtual void t208();                       // index 128
    virtual void t20C();                       // index 129
    virtual void t210();                       // index 130
    virtual void t214();                       // index 131
    virtual void t218();                       // index 132
    virtual void t21C();                       // index 133
    virtual void t220();                       // index 134
    virtual void t224();                       // index 135
    virtual u32 vf228();                       // index 136 -> vtable 0x228 (state id)
    virtual u32 vf22C();                       // index 137 -> vtable 0x22C

    u8 _pad04[0x64 - 0x04];                    // 0x04-0x63
    u32 field_64;                              // 0x64 (flags)
    u32 field_68;                              // 0x68 (flags)
    u8 _pad6C[0x74 - 0x6C];                    // 0x6C-0x73
    u32 field_74;                              // 0x74
    u8 _pad78[0x8C - 0x78];                    // 0x78-0x8B
    u16 field_8C;                              // 0x8C
    u8 _pad8E[0x98 - 0x8E];                    // 0x8E-0x97
    u32 field_98;                              // 0x98
    u8 _pad9C[0xC4 - 0x9C];                    // 0x9C-0xC3
    u32 field_C4;                              // 0xC4
};

// Object behind func_800FE68C (lbl_eu_80663F14): actor-id word at +0x90E4.
struct CfWalkGlobal {
    u8 _00[0x90E4];                            // 0x00-0x90E3
    u32 field_90E4;                            // 0x90E4
};

// Stack-allocated enum-list holder (func_80043D90 ctor / __dt__80043E88 dtor).
struct CfWalkEnumHolder {
    void* list;   // 0x0
    u32 handle;   // 0x4
};

// Enum list returned by func_80043F18: element count at +0x620.
struct CfWalkEnumList {
    u8 _pad00[0x620];                          // 0x00-0x61F
    u32 field_620;                             // 0x620
};

// Item returned by func_800F6EC0: object pointer at +0x4, position at +0x8,
// type byte at +0x18.
struct CfWalkEnumItem {
    u8 _pad00[0x04];                           // 0x00-0x03
    void* field_04;                            // 0x4
    f32 field_08;                              // 0x8
    u8 _pad0C[0x18 - 0x0C];                    // 0x0C-0x17
    u8 field_18;                               // 0x18
};

// Stack CRect view filled by func_8043E928; a signed 16-bit value is read
// back from +0x4.
struct CfWalkRect4 {
    u8 _00[0x04];                              // 0x00-0x03
    s16 field_4;                               // 0x4
    u8 _06[0x02];                              // 0x6-0x7
};

// Object behind the move sub-object's field_38 (battle object +0x3ED4):
// vtable slot 0x40 returns a status word.
class CfWalkField3ED4 {
public:
    virtual void f0();   virtual void f1();   virtual void f2();   virtual void f3();
    virtual void f4();   virtual void f5();   virtual void f6();   virtual void f7();
    virtual void f8();   virtual void f9();   virtual void f10();  virtual void f11();
    virtual void f12();  virtual void f13();
    virtual u32 vf40();                      // index 14 -> vtable 0x40
};

// Global gimmick container (getUnk80664658 result): flag word at +0x214,
// stored object at +0x218.
struct CfWalkGimmickGlobal {
    u8 _00[0x214];                           // 0x00-0x213
    u32 field_214;                           // 0x214
    void* field_218;                         // 0x218
};

// Battle sub-object (talk-source field_C4): flags at +0x270 and +0x4EC.
struct CfWalkBattleSubC4 {
    u8 _00[0x270];                           // 0x00-0x26F
    u32 field_270;                           // 0x270
    u8 _274[0x4EC - 0x274];                  // 0x274-0x4EB
    u32 field_4EC;                           // 0x4EC
};

// Dynamic-cast result status bytes at +0x90/+0x91.
struct CfWalkCastFlags {
    u8 _00[0x90];                            // 0x00-0x8F
    u8 field_90;                             // 0x90
    u8 field_91;                             // 0x91
};

// Art/action record (func_80140AFC / func_80140CA4 result): id word at +0x4.
struct CfWalkArtRec {
    u8 _00[0x4];                             // 0x00-0x03
    u32 field_4;                             // 0x4
};

// Battle-state object (field_18->field_3F60 target): flag words at +0xC and
// +0x4EC.
struct CfWalkBattleState3F60 {
    u8 _00[0xC];                             // 0x00-0x0B
    u32 field_0C;                            // 0xC
    u8 _10[0x4EC - 0x10];                    // 0x10-0x4EB
    u32 field_4EC;                           // 0x4EC
};

// Phantom view of cf::CfObject for the direct member call that emits the
// mangled reloc func_800BE898__Q22cf8CfObjectFiUlff (note unsigned long so
// the mangling matches retail's Ul).
class CfObject {
public:
    void func_800BE898(int a, unsigned long b, float c, float d);
};

} // namespace cf

// Singly-linked chain node (next pointer at +0); battle lists hang off the
// battle manager's head node.
struct CfWalkChainNode {
    CfWalkChainNode* next;                     // 0x0
};

// cf::CfGameManager view (getInstance__Q22cf13CfGameManagerFv result): the
// walker's battle-object handle lives at +0x80.
class CfWalkGameMgr {
public:
    u8 _pad00[0x80];                           // 0x00-0x7F
    void* field_80;                            // 0x80 battle-object handle
};

// Minimal view of cf::CBattleManager for the vtable slots this TU calls
// (func_80085220 at 0x1c). The real class lives in CBattleManager.hpp, which
// is not included here: its CChainActorList.hpp/CSuddenCommu.hpp chain
// conflicts on the func_80174C98 extern "C" declaration. With -RTTI on,
// declared index N -> vtable offset (N+2)*4.
class CfWalkBMView {
public:
    virtual void vf08();                    // index 0 -> vtable 0x8
    virtual void vf0C();                    // index 1
    virtual void vf10();                    // index 2 -> vtable 0x10
    virtual void vf14();                    // index 3
    virtual void vf18();                    // index 4
    virtual void func_80085220(u32 a, u32 b);  // index 5 -> vtable 0x1c
    virtual void vf20();                    // index 6
    virtual void vf24();                    // index 7
    virtual u32 vf28(u32 a);                // index 8 -> vtable 0x28

    u8 _pad04[0x08 - 0x04];                    // 0x04-0x07
    CfWalkChainNode* field_08;                 // 0x08 battle list head
    u8 _pad0C[0x48 - 0x0C];                    // 0x0C-0x47
    CfWalkChainNode* field_48;                 // 0x48 battle list head (walk sweep)
    u8 _pad4C[0x1AA - 0x4C];                   // 0x4C-0x1A9
    u8 field_1AA;                              // 0x1AA (battle-state flag)
    u8 _pad1AB[0x20C8 - 0x1AB];                // 0x1AB-0x20C7
    s16 field_20C8;                            // 0x20C8
};

// C++-mangled import (retail symbol func_800B708C__Fi; declared here so MWCC
// emits the mangled reloc name).
extern void* func_800B708C(int id);

// func_8006EF04__Fi: presentation/event flag-bit probe (mangled C++ match).
extern bool func_8006EF04(int mask);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
void func_800C6EC0(void* self);
void func_800CFFA0(void* self);
void func_800C969C(void* self);
void func_800C5998(void* self);
void func_800C2C90(cf::CfObjectImplWalker* self);
void func_800C525C(cf::CfObjectImplWalker* self);
void func_800CC020(void* self);
void func_800C5B00(void);
void func_802A0E08(void* self);
void func_80140E00(u32 a, u32 b, u32 c);
bool func_80226B94(void);
u32 func_80086DA0__Q22cf13CfGameManagerFv();
u32 func_80086DA4__Q22cf13CfGameManagerFv();
int func_8011CD5C(void);
int func_80257308(void);
int func_802AC510(void);
void func_80142C80(void);
void* func_8009ECB0(void);
u32 func_8009EC9C(u32 id);
int func_8026178C(u32 a, u32 b);
void func_80109784(u32 a, u32 b, u32 c);
void func_800E1B5C(void* mgr, void* battleObj);
cf::CfWalkGlobal* func_800FE68C();
CfWalkBMView* getInstance__Q22cf14CBattleManagerFv();
void* func_800BF324(void* objParam);
void func_80043D90(cf::CfWalkEnumHolder* holder);
cf::CfWalkEnumList* func_80043F18(cf::CfWalkEnumHolder* holder);
void func_800F4A98(cf::CfWalkEnumList* list, u32 type, u32 filter);
void* func_800F6E98(cf::CfWalkEnumList* list, int index);
void* func_800BBC0C(void* objParam);
void func_800BC3D8(void* player, float value);
void __dt__80043E88(cf::CfWalkEnumHolder* holder, int flags);
int func_8013EB90(int v);
void* getPlayer__Q22cf13CfGameManagerFi(int index);
void* func_8016FE34(void* src);
void func_800BE12C(void* obj, int a, int b, int c, int d);
void func_801F8E20(void* obj, int flag);
void func_8013EC6C(u32 a, u32 b);
CfWalkGameMgr* getInstance__Q22cf13CfGameManagerFv();
void func_800FE950(void* obj, u32 a, u32 b, u32 c);
void func_800FE920(void* obj);
void func_800FE938(void* obj);
u32 func_800FE7D8(void* obj);
void func_800FE738();
f32 func_80496288(void* scene);
void func_800C819C();
void func_800C86E8(void* self);
void func_800D9978(void* mgr, void* obj);
u32 func_80174C98(void* actor, u32* outVal, u32 flags);
void func_80174B4C(void* actor, u32 flags);
void func_8006BBF4(void* obj, u32 mask, int flag);
u32 func_8009CF8C(u32 resourceId);
int func_80133F48(int id, float f);
void* func_800451D8(u32 cls, int param);
void func_80485774(void* obj, u32 flags);
void func_8015AFA4(u32 a, u32 b);
void func_8013E424(void* str, int flags);
void* func_800821F8__Q22cf13CfGameManagerFv(void* self);
u32 func_80082694__Q22cf13CfGameManagerFv(u32 id);
void func_8008269C__Q22cf13CfGameManagerFv(u32 id, u32 value);
void func_800826F0__Q22cf13CfGameManagerFv(u32 id);
int func_80085840__Q22cf13CfGameManagerFv();
void* __dynamic_cast(void* obj, long offset, const void* srcType, const void* dstType, void* tmp);

// Enum-list / selector helpers (CfObjectEnumList + selector subsystem).
void func_800F6ED0(cf::CfWalkEnumList* list, void* value);
void* func_800F6E08(cf::CfWalkEnumList* list);
void* func_800F6EC0(cf::CfWalkEnumList* list, u32 index);
int func_800FE6A4(cf::CfWalkGlobal* g, u32 a, u32 b, u32 c);
int func_80148778(void* obj, u32 flag);

// Bdat table helpers.
void* func_8003AA34(void);
void* getFP__FPCc(const char* path);
u32 func_8003B1EC(void* bdat);
char* getBdatStringColumnValue(void* bdat, const char* column, int row);
void func_8013D07C(u32 obj, const char* str, int flag);
int func_801413DC(u32 a, int b);
void func_802919A0(void);
void func_8029194C(void);
int rand(void);

// Action/art play + system helpers.
void* func_8049627C(void* scene, int flag);
void func_8043E928__5CViewFRQ22ml5CRectP5CView(void* rect, void* view);
// func_80496044 is declared in kyoshin/cf/CfGameManager.hpp (already in scope).
// func_80084BF4__Q22cf13CfGameManagerFv (retail mangled C++ name).
int func_80084BF4__Q22cf13CfGameManagerFv(void);
int func_800967F8(void);
// getInstance__11CSysWinBuffFv is declared in CMainMenu.hpp (already in scope).
void* getUnk80664658(void);
int func_804BE398(void* vec, u32 a, u32 b, u32 c, f32 d, f32 e);
void* func_804BE520(int index);
void func_800BF29C(void* obj, int a, int b, u32 c, f32 d, f32 e);
void func_800BC3B0(void* obj, f32 value);
int func_8012CD24(void);
void func_8013D1E8(void* obj);
void func_8009D018(void* obj, int flag);
void func_80291A04(void);
void func_8009ECD0(u32 id);
void* func_80140AFC(u32 id);
void* func_80140CA4(u32 id);
void func_8013F354(void* obj);
void func_8013F3EC(void* obj);
void func_8013F2A0(void* obj);
int func_8013EC58(void);
void func_8013EC60(void);
int func_80122448(void);

// Enum-list constructors (decompiler placeholder addresses; same pattern as
// __dt__80043E88).
void* __ct__800FB044(void* list, f32 a, void* obj, int b);
void* __ct__800FA9B4(void* list, void* obj, int b);

// Intra-unit callbacks (C linkage so the reloc names match retail).
void func_800C3AD4(cf::CfObjectImplWalker* self);
int func_800C4244(cf::CfObjectImplWalker* self, u32 battleId, u32 arg);
int func_800C4BD4(cf::CfObjectImplWalker* self, u32 a, u32 b);
}

// sdata2 literals referenced by this unit (global scope: not mangled).
// const so MWCC treats the load as a constant and hoists it to the top of
// the store block (retail func_800C1F44 shape; MWCC_REFERENCE §extern const
// float hoist).
extern const f32 lbl_eu_80666B84;
extern f32 lbl_eu_80666BA4;
extern const f32 lbl_eu_80666B80;   // sdata2 constant
extern const f32 lbl_eu_80666BA0;   // sdata2 constant
extern const f32 lbl_eu_80666BB0;   // sdata2 constant
extern const f32 lbl_eu_80666BB4;   // sdata2 constant
extern const f32 lbl_eu_80666BB8;   // sdata2 constant
extern const f32 lbl_eu_80666BBC;   // sdata2 constant
extern const f32 lbl_eu_80666BC0;   // sdata2 constant

// Global-scope data symbols (MWCC does not mangle global variable names).
extern u32 lbl_eu_80663E24;   // presentation/event bitfield (.sbss)
extern u32 lbl_eu_80663E28;   // presentation bitfield 2 (.sbss)
extern void* lbl_eu_80663E14; // scene object pointer (.sbss)
extern f32 lbl_eu_80666B8C;   // sdata2 constant
extern const f32 lbl_eu_80666B88;  // sdata2 constant (0.5f)
extern const f32 lbl_eu_80666B90;  // sdata2 constant
extern const f64 lbl_eu_80666B98;  // sdata2 int->float magic (0x4330000080000000)
extern const f64 lbl_eu_80666BA8;  // sdata2 int->float magic (0x4330000080000000)
extern char lbl_eu_804FC694[];  // .rodata string/buffer
extern u32 lbl_eu_804FC670[3];  // .data table (player-slot effect ids)
// RTTI typeinfo labels for the move sub-object dynamic_cast. Declared as
// 4-byte pointers so MWCC treats them as sdata2 (emits li @sda21, not lis/addi).
extern const void* lbl_eu_80661C08;
extern const void* lbl_eu_80661C10;
extern const void* lbl_eu_80661CB0;
extern const void* lbl_eu_806618F0;
extern char lbl_eu_80573E18;  // .data string/buffer

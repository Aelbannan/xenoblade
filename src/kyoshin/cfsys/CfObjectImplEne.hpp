#pragma once

#include <types.h>
#include "monolib/util/FixStr.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"

namespace cf {

class CfObjectImplEne {
public:
    virtual ~CfObjectImplEne();

    // TODO: add fields
};

class CfImplEneBattleObj;  // forward decl for the typed member below

// ---------------------------------------------------------------------------
// vtable dispatch views (never instantiated; no vtables emitted).
// With -RTTI on, MWCC reserves two leading vtable slots (offset-to-top +
// typeinfo), so the virtual at declared index N sits at vtable offset (N+2)*4.
// ---------------------------------------------------------------------------

// Self object of func_800CFFCC / func_800D0090 / func_800D1020 /
// func_800D0A60 / func_800D0B04: vtable slots 0x30 and 0xe4 are called;
// fields at +0x14 (ptr), +0x18 (ptr), +0x28 (opaque buffer), +0x36C (u32).
class CfObjectImplEneObj {
public:
    virtual void vf00() = 0;                 // index 0
    virtual void vf04() = 0;                 // index 1
    virtual void vf08() = 0;                 // index 2
    virtual void vf0C() = 0;                 // index 3
    virtual void vf10() = 0;                 // index 4
    virtual void vf14() = 0;                 // index 5
    virtual void vf18() = 0;                 // index 6
    virtual void vf1C() = 0;                 // index 7
    virtual void vf20() = 0;                 // index 8
    virtual void vf24() = 0;                 // index 9
    virtual void vf30(u32 a, u32 b) = 0;     // index 10 -> vtable offset 0x30
    virtual void vf34() = 0;                 // index 11
    virtual void vf38() = 0;                 // index 12
    virtual void vf3C() = 0;                 // index 13
    virtual int vf40(u32 a) = 0;             // index 14 -> vtable offset 0x40
    virtual void vf44() = 0;                 // index 15
    virtual void vf48() = 0;                 // index 16
    virtual void vf4C() = 0;                 // index 17
    virtual void vf50() = 0;                 // index 18
    virtual void vf54() = 0;                 // index 19
    virtual void vf58() = 0;                 // index 20
    virtual void vf5C() = 0;                 // index 21
    virtual void vf60() = 0;                 // index 22
    virtual void vf64() = 0;                 // index 23
    virtual void vf68() = 0;                 // index 24
    virtual void vf6C() = 0;                 // index 25
    virtual void vf70() = 0;                 // index 26
    virtual void vf74() = 0;                 // index 27
    virtual void vf78() = 0;                 // index 28
    virtual void vf7C() = 0;                 // index 29
    virtual void vf80() = 0;                 // index 30
    virtual void vf84() = 0;                 // index 31
    virtual void vf88() = 0;                 // index 32
    virtual void vf8C() = 0;                 // index 33
    virtual void vf90() = 0;                 // index 34
    virtual void vf94() = 0;                 // index 35
    virtual void vf98() = 0;                 // index 36
    virtual void vf9C() = 0;                 // index 37
    virtual void vfA0() = 0;                 // index 38
    virtual void vfA4() = 0;                 // index 39
    virtual void vfA8() = 0;                 // index 40
    virtual void vfAC() = 0;                 // index 41
    virtual void vfB0() = 0;                 // index 42
    virtual void vfB4() = 0;                 // index 43
    virtual void vfB8() = 0;                 // index 44
    virtual void vfBC() = 0;                 // index 45
    virtual void vfC0() = 0;                 // index 46
    virtual void vfC4() = 0;                 // index 47
    virtual void vfC8() = 0;                 // index 48
    virtual void vfCC() = 0;                 // index 49
    virtual void vfD0() = 0;                 // index 50
    virtual void vfD4() = 0;                 // index 51
    virtual void vfD8() = 0;                 // index 52
    virtual void vfDC() = 0;                 // index 53
    virtual void vfE0() = 0;                 // index 54
    virtual void* vfE4() = 0;                // index 55 -> vtable offset 0xe4
    virtual void vfE8() = 0;                 // index 56
    virtual void vfEC() = 0;                 // index 57
    virtual void vfF0() = 0;                 // index 58
    virtual void vfF4() = 0;                 // index 59
    virtual void vfF8() = 0;                 // index 60
    virtual void vfFC() = 0;                 // index 61
    virtual void vf100() = 0;                // index 62 -> vtable offset 0x100

    u8 _pad04[0x10];                         // 0x04-0x13
    void* field_14;                          // 0x14
    void* field_18;                          // 0x18 (battle object base)
    u8 _pad1C[0x0C];                         // 0x1C-0x27
    u8 field_28[0x344];                      // 0x28-0x36B (opaque; address taken)
    u32 field_36C;                           // 0x36C
};

// Object with a u32 token at +0x70 (read by func_800CFFCC).
struct CfObjectImplEneToken {
    u8 _pad00[0x70];                         // 0x00-0x6F
    u32 field_70;                            // 0x70
};

// Battle-actor result of func_800B8A64: u16 flag word at +0xA0 (bit 0 tested
// by func_800D0B04).
struct CfObjectImplEneActor {
    u8 _pad00[0xA0];                         // 0x00-0x9F
    u16 field_A0;                            // 0xA0
};

// Result of self->vfE4(): its vtable slot 0x10 is invoked by func_800D00DC.
class CfObjectImplEneE4 {
public:
    virtual void e00() = 0;                  // index 0
    virtual void e04() = 0;                  // index 1
    virtual void e10() = 0;                  // index 2 -> vtable offset 0x10
};

// Object at field_14->field_98: vtable slot 0x5c takes a float.
class CfObjectImplEne98 {
public:
    virtual void n00() = 0;                  // index 0
    virtual void n04() = 0;                  // index 1
    virtual void n08() = 0;                  // index 2
    virtual void n0C() = 0;                  // index 3
    virtual void n10() = 0;                  // index 4
    virtual void n14() = 0;                  // index 5
    virtual void n18() = 0;                  // index 6
    virtual void n1C() = 0;                  // index 7
    virtual void n20() = 0;                  // index 8
    virtual void n24() = 0;                  // index 9
    virtual void n28() = 0;                  // index 10
    virtual void n2C() = 0;                  // index 11
    virtual void n30() = 0;                  // index 12
    virtual void n34() = 0;                  // index 13
    virtual void n38() = 0;                  // index 14
    virtual void n3C() = 0;                  // index 15
    virtual void n40() = 0;                  // index 16
    virtual void n44() = 0;                  // index 17
    virtual void n48() = 0;                  // index 18
    virtual void n4C() = 0;                  // index 19
    virtual void n50() = 0;                  // index 20
    virtual void n5C(float v) = 0;           // index 21 -> vtable offset 0x5c
};

// Object at field_14->field_C4: float written at +0x50C.
struct CfObjectImplEneC4 {
    u8 _pad00[0x50C];                        // 0x00-0x50B
    float field_50C;                         // 0x50C
};

// self->field_14 target object: vtable slot 0x128 (returns a position object);
// words at +0x90 / +0x98 / +0xC4.
class CfObjectImplEne14 {
public:
    virtual void v00() = 0;                  // index 0
    virtual void v04() = 0;                  // index 1
    virtual void v08() = 0;                  // index 2
    virtual void v0C() = 0;                  // index 3
    virtual void v10() = 0;                  // index 4
    virtual void v14() = 0;                  // index 5
    virtual void v18() = 0;                  // index 6
    virtual void v1C() = 0;                  // index 7
    virtual void v20() = 0;                  // index 8
    virtual void v24() = 0;                  // index 9
    virtual void v28() = 0;                  // index 10
    virtual void v2C() = 0;                  // index 11
    virtual void v30() = 0;                  // index 12
    virtual void v34() = 0;                  // index 13
    virtual void v38() = 0;                  // index 14
    virtual void v3C() = 0;                  // index 15
    virtual void v40() = 0;                  // index 16
    virtual void v44() = 0;                  // index 17
    virtual void v48() = 0;                  // index 18
    virtual void v4C() = 0;                  // index 19
    virtual void v50() = 0;                  // index 20
    virtual void v54() = 0;                  // index 21
    virtual void v58() = 0;                  // index 22
    virtual void v5C() = 0;                  // index 23
    virtual void v60() = 0;                  // index 24
    virtual void v64() = 0;                  // index 25
    virtual void v68() = 0;                  // index 26
    virtual void v6C() = 0;                  // index 27
    virtual void v70() = 0;                  // index 28
    virtual void v74() = 0;                  // index 29
    virtual void v78() = 0;                  // index 30
    virtual void v7C() = 0;                  // index 31
    virtual void v80() = 0;                  // index 32
    virtual void v84() = 0;                  // index 33
    virtual void v88() = 0;                  // index 34
    virtual void v8C() = 0;                  // index 35
    virtual void v90() = 0;                  // index 36
    virtual void v94() = 0;                  // index 37
    virtual void v98() = 0;                  // index 38
    virtual void v9C() = 0;                  // index 39
    virtual void vA0() = 0;                  // index 40
    virtual void vA4() = 0;                  // index 41
    virtual void vA8() = 0;                  // index 42
    virtual void vAC() = 0;                  // index 43
    virtual void vB0() = 0;                  // index 44
    virtual void vB4() = 0;                  // index 45
    virtual void vB8() = 0;                  // index 46
    virtual void vBC() = 0;                  // index 47
    virtual void vC0() = 0;                  // index 48
    virtual void vC4() = 0;                  // index 49
    virtual void vC8() = 0;                  // index 50
    virtual void vCC() = 0;                  // index 51
    virtual void vD0() = 0;                  // index 52
    virtual void vD4() = 0;                  // index 53
    virtual void vD8() = 0;                  // index 54
    virtual void vDC() = 0;                  // index 55
    virtual void vE0() = 0;                  // index 56
    virtual void vE4() = 0;                  // index 57
    virtual void vE8() = 0;                  // index 58
    virtual void vEC() = 0;                  // index 59
    virtual void vF0() = 0;                  // index 60
    virtual void vF4() = 0;                  // index 61
    virtual void vF8() = 0;                  // index 62
    virtual void vFC() = 0;                  // index 63
    virtual void w00() = 0;                  // index 64
    virtual void w04() = 0;                  // index 65
    virtual void w08() = 0;                  // index 66
    virtual void w0C() = 0;                  // index 67
    virtual void w10() = 0;                  // index 68
    virtual void w14() = 0;                  // index 69
    virtual void w18() = 0;                  // index 70
    virtual void w1C() = 0;                  // index 71
    virtual void* v128() = 0;                // index 72 -> vtable offset 0x128

    u8 _pad04[0x90 - 0x04];                  // 0x04-0x8F
    u32 field_90;                            // 0x90
    u8 _pad94[0x98 - 0x94];                  // 0x94-0x97
    void* field_98;                          // 0x98 (CfObjectImplEne98*)
    u8 _pad9C[0xC4 - 0x9C];                  // 0x9C-0xC3
    void* field_C4;                          // 0xC4 (CfObjectImplEneC4*)
};

// Result of func_800AD860(self->field_14): u16 flag word at +0x45CA.
struct CfObjectImplEneFlag {
    u8 _pad00[0x45CA];                       // 0x00-0x45C9
    u16 field_45CA;                          // 0x45CA
};

// Position block returned by the vtable[0x128] target objects: x/y/z floats.
struct CfImplEnePos {
    u8 _pad00[0x0C];                         // 0x00-0x0B
    float field_0C;                          // 0x0C (x)
    u8 _pad10[0x1C - 0x10];                  // 0x10-0x1B
    float field_1C;                          // 0x1C (y)
    u8 _pad20[0x2C - 0x20];                  // 0x20-0x2B
    float field_2C;                          // 0x2C (z)
};

// battleObj->field_3F34 target: word at +0x7A4 (bit 27 read by func_800D0C2C).
struct CfImplEneField3F34 {
    u8 _pad00[0x7A4];                        // 0x00-0x7A3
    u32 field_7A4;                           // 0x7A4
};

// Result of CBattleManager::func_800EA444: two id words compared against
// battleObj->field_3F10.
struct CfImplEneTarget {
    u32 field_00;                            // 0x00
    u32 field_04;                            // 0x04
};

// Enum list returned by func_80043F18: element count at +0x620.
struct CfEnumList {
    u8 _pad00[0x620];                        // 0x00-0x61F
    u32 field_620;                           // 0x620
};

// battleObj->field_04: sub-object whose vtable slot 0x30 returns a word holder
// (first word read as the actor id by func_80174C98 callers).
class CfImplEneBattleObj4 {
public:
    virtual void bf00() = 0;                 // index 0
    virtual void bf04() = 0;                 // index 1
    virtual void bf08() = 0;                 // index 2
    virtual void bf0C() = 0;                 // index 3
    virtual void bf10() = 0;                 // index 4
    virtual void bf14() = 0;                 // index 5
    virtual void bf18() = 0;                 // index 6
    virtual void bf1C() = 0;                 // index 7
    virtual void bf20() = 0;                 // index 8
    virtual void bf24() = 0;                 // index 9
    virtual void* bf30() = 0;                // index 10 -> vtable offset 0x30
};

// battleObj->field_3F60: vtable slot 0x80 called with (int); word at +0x4EC.
class CfImplEneField3F60 {
public:
    virtual void g00() = 0;                  // index 0
    virtual void g04() = 0;                  // index 1
    virtual void g08() = 0;                  // index 2
    virtual void g0C() = 0;                  // index 3
    virtual void g10() = 0;                  // index 4
    virtual void g14() = 0;                  // index 5
    virtual void g18() = 0;                  // index 6
    virtual void g1C() = 0;                  // index 7
    virtual void g20() = 0;                  // index 8
    virtual void g24() = 0;                  // index 9
    virtual void g28() = 0;                  // index 10
    virtual void g2C() = 0;                  // index 11
    virtual void g30() = 0;                  // index 12
    virtual void g34() = 0;                  // index 13
    virtual void g38() = 0;                  // index 14
    virtual void g3C() = 0;                  // index 15
    virtual void g40() = 0;                  // index 16
    virtual void g44() = 0;                  // index 17
    virtual void g48() = 0;                  // index 18
    virtual void g4C() = 0;                  // index 19
    virtual void g50() = 0;                  // index 20
    virtual void g54() = 0;                  // index 21
    virtual void g58() = 0;                  // index 22
    virtual void g5C() = 0;                  // index 23
    virtual void g60() = 0;                  // index 24
    virtual void g64() = 0;                  // index 25
    virtual void g68() = 0;                  // index 26
    virtual void g6C() = 0;                  // index 27
    virtual void g70() = 0;                  // index 28
    virtual void g74() = 0;                  // index 29
    virtual int g80(int a) = 0;              // index 30 -> vtable offset 0x80

    u8 _pad04[0x4EC - 0x04];                 // 0x04-0x4EB
    u32 field_4EC;                           // 0x4EC
};

// Sub-object rooted at +0x3E9C of the battle object: its own vtable, slot
// 0x4C called by func_800D0B04. Declared as a member of CfImplEneBattleObj,
// so the virtuals are non-pure (like CHelp_EnemyEnableSub) - the class is
// never instantiated, so no out-of-line definitions or vtable are emitted.
class CfImplEneSub {
public:
    virtual void sf00();                     // index 0
    virtual void sf04();                     // index 1
    virtual void sf08(u32 a);                // index 2 -> vtable offset 0x8
    virtual int sf0C(u32 a);                 // index 3 -> vtable offset 0xc
    virtual void sf10();                     // index 4
    virtual void sf14();                     // index 5
    virtual void sf18();                     // index 6
    virtual void sf1C();                     // index 7
    virtual void sf20();                     // index 8
    virtual void sf24();                     // index 9
    virtual void sf28();                     // index 10
    virtual void sf2C();                     // index 11
    virtual void sf30();                     // index 12
    virtual void sf34();                     // index 13
    virtual void sf38();                     // index 14
    virtual void sf3C();                     // index 15
    virtual void sf40();                     // index 16
    virtual void* sf4C();                    // index 17 -> vtable offset 0x4c
    virtual void sf50();                     // index 18
    virtual void sf54();                     // index 19
    virtual void sf58();                     // index 20
    virtual void sf5C();                     // index 21
    virtual void sf60();                     // index 22
    virtual void sf64();                     // index 23
    virtual void sf68();                     // index 24
    virtual void sf6C();                     // index 25
    virtual void sf70();                     // index 26
    virtual void sf74();                     // index 27
    virtual void sf78();                     // index 28
    virtual void sf7C();                     // index 29
    virtual void sf80();                     // index 30
    virtual void sf84();                     // index 31
    virtual void sf88();                     // index 32
    virtual void sf8C();                     // index 33
    virtual void sf90();                     // index 34
    virtual void sf94();                     // index 35
    virtual void sf98();                     // index 36
    virtual void sf9C();                     // index 37
    virtual void sfA0();                     // index 38
    virtual void sfA4();                     // index 39
    virtual void sfA8();                     // index 40
    virtual void sfAC();                     // index 41
    virtual void sfB0();                     // index 42
    virtual void sfB4();                     // index 43
    virtual void sfB8();                     // index 44
    virtual void sfBC();                     // index 45
    virtual void sfC0();                     // index 46
    virtual void sfC4();                     // index 47
    virtual void sfC8();                     // index 48
    virtual void sfCC();                     // index 49
    virtual void sfD0();                     // index 50
    virtual void sfD4();                     // index 51
    virtual void sfD8();                     // index 52
    virtual void sfDC();                     // index 53
    virtual void sfE0();                     // index 54
    virtual void sfE4();                     // index 55
    virtual void sfE8();                     // index 56
    virtual void sfEC();                     // index 57
    virtual void sfF0();                     // index 58
    virtual void sfF4();                     // index 59
    virtual void sfF8();                     // index 60
    virtual void sfFC();                     // index 61
    virtual void sg00();                     // index 62
    virtual void sg04();                     // index 63
    virtual void sg08();                     // index 64
    virtual void sg0C();                     // index 65
    virtual void sg10();                     // index 66
    virtual void sg14();                     // index 67
    virtual void sg18();                     // index 68
    virtual void sg1C();                     // index 69
    virtual void sg20();                     // index 70
    virtual void sg24();                     // index 71
    virtual void* sg28();                    // index 72 -> vtable offset 0x128
    virtual void sg2C();                     // index 73
    virtual void sg30();                     // index 74
    virtual void sg34();                     // index 75
    virtual void sg38();                     // index 76
    virtual void sg3C();                     // index 77
    virtual void sg40();                     // index 78
    virtual void sg44();                     // index 79
    virtual void sg48();                     // index 80
    virtual void sg4C();                     // index 81
    virtual void sg50();                     // index 82
    virtual void sg54();                     // index 83
    virtual void sg58();                     // index 84
    virtual void sg5C();                     // index 85
    virtual void sg60();                     // index 86
    virtual void sg64();                     // index 87
    virtual void sg68();                     // index 88
    virtual float sg6C();                    // index 89 -> vtable offset 0x16c
    virtual void sg70();                     // index 90
    virtual void sg74();                     // index 91
    virtual void sg78();                     // index 92
    virtual void sg7C();                     // index 93
    virtual void sg80();                     // index 94
    virtual void sg84();                     // index 95
    virtual void sg88();                     // index 96
    virtual void sg8C();                     // index 97
    virtual void sg90();                     // index 98
    virtual void sg94();                     // index 99
    virtual void sg98();                     // index 100
    virtual void sg9C();                     // index 101
    virtual void sgA0();                     // index 102
    virtual void sgA4();                     // index 103
    virtual void sgA8();                     // index 104
    virtual void sgAC();                     // index 105
    virtual void sgB0();                     // index 106
    virtual void sgB4();                     // index 107
    virtual void sgB8();                     // index 108
    virtual void sgBC();                     // index 109
    virtual void sgC0();                     // index 110
    virtual void sgC4();                     // index 111
    virtual void sgC8();                     // index 112
    virtual void sgCC();                     // index 113
    virtual void sgD0();                     // index 114
    virtual void sgD4();                     // index 115
    virtual void sgD8();                     // index 116
    virtual void sgDC();                     // index 117
    virtual void sgE0();                     // index 118
    virtual void sgE4();                     // index 119
    virtual void sgE8();                     // index 120
    virtual void sgEC();                     // index 121
    virtual void sgF0();                     // index 122
    virtual void sgF4();                     // index 123
    virtual void sgF8();                     // index 124
    virtual void sgFC();                     // index 125
    virtual void sh00();                     // index 126
    virtual void sh04();                     // index 127
    virtual void sh08();                     // index 128
    virtual void sh0C();                     // index 129
    virtual int sh10(u32 a);                 // index 130 -> vtable offset 0x210
};

// Battle object (target of self->field_18 / func_800D0B04 loop): vtable slot
// 0x2C4 called with (int, float, float, float); sub-object at +0x3E9C;
// u32 flags at +0x3F34 and +0x3F60.
class CfImplEneBattleObj {
public:
    virtual void bf08() = 0;                 // index 0 (vtable offset 0x08)
    virtual void bf0C() = 0;                 // index 1
    virtual void bf10() = 0;                 // index 2
    virtual void bf14() = 0;                 // index 3
    virtual void bf18() = 0;                 // index 4
    virtual void bf1C() = 0;                 // index 5
    virtual void bf20() = 0;                 // index 6
    virtual void bf24() = 0;                 // index 7
    virtual void bf28() = 0;                 // index 8
    virtual void bf2C() = 0;                 // index 9
    virtual void bf30() = 0;                 // index 10
    virtual void bf34() = 0;                 // index 11
    virtual void bf38() = 0;                 // index 12
    virtual void bf3C() = 0;                 // index 13
    virtual void bf40() = 0;                 // index 14
    virtual void bf44() = 0;                 // index 15
    virtual void bf48() = 0;                 // index 16
    virtual void bf4C() = 0;                 // index 17
    virtual void bf50() = 0;                 // index 18
    virtual void bf54() = 0;                 // index 19
    virtual void bf58() = 0;                 // index 20
    virtual void bf5C() = 0;                 // index 21
    virtual void bf60() = 0;                 // index 22
    virtual void bf64() = 0;                 // index 23
    virtual void bf68() = 0;                 // index 24
    virtual void bf6C() = 0;                 // index 25
    virtual void bf70() = 0;                 // index 26
    virtual void bf74() = 0;                 // index 27
    virtual void bf78() = 0;                 // index 28
    virtual void bf7C() = 0;                 // index 29
    virtual void bf80() = 0;                 // index 30
    virtual void bf84() = 0;                 // index 31
    virtual void bf88() = 0;                 // index 32
    virtual void bf8C() = 0;                 // index 33
    virtual void bf90() = 0;                 // index 34
    virtual void bf94() = 0;                 // index 35
    virtual void bf98() = 0;                 // index 36
    virtual void bf9C() = 0;                 // index 37
    virtual void bfA0() = 0;                 // index 38
    virtual void bfA4() = 0;                 // index 39
    virtual void bfA8() = 0;                 // index 40
    virtual void bfAC() = 0;                 // index 41
    virtual void bfB0() = 0;                 // index 42
    virtual void bfB4() = 0;                 // index 43
    virtual void bfB8() = 0;                 // index 44
    virtual void bfBC() = 0;                 // index 45
    virtual void bfC0() = 0;                 // index 46
    virtual void bfC4() = 0;                 // index 47
    virtual void bfC8() = 0;                 // index 48
    virtual void bfCC() = 0;                 // index 49
    virtual void bfD0() = 0;                 // index 50
    virtual void bfD4() = 0;                 // index 51
    virtual float bfD8() = 0;                // index 52 -> vtable offset 0xd8
    virtual void bfDC() = 0;                 // index 53
    virtual int bfE0() = 0;                  // index 54 -> vtable offset 0xe0
    virtual void bfE4() = 0;                 // index 55
    virtual void bfE8() = 0;                 // index 56
    virtual void bfEC() = 0;                 // index 57
    virtual void bfF0() = 0;                 // index 58
    virtual void bfF4() = 0;                 // index 59
    virtual void bfF8() = 0;                 // index 60
    virtual void bfFC() = 0;                 // index 61
    virtual void bg00() = 0;                 // index 62
    virtual void bg04() = 0;                 // index 63
    virtual void bg08() = 0;                 // index 64
    virtual void bg0C() = 0;                 // index 65
    virtual void bg10() = 0;                 // index 66
    virtual void bg14() = 0;                 // index 67
    virtual void bg18(float v) = 0;          // index 68 -> vtable offset 0x118
    virtual void bg1C() = 0;                 // index 69
    virtual void bg20() = 0;                 // index 70
    virtual void bg24() = 0;                 // index 71
    virtual float bg28() = 0;                // index 72 -> vtable offset 0x128
    virtual float bg2C() = 0;                // index 73 -> vtable offset 0x12c
    virtual void bg30() = 0;                 // index 74
    virtual void bg34() = 0;                 // index 75
    virtual void bg38() = 0;                 // index 76
    virtual void bg3C() = 0;                 // index 77
    virtual void bg40() = 0;                 // index 78
    virtual void bg44() = 0;                 // index 79
    virtual void bg48() = 0;                 // index 80
    virtual void bg4C() = 0;                 // index 81
    virtual void bg50() = 0;                 // index 82
    virtual void bg54() = 0;                 // index 83
    virtual void bg58() = 0;                 // index 84
    virtual void bg5C() = 0;                 // index 85
    virtual void bg60() = 0;                 // index 86
    virtual void bg64() = 0;                 // index 87
    virtual void bg68() = 0;                 // index 88
    virtual void bg6C() = 0;                 // index 89
    virtual void bg70() = 0;                 // index 90
    virtual void bg74() = 0;                 // index 91
    virtual void bg78() = 0;                 // index 92
    virtual void bg7C() = 0;                 // index 93
    virtual void bg80() = 0;                 // index 94
    virtual void bg84() = 0;                 // index 95
    virtual void bg88() = 0;                 // index 96
    virtual void bg8C() = 0;                 // index 97
    virtual void bg90() = 0;                 // index 98
    virtual void bg94() = 0;                 // index 99
    virtual void bg98() = 0;                 // index 100
    virtual void bg9C() = 0;                 // index 101
    virtual void bgA0() = 0;                 // index 102
    virtual void bgA4() = 0;                 // index 103
    virtual void bgA8() = 0;                 // index 104
    virtual void bgAC() = 0;                 // index 105
    virtual void bgB0() = 0;                 // index 106
    virtual void bgB4() = 0;                 // index 107
    virtual void bgB8() = 0;                 // index 108
    virtual void bgBC() = 0;                 // index 109
    virtual void bgC0() = 0;                 // index 110
    virtual void bgC4() = 0;                 // index 111
    virtual void bgC8() = 0;                 // index 112
    virtual void bgCC() = 0;                 // index 113
    virtual void bgD0() = 0;                 // index 114
    virtual void bgD4() = 0;                 // index 115
    virtual void bgD8() = 0;                 // index 116
    virtual void bgDC() = 0;                 // index 117
    virtual void bgE0() = 0;                 // index 118
    virtual void bgE4() = 0;                 // index 119
    virtual void bgE8() = 0;                 // index 120
    virtual void bgEC() = 0;                 // index 121
    virtual void bgF0() = 0;                 // index 122
    virtual void bgF4() = 0;                 // index 123
    virtual void bgF8() = 0;                 // index 124
    virtual void bgFC() = 0;                 // index 125
    virtual void bh00() = 0;                 // index 126
    virtual void bh04() = 0;                 // index 127
    virtual void bh08() = 0;                 // index 128
    virtual void bh0C() = 0;                 // index 129
    virtual void bh10() = 0;                 // index 130
    virtual void bh14() = 0;                 // index 131
    virtual void bh18() = 0;                 // index 132
    virtual void bh1C() = 0;                 // index 133
    virtual void bh20() = 0;                 // index 134
    virtual void bh24() = 0;                 // index 135
    virtual void bh28() = 0;                 // index 136
    virtual void bh2C() = 0;                 // index 137
    virtual int bh30() = 0;                  // index 138 -> vtable offset 0x230
    virtual void* bh34() = 0;                // index 139 -> vtable offset 0x234
    virtual void bh38() = 0;                 // index 140
    virtual void bh3C() = 0;                 // index 141
    virtual void bh40() = 0;                 // index 142
    virtual void bh44() = 0;                 // index 143
    virtual void bh48() = 0;                 // index 144
    virtual void bh4C() = 0;                 // index 145
    virtual void bh50() = 0;                 // index 146
    virtual void bh54() = 0;                 // index 147
    virtual void bh58() = 0;                 // index 148
    virtual void bh5C() = 0;                 // index 149
    virtual void bh60() = 0;                 // index 150
    virtual void bh64() = 0;                 // index 151
    virtual void bh68() = 0;                 // index 152
    virtual void bh6C() = 0;                 // index 153
    virtual void bh70() = 0;                 // index 154
    virtual void bh74() = 0;                 // index 155
    virtual void bh78() = 0;                 // index 156
    virtual void bh7C() = 0;                 // index 157
    virtual void bh80() = 0;                 // index 158
    virtual void bh84() = 0;                 // index 159
    virtual void bh88() = 0;                 // index 160
    virtual void bh8C() = 0;                 // index 161
    virtual void bh90() = 0;                 // index 162
    virtual void bh94() = 0;                 // index 163
    virtual void bh98() = 0;                 // index 164
    virtual void bh9C() = 0;                 // index 165
    virtual void bhA0() = 0;                 // index 166
    virtual void bhA4() = 0;                 // index 167
    virtual void bhA8() = 0;                 // index 168
    virtual void bhAC() = 0;                 // index 169
    virtual void bhB0() = 0;                 // index 170
    virtual void bhB4() = 0;                 // index 171
    virtual void bhB8() = 0;                 // index 172
    virtual int bhBC() = 0;                  // index 173 -> vtable offset 0x2bc
    virtual void bhC0() = 0;                 // index 174
    virtual void bhC4(int a, float b, float c, float d) = 0;  // index 175 -> 0x2c4
    virtual void bhC8() = 0;                 // index 176
    virtual void bhCC() = 0;                 // index 177
    virtual void bhD0() = 0;                 // index 178
    virtual void bhD4() = 0;                 // index 179
    virtual void bhD8() = 0;                 // index 180
    virtual void bhDC() = 0;                 // index 181
    virtual void bhE0() = 0;                 // index 182
    virtual void bhE4() = 0;                 // index 183
    virtual void bhE8() = 0;                 // index 184
    virtual void bhEC() = 0;                 // index 185
    virtual void bhF0() = 0;                 // index 186
    virtual void bhF4() = 0;                 // index 187
    virtual void bhF8() = 0;                 // index 188
    virtual void bhFC() = 0;                 // index 189
    virtual void bi00() = 0;                 // index 190
    virtual void bi04() = 0;                 // index 191
    virtual void bi08() = 0;                 // index 192
    virtual void bi0C() = 0;                 // index 193
    virtual void bi10() = 0;                 // index 194
    virtual void bi14() = 0;                 // index 195
    virtual void bi18() = 0;                 // index 196
    virtual void bi1C() = 0;                 // index 197
    virtual void bi20() = 0;                 // index 198
    virtual void bi24() = 0;                 // index 199
    virtual void bi28() = 0;                 // index 200
    virtual void bi2C() = 0;                 // index 201
    virtual void bi30() = 0;                 // index 202
    virtual void bi34() = 0;                 // index 203
    virtual void bi38() = 0;                 // index 204
    virtual void bi3C() = 0;                 // index 205
    virtual void bi40() = 0;                 // index 206
    virtual int bi44() = 0;                  // index 207 -> vtable offset 0x344

    CfImplEneBattleObj4* field_04;           // 0x04
    u8 _pad08[0x3374 - 0x08];                // 0x08-0x3373
    u32 field_3374;                          // 0x3374 (flags)
    u8 _pad3378[0x3e9c - 0x3378];            // 0x3378-0x3E9B
    CfImplEneSub mSub;                       // 0x3E9C
    u8 _pad3ea0[0x3f04 - 0x3ea0];            // 0x3EA0-0x3F03
    u32 field_3F04;                          // 0x3F04 (flags)
    u8 _pad3f08[0x3f10 - 0x3f08];            // 0x3F08-0x3F0F
    u32 field_3F10;                          // 0x3F10
    u8 _pad3f14[0x3f28 - 0x3f14];            // 0x3F14-0x3F27
    u16 field_3F28;                          // 0x3F28 (battle id)
    u8 _pad3f2a[0x3f34 - 0x3f2a];            // 0x3F2A-0x3F33
    u32 field_3F34;                          // 0x3F34
    u8 _pad3f38[0x3f60 - 0x3f38];            // 0x3F38-0x3F5F
    u32 field_3F60;                          // 0x3F60
};

} // namespace cf

// ---------------------------------------------------------------------------
// C-ABI imports (retail symbols are unmangled).
// ---------------------------------------------------------------------------
extern "C" {
void func_800CA948(void* self);
void func_800CAA44(void* self);
void func_800CD5DC(void* self, u32 id, u32 kind, u32 x, u32 y, u32 z);
void func_8015BB3C(void* a, void* b, void* c);
void func_802A0B8C(void* self, void* owner);
void* func_800B8A64(void* self);
void* func_8016FE34(void* src);
void* func_801984E4(void* self, unsigned long idx);
int func_800DA06C(void* self, unsigned int value);
void func_800D9978(void* mgr, void* obj);
int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
void func_801BFE8C(u32 a, u32 b, u32 c);
void func_804E3CCC(void* effect);
void func_804E3D48(void* effect, void* parent);
void func_800CA964(void* self);
void func_800CAB2C(void* self);
void func_800CED64(void* self, int flag);
void func_802A0E08(void* self);
void* func_800630C8();
void* func_80066E7C(void* self, u32 id);
void func_804E3B08(void* effect);
float func_80484EB0(void* obj);
void func_800BC3D8(void* player, float value);
void func_800BE824(void* obj, int flag);
void func_800F3C6C(void* mgr, u32 key);
void func_800E9B54(void* mgr, void* obj, int a, int b);
void func_800D9CA0(void* mgr, void* target);
void func_80197BA4(void* obj, int a, int b);
void func_80140E00(u32 a, u32 b, u32 c);
void func_8013E2E0(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9);
void func_80043D90(void* holder);
void* func_80043F18(void* holder);
void func_800F4A98(void* list, u32 type, u32 filter);
void* func_800F6EAC(void* list, u32 idx);
void __dt__80043E88(void* holder, int);
void func_80174B4C(void* actor, u32 flags);
}

// C++-mangled imports (declared so MWCC emits the retail mangled names).
void* func_800AD860(void* obj);   // func_800AD860__FPv
bool func_8006EF04(int mask);     // func_8006EF04__Fi
// func_800B708C__Fi is declared (void* return) by code_802B8A3C.hpp via
// CBattleManager.hpp - same mangled name, do not redeclare here.

// sdata2 / sdata literals referenced by this unit (global scope: not mangled).
extern f32 lbl_eu_80666CEC;
extern f32 lbl_eu_80666CE4;
extern const char* lbl_eu_80661D48;
extern f32 lbl_eu_80666CE0;
extern f32 lbl_eu_80666CE8;
extern f32 lbl_eu_80666CF0;
extern f32 lbl_eu_80666CF4;
extern f32 lbl_eu_8066A208;

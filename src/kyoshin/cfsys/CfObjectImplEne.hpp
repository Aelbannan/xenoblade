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
    virtual void vf40() = 0;                 // index 14
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
    virtual void vfE4() = 0;                 // index 55 -> vtable offset 0xe4

    u8 _pad04[0x10];                         // 0x04-0x13
    void* field_14;                          // 0x14
    CfImplEneBattleObj* field_18;            // 0x18 (battle object base)
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

// Sub-object rooted at +0x3E9C of the battle object: its own vtable, slot
// 0x4C called by func_800D0B04. Declared as a member of CfImplEneBattleObj,
// so the virtuals are non-pure (like CHelp_EnemyEnableSub) — the class is
// never instantiated, so no out-of-line definitions or vtable are emitted.
class CfImplEneSub {
public:
    virtual void sf00();                     // index 0
    virtual void sf04();                     // index 1
    virtual void sf08();                     // index 2
    virtual void sf0C();                     // index 3
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
    virtual void bfD8() = 0;                 // index 52
    virtual void bfDC() = 0;                 // index 53
    virtual void bfE0() = 0;                 // index 54
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
    virtual void bg18() = 0;                 // index 68
    virtual void bg1C() = 0;                 // index 69
    virtual void bg20() = 0;                 // index 70
    virtual void bg24() = 0;                 // index 71
    virtual void bg28() = 0;                 // index 72
    virtual void bg2C() = 0;                 // index 73
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
    virtual void bh30() = 0;                 // index 138
    virtual void bh34() = 0;                 // index 139
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
    virtual void bhBC() = 0;                 // index 173
    virtual void bhC0() = 0;                 // index 174
    virtual void bhC4(int a, float b, float c, float d) = 0;  // index 175 -> 0x2c4

    u8 _pad04[0x3e9c - 0x04];                // 0x04-0x3E9B
    CfImplEneSub mSub;                       // 0x3E9C
    u8 _pad3ea0[0x3f34 - 0x3ea0];            // 0x3EA0-0x3F33
    u32 field_3F34;                          // 0x3F34
    u8 _pad3f38[0x3f60 - 0x3f38];            // 0x3F38-0x3F5F
    void* field_3F60;                        // 0x3F60 (token/id pointer)
};

} // namespace cf

// ---------------------------------------------------------------------------
// C-ABI imports (retail symbols are unmangled).
// ---------------------------------------------------------------------------
extern "C" {
void func_800CA948(void* self);
void func_800CAA44(void* self);
void func_800CD5DC(void* self);
void func_8015BB3C(void* a, void* b);
void func_802A0B8C(void* self, void* owner);
void* func_800B8A64(void* self);
void* func_8016FE34(int id);
void* func_801984E4(void* self, unsigned long idx);
int func_800DA06C(void* self, unsigned int value);
void func_800D9978(void* mgr, void* obj);
int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
void func_801BFE8C(u32 a, u32 b, u32 c);
void func_804E3CCC(void* effect);
void func_804E3D48(void* effect, void* parent);
}

// C++-mangled imports (declared so MWCC emits the retail mangled names).
void* func_800AD860(void* obj);   // func_800AD860__FPv
// func_800B708C__Fi is declared (void* return) by code_802B8A3C.hpp via
// CBattleManager.hpp — same mangled name, do not redeclare here.

// sdata2 / sdata literals referenced by this unit (global scope: not mangled).
extern f32 lbl_eu_80666CEC;
extern f32 lbl_eu_80666CE4;
extern const char* lbl_eu_80661D48;

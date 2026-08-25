#pragma once

#include <types.h>

// Internal driver struct for CfObjectImplPc.
// Function func_800CA274 accesses a sub-object at +0x18 and reads/writes an int at +0x3e98.
struct CfObjectImplPcSubObj;
struct CfObjectImplPcData {
    u8 _00_17[0x18];              // 0x00-0x17
    CfObjectImplPcSubObj* mSubObj;                // 0x18, sub-object with field at +0x3e98
};

struct CfObjectImplPcSubObj {
    u8 _00_3E97[0x3e98];          // 0x00-0x3e97
    int mSomeValue;               // 0x3e98
};

namespace cf {

class CfObjectMove;  // forward decl (func_800BFC68 signature)

// ---------------------------------------------------------------------------
// vtable dispatch views (never instantiated; no vtables emitted).
// With -RTTI on, MWCC reserves two leading vtable slots (offset-to-top +
// typeinfo), so the virtual at declared index N sits at vtable offset (N+2)*4.
// ---------------------------------------------------------------------------

// Sub-object at CfObjectImplPc::field_378: vtable slots 0x08 / 0x0C / 0x10.
class CfObjectImplPcSub {
public:
    virtual void vf08(u32 flag) = 0;                  // index 0 -> vtable offset 0x08
    virtual void vf0C(u32 a, u32 b, u32 c, u32 d) = 0; // index 1 -> vtable offset 0x0C
    virtual void vf10() = 0;                          // index 2 -> vtable offset 0x10
    virtual void s14() = 0;                           // index 3 -> vtable offset 0x14
};

// Vtable-dispatch view over CfObjectImplPc3F60 for slot 0x80 (index 30);
// called with a flag word, result gates the player-target sync.
class CfObjectImplPc3F60V80 {
public:
    virtual void q08();
    virtual void q0C();
    virtual void q10();
    virtual void q14();
    virtual void q18();
    virtual void q1C();
    virtual void q20();
    virtual void q24();
    virtual void q28();
    virtual void q2C();
    virtual void q30();
    virtual void q34();
    virtual void q38();
    virtual void q3C();
    virtual void q40();
    virtual void q44();
    virtual void q48();
    virtual void q4C();
    virtual void q50();
    virtual void q54();
    virtual void q58();
    virtual void q5C();
    virtual void q60();
    virtual void q64();
    virtual void q68();
    virtual void q6C();
    virtual void q70();
    virtual void q74();
    virtual void q78();
    virtual void q7C();
    virtual u32 vf80(u32 flag);      // index 30 -> vtable offset 0x80
};

// Object referenced by CfObjectImplPc18::field_3F60 / field_3F64 /
// field_3F68 (also compared against raw battle ids as integers).
struct CfObjectImplPc3F60 {
    u8 _pad00[0x0C];                 // 0x00-0x0B
    u32 field_0C;                    // 0x0C
    u8 _pad10[0x374 - 0x10];
    s32 field_374;                   // 0x374 state word
    u8 _pad378[0x4B4 - 0x378];
    u32 field_4B4;                   // 0x4B4
    u8 _pad4B8[0x4C8 - 0x4B8];
    void* field_4C8;                 // 0x4C8
    u8 _pad4CC[0x4EC - 0x4CC];
    u32 field_4EC;                   // 0x4EC flags (bit 30)
};

// Battle object's +0x04 sub-object: vtable slots 0x20 (flag setter) and 0x30
// (returns a word holder whose first word is read as an id) are called by
// func_800C86E8. Never instantiated, so no vtable is emitted.
class CfObjectImplPcBattle4 {
public:
    virtual void b08();              // index 0
    virtual void b0C();              // index 1
    virtual void b10();              // index 2
    virtual void b14();              // index 3
    virtual void b18();              // index 4
    virtual void b1C();              // index 5
    virtual void vf20(u32 flag);     // index 6 -> vtable offset 0x20
    virtual void b24();              // index 7
    virtual void b28();              // index 8
    virtual void b2C();              // index 9
    virtual u32* vf30();             // index 10 -> vtable offset 0x30
};

// Embedded sub-object at +0x3E9C of the player battle object (the
// CfObjectMove part): vtable slots 0x4C / 0x50 / 0x104 / 0x204 / 0x208 /
// 0x20C / 0x210 are called by this TU. Never instantiated, so no vtable is
// emitted.
class CfObjectImplPcSub3E9C {
public:
    virtual void v08(u32 flag);      // index 0
    virtual int v0C(u32 flag);       // index 1 (result checked)
    virtual void v10();              // index 2
    virtual void v14();              // index 3
    virtual void v18();              // index 4
    virtual void v1C();              // index 5
    virtual void v20();              // index 6
    virtual void v24();              // index 7
    virtual void v28();              // index 8
    virtual void v2C();              // index 9
    virtual void v30();              // index 10
    virtual void v34();              // index 11
    virtual void v38();              // index 12
    virtual void v3C();              // index 13
    virtual void v40();              // index 14
    virtual void v44();              // index 15
    virtual void v48();              // index 16
    virtual u32 sf4C();              // index 17 -> vtable offset 0x4C
    virtual void sf50(u32 a);        // index 18 -> vtable offset 0x50
    virtual void v54();              // index 19
    virtual void v58();              // index 20
    virtual void v5C();              // index 21
    virtual void v60();              // index 22
    virtual void v64();              // index 23
    virtual void v68();              // index 24
    virtual void v6C();              // index 25
    virtual void v70();              // index 26
    virtual void v74();              // index 27
    virtual void v78();              // index 28
    virtual void v7C();              // index 29
    virtual void v80();              // index 30
    virtual void v84();              // index 31
    virtual void v88();              // index 32
    virtual void v8C();              // index 33
    virtual void v90();              // index 34
    virtual void v94();              // index 35
    virtual void v98();              // index 36
    virtual void v9C();              // index 37
    virtual void vA0();              // index 38
    virtual void vA4();              // index 39
    virtual void vA8();              // index 40
    virtual void vAC();              // index 41
    virtual void vB0();              // index 42
    virtual void vB4();              // index 43
    virtual void vB8();              // index 44
    virtual void vBC();              // index 45
    virtual void vC0();              // index 46
    virtual void vC4();              // index 47
    virtual void vC8();              // index 48
    virtual void vCC();              // index 49
    virtual void vD0();              // index 50
    virtual void vD4();              // index 51
    virtual void vD8();              // index 52
    virtual void vDC();              // index 53
    virtual void vE0();              // index 54
    virtual void vE4();              // index 55
    virtual void vE8();              // index 56
    virtual void vEC();              // index 57
    virtual void vF0();              // index 58
    virtual void vF4();              // index 59
    virtual void vF8();              // index 60
    virtual void* vFC();             // index 61
    virtual void v100();             // index 62
    virtual void sf104(const char* name, u32 flag);  // index 63 -> vtable offset 0x104
    virtual void v108();             // index 64
    virtual void v10C();             // index 65
    virtual void* v110();            // index 66
    virtual void v114();             // index 67
    virtual void v118();             // index 68
    virtual void v11C();             // index 69
    virtual void v120();             // index 70
    virtual void v124();             // index 71
    virtual void v128();             // index 72
    virtual void v12C();             // index 73
    virtual void v130();             // index 74
    virtual void v134();             // index 75
    virtual void v138();             // index 76
    virtual void v13C();             // index 77
    virtual void v140();             // index 78
    virtual void v144();             // index 79
    virtual void v148();             // index 80
    virtual void v14C();             // index 81
    virtual void v150();             // index 82
    virtual void v154();             // index 83
    virtual void v158();             // index 84
    virtual void v15C();             // index 85
    virtual u32 v160();              // index 86 -> validity word
    virtual void v164();             // index 87
    virtual void v168();             // index 88
    virtual void v16C();             // index 89
    virtual void v170();             // index 90
    virtual void v174();             // index 91
    virtual void v178();             // index 92
    virtual void v17C();             // index 93
    virtual void v180();             // index 94
    virtual void v184();             // index 95
    virtual void v188();             // index 96
    virtual void v18C();             // index 97
    virtual void v190();             // index 98
    virtual void v194();             // index 99
    virtual void v198();             // index 100
    virtual void v19C();             // index 101
    virtual void v1A0();             // index 102
    virtual void v1A4();             // index 103
    virtual void v1A8();             // index 104
    virtual void v1AC();             // index 105
    virtual void v1B0();             // index 106
    virtual void v1B4();             // index 107
    virtual void v1B8();             // index 108
    virtual void v1BC();             // index 109
    virtual void v1C0();             // index 110
    virtual void v1C4();             // index 111
    virtual void v1C8();             // index 112
    virtual void v1CC();             // index 113
    virtual void v1D0();             // index 114
    virtual void v1D4();             // index 115
    virtual void v1D8();             // index 116
    virtual void v1DC();             // index 117
    virtual void v1E0();             // index 118
    virtual void v1E4();             // index 119
    virtual void v1E8();             // index 120
    virtual void v1EC();             // index 121
    virtual void v1F0();             // index 122
    virtual void v1F4();             // index 123
    virtual void v1F8();             // index 124
    virtual void v1FC();             // index 125
    virtual void v200();             // index 126
    virtual void sf204(u32 a, u32 b, u32 c, u32 d, u32 e);  // index 127 -> 0x204
    virtual void sf208(u32 a);       // index 128 -> vtable offset 0x208
    virtual void sf20C(u32 a);       // index 129 -> vtable offset 0x20C
    virtual u32 sf210(u32 a);        // index 130 -> vtable offset 0x210
    virtual void* sf220(u16 id);     // index 131 -> vtable offset 0x220
};

// Object referenced by CfObjectImplPcSub3E9C::field_C4: signed state word at
// +0x374 (compared signed against 0xB/0xC).
struct CfObjectImplPcC4 {
    u8 _pad00[0x374];
    s32 field_374;                   // 0x374
};

// Data overlay over the +0x3E9C embedded sub-object's scalar fields (the
// dispatch view above carries a vptr, so it cannot also host the data
// layout).
struct CfObjectImplPcSub3E9CData {
    u8 _pad00[0x8C];
    u16 field_8C;                    // 0x08C action state
    u8 _pad8E[0xC4 - 0x8E];
    CfObjectImplPcC4* field_C4;      // 0x0C4
};

// Result of CfObjectImplPc18::vf27C: u16 word at +0x00 (cleared by
// func_800C5998).
struct CfObjectImplPc27C {
    u16 field_00;                    // 0x00
};

// View of the +0x3E9C embedded sub-object used only for the vtable-offset
// 0x10 call that takes a flag word (index 2).
class CfObjectImplPcSubV10 {
public:
    virtual void w00();              // index 0
    virtual void w04();              // index 1
    virtual void vf10(u32 flag);     // index 2 -> vtable offset 0x10
};

// Polymorphic object stored at CfObjectImplPc18::field_3ED4 (source operand
// of the __dynamic_cast in func_800C75D4). Never instantiated here.
class CfObjectImplPcED4 {
public:
    virtual void e08();              // index 0
};

// Result record returned by func_80149154: signed magnitude at +0x10.
struct CfObjectImplPc149154 {
    u8 _pad00[0x10];
    s32 field_10;                    // 0x10
};

// Command-name view of the event record passed to func_800C75D4.
struct CfObjectImplPcEvtCmd {
    u8 _pad00[0x0A];
    u8 field_0A;                     // 0x0A request mode (0x12 gate)
    u8 _pad0B[0x1C - 0x0B];
    char cmd[0x18];                  // 0x1C command name (vs lbl_eu_804FC758)
};

// View of the object returned by func_800FE68C: word at +0x90E4 feeds
// func_800B708C in func_800C891C's actor fallback.
struct CfObjectImplPcFe68C {
    u8 _pad00[0x90E4];
    u32 field_90E4;                  // 0x90E4
};

// Status-effect request built on the stack in func_800C75D4 (memset 0x34).
struct CfObjectImplPcEffArg {
    u32 field_00;                    // 0x00 battle id
    u8 _pad04[0x08 - 0x04];
    u32 field_08;                    // 0x08 flag word
    u16 field_0C;                    // 0x0C effect id (0x5f/0x58/0x59/0x5a)
    u8 _pad0E[0x10 - 0x0E];
    u32 field_10;                    // 0x10 payload word
    u8 _pad14[0x20 - 0x14];
    f32 field_20;                    // 0x20 payload float
    u8 _pad24[0x2E - 0x24];
    u16 field_2E;                    // 0x2E kind
    u8 _pad30[0x34 - 0x30];
};

// Node of the battle manager's intrusive linked list (sentinel head at
// CBattleManager+0x48; next pointer at offset 0).
struct CfBattleChainNode {
    CfBattleChainNode* mNext;        // 0x00
};

// Object embedded at CfObjectImplPc18::+0x08 (arts container): vtable slots
// 0x20 (command dispatch) and 0x30 are called by this TU.
class CfObjectImplPc8 {
public:
    // Concrete (non-pure) so it can be embedded at +0x08; all virtuals are
    // inline, so MWCC emits no vtable for this view.
    virtual void v08() {}            // index 0
    virtual void v0C() {}            // index 1
    virtual void v10() {}            // index 2
    virtual void v14() {}            // index 3
    virtual void v18() {}            // index 4
    virtual void v1C() {}            // index 5
    virtual void vf20(u32 a) {}      // index 6 -> vtable offset 0x20
    virtual void v24() {}            // index 7
    virtual void v28() {}            // index 8
    virtual void v2C() {}            // index 9
    virtual void vf30(u32 a) {}      // index 10 -> vtable offset 0x30
};

// Result of CfObjectImplPc18::v2A4 (+0x29C): target object pointer at +0x50
// (its u16 id at +0x48) and a flag word at +0x78 (bit 0x400).
struct CfObjectImplPc2A4Sub {
    u8 _pad00[0x48];
    u16 field_48;
    u8 _pad4A[0x76 - 0x4A];
    u8 field_76;                     // 0x76 stage byte (<3 test)
};

struct CfObjectImplPc2A4Res {
    u8 _pad00[0x48];
    u16 field_48;                    // 0x48
    u8 _pad4A[0x50 - 0x4A];
    CfObjectImplPc2A4Sub* field_50;  // 0x50
    u8 _pad54[0x74 - 0x54];
    u32 field_74;                    // 0x74 flags (bit 0)
    u32 field_78;                    // 0x78 flags (bit 0x80)
};

// Block at CfObjectImplPc18::+0x3380 (managed by func_8014B2DC): word flag at
// +0x04, u16 flag at +0x08, word flag at +0xAFC.
struct CfObjectImplPc3380 {
    u8 _pad00[0x04];
    u32 field_04;                    // 0x04
    u16 field_08;                    // 0x08
    u8 _pad0A[0x210 - 0x0A];
    u32 field_3590;                  // 0x210 (abs 0x3590)
    u32 field_3594;                  // 0x214 (abs 0x3594)
    u8 _pad218[0xADC - 0x218];
    u32 field_ADC;                   // 0xADC (abs 0x3E5C actor id)
    u8 _padAE0[0xAFC - 0xAE0];
    u32 field_AFC;                   // 0xAFC
};

// Result of the battle object's vtable slot 0x298 (#164): actor id word at
// +0x04 plus a 16-word entry table at +0x08.
struct CfObjectImplPc298Res {
    u8 _pad00[0x04];
    s32 field_04;                    // 0x04 (fed to func_800B708C)
    u32 entries[16];                 // 0x08-0x47
    s32 field_48;                    // 0x48 (signed 7..9 range test)
    u8 _pad4C[0x50 - 0x4C];
    CfObjectImplPc2A4Sub* field_50;  // 0x50
    u8 _pad54[0x74 - 0x54];
    u32 field_74;                    // 0x74 flags (& 0x62)
    u32 field_78;                    // 0x78 flags (bit 0x800)
};

// Manager/effect object handed back by func_8008187C / func_800817BC and by
// the battle object's vtable slot 0x220; vtable slot 0x194 receives the
// event's bit-7 flag. Never instantiated, so no vtable is emitted.
class CfObjectImplPcEffObj {
public:
    virtual void q00();
    virtual void q04();
    virtual void q08();
    virtual void q0C();
    virtual void q10();
    virtual void q14();
    virtual void q18();
    virtual void q1C();
    virtual void q20();
    virtual void q24();
    virtual void q28();
    virtual void q2C();
    virtual void q30();
    virtual void q34();
    virtual void q38();
    virtual void q3C();
    virtual void q40();
    virtual void q44();
    virtual void q48();
    virtual void q4C();
    virtual void q50();
    virtual void q54();
    virtual void q58();
    virtual void q5C();
    virtual void q60();
    virtual void q64();
    virtual void q68();
    virtual void q6C();
    virtual void q70();
    virtual void q74();
    virtual void q78();
    virtual void q7C();
    virtual void q80();
    virtual void q84();
    virtual void q88();
    virtual void q8C();
    virtual void q90();
    virtual void q94();
    virtual void q98();
    virtual void q9C();
    virtual void qA0();
    virtual void qA4();
    virtual void qA8();
    virtual void qAC();
    virtual void qB0();
    virtual void qB4();
    virtual void qB8();
    virtual void qBC();
    virtual void qC0();
    virtual void qC4();
    virtual void qC8();
    virtual void qCC();
    virtual void qD0();
    virtual void qD4();
    virtual void qD8();
    virtual void qDC();
    virtual void qE0();
    virtual void qE4();
    virtual void qE8();
    virtual void qEC();
    virtual void qF0();
    virtual void qF4();
    virtual void qF8();
    virtual void qFC();
    virtual void q100();
    virtual void q104();
    virtual void q108();
    virtual void q10C();
    virtual void q110();
    virtual void q114();
    virtual void q118();
    virtual void q11C();
    virtual void q120();
    virtual void q124();
    virtual void q128();
    virtual void q12C();
    virtual void q130();
    virtual void q134();
    virtual void q138();
    virtual void q13C();
    virtual void q140();
    virtual void q144();
    virtual void q148();
    virtual void q14C();
    virtual void q150();
    virtual void q154();
    virtual void q158();
    virtual void q15C();
    virtual void q160();
    virtual void q164();
    virtual void q168();
    virtual void q16C();
    virtual void q170();
    virtual void q174();
    virtual void q178();
    virtual void q17C();
    virtual void q180();
    virtual void q184();
    virtual void q188();
    virtual void q18C();
    virtual void q190();
    virtual void vf194(u32 flag);    // index 99 -> vtable offset 0x194
};

// Object stored at CfObjectImplPc18::field_4594/4598 whose vtable slot 0xA8
// returns a child id word. Never instantiated, so no vtable is emitted.
class CfObjectImplPcTgt {
public:
    virtual void t00();
    virtual void t04();
    virtual void t08();
    virtual void t0C();
    virtual void t10();
    virtual void t14();
    virtual void t18();
    virtual void t1C();
    virtual void t20();
    virtual void t24();
    virtual void t28();
    virtual void t2C();
    virtual void t30();
    virtual void t34();
    virtual void t38();
    virtual void t3C();
    virtual void t40();
    virtual void t44();
    virtual void t48();
    virtual void t4C();
    virtual void t50();
    virtual void t54();
    virtual void t58();
    virtual void t5C();
    virtual void t60();
    virtual void t64();
    virtual void t68();
    virtual void t6C();
    virtual void t70();
    virtual void t74();
    virtual void t78();
    virtual void t7C();
    virtual void t80();
    virtual void t84();
    virtual void t88();
    virtual void t8C();
    virtual void t90();
    virtual void t94();
    virtual void t98();
    virtual void t9C();
    virtual u32 vfA8();              // index 40 -> vtable offset 0xA8
};

// Object at CfObjectImplPc::field_18 (player battle object): arts list at
// +0x08, embedded sub-object at +0x3E9C, ids at +0x3F10 / +0x3F28, flags at
// +0x3F2C, sub-object at +0x3F60, byte at +0x45B0. Vtable slots 0x27C and
// 0x2C4 are called by this TU. Never instantiated, so no vtable is emitted.
// Result of the battle object's vtable slot 0x2F4 (#187): s16 offset at
// +0x02 (indexes a byte table starting at +0x04) and an f32 scale at +0x10.
// Result of the battle object's vtable slot 0x2F4 (#187): s16 offset at
// +0x02 (indexes a byte table starting at +0x04) and an f32 scale at +0x10.
struct CfObjectImplPc2F4 {
    u8 _pad00[0x02];
    s16 field_02;
    u8 _pad04[0x10 - 0x04];
    f32 field_10;
};

class CfObjectImplPc18 {
public:
    virtual void v08() = 0;          // index 0
    virtual void v0C() = 0;          // index 1
    virtual void v10() = 0;          // index 2
    virtual void v14() = 0;          // index 3
    virtual void v18() = 0;          // index 4
    virtual void v1C() = 0;          // index 5
    virtual void v20() = 0;          // index 6
    virtual void v24() = 0;          // index 7
    virtual void v28() = 0;          // index 8
    virtual void v2C() = 0;          // index 9
    virtual void v30() = 0;          // index 10
    virtual void v34() = 0;          // index 11
    virtual void v38() = 0;          // index 12
    virtual void v3C() = 0;          // index 13
    virtual void v40() = 0;          // index 14
    virtual void v44() = 0;          // index 15
    virtual void v48() = 0;          // index 16
    virtual void v4C() = 0;          // index 17
    virtual void v50() = 0;          // index 18
    virtual void v54() = 0;          // index 19
    virtual void v58() = 0;          // index 20
    virtual void v5C() = 0;          // index 21
    virtual void v60() = 0;          // index 22
    virtual void v64() = 0;          // index 23
    virtual void v68() = 0;          // index 24
    virtual void v6C() = 0;          // index 25
    virtual void v70() = 0;          // index 26
    virtual void v74() = 0;          // index 27
    virtual void v78() = 0;          // index 28
    virtual void v7C() = 0;          // index 29
    virtual void v80() = 0;          // index 30
    virtual void v84() = 0;          // index 31
    virtual void v88() = 0;          // index 32
    virtual void v8C() = 0;          // index 33
    virtual void v90() = 0;          // index 34
    virtual void v94() = 0;          // index 35
    virtual void v98() = 0;          // index 36
    virtual void v9C() = 0;          // index 37
    virtual void vA0() = 0;          // index 38
    virtual void vA4() = 0;          // index 39
    virtual void vA8() = 0;          // index 40
    virtual void vAC() = 0;          // index 41
    virtual void vB0() = 0;          // index 42
    virtual void vB4() = 0;          // index 43
    virtual void vB8() = 0;          // index 44
    virtual void vBC() = 0;          // index 45
    virtual void vC0() = 0;          // index 46
    virtual void vC4() = 0;          // index 47
    virtual void vC8() = 0;          // index 48
    virtual void vCC() = 0;          // index 49
    virtual void vD0() = 0;          // index 50
    virtual void vD4() = 0;          // index 51
    virtual void vD8() = 0;          // index 52
    virtual void vDC() = 0;          // index 53
    virtual void vE0() = 0;          // index 54
    virtual void vE4() = 0;          // index 55
    virtual void vE8() = 0;          // index 56
    virtual void vEC() = 0;          // index 57
    virtual void vF0() = 0;          // index 58
    virtual void vF4() = 0;          // index 59
    virtual void vF8() = 0;          // index 60
    virtual void* vFC() = 0;         // index 61
    virtual void* v100() = 0;        // index 62
    virtual void v104() = 0;         // index 63
    virtual void v108() = 0;         // index 64
    virtual void v10C() = 0;         // index 65
    virtual void v110() = 0;         // index 66
    virtual void v114() = 0;         // index 67
    virtual void v118() = 0;         // index 68
    virtual void v11C(f32 value) = 0;  // index 69
    virtual void v120() = 0;         // index 70
    virtual void v124(f32 value) = 0;  // index 71
    virtual f32 v128() = 0;          // index 72
    virtual f32 v12C() = 0;          // index 73
    virtual void v130() = 0;         // index 74
    virtual void v134() = 0;         // index 75
    virtual void v138() = 0;         // index 76
    virtual void v13C() = 0;         // index 77
    virtual void v140() = 0;         // index 78
    virtual void v144() = 0;         // index 79
    virtual void v148() = 0;         // index 80
    virtual void v14C() = 0;         // index 81
    virtual void v150() = 0;         // index 82
    virtual void vf154(f32 value) = 0;  // index 83 -> vtable offset 0x154
    virtual void v158() = 0;         // index 84
    virtual void v15C() = 0;         // index 85
    virtual f32 v160() = 0;          // index 86
    virtual void v164() = 0;         // index 87
    virtual void v168() = 0;         // index 88
    virtual void v16C() = 0;         // index 89
    virtual void v170() = 0;         // index 90
    virtual void v174() = 0;         // index 91
    virtual void v178() = 0;         // index 92
    virtual void v17C() = 0;         // index 93
    virtual void v180() = 0;         // index 94
    virtual void v184() = 0;         // index 95
    virtual void v188() = 0;         // index 96
    virtual void v18C() = 0;         // index 97
    virtual void v190() = 0;         // index 98
    virtual void v194(u32 flag) = 0;  // index 99 -> vtable offset 0x194
    virtual void v198() = 0;         // index 100
    virtual void v19C() = 0;         // index 101
    virtual void v1A0() = 0;         // index 102
    virtual void v1A4() = 0;         // index 103
    virtual void v1A8() = 0;         // index 104
    virtual void v1AC() = 0;         // index 105
    virtual void v1B0() = 0;         // index 106
    virtual void v1B4() = 0;         // index 107
    virtual void v1B8() = 0;         // index 108
    virtual void v1BC() = 0;         // index 109
    virtual void v1C0() = 0;         // index 110
    virtual void v1C4() = 0;         // index 111
    virtual void v1C8() = 0;         // index 112
    virtual void v1CC() = 0;         // index 113
    virtual void v1D0() = 0;         // index 114
    virtual void v1D4() = 0;         // index 115
    virtual void v1D8() = 0;         // index 116
    virtual void v1DC() = 0;         // index 117
    virtual void v1E0() = 0;         // index 118
    virtual void v1E4() = 0;         // index 119
    virtual void v1E8() = 0;         // index 120
    virtual void v1EC() = 0;         // index 121
    virtual void v1F0() = 0;         // index 122
    virtual void v1F4() = 0;         // index 123
    virtual void v1F8() = 0;         // index 124
    virtual void v1FC() = 0;         // index 125
    virtual void v200() = 0;         // index 126
    virtual void v204() = 0;         // index 127
    virtual void v208() = 0;         // index 128
    virtual void v20C() = 0;         // index 129
    virtual void v210() = 0;         // index 130
    virtual void v214() = 0;         // index 131
    virtual void v218() = 0;         // index 132
    virtual void v21C() = 0;         // index 133
    virtual void v220() = 0;         // index 134
    virtual void v224() = 0;         // index 135
    virtual void v228() = 0;         // index 136
    virtual void v22C() = 0;         // index 137
    virtual void v230() = 0;         // index 138
    virtual void v234() = 0;         // index 139
    virtual void v238() = 0;         // index 140
    virtual void v23C() = 0;         // index 141
    virtual void v240() = 0;         // index 142
    virtual void v244() = 0;         // index 143
    virtual void v248() = 0;         // index 144
    virtual void v24C() = 0;         // index 145
    virtual void v250() = 0;         // index 146
    virtual void v254() = 0;         // index 147
    virtual void v258() = 0;         // index 148
    virtual void v25C() = 0;         // index 149
    virtual void v260() = 0;         // index 150
    virtual void v264() = 0;         // index 151
    virtual void v268() = 0;         // index 152
    virtual void v26C() = 0;         // index 153
    virtual void v270() = 0;         // index 154
    virtual void v274() = 0;         // index 155
    virtual void v278() = 0;         // index 156
    virtual CfObjectImplPc27C* vf27C() = 0;  // index 157 -> vtable offset 0x27C
    virtual void v280() = 0;         // index 158
    virtual void v284() = 0;         // index 159
    virtual void v288() = 0;         // index 160
    virtual void v28C() = 0;         // index 161
    virtual void* vf290() = 0;       // index 162 -> 0x290
    virtual void v294() = 0;         // index 163
    virtual CfObjectImplPc298Res* v298() = 0;  // index 164 -> 0x298
    virtual CfObjectImplPc2A4Res* v29C(u32 a) = 0;  // 165 -> 0x29C
    virtual void v2A0() = 0;         // index 166
    virtual void v2A4() = 0;         // index 167 -> 0x2A4
    virtual void* v2A8() = 0;        // index 168 -> 0x2A8
    virtual void v2AC() = 0;         // index 169
    virtual void v2B0() = 0;         // index 170
    virtual void v2B4() = 0;         // index 171
    virtual void v2B8() = 0;         // index 172
    virtual s32 v2BC() = 0;          // index 173 -> 0x2BC
    virtual void v2C0() = 0;         // index 174
    virtual void vf2C4(u8* obj, f32 a, f32 b, f32 c) = 0;  // index 175 -> 0x2C4
    virtual void w2C8() = 0;         // 176 -> 0x2C8
    virtual void w2CC() = 0;         // 177 -> 0x2CC
    virtual void w2D0() = 0;         // 178 -> 0x2D0
    virtual void w2D4() = 0;         // 179 -> 0x2D4
    virtual void w2D8() = 0;         // 180 -> 0x2D8
    virtual void w2DC() = 0;         // 181 -> 0x2DC
    virtual void w2E0() = 0;         // 182 -> 0x2E0
    virtual void w2E4() = 0;         // 183 -> 0x2E4
    virtual void w2E8() = 0;         // 184 -> 0x2E8
    virtual void w2EC() = 0;         // 185 -> 0x2EC
    virtual void w2F0() = 0;         // 186 -> 0x2F0
    virtual CfObjectImplPc2F4* vf2F4() = 0;  // 187 -> 0x2F4
    virtual void w2F8(u32 a) = 0;    // 188 -> 0x2F8
    virtual void vf2FC(s32 a) = 0;   // 189 -> 0x2FC
    virtual void w300() = 0;         // 190 -> 0x300
    virtual void vf304(u32 a) = 0;   // 191 -> 0x304
    virtual s32 vf308() = 0;         // 192 -> 0x308 (compared signed)
    virtual void v310() = 0;         // 193
    virtual void v314() = 0;         // 194 -> 0x314
    virtual void v318() = 0;         // 195
    virtual void v31C() = 0;         // 196
    virtual void v320() = 0;         // 197
    virtual void v324() = 0;         // 198
    virtual void v328() = 0;         // 199
    virtual void v32C() = 0;         // 200
    virtual void v330() = 0;         // 201
    virtual void v334() = 0;         // 202
    virtual void v338() = 0;         // 203
    virtual void v33C() = 0;         // 204
    virtual void v340() = 0;         // 205
    virtual void v344() = 0;         // 206
    virtual void v348() = 0;         // 207
    virtual void v34C() = 0;         // 208
    virtual void v350() = 0;         // 209
    virtual void v354() = 0;         // 210
    virtual void v358() = 0;         // 211
    virtual void v35C() = 0;         // 212
    virtual void v360() = 0;         // 213
    virtual void v364() = 0;         // 214
    virtual s32 v368(void* obj) = 0; // 215 -> 0x5C0 (result checked)
    virtual void v36C() = 0;         // 216
    virtual void v370() = 0;         // 217
    virtual void v374() = 0;         // 218
    virtual void v378() = 0;         // 219
    virtual void v37C() = 0;         // 220
    virtual void v380() = 0;         // 221
    virtual void v5A8() = 0;         // 222 -> 0x5A8
    virtual void v5AC() = 0;         // 223 -> 0x5AC

    CfObjectImplPcBattle4* field_04; // 0x04
    CfObjectImplPc8 mArts;           // 0x08 arts container (func_80148778 operates on &+0x8)
    u8 _pad0C[0x3374 - 0x0C];        // 0x0C-0x3373
    u32 field_3374;                  // 0x3374 flags
    u8 _pad3378[0x3380 - 0x3378];    // 0x3378-0x337F
    CfObjectImplPc3380 field_3380;   // 0x3380 block managed by func_8014B2DC
    u8 _pad3E80[0x3E98 - 0x3E80];
    s32 field_3E98;                  // 0x3E98
    CfObjectImplPcSub3E9C mSub;      // 0x3E9C embedded sub-object
    u8 _pad3EA0[0x3ED4 - 0x3EA0];    // 0x3EA0-0x3ED3
    CfObjectImplPcED4* field_3ED4;   // 0x3ED4 (__dynamic_cast source)
    u8 _pad3ED8[0x3F00 - 0x3ED8];    // 0x3ED8-0x3EFF
    u32 field_3F00;                  // 0x3F00 (bit 30 gates target sync)
    u8 _pad3F04[0x3F10 - 0x3F04];    // 0x3F04-0x3F0F
    u32 field_3F10;                  // 0x3F10
    u8 _pad3F14[0x3F28 - 0x3F14];    // 0x3F14-0x3F27
    u16 field_3F28;                  // 0x3F28
    u8 _pad3F2A[0x3F2C - 0x3F2A];    // 0x3F2A-0x3F2B
    u32 field_3F2C;                  // 0x3F2C
    u8 _pad3F30[0x3F34 - 0x3F30];    // 0x3F30-0x3F33
    u8* field_3F34;                  // 0x3F34 data block (func_800ACEF8 src)
    u8 _pad3F38[0x3F60 - 0x3F38];    // 0x3F38-0x3F5F
    CfObjectImplPc3F60* field_3F60;  // 0x3F60
    u32 field_3F64;                  // 0x3F64 own battle id
    u32 field_3F68;                  // 0x3F68 secondary battle id
    u8 _pad3F6C[0x4594 - 0x3F6C];    // 0x3F6C-0x4593
    CfObjectImplPcTgt* field_4594;   // 0x4594
    CfObjectImplPcTgt* field_4598;   // 0x4598
    u8 _pad459C[0x45B0 - 0x459C];    // 0x459C-0x45AF
    u8 field_45B0;                   // 0x45B0
};

// Enum list returned by func_80043F18: element count at +0x620.
struct CfEnumList {
    u8 _pad00[0x620];                // 0x00-0x61F
    u32 field_620;                   // 0x620
};

// Object at CfObjectImplPc98: vtable slot 0x28 called with (const char*, int).
class CfObjectImplPc98 {
public:
    virtual void vf00() = 0;                           // index 0
    virtual void vf04() = 0;                           // index 1
    virtual void vf08() = 0;                           // index 2
    virtual void vf0C() = 0;                           // index 3
    virtual void vf10() = 0;                           // index 4
    virtual void vf14() = 0;                           // index 5
    virtual void vf18() = 0;                           // index 6
    virtual void vf1C() = 0;                           // index 7
    virtual void vf28(const char* str, int flag) = 0;  // index 8 -> vtable offset 0x28
};

// Object at CfObjectImplPc::field_14: token word at +0x70, u16 state at
// +0x8C, sub-object at +0x98.
struct CfObjectImplPc14 {
    u8 _pad00[0x70];                 // 0x00-0x6F
    u32 field_70;                    // 0x70
    u8 _pad74[0x8C - 0x74];          // 0x74-0x8B
    u16 field_8C;                    // 0x8C
    u8 _pad8E[0x98 - 0x8E];          // 0x8E-0x97
    CfObjectImplPc98* field_98;      // 0x98
    u8 _pad9C[0xC4 - 0x9C];          // 0x9C-0xC3
    u32 field_C4;                    // 0xC4 token id (func_800C75D4 gate)
};

// Player-character implementation object. Fields at +0x14 / +0x18 / +0x28 /
// +0x378 / +0x37C; vtable slots 0x48 / 0x80 / 0x94 / 0xE4 invoked by this TU.
class CfObjectImplPc {
public:
    virtual ~CfObjectImplPc();                       // index 0 -> vtable offset 0x08
    virtual void vf04() = 0;                         // index 1
    virtual void vf08() = 0;                         // index 2
    virtual void vf0C() = 0;                         // index 3
    virtual void vf10() = 0;                         // index 4
    virtual void vf14() = 0;                         // index 5
    virtual void vf18() = 0;                         // index 6
    virtual void vf1C() = 0;                         // index 7
    virtual void vf20() = 0;                         // index 8
    virtual void vf24() = 0;                         // index 9
    virtual void vf28() = 0;                         // index 10
    virtual void vf2C() = 0;                         // index 11
    virtual void vf30() = 0;                         // index 12
    virtual void vf34() = 0;                         // index 13
    virtual void vf38() = 0;                         // index 14
    virtual void vf3C() = 0;                         // index 15
    virtual u32 vf40() = 0;                          // index 16 -> vtable offset 0x48
    virtual void vf44() = 0;                         // index 17
    virtual void vf48() = 0;                         // index 18
    virtual void vf4C() = 0;                         // index 19
    virtual void vf50() = 0;                         // index 20
    virtual void vf54() = 0;                         // index 21
    virtual void vf58() = 0;                         // index 22
    virtual void vf5C() = 0;                         // index 23
    virtual void vf60() = 0;                         // index 24
    virtual void vf64() = 0;                         // index 25
    virtual void vf68(u32 a) = 0;                    // index 26 -> vtable offset 0x70
    virtual void vf6C() = 0;                         // index 27
    virtual void vf70(u32 a) = 0;                    // index 28
    virtual void vf74() = 0;                         // index 29
    virtual void vf78() = 0;                         // index 30
    virtual void vf7C() = 0;                         // index 31
    virtual void vf80() = 0;                         // index 32
    virtual void vf84() = 0;                         // index 33
    virtual void vf88() = 0;                         // index 34
    virtual void vf94(u32 a) = 0;                    // index 35 -> vtable offset 0x94
    virtual void vf98(u16 a) = 0;                    // index 36 -> 0x98
    virtual void vf9C() = 0;                         // index 37
    virtual void vfA0() = 0;                         // index 38
    virtual void vfA4() = 0;                         // index 39
    virtual void vfA8() = 0;                         // index 40
    virtual void vfAC() = 0;                         // index 41
    virtual void vfB0() = 0;                         // index 42
    virtual void vfB4() = 0;                         // index 43
    virtual void vfB8() = 0;                         // index 44
    virtual void vfBC() = 0;                         // index 45
    virtual void vfC0() = 0;                         // index 46
    virtual void vfC4() = 0;                         // index 47
    virtual void vfC8() = 0;                         // index 48
    virtual void vfCC() = 0;                         // index 49
    virtual void vfD0() = 0;                         // index 50
    virtual void vfD4() = 0;                         // index 51
    virtual void vfD8() = 0;                         // index 52
    virtual void vfDC() = 0;                         // index 53
    virtual void vfE0() = 0;                         // index 54
    virtual u8* vfE4() = 0;                          // index 55 -> vtable offset 0xE4

    u8 _pad04[0x10];                                 // 0x04-0x13
    CfObjectImplPc14* field_14;                      // 0x14
    CfObjectImplPc18* field_18;                      // 0x18
    u8 _pad1C[0x0C];                                 // 0x1C-0x27
    u8 field_28[0x340];                              // 0x28-0x367
    u32 field_368;                                   // 0x368
    u32 field_36C;                                   // 0x36C
    u32 field_370;                                   // 0x370
    u32 field_374;                                   // 0x374
    CfObjectImplPcSub* field_378;                    // 0x378
    u32 field_37C;                                   // 0x37C
};

} // namespace cf

// Event record passed as arg2 to func_800C9A20 / func_800CAB30:
// u16 event id at +0x0C, u16 gate at +0x2E, flag word at +0x30 (bit 1).
struct CfObjectImplPcEvt {
    u8 _pad00[0x0A];
    u8 field_0A;                     // 0x0A request mode (0 = gm effect, 1 =
                                     // slot 0x220, 2 = area manager)
    u8 _pad0B;
    u16 field_0C;
    u8 field_0E;                     // effect type (low 7 bits) + flag bit 7
    u8 _pad0F[0x20 - 0x0F];
    f32 field_20;                    // 0x20 float payload (word-copied by
                                     // the struct copy in func_800C9CEC)
    u8 _pad24[0x2E - 0x24];
    u16 field_2E;
    u32 field_30;

    // Inline accessor: retail compares run signed (cmpi) on the widened id.
    int id() const { return field_0C; }
};

// ---------------------------------------------------------------------------
// C-ABI imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" {
void func_800CAA44(void* self);
void func_800CEE28(void* self, u32 param);
void func_800CEBE0(void* self);
int func_80148778(void* obj, int id);
unsigned int func_800822F4__Q22cf13CfGameManagerFv(void);
void func_800CA948(u8* self);
void func_8015BB3C(u8* a, u8* b, u8* c);
void func_802A0B8C(u8* self, u8* owner);
u32 func_80061FE8();
char* func_800BEDC4(u8* move, int index);
char* func_800BED80(u8* move, int index);
u8* func_8016FE34(u8* src);
u8* func_800F6EAC(u8* list, u32 idx);
u8* func_800F6ED0(u8* list, u32 value);
u8* func_800F6E08(u8* list);
void func_80043D90(u8* holder);
u8* func_80043F18(u8* holder);
void func_800F4A98(u8* list, u32 type, u32 filter);
void __dt__80043E88(u8* holder, int flag);
void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
void func_800CB9AC(u8* self, u32 param);
u8* __ct__cf_CPcEffect07(u8* obj, u8* actor);
int func_80145F78(int id);
int func_80145C00(int val);
bool func_802799F0(void* chain, void* obj);
bool func_80260264(void* obj, s32 idx, s32* out);
void func_800BF29C(void* sub, u32 a, f32 b, u32 c, f32 d, u32 e);
void func_800BF2B0(void* sub, u32 a, u32 b);
void func_800BE12C(void* sub, u32 a, u32 b, s32 c, u32 d);
void func_8004CEF8(void* obj, s32 val);
void func_802A300C(void* obj);
void func_800EC8FC(void* mgr, void* obj, void* evtCopy, s32 flag);
void func_800CB454(cf::CfObjectImplPc* self, CfObjectImplPcEvt* evt);
void func_8018C820(void* obj, int value);
void func_8014B2DC(void* blk);
void func_801B248C(int value);
void func_800CAB30(cf::CfObjectImplPc* self, CfObjectImplPcEvt* evt);
void func_800F3970(void* mgr, void* obj, void* obj2, s32 a, s32 b);
void func_800CD5DC(void* self, u32 a, u32 b, u32 c, u32 d, u32 e);
int func_8004B9B8(void* obj);
void func_8004B9D4(void* obj, u32 a, u32 b, void* c, u32 d);
void func_801BFE8C(u32 a, u32 b, u32 c);
void func_802A3074(void* obj);
void func_800CC964(void);
void func_800983B8(void* a, int b);
unsigned int func_80082568__Q22cf13CfGameManagerFv(unsigned int a,
                                                    unsigned int b,
                                                    unsigned int c);
int func_80260518(void* obj, int id, u32* outW, f32* outF);
void func_800EA9A8(void* mgr, void* obj, void* arg, int a, int b);
void func_800D81A8(int a, void* obj, int c);
void func_802A2A74(void* a, void* b);
void func_802809C8(void);
void* func_80149154(void* obj, int id);
void func_80133F48(int a, float b);
int func_800DA06C(void* mgr, void* obj);
void func_800451D8(u32 entry, void* sub);
u8* func_8009EC9C(int id);
u8* func_8009D7E4(u8* info, int a);
}

// C++-mangled imports (plain declarations let MWCC append the retail
// mangling from the parameter types).
bool func_8006EF04(int mask);                      // func_8006EF04__Fi
cf::CfObjectImplPc18* func_800BFC68(cf::CfObjectMove* objMove);  // func_800BFC68__FPQ22cf12CfObjectMove
// (func_800B708C__Fi is declared by kyoshin/cf/CfSoundMan.hpp)

// sdata2 constants used by func_800CA294 / func_800C86E8 / func_800C9A20 /
// func_800C75D4.
extern f32 lbl_eu_80666BC8;
extern f32 lbl_eu_80666BCC;
extern f32 lbl_eu_80666BD0;
extern f32 lbl_eu_80666BE8;
extern f32 lbl_eu_80666BEC;
extern f32 lbl_eu_80666BF0;
extern f32 lbl_eu_80666BF4;
extern f32 lbl_eu_80666BF8;
extern f32 lbl_eu_80666BFC;
extern f32 lbl_eu_80666C00;
extern f64 lbl_eu_80666BD8;
extern f64 lbl_eu_80666BE0;

// Handler-entry table copied to the stack by func_800C75D4 (indexed by the
// battle id at +0x3F28) and the two typeinfo operands of its __dynamic_cast.
extern u32 lbl_eu_804FC730[9];
extern u32 lbl_eu_804FC718[6];
extern void* lbl_eu_80661C60;
extern void* lbl_eu_80661BE8;

// String table (rodata) referenced by func_800CA458 (substring at +0x3B).
extern char lbl_eu_804FC758[];

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__Q22cf14CfObjectImplPcFv(void* self);
extern "C" void* func_800FE68C(void);
extern "C" void func_800EC918(void* mgr, void* obj, void* obj2, void* arg,
                               u32 flag);
extern "C" int func_8026178C(void* obj, u32 idx);
extern "C" int func_8025FB10(void* obj, u32 idx);
extern "C" void func_800CAB2C(void* self);
extern "C" void func_8014AC38(void* blk, void* rec);
extern "C" void func_800F38E0(void* mgr, void* obj, s32 id);
extern "C" void func_800F3958(void* mgr, void* obj, s32 id);
extern "C" void func_800E9B54(void* mgr, void* obj, u32 a, u32 b);
extern "C" void func_800D9CA0(void* mgr, void* obj);
extern "C" void* func_800EA444(void* mgr);
extern "C" int func_801B1CCC(int idx);
extern "C" int func_800B8C78(int id);
extern "C" void func_80084654__Q22cf13CfGameManagerFv(int flag);
extern "C" int func_8025FDB8(void* obj, u32 idx);
extern "C" f32 func_80260010(void* obj, u32 idx);

namespace ml {
namespace math {
int mtRand(int n);                   // mtRand__Q22ml4mathFi
}
}
extern "C" void func_800ACF78(void* obj, void* target, u32 child);
extern "C" void func_800ACEF8(void* obj, const void* src);
extern "C" void func_800ACFD8(void* obj, void* target);
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 index);
extern "C" void* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);

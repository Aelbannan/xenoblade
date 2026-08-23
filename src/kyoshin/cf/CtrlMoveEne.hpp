#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>

namespace cf {

// Target-state object behind CfObjectMove::mTargetC4: CfObject_UnkVirtualFunc18
// reads bit 23 of the +0x270 state word.
struct CfObjectMoveC4Target {
    u8 _pad[0x270];         // 0x00-0x26F
    u32 field_0x270;        // 0x270
};

struct CNpcBaseDataView;  // forward decl (movement data block, defined below)

class CfObjectMove {
public:
    int CfObject_UnkVirtualFunc18();

    // TODO: add fields
    void CfObject_UnkVirtualFunc58();
    void CfObjectMove_UnkVirtualFunc5();

    // 0x00: vtable
    u8 _pad[0x34];                    // 0x00-0x33
    CNpcBaseDataView* field_0x34;     // 0x34 movement data block
    f32 field_0x38;                   // 0x38 (ctor zeroes; base-controller float)
    f32 field_0x3C;                   // 0x3C (func_8008C4F0 stores lbl_eu_806665F0)
    u8 _pad40[0x48 - 0x40];           // 0x40-0x47
    void* field_0x48;                 // 0x48 secondary vtable (ctor stores lbl_eu_805279FC)
    f32 field_0x4C;                   // 0x4C (ctor / func_8008E06C store the move-sub +0xCC result)
    u32 field_0x50;                   // 0x50 (func_8008E06C stores the bdat-derived state)
    u32 field_0x54;                   // 0x54 (func_8008E06C zeroes)
    f32 field_0x58;                   // 0x58 (func_8008E06C stores the movement-rate divisor)
    f32 field_0x5C;                   // 0x5C (func_8008E06C stores the rand/level-derived value)
    u8 _pad60[0xC4 - 0x60];           // 0x60-0xC3
    CfObjectMoveC4Target* mTargetC4;  // 0xC4
    u8 _padC8[0x12C - 0xC8];          // 0xC8-0x12B
    void (CfObjectMove::*mMoveHook)();  // 0x12C-0x137 ptmf hook (func_8008A23C / ctor)
    ml::CVec3 mVec138;                  // 0x138 (ctor copies ml::CVec3::zero)
    ml::CVec3 mVec144;                  // 0x144 (ctor copies ml::CVec3::zero)
    u8 _pad150[0x160 - 0x150];          // 0x150-0x15F
    f32 field_0x160;                    // 0x160 (ctor stores lbl_eu_806665C4; func_8008C4F0 reads)
    f32 field_0x164;                    // 0x164 (ctor zeroes)
    f32 field_0x168;                    // 0x168 (zeroed by func_8008D444)
    u8 _pad16C[0x170 - 0x16C];          // 0x16C-0x16F
    f32 field_0x170;                    // 0x170 (ctor stores lbl_eu_806665C8)
    f32 field_0x174;                    // 0x174 (ctor stores lbl_eu_806665C8)
    f32 field_0x178;                    // 0x178 (zeroed by func_8008D444)
    u32 field_0x17C;                    // 0x17C flags word
    u32 field_0x180;                    // 0x180 flags word (ctor / func_8008E06C raise bit 0x800)
    u16 field_0x184;                    // 0x184 (zeroed by func_8008D444)
    u16 field_0x186;                    // 0x186 (ctor zeroes)
    u8 _pad188[0x18A - 0x188];          // 0x188-0x189
    u16 field_0x18A;                    // 0x18A (ctor stores mtRand(0x3C))
    u16 field_0x18C;                    // 0x18C (func_8008E06C stores 2)
    u8 _pad18E[0x190 - 0x18E];          // 0x18E-0x18F
    u16 field_0x190;                    // 0x190 (zeroed by func_8008D444)
    u16 field_0x192;                    // 0x192 (zeroed by func_8008D444)
    u8 _pad194[0x196 - 0x194];          // 0x194-0x195
    u16 field_0x196;                    // 0x196 (zeroed by func_8008D444)
    s16 field_0x198;                    // 0x198 (func_8008C4F0 lha's and compares to 90)
    u8 _pad19A[0x19C - 0x19A];          // 0x19A-0x19B
    f32 field_0x19C;                    // 0x19C (zeroed by func_8008D444)
};

// View of the CfObjectMove layout used by the func_8009156C / func_8008EF04 /
// func_8008F9EC family: the base CCtrlMoveBase position/velocity words and
// the +0x60..+0x72 / +0x15C / +0x18E fields those functions touch. Kept as a
// separate view so the matched CfObjectMove stays byte-for-byte the same.
struct CfObjectMoveView9 {
    u8 _pad[0x0C];                    // 0x00-0x0B
    ml::CVec3 mPosition;              // 0x0C (base CCtrlMoveBase)
    ml::CVec3 mVelocity;              // 0x18 (base CCtrlMoveBase)
    u8 _pad24[0x30 - 0x24];           // 0x24-0x2F
    void* field_0x30;                 // 0x30 (base mpSomePtr)
    CNpcBaseDataView* field_0x34;     // 0x34 movement data block
    f32 field_0x38;                   // 0x38
    f32 field_0x3C;                   // 0x3C
    u8 _pad40[0x4C - 0x40];           // 0x40-0x4B
    f32 field_0x4C;                   // 0x4C heading word
    f32 field_0x50;                   // 0x50
    u8 _pad54[0x58 - 0x54];           // 0x54-0x57
    f32 field_0x58;                   // 0x58 movement-rate divisor
    f32 field_0x5C;                   // 0x5C
    f32 field_0x60;                   // 0x60
    f32 field_0x64;                   // 0x64
    f32 field_0x68;                   // 0x68
    f32 field_0x6C;                   // 0x6C
    u16 field_0x70;                   // 0x70
    u16 field_0x72;                   // 0x72
    u8 _pad74[0x15C - 0x74];          // 0x74-0x15B
    int field_0x15C;                  // 0x15C actor id
    u8 _pad160[0x17C - 0x160];        // 0x160-0x17B
    u32 field_0x17C;                  // 0x17C flags
    u32 field_0x180;                  // 0x180 flags
    u16 field_0x184;                  // 0x184
    u16 field_0x186;                  // 0x186
    u8 _pad188[0x18C - 0x188];        // 0x188-0x18B
    u16 field_0x18C;                  // 0x18C
    s16 field_0x18E;                  // 0x18E
    u8 _pad190[0x198 - 0x190];        // 0x190-0x197
    s16 field_0x198;                  // 0x198
};

// Word view of CfObjectMove::field_0x4C: func_8008E06C clears it with an
// integer store and ORs bit 2 into it, while the ctor stores a float there.
struct CfObjectMove4CView {
    u8 _pad[0x4C];                  // 0x00-0x4B
    u32 field_4C;                   // 0x4C
};

// View of the battle-object sub at +0x3F60 (func_8008C4F0 toggles bit flags
// in its +0xC word and +0x4EC word).
struct CfObj3F60View {
    u8 _pad[0xC];                   // 0x00-0x0B
    u32 field_C;                    // 0x0C
    u8 _pad10[0x4EC - 0x10];        // 0x10-0x4EB
    u32 field_4EC;                  // 0x4EC
};

// func_8008B580's extension of the +0x3F60 flag object: the +0x532 halfword.
struct CFunc8008B580F60 {
    u8 _pad[0x532];                 // 0x00-0x531
    s16 field_532;                  // 0x532
};

// Sub-object embedded at +0x3E9C of the enemy battle object passed to
// func_8008D444 / func_8008E06C: vtable slot +0x8 (declared index 0 with
// -RTTI) is called with a u32 by func_8008D444, slot +0xC4 with a float
// angle by func_8008E06C, and slot +0x150 (no args) by func_8008B580.
// Declared virtuals only; never instantiated, so no vtable emitted.
class CFunc8008D444Sub {
public:
    virtual void sv08(u32 a);           // index 0 -> vtable +0x8
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual void _v8C();
    virtual void _v90();
    virtual void _v94();
    virtual void _v98();
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual void _vAC();
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4(float angle);     // index 0x30 -> vtable +0xC4 (func_8008E06C)
};

// func_8008B580's view of the +0x3E9C embedded sub-object: same base virtuals
// plus the +0x150 slot (index 0x52). Separate class so the embedded
// CFunc8008D444Sub used by the matched functions stays at its original size.
class CFunc8008B580Sub150 {
public:
    virtual void sv08(u32 a);
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual void _v8C();
    virtual void _v90();
    virtual void _v94();
    virtual void _v98();
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual void _vAC();
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4(float angle);
    virtual void _vC8();
    virtual void _vCC();
    virtual void _vD0();
    virtual void _vD4();
    virtual void _vD8();
    virtual void _vDC();
    virtual void _vE0();
    virtual void _vE4();
    virtual void _vE8();
    virtual void _vEC();
    virtual void _vF0();
    virtual void _vF4();
    virtual void _vF8();
    virtual void _vFC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150(u32 arg);        // index 0x52 -> vtable +0x150 (func_8008B580 passes 0)
};

// The +0x04 object of the enemy battle object (func_8008B580 reads the first
// word of its +0x30 vtable-slot result). Declared virtuals only.
struct CFunc8008B580Word {
    u32 field_0;                        // 0x00
};
class CFunc8008B580Sub4 {
public:
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual CFunc8008B580Word* _v30();  // vtable +0x30
};

// func_8008A2C8's sub-object vtable: slots +0x0C (int query), +0x4C (int
// query), +0x50 (int arg), +0x8C (float), +0xAC (position), +0x110 (int),
// +0x150 (int arg). Declared before CFunc8008B580Obj, which embeds the same
// sub-object as a raw vtable view.
struct CFunc8008A2C8SubVtbl {
    void* r00[0x0C / 4];                                // 0x00-0x0B
    int (*fn_0x0C)(void* self, int arg);                // 0x0C
    void* r10[(0x4C - 0x10) / 4];                       // 0x10-0x4B
    int (*fn_0x4C)(void* self, int arg);                // 0x4C
    void (*fn_0x50)(void* self, int arg);               // 0x50
    void* r54[(0x8C - 0x54) / 4];                       // 0x54-0x8B
    f32 (*fn_0x8C)(void* self);                         // 0x8C
    void* r90[(0xAC - 0x90) / 4];                       // 0x90-0xAB
    ml::CVec3* (*fn_0xAC)(void* self);                  // 0xAC
    void* rB0[(0x110 - 0xB0) / 4];                      // 0xB0-0x10F
    int (*fn_0x110)(void* self, int arg);               // 0x110
    void* r114[(0x150 - 0x114) / 4];                    // 0x114-0x14F
    void (*fn_0x150)(void* self, int arg);              // 0x150
};
struct CFunc8008A2C8Sub {
    CFunc8008A2C8SubVtbl* vtable;                       // 0x00
};

// func_8008A2C8's extension of the +0x3F60 flag object: +0x4F8 float and the
// +0x532 halfword.
struct CFunc8008A2C8F60 {
    u8 _pad[0xC];                                       // 0x00-0x0B
    u32 field_C;                                        // 0x0C
    u8 _pad10[0x4EC - 0x10];                            // 0x10-0x4EB
    u32 field_4EC;                                      // 0x4EC
    u8 _pad4F0[0x4F8 - 0x4F0];                          // 0x4F0-0x4F7
    f32 field_4F8;                                      // 0x4F8
    u8 _pad4FC[0x532 - 0x4FC];                          // 0x4FC-0x531
    s16 field_532;                                      // 0x532
};

// View of the enemy battle object passed to func_8008D444 (CfImplEneBattleObj
// layout): +0x3374 flags, +0x3E9C embedded sub-object, +0x3F60 the sub
// func_8008C4F0 toggles, +0x4550 flags. func_8008B580's separate polymorphic
// view with the vtable slots +0x11C / +0x128 / +0x2BC and the +0x04 word.
struct CFunc8008B580Obj {
    u8 _pad[0x4];                       // 0x00-0x03
    CFunc8008B580Sub4* field_04;        // 0x04 (func_8008B580 reads a word via its +0x30 vtable slot)
    u8 _pad08[0x3374 - 0x08];           // 0x08-0x3373
    u32 field_3374;                     // 0x3374 flags
    u8 _pad3378[0x3E9C - 0x3378];       // 0x3378-0x3E9B
    // 0x3E9C embedded sub-object, raw vtable-slot view (func_8008A2C8 calls
    // slots +0x0C/+0x8C/+0xAC/+0x110; func_8008B580 casts it to the +0x150
    // virtual-slot view).
    CFunc8008A2C8Sub mSub2;             // 0x3E9C embedded sub-object
    u8 _pad3EA0[0x3F60 - 0x3EA0];       // 0x3EA0-0x3F5F
    CfObj3F60View* field_3F60;          // 0x3F60 (func_8008C4F0 flag object)
    u8 _pad3F64[0x4550 - 0x3F64];       // 0x3F64-0x454F
    u32 field_4550;                     // 0x4550 flags
    u8 _pad4554[0x45C0 - 0x4554];       // 0x4554-0x45BF
    u16 field_45C0;                     // 0x45C0 (func_800B89CC id)
    u16 field_45C4;                     // 0x45C4
    u16 field_45C6;                     // 0x45C6

    // Declared virtuals only (never instantiated here -> no vtable emitted):
    // -RTTI places the Nth declared virtual at vtable offset (N+2)*4.
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual void _v8C();
    virtual void _v90();
    virtual void _v94();
    virtual void _v98();
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual void _vAC();
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4();
    virtual void _vC8();
    virtual void _vCC();
    virtual void _vD0();
    virtual void _vD4();
    virtual void _vD8();
    virtual void _vDC();
    virtual void _vE0();
    virtual void _vE4();
    virtual void _vE8();
    virtual void _vEC();
    virtual void _vF0();
    virtual void _vF4();
    virtual void _vF8();
    virtual void _vFC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual float _v11C(float f);       // vtable +0x11C (func_8008B580: negated HP)
    virtual void _v120();
    virtual void _v124();
    virtual float _v128();              // vtable +0x128 (func_8008B580: HP query)
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual void _v158();
    virtual void _v15C();
    virtual void _v160();
    virtual void _v164();
    virtual void _v168();
    virtual void _v16C();
    virtual void _v170();
    virtual void _v174();
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual void _v188();
    virtual void _v18C();
    virtual void _v190();
    virtual void _v194();
    virtual void _v198();
    virtual void _v19C();
    virtual void _v1A0();
    virtual void _v1A4();
    virtual void _v1A8();
    virtual void _v1AC();
    virtual void _v1B0();
    virtual void _v1B4();
    virtual void _v1B8();
    virtual void _v1BC();
    virtual void _v1C0();
    virtual void _v1C4();
    virtual void _v1C8();
    virtual void _v1CC();
    virtual void _v1D0();
    virtual void _v1D4();
    virtual void _v1D8();
    virtual void _v1DC();
    virtual void _v1E0();
    virtual void _v1E4();
    virtual void _v1E8();
    virtual void _v1EC();
    virtual void _v1F0();
    virtual void _v1F4();
    virtual void _v1F8();
    virtual void _v1FC();
    virtual void _v200();
    virtual void _v204();
    virtual void _v208();
    virtual void _v20C();
    virtual void _v210();
    virtual void _v214();
    virtual void _v218();
    virtual void _v21C();
    virtual void _v220();
    virtual void _v224();
    virtual void _v228();
    virtual void _v22C();
    virtual void _v230();
    virtual void _v234();
    virtual void _v238();
    virtual void _v23C();
    virtual void _v240();
    virtual void _v244();
    virtual void _v248();
    virtual void _v24C();
    virtual void _v250();
    virtual void _v254();
    virtual void _v258();
    virtual void _v25C();
    virtual void _v260();
    virtual void _v264();
    virtual void _v268();
    virtual void _v26C();
    virtual void _v270();
    virtual void _v274();
    virtual void _v278();
    virtual void _v27C();
    virtual void _v280();
    virtual void _v284();
    virtual void _v288();
    virtual void _v28C();
    virtual void _v290();
    virtual void _v294();
    virtual void _v298();
    virtual void _v29C();
    virtual void _v2A0();
    virtual void* _v2A4();              // vtable +0x2A4 (func_8008A2C8: sub-object ptr)
    virtual void _v2A8();
    virtual void _v2AC();
    virtual void _v2B0();
    virtual void _v2B4();
    virtual void _v2B8();
    virtual int _v2BC();                 // vtable +0x2BC (func_8008B580 busy/active query)
};

// Plain (non-polymorphic) view of the enemy battle object used by the
// matched func_8008C4F0 / func_8008D444 / func_8008E06C and func_8008E2D4:
// keeping this struct free of virtuals preserves their compiled bytes.
struct CFunc8008D444Obj {
    u8 _pad[0x3374];                    // 0x00-0x3373
    u32 field_3374;                     // 0x3374 flags
    u8 _pad3378[0x3E9C - 0x3378];       // 0x3378-0x3E9B
    CFunc8008D444Sub mSub;              // 0x3E9C embedded sub-object
    u8 _pad3EA0[0x3F60 - 0x3EA0];       // 0x3EA0-0x3F5F
    CfObj3F60View* field_3F60;          // 0x3F60 (func_8008C4F0 flag object)
    u8 _pad3F64[0x4550 - 0x3F64];       // 0x3F64-0x454F
    u32 field_4550;                     // 0x4550 flags
};

// Movement sub-object behind CNpcBaseDataView::field_0x28 (the same object
// CtrlMoveNpc.cpp names CNpcMoveSub). func_80093618 calls vtable +0x74
// (returns a state word) and +0xC4 (no args). Declared virtuals only; never
// instantiated here, so no vtable is emitted.
class CNpcMoveSubView {
public:
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual int _v74();               // vtable +0x74
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual float _v8C();             // vtable +0x8C (func_8008A23C compares to 0.0f)
    virtual void _v90();
    virtual void _v94();
    virtual void _v98();
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual void _vAC();
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4();              // vtable +0xC4
    virtual void _vC8();
    virtual float _vCC();             // vtable +0xCC (returns a float; ctor / func_8008E06C)
    virtual void _vD0();
    virtual void _vD4();
    virtual void _vD8();
    virtual void _vDC();
    virtual void _vE0();
    virtual void _vE4();
    virtual void _vE8();
    virtual void _vEC();
    virtual void _vF0();
    virtual void _vF4();
    virtual void _vF8();
    virtual void _vFC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual float* _v138();           // vtable +0x138 (movement-rate divisor query)
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual void _v158();
    virtual void _v15C();
    virtual void _v160();
    virtual void _v164();
    virtual void _v168();
    virtual void _v16C();
    virtual void _v170();
    virtual void _v174();
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual void _v188();
    virtual void _v18C();
    virtual void _v190();
    virtual void _v194();
    virtual void _v198();
    virtual void _v19C();
    virtual void _v1A0();
    virtual void _v1A4();
    virtual void _v1A8();
    virtual void _v1AC();
    virtual void _v1B0();
    virtual void _v1B4();
    virtual void _v1B8();
    virtual void _v1BC();
    virtual void _v1C0();
    virtual void _v1C4();
    virtual void _v1C8();
    virtual void _v1CC();
    virtual void _v1D0();
    virtual void _v1D4();
    virtual float* _v1D8();           // vtable +0x1D8 (movement-rate query)
};

// func_8008F9EC / func_8009156C view of the move sub-object: same virtual
// layout as CNpcMoveSubView but with +0x98 typed int, +0xAC returning the
// position, and a +0xC4 data word. Kept as a separate view so the matched
// func_8008C4F0 / func_8008A23C keep their exact CNpcMoveSubView.
class CFunc8008F9ECSub {
public:
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual int _v74();               // vtable +0x74
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual float _v8C();             // vtable +0x8C
    virtual void _v90();
    virtual void _v94();
    virtual int _v98();               // vtable +0x98 (func_8008F9EC: nonzero = active)
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual ml::CVec3* _vAC();        // vtable +0xAC (position getter)
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4();              // vtable +0xC4
    virtual void _vC8();
    virtual float _vCC();             // vtable +0xCC (heading index)
    virtual void _vD0();
    virtual void _vD4();
    virtual void _vD8();
    virtual void _vDC();
    virtual void _vE0();
    virtual void _vE4();
    virtual void _vE8();
    virtual void _vEC();
    virtual void _vF0();
    virtual void _vF4();
    virtual void _vF8();
    virtual void _vFC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual float* _v138();           // vtable +0x138 (movement-rate divisor query)
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual void _v158();
    virtual void _v15C();
    virtual void _v160();
    virtual void _v164();
    virtual void _v168();
    virtual void _v16C();
    virtual void _v170();
    virtual void _v174();
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual void _v188();
    virtual void _v18C();
    virtual void _v190();
    virtual void _v194();
    virtual void _v198();
    virtual void _v19C();
    virtual void _v1A0();
    virtual void _v1A4();
    virtual void _v1A8();
    virtual void _v1AC();
    virtual void _v1B0();
    virtual void _v1B4();
    virtual void _v1B8();
    virtual void _v1BC();
    virtual void _v1C0();
    virtual void _v1C4();
    virtual void _v1C8();
    virtual void _v1CC();
    virtual void _v1D0();
    virtual void _v1D4();
    virtual float* _v1D8();           // vtable +0x1D8 (movement-rate query)

    u8 _pad04[0xC4 - 0x04];           // 0x04-0xC3
    void* field_C4;                   // 0xC4 target word (null-checked)
};

// func_8008EF04's view of the move sub-object: same object as CNpcMoveSubView
// but the +0xC4 vtable slot is called WITH a float heading (the no-arg +0xC4
// slot on CNpcMoveSubView is pinned by the matched func_80093618), and the
// +0x98 / +0xC4 words are read as data. Declared virtuals only; never
// instantiated, so no vtable emitted.
class CFunc8008EF04Sub {
public:
    virtual void _v08();
    virtual int _v0C(int arg);        // vtable +0x0C
    virtual void _v10(int arg);       // vtable +0x10
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual void _v8C();
    virtual void _v90();
    virtual void _v94();
    virtual void _v98();
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual void _vAC();
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4(f32 heading);   // vtable +0xC4 (func_8008EF04 passes field_0x4C)
    virtual void _vC8();
    virtual float _vCC();             // vtable +0xCC (heading index)
    virtual void _vD0();
    virtual void _vD4();
    virtual void _vD8();
    virtual void _vDC();
    virtual void _vE0();
    virtual void _vE4();
    virtual void _vE8();
    virtual void _vEC();
    virtual void _vF0();
    virtual void _vF4();
    virtual void _vF8();
    virtual void _vFC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual void _v138();
    virtual void _v13C();
    virtual void _v140();
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150(int arg);      // vtable +0x150

    u8 _pad04[0x98 - 0x04];           // 0x04-0x97
    void* field_98;                   // 0x98
    u8 _pad9C[0xC4 - 0x9C];           // 0x9C-0xC3
    void* field_C4;                   // 0xC4
};

// Object behind the move sub's +0x98 word (func_8008EF04 tests +0x7A4).
struct CFunc8008EF04Sub98 {
    u8 _pad[0x7A4];                 // 0x00-0x7A3
    u32 field_7A4;                  // 0x7A4 flags
};

// View of the movement-data object func_80093618 operates on (same layout as
// CCtrlMoveNpc::mBaseData / CNpcBaseData in CtrlMoveNpc.cpp): +0xC holds a
// float, +0x28 the movement sub-object.
struct CNpcBaseDataView {
    u8 _pad[0xC];                   // 0x00-0x0B
    f32 field_0xC;                  // 0x0C
    u8 _pad10[0x28 - 0x10];         // 0x10-0x27
    CNpcMoveSubView* field_0x28;    // 0x28
};

// func_8008EF04 / func_8008F9EC / func_8009156C extension of the move-data
// block: the +0x14 float they clear when idling.
struct CFunc8009DataView {
    u8 _pad[0x14];                  // 0x00-0x13
    f32 field_0x14;                 // 0x14
};

// View of the movement-controller object func_8008B934 / func_8008B95C /
// func_8008B974 operate on (CtrlMoveEne unit): +0x15C is an actor id fed to
// func_800B708C, +0x17C a flags word.
struct CCtrlMoveEneView {
    u8 _pad[0x15C];                 // 0x00-0x15B
    int field_0x15C;                // 0x15C
    u8 _pad160[0x17C - 0x160];      // 0x160-0x17B
    u32 field_0x17C;                // 0x17C
};

// One entry of the move-list array at +0x78 that func_8008E2D4 fills (0x14
// bytes each): three floats, a flags halfword, two bytes and a scaled float.
struct CfMoveSubEntry {
    f32 field_0x0;                  // 0x00
    f32 field_0x4;                  // 0x04
    f32 field_0x8;                  // 0x08
    u16 field_0xC;                  // 0x0C flags
    u8 field_0xE;                   // 0x0E
    u8 field_0xF;                   // 0x0F
    f32 field_0x10;                 // 0x10
};

// func_8008E2D4's word/halfword view of CfObjectMove fields 0x58-0x72: the
// init path stores u16s at 0x58/0x5A and words at 0x5C/0x60, while the
// matched func_8008E06C / func_8008F9EC read the same storage as f32s.
struct CfObjectMoveInitView {
    u8 _pad[0x34];                  // 0x00-0x33
    CNpcBaseDataView* field_0x34;   // 0x34
    u8 _pad38[0x4C - 0x38];         // 0x38-0x4B
    f32 field_0x4C;                 // 0x4C
    f32 field_0x50;                 // 0x50
    f32 field_0x54;                 // 0x54
    u16 field_0x58;                 // 0x58 (entry count)
    u16 field_0x5A;                 // 0x5A
    u32 field_0x5C;                 // 0x5C (bdat-derived state)
    u32 field_0x60;                 // 0x60 (timer/derived word)
    f32 field_0x64;                 // 0x64
    f32 field_0x68;                 // 0x68
    f32 field_0x6C;                 // 0x6C
    u16 field_0x70;                 // 0x70 flags
    u16 field_0x72;                 // 0x72
    u8 _pad74[0x78 - 0x74];         // 0x74-0x77
    CfMoveSubEntry field_0x78[8];   // 0x78 move-list array (8 * 0x14 = 0xA0; overlaps later words)
    u8 _pad118[0x12C - 0x118];      // 0x118-0x12B
    void (CfObjectMove::*mMoveHook)();  // 0x12C ptmf hook
    u8 _pad138[0x17C - 0x138];      // 0x138-0x17B
    u32 field_0x17C;                // 0x17C flags
    u32 field_0x180;                // 0x180 flags
    u8 _pad184[0x18C - 0x184];      // 0x184-0x18B
    u16 field_0x18C;                // 0x18C
};

// Gimmick object from the func_800B6BC8 circular list scanned by
// func_8009156C: vtable +0xAC yields the position, +0x110 a target object.
// Declared virtuals only; never instantiated, so no vtable emitted.
class CFunc8009156CGimmick {
public:
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual void _v4C();
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();
    virtual void _v5C();
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual void _v74();
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual void _v84();
    virtual void _v88();
    virtual void _v8C();
    virtual void _v90();
    virtual void _v94();
    virtual void _v98();
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual ml::CVec3* _vAC();       // vtable +0xAC (position getter)
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4();
    virtual void _vC8();
    virtual void _vCC();
    virtual void _vD0();
    virtual void _vD4();
    virtual void _vD8();
    virtual void _vDC();
    virtual void _vE0();
    virtual void _vE4();
    virtual void _vE8();
    virtual void _vEC();
    virtual void _vF0();
    virtual void _vF4();
    virtual void _vF8();
    virtual void _vFC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void* _v110();           // vtable +0x110 (target object)
};

// Target object returned by the gimmick's +0x110 slot (func_8009156C marks it
// when the move data's +0x28 sub-object has a +0xC4 target and the distance is
// small): +0xB8 points at a move-data block, +0x1E0 receives the actor id and
// +0x200 is a flags word.
struct CFunc8009156CTargetB8Sub {
    u8 _pad[0xC4];                  // 0x00-0xC3
    void* field_C4;                 // 0xC4
};
struct CFunc8009156CTargetB8 {
    u8 _pad[0x28];                  // 0x00-0x27
    CFunc8009156CTargetB8Sub* field_28;  // 0x28
};
struct CFunc8009156CTarget {
    u8 _pad[0xB8];                  // 0x00-0xB7
    CFunc8009156CTargetB8* field_B8;  // 0xB8
    u8 _padBC[0x1E0 - 0xBC];        // 0xBC-0x1DF
    u32 field_1E0;                  // 0x1E0
    u8 _pad1E4[0x200 - 0x1E4];      // 0x1E4-0x1FF
    u32 field_200;                  // 0x200 flags
};

// +0x508 height word of the move sub's +0xC4 target object (func_8008B9C0
// gates the fall-gap span on it).
struct CFunc8008B9C0Target {
    u8 _pad[0x508];             // 0x00-0x507
    f32 field_0x508;            // 0x508
};

// func_8008CDE8's view of CfObjectMove: +0x150 target position, +0x17C flags
// and +0x18A timer halfword.
struct CFunc8008CDE8View {
    u8 _pad[0xC];               // 0x00-0x0B
    ml::CVec3 mPosition;        // 0x0C
    u8 _pad18[0x150 - 0x18];    // 0x18-0x14F
    ml::CVec3 mVec150;          // 0x150 target position
    u8 _pad15C[0x17C - 0x15C];  // 0x15C-0x17B
    u32 field_0x17C;            // 0x17C flags
    u8 _pad180[0x18A - 0x180];  // 0x180-0x189
    u16 field_0x18A;            // 0x18A timer
};

// Global coli-probe object behind lbl_eu_80665958 (func_8008CDE8 passes it to
// func_804B526C).
struct CFunc8008CDE8Probe {
    u8 _pad[0x60];              // 0x00-0x5F
    f32 field_0x60;             // 0x60
    f32 field_0x64;             // 0x64
    f32 field_0x68;             // 0x68
};

// Circular gimmick list returned by func_800B6BC8 (same layout as
// CfGimmickList): head node at +0x04 is the sentinel, nodes terminate when
// they wrap back to it.
struct CFunc8009156CNode {
    CFunc8009156CNode* next;        // 0x00
    u8 _pad04[0x08 - 0x04];         // 0x04-0x07
    CFunc8009156CGimmick* object;   // 0x08
};
struct CFunc8009156CList {
    u8* field_00;                   // 0x00
    CFunc8009156CNode* head;        // 0x04
};

// ---------------------------------------------------------------------------
// func_8008E760 views
// ---------------------------------------------------------------------------

// Word view of the 12-byte position words func_8008E760 copies at +0x00 and
// +0x144 (retail moves them with lwz/stw pairs, so they are raw words here).
struct CFunc8008E760Vec3W {
    u32 x;                          // 0x00
    u32 y;                          // 0x04
    u32 z;                          // 0x08
};

// Vtable of the +0x3E9C embedded sub-object as func_8008E760 calls it:
// slots +0x9C (takes a pointer), +0xAC (position getter) and +0xC4 (no args).
// Declared as a raw vtable struct so no vtable is emitted.
struct CFunc8008E760SubVtbl {
    void* r00[0x9C / 4];                                // 0x00-0x9B
    void (*fn_0x9C)(void* self, void* a);               // 0x9C
    void* rA0[(0xAC - 0xA0) / 4];                       // 0xA0-0xAB
    ml::CVec3* (*fn_0xAC)(void* self);                  // 0xAC
    void* rB0[(0xC4 - 0xB0) / 4];                       // 0xB0-0xC3
    void (*fn_0xC4)(void* self);                        // 0xC4 (no args)
};
struct CFunc8008E760Sub {
    CFunc8008E760SubVtbl* vtable;                       // 0x00
};

// Battle object r31 (func_80198310 result): vtable slot +0x5B4 returns a
// float, +0x3E9C the embedded sub-object, +0x44D8 a float.
struct CFunc8008E760BattleVtbl {
    void* r00[0x5B4 / 4];                               // 0x00-0x5B3
    f32 (*fn_0x5B4)(void* self);                        // 0x5B4
};
struct CFunc8008E760BattleObj {
    CFunc8008E760BattleVtbl* vtable;                    // 0x00
    u8 _pad04[0x3E9C - 0x04];                           // 0x04-0x3E9B
    CFunc8008E760Sub mSub;                              // 0x3E9C
    u8 _pad3EA0[0x44D8 - 0x3EA0];                       // 0x3EA0-0x44D7
    f32 field_44D8;                                     // 0x44D8
};

// Battle object r30 (derived from the move-data +0x28 sub-object): the bdat
// columns at +0x3F14 / +0x3F28, the flag words and the halfwords at +0x45C0..
// +0x45C6.
struct CFunc8008E760Obj {
    u8 _pad[0x3374];                                    // 0x00-0x3373
    u32 field_3374;                                     // 0x3374
    u8 _pad3378[0x3E9C - 0x3378];                       // 0x3378-0x3E9B
    CFunc8008E760Sub mSub;                              // 0x3E9C
    u8 _pad3EA0[0x3F00 - 0x3EA0];                       // 0x3EA0-0x3EFF
    u32 field_3F00;                                     // 0x3F00
    u8 _pad3F04[0x3F14 - 0x3F04];                       // 0x3F04-0x3F13
    char field_3F14[0x14];                              // 0x3F14 (file-path string for getFP__FPCc)
    u16 field_3F28;                                     // 0x3F28 (bdat row id)
    u8 _pad3F2A[0x44D8 - 0x3F2A];                       // 0x3F2A-0x44D7
    f32 field_44D8;                                     // 0x44D8
    u8 _pad44DC[0x45C0 - 0x44DC];                       // 0x44DC-0x45BF
    u16 field_45C0;                                     // 0x45C0
    u16 field_45C4;                                     // 0x45C4
    u16 field_45C6;                                     // 0x45C6
};

// func_800B89CC result (r28): the +0x8C float read by func_8008E760.
struct CFunc8008E760B89 {
    u8 _pad[0x8C];                                      // 0x00-0x8B
    f32 field_8C;                                       // 0x8C
};

// func_801974CC result: the +0x24 mode byte func_8008E760 tests.
struct CFunc8008E760Elem {
    u8 _pad[0x24];                                      // 0x00-0x23
    u8 field_24;                                        // 0x24
};

// Stack block passed to func_80198710 / func_8019876C (CfPartyInfo layout:
// func_8008E760 clears its +0x2D byte when the +0x180 bit-0 flag is set).
struct CFunc8008E760PartyInfo {
    u32 field_00;                                       // 0x00
    u32 field_04;                                       // 0x04
    u32 field_08;                                       // 0x08
    s32 field_0C;                                       // 0x0C
    u32 field_10;                                       // 0x10
    s32 field_14;                                       // 0x14
    f32 field_18;                                       // 0x18
    f32 field_1C;                                       // 0x1C
    f32 field_20;                                       // 0x20
    u32 field_24;                                       // 0x24
    f32 field_28;                                       // 0x28
    u8 field_2C;                                        // 0x2C
    u8 field_2D;                                        // 0x2D
    u8 field_2E;                                        // 0x2E
};

// Move-data block view for the +0x74 halfword func_8008E760 ORs 0x2000 into.
struct CFunc8008E760Data {
    u8 _pad[0x74];                                      // 0x00-0x73
    u16 field_74;                                       // 0x74
};

// func_8008E760's view of CfObjectMove: the position words at +0x00, the
// hook ptmf at +0x12C, the word-triple mVec144 and the fields the function
// initializes. field_4C/field_50/field_54 are u32 here because the pos copy
// uses lwz/stw word stores (the float-store path goes through CfObjectMove).
struct CFunc8008E760View {
    CFunc8008E760Vec3W mPos0W;          // 0x00
    u8 field_0C[4];                     // 0x0C (arg to _v9C / func_8019876C)
    f32 field_0x10;                     // 0x10
    u8 _pad14[0x34 - 0x14];             // 0x14-0x33
    CNpcBaseDataView* field_0x34;       // 0x34
    u8 _pad38[0x4C - 0x38];             // 0x38-0x4B
    u32 field_4C;                       // 0x4C (word stores; float store via CfObjectMove)
    u32 field_50;                       // 0x50
    u32 field_54;                       // 0x54
    u16 field_0x58;                     // 0x58
    u16 field_0x5A;                     // 0x5A
    u32 field_0x5C;                     // 0x5C
    f32 field_0x60;                     // 0x60
    f32 field_0x64;                     // 0x64
    u8 _pad68[0x70 - 0x68];             // 0x68-0x6F
    u16 field_0x70;                     // 0x70
    u16 field_0x72;                     // 0x72
    u8 _pad74[0x76 - 0x74];             // 0x74-0x75
    u16 field_0x76;                     // 0x76
    u8 _pad78[0x12C - 0x78];            // 0x78-0x12B
    void (CfObjectMove::*mMoveHook)();  // 0x12C ptmf
    u8 _pad138[0x144 - 0x138];          // 0x138-0x143
    CFunc8008E760Vec3W mVec144W;        // 0x144
    u8 _pad150[0x160 - 0x150];          // 0x150-0x15F
    f32 field_0x160;                    // 0x160
    f32 field_0x164;                    // 0x164
    f32 field_0x168;                    // 0x168
    f32 field_0x16C;                    // 0x16C
    u8 _pad170[0x17C - 0x170];          // 0x170-0x17B
    u32 field_0x17C;                    // 0x17C
    u32 field_0x180;                    // 0x180
    u16 field_0x184;                    // 0x184
    u16 field_0x186;                    // 0x186
    u8 _pad188[0x18C - 0x188];          // 0x188-0x18B
    u16 field_0x18C;                    // 0x18C
    u16 field_0x18E;                    // 0x18E
};

// func_80090DB4's view of CfObjectMove: base position/velocity words, the
// move-list entry pointer arithmetic at +0x78, the hook ptmf at +0x12C, the
// mVec138 words and the timers/flags.
struct CFunc80090DB4View {
    u8 _pad[0x0C];                      // 0x00-0x0B
    ml::CVec3 mPosition;                // 0x0C
    ml::CVec3 mVelocity;                // 0x18
    u8 _pad24[0x34 - 0x24];             // 0x24-0x33
    CNpcBaseDataView* field_0x34;       // 0x34
    u8 _pad38[0x4C - 0x38];             // 0x38-0x4B
    f32 field_0x4C;                     // 0x4C
    f32 field_0x50;                     // 0x50
    f32 field_0x54;                     // 0x54
    s16 field_0x58;                     // 0x58
    s16 field_0x5A;                     // 0x5A
    u32 field_0x5C;                     // 0x5C (ptmf dispatch index)
    u32 field_0x60;                     // 0x60 timer
    u8 _pad64[0x6C - 0x64];             // 0x64-0x6B
    f32 field_0x6C;                     // 0x6C
    u16 field_0x70;                     // 0x70
    u16 field_0x72;                     // 0x72
    u8 _pad74[0x78 - 0x74];             // 0x74-0x77
    CfMoveSubEntry field_0x78[8];       // 0x78 move-list entries
    u8 _pad118[0x12C - 0x118];          // 0x118-0x12B
    void (CfObjectMove::*mMoveHook)();  // 0x12C ptmf
    CFunc8008E760Vec3W mVec138W;        // 0x138
    u8 _pad144[0x160 - 0x144];          // 0x144-0x15F
    f32 field_0x160;                    // 0x160
    u8 _pad164[0x17C - 0x164];          // 0x164-0x17B
    u32 field_0x17C;                    // 0x17C
    u32 field_0x180;                    // 0x180
    u16 field_0x184;                    // 0x184
    u16 field_0x186;                    // 0x186
};

// Move sub-object as func_80090DB4 calls it: vtable +0x0C (int query), +0x10
// (int arg), +0xAC (position getter) and +0xC4 (float angle); data words at
// +0x98 / +0xC4 / +0x63C.
struct CFunc80090DB4SubVtbl {
    void* r00[0x0C / 4];                                // 0x00-0x0B
    int (*fn_0x0C)(void* self, int arg);                // 0x0C
    void (*fn_0x10)(void* self, int arg);               // 0x10
    void* r14[(0xAC - 0x14) / 4];                       // 0x14-0xAB
    ml::CVec3* (*fn_0xAC)(void* self);                  // 0xAC
    void* rB0[(0xC4 - 0xB0) / 4];                       // 0xB0-0xC3
    void (*fn_0xC4)(void* self, f32 angle);             // 0xC4
};
struct CFunc80090DB4Sub {
    CFunc80090DB4SubVtbl* vtable;                       // 0x00
    u8 _pad04[0x98 - 0x04];                             // 0x04-0x97
    void* field_98;                                     // 0x98
    u8 _pad9C[0xC4 - 0x9C];                             // 0x9C-0xC3
    void* field_C4;                                     // 0xC4
    u8 _padC8[0x63C - 0xC8];                            // 0xC8-0x63B
    f32 field_63C;                                      // 0x63C
};

// Vtable of the +0x3E9C embedded sub-object as func_8008D51C calls it:
// slots +0x50 (int arg), +0x8C (float query), +0x9C (pointer arg), +0xAC
// (position getter), +0xC4 (float angle), +0x138 (float* divisor) and +0x150
// (int arg).
struct CFunc8008D51CSubVtbl {
    void* r00[0x50 / 4];                                // 0x00-0x4F
    void (*fn_0x50)(void* self, int arg);               // 0x50
    void* r54[(0x8C - 0x54) / 4];                       // 0x54-0x8B
    f32 (*fn_0x8C)(void* self);                         // 0x8C
    void* r90[(0x9C - 0x90) / 4];                       // 0x90-0x9B
    void (*fn_0x9C)(void* self, void* a);               // 0x9C
    void* rA0[(0xAC - 0xA0) / 4];                       // 0xA0-0xAB
    ml::CVec3* (*fn_0xAC)(void* self);                  // 0xAC
    void* rB0[(0xC4 - 0xB0) / 4];                       // 0xB0-0xC3
    void (*fn_0xC4)(void* self, f32 angle);             // 0xC4
    void* rC8[(0x138 - 0xC8) / 4];                      // 0xC8-0x137
    float* (*fn_0x138)(void* self);                     // 0x138
    void* r13C[(0x150 - 0x13C) / 4];                    // 0x13C-0x14F
    void (*fn_0x150)(void* self, int arg);              // 0x150
};
struct CFunc8008D51CSub {
    CFunc8008D51CSubVtbl* vtable;                       // 0x00
};

// One 0x14-byte slot of the +0x1928 array func_8008D51C zeroes (word at +0x14
// then the five floats).
struct CFunc8008D51CSlot {
    f32 f0;                                             // 0x00
    f32 f4;                                             // 0x04
    f32 f8;                                             // 0x08
    f32 fC;                                             // 0x0C
    f32 f10;                                            // 0x10
    u32 w14;                                            // 0x14
};
// func_8008D51C's view of the enemy battle object: the +0x1928 zero region,
// +0x3374 flags, the +0x3E9C sub-object, the +0x3F60 flag object and the
// +0x455A halfword.
struct CFunc8008D51CObj {
    u8 _pad[0x1928];                                    // 0x00-0x1927
    CFunc8008D51CSlot slots[10];                        // 0x1928-0x19E7
    u8 _pad19E8[0x3374 - 0x19E8];                       // 0x19E8-0x3373
    u32 field_3374;                                     // 0x3374
    u8 _pad3378[0x3E9C - 0x3378];                       // 0x3378-0x3E9B
    CFunc8008D51CSub mSub;                              // 0x3E9C
    u8 _pad3EA0[0x3F60 - 0x3EA0];                       // 0x3EA0-0x3F5F
    CfObj3F60View* field_3F60;                          // 0x3F60
    u8 _pad3F64[0x455A - 0x3F64];                       // 0x3F64-0x4559
    u16 field_455A;                                     // 0x455A
};

// func_8008D51C's view of CfObjectMove: the +0x144/+0x168..+0x19C fields it
// reads and writes (0x188 / 0x18E are s16s read with lha).
struct CFunc8008D51CView {
    u8 _pad[0x34];                              // 0x00-0x33
    CNpcBaseDataView* field_0x34;               // 0x34
    u8 _pad38[0x144 - 0x38];                    // 0x38-0x143
    ml::CVec3 mVec144;                          // 0x144
    u8 _pad150[0x168 - 0x150];                  // 0x150-0x167
    f32 field_0x168;                            // 0x168
    f32 field_0x16C;                            // 0x16C
    f32 field_0x170;                            // 0x170
    f32 field_0x174;                            // 0x174
    u8 _pad178[0x17C - 0x178];                  // 0x178-0x17B
    u32 field_0x17C;                            // 0x17C
    u32 field_0x180;                            // 0x180
    u16 field_0x184;                            // 0x184
    u16 field_0x186;                            // 0x186
    s16 field_0x188;                            // 0x188
    u16 field_0x18A;                            // 0x18A
    u16 field_0x18C;                            // 0x18C
    s16 field_0x18E;                            // 0x18E
    u8 _pad190[0x19C - 0x190];                  // 0x190-0x19B
    f32 field_0x19C;                            // 0x19C
};

// func_8008FE8C's view of CfObjectMove: base position/velocity words, the
// move-list entries at +0x78, the mVec138 words, the heading/turn state and
// the timers.
struct CFunc8008FE8CView {
    u8 _pad[0x0C];                              // 0x00-0x0B
    ml::CVec3 mPosition;                        // 0x0C
    ml::CVec3 mVelocity;                        // 0x18
    u8 _pad24[0x34 - 0x24];                     // 0x24-0x33
    CNpcBaseDataView* field_0x34;               // 0x34
    u8 _pad38[0x3C - 0x38];                     // 0x38-0x3B
    f32 field_0x3C;                             // 0x3C
    u8 _pad40[0x4C - 0x40];                     // 0x40-0x4B
    f32 field_0x4C;                             // 0x4C
    f32 field_0x50;                             // 0x50
    f32 field_0x54;                             // 0x54
    s16 field_0x58;                             // 0x58
    s16 field_0x5A;                             // 0x5A
    u32 field_0x5C;                             // 0x5C (ptmf dispatch index)
    u32 field_0x60;                             // 0x60 timer
    f32 field_0x64;                             // 0x64 turn target
    f32 field_0x68;                             // 0x68 turn current
    f32 field_0x6C;                             // 0x6C
    u16 field_0x70;                             // 0x70 flags
    u16 field_0x72;                             // 0x72 gate
    s16 field_0x74;                             // 0x74 timer
    u8 _pad76[0x78 - 0x76];                     // 0x76-0x77
    CfMoveSubEntry field_0x78[8];               // 0x78 move-list entries
    u8 _pad118[0x138 - 0x118];                  // 0x118-0x137
    CFunc8008E760Vec3W mVec138W;                // 0x138
    u8 _pad144[0x160 - 0x144];                  // 0x144-0x15F
    f32 field_0x160;                            // 0x160
    u8 _pad164[0x17C - 0x164];                  // 0x164-0x17B
    u32 field_0x17C;                            // 0x17C
    u32 field_0x180;                            // 0x180
    u16 field_0x184;                            // 0x184
    u16 field_0x186;                            // 0x186
};

// Move sub-object as func_8008FE8C calls it: vtable +0x0C (int query), +0x10
// (int arg), +0x9C (pointer arg), +0xAC (position getter), +0xC4 (float
// angle) and +0xCC (heading query); data words at +0x98 / +0xC4 / +0x63C.
struct CFunc8008FE8CSubVtbl {
    void* r00[0x0C / 4];                                // 0x00-0x0B
    int (*fn_0x0C)(void* self, int arg);                // 0x0C
    void (*fn_0x10)(void* self, int arg);               // 0x10
    void* r14[(0x9C - 0x14) / 4];                       // 0x14-0x9B
    void (*fn_0x9C)(void* self, void* a);               // 0x9C
    void* rA0[(0xAC - 0xA0) / 4];                       // 0xA0-0xAB
    ml::CVec3* (*fn_0xAC)(void* self);                  // 0xAC
    void* rB0[(0xC4 - 0xB0) / 4];                       // 0xB0-0xC3
    void (*fn_0xC4)(void* self, f32 angle);             // 0xC4
    void* rC8[(0xCC - 0xC8) / 4];                       // 0xC8-0xCB
    float (*fn_0xCC)(void* self);                       // 0xCC
};
struct CFunc8008FE8CSub {
    CFunc8008FE8CSubVtbl* vtable;                       // 0x00
    u8 _pad04[0x98 - 0x04];                             // 0x04-0x97
    void* field_98;                                     // 0x98
    u8 _pad9C[0xC4 - 0x9C];                             // 0x9C-0xC3
    void* field_C4;                                     // 0xC4
    u8 _padC8[0x63C - 0xC8];                            // 0xC8-0x63B
    f32 field_63C;                                      // 0x63C
};

// func_8008A2C8's view of CfObjectMove: the velocity words, the mVec138 /
// mVec144 copies, the heading/timer halfwords and the +0x164..+0x19C floats.
struct CFunc8008A2C8View {
    u8 _pad[0x0C];                              // 0x00-0x0B
    ml::CVec3 mPos0;                            // 0x0C
    ml::CVec3 mVelocity;                        // 0x18
    u8 _pad24[0x34 - 0x24];                     // 0x24-0x33
    CNpcBaseDataView* field_0x34;               // 0x34
    u8 _pad38[0x3C - 0x38];                     // 0x38-0x3B
    f32 field_0x3C;                             // 0x3C
    u8 _pad40[0x138 - 0x40];                    // 0x40-0x137
    CFunc8008E760Vec3W mVec138W;                // 0x138
    CFunc8008E760Vec3W mVec144W;                // 0x144
    u8 _pad150[0x164 - 0x150];                  // 0x150-0x163
    f32 field_0x164;                            // 0x164
    f32 field_0x168;                            // 0x168
    f32 field_0x16C;                            // 0x16C
    f32 field_0x170;                            // 0x170
    f32 field_0x174;                            // 0x174
    f32 field_0x178;                            // 0x178
    u32 field_0x17C;                            // 0x17C
    u32 field_0x180;                            // 0x180
    u16 field_0x184;                            // 0x184
    u16 field_0x186;                            // 0x186
    u8 _pad188[0x190 - 0x188];                  // 0x188-0x18F
    u16 field_0x190;                            // 0x190
    s16 field_0x192;                            // 0x192
    u8 _pad194[0x196 - 0x194];                  // 0x194-0x195
    s16 field_0x196;                            // 0x196
    s16 field_0x198;                            // 0x198
    u8 _pad19A[0x19C - 0x19A];                  // 0x19A-0x19B
    f32 field_0x19C;                            // 0x19C
};

// func_8008A2C8 helpers: the CBattleManager +0x1AA byte and the global
// settings +0x214 flag word.
struct CFunc8008A2C8BMan {
    u8 _pad[0x1AA];                                     // 0x00-0x1A9
    u8 field_1AA;                                       // 0x1AA
};
struct CFunc8008A2C8Global {
    u8 _pad[0x214];                                     // 0x00-0x213
    u32 field_214;                                      // 0x214
};
// Proximity threshold table (retail .data floats indexed by the +0x17C
// mode bits in func_8008A2C8).
extern const f32 lbl_eu_804FB9C8[];

} // namespace cf

// C-linkage imports/exports (retail symbol names - keep linkage/signatures
// verbatim): __ptmf_test is the compiler-rt PTMF predicate; func_800895A8 is
// defined in CtrlMoveBase.cpp; func_8008C4F0 / func_8008D51C in this TU. The
// extern "C" keeps the call-site relocs at the unmangled retail names.
extern "C" long __ptmf_test(void* ptmf);
extern "C" void func_800895A8(cf::CfObjectMove* self);
extern "C" void func_8008C4F0(cf::CfObjectMove* self);
extern "C" int func_8008D51C(cf::CfObjectMove* self);
// Defined in CtrlMoveBase.cpp (func_80089990) and CtrlAct.cpp / CtrlMoveBase
// (func_800D581C / func_800D59FC); called by func_8008D444 on the move-data
// block / controller.
extern "C" void func_80089990(cf::CfObjectMove* self);
extern "C" void func_800D581C(cf::CNpcBaseDataView* data);
extern "C" void func_800D59FC(cf::CNpcBaseDataView* data);
// Per-frame move helpers (CtrlMoveBase.cpp / CtrlMoveNpc.cpp): func_80088974
// computes the approach direction, func_80089694 commits a velocity.
extern "C" int func_80088974(cf::CfObjectMove* self, ml::CVec3* out,
                             const ml::CVec3* src, int flagA, int flagB);
extern "C" void func_80089694(cf::CfObjectMove* self, const ml::CVec3* vec,
                              f32 f);
// Circular gimmick-object list accessor (CfGimmick.cpp) and battle-event
// helper (CfObjectImpl.cpp); func_80174C98 queries an actor word (CfAct.cpp).
extern "C" cf::CFunc8009156CList* func_800B6BC8();
extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);
// Icon-display helper (CActParamAnim.cpp).
extern "C" void func_8004B9D4(void* w, int a, int b, int c, int d);
// bdat row-value reader (CfObjectEne.cpp): (obj, column, row) -> raw word.
extern "C" u32 func_800AF82C(void* self, const char* column, int row);
// Move-data approach helper (CtrlMoveBase.cpp) and coli segment probe used by
// func_8008CDE8 (retail C-ABI names).
extern "C" int func_80089E88(cf::CfObjectMove* self, ml::CVec3* pos, int flag);
extern "C" int func_804B526C(void* a, void* b, void* c, void* d, int e, int f,
                             void* g);
// Global coli probe object (retail .sdata pointer) and the probe-block buffer
// func_8008CDE8 passes to func_804B526C.
extern cf::CFunc8008CDE8Probe* lbl_eu_80665958;
extern u8 lbl_eu_80571810[0x38];
// Collision-list height probe API used by func_8008B9C0 (retail C-ABI names;
// same declarations as CtrlAct.hpp). func_804BE398 is the 6-arg ground probe
// passing two FP args after the GPR args.
extern "C" int func_804BE398(void* vec, int a, int b, int c, f32 d, f32 e);
extern "C" int func_804BE4AC(void);
extern "C" void* func_804BE50C(u32 index);
extern "C" void* func_804BE520(int index);
extern "C" int func_804BE5A4(int a, int b);
// CtrlMoveBase.cpp helper func_8008B9C0 re-probes with (retail C-ABI name).
extern "C" int func_8008A01C(cf::CfObjectMove* self, ml::CVec3* pos);
// In-unit approach probe (retail func_8008B9C0): extern "C" keeps the
// call-site relocs in func_8008F9EC / func_8008F2E0 at the unmangled name.
extern "C" int func_8008B9C0(cf::CfObjectMove* self, ml::CVec3* out,
                             cf::CfObjectMove* other, f32 f1, f32 f2, int arg);
// nw4r fixed-point math kernels (C-ABI SDK functions; names are the retail
// mangled identifiers - keep C linkage so the call relocs match).
extern "C" f32 CosFIdx__Q24nw4r4mathFf(f32);
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
// CfGameManager member-count accessor (retail mangled name kept verbatim).
extern "C" int func_80086B44__Q22cf13CfGameManagerFv();
// func_8008E760 call sites: battle-object probe helpers (code_800B06A4.cpp),
// parts-element lookup (CPartsChange.cpp), party-info state builder/dispatch
// (CPartsChange.cpp) and the file/bdat column readers (retail C-ABI names).
extern "C" void* func_800B8AFC(void* self);
extern "C" void* func_800B89CC(u32 id);
extern "C" void* func_80198310(void);
extern "C" void* func_80193670(void);
extern "C" void* func_801974CC(void* a, void* b);
extern "C" void func_80198710(void* out, void* src, f32 a, int b, int c,
                               f32 d, f32 e);
extern "C" int func_8019876C(void* a, void* b);
extern "C" void* getFP__FPCc(const char* name);
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* column,
                                         int index);
// func_8008D51C call sites: battle-object state helpers and the move-data
// refresh (retail C-ABI names; func_8004C5EC is declared in CtrlNpc.hpp).
extern "C" void func_8004B7C0(void* self, const ml::CVec3* vec);
extern "C" f32 func_80496288(void* self);
extern "C" void func_80174B4C(void* obj, u32 flags);   // unified with CVision.hpp/CtrlAct.hpp forms (ABI-identical)
extern "C" void func_80193710(void* self);
// func_8008FE8C call site: direction-commit helper (CtrlMoveBase.cpp) and the
// C runtime rand() used for the turn jitter.
extern "C" void func_800898D4(void* self, ml::CVec3* vec);
extern "C" int rand(void);
// func_8008A2C8 call sites: battle-object helpers, the CfGameManager /
// CBattleManager singletons and the parts/actor lookups (retail names).
extern "C" int func_800B708C__Fi(int id);
extern "C" void* func_8016FE34(void* src);
extern "C" int func_80148778(void* self, int id);
extern "C" int func_801984F0(void* self, int index);
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
extern "C" void* getUnk80664658(void);
extern "C" void* getInstance__Q22cf14CBattleManagerFv(void);
extern "C" void func_800D9CA0(void* self, void* obj);
// func_8008A2C8 call site: game-flag query (C++ linkage mangles to the retail
// func_8006EF04__Fi).
int func_8006EF04(int r3);

// .sdata2 float constants compared/stored by the func_80092CC4 family.
// Declared const so MWCC treats the pool loads as constants and hoists them
// to retail's early position (docs/MWCC_CASES.md SDA scheduling).
extern const f32 lbl_eu_806665C0;  // 0.0f
extern const f32 lbl_eu_806665E4;  // 1.0f (rand-gated fallback)

// Further sdata2 constants used by the ctor (__ct__8008A104) and the
// func_8008C4F0 / func_8008E06C family.
extern const f32 lbl_eu_806665C4;   // ctor -> field_0x160
extern const f32 lbl_eu_806665C8;   // ctor -> field_0x170/0x174
extern const f32 lbl_eu_806665E0;   // func_8008C4F0 -> func_800899AC value
extern const f32 lbl_eu_806665F0;   // func_8008C4F0 -> field_0x3C
// Heading-scale factor (func_8008B580 / func_8008EF04 / func_8008F9EC scale
// the +0xCC heading index by it before CosFIdx/SinFIdx).
extern const f32 lbl_eu_806665CC;
// Heading-difference cosine threshold (func_8008EF04 compares against it).
extern const f32 lbl_eu_806665DC;
// func_8008B9C0 scan constants: mtRand(201) walk scale, ground-probe depth,
// height band and the fall-gap thresholds (upper 0.5 / lower 1.5 scales).
extern const f32 lbl_eu_80661C30;   // 0.005 (mtRand(201) scale)
extern const f32 lbl_eu_80666614;   // -20 (probe depth)
extern const f32 lbl_eu_80666618;   // 100 (height band)
extern const f32 _lbl_eu_80666604;  // 10000 (init best gap)
extern const f32 _lbl_eu_80666608;  // func_8008E760 field_0x16C fallback (mode byte != 3)
extern const f32 lbl_eu_8066661C;   // 0.5 (upper gap scale)
extern const f32 lbl_eu_806665D0;   // 1.5 (lower gap scale)
// func_8008CDE8 turn-probe constants: random-walk scale (-1), pi/2 turn bias.
extern const f32 lbl_eu_8066663C;   // -1
extern const f32 lbl_eu_8066A200;   // pi/2
// func_8008F9EC rand-scaled heading constants (angle scale, lerp offsets).
extern const f32 lbl_eu_80666610;
extern const f32 lbl_eu_80666638;
extern const f32 lbl_eu_80666668;
extern const f32 lbl_eu_8066A204;
// func_8009156C gimmick-scan distance threshold.
extern const f32 lbl_eu_80666678;
extern const f32 lbl_eu_80666648;   // func_8008E06C fallback for field_0x5C
extern const f32 lbl_eu_8066A210;   // pi/2 (func_8008E06C rand angle scale)
extern const f32 lbl_eu_80666628;   // func_80090DB4 pursuit-move scale
extern const f32 lbl_eu_806665EC;   // func_8008D51C +0x168 reset threshold
extern const f32 lbl_eu_80666640;   // func_8008D51C close-range threshold
extern const f32 lbl_eu_80666644;   // func_8008D51C move-speed divisor
extern const f32 lbl_eu_8066666C;   // func_8008FE8C lerp factor
// func_8008FE8C turn-range / close thresholds.
extern const f32 lbl_eu_80666670;
extern const f32 lbl_eu_80666674;
// func_8008A2C8 ground-probe / pursuit constants.
extern const f32 lbl_eu_806665D4;
extern const f32 lbl_eu_806665D8;
extern const f32 lbl_eu_806665E8;
extern const f32 lbl_eu_806665F4;
extern const f32 lbl_eu_806665F8;
extern const f32 lbl_eu_806665FC;
extern const f32 lbl_eu_80666600;
extern const f32 lbl_eu_8066660C;
// func_8008E760 field_0x160/0x4 scale (mode 2), field_0x16C fallbacks for the
// bdat bytes 0xFC / 0xFB, and the field_0x16C squaring threshold.
extern const f32 lbl_eu_80666658;
extern const f32 lbl_eu_8066665C;
extern const f32 lbl_eu_80666660;
extern const f32 lbl_eu_80666664;
// MWCC int->float conversion doubles (func_8008E06C): 80666620 is the signed
// 2^52+2^31 constant, 80666650 the plain 2^52 constant.
extern const f64 lbl_eu_80666620;
extern const f64 lbl_eu_80666650;
// func_8008D51C bdat-state pointer (retail .sdata word).
extern void* lbl_eu_80663E14;
// bdat column-name table func_8008E06C passes to func_800AF7E4 (+0xA column).
extern const char lbl_eu_804FB9E8[];

// Ptmf hook constants (retail data symbols): the ctor installs __ptmf_null
// and then the hook at lbl_eu_805278E4; func_8008E06C installs the hooks at
// lbl_eu_80527830 + 0x118 / 0x124 / 0x130.
extern void (cf::CfObjectMove::*const __ptmf_null)();
extern void (cf::CfObjectMove::*const lbl_eu_805278E4)();
typedef void (cf::CfObjectMove::*CfMoveDispatchPtmf)(void* a, void* b);
struct CfMoveHookPtmfs {
    u8 _pad[0x118];                                  // 0x00-0x117
    void (cf::CfObjectMove::*hook118)();             // 0x118
    void (cf::CfObjectMove::*hook124)();             // 0x124
    void (cf::CfObjectMove::*hook130)();             // 0x130
};
// Full hook-table view (adds the +0x13C..+0x1B4 records used by
// func_8008E2D4 / func_8008E760 / func_80090DB4). Declared as the symbol's
// extern type so member access emits the retail addi + base-load ptmf copy.
struct CfMoveHookPtmfsAll {
    u8 _pad[0x118];                                  // 0x00-0x117
    void (cf::CfObjectMove::*hook118)();             // 0x118
    void (cf::CfObjectMove::*hook124)();             // 0x124
    void (cf::CfObjectMove::*hook130)();             // 0x130
    void (cf::CfObjectMove::*hook13C)();             // 0x13C
    void (cf::CfObjectMove::*hook148)();             // 0x148
    void (cf::CfObjectMove::*hook154)();             // 0x154
    void (cf::CfObjectMove::*hook160)();             // 0x160
    void (cf::CfObjectMove::*hook16C)();             // 0x16C
    void (cf::CfObjectMove::*hook178)();             // 0x178
    void (cf::CfObjectMove::*hook184)();             // 0x184
    void (cf::CfObjectMove::*hook190)();             // 0x190
    void (cf::CfObjectMove::*hook19C)();             // 0x19C
    void (cf::CfObjectMove::*hook1A8)();             // 0x1A8 (func_80090DB4)
    void (cf::CfObjectMove::*hook1B4)();             // 0x1B4 (func_80090DB4)
};
extern const CfMoveHookPtmfsAll lbl_eu_80527830;
// func_8008E2D4's extension of the hook table (+0x13C / +0x148 records).
struct CfMoveHookPtmfsE2D4 {
    u8 _pad[0x13C];                                  // 0x00-0x13B
    void (cf::CfObjectMove::*hook13C)();             // 0x13C
    void (cf::CfObjectMove::*hook148)();             // 0x148
};
// func_8008E760's extension of the hook table (+0x154 / +0x160 / +0x16C
// records).
struct CfMoveHookPtmfsE760 {
    u8 _pad[0x154];                                  // 0x00-0x153
    void (cf::CfObjectMove::*hook154)();             // 0x154
    void (cf::CfObjectMove::*hook160)();             // 0x160
    void (cf::CfObjectMove::*hook16C)();             // 0x16C
};
// Ptmf hook constant installed by func_8009156C (3-word record at
// lbl_eu_805279F0).
extern void (cf::CfObjectMove::*const lbl_eu_805279F0)();
// Secondary vtable installed at +0x48 by the ctor.
extern const u8 lbl_eu_805279FC[];

// Imports for the ctor / func_8008C4F0 / func_8008E06C call sites (retail
// unmangled names). extern "C" keeps the call-site relocs unmangled - a plain
// C++ declaration makes MWCC append the __FP<params> mangling at the call
// site. __ct__80088904 / func_80089684 / func_800899AC are defined in
// CtrlMoveBase.cpp; func_8004B8B0 in CActParamAnim.cpp; func_800AF7E4 in
// CfObjectEne.cpp; func_8008D444 / func_8008BEEC in this TU.
extern "C" void __ct__80088904(cf::CfObjectMove* self);
extern "C" void func_80089684(cf::CfObjectMove* self);
extern "C" void func_8004B8B0(void* self, u32 a, u32 b, f32 value);
extern "C" void func_800899AC(void* obj, f32 value);
extern "C" u32 func_800AF7E4(void* self, const char* column);
extern "C" void func_8008D444(cf::CfObjectMove* self, cf::CFunc8008D444Obj* obj, int flag);
extern "C" void func_8008BEEC(cf::CfObjectMove* self, void* obj, int arg2, u32 arg3);
extern "C" void func_8008CDE8(cf::CfObjectMove* self, ml::CVec3* out, f32 f1);

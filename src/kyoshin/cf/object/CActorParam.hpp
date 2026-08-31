#pragma once

#include <types.h>
#include "kyoshin/cf/object/CDebugState.hpp"
#include "kyoshin/cf/object/CBattleState.hpp"
#include "kyoshin/cf/object/CActorState.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include <cstring>

// `const` so MWCC treats the sdata2 pool load as a constant and hoists it
// above the frame stores (MWCC_CASES §extern-const-float-hoist); plain
// `extern float` leaves the lfs after the GPR saves (4-byte shift).
extern const float lbl_eu_806677E4; // sdata2: default gauge value (CActorParam_UnkVirtualFunc67)
extern const float lbl_eu_806677E0; // sdata2: gauge default (CActorParam_UnkVirtualFunc141/142)
extern void* lbl_eu_806640DC; // .sbss arts bdat file pointer (CActorParam_UnkVirtualFunc87)
extern char lbl_eu_80503438[]; // rodata column-name string block (CActorParam_UnkVirtualFunc87)
extern double lbl_eu_80667848; // sdata2: +0.5 rounding constant (CActorParam_UnkVirtualFunc149)
extern double lbl_eu_80667850; // sdata2: -0.5 rounding constant (CActorParam_UnkVirtualFunc149)
extern const float lbl_eu_806677E8; // sdata2: gauge lower clamp (CActorParam_UnkVirtualFunc175)
extern const float lbl_eu_80667864; // sdata2: gauge upper clamp (CActorParam_UnkVirtualFunc175)
extern const double lbl_eu_806677F8; // sdata2: 0x4330000080000000 u32->double magic (CActorParam_UnkVirtualFunc11)
extern float lbl_eu_80667830; // sdata2: scale constant (CActorParam_UnkVirtualFunc11)
extern const double lbl_eu_806677F0; // sdata2: u8/u16->double magic 0x4330000000000000 (Func10/159)
extern const float lbl_eu_80667818; // sdata2: arts-magnitude scale (Func10)
extern const float lbl_eu_806677EC; // sdata2: 30.0f gauge divisor (Func140 / func_801765A4; const so it hoists above the frame stores)
extern const float lbl_eu_80667868; // sdata2: SP/exp gain scale (CActorParam_UnkVirtualFunc12)
extern const float lbl_eu_8066786C; // sdata2: arts-gauge threshold (CActorParam_UnkVirtualFunc12)
extern const float lbl_eu_80667800; // sdata2: 2.0f arts-slot default (Func140; const so the load is scheduled before the unk0 store)
extern const float lbl_eu_80667804; // sdata2: 3.0f arts-slot default (Func140; const so MWCC loads it once for the two stores)
// lbl_eu_8066A1F8 (pi) is declared in CfObjectEnumList.hpp
// CActorParam_UnkVirtualFunc177 constants (const so MWCC hoists each into a
// callee-saved FPR above the main battle-entry loop, matching retail).
extern const float lbl_eu_80667814; // 0.05f crit-scale
extern const double lbl_eu_80667820; // 0.15 clamp-level rate
extern const float lbl_eu_80667828; // -1.0f negate
extern const float lbl_eu_8066782C; // 0.5f half multiplier
extern const float lbl_eu_80667834; // 0.9f
extern const float lbl_eu_80667838; // 0.1f status tick
extern const float lbl_eu_8066783C; // 5.0f default duration
extern const float lbl_eu_80667840; // 2.5f boost multiplier
extern const float lbl_eu_80667844; // 4.0f extended duration
extern const float lbl_eu_80667808; // sdata2: unk1620 default (ctor)
extern const float lbl_eu_8066780C; // sdata2: unk1630 multiplier (ctor)
extern const float lbl_eu_80667810; // sdata2: unk1630 pi divisor (ctor)

namespace cf {

    class UnkClass_CActorParam15E0;

    //size: 0x7C
    struct CActorParam_UnkStruct2 {
        u8 unk0[0x40];
        u16 unk40;
        u8 unk42[0x78 - 0x42];
        u32 unk78;
    };

    //might be fake?
    struct CActorParam_UnkStruct6 {
        CActorParam_UnkStruct6(){
            unk0 = 0;
        }
    
        u8 unk0;
    };

    struct CActorParam_Bitflags {
        CActorParam_Bitflags(){
            flags = 0;
        }

        u32 flags;
    };

    //size: 0xBC
    struct CActorParam_UnkStruct1 {
        CActorParam_UnkStruct1() {
            init();
        }

        // Order matches retail ctor scheduling: scalars, unk8 wipe, gauge
        // floats default to the sdata2 constant (not 0!), unk84 wipe, flags
        // last.
        void init(){
            unk0 = 0;
            unk4 = 0;
            std::memset(unk8, 0, sizeof(unk8));
            unk48 = 0;
            unk4C = -1;
            unk50 = 0;
            unk54 = lbl_eu_806677E4;
            unk58 = lbl_eu_806677E4;
            unk5C = lbl_eu_806677E4;
            unk60 = lbl_eu_806677E4;
            unk64 = lbl_eu_806677E4;
            unk7C = 0;
            unk80 = 0;
            unkB8 = 0;
            unk68 = lbl_eu_806677E4;
            unk6C = lbl_eu_806677E4;
            unk70 = 0;
            unk72 = 0;
            std::memset(unk84, 0, sizeof(unk84));
            mFlagsArray[0].flags = 0;
            mFlagsArray[1].flags = 0;
        }

        u32 unk0;
        u32 unk4;
        u8 unk8[0x40];
        u32 unk48;
        int unk4C;
        CActorParam_UnkStruct2* unk50;
        float unk54;
        float unk58;
        float unk5C;
        float unk60;
        float unk64;
        float unk68;
        float unk6C;
        s16 unk70;
        s16 unk72;
        CActorParam_Bitflags mFlagsArray[2]; //0x74
        u32 unk7C;
        u16 unk80;
        // +0x82..+0x83 is padding: CActorParam_UnkVirtualFunc137's inline
        // struct copy skips it, so unk84 is declared as a u32 array (the
        // 4-byte alignment forces the gap as natural padding that MWCC's
        // member-wise struct copy omits) instead of u8[0x34].
        u32 unk84[13]; // 0x84..0xB7
        u32 unkB8;

        enum Flags_74 {
            FLAG_BIT_0 = (1 << 0),
            FLAG_BIT_1 = (1 << 1),
            FLAG_BIT_2 = (1 << 2),
            FLAG_BIT_3 = (1 << 3),
            FLAG_BIT_4 = (1 << 4),
            FLAG_BIT_5 = (1 << 5),
            FLAG_BIT_6 = (1 << 6),
            FLAG_BIT_7 = (1 << 7),
            FLAG_BIT_8 = (1 << 8),
            FLAG_BIT_9 = (1 << 9),
            FLAG_BIT_10 = (1 << 10),
            FLAG_BIT_11 = (1 << 11),
            FLAG_BIT_12 = (1 << 12),
            FLAG_BIT_13 = (1 << 13),
            FLAG_BIT_14 = (1 << 14),
            FLAG_BIT_15 = (1 << 15),
            FLAG_BIT_16 = (1 << 16),
            FLAG_BIT_17 = (1 << 17),
            FLAG_BIT_18 = (1 << 18),
            FLAG_BIT_19 = (1 << 19),
            FLAG_BIT_20 = (1 << 20),
            FLAG_BIT_21 = (1 << 21),
            FLAG_BIT_22 = (1 << 22),
            FLAG_BIT_23 = (1 << 23),
            FLAG_BIT_24 = (1 << 24),
            FLAG_BIT_25 = (1 << 25),
            FLAG_BIT_26 = (1 << 26),
            FLAG_BIT_27 = (1 << 27),
            FLAG_BIT_28 = (1 << 28),
            FLAG_BIT_29 = (1 << 29),
            FLAG_BIT_30 = (1 << 30),
            FLAG_BIT_31 = (1 << 31),
        };
    };

    //size: 0x52
    struct CActorParam_UnkStruct4 {
        CActorParam_UnkStruct4() {
            std::memset(this, 0, sizeof(*this)); //wtf??
        }

        u8 unk0[0x4E];
        CActorParam_UnkStruct6 unk4E[4];
    };

    //TODO: related to above struct?
    //size: 0x78
    struct CActorParam_UnkStruct3 {
        CActorParam_UnkStruct3() {
            unk74 = 0;

            std::memset(this, 0, sizeof(*this)); //wtf??

            unk5C = lbl_eu_806677E8;
            unk38 = 5;
            unk3A = 5;
        }

        u8 unk0[0x38];
        u16 unk38;
        u16 unk3A;
        u8 unk3C[0x44 - 0x3C];
        float unk44;
        u8 unk48[4];
        float unk4C;
        u8 unk50[0x5C - 0x50];
        float unk5C;
        u8 unk60[0x70 - 0x60];
        CActorParam_UnkStruct6 unk70[4];
        u32 unk74;
    };

    //size: 0x18
    struct CActorParam_UnkStruct5 {
        CActorParam_UnkStruct5(){
            std::memset(this, 0, sizeof(*this)); //wtf??
        }

        void init(){
            unk14 = 0;
            unk4 = 0;
            unk0 = 0;
            unkC = 0;
            unk8 = 0;
            unk10 = 0;
        }

        float unk0;
        float unk4;
        float unk8;
        float unkC;
        float unk10;
        u32 unk14;
    };

    // Absolute-offset view of the byte fields at 0x162B/0x162C (member
    // block is +8 shifted by the base-class layout; see CActorParamGaugeView).
    // Used by CActorParam_UnkVirtualFunc12's gauge sets.
    struct CActorParam162BView {
        u8 _pad[0x162B];
        u8 field_0x162B; // 0x162B gauge byte
        u8 field_0x162C; // 0x162C gauge byte
    };

    // Absolute-offset view of the gauge fields at 0x160C..0x1618. The
    // CActorParam member block is shifted +8 by the base-class layout (the
    // empty CDebugState base follows CBattleState's 0x15DC bytes), so raw
    // views pin the retail offsets (same approach as CfActorParamFields in
    // CfObjectActor.hpp). Used by CActorParam_UnkVirtualFunc54/58/60/64.
    struct CActorParamGaugeView {
        u8 _pad[0x160C];
        s16 field_0x160C; // 0x160C gauge current (Func54/58)
        s16 field_0x160E; // 0x160E gauge max (Func54)
        u8 _pad1610[0x1614 - 0x1610];
        s16 field_0x1614; // 0x1614 gauge current (Func60/64)
        s16 field_0x1616; // 0x1616 gauge max (Func60)
    };

    // Absolute-offset view of the 8-entry CActorParam_UnkStruct5 array at
    // 0x1928 (member block is +8 shifted by the base-class layout; see
    // CActorParamGaugeView). Used by CActorParam_UnkVirtualFunc141/143.
    struct CActorParamUnk1928View {
        u8 _pad[0x1928];
        CActorParam_UnkStruct5 entries[8];
    };

    // Absolute-offset view of the 10-entry CActorParam_UnkStruct1 array at
    // 0x2A84 (member block is +8 shifted by the base-class layout; see
    // CActorParamGaugeView). Used by CActorParam_UnkVirtualFunc137.
    struct CActorParam2A84View {
        u8 _pad[0x2A84];
        CActorParam_UnkStruct1 entries[10];
    };

    // Absolute-offset view of the fields touched by func_801765A4 (member
    // block is +8 shifted by the base-class layout; see CActorParamGaugeView).
    struct CActorParam765View {
        u8 _pad15DC[0x15DC];
        void* field_0x15DC;
        u8 _pad15E0[0x15FC - 0x15E0];
        float field_0x15FC;
        u8 _pad1600[0x2A80 - 0x1600];
        u32 field_0x2A80;
        u8 _pad2A84[0x3358 - 0x2A84];
        s16 field_0x3358;
    };

    // Absolute-offset view of the arts gauge fields at 0x17F0/0x17FC (inside
    // the unk17E4 CActorParam_UnkStruct3 block; member block is +8 shifted by
    // the base-class layout, see CActorParamGaugeView). Used by
    // CActorParam_UnkVirtualFunc48.
    struct CActorParamArtsGaugeView {
        u8 _pad[0x17F0];
        float field_0x17F0; // 0x17F0 arts gauge current (Func48)
        u8 _pad17F4[0x17FC - 0x17F4];
        float field_0x17FC; // 0x17FC arts gauge max (Func48)
    };

    // Absolute-offset view of the status flag byte at 0x3354 (member block is
    // +8 shifted by the base-class layout; see CActorParamGaugeView). Used by
    // CActorParam_UnkVirtualFunc135.
    struct CActorParam3354View {
        u8 _pad[0x3354];
        u8 field_0x3354;
    };

    // Absolute-offset view of the status fields at 0x3358..0x3368 (member
    // block is +8 shifted by the base-class layout; see CActorParamGaugeView).
    // Used by CActorParam_UnkVirtualFunc158/160.
    struct CActorParamStatusView {
        u8 _pad[0x3358];
        u16 field_0x3358;
        s16 field_0x335A; // 0x335A status counter (Func158 clamps to [0,4])
        u8 _pad335C[0x335E - 0x335C];
        u8 field_0x335E;
        u8 _pad335F[0x3368 - 0x335F];
        float field_0x3368;
    };
    // Absolute-offset view of the status flag word at 0x3374 (member block is
    // +8 shifted; see CActorParamGaugeView). Used by CActorParam_UnkVirtualFunc6.
    struct CActorParamFlagsView {
        u8 _pad[0x3374];
        u32 field_0x3374;
    };

    // Absolute-offset view of the arts-stat fields touched by
    // CActorParam_UnkVirtualFunc174 (member block is +8 shifted by the
    // base-class layout; see CActorParamGaugeView).
    struct CActorParamArtsStatView {
        u8 _pad174E[0x174E];
        s16 field_0x174E;
        u8 _pad1750[0x1808 - 0x1750];
        float field_0x1808;
        float field_0x180C;
        u8 _pad1810[0x1812 - 0x1810];
        s16 field_0x1812;
        u8 _pad1814[0x1816 - 0x1814];
        s16 field_0x1816;
        u8 _pad1818[0x181A - 0x1818];
        s16 field_0x181A;
        u8 _pad181C[0x1820 - 0x181C];
        u8 field_0x1820;
        u8 _pad1821[0x1824 - 0x1821];
        float field_0x1824;
        float field_0x1828;
        float field_0x182C;
        u8 _pad1830[0x1839 - 0x1830];
        u8 field_0x1839;
        u8 _pad183A[0x3374 - 0x183A];
        u32 field_0x3374;
        s16 field_0x3378;
    };

    // Row returned by func_8009D7E4 (the arts-stats sub-row at +0x1C of the
    // char-data object); consumed by CActorParam_UnkVirtualFunc174.
    struct CActorParamArtsRow {
        float field_0x0;
        float field_0x4;
        float field_0x8;
        s16 field_0xC;
        s16 field_0xE;
        u16 field_0x10;
        s16 field_0x12;
        u16 field_0x14;
        s16 field_0x16;
        s16 field_0x18;
        u16 field_0x1A;
        u8 _pad1C[0x1D - 0x1C];
        u8 field_0x1D;
        u16 field_0x1E;
    };

    // Char-data object passed to CActorParam_UnkVirtualFunc174 (retail ABI r4)
    // or returned by func_8009EC9C: arts-stats sub-row at +0x1C fed to
    // func_8009D7E4.
    struct CActorParam174Arg {
        u8 _pad1C[0x1C];
        u8 field_0x1C[0x40]; // +0x1C: arts-stats sub-row base
    };

    // Argument passed through the CActorParam_UnkVirtualFunc179 slot (retail
    // ABI r4): u16 dispatch id at +0xC (same shape as CfActorParamArg in
    // CfObjectActor.hpp, whose override calls this base with the same arg).
    struct CActorParam179Arg {
        u8 _pad[0xC];
        u16 field_0xC;
    };

    // u32<->double conversion helper: pins the 0x4330000080000000 magic to
    // the named sdata2 constant instead of an MWCC-synthesised pool entry
    // (same convention as F64Conv in CtrlAct.hpp). Used by
    // CActorParam_UnkVirtualFunc11.
    union CActorParamF64Conv {
        u32 w[2];
        double d;
    };

    // Argument passed through the CActorParam_UnkVirtualFunc180 slot (retail
    // ABI r4): u16 dispatch id at +0xC, target actor id at +0x10, s16 at
    // +0x14 (same arg family as CActorParam179Arg).
    struct CActorParam180Arg {
        u8 _pad[0xC];
        u16 field_0xC;
        u8 _padE[2];
        void* field_0x10;
        s16 field_0x14;
    };

    // Argument passed through the CActorParam_UnkVirtualFunc12 slot (retail
    // ABI r4): flags block at +0x50, u16 dispatch id at +0x80, int roll value
    // at +0xA8 (fed to the signed %100 magic-division).
    struct CActorParam12Arg {
        u8 _pad[0x50];
        void* field_0x50; // 0x50: CActorParam12Flags*
        u8 _pad54[0x80 - 0x54];
        u16 field_0x80; // 0x80: u16 dispatch id
        u8 _pad82[0xA8 - 0x82];
        int field_0xA8; // 0xA8: roll value (signed % 100)
    };

    // Flags block reachable through CActorParam12Arg::field_0x50.
    struct CActorParam12Flags {
        u8 _pad[0x78];
        u32 field_0x78; // 0x78: flags (bit 30 = enhanced path, bit 14 = skip byte)
    };

    // Actor view for CActorParam_UnkVirtualFunc12's target reads (member
    // block is +8 shifted by the base-class layout; see CActorParamGaugeView).
    struct CActorParam12ActorView {
        u8 _pad[0x3F00];
        u32 field_0x3F00; // 0x3F00: flags (bit 1 gate)
        u8 _pad3F04[0x3F10 - 0x3F04];
        u32 field_0x3F10; // 0x3F10: actor id
        u8 _pad3F14[0x3F28 - 0x3F14];
        u16 field_0x3F28; // 0x3F28: actor state id (1 = target)
    };

    // Argument passed through the CActorParam_UnkVirtualFunc11 slot (retail
    // ABI r4): u32 status word at +0x74 (bit 0x100 gate).
    struct CActorParam11Arg {
        u8 _pad[0x74];
        u32 field_0x74;
    };

    // Target-object view for CActorParam_UnkVirtualFunc140 (retail ABI r4):
    // CBattleState subobject at +8 (probed with func_80148778), actor id at
    // +0x3F10.
    struct CActorParam140Target {
        u8 _pad8[0x8];
        u8 cbstate[0x3F10 - 0x8]; // +8: CBattleState subobject
        u32 field_0x3F10; // 0x3F10: actor id
    };

    // Argument passed through the CActorParam_UnkVirtualFunc10 slot (retail
    // ABI r4): target-object pointer at +0x50, status word at +0x74 (bit 0
    // gate).
    struct CActorParam10Arg {
        u8 _pad[0x50];
        void* field_0x50; // 0x50: CActorParam10Target*
        u8 _pad54[0x74 - 0x54];
        u32 field_0x74; // 0x74: status word (bit 0 gate)
    };

    // Vtable interface for the target object's embedded vtable pointer at
    // +0x84 (slot 0xC returns the int read by Func10's u32 conversion).
    struct CActorParam10TargetVt {
        virtual void _v008();
        virtual void _v00C();
        virtual int vf0C(); // 0xC
    };

    // Object reachable through CActorParam10Arg::field_0x50.
    struct CActorParam10Target {
        u8 _pad[0x44];
        u8 field_0x44; // 0x44: mode byte (1 skips the Func133 gate)
        u8 _pad45[0x78 - 0x45];
        u32 field_0x78; // 0x78: flags (0x40000000 / 0x4000 / 0x20)
        u8 _pad7C[0x84 - 0x7C];
        CActorParam10TargetVt* vt; // 0x84: embedded vtable
    };

    //size: 0x3384
    class CActorParam : public CActorState, public CBattleState, public CDebugState {
    public:
        CActorParam(UNKTYPE* r4, UNKTYPE* r5);
    #pragma region vtable
        virtual const char* CActorParam_UnkVirtualFunc1();   //0x98
        virtual void* CActorParam_UnkVirtualFunc2();   //0x9C
        virtual void CActorParam_UnkVirtualFunc3();   //0xA0
        virtual void CActorParam_UnkVirtualFunc4(void* arts);   //0xA4
        virtual void CActorParam_UnkVirtualFunc5(int flag);   //0xA8
        virtual void CActorParam_UnkVirtualFunc6(int val);   //0xAC
        virtual void CActorParam_UnkVirtualFunc7();   //0xB0
        virtual void CActorParam_UnkVirtualFunc8();   //0xB4
        virtual void CActorParam_UnkVirtualFunc9();   //0xB8
        virtual void CActorParam_UnkVirtualFunc10();  //0xBC
        virtual void CActorParam_UnkVirtualFunc11();  //0xC0
        virtual void CActorParam_UnkVirtualFunc12();  //0xC4
        virtual void CActorParam_UnkVirtualFunc13();  //0xC8
        virtual void CActorParam_UnkVirtualFunc14(u8 val);  //0xCC
        virtual void CActorParam_UnkVirtualFunc15();  //0xD0
        virtual void CActorParam_UnkVirtualFunc16(float val);  //0xD4
        virtual void CActorParam_UnkVirtualFunc17();  //0xD8
        virtual void CActorParam_UnkVirtualFunc18();  //0xDC
        virtual u32 CActorParam_UnkVirtualFunc19();  //0xE0
        virtual void CActorParam_UnkVirtualFunc20();  //0xE4
        virtual void CActorParam_UnkVirtualFunc21();  //0xE8
        virtual void CActorParam_UnkVirtualFunc22();  //0xEC
        virtual void CActorParam_UnkVirtualFunc23();  //0xF0
        virtual void CActorParam_UnkVirtualFunc24();  //0xF4
        virtual void CActorParam_UnkVirtualFunc25();  //0xF8
        virtual u32 CActorParam_UnkVirtualFunc26();  //0xFC
        virtual void CActorParam_UnkVirtualFunc27();  //0x100
        virtual void CActorParam_UnkVirtualFunc28();  //0x104
        virtual u32 CActorParam_UnkVirtualFunc29();  //0x108
        virtual void CActorParam_UnkVirtualFunc30();  //0x10C
        virtual void CActorParam_UnkVirtualFunc31();  //0x110
        virtual void CActorParam_UnkVirtualFunc32();  //0x114
        virtual void CActorParam_UnkVirtualFunc33(float val);  //0x118
        virtual void CActorParam_UnkVirtualFunc34(float value);  //0x11C
        virtual void CActorParam_UnkVirtualFunc35(float value, int a, int b, int c);  //0x120
        virtual void CActorParam_UnkVirtualFunc36();  //0x124
        virtual float CActorParam_UnkVirtualFunc37();  //0x128
        virtual float CActorParam_UnkVirtualFunc38();  //0x12C
        virtual float CActorParam_UnkVirtualFunc39();  //0x130
        virtual void CActorParam_UnkVirtualFunc40();  //0x134
        virtual void CActorParam_UnkVirtualFunc41(float val);  //0x138
        virtual void CActorParam_UnkVirtualFunc42(float val);  //0x13C
        virtual void CActorParam_UnkVirtualFunc43();  //0x140
        virtual void CActorParam_UnkVirtualFunc44();  //0x144
        virtual void CActorParam_UnkVirtualFunc45();  //0x148
        virtual void CActorParam_UnkVirtualFunc46();  //0x14C
        virtual void CActorParam_UnkVirtualFunc47(float val);  //0x150
        virtual void CActorParam_UnkVirtualFunc48(float delta);  //0x154
        virtual float CActorParam_UnkVirtualFunc49();  //0x158
        virtual void CActorParam_UnkVirtualFunc50();  //0x15C
        virtual float CActorParam_UnkVirtualFunc51();  //0x160
        virtual void CActorParam_UnkVirtualFunc52();  //0x164
        virtual void CActorParam_UnkVirtualFunc53();  //0x168
        virtual void CActorParam_UnkVirtualFunc54(int delta);  //0x16C
        virtual void CActorParam_UnkVirtualFunc55(u16 val);  //0x170
        // int (not s16): Unk179 passes the getter result to Unk54 via mr r4,r3;
        // s16 forces MWCC to insert extsh and breaks the 0x94 match.
        virtual int CActorParam_UnkVirtualFunc56();  //0x174
        virtual int CActorParam_UnkVirtualFunc57();  //0x178
        virtual void CActorParam_UnkVirtualFunc58();  //0x17C
        virtual void CActorParam_UnkVirtualFunc59();  //0x180
        virtual void CActorParam_UnkVirtualFunc60(int delta);  //0x184
        virtual void CActorParam_UnkVirtualFunc61(u16 val);  //0x188
        virtual int CActorParam_UnkVirtualFunc62();  //0x18C
        virtual int CActorParam_UnkVirtualFunc63();  //0x190
        virtual void CActorParam_UnkVirtualFunc64();  //0x194
        virtual void CActorParam_UnkVirtualFunc65(float val);  //0x198
        virtual float CActorParam_UnkVirtualFunc66();  //0x19C
        virtual void CActorParam_UnkVirtualFunc67();  //0x1A0
        virtual void CActorParam_UnkVirtualFunc68(float val);  //0x1A4
        virtual float CActorParam_UnkVirtualFunc69();  //0x1A8
        virtual void CActorParam_UnkVirtualFunc70();  //0x1AC
        virtual void CActorParam_UnkVirtualFunc71();  //0x1B0
        virtual void CActorParam_UnkVirtualFunc72();  //0x1B4
        virtual void CActorParam_UnkVirtualFunc73();  //0x1B8
        virtual void CActorParam_UnkVirtualFunc74(float val);  //0x1BC
        virtual void CActorParam_UnkVirtualFunc75();  //0x1C0
        virtual void* CActorParam_UnkVirtualFunc76();  //0x1C4
        virtual void CActorParam_UnkVirtualFunc77();  //0x1C8
        virtual void CActorParam_UnkVirtualFunc78();  //0x1CC
        virtual void CActorParam_UnkVirtualFunc79();  //0x1D0
        virtual void CActorParam_UnkVirtualFunc80();  //0x1D4
        virtual void CActorParam_UnkVirtualFunc81(u32 val);  //0x1D8
        virtual void CActorParam_UnkVirtualFunc82();  //0x1DC
        virtual void CActorParam_UnkVirtualFunc83(u32 addend);  //0x1E0
        virtual void CActorParam_UnkVirtualFunc84();  //0x1E4
        virtual u32 CActorParam_UnkVirtualFunc85();  //0x1E8
        virtual int CActorParam_UnkVirtualFunc86();  //0x1EC
        virtual u32 CActorParam_UnkVirtualFunc87();  //0x1F0
        virtual void CActorParam_UnkVirtualFunc88();  //0x1F4
        virtual void CActorParam_UnkVirtualFunc89();  //0x1F8
        virtual void CActorParam_UnkVirtualFunc90(void* arg);  //0x1FC
        virtual u32 CActorParam_UnkVirtualFunc91();  //0x200
        virtual void CActorParam_UnkVirtualFunc92(const void* src);  //0x204
        virtual void CActorParam_UnkVirtualFunc93();  //0x208
        virtual void* CActorParam_UnkVirtualFunc94();  //0x20C
        virtual void CActorParam_UnkVirtualFunc95();  //0x210
        virtual void CActorParam_UnkVirtualFunc96();  //0x214
        virtual void CActorParam_UnkVirtualFunc97();  //0x218
        virtual void CActorParam_UnkVirtualFunc98(const void* src);  //0x21C
        virtual void CActorParam_UnkVirtualFunc99();  //0x220
        virtual void* CActorParam_UnkVirtualFunc100(); //0x224
        virtual void CActorParam_UnkVirtualFunc101(); //0x228
        virtual void CActorParam_UnkVirtualFunc102(); //0x22C
        virtual void CActorParam_UnkVirtualFunc103(); //0x230
        virtual void CActorParam_UnkVirtualFunc104(); //0x234
        virtual void CActorParam_UnkVirtualFunc105(); //0x238
        virtual void CActorParam_UnkVirtualFunc106(); //0x23C
        virtual void CActorParam_UnkVirtualFunc107(); //0x240
        virtual void CActorParam_UnkVirtualFunc108(); //0x244
        virtual void CActorParam_UnkVirtualFunc109(); //0x248
        virtual void CActorParam_UnkVirtualFunc110(); //0x24C
        virtual void CActorParam_UnkVirtualFunc111(); //0x250
        virtual void CActorParam_UnkVirtualFunc112(); //0x254
        virtual u32* CActorParam_UnkVirtualFunc113(); //0x258
        virtual void CActorParam_UnkVirtualFunc114(); //0x25C
        virtual bool CActorParam_UnkVirtualFunc115(); //0x260
        virtual void CActorParam_UnkVirtualFunc116(float val); //0x264
        virtual float* CActorParam_UnkVirtualFunc117(); //0x268
        virtual void CActorParam_UnkVirtualFunc118(); //0x26C
        virtual float* CActorParam_UnkVirtualFunc119(); //0x270
        virtual void CActorParam_UnkVirtualFunc120(); //0x274
        virtual void CActorParam_UnkVirtualFunc121(); //0x278
        virtual void* CActorParam_UnkVirtualFunc122(); //0x27C
        virtual void CActorParam_UnkVirtualFunc123(); //0x280
        virtual void CActorParam_UnkVirtualFunc124(); //0x284
        virtual void* CActorParam_UnkVirtualFunc125(); //0x288
        virtual void* CActorParam_UnkVirtualFunc126(); //0x28C
        virtual UnkClass_CActorParam15E0* CActorParam_UnkVirtualFunc127(); //0x290
        virtual void CActorParam_UnkVirtualFunc128(); //0x294
        virtual CActorParam_UnkStruct1* CActorParam_UnkVirtualFunc129(); //0x298
        virtual void CActorParam_UnkVirtualFunc130(); //0x29C
        virtual void CActorParam_UnkVirtualFunc131(); //0x2A0
        virtual void* CActorParam_UnkVirtualFunc132(); //0x2A4
        virtual u8 CActorParam_UnkVirtualFunc133(); //0x2A8
        virtual void CActorParam_UnkVirtualFunc134(); //0x2AC
        virtual void CActorParam_UnkVirtualFunc135(); //0x2B0
        virtual void CActorParam_UnkVirtualFunc136(); //0x2B4
        virtual void CActorParam_UnkVirtualFunc137(); //0x2B8
        virtual bool CActorParam_UnkVirtualFunc138(); //0x2BC
        virtual void CActorParam_UnkVirtualFunc139(); //0x2C0
        virtual void CActorParam_UnkVirtualFunc140(); //0x2C4
        virtual void CActorParam_UnkVirtualFunc141(); //0x2C8
        virtual void CActorParam_UnkVirtualFunc142(); //0x2CC
        virtual void CActorParam_UnkVirtualFunc143(); //0x2D0
        virtual void CActorParam_UnkVirtualFunc144(); //0x2D4
        virtual void CActorParam_UnkVirtualFunc145(); //0x2D8
        virtual void CActorParam_UnkVirtualFunc146(); //0x2DC
        virtual void CActorParam_UnkVirtualFunc147(); //0x2E0
        virtual void CActorParam_UnkVirtualFunc148(); //0x2E4
        virtual void* CActorParam_UnkVirtualFunc149(); //0x2E8
        virtual void* CActorParam_UnkVirtualFunc150(); //0x2EC
        virtual void CActorParam_UnkVirtualFunc151(); //0x2F0
        virtual void* CActorParam_UnkVirtualFunc152(); //0x2F4
        virtual void CActorParam_UnkVirtualFunc153(int arg); //0x2F8
        virtual void CActorParam_UnkVirtualFunc154(); //0x2FC
        virtual void CActorParam_UnkVirtualFunc155(); //0x300
        virtual void CActorParam_UnkVirtualFunc156(); //0x304
        virtual int CActorParam_UnkVirtualFunc157(); //0x308
        virtual void CActorParam_UnkVirtualFunc158(); //0x30C
        virtual void CActorParam_UnkVirtualFunc159(); //0x310
        virtual void CActorParam_UnkVirtualFunc160(); //0x314
        virtual void CActorParam_UnkVirtualFunc161(); //0x318
        virtual void CActorParam_UnkVirtualFunc162(); //0x31C
        virtual void CActorParam_UnkVirtualFunc163(); //0x320
        virtual void CActorParam_UnkVirtualFunc164(); //0x324
        virtual void* CActorParam_UnkVirtualFunc165(); //0x328
        virtual void CActorParam_UnkVirtualFunc166(); //0x32C
        virtual void CActorParam_UnkVirtualFunc167(); //0x330
        virtual void CActorParam_UnkVirtualFunc168(); //0x334
        virtual void CActorParam_UnkVirtualFunc169(); //0x338
        virtual void CActorParam_UnkVirtualFunc170(); //0x33C
        virtual void CActorParam_UnkVirtualFunc171(); //0x340
        virtual void CActorParam_UnkVirtualFunc172(); //0x344
        virtual void CActorParam_UnkVirtualFunc173(); //0x348
        virtual void CActorParam_UnkVirtualFunc174(); //0x34C
        virtual void CActorParam_UnkVirtualFunc175(float dt); //0x350
        virtual void CActorParam_UnkVirtualFunc176(float f1); //0x354
        virtual void CActorParam_UnkVirtualFunc177(float dt); //0x358
        virtual int CActorParam_UnkVirtualFunc178(); //0x35C
        virtual void CActorParam_UnkVirtualFunc179(); //0x360
        virtual void CActorParam_UnkVirtualFunc180(); //0x364
        virtual void CActorParam_UnkVirtualFunc181(); //0x368
    #pragma endregion

        UNKTYPE* unk15DC;
        UnkClass_CActorParam15E0* unk15E0;
        u32 unk15E4;
        float unk15E8;
        u32 unk15EC;
        u32 unk15F0;
        u8 unk15F4[4];        // 0x15F4
        float unk15F8;         // 0x15F8
        float unk15FC;
        u32 unk1600;
        u32 unk1604;
        u32 unk1608;
        u16 unk160C;
        u16 unk160E;
        float unk1610;
        u16 unk1614;
        u16 unk1616;
        float unk1618;
        u32 unk161C;
        float unk1620;
        float unk1624;
        u8 unk1628;
        u8 unk1629;
        u8 unk162A;
        u8 unk162B;
        u8 unk162C;
        float unk1630;
        u32 unk1634;
        u32 unk1638;
        u32 unk163C;
        u32 unk1640;
        u32 unk1644;
        u16 unk1648;
        u16 unk164A;
        u16 unk164C;
        u8 unk164E[2];
        CActorParam_UnkStruct3 unk1650;
        CActorParam_UnkStruct3 unk16C8;
        CActorParam_UnkStruct4 unk1740;
        CActorParam_UnkStruct4 unk1792;
        CActorParam_UnkStruct3 unk17E4;
        CActorParam_UnkStruct3 unk185C;
        CActorParam_UnkStruct4 unk18D4;
        u8 unk1926[2]; //filler?
        CActorParam_UnkStruct5 unk1928[8];
        CArtsSet mArtsSet; //0x19E8
        u8 unk2740[0xC];
        CAttackSet mAttackSet; //0x274C
        u32 unk2A80; //probably not here
        CActorParam_UnkStruct1 unk2A84[10];
        CActorParam_UnkStruct1 unk31DC;
        CActorParam_UnkStruct1 unk3298;
        u8 unk3354;
        u8 unk3355[3]; //padding?
        u16 unk3358;
        u16 unk335A;
        u8 unk335C[5];
        u8 unk3361[3]; //padding?
        float unk3364;
        float unk3368;
        u32 unk336C;
        u32 unk3370;
        u32 unk3374;
        u8 unk3378[4];
        float unk337C;
    CActorParam();
    void CBattleState_UnkVirtualFunc18();
    void CBattleState_UnkVirtualFunc17();
    int CBattleState_UnkVirtualFunc3();
    void CBattleState_UnkVirtualFunc2();
    };

inline u32 cf::CActorParam::CActorParam_UnkVirtualFunc19() { return unk15EC; }
inline u32 cf::CActorParam::CActorParam_UnkVirtualFunc29() { return *(u32*)&unk17E4; }
inline u32* cf::CActorParam::CActorParam_UnkVirtualFunc113() { return &unk161C; }
inline bool cf::CActorParam::CActorParam_UnkVirtualFunc115() { return !!unk1628; }
inline float* cf::CActorParam::CActorParam_UnkVirtualFunc117() { return &unk1620; }
inline float* cf::CActorParam::CActorParam_UnkVirtualFunc119() { return &unk1624; }
inline void* cf::CActorParam::CActorParam_UnkVirtualFunc122() { return &mArtsSet; }
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv();

#pragma once

#include <types.h>
#include "kyoshin/cf/object/CDebugState.hpp"
#include "kyoshin/cf/object/CBattleState.hpp"
#include "kyoshin/cf/object/CActorState.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include <cstring>

// `const` so MWCC treats the sdata2 pool load as a constant and hoists it
// above the frame stores (MWCC_CASES section extern-const-float-hoist); plain
// `extern float` leaves the lfs after the GPR saves (4-byte shift).
extern const float lbl_eu_806677E4; // sdata2: default gauge value (CActorParam_resetGaugeBattleEntry)
extern const float lbl_eu_806677E0; // sdata2: gauge default (CActorParam_resetGaugeEntry/resetAllGaugeEntries)
extern void* lbl_eu_806640DC; // .sbss arts bdat file pointer (CActorParam_getLevelExp)
extern char lbl_eu_80503438[]; // rodata column-name string block (CActorParam_getLevelExp)
extern double lbl_eu_80667848; // sdata2: +0.5 rounding constant (CActorParam_findMaxGaugeEntry/findMinGaugeEntry)
extern double lbl_eu_80667850; // sdata2: -0.5 rounding constant (CActorParam_findMaxGaugeEntry/findMinGaugeEntry)
extern const float lbl_eu_806677E8; // sdata2: gauge lower clamp (CActorParam_updateHateEntries)
extern const float lbl_eu_80667864; // sdata2: gauge upper clamp (CActorParam_updateHateEntries)
extern const double lbl_eu_806677F8; // sdata2: 0x4330000080000000 u32->double magic (CActorParam_applyStatusHeal)
extern float lbl_eu_80667830; // sdata2: scale constant (CActorParam_applyStatusHeal)
extern const double lbl_eu_806677F0; // sdata2: u8/u16->double magic 0x4330000000000000 (Func10/159)
extern const float lbl_eu_80667818; // sdata2: arts-magnitude scale (Func10)
extern const float lbl_eu_806677EC; // sdata2: 30.0f gauge divisor (Func140 / func_801765A4; const so it hoists above the frame stores)
extern const float lbl_eu_80667868; // sdata2: SP/exp gain scale (CActorParam_applyEventStatus)
extern const float lbl_eu_8066786C; // sdata2: arts-gauge threshold (CActorParam_applyEventStatus)
extern const float lbl_eu_80667800; // sdata2: 2.0f arts-slot default (Func140; const so the load is scheduled before the unk0 store)
extern const float lbl_eu_80667804; // sdata2: 3.0f arts-slot default (Func140; const so MWCC loads it once for the two stores)
// lbl_eu_8066A1F8 (pi) is declared in CfObjectEnumList.hpp
// CActorParam_updateStatusEntries constants (const so MWCC hoists each into a
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
extern const float lbl_eu_80667858; // sdata2: 0.7f (unreferenced; kept by the TU pool anchor)
extern const float lbl_eu_8066785C; // sdata2: 0.001f (unreferenced; kept by the TU pool anchor)
extern const float lbl_eu_80667860; // sdata2: 9999.0f (unreferenced; kept by the TU pool anchor)

namespace cf {

    // Per-actor status/parameter table (the +0x3534 subobject of the
    // func_8009EC9C character record; CfObjectPc.cpp writes it to +0x15E0).
    // ID-keyed parameter entries + bitmap/counters, read through the
    // code_8025FB10 accessor family (func_80260264 / func_8026178C /
    // func_80260A6C). UnkClass_CActorParam15E0 is kept as a compat alias:
    // CfObjectPc.hpp / CfGameManager.cpp spell the old name (other owners).
    class CActorParamStatusTable;
    typedef CActorParamStatusTable UnkClass_CActorParam15E0;

    //size: 0x88 (retail embeds vt at +0x84; SubObj fields mapped)
    struct CActorParam_UnkStruct2 {
        u8 pad00[0x28];
        u16 field_28;
        u8 pad2A[2];
        f32 field_2C;
        u8 pad30[4];
        s16 field_34;
        u8 pad36[6];
        u16 field_3C;
        u8 pad3E[2];
        u16 unk40;
        u8 pad42[2];
        u8 field_44;
        u8 pad45[3];
        u16 field_48;
        s16 field_4A;
        s16 field_4C;
        u8 pad4E[2];
        f32 field_50;
        f32 field_54;
        u8 pad58[0x5C - 0x58];
        u16 field_5C;
        u16 field_5E;
        f32 field_60;
        u16 field_64;
        u8 field_66;
        u8 field_67;
        s16 field_68;
        s16 field_6A;
        u8 pad6C[2];
        u8 field_6E;
        u8 field_6F;
        u8 pad70[0x78 - 0x70];
        u32 unk78;
        u8 pad7C[4];
        f32 field_80;
        void* field_84;
    };

    //might be fake?
    struct CActorParam_UnkStruct6 {
        CActorParam_UnkStruct6(){
            unk0 = 0;
        }
    
        u8 unk0;
    };

    struct CActorParam_Bitflags {
        u32 flags;
    };

    //size: 0xBC
    struct CActorParam_UnkStruct1 {
        union {
            struct {
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
            };
            struct {
                u32 w00;
                u32 w04;
                u64 q08;
                u64 q10;
                u64 q18;
                u64 q20;
                u64 q28;
                u64 q30;
                u64 q38;
                u64 q40;
                u32 w48;
                u32 w4C;
                u32 w50;
                float f54;
                float f58;
                float f5C;
                float f60;
                float f64;
                float f68;
                float f6C;
                s16 s70;
                s16 s72;
                u64 q74;
                u32 w7C;
                u16 u80;
                u16 pad82;
                u64 q84;
                u64 q8C;
                u64 q94;
                u64 q9C;
                u64 qA4;
                u64 qAC;
                u32 wB4;
                u32 wB8;
            };
        };
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

    // Absolute field offsets in this TU are spelled raw at the use site
    // (*(T*)((u8*)self + 0xOFF)) with the retail type, so each access folds
    // base+displacement with no extra saved reg. Member access is only used
    // where the header layout matches retail (through unk1928/unk2A80); the
    // tail past the arts set lives +0x34 off (UnkStruct1 aligns to 0xC0),
    // so tail fields stay absolute. The two Views below are shared with
    // CfObjectEne.cpp and stay defined here for that TU.

    // Absolute-offset view of the 8-entry CActorParam_UnkStruct5 array at
    // 0x1928. Shared with CfObjectEne.cpp (its Func141/143/144/1928-block
    // helpers). This TU now uses self->unk1928 directly.
    struct CActorParamUnk1928View {
        u8 _pad[0x1928];
        CActorParam_UnkStruct5 entries[8];
    };

    // Absolute-offset view of the status flag byte at 0x3354. Shared with
    // CfObjectEne.cpp (its getStatsSlot override). This TU uses raw stores.
    struct CActorParam3354View {
        u8 _pad[0x3354];
        u8 field_0x3354;
    };

    // Row returned by func_8009D7E4 (the arts-stats sub-row at +0x1C of the
    // char-data object); consumed by CActorParam_applyArtsStats.
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

    // Char-data object passed to CActorParam_applyArtsStats (retail ABI r4)
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
    // CActorParam_applyStatusHeal.
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

    // Argument passed through the CActorParam_applyEventStatus slot (retail
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

    // NOTE: the old foreign-actor view for Func12's target reads (flags at
    // 0x3F00, actor id at 0x3F10, state id at 0x3F28) was folded into raw
    // absolute loads at the call sites; the actor's owning class is out of
    // scope for this header.

    // Argument passed through the CActorParam_applyStatusHeal slot (retail
    // ABI r4): u32 status word at +0x74 (bit 0x100 gate).
    struct CActorParam11Arg {
        u8 _pad[0x74];
        u32 field_0x74;
    };

    // Target-object view for CActorParam_addHate (retail ABI r4):
    // CBattleState subobject at +8 (probed with func_80148778), actor id at
    // +0x3F10.
    struct CActorParam140Target {
        u8 _pad8[0x8];
        u8 cbstate[0x3F10 - 0x8]; // +8: CBattleState subobject
        u32 field_0x3F10; // 0x3F10: actor id
    };

    // Argument passed through the CActorParam_applyTargetEvent slot (retail
    // ABI r4): target-object pointer at +0x50, status word at +0x74 (bit 0
    // gate).
    struct CActorParam10Arg {
        u8 _pad[0x50];
        void* field_0x50; // 0x50: CActorParam10Target*
        u8 _pad54[0x74 - 0x54];
        u32 field_0x74; // 0x74: status word (bit 0 gate)
    };

    // Object reachable through CActorParam10Arg::field_0x50. The embedded
    // cf::CAttackParam arts-row object (vptr at +0x84 via the
    // CAttackParamPrefix base, slot 0xC is CAttackParam_getArtsFlagByte) is
    // dispatched via the real base virtual at the call site, so no member
    // is declared for the tail past +0x78.
    struct CActorParam10Target {
        u8 _pad[0x44];
        u8 field_0x44; // 0x44: mode byte (1 skips the Func133 gate)
        u8 _pad45[0x78 - 0x45];
        u32 field_0x78; // 0x78: flags (0x40000000 / 0x4000 / 0x20)
    };

    //size: 0x3384
    class CActorParam : public CActorState, public CBattleState, public CDebugState {
    public:
        CActorParam(UNKTYPE* r4, UNKTYPE* r5);
    #pragma region vtable
        virtual const char* CActorParam_getActorName();   //0x98
        virtual void* CActorParam_getActor();   //0x9C (base returns NULL; actors return self)
        virtual void CActorParam_UnkVirtualFunc3();   //0xA0
        virtual void CActorParam_resetArtsStatus(void* arts);   //0xA4
        virtual void CActorParam_commitArtsStatus(int flag);   //0xA8
        virtual void CActorParam_UnkVirtualFunc6(int val);   //0xAC
        virtual void CActorParam_setupActorState();   //0xB0
        virtual void CActorParam_resetArtsMatrix();   //0xB4
        virtual void CActorParam_refreshBattleStatus();   //0xB8
        virtual void CActorParam_applyTargetEvent(CActorParam10Arg* arg);  //0xBC
        virtual void CActorParam_applyStatusHeal(CActorParam11Arg* arg);  //0xC0
        virtual void CActorParam_applyEventStatus(CActorParam12Arg* arg);  //0xC4
        virtual void CActorParam_resetGaugesAndRefresh();  //0xC8
        virtual void CActorParam_setStatByte(u8 val);  //0xCC
        virtual void CActorParam_UnkVirtualFunc15();  //0xD0
        virtual void CActorParam_setStatScale(float val);  //0xD4
        virtual void CActorParam_UnkVirtualFunc17();  //0xD8
        virtual void CActorParam_setActorType(u32 val);  //0xDC (was UnkVirtualFunc18)
        virtual u32 CActorParam_getActorType();  //0xE0
        virtual void CActorParam_setEnemyType(u32 val);  //0xE4 (was UnkVirtualFunc20)
        virtual void CActorParam_setScale(float val);  //0xE8
        virtual void CActorParam_UnkVirtualFunc22();  //0xEC
        virtual float CActorParam_getScale();  //0xF0
        virtual void CActorParam_UnkVirtualFunc24();  //0xF4
        virtual void CActorParam_UnkVirtualFunc25();  //0xF8
        virtual u32 CActorParam_getArtsLevel();  //0xFC
        virtual void CActorParam_UnkVirtualFunc27();  //0x100
        virtual void CActorParam_UnkVirtualFunc28();  //0x104
        virtual u32 CActorParam_getActorLevel();  //0x108
        virtual void CActorParam_UnkVirtualFunc30();  //0x10C
        virtual void CActorParam_UnkVirtualFunc31();  //0x110
        virtual void CActorParam_UnkVirtualFunc32();  //0x114
        virtual void CActorParam_setHp(float val);  //0x118
        virtual void CActorParam_addHp(float value);  //0x11C
        virtual void CActorParam_applyDamage(float value, int a, int b, int c);  //0x120
        virtual void CActorParam_UnkVirtualFunc36();  //0x124
        virtual float CActorParam_getHp();  //0x128
        virtual float CActorParam_getDamageScale();  //0x12C
        virtual float CActorParam_UnkVirtualFunc39();  //0x130
        virtual void CActorParam_applyStoredHp();  //0x134
        virtual void CActorParam_UnkVirtualFunc41(float val);  //0x138
        virtual void CActorParam_UnkVirtualFunc42(float val);  //0x13C
        virtual float CActorParam_UnkVirtualFunc43();  //0x140
        virtual void CActorParam_UnkVirtualFunc44();  //0x144
        virtual void CActorParam_UnkVirtualFunc45();  //0x148
        virtual void CActorParam_applyStoredHitCharge();  //0x14C
        virtual void CActorParam_setTensionPoints(float val);  //0x150
        virtual void CActorParam_addArtsGauge(float delta);  //0x154
        virtual float CActorParam_getArtsGauge();  //0x158
        virtual float CActorParam_UnkVirtualFunc50();  //0x15C
        virtual float CActorParam_UnkVirtualFunc51();  //0x160
        virtual void CActorParam_applyStoredArtsGauge();  //0x164
        virtual void CActorParam_UnkVirtualFunc53(int val);  //0x168
        virtual void CActorParam_addGauge(int delta);  //0x16C
        virtual void CActorParam_setGaugeFlagA(u16 val);  //0x170
        // int (not s16): Unk179 passes the getter result to Unk54 via mr r4,r3;
        // s16 forces MWCC to insert extsh and breaks the 0x94 match.
        virtual int CActorParam_getGauge();  //0x174
        virtual int CActorParam_getGaugeMax();  //0x178
        virtual void CActorParam_clearGauge();  //0x17C
        virtual void CActorParam_UnkVirtualFunc59(int val);  //0x180
        virtual void CActorParam_addSecondGauge(int delta);  //0x184
        virtual void CActorParam_setGaugeFlagB(u16 val);  //0x188
        virtual int CActorParam_getSecondGauge();  //0x18C
        virtual int CActorParam_getSecondGaugeMax();  //0x190
        virtual void CActorParam_clearSecondGauge();  //0x194
        virtual void CActorParam_setGaugeRateA(float val);  //0x198
        virtual float CActorParam_UnkVirtualFunc66();  //0x19C
        virtual void CActorParam_resetGaugeBattleEntry();  //0x1A0
        virtual void CActorParam_setGaugeRateB(float val);  //0x1A4
        virtual float CActorParam_UnkVirtualFunc69();  //0x1A8
        virtual void CActorParam_resetSecondGaugeBattleEntry();  //0x1AC
        virtual void CActorParam_UnkVirtualFunc71();  //0x1B0
        virtual float CActorParam_UnkVirtualFunc72();  //0x1B4
        virtual void CActorParam_UnkVirtualFunc73();  //0x1B8
        virtual void CActorParam_setMoveRate(float val);  //0x1BC
        // Retail func_8003CC0C leaves a booleanized int in r4 into the bctr
        // (ocUnit.cpp); the CfObjectEne Fv impl ignores it.
        virtual void CActorParam_UnkVirtualFunc75(int flag);  //0x1C0
        virtual void* CActorParam_getMoveRate();  //0x1C4
        virtual void CActorParam_UnkVirtualFunc77();  //0x1C8
        virtual void CActorParam_UnkVirtualFunc78();  //0x1CC
        virtual void CActorParam_UnkVirtualFunc79();  //0x1D0
        virtual void CActorParam_UnkVirtualFunc80();  //0x1D4
        virtual void CActorParam_setSpentCurrency(u32 val);  //0x1D8
        virtual void CActorParam_addSpentCurrency(u32 val);  //0x1DC
        virtual void CActorParam_addAccumCurrency(u32 addend);  //0x1E0
virtual u32 CActorParam_getTotalCurrency();  //0x1E4
        virtual u32 CActorParam_getSpentCurrency();  //0x1E8
        virtual int CActorParam_getCurrencyBalance();  //0x1EC
        virtual u32 CActorParam_getLevelExp();  //0x1F0
        virtual void CActorParam_UnkVirtualFunc88(u32 a, u32 b, u32 c);  //0x1F4
        virtual void CActorParam_setSecondCurrency(u32 val);  //0x1F8 (was UnkVirtualFunc89)
        virtual void CActorParam_addSecondCurrency(u32 val);  //0x1FC
        virtual u32 CActorParam_getSecondCurrency();  //0x200 (was UnkVirtualFunc91)
        virtual void CActorParam_UnkVirtualFunc92(const void* src);  //0x204
        virtual void CActorParam_UnkVirtualFunc93();  //0x208
        virtual void* CActorParam_getArtsDataBlock();  //0x20C
        virtual void* CActorParam_getStatusSyncBlock();  //0x210 (retail impl returns the +0x16C8 block)
        virtual void* CActorParam_getArtsSyncBlock();  //0x214 (was UnkVirtualFunc96; retail impl returns the +0x1740 block)
        virtual void CActorParam_UnkVirtualFunc97();  //0x218
        virtual void CActorParam_UnkVirtualFunc98(const void* src);  //0x21C
        virtual void CActorParam_UnkVirtualFunc99();  //0x220
        virtual void* CActorParam_getBattleParams(); //0x224
virtual void* CActorParam_getBaseStats(); //0x228 (retail impl returns the +0x185c block)
virtual void* CActorParam_getBonusStats(); //0x22C (retail impl returns the +0x18d4 block)
        virtual void CActorParam_refreshBattleAction(); //0x230 (was UnkVirtualFunc103)
        virtual void CActorParam_UnkVirtualFunc104(); //0x234
        virtual void CActorParam_UnkVirtualFunc105(); //0x238
        virtual void CActorParam_UnkVirtualFunc106(); //0x23C
        virtual void CActorParam_UnkVirtualFunc107(); //0x240
        virtual void CActorParam_UnkVirtualFunc108(); //0x244
        virtual u32 CActorParam_UnkVirtualFunc109(); //0x248
        virtual u32 CActorParam_UnkVirtualFunc110(); //0x24C
        virtual void CActorParam_UnkVirtualFunc111(); //0x250
        virtual void CActorParam_setParam161C(u32 val); //0x254 (was UnkVirtualFunc112)
        virtual u32* CActorParam_UnkVirtualFunc113(); //0x258
        virtual void CActorParam_setFlag1628(u8 val); //0x25C (was UnkVirtualFunc114)
        virtual bool CActorParam_UnkVirtualFunc115(); //0x260
        virtual void CActorParam_setGaugeFloat(float val); //0x264
        virtual float* CActorParam_UnkVirtualFunc117(); //0x268
        virtual void CActorParam_setGauge1624(float val); //0x26C (was UnkVirtualFunc118)
        virtual float* CActorParam_UnkVirtualFunc119(); //0x270
        virtual void CActorParam_copyArtsSetBlock(); //0x274 (was UnkVirtualFunc120)
        virtual void* CActorParam_getArtsSetBlock(); //0x278 (was UnkVirtualFunc121; +0x19e8 block)
        virtual void* CActorParam_getArtsSet(); //0x27C
        virtual void CActorParam_copyAtkSetBlock(); //0x280 (was UnkVirtualFunc123)
        virtual void* CActorParam_getAtkSetBlock(); //0x284 (was UnkVirtualFunc124; +0x2740 block)
        virtual void* CActorParam_getArtsSlotIds(); //0x288
        virtual void* CActorParam_getCopyParam(); //0x28C
        virtual CActorParamStatusTable* CActorParam_getStatusTable(); //0x290
        virtual void CActorParam_UnkVirtualFunc128(); //0x294
        virtual CActorParam_UnkStruct1* CActorParam_getBattleStats(); //0x298
        virtual void* CActorParam_UnkVirtualFunc130(int arg); //0x29C
        virtual void* CActorParam_UnkVirtualFunc131(); //0x2A0
        virtual void* CActorParam_getMoveRecord(); //0x2A4
        virtual u8 CActorParam_getStatsSlot(); //0x2A8
        virtual void CActorParam_resetStatsSlot(); //0x2AC (was UnkVirtualFunc134)
        virtual void CActorParam_reinitBattleStats(); //0x2B0
        virtual void CActorParam_initBattleStats(); //0x2B4
        virtual void CActorParam_UnkVirtualFunc137(); //0x2B8
        virtual bool CActorParam_isBattleLocked(); //0x2BC
        virtual void CActorParam_UnkVirtualFunc139(); //0x2C0
        virtual void CActorParam_addHate(void* arg, float x, float y, float z); //0x2C4
        virtual void CActorParam_resetGaugeEntry(void* arg); //0x2C8
        virtual void CActorParam_resetAllGaugeEntries(); //0x2CC
        virtual void CActorParam_UnkVirtualFunc143(); //0x2D0
        virtual void* CActorParam_findGaugeEntry(unsigned int arg); //0x2D4
        virtual void CActorParam_UnkVirtualFunc145(); //0x2D8
        virtual void CActorParam_UnkVirtualFunc146(); //0x2DC
        virtual void CActorParam_UnkVirtualFunc147(); //0x2E0
        virtual void CActorParam_resetGaugeEntries(); //0x2E4 (was UnkVirtualFunc148)
        virtual void* CActorParam_findMaxGaugeEntry(); //0x2E8
        virtual void* CActorParam_findMinGaugeEntry(); //0x2EC
        virtual void CActorParam_UnkVirtualFunc151(); //0x2F0
        virtual void* CActorParam_getTensionStatus(); //0x2F4
        virtual void CActorParam_accumulateTension(int arg); //0x2F8
        virtual void CActorParam_raiseTensionValue(int arg); //0x2FC
        virtual void CActorParam_UnkVirtualFunc155(); //0x300
        virtual void CActorParam_resetTensionState(int arg); //0x304
        virtual int CActorParam_getStatusCount(); //0x308
        virtual void CActorParam_UnkVirtualFunc158(); //0x30C
        virtual void CActorParam_UnkVirtualFunc159(); //0x310
        virtual void CActorParam_initStatusCounter(); //0x314
        virtual void CActorParam_UnkVirtualFunc161(); //0x318
        virtual void CActorParam_UnkVirtualFunc162(); //0x31C
        virtual void* CActorParam_UnkVirtualFunc163(); //0x320
        virtual void* CActorParam_getActorModeFlags(); //0x324
        virtual void* CActorParam_getBattleHitFlags(); //0x328
        virtual void CActorParam_UnkVirtualFunc166(); //0x32C
        virtual void CActorParam_UnkVirtualFunc167(); //0x330
        virtual void CActorParam_setHitRange(float val); //0x334 (was UnkVirtualFunc168)
        virtual void CActorParam_UnkVirtualFunc169(); //0x338
        virtual float CActorParam_UnkVirtualFunc170(); //0x33C
        virtual float CActorParam_UnkVirtualFunc171(); //0x340
        virtual void CActorParam_UnkVirtualFunc172(); //0x344
        virtual void CActorParam_UnkVirtualFunc173(); //0x348
        virtual void CActorParam_applyArtsStats(); //0x34C
        virtual void CActorParam_updateHateEntries(float dt); //0x350
        virtual void CActorParam_decayArtsMatrix(float f1); //0x354
        virtual void CActorParam_updateStatusEntries(float dt); //0x358
        virtual int CActorParam_UnkVirtualFunc178(); //0x35C
        virtual void CActorParam_UnkVirtualFunc179(); //0x360
        virtual void CActorParam_UnkVirtualFunc180(); //0x364
        virtual void CActorParam_UnkVirtualFunc181(); //0x368
    #pragma endregion

    // Wave-36/59/62: former UnkVirtual caller aliases that are now real virtuals
    // (setStatByte/setStatScale/getArtsLevel/setGaugeFlagA/B/setGaugeRateA/B/
    // setMoveRate/getMoveRate/setSpentCurrency/addSpentCurrency/
    // addAccumCurrency/getSpentCurrency/addSecondCurrency/setGaugeFloat/
    // getArtsSlotIds/getCopyParam/getTensionStatus;
    // Wave-62 also flipped setActorType/setEnemyType/refreshBattleAction/
    // setParam161C/setFlag1628/setGauge1624/resetStatsSlot/resetGaugeEntries/
    // setHitRange). Remaining aliases below still forward to Unk-named
    // virtuals whose overrides live in out-of-scope TUs (or whose hand-built
    // CREvtModel spellings are not yet promoted).
    void CActorParam_resetActorState(int val) { CActorParam_UnkVirtualFunc6(val); } //0xAC
    void CActorParam_applyCurrencyChange(u32 a, u32 b, u32 c) { CActorParam_UnkVirtualFunc88(a, b, c); } //0x1F4
    void CActorParam_UnkVirtualFunc89(u32 val) { CActorParam_setSecondCurrency(val); } //0x1F8: legacy Unk spelling
    u32 CActorParam_UnkVirtualFunc91() { return CActorParam_getSecondCurrency(); } //0x200: legacy Unk spelling
    void CActorParam_rearmArtsList() { CActorParam_UnkVirtualFunc166(); } //0x32C
    void CActorParam_rearmAttackList() { CActorParam_UnkVirtualFunc167(); } //0x330: attack-set loader (getAtkParam records)
    void CActorParam_UnkVirtualFunc18(u32 val) { CActorParam_setActorType(val); } //0xDC: legacy Unk spelling
    void CActorParam_UnkVirtualFunc20(u32 val) { CActorParam_setEnemyType(val); } //0xE4: legacy Unk spelling
    void CActorParam_UnkVirtualFunc168(float val) { CActorParam_setHitRange(val); } //0x334: legacy Unk spelling
    // Wave-64: Unk primary flipped for 0x274/0x278/0x280/0x284; Unk aliases
    // keep CREvtModel Fv spellings. getArtsSetBlock call sites (CMenuArtsSelect)
    // now hit the real virtual.
    void CActorParam_UnkVirtualFunc120() { CActorParam_copyArtsSetBlock(); } //0x274
    void* CActorParam_UnkVirtualFunc121() { return CActorParam_getArtsSetBlock(); } //0x278
    void CActorParam_UnkVirtualFunc123() { CActorParam_copyAtkSetBlock(); } //0x280
    void* CActorParam_UnkVirtualFunc124() { return CActorParam_getAtkSetBlock(); } //0x284
    float CActorParam_getHitRate() { return CActorParam_UnkVirtualFunc39(); } //0x130: ratio at +0x17E8/+0x17F4, hit-chance gate
    float CActorParam_getHitRange() { return CActorParam_UnkVirtualFunc171(); } //0x340: range at +0x1630, position tolerance
    void* CActorParam_getStateFlags() { return CActorParam_UnkVirtualFunc163(); } //0x320: flags at +0x1648, u16 bit test
    float CActorParam_getArtsScale() { return CActorParam_UnkVirtualFunc50(); } //0x15C: +0x17FC arts-scale (/100 at call sites)
    void CActorParam_pushBattleStats() { CActorParam_UnkVirtualFunc137(); } //0x2B8: advance slot idx, inherit previous
    void* CActorParam_getStatsEntry(int index) { return CActorParam_UnkVirtualFunc130(index); } //0x29C: indexed entry into +0x2A84 array
    u32 CActorParam_getGradeByte() { return CActorParam_UnkVirtualFunc110(); } //0x24C: byte at +0x183B, rank gate
    void* CActorParam_getStagingStats() { return CActorParam_UnkVirtualFunc131(); } //0x2A0: +0x31DC work slot, snapshot dest
    void CActorParam_UnkVirtualFunc134() { CActorParam_resetStatsSlot(); } //0x2AC: legacy Unk spelling
    void CActorParam_UnkVirtualFunc148() { CActorParam_resetGaugeEntries(); } //0x2E4: legacy Unk spelling
    float CActorParam_getHitCenter() { return CActorParam_UnkVirtualFunc170(); } //0x33C: pi - hit range, window center
    void CActorParam_addHitCharge(float val) { CActorParam_UnkVirtualFunc42(val); } //0x13C: clamped add to +0x17EC charge
    u32 CActorParam_getRankByte() { return CActorParam_UnkVirtualFunc109(); } //0x248: byte at +0x183A, rank gate
    float CActorParam_getGaugeRateA() { return CActorParam_UnkVirtualFunc66(); } //0x19C: rate at +0x1610, pairs with setGaugeRateA (Func65)
    float CActorParam_getGaugeRateB() { return CActorParam_UnkVirtualFunc69(); } //0x1A8: rate at +0x1618, pairs with setGaugeRateB (Func68)
    void CActorParam_setGauge(int val) { CActorParam_UnkVirtualFunc53(val); } //0x168: store gauge current at +0x160C (base impl in CfObjectEne.cpp)
    void CActorParam_setSecondGauge(int val) { CActorParam_UnkVirtualFunc59(val); } //0x180: store second-gauge current at +0x1614 (base impl in CfObjectEne.cpp)
    void* CActorParam_UnkVirtualFunc96() { return CActorParam_getArtsSyncBlock(); } //0x214: legacy Unk spelling
    void CActorParam_setHitCharge(float val) { CActorParam_UnkVirtualFunc41(val); } //0x138: set at +0x17EC, pairs with addHitCharge (Func42)
    void CActorParam_UnkVirtualFunc103() { CActorParam_refreshBattleAction(); } //0x230: legacy Unk spelling
    // (getArtsSetBlock promoted to virtual +0x278 in Wave-64)
    float CActorParam_getArtsGaugeRatio() { return CActorParam_UnkVirtualFunc51(); } //0x160: arts gauge current/max (+0x17F0/+0x17FC), frame-progress fraction
    void CActorParam_copyArtsBlock1650(const void* src) { CActorParam_UnkVirtualFunc92(src); } //0x204: copies ParamCopyBlock to +0x1650 (base impl in CfObjectEne.cpp)
    void CActorParam_copyArtsBlock17E4(const void* src) { CActorParam_UnkVirtualFunc98(src); } //0x21C: copies ParamCopyBlock to +0x17E4 (base impl in CfObjectEne.cpp)
    int CActorParam_consumeCurrencyStep() { return CActorParam_UnkVirtualFunc178(); } //0x35C: drains one queued currency unit, 1 when acted (base returns 0)
    // Wave-62: Unk primaries flipped for 0x254/0x25C/0x26C; Unk aliases below.
    // Hand-built tables keep Unk Fv linker names (Ene forced-name bodies).
    void CActorParam_UnkVirtualFunc112(u32 val) { CActorParam_setParam161C(val); } //0x254
    void CActorParam_UnkVirtualFunc114(u8 val) { CActorParam_setFlag1628(val); } //0x25C
    void CActorParam_UnkVirtualFunc118(float val) { CActorParam_setGauge1624(val); } //0x26C
    // Wave-39 caller aliases (non-virtual, inline): behavior-derived names
    // for the enemy-mark slots used by CMenuEnemyState.cpp. 0x258 selects
    // the mark slot-type texture (1/2/3); 0x260 is the mark visibility flag
    // OR'd into the panel highlight bit. Same arity; MWCC inlines them into
    // the identical virtual dispatch.
    u32* CActorParam_getMarkKindPtr() { return CActorParam_UnkVirtualFunc113(); } //0x258
    bool CActorParam_isMarkVisible() { return CActorParam_UnkVirtualFunc115(); } //0x260
    // Wave-40 UnkVirtual mop-up alias (non-virtual, inline): same-slot
    // forwarder for the ocUnit func_8003CC0C call site (booleanized flag in
    // r4, ignored by the Ene impl which returns the +0x1828 status block).
    // The virtual keeps its Unk name (hand-built vtables spell the mangled
    // name). MWCC inlines the identical virtual dispatch.
    void CActorParam_setStatusSelect(int flag) { CActorParam_UnkVirtualFunc75(flag); } //0x1C0
    // Wave-41 UnkVirtual mop-up aliases (non-virtual, inline): same-slot
    // forwarders for the CAIAction.cpp call sites. The virtuals keep their
    // Unk names (hand-built vtables spell the mangled names). MWCC inlines
    // the identical virtual dispatch.
    float CActorParam_getHitCharge() { return CActorParam_UnkVirtualFunc43(); } //0x140: +0x17EC charge current, pairs with setHitCharge/addHitCharge
    float CActorParam_getArtsPower() { return CActorParam_UnkVirtualFunc72(); } //0x1B4: +0x1824 power reading, action-power probe
    // (All former Wave-45 same-slot aliases have been promoted to real
    // virtuals above; the CREvtModel.cpp hand-built vtable now spells the
    // behavior names.)

        UNKTYPE* unk15DC;
        CActorParamStatusTable* unk15E0;
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
    void CBattleState_UnkVirtualFunc17();
    };

inline u32 cf::CActorParam::CActorParam_getActorType() { return unk15EC; }
inline u32 cf::CActorParam::CActorParam_getActorLevel() { return *(u32*)&unk17E4; }
inline u32* cf::CActorParam::CActorParam_UnkVirtualFunc113() { return &unk161C; }
inline bool cf::CActorParam::CActorParam_UnkVirtualFunc115() { return !!unk1628; }
inline float* cf::CActorParam::CActorParam_UnkVirtualFunc117() { return &unk1620; }
inline float* cf::CActorParam::CActorParam_UnkVirtualFunc119() { return &unk1624; }
inline void* cf::CActorParam::CActorParam_getArtsSet() { return &mArtsSet; }
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 CActorParam_UnkVirtualFunc3__Q22cf11CActorParamFv();

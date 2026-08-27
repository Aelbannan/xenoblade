#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/chain/CChainTemp.hpp"
#include <nw4r/math/math_types.h>

namespace cf {
    //size: 0x8
    class CChainTimer {
    public:
        s16 unk0;      // 0x0 - read/written signed by chain code (retail lha/sth)
        u8 unk2[2];
        //0x4: vtable
        virtual ~CChainTimer(){}
    };

    // Virtual-dispatch shim for the embedded voice sub-object at +0x3E9C of
    // the chain battle objects. Declared virtual N sits at vtable byte offset
    // (N+2)*4 (-RTTI on). Slots 0x4C (index 17, arts index), 0x50 (index 18)
    // and 0xAC (index 41) are used by the chain code. Never instantiated, so
    // no vtable emits and the declared (non-pure) virtuals need no bodies.
    class CChainVoiceSub {
    public:
        virtual void v00(); virtual void v01(); virtual void v02();
        virtual void v03(); virtual void v04(); virtual void v05();
        virtual void v06(); virtual void v07(); virtual void v08();
        virtual void v09(); virtual void v10(); virtual void v11();
        virtual void v12(); virtual void v13(); virtual void v14();
        virtual void v15(); virtual void v16(); virtual u32 v17(); // index 17 -> vtable offset 0x4C (arts index, cf. CChainVoiceSub17)
        virtual void v18(u32 v); // index 18 -> vtable offset 0x50
        virtual void v19(); virtual void v20(); virtual void v21();
        virtual void v22(); virtual void v23(); virtual void v24();
        virtual void v25(); virtual void v26(); virtual void v27();
        virtual void v28(); virtual void v29(); virtual void v30();
        virtual void v31(); virtual void v32(); virtual void v33();
        virtual void v34(); virtual void v35(); virtual void v36();
        virtual void v37(); virtual void v38(); virtual void v39();
        virtual void v40();
        virtual nw4r::math::VEC3* v41(); // index 41 -> vtable offset 0xAC
    };

    // u16 arts id at +0x48 of the holder behind CChainAction::field_50.
    class CChainArtsId {
    public:
        u8 _pad0[0x48];
        u16 field_48;           // 0x48
    };

    // Voice-action object returned by the battle object's vtable slot 0x298.
    // Only the arts-id holder and gauge fields touched by func_8027D478 are
    // declared; the rest are opaque.
    class CChainGaugeAction {
    public:
        u8 _pad0[0x50];
        CChainArtsId* field_50; // 0x50 arts-id holder
        u8 _pad54[0x58 - 0x54]; // 0x54-0x57
        f32 field_58;           // 0x58 gauge
        f32 field_5C;           // 0x5C gauge
        f32 field_60;           // 0x60 gauge
        u8 _pad64[0x74 - 0x64];
        u32 field_74;           // 0x74 flags (bit 0x80 = hold gauge)
    };

    // Embedded sub-object at +0x08 of the chain battle object: its vtable
    // pointer sits at the sub-object's own start (retail `lwzu r12, 0x8(r3)`
    // virtual-dispatch form). Slots 0x14 (index 3) and 0x20 (index 6) are used
    // by the chain code (CChainSub8E). Never instantiated, so the declared
    // virtuals need no bodies.
    class CChainSub8 {
    public:
        virtual void s00(); virtual void s01(); virtual void s02();
        virtual void s03(u32 v); // index 3 -> vtable offset 0x14 (chain-voice enable, cf. CChainSub8E::e03)
        virtual void s04(); virtual void s05();
        virtual void s06(u32 v); // index 6 -> vtable offset 0x20
        virtual void s07();
    };

    // Minimal mirror of the battle-object sub-object at +0x04: its vtable
    // slot 0x30 returns a word holder (first word = actor id for
    // func_80174C98). Declared virtual N lands at vtable byte offset (N+2)*4
    // (kyoshin builds with -RTTI on).
    class CChainSub4 {
    public:
        virtual void f00() = 0;
        virtual void f01() = 0;
        virtual void f02() = 0;
        virtual void f03() = 0;
        virtual void f04() = 0;
        virtual void f05() = 0;
        virtual void f06(u32 v) = 0; // index 6 -> vtable offset 0x20
        virtual void f07() = 0;
        virtual void f08() = 0;
        virtual void f09() = 0;
        virtual void* f30() = 0; // index 10 -> vtable offset 0x30
    };

    // Word holder target of battleObj->field_3F60: u32 flags at +0x4EC.
    class CChainField3F60 {
    public:
        u8 _pad[0x4EC];
        u32 field_4EC; // 0x4EC
    };

    // Interface mirror of the chain battle object (CfImplEneBattleObj
    // layout). Never instantiated - only used to reach vtable slots and
    // fields at their exact offsets. Declared virtual N sits at vtable byte
    // offset (N+2)*4.
    class CChainBattleObj {
    public:
        virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0; virtual void v003() = 0;
        virtual void v004() = 0; virtual void v005() = 0; virtual void v006() = 0; virtual void v007() = 0;
        virtual void v008() = 0; virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
        virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0; virtual void v015() = 0;
        virtual void v016() = 0; virtual void v017() = 0; virtual void v018() = 0; virtual void v019() = 0;
        virtual void v020() = 0; virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
        virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0; virtual void v027() = 0;
        virtual void v028() = 0; virtual void v029() = 0; virtual void v030() = 0; virtual void v031() = 0;
        virtual void v032() = 0; virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
        virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0; virtual void v039() = 0;
        virtual void v040() = 0; virtual void v041() = 0; virtual void v042() = 0; virtual void v043() = 0;
        virtual void v044() = 0; virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
        virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0; virtual void v051() = 0;
        virtual void v052() = 0; virtual void v053() = 0; virtual void v054() = 0; virtual void v055() = 0;
        virtual void v056() = 0; virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
        virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0; virtual void v063() = 0;
        virtual void v064() = 0; virtual void v065() = 0; virtual void v066() = 0; virtual void v067() = 0;
        virtual void v068() = 0; virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
        virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0; virtual void v075() = 0;
        virtual void v076() = 0; virtual void v077() = 0; virtual void v078() = 0; virtual void v079() = 0;
        virtual void v080() = 0; virtual void v081() = 0;
        virtual void v082(float v) = 0; // index 82 -> vtable offset 0x150
        virtual void v083() = 0;
        virtual float v084() = 0; // index 84 -> vtable offset 0x158
        virtual float v085() = 0; // index 85 -> vtable offset 0x15c
        virtual void v086() = 0; virtual void v087() = 0; virtual void v088() = 0; virtual void v089() = 0;
        virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0; virtual void v093() = 0;
        virtual void v094() = 0; virtual void v095() = 0; virtual void v096() = 0; virtual void v097() = 0;
        virtual void v098() = 0; virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0;
        virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0; virtual void v105() = 0;
        virtual void v106() = 0; virtual void v107() = 0; virtual void v108() = 0; virtual void v109() = 0;
        virtual void v110() = 0; virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0;
        virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0; virtual void v117() = 0;
        virtual void v118() = 0; virtual void v119() = 0; virtual void v120() = 0; virtual void v121() = 0;
        virtual void v122() = 0; virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0;
        virtual void v126() = 0; virtual void v127() = 0; virtual void v128() = 0; virtual void v129() = 0;
        virtual void v130() = 0; virtual void v131() = 0; virtual void v132() = 0; virtual void v133() = 0;
        virtual void v134() = 0; virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
        virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0; virtual void v141() = 0;
        virtual void v142() = 0; virtual void v143() = 0; virtual void v144() = 0; virtual void v145() = 0;
        virtual void v146() = 0; virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0;
        virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0; virtual void v153() = 0;
        virtual void v154() = 0; virtual void v155() = 0; virtual void v156() = 0;
        virtual void* v157() = 0; // index 157 -> vtable offset 0x27c (arts set)
        virtual void v158() = 0; virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0;
        virtual int v162() = 0; // index 162 -> vtable offset 0x290 (delta gate, cf. CChainBattleObj750::v162)
        virtual void v163() = 0;
        virtual class CChainGaugeSub* v164() = 0; // index 164 -> vtable offset 0x298 (gauge sub-object)
        virtual void v165() = 0;
        virtual void v166() = 0; virtual class CChainCombo_ArtsCategoryHolder* v167() = 0; virtual int v168() = 0; virtual void v169() = 0; // 167->0x2A4 arts holder, 168->0x2A8 (cf. CChainBattleObj750)
        virtual void v170() = 0; virtual void v171() = 0; virtual void v172() = 0;
        virtual int v173() = 0;  // index 173 -> vtable offset 0x2bc
        virtual void v174() = 0; virtual void v175() = 0; virtual void v176() = 0; virtual void v177() = 0;
        virtual void v178() = 0; virtual void v179() = 0; virtual void v180() = 0; virtual void v181() = 0;
        virtual void v182() = 0; virtual void v183() = 0; virtual void v184() = 0; virtual void v185() = 0;
        virtual void v186() = 0; virtual void v187() = 0; virtual void v188() = 0; virtual void v189() = 0;
        virtual void v190() = 0; virtual void v191() = 0; virtual void v192() = 0; virtual void v193() = 0;
        virtual void v194() = 0; virtual void v195() = 0; virtual void v196() = 0; virtual void v197() = 0;
        virtual void v198() = 0; virtual void v199() = 0; virtual void v200() = 0; virtual void v201() = 0;
        virtual void v202() = 0; virtual void v203() = 0; virtual void v204() = 0; virtual void v205() = 0;
        virtual void v206() = 0; virtual void v207() = 0; virtual void v208() = 0; virtual void v209() = 0;
        virtual void v210() = 0; virtual void v211() = 0; virtual void v212() = 0; virtual void v213() = 0;
        virtual void v214() = 0; virtual void v215() = 0; virtual void v216() = 0; virtual void v217() = 0;
        virtual void v218() = 0; virtual void v219() = 0; virtual void v220() = 0; virtual void v221() = 0;
        virtual void v222() = 0; virtual void v223() = 0; virtual void v224() = 0; virtual void v225() = 0;
        virtual void v226() = 0; virtual void v227() = 0; virtual void v228() = 0; virtual void v229() = 0;
        virtual void v230() = 0; virtual void v231() = 0; virtual void v232() = 0; virtual void v233() = 0;
        virtual void v234() = 0; virtual void v235() = 0; virtual void v236() = 0; virtual void v237() = 0;
        virtual void v238() = 0; virtual void v239() = 0; virtual void v240() = 0; virtual void v241() = 0;
        virtual void v242() = 0; virtual void v243() = 0; virtual void v244() = 0; virtual void v245() = 0;
        virtual void v246() = 0; virtual void v247() = 0; virtual void v248() = 0; virtual void v249() = 0;
        virtual void v250() = 0; virtual void v251() = 0; virtual void v252() = 0; virtual void v253() = 0;
        virtual void v254() = 0; virtual void v255() = 0; virtual void v256() = 0; virtual void v257() = 0;
        virtual void v258() = 0; virtual void v259() = 0; virtual void v260() = 0; virtual void v261() = 0;
        virtual void v262() = 0; virtual void v263() = 0; virtual void v264() = 0; virtual void v265() = 0;
        virtual void v266() = 0; virtual void v267() = 0; virtual void v268() = 0; virtual void v269() = 0;
        virtual void v270() = 0; virtual void v271() = 0; virtual void v272() = 0; virtual void v273() = 0;
        virtual void v274() = 0; virtual void v275() = 0; virtual void v276() = 0; virtual void v277() = 0;
        virtual void v278() = 0; virtual void v279() = 0; virtual void v280() = 0; virtual void v281() = 0;
        virtual void v282() = 0; virtual void v283() = 0; virtual void v284() = 0; virtual void v285() = 0;
        virtual void v286() = 0; virtual void v287() = 0; virtual void v288() = 0; virtual void v289() = 0;
        virtual void v290() = 0; virtual void v291() = 0; virtual void v292() = 0; virtual void v293() = 0;
        virtual void v294() = 0; virtual void v295() = 0; virtual void v296() = 0; virtual void v297() = 0;
        virtual void v298() = 0; virtual void v299() = 0; virtual void v300() = 0; virtual void v301() = 0;
        virtual void v302() = 0; virtual void v303() = 0; virtual void v304() = 0; virtual void v305() = 0;
        virtual void v306() = 0; virtual void v307() = 0; virtual void v308() = 0; virtual void v309() = 0;
        virtual void v310() = 0; virtual void v311() = 0; virtual void v312() = 0; virtual void v313() = 0;
        virtual void v314() = 0; virtual void v315() = 0; virtual void v316() = 0; virtual void v317() = 0;
        virtual void v318() = 0; virtual void v319() = 0; virtual void v320() = 0; virtual void v321() = 0;
        virtual void v322() = 0; virtual void v323() = 0; virtual void v324() = 0; virtual void v325() = 0;
        virtual void v326() = 0; virtual void v327() = 0; virtual void v328() = 0; virtual void v329() = 0;
        virtual void v330() = 0; virtual void v331() = 0; virtual void v332() = 0; virtual void v333() = 0;
        virtual void v334() = 0; virtual void v335() = 0; virtual void v336() = 0; virtual void v337() = 0;
        virtual void v338() = 0; virtual void v339() = 0; virtual void v340() = 0; virtual void v341() = 0;
        virtual void v342() = 0; virtual void v343() = 0; virtual void v344() = 0; virtual void v345() = 0;
        virtual void v346() = 0; virtual void v347() = 0; virtual void v348() = 0; virtual void v349() = 0;
        virtual void v350() = 0; virtual void v351() = 0; virtual void v352() = 0; virtual void v353() = 0;
        virtual void v354() = 0; virtual void v355() = 0; virtual void v356() = 0; virtual void v357() = 0;
        virtual void v358() = 0; virtual void v359() = 0; virtual void v360() = 0; virtual void v361() = 0;
        virtual void v362() = 0; virtual float v363() = 0; virtual void v364() = 0; virtual void v365() = 0; // 363->0x5b4 gauge float (cf. CChainBattleObj5B4)
        virtual void v366() = 0;
        virtual void v367(float v) = 0; // index 367 -> vtable offset 0x5c4

        CChainSub4* field_04;        // 0x04
        CChainSub8 mSub8;            // 0x08 (embedded; vtable at its start)
        u8 _pad0C[0x3374 - 0x0C];   // 0x0C-0x3373
        u32 field_3374;             // 0x3374 (flags)
        u8 _pad3378[0x3380 - 0x3378]; // 0x3378-0x337F
        u8 mField3380;              // 0x3380 (opaque sub-object marker; address taken)
        u8 _pad3381[0x3E6C - 0x3381]; // 0x3381-0x3E6B
        u16 field_3E6C;             // 0x3E6C (chain flag bits; bit 12 = chain active)
        u8 _pad3E6E[0x3E9C - 0x3E6E]; // 0x3E6E-0x3E9B
        CChainVoiceSub mSub;        // 0x3E9C (embedded voice sub-object)
        u8 _pad3EA0[0x3F00 - 0x3EA0]; // 0x3EA0-0x3EFF
        u32 field_3F00;             // 0x3F00 (flags; bit 30 read by chain start)
        u8 _pad3F04[0x3F28 - 0x3F04]; // 0x3F04-0x3F27
        u16 field_3F28;             // 0x3F28 (battle id)
        u8 _pad3F2A[0x3F60 - 0x3F2A]; // 0x3F2A-0x3F5F
        CChainField3F60* field_3F60; // 0x3F60
    };

    // Chain-state object operated on by func_8027Dxxx / func_8027Exxx.
    // Mirrors cf::UnkClass_800D8DBC with signed/typed fields so the retail
    // lha/sth accesses and offsets reproduce exactly. Size 0xA4.
    class CChainState {
    public:
        s16 field_0;                // 0x0 state id
        u8 unk2[2];                 // 0x2
        CChainBattleObj* field_4;   // 0x4 battle object 1
        CChainBattleObj* field_8;   // 0x8 battle object 2
        u32 field_C;                // 0xC actor id
        CChainBattleObj* field_10;  // 0x10 wave target object
        s16 field_14;               // 0x14 wave phase counter
        u8 unk16[0x1C - 0x16];      // 0x16-0x1B
        CChainTemp mChainTemp;      // 0x1C (arts music slot table; address taken)
        CChainTime field_84;        // 0x84
        CChainTimer field_9C;       // 0x9C
    };

    // Result of CMenuArtsSelect_getSelectState: s8 state bytes at +0x0/+0x1.
    struct CArtsSelectStateView {
        s8 field_0; // 0x0
        s8 field_1; // 0x1
    };
}

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" {
int func_800B8920(void* addr);
int func_80148778(void*, int);
void* func_8016FE34(void* source);
unsigned long func_801BA2C8(void* self);
void func_800BBA7C(void* obj, f32* vec);
void* getArtsParamAtCnt(void* self, unsigned int index);
void* getArtsParamRC(const void* arts, int index, int subindex);
int func_80154280(void* param, void* actor, int flags);
void func_80082A0C__Q22cf13CfGameManagerFv();
void func_80082A7C__Q22cf13CfGameManagerFv(void* object);
void func_80082B38__Q22cf13CfGameManagerFv();
void func_801043BC();
void func_8027CC3C(void* self, void* owner);
void func_8027CD08(void* self, void* owner);
int func_80146300(int val, int flag);
// (func_8025FB10: single winning decl lives on chain/CChainActorList.hpp.)
int* func_8009ECB0();
}
// C++-mangled retail helper func_800B708C__Fi (actor id -> action source).
extern void* func_800B708C(int id);

// Data imports (plain globals - MWCC does not mangle global data names).
extern f32 lbl_eu_80668A90;
extern f32 lbl_eu_80668A94;
extern f32 lbl_eu_80668A98;
extern f32 lbl_eu_80668AA8;
extern f64 lbl_eu_80668AA0;
extern f64 lbl_eu_80668AB0;
extern f64 lbl_eu_80668AB8;
extern f32 lbl_eu_80668AC0;
extern f32 lbl_eu_80668AC4;
extern f32 lbl_eu_80668AC8;
extern f32 lbl_eu_80656C40[3];
extern f32 lbl_eu_80656C64[3];

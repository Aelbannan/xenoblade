#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainMember.hpp"
#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"

namespace cf {
    //size: 0x1F0C?
    class CChain {
    public:
        CChain();
        ~CChain(){}
        void func_8027728C();
        bool chkActorList();

        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainMember mChainMember; //0x1DC8
        CChainTimer mChainTimer1; //0x1EB4
        CChainTimer mChainTimer2; //0x1EBC
        CChainTime mChainTime; //0x1EC4
        CChainChance mChainChance; //0x1EDC
        CChainCombo mChainCombo; //0x1EF4
        u8 unk1F0C[0x14];
    };

    // Tail view of CChainChance (its trailing unk14[4] region) as a signed
    // accumulator: retail func_80277A7C reads/writes it with lwz/stw at
    // CChain+0x1EF0 (chain gauge accumulation, clamped at 0x98967F).
    struct CChainChanceTail {
        u8 field_0[0x14]; //0x0
        s32 field_0x14;   //0x14
    };

    // View of the battle manager tail for func_802795D4: s16 state word at
    // +0x20C8 (retail lha) and the sudden-commu sub-object at +0x216C
    // (address taken by func_801BA2C8).
    struct CBattleManagerTail {
        u8 field_0[0x20C8];            //0x0
        s16 field_0x20C8;              //0x20C8
        u8 field_0x20CA[0x216C - 0x20CA];
        u8 field_0x216C[0];            //0x216C (address taken)
    };

    // Battle-object view for func_80279F6C (chain-actor unk0 target): move
    // sub-object pointer at +0x4 (vtable slot 0x20 called with a flag), the
    // voice-act sub-object at +0x3380 (address taken by func_801537E0), and
    // a u16 flag word at +0x3388.
    class CChainBattleObjF {
    public:
        u8 field_0[0x4];       //0x0
        CChainSub4* field_4;   //0x4
        u8 field_8[0x3380 - 0x8];
        u8 field_0x3380;       //0x3380
        u8 field_0x3381[0x3388 - 0x3381];
        u16 field_0x3388;      //0x3388
    };

    // Interface for the chain-actor manual vtable at +0x70: declared virtual
    // #23 lands at vtable byte offset +0x64 (func_80279694 dispatches
    // (self,key) there). Same pad-first / -RTTI layout trick as
    // CChainActorVtIf: the vptr sits after the 0x70 pad.
    class CChainActorVtIf64 {
    public:
        u8 pad70[0x70];
        virtual void v000() = 0;
        virtual void v001() = 0;
        virtual void v002() = 0;
        virtual void v003() = 0;
        virtual void v004() = 0;
        virtual void v005() = 0;
        virtual void v006() = 0;
        virtual void v007() = 0;
        virtual void v008() = 0;
        virtual void v009() = 0;
        virtual void v010() = 0;
        virtual void v011() = 0;
        virtual void v012() = 0;
        virtual void v013() = 0;
        virtual void v014() = 0;
        virtual void v015() = 0;
        virtual void v016() = 0;
        virtual void v017() = 0;
        virtual void v018() = 0;
        virtual void v019() = 0;
        virtual void v020() = 0;
        virtual void v021() = 0;
        virtual void v022() = 0;
        virtual void v023(int key) = 0; // slot 25 / +0x64
    };

    // Manual-vtable interface for func_80279778: the actor's manual vtable
    // at +0x70, slot 0x28 (declared virtual #8). The slot returns an int
    // that becomes the function's result on the fall-through path.
    class CChainActorVtIf28 {
    public:
        u8 pad70[0x70];
        virtual void v000() = 0;
        virtual void v001() = 0;
        virtual void v002() = 0;
        virtual void v003() = 0;
        virtual void v004() = 0;
        virtual void v005() = 0;
        virtual void v006() = 0;
        virtual void v007() = 0;
        virtual int v008() = 0; // slot 10 / +0x28
    };

    // Manual-vtable interface for the chain-actor vtables at +0x70, extended
    // past CChainActorVtIf64/28 to the slots used by func_80278E0C /
    // func_8027936C / func_8027A338: +0x1c (slot 7), +0x34 (slot 13),
    // +0x38 (slot 14), +0x5c (slot 23), +0x68 (slot 26), +0x6c (slot 27).
    // Same pad-first / -RTTI layout trick: declared virtual #k lands at
    // vtable byte offset (k+2)*4.
    class CChainActorVtIf2 {
    public:
        u8 pad70[0x70];
        virtual void v000() = 0;
        virtual void v001() = 0;
        virtual void v002() = 0;
        virtual void v003() = 0;
        virtual void v004() = 0;
        virtual void v005(int v) = 0;       // slot 7 / +0x1c (activate-with-flag)
        virtual void v006() = 0;
        virtual void v007() = 0;
        virtual void v008() = 0;
        virtual void v009() = 0;
        virtual void v010() = 0;
        virtual void v011() = 0;            // slot 13 / +0x34
        virtual void v012(int a, int b) = 0; // slot 14 / +0x38
        virtual void v013() = 0;
        virtual void v014() = 0;
        virtual void v015() = 0;
        virtual void v016() = 0;
        virtual void v017() = 0;
        virtual void v018() = 0;
        virtual void v019() = 0;
        virtual void v020() = 0;
        virtual int v021() = 0;             // slot 23 / +0x5c (chainable check)
        virtual void v022() = 0;
        virtual void v023() = 0;
        virtual int v024() = 0;             // slot 26 / +0x68 (run key query)
        virtual void v025(int v) = 0;       // slot 27 / +0x6c
    };

    // Manual-vtable interface for func_80278F84: slots 0x24 (declared #7,
    // takes the flag arg), 0x2c (declared #9) and 0x58 (declared #20) on the
    // chain actor's manual vtable at +0x70. Same pad-first / -RTTI layout
    // trick as CChainActorVtIf2: declared virtual #k lands at vtable byte
    // offset (k+2)*4. Never instantiated, so no vtable emits.
    class CChainActorVtIf84 {
    public:
        u8 pad70[0x70];
        virtual void v000() = 0;
        virtual void v001() = 0;
        virtual void v002() = 0;
        virtual void v003() = 0;
        virtual void v004() = 0;
        virtual void v005() = 0;
        virtual void v006() = 0;
        virtual int v007(int v) = 0; // slot 9 / +0x24 (activate-with-flag)
        virtual void v008() = 0;
        virtual int v009() = 0;      // slot 11 / +0x2c (chain-state query)
        virtual void v010() = 0;
        virtual void v011() = 0;
        virtual void v012() = 0;
        virtual void v013() = 0;
        virtual void v014() = 0;
        virtual void v015() = 0;
        virtual void v016() = 0;
        virtual void v017() = 0;
        virtual void v018() = 0;
        virtual void v019() = 0;
        virtual int v020() = 0;      // slot 22 / +0x58 (actor value query)
    };

    // Battle-object vtable interface for func_80278E0C: slot +0x5b4
    // (declared virtual #363) returns a float that is stored into the
    // chain's unk0[0x14]. Never instantiated, so no vtable emits.
    class CChainBattleObj5B4 {
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
        virtual void v080() = 0; virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
        virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0; virtual void v087() = 0;
        virtual void v088() = 0; virtual void v089() = 0; virtual void v090() = 0; virtual void v091() = 0;
        virtual void v092() = 0; virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
        virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0; virtual void v099() = 0;
        virtual void v100() = 0; virtual void v101() = 0; virtual void v102() = 0; virtual void v103() = 0;
        virtual void v104() = 0; virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
        virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0; virtual void v111() = 0;
        virtual void v112() = 0; virtual void v113() = 0; virtual void v114() = 0; virtual void v115() = 0;
        virtual void v116() = 0; virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
        virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0; virtual void v123() = 0;
        virtual void v124() = 0; virtual void v125() = 0; virtual void v126() = 0; virtual void v127() = 0;
        virtual void v128() = 0; virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
        virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0; virtual void v135() = 0;
        virtual void v136() = 0; virtual void v137() = 0; virtual void v138() = 0; virtual void v139() = 0;
        virtual void v140() = 0; virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
        virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0; virtual void v147() = 0;
        virtual void v148() = 0; virtual void v149() = 0; virtual void v150() = 0; virtual void v151() = 0;
        virtual void v152() = 0; virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
        virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0; virtual void v159() = 0;
        virtual void v160() = 0; virtual void v161() = 0; virtual void v162() = 0; virtual void v163() = 0;
        virtual void v164() = 0; virtual void v165() = 0; virtual void v166() = 0; virtual void v167() = 0;
        virtual void v168() = 0; virtual void v169() = 0; virtual void v170() = 0; virtual void v171() = 0;
        virtual void v172() = 0; virtual void v173() = 0; virtual void v174() = 0; virtual void v175() = 0;
        virtual void v176() = 0; virtual void v177() = 0; virtual void v178() = 0; virtual void v179() = 0;
        virtual void v180() = 0; virtual void v181() = 0; virtual void v182() = 0; virtual void v183() = 0;
        virtual void v184() = 0; virtual void v185() = 0; virtual void v186() = 0; virtual void v187() = 0;
        virtual void v188() = 0; virtual void v189() = 0; virtual void v190() = 0; virtual void v191() = 0;
        virtual void v192() = 0; virtual void v193() = 0; virtual void v194() = 0; virtual void v195() = 0;
        virtual void v196() = 0; virtual void v197() = 0; virtual void v198() = 0; virtual void v199() = 0;
        virtual void v200() = 0; virtual void v201() = 0; virtual void v202() = 0; virtual void v203() = 0;
        virtual void v204() = 0; virtual void v205() = 0; virtual void v206() = 0; virtual void v207() = 0;
        virtual void v208() = 0; virtual void v209() = 0; virtual void v210() = 0; virtual void v211() = 0;
        virtual void v212() = 0; virtual void v213() = 0; virtual void v214() = 0; virtual void v215() = 0;
        virtual void v216() = 0; virtual void v217() = 0; virtual void v218() = 0; virtual void v219() = 0;
        virtual void v220() = 0; virtual void v221() = 0; virtual void v222() = 0; virtual void v223() = 0;
        virtual void v224() = 0; virtual void v225() = 0; virtual void v226() = 0; virtual void v227() = 0;
        virtual void v228() = 0; virtual void v229() = 0; virtual void v230() = 0; virtual void v231() = 0;
        virtual void v232() = 0; virtual void v233() = 0; virtual void v234() = 0; virtual void v235() = 0;
        virtual void v236() = 0; virtual void v237() = 0; virtual void v238() = 0; virtual void v239() = 0;
        virtual void v240() = 0; virtual void v241() = 0; virtual void v242() = 0; virtual void v243() = 0;
        virtual void v244() = 0; virtual void v245() = 0; virtual void v246() = 0; virtual void v247() = 0;
        virtual void v248() = 0; virtual void v249() = 0; virtual void v250() = 0; virtual void v251() = 0;
        virtual void v252() = 0; virtual void v253() = 0; virtual void v254() = 0; virtual void v255() = 0;
        virtual void v256() = 0; virtual void v257() = 0; virtual void v258() = 0; virtual void v259() = 0;
        virtual void v260() = 0; virtual void v261() = 0; virtual void v262() = 0; virtual void v263() = 0;
        virtual void v264() = 0; virtual void v265() = 0; virtual void v266() = 0; virtual void v267() = 0;
        virtual void v268() = 0; virtual void v269() = 0; virtual void v270() = 0; virtual void v271() = 0;
        virtual void v272() = 0; virtual void v273() = 0; virtual void v274() = 0; virtual void v275() = 0;
        virtual void v276() = 0; virtual void v277() = 0; virtual void v278() = 0; virtual void v279() = 0;
        virtual void v280() = 0; virtual void v281() = 0; virtual void v282() = 0; virtual void v283() = 0;
        virtual void v284() = 0; virtual void v285() = 0; virtual void v286() = 0; virtual void v287() = 0;
        virtual void v288() = 0; virtual void v289() = 0; virtual void v290() = 0; virtual void v291() = 0;
        virtual void v292() = 0; virtual void v293() = 0; virtual void v294() = 0; virtual void v295() = 0;
        virtual void v296() = 0; virtual void v297() = 0; virtual void v298() = 0; virtual void v299() = 0;
        virtual void v300() = 0; virtual void v301() = 0; virtual void v302() = 0; virtual void v303() = 0;
        virtual void v304() = 0; virtual void v305() = 0; virtual void v306() = 0; virtual void v307() = 0;
        virtual void v308() = 0; virtual void v309() = 0; virtual void v310() = 0; virtual void v311() = 0;
        virtual void v312() = 0; virtual void v313() = 0; virtual void v314() = 0; virtual void v315() = 0;
        virtual void v316() = 0; virtual void v317() = 0; virtual void v318() = 0; virtual void v319() = 0;
        virtual void v320() = 0; virtual void v321() = 0; virtual void v322() = 0; virtual void v323() = 0;
        virtual void v324() = 0; virtual void v325() = 0; virtual void v326() = 0; virtual void v327() = 0;
        virtual void v328() = 0; virtual void v329() = 0; virtual void v330() = 0; virtual void v331() = 0;
        virtual void v332() = 0; virtual void v333() = 0; virtual void v334() = 0; virtual void v335() = 0;
        virtual void v336() = 0; virtual void v337() = 0; virtual void v338() = 0; virtual void v339() = 0;
        virtual void v340() = 0; virtual void v341() = 0; virtual void v342() = 0; virtual void v343() = 0;
        virtual void v344() = 0; virtual void v345() = 0; virtual void v346() = 0; virtual void v347() = 0;
        virtual void v348() = 0; virtual void v349() = 0; virtual void v350() = 0; virtual void v351() = 0;
        virtual void v352() = 0; virtual void v353() = 0; virtual void v354() = 0; virtual void v355() = 0;
        virtual void v356() = 0; virtual void v357() = 0; virtual void v358() = 0; virtual void v359() = 0;
        virtual void v360() = 0; virtual void v361() = 0; virtual void v362() = 0;
        virtual float v363() = 0; // index 363 -> vtable offset 0x5b4
    };

    // Battle-object view for func_80279A4C: base vtable at +0x0 (declared
    // virtual #167 lands at +0x2A4, returning the arts-category holder),
    // move sub-object pointer at +0x4, battle id at +0x3F28.
    class CChainBattleObj2A4 {
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
        virtual void v080() = 0; virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
        virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0; virtual void v087() = 0;
        virtual void v088() = 0; virtual void v089() = 0; virtual void v090() = 0; virtual void v091() = 0;
        virtual void v092() = 0; virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
        virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0; virtual void v099() = 0;
        virtual void v100() = 0; virtual void v101() = 0; virtual void v102() = 0; virtual void v103() = 0;
        virtual void v104() = 0; virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
        virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0; virtual void v111() = 0;
        virtual void v112() = 0; virtual void v113() = 0; virtual void v114() = 0; virtual void v115() = 0;
        virtual void v116() = 0; virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
        virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0; virtual void v123() = 0;
        virtual void v124() = 0; virtual void v125() = 0; virtual void v126() = 0; virtual void v127() = 0;
        virtual void v128() = 0; virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
        virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0; virtual void v135() = 0;
        virtual void v136() = 0; virtual void v137() = 0; virtual void v138() = 0; virtual void v139() = 0;
        virtual void v140() = 0; virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
        virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0; virtual void v147() = 0;
        virtual void v148() = 0; virtual void v149() = 0; virtual void v150() = 0; virtual void v151() = 0;
        virtual void v152() = 0; virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
        virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0; virtual void v159() = 0;
        virtual void v160() = 0; virtual void v161() = 0; virtual void v162() = 0; virtual void v163() = 0;
        virtual void v164() = 0; virtual void v165() = 0; virtual void v166() = 0;
        virtual CChainCombo_ArtsCategoryHolder* v167() = 0; // index 167 -> +0x2A4

        CChainSub4* field_04;        //0x04 move sub-object (slot 0x30 -> word holder)
        u8 _pad08[0x3F28 - 0x08];
        u16 field_3F28;              //0x3F28 battle id
    };

    // Arts-category object probed by func_80279A4C at +0x77 (retail lbz).
    struct CChainArtsCat77 {
        u8 field_0[0x77];
        u8 field_0x77;   //0x77
    };

    // Timing-result object returned by the embedded voice sub-object's
    // vtable slot 0x110 (func_8027A8C8 compares its +0x14 float against
    // lbl_eu_80668A60).
    class CChainVoiceRes {
    public:
        u8 _pad0[0x14];
        f32 field_14;           // 0x14
    };

    // Virtual-dispatch shim for the embedded voice sub-object at +0x3E9C of
    // the chain battle objects, extended past the CChainTimer.hpp
    // CChainVoiceSub (which stops at slot 0xAC): declared virtual N sits at
    // vtable byte offset (N+2)*4, so #66 lands at +0x110. Never
    // instantiated, so no vtable emits and the declared (non-pure)
    // virtuals need no bodies.
    class CChainVoiceSub110 {
    public:
        virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
        virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
        virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
        virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
        virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
        virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
        virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
        virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
        virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
        virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
        virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
        virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
        virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
        virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
        virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
        virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
        virtual void v064(); virtual void v065();
        virtual CChainVoiceRes* v066(); // index 66 -> vtable offset 0x110
    };

    // Battle-object view for func_8027A8C8: move sub-object pointer at +0x4,
    // the embedded voice sub-object at +0x3E9C (slot 0x110 probed for
    // timing), and the battle-object pointer at +0x3F60.
    class CChainBattleObjVoice {
    public:
        u8 _pad0[0x4];
        CChainSub4* field_04;         // 0x4
        u8 _pad08[0x3E9C - 0x8];
        CChainVoiceSub110 mSub;       // 0x3E9C (embedded; vtable at its start)
        u8 _pad3EA0[0x3F60 - 0x3EA0];
        CChainField3F60* field_3F60;  // 0x3F60
    };

    // Holder for func_8027A8C8: battle object pointer at +0x0.
    class CChainVoiceHolder {
    public:
        CChainBattleObjVoice* field_0; // 0x0
    };

    // Embedded sub-object at +0x08 dispatch interface for func_80279E48:
    // slots 0x14 (index 3) and 0x20 (index 6) take a u32 flag (the
    // CChainTimer.hpp CChainSub8 mirror declares slot 0x14 without an arg,
    // so this interface exists for the flagged call). Never instantiated, so
    // the declared (non-pure) virtuals need no bodies.
    class CChainSub8E {
    public:
        virtual void e00();
        virtual void e01();
        virtual void e02();
        virtual void e03(u32 v); // index 3 -> vtable offset 0x14
        virtual void e04();
        virtual void e05();
        virtual void e06(u32 v); // index 6 -> vtable offset 0x20
        virtual void e07();
    };

    // Battle-object view for func_80279E48: embedded sub-object at +0x8
    // (lwzu dispatch) and the voice-act marker at +0x3380. The base vtable
    // slot 0xB0 (v042) is reached through CChainBattleObj2A4.
    class CChainBattleObjE {
    public:
        u8 _pad0[0x4];
        CChainSub4* field_4;      // 0x4 move sub-object (slot 0x30 -> word holder)
        CChainSub8E mSub8;        // 0x08 (embedded; vtable at its start)
        u8 _pad0C[0x3380 - 0x0C];
        u8 mField3380;            // 0x3380 (voice-act marker; address taken)
        u8 _pad3381[0x3388 - 0x3381];
        u16 field_0x3388;         // 0x3388 flag word (bit 1 = chain-cancel voice)
        u8 _pad338A[0x3E98 - 0x338A];
        u32 field_0x3E98;         // 0x3E98 (cleared by func_80279B34)
    };

    // Battle-object view for func_80279214: move sub-object pointer at +0x4
    // (slot 0x30 -> word holder), a 32-byte-entry table at +0x358C indexed
    // by (field_3590 % field_3598), and the +0x3594 gate flag.
    class CChainBattleObj214 {
    public:
        u8 _pad0[0x4];
        CChainSub4* field_04;      // 0x4 move sub-object
        u8 _pad08[0x358C - 0x8];
        u8* field_358C;            // 0x358C table base (32-byte entries)
        u32 field_3590;            // 0x3590
        u32 field_3594;            // 0x3594 gate flag
        u32 field_3598;            // 0x3598
    };

    // 32-byte table entry probed by func_80279214 at +0xD (signed: retail
    // compares the byte with cmpwi).
    class CChainArt214Entry {
    public:
        u8 field_0[0xD];
        s8 field_0xD;              // 0xD
    };

    // Layout mirror of CChain used by func_80279694: the member list viewed
    // as a CChainList member so member-array accesses fold the 0x1DC8 offset
    // into the load (a C-style cast of &self->mChainMember would materialize
    // the base into a register first).
    struct CChainMemberListMirror {
        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainList mChainMember;         //0x1DC8
        u8 unk1EB4[0x1F0C - 0x1EB4];
    };
}

// Forward decls for the CChain tail overlays (full defs live in
// CMenuBattleChain.hpp / CUIErrMesWin.hpp; func_8027728C only passes their
// addresses through to the reset helpers).
struct CBattleChainMenuState;
struct CErrMesEntry;
struct CErrMesOwner;

// func_80276D30 is defined in this TU (C linkage inherited from the extern
// "C" block below, so both the definition and the call reloc stay unmangled).

// Chain-voice data symbols (.sdata float / .sbss byte).
extern float lbl_eu_80668A18;
extern float lbl_eu_80668A1C;
extern u8 lbl_eu_80663DA0;

// Voice-timing threshold compared against the voice sub-object's slot-0x110
// result by func_8027A8C8 (.sdata2 float; const lets MWCC schedule the
// pool load at retail's position, cf. MWCC_REFERENCE SDA-pool hoist).
extern const f32 lbl_eu_80668A60;

// Chain-voice global state (retail .sdata lbl_eu_80662A20): -1 is the
// "not loaded" sentinel. The func_80276C*/func_8027715* helpers probe it with
// the unsigned (value + 0x10000) == 0xffff sentinel test (MWCC addis/cmplwi
// idiom, cf. func_80190394 in code_8018F8D8.cpp).
extern s32 lbl_eu_80662A20;

// C-linkage callees (retail symbols are unmangled globals - C linkage keeps
// the call relocs verbatim). Voice-manager helpers are defined in
// kyoshin/cf/voice/CCharVoiceMan.cpp; the chain-actor-list helpers in
// CChainActorList.cpp; the chain-menu / err-mes helpers in
// CMenuBattleChain.cpp / CUIErrMesWin.cpp.

// Circular gimmick object list consumed by func_800B6BC8 (layout mirror of
// the CfGimmick.hpp structs - that header's func_800B3A88 C-import clashes
// with CChainEffect.hpp's void* variant, so its func_800B6BC8 C-import and
// the list structs are mirrored here instead).
struct CChainGimmickListNode {
    CChainGimmickListNode* next; //0x00
    u8 gap04[0x08 - 0x04];
    void* object;               //0x08
};
struct CChainGimmickList {
    void* field_00;             //0x00
    CChainGimmickListNode* head; //0x04
};

// CfGameManager mode-flag word (.sbss; sda21 access).
extern u32 lbl_eu_80663E28;

namespace cf { class CfObjectActor; }

extern "C" {
    void func_802A1500();
    void func_802A35B8(u32 arg);
    int func_802A3748(u32 arg);
    int func_802A3214();
    int func_802A3680(int a, CChainBattleObjTail* b, int c);
    void func_8027B164(cf::CChainActorList* self);
    cf::CChainActor* func_8027CA98(cf::CChainList* self, u32 key);
    void func_8027C45C(cf::CChainList* self);
    void func_802AB3D0(CBattleChainMenuState* self);
    u8* func_802B48A0(CErrMesEntry* self);
    int func_80276D30(u8* self);
    // Same-TU voice reset helper (defined in CChain.cpp); C linkage keeps
    // the call reloc from func_8027732C verbatim.
    void func_80276C58();
    // Chain-actor-list helpers (CChainActorList.cpp). C linkage keeps the
    // call reloc names verbatim (MWCC otherwise mangles global functions).
    void func_8027B200(cf::CChainActorList* self);
    void func_8027B2CC(cf::CChainActorList* self);
    void func_8027C49C(cf::CChainList* self);
    void func_8027C560(cf::CChainList* self);
    void func_8027C0B0(cf::CChainChanceS* self);
    int  func_8027B814(cf::CChainActorList* self, u32 key);
    int  func_8027BC14(cf::CChainActorList* self, u32 key);
    int  func_8027C5E4(cf::CChainList* self, u32 key);
    // Chain-start helpers used by func_8027936C / func_80278E0C
    // (CChainActorList.cpp / CMenuBattleChain.cpp).
    void func_8027BA0C(cf::CChainActorList* self, cf::CChainList* other, cf::CfObjectActor* target);
    void func_8027C924(cf::CChainList* self, int target);
    void func_8027C6B4(cf::CChainList* self, int target, int index);
    void func_8027BF58(cf::CChainFlag* self);
    void func_8027C040(cf::CChainFlag* self);
    void func_802AB474(CBattleChainMenuState* self);
    // Presentation/event flag-bit probe (code_800F42AC.cpp; retail symbol
    // carries the Fi suffix) and battle-manager sub-check (CBattleManager).
    bool func_8006EF04__Fi(int mask);
    void* func_800EA444(void* bm);
    // Arts-select slot probe (CMenuArtsSelect.cpp): unmangled at the retail
    // call site, so declared here with C linkage rather than through the
    // class member (which mangles to func_80107C54__15CMenuArtsSelectFl).
    int func_80107C54(void* menu, int index);
    // Voice-act / chain arts-combo helpers. func_801537F0 lives in
    // CVision.hpp (same signature, no clash); func_8014AE00 and
    // func_80293E24 have no header declaration, so they are declared here.
    void func_801537F0(void* self);
    void func_8014AE00(void* self);
    void func_80293E24(cf::CChainCombo* self, cf::CfObjectActor* actor);
    // Same-TU helper (defined in CChain.cpp after its first caller).
    void func_80277B38(cf::CChain* self);
    // Chain-menu / err-mes helpers (CMenuBattleChain.cpp / CUIErrMesWin.cpp).
    void func_802AB410(CBattleChainMenuState* self);
    void func_802B48B8(CErrMesEntry* self);
    // Error-message record register helpers (CUIErrMesWin.cpp) - same family
    // as func_802B48A0 / func_802B48B8 above (chain-state codes 1/2/3).
    void func_802B48E4(CErrMesEntry* self, CErrMesOwner* owner);
    void func_802B4968(CErrMesEntry* self, CErrMesOwner* owner);
    void func_802B4A68(CErrMesEntry* self, CErrMesOwner* owner);
    // Battle-chain menu helpers (CMenuBattleChain.cpp).
    void func_802AB5E4(CBattleChainMenuState* self);
    int func_802AB510(CBattleChainMenuState* self, u8* out);
}

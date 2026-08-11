#pragma once

// CArtsParam/CAttackParam are defined in CArtsSet.hpp (included by the cpp).

#include <types.h>

namespace cf {

// Interface mirrors for the battle-object actor passed to func_80154280
// (arts usability check). kyoshin builds with -RTTI on, so declared virtual
// index N sits at vtable byte offset (N+2)*4 - the named slots below land on
// the exact retail offsets read by func_80154280. The virtuals are declared
// without bodies (like CChainVoiceSub in CChainTimer.hpp): the mirrors are
// never instantiated, so no vtable is emitted and the bodies are never
// needed.

// Sub-object at battle-object +0x04: vtable slot 0x30 returns a word holder.
class CArtsParamSub4 {
public:
    virtual void f00(); virtual void f01(); virtual void f02();
    virtual void f03(); virtual void f04(); virtual void f05();
    virtual void f06(); virtual void f07(); virtual void f08();
    virtual void f09();
    virtual void* f30(); // index 10 -> vtable offset 0x30
};

// Sub-object embedded at battle-object +0x3E9C (voice/action sub-system).
class CArtsParamSub {
public:
    virtual void s00(); virtual void s01(); virtual void s02();
    virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08();
    virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14();
    virtual void s15(); virtual void s16();
    virtual void* s17(); // index 17 -> vtable offset 0x4c
    virtual void s18(); virtual void s19(); virtual void s20();
    virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26();
    virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32();
    virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38();
    virtual void s39(); virtual void s40();
    virtual void* s41(); // index 41 -> vtable offset 0xac
    virtual void s42(); virtual void s43(); virtual void s44();
    virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50();
    virtual void s51(); virtual void s52(); virtual void s53();
    virtual void s54(); virtual void s55(); virtual void s56();
    virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61(); virtual void s62();
    virtual void s63(); virtual void s64(); virtual void s65();
    virtual void s66(); virtual void s67(); virtual void s68();
    virtual void s69(); virtual void s70(); virtual void s71();
    virtual void s72();
    virtual void* s73(); // index 73 -> vtable offset 0x12c
};

// Battle-object / arts-set actor (both the r4 argument of func_80154280 and
// the object returned by its vtable+0xa0 slot).
class CArtsParamActor {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37();
    virtual void* v38(); // index 38 -> vtable offset 0xa0
    virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71();
    virtual float v72(); // index 72 -> vtable offset 0x128
    virtual void v73();
    virtual float v74(); // index 74 -> vtable offset 0x130
    virtual void v75(); virtual void v76(); virtual void v77();
    virtual float v78(); // index 78 -> vtable offset 0x140
    virtual void v79(); virtual void v80(); virtual void v81();
    virtual void v82(); virtual void v83();
    virtual float v84(); // index 84 -> vtable offset 0x158
    virtual void v85(); virtual void v86(); virtual void v87();
    virtual void v88(); virtual void v89(); virtual void v90();
    virtual void v91(); virtual void v92(); virtual void v93();
    virtual void v94(); virtual void v95(); virtual void v96();
    virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109();
    virtual float* v110(); // index 110 -> vtable offset 0x1c0
    virtual float* v111(); // index 111 -> vtable offset 0x1c4
    virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153();
    virtual void v154(); virtual void v155();
    virtual void* v156(); // index 156 -> vtable offset 0x278 (arts set)
    virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162();
    virtual void* v163(); // index 163 -> vtable offset 0x294
    virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172();
    virtual int v173();  // index 173 -> vtable offset 0x2bc
    virtual void v174(); virtual void v175(); virtual void v176();
    virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184(); virtual void v185();
    virtual void v186(); virtual void v187(); virtual void v188();
    virtual void v189(); virtual void v190(); virtual void v191();
    virtual int v192();  // index 192 -> vtable offset 0x308
    virtual void v193(); virtual void v194(); virtual void v195();
    virtual void v196(); virtual void v197(); virtual void v198();
    virtual void v199(); virtual void v200(); virtual void v201();
    virtual void v202(); virtual void v203(); virtual void v204();
    virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210();
    virtual void v211(); virtual void v212(); virtual void v213();
    virtual void v214(); virtual void v215(); virtual void v216();
    virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222();
    virtual void v223(); virtual void v224(); virtual void v225();
    virtual void v226(); virtual void v227(); virtual void v228();
    virtual void v229(); virtual void v230(); virtual void v231();
    virtual void v232(); virtual void v233(); virtual void v234();
    virtual void v235(); virtual void v236(); virtual void v237();
    virtual void v238(); virtual void v239(); virtual void v240();
    virtual void v241(); virtual void v242(); virtual void v243();
    virtual void v244(); virtual void v245(); virtual void v246();
    virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252();
    virtual void v253(); virtual void v254(); virtual void v255();
    virtual void v256(); virtual void v257(); virtual void v258();
    virtual void v259(); virtual void v260(); virtual void v261();
    virtual void v262(); virtual void v263(); virtual void v264();
    virtual void v265(); virtual void v266(); virtual void v267();
    virtual void v268(); virtual void v269(); virtual void v270();
    virtual void v271(); virtual void v272(); virtual void v273();
    virtual void v274(); virtual void v275(); virtual void v276();
    virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282();
    virtual void v283(); virtual void v284(); virtual void v285();
    virtual void v286(); virtual void v287(); virtual void v288();
    virtual void v289(); virtual void v290(); virtual void v291();
    virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297();
    virtual void v298(); virtual void v299(); virtual void v300();
    virtual void v301(); virtual void v302(); virtual void v303();
    virtual void v304(); virtual void v305(); virtual void v306();
    virtual void v307(); virtual void v308(); virtual void v309();
    virtual void v310(); virtual void v311(); virtual void v312();
    virtual void v313(); virtual void v314(); virtual void v315();
    virtual void v316(); virtual void v317(); virtual void v318();
    virtual void v319(); virtual void v320(); virtual void v321();
    virtual void v322(); virtual void v323(); virtual void v324();
    virtual void v325(); virtual void v326(); virtual void v327();
    virtual void v328(); virtual void v329(); virtual void v330();
    virtual void v331(); virtual void v332(); virtual void v333();
    virtual void v334(); virtual void v335(); virtual void v336();
    virtual void v337(); virtual void v338(); virtual void v339();
    virtual void v340(); virtual void v341(); virtual void v342();
    virtual void v343(); virtual void v344(); virtual void v345();
    virtual void v346(); virtual void v347(); virtual void v348();
    virtual void v349(); virtual void v350(); virtual void v351();
    virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357();
    virtual void v358(); virtual void v359(); virtual void v360();
    virtual void v361(); virtual void v362();
    virtual float v363(); // index 363 -> vtable offset 0x5b4
    virtual float v364(); // index 364 -> vtable offset 0x5b8
    virtual void v365(); virtual void v366();

    CArtsParamSub4* field_04;                 // 0x04
    u8 _pad08[0x3E9C - 0x08];                 // 0x08-0x3E9B
    CArtsParamSub mSub;                       // 0x3E9C (embedded sub-object)
    u8 _pad3EA0[0x3F00 - 0x3EA0];             // 0x3EA0-0x3EFF
    u32 field_3F00;                           // 0x3F00 (flags; bit 30 read)
    u8 _pad3F04[0x3F28 - 0x3F04];             // 0x3F04-0x3F27
    u16 field_3F28;                           // 0x3F28 (battle id)
    u8 _pad3F2A[0x3F60 - 0x3F2A];             // 0x3F2A-0x3F5F
    void* field_3F60;                         // 0x3F60
};

// Word holder behind battle-object field_3F60 (func_8004DAC4 key).
struct CArtsParamField3F60 {
    u8 _pad[0x374];
    u32 field_374; // 0x374
};

} // namespace cf

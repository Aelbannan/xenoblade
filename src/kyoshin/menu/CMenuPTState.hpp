#pragma once

#include <types.h>

// CPartyStateWin.hpp references cf::CfObjectMove before its own forward decl;
// pre-declare it here (same pattern as other includers).
namespace cf { class CfObjectMove; }
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CPartyStateWin.hpp"

// Vtable adjustor thunks -- when a derived class overrides CMenuPTState
// virtual functions and the vtable slot is at offset +0x58 from the
// CMenuPTState subobject, these thunks adjust 'this' by -0x58 before
// tail-calling the real member function.

// Byte-range shim over the CProcess header + PTMF zone, so the allocating
// factory constructor can write the vtable (+0x10) and callback slots
// (+0x3C..0x53) without raw pointer arithmetic. Layout matches CProcess
// (0x00-0x3B) exactly.
struct CMenuPTStateCtorShim {
    u8 _00[0x10];
    void* vtable;          // 0x10 -- CProcess vtable, overwritten by this ctor
    u8 _14[0x28];          // 0x14-0x3B -- rest of CProcess
    u32 callbacks[6];      // 0x3C-0x53 -- PTMF callback slots (__ptmf_null copies)
    u8 field54;            // 0x54
    u8 field55;            // 0x55
};

// CProcess header + PTMF imports for the retail singleton factory (see .cpp).
class CMenuPTState : public CProcess, public IScnRender {
public:
    virtual ~CMenuPTState();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks.
    void func_80192BE4();
    void func_80192BEC();

    // +0x3C-0x57: compiler-managed vtable/ptmf data
    // +0x58: IScnRender vtable (implicit)
    CProcess* mStoredParent; // 0x5C
    CBgTex mBgTex; // 0x60
    CPartyStateWin mPartyStateWin; // 0x80
    u8 mField_6C6C; // 0x6C6C
};

namespace cf {

// Abstract vtable view of the battle actor polled by func_80192C2C: only the
// state getter at vtable slot 0x308 (index 192) is used.
class CMenuPtStateActor {
public:
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0; virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0; virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0; virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0; virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0; virtual void v027() = 0; virtual void v028() = 0; virtual void v029() = 0;
    virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0; virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0; virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0;
    virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0; virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0; virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0;
    virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0; virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0; virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0;
    virtual void v066() = 0; virtual void v067() = 0; virtual void v068() = 0; virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0; virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;
    virtual void v078() = 0; virtual void v079() = 0; virtual void v080() = 0; virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
    virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0; virtual void v087() = 0; virtual void v088() = 0; virtual void v089() = 0;
    virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0; virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
    virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0; virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0;
    virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0; virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
    virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0; virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0;
    virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0; virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
    virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0; virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0;
    virtual void v126() = 0; virtual void v127() = 0; virtual void v128() = 0; virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
    virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0; virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
    virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0; virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
    virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0; virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0;
    virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0; virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
    virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0; virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0;
    virtual void v162() = 0; virtual void v163() = 0; virtual void v164() = 0; virtual void v165() = 0; virtual void v166() = 0; virtual void v167() = 0;
    virtual void v168() = 0; virtual void v169() = 0; virtual void v170() = 0; virtual void v171() = 0; virtual void v172() = 0; virtual void v173() = 0;
    virtual void v174() = 0; virtual void v175() = 0; virtual void v176() = 0; virtual void v177() = 0; virtual void v178() = 0; virtual void v179() = 0;
    virtual void v180() = 0; virtual void v181() = 0; virtual void v182() = 0; virtual void v183() = 0; virtual void v184() = 0; virtual void v185() = 0;
    virtual void v186() = 0; virtual void v187() = 0; virtual void v188() = 0; virtual void v189() = 0; virtual void v190() = 0; virtual void v191() = 0;
    virtual int getCount() = 0; // slot 0x308
};

class UnkClass_80192BF4 {
public:
    UnkClass_80192BF4();
    void __ct__80192C10();

    // +0x00: counter/state
    u32 field_0x00;
    // +0x04: float timer/accumulator
    f32 field_0x04;
    // +0x08: float value
    f32 field_0x08;
};
} // namespace cf
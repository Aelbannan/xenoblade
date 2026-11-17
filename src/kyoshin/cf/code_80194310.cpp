// TU: kyoshin/cf/code_80194310
// cf::UnkClass_80192BF4 - battle timer/accumulator block owned by
// cf::CBattleManager (+0x19C). Sits between CMenuPTState and CPartsChange in
// the retail link order (after CMenuPTState's this-adjusting thunks, so it is
// a separate translation unit).

#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/CPartyStateWinImports.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CfEnumListHolder.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // lbl_eu_80663E14
#include "kyoshin/cf/object/CAIAction.hpp"   // getObjectAt / isBattleStatusActive

// Battle-actor view polled by refreshPtStateAccum (unit-private).
namespace cf {

// Abstract vtable view of the battle actor polled by refreshPtStateAccum: only the
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
} // namespace cf

// ---------------------------------------------------------------------------
// UnkClass_80192BF4: PT-state FX/timer accumulator
// ---------------------------------------------------------------------------


// us-80194310 - FUNC_80192BF4 constructor.
// Initializes the counter/timer/wealth block. Byte-identical (implicit float pool).
cf::UnkClass_80192BF4::UnkClass_80192BF4() {
    field_0x00 = 0;
    field_0x04 = 0.0f;
    field_0x08 = 1.0f;
}

// Initializes a UnkClass_80192BF4-like struct at offsets 0x00/0x04/0x08.
// Retail-unmangled symbol name (see refreshPtStateAccum).
extern "C" void __ct__80192C10(cf::UnkClass_80192BF4* self) {
    self->field_0x00 = 0;
    self->field_0x04 = 0.0f;
    self->field_0x08 = 1.0f;
}

// us-80194348 - refreshPtStateAccum.
// If the accumulator is still positive, bump the counter, then query the
// passed actor (vtable slot 0x308) for a count and store (count*2+6) into both
// the value and the timer fields.
// Retail-unmangled symbol: MWCC would mangle a plain free function here, so
// C linkage keeps the exact linker name.
extern "C" void refreshPtStateAccum(cf::UnkClass_80192BF4* self, cf::CMenuPtStateActor* obj) {
    if (self->field_0x04 > 0.0f) {
        self->field_0x00++;
    }

    // Virtual call through slot 0x308 (getCount) -- MWCC stages the vtable
    // through r12 for real member virtual calls.
    int count = obj->getCount();

    f32 val = (f32)(count * 2 + 6);
    self->field_0x08 = val;
    self->field_0x04 = val;
}

// ---------------------------------------------------------------------------
// updatePtStateTimer: continuous PT-state timer update
// ---------------------------------------------------------------------------

// Opaque list holder/list views (CPartyStateWin.hpp declares the shared
// retail-unmangled helper family with void* parameters).
struct CEnumList {
    u8 _00[0x620];
    u32 count; // 0x620
};

// Scan the battle object list for any actor carrying status 0x10 or 0xf.
static inline bool hasPtStateActor() {
    CfEnumListHolder holder;
    startEnumObjects(holder.get(), 0x80000000, 0);

    u8* actor;
    u32 i = 0;
    // List handle is re-fetched twice per iteration (count check + element
    // fetch); never cached in a register across calls.
    while (((CEnumList*)holder.get())->count > i) {
        // Test-before-copy shape: retail tests r3 straight from the call,
        // copies it, then subtracts the record offset when non-null.
        actor = (u8*)getObjectAt(holder.get(), i);
        if (actor != nullptr) {
            actor -= 0x3e9c;
        }
        if (isBattleStatusActive(actor + 8, 0x10) != 0 ||
            isBattleStatusActive(actor + 8, 0xf) != 0) {
            return true;
        }
        i++;
    }
    return false;
}

// us-801943cc - updatePtStateTimer.
// If the timer is still running (>threshold), scan the battle object list for any
// actor carrying status 0x10 or 0xf. If none is found, decay the timer by the
// per-frame delta; when it crosses zero reset the whole block.
// NOTE: the element address is (elem - 0x3e9c) + 8, i.e. the status getter is
// invoked on a fixed header offset within the battle-actor record.
// Retail-unmangled symbol (see refreshPtStateAccum).
extern "C" void updatePtStateTimer(cf::UnkClass_80192BF4* self) {
    // Written as a '>' guard so MWCC tests the GT bit (plain bge-style skip),
    // matching retail; '<' forms emit an extra cror.
    if (self->field_0x04 > 0.0f) {
        bool found = hasPtStateActor();
        if (!found) {
            // No matching actor: decay the timer; when it crosses the threshold,
            // reset the block.
            // The product is materialized separately -- writing it as one
            // expression makes MWCC contract to fnmsubs, which retail lacks.
            f32 decay = 0.033333335f * Scn_GetFrameDelta(lbl_eu_80663E14);
            self->field_0x04 -= decay;
            if (self->field_0x04 <= 0.0f) {
                self->field_0x00 = 0;
                self->field_0x04 = 0.0f;
                self->field_0x08 = 1.0f;
            }
        }
    }
}


#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/CVision.hpp"
// NOTE: CfObjectActor.hpp deliberately not included - it drags in
// CAIAction.hpp whose C-linkage import decls (func_80174C98/func_8009EC9C)
// conflict with the owner declarations in CfMapItemManager.hpp.
namespace cf { class CfObjectActor; }
#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/math/Random.hpp"
#include <new>
#include <cstring>

// C-linkage pseudo-imports for this TU now live in the "C-linkage imports"
// section of kyoshin/cf/chain/CChainActorList.hpp (real imports).

// Search helper for func_8027B770: written with return-inside-loop so MWCC
// inlines it with the retail two-branch shape (bne-next / b-merge).
static cf::CChainActor* searchActorByKey(cf::CChainActorList* self, u32 key);
// Head-pointer variant used by func_8027B770: taking the sentinel node
// instead of the list keeps no live copy of `self` across the inline
// boundary, freeing r5/r6 for head/node like retail.
static cf::CChainActor* searchActorByHead(_reslist_node<cf::CChainActor*>* head, u32 key);
// Call-site overload (s32) so func_8027CBE8 emits a real bl instead of
// inlining the u32 definition below (retail keeps the call; the definition's
// u32 overload is what matches retail func_8027BFE0's bytes).
void func_8027BFE0(s32 param);
// noinline: -inline auto would inline this same-TU helper at call sites, but
// retail emits a real bl func_8027C45C (MWCC_CASES §8720).
extern "C" __declspec(noinline) void func_8027C45C(cf::CChainList* self);

// Battle-object vtable shim for func_8027C1A8: slot 162 (+0x290) is a
// gate/value query and slot 194 (+0x308) returns the tuning-pool selector.
// kyoshin compiles with -RTTI on, so declared virtual #k lands at vtable byte
// offset (k+2)*4.
class CChainBattleVtIf {
public:
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
    virtual void v023() = 0;
    virtual void v024() = 0;
    virtual void v025() = 0;
    virtual void v026() = 0;
    virtual void v027() = 0;
    virtual void v028() = 0;
    virtual void v029() = 0;
    virtual void v030() = 0;
    virtual void v031() = 0;
    virtual void v032() = 0;
    virtual void v033() = 0;
    virtual void v034() = 0;
    virtual void v035() = 0;
    virtual void v036() = 0;
    virtual void v037() = 0;
    virtual void v038() = 0;
    virtual void v039() = 0;
    virtual void v040() = 0;
    virtual void v041() = 0;
    virtual void v042() = 0;
    virtual void v043() = 0;
    virtual void v044() = 0;
    virtual void v045() = 0;
    virtual void v046() = 0;
    virtual void v047() = 0;
    virtual void v048() = 0;
    virtual void v049() = 0;
    virtual void v050() = 0;
    virtual void v051() = 0;
    virtual void v052() = 0;
    virtual void v053() = 0;
    virtual void v054() = 0;
    virtual void v055() = 0;
    virtual void v056() = 0;
    virtual void v057() = 0;
    virtual void v058() = 0;
    virtual void v059() = 0;
    virtual void v060() = 0;
    virtual void v061() = 0;
    virtual void v062() = 0;
    virtual void v063() = 0;
    virtual void v064() = 0;
    virtual void v065() = 0;
    virtual void v066() = 0;
    virtual void v067() = 0;
    virtual void v068() = 0;
    virtual void v069() = 0;
    virtual void v070() = 0;
    virtual void v071() = 0;
    virtual void v072() = 0;
    virtual void v073() = 0;
    virtual void v074() = 0;
    virtual void v075() = 0;
    virtual void v076() = 0;
    virtual void v077() = 0;
    virtual void v078() = 0;
    virtual void v079() = 0;
    virtual void v080() = 0;
    virtual void v081() = 0;
    virtual void v082() = 0;
    virtual void v083() = 0;
    virtual void v084() = 0;
    virtual void v085() = 0;
    virtual void v086() = 0;
    virtual void v087() = 0;
    virtual void v088() = 0;
    virtual void v089() = 0;
    virtual void v090() = 0;
    virtual void v091() = 0;
    virtual void v092() = 0;
    virtual void v093() = 0;
    virtual void v094() = 0;
    virtual void v095() = 0;
    virtual void v096() = 0;
    virtual void v097() = 0;
    virtual void v098() = 0;
    virtual void v099() = 0;
    virtual void v100() = 0;
    virtual void v101() = 0;
    virtual void v102() = 0;
    virtual void v103() = 0;
    virtual void v104() = 0;
    virtual void v105() = 0;
    virtual void v106() = 0;
    virtual void v107() = 0;
    virtual void v108() = 0;
    virtual void v109() = 0;
    virtual void v110() = 0;
    virtual void v111() = 0;
    virtual void v112() = 0;
    virtual void v113() = 0;
    virtual void v114() = 0;
    virtual void v115() = 0;
    virtual void v116() = 0;
    virtual void v117() = 0;
    virtual void v118() = 0;
    virtual void v119() = 0;
    virtual void v120() = 0;
    virtual void v121() = 0;
    virtual void v122() = 0;
    virtual void v123() = 0;
    virtual void v124() = 0;
    virtual void v125() = 0;
    virtual void v126() = 0;
    virtual void v127() = 0;
    virtual void v128() = 0;
    virtual void v129() = 0;
    virtual void v130() = 0;
    virtual void v131() = 0;
    virtual void v132() = 0;
    virtual void v133() = 0;
    virtual void v134() = 0;
    virtual void v135() = 0;
    virtual void v136() = 0;
    virtual void v137() = 0;
    virtual void v138() = 0;
    virtual void v139() = 0;
    virtual void v140() = 0;
    virtual void v141() = 0;
    virtual void v142() = 0;
    virtual void v143() = 0;
    virtual void v144() = 0;
    virtual void v145() = 0;
    virtual void v146() = 0;
    virtual void v147() = 0;
    virtual void v148() = 0;
    virtual void v149() = 0;
    virtual void v150() = 0;
    virtual void v151() = 0;
    virtual void v152() = 0;
    virtual void v153() = 0;
    virtual void v154() = 0;
    virtual void v155() = 0;
    virtual void v156() = 0;
    virtual void v157() = 0;
    virtual void v158() = 0;
    virtual void v159() = 0;
    virtual int v160() = 0;  // slot 162 / +0x290 gate query
    virtual void v161() = 0;
    virtual void v162() = 0;
    virtual void v163() = 0;
    virtual void v164() = 0;
    virtual void v165() = 0;
    virtual void v166() = 0;
    virtual void v167() = 0;
    virtual void v168() = 0;
    virtual void v169() = 0;
    virtual void v170() = 0;
    virtual void v171() = 0;
    virtual void v172() = 0;
    virtual void v173() = 0;
    virtual void v174() = 0;
    virtual void v175() = 0;
    virtual void v176() = 0;
    virtual void v177() = 0;
    virtual void v178() = 0;
    virtual void v179() = 0;
    virtual void v180() = 0;
    virtual void v181() = 0;
    virtual void v182() = 0;
    virtual void v183() = 0;
    virtual void v184() = 0;
    virtual void v185() = 0;
    virtual void v186() = 0;
    virtual void v187() = 0;
    virtual void v188() = 0;
    virtual void v189() = 0;
    virtual void v190() = 0;
    virtual void v191() = 0;
    virtual int v192() = 0;  // slot 194 / +0x308 pool selector
};

// Vtable-layout shim so the arts-set getter (vtable slot 0x9f = offset 0x27c)
// can be invoked as a genuine virtual call (MWCC uses r12 for virtual calls).
class CArtsSetOwnerShim {
public:
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
    virtual void v023() = 0;
    virtual void v024() = 0;
    virtual void v025() = 0;
    virtual void v026() = 0;
    virtual void v027() = 0;
    virtual void v028() = 0;
    virtual void v029() = 0;
    virtual void v030() = 0;
    virtual void v031() = 0;
    virtual void v032() = 0;
    virtual void v033() = 0;
    virtual void v034() = 0;
    virtual void v035() = 0;
    virtual void v036() = 0;
    virtual void v037() = 0;
    virtual void v038() = 0;
    virtual void v039() = 0;
    virtual void v040() = 0;
    virtual void v041() = 0;
    virtual void v042() = 0;
    virtual void v043() = 0;
    virtual void v044() = 0;
    virtual void v045() = 0;
    virtual void v046() = 0;
    virtual void v047() = 0;
    virtual void v048() = 0;
    virtual void v049() = 0;
    virtual void v050() = 0;
    virtual void v051() = 0;
    virtual void v052() = 0;
    virtual void v053() = 0;
    virtual void v054() = 0;
    virtual void v055() = 0;
    virtual void v056() = 0;
    virtual void v057() = 0;
    virtual void v058() = 0;
    virtual void v059() = 0;
    virtual void v060() = 0;
    virtual void v061() = 0;
    virtual void v062() = 0;
    virtual void v063() = 0;
    virtual void v064() = 0;
    virtual void v065() = 0;
    virtual void v066() = 0;
    virtual void v067() = 0;
    virtual void v068() = 0;
    virtual void v069() = 0;
    virtual void v070() = 0;
    virtual void v071() = 0;
    virtual void v072() = 0;
    virtual void v073() = 0;
    virtual void v074() = 0;
    virtual void v075() = 0;
    virtual void v076() = 0;
    virtual void v077() = 0;
    virtual void v078() = 0;
    virtual void v079() = 0;
    virtual void v080() = 0;
    virtual void v081() = 0;
    virtual void v082() = 0;
    virtual void v083() = 0;
    virtual void v084() = 0;
    virtual void v085() = 0;
    virtual void v086() = 0;
    virtual void v087() = 0;
    virtual void v088() = 0;
    virtual void v089() = 0;
    virtual void v090() = 0;
    virtual void v091() = 0;
    virtual void v092() = 0;
    virtual void v093() = 0;
    virtual void v094() = 0;
    virtual void v095() = 0;
    virtual void v096() = 0;
    virtual void v097() = 0;
    virtual void v098() = 0;
    virtual void v099() = 0;
    virtual void v100() = 0;
    virtual void v101() = 0;
    virtual void v102() = 0;
    virtual void v103() = 0;
    virtual void v104() = 0;
    virtual void v105() = 0;
    virtual void v106() = 0;
    virtual void v107() = 0;
    virtual void v108() = 0;
    virtual void v109() = 0;
    virtual void v110() = 0;
    virtual void v111() = 0;
    virtual void v112() = 0;
    virtual void v113() = 0;
    virtual void v114() = 0;
    virtual void v115() = 0;
    virtual void v116() = 0;
    virtual void v117() = 0;
    virtual void v118() = 0;
    virtual void v119() = 0;
    virtual void v120() = 0;
    virtual void v121() = 0;
    virtual void v122() = 0;
    virtual void v123() = 0;
    virtual void v124() = 0;
    virtual void v125() = 0;
    virtual void v126() = 0;
    virtual void v127() = 0;
    virtual void v128() = 0;
    virtual void v129() = 0;
    virtual void v130() = 0;
    virtual void v131() = 0;
    virtual void v132() = 0;
    virtual void v133() = 0;
    virtual void v134() = 0;
    virtual void v135() = 0;
    virtual void v136() = 0;
    virtual void v137() = 0;
    virtual void v138() = 0;
    virtual void v139() = 0;
    virtual void v140() = 0;
    virtual void v141() = 0;
    virtual void v142() = 0;
    virtual void v143() = 0;
    virtual void v144() = 0;
    virtual void v145() = 0;
    virtual void v146() = 0;
    virtual void v147() = 0;
    virtual void v148() = 0;
    virtual void v149() = 0;
    virtual void v150() = 0;
    virtual void v151() = 0;
    virtual void v152() = 0;
    virtual void v153() = 0;
    virtual void v154() = 0;
    virtual void v155() = 0;
    virtual void v156() = 0;
    virtual cf::CArtsSet* getArtsSet() = 0; // slot 0x9f
};

// Fake single-inheritance "interface" mirroring CChainActor's manually-managed
// vtable at +0x70.  kyoshin compiles with -RTTI on, so every vtable carries two
// hidden typeinfo slots (byte 0 and 4) and DECLARED virtual #k lands at vtable
// byte offset (k+2)*4.  Casting a CChainActor* to this interface and calling a
// virtual makes MWCC emit retail's `lwz r12,0x70(r3); lwz r12,off(r12); bcctrl`
// dispatch instead of a function-pointer cast that colors the vptr load r4/r5.
class CChainActorVtIf {
public:
    u8 pad70[0x70];
    virtual void v000() = 0; // slot 2  / +8
    virtual void v001() = 0; // slot 3  / +12
    virtual void v002() = 0; // slot 4  / +16
    virtual void v003() = 0; // slot 5  / +20  (dead-actor destroy/remove)
    virtual void v004(int) = 0; // slot 6  / +24  (chain-combo advance; retail passes the target arg in r4)
    virtual void v005(int) = 0; // slot 7  / +28  (activate-with-flag)
    virtual int  v006(int) = 0; // slot 8  / +32  (chainable-vs-key/move check)
    virtual int  v007(int) = 0; // slot 9  / +36  (activate-with-flag)
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual void v010(int, cf::CChainActor*, int) = 0; // slot 12 / +48 (chain-link: target, next actor, flag)
    virtual void v011() = 0;
    virtual void v012() = 0;
    virtual void v013() = 0;
    virtual void v014() = 0;
    virtual int  v015(cf::CChainActor*) = 0; // slot 17 / +68 (run-chainable check)
    virtual int  v016(int) = 0; // slot 18 / +72  (chainable-against-key check)
    virtual int  v017() = 0; // slot 19 / +76  (activation gate)
    virtual void v018() = 0;
    virtual int  v019() = 0; // slot 21 / +84 (priority/arts value query)
    virtual int  v020() = 0; // slot 22 / +88 (actor value query)
    virtual int  v021() = 0; // slot 23 / +92  (chainable check)
    virtual void v022() = 0;
    virtual void v023() = 0;
    virtual int  v024() = 0; // slot 26 / +104 (run key query)
    virtual void v025() = 0;
    virtual void v026() = 0;
    virtual int  v027() = 0; // slot 29 / +116 (anti-gate: retail returns 0 when nonzero)
};

// Function-pointer types for CChainActor's manually-managed vtable (mVTable at
// 0x70).
typedef void (*CChainActorVFn)(cf::CChainActor*);
typedef int (*CChainActorIVFn)(cf::CChainActor*);
typedef int (*CChainActorIVFn2)(cf::CChainActor*, void*);
typedef int (*CChainActorChkFn)(cf::CChainActor*, int);
typedef int (*CChainActorChkFn2)(cf::CChainActor*, cf::CChainActor*);
typedef void (*CChainActorChainFn)(cf::CChainActor*, int, cf::CChainActor*, int);

namespace cf {
    // Retail ctor stores the manual vtables, zeroes the CChainTemp-ish buffer
    // and its flag byte. The dtor is the plain empty+delete shape (no base or
    // member dtor calls), matching retail's 0x40-byte body.
    CChainActorPc::CChainActorPc() {
        this->mVTable = (u32)&lbl_eu_80538290; // +0x70 actor vtable (temp)
        this->field_68 = (u32)&lbl_eu_80538338; // +0x68 sub-object vtable
        std::memset(this->field_4, 0, 0x60);
        this->field_64 = 0;
        this->unk6C = 0;
        this->unk0 = 0;
        this->mVTable = (u32)&lbl_eu_805384E0; // final vtable
        // Retail constructs the CChainEffect region at the END of the ctor
        // body (no dtor is ever emitted for it); call the ctor via its alias.
        __ct__Q22cf12CChainEffectFv((CChainEffect*)this->field_74);
    }
    CChainActorPc::~CChainActorPc() {}

    CChainActorEne::CChainActorEne() {
        this->mVTable = (u32)&lbl_eu_80538290; // +0x70 actor vtable (temp)
        this->field_68 = (u32)&lbl_eu_80538338; // +0x68 sub-object vtable
        std::memset(this->field_4, 0, 0x60);
        this->field_64 = 0;
        this->unk6C = 0;
        this->unk0 = 0;
        this->mVTable = (u32)&lbl_eu_80538458; // final vtable
        // See CChainActorPc ctor comment: ctor alias call keeps the dtor empty.
        __ct__Q22cf12CChainEffectFv((CChainEffect*)this->field_74);
    }
    CChainActorEne::~CChainActorEne() {}

    CChainActorList::CChainActorList(){
        mChainActorList.reserve(CWorkThreadSystem::getWorkMem(), 59);
        // Detach every node of the freshly built reslist (inlined clearList
        // shape). NOTE: every equivalent source shape here colors the walker
        // into r5 where retail uses r3 - a compiler-internal tie-break
        // (MWCC_CASES wall #11); this is the best-known form.
        _reslist_node<cf::CChainActor*>* nul = 0;
        _reslist_node<cf::CChainActor*>* p = mChainActorList.mStartNodePtr;
        p = p->mNext;
        _reslist_node<cf::CChainActor*>* node;
        while (mChainActorList.mStartNodePtr != p) {
            node = p;
            p = p->mNext;
            node->mNext = nul;
        }
        mChainActorList.mStartNodePtr->mNext = mChainActorList.mStartNodePtr;
        mChainActorList.mStartNodePtr->mPrev = mChainActorList.mStartNodePtr;
        func_802811FC(this);
        unk1DA8[0] = 0;
    }

    CChainActorList::~CChainActorList(){
        // Pass 1: destroy every actor through its manual vtable slot 17
        // (same body as func_8027B164, inlined by retail).
        _reslist_node<cf::CChainActor*>* node =
            mChainActorList.mStartNodePtr->mNext;
        while (node != mChainActorList.mStartNodePtr) {
            cf::CChainActor* actor = node->mItem;
            ((CChainActorVtIf*)actor)->v003();
            node = node->mNext;
        }
        // Pass 2: unlink every node, marking its slot free (mNext = 0).
        _reslist_node<cf::CChainActor*>* head = mChainActorList.mStartNodePtr;
        node = head->mNext;
        while (node != head) {
            _reslist_node<cf::CChainActor*>* cur = node;
            node = cur->mNext;
            _reslist_node<cf::CChainActor*>* prev = cur->mPrev;
            prev->mNext = node;
            node->mPrev = prev;
            cur->mNext = 0;
        }
        func_802811FC(this);
        unk1DA8[0] = 0;
        // Retail clears the ring once more before releasing the node array
        // (inlined reslist::clearList).
        mChainActorList.clearList();
        // Inlined reslist::destroyList.
        mChainActorList.clearList();
        if (mChainActorList.unk1C == false && mChainActorList.mList != nullptr) {
            delete[] mChainActorList.mList;
            mChainActorList.mList = nullptr;
        }
        mChainActorList.mCapacity = 0;
    }
}

// Appends @p p to the actor list at index mCount, then increments mCount.
// noinline + extern "C": retail callers emit a real bl to the bare symbol
// func_8027C5CC (MWCC_CASES §8720, §8717).
extern "C" __declspec(noinline) void func_8027C5CC(cf::CChainList* self, cf::CChainActor* p) {
    self->mActors[self->mCount++] = p;
}

// Returns the list entry whose first u32 field matches @p key.
cf::CChainActor* func_8027CA98(cf::CChainList* self, u32 key) {
    for (int i = 0; i < (int)self->mCount; i++) {
        if (self->mActors[i]->unk0 == key)
            return self->mActors[i];
    }
    return 0;
}

void func_8027B164(cf::CChainActorList* self){
    // Pass 1: call the vtable[5] (destroy/remove) hook on every actor.
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        ((CChainActorVtIf*)node->mItem)->v003();
        node = node->mNext;
    }
    // Pass 2: detach every node (marking each slot free via mNext = 0).
    _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
    node = head->mNext;
    while (node != head) {
        _reslist_node<cf::CChainActor*>* cur = node;
        node = cur->mNext;
        _reslist_node<cf::CChainActor*>* prev = cur->mPrev;
        prev->mNext = node;
        node->mPrev = prev;
        cur->mNext = 0;
    }
    func_802811FC(self);
    self->unk1DA8[0] = 0;
}
// Removes every reslist actor whose referenced object is dead.
void func_8027B200(cf::CChainActorList* self){
    _reslist_node<cf::CChainActor*>* node = self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        u32 base = node->mItem->unk0;
        if (base != 0) base += 0x3e9c;
        if (func_800B8920((void*)base) == 0) {
            cf::CChainActor* actor = node->mItem;
            ((CChainActorVtIf*)actor)->v003();
            // Pass the node's own item slot so the inlined remove compares
            // curr->mItem against a re-read r30->mItem (retail shape).
            self->mChainActorList.remove(node->mItem);
            node = node->mPrev;
        }
        node = node->mNext;
    }
}
// Walks the actor reslist. For each contiguous run of actors where every
// successor keeps vtable[17](first,next) nonzero, records the run members into
// one of two local arrays (chainable via vtable[8] go to chainArr, the rest to
// otherArr), then drives each array's chain/skill linkage through vtable[12].
void func_8027B2CC(cf::CChainActorList* self){
    // Stack order matters: retail puts otherArr at r1+0x8 and chainArr at
    // r1+0xf4; MWCC places the first-declared array at the higher slot.
    cf::CChainActor* chainArr[0x3B];
    cf::CChainActor* otherArr[0x3B];
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        int chainCount = 0;
        int otherCount = 0;
        int key = ((CChainActorVtIf*)node->mItem)->v024();
        _reslist_node<cf::CChainActor*>* cur = node;
        while (true) {
            // Both hooks run on the current node; the member is grabbed after
            // the chainable test so no node temp lives across the calls.
            ((CChainActorVtIf*)cur->mItem)->v002();
            if (((CChainActorVtIf*)cur->mItem)->v006(key) != 0) {
                cf::CChainActor* actor = cur->mItem;
                cur = cur->mNext;
                chainArr[chainCount++] = actor;
            } else {
                cf::CChainActor* actor = cur->mItem;
                cur = cur->mNext;
                otherArr[otherCount++] = actor;
            }
            if (cur == self->mChainActorList.mStartNodePtr) break;
            if (((CChainActorVtIf*)node->mItem)->v015(cur->mItem) == 0) break;
        }

        // Pass: link chainable and non-chainable actors into a ring.  When
        // there are at least two chainable actors AND the first passes its
        // vtable[19] gate, chain links use flag=1; otherwise flag=0.  The
        // last slot wraps around (next = i+1 < count ? i+1 : 0).
        int chk = 0;
        if (chainCount >= 2) {
            cf::CChainActor* first = chainArr[0];
            chk = (first != 0) ? ((CChainActorVtIf*)first)->v017() : 0;
        }
        if (chainCount >= 2 && chk != 0) {
            if (chainCount == 1) {
                ((CChainActorVtIf*)chainArr[0])->v010(0, 0, 0);
            } else if (chainCount == 2) {
                ((CChainActorVtIf*)chainArr[0])->v010(1, chainArr[1], 1);
                ((CChainActorVtIf*)chainArr[1])->v010(0, 0, 0);
            } else if (chainCount >= 3) {
                for (int i = 0; i < chainCount; i++) {
                    int next = (i + 1 < chainCount) ? i + 1 : 0;
                    ((CChainActorVtIf*)chainArr[i])->v010(1, chainArr[next], 1);
                }
            }
        } else {
            if (chainCount == 1) {
                ((CChainActorVtIf*)chainArr[0])->v010(0, 0, 0);
            } else if (chainCount == 2) {
                ((CChainActorVtIf*)chainArr[0])->v010(0, chainArr[1], 1);
                ((CChainActorVtIf*)chainArr[1])->v010(0, 0, 0);
            } else if (chainCount >= 3) {
                for (int i = 0; i < chainCount; i++) {
                    int next = (i + 1 < chainCount) ? i + 1 : 0;
                    ((CChainActorVtIf*)chainArr[i])->v010(0, chainArr[next], 1);
                }
            }
            // Retail tail-duplicates the non-chainable linkage in both arms.
            if (otherCount == 1) {
                ((CChainActorVtIf*)otherArr[0])->v010(0, 0, 0);
            } else if (otherCount == 2) {
                ((CChainActorVtIf*)otherArr[0])->v010(0, otherArr[1], 1);
                ((CChainActorVtIf*)otherArr[1])->v010(0, 0, 0);
            } else if (otherCount >= 3) {
                for (int i = 0; i < otherCount; i++) {
                    int next = (i + 1 < otherCount) ? i + 1 : 0;
                    ((CChainActorVtIf*)otherArr[i])->v010(0, otherArr[next], 1);
                }
            }
        }
        if (otherCount == 1) {
            ((CChainActorVtIf*)otherArr[0])->v010(0, 0, 0);
        } else if (otherCount == 2) {
            ((CChainActorVtIf*)otherArr[0])->v010(0, otherArr[1], 1);
            ((CChainActorVtIf*)otherArr[1])->v010(0, 0, 0);
        } else if (otherCount >= 3) {
            for (int i = 0; i < otherCount; i++) {
                int next = (i + 1 < otherCount) ? i + 1 : 0;
                ((CChainActorVtIf*)otherArr[i])->v010(0, otherArr[next], 1);
            }
        }

        node = cur;
    }
}
cf::CChainActor* func_8027B770(cf::CChainActorList* self, u32 key){
    // Inlined search: testing the call result directly keeps the value in
    // the return register r3 at the merge, matching retail.
    cf::CChainActor* found =
        searchActorByHead(self->mChainActorList.mStartNodePtr, key);
    if (found == 0) {
        cf::CChainActor* newActor = func_8028120C(self);
        func_8027B8C8(self, newActor);
        if (self->unk1DA8[0]) {
            ((CChainActorVtIf*)newActor)->v005(1);
        }
    } else {
        // Returning the found actor here colors the merged search result
        // into r3 (the return register), like retail; the not-found path
        // still falls through with unspecified r3.
        return found;
    }
}
// Removes the first actor whose unk0 matches @p key: destroys it via
// vtable[5] and unlinks its reslist node. Returns 1 if found, else 0.
int func_8027B814(cf::CChainActorList* self, u32 key) {
    _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
    _reslist_node<cf::CChainActor*>* node = head->mNext;
    while (node != head) {
        cf::CChainActor* actor = node->mItem;
        if (key == actor->unk0) {
            ((CChainActorVtIf*)actor)->v003();
            // Pass the node's own item slot: the inlined remove re-reads
            // node->mItem each iteration (retail keeps the node in r31).
            self->mChainActorList.remove(node->mItem);
            return 1;
        }
        node = node->mNext;
    }
    return 0;
}
// Inserts @p actor into $self's reslist ordered by the actors' priority
// values (vtable +0x54): walk from the head until an actor is chainable
// against @p actor (vtable +0x44), then skip past actors whose priority is
// <= @p actor's. The sentinel pointer is re-read from $self at every compare
// (retail never caches it). Uses an empty node slot from the preallocated
// node array, as reslist::insert does. Retail's symbol is the unmangled C-ABI
// name func_8027B8C8 (call sites must reference it unmangled).
extern "C" void func_8027B8C8(cf::CChainActorList* self, cf::CChainActor* actor) {
    int myVal = ((CChainActorVtIf*)actor)->v019();
    _reslist_node<cf::CChainActor*>* pre =
        self->mChainActorList.mStartNodePtr->mNext;
    while (pre != self->mChainActorList.mStartNodePtr) {
        if (((CChainActorVtIf*)pre->mItem)->v015(actor)) {
            while (pre != self->mChainActorList.mStartNodePtr &&
                   ((CChainActorVtIf*)pre->mItem)->v015(actor)) {
                if (myVal <
                    ((CChainActorVtIf*)pre->mItem)->v019()) goto place;
                pre = pre->mNext;
            }
            goto place;
        }
        pre = pre->mNext;
    }
place:;
    // Find an empty slot in the preallocated node array and link before @p pre.
    _reslist_node<cf::CChainActor*>* temp;
    int i;
    int byteOff;
    int capacity = self->mChainActorList.mCapacity;
    for (i = 0, byteOff = 0; i < capacity; i++) {
        if (*(void**)((u8*)self->mChainActorList.mList + byteOff) == 0) break;
        byteOff += 12;
    }
    temp = &self->mChainActorList.mList[i];
    temp->setItem(actor);
    temp->mNext = pre;
    temp->mPrev = pre->mPrev;
    pre->mPrev->mNext = temp;
    pre->mPrev = temp;
}
// Removes slave/target actors that have become chainable against @p target.
// First, if an actor directly references @p target (unk0 == target) it is
// activated via its vtable slot 20 and @p other is cleared. Then every
// remaining chainable actor (vtable slot 18 against target, and slot 8
// against target's move subobject) is appended to @p other and removed from
// the reslist.
// Return-inside-loop search helper: when inlined at func_8027BA0C's first
// phase, MWCC keeps the key in the caller's incoming param register and emits
// the retail two-branch shape (bne-next / b-merge with li r3,0 at the merge).
static cf::CChainActor* findActorByTarget(u32 key, cf::CChainActorList* self) {
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
    while (node != head) {
        cf::CChainActor* x = node->mItem;
        if (key == ((CChainActorKeyView*)x)->mTargetObj) return x;
        node = node->mNext;
    }
    return 0;
}

// Sweep body kept as a helper so MWCC inlines it with target as a fresh
// parameter web (retail recomputes target+0x3E9C per iteration instead of
// caching it across the loop).
static void sweepChainable(cf::CChainActorList* self, cf::CChainList* other,
                           cf::CfObjectActor* target) {
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        if (((CChainActorVtIf*)node->mItem)->v016((int)target) != 0) {
            if (((CChainActorVtIf*)node->mItem)->v006(
                    ((CChainSubVtIf*)&((CChainBattleObjTail*)target)
                         ->field_0x3E9C)
                        ->v017()) != 0) {
                func_8027C5CC(other, node->mItem);
                self->mChainActorList.remove(node->mItem);
                node = node->mPrev;
            }
        }
        node = node->mNext;
    }
}

void func_8027BA0C(cf::CChainActorList* self, cf::CChainList* other,
                   cf::CfObjectActor* target) {
    // 1. Activate the resident actor whose unk0 references @p target.
    cf::CChainActor* found = findActorByTarget((u32)target, self);
    if (found != 0) ((CChainActorVtIf*)found)->v018();
    func_8027C45C(other);
    // 2. Sweep every actor; the ones chainable against target move into @p other.
    sweepChainable(self, other, target);
}
// Head-pointer search body (see forward declaration above).
static cf::CChainActor* searchActorByHead(_reslist_node<cf::CChainActor*>* head,
                                          u32 key) {
    _reslist_node<cf::CChainActor*>* node = head->mNext;
    while (node != head) {
        if (key == node->mItem->unk0) return node->mItem;
        node = node->mNext;
    }
    return 0;
}
// BB4C-private search: no head local, so the sentinel is an unnamed temp
// colored into r6 while the walker takes r7 (retail shape). Best-known
// shape (banked draft): structural bytes match retail exactly; the residual
// is a fixed r6/r7 Chaitin swap between the sentinel and the walker inside
// the inlined loop (see open-item notes).
static cf::CChainActor* findActorForBB4C(u32 key, cf::CChainActorList* self) {
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    _reslist_node<cf::CChainActor*>* head =
        self->mChainActorList.mStartNodePtr;
    cf::CChainActor* x;
    while (node != head) {
        x = node->mItem;
        if (key == x->unk0) return x;
        node = node->mNext;
    }
    return 0;
}

// For each actor in @p list, ensures it is present in @p self's reslist
// (adding it via func_8027B8C8 when missing), then clears @p list.
void func_8027BB4C(cf::CChainActorList* self, cf::CChainList* list){
    cf::CChainList* l = list;
    for (int i = 0; i < (int)l->mCount; i++) {
        cf::CChainActor* actor = (i < (int)list->mCount) ? list->mActors[i] : 0;
        if (findActorForBB4C(((CChainActorKeyView*)actor)->mTargetObj, self) == 0)
            func_8027B8C8(self, actor);
    }
    func_8027C45C(list);
}
// Same-TU search helper inlined at func_8027BC14. Argument order (self, key)
// reproduces retail's register coloring (the located actor lands in r31).
static cf::CChainActor* findActorForKey(cf::CChainActorList* self, u32 key) {
    _reslist_node<cf::CChainActor*>* head =
        self->mChainActorList.mStartNodePtr;
    _reslist_node<cf::CChainActor*>* node = head->mNext;
    while (node != head) {
        cf::CChainActor* x = node->mItem;
        if (key == ((CChainActorKeyView*)x)->mTargetObj) return x;
        node = node->mNext;
    }
    return 0;
}

// Chain-activation validator for the actor whose unk0 == @p key. Returns 1
// when the candidate passes its activation gates AND at least two actors in
// the list are chainable against it, else 0.
int func_8027BC14(cf::CChainActorList* self, u32 key){
    // Locate the actor whose unk0 matches key.
    cf::CChainActor* actor = findActorForKey(self, key);
    if (actor == 0) return 0;
    // The candidate must pass its own activation gate (vtable[19]).
    int gate = (actor != 0) ? ((CChainActorVtIf*)actor)->v017() : 0;
    if (gate == 0) return 0;
    // ... and must be chainable against the key's move sub-object.
    if (((CChainActorVtIf*)actor)->v006(
            ((CChainSubVtIf*)&((CChainBattleObjTail*)key)->field_0x3E9C)
                ->v017()) == 0)
        return 0;
    // ... and must pass the "already chained" anti-gate (vtable[29]).
    if (((CChainActorVtIf*)actor)->v027() != 0) return 0;
    // The battle object's probed address must not be in any of three states.
    if (func_80148778(&((CChainTargetObj*)key)->field_8, 0xeb) != 0) return 0;
    if (func_80148778(&((CChainTargetObj*)key)->field_8, 0xcb) != 0) return 0;
    if (func_80148778(&((CChainTargetObj*)key)->field_8, 0xf8) != 0) return 0;
    // The battle object's arts-selection state must not match the no-chain id.
    int local = *((CChainSubVtIf*)((CChainTargetObj*)key)->field_4)->v010();
    if (func_80174C98((CChainTargetObj*)key, &local, 0x1f) != 0) return 0;
    if (func_8004C5EC((void*)((CChainBattleObjTail*)key)->field_0x3F60) == 0x31)
        return 0;
    if (((CChainActorVtIf*)actor)->v007(1) == 0) return 0;
    // Count how many other actors are chainable against the candidate. The
    // sentinel is NOT cached here: retail re-reads self->... every iteration.
    int count = 0;
    _reslist_node<cf::CChainActor*>* cur;
    for (cur = self->mChainActorList.mStartNodePtr->mNext;
         cur != self->mChainActorList.mStartNodePtr; cur = cur->mNext) {
        if (((CChainActorVtIf*)cur->mItem)->v016((int)key) != 0) {
            if (((CChainActorVtIf*)cur->mItem)->v006(
                    ((CChainSubVtIf*)&((CChainBattleObjTail*)key)->field_0x3E9C)
                        ->v017()) != 0) {
                count++;
            }
        }
    }
    // Retail's bit-twiddled "count >= 2" test: bit 31 of
    // ((count^1)>>1) - ((count^1)&count) is set exactly then.
    int t = count ^ 1;
    return ((u32)((t >> 1) - (t & count))) >> 31;
}
// Returns 1 if some reslist actor is a valid chain-activation target, else 0.
// The target object (actor->unk0) is re-read before each probe so it stays in
// temps across the func_80148778 calls; only the post-check read lives in a
// callee-saved register (retail r31), matching the func_8027B200 pattern.
int func_8027BE84(cf::CChainActorList* self){
    _reslist_node<cf::CChainActor*>* node;
    CChainTargetObj* obj;
    node = self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        if (func_80148778(&((CChainTargetObj*)node->mItem->unk0)->field_8, 0x10c) == 0 &&
            func_80148778(&((CChainTargetObj*)node->mItem->unk0)->field_8, 0xf8) == 0) {
            obj = (CChainTargetObj*)node->mItem->unk0;
            int local = *((CChainSubVtIf*)((CChainTargetObj*)node->mItem->unk0)->field_4)->v010();
            if (func_80174C98(obj, &local, 0x801) != 0) return 1;
        }
        node = node->mNext;
    }
    return 0;
}
// When the chain flag bit 1 is set, query a value and emit threshold events.
void func_8027BF58(cf::CChainFlag* self) {
    if (self->field_0x3F00 & 2) {
        u32 v = func_8027EE88(0x2f, 1);
        if (v >= 1) {
            func_8027EEF4(0x2f);
        }
        if (v >= 0x32) {
            func_8027EEF4(0x30);
        }
        if (v >= 0xc8) {
            func_8027EEF4(0x31);
        }
        if (v >= 0x3e8) {
            func_8027EEF4(0x32);
        }
        lbl_eu_80662A80 = 1;
    }
}

void func_8027C040(cf::CChainFlag* self) {
    if (self->field_0x3F00 & 2) {
        lbl_eu_80662A80++;
        if (lbl_eu_80662A80 >= 4) {
            func_8027EEF4(0x36);
        }
        if (lbl_eu_80662A80 >= 5) {
            func_8027EEF4(0x37);
        }
    }
}
// Resets the chain-chance state: clears the chain count, both step counters
// and the step flag byte.
void func_8027C098(cf::CChainChanceS* self) {
    self->mChainCount = 0;
    self->mField08 = 0;
    self->mField0A = 0;
    self->mField0C = 0;
}
// Decrements the chain count; plays the indexed chain sound at the wrap
// point and advances the step counters (field_8 / field_A wraps at 4).
void func_8027C0B0(cf::CChainChanceS* self) {
    int flag;
    s16 count = self->mChainCount;
    if (count < 1) {
        flag = 0;
    } else {
        count--;
        self->mChainCount = count;
        flag = count <= 0;
    }
    if (flag != 0) {
        float snd = lbl_eu_8050EDE0[self->mField0A];
        func_80133F48(3, snd);
        // Both counters are read before either store; the wrap check uses
        // the already-updated field0A value.
        int nextB = self->mField08 + 1;
        int nextA = self->mField0A + 1;
        self->mField0A = nextA;
        self->mField08 = nextB;
        if ((s16)nextA >= 4) self->mField0A = 3;
    }
}
// Rolls the dice on a chain activation between battle objects @p objA and
// @p objB under the chain state in @p self. The threshold comes from the
// tuning-pool record selected by objA's vtable(+0x308) query, offset by one
// 0x14-byte record when the step flag at +0xC is consumed, and scaled by the
// pair value capped at 5000. Succeeds when mtRand(100) exceeds the score.
// C linkage comes from the func_8027C1A8 declaration in the TU header
// (retail callers bl the unmangled label; MWCC_CASES func_800F41A0 record).
int func_8027C1A8(cf::CChainChanceS* self,
                  CChainActorObjId* objA,
                  CChainActorObjId* objB) {
    if (self->mField08 >= 0xf) {
        return 0;
    }
    int tblOff = lbl_eu_8050EDD0[((CChainBattleVtIf*)objA)->v192()];
    u32 pair = (u16)getTableValueByPair__Q22cf13CfGameManagerFv(
        objA->field_0x3F28, objB->field_0x3F28);
    u32 val;
    if (((CChainBattleVtIf*)objA)->v160() != 0) {
        ((CChainBattleVtIf*)objA)->v160();
        val = (u16)func_8025FB10(objA, 0x6b);
    } else {
        val = 0;
    }

    // Consume the step flag: while set, bump the accumulator by 0x14.
    int fv = self->mField0C;
    self->mField0C = 0;
    int flagOff = ((-fv | fv) >> 31) & 0x14;

    u32 cnt = pair > 0x1388 ? 0x1388 : pair;
    // u32->f64 via the 0x4330 biased-magic bit pattern; the pool double is the
    // matching de-bias constant, so one fsubs finishes the conversion.
    union { u32 w[2]; double d; } conv;
    conv.w[1] = cnt;
    conv.w[0] = 0x43300000;
    double base = lbl_eu_80668A78;
    int off2 = (int)(val + tblOff) + flagOff;
    int q1 = (int)(lbl_eu_80668A68 * (conv.d - base) / lbl_eu_80668A6C);
    u32 acc = (u16)(off2 + q1);
    if (pair > 0x1388) {
        u32 over = pair - 0x1388;
        conv.w[1] = over;
        conv.w[0] = 0x43300000;
        int q2 = (int)(lbl_eu_80668A70 * (conv.d - base) / lbl_eu_80668A6C);
        acc = (u16)(acc + q2);
    }
    int rnd = ml::math::mtRand(100);
    int mixed = (int)acc ^ rnd;
    return (u32)((mixed >> 1) - (mixed & (int)acc)) >> 31;
}
// Starts a chain message; writes 0xa to the message id on success.
int func_8027C154(cf::CChainMsg* self) {
    // Retail's call site is a stale-register call: no arguments are set up.
    // Route through an unprototyped pointer type so the call passes none;
    // MWCC folds the constant function address back into a direct bl.
    typedef int (*UnprotoChainRollFn)();
    if (((UnprotoChainRollFn)func_8027C1A8)() != 0) {
        func_802A07F4(0xc3, 0);
        self->field_0x0 = 0xa;
        return 1;
    }
    return 0;
}
// Emits chain-tally threshold events based on the accumulated counter.
// func_8027CBE8 calls the s32 overload declared above (a different mangled
// symbol), so this u32 definition is never inlined there and stays a
// byte-identical match.
void func_8027BFE0(unsigned int param) {
    if (param >= 0xBB8) {
        func_8027EEF4(0x33);
    }
    if (param >= 0x7530) {
        func_8027EEF4(0x34);
    }
    if (param >= 0x186A0) {
        func_8027EEF4(0x35);
    }
}
// Reads the current pad press. If a chain-trigger button is held, performs the
// action selected by func_8017FD4C; writes result to *out and returns 1.
int func_8027C33C(cf::CChainAction* self, u8* out){
    if (self->field_0 > 0) return 0;
    int sel = func_8017FD44();
    if (sel == 0) {
        *out = 0;
        return 1;
    }
    CPad* pad = cf::CfGameManager::getCurrentPad();
    // The chain-trigger pad bit depends on controller type (classic vs wii).
    u32 bit = (isClassicController__Q22cf13CfGameManagerFv(-1) != 0)
                  ? (pad->mPressedButtonFlags >> 22) & 1
                  : (pad->mPressedButtonFlags >> 5) & 1;
    if (bit != 0) {
        int action = func_8017FD4C(sel);
        // Sparse dispatch: goto-chain keeps the retail compare-chain layout with
        // the case bodies appended after the tests (if/else-if would inline them).
        if (action == 1) goto case1;
        if (action == 2) goto case2;
        self->field_0xc = 0;
        *out = 0;
        return 1;
    case1:
        func_802A07F4(0xca, 0);
        self->field_0xc = 0;
        *out = 1;
        return 1;
    case2:
        func_802A07F4(0xc9, 0);
        self->field_0xc = 1;
        *out = 1;
        return 1;
    }
    return 0;
}
// Zeroes the actor list: clears the pointer array, count, and flag.
// noinline + extern "C": retail callers emit a real bl to the bare symbol
// func_8027C45C (MWCC_CASES §8720, §8717).
extern "C" __declspec(noinline) void func_8027C45C(cf::CChainList* self) {
    memset(self->mActors, 0, sizeof(self->mActors));
    self->mCount = 0;
    self->mFlag = 0;
}
// Removes from @p self every actor whose referenced object is dead
// (func_800B8920), calling vtable[5] and shifting the array down.
void func_8027C49C(cf::CChainList* self){
    cf::CChainActor** p;
    int i = 0;
    while (i < (int)self->mCount) {
        p = &self->mActors[i];
        u32 base = (*p)->unk0;
        if (base != 0) base += 0x3e9c;
        if (func_800B8920((void*)base) == 0) {
            cf::CChainActor* actor = *p;
            ((CChainActorVtIf*)actor)->v003();
            if ((int)self->mCount - i - 1 > 0) {
                memcpy(p, p + 1, ((int)self->mCount - 1 - i) * 4);
            }
            self->mCount--;
            i--;
        }
        i++;
    }
}

// Calls vtable[4] on every actor in the list (r12 bidirectional dispatch via
// the manual vtable at +0x70).
void func_8027C560(cf::CChainList* self) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* a = self->mActors[i];
        ((CChainActorVtIf*)a)->v002();
    }
}
// Removes the actor whose unk0 matches @p key (if any); returns whether found.
int func_8027C5E4(cf::CChainList* self, u32 key){
    for (int i = 0; i < (int)self->mCount; i++) {
        if (self->mActors[i]->unk0 == key) {
            cf::CChainActor** p = &self->mActors[i];
            cf::CChainActor* actor = self->mActors[i];
            ((CChainActorVtIf*)actor)->v003();
            if ((int)self->mCount - i - 1 > 0) {
                memcpy(p, p + 1, ((int)self->mCount - 1 - i) * 4);
            }
            self->mCount--;
            self->mFlag = 1;
            return 1;
        }
    }
    return 0;
}

// One chain-link step: fetch the actor at @p slotIdx (null when out of
// range), compute the successor index (the retail min idiom saturates to 0,
// not count, when slotIdx+1 is out of range), and drive the chain vtable[12]
// call. The current actor is fetched through an element pointer (retail's
// strength-reduced moving pointer inside the loops) while the successor uses
// the computed base+index form, exactly like retail.
#define CHAIN_LINK_BODY(idxVar, slotExpr) \
    do { \
        cf::CChainActor* actor = ((idxVar) < (int)self->mCount) ? (slotExpr) : 0; \
        int next = (((idxVar) + 1 < (int)self->mCount) ? (idxVar) + 1 : 0); \
        cf::CChainActor* nextActor = \
            (next < (int)self->mCount) ? self->mActors[next] : 0; \
        ((CChainActorVtIf*)actor)->v010(target, nextActor, 0); \
    } while (0)

// Drives the chain-link call for one slot (@p index), or for every slot when
// @p index is -1: the -1 path nests three slot loops (retail's dead i/j/k ==
// -1 guards fall through, and the != -1 single-slot blocks come after), with
// the innermost re-entering this function per slot.
// extern "C" definition (same idiom as func_8027B8C8/func_8027C45C): retail's
// symbol is the bare name func_8027C6B4, and the recursive innermost-loop
// call must emit `bl func_8027C6B4` (unmangled) to match retail's reloc.
extern "C" void func_8027C6B4(cf::CChainList* self, int target, int index){
    int i, j, k, m;
    cf::CChainActor** it;
    cf::CChainActor** jt;
    cf::CChainActor** kt;
    if (index == -1) {
        for (i = 0, it = self->mActors; i < (int)self->mCount; i++, it++) {
            if (i == -1) {
                for (j = 0, jt = self->mActors; j < (int)self->mCount; j++, jt++) {
                    if (j == -1) {
                        for (k = 0, kt = self->mActors; k < (int)self->mCount;
                             k++, kt++) {
                            if (k == -1) {
                                for (m = 0; m < (int)self->mCount; m++) {
                                    func_8027C6B4(self, target, m);
                                }
                            } else {
                                CHAIN_LINK_BODY(k, *kt);
                            }
                        }
                    } else {
                        CHAIN_LINK_BODY(j, *jt);
                    }
                }
            } else {
                CHAIN_LINK_BODY(i, *it);
            }
        }
    } else {
        CHAIN_LINK_BODY(index, self->mActors[index]);
    }
}
// For each actor while @p target is nonzero, advances the chain combo
// (vtable[6]) and if the actor is chainable (vtable[23]) and there is a
// distinct next actor, accumulates the arts pair via addTableValueWithClamp.
// The actor pointer is re-read from the list before each use (retail reloads
// *r31 after every call), and the next-index saturation is `(i+1 < count)
// ? i+1 : 0` (the retail min idiom yields 0, not count, when i+1 >= count).
void func_8027C924(cf::CChainList* self, int target){
    for (int i = 0; i < (int)self->mCount; i++) {
        ((CChainActorVtIf*)self->mActors[i])->v004(target);
        if (target != 0 && ((CChainActorVtIf*)self->mActors[i])->v021() != 0) {
            int next = (i + 1 < (int)self->mCount) ? (i + 1) : 0;
            if (i != next) {
                cf::CChainActor* other = self->mActors[next];
                addTableValueWithClamp__Q22cf13CfGameManagerFv(
                    ((CChainActorObjId*)self->mActors[i]->unk0)->field_0x3F28,
                    ((CChainActorObjId*)other->unk0)->field_0x3F28, 0xa);
            }
        }
    }
}
// Returns 1 if any resident actor's vtable[22] (+0x58) value matches @p key.
int func_8027CA0C(cf::CChainList* self, int key) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* actor = self->mActors[i];
        if (((CChainActorVtIf*)actor)->v020() == key)
            return 1;
    }
    return 0;
}
// Checks (via func_80174C98) whether the resident actors satisfy @p condition;
// the polarity of the result depends on @p check.
int func_8027CAE0(cf::CChainList* self, int target, int check){
    cf::CChainActor** p;
    CChainTargetObj* obj;
    if (check == 0) {
        obj = 0;
        p = self->mActors;
        for (int i = 0; i < (int)self->mCount; i++) {
            obj = (CChainTargetObj*)(*p)->unk0;
            int local = *((CChainSubVtIf*)obj->field_4)->v010();
            if (func_80174C98(obj, &local, target) != 0) return 1;
            p++;
        }
        return 0;
    }
    obj = 0;
    p = self->mActors;
    for (int i = 0; i < (int)self->mCount; i++) {
        obj = (CChainTargetObj*)(*p)->unk0;
        int local = *((CChainSubVtIf*)obj->field_4)->v010();
        if (func_80174C98(obj, &local, target) == 0) return 0;
        p++;
    }
    return 1;
}
// If the counter is positive, runs chain update steps and resets it.
void func_8027CBE8(cf::CChainCounter* self) {
    if (self->field_0x0 > 0) {
        requestCancelChain__Fv();
        func_8013C54C();
        func_8013E800(self->field_0x0);
        func_8027BFE0(self->field_0x0);
        self->field_0x0 = 0;
    }
}
// Copies the owner's arts-param tuning values into the 3x8 slot table and
// marks it enabled.
void func_8027CC3C(cf::CChainMusic* self, CArtsSetOwnerShim* owner){
    cf::CArtsSet* arts = owner->getArtsSet();
    float fallback = lbl_eu_80668A80;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            if (getArtsSlotRC(arts, (short)i, (short)j) != 0) {
                self->mSlots[i][j] = ((cf::CAttackParam*)getArtsParamRC2(arts, i, j))->unk80;
            } else {
                self->mSlots[i][j] = fallback;
            }
        }
    }
    self->mEnabled = 1;
}
// Writes the 3x8 slot table back into the owner's arts-param tuning values
// and clears the enable flag.
void func_8027CD08(cf::CChainMusic* self, CArtsSetOwnerShim* owner){
    if (self->mEnabled == 0) return;
    cf::CArtsSet* arts = owner->getArtsSet();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            if (getArtsSlotRC(arts, (short)i, (short)j) != 0) {
                ((cf::CAttackParam*)getArtsParamRC2(arts, i, j))->unk80 = self->mSlots[i][j];
            }
        }
    }
    self->mEnabled = 0;
}
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainApi.hpp"
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"
#include "kyoshin/menu/CMenuZeal.hpp"
#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/CVision.hpp"
// NOTE: CfObjectActor.hpp deliberately not included - it drags in
// CAIAction.hpp whose C-linkage import decls (testActorStatus/CtrlObjectParam_GetCharEntry)
// conflict with the owner declarations in CfMapItemManager.hpp.
namespace cf { class CfObjectActor; }
#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/math/Random.hpp"
#include <new>
#include <cstring>
#include "kyoshin/cf/chain/CChainActorListImports.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/CVision.hpp"

// C-linkage pseudo-imports for this TU now live in the "C-linkage imports"
// section of kyoshin/cf/chain/CChainActorList.hpp (real imports).

// Search helper for Chain_FindOrCreateActor: written with return-inside-loop so MWCC
// inlines it with the retail two-branch shape (bne-next / b-merge).
static cf::CChainActor* findActorByTarget(u32 key, cf::CChainActorList* self);
static void sweepChainable(cf::CChainActorList* self, cf::CChainList* other,
                           cf::CfObjectActor* target);
static cf::CChainActor* findActorForBB4C(u32 key, cf::CChainActorList* self);
static cf::CChainActor* findActorForKey(cf::CChainActorList* self, u32 key);
extern "C" __declspec(noinline) void ChainList_PushBack(cf::CChainList* self, cf::CChainActor* p);

static cf::CChainActor* searchActorByKey(cf::CChainActorList* self, u32 key);
// Head-pointer variant used by Chain_FindOrCreateActor: taking the sentinel node
// instead of the list keeps no live copy of `self` across the inline
// boundary, freeing r5/r6 for head/node like retail.
static cf::CChainActor* searchActorByHead(_reslist_node<cf::CChainActor*>* head, u32 key);
// Retail keeps Chain_EmitTallyEvents as one bare-C function and emits a real
// bl for it from ChainCounter_Drain; noinline keeps MWCC (-inline auto) from
// inlining the same-TU definition at the call site (ChainList_Clear idiom).
extern "C" __declspec(noinline) void Chain_EmitTallyEvents(unsigned int param);
// noinline: -inline auto would inline this same-TU helper at call sites, but
// retail emits a real bl ChainList_Clear (MWCC_CASES §8720).
extern "C" __declspec(noinline) void ChainList_Clear(cf::CChainList* self);

namespace cf {

    CChainActorList::CChainActorList(){
        mChainActorList.reserve(CWorkThreadSystem::getWorkMem(), 59);
        // Detach every node of the freshly reserved list through the real
        // reslist API (reslist::clear -> _reslist_base::clearList).
        mChainActorList.clear();
        ScnLog_ClearStateWords(this);
        unk1DA8[0] = 0;
    }


    // Retail ctor stores the manual vtables, zeroes the CChainTemp-ish buffer
    // and its flag byte. The dtor is the plain empty+delete shape (no base or
    // member dtor calls), matching retail's 0x40-byte body.
    CChainActorPc::CChainActorPc() {
        this->mVTable = &lbl_eu_80538290; // +0x70 actor vtable (temp)
        this->field_68 = &lbl_eu_80538338; // +0x68 sub-object vtable
        std::memset(this->field_4, 0, 0x60);
        this->field_64 = 0;
        this->unk6C = 0;
        this->unk0 = 0;
        this->mVTable = &lbl_eu_805384E0; // final vtable
        // Retail constructs the CChainEffect region at the END of the ctor
        // body (no dtor is ever emitted for it); call the ctor via its alias.
        __ct__Q22cf12CChainEffectFv((CChainEffect*)this->field_74);
    }



    CChainActorPc::~CChainActorPc() {}


    CChainActorEne::CChainActorEne() {
        this->mVTable = &lbl_eu_80538290; // +0x70 actor vtable (temp)
        this->field_68 = &lbl_eu_80538338; // +0x68 sub-object vtable
        std::memset(this->field_4, 0, 0x60);
        this->field_64 = 0;
        this->unk6C = 0;
        this->unk0 = 0;
        this->mVTable = &lbl_eu_80538458; // final vtable
        // See CChainActorPc ctor comment: ctor alias call keeps the dtor empty.
        __ct__Q22cf12CChainEffectFv((CChainEffect*)this->field_74);
    }



    CChainActorEne::~CChainActorEne() {}



    CChainActorList::~CChainActorList(){
        // Pass 1: destroy every actor through its manual vtable slot 17
        // (same body as Chain_ClearAll, inlined by retail).
        _reslist_node<cf::CChainActor*>* node =
            mChainActorList.mStartNodePtr->mNext;
        while (node != mChainActorList.mStartNodePtr) {
            cf::CChainActor* actor = node->mItem;
            actor->CChainActor_ClearTargetRef();
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
        ScnLog_ClearStateWords(this);
        unk1DA8[0] = 0;
        // Retail clears the ring once more before releasing the node array.
        mChainActorList.clear();
        // Release the node array through the real reslist API.
        mChainActorList.destroyList();
    }

}


extern "C" void Chain_ClearAll(cf::CChainActorList* self){
    // Pass 1: call the vtable[5] (destroy/remove) hook on every actor.
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        node->mItem->CChainActor_ClearTargetRef();
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
    ScnLog_ClearStateWords(self);
    self->unk1DA8[0] = 0;
}



// Removes every reslist actor whose referenced object is dead.
extern "C" void Chain_SweepDeadActors(cf::CChainActorList* self){
    _reslist_node<cf::CChainActor*>* node = self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        u32 base = node->mItem->unk0;
        if (base != 0) base += 0x3e9c;
        if (lookupWorkAtAddr((void*)base) == 0) {
            cf::CChainActor* actor = node->mItem;
            actor->CChainActor_ClearTargetRef();
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
extern "C" void Chain_RebuildLinks(cf::CChainActorList* self){
    // Stack order matters: retail puts otherArr at r1+0x8 and chainArr at
    // r1+0xf4; MWCC places the first-declared array at the higher slot.
    cf::CChainActor* chainArr[0x3B];
    cf::CChainActor* otherArr[0x3B];
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        int chainCount = 0;
        int otherCount = 0;
        int key = node->mItem->CChainActor_FetchRunKey();
        _reslist_node<cf::CChainActor*>* cur = node;
        while (true) {
            // Both hooks run on the current node; the member is grabbed after
            // the chainable test so no node temp lives across the calls.
            cur->mItem->CChainActor_CleanupVoiceEnd();
            if (cur->mItem->CChainActor_CheckReady(key) != 0) {
                cf::CChainActor* actor = cur->mItem;
                cur = cur->mNext;
                chainArr[chainCount++] = actor;
            } else {
                cf::CChainActor* actor = cur->mItem;
                cur = cur->mNext;
                otherArr[otherCount++] = actor;
            }
            if (cur == self->mChainActorList.mStartNodePtr) break;
            if (node->mItem->CChain_getZero_A9F4(cur->mItem) == 0) break;
        }

        // Pass: link chainable and non-chainable actors into a ring.  When
        // there are at least two chainable actors AND the first passes its
        // vtable[19] gate, chain links use flag=1; otherwise flag=0.  The
        // last slot wraps around (next = i+1 < count ? i+1 : 0).
        int chk = 0;
        if (chainCount >= 2) {
            cf::CChainActor* first = chainArr[0];
            chk = (first != 0) ? first->CChain_getZero_A9EC() : 0;
        }
        if (chainCount >= 2 && chk != 0) {
            if (chainCount == 1) {
                chainArr[0]->CChain_noop_AA0C(0, 0, 0);
            } else if (chainCount == 2) {
                chainArr[0]->CChain_noop_AA0C(1, chainArr[1], 1);
                chainArr[1]->CChain_noop_AA0C(0, 0, 0);
            } else if (chainCount >= 3) {
                for (int i = 0; i < chainCount; i++) {
                    int next = (i + 1 < chainCount) ? i + 1 : 0;
                    chainArr[i]->CChain_noop_AA0C(1, chainArr[next], 1);
                }
            }
        } else {
            if (chainCount == 1) {
                chainArr[0]->CChain_noop_AA0C(0, 0, 0);
            } else if (chainCount == 2) {
                chainArr[0]->CChain_noop_AA0C(0, chainArr[1], 1);
                chainArr[1]->CChain_noop_AA0C(0, 0, 0);
            } else if (chainCount >= 3) {
                for (int i = 0; i < chainCount; i++) {
                    int next = (i + 1 < chainCount) ? i + 1 : 0;
                    chainArr[i]->CChain_noop_AA0C(0, chainArr[next], 1);
                }
            }
            // Retail tail-duplicates the non-chainable linkage in both arms.
            if (otherCount == 1) {
                otherArr[0]->CChain_noop_AA0C(0, 0, 0);
            } else if (otherCount == 2) {
                otherArr[0]->CChain_noop_AA0C(0, otherArr[1], 1);
                otherArr[1]->CChain_noop_AA0C(0, 0, 0);
            } else if (otherCount >= 3) {
                for (int i = 0; i < otherCount; i++) {
                    int next = (i + 1 < otherCount) ? i + 1 : 0;
                    otherArr[i]->CChain_noop_AA0C(0, otherArr[next], 1);
                }
            }
        }
        if (otherCount == 1) {
            otherArr[0]->CChain_noop_AA0C(0, 0, 0);
        } else if (otherCount == 2) {
            otherArr[0]->CChain_noop_AA0C(0, otherArr[1], 1);
            otherArr[1]->CChain_noop_AA0C(0, 0, 0);
        } else if (otherCount >= 3) {
            for (int i = 0; i < otherCount; i++) {
                int next = (i + 1 < otherCount) ? i + 1 : 0;
                otherArr[i]->CChain_noop_AA0C(0, otherArr[next], 1);
            }
        }

        node = cur;
    }
}


extern "C" cf::CChainActor* Chain_FindOrCreateActor(cf::CChainActorList* self, u32 key){
    // Inlined search: testing the call result directly keeps the value in
    // the return register r3 at the merge, matching retail.
    cf::CChainActor* found =
        searchActorByHead(self->mChainActorList.mStartNodePtr, key);
    if (found == 0) {
        cf::CChainActor* newActor = SysWinLog_AllocFreeSlot(self);
        Chain_InsertOrdered(self, newActor);
        if (self->unk1DA8[0]) {
            newActor->CChainActor_ToggleMoveFlag(1);
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
extern "C" int Chain_RemoveActor(cf::CChainActorList* self, u32 key) {
    _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
    _reslist_node<cf::CChainActor*>* node = head->mNext;
    while (node != head) {
        cf::CChainActor* actor = node->mItem;
        if (key == actor->unk0) {
            actor->CChainActor_ClearTargetRef();
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
// name Chain_InsertOrdered (call sites must reference it unmangled).
extern "C" void Chain_InsertOrdered(cf::CChainActorList* self, cf::CChainActor* actor) {
    int myVal = actor->CChain_getZero_A9E0();
    _reslist_node<cf::CChainActor*>* pre =
        self->mChainActorList.mStartNodePtr->mNext;
    while (pre != self->mChainActorList.mStartNodePtr) {
        if (pre->mItem->CChain_getZero_A9F4(actor)) {
            while (pre != self->mChainActorList.mStartNodePtr &&
                   pre->mItem->CChain_getZero_A9F4(actor)) {
                if (myVal <
                    pre->mItem->CChain_getZero_A9E0()) goto place;
                pre = pre->mNext;
            }
            goto place;
        }
        pre = pre->mNext;
    }
place:;
    // Find an empty slot in the preallocated node array and link before @p pre.
    _reslist_node<cf::CChainActor*>* temp;
    int i = self->mChainActorList.findFirstEmptySlotIndex();
    temp = &self->mChainActorList.mList[i];
    temp->setItem(actor);
    temp->mNext = pre;
    temp->mPrev = pre->mPrev;
    pre->mPrev->mNext = temp;
    pre->mPrev = temp;
}


extern "C" void Chain_CollectChainable(cf::CChainActorList* self, cf::CChainList* other,
                   cf::CfObjectActor* target) {
    // 1. Activate the resident actor whose unk0 references @p target.
    cf::CChainActor* found = findActorByTarget((u32)target, self);
    if (found != 0) found->CChain_noop_A9E8();
    ChainList_Clear(other);
    // 2. Sweep every actor; the ones chainable against target move into @p other.
    sweepChainable(self, other, target);
}


// For each actor in @p list, ensures it is present in @p self's reslist
// (adding it via Chain_InsertOrdered when missing), then clears @p list.
extern "C" void Chain_MergeList(cf::CChainActorList* self, cf::CChainList* list){
    cf::CChainList* l = list;
    for (int i = 0; i < (int)l->mCount; i++) {
        cf::CChainActor* actor = (i < (int)list->mCount) ? list->mActors[i] : 0;
        if (findActorForBB4C(actor->unk0, self) == 0)
            Chain_InsertOrdered(self, actor);
    }
    ChainList_Clear(list);
}

// Removes slave/target actors that have become chainable against @p target.
// First, if an actor directly references @p target (unk0 == target) it is
// activated via its vtable slot 20 and @p other is cleared. Then every
// remaining chainable actor (vtable slot 18 against target, and slot 8
// against target's move subobject) is appended to @p other and removed from
// the reslist.
// Return-inside-loop search helper: when inlined at Chain_CollectChainable's first
// phase, MWCC keeps the key in the caller's incoming param register and emits
// the retail two-branch shape (bne-next / b-merge with li r3,0 at the merge).
static cf::CChainActor* findActorByTarget(u32 key, cf::CChainActorList* self) {
    typedef reslist<cf::CChainActor*>::iterator ActorIter;
    for (ActorIter it = self->mChainActorList.begin(),
                   end = self->mChainActorList.end();
         it != end; ++it) {
        cf::CChainActor* x = *it;
        if (key == x->unk0) return x;
    }
    return 0;
}

// Sweep body kept as a helper
// parameter web (retail recomputes target+0x3E9C per iteration instead of
// caching it across the loop).
static void sweepChainable(cf::CChainActorList* self, cf::CChainList* other,
                           cf::CfObjectActor* target) {
    typedef reslist<cf::CChainActor*>::iterator ActorIter;
    for (ActorIter it = self->mChainActorList.begin(),
                   end = self->mChainActorList.end();
         it != end; ++it) {
        if ((*it)->CChain_getZero_A9FC((void*)(int)target) != 0) {
            if ((*it)->CChainActor_CheckReady(
                    reinterpret_cast<cf::CChainBattleObj*>(target)->mSub.v17()) != 0) {
                ChainList_PushBack(other, *it);
                self->mChainActorList.remove(*it);
                --it;
            }
        }
    }
}


// Chain-activation validator
// when the candidate passes its activation gates AND at least two actors in
// the list are chainable against it, else 0.
extern "C" int Chain_CheckActivatable(cf::CChainActorList* self, u32 key){
    // Locate the actor whose unk0 matches key.
    cf::CChainActor* actor = findActorForKey(self, key);
    if (actor == 0) return 0;
    // The candidate must pass its own activation gate (vtable[19]).
    int gate = (actor != 0) ? actor->CChain_getZero_A9EC() : 0;
    if (gate == 0) return 0;
    // ... and must be chainable against the key's voice sub-object.
    if (actor->CChainActor_CheckReady(
            reinterpret_cast<cf::CChainBattleObj*>(key)->mSub.v17()) == 0)
        return 0;
    // ... and must pass the "already chained" anti-gate (vtable[29]).
    if (actor->CChain_getZero_A9D0() != 0) return 0;
    // The battle object's probed address must not be in any of three states.
    if (isBattleStatusActive(&reinterpret_cast<cf::CChainBattleObj*>(key)->mSub8, 0xeb) != 0) return 0;
    if (isBattleStatusActive(&reinterpret_cast<cf::CChainBattleObj*>(key)->mSub8, 0xcb) != 0) return 0;
    if (isBattleStatusActive(&reinterpret_cast<cf::CChainBattleObj*>(key)->mSub8, 0xf8) != 0) return 0;
    // The battle object's arts-selection state must not match the no-chain id.
    int local = *reinterpret_cast<int*>(reinterpret_cast<cf::CChainBattleObj*>(key)->field_04->f30());
    if (testActorStatus((void*)key, &local, 0x1f) != 0) return 0;
    if (getAnimModelId(reinterpret_cast<cf::CChainBattleObj*>(key)->field_3F60) == 0x31)
        return 0;
    if (actor->CChainActor_ScanArtsSlots(1) == 0) return 0;
    // Count how many other actors are chainable against the candidate.
    // The end iterator is cached; MWCC keeps the retail sentinel compare.
    int count = 0;
    typedef reslist<cf::CChainActor*>::iterator ActorIter;
    for (ActorIter it = self->mChainActorList.begin(),
                   end = self->mChainActorList.end();
         it != end; ++it) {
        if ((*it)->CChain_getZero_A9FC((void*)(int)key) != 0) {
            if ((*it)->CChainActor_CheckReady(
                    reinterpret_cast<cf::CChainBattleObj*>(key)->mSub.v17()) != 0) {
                count++;
            }
        }
    }
    // Retail's bit-twiddled "count >= 2" test: bit 31 of
    // ((count^1)>>1) - ((count^1)&count) is set exactly then.
    int t = count ^ 1;
    return ((u32)((t >> 1) - (t & count))) >> 31;
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
// BB4C-private search: the iterator pair keeps the same inlined walk as the
// original head-pointer form. Best-known shape (banked draft): structural
// bytes match retail exactly; the residual is a fixed r6/r7 Chaitin swap
// between the sentinel and the walker inside the inlined loop (see notes).
// The list API is used here for source shape, not to change codegen.
static cf::CChainActor* findActorForBB4C(u32 key, cf::CChainActorList* self) {
    typedef reslist<cf::CChainActor*>::iterator ActorIter;
    for (ActorIter it = self->mChainActorList.begin(),
                   end = self->mChainActorList.end();
         it != end; ++it) {
        cf::CChainActor* x = *it;
        if (key == x->unk0) return x;
    }
    return 0;
}


// Returns 1 if some reslist actor is a valid chain-activation target, else 0.
// The target object (actor->unk0) is re-read before each probe so it stays in
// temps across the isBattleStatusActive calls; only the post-check read lives in a
// callee-saved register (retail r31), matching the Chain_SweepDeadActors pattern.
extern "C" int Chain_HasValidTarget(cf::CChainActorList* self){
    _reslist_node<cf::CChainActor*>* node;
    cf::CChainBattleObj* obj;
    node = self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        if (isBattleStatusActive(&reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0)->mSub8, 0x10c) == 0 &&
            isBattleStatusActive(&reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0)->mSub8, 0xf8) == 0) {
            obj = reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0);
            int local = *reinterpret_cast<int*>(reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0)->field_04->f30());
            if (testActorStatus(obj, &local, 0x801) != 0) return 1;
        }
        node = node->mNext;
    }
    return 0;
}

// Same-TU search helper inlined at Chain_CheckActivatable. Argument order (self, key)
// reproduces retail's register coloring (the located actor lands in r31).
static cf::CChainActor* findActorForKey(cf::CChainActorList* self, u32 key) {
    typedef reslist<cf::CChainActor*>::iterator ActorIter;
    for (ActorIter it = self->mChainActorList.begin(),
                   end = self->mChainActorList.end();
         it != end; ++it) {
        cf::CChainActor* x = *it;
        if (key == x->unk0) return x;
    }
    return 0;
}


// When the chain flag bit 1 is set, query a value and emit threshold events.
extern "C" void Chain_EmitThresholdEvents(cf::CChainFlag* self) {
    if (self->field_0x3F00 & 2) {
        u32 v = SysWinLog_BumpEventValue(0x2f, 1);
        if (v >= 1) {
            SysWinLog_QueueEvent(0x2f);
        }
        if (v >= 0x32) {
            SysWinLog_QueueEvent(0x30);
        }
        if (v >= 0xc8) {
            SysWinLog_QueueEvent(0x31);
        }
        if (v >= 0x3e8) {
            SysWinLog_QueueEvent(0x32);
        }
        lbl_eu_80662A80 = 1;
    }
}


// Emits chain-tally threshold events based on the accumulated counter.
// Single bare-C definition matching retail's bytes; the noinline decl above
// keeps the ChainCounter_Drain call a real bl instead of inlining it here.
// Thresholds 0xBB8/0x7530/0x186A0 queue SysWin events 0x33/0x34/0x35.
extern "C" void Chain_EmitTallyEvents(unsigned int param) {
    if (param >= 0xBB8) {
        SysWinLog_QueueEvent(0x33);
    }
    if (param >= 0x7530) {
        SysWinLog_QueueEvent(0x34);
    }
    if (param >= 0x186A0) {
        SysWinLog_QueueEvent(0x35);
    }
}


extern "C" void Chain_EmitCountEvents(cf::CChainFlag* self) {
    if (self->field_0x3F00 & 2) {
        lbl_eu_80662A80++;
        if (lbl_eu_80662A80 >= 4) {
            SysWinLog_QueueEvent(0x36);
        }
        if (lbl_eu_80662A80 >= 5) {
            SysWinLog_QueueEvent(0x37);
        }
    }
}



// Resets the chain-chance state: clears the chain count, both step counters
// and the step flag byte.
extern "C" void ChainChance_Reset(cf::CChainChanceS* self) {
    self->mChainCount = 0;
    self->mField08 = 0;
    self->mField0A = 0;
    self->mField0C = 0;
}


// Decrements the chain count; plays the indexed chain sound at the wrap
// point and advances the step counters (field_8 / field_A wraps at 4).
extern "C" void Chain_TickChanceCount(cf::CChainChanceS* self) {
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
        CUICfManager_queueFactoryMenu(3, snd);
        // Both counters are read before either store; the wrap check uses
        // the already-updated field0A value.
        int nextB = self->mField08 + 1;
        int nextA = self->mField0A + 1;
        self->mField0A = nextA;
        self->mField08 = nextB;
        if ((s16)nextA >= 4) self->mField0A = 3;
    }
}


// Starts a chain message; writes 0xa to the message id on success.
extern "C" int Chain_RollActivation(cf::CChainMsg* self) {
    // Retail's call site is a stale-register call: no arguments are set up.
    // Route through an unprototyped pointer type so the call passes none;
    // MWCC folds the constant function address back into a direct bl.
    typedef int (*UnprotoChainRollFn)();
    if (((UnprotoChainRollFn)Chain_CheckChanceRoll)() != 0) {  // DEBT(callcast): C symbol with 3 params; retail call site is a stale-register call with no args set up
        chainResolveMemberPtr(0xc3, 0);
        self->field_0x0 = 0xa;
        return 1;
    }
    return 0;
}


// Rolls the dice on a chain activation between battle objects @p objA and
// @p objB under the chain state in @p self. The threshold comes from the
// tuning-pool record selected by objA's vtable(+0x308) query, offset by one
// 0x14-byte record when the step flag at +0xC is consumed, and scaled by the
// pair value capped at 5000. Succeeds when mtRand(100) exceeds the score.
// C linkage comes from the Chain_CheckChanceRoll declaration in the TU header
// (retail callers bl the unmangled label; MWCC_CASES CBattleMan_OnActorsEmpty record).
int Chain_CheckChanceRoll(cf::CChainChanceS* self,
                  cf::CChainBattleObj* objA,
                  cf::CChainBattleObj* objB) {
    if (self->mField08 >= 0xf) {
        return 0;
    }
    int tblOff = lbl_eu_8050EDD0[objA->v192()];
    u32 pair = (u16)getTableValueByPair__Q22cf13CfGameManagerFv(
        objA->field_3F28, objB->field_3F28);
    u32 val;
    if (objA->v160() != 0) {
        objA->v160();
        val = (u16)IdTable_SumValues((CUnkObj8025FB10*)objA, 0x6b);
    } else {
        val = 0;
    }

    // Consume the step flag: while set, bump the accumulator by 0x14.
    int fv = self->mField0C;
    self->mField0C = 0;
    int flagOff = ((-fv | fv) >> 31) & 0x14;

    u32 cnt = pair > 0x1388 ? 0x1388 : pair;
    int off2 = (int)(val + tblOff) + flagOff;
    int q1 = (int)(30.0f * (double)cnt / 5000.0f);
    u32 acc = (u16)(off2 + q1);
    if (pair > 0x1388) {
        u32 over = pair - 0x1388;
        int q2 = (int)(20.0f * (double)over / 5000.0f);
        acc = (u16)(acc + q2);
    }
    int rnd = ml::math::mtRand(100);
    int mixed = (int)acc ^ rnd;
    return (u32)((mixed >> 1) - (mixed & (int)acc)) >> 31;
}


// Reads the current pad press. If a chain-trigger button is held, performs the
// action selected by GetZealResult; writes result to *out and returns 1.
extern "C" int Chain_ReadPadAction(cf::CChainAction* self, u8* out){
    if (self->field_0 > 0) return 0;
    int sel = (int)GetZealObjPtr();
    if (sel == 0) {
        *out = 0;
        return 1;
    }
    CPad* pad = cf::CfGameManager::getCurrentPad();
    // The chain-trigger pad bit depends on controller type (classic vs wii).
    u32 bit = (cf::CfGameManager::isClassicController(-1) != 0)
                  ? (pad->mPressedButtonFlags >> 22) & 1
                  : (pad->mPressedButtonFlags >> 5) & 1;
    if (bit != 0) {
        int action = GetZealResult(sel);
        // Sparse dispatch: goto-chain keeps the retail compare-chain layout with
        // the case bodies appended after the tests (if/else-if would inline them).
        if (action == 1) goto case1;
        if (action == 2) goto case2;
        self->field_0xc = 0;
        *out = 0;
        return 1;
    case1:
        chainResolveMemberPtr(0xca, 0);
        self->field_0xc = 0;
        *out = 1;
        return 1;
    case2:
        chainResolveMemberPtr(0xc9, 0);
        self->field_0xc = 1;
        *out = 1;
        return 1;
    }
    return 0;
}


// Zeroes the actor list: clears the pointer array, count, and flag.
// noinline + extern "C": retail callers emit a real bl to the bare symbol
// ChainList_Clear (MWCC_CASES §8720, §8717).
extern "C" __declspec(noinline) void ChainList_Clear(cf::CChainList* self) {
    memset(self->mActors, 0, sizeof(self->mActors));
    self->mCount = 0;
    self->mFlag = 0;
}


// Removes from @p self every actor whose referenced object is dead
// (lookupWorkAtAddr), calling vtable[5] and shifting the array down.
extern "C" void ChainList_SweepDead(cf::CChainList* self){
    cf::CChainActor** p;
    int i = 0;
    while (i < (int)self->mCount) {
        p = &self->mActors[i];
        u32 base = (*p)->unk0;
        if (base != 0) base += 0x3e9c;
        if (lookupWorkAtAddr((void*)base) == 0) {
            cf::CChainActor* actor = *p;
            actor->CChainActor_ClearTargetRef();
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
extern "C" void ChainList_UpdateAll(cf::CChainList* self) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* a = self->mActors[i];
        a->CChainActor_CleanupVoiceEnd();
    }
}


// Appends @p p to the actor list at index mCount, then increments mCount.
// noinline + extern "C": retail callers emit a real bl to the bare symbol
// ChainList_PushBack (MWCC_CASES §8720, §8717).
extern "C" __declspec(noinline) void ChainList_PushBack(cf::CChainList* self, cf::CChainActor* p) {
    self->mActors[self->mCount++] = p;
}



// Removes the actor whose unk0 matches @p key (if any); returns whether found.
extern "C" int ChainList_RemoveKey(cf::CChainList* self, u32 key){
    for (int i = 0; i < (int)self->mCount; i++) {
        if (self->mActors[i]->unk0 == key) {
            cf::CChainActor** p = &self->mActors[i];
            cf::CChainActor* actor = self->mActors[i];
            actor->CChainActor_ClearTargetRef();
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
        actor->CChain_noop_AA0C(target, nextActor, 0); \
    } while (0)

// Drives the chain-link call for one slot (@p index), or for every slot when
// @p index is -1: the -1 path nests three slot loops (retail's dead i/j/k ==
// -1 guards fall through, and the != -1 single-slot blocks come after), with
// the innermost re-entering this function per slot.
// extern "C" definition (same idiom as Chain_InsertOrdered/ChainList_Clear): retail's
// symbol is the bare name Chain_LinkList, and the recursive innermost-loop
// call must emit `bl Chain_LinkList` (unmangled) to match retail's reloc.
extern "C" void Chain_LinkList(cf::CChainList* self, int target, int index){
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
                                    Chain_LinkList(self, target, m);
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
extern "C" void ChainList_StepTargets(cf::CChainList* self, int target){
    for (int i = 0; i < (int)self->mCount; i++) {
        self->mActors[i]->CChainActor_ToggleCancelVoice(target);
        if (target != 0 && self->mActors[i]->CChain_getZero_A584() != 0) {
            int next = (i + 1 < (int)self->mCount) ? (i + 1) : 0;
            if (i != next) {
                cf::CChainActor* other = self->mActors[next];
                addTableValueWithClamp__Q22cf13CfGameManagerFv(
                    reinterpret_cast<cf::CChainBattleObj*>(self->mActors[i]->unk0)->field_3F28,
                    reinterpret_cast<cf::CChainBattleObj*>(other->unk0)->field_3F28, 0xa);
            }
        }
    }
}


// Returns 1 if any resident actor's vtable[22] (+0x58) value matches @p key.
extern "C" int ChainList_HasCount(cf::CChainList* self, int key) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* actor = self->mActors[i];
        if (actor->CChain_getChainCount() == key)
            return 1;
    }
    return 0;
}


// Returns the list entry whose first u32 field matches @p key.
extern "C" cf::CChainActor* ChainList_FindKey(cf::CChainList* self, u32 key) {
    for (int i = 0; i < (int)self->mCount; i++) {
        if (self->mActors[i]->unk0 == key)
            return self->mActors[i];
    }
    return 0;
}


// Checks (via testActorStatus) whether the resident actors satisfy @p condition;
// the polarity of the result depends on @p check.
extern "C" int ChainList_CheckCondition(cf::CChainList* self, int target, int check){
    cf::CChainActor** p;
    cf::CChainBattleObj* obj;
    if (check == 0) {
        obj = 0;
        p = self->mActors;
        for (int i = 0; i < (int)self->mCount; i++) {
            obj = reinterpret_cast<cf::CChainBattleObj*>((*p)->unk0);
            int local = *reinterpret_cast<int*>(obj->field_04->f30());
            if (testActorStatus(obj, &local, target) != 0) return 1;
            p++;
        }
        return 0;
    }
    obj = 0;
    p = self->mActors;
    for (int i = 0; i < (int)self->mCount; i++) {
        obj = reinterpret_cast<cf::CChainBattleObj*>((*p)->unk0);
        int local = *reinterpret_cast<int*>(obj->field_04->f30());
        if (testActorStatus(obj, &local, target) == 0) return 0;
        p++;
    }
    return 1;
}


// If the counter is positive, runs chain update steps and resets it.
extern "C" void ChainCounter_Drain(cf::CChainCounter* self) {
    if (self->field_0x0 > 0) {
        requestCancelChain__Fv();
        UIWin_GetInstance();
        UIWin_Create6F8B0Win(self->field_0x0);
        Chain_EmitTallyEvents(self->field_0x0);
        self->field_0x0 = 0;
    }
}


// Copies the owner's arts-param tuning values into the 3x8 slot table and
// marks it enabled.
extern "C" void ChainMusic_SaveSlots(cf::CChainMusic* self, cf::CChainBattleObj* owner){
    cf::CArtsSet* arts = (cf::CArtsSet*)(cf::CArtsSet*)reinterpret_cast<cf::CChainBattleObj*>(owner)->v157();
    float fallback = lbl_eu_80668A80[0];
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
extern "C" void ChainMusic_RestoreSlots(cf::CChainMusic* self, cf::CChainBattleObj* owner){
    if (self->mEnabled == 0) return;
    cf::CArtsSet* arts = (cf::CArtsSet*)(cf::CArtsSet*)reinterpret_cast<cf::CChainBattleObj*>(owner)->v157();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            if (getArtsSlotRC(arts, (short)i, (short)j) != 0) {
                ((cf::CAttackParam*)getArtsParamRC2(arts, i, j))->unk80 = self->mSlots[i][j];
            }
        }
    }
    self->mEnabled = 0;
}

// --- hand-built retail data (splits: .sdata2 80668A80-80668A88) ---
// .sdata2 zero-float pair (retail lbl_eu_80668A80, 8 bytes). Defined at EOF
// so the const value cannot propagate into the functions above (their lfs
// loads stay symbolic, as retail). Zero-initialized: needs the section
// pragma (declspec alone sinks to nobits); the writable flag is immaterial
// (cf. code_80213488's floats, same recipe).
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") float lbl_eu_80668A80[2] = { 0.0f, 0.0f };
#pragma section data_type ".data" ".data"

// --- hand-built retail tables (splits: .rodata 8050ED18-8050EE10,
// .sdata 80662A40-80662A90) ---
// Retail keeps RTTI name strings, two tuning tables, ten 8-byte typeinfo
// pairs ({classname, base-or-0}, cf. CScrollBar's RTTI pair) and one s32
// counter in this TU. The TU compiles -RTTI off (configure.py), so MWCC
// emits no __RTTI__/typeinfo names of its own and these definitions own
// .rodata/.sdata outright. .data stays rule-stripped: the compiler vtable
// run for List/reslist/base cannot interleave with the manual vtables, and
// the frozen rule cannot gain the renames; see the t3 report.
// .rodata layout: 9 strings (sizes strlen+1+pad-to-4; MWCC packs
// same-section char arrays in declaration order), then the u16 id table
// (EDD0: indices selected by v192()), the float table (EDE0: 1.0/1.2/1.4/1.6
// selected by mField0A in Chain_TickChanceCount), then the Flusher string.
// Total: 0x18+0x10+0x18+0x10+0x14+0x1C+0x24+0x14+0x10+0x10+0x20 = 0xF8.
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050ED18[0x18] = "cf::CChainChance";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050ED30[0x10] = "cf::CChainTimer";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050ED40[0x18] = "cf::CChainMember";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050ED58[0x10] = "cf::CChainActor";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050ED68[0x14] = "cf::CChainActorList";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050ED7C[0x1C] = "reslist<cf::CChainActor *>";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050ED98[0x24] = "_reslist_base<cf::CChainActor *>";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050EDBC[0x14] = "cf::CChainTemp";
__declspec(section ".rodata") __attribute__((used)) unsigned short lbl_eu_8050EDD0[8] = { 0, 15, 20, 25, 30, 0, 0, 0 };
extern "C" __declspec(section ".rodata") __attribute__((used)) float lbl_eu_8050EDE0[4] = { 1.0f, 1.2f, 1.4f, 1.6f };
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_8050EDF0[0x20] = "cf::Flusher<cf::CfObjectActor>";
// .sdata typeinfo pairs (retail order; E5's second word names the .data
// triple lbl_eu_80538320, still rule-stripped, so it links UNDEF for now).
extern u8 lbl_eu_80538320[];
__declspec(section ".sdata") __attribute__((used)) void* __RTTI__Q22cf12CChainChance[2] = { lbl_eu_8050ED18, 0 };
__declspec(section ".sdata") __attribute__((used)) void* __RTTI__Q22cf11CChainTimer[2] = { lbl_eu_8050ED30, 0 };
__declspec(section ".sdata") __attribute__((used)) void* __RTTI__Q22cf12CChainMember[2] = { lbl_eu_8050ED40, 0 };
__declspec(section ".sdata") __attribute__((used)) void* __RTTI__Q22cf11CChainActor[2] = { lbl_eu_8050ED58, 0 };
__declspec(section ".sdata") __attribute__((used)) void* __RTTI__Q22cf15CChainActorList[2] = { lbl_eu_8050ED68, 0 };
__declspec(section ".sdata") __attribute__((used)) void* lbl_eu_80662A68[2] = { lbl_eu_8050ED7C, lbl_eu_80538320 };
__declspec(section ".sdata") __attribute__((used)) void* lbl_eu_80662A70[2] = { lbl_eu_8050ED98, 0 };
__declspec(section ".sdata") __attribute__((used)) void* __RTTI__Q22cf10CChainTemp[2] = { lbl_eu_8050EDBC, 0 };
// .sdata event counter (retail initial value 1; the trailing 4 bytes to
// A88 are MWCC's alignment pad for the 8-byte pair that follows).
extern "C" __declspec(section ".sdata") int lbl_eu_80662A80 = 1;
__declspec(section ".sdata") __attribute__((used)) void* lbl_eu_80662A88[2] = { lbl_eu_8050EDF0, 0 };

// --- hand-built retail .data (splits: .data 8053826C-80538358, 0xEC) ---
// Retail .data is the CChain* vtable run, all zero bytes + ADDR32 slots:
//   8053826C/78/84 (0xC) CChainChance/CChainTimer/CChainMember vtables
//   80538290 (0x78)      CChainActor vtable (the +0x70 vptr the Pc/Ene ctors
//                        store by hand)
//   80538308 (0xC)       CChainActorList vtable
//   80538314 (0xC)       reslist<CChainActor*> vtable
//   80538320 (0xC)       reslist RTTI base record
//   8053832C (0xC)       _reslist_base<CChainActor*> vtable
//   80538338 (0x10)      CChainTemp vtable
//   80538348 (0x10)      Flusher<CfObjectActor> vtable
// The first four are typed here (word 0 = the .sdata RTTI pair above, word 1
// the zero offset-to-top, then the dtor and -- for the actor table -- the 27
// C-linkage virtual slots, all defined by CChain.o in the link). They occupy
// exactly retail 0x00-0x9C, so MWCC's own vtable run for this TU (the
// CChainActorList/reslist/_reslist_base vtables its ctor/dtor vptr stores
// need) lands on retail 0x9C/0xA8/0xB4 and the rule renames the first two
// back onto their retail labels; see below for the 0x2C residual.
extern "C" void* __dt__Q22cf12CChainChanceFv(void*, int);
extern "C" void* __dt__Q22cf11CChainTimerFv(void*, int);
extern "C" void* __dt__Q22cf12CChainMemberFv(void*, int);
extern "C" void* __dt__Q22cf11CChainActorFv(void*, int);
extern "C" void CChain_setFieldAndClear(void*, int);
extern "C" int CChainActor_CheckReady(void*, int);
extern "C" int CChainActor_ScanArtsSlots(void*, int);
extern "C" int CChainActor_CheckFacing(void*);
extern "C" int CChainActor_CheckVoice(void*);
extern "C" void CChain_noop_AA0C(void*, void*, int);
extern "C" void CChain_noop_795D0(void*);
extern "C" void CChain_noop_78E00(void*, int, int);
extern "C" int CChain_getZero_78E04(void*);
extern "C" int CChain_getZero_AA04(void*);
extern "C" int CChain_getZero_A9F4(void*, void*);
extern "C" int CChain_getZero_A9FC(void*, void*);
extern "C" int CChain_getZero_A9EC(void*);
extern "C" void CChain_noop_A9E8(void*);
extern "C" int CChain_getZero_A9E0(void*);
extern "C" int CChain_getChainCount(void*);
extern "C" int CChain_getZero_A584(void*);
extern "C" int CChain_getZero_A9D8(void*);
extern "C" void CChain_noop_79768(void*, int);
extern "C" int CChainActor_FetchRunKey(void*);
extern "C" void CChainActor_FwdRunKey(void*, int);
extern "C" int CChainActor_NotifyBattleObj(void*);
extern "C" int CChain_getZero_A9D0(void*);

__declspec(section ".data") __attribute__((used, aligned(4)))
void* lbl_eu_8053826C[3] = {
    __RTTI__Q22cf12CChainChance, 0, (void*)__dt__Q22cf12CChainChanceFv,
};
__declspec(section ".data") __attribute__((used, aligned(4)))
void* lbl_eu_80538278[3] = {
    __RTTI__Q22cf11CChainTimer, 0, (void*)__dt__Q22cf11CChainTimerFv,
};
__declspec(section ".data") __attribute__((used, aligned(4)))
void* lbl_eu_80538284[3] = {
    __RTTI__Q22cf12CChainMember, 0, (void*)__dt__Q22cf12CChainMemberFv,
};
// CChainActor vtable (0x78): header [RTTI, 0] + 28 slots, retail order.
// MWCC aligns an object whose size is a multiple of 8 to 8 bytes, so a single
// 0x78 array would land at 0x28 (retail 0x24) and push the compiler's vtable
// run off retail 0x9C/0xA8. Split 29 words + 1 word (both size%8 == 4) keeps
// the table at 0x24 with all 29 relocs in retail order; the last word is the
// same table's slot 29 (see the wave-DL7 report).
extern "C" __declspec(section ".data") __attribute__((used, aligned(4)))
u32 lbl_eu_80538290[29] = {
    (u32)__RTTI__Q22cf11CChainActor, 0, (u32)&__dt__Q22cf11CChainActorFv,
    (u32)CChain_setFieldAndClear,      // +0x0C  (slot 2)
    (u32)::CChainActor_CleanupVoiceEnd,  // +0x10
    (u32)::CChainActor_ClearTargetRef,   // +0x14
    (u32)::CChainActor_ToggleCancelVoice,// +0x18
    (u32)::CChainActor_ToggleMoveFlag,   // +0x1C
    (u32)CChainActor_CheckReady,       // +0x20
    (u32)CChainActor_ScanArtsSlots,    // +0x24
    (u32)CChainActor_CheckFacing,      // +0x28
    (u32)CChainActor_CheckVoice,       // +0x2C
    (u32)CChain_noop_AA0C,             // +0x30
    (u32)CChain_noop_795D0,            // +0x34
    (u32)CChain_noop_78E00,            // +0x38
    (u32)CChain_getZero_78E04,         // +0x3C
    (u32)CChain_getZero_AA04,          // +0x40
    (u32)CChain_getZero_A9F4,          // +0x44
    (u32)CChain_getZero_A9FC,          // +0x48
    (u32)CChain_getZero_A9EC,          // +0x4C
    (u32)CChain_noop_A9E8,             // +0x50
    (u32)CChain_getZero_A9E0,          // +0x54
    (u32)CChain_getChainCount,         // +0x58
    (u32)CChain_getZero_A584,          // +0x5C
    (u32)CChain_getZero_A9D8,          // +0x60
    (u32)CChain_noop_79768,            // +0x64
    (u32)CChainActor_FetchRunKey,      // +0x68
    (u32)CChainActor_FwdRunKey,        // +0x6C
    (u32)CChainActor_NotifyBattleObj,  // +0x70
};
__declspec(section ".data") __attribute__((used, aligned(4)))
u32 lbl_eu_80538290_slot29[1] = {
    (u32)CChain_getZero_A9D0,          // +0x74 (table word 29)
};


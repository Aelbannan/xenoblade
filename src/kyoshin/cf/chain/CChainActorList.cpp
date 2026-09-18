#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainTimer.hpp"
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

// Search helper for Chain_FindOrCreateActor: written with return-inside-loop so MWCC
// inlines it with the retail two-branch shape (bne-next / b-merge).
static cf::CChainActor* searchActorByKey(cf::CChainActorList* self, u32 key);
// Head-pointer variant used by Chain_FindOrCreateActor: taking the sentinel node
// instead of the list keeps no live copy of `self` across the inline
// boundary, freeing r5/r6 for head/node like retail.
static cf::CChainActor* searchActorByHead(_reslist_node<cf::CChainActor*>* head, u32 key);
// Call-site overload (s32) so ChainCounter_Drain emits a real bl instead of
// inlining the u32 definition below (retail keeps the call; the definition's
// u32 overload is what matches retail Chain_EmitTallyEvents's bytes).
void Chain_EmitTallyEvents(s32 param);
// noinline: -inline auto would inline this same-TU helper at call sites, but
// retail emits a real bl ChainList_Clear (MWCC_CASES §8720).
extern "C" __declspec(noinline) void ChainList_Clear(cf::CChainList* self);

namespace cf {
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
        ScnLog_ClearStateWords(this);
        unk1DA8[0] = 0;
    }

    CChainActorList::~CChainActorList(){
        // Pass 1: destroy every actor through its manual vtable slot 17
        // (same body as func_8027B164, inlined by retail).
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
// ChainList_PushBack (MWCC_CASES §8720, §8717).
extern "C" __declspec(noinline) void ChainList_PushBack(cf::CChainList* self, cf::CChainActor* p) {
    self->mActors[self->mCount++] = p;
}

// Returns the list entry whose first u32 field matches @p key.
cf::CChainActor* ChainList_FindKey(cf::CChainList* self, u32 key) {
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
void Chain_SweepDeadActors(cf::CChainActorList* self){
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
        int key = node->mItem->CChainActor_FetchRunKey();
        _reslist_node<cf::CChainActor*>* cur = node;
        while (true) {
            // Both hooks run on the current node; the member is grabbed after
            // the chainable test so no node temp lives across the calls.
            cur->mItem->CChainActor_CleanupVoiceEnd();
            if (cur->mItem->func_8027A024(key) != 0) {
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
cf::CChainActor* Chain_FindOrCreateActor(cf::CChainActorList* self, u32 key){
    // Inlined search: testing the call result directly keeps the value in
    // the return register r3 at the merge, matching retail.
    cf::CChainActor* found =
        searchActorByHead(self->mChainActorList.mStartNodePtr, key);
    if (found == 0) {
        cf::CChainActor* newActor = func_8028120C(self);
        func_8027B8C8(self, newActor);
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
int Chain_RemoveActor(cf::CChainActorList* self, u32 key) {
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
// name func_8027B8C8 (call sites must reference it unmangled).
extern "C" void func_8027B8C8(cf::CChainActorList* self, cf::CChainActor* actor) {
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
    int i;
    int byteOff;
    int capacity = self->mChainActorList.mCapacity;
    for (i = 0, byteOff = 0; i < capacity; i++) {
        if (reinterpret_cast<_reslist_node<cf::CChainActor*>*>(
                (u8*)self->mChainActorList.mList + byteOff)->mNext == 0)
            break;
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
        if (key == x->unk0) return x;
        node = node->mNext;
    }
    return 0;
}

// Sweep body kept as a helper
// parameter web (retail recomputes target+0x3E9C per iteration instead of
// caching it across the loop).
static void sweepChainable(cf::CChainActorList* self, cf::CChainList* other,
                           cf::CfObjectActor* target) {
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        if (node->mItem->CChain_getZero_A9FC((void*)(int)target) != 0) {
            if (node->mItem->func_8027A024(
                    reinterpret_cast<cf::CChainBattleObj*>(target)->mSub.v17()) != 0) {
                ChainList_PushBack(other, node->mItem);
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
    if (found != 0) found->CChain_noop_A9E8();
    ChainList_Clear(other);
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
        if (findActorForBB4C(actor->unk0, self) == 0)
            func_8027B8C8(self, actor);
    }
    ChainList_Clear(list);
}
// Same-TU search helper inlined at func_8027BC14. Argument order (self, key)
// reproduces retail's register coloring (the located actor lands in r31).
static cf::CChainActor* findActorForKey(cf::CChainActorList* self, u32 key) {
    _reslist_node<cf::CChainActor*>* head =
        self->mChainActorList.mStartNodePtr;
    _reslist_node<cf::CChainActor*>* node = head->mNext;
    while (node != head) {
        cf::CChainActor* x = node->mItem;
        if (key == x->unk0) return x;
        node = node->mNext;
    }
    return 0;
}

// Chain-activation validator
// when the candidate passes its activation gates AND at least two actors in
// the list are chainable against it, else 0.
int func_8027BC14(cf::CChainActorList* self, u32 key){
    // Locate the actor whose unk0 matches key.
    cf::CChainActor* actor = findActorForKey(self, key);
    if (actor == 0) return 0;
    // The candidate must pass its own activation gate (vtable[19]).
    int gate = (actor != 0) ? actor->CChain_getZero_A9EC() : 0;
    if (gate == 0) return 0;
    // ... and must be chainable against the key's voice sub-object.
    if (actor->func_8027A024(
            reinterpret_cast<cf::CChainBattleObj*>(key)->mSub.v17()) == 0)
        return 0;
    // ... and must pass the "already chained" anti-gate (vtable[29]).
    if (actor->CChain_getZero_A9D0() != 0) return 0;
    // The battle object's probed address must not be in any of three states.
    if (func_80148778(&reinterpret_cast<cf::CChainBattleObj*>(key)->mSub8, 0xeb) != 0) return 0;
    if (func_80148778(&reinterpret_cast<cf::CChainBattleObj*>(key)->mSub8, 0xcb) != 0) return 0;
    if (func_80148778(&reinterpret_cast<cf::CChainBattleObj*>(key)->mSub8, 0xf8) != 0) return 0;
    // The battle object's arts-selection state must not match the no-chain id.
    int local = *reinterpret_cast<int*>(reinterpret_cast<cf::CChainBattleObj*>(key)->field_04->f30());
    if (func_80174C98((void*)key, &local, 0x1f) != 0) return 0;
    if (getAnimModelId(reinterpret_cast<cf::CChainBattleObj*>(key)->field_3F60) == 0x31)
        return 0;
    if (actor->func_8027A338(1) == 0) return 0;
    // Count how many other actors are chainable against the candidate. The
    // sentinel is NOT cached here: retail re-reads self->... every iteration.
    int count = 0;
    _reslist_node<cf::CChainActor*>* cur;
    for (cur = self->mChainActorList.mStartNodePtr->mNext;
         cur != self->mChainActorList.mStartNodePtr; cur = cur->mNext) {
        if (cur->mItem->CChain_getZero_A9FC((void*)(int)key) != 0) {
            if (cur->mItem->func_8027A024(
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
// Returns 1 if some reslist actor is a valid chain-activation target, else 0.
// The target object (actor->unk0) is re-read before each probe so it stays in
// temps across the func_80148778 calls; only the post-check read lives in a
// callee-saved register (retail r31), matching the Chain_SweepDeadActors pattern.
int Chain_HasValidTarget(cf::CChainActorList* self){
    _reslist_node<cf::CChainActor*>* node;
    cf::CChainBattleObj* obj;
    node = self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        if (func_80148778(&reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0)->mSub8, 0x10c) == 0 &&
            func_80148778(&reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0)->mSub8, 0xf8) == 0) {
            obj = reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0);
            int local = *reinterpret_cast<int*>(reinterpret_cast<cf::CChainBattleObj*>(node->mItem->unk0)->field_04->f30());
            if (func_80174C98(obj, &local, 0x801) != 0) return 1;
        }
        node = node->mNext;
    }
    return 0;
}
// When the chain flag bit 1 is set, query a value and emit threshold events.
void Chain_EmitThresholdEvents(cf::CChainFlag* self) {
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

void Chain_EmitCountEvents(cf::CChainFlag* self) {
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
void ChainChance_Reset(cf::CChainChanceS* self) {
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
// Rolls the dice on a chain activation between battle objects @p objA and
// @p objB under the chain state in @p self. The threshold comes from the
// tuning-pool record selected by objA's vtable(+0x308) query, offset by one
// 0x14-byte record when the step flag at +0xC is consumed, and scaled by the
// pair value capped at 5000. Succeeds when mtRand(100) exceeds the score.
// C linkage comes from the func_8027C1A8 declaration in the TU header
// (retail callers bl the unmangled label; MWCC_CASES CBattleMan_OnActorsEmpty record).
int func_8027C1A8(cf::CChainChanceS* self,
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
        val = (u16)IdTable_SumValues(objA, 0x6b);
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
int Chain_RollActivation(cf::CChainMsg* self) {
    // Retail's call site is a stale-register call: no arguments are set up.
    // Route through an unprototyped pointer type so the call passes none;
    // MWCC folds the constant function address back into a direct bl.
    typedef int (*UnprotoChainRollFn)();
    if (((UnprotoChainRollFn)func_8027C1A8)() != 0) {
        chainResolveMemberPtr(0xc3, 0);
        self->field_0x0 = 0xa;
        return 1;
    }
    return 0;
}
// Emits chain-tally threshold events based on the accumulated counter.
// ChainCounter_Drain calls the s32 overload declared above (a different mangled
// symbol), so this u32 definition is never inlined there and stays a
// byte-identical match.
void Chain_EmitTallyEvents(unsigned int param) {
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
// Reads the current pad press. If a chain-trigger button is held, performs the
// action selected by func_8017FD4C; writes result to *out and returns 1.
int Chain_ReadPadAction(cf::CChainAction* self, u8* out){
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
void ChainList_SweepDead(cf::CChainList* self){
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
void ChainList_UpdateAll(cf::CChainList* self) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* a = self->mActors[i];
        a->CChainActor_CleanupVoiceEnd();
    }
}
// Removes the actor whose unk0 matches @p key (if any); returns whether found.
int ChainList_RemoveKey(cf::CChainList* self, u32 key){
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
// extern "C" definition (same idiom as func_8027B8C8/ChainList_Clear): retail's
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
void ChainList_StepTargets(cf::CChainList* self, int target){
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
int ChainList_HasCount(cf::CChainList* self, int key) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* actor = self->mActors[i];
        if (actor->CChain_getChainCount() == key)
            return 1;
    }
    return 0;
}
// Checks (via func_80174C98) whether the resident actors satisfy @p condition;
// the polarity of the result depends on @p check.
int ChainList_CheckCondition(cf::CChainList* self, int target, int check){
    cf::CChainActor** p;
    cf::CChainBattleObj* obj;
    if (check == 0) {
        obj = 0;
        p = self->mActors;
        for (int i = 0; i < (int)self->mCount; i++) {
            obj = reinterpret_cast<cf::CChainBattleObj*>((*p)->unk0);
            int local = *reinterpret_cast<int*>(obj->field_04->f30());
            if (func_80174C98(obj, &local, target) != 0) return 1;
            p++;
        }
        return 0;
    }
    obj = 0;
    p = self->mActors;
    for (int i = 0; i < (int)self->mCount; i++) {
        obj = reinterpret_cast<cf::CChainBattleObj*>((*p)->unk0);
        int local = *reinterpret_cast<int*>(obj->field_04->f30());
        if (func_80174C98(obj, &local, target) == 0) return 0;
        p++;
    }
    return 1;
}
// If the counter is positive, runs chain update steps and resets it.
void ChainCounter_Drain(cf::CChainCounter* self) {
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
void ChainMusic_SaveSlots(cf::CChainMusic* self, cf::CChainBattleObj* owner){
    cf::CArtsSet* arts = (cf::CArtsSet*)(cf::CArtsSet*)reinterpret_cast<cf::CChainBattleObj*>(owner)->v157();
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
void ChainMusic_RestoreSlots(cf::CChainMusic* self, cf::CChainBattleObj* owner){
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
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"
#include "kyoshin/cf/CArtsSet.hpp"
#include <cstring>

// Cross-unit C-linkage callees / globals referenced by the functions below.
extern "C" void func_802A07F4(int, void*);
extern "C" u32 func_8013C54C();
extern "C" void func_8013E800(int);
extern "C" int lbl_eu_80662A80;
extern "C" int func_800B8920(void* addr);
extern "C" int func_80148778(void*, int);
extern "C" int func_80174C98(void*, int*, int);
extern "C" void func_80082568__Q22cf13CfGameManagerFv(int, int, int);
extern "C" int func_8017FD44(void*);
extern "C" int func_8017FD4C(int);
extern "C" int getArtsSlotRC(const void* arts, short index, short subindex);
extern "C" void* getArtsParamRC2(const void* arts, int index, int subindex);

bool func_8027C1A8();
void func_8027C45C(cf::CChainList* self);
float lbl_eu_80668A80;

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
    virtual void v004() = 0; // slot 6  / +24  (chain-combo advance)
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual void v010() = 0; // slot 12 / +48
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
    virtual int  v021() = 0; // slot 23 / +92  (chainable check)
    virtual void v022() = 0;
    virtual void v023() = 0;
    virtual void v024() = 0;
    virtual void v025() = 0;
    virtual void v026() = 0;
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
    CChainActorList::CChainActorList(){

    }

    CChainActorList::~CChainActorList(){
        
    }
}

// Appends @p p to the actor list at index mCount, then increments mCount.
void func_8027C5CC(cf::CChainList* self, cf::CChainActor* p) {
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

void func_8027B164(){}
// Removes every reslist actor whose referenced object is dead.
void func_8027B200(cf::CChainActorList* self){
    _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
    _reslist_node<cf::CChainActor*>* node = head->mNext;
    while (node != head) {
        cf::CChainActor* actor = node->mItem;
        u32 base = actor->unk0;
        if (base != 0) base += 0x3e9c;
        if (func_800B8920((void*)base) == 0) {
            ((CChainActorVFn*)actor->mVTable)[5](actor);
            self->mChainActorList.remove(actor);
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
    cf::CChainActor* chainArr[0x3B];
    cf::CChainActor* otherArr[0x3B];
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        int chainCount = 0;
        int chainIdx = 0;
        int otherCount = 0;
        int otherIdx = 0;
        int key = ((CChainActorIVFn*)node->mItem->mVTable)[26](node->mItem);
        _reslist_node<cf::CChainActor*>* cur = node;
        while (true) {
            ((CChainActorVFn*)cur->mItem->mVTable)[4](cur->mItem);
            if (((CChainActorChkFn*)cur->mItem->mVTable)[8](
                    cur->mItem, key) != 0) {
                chainArr[chainIdx++] = cur->mItem;
                chainCount++;
            } else {
                otherArr[otherIdx++] = cur->mItem;
                otherCount++;
            }
            cur = cur->mNext;
            if (cur == self->mChainActorList.mStartNodePtr) break;
            if (((CChainActorChkFn2*)node->mItem->mVTable)[17](
                    node->mItem, cur->mItem) == 0) break;
        }

        // Pass: link chainable and non-chainable actors into a ring.  Branch A
        // is taken when >=2 chainable actors AND the first passes its vtable[19]
        // gate; it links chain actors with r4=1.  Otherwise (branch B) every
        // call uses r4=0 in the first slot.  r6 is 1 when linking to the
        // wrapping-around next actor.
        int chk = 0;
        if (chainCount >= 2) {
            cf::CChainActor* first = chainArr[0];
            chk = (first != 0) ? ((CChainActorIVFn*)first->mVTable)[19](first)
                               : 0;
        }
        if (chainCount >= 2 && chk != 0) {
            if (chainCount == 1) {
                ((CChainActorChainFn*)chainArr[0]->mVTable)[12](
                    chainArr[0], 0, 0, 0);
            } else if (chainCount == 2) {
                ((CChainActorChainFn*)chainArr[0]->mVTable)[12](
                    chainArr[0], 1, chainArr[1], 1);
                ((CChainActorChainFn*)chainArr[1]->mVTable)[12](
                    chainArr[1], 0, 0, 0);
            } else {
                for (int i = 0; i < chainCount; i++) {
                    ((CChainActorChainFn*)chainArr[i]->mVTable)[12](
                        chainArr[i], 1,
                        chainArr[(i + 1) % chainCount], 1);
                }
            }
            if (otherCount == 1) {
                ((CChainActorChainFn*)otherArr[0]->mVTable)[12](
                    otherArr[0], 0, 0, 0);
            } else if (otherCount == 2) {
                ((CChainActorChainFn*)otherArr[0]->mVTable)[12](
                    otherArr[0], 0, otherArr[1], 1);
                ((CChainActorChainFn*)otherArr[1]->mVTable)[12](
                    otherArr[1], 0, 0, 0);
            } else if (otherCount >= 3) {
                for (int i = 0; i < otherCount; i++) {
                    ((CChainActorChainFn*)otherArr[i]->mVTable)[12](
                        otherArr[i], 0,
                        otherArr[(i + 1) % otherCount], 1);
                }
            }
        } else {
            if (chainCount == 1) {
                ((CChainActorChainFn*)chainArr[0]->mVTable)[12](
                    chainArr[0], 0, 0, 0);
            } else if (chainCount == 2) {
                ((CChainActorChainFn*)chainArr[0]->mVTable)[12](
                    chainArr[0], 0, chainArr[1], 1);
                ((CChainActorChainFn*)chainArr[1]->mVTable)[12](
                    chainArr[1], 0, 0, 0);
            } else if (chainCount >= 3) {
                for (int i = 0; i < chainCount; i++) {
                    ((CChainActorChainFn*)chainArr[i]->mVTable)[12](
                        chainArr[i], 0,
                        chainArr[(i + 1) % chainCount], 1);
                }
            }
            if (otherCount == 1) {
                ((CChainActorChainFn*)otherArr[0]->mVTable)[12](
                    otherArr[0], 0, 0, 0);
            } else if (otherCount == 2) {
                ((CChainActorChainFn*)otherArr[0]->mVTable)[12](
                    otherArr[0], 0, otherArr[1], 1);
                ((CChainActorChainFn*)otherArr[1]->mVTable)[12](
                    otherArr[1], 0, 0, 0);
            } else if (otherCount >= 3) {
                for (int i = 0; i < otherCount; i++) {
                    ((CChainActorChainFn*)otherArr[i]->mVTable)[12](
                        otherArr[i], 0,
                        otherArr[(i + 1) % otherCount], 1);
                }
            }
        }

        node = cur;
    }
}
void func_8027B770(){}
void func_8027B814(){}
// Inserts @p actor into $self's reslist at the position ordered by
// vtable slot 21 (a priority/arts value), before the first actor that is
// chainable (slot 17) and strictly larger. Uses an empty node slot from the
// preallocated node array, as reslist::insert does.
void func_8027B8C8(cf::CChainActorList* self, cf::CChainActor* actor) {
    int myVal = ((CChainActorIVFn*)actor->mVTable)[21](actor);
    _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
    _reslist_node<cf::CChainActor*>* pre = head->mNext;
    while (pre != head) {
        if (((CChainActorIVFn2*)pre->mItem->mVTable)[17](pre->mItem, actor) != 0) {
            while (pre != head &&
                   ((CChainActorIVFn2*)pre->mItem->mVTable)[17](pre->mItem,
                                                                actor) != 0) {
                int val = ((CChainActorIVFn*)pre->mItem->mVTable)[21](
                    pre->mItem);
                if (myVal < val) break;
                pre = pre->mNext;
            }
            break;
        }
        pre = pre->mNext;
    }

    // Find an empty slot in the preallocated node array and link before @p pre.
    int i = 0;
    while (i < self->mChainActorList.mCapacity) {
        if (self->mChainActorList.mList[i].mNext == 0) break;
        i++;
    }
    _reslist_node<cf::CChainActor*>* temp = &self->mChainActorList.mList[i];
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
void func_8027BA0C(cf::CChainActorList* self, cf::CChainList* other,
                   cf::CfObjectActor* target) {
    // 1. Find the actor whose unk0 matches @p target and activate it.
    _reslist_node<cf::CChainActor*>* node =
        self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        if (node->mItem->unk0 == (u32)target) break;
        node = node->mNext;
    }
    cf::CChainActor* found =
        (node != self->mChainActorList.mStartNodePtr) ? node->mItem : 0;
    if (found != 0) ((CChainActorVFn*)found->mVTable)[20](found);
    func_8027C45C(other);

    // 2. Sweep every actor; the ones chainable against target move into @p other.
    node = self->mChainActorList.mStartNodePtr->mNext;
    while (node != self->mChainActorList.mStartNodePtr) {
        if (((CChainActorIVFn2*)node->mItem->mVTable)[18](node->mItem,
                                                        target) != 0) {
            int mv = ((int(*)(void*))(*((void***)((u8*)target + 0x3e9c)))[19])
                     ((u8*)target + 0x3e9c);
            if (((CChainActorIVFn2*)node->mItem->mVTable)[8](
                    node->mItem, (void*)mv) != 0) {
                func_8027C5CC(other, node->mItem);
                self->mChainActorList.remove(node->mItem);
                node = node->mPrev;
            }
        }
        node = node->mNext;
    }
}
// For each actor in @p list, ensures it is present in @p self's reslist
// (adding it via func_8027B8C8 when missing), then clears @p list.
void func_8027BB4C(cf::CChainActorList* self, cf::CChainList* list){
    for (int i = 0; i < (int)list->mCount; i++) {
        cf::CChainActor* actor = (i < (int)list->mCount) ? list->mActors[i] : 0;
        cf::CChainActor* found = 0;
        _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
        _reslist_node<cf::CChainActor*>* node = head->mNext;
        while (node != head) {
            cf::CChainActor* x = node->mItem;
            if (x->unk0 == actor->unk0) { found = x; break; }
            node = node->mNext;
        }
        if (found == 0) func_8027B8C8(self, list->mActors[i]);
    }
    func_8027C45C(list);
}
void func_8027BC14(){}
// Returns 1 if some reslist actor is a valid chain-activation target, else 0.
int func_8027BE84(cf::CChainActorList* self){
    _reslist_node<cf::CChainActor*>* head = self->mChainActorList.mStartNodePtr;
    _reslist_node<cf::CChainActor*>* node = head->mNext;
    while (node != head) {
        cf::CChainActor* actor = node->mItem;
        u32 unk0 = actor->unk0;
        if (func_80148778((void*)(unk0 + 8), 0x10c) != 0) { node = node->mNext; continue; }
        if (func_80148778((void*)(unk0 + 8), 0xf8) != 0) { node = node->mNext; continue; }
        void* sub = *(void**)(unk0 + 4);
        u8** svt = *(u8***)sub;
        int local = ((int(*)(void*))svt[12])(sub);
        if (func_80174C98((void*)unk0, &local, 0x801) != 0) return 1;
        node = node->mNext;
    }
    return 0;
}
extern "C" void func_8027EEF4(int);
extern "C" u32 func_8027EE88(int, int);

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
void func_8027C0B0(){}
// Starts a chain message; writes 0xa to the message id on success.
int func_8027C154(cf::CChainMsg* self) {
    if (func_8027C1A8() != 0) {
        func_802A07F4(0xc3, 0);
        self->field_0x0 = 0xa;
        return 1;
    }
    return 0;
}
bool func_8027C1A8() {}
// Reads the current pad press. If a chain-trigger button is held, performs the
// action selected by func_8017FD4C; writes result to *out and returns 1.
int func_8027C33C(cf::CChainAction* self, u8* out){
    if ((short)self->field_0 > 0) return 0;
    int sel = func_8017FD44(self);
    if (sel == 0) {
        *out = 0;
        return 1;
    }
    return 0;
}
// Zeroes the actor list: clears the pointer array, count, and flag.
void func_8027C45C(cf::CChainList* self) {
    memset(self->mActors, 0, sizeof(self->mActors));
    self->mCount = 0;
    self->mFlag = 0;
}
// Removes from @p self every actor whose referenced object is dead
// (func_800B8920), calling vtable[5] and shifting the array down.
void func_8027C49C(cf::CChainList* self){
    int i = 0;
    while (i < (int)self->mCount) {
        cf::CChainActor** p = &self->mActors[i];
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

// Calls vtable[4] on every actor in the list.
void func_8027C560(cf::CChainList* self) {
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* a = self->mActors[i];
        ((CChainActorVFn*)a->mVTable)[4](a);
    }
}
// Removes the actor whose unk0 matches @p key (if any); returns whether found.
int func_8027C5E4(cf::CChainList* self, u32 key){
    for (int i = 0; i < (int)self->mCount; i++) {
        if (self->mActors[i]->unk0 == key) {
            cf::CChainActor* actor = self->mActors[i];
            ((CChainActorVFn*)actor->mVTable)[5](actor);
            if ((int)self->mCount - i - 1 > 0) {
                memcpy(&self->mActors[i], &self->mActors[i + 1],
                       ((int)self->mCount - 1 - i) * 4);
            }
            self->mCount--;
            self->mFlag = 1;
            return 1;
        }
    }
    return 0;
}
void func_8027C6B4(){}
// For each actor while @p target is nonzero, advances the chain combo
// (vtable[6]), and if the actor is chainable (vtable[23]) and there is a
// distinct next actor, accumulates the arts pair via func_80082568.
void func_8027C924(cf::CChainList* self, int target){
    for (int i = 0; i < (int)self->mCount; i++) {
        cf::CChainActor* actor = self->mActors[i];
        ((CChainActorVFn*)actor->mVTable)[6](actor);
        if (target != 0 && ((CChainActorIVFn*)actor->mVTable)[23](actor) != 0) {
            int next = (i + 1 < (int)self->mCount) ? (i + 1) : (int)self->mCount;
            cf::CChainActor* other = self->mActors[next];
            if (i != next) {
                func_80082568__Q22cf13CfGameManagerFv(
                    *(u16*)(actor->unk0 + 0x3f28),
                    *(u16*)(other->unk0 + 0x3f28), 0xa);
            }
        }
    }
}
void func_8027CA0C(){}
// Checks (via func_80174C98) whether the resident actors satisfy @p condition;
// the polarity of the result depends on @p check.
int func_8027CAE0(cf::CChainList* self, int target, int check){
    if (check == 0) {
        for (int i = 0; i < (int)self->mCount; i++) {
            u32 unk0 = self->mActors[i]->unk0;
            void* sub = *(void**)(unk0 + 4);
            u8** svt = *(u8***)sub;
            int local = ((int(*)(void*))svt[12])(sub);
            if (func_80174C98((void*)unk0, &local, target) != 0) return 1;
        }
        return 0;
    } else {
        for (int i = 0; i < (int)self->mCount; i++) {
            u32 unk0 = self->mActors[i]->unk0;
            void* sub = *(void**)(unk0 + 4);
            u8** svt = *(u8***)sub;
            int local = ((int(*)(void*))svt[12])(sub);
            if (func_80174C98((void*)unk0, &local, target) == 0) return 0;
        }
        return 1;
    }
}
// If the counter is positive, runs chain update steps and resets it.
void func_8027CBE8(cf::CChainCounter* self) {
    if (self->field_0x0 > 0) {
        func_802AA338__Fv();
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
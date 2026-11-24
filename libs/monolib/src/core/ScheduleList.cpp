// Auto-scaffolded catalog TU for monolib/src/core/ScheduleList
// The ScheduleList / reslist / IScheduleContainer vtables and RTTI are emitted
// by MWCC from the real polymorphic classes below. Their retail linker labels
// are mapped to the compiler spellings in config/*/symbols.txt.

#include <harness_catalog.h>
#include "monolib/core/CSchedule.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/reslist.hpp"

extern "C" void schedReleaseAllItems(CSchedule* self);

// name-guess: the class names are the retail RTTI strings; the member names
// follow the existing SchedList_* retail entry points at their vtable slots.
struct __declspec(novtable) IScheduleContainer {
    virtual ~IScheduleContainer() = 0;
};
inline IScheduleContainer::~IScheduleContainer() {}

struct ScheduleList : IScheduleContainer {
    ScheduleList();
    virtual ~ScheduleList();
    virtual void step(f32 dt);
    virtual void sweepAll();
    virtual void sweepMatched(u8* x);
    virtual CSchedule* add(ScheduleEntry* entries, u8* p8, u8* pc,
                          u32 fa, u32 fb, u8* p10);
    virtual void drop(CSchedule* item);
    virtual void dropByField14(u32 x);
    virtual void clearMatchTags(u32 x);
    virtual u32 tally();
    virtual void* ringHead();

    reslist<CSchedule*> mRes;
};

typedef _reslist_node<CSchedule*> SLNode;

template<> _reslist_base<CSchedule*>::~_reslist_base() {
    SLNode* cur;
    SLNode* node;
    node = mStartNodePtr->mNext;
    while (node != mStartNodePtr) {
        cur = node;
        node = node->mNext;
        cur->mNext = 0;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;
    if (unk1C == 0) {
        if (mList != 0) {
            operator delete[](mList);
            mList = 0;
        }
    }
}

// ~reslist<CSchedule*> inlines the base destructor body exactly as retail.
template<> reslist<CSchedule*>::~reslist() {}

typedef ScheduleList SLList;

// ============================================================================
// .sbss (16B): zero-init globals owned by this unit.
// ============================================================================
extern "C" SLList* sMainScheduleList = 0;
extern "C" SLList* sAlternateScheduleList = 0;
extern "C" void*   lbl_eu_80665A58 = 0;
extern "C" char    lbl_eu_80665A5C = 0;

// ---------------------------------------------------------------------------
// Virtual-dispatch interfaces. Both object families carry their vtable
// pointer inside the retail layout (SLList at 0x00 as mSelfVtbl), so the
// retail entry points are reached with
// genuine C++ virtual calls. These abstract interface types mirror the
// retail slots (two reserved entries precede the destructor slot in both
// tables); nothing here is constructed or deleted, so MWCC emits no
// compiler-generated vtables or typeinfo for them.
// ---------------------------------------------------------------------------
struct SLDispatch {
    virtual void destroy(int flag) = 0;                     // +0x08
    virtual void step(f32 dt) = 0;                          // +0x0c
    virtual void updateAll() = 0;                           // +0x10
    virtual void find(void* x) = 0;                         // +0x14
    virtual CSchedule* add(ScheduleEntry* entries, void* p8,
                           void* pc, u32 fa, u32 fb, void* p10) = 0; // +0x18
    virtual void removeItem(CSchedule* item) = 0;           // +0x1c
    virtual void clearTimes(u32 x) = 0;                     // +0x20
    virtual void slSlot24(u8* x) = 0;                       // +0x24
    virtual u32 count() = 0;                                // +0x28
};



// Global schedule-list mode flag (bit 12 selects the alternate list) packed
// with the mem-manager allocation handle at +2 / +4. Defined in another TU;
// incomplete-array extern keeps MWCC from choosing sda21 (retail uses lis+@l
// far addressing - see MWCC_CASES §1a).
extern u16 lbl_eu_8065FC18[];

static u32 slAllocHandle(void) {
    return *(u32*)&lbl_eu_8065FC18[2];
}

// Compiler-generated ScheduleList ctor installs both vptrs; only the node
// reservation remains in the source-level constructor.
ScheduleList::ScheduleList() {
    mRes.mList = (SLNode*)mtl::MemManager::allocate_array(0x1800, slAllocHandle());
    for (u32 i = 0x200; i != 0; i--) {
        mRes.mList[i - 1].mNext = 0;
    }
    mRes.mCapacity = 0x200;
    SLNode* slNode = mRes.mStartNodePtr->mNext;
    while (slNode != mRes.mStartNodePtr) {
        SLNode* slCur = slNode;
        slNode = slNode->mNext;
        slCur->mNext = 0;
    }
    mRes.mStartNodePtr->mNext = mRes.mStartNodePtr;
    mRes.mStartNodePtr->mPrev = mRes.mStartNodePtr;
}

// ---------------------------------------------------------------------------
// SchedList_StepAll: step every schedule (CSchedule::Update with a delta), then
// remove-and-destroy any schedule whose flags select the removal path
// (bit 10 set, bit 15 clear).
// ---------------------------------------------------------------------------
void ScheduleList::step(f32 dt) {
    SLNode* node;
    u32 count = 0;
    CSchedule* item;
    SLNode* sentinel;
    u16 flags;
    SLNode* rnode;
    int removed;

    sentinel = mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(SLNode**)&mRes.mStartNodePtr)->mNext;
        while (node != mRes.mStartNodePtr && node != 0) {
            (node->mItem)->Update(dt);
            node = node->mNext;
        }
        do {
            removed = 0;
            sentinel = mRes.mStartNodePtr;
            rnode = sentinel->mNext;
            while (rnode != sentinel && rnode != 0) {
                item = rnode->mItem;
                if (item->field_0x00.b10 && !item->field_0x00.b15) {
                    if (item != 0) {
                        delete item;
                        rnode->mItem = 0;
                    }
                    {
                        SLNode* prev = rnode->mPrev;
                        SLNode* next = rnode->mNext;
                        prev->mNext = next;
                        next->mPrev = prev;
                        rnode->mNext = 0;
                    }
                    removed = 1;
                    break;
                }
                rnode = rnode->mNext;
            }
        } while (removed != 0);
    }
}

// SchedList_SweepAll: schedReleaseAllItems on each schedule, guarded by a non-empty list.
void ScheduleList::sweepAll() {
    SLNode* node;
    u32 count = 0;
    SLNode* sentinel;
    sentinel = mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(SLNode**)&mRes.mStartNodePtr)->mNext;
        while (node != mRes.mStartNodePtr && node != 0) {
            schedReleaseAllItems(node->mItem);
            node = node->mNext;
        }
    }
}

// SchedList_SweepMatched: CSchedule::FeedTargetDist on each schedule whose field_0x10 matches x.
void ScheduleList::sweepMatched(u8* x) {
    SLNode* node;
    u32 count = 0;
    SLNode* sentinel;
    sentinel = mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(SLNode**)&mRes.mStartNodePtr)->mNext;
        while (node != mRes.mStartNodePtr) {
            CSchedule* item = node->mItem;
            if (x == item->field_0x10) {
                item->FeedTargetDist();
            }
            node = node->mNext;
        }
    }
}

// ---------------------------------------------------------------------------
// SchedList_AddItem: allocate a CSchedule, initialize it with the six forwarded
// arguments, and push it onto the ring's first free slot.
//
// The tail is reslist<CSchedule*>::push_back inlined in place: the sentinel is
// captured before the free-slot scan, then setItem's try/catch guarded store
// (which carries the exception-frame sp-save) and the splice onto the sentinel.
// ---------------------------------------------------------------------------
CSchedule* ScheduleList::add(ScheduleEntry* entries, u8* p8, u8* pc,
                             u32 fa, u32 fb, u8* p10) {
    CSchedule* p;

    if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) >= 0xf0) {
        p = new (slAllocHandle()) CSchedule;
    } else {
        p = 0;
    }
    if (p == 0) {
        return 0;
    }
    if (p->Init(entries, p8, pc, fa, fb, p10) == 0) {
        if (p != 0) {
            delete p;
        }
        return 0;
    }

    SLNode* startNode = mRes.mStartNodePtr;
    int i = mRes.findFirstEmptySlotIndex();
    SLNode* temp = &mRes.mList[i];
    temp->setItem(p);
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
    return p;
}

// SchedList_DropItem: find node holding x, destroy x, unlink the node.
void ScheduleList::drop(CSchedule* x) {
    SLNode* node;
    SLNode* sentinel;
    if (x == 0) return;
    sentinel = mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel && node->mItem != x) {
        node = node->mNext;
    }
    if (node == sentinel) return;
    if (x != 0) {
        delete x;
    }
    {
        SLNode* prev = node->mPrev;
        SLNode* next = node->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        node->mNext = 0;
    }
}

// SchedList_DropByField14: remove first node whose item's field_0x14 matches x;
// field_0x18 match clears instead.
void ScheduleList::dropByField14(u32 x) {
    SLNode* node;
    SLNode* cur;
    CSchedule* item;
    if (x == 0) return;
    node = (*(SLNode**)&mRes.mStartNodePtr)->mNext;
    while (node != mRes.mStartNodePtr) {
        cur = node;
        node = node->mNext;
        item = cur->mItem;
        if (item->field_0x14 == x) {
            if (item != 0) {
                delete item;
                cur->mItem = 0;
            }
            {
                SLNode* prev = cur->mPrev;
                SLNode* next = cur->mNext;
                prev->mNext = next;
                next->mPrev = prev;
                cur->mNext = 0;
            }
        } else if (item->field_0x18 == x) {
            item->field_0x18 = 0;
        }
    }
}

// SchedList_ClearMatchTags: clear any schedule's field_0x14/field_0x18 that matches x.
void ScheduleList::clearMatchTags(u32 x) {
    u32 zero = 0;
    SLNode* cur;
    SLNode* node;
    SLNode* sentinel;
    CSchedule* item;
    sentinel = mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != mRes.mStartNodePtr) {
        cur = node;
        node = node->mNext;
        item = cur->mItem;
        if (item->field_0x14 == x) {
            item->field_0x14 = zero;
        } else if (item->field_0x18 == x) {
            item->field_0x18 = zero;
        }
    }
}

// SchedList_TallyItems: count the schedules in the ring (excludes the sentinel).
u32 ScheduleList::tally() {
    SLNode* node;
    SLNode* sentinel;
    u32 count = 0;
    sentinel = mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    return count;
}

// SchedList_RingHead: walk the sentinel ring back to its head.
void* ScheduleList::ringHead() {
    SLNode* head = mRes.mStartNodePtr;
    SLNode* cur = head->mNext;
    while (cur != head) {
        cur = cur->mNext;
    }
    return head;
}

// ---------------------------------------------------------------------------
// __ct__804E4B64: allocate a ScheduleList (0x24), construct it, and store it
// into the global list selected by arg (0 -> sMainScheduleList, else sAlternateScheduleList).
// ---------------------------------------------------------------------------
void __ct__804E4B64(int arg) {
    SLList* list;
    if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
        list = 0;
    } else {
        list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
        if (list != 0) {
            new (list) ScheduleList();
        }
    }
    if (arg == 0) {
        sMainScheduleList = list;
    }
    if (arg != 0) {
        sAlternateScheduleList = list;
    }
}

// ~ScheduleList destroys the schedules and clears the ring; MWCC emits the
// ScheduleList vptr store and inlines ~reslist after the source body.
ScheduleList::~ScheduleList() {
    SLNode* iNode;
    CSchedule* item;
    SLNode* clNode;
    SLNode* clCur;

    iNode = mRes.mStartNodePtr->mNext;
    while (iNode != mRes.mStartNodePtr) {
        item = iNode->mItem;
        if (item != 0) {
            delete item;
            iNode->mItem = 0;
        }
        iNode = iNode->mNext;
    }
    clNode = mRes.mStartNodePtr->mNext;
    while (clNode != mRes.mStartNodePtr) {
        clCur = clNode;
        clNode = clNode->mNext;
        clCur->mNext = 0;
    }
    mRes.mStartNodePtr->mNext = mRes.mStartNodePtr;
    mRes.mStartNodePtr->mPrev = mRes.mStartNodePtr;
}

extern "C" void SchedList_TeardownAll(u32 x) {
    if (x != 0) {
        if (sAlternateScheduleList != 0) {
            if (sAlternateScheduleList != 0) {
                ((SLDispatch*)sAlternateScheduleList)->destroy(1);
            }
            sAlternateScheduleList = 0;
        }
    }
    if (sMainScheduleList != 0) {
        if (sMainScheduleList != 0) {
            ((SLDispatch*)sMainScheduleList)->destroy(1);
        }
        sMainScheduleList = 0;
    }
}

// SchedList_AdvanceBoth: advance both global schedule lists (vtable 0x10).
extern "C" void SchedList_AdvanceBoth() {
    SLList* a = sMainScheduleList;
    if (a != 0) {
        ((SLDispatch*)a)->updateAll();
    }
    SLList* b = sAlternateScheduleList;
    if (b != 0) {
        ((SLDispatch*)b)->updateAll();
    }
}

// SchedList_StepBoth: forward delta to both global schedule lists (vtable 0xc).
extern "C" void SchedList_StepBoth(f32 x) {
    SLList* a = sMainScheduleList;
    if (a != 0) {
        ((SLDispatch*)a)->step(x);
    }
    SLList* b = sAlternateScheduleList;
    if (b != 0) {
        ((SLDispatch*)b)->step(x);
    }
}

// SchedList_FindBoth: hand an argument to both global schedule lists (vtable 0x14).
extern "C" void SchedList_FindBoth(u32 x) {
    SLList* a = sMainScheduleList;
    if (a != 0) {
        ((SLDispatch*)a)->find((void*)x);
    }
    SLList* b = sAlternateScheduleList;
    if (b != 0) {
        ((SLDispatch*)b)->find((void*)x);
    }
}

// SchedList_RouteAddReq: route an add-schedule request to one of the two lists.
extern "C" void SchedList_RouteAddReq(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5) {
    SLList* list;
    u32 use54 = 0;
    if (((lbl_eu_8065FC18[0] >> 12) & 1) != 0 && p4 == 0) {
        use54 = 1;
    }
    list = use54 ? sAlternateScheduleList : sMainScheduleList;
    ((SLDispatch*)list)->add((ScheduleEntry*)p0, (void*)p1,
                             (void*)p2, p3, p4, (void*)p5);
}

// ---------------------------------------------------------------------------
// __ct__804E4F9C: sinit that rebuilds both global schedule lists.
// ---------------------------------------------------------------------------
void __ct__804E4F9C() {
    SLList* list;
    SLList* old;
    if (sMainScheduleList != 0) {
        if (sMainScheduleList != 0) {
            ((SLDispatch*)sMainScheduleList)->destroy(1);
        }
        sMainScheduleList = 0;
    }
    if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
        list = 0;
    } else {
        list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
        if (list != 0) {
            new (list) ScheduleList();
        }
    }
    sMainScheduleList = list;

    old = sAlternateScheduleList;
    if (old != 0) {
        if (old != 0) {
            ((SLDispatch*)old)->destroy(1);
        }
        sAlternateScheduleList = 0;
        if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
            list = 0;
        } else {
            list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
            if (list != 0) {
                new (list) ScheduleList();
            }
        }
        sAlternateScheduleList = list;
    }
}

// SchedList_DropBoth: forward an argument to both global schedule lists (vtable 0x1c).
extern "C" void SchedList_DropBoth(u32 x) {
    SLList* a = sMainScheduleList;
    if (a != 0) {
        ((SLDispatch*)a)->removeItem((CSchedule*)x);
    }
    SLList* b = sAlternateScheduleList;
    if (b != 0) {
        ((SLDispatch*)b)->removeItem((CSchedule*)x);
    }
}

// SchedList_TallyBoth: sum the per-list counts reported by both global lists.
extern "C" u32 SchedList_TallyBoth() {
    u32 total = 0;
    SLList* a = sMainScheduleList;
    if (a != 0) {
        total = ((SLDispatch*)a)->count();
    }
    SLList* b = sAlternateScheduleList;
    if (b != 0) {
        total += ((SLDispatch*)b)->count();
    }
    return total;
}

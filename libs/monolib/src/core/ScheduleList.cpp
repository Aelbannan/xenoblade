// Auto-scaffolded catalog TU for monolib/src/core/ScheduleList
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/core/CSchedule.hpp"
#include "monolib/util/MemManager.hpp"

// Imports from code_804E36DC.cpp / CSchedule.cpp (defined there with C
// linkage; the canonical declarations live in headers that are out of this
// TU's writable scope).
extern "C" void func_804E3B08(CSchedule* self);
extern "C" void func_804E3B6C(CSchedule* self);
extern "C" void func_804E36DC(CSchedule* self, f32 dt);
extern "C" void __ct__CSchedule(CSchedule* self);
extern "C" int func_804E3434(CSchedule* self, ScheduleEntry* entries,
                             u8* field_0x08_ptr, u8* field_0x0c_ptr,
                             u32 flags_a, u32 flags_b, u8* field_0x10_ptr);

// ---------------------------------------------------------------------------
// reslist<CSchedule> mirror (the canonical template lives in
// monolib/util/reslist.hpp, which is out of this TU's writable scope). The
// retail CSchedule instantiation stores a POINTER in the node (the walk
// functions dereference mItem at 0x8, and the dtor's delete[] emits a bare
// __dla__ with no per-element dtor loop), so a minimal local mirror with
// T* mItem is used. The base object is embedded inside ScheduleList at
// offset 0x4, giving the retail two vptr slots: the ScheduleList vtable at
// 0x0 (80570078) and the reslist vtable at 0x4 (805700B4, temporarily
// 805700CC while the _reslist_base ctor runs).
// ---------------------------------------------------------------------------

template <typename T>
struct _reslist_node {
    _reslist_node<T>* mNext;  // 0x0
    _reslist_node<T>* mPrev;  // 0x4
    T* mItem;                 // 0x8
};

template <typename T>
class _reslist_base {
public:
    _reslist_base() {
        mList = nullptr;
        mCapacity = 0;
        unk1C = false;
        mStartNodePtr = &mStartNode;
        mStartNodePtr->mNext = &mStartNode;
        mStartNodePtr->mPrev = mStartNode.mNext;
    }

    virtual ~_reslist_base() {
        _reslist_node<T>* node = mStartNodePtr->mNext;
        while (node != mStartNodePtr) {
            _reslist_node<T>* cur = node;
            node = node->mNext;
            cur->mNext = nullptr;
        }
        mStartNodePtr->mNext = mStartNodePtr;
        mStartNodePtr->mPrev = mStartNodePtr;
        if (unk1C == false) {
            if (mList != nullptr) {
                delete[] mList;
                mList = nullptr;
            }
        }
    }

    _reslist_node<T>* mStartNodePtr;  // 0x4
    _reslist_node<T> mStartNode;      // 0x8
    _reslist_node<T>* mList;          // 0x14
    int mCapacity;                    // 0x18
    bool unk1C;                       // 0x1c
};

template <typename T>
class reslist : public _reslist_base<T> {
public:
    reslist() : _reslist_base<T>() {}
    virtual ~reslist() {}
};

// Emit the retail dtor instantiations.
template class _reslist_base<CSchedule>;
template class reslist<CSchedule>;

// Node of the ScheduleList's intrusive ring list: links at 0x0/0x4, then a
// pointer to the schedule item at 0x8.
typedef _reslist_node<CSchedule> ScheduleListNode;

// The retail CSchedule stores a vtable-like pointer at offset 0xec whose
// entry [2] is the destroy dispatch (called with delete flag 1). The
// struct-with-fn-pointer form reproduces the retail two-load dispatch
// `lwz r12,0xec(r3); lwz r12,8(r12); mtctr; bctrl`.
struct CScheduleTable {
    void* mSlot0;                 // 0x0
    void* mSlot1;                 // 0x4
    void (*mDestroy)(void*, int); // 0x8
    void* mSlot3;                 // 0xc
};

// Fake single-inheritance iface (MWCC_REFERENCE "double-hop thunks" pattern):
// a non-polymorphic base of 0xec bytes shifts the vptr to offset 0xec (the
// retail CSchedule's mTablePtr), and MWCC emits the retail dispatch form
// byte-for-byte (vptr load into r12, this stays the CSchedule, destroy at
// vtable[2] after the two hidden typeinfo slots).
struct CSchedShift {
    u8 pad[0xec];  // mirrors CSchedule's 0x0..0xeb data region
};
struct CSchedIf : CSchedShift {
    virtual void destroy(int flag) {}  // vtable[2]
};

// Bitfield view of CSchedule::field_0x00 (u16): bit 10 (0x400) schedules the
// item for destruction, bit 15 (0x8000) gates the removal path. Reading a
// named bitfield makes MWCC emit the retail extrwi. tests.
struct CSchedFlagBits {
    u16 b15 : 1;   // bit 15 (0x8000)
    u16 : 4;       // bits 14..11
    u16 b10 : 1;   // bit 10 (0x400)
    u16 : 10;      // bits 9..0
};

// Schedule list. Polymorphic (vtable at 0x0); the ring sentinel pointer
// lives at 0x8. MWCC emits two dtor slots ahead of the declared virtuals, so
// the first declared virtual sits at vtable 0x8. An embedded reslist base
// sits at offset 0x4 (its own vptr at 0x4; the reslist dtor re-runs the
// clear and frees the slot array when the ScheduleList dtor runs).
class ScheduleList {
public:
    virtual void vfunc00(u32 x);  // vtable 0x8
    virtual void vfunc03(f32 x);  // vtable 0xc
    virtual void vfunc04();       // vtable 0x10
    virtual void vfunc05(u32 x);  // vtable 0x14
    virtual void vfunc06(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);  // vtable 0x18
    virtual void vfunc07(u32 x);  // vtable 0x1c
    virtual void vfunc08();       // vtable 0x20
    virtual void vfunc09();       // vtable 0x24
    virtual u32 vfunc0A();        // vtable 0x28
    virtual ~ScheduleList();
    ScheduleList() {}
    reslist<CSchedule> mReslist;  // 0x4: embedded reslist (vptr at 0x4)
};

// The two global schedule lists (sbss), populated by the sinit ctors.
extern ScheduleList* lbl_eu_80665A50;
extern ScheduleList* lbl_eu_80665A54;

// Global schedule-list mode flag (bit 12 selects the alternate list) packed
// with the mem-manager allocation handle at +4. Incomplete-array extern
// keeps MWCC from choosing sda21 (retail uses lis+@l far addressing - see
// MWCC_REFERENCE §1a).
extern u16 lbl_eu_8065FC18[];

// ---------------------------------------------------------------------------
// func_804E45F4: step every schedule (func_804E36DC with a delta), then
// remove-and-destroy any schedule whose flags select the removal path
// (bit 10 set, bit 15 clear).
// ---------------------------------------------------------------------------
void func_804E45F4(ScheduleList* self, f32 dt) {
    ScheduleListNode* node;   // count loop + middle walk
    u32 count = 0;
    ScheduleListNode* sentinel;
    ScheduleListNode* rnode;  // removal loop
    int removed;

    sentinel = self->mReslist.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        // Fresh walk-init load: the volatile re-read of the start pointer
        // forces a reload instead of reusing the count loop's value (MWCC
        // CSEs the identical loads otherwise - see MWCC_REFERENCE.md).
        node = (*(volatile ScheduleListNode**)&self->mReslist.mStartNodePtr)->mNext;
        while (node != self->mReslist.mStartNodePtr && node != 0) {
            func_804E36DC(node->mItem, dt);
            node = node->mNext;
        }
        // Remove-and-destroy loop; the walk restarts after every removal.
        do {
            removed = 0;
            sentinel = self->mReslist.mStartNodePtr;
            rnode = sentinel->mNext;
            while (rnode != sentinel && rnode != 0) {
                CSchedule* item = rnode->mItem;
                CSchedFlagBits flags = *(CSchedFlagBits*)&item->field_0x00;
                if (flags.b10 && !flags.b15) {
                    // Doubled null checks mirror retail (MWCC keeps both branches).
                    if (item != 0) {
                        if (item != 0) {
                            ((CSchedIf*)item)->destroy(1);
                        }
                        rnode->mItem = 0;
                    }
                    {
                        ScheduleListNode* prev = rnode->mPrev;
                        ScheduleListNode* next = rnode->mNext;
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

// Run func_804E3B08 (emit/update every resolved item) on each schedule in the
// ring, guarded by a non-empty list check. The volatile re-read of the start
// pointer forces a fresh walk-init load instead of reusing the count loop's
// value (MWCC CSEs the identical loads otherwise - see MWCC_REFERENCE.md).
void func_804E4718(ScheduleList* self) {
    ScheduleListNode* node;
    u32 count = 0;
    ScheduleListNode* sentinel;
    sentinel = self->mReslist.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(volatile ScheduleListNode**)&self->mReslist.mStartNodePtr)->mNext;
        while (node != self->mReslist.mStartNodePtr && node != 0) {
            func_804E3B08(node->mItem);
            node = node->mNext;
        }
    }
}

// Run func_804E3B6C (distance update) on each schedule whose field_0x10
// matches x.
void func_804E479C(ScheduleList* self, u8* x) {
    ScheduleListNode* node;
    u32 count = 0;
    ScheduleListNode* sentinel;
    sentinel = self->mReslist.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(volatile ScheduleListNode**)&self->mReslist.mStartNodePtr)->mNext;
        while (node != self->mReslist.mStartNodePtr) {
            CSchedule* item = node->mItem;
            if (x == item->field_0x10) {
                func_804E3B6C(item);
            }
            node = node->mNext;
        }
    }
}

// ---------------------------------------------------------------------------
// func_804E4830: allocate a CSchedule, initialize it with the six forwarded
// arguments, and push it onto the ring's first free slot. Returns the new
// schedule (or 0 on failure). The setItem try/catch forces the retail
// frame marker (`mr r31,r1` / `stw r1` epilogue - same shape as
// reslist::push_back in CUICfManager).
// ---------------------------------------------------------------------------
CSchedule* func_804E4830(ScheduleList* self, ScheduleEntry* entries,
                         u8* field_0x08_ptr, u8* field_0x0c_ptr,
                         u32 flags_a, u32 flags_b, u8* field_0x10_ptr) {
    ScheduleListNode* sentinel;
    ScheduleListNode* temp;
    u32 capacity;
    u32 i;
    CSchedule* p;

    if (mtl::MemManager::getMaxAllocSize(*(u32*)&lbl_eu_8065FC18[2]) < 0xf0) {
        p = 0;
    } else {
        p = (CSchedule*)mtl::MemManager::allocate(0xf0, *(u32*)&lbl_eu_8065FC18[2]);
        if (p != 0) {
            __ct__CSchedule(p);
        }
    }
    if (p == 0) {
        return 0;
    }
    if (func_804E3434(p, entries, field_0x08_ptr, field_0x0c_ptr,
                      flags_a, flags_b, field_0x10_ptr) == 0) {
        if (p != 0) {
            if (p != 0) {
                ((CSchedIf*)p)->destroy((s16)1);
            }
        }
        return 0;
    }
    // Find the first free slot (a node whose mNext is null).
    sentinel = self->mReslist.mStartNodePtr;
    i = 0;
    capacity = self->mReslist.mCapacity;
    while (i < capacity) {
        if (self->mReslist.mList[i].mNext == 0) {
            break;
        }
        i++;
    }
    // Push the schedule onto the ring (push_back shape with setItem).
    temp = &self->mReslist.mList[i];
    {
        CSchedule** itemPtr = &temp->mItem;
        if (itemPtr != 0) {
            try {
                *itemPtr = p;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = sentinel;
    temp->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = temp;
    sentinel->mPrev = temp;
    return p;
}

// Find the node holding x and remove it from the ring, destroying x first via
// its vtable dispatch (vtable pointer at 0xec, entry [1], delete flag 1). The
// doubled null checks mirror retail (MWCC keeps both branches - same pattern
// as func_804E4D58).
void func_804E498C(ScheduleList* self, CSchedule* x) {
    ScheduleListNode* node;
    ScheduleListNode* sentinel;
    if (x == 0) return;
    sentinel = self->mReslist.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel && node->mItem != x) {
        node = node->mNext;
    }
    if (node == sentinel) return;
    if (x != 0) {
        if (x != 0) {
            ((CSchedIf*)x)->destroy(1);
        }
    }
    {
        ScheduleListNode* prev = node->mPrev;
        ScheduleListNode* next = node->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        node->mNext = 0;
    }
}

// Walk the ring removing the first node whose item matches x (by
// field_0x14), destroying the item through its vtable dispatch. A node whose
// item's field_0x18 matches x is cleared instead; field_0x14 match wins.
void func_804E4A20(ScheduleList* self, u32 x) {
    ScheduleListNode* node;
    ScheduleListNode* cur;
    CSchedule* item;
    if (x == 0) return;
    node = self->mReslist.mStartNodePtr->mNext;
    while (node != self->mReslist.mStartNodePtr) {
        cur = node;
        node = node->mNext;
        item = cur->mItem;
        if (item->field_0x14 == x) {
            if (item != 0) {
                if (item != 0) {
                    ((CSchedIf*)item)->destroy(1);
                }
                cur->mItem = 0;
            }
            {
                ScheduleListNode* prev = cur->mPrev;
                ScheduleListNode* next = cur->mNext;
                prev->mNext = next;
                next->mPrev = prev;
                cur->mNext = 0;
            }
        } else if (item->field_0x18 == x) {
            item->field_0x18 = 0;
        }
    }
}

// Walk the ring, clearing any schedule's field_0x14/field_0x18 that matches x.
void func_804E4AD4(ScheduleList* self, u32 x) {
    u32 zero = 0;
    ScheduleListNode* cur;
    ScheduleListNode* node;
    ScheduleListNode* sentinel;
    CSchedule* item;
    sentinel = self->mReslist.mStartNodePtr;
    node = sentinel->mNext;
    while (node != self->mReslist.mStartNodePtr) {
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

// Count the schedules in the ring (excludes the sentinel node).
u32 func_804E4B24(ScheduleList* self) {
    ScheduleListNode* node;
    ScheduleListNode* sentinel;
    u32 count = 0;
    sentinel = self->mReslist.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    return count;
}

void* func_804E4B48(u8* self) {
    u8* head = *(u8**)((u8*)self + 8);
    u8* cur = *(u8**)head;
    while (cur != head) {
        cur = *(u8**)cur;
    }
    return head;
}

// ---------------------------------------------------------------------------
// __ct__804E4B64: allocate a ScheduleList (0x24 bytes) from the mem-manager
// handle, construct it, and store it into the global list selected by arg
// (0 -> lbl_eu_80665A50, nonzero -> lbl_eu_80665A54).
// ---------------------------------------------------------------------------
void __ct__804E4B64(int arg) {
    ScheduleList* list;
    u32 i;

    if (mtl::MemManager::getMaxAllocSize(*(u32*)&lbl_eu_8065FC18[2]) < 0x24) {
        list = 0;
    } else {
        list = (ScheduleList*)mtl::MemManager::allocate(0x24, *(u32*)&lbl_eu_8065FC18[2]);
        if (list != 0) {
            new (list) ScheduleList;
            // reserve(handle, 0x200): slot array of 0x200 nodes, mNext=0.
            list->mReslist.mList =
                new (*(u32*)&lbl_eu_8065FC18[2]) ScheduleListNode[0x200];
            for (i = 0; i < 0x200; i++) {
                list->mReslist.mList[i].mNext = 0;
            }
            list->mReslist.mCapacity = 0x200;
            // Reset the ring (degenerate on a fresh list; mirrors retail).
            {
                ScheduleListNode* node = list->mReslist.mStartNodePtr->mNext;
                while (node != list->mReslist.mStartNodePtr) {
                    ScheduleListNode* cur = node;
                    node = node->mNext;
                    cur->mNext = 0;
                }
                list->mReslist.mStartNodePtr->mNext = list->mReslist.mStartNodePtr;
                list->mReslist.mStartNodePtr->mPrev = list->mReslist.mStartNodePtr;
            }
        }
    }
    if (arg == 0) {
        lbl_eu_80665A50 = list;
    }
    if (arg != 0) {
        lbl_eu_80665A54 = list;
    }
}

// Terminate the global schedule lists. When x != 0 the second list (54) is
// shut down first; list 50 is always shut down. The redundant inner null
// checks mirror retail: MWCC CSEs the repeated test into the dead second
// branch (see MWCC_REFERENCE.md dead-branch notes).
void func_804E4D58(u32 x) {
    if (x != 0) {
        if (lbl_eu_80665A54 != 0) {
            if (lbl_eu_80665A54 != 0) {
                lbl_eu_80665A54->vfunc00(1);
            }
            lbl_eu_80665A54 = 0;
        }
    }
    if (lbl_eu_80665A50 != 0) {
        if (lbl_eu_80665A50 != 0) {
            lbl_eu_80665A50->vfunc00(1);
        }
        lbl_eu_80665A50 = 0;
    }
}

// Advance both global schedule lists (no-arg update hook at vtable 0x10).
void func_804E4E38() {
    ScheduleList* a = lbl_eu_80665A50;
    if (a != 0) {
        a->vfunc04();
    }
    ScheduleList* b = lbl_eu_80665A54;
    if (b != 0) {
        b->vfunc04();
    }
}

// Forward delta to both global schedule lists (vtable 0xc hook).
void func_804E4DD4(f32 x) {
    ScheduleList* a = lbl_eu_80665A50;
    if (a != 0) {
        a->vfunc03(x);
    }
    ScheduleList* b = lbl_eu_80665A54;
    if (b != 0) {
        b->vfunc03(x);
    }
}

// Hand an argument to both global schedule lists (vtable 0x14 hook).
void func_804E4E8C(u32 x) {
    ScheduleList* a = lbl_eu_80665A50;
    if (a != 0) {
        a->vfunc05(x);
    }
    ScheduleList* b = lbl_eu_80665A54;
    if (b != 0) {
        b->vfunc05(x);
    }
}

// Route an add-schedule request to one of the two global lists: the 54 list
// when the mode flag's bit 12 is set and the fifth argument is null, else the
// 50 list. Forwards all six arguments to the list's vfunc06 (vtable 0x18).
void func_804E4EF8(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5) {
    ScheduleList* list;
    u32 use54 = 0;
    if (((lbl_eu_8065FC18[0] >> 12) & 1) != 0 && p4 == 0) {
        use54 = 1;
    }
    list = use54 ? lbl_eu_80665A54 : lbl_eu_80665A50;
    list->vfunc06(p0, p1, p2, p3, p4, p5);
}

// ---------------------------------------------------------------------------
// __ct__804E4F9C: sinit that rebuilds both global schedule lists. List 50 is
// always recreated; list 54 is only recreated when it already existed. The
// destroy guards are nested so MWCC emits the retail chain of dead branches
// (two levels for list 50, three for list 54).
// ---------------------------------------------------------------------------
void __ct__804E4F9C() {
    ScheduleList* list;
    u32 i;

    if (lbl_eu_80665A50 != 0) {
        if (lbl_eu_80665A50 != 0) {
            lbl_eu_80665A50->vfunc00(1);
        }
        lbl_eu_80665A50 = 0;
    }
    // ---- construct list 50 ----
    if (mtl::MemManager::getMaxAllocSize(*(u32*)&lbl_eu_8065FC18[2]) < 0x24) {
        list = 0;
    } else {
        list = (ScheduleList*)mtl::MemManager::allocate(0x24, *(u32*)&lbl_eu_8065FC18[2]);
        if (list != 0) {
            new (list) ScheduleList;
            list->mReslist.mList =
                new (*(u32*)&lbl_eu_8065FC18[2]) ScheduleListNode[0x200];
            for (i = 0; i < 0x200; i++) {
                list->mReslist.mList[i].mNext = 0;
            }
            list->mReslist.mCapacity = 0x200;
            {
                ScheduleListNode* node = list->mReslist.mStartNodePtr->mNext;
                while (node != list->mReslist.mStartNodePtr) {
                    ScheduleListNode* cur = node;
                    node = node->mNext;
                    cur->mNext = 0;
                }
                list->mReslist.mStartNodePtr->mNext = list->mReslist.mStartNodePtr;
                list->mReslist.mStartNodePtr->mPrev = list->mReslist.mStartNodePtr;
            }
        }
    }
    lbl_eu_80665A50 = list;

    if (lbl_eu_80665A54 != 0) {
        if (lbl_eu_80665A54 != 0) {
            if (lbl_eu_80665A54 != 0) {
                lbl_eu_80665A54->vfunc00(1);
            }
            lbl_eu_80665A54 = 0;
        }
        // ---- construct list 54 ----
        if (mtl::MemManager::getMaxAllocSize(*(u32*)&lbl_eu_8065FC18[2]) < 0x24) {
            list = 0;
        } else {
            list = (ScheduleList*)mtl::MemManager::allocate(0x24, *(u32*)&lbl_eu_8065FC18[2]);
            if (list != 0) {
                new (list) ScheduleList;
                list->mReslist.mList =
                    new (*(u32*)&lbl_eu_8065FC18[2]) ScheduleListNode[0x200];
                for (i = 0; i < 0x200; i++) {
                    list->mReslist.mList[i].mNext = 0;
                }
                list->mReslist.mCapacity = 0x200;
                {
                    ScheduleListNode* node = list->mReslist.mStartNodePtr->mNext;
                    while (node != list->mReslist.mStartNodePtr) {
                        ScheduleListNode* cur = node;
                        node = node->mNext;
                        cur->mNext = 0;
                    }
                    list->mReslist.mStartNodePtr->mNext = list->mReslist.mStartNodePtr;
                    list->mReslist.mStartNodePtr->mPrev = list->mReslist.mStartNodePtr;
                }
            }
        }
        lbl_eu_80665A54 = list;
    }
}

// Forward an argument to both global schedule lists (vtable 0x1c hook).
void func_804E536C(u32 x) {
    ScheduleList* a = lbl_eu_80665A50;
    if (a != 0) {
        a->vfunc07(x);
    }
    ScheduleList* b = lbl_eu_80665A54;
    if (b != 0) {
        b->vfunc07(x);
    }
}

// Sum the per-list counts reported by both global lists (vtable 0x28 hook).
u32 func_804E53D8() {
    u32 total = 0;
    ScheduleList* a = lbl_eu_80665A50;
    if (a != 0) {
        total = a->vfunc0A();
    }
    ScheduleList* b = lbl_eu_80665A54;
    if (b != 0) {
        total += b->vfunc0A();
    }
    return total;
}

// ---------------------------------------------------------------------------
// ScheduleList dtor: destroy every schedule in the ring through its 0xec
// vtable dispatch, then unlink all nodes. The embedded reslist member dtor
// (vtable at 0x4 -> 805700CC) then re-runs the clear and frees the slot
// array.
// ---------------------------------------------------------------------------
ScheduleList::~ScheduleList() {
    ScheduleListNode* node;
    ScheduleListNode* cur;
    CSchedule* item;

    node = mReslist.mStartNodePtr->mNext;
    while (node != mReslist.mStartNodePtr) {
        item = node->mItem;
        if (item != 0) {
            if (item != 0) {
                ((CSchedIf*)item)->destroy(1);
            }
            node->mItem = 0;
        }
        node = node->mNext;
    }
    node = mReslist.mStartNodePtr->mNext;
    while (node != mReslist.mStartNodePtr) {
        cur = node;
        node = node->mNext;
        cur->mNext = 0;
    }
    mReslist.mStartNodePtr->mNext = mReslist.mStartNodePtr;
    mReslist.mStartNodePtr->mPrev = mReslist.mStartNodePtr;
}

// Auto-scaffolded catalog TU for monolib/src/core/ScheduleList
// Dissolved-data form (monolibdata2 splice): the retail split object carries
// the ScheduleList / reslist / IScheduleContainer data sections with RELOCS
// (zero bytes in the .o, real addresses at link), so every pointer is spelled
// as `(u32)&symbol` (R_PPC_ADDR32) rather than a literal. The vtables/RTTI
// are therefore reproduced as raw extern "C" arrays instead of C++ virtual
// class auto-emission (which would add compiler-generated vtables/typeinfo
// with different layout and names).

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
extern "C" void __dt__9CScheduleFv(void* self, int flag);

// MWCC runtime delete helpers (retail `delete` / `delete[]` lowering).
extern "C" void __dla__FPv(void* mem);
extern "C" void __dl__FPv(void* mem);

// ---------------------------------------------------------------------------
// Plain layout mirror of the ScheduleList object (non-polymorphic; the C++
// virtual machinery would auto-emit vtables/RTTI this TU deliberately
// sprawls as raw data).
//
//  0x0 ScheduleList vtable ptr
//  0x4 embedded reslist<CSchedule> (its own vptr)
//    0x4 mStartNodePtr           0xc mStartNode (12B)
//    0x18 mList  0x1c mCapacity  0x20 unk1C   (size 0x24)
// ---------------------------------------------------------------------------
struct SLNode {
    SLNode* mNext;   // 0x0
    SLNode* mPrev;   // 0x4
    void*   mItem;   // 0x8
};

struct SLResBase {
    void*   vtable;         // 0x0
    SLNode* mStartNodePtr;  // 0x4
    SLNode  mStartNode;     // 0x8 (8..0x14)
    SLNode* mList;          // 0x14
    int     mCapacity;      // 0x18
    unsigned char unk1C;    // 0x1c
};

struct SLList {
    void*     mSelfVtbl;    // 0x0
    SLResBase mRes;         // 0x4
};

// Forward declarations so cross-section reloc expressions resolve (the
// .data <-> .sdata locator rings reference each other's labels).
extern "C" const char lbl_eu_80524740[16];
extern "C" const char lbl_eu_80524750[24];
extern "C" const char lbl_eu_80524768[28];
extern "C" const char lbl_eu_80524784[20];
extern "C" u32 lbl_eu_80663C10[2];
extern "C" u32 lbl_eu_80663C18[2];
extern "C" u32 lbl_eu_80663C20[2];
extern "C" u32 lbl_eu_80663C28[2];
extern "C" u32 lbl_eu_805700A8[3];
extern "C" u32 lbl_eu_805700B4[3];
extern "C" u32 lbl_eu_805700C0[3];
extern "C" u32 lbl_eu_805700CC[3];

// ============================================================================
// .rodata (88B, align 8): RTTI typeinfo name strings (no relocs).
// ============================================================================
extern "C" __declspec(align(8)) const char lbl_eu_80524740[16] = {
    'S','c','h','e','d','u','l','e','L','i','s','t', 0,0,0,0,
};
extern "C" const char lbl_eu_80524750[24] = {
    'r','e','s','l','i','s','t','<','C','S','c','h','e','d','u','l','e',' ','*','>', 0,0,0,0,
};
extern "C" const char lbl_eu_80524768[28] = {
    '_','r','e','s','l','i','s','t','_','b','a','s','e','<','C','S','c','h','e','d','u','l','e',' ','*','>', 0,0,
};
extern "C" const char lbl_eu_80524784[20] = {
    'I','S','c','h','e','d','u','l','e','C','o','n','t','a','i','n','e','r', 0,0,
};

// ============================================================================
// .sdata (32B, align 8): RTTI locators { typeinfo name, base vtable }.
// ============================================================================
extern "C" __declspec(align(8)) u32 lbl_eu_80663C10[2] = {
    (u32)&lbl_eu_80524740, (u32)&lbl_eu_805700A8,
};
extern "C" u32 lbl_eu_80663C18[2] = {
    (u32)&lbl_eu_80524750, (u32)&lbl_eu_805700C0,
};
extern "C" u32 lbl_eu_80663C20[2] = {
    (u32)&lbl_eu_80524768, 0,
};
extern "C" u32 lbl_eu_80663C28[2] = {
    (u32)&lbl_eu_80524784, 0,
};

// ============================================================================
// .data (96B, align 8): ScheduleList vtable (48B) + 4 x 12B sub-tables.
// ============================================================================
extern "C" SLResBase* __dt___reslist_base_CSchedule(SLResBase* self, int flag);
extern "C" SLResBase* __dt__reslist_CSchedule(SLResBase* self, int flag);
extern "C" SLList* __dt__12ScheduleListFv(SLList* self, int flag);
extern "C" void func_804E45F4(SLList* self, f32 dt);
extern "C" void func_804E4718(SLList* self);
extern "C" void func_804E479C(SLList* self, u8* x);
extern "C" CSchedule* func_804E4830(SLList* self, ScheduleEntry* entries,
                                    u8* p8, u8* pc, u32 fa, u32 fb, u8* p10);
extern "C" void func_804E498C(SLList* self, CSchedule* x);
extern "C" void func_804E4A20(SLList* self, u32 x);
extern "C" void func_804E4AD4(SLList* self, u32 x);
extern "C" u32 func_804E4B24(SLList* self);
extern "C" void* func_804E4B48(u8* self);

extern "C" __declspec(align(8)) u32 lbl_eu_80570078[12] __attribute__((weak)) = {
    (u32)&lbl_eu_80663C10, 0x00000000,
    (u32)&__dt__12ScheduleListFv, (u32)&func_804E45F4,
    (u32)&func_804E4718, (u32)&func_804E479C,
    (u32)&func_804E4830, (u32)&func_804E498C,
    (u32)&func_804E4A20, (u32)&func_804E4AD4,
    (u32)&func_804E4B24, (u32)&func_804E4B48,
};
extern "C" u32 lbl_eu_805700A8[3] = {
    (u32)&lbl_eu_80663C28, 0, 0,
};
extern "C" u32 lbl_eu_805700B4[3] __attribute__((weak)) = {
    (u32)&lbl_eu_80663C18, 0, (u32)&__dt__reslist_CSchedule,
};
extern "C" u32 lbl_eu_805700C0[3] = {
    (u32)&lbl_eu_80663C20, 0, 0,
};
extern "C" u32 lbl_eu_805700CC[3] __attribute__((weak)) = {
    (u32)&lbl_eu_80663C20, 0, (u32)&__dt___reslist_base_CSchedule,
};

// ============================================================================
// .sbss (24B): zero-init globals owned by this unit.
// ============================================================================
extern "C" SLList* lbl_eu_80665A50 = 0;
extern "C" SLList* lbl_eu_80665A54 = 0;
extern "C" void*   lbl_eu_80665A58 = 0;
extern "C" char    lbl_eu_80665A5C = 0;
extern "C" void*   lbl_eu_80665A60 = 0;
extern "C" char    lbl_eu_80665A64 = 0;
extern "C" char    lbl_eu_80665A65 = 0;
extern "C" char    lbl_eu_80665A66 = 0;
// Trailing byte: retail .sbss packs 23 bytes of objects into a 24-byte
// 8-aligned region (the DOL keeps the section at its rounded size); NOBITS
// gate compares size+align only, so an anonymous tail byte closes it.
extern "C" char    lbl_eu_80665A67 = 0;

// ---------------------------------------------------------------------------
// Virtual-dispatch interfaces. Both object families carry their vtable
// pointer inside the retail layout (SLList at 0x00 as mSelfVtbl; CSchedule
// at 0xEC as mTablePtr), so the retail entry points are reached with
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

struct CSchedDispatchBase { u8 opaque[0xec]; };
struct CSchedDispatch : CSchedDispatchBase {
    virtual void destroy(int flag) = 0;                     // +0x08
};

static void scheduleDestroy(CSchedule* item) {
    ((CSchedDispatch*)item)->destroy(1);
}

// Packed view of CSchedule::field_0x00 (s16 flags word). The schedule-step
// removal path tests two 1-bit fields: destroy request at LSB position 9
// (0x200) and clear-on-match at position 14 (0x4000); MWCC extracts each
// with a record-form rlwinm (SH = 32 - pos - 1).
struct SchedFlagBits {
    u16 clearOnMatch : 1;   // extracted at bit 14
    u16 midPad : 4;
    u16 destroy : 1;        // extracted at bit 9
};

// Global schedule-list mode flag (bit 12 selects the alternate list) packed
// with the mem-manager allocation handle at +2 / +4. Defined in another TU;
// incomplete-array extern keeps MWCC from choosing sda21 (retail uses lis+@l
// far addressing - see MWCC_CASES §1a).
extern u16 lbl_eu_8065FC18[];

static u32 slAllocHandle(void) {
    return *(u32*)&lbl_eu_8065FC18[2];
}

// ---------------------------------------------------------------------------
// ScheduleList constructor body. Retail has ONE ScheduleList::ScheduleList
// that MWCC inlined at every allocation site (__ct__804E4B64 once,
// __ct__804E4F9C twice) - spelled as a macro so each site compiles its own
// copy exactly like an inlined callee. Store order mirrors retail codegen:
// ScheduleList vptr, embedded-reslist ctor vptr (base, then overridden by
// the derived reslist table after member init), member init, node array.
// ---------------------------------------------------------------------------
#define SCHEDULELIST_CTOR_BODY(list)                                       \
    do {                                                                   \
        SLNode* slNode;                                                    \
        SLNode* slCur;                                                     \
        u32 slI;                                                           \
        (list)->mSelfVtbl = (void*)&lbl_eu_80570078;                       \
        (list)->mRes.vtable = (void*)&lbl_eu_805700CC;                     \
        (list)->mRes.mList = 0;                                            \
        (list)->mRes.mCapacity = 0;                                        \
        (list)->mRes.unk1C = 0;                                            \
        (list)->mRes.mStartNodePtr = &(list)->mRes.mStartNode;             \
        (list)->mRes.mStartNodePtr->mNext = (list)->mRes.mStartNodePtr;    \
        (list)->mRes.mStartNodePtr->mPrev = (list)->mRes.mStartNodePtr;    \
        (list)->mRes.vtable = (void*)&lbl_eu_805700B4;                     \
        (list)->mRes.mList =                                               \
            (SLNode*)mtl::MemManager::allocate_array(0x1800, slAllocHandle()); \
        for (slI = 0x200; slI != 0; slI--) {                               \
            (list)->mRes.mList[slI - 1].mNext = 0;                         \
        }                                                                  \
        (list)->mRes.mCapacity = 0x200;                                    \
        slNode = (list)->mRes.mStartNodePtr->mNext;                        \
        while (slNode != (list)->mRes.mStartNodePtr) {                     \
            slCur = slNode;                                                \
            slNode = slNode->mNext;                                        \
            slCur->mNext = 0;                                              \
        }                                                                  \
        (list)->mRes.mStartNodePtr->mNext = (list)->mRes.mStartNodePtr;    \
        (list)->mRes.mStartNodePtr->mPrev = (list)->mRes.mStartNodePtr;    \
    } while (0)

// ---------------------------------------------------------------------------
// func_804E45F4: step every schedule (func_804E36DC with a delta), then
// remove-and-destroy any schedule whose flags select the removal path
// (bit 10 set, bit 15 clear).
// ---------------------------------------------------------------------------
void func_804E45F4(SLList* self, f32 dt) {
    SLNode* node;
    u32 count = 0;
    CSchedule* item;
    SLNode* sentinel;
    u16 flags;
    SLNode* rnode;
    int removed;

    sentinel = self->mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(volatile SLNode**)&self->mRes.mStartNodePtr)->mNext;
        while (node != self->mRes.mStartNodePtr && node != 0) {
            func_804E36DC((CSchedule*)node->mItem, dt);
            node = node->mNext;
        }
        do {
            removed = 0;
            sentinel = self->mRes.mStartNodePtr;
            rnode = sentinel->mNext;
            while (rnode != sentinel && rnode != 0) {
                item = (CSchedule*)rnode->mItem;
                if ((((SchedFlagBits*)&item->field_0x00)->destroy) &&
                    !(((SchedFlagBits*)&item->field_0x00)->clearOnMatch)) {
                    if (item != 0) {
                        if (item != 0) {
                            scheduleDestroy(item);
                        }
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

// func_804E4718: func_804E3B08 on each schedule, guarded by a non-empty list.
void func_804E4718(SLList* self) {
    SLNode* node;
    u32 count = 0;
    SLNode* sentinel;
    sentinel = self->mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(volatile SLNode**)&self->mRes.mStartNodePtr)->mNext;
        while (node != self->mRes.mStartNodePtr && node != 0) {
            func_804E3B08((CSchedule*)node->mItem);
            node = node->mNext;
        }
    }
}

// func_804E479C: func_804E3B6C on each schedule whose field_0x10 matches x.
void func_804E479C(SLList* self, u8* x) {
    SLNode* node;
    u32 count = 0;
    SLNode* sentinel;
    sentinel = self->mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        node = (*(volatile SLNode**)&self->mRes.mStartNodePtr)->mNext;
        while (node != self->mRes.mStartNodePtr) {
            CSchedule* item = (CSchedule*)node->mItem;
            if (x == item->field_0x10) {
                func_804E3B6C(item);
            }
            node = node->mNext;
        }
    }
}

// ---------------------------------------------------------------------------
// func_804E4830: allocate a CSchedule, initialize it with the six forwarded
// arguments, and push it onto the ring's first free slot.
// ---------------------------------------------------------------------------
CSchedule* func_804E4830(SLList* self, ScheduleEntry* entries,
                         u8* p8, u8* pc, u32 fa, u32 fb, u8* p10) {
    SLNode* sentinel;
    SLNode* temp;
    u32 capacity;
    u32 i;
    CSchedule* p;

    if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0xf0) {
        p = 0;
    } else {
        p = (CSchedule*)mtl::MemManager::allocate(0xf0, slAllocHandle());
        if (p != 0) {
            __ct__CSchedule(p);
        }
    }
    if (p == 0) {
        return 0;
    }
    if (func_804E3434(p, entries, p8, pc, fa, fb, p10) == 0) {
        if (p != 0) {
            if (p != 0) {
                scheduleDestroy(p);
            }
        }
        return 0;
    }
    sentinel = self->mRes.mStartNodePtr;
    i = 0;
    capacity = self->mRes.mCapacity;
    while (i < capacity) {
        if (self->mRes.mList[i].mNext == 0) {
            break;
        }
        i++;
    }
    temp = &self->mRes.mList[i];
    // reslist setItem: the inlined member call carries null-guards (and, at
    // retail frame sizes, an exception sp-save).
    if (temp != 0) {
        if (temp != 0) {
            temp->mItem = p;
        }
    }
    temp->mNext = sentinel;
    temp->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = temp;
    sentinel->mPrev = temp;
    return p;
}

// func_804E498C: find node holding x, destroy x, unlink the node.
void func_804E498C(SLList* self, CSchedule* x) {
    SLNode* node;
    SLNode* sentinel;
    if (x == 0) return;
    sentinel = self->mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel && node->mItem != x) {
        node = node->mNext;
    }
    if (node == sentinel) return;
    if (x != 0) {
        if (x != 0) {
            scheduleDestroy(x);
        }
    }
    {
        SLNode* prev = node->mPrev;
        SLNode* next = node->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        node->mNext = 0;
    }
}

// func_804E4A20: remove first node whose item's field_0x14 matches x;
// field_0x18 match clears instead.
void func_804E4A20(SLList* self, u32 x) {
    SLNode* node;
    SLNode* cur;
    CSchedule* item;
    if (x == 0) return;
    node = self->mRes.mStartNodePtr->mNext;
    while (node != self->mRes.mStartNodePtr) {
        cur = node;
        node = node->mNext;
        item = (CSchedule*)cur->mItem;
        if (item->field_0x14 == x) {
            if (item != 0) {
                if (item != 0) {
                    scheduleDestroy(item);
                }
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

// func_804E4AD4: clear any schedule's field_0x14/field_0x18 that matches x.
void func_804E4AD4(SLList* self, u32 x) {
    u32 zero = 0;
    SLNode* cur;
    SLNode* node;
    SLNode* sentinel;
    CSchedule* item;
    sentinel = self->mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != self->mRes.mStartNodePtr) {
        cur = node;
        node = node->mNext;
        item = (CSchedule*)cur->mItem;
        if (item->field_0x14 == x) {
            item->field_0x14 = zero;
        } else if (item->field_0x18 == x) {
            item->field_0x18 = zero;
        }
    }
}

// func_804E4B24: count the schedules in the ring (excludes the sentinel).
u32 func_804E4B24(SLList* self) {
    SLNode* node;
    SLNode* sentinel;
    u32 count = 0;
    sentinel = self->mRes.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        node = node->mNext;
        count++;
    }
    return count;
}

// func_804E4B48: walk the sentinel ring back to its head.
void* func_804E4B48(u8* self) {
    u8* head = *(u8**)((u8*)self + 8);
    u8* cur = *(u8**)head;
    while (cur != head) {
        cur = *(u8**)cur;
    }
    return head;
}

// ---------------------------------------------------------------------------
// __ct__804E4B64: allocate a ScheduleList (0x24), construct it, and store it
// into the global list selected by arg (0 -> lbl_eu_80665A50, else 54).
// ---------------------------------------------------------------------------
void __ct__804E4B64(int arg) {
    SLList* list;
    if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
        list = 0;
    } else {
        list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
        if (list != 0) {
            SCHEDULELIST_CTOR_BODY(list);
        }
    }
    if (arg == 0) {
        lbl_eu_80665A50 = list;
    }
    if (arg != 0) {
        lbl_eu_80665A54 = list;
    }
}

// __dt___reslist_base_CSchedule: clear ring, free slot array, optionally free
// self. CW destructors return this; the entry null-guard and the vptr store
// back to the base-class table are part of retail codegen.
// Shared ~reslist_base body. Kept static with its address never taken so
// MWCC auto-inlines it into both exported dtors - the derived-dtor inlining
// context is what produces retail's register coloring there.
SLResBase* __dt___reslist_base_CSchedule(SLResBase* base, int flag) {
    SLNode* node;
    SLNode* cur;
    if (base != 0) {
        *(void**)&base->vtable = (void*)&lbl_eu_805700CC;
        node = base->mStartNodePtr->mNext;
        while (node != base->mStartNodePtr) {
            cur = node;
            node = node->mNext;
            cur->mNext = 0;
        }
        base->mStartNodePtr->mNext = base->mStartNodePtr;
        base->mStartNodePtr->mPrev = base->mStartNodePtr;
        if (base->unk1C == 0) {
            if (base->mList != 0) {
                __dla__FPv(base->mList);
                base->mList = 0;
            }
        }
        if (flag > 0) {
            __dl__FPv(base);
        }
    }
    return base;
}

// __dt__reslist_CSchedule: ~reslist<CSchedule*> - doubled entry guard around
// the (inlined base) dtor body storing the base-class vtable; the delete-self
// flag check sits outside the inner guard.
SLResBase* __dt__reslist_CSchedule(SLResBase* self, int flag) {
    SLNode* node;
    SLNode* cur;
    if (self != 0) {
        if (self != 0) {
            *(void**)&self->vtable = (void*)&lbl_eu_805700CC;
            node = self->mStartNodePtr->mNext;
            while (node != self->mStartNodePtr) {
                cur = node;
                node = node->mNext;
                cur->mNext = 0;
            }
            self->mStartNodePtr->mNext = self->mStartNodePtr;
            self->mStartNodePtr->mPrev = self->mStartNodePtr;
            if (self->unk1C == 0) {
                if (self->mList != 0) {
                    __dla__FPv(self->mList);
                    self->mList = 0;
                }
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// __dt__12ScheduleListFv: destroy every schedule in the ring (virtual dtor
// dispatch), unlink nodes, run the embedded reslist member dtor inline
// (doubled guard + derived-reslist vptr store), then optionally free self.
// ---------------------------------------------------------------------------
SLList* __dt__12ScheduleListFv(SLList* self, int flag) {
    SLResBase* res;
    SLNode* iNode;
    SLNode* iCur;
    CSchedule* item;
    SLNode* clNode;
    SLNode* clCur;
    SLNode* mNode;
    SLNode* mCur;

    if (self != 0) {
        *(void**)&self->mSelfVtbl = (void*)&lbl_eu_80570078;
        iNode = self->mRes.mStartNodePtr->mNext;
        while (iNode != self->mRes.mStartNodePtr) {
            item = (CSchedule*)iNode->mItem;
            if (item != 0) {
                if (item != 0) {
                    scheduleDestroy(item);
                }
                iNode->mItem = 0;
            }
            iNode = iNode->mNext;
        }
        clNode = self->mRes.mStartNodePtr->mNext;
        while (clNode != self->mRes.mStartNodePtr) {
            SLNode* clCur = clNode;
            clNode = clNode->mNext;
            clCur->mNext = 0;
        }
        self->mRes.mStartNodePtr->mNext = self->mRes.mStartNodePtr;
        self->mRes.mStartNodePtr->mPrev = self->mRes.mStartNodePtr;

        res = &self->mRes;
        if (res != 0) {
            if (res != 0) {
                *(void**)&res->vtable = (void*)&lbl_eu_805700CC;
                mNode = res->mStartNodePtr->mNext;
                while (mNode != res->mStartNodePtr) {
                    mCur = mNode;
                    mNode = mNode->mNext;
                    mCur->mNext = 0;
                }
                res->mStartNodePtr->mNext = res->mStartNodePtr;
                res->mStartNodePtr->mPrev = res->mStartNodePtr;
                if (res->unk1C == 0) {
                    if (res->mList != 0) {
                        __dla__FPv(res->mList);
                        res->mList = 0;
                    }
                }
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// func_804E4D58: terminate the global schedule lists.
void func_804E4D58(u32 x) {
    if (x != 0) {
        if (lbl_eu_80665A54 != 0) {
            if (lbl_eu_80665A54 != 0) {
                ((SLDispatch*)lbl_eu_80665A54)->destroy(1);
            }
            lbl_eu_80665A54 = 0;
        }
    }
    if (lbl_eu_80665A50 != 0) {
        if (lbl_eu_80665A50 != 0) {
            ((SLDispatch*)lbl_eu_80665A50)->destroy(1);
        }
        lbl_eu_80665A50 = 0;
    }
}

// func_804E4E38: advance both global schedule lists (vtable 0x10).
void func_804E4E38() {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        ((SLDispatch*)a)->updateAll();
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        ((SLDispatch*)b)->updateAll();
    }
}

// func_804E4DD4: forward delta to both global schedule lists (vtable 0xc).
void func_804E4DD4(f32 x) {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        ((SLDispatch*)a)->step(x);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        ((SLDispatch*)b)->step(x);
    }
}

// func_804E4E8C: hand an argument to both global schedule lists (vtable 0x14).
void func_804E4E8C(u32 x) {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        ((SLDispatch*)a)->find((void*)x);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        ((SLDispatch*)b)->find((void*)x);
    }
}

// func_804E4EF8: route an add-schedule request to one of the two lists.
void func_804E4EF8(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5) {
    SLList* list;
    u32 use54 = 0;
    if (((lbl_eu_8065FC18[0] >> 12) & 1) != 0 && p4 == 0) {
        use54 = 1;
    }
    list = use54 ? lbl_eu_80665A54 : lbl_eu_80665A50;
    ((SLDispatch*)list)->add((ScheduleEntry*)p0, (void*)p1,
                             (void*)p2, p3, p4, (void*)p5);
}

// ---------------------------------------------------------------------------
// __ct__804E4F9C: sinit that rebuilds both global schedule lists.
// ---------------------------------------------------------------------------
void __ct__804E4F9C() {
    SLList* list;
    SLList* old;
    if (lbl_eu_80665A50 != 0) {
        if (lbl_eu_80665A50 != 0) {
            ((SLDispatch*)lbl_eu_80665A50)->destroy(1);
        }
        lbl_eu_80665A50 = 0;
    }
    if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
        list = 0;
    } else {
        list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
        if (list != 0) {
            SCHEDULELIST_CTOR_BODY(list);
        }
    }
    lbl_eu_80665A50 = list;

    old = lbl_eu_80665A54;
    if (old != 0) {
        if (old != 0) {
            ((SLDispatch*)old)->destroy(1);
        }
        lbl_eu_80665A54 = 0;
        if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
            list = 0;
        } else {
            list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
            if (list != 0) {
                SCHEDULELIST_CTOR_BODY(list);
            }
        }
        lbl_eu_80665A54 = list;
    }
}

// func_804E536C: forward an argument to both global schedule lists (vtable 0x1c).
void func_804E536C(u32 x) {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        ((SLDispatch*)a)->removeItem((CSchedule*)x);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        ((SLDispatch*)b)->removeItem((CSchedule*)x);
    }
}

// func_804E53D8: sum the per-list counts reported by both global lists.
u32 func_804E53D8() {
    u32 total = 0;
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        total = ((SLDispatch*)a)->count();
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        total += ((SLDispatch*)b)->count();
    }
    return total;
}

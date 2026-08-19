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
extern "C" void __dt__12ScheduleListFv(SLList* self, int flag);
extern "C" void __dt__reslist_CSchedule(SLResBase* self, int flag);
extern "C" void __dt___reslist_base_CSchedule(SLResBase* self, int flag);
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

extern "C" __declspec(align(8)) u32 lbl_eu_80570078[12] = {
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
extern "C" u32 lbl_eu_805700B4[3] = {
    (u32)&lbl_eu_80663C18, 0, (u32)&__dt__reslist_CSchedule,
};
extern "C" u32 lbl_eu_805700C0[3] = {
    (u32)&lbl_eu_80663C20, 0, 0,
};
extern "C" u32 lbl_eu_805700CC[3] = {
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

// Global schedule-list mode flag (bit 12 selects the alternate list) packed
// with the mem-manager allocation handle at +2 / +4. Defined in another TU;
// incomplete-array extern keeps MWCC from choosing sda21 (retail uses lis+@l
// far addressing - see MWCC_CASES §1a).
extern u16 lbl_eu_8065FC18[];

static u32 slAllocHandle(void) {
    return *(u32*)&lbl_eu_8065FC18[2];
}

// Construct a fresh ScheduleList in place: base ring init + reserve a slot
// array of 0x200 nodes.
static void scheduleListCtor(SLList* list) {
    SLNode* node;
    u32 i;
    list->mRes.mList = 0;
    list->mRes.mCapacity = 0;
    list->mRes.unk1C = 0;
    list->mRes.mStartNodePtr = &list->mRes.mStartNode;
    list->mRes.mStartNodePtr->mNext = &list->mRes.mStartNode;
    list->mRes.mStartNodePtr->mPrev = list->mRes.mStartNode.mNext;

    list->mRes.mList = (SLNode*)mtl::MemManager::allocate_array(0x1800, slAllocHandle());
    for (i = 0; i < 0x200; i++) {
        list->mRes.mList[i].mNext = 0;
    }
    list->mRes.mCapacity = 0x200;
    node = list->mRes.mStartNodePtr->mNext;
    while (node != list->mRes.mStartNodePtr) {
        SLNode* cur = node;
        node = node->mNext;
        cur->mNext = 0;
    }
    list->mRes.mStartNodePtr->mNext = list->mRes.mStartNodePtr;
    list->mRes.mStartNodePtr->mPrev = list->mRes.mStartNodePtr;
}

// ---------------------------------------------------------------------------
// func_804E45F4: step every schedule (func_804E36DC with a delta), then
// remove-and-destroy any schedule whose flags select the removal path
// (bit 10 set, bit 15 clear).
// ---------------------------------------------------------------------------
void func_804E45F4(SLList* self, f32 dt) {
    SLNode* node;
    u32 count = 0;
    SLNode* sentinel;
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
                CSchedule* item = (CSchedule*)rnode->mItem;
                u16 flags = *(u16*)&item->field_0x00;
                if ((flags & 0x400) && !(flags & 0x8000)) {
                    if (item != 0) {
                        if (item != 0) {
                            __dt__9CScheduleFv(item, 1);
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
                __dt__9CScheduleFv(p, 1);
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
    temp->mItem = p;
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
            __dt__9CScheduleFv(x, 1);
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
                    __dt__9CScheduleFv(item, 1);
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
            scheduleListCtor(list);
        }
    }
    if (arg == 0) {
        lbl_eu_80665A50 = list;
    }
    if (arg != 0) {
        lbl_eu_80665A54 = list;
    }
}

// __dt___reslist_base_CSchedule: clear ring, free slot array, optionally free self.
void __dt___reslist_base_CSchedule(SLResBase* base, int flag) {
    SLNode* node = base->mStartNodePtr->mNext;
    while (node != base->mStartNodePtr) {
        SLNode* cur = node;
        node = node->mNext;
        cur->mNext = 0;
    }
    base->mStartNodePtr->mNext = base->mStartNodePtr;
    base->mStartNodePtr->mPrev = base->mStartNodePtr;
    if (base->unk1C == 0) {
        if (base->mList != 0) {
            mtl::MemManager::deallocate(base->mList);
            base->mList = 0;
        }
    }
    if (flag > 0) {
        mtl::MemManager::deallocate(base);
    }
}

// __dt__reslist_CSchedule: forward to the base dtor.
void __dt__reslist_CSchedule(SLResBase* self, int flag) {
    __dt___reslist_base_CSchedule(self, flag);
}

// ---------------------------------------------------------------------------
// __dt__12ScheduleListFv: destroy every schedule in the ring, unlink nodes,
// run the embedded reslist member dtor, then optionally free self.
// ---------------------------------------------------------------------------
void __dt__12ScheduleListFv(SLList* self, int flag) {
    SLNode* node;
    SLNode* cur;
    CSchedule* item;

    node = self->mRes.mStartNodePtr->mNext;
    while (node != self->mRes.mStartNodePtr) {
        item = (CSchedule*)node->mItem;
        if (item != 0) {
            if (item != 0) {
                __dt__9CScheduleFv(item, 1);
            }
            node->mItem = 0;
        }
        node = node->mNext;
    }
    node = self->mRes.mStartNodePtr->mNext;
    while (node != self->mRes.mStartNodePtr) {
        cur = node;
        node = node->mNext;
        cur->mNext = 0;
    }
    self->mRes.mStartNodePtr->mNext = self->mRes.mStartNodePtr;
    self->mRes.mStartNodePtr->mPrev = self->mRes.mStartNodePtr;

    __dt__reslist_CSchedule(&self->mRes, 0);
    if (flag > 0) {
        mtl::MemManager::deallocate(self);
    }
}

// func_804E4D58: terminate the global schedule lists.
void func_804E4D58(u32 x) {
    if (x != 0) {
        if (lbl_eu_80665A54 != 0) {
            if (lbl_eu_80665A54 != 0) {
                __dt__12ScheduleListFv(lbl_eu_80665A54, 1);
            }
            lbl_eu_80665A54 = 0;
        }
    }
    if (lbl_eu_80665A50 != 0) {
        if (lbl_eu_80665A50 != 0) {
            __dt__12ScheduleListFv(lbl_eu_80665A50, 1);
        }
        lbl_eu_80665A50 = 0;
    }
}

// func_804E4E38: advance both global schedule lists (vtable 0x10).
void func_804E4E38() {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        func_804E4718(a);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        func_804E4718(b);
    }
}

// func_804E4DD4: forward delta to both global schedule lists (vtable 0xc).
void func_804E4DD4(f32 x) {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        func_804E45F4(a, x);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        func_804E45F4(b, x);
    }
}

// func_804E4E8C: hand an argument to both global schedule lists (vtable 0x14).
void func_804E4E8C(u32 x) {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        func_804E479C(a, (u8*)x);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        func_804E479C(b, (u8*)x);
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
    func_804E4830(list, (ScheduleEntry*)p0, (u8*)p1, (u8*)p2, p3, p4, (u8*)p5);
}

// ---------------------------------------------------------------------------
// __ct__804E4F9C: sinit that rebuilds both global schedule lists.
// ---------------------------------------------------------------------------
void __ct__804E4F9C() {
    SLList* list;
    if (lbl_eu_80665A50 != 0) {
        if (lbl_eu_80665A50 != 0) {
            __dt__12ScheduleListFv(lbl_eu_80665A50, 1);
        }
        lbl_eu_80665A50 = 0;
    }
    if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
        list = 0;
    } else {
        list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
        if (list != 0) {
            scheduleListCtor(list);
        }
    }
    lbl_eu_80665A50 = list;

    if (lbl_eu_80665A54 != 0) {
        if (lbl_eu_80665A54 != 0) {
            if (lbl_eu_80665A54 != 0) {
                __dt__12ScheduleListFv(lbl_eu_80665A54, 1);
            }
            lbl_eu_80665A54 = 0;
        }
        if (mtl::MemManager::getMaxAllocSize(slAllocHandle()) < 0x24) {
            list = 0;
        } else {
            list = (SLList*)mtl::MemManager::allocate(0x24, slAllocHandle());
            if (list != 0) {
                scheduleListCtor(list);
            }
        }
        lbl_eu_80665A54 = list;
    }
}

// func_804E536C: forward an argument to both global schedule lists (vtable 0x1c).
void func_804E536C(u32 x) {
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        func_804E4A20(a, x);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        func_804E4A20(b, x);
    }
}

// func_804E53D8: sum the per-list counts reported by both global lists.
u32 func_804E53D8() {
    u32 total = 0;
    SLList* a = lbl_eu_80665A50;
    if (a != 0) {
        total = func_804E4B24(a);
    }
    SLList* b = lbl_eu_80665A54;
    if (b != 0) {
        total += func_804E4B24(b);
    }
    return total;
}

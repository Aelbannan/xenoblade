// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemPool
#include <harness_catalog.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/data_vtables.hpp"

// Local mirror of the monolib reslist template (ScheduleList.cpp precedent):
// the shared reslist.hpp instantiates remove()/find() which need operators the
// opaque item type cannot provide, while the ctor/dtor/vtable codegen is
// identical. Layout matches _reslist_base<T>: vtable@0, mStartNodePtr@4,
// sentinel@8, mList@0x14, mCapacity@0x18, unk1C@0x1c.
template <typename T>
struct _reslist_node {
    _reslist_node<T>* mNext;  // 0x0
    _reslist_node<T>* mPrev;  // 0x4
    T mItem;                  // 0x8
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
        if (unk1C == false && mList != nullptr) {
            delete[] mList;
            mList = nullptr;
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
    reslist();
    virtual ~reslist() {}
};

// Out-of-class (non-inline) so the explicit instantiation below emits the
// standalone ctor body instead of folding it into call sites.
template <typename T>
reslist<T>::reslist() : _reslist_base<T>() {}

// Item object stored in the pool's slot arrays and referenced by the reslist
// sub-pools. The retail class carries a large vtable (>= 0xD0 bytes); only
// the slot at offset 0xD0, used by CScnItemPool::func_8048CEDC, is modelled
// here. MWCC prepends a 2-entry header to the vtable, so the 51st declared
// virtual lands at offset 0xD0. Slot size is 0x58 bytes.
class CScnItem {
public:
    virtual void vfunc08(int arg);
    virtual void vfunc0C();
    virtual void vfunc10(int arg);
    virtual void vfunc14();
    virtual void vfunc18();
    virtual void vfunc1C();
    virtual void vfunc20();
    virtual void vfunc24();
    virtual void vfunc28();
    virtual void vfunc2C();
    virtual void vfunc30();
    virtual void vfunc34();
    virtual void vfunc38();
    virtual void vfunc3C();
    virtual void vfunc40();
    virtual void vfunc44();
    virtual void vfunc48();
    virtual void vfunc4C();
    virtual void vfunc50();
    virtual void vfunc54();
    virtual void vfunc58();
    virtual void vfunc5C();
    virtual void vfunc60();
    virtual void vfunc64();
    virtual void vfunc68();
    virtual void vfunc6C();
    virtual void vfunc70();
    virtual void vfunc74();
    virtual void vfunc78();
    virtual void vfunc7C();
    virtual void vfunc80();
    virtual void vfunc84();
    virtual void vfunc88();
    virtual void vfunc8C();
    virtual void vfunc90();
    virtual void vfunc94();
    virtual void vfunc98();
    virtual void vfunc9C();
    virtual void vfuncA0();
    virtual void vfuncA4();
    virtual u32 vfuncA8();
    virtual void vfuncAC();
    virtual void vfuncB0();
    virtual void vfuncB4();
    virtual void vfuncB8();
    virtual void vfuncBC();
    virtual void vfuncC0();
    virtual void vfuncC4();
    virtual void vfuncC8();
    virtual void vfuncCC();
    virtual void vfuncD0();  // vtable offset 0xD0

    u8 _04[0x54];            // 0x04..0x58 opaque payload
};

// 0x3A8-byte slot type for the pool's big-item array (func_8048C480).
struct CScnItemBig {
    u8 _00[0x3A8];
};

// Emit the retail ctor/dtor/vtable instantiations for reslist<CScnItem*>.
// The base ctor is inlined; MWCC emits the base-vtable store at entry and
// the derived-vtable store at the end, reproducing the retail double-store
// schedule.
template class reslist<CScnItem*>;

// Doubly-linked list node used by the item-pool free-list helpers.
struct CScnItemPoolLink {
    CScnItemPoolLink* field_0x00;  // next
    CScnItemPoolLink* field_0x04;  // prev
};

// Singly-linked list node used by the item-pool search helper.
struct CScnItemPoolNode {
    CScnItemPoolNode* field_0x00;  // next
    u8 _04[4];
    u32 field_0x08;  // key
};

struct CScnItemPoolState {
    u32 value00;
    CScnItemPoolLink* field_0x04;  // reslist mStartNodePtr
    u16 value08;
    u8 _0A[0x0E];
    u32 value18;
    u8 _1C[0x10];
    u8 field_0x2c[0x18];  // 0x2C sub-pool header (capacity lands at 0x44)
    u32 value44;          // 0x44 sub-pool capacity (0x2C + 0x18)
    u8 _48[0x24];         // 0x48..0x6C
    u8 field_0x6c[0x20];  // 0x6C sub-pool #3 header (capacity at 0x84)
    u32 value8C;
    u8 _90[0x3C];
    u8* field_0xcc;       // 0xCC free-flag byte array
    CScnItem* field_0xd0; // 0xD0 slot array (0x58-byte items)
    u8* field_0xd4;       // 0xD4 big-item free-flag byte array
    CScnItemBig* field_0xd8;  // 0xD8 big-item slot array (0x3A8-byte items)
};

// reslist storage layout torn down by the non-virtual dtor __dt__8048C378.
// Layout matches _reslist_base<T> (vtable@0, mStartNodePtr@4, sentinel@8).
struct CScnItemPoolListData {
    u32 field_0x00;                // 0x00 vtable slot
    CScnItemPoolLink* field_0x04;  // 0x04 mStartNodePtr (sentinel)
    CScnItemPoolLink field_0x08;   // 0x08 sentinel node
    u32 field_0x10;                // 0x10
    CScnItemPoolLink* field_0x14;  // 0x14 mList (node array)
    int field_0x18;                // 0x18 mCapacity
    bool field_0x1c;               // 0x1C external-ownership flag
};

// CScnItemPool - scene item pool. Layout (from the retail ctor):
//   0x00: IWorkEvent base, 0x04: CDeviceVICb base, 0x08: id
//   0x0C..0xCC: six reslist sub-pools (0x20 bytes each)
//   0xCC/0xD0: small-slot pool (flags / 0x58-byte slots)
//   0xD4/0xD8: big-slot pool (flags / 0x3A8-byte slots)
class CScnItemPool {
public:
    u8 _00[0x0C];              // 0x00 bases + id
    u8 mList0C[0x20];          // 0x0C reslist sub-pool #0
    u8 mList2C[0x20];          // 0x2C
    u8 mList4C[0x20];          // 0x4C
    u8 mList6C[0x20];          // 0x6C
    u8 mList8C[0x20];          // 0x8C
    u8 mListAC[0x20];          // 0xAC
    u8* mFlagsCC;              // 0xCC
    CScnItem* mSlotsD0;        // 0xD0
    u8* mFlagsD4;              // 0xD4
    CScnItemBig* mSlotsD8;     // 0xD8

    void func_8048CEDC();
    void func_8048CF58();
    void update();
    void func_8048D014();   // this-4 thunk -> func_8048CF58 (vtable+0x98)
    void func_8048D01C();   // this-4 thunk -> ~CScnItemPool   (vtable+0x90)
};

// Cross-unit pool-item helpers (retail C-ABI, unmangled symbols).
extern "C" void func_80485774(CScnItem* item, u32 arg);
extern "C" void func_804859E8(CScnItem* item, u32 arg);
extern "C" void func_804838DC(CScnItem* item, u32 arg);

// reslist<CScnItem> constructor (retail __ct__reslist_CScnItem). The base
// ctor is inlined; MWCC emits the base-vtable store at entry and the
// derived-vtable store at the end, reproducing the retail double-store
// schedule.
//
// NOTE: __ct__reslist_CScnItem is emitted by `template class reslist<CScnItem>`
// above.

extern "C" __declspec(noinline) u32 func_8048C478(u8* self) { return ((CScnItemPoolState*)self)->value18; }
// volatile load: keeps the body byte-identical (lhz) while making it impure to
// -ipa, so func_8048C750's three kind checks stay three real calls (retail
// never merges them across the TU boundary).
extern "C" __declspec(noinline) u16 func_8048C690(u8* self) { return *(volatile u16*)&((CScnItemPoolState*)self)->value08; }
// noinline: retail CALLS this from func_8048C750's kind-1 branch; without it
// MWCC auto-inlines the field load into the caller.
extern "C" __declspec(noinline) u32 func_8048C8BC(u8* self) { return ((CScnItemPoolState*)self)->value8C; }
extern "C" __declspec(noinline) u32 func_8048C9F4(u8* self) { return ((CScnItemPoolState*)self)->value00 + 8; }
// func_8048C524: push an item pointer onto the reslist ring. Finds the first
// free slot (a node whose mNext is null), stores the item through the guarded
// setItem pattern, and links the node onto the ring. The setItem try/catch
// forces the retail frame marker (mr r31,r1 / stw r1) - same shape as
// ScheduleList.cpp func_804E4830.
extern "C" __declspec(noinline) void func_8048C524(u32 a, u32* b) {
    _reslist_base<CScnItem*>* self = (_reslist_base<CScnItem*>*)a;
    int i;
    int byteOff;
    _reslist_node<CScnItem*>* temp;
    int capacity;
    _reslist_node<CScnItem*>* sentinel;
    sentinel = self->mStartNodePtr;
    capacity = self->mCapacity;
    for (i = 0, byteOff = 0; i < capacity; i++) {
        if (*(void**)((u8*)self->mList + byteOff) == 0) {
            break;
        }
        byteOff += 12;
    }
    temp = &self->mList[i];
    {
        CScnItem** itemPtr = &temp->mItem;
        if (itemPtr != 0) {
            try {
                *itemPtr = (CScnItem*)*b;
            } catch (...) {
                throw;
            }
        }
    }
    temp->mNext = sentinel;
    temp->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = temp;
    sentinel->mPrev = temp;
}
extern "C" __declspec(noinline) void func_8048C5AC(int* dst, const void* src) { *(u32*)dst = *(const u32*)((const u8*)src + 4); }
extern "C" __declspec(noinline) void* func_8048C698(u8* self, int kind) {
    switch (kind) {
    case 1:
        return (void*)((char*)self + 0x0C);
    case 2:
        return (void*)((char*)self + 0x2C);
    case 3:
        return (void*)((char*)self + 0x4C);
    case 4:
        return (void*)((char*)self + 0x6C);
    case 5:
        return (void*)((char*)self + 0x8C);
    default:
        return nullptr;
    }
}
extern "C" __declspec(noinline) void* func_8048C6F4(u8* self, s32 kind) {
    switch (kind) {
    case 1: return (u8*)self + 0xC;
    case 2: return (u8*)self + 0x2C;
    case 3: return (u8*)self + 0x4C;
    case 4: return (u8*)self + 0x6C;
    case 5: return (u8*)self + 0x8C;
    default: return 0;
    }
}
// Scene-side receiver for func_8048C750's kind-1 branch: opaque object whose
// second declared virtual (vtable+0x0C under -RTTI on) takes the item.
struct CScnPoolSceneIf {
    virtual void pad08();
    virtual void apply(CScnItem* item);
};

// func_8048C750: releases `item` from the pool per its kind.
//  kind 1: scene = pool->id resolved via func_8048C8BC; scene->apply(item);
//          item->vfuncCC(); func_804838DC(item, 0); recycle onto sub-pool 0xAC
//          via func_8048C524.
//  kind 2: item->vfunc08(-1); clear small-slot flag at (item-mSlotsD0)/0x58.
//  kind 4: item->vfunc08(-1); clear big-slot flag at (item-mSlotsD8)/0x3A8
//          (signed division — retail emits the add-back correction form).
//  other:  double-checked-null item->vfunc08(1).
extern "C" __declspec(noinline) void func_8048C750(CScnItemPool* self, CScnItem* item) {
    if (func_8048C690((u8*)item) == 1) {
        CScnPoolSceneIf* scene = (CScnPoolSceneIf*)func_8048C8BC(*(u8**)((char*)self + 8));
        scene->apply(item);
        item->vfuncCC();
        func_804838DC(item, 0);
        func_8048C524((u32)((char*)self + 0xAC), (u32*)&item);
    } else if (func_8048C690((u8*)item) == 2) {
        item->vfunc08(-1);
        self->mFlagsCC[((u32)item - (u32)self->mSlotsD0) / 0x58] = 0;
    } else if (func_8048C690((u8*)item) == 4) {
        item->vfunc08(-1);
        self->mFlagsD4[(CScnItemBig*)item - self->mSlotsD8] = 0;
    } else {
        if (item != nullptr) {
            if (item != nullptr) {
                item->vfunc08(1);
            }
        }
    }
}
// us-80490a08: walks the singly-linked list, advancing *list until a node
// equals **target or its field_0x08 matches *key; returns it via *out.
extern "C" __declspec(noinline) void func_8048C994(CScnItemPoolNode** out, CScnItemPoolNode** list,
                   CScnItemPoolNode** target, u32* key) {
    CScnItemPoolNode* node;
    while ((node = *list) != *target && node->field_0x08 != *key) {
        *list = node->field_0x00;
    }
    *out = node;
}
extern "C" __declspec(noinline) void func_8048C9C8(int* self) { *(u32*)self = *(u32*)(*(u32**)self); }
extern "C" __declspec(noinline) u32 func_8048C9D8(u32* a, u32* b) { return *a != *b; }
extern "C" __declspec(noinline) void func_8048CA00(u32* dst, const void* src) { *(u32*)dst = *(u32*)(*(u32**)((const u8*)src + 4)); }
extern "C" __declspec(noinline) u32 func_8048CA10(u32* a, u32* b) { return *a == *b; }
extern "C" __declspec(noinline) int* func_8048CA28(int* dst, int* src) { *dst = *src; return dst; }
// us-80490aa8: unlinks the node at *list from a doubly-linked list, clears its
// field_0x00, and returns the original field_0x00 via *out.
extern "C" __declspec(noinline) void func_8048CA34(CScnItemPoolLink** out, u32 unused, CScnItemPoolLink** list) {
    CScnItemPoolLink* node = *list;
    CScnItemPoolLink* b = node->field_0x00;
    CScnItemPoolLink* a = node->field_0x04;
    a->field_0x00 = b;
    b->field_0x04 = a;
    node = *list;
    node->field_0x00 = 0;
    *out = b;
}
// func_8048C8C4: unlinks the node holding `item` from the sub-pool selected by
// the item's id (offset 8), then hands (self, item) to func_8048C750.
// Returns 1 when the item was found and unlinked, 0 otherwise.
// noinline: retail CALLS this from func_8048CB14; without it MWCC auto-inlines
// the whole sub-pool walk (C690/C698/CA34/C750 cascade) into the caller.
extern "C" __declspec(noinline) int func_8048C8C4(CScnItemPool* self, CScnItem* item) {
    u16 kind = func_8048C690((u8*)item);
    _reslist_base<CScnItem*>* list = (_reslist_base<CScnItem*>*)func_8048C698((u8*)self, kind);
    _reslist_node<CScnItem*>* found;     // sp+0x20
    _reslist_node<CScnItem*>* node;      // sp+0x1c
    _reslist_node<CScnItem*>* sentinel;  // sp+0x18
    _reslist_node<CScnItem*>* sentinel2; // sp+0x14
    _reslist_node<CScnItem*>* out;       // sp+0x10
    _reslist_node<CScnItem*>* tmp;       // sp+0x0c
    func_8048C5AC((int*)&sentinel, list);
    func_8048CA00((u32*)&node, list);
    func_8048C994((CScnItemPoolNode**)&found, (CScnItemPoolNode**)&node,
                  (CScnItemPoolNode**)&sentinel, (u32*)&item);
    func_8048C5AC((int*)&sentinel2, list);
    if (func_8048CA10((u32*)&found, (u32*)&sentinel2) != 0) {
        return 0;
    }
    func_8048CA34((CScnItemPoolLink**)&out, (u32)list,
                  (CScnItemPoolLink**)func_8048CA28((int*)&tmp, (int*)&found));
    func_8048C750(self, item);
    return 1;
}
// func_8048CA5C: drains the sub-pool selected by `kind` (0 = none), unlinking
// the head node and handing each item to func_8048C750.
extern "C" __declspec(noinline) void func_8048CA5C(u8* self, int kind) {
    if (kind != 0) {
        _reslist_base<CScnItem*>* list = (_reslist_base<CScnItem*>*)func_8048C698(self, kind);
        _reslist_node<CScnItem*>* itemNode;  // sp+0x18
        _reslist_node<CScnItem*>* out;       // sp+0x14
        _reslist_node<CScnItem*>* unlink;    // sp+0x10
        _reslist_node<CScnItem*>* node;      // sp+0x0c
        _reslist_node<CScnItem*>* sentinel;  // sp+0x08
        while ((func_8048C5AC((int*)&sentinel, list),
                func_8048CA00((u32*)&node, list),
                func_8048C9D8((u32*)&node, (u32*)&sentinel)) != 0) {
            func_8048CA00((u32*)&itemNode, list);
            CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&itemNode);
            func_8048CA00((u32*)&unlink, list);
            func_8048CA34((CScnItemPoolLink**)&out, (u32)list, (CScnItemPoolLink**)&unlink);
            func_8048C750((CScnItemPool*)self, item);
        }
    }
}
// func_8048CB14: walks the reslist at self+0xC calling vfuncA8 on each item;
// when the returned key matches `key`, hands (self, item) to func_8048C8C4
// and stops; otherwise advances to the next node.
void func_8048CB14(CScnItemPool* self, u32 key) {
    _reslist_node<CScnItem*>* node;
    _reslist_node<CScnItem*>* sentinel;
    func_8048CA00((u32*)&node, &self->mList0C);
    while ((func_8048C5AC((int*)&sentinel, &self->mList0C),
            func_8048C9D8((u32*)&node, (u32*)&sentinel)) != 0) {
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&node);
        if (key == item->vfuncA8()) {
            func_8048C8C4(self, *(CScnItem**)func_8048C9F4((u8*)&node));
            break;
        }
        func_8048C9C8((int*)&node);
    }
}
// func_8048CBC0: iterates the reslist at 0xC and calls func_80485774 on each
// item, forwarding `arg`.
extern "C" void func_8048CBC0(CScnItemPool* self, u32 arg) {
    CScnItemPoolNode* iter;
    CScnItemPoolNode* sentinel;
    func_8048CA00((u32*)&iter, &self->mList0C);
    while ((func_8048C5AC((int*)&sentinel, &self->mList0C),
            func_8048C9D8((u32*)&iter, (u32*)&sentinel)) != 0) {
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&iter);
        func_80485774(item, arg);
        func_8048C9C8((int*)&iter);
    }
}
// func_8048CC40: iterates the reslist at 0xC and calls func_804859E8 on each
// item, forwarding `arg`.
extern "C" void func_8048CC40(CScnItemPool* self, u32 arg) {
    CScnItemPoolNode* iter;
    CScnItemPoolNode* sentinel;
    func_8048CA00((u32*)&iter, &self->mList0C);
    while ((func_8048C5AC((int*)&sentinel, &self->mList0C),
            func_8048C9D8((u32*)&iter, (u32*)&sentinel)) != 0) {
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&iter);
        func_804859E8(item, arg);
        func_8048C9C8((int*)&iter);
    }
}
// func_8048CCC0: initializes the six sub-pools (kind 0..5).
void func_8048CCC0(u8* self) {
    for (int i = 0; i < 6; i++) {
        func_8048CA5C(self, (u16)i);
    }
}
// func_8048CDA8: iterator default-construct helper (empty in retail).
// noinline: retail CALLS this empty function; without it MWCC folds it away.
extern "C" __declspec(noinline) void func_8048CDA8(_reslist_node<CScnItem*>** iter) {}
// func_8048CDAC: copy a node pointer through a stack slot.
// noinline: retail CALLS this from func_8048CD0C (twice); without it MWCC
// auto-inlines the load/store body into the caller.
extern "C" __declspec(noinline) void func_8048CDAC(int* dst, int* src) { *dst = *src; }

// func_8048CD0C: walks the reslist at self+0xC and calls vfunc14 on each
// item. The walk advances through a copied iterator so the item is read from
// the pre-advance node; the sentinel refresh stays in the loop condition
// (comma-operator form) to match the retail test-at-bottom shape.
void func_8048CD0C(CScnItemPool* self) {
    _reslist_node<CScnItem*>* iter;
    _reslist_node<CScnItem*>* node;
    _reslist_node<CScnItem*>* sentinel;
    func_8048CDA8(&iter);
    func_8048CA00((u32*)&node, &self->mList0C);
    while ((func_8048C5AC((int*)&sentinel, &self->mList0C),
            func_8048C9D8((u32*)&node, (u32*)&sentinel)) != 0) {
        func_8048CDAC((int*)&iter, (int*)&node);
        func_8048C9C8((int*)&iter);
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&node);
        item->vfunc14();
        func_8048CDAC((int*)&node, (int*)&iter);
    }
}

// Node-array view of a reslist sub-pool (allocated node array + capacity),
// used by func_8048C0EC.
struct CScnItemPoolNodeArray {
    u8 _00[0x14];
    _reslist_node<CScnItem*>* mNodes;  // 0x14
    int mCapacity;                     // 0x18
};

// func_8048C0EC: (re)initializes sub-pool #0's node array: allocates `count`
// nodes (0xC bytes each) from MemManager, clears every node's mNext (the free
// marker used by func_8048C524), and records the capacity.
extern "C" void func_8048C0EC(CScnItemPoolNodeArray* self, u32 handle, int count) {
    self->mNodes = (_reslist_node<CScnItem*>*)mtl::MemManager::allocate_array(count * 12, handle);
    _reslist_node<CScnItem*>* node;
    int i;
    for (i = 0; i < count; i++) {
        node = &self->mNodes[i];
        node->mNext = 0;
    }
    self->mCapacity = count;
}
// us-8049056c: forwards (arg1, &slot) to func_8048C524 with arg2 stored in a
// stack slot; the slot result is discarded. The first argument is unused
// (retail copies r4 into r3 for the call).
// noinline: retail CALLS this forwarder from func_8048C630; without it MWCC
// auto-inlines the body (and its func_8048C524 tail call) into the caller.
extern "C" __declspec(noinline) void func_8048C4F8(u32 unused, u32 arg1, u32 arg2) {
    u32 slot = arg2;
    func_8048C524(arg1, &slot);
}
// func_8048C60C: node count of the reslist at self (sentinel walk).
// Declare cur BEFORE end so end's vreg is born after cur's — the
// loop-invariant end takes the higher scratch r5 (retail lwz r5,4(r3)).
// noinline: retail CALLS this helper from func_8048C5B8; without it MWCC
// auto-inlines the same-TU body into the caller (19-structural residual).
extern "C" __declspec(noinline) u32 func_8048C60C(u8* self) {
    CScnItemPoolState* pool = (CScnItemPoolState*)self;
    CScnItemPoolLink* cur;
    CScnItemPoolLink* end = pool->field_0x04;
    cur = end->field_0x00;
    u32 count = 0;
    while (cur != end) {
        cur = cur->field_0x00;
        count++;
    }
    return count;
}
// func_8048C5B8: returns 1 when the sub-pool selected by `kind` still has
// free slots (mCapacity > current size). The comparison result is returned
// directly, so MWCC emits the branchless xor/cntlzw/slw/srwi test.
u32 func_8048C5B8(u8* self, s32 kind) {
    u8* list = (u8*)func_8048C6F4(self, kind);
    u32 size = func_8048C60C(list);
    u32 cap = func_8048C478(list);
    return cap > size;
}
// func_8048C630: registers `other`'s list into the sub-pool selected by
// other->value08, then reports success.
u32 func_8048C630(u8* self, u8* other) {
    u16 kind = func_8048C690(other);
    void* slot = func_8048C698(self, kind);
    func_8048C4F8((u32)self, (u32)slot, (u32)other);
    return 1;
}
// func_8048C480: allocates a free slot from the big-item pool (flags at 0xD4,
// 0x3A8-byte slots at 0xD8), marking its flag. Returns the slot pointer, or
// null when the pool is full.
extern "C" CScnItemBig* func_8048C480(u8* self) {
    CScnItemPoolState* pool = (CScnItemPoolState*)self;
    u32 i = 0;
    while (i < func_8048C478(pool->field_0x6c)) {
        if (pool->field_0xd4[i] == 0) {
            pool->field_0xd4[i] = 1;
            return &pool->field_0xd8[i];
        }
        i++;
    }
    return 0;
}
// func_8048C400: allocates a free slot from the pool, marks its flag and
// returns the slot pointer, or null when the pool is full.
CScnItem* func_8048C400(u8* self) {
    CScnItemPoolState* pool = (CScnItemPoolState*)self;
    u32 i = 0;
    while (i < func_8048C478(pool->field_0x2c)) {
        if (pool->field_0xcc[i] == 0) {
            pool->field_0xcc[i] = 1;
            return &pool->field_0xd0[i];
        }
        i++;
    }
    return 0;
}
// CScnItemPool::func_8048CEDC - iterates the reslist at 0xC and calls the
// item's vtable slot at offset 0xD0 on each item. The comma-condition keeps
// the sentinel refresh (func_8048C5AC) before the loop test (func_8048C9D8)
// at the bottom of the loop, matching the retail test-at-bottom shape.
void CScnItemPool::func_8048CEDC() {
    CScnItemPoolNode* iter;
    CScnItemPoolNode* sentinel;
    func_8048CA00((u32*)&iter, &mList0C);
    while ((func_8048C5AC((int*)&sentinel, &mList0C),
            func_8048C9D8((u32*)&iter, (u32*)&sentinel)) != 0) {
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&iter);
        item->vfuncD0();
        func_8048C9C8((int*)&iter);
    }
}

// CScnItemPool::update - invokes the item vtable slot at 0x0C for every item
// of sub-pools 0x6C, 0x4C and 0x0C, in that order.
void CScnItemPool::update() {
    CScnItemPoolNode* node1;      // sp+0x1c
    CScnItemPoolNode* node2;      // sp+0x18
    CScnItemPoolNode* node3;      // sp+0x14
    CScnItemPoolNode* sentinel1;  // sp+0x10
    CScnItemPoolNode* sentinel2;  // sp+0x0c
    CScnItemPoolNode* sentinel3;  // sp+0x08
    func_8048CA00((u32*)&node1, &mList6C);
    while ((func_8048C5AC((int*)&sentinel1, &mList6C),
            func_8048C9D8((u32*)&node1, (u32*)&sentinel1)) != 0) {
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&node1);
        item->vfunc0C();
        func_8048C9C8((int*)&node1);
    }
    func_8048CA00((u32*)&node2, &mList4C);
    while ((func_8048C5AC((int*)&sentinel2, &mList4C),
            func_8048C9D8((u32*)&node2, (u32*)&sentinel2)) != 0) {
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&node2);
        item->vfunc0C();
        func_8048C9C8((int*)&node2);
    }
    func_8048CA00((u32*)&node3, &mList0C);
    while ((func_8048C5AC((int*)&sentinel3, &mList0C),
            func_8048C9D8((u32*)&node3, (u32*)&sentinel3)) != 0) {
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&node3);
        item->vfunc0C();
        func_8048C9C8((int*)&node3);
    }
}

// __dt__8048C378: non-virtual teardown of a reslist sub-pool - unlinks all
// nodes, frees the mList node array (unless externally owned), and resets the
// capacity. The retail symbol is address-named, so this is written as an
// extern "C" free function (repo convention for __dt__8048xxxx symbols).
extern "C" void __dt__8048C378(CScnItemPoolListData* self) {
    CScnItemPoolLink* node = self->field_0x04->field_0x00;
    CScnItemPoolLink* sentinel;
    while (node != (sentinel = self->field_0x04)) {
        CScnItemPoolLink* cur = node;
        node = node->field_0x00;
        cur->field_0x00 = nullptr;
    }
    sentinel->field_0x00 = sentinel;
    self->field_0x04->field_0x04 = self->field_0x04;
    if (self->field_0x1c == false && self->field_0x14 != nullptr) {
        delete[] self->field_0x14;
        self->field_0x14 = nullptr;
    }
    self->field_0x18 = 0;
}
// func_8048CF58: 4-byte tail-call wrapper to func_8048CF5C (retail: b func_8048CF5C)
extern "C" void func_8048CF5C(CScnItemPool* self);
__declspec(noinline) void CScnItemPool::func_8048CF58() { func_8048CF5C(this); }
// CDeviceVICb-subobject thunks: adjust this from the +4 secondary base back to
// the primary and tail-call the implementation.
extern "C" void __dt__12CScnItemPoolFv(CScnItemPool* self, int flags);
extern "C" void __dt__11CDeviceVICbFv(void* self, int flags);
extern "C" void __dt__10IWorkEventFv(void* self, int flags);
void CScnItemPool::func_8048D014() {
    ((CScnItemPool*)((char*)this - 4))->func_8048CF58();
}
// One-arg opaque view: retail's thunk tail-calls WITHOUT setting r4, so the
// call site must not materialize the flags argument.
void CScnItemPool::func_8048D01C() {
    ((void (*)(CScnItemPool*))__dt__12CScnItemPoolFv)((CScnItemPool*)((char*)this - 4));
}

// __dt__12CScnItemPoolFv: complete-object destructor.
extern "C" u32 lbl_eu_8056E488[];
extern "C" __declspec(noinline) void __dt__12CScnItemPoolFv(CScnItemPool* self, int flags) {
    if (self != nullptr) {
        *(void**)self = (void*)&lbl_eu_8056E488;
        *(void**)((char*)self + 4) = (char*)&lbl_eu_8056E488 + 0x88;
        func_8048CCC0((u8*)self);
        __dt__8048C378((CScnItemPoolListData*)((char*)self + 0x0C));
        __dt__8048C378((CScnItemPoolListData*)((char*)self + 0x2C));
        __dt__8048C378((CScnItemPoolListData*)((char*)self + 0x4C));
        __dt__8048C378((CScnItemPoolListData*)((char*)self + 0x6C));
        __dt__8048C378((CScnItemPoolListData*)((char*)self + 0x8C));
        __dt__8048C378((CScnItemPoolListData*)((char*)self + 0xAC));
        if (self->mFlagsCC != nullptr) {
            mtl::MemManager::deallocate(self->mFlagsCC);
        }
        self->mFlagsCC = nullptr;
        if (self->mSlotsD0 != nullptr) {
            mtl::MemManager::deallocate(self->mSlotsD0);
        }
        self->mSlotsD0 = nullptr;
        if (self->mFlagsD4 != nullptr) {
            mtl::MemManager::deallocate(self->mFlagsD4);
        }
        self->mFlagsD4 = nullptr;
        if (self->mSlotsD8 != nullptr) {
            mtl::MemManager::deallocate(self->mSlotsD8);
        }
        self->mSlotsD8 = nullptr;
        ((reslist<CScnItem*>*)((char*)self + 0xAC))->~reslist();
        ((reslist<CScnItem*>*)((char*)self + 0x8C))->~reslist();
        ((reslist<CScnItem*>*)((char*)self + 0x6C))->~reslist();
        ((reslist<CScnItem*>*)((char*)self + 0x4C))->~reslist();
        ((reslist<CScnItem*>*)((char*)self + 0x2C))->~reslist();
        ((reslist<CScnItem*>*)((char*)self + 0x0C))->~reslist();
        __dt__11CDeviceVICbFv((void*)((char*)self + 4), 0);
        __dt__10IWorkEventFv((void*)self, 0);
        if (flags < 0) {
            delete self;
        }
    }
}
// func_8048CF5C: drains the sub-pool at 0xAC, unlinking each node and calling
// the item's vtable slot at 0x08 with argument 1 (non-null items only).
// ===== Dissolved monolibdata2 data owned by this TU (retail .data
// 0x8056E488-0x8056E568, .rodata 0x80523F18-0x80523F58) =====
// The class vtable (+CDeviceVICb sub-vtable group), the RTTI base-list block
// and the class-name string are spelled by hand (CLibCriMoviePlay.cpp
// pattern); every word is an explicit &reloc so the object reproduces the
// retail reloc names exactly.

// Foreign words referenced by the blocks below (defined in monolibdata2 /
// other TUs). func_8048CF58__12CScnItemPoolFv is this TU's own member — the
// extern "C" spelling matches its C++ mangled symbol exactly.
extern "C" {
extern u32 lbl_eu_80663938[];  // RTTI locator ti(CScnItemPool) (.sdata)
extern u32 lbl_eu_80663618[];  // RTTI locator ti(reslist<CScnItem*>) (.sdata)
extern u32 lbl_eu_80663940[];  // RTTI locator (reslist<CScnItem*> base-list head)
extern u32 lbl_eu_80663948[];  // RTTI locator ti(_reslist_base<CScnItem*>) (.sdata)
extern void func_8048D01C__12CScnItemPoolFv();
extern void func_8048D014__12CScnItemPoolFv();
extern void viBeforeDrawDone__11CDeviceVICbFv();
extern void viBeginFrame__11CDeviceVICbFv();
extern void func_8048CF58__12CScnItemPoolFv();
}

// [.rodata] 0x80523F18 (0xD + 3 align pad): CScnItemPool RTTI class-name
// string, referenced by the .sdata locator lbl_eu_80663938 (monolibdata2).
// Sized 0x10 so the retail align pad lands inside the definition.
extern "C" const char lbl_eu_80523F18[0x10] = "CScnItemPool";

// [.data] 0x8056E488-0x8056E52C (0xA4): __vt__12CScnItemPool — primary group
// ([rtti, 0, dtor, IWorkEvent 1-31]) followed by the CDeviceVICb sub-vtable
// group ([rtti, -4, 5 slots]).
extern "C" u32 lbl_eu_8056E488[41] = {
    (u32)&lbl_eu_80663938, 0x00000000,
    (u32)&__dt__12CScnItemPoolFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    // CDeviceVICb sub-vtable group (this -4)
    (u32)&lbl_eu_80663938, 0xFFFFFFFC,
    (u32)&func_8048D01C__12CScnItemPoolFv,
    (u32)&viBeforeDrawDone__11CDeviceVICbFv,
    (u32)&func_8048D014__12CScnItemPoolFv,
    (u32)&viBeginFrame__11CDeviceVICbFv,
    (u32)&func_8048CF58__12CScnItemPoolFv,
};

// [.data] 0x8056E52C-0x8056E540 (0x14): RTTI base-list block
// ([lbl_eu_80663618, 4], [IWorkEvent, 0], [0]).
extern "C" u32 lbl_eu_8056E52C[5] = {
    (u32)&lbl_eu_80663618, 0x00000004,
    (u32)&__RTTI__10IWorkEvent, 0x00000000,
    0x00000000,
};

#pragma push
#pragma auto_inline off
extern "C" void func_8048CF5C(CScnItemPool* self) {
    _reslist_base<CScnItem*>* list = (_reslist_base<CScnItem*>*)&self->mListAC;
    _reslist_node<CScnItem*>* itemNode;  // sp+0x18
    _reslist_node<CScnItem*>* out;       // sp+0x14
    _reslist_node<CScnItem*>* unlink;    // sp+0x10
    _reslist_node<CScnItem*>* node;      // sp+0x0c
    _reslist_node<CScnItem*>* sentinel;  // sp+0x08
    while ((func_8048C5AC((int*)&sentinel, list),
            func_8048CA00((u32*)&node, list),
            func_8048C9D8((u32*)&node, (u32*)&sentinel)) != 0) {
        func_8048CA00((u32*)&itemNode, list);
        CScnItem* item = *(CScnItem**)func_8048C9F4((u8*)&itemNode);
        func_8048CA00((u32*)&unlink, list);
        func_8048CA34((CScnItemPoolLink**)&out, (u32)list, (CScnItemPoolLink**)&unlink);
        if (item != 0) {
            if (item != 0) {
                item->vfunc08(1);
            }
        }
    }
}
#pragma pop

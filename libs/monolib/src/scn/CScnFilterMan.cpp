// Auto-scaffolded catalog TU for monolib/src/scn/CScnFilterMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"  // __construct_new_array
#include "libs/monolib/src/scn/CScnFilterMan.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CGXCache.hpp"
#include "monolib/core/CViewRoot.hpp"

// Setter helpers are declared here but defined at the BOTTOM of this file
// (after every caller) so MWCC's -inline auto cannot fold them into the
// tail-call wrappers below; retail keeps them as separate functions (same
// pattern as CGXCache.cpp resetGXStateA).
extern "C" void ScnFilterListIter_setNode(CScnFilterListIter* self, u32 val);
extern "C" void ScnFilterIterListIter_setNode(CScnFilterListIter* self, u32 val);
extern "C" void ScnFilterIterList_clear(CScnFilterIteratorReslist* self);
extern "C" void ScnFilterIterList_destroyItem(CScnFilterIterator* item);
extern "C" void ScnFilterIterList_freeNode(void* self, void* out);
extern "C" void ScnFilterList_pushBack(CScnFilterReslist* list, u32* val);
extern "C" void* ScnFilterNode_store(void* self, u32 val);
extern "C" void ScnFilterIterListIter_copy(int* dst, int* src);
extern "C" void ScnFilterIterListIter_advance(u32* self);
extern "C" void ScnFilterList_clear(CScnFilterReslist* self);
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_getSentinel(CScnFilterList* list);
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_getFirst(CScnFilterList* list);
extern "C" void ScnFilterList_countNodes(CScnFilterListNode** cur, CScnFilterListNode** end, u32* out);

// CGXCache helpers reached by their flat retail symbols (the member decls in
// CGXCache.hpp are zero-param; func_8044A7F8 actually lives on r4-r8).
extern "C" void resetGXStateA__8CGXCacheFv(CGXCache* cache);
extern "C" void func_8044A7F8__8CGXCacheFv(CGXCache* cache, u32 w0, u32 w1, u32 w2,
                                           u32 w3, int flag);
extern "C" u32 ScnFilterList_size(CScnFilterReslist* list);
extern "C" CGXCache* ScnFilterMan_getGXCache(void);
extern "C" s32 ScnFilter_isRenderEnabled(CScnFilter* filter);
extern "C" void func_8049D9D0(CScnFilterMan* self);

// Flattened template ctors defined below; forward-declared so the derived
// ctors above them emit bl calls instead of inlining the base init.
void __ct___reslist_base_CScnFilter(CScnFilterReslist* obj);
extern "C" void* __ct__8049CC10(CScnFilterReslist* obj);
extern "C" void* __dt___reslist_base_CScnFilter(CScnFilterReslist* self, int mode);
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_findFreeSlot(CScnFilterReslist* list);
extern "C" __declspec(noinline) void ScnFilterList_setItem(CScnFilter** item, u32 val);
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterIterList_findFreeSlot(CScnFilterReslist* list);
extern "C" __declspec(noinline) void ScnFilterIterList_setItem(CScnFilter** item, u32 val);

// reslist<CScnFilter*> ctor (retail __ct__reslist_CScnFilter): base init,
// then install the derived vtable. The flattened retail name cannot be
// produced by a template instantiation under Wii/1.1 (symbolic mangling),
// so it is spelled out (MWCC_CASES "flattened constructor" pattern).
#pragma push
#pragma auto_inline off
extern "C" CScnFilterReslist* __ct__reslist_CScnFilter(CScnFilterReslist* obj) {
    __ct___reslist_base_CScnFilter(obj);
    obj->mVtable = lbl_eu_8056EBAC;
    return obj;
}
#pragma pop

// flattened base ctor for the iterator reslist (retail __ct__8049CC10):
// zero the vptr slot, run the no-op member init helper at +8, then set up
// the empty ring (mList/capacity/flag cleared, sentinel linked to itself).
extern "C" CScnFilterMan* ScnFilterMan_initFilterList(CScnFilterMan* self);
extern "C" __declspec(noinline) void* __ct__8049CC10(CScnFilterReslist* obj) {
    obj->mVtable = (u32*)lbl_eu_8056EBA0;
    ScnFilterMan_initFilterList((CScnFilterMan*)((char*)obj + 8));
    obj->mList = NULL;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = obj->mStartNodePtr;
    obj->mStartNodePtr->mPrev = obj->mStartNodePtr;
    return obj;
}

// Unknown-class ctor (retail __ct__8049CBD4): init via __ct__8049CC10, then
// install the derived vtable. The object has a reslist-shaped layout
// (vtable@0, sentinel@8, mList@0x14, mCapacity@0x18, flag@0x1C).
#pragma push
#pragma auto_inline off
extern "C" CScnFilterReslist* __ct__8049CBD4(CScnFilterReslist* obj) {
    __ct__8049CC10(obj);
    obj->mVtable = lbl_eu_8056EB88;
    return obj;
}
#pragma pop

// _reslist_base<CScnFilter*> ctor (retail __ct___reslist_base_CScnFilter):
// zeroes the storage fields and links the sentinel node onto itself. The
// flattened retail name cannot be produced by a template instantiation under
// Wii/1.1 (symbolic mangling), so it is spelled out (MWCC_CASES
// "flattened constructor" pattern, cf. CVirtualLightAmb).
__declspec(noinline) void __ct___reslist_base_CScnFilter(CScnFilterReslist* obj) {
    obj->mVtable = lbl_eu_8056EBC4;
    obj->mList = nullptr;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = &obj->mStartNode;
    obj->mStartNodePtr->mPrev = &obj->mStartNode;
}

// reslist<CScnFilter*> deleting destructor (retail __dt__reslist_CScnFilter):
// destroy the base list, then free the object when mode > 0.
CScnFilterReslist* __dt__reslist_CScnFilter(CScnFilterReslist* obj, int mode) {
    if (obj != NULL) {
        __dt___reslist_base_CScnFilter(obj, 0);
        if (mode > 0) {
            delete obj;
        }
    }
    return obj;
}

// _reslist_base<CScnFilter*> deleting destructor (retail
// __dt___reslist_base_CScnFilter): clear the ring, free the node array
// unless it is externally owned (field_0x1C), then free the object when
// deleting > 0. Kept extern "C" so the derived dtor's bl stays flat-named.
extern "C" __declspec(noinline) void* __dt___reslist_base_CScnFilter(CScnFilterReslist* self, int deleting) {
    if (self != NULL) {
        self->mVtable = lbl_eu_8056EBC4;
        ScnFilterList_clear(self);
        if (self->field_0x1C == false) {
            if (self->mList != NULL) {
                delete[] self->mList;
                self->mList = NULL;
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}


// destroy a node's item (no-op in retail). Flat symbol + noinline so the
// bl at every call site stays emitted (MWCC -inline auto would fold it).
extern "C" __declspec(noinline) void ScnFilterList_destroyItem(CScnFilter** item) {}

#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void ScnFilterZeroWord(void* self) { *(u32*)self = 0; }
#pragma pop


// reset a node's mNext after removal (no-op-ish free marker). noinline keeps
// the call emitted at the clearList/erase call sites.
extern "C" __declspec(noinline) void ScnFilterList_freeNode(void* self, void* out) { *(u32*)out = 0; }


// Guard the no-op stub so the call sites survive (MWCC_CASES
// empty-stub pattern: -ipa file inlines the empty body and drops the bl).
#pragma push
#pragma auto_inline off
extern "C" void ScnFilterList_initNoop(CScnFilterReslist* list) {}
#pragma pop

// (re)initialise the filter list member at self+8 and return self. The
// callee ScnFilterList_initNoop is a no-op in retail; the call is still emitted
// (retail ctor-style helper pattern).
extern "C" CScnFilterMan* ScnFilterMan_initFilterList(CScnFilterMan* self) {
    ScnFilterList_initNoop(&self->field_08);
    return self;
}

// Iterator-reslist base deleting destructor (retail
// __dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter):
// clear the ring, free the node array (cookie'd delete[]: the node item has
// a user ctor, so the array carries MWCC's 0x10 cookie; the node dtor stays
// trivial so delete[] skips the per-element dtor loop) unless externally
// owned, then free the object when deleting > 0. Kept extern "C" so the
// derived dtor's bl stays flat-named.
extern "C" __declspec(noinline) void* __dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter(CScnFilterIteratorReslist* self, int deleting) {
    if (self != NULL) {
        self->mVtable = lbl_eu_8056EBA0;
        ScnFilterIterList_clear(self);
        if (self->field_0x1C == false) {
            if (self->mList != NULL) {
                delete[] self->mList;
                self->mList = NULL;
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

// clearList for the iterator reslist: walk the ring from the first node,
// destroy each node's item (ScnFilterIterList_destroyItem) and reset its mNext
// (ScnFilterIterList_freeNode), then relink the sentinel onto itself. Same shape as
// ScnFilterList_clear (reslist.hpp clearList).
#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void ScnFilterIterList_clear(CScnFilterIteratorReslist* self) {
    CScnFilterIterNode* node = self->mStartNodePtr->mNext;
    while (node != self->mStartNodePtr) {
        CScnFilterIterNode* cur = node;
        node = node->mNext;
        ScnFilterIterList_destroyItem(&cur->mItem);
        ScnFilterIterList_freeNode(self, cur);
    }
    self->mStartNodePtr->mNext = self->mStartNodePtr;
    self->mStartNodePtr->mPrev = self->mStartNodePtr;
}
#pragma pop

// destroy an iterator item (no-op in retail). Flat symbol + noinline so the
// bl at the clearList call site stays emitted.
extern "C" __declspec(noinline) void ScnFilterIterList_destroyItem(CScnFilterIterator* item) {}

extern "C" __declspec(noinline) void ScnFilterIterList_freeNode(void* unused, void* out) { *(u32*)out = 0; }

// reslist<CScnFilter*>::iterator deleting destructor (retail
// __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter): destroy
// the (empty) iterator base, then free the object when mode > 0. Same shape
// as __dt__reslist_CScnFilter. Kept extern "C" - retail callers (the man
// dtor) and the blob vtable both use the flat name.
extern "C" __declspec(noinline) CScnFilterListIter* __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter(CScnFilterListIter* self, int mode) {
    if (self != NULL) {
        __dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter((CScnFilterIteratorReslist*)self, 0);
        if (mode > 0) {
            delete self;
        }
    }
    return self;
}

// reserve the reslist node array (retail ScnFilterList_allocate): allocate
// capacity*0xC bytes via MemManager::allocate_array (the alloc handle in r4
// stays live in its incoming register, so retail emits no move), zero the
// mNext of every node (MWCC auto-unrolls the fill loop 8x with overflow
// guards), then store the capacity.
extern "C" void ScnFilterList_allocate(CScnFilterReslist* list, u32 handle, int capacity);
#pragma push
#pragma auto_inline off
extern "C" void ScnFilterList_reserve(CScnFilterReslist* list, u32 handle, int capacity) { ScnFilterList_allocate(list, handle, capacity); }
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void ScnFilterList_allocate(CScnFilterReslist* list, u32 handle, int capacity) {
    list->mList = (CScnFilterListNode*)mtl::MemManager::allocate_array((u32)capacity * 0xC, handle);
    for (int i = 0; i < capacity; i++) {
        list->mList[i].mNext = nullptr;
    }
    list->mCapacity = capacity;
}
#pragma pop

extern "C" void ScnFilterIterList_allocate(CScnFilterIteratorReslist* list, u32 handle, int capacity);
#pragma push
#pragma auto_inline off
extern "C" void ScnFilterIterList_reserve(CScnFilterIteratorReslist* list, u32 handle, int capacity) { ScnFilterIterList_allocate(list, handle, capacity); }
#pragma pop

// reserve for the iterator reslist: the node item (CScnFilterIterator) has a
// user-declared default ctor, so the node array carries MWCC's 0x10 cookie
// (alloc size = capacity*0xC + 0x10) and element construction goes through
// __construct_new_array with the flat init helper ScnFilterMan_initFilterList as the
// per-element ctor. Afterwards clear every node's mNext.
#pragma push
#pragma auto_inline off
extern "C" void ScnFilterIterList_allocate(CScnFilterIteratorReslist* list, u32 handle, int capacity) {
    list->mList = (CScnFilterIterNode*)__construct_new_array(
        mtl::MemManager::allocate_array((u32)capacity * 0xC + 0x10, handle),
        (void (*)(void*))ScnFilterMan_initFilterList, nullptr, 0xC, capacity);
    for (int i = 0; i < capacity; i++) {
        list->mList[i].mNext = nullptr;
    }
    list->mCapacity = capacity;
}
#pragma pop

// ~CScnFilterMan: deregister from the scene render-callback list, flush
// both reslists, then run member/base destructors. MWCC auto-generates the
// null-check prologue, the delete-flag tail (operator delete), and the
// `this` return. The vtable store re-asserts the FMBlob vtable before the
// callback removal (retail +0x24 lis/addi/stw).
namespace FMDtorCalls {
extern "C" CScnFilterReslist* __dt__reslist_CScnFilter(CScnFilterReslist* obj, int mode);
}
extern "C" void __dt__10IScnRenderFv(void* self, int flag);
extern "C" u32 lbl_eu_8056EB78[4];
extern "C" void ScnFilterList_destroy(void* self);
extern "C" void ScnFilterIterList_destroy(CScnFilterIteratorReslist* self);

// flattened init function (retail __ct__CScnFilterMan): base IScnRender
// init, vptr install, both reslists constructed, flags cleared, seed lists,
// register with the scene render-callback chain.
extern "C" void __ct__IScnRender(void* self);
extern "C" void zeroFirstWord(void* self);
extern "C" void* func_80496018(CScn* scene);
extern "C" u32 lbl_eu_8056EB78[4];

extern "C" CScnFilterMan* __ct__CScnFilterMan(CScnFilterMan* self, CScn* scene) {
    __ct__IScnRender(self);
    self->mScene = scene;
    *(void**)self = (void*)lbl_eu_8056EB78;
    __ct__reslist_CScnFilter(&self->field_08);
    __ct__8049CBD4((CScnFilterReslist*)&self->field_28);
    ScnFilterZeroWord(&self->field_48);
    zeroFirstWord(&self->field_48);
    ScnFilterList_reserve(&self->field_08, (u32)func_80496018(scene), 4);
    ScnFilterIterList_reserve(&self->field_28, (u32)func_80496018(scene), 4);
    scene->addRenderCB((IScnRender*)self, 3, 0);
    return self;
}

CScnFilterMan::~CScnFilterMan() {
    *(void**)this = (void*)lbl_eu_8056EB78;
    mScene->removeRenderCB((IScnRender*)this);
    ScnFilterList_destroy(&field_08);
    ScnFilterIterList_destroy(&field_28);
    __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter((CScnFilterListIter*)&field_28, -1);
    FMDtorCalls::__dt__reslist_CScnFilter(&field_08, -1);
    __dt__10IScnRenderFv(this, 0);
}

extern "C" void __dt__8049D130(CScnFilterReslist* self);
#pragma push
#pragma auto_inline off
extern "C" void ScnFilterList_destroy(void* self) { __dt__8049D130((CScnFilterReslist*)self); }
#pragma pop

// destroyList for the filter reslist member: clear the ring, free the node
// array unless it is externally owned (field_0x1C), and reset the capacity.
extern "C" __declspec(noinline) void __dt__8049D130(CScnFilterReslist* self) {
    ScnFilterList_clear(self);
    if (self->field_0x1C == false) {
        if (self->mList != NULL) {
            delete[] self->mList;
            self->mList = NULL;
        }
    }
    self->mCapacity = 0;
}

extern "C" void __dt__8049D18C(CScnFilterIteratorReslist* self);
#pragma push
#pragma auto_inline off
extern "C" void ScnFilterIterList_destroy(CScnFilterIteratorReslist* self) { __dt__8049D18C(self); }
#pragma pop

extern "C" void __dla__FPv(void* p);

// reslist-base dtor: clear the node array and ownership flag. The mList
// backing block (allocated 16 bytes before the array) is freed when the
// ownership flag is clear and the array is non-null.
#pragma push
#pragma auto_inline off
extern "C" void __dt__8049D18C(CScnFilterIteratorReslist* self) {
    ScnFilterIterList_clear(self);
    if (self->field_0x1C != 0)
        goto done;
    if (self->mList == 0)
        goto done;
    if (self->mList != 0)
        __dla__FPv((char*)self->mList - 16);
    self->mList = 0;
done:
    self->mCapacity = 0;
}
#pragma pop

// Forward (self, val) to ScnFilterList_pushBack as (reslist member at self+8, &val).
extern "C" void ScnFilterMan_appendFilter(CScnFilterMan* self, u32 val) {
    ScnFilterList_pushBack(&self->field_08, &val);
}

// iter = list end sentinel (retail: lwz r4,0x4(r4); b ScnFilterListIter_setNode)
extern "C" __declspec(noinline) void ScnFilterList_getEnd(CScnFilterListIter* self, CScnFilterList* list) {
    ScnFilterListIter_setNode(self, (u32)list->mStartNodePtr);
}

extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_allocNode(
    CScnFilterReslist* list, u32 val);

// Allocate a node for `val`, link it into the list before `iter`'s node, and
// point the output iterator at it.
extern "C" __declspec(noinline) void ScnFilterList_insert(CScnFilterListIter* self, CScnFilterReslist* list,
                   CScnFilterListIter* iter, u32* val) {
    CScnFilterListNode* node = ScnFilterList_allocNode(list, (u32)val);
    node->mNext = iter->mNode;
    node->mPrev = iter->mNode->mPrev;
    iter->mNode->mPrev->mNext = node;
    iter->mNode->mPrev = node;
    ScnFilterListIter_setNode(self, (u32)node);
}

// Insert `val` at the end of the filter list (retail: end-iterator then
// insert via the D26C/D27C pair).
extern "C" __declspec(noinline) void ScnFilterList_pushBack(CScnFilterReslist* list, u32* val) {
    CScnFilterListIter iter2;
    CScnFilterListIter iter;
    ScnFilterList_getEnd(&iter, (CScnFilterList*)list);
    ScnFilterList_insert(&iter2, list, &iter, val);
}

// Get a free node slot from the node array and install `val` into its item
// slot, returning the node (retail: free-slot search then item setter).
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_allocNode(
    CScnFilterReslist* list, u32 val) {
    CScnFilterListNode* node = ScnFilterList_findFreeSlot(list);
    ScnFilterList_setItem(&node->mItem, val);
    return node;
}

// setItem for the filter reslist: construct a CScnFilter* cell at *item via
// the placement-new operator (scnVlPlaceNew), then copy *val into it. The
// try/catch is the retail setItem shape (forces MWCC's frame-pointer prologue).
extern "C" __declspec(noinline) void ScnFilterList_setItem(CScnFilter** item, u32 val) {
    CScnFilter** p = (CScnFilter**)scnVlPlaceNew((void*)4, (void*)item);
    if (p != NULL) {
        try {
            *p = *(CScnFilter**)val;
        } catch (...) {
            throw;
        }
    }
}

// Find the first free slot (mNext == 0) in the node array and return its
// address. Retail uses the counted-loop form (mtctr/bdnz) with a byte
// offset induction variable plus the index for the final mulli.
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_findFreeSlot(CScnFilterReslist* list) {
    int capacity = list->mCapacity;
    int i;
    for (i = 0; i < capacity; i++) {
        if (list->mList[i].mNext == 0) {
            break;
        }
    }
    return &list->mList[i];
}

// Add/remove a filter value: find `val` in the filter reslist; if present,
// either erase its node from the filter list (when the manager's flag bit 0
// is clear) or re-insert the value into the iterator reslist (flag set).
extern "C" void ScnFilterList_getEnd(CScnFilterListIter* self, CScnFilterList* list);
extern "C" void ScnFilterList_getBegin(CScnFilterListIter* self, CScnFilterList* list);
extern "C" void ScnFilterList_find(int* out, CScnFilterListIter* first, u32* last, u32* value);
extern "C" u32 ScnFilterListIter_notEqual(u32* a, u32* b);
extern "C" int* ScnFilterListIter_copy(int* dst, int* src);
extern "C" void ScnFilterList_erase(CScnFilterListIter* self, CScnFilterReslist* list, CScnFilterListIter* it);
extern "C" void ScnFilterIterList_pushBack(CScnFilterReslist* list, u32* val);
extern "C" void ScnFilterMan_toggleFilter(CScnFilterMan* man, u32 val) {
    // MWCC assigns frame slots in reverse declaration order; this order
    // reproduces the retail layout (found@0x20, begin@0x1c, end@0x18,
    // end2@0x14, erase@0x10, tmp@0xc).
    CScnFilterListIter itFound;
    CScnFilterListIter itBegin;
    CScnFilterListIter itEnd;
    CScnFilterListIter itEnd2;
    CScnFilterListIter itErase;
    u32 tmpVal;

    ScnFilterList_getEnd(&itEnd, (CScnFilterList*)&man->field_08);
    ScnFilterList_getBegin(&itBegin, (CScnFilterList*)&man->field_08);
    ScnFilterList_find((int*)&itFound, &itBegin, (u32*)&itEnd, &val);
    ScnFilterList_getEnd(&itEnd2, (CScnFilterList*)&man->field_08);
    if (ScnFilterListIter_notEqual((u32*)&itFound, (u32*)&itEnd2) != 0) {
        if (testFlagMask(&man->field_48, 1) == 0) {
            ScnFilterList_erase(&itErase, &man->field_08,
                (CScnFilterListIter*)ScnFilterListIter_copy((int*)&tmpVal, (int*)&itFound));
        } else {
            ScnFilterIterList_pushBack((CScnFilterReslist*)&man->field_28, (u32*)&itFound);
        }
    }
}

// noinline keeps the bl from ScnFilterListIter_copyAdvance's advance out-of-line (retail
// emits the call; -ipa file would inline this small body and drop it).
extern "C" __declspec(noinline) void ScnFilterListIter_advance(u32* self) { *self = *(u32*)(*(u32**)self); }

extern "C" __declspec(noinline) void* ScnFilterListIter_getItem(void* self){ return (void*)((char*)*(void**)self + 8); }

// iter = first node (retail: lwz r4,0x4(r4); lwz r4,0x0(r4); b ScnFilterListIter_setNode)
extern "C" __declspec(noinline) void ScnFilterList_getBegin(CScnFilterListIter* self, CScnFilterList* list) {
    ScnFilterListIter_setNode(self, (u32)list->mStartNodePtr->mNext);
}

extern "C" __declspec(noinline) u32 ScnFilterListIter_notEqual(u32* a, u32* b) { return *a != *b; }


extern "C" __declspec(noinline) int* ScnFilterListIter_copy(int* dst, int* src){
    *dst = *src;
    return dst;   // retail callers chain the returned dst pointer
}

// Advance `first` until it hits `last` or its item equals *value, then
// copy the resulting iterator into *out (retail performs the copy on every
// exit path, including running off the end).
void ScnFilterList_find(int* out, CScnFilterListIter* first, u32* last, u32* value) {
    while (ScnFilterListIter_notEqual((u32*)first, last) &&
           *(CScnFilter**)ScnFilterListIter_getItem(first) != *(CScnFilter**)value) {
        ScnFilterListIter_advance((u32*)first);
    }
    ScnFilterListIter_copy(out, (int*)first);
}

// Remove `it`'s node from the ring (unlink prev/next), destroy its item and
// free the node, then set self->mNode to the next node (retail
// ScnFilterList_erase; node read back through it->mNode at each step).
extern "C" __declspec(noinline) void ScnFilterList_erase(CScnFilterListIter* self, CScnFilterReslist* list, CScnFilterListIter* it) {
    CScnFilterListNode* node = it->mNode;
    CScnFilterListNode* prev = node->mPrev;
    CScnFilterListNode* next = node->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    ScnFilterList_destroyItem(&it->mNode->mItem);
    ScnFilterList_freeNode(list, it->mNode);
    ScnFilterListIter_setNode(self, (u32)next);
}

// iter = list end sentinel via the D64C setter
extern "C" __declspec(noinline) void ScnFilterIterList_getEnd(CScnFilterListIter* self, CScnFilterList* list) {
    ScnFilterIterListIter_setNode(self, (u32)list->mStartNodePtr);
}

extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterIterList_allocNode(
    CScnFilterReslist* list, u32 val);
extern "C" void ScnFilterIterListIter_setNode(CScnFilterListIter* self, u32 val);

// Allocate a node for `val`, link it into the list before `iter`'s node, and
// point the output iterator at it (same shape as ScnFilterList_insert).
extern "C" __declspec(noinline) void ScnFilterIterList_insert(CScnFilterListIter* self, CScnFilterReslist* list,
                   CScnFilterListIter* iter, u32* val) {
    CScnFilterListNode* node = ScnFilterIterList_allocNode(list, (u32)val);
    node->mNext = iter->mNode;
    node->mPrev = iter->mNode->mPrev;
    iter->mNode->mPrev->mNext = node;
    iter->mNode->mPrev = node;
    ScnFilterIterListIter_setNode(self, (u32)node);
}

// Insert `val` at the end of the filter list (retail: end-iterator then
// insert via the D644/D654 pair; same shape as ScnFilterList_pushBack).
extern "C" void ScnFilterIterList_pushBack(CScnFilterReslist* list, u32* val) {
    CScnFilterListIter iter2;
    CScnFilterListIter iter;
    ScnFilterIterList_getEnd(&iter, (CScnFilterList*)list);
    ScnFilterIterList_insert(&iter2, list, &iter, val);
}

// Get a free node slot from the node array and install `val` into its item
// slot, returning the node (retail: free-slot search then item setter).
CScnFilterListNode* ScnFilterIterList_allocNode(CScnFilterReslist* list, u32 val) {
    CScnFilterListNode* node = ScnFilterIterList_findFreeSlot(list);
    ScnFilterIterList_setItem(&node->mItem, val);
    return node;
}

// setItem for the second filter list: same placement-new shape as
// ScnFilterList_setItem, but the value copy goes through ScnFilterListIter_copy.
extern "C" __declspec(noinline) void ScnFilterIterList_setItem(CScnFilter** item, u32 val) {
    CScnFilter** p = (CScnFilter**)scnVlPlaceNew((void*)4, (void*)item);
    if (p != NULL) {
        try {
            ScnFilterListIter_copy((int*)p, (int*)val);
        } catch (...) {
            throw;
        }
    }
}

// Same free-slot search as ScnFilterList_findFreeSlot (retail bodies are identical).
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterIterList_findFreeSlot(CScnFilterReslist* list) {
    int capacity = list->mCapacity;
    int i;
    for (i = 0; i < capacity; i++) {
        if (list->mList[i].mNext == 0) {
            break;
        }
    }
    return &list->mList[i];
}

// Per-frame update: mark the manager active (+0x48 |= 1), run every enabled
// filter's update slot (vtable +0xC, fed the scene pointer), rebuild the
// filter reslist from the iterator reslist, then flush the iterator list.
namespace FMCb {
struct Slot {
    virtual void unk_04();
    virtual void unk_08(CScn* scene);
    // NOTE: MWCC places two implicit dtor slots ahead of these, so the next
    // declared virtual lands at vtable+0x10 (the slot ScnFilterMan_preRender calls).
    virtual void unk_10(CScn* scene);
};
} // namespace FMCb
extern "C" void clearFlagMask(u32* self, u32 val);
extern "C" void ScnFilterMan_setFlags(u32* self, u32 flags);
extern "C" void* ScnFilterIterListIter_getItem(void* self);
extern "C" __declspec(noinline) void ScnFilterListIter_copyAdvance(CScnFilterListIter* dst, CScnFilterListIter* src, int unused);
extern "C" __declspec(noinline) void ScnFilterIterListIter_copyAdvance(CScnFilterListIter* dst, CScnFilterListIter* src, int unused);
extern "C" u32 ScnFilterIterListIter_notEqual(const u32* a, const u32* b);
extern "C" void ScnFilterIterList_flush(CScnFilterIteratorReslist* self);
extern "C" s32 ScnFilter_isUpdateEnabled(CScnFilter* filter);
extern "C" void ScnFilterIterList_getBegin(CScnFilterListIter* self, CScnFilterList* list);

void CScnFilterMan::update() {
    ScnFilterMan_setFlags(&field_48, 1);
    // declaration order drives MWCC's reverse slot allocation (retail:
    // itA=36, it2=32, tmp=28, itEnd=24, itOut=20, val=16, adv=12, end2=8)
    CScnFilterListIter itA;
    CScnFilterListIter it2;
    CScnFilterListIter itTmp;
    CScnFilterListIter itEnd;
    CScnFilterListIter itOut;
    u32 tmpVal;
    CScnFilterListIter itAdv;
    CScnFilterListIter itEnd2;

    ScnFilterList_getBegin(&itA, (CScnFilterList*)&field_08);
    while (ScnFilterList_getEnd(&itEnd, (CScnFilterList*)&field_08),
           ScnFilterListIter_notEqual((u32*)&itA, (u32*)&itEnd)) {
        if (ScnFilter_isUpdateEnabled(*(CScnFilter**)ScnFilterListIter_getItem(&itA)) == 0) {
            ((FMCb::Slot*)*(CScnFilter**)ScnFilterListIter_getItem(&itA))->unk_08(mScene);
        }
        ScnFilterListIter_copyAdvance(&itTmp, &itA, 0);
    }
    clearFlagMask(&field_48, 1);

    ScnFilterIterList_getBegin(&it2, (CScnFilterList*)&field_28);
    while (ScnFilterIterList_getEnd(&itEnd2, (CScnFilterList*)&field_28),
           ScnFilterIterListIter_notEqual((u32*)&it2, (u32*)&itEnd2)) {
        CScnFilterListIter* pNext =
            (CScnFilterListIter*)ScnFilterListIter_copy((int*)&tmpVal, (int*)(u32*)ScnFilterIterListIter_getItem(&it2));
        ScnFilterList_erase(&itOut, &field_08, pNext);
        ScnFilterIterListIter_copyAdvance(&itAdv, &it2, 0);
    }
    ScnFilterIterList_flush(&field_28);
}

extern "C" __declspec(noinline) void ScnFilterMan_setFlags(u32* self, u32 flags) { *self |= flags; }

// True when the filter's mFlags bit 0 is clear (retail: flag-test helper
// call, result inverted via the cntlzw/srwi "== 0" idiom).
extern "C" __declspec(noinline) s32 ScnFilter_isUpdateEnabled(CScnFilter* filter) {
    return testFlagMask(&filter->mFlags, 1) == 0;
}

// Copy src into dst, then advance src by one node (retail: *dst = *src via
// ScnFilterListIter_copy, then src->mNode = src->mNode->mNext via ScnFilterListIter_advance).
extern "C" __declspec(noinline) void ScnFilterListIter_copyAdvance(CScnFilterListIter* dst, CScnFilterListIter* src, int unused) {
    ScnFilterListIter_copy((int*)dst, (int*)src);
    ScnFilterListIter_advance((u32*)src);
}

// iter = first node via the D64C setter
extern "C" __declspec(noinline) void ScnFilterIterList_getBegin(CScnFilterListIter* self, CScnFilterList* list) {
    ScnFilterIterListIter_setNode(self, (u32)list->mStartNodePtr->mNext);
}

extern "C" __declspec(noinline) void* ScnFilterIterListIter_getItem(void* self){ return (void*)((char*)*(void**)self + 8); }

// Copy src into dst, then advance src by one node (ScnFilterIterListIter_copy copy +
// ScnFilterIterListIter_advance advance pair).
extern "C" __declspec(noinline) void ScnFilterIterListIter_copyAdvance(CScnFilterListIter* dst, CScnFilterListIter* src, int unused) {
    ScnFilterIterListIter_copy((int*)dst, (int*)src);
    ScnFilterIterListIter_advance((u32*)src);
}

// noinline keeps the bl calls from ScnFilterIterListIter_copyAdvance out-of-line (retail emits
// the calls; -ipa file would inline these small bodies and drop them).
extern "C" __declspec(noinline) void ScnFilterIterListIter_copy(int* dst, int* src){
    *dst = *src;
}

extern "C" __declspec(noinline) void ScnFilterIterListIter_advance(u32* self) { *self = *(u32*)(*(u32**)self); }

// != on the u32 values at the two pointers (MWCC dual-subf idiom).
extern "C" __declspec(noinline) u32 ScnFilterIterListIter_notEqual(const u32* a, const u32* b) { return *a != *b; }

extern "C" void ScnFilterIterList_clear(CScnFilterIteratorReslist* self);
extern "C" __declspec(noinline) void ScnFilterIterList_flush(CScnFilterIteratorReslist* self) { ScnFilterIterList_clear(self); }

extern "C" u8 ScnFilter_getState(void* self);
extern "C" s32 ScnFilterListIter_equal(CScnFilterListIter* a, CScnFilterListIter* b);

// Bubble-sort the filter list by each filter's state byte (descending):
// repeatedly walk the list swapping adjacent out-of-order items until a
// full pass makes no swap. Slot layout follows retail (cur=0x1c ..
// end2=0x8); declaration order drives MWCC's reverse allocation.
#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void func_8049D9D0(CScnFilterMan* self) {
    bool swapped;
    CScnFilterListIter itCur;   // 0x1c
    CScnFilterListIter itAdj;   // 0x18
    CScnFilterListIter itTmp;   // 0x14
    CScnFilterListIter itEnd;   // 0x10
    CScnFilterListIter itCopy;  // 0xc
    CScnFilterListIter itEnd2;  // 0x8
    do {
        swapped = false;
        ScnFilterList_getBegin(&itCur, (CScnFilterList*)&self->field_08);
        while (ScnFilterList_getEnd(&itEnd2, (CScnFilterList*)&self->field_08),
               ScnFilterListIter_notEqual((u32*)&itCur, (u32*)&itEnd2)) {
            ScnFilterListIter_copy((int*)&itAdj, (int*)&itCur);
            ScnFilterListIter_copyAdvance(&itTmp, &itAdj, 0);
            ScnFilterList_getEnd(&itEnd, (CScnFilterList*)&self->field_08);
            // Adjacent iterator ran past the end: pass complete.
            if (ScnFilterListIter_equal(&itAdj, &itEnd)) {
                break;
            }
            u32 stateAdj = ScnFilter_getState(ScnFilterListIter_getItem(&itAdj));
            u32 stateCur = ScnFilter_getState(ScnFilterListIter_getItem(&itCur));
            if (stateCur > stateAdj) {
                // Swap the item payloads (full-word moves in retail).
                u32 valAdj = *(u32*)ScnFilterListIter_getItem(&itAdj);
                u32 valCur = *(u32*)ScnFilterListIter_getItem(&itCur);
                *(u32*)ScnFilterListIter_getItem(&itAdj) = valCur;
                *(u32*)ScnFilterListIter_getItem(&itCur) = valAdj;
                swapped = true;
            }
            ScnFilterListIter_copyAdvance(&itCopy, &itCur, 0);
        }
    } while (swapped);
}
#pragma pop

struct CScnFilterState {
    u8 _00[4];
    u8 state;
};

u8 ScnFilter_getState(void* self) {
    return ((CScnFilterState*)self)->state;
}

// reslist iterator equality: true when both iterators hold the same node.
extern "C" s32 ScnFilterListIter_equal(CScnFilterListIter* a, CScnFilterListIter* b) {
    return a->mNode == b->mNode;
}

// Pre-render callback: bail out when the filter list is empty, reset the GX
// cache state twice (retail re-fetches the instance between calls), notify
// each enabled filter via its vtable+0x10 slot, advancing as we walk.
extern "C" __declspec(noinline) void ScnFilterMan_preRender(CScnFilterMan* self) {
    if (ScnFilterList_size(&self->field_08) == 0) {
        return;
    }
    resetGXStateA__8CGXCacheFv(ScnFilterMan_getGXCache());
    func_8044A7F8__8CGXCacheFv(ScnFilterMan_getGXCache(), 1, 4, 5, 0, 1);
    func_8049D9D0(self);

    // Declaration order drives MWCC's reverse slot allocation (retail:
    // itOut=0x10, itTmp=0xc, itEnd=0x8).
    CScnFilterListIter itOut;
    CScnFilterListIter itTmp;
    CScnFilterListIter itEnd;

    ScnFilterList_getBegin(&itOut, (CScnFilterList*)&self->field_08);
    while (ScnFilterList_getEnd(&itEnd, (CScnFilterList*)&self->field_08),
           ScnFilterListIter_notEqual((u32*)&itOut, (u32*)&itEnd)) {
        if (ScnFilter_isRenderEnabled(*(CScnFilter**)ScnFilterListIter_getItem(&itOut))) {
            ((FMCb::Slot*)*(CScnFilter**)ScnFilterListIter_getItem(&itOut))->unk_10(self->mScene);
        }
        ScnFilterListIter_copyAdvance(&itTmp, &itOut, 0);
    }
}

// Retail refs the CDeviceGX static (extern; defined in retail data object)
// by its mangled name cacheInstance__9CDeviceGX via SDA21.
extern "C" { extern void* cacheInstance__9CDeviceGX; }
extern "C" __declspec(noinline) CGXCache* ScnFilterMan_getGXCache(void) { return (CGXCache*)cacheInstance__9CDeviceGX; }

// True when the filter's mFlags has bit 1 set (flag-test helper tail call).
extern "C" s32 ScnFilter_isRenderEnabled(CScnFilter* filter) {
    return testFlagMask(&filter->mFlags, 2);
}

// reslist size(): count the nodes from the first node to the end sentinel.
// The walk itself lives in ScnFilterList_countNodes (split out by the original source).
extern "C" u32 ScnFilterList_size(CScnFilterReslist* list) {
    u32 length = 0;
    CScnFilterListNode* curNode;
    CScnFilterListNode* endNode;
    endNode = ScnFilterList_getSentinel((CScnFilterList*)list);
    curNode = ScnFilterList_getFirst((CScnFilterList*)list);
    ScnFilterList_countNodes(&curNode, &endNode, &length);
    return length;
}

extern "C" u32 ScnFilterNode_notEqual(const u32* a, const u32* b);
extern "C" void ScnFilterNode_advance(u32* self);

extern "C" __declspec(noinline) void ScnFilterList_countNodes(CScnFilterListNode** cur, CScnFilterListNode** end, u32* out) {
    while (ScnFilterNode_notEqual((const u32*)cur, (const u32*)end) != 0) {
        ScnFilterNode_advance((u32*)cur);
        (*out)++;
    }
}

extern "C" __declspec(noinline) void ScnFilterNode_advance(u32* self) {
    *self = *(u32*)(*(u32**)self);
}

extern "C" __declspec(noinline) u32 ScnFilterNode_notEqual(const u32* a, const u32* b) {
    return *a != *b;
}

// Round-trip the first node's mNext through a stack slot via ScnFilterNode_store
// and return it (one more deref than ScnFilterList_getSentinel): the retail derefs the
// callee's preserved r3 (the local's address), not the stack slot.
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_getFirst(CScnFilterList* list) {
    CScnFilterListNode* node;
    return *(CScnFilterListNode**)ScnFilterNode_store(&node, (u32)list->mStartNodePtr->mNext);
}

extern "C" __declspec(noinline) void* ScnFilterNode_store(void* self, u32 val) {
    *(u32*)((u8*)self + 0x0) = val;
    return self;
}

// Getter that round-trips the list's sentinel pointer through the node
// setter: retail reads the pointer at self+4, stores it via ScnFilterNode_store
// into a local, and returns that local (deref of the callee's preserved r3).
extern "C" __declspec(noinline) CScnFilterListNode* ScnFilterList_getSentinel(CScnFilterList* list) {
    CScnFilterListNode* node;
    return *(CScnFilterListNode**)ScnFilterNode_store(&node, (u32)list->mStartNodePtr);
}

void CScnFilterMan::cbRenderBefore() {
    ScnFilterMan_preRender(this);
    CGXCache* cache = (CGXCache*)ScnFilterMan_getGXCache();
    cache->resetGXStateA();
    CViewRoot::updateViewRoot();
}

// clearList for the filter reslist: walk the ring from the first node,
// destroy each node's item (ScnFilterList_destroyItem) and reset its mNext
// (ScnFilterList_freeNode), then relink the sentinel onto itself (reslist.hpp
// clearList shape, retail ScnFilterList_clear).
extern "C" __declspec(noinline) void ScnFilterList_clear(CScnFilterReslist* self) {
    CScnFilterListNode* node = self->mStartNodePtr->mNext;
    while (node != self->mStartNodePtr) {
        CScnFilterListNode* cur = node;
        node = node->mNext;
        ScnFilterList_destroyItem(&cur->mItem);
        ScnFilterList_freeNode(self, cur);
    }
    self->mStartNodePtr->mNext = self->mStartNodePtr;
    self->mStartNodePtr->mPrev = self->mStartNodePtr;
}


// Iterator node setters (retail flat symbols): store the node pointer into
// the iterator. Defined here at the bottom of the file (after every caller)
// so MWCC's -inline auto cannot fold them into the tail-call wrappers above;
// noinline keeps the b/bl at each call site emitted (same pattern as the
// CGXCache resetGXStateA helpers).
extern "C" __declspec(noinline) void ScnFilterListIter_setNode(CScnFilterListIter* self, u32 val) {
    self->mNode = (CScnFilterListNode*)val;
}

extern "C" __declspec(noinline) void ScnFilterIterListIter_setNode(CScnFilterListIter* self, u32 val) {
    self->mNode = (CScnFilterListNode*)val;
}


// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace FMBlob {
extern "C" void __dt__13CScnFilterManFv();
extern "C" void cbRenderBefore__13CScnFilterManFv();
extern "C" void __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter();
extern "C" void __dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter();
extern "C" void __dt__reslist_CScnFilter();
extern "C" void __dt___reslist_base_CScnFilter();
}
extern "C" u32 lbl_eu_80663A40; extern "C" u32 lbl_eu_80663A48;
extern "C" u32 lbl_eu_80663A50; extern "C" u32 lbl_eu_80663A58;
extern "C" u32 lbl_eu_80663A60;

// [.data] 0x8056EB78-0x8056EBD0 (88B): 7 vtable sub-objects
extern "C" u32 lbl_eu_8056EB78[4] = {
    (u32)&lbl_eu_80663A40, 0x00000000,
    (u32)&FMBlob::__dt__13CScnFilterManFv, (u32)&FMBlob::cbRenderBefore__13CScnFilterManFv,
};
extern "C" u32 lbl_eu_8056EB88[3] = { (u32)&lbl_eu_80663A48, 0x00000000,
    (u32)&FMBlob::__dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter };
extern "C" u32 lbl_eu_8056EB94[3] = { (u32)&lbl_eu_80663A50, 0x00000000, 0x00000000 };
extern "C" u32 lbl_eu_8056EBA0[3] = { (u32)&lbl_eu_80663A50, 0x00000000,
    (u32)&FMBlob::__dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter };
extern "C" u32 lbl_eu_8056EBAC[3] = { (u32)&lbl_eu_80663A58, 0x00000000,
    (u32)&FMBlob::__dt__reslist_CScnFilter };
extern "C" u32 lbl_eu_8056EBB8[3] = { (u32)&lbl_eu_80663A60, 0x00000000, 0x00000000 };
extern "C" u32 lbl_eu_8056EBC4[3] = { (u32)&lbl_eu_80663A60, 0x00000000,
    (u32)&FMBlob::__dt___reslist_base_CScnFilter };

// [.rodata] 0x80524138-0x80524218 (224B) -- RTTI typeinfo name strings
extern "C" __declspec(align(4)) const char lbl_eu_80524138[0xE0] = {
    0x43,0x53,0x63,0x6E,0x46,0x69,0x6C,0x74,0x65,0x72,0x4D,0x61,0x6E,0x00,0x00,0x00,
    0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,
    0x5F,0x69,0x74,0x65,0x72,0x61,0x74,0x6F,0x72,0x3C,0x43,0x53,0x63,0x6E,0x46,0x69,
    0x6C,0x74,0x65,0x72,0x20,0x2A,0x2C,0x20,0x43,0x53,0x63,0x6E,0x46,0x69,0x6C,0x74,
    0x65,0x72,0x20,0x2A,0x26,0x2C,0x20,0x43,0x53,0x63,0x6E,0x46,0x69,0x6C,0x74,0x65,
    0x72,0x20,0x2A,0x2A,0x3E,0x3E,0x00,0x00,
    0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x5F,0x72,
    0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x69,0x74,0x65,0x72,0x61,0x74,0x6F,0x72,0x3C,
    0x43,0x53,0x63,0x6E,0x46,0x69,0x6C,0x74,0x65,0x72,0x20,0x2A,0x2C,0x20,0x43,0x53,
    0x63,0x6E,0x46,0x69,0x6C,0x74,0x65,0x72,0x20,0x2A,0x26,0x2C,0x20,0x43,0x53,0x63,
    0x6E,0x46,0x69,0x6C,0x74,0x65,0x72,0x20,0x2A,0x2A,0x3E,0x3E,0x00,0x00,0x00,0x00,
    0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x43,0x53,0x63,0x6E,0x46,0x69,0x6C,0x74,
    0x65,0x72,0x20,0x2A,0x3E,0x00,0x00,0x00,
    0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x43,0x53,
    0x63,0x6E,0x46,0x69,0x6C,0x74,0x65,0x72,0x20,0x2A,0x3E,0x00,0x00,0x00,0x00,
};

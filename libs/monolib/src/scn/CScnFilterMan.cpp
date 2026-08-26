// Auto-scaffolded catalog TU for monolib/src/scn/CScnFilterMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnFilterMan.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CGXCache.hpp"
#include "monolib/core/CViewRoot.hpp"

// Setter helpers are declared here but defined at the BOTTOM of this file
// (after every caller) so MWCC's -inline auto cannot fold them into the
// tail-call wrappers below; retail keeps them as separate functions (same
// pattern as CGXCache.cpp func_8044BE38).
extern "C" void func_8049D274(CScnFilterListIter* self, u32 val);
extern "C" void func_8049D64C(CScnFilterListIter* self, u32 val);
extern "C" void func_8049CD34(CScnFilterIteratorReslist* self);
extern "C" void func_8049CDAC(CScnFilterIterator* item);
extern "C" void func_8049CDB0(void* self, void* out);
void func_8049D218(CScnFilterReslist* list, u32* val);
void* func_8049DD28(void* self, u32 val);
extern "C" void func_8049D994(int* dst, int* src);
extern "C" void func_8049D9A0(u32* self);
extern "C" void func_8049CAF4(CScnFilterReslist* self);
extern "C" __declspec(noinline) CScnFilterListNode* func_8049DD30(CScnFilterList* list);
extern "C" __declspec(noinline) CScnFilterListNode* func_8049DCF4(CScnFilterList* list);
extern "C" void func_8049DC5C(CScnFilterListNode** cur, CScnFilterListNode** end, u32* out);

// CGXCache helpers reached by their flat retail symbols (the member decls in
// CGXCache.hpp are zero-param; func_8044A7F8 actually lives on r4-r8).
extern "C" void func_8044BE38__8CGXCacheFv(CGXCache* cache);
extern "C" void func_8044A7F8__8CGXCacheFv(CGXCache* cache, u32 w0, u32 w1, u32 w2,
                                           u32 w3, int flag);
extern "C" u32 func_8049DC04(CScnFilterReslist* list);
extern "C" CGXCache* func_8049DBF0(void);
extern "C" s32 func_8049DBF8(CScnFilter* filter);
extern "C" void func_8049D9D0(CScnFilterMan* self);

// Flattened template ctors defined below; forward-declared so the derived
// ctors above them emit bl calls instead of inlining the base init.
void __ct___reslist_base_CScnFilter(CScnFilterReslist* obj);
extern "C" void* __ct__8049CC10(CScnFilterReslist* obj);
extern "C" void* __dt___reslist_base_CScnFilter(CScnFilterReslist* self, int mode);
extern "C" __declspec(noinline) CScnFilterListNode* func_8049D394(CScnFilterReslist* list);
extern "C" __declspec(noinline) void func_8049D338(CScnFilter** item, u32 val);
extern "C" __declspec(noinline) CScnFilterListNode* func_8049D76C(CScnFilterReslist* list);
extern "C" __declspec(noinline) void func_8049D710(CScnFilter** item, u32 val);

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
extern "C" CScnFilterMan* func_8049CC70(CScnFilterMan* self);
extern "C" __declspec(noinline) void* __ct__8049CC10(CScnFilterReslist* obj) {
    obj->mVtable = (u32*)lbl_eu_8056EBA0;
    func_8049CC70((CScnFilterMan*)((char*)obj + 8));
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
        func_8049CAF4(self);
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
extern "C" __declspec(noinline) void func_8049CB6C(CScnFilter** item) {}

#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void func_8049C9F8(void* self) { *(u32*)self = 0; }
#pragma pop


// reset a node's mNext after removal (no-op-ish free marker). noinline keeps
// the call emitted at the clearList/erase call sites.
extern "C" __declspec(noinline) void func_8049CB70(void* self, void* out) { *(u32*)out = 0; }


// Guard the no-op stub so the call sites survive (MWCC_CASES
// empty-stub pattern: -ipa file inlines the empty body and drops the bl).
#pragma push
#pragma auto_inline off
extern "C" void func_8049CCA4(CScnFilterReslist* list) {}
#pragma pop

// (re)initialise the filter list member at self+8 and return self. The
// callee func_8049CCA4 is a no-op in retail; the call is still emitted
// (retail ctor-style helper pattern).
extern "C" CScnFilterMan* func_8049CC70(CScnFilterMan* self) {
    func_8049CCA4(&self->field_08);
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
        func_8049CD34(self);
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
// destroy each node's item (func_8049CDAC) and reset its mNext
// (func_8049CDB0), then relink the sentinel onto itself. Same shape as
// func_8049CAF4 (reslist.hpp clearList).
#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void func_8049CD34(CScnFilterIteratorReslist* self) {
    CScnFilterIterNode* node = self->mStartNodePtr->mNext;
    while (node != self->mStartNodePtr) {
        CScnFilterIterNode* cur = node;
        node = node->mNext;
        func_8049CDAC(&cur->mItem);
        func_8049CDB0(self, cur);
    }
    self->mStartNodePtr->mNext = self->mStartNodePtr;
    self->mStartNodePtr->mPrev = self->mStartNodePtr;
}
#pragma pop

// destroy an iterator item (no-op in retail). Flat symbol + noinline so the
// bl at the clearList call site stays emitted.
extern "C" __declspec(noinline) void func_8049CDAC(CScnFilterIterator* item) {}

extern "C" __declspec(noinline) void func_8049CDB0(void* unused, void* out) { *(u32*)out = 0; }

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

extern "C" void func_8049CE18(void* list, void* val, int prio);
#pragma push
#pragma auto_inline off
extern "C" void func_8049CE14(void* list, void* val, int prio) { func_8049CE18(list, val, prio); }
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_8049CE18(void* list, void* val, int prio){
}
#pragma pop

extern "C" void func_8049CF48(void* list, void* val, int prio);
#pragma push
#pragma auto_inline off
extern "C" void func_8049CF44(void* list, void* val, int prio) { func_8049CF48(list, val, prio); }
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void func_8049CF48(void* list, void* val, int prio){
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
extern "C" void func_8049D12C(void* self);
extern "C" void func_8049D188(CScnFilterIteratorReslist* self);

// flattened init function (retail __ct__CScnFilterMan): base IScnRender
// init, vptr install, both reslists constructed, flags cleared, seed lists,
// register with the scene render-callback chain.
extern "C" void __ct__IScnRender(void* self);
extern "C" void func_800B0A90(void* self);
extern "C" void* func_80496018(CScn* scene);
extern "C" u32 lbl_eu_8056EB78[4];

extern "C" CScnFilterMan* __ct__CScnFilterMan(CScnFilterMan* self, CScn* scene) {
    __ct__IScnRender(self);
    self->mScene = scene;
    *(void**)self = (void*)lbl_eu_8056EB78;
    __ct__reslist_CScnFilter(&self->field_08);
    __ct__8049CBD4((CScnFilterReslist*)&self->field_28);
    func_8049C9F8(&self->field_48);
    func_800B0A90(&self->field_48);
    func_8049CE14(&self->field_08, func_80496018(scene), 4);
    func_8049CF44(&self->field_28, func_80496018(scene), 4);
    scene->addRenderCB((IScnRender*)self, 3, 0);
    return self;
}

CScnFilterMan::~CScnFilterMan() {
    *(void**)this = (void*)lbl_eu_8056EB78;
    mScene->removeRenderCB((IScnRender*)this);
    func_8049D12C(&field_08);
    func_8049D188(&field_28);
    __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter((CScnFilterListIter*)&field_28, -1);
    FMDtorCalls::__dt__reslist_CScnFilter(&field_08, -1);
    __dt__10IScnRenderFv(this, 0);
}

extern "C" void __dt__8049D130(CScnFilterReslist* self);
#pragma push
#pragma auto_inline off
extern "C" void func_8049D12C(void* self) { __dt__8049D130((CScnFilterReslist*)self); }
#pragma pop

// destroyList for the filter reslist member: clear the ring, free the node
// array unless it is externally owned (field_0x1C), and reset the capacity.
extern "C" __declspec(noinline) void __dt__8049D130(CScnFilterReslist* self) {
    func_8049CAF4(self);
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
extern "C" void func_8049D188(CScnFilterIteratorReslist* self) { __dt__8049D18C(self); }
#pragma pop

extern "C" void __dla__FPv(void* p);

// reslist-base dtor: clear the node array and ownership flag. The mList
// backing block (allocated 16 bytes before the array) is freed when the
// ownership flag is clear and the array is non-null.
#pragma push
#pragma auto_inline off
extern "C" void __dt__8049D18C(CScnFilterIteratorReslist* self) {
    func_8049CD34(self);
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

// Forward (self, val) to func_8049D218 as (reslist member at self+8, &val).
void func_8049D1EC(CScnFilterMan* self, u32 val) {
    func_8049D218(&self->field_08, &val);
}

// iter = list end sentinel (retail: lwz r4,0x4(r4); b func_8049D274)
extern "C" __declspec(noinline) void func_8049D26C(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D274(self, (u32)list->mStartNodePtr);
}

extern "C" __declspec(noinline) CScnFilterListNode* func_8049D2F0(
    CScnFilterReslist* list, u32 val);

// Allocate a node for `val`, link it into the list before `iter`'s node, and
// point the output iterator at it.
extern "C" __declspec(noinline) void func_8049D27C(CScnFilterListIter* self, CScnFilterReslist* list,
                   CScnFilterListIter* iter, u32* val) {
    CScnFilterListNode* node = func_8049D2F0(list, (u32)val);
    node->mNext = iter->mNode;
    node->mPrev = iter->mNode->mPrev;
    iter->mNode->mPrev->mNext = node;
    iter->mNode->mPrev = node;
    func_8049D274(self, (u32)node);
}

// Insert `val` at the end of the filter list (retail: end-iterator then
// insert via the D26C/D27C pair).
__declspec(noinline) void func_8049D218(CScnFilterReslist* list, u32* val) {
    CScnFilterListIter iter2;
    CScnFilterListIter iter;
    func_8049D26C(&iter, (CScnFilterList*)list);
    func_8049D27C(&iter2, list, &iter, val);
}

// Get a free node slot from the node array and install `val` into its item
// slot, returning the node (retail: free-slot search then item setter).
extern "C" __declspec(noinline) CScnFilterListNode* func_8049D2F0(
    CScnFilterReslist* list, u32 val) {
    CScnFilterListNode* node = func_8049D394(list);
    func_8049D338(&node->mItem, val);
    return node;
}

// setItem for the filter reslist: construct a CScnFilter* cell at *item via
// the placement-new operator (func_804932B4), then copy *val into it. The
// try/catch is the retail setItem shape (forces MWCC's frame-pointer prologue).
extern "C" __declspec(noinline) void func_8049D338(CScnFilter** item, u32 val) {
    CScnFilter** p = (CScnFilter**)func_804932B4((void*)4, (void*)item);
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
extern "C" __declspec(noinline) CScnFilterListNode* func_8049D394(CScnFilterReslist* list) {
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
extern "C" void func_8049D26C(CScnFilterListIter* self, CScnFilterList* list);
extern "C" void func_8049D53C(CScnFilterListIter* self, CScnFilterList* list);
extern "C" void func_8049D490(int* out, CScnFilterListIter* first, u32* last, u32* value);
extern "C" u32 func_8049D548(u32* a, u32* b);
extern "C" int* func_8049D564(int* dst, int* src);
extern "C" void func_8049D570(CScnFilterListIter* self, CScnFilterReslist* list, CScnFilterListIter* it);
extern "C" void func_8049D5F0(CScnFilterReslist* list, u32* val);
void func_8049D3D8(CScnFilterMan* man, u32 val) {
    // MWCC assigns frame slots in reverse declaration order; this order
    // reproduces the retail layout (found@0x20, begin@0x1c, end@0x18,
    // end2@0x14, erase@0x10, tmp@0xc).
    CScnFilterListIter itFound;
    CScnFilterListIter itBegin;
    CScnFilterListIter itEnd;
    CScnFilterListIter itEnd2;
    CScnFilterListIter itErase;
    u32 tmpVal;

    func_8049D26C(&itEnd, (CScnFilterList*)&man->field_08);
    func_8049D53C(&itBegin, (CScnFilterList*)&man->field_08);
    func_8049D490((int*)&itFound, &itBegin, (u32*)&itEnd, &val);
    func_8049D26C(&itEnd2, (CScnFilterList*)&man->field_08);
    if (func_8049D548((u32*)&itFound, (u32*)&itEnd2) != 0) {
        if (func_8004B3D8(&man->field_48, 1) == 0) {
            func_8049D570(&itErase, &man->field_08,
                (CScnFilterListIter*)func_8049D564((int*)&tmpVal, (int*)&itFound));
        } else {
            func_8049D5F0((CScnFilterReslist*)&man->field_28, (u32*)&itFound);
        }
    }
}

// noinline keeps the bl from func_8049D914's advance out-of-line (retail
// emits the call; -ipa file would inline this small body and drop it).
extern "C" __declspec(noinline) void func_8049D520(u32* self) { *self = *(u32*)(*(u32**)self); }

extern "C" __declspec(noinline) void* func_8049D530(void* self){ return (void*)((char*)*(void**)self + 8); }

// iter = first node (retail: lwz r4,0x4(r4); lwz r4,0x0(r4); b func_8049D274)
extern "C" __declspec(noinline) void func_8049D53C(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D274(self, (u32)list->mStartNodePtr->mNext);
}

extern "C" __declspec(noinline) u32 func_8049D548(u32* a, u32* b) { return *a != *b; }


extern "C" __declspec(noinline) int* func_8049D564(int* dst, int* src){
    *dst = *src;
    return dst;   // retail callers chain the returned dst pointer
}

// Advance `first` until it hits `last` or its item equals *value, then
// copy the resulting iterator into *out (retail performs the copy on every
// exit path, including running off the end).
void func_8049D490(int* out, CScnFilterListIter* first, u32* last, u32* value) {
    while (func_8049D548((u32*)first, last) &&
           *(CScnFilter**)func_8049D530(first) != *(CScnFilter**)value) {
        func_8049D520((u32*)first);
    }
    func_8049D564(out, (int*)first);
}

// Remove `it`'s node from the ring (unlink prev/next), destroy its item and
// free the node, then set self->mNode to the next node (retail
// func_8049D570; node read back through it->mNode at each step).
extern "C" __declspec(noinline) void func_8049D570(CScnFilterListIter* self, CScnFilterReslist* list, CScnFilterListIter* it) {
    CScnFilterListNode* node = it->mNode;
    CScnFilterListNode* prev = node->mPrev;
    CScnFilterListNode* next = node->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    func_8049CB6C(&it->mNode->mItem);
    func_8049CB70(list, it->mNode);
    func_8049D274(self, (u32)next);
}

// iter = list end sentinel via the D64C setter
extern "C" __declspec(noinline) void func_8049D644(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D64C(self, (u32)list->mStartNodePtr);
}

extern "C" __declspec(noinline) CScnFilterListNode* func_8049D6C8(
    CScnFilterReslist* list, u32 val);
extern "C" void func_8049D64C(CScnFilterListIter* self, u32 val);

// Allocate a node for `val`, link it into the list before `iter`'s node, and
// point the output iterator at it (same shape as func_8049D27C).
extern "C" __declspec(noinline) void func_8049D654(CScnFilterListIter* self, CScnFilterReslist* list,
                   CScnFilterListIter* iter, u32* val) {
    CScnFilterListNode* node = func_8049D6C8(list, (u32)val);
    node->mNext = iter->mNode;
    node->mPrev = iter->mNode->mPrev;
    iter->mNode->mPrev->mNext = node;
    iter->mNode->mPrev = node;
    func_8049D64C(self, (u32)node);
}

// Insert `val` at the end of the filter list (retail: end-iterator then
// insert via the D644/D654 pair; same shape as func_8049D218).
extern "C" void func_8049D5F0(CScnFilterReslist* list, u32* val) {
    CScnFilterListIter iter2;
    CScnFilterListIter iter;
    func_8049D644(&iter, (CScnFilterList*)list);
    func_8049D654(&iter2, list, &iter, val);
}

// Get a free node slot from the node array and install `val` into its item
// slot, returning the node (retail: free-slot search then item setter).
CScnFilterListNode* func_8049D6C8(CScnFilterReslist* list, u32 val) {
    CScnFilterListNode* node = func_8049D76C(list);
    func_8049D710(&node->mItem, val);
    return node;
}

// setItem for the second filter list: same placement-new shape as
// func_8049D338, but the value copy goes through func_8049D564.
extern "C" __declspec(noinline) void func_8049D710(CScnFilter** item, u32 val) {
    CScnFilter** p = (CScnFilter**)func_804932B4((void*)4, (void*)item);
    if (p != NULL) {
        try {
            func_8049D564((int*)p, (int*)val);
        } catch (...) {
            throw;
        }
    }
}

// Same free-slot search as func_8049D394 (retail bodies are identical).
extern "C" __declspec(noinline) CScnFilterListNode* func_8049D76C(CScnFilterReslist* list) {
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
    // declared virtual lands at vtable+0x10 (the slot func_8049DB14 calls).
    virtual void unk_10(CScn* scene);
};
} // namespace FMCb
extern "C" void func_8004B694(u32* self, u32 val);
extern "C" void func_8049D8D4(u32* self, u32 flags);
extern "C" void* func_8049D954(void* self);
extern "C" __declspec(noinline) void func_8049D914(CScnFilterListIter* dst, CScnFilterListIter* src, int unused);
extern "C" __declspec(noinline) void func_8049D960(CScnFilterListIter* dst, CScnFilterListIter* src, int unused);
extern "C" u32 func_8049D9B0(const u32* a, const u32* b);
extern "C" void func_8049D9CC(CScnFilterIteratorReslist* self);
extern "C" s32 func_8049D8E4(CScnFilter* filter);
extern "C" void func_8049D948(CScnFilterListIter* self, CScnFilterList* list);

void CScnFilterMan::update() {
    func_8049D8D4(&field_48, 1);
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

    func_8049D53C(&itA, (CScnFilterList*)&field_08);
    while (func_8049D26C(&itEnd, (CScnFilterList*)&field_08),
           func_8049D548((u32*)&itA, (u32*)&itEnd)) {
        if (func_8049D8E4(*(CScnFilter**)func_8049D530(&itA)) == 0) {
            ((FMCb::Slot*)*(CScnFilter**)func_8049D530(&itA))->unk_08(mScene);
        }
        func_8049D914(&itTmp, &itA, 0);
    }
    func_8004B694(&field_48, 1);

    func_8049D948(&it2, (CScnFilterList*)&field_28);
    while (func_8049D644(&itEnd2, (CScnFilterList*)&field_28),
           func_8049D9B0((u32*)&it2, (u32*)&itEnd2)) {
        CScnFilterListIter* pNext =
            (CScnFilterListIter*)func_8049D564((int*)&tmpVal, (int*)(u32*)func_8049D954(&it2));
        func_8049D570(&itOut, &field_08, pNext);
        func_8049D960(&itAdv, &it2, 0);
    }
    func_8049D9CC(&field_28);
}

extern "C" __declspec(noinline) void func_8049D8D4(u32* self, u32 flags) { *self |= flags; }

// True when the filter's mFlags bit 0 is clear (retail: flag-test helper
// call, result inverted via the cntlzw/srwi "== 0" idiom).
extern "C" __declspec(noinline) s32 func_8049D8E4(CScnFilter* filter) {
    return func_8004B3D8(&filter->mFlags, 1) == 0;
}

// Copy src into dst, then advance src by one node (retail: *dst = *src via
// func_8049D564, then src->mNode = src->mNode->mNext via func_8049D520).
extern "C" __declspec(noinline) void func_8049D914(CScnFilterListIter* dst, CScnFilterListIter* src, int unused) {
    func_8049D564((int*)dst, (int*)src);
    func_8049D520((u32*)src);
}

// iter = first node via the D64C setter
extern "C" __declspec(noinline) void func_8049D948(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D64C(self, (u32)list->mStartNodePtr->mNext);
}

extern "C" __declspec(noinline) void* func_8049D954(void* self){ return (void*)((char*)*(void**)self + 8); }

// Copy src into dst, then advance src by one node (func_8049D994 copy +
// func_8049D9A0 advance pair).
extern "C" __declspec(noinline) void func_8049D960(CScnFilterListIter* dst, CScnFilterListIter* src, int unused) {
    func_8049D994((int*)dst, (int*)src);
    func_8049D9A0((u32*)src);
}

// noinline keeps the bl calls from func_8049D960 out-of-line (retail emits
// the calls; -ipa file would inline these small bodies and drop them).
extern "C" __declspec(noinline) void func_8049D994(int* dst, int* src){
    *dst = *src;
}

extern "C" __declspec(noinline) void func_8049D9A0(u32* self) { *self = *(u32*)(*(u32**)self); }

// != on the u32 values at the two pointers (MWCC dual-subf idiom).
extern "C" __declspec(noinline) u32 func_8049D9B0(const u32* a, const u32* b) { return *a != *b; }

extern "C" void func_8049CD34(CScnFilterIteratorReslist* self);
extern "C" __declspec(noinline) void func_8049D9CC(CScnFilterIteratorReslist* self) { func_8049CD34(self); }

// Guarded so -ipa cannot fold the stub body into func_8049DB14's call site.
// Retail name is zero-param mangled although r3 (self) is live.
#pragma push
#pragma auto_inline off
extern "C" void func_8049D9D0(CScnFilterMan* self) {}
#pragma pop

struct CScnFilterState {
    u8 _00[4];
    u8 state;
};

u8 func_8049DAF4(void* self) {
    return ((CScnFilterState*)self)->state;
}

// reslist iterator equality: true when both iterators hold the same node.
s32 func_8049DAFC(CScnFilterListIter* a, CScnFilterListIter* b) {
    return a->mNode == b->mNode;
}

// Pre-render callback: bail out when the filter list is empty, reset the GX
// cache state twice (retail re-fetches the instance between calls), notify
// each enabled filter via its vtable+0x10 slot, advancing as we walk.
__declspec(noinline) void func_8049DB14(CScnFilterMan* self) {
    if (func_8049DC04(&self->field_08) == 0) {
        return;
    }
    func_8044BE38__8CGXCacheFv(func_8049DBF0());
    func_8044A7F8__8CGXCacheFv(func_8049DBF0(), 1, 4, 5, 0, 1);
    func_8049D9D0(self);

    // Declaration order drives MWCC's reverse slot allocation (retail:
    // itOut=0x10, itTmp=0xc, itEnd=0x8).
    CScnFilterListIter itOut;
    CScnFilterListIter itTmp;
    CScnFilterListIter itEnd;

    func_8049D53C(&itOut, (CScnFilterList*)&self->field_08);
    while (func_8049D26C(&itEnd, (CScnFilterList*)&self->field_08),
           func_8049D548((u32*)&itOut, (u32*)&itEnd)) {
        if (func_8049DBF8(*(CScnFilter**)func_8049D530(&itOut))) {
            ((FMCb::Slot*)*(CScnFilter**)func_8049D530(&itOut))->unk_10(self->mScene);
        }
        func_8049D914(&itTmp, &itOut, 0);
    }
}

// Retail refs the CDeviceGX static (extern; defined in retail data object)
// by its mangled name cacheInstance__9CDeviceGX via SDA21.
extern "C" { extern void* cacheInstance__9CDeviceGX; }
extern "C" __declspec(noinline) CGXCache* func_8049DBF0(void) { return (CGXCache*)cacheInstance__9CDeviceGX; }

// True when the filter's mFlags has bit 1 set (flag-test helper tail call).
extern "C" s32 func_8049DBF8(CScnFilter* filter) {
    return func_8004B3D8(&filter->mFlags, 2);
}

// reslist size(): count the nodes from the first node to the end sentinel.
// The walk itself lives in func_8049DC5C (split out by the original source).
extern "C" u32 func_8049DC04(CScnFilterReslist* list) {
    u32 length = 0;
    CScnFilterListNode* curNode;
    CScnFilterListNode* endNode;
    endNode = func_8049DD30((CScnFilterList*)list);
    curNode = func_8049DCF4((CScnFilterList*)list);
    func_8049DC5C(&curNode, &endNode, &length);
    return length;
}

extern "C" u32 func_8049DCD8(const u32* a, const u32* b);
extern "C" void func_8049DCC8(u32* self);

extern "C" __declspec(noinline) void func_8049DC5C(CScnFilterListNode** cur, CScnFilterListNode** end, u32* out) {
    while (func_8049DCD8((const u32*)cur, (const u32*)end) != 0) {
        func_8049DCC8((u32*)cur);
        (*out)++;
    }
}

extern "C" __declspec(noinline) void func_8049DCC8(u32* self) {
    *self = *(u32*)(*(u32**)self);
}

extern "C" __declspec(noinline) u32 func_8049DCD8(const u32* a, const u32* b) {
    return *a != *b;
}

// Round-trip the first node's mNext through a stack slot via func_8049DD28
// and return it (one more deref than func_8049DD30): the retail derefs the
// callee's preserved r3 (the local's address), not the stack slot.
extern "C" __declspec(noinline) CScnFilterListNode* func_8049DCF4(CScnFilterList* list) {
    CScnFilterListNode* node;
    return *(CScnFilterListNode**)func_8049DD28(&node, (u32)list->mStartNodePtr->mNext);
}

__declspec(noinline) void* func_8049DD28(void* self, u32 val) {
    *(u32*)((u8*)self + 0x0) = val;
    return self;
}

// Getter that round-trips the list's sentinel pointer through the node
// setter: retail reads the pointer at self+4, stores it via func_8049DD28
// into a local, and returns that local (deref of the callee's preserved r3).
extern "C" __declspec(noinline) CScnFilterListNode* func_8049DD30(CScnFilterList* list) {
    CScnFilterListNode* node;
    return *(CScnFilterListNode**)func_8049DD28(&node, (u32)list->mStartNodePtr);
}

void CScnFilterMan::cbRenderBefore() {
    func_8049DB14(this);
    CGXCache* cache = (CGXCache*)func_8049DBF0();
    cache->func_8044BE38();
    CViewRoot::func_80442DA8();
}

// clearList for the filter reslist: walk the ring from the first node,
// destroy each node's item (func_8049CB6C) and reset its mNext
// (func_8049CB70), then relink the sentinel onto itself (reslist.hpp
// clearList shape, retail func_8049CAF4).
extern "C" __declspec(noinline) void func_8049CAF4(CScnFilterReslist* self) {
    CScnFilterListNode* node = self->mStartNodePtr->mNext;
    while (node != self->mStartNodePtr) {
        CScnFilterListNode* cur = node;
        node = node->mNext;
        func_8049CB6C(&cur->mItem);
        func_8049CB70(self, cur);
    }
    self->mStartNodePtr->mNext = self->mStartNodePtr;
    self->mStartNodePtr->mPrev = self->mStartNodePtr;
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

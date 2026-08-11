// Auto-scaffolded catalog TU for monolib/src/scn/CScnFilterMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnFilterMan.hpp"
#include "monolib/device/CGXCache.hpp"
#include "monolib/core/CViewRoot.hpp"

// Setter helpers are declared here but defined at the BOTTOM of this file
// (after every caller) so MWCC's -inline auto cannot fold them into the
// tail-call wrappers below; retail keeps them as separate functions (same
// pattern as CGXCache.cpp func_8044BE38).
extern "C" void func_8049D274(CScnFilterListIter* self, u32 val);
extern "C" void func_8049D64C(CScnFilterListIter* self, u32 val);
void func_8049D218(CScnFilterReslist* list, u32* val);
void func_8049DD28(void* self, u32 val);
extern "C" void func_8049D994(int* dst, int* src);
extern "C" void func_8049D9A0(u32* self);

// Flattened template ctors defined below; forward-declared so the derived
// ctors above them emit bl calls instead of inlining the base init.
void __ct___reslist_base_CScnFilter(CScnFilterReslist* obj);
void __ct__8049CC10(CScnFilterReslist* obj);
extern "C" void __dt___reslist_base_CScnFilter(CScnFilterReslist* self, int mode);
extern "C" __declspec(noinline) CScnFilterListNode* func_8049D394(CScnFilterReslist* list);
extern "C" __declspec(noinline) void func_8049D338(CScnFilter** item, u32 val);
extern "C" __declspec(noinline) CScnFilterListNode* func_8049D76C(CScnFilterReslist* list);
extern "C" __declspec(noinline) void func_8049D710(CScnFilter** item, u32 val);

// reslist<CScnFilter*> ctor (retail __ct__reslist_CScnFilter): base init,
// then install the derived vtable. The flattened retail name cannot be
// produced by a template instantiation under Wii/1.1 (symbolic mangling),
// so it is spelled out (MWCC_REFERENCE "flattened constructor" pattern).
CScnFilterReslist* __ct__reslist_CScnFilter(CScnFilterReslist* obj) {
    __ct___reslist_base_CScnFilter(obj);
    obj->mVtable = lbl_eu_8056EBAC;
    return obj;
}

// Unknown-class ctor (retail __ct__8049CBD4): init via __ct__8049CC10, then
// install the derived vtable. The object has a reslist-shaped layout
// (vtable@0, sentinel@8, mList@0x14, mCapacity@0x18, flag@0x1C).
CScnFilterReslist* __ct__8049CBD4(CScnFilterReslist* obj) {
    __ct__8049CC10(obj);
    obj->mVtable = lbl_eu_8056EB88;
    return obj;
}

// _reslist_base<CScnFilter*> ctor (retail __ct___reslist_base_CScnFilter):
// zeroes the storage fields and links the sentinel node onto itself. The
// flattened retail name cannot be produced by a template instantiation under
// Wii/1.1 (symbolic mangling), so it is spelled out (MWCC_REFERENCE
// "flattened constructor" pattern, cf. CVirtualLightAmb).
__declspec(noinline) void __ct___reslist_base_CScnFilter(CScnFilterReslist* obj) {
    obj->mVtable = lbl_eu_8056EBC4;
    CScnFilterListNode* head = &obj->mStartNode;
    obj->mList = nullptr;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = head;
    head->mNext = head;
    head->mPrev = head;
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


void func_8049CB6C(void) {}

void func_8049C9F8(void){}


void func_8049CB70(void){}


extern "C" void func_8049CCA4(CScnFilterReslist* list) {}

// (re)initialise the filter list member at self+8 and return self. The
// callee func_8049CCA4 is a no-op in retail; the call is still emitted
// (retail ctor-style helper pattern).
CScnFilterMan* func_8049CC70(CScnFilterMan* self) {
    func_8049CCA4(&self->field_0x08);
    return self;
}

void __dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter(){}

#pragma push
#pragma auto_inline off
extern "C" void func_8049CD34(){}
#pragma pop

void func_8049CDAC(void) {}

extern "C" void func_8049CDB0(void* unused, void* out) { *(u32*)out = 0; }

void __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter(){}

extern "C" void func_8049CE18();
extern "C" void func_8049CE14(void) { func_8049CE18(); }

#pragma push
#pragma auto_inline off
extern "C" void func_8049CE18(){}
#pragma pop

extern "C" void func_8049CF48();
extern "C" void func_8049CF44(void) { func_8049CF48(); }

#pragma push
#pragma auto_inline off
extern "C" void func_8049CF48(){}
#pragma pop

CScnFilterMan::~CScnFilterMan() {}

extern "C" void __dt__8049D130();
extern "C" void func_8049D12C(void) { __dt__8049D130(); }

#pragma push
#pragma auto_inline off
extern "C" void __dt__8049D130(){}
#pragma pop

extern "C" void __dt__8049D18C();
extern "C" void func_8049D188(void) { __dt__8049D18C(); }

#pragma push
#pragma auto_inline off
extern "C" void __dt__8049D18C(){}
#pragma pop

// Forward (self, val) to func_8049D218 as (reslist member at self+8, &val).
void func_8049D1EC(CScnFilterMan* self, u32 val) {
    func_8049D218(&self->field_0x08, &val);
}

// iter = list end sentinel (retail: lwz r4,0x4(r4); b func_8049D274)
extern "C" __declspec(noinline) void func_8049D26C(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D274(self, (u32)list->mStartNodePtr);
}

extern "C" __declspec(noinline) void func_8049D27C(CScnFilterListIter* self, CScnFilterReslist* list,
                   CScnFilterListIter* iter, u32* val) {}

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
CScnFilterListNode* func_8049D2F0(CScnFilterReslist* list, u32 val) {
    CScnFilterListNode* node = func_8049D394(list);
    func_8049D338(&node->mItem, val);
    return node;
}

extern "C" __declspec(noinline) void func_8049D338(CScnFilter** item, u32 val) {}

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

void func_8049D3D8(){}

void func_8049D490(){}

extern "C" void func_8049D520(u32* self) { *self = *(u32*)(*(u32**)self); }

extern "C" void* func_8049D530(void* self){ return (void*)((char*)*(void**)self + 8); }

// iter = first node (retail: lwz r4,0x4(r4); lwz r4,0x0(r4); b func_8049D274)
void func_8049D53C(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D274(self, (u32)list->mStartNodePtr->mNext);
}

void func_8049D548(){}

extern "C" void func_8049D564(int* dst, int* src){
    *dst = *src;
}

void func_8049D570(){}

// iter = list end sentinel via the D64C setter
extern "C" __declspec(noinline) void func_8049D644(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D64C(self, (u32)list->mStartNodePtr);
}

extern "C" __declspec(noinline) void func_8049D654(CScnFilterListIter* self, CScnFilterReslist* list,
                   CScnFilterListIter* iter, u32* val) {}

// Insert `val` at the end of the filter list (retail: end-iterator then
// insert via the D644/D654 pair; same shape as func_8049D218).
void func_8049D5F0(CScnFilterReslist* list, u32* val) {
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

extern "C" __declspec(noinline) void func_8049D710(CScnFilter** item, u32 val) {}

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

void CScnFilterMan::update() {}

extern "C" void func_8049D8D4(u32* self, u32 flags) { *self |= flags; }

// True when the filter's mFlags bit 0 is clear (retail: flag-test helper
// call, result inverted via the cntlzw/srwi "== 0" idiom).
s32 func_8049D8E4(CScnFilter* filter) {
    return func_8004B3D8(&filter->mFlags, 1) == 0;
}

// Copy src into dst, then advance src by one node (retail: *dst = *src via
// func_8049D564, then src->mNode = src->mNode->mNext via func_8049D520).
void func_8049D914(CScnFilterListIter* dst, CScnFilterListIter* src) {
    func_8049D564((int*)dst, (int*)src);
    func_8049D520((u32*)src);
}

// iter = first node via the D64C setter
void func_8049D948(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D64C(self, (u32)list->mStartNodePtr->mNext);
}

extern "C" void* func_8049D954(void* self){ return (void*)((char*)*(void**)self + 8); }

// Copy src into dst, then advance src by one node (func_8049D994 copy +
// func_8049D9A0 advance pair).
void func_8049D960(CScnFilterListIter* dst, CScnFilterListIter* src) {
    func_8049D994((int*)dst, (int*)src);
    func_8049D9A0((u32*)src);
}

extern "C" void func_8049D994(int* dst, int* src){
    *dst = *src;
}

extern "C" void func_8049D9A0(u32* self) { *self = *(u32*)(*(u32**)self); }

// != on the u32 values at the two pointers (MWCC dual-subf idiom).
__declspec(noinline) u32 func_8049D9B0(u32* a, u32* b) { return *a != *b; }

extern "C" void func_8049CD34();
extern "C" void func_8049D9CC(void) { func_8049CD34(); }

void func_8049D9D0(){}

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

__declspec(noinline) void func_8049DB14(CScnFilterMan* self) {}

// Retail refs the CDeviceGX static (extern; defined in retail data object)
// by its mangled name cacheInstance__9CDeviceGX via SDA21.
extern "C" { extern void* cacheInstance__9CDeviceGX; }
__declspec(noinline) CGXCache* func_8049DBF0(void) { return (CGXCache*)cacheInstance__9CDeviceGX; }

// True when the filter's mFlags has bit 1 set (flag-test helper tail call).
s32 func_8049DBF8(CScnFilter* filter) {
    return func_8004B3D8(&filter->mFlags, 2);
}

void func_8049DC04(){}

void func_8049DC5C(){}

extern "C" void func_8049DCC8(u32* self) { *self = *(u32*)(*(u32**)self); }

__declspec(noinline) u32 func_8049DCD8(u32* a, u32* b) { return *a != *b; }

// Round-trip the first node's mNext through a stack slot via func_8049DD28
// and return it (one more deref than func_8049DD30).
CScnFilterListNode* func_8049DCF4(CScnFilterList* list) {
    CScnFilterListNode* node;
    func_8049DD28(&node, (u32)list->mStartNodePtr->mNext);
    return node;
}

__declspec(noinline) void func_8049DD28(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

// Getter that round-trips the list's sentinel pointer through the node
// setter: retail reads the pointer at self+4, stores it via func_8049DD28
// into a local, and returns that local.
CScnFilterListNode* func_8049DD30(CScnFilterList* list) {
    CScnFilterListNode* node;
    func_8049DD28(&node, (u32)list->mStartNodePtr);
    return node;
}

void CScnFilterMan::cbRenderBefore() {
    func_8049DB14(this);
    CGXCache* cache = (CGXCache*)func_8049DBF0();
    cache->func_8044BE38();
    CViewRoot::func_80442DA8();
}

extern "C" void func_8049CAF4() {}


// Auto-scaffolded catalog TU for monolib/src/scn/CScnFilterMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnFilterMan.hpp"

// Setter helpers are declared here but defined at the BOTTOM of this file
// (after every caller) so MWCC's -inline auto cannot fold them into the
// tail-call wrappers below; retail keeps them as separate functions (same
// pattern as CGXCache.cpp func_8044BE38).
void func_8049D274(CScnFilterListIter* self, u32 val);
void func_8049D64C(CScnFilterListIter* self, u32 val);


void func_8049CB6C(void) {}

void func_8049C9F8(void){}


void func_8049CB70(void){}


void func_8049CC70(){}

void func_8049CCA4(void) {}

void __dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter(){}

void func_8049CD34(){}

void func_8049CDAC(void) {}

extern "C" void func_8049CDB0(void* unused, void* out) { *(u32*)out = 0; }

void __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter(){}

void func_8049CE14(void){}

void func_8049CE18(){}

void func_8049CF44(void){}

void func_8049CF48(){}

CScnFilterMan::~CScnFilterMan() {}

void func_8049D12C(void){}

void __dt__8049D130(){}

void func_8049D188(void){}

void __dt__8049D18C(){}

void func_8049D1EC(){}

void func_8049D218(){}

// iter = list end sentinel (retail: lwz r4,0x4(r4); b func_8049D274)
void func_8049D26C(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D274(self, (u32)list->mStartNodePtr);
}

void func_8049D27C(){}

void func_8049D2F0(){}

void func_8049D338(){}

void func_8049D394(){}

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

void func_8049D5F0(){}

// iter = list end sentinel via the D64C setter
void func_8049D644(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D64C(self, (u32)list->mStartNodePtr);
}

void func_8049D654(){}

void func_8049D6C8(){}

void func_8049D710(){}

void func_8049D76C(){}

void CScnFilterMan::update() {}

extern "C" void func_8049D8D4(u32* self, u32 flags) { *self |= flags; }

void func_8049D8E4(){}

void func_8049D914(){}

// iter = first node via the D64C setter
void func_8049D948(CScnFilterListIter* self, CScnFilterList* list) {
    func_8049D64C(self, (u32)list->mStartNodePtr->mNext);
}

extern "C" void* func_8049D954(void* self){ return (void*)((char*)*(void**)self + 8); }

void func_8049D960(){}

extern "C" void func_8049D994(int* dst, int* src){
    *dst = *src;
}

extern "C" void func_8049D9A0(u32* self) { *self = *(u32*)(*(u32**)self); }

void func_8049D9B0(){}

void func_8049D9CC(void){}

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

void func_8049DB14(){}

// Retail refs the CDeviceGX static (extern; defined in retail data object)
// by its mangled name cacheInstance__9CDeviceGX via SDA21.
extern "C" { extern void* cacheInstance__9CDeviceGX; }
extern "C" void* func_8049DBF0(void) { return cacheInstance__9CDeviceGX; }

// True when the filter's mFlags has bit 1 set (flag-test helper tail call).
s32 func_8049DBF8(CScnFilter* filter) {
    return func_8004B3D8(&filter->mFlags, 2);
}

void func_8049DC04(){}

void func_8049DC5C(){}

extern "C" void func_8049DCC8(u32* self) { *self = *(u32*)(*(u32**)self); }

void func_8049DCD8(){}

void func_8049DCF4(){}

void func_8049DD28(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

void func_8049DD30(){}

void CScnFilterMan::cbRenderBefore() {}

extern "C" void func_8049CAF4() {}


// Auto-scaffolded catalog TU for monolib/src/scn/CScnVirtualLight
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnVirtualLight.hpp"
#include "libs/monolib/src/scn/CVirtualLightAmb.hpp"
#include "monolib/util/MemManager.hpp"
#include "libs/monolib/src/scn/CScnCameraMan.hpp"
#include "libs/monolib/src/scn/CScnFilterMan.hpp"
#include "monolib/math/CVec3.hpp"
#include "functions.hpp"

void func_80492030(void) {}

// _reslist_base<CVirtualLightObjPtr> ctor (retail __ct__804920B0): installs
// the base vtable, zeroes the storage fields and links the sentinel node
// onto itself. noinline keeps the bl from the derived ctor __ct__80492074
// (retail keeps the base init out-of-line, cf. CScnFilterMan). extern "C"
// keeps the bl reloc flat-named (C++ linkage would mangle the __ct__ name
// with the arg types).
extern "C" __declspec(noinline) void __ct__804920B0(CScnVirtualLightReslist* obj) {
    obj->mVtable = (void*)lbl_eu_80526448;
    obj->mList = nullptr;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = &obj->mStartNode;
    obj->mStartNodePtr->mPrev = obj->mStartNode.mNext;
}

// reslist<CVirtualLightObjPtr> ctor (retail __ct__80492074): base init then
// install the derived vtable (flattened-name pattern, cf. CScnFilterMan).
// extern "C" keeps the emitted symbol flat-named so the exact-name match
// (unit sweep) resolves it; the bl to the base ctor stays flat too.
extern "C" CScnVirtualLightReslist* __ct__80492074(CScnVirtualLightReslist* obj) {
    __ct__804920B0(obj);
    obj->mVtable = (void*)lbl_eu_80526430;
    return obj;
}

// Empty deleting destructor (retail keeps the delete-if-flag epilogue).
CLight::~CLight() {}

// Empty deleting destructor (retail keeps the delete-if-flag epilogue).
CVirtualLightObj::~CVirtualLightObj() {}

// Item-destroy hook for the reslist clear walk (retail body is just blr).
// noinline keeps the bl from func_804920E0's clear loop out-of-line.
extern "C" __declspec(noinline) void func_80492158(CVirtualLightObj** item) {}

// Reset a node's mNext to null after its item is destroyed (retail
// func_8049215C: li r0,0; stw r0,0(r4); blr). noinline keeps the bl from
// func_804920E0's clear loop out-of-line (the trivial body would otherwise
// be inlined, dropping the retail call-site reloc).
extern "C" __declspec(noinline) void func_8049215C(void* self, void* out) { *(u32*)out = 0; }

extern "C" void func_8049216C(void* self);
extern "C" void func_80492168(void* self) { func_8049216C(self); }


// Base reslist teardown helper (retail func_804920E0; defined at the bottom
// of this TU under #pragma auto_inline off).
extern "C" void func_804920E0(CScnVirtualLightReslist* self);

extern "C" void __dt__804923A0(CScnVirtualLightReslist* obj);
extern "C" void func_8049239C(CScnVirtualLightReslist* self) {
    __dt__804923A0(self);
}

// destroyList for the virtual-light reslist (retail __dt__804923A0): run the
// base list teardown, free the node array unless it is externally owned
// (field_0x1C), and reset the capacity. noinline keeps the tail-call
// `b __dt__804923A0` from func_8049239C (retail keeps it out-of-line).
extern "C" __declspec(noinline) void __dt__804923A0(CScnVirtualLightReslist* obj) {
    func_804920E0(obj);
    if (obj->field_0x1C == false && obj->mList != nullptr) {
        delete[] obj->mList;
        obj->mList = nullptr;
    }
    obj->mCapacity = 0;
}


void func_804929C0(){}

extern "C" __declspec(noinline) void func_80492A5C(void* self, u32 val) { ((CScnVirtualLightData*)self)->value00 = val; }

void func_80492A50(CScnVirtualLightData* self, CScnVirtualLightValueSrc* src) {
    func_80492A5C(self, *src->mValue);
}

extern "C" void* func_80492A64(void* self){ return (void*)((char*)*(void**)self + 8); }

extern "C" void func_80492A70(u32* self) { *self = *(u32*)(*(u32**)self); }

// retail: lwz r4,0x4(r4); b func_80492A5C (r3 passes through). noinline
// keeps the bl from func_80493148 out-of-line (retail calls it).
extern "C" __declspec(noinline) void func_80492A80(void* self, void* src) { func_80492A5C(self, *(u32*)((char*)src + 4)); }

u32 func_80492A88(const u32* a, const u32* b) {
    return *a != *b;
}

extern "C" void func_804920E0(CScnVirtualLightReslist* self);
extern "C" void func_80492AA4(void* self) { func_804920E0((CScnVirtualLightReslist*)self); }

extern "C" void func_80492AA8(int* dst, int* src){
    *dst = *src;
}

// Erase the node pointed to by *in from its ring: unlink it, destroy its
// item (func_80492158), reset it (func_8049215C) and write the following
// node to *out via func_80492A5C (retail func_80492AB4). The *in re-reads
// mirror the retail aliasing reloads around the two calls.
extern "C" void func_80492AB4(CScnVirtualLightData* out, CScnVirtualLightReslist* list,
                              CScnVirtualLightNode** in) {
    CScnVirtualLightNode* node = *in;
    CScnVirtualLightNode* prev = node->mPrev;
    CScnVirtualLightNode* next = node->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    func_80492158(&(*in)->mItem);
    func_8049215C(list, *in);
    func_80492A5C(out, (u32)next);
}

extern "C" void func_80492B34(int* dst, int* src){
    *dst = *src;
}

void func_80492B40(){}

// Creates a CVirtualLightAmb for the given light-data field and initializes it
// from `data` (4 words) and `val`, returning the new light. Wrapper family:
// func_80492DB8/DD4/DE0/DFC. extern "C" keeps the caller relocs (and the
// emitted symbol) flat; taking &ptr pins the light to the frame like retail.
extern "C" __declspec(noinline) CVirtualLightAmb* func_804930BC(CScnVirtualLightData* self, u8* field,
                              const func_800407C8_tmp* data, f32 val) {
    CVirtualLightAmb* ptr;
    void* mem = mtl::MemManager::allocate(0x34, self->value08);
    if (mem != 0) {
        ptr = __ct__CVirtualLightAmb((CVirtualLightAmb*)mem);
    } else {
        ptr = (CVirtualLightAmb*)mem;
    }
    func_80493148((CScnVirtualLightData*)field, (u32)&ptr);
    copyWord4Offset((u32*)ptr, (const u32*)data);
    func_80493140(ptr, val);
    return ptr;
}

// Same family as func_804930BC but with the extra 3-float `val` pointer (r6)
// and the f32 scalar in f1; creates+initializes the light object at `field`.
extern "C" __declspec(noinline) void func_804933AC(CScnVirtualLightData* self, u8* field,
                              const func_800407C8_tmp* data, const func_800407C8_tmp* val,
                              f32 value) {}

void func_80492DB8(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, &self->_0C[0], arg, val);
}

// func_804933AC variant: also passes a 3-float `val` (direction) in r6, and an
// f32 scalar in f1 (see func_804933AC's own body for how it consumes them).
void func_80492DC4(CScnVirtualLightData* self, const func_800407C8_tmp* arg,
                   const func_800407C8_tmp* val, f32 value) {
    func_804933AC(self, &self->_0C[0], arg, val, value);
}

void func_80492DD4(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, (u8*)&self->value4C, arg, val);
}

void func_80492DE0(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, (u8*)&self->value2C, arg, val);
}

void func_80492DEC(CScnVirtualLightData* self, const func_800407C8_tmp* arg,
                   const func_800407C8_tmp* val, f32 value) {
    func_804933AC(self, (u8*)&self->value2C, arg, val, value);
}

void func_80492DFC(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, &self->_6C[0], arg, val);
}

void func_80492E08(){}

// Retail func_80493140: store `val` into the light's +0x14 field. noinline
// keeps the bl from func_804930BC out-of-line (retail calls it; the trivial
// store body would otherwise be inlined, dropping the call-site reloc).
__declspec(noinline) void func_80493140(void* self, float val) { ((CScnVirtualLightData*)self)->value14 = val; }

// Forward decls for the reslist item-insert helpers (defined below; the
// func_80492A80 / func_80493148 wrappers reference them).
extern "C" __declspec(noinline) CScnVirtualLightNode* func_80493210(CScnVirtualLightReslist* list, u32 val);
extern "C" __declspec(noinline) CScnVirtualLightNode* func_804932BC(CScnVirtualLightReslist* list);
extern "C" __declspec(noinline) void func_80493258(CVirtualLightObj** item, u32 val);
extern "C" __declspec(noinline) void func_8049319C(u32* out, CScnVirtualLightData* self,
                                                   u32* in, u32 arg2);

// Copy the light-data value at self+4 into *inValue, then hand off to the
// reslist insert helper (retail func_80493148). outValue is declared first so
// MWCC gives it the higher stack slot (sp+0xC) and inValue sp+0x8 (retail
// order: first call writes sp+8, second call reads sp+8 / writes sp+0xC).
// noinline keeps the bl from func_804930BC out-of-line (retail calls it).
__declspec(noinline) void func_80493148(CScnVirtualLightData* self, u32 arg2) {
    u32 outValue;
    u32 inValue;
    func_80492A80(&inValue, self);
    func_8049319C(&outValue, self, &inValue, arg2);
}

// (retail func_8049319C) reslist item-insert step: allocate a node for arg2,
// link it before the node pointed to by *in (the sentinel; this is the
// push_back shape) and write the new node to *out via func_80492A5C. The
// repeated *in loads mirror the retail aliasing reloads (non-const in keeps
// MWCC from caching the pointee across the node stores).
extern "C" __declspec(noinline) void func_8049319C(u32* out, CScnVirtualLightData* self,
                                                   u32* in, u32 arg2) {
    CScnVirtualLightNode* node = func_80493210((CScnVirtualLightReslist*)self, arg2);
    node->mNext = (CScnVirtualLightNode*)*in;
    node->mPrev = ((CScnVirtualLightNode*)*in)->mPrev;
    ((CScnVirtualLightNode*)*in)->mPrev->mNext = node;
    ((CScnVirtualLightNode*)*in)->mPrev = node;
    func_80492A5C(out, (u32)node);
}

// Allocate a node from the free slot and install `val` into its item slot
// (retail func_80493210; the push_front/back item-insert step).
CScnVirtualLightNode* func_80493210(CScnVirtualLightReslist* list, u32 val) {
    CScnVirtualLightNode* node = func_804932BC(list);
    func_80493258(&node->mItem, val);
    return node;
}

// setItem for the reslist node (retail func_80493258): placement-new a
// CVirtualLightObj* cell at *item via the placement-new operator
// (func_804932B4) and copy *val into it. try/catch is the retail setItem
// shape (forces MWCC's frame-pointer prologue).
extern "C" __declspec(noinline) void func_80493258(CVirtualLightObj** item, u32 val) {
    CVirtualLightObj** p = (CVirtualLightObj**)func_804932B4((u8*)4, (u8*)item);
    if (p != NULL) {
        try {
            *p = *(CVirtualLightObj**)val;
        } catch (...) {
            throw;
        }
    }
}

// Placement-new operator (retail: mr r3,r4; blr). noinline keeps the `bl
// func_804932B4` at the setItem call sites out-of-line (a trivial body here
// would be inlined by MWCC, dropping the reloc the retail call sites have).
extern "C" __declspec(noinline) void* func_804932B4(void* self, void* param){ return param; }

// Find the first free slot (mNext == 0) in the node array and return its
// address (retail func_804932BC). Retail uses the counted-loop form
// (mtctr/bdnz) with a byte-offset induction variable plus the index for the
// final mulli; noinline keeps the bl from func_80493210 out-of-line.
extern "C" __declspec(noinline) CScnVirtualLightNode* func_804932BC(CScnVirtualLightReslist* list) {
    int capacity = list->mCapacity;
    int i;
    for (i = 0; i < capacity; i++) {
        if (list->mList[i].mNext == 0) {
            break;
        }
    }
    return &list->mList[i];
}

void func_80493300(){}

void func_8049347C(){}

u32 func_80493574(void* self) { return ((CScnVirtualLightData*)self)->value2C; }

extern "C" void func_8049357C(int* dst, int* src){
    *dst = *src;
}

extern "C" void* func_80493588(void* self){ return (void*)((char*)*(void**)self + 8); }

extern "C" void func_80493594(u32* self) { *self = *(u32*)(*(u32**)self); }

u32 func_804935A4(const u32* a, const u32* b) {
    return *a != *b;
}

void func_804935C0(){}

void func_804936AC(){}

int func_80493B78(CScnCameraMan* cam) {
    return func_8049B158(cam, cam->mCamId);
}

u32 func_80493B80(void* self) { return ((CScnVirtualLightData*)self)->value68; }

extern "C" u32 func_80493B88(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1160); }

extern "C" u32 func_80493B98(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1150); }

extern "C" u32 func_80493BA8(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1140); }

s32 func_80493BB8(CScnVirtualLightData* self) {
    return func_8004B3D8(&self->value30, 0x10000);
}

s32 func_80493BC4(CScnVirtualLightData* self, u32 flags) {
    return func_8004B3D8(&self->value30, flags);
}

// Retail: mr r0,r3; mr r3,r4; mr r4,r0; b .+4; <paired-single subtract body>.
// The mr-swap adapts the (a, out, b) entry to a natural-order (out, a, b)
// body, and the `b .+4` between the swap block and the ps body is the
// documented unreproducible MWCC scheduler barrier (MWCC_REFERENCE sec. 4;
// cf. func_80493C08 below - byte-identity needs a policy exception). MWCC's
// -O4,p auto-vectorizer also only pairs FMA/lerp shapes, never this pure
// subtract (verified: field ops, paired-member struct, and loop forms all
// emit scalar lfs/fsubs), so the ps body itself needs the nw4r VEC3Sub asm
// kernel. Natural reconstruction kept: correct 0x34 size and semantics.
void func_80493BCC(Vec* a, Vec* out, const Vec* b) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

void* func_80493C00(void* self) { return &((CScnVirtualLightData*)self)->_04; }

// Retail: addi r3,r3,0x54; b .+4; addi r3,r3,0xB8; blr. The b .+4 scheduler
// barrier is the documented unreproducible MWCC artifact (MWCC_REFERENCE sec. 4;
// GetTextColor is the only getter precedent and was never byte-matched).
// Natural reconstruction kept; byte-identity needs a policy exception.
void* func_80493C08(void* self) {
    return (void*)((u8*)self + 0x54 + 0xB8);
}

float func_80493C18(void* self) { return ((CScnVirtualLightData*)self)->value3C; }

extern "C" void func_80493C20(void* self, u32 val, u32 idx) { *(u32*)((u8*)self + idx*4 + 0x1160) = val; }

void func_80493C30(){}

float func_80493EC0(void* self) { return ((CScnVirtualLightData*)self)->valueD4; }

u8 func_80493EC8(void* self) {
    return ((CScnVirtualLightData*)self)->enabled;
}

void* func_80493ED0(void* self) { return &((CScnVirtualLightData*)self)->value14; }

u32 func_80493ED8(void* self) { return ((CScnVirtualLightData*)self)->value08; }

u32 func_80493EE0(void* self) { return ((CScnVirtualLightData*)self)->value64; }

s32 func_80493EE8(CScnVirtualLightData* self) {
    return func_8004B3D8(&self->value484, 1);
}

u32 func_80493EF4(void* self) { return ((CScnVirtualLightData*)self)->value480; }

extern "C" void* func_80493EFC(void* self, u32 idx){ return (void*)((char*)self + idx * 64); }

void func_80493F08(){}

float func_804940E0(void* self) { return ((CScnVirtualLightData*)self)->value34; }

float func_804940E8(void* self) { return ((CScnVirtualLightData*)self)->value38; }

extern "C" void* func_804940F0(void* self) { return (void*)((u8*)self + 0x34); }

extern "C" float func_804940F8(void* self) { return ((CScnVirtualLightData*)self)->value40; }

extern "C" float func_80494100(void* self) { return ((CScnVirtualLightData*)self)->value40; }

extern "C" float func_80494108(void* self) { return ((CScnVirtualLightData*)self)->value44; }

extern "C" void* func_80494110(void* self) { return (void*)((u8*)self + 0x34); }

extern "C" float func_80494118(void* self) { return ((CScnVirtualLightData*)self)->value4C; }

extern "C" float func_80494120(void* self) { return ((CScnVirtualLightData*)self)->value48; }

// Allocate a free slot from the 128-slot light pool: find the first handle
// slot whose pointer is null, link it to the record array entry and bump the
// slot count, returning the handle (retail func_80494128). The member
// re-reads in the success path (mHandles / mCount) mirror the retail loads.
void* func_80494128(CScnVirtualLightPool* self) {
    for (int i = 0; i < 128; i++) {
        if (self->mHandles[i] == 0) {
            self->mHandles[i] = &self->mRecords[i];
            self->mCount++;
            return self->mHandles[i];
        }
    }
    return 0;
}

// Free a pool slot: find the handle that points at `slot`, reset the light
// (func_804C03A0), clear the handle and decrement the slot count, clamped at
// 0. The mHandles base reload every iteration mirrors the retail aliasing
// loads around the bl (retail func_80494188).
void func_80494188(CScnVirtualLightPool* self, CScnVirtualLightPoolSlot* slot) {
    for (int i = 0; i < 128; i++) {
        if (self->mHandles[i] == slot) {
            func_804C03A0((u8*)slot, 0);
            self->mHandles[i] = 0;
            u32 count = self->mCount - 1;
            self->mCount = count;
            if ((s32)count < 0) {
                self->mCount = 0;
            }
        }
    }
}

void func_80494208(){}

void func_804942BC(f32* a, const f32* b) {
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
    a[3] += b[3];
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static-init for the retail-owned vec4 object lbl_eu_80658648: fills it with
// (v, v, v, w) from the two sdata2 constants and tail-calls the vec4 setter.
void sinit_80494300() {
    func_800407C8(&lbl_eu_80658648, lbl_eu_8066AA14, lbl_eu_8066AA14,
                  lbl_eu_8066AA14, lbl_eu_8066AA18);
}

#pragma push
#pragma auto_inline off
// clearList for the virtual-light reslist (retail func_804920E0): walk the
// ring from the first node, destroy each node's item (func_80492158) and
// reset its mNext (func_8049215C), then relink the sentinel onto itself
// (reslist.hpp clearList shape).
extern "C" void func_804920E0(CScnVirtualLightReslist* self) {
    CScnVirtualLightNode* node = self->mStartNodePtr->mNext;
    while (node != self->mStartNodePtr) {
        CScnVirtualLightNode* cur = node;
        node = node->mNext;
        func_80492158(&cur->mItem);
        func_8049215C(self, cur);
    }
    self->mStartNodePtr->mNext = self->mStartNodePtr;
    self->mStartNodePtr->mPrev = self->mStartNodePtr;
}
extern "C" void func_8049216C(void* self) {}
#pragma pop
extern "C" void func_804923F8() {}

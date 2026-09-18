// Auto-scaffolded catalog TU for monolib/src/scn/CScnVirtualLight
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnVirtualLight.hpp"
#include "libs/monolib/src/scn/CVirtualLightAmb.hpp"
#include "monolib/util/MemManager.hpp"
#include "libs/monolib/src/scn/CScnCameraMan.hpp"
#include "libs/monolib/src/scn/CScnFilterMan.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CVec4.hpp"
#include "functions.hpp"

extern "C" __declspec(noinline) void scnVlInitVec4(void* self) {}

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
extern "C" __declspec(noinline) CScnVirtualLightReslist* __ct__80492074(CScnVirtualLightReslist* obj) {
    __ct__804920B0(obj);
    obj->mVtable = (void*)lbl_eu_80526430;
    return obj;
}

// Retail __ct__CScnVirtualLight (flat name; cf. CScnItemLightNw4r.cpp):
// installs the vtable, stores the scene pool, seeds the allocation handle to
// INVALID, default-constructs the four reslists (__ct__80492074) and the
// three blend vec4s (scnVlInitVec4), arms the enabled byte, then fetches the
// pool's ALLOC_HANDLE through CScnItemPool_resolveScene + the vtable+0x2C virtual and
// creates the manager region. The reslists reserve 8/8/8/2 nodes, the
// CLight[0x80] / LightObj[0x80] arrays are built with __construct_new_array
// and cross-linked via LightCtlSetObj, the 128-slot allocation is created,
// and the lights are initialized by scnVlInitLights.
extern "C" CScnVirtualLight* __ct__CScnVirtualLight(CScnVirtualLight* self,
                                                    CScnVirtualLightSrc* src) {
    *(void**)self = (void*)lbl_eu_8056E868;
    self->mSub = src;
    self->value08 = (u32)-1;
    __ct__80492074(&self->res_0C);
    __ct__80492074(&self->res_2C);
    __ct__80492074(&self->res_4C);
    __ct__80492074(&self->res_6C);
    scnVlInitVec4(&self->field_0x8C);
    scnVlInitVec4(&self->field_0x9C);
    scnVlInitVec4(&self->field_0xAC);
    f32 d4 = lbl_eu_8066AA10;
    // comma sequencing pins the byte store ahead of the float store
    self->enabled = 1, self->valueD4 = d4;
    u32 handle = ((CScnVirtualLightHandleSrc*)CScnItemPool_resolveScene(src))->GetHandle();
    self->value08 = mtl::MemManager::create(handle, 0x10000, lbl_eu_80524044);
    scnVlReserve(&self->res_0C, self->value08, 8);
    scnVlReserve(&self->res_2C, self->value08, 8);
    scnVlReserve(&self->res_4C, self->value08, 8);
    scnVlReserve(&self->res_6C, self->value08, 2);
    self->mCLights = (CLight*)__construct_new_array(
        mtl::MemManager::allocate_array(0x2010, self->value08),
        &__ct__6CLightFv, &__dt__6CLightFv, 0x40, 0x80);
    self->mLightObjs = (nw4r::g3d::LightObj*)__construct_new_array(
        mtl::MemManager::allocate_array(0x2210, self->value08),
        &__ct__Q34nw4r3g3d8LightObjFv, &__dt__Q34nw4r3g3d8LightObjFv, 0x44,
        0x80);
    int i = 0;
    do {
        LightCtlSetObj(&self->mCLights[i], &self->mLightObjs[i]);
        i++;
    } while (i < 0x80);
    self->mExtraAlloc = (u32*)mtl::MemManager::allocate_head(self->value08, 0x200, 4);
    scnVlInitLights(self);
    return self;
}

// Empty deleting destructor (retail keeps the delete-if-flag epilogue).
CLight::~CLight() {}

// Empty deleting destructor (retail keeps the delete-if-flag epilogue).
CVirtualLightObj::~CVirtualLightObj() {}

// Item-destroy hook for the reslist clear walk (retail body is just blr).
// noinline keeps the bl from scnVlClearList's clear loop out-of-line.
extern "C" __declspec(noinline) void scnVlItemNop(CVirtualLightObj** item) {}

// Reset a node's mNext to null after its item is destroyed (retail
// scnVlZeroNext: li r0,0; stw r0,0(r4); blr). noinline keeps the bl from
// scnVlClearList's clear loop out-of-line (the trivial body would otherwise
// be inlined, dropping the retail call-site reloc).
extern "C" __declspec(noinline) void scnVlZeroNext(void* self, void* out) { *(u32*)out = 0; }

extern "C" void scnVlAllocNodes(CScnVirtualLightReslist* self, u32 handle, int capacity);
extern "C" __declspec(noinline) void scnVlReserve(void* self, u32 handle, int capacity) {
    scnVlAllocNodes((CScnVirtualLightReslist*)self, handle, capacity);
}


// Base reslist teardown helper (retail scnVlClearList; defined at the bottom
// of this TU under #pragma auto_inline off).
extern "C" void scnVlClearList(CScnVirtualLightReslist* self);

extern "C" void __dt__804923A0(CScnVirtualLightReslist* obj);
extern "C" __declspec(noinline) void scnVlDtorList(CScnVirtualLightReslist* self) {
    __dt__804923A0(self);
}

// Destroy the scene light manager: reset via func_804923F8, destroyList the
// four reslists (scnVlDtorList), free the CLight/LightObj arrays and the
// extra allocation, erase the resource id, then run each reslist's explicit
// dtor (retail __dt__reslist_CVirtualLightObj, flat-name import) in reverse
// member order. The vtable (lbl_eu_8056E868) is stored manually because the
// class is __declspec(novtable); the virtual dtor supplies the deleting
// epilogue (flag > 0 -> __dl__FPv).
CScnVirtualLight::~CScnVirtualLight() {
    *(void**)this = (void*)lbl_eu_8056E868;
    func_804923F8(this, 0);
    scnVlDtorList(&res_0C);
    scnVlDtorList(&res_4C);
    scnVlDtorList(&res_2C);
    scnVlDtorList(&res_6C);
    if (mCLights != nullptr) {
        delete[] mCLights;
        mCLights = nullptr;
    }
    if (mLightObjs != nullptr) {
        delete[] mLightObjs;
        mLightObjs = nullptr;
    }
    mtl::MemManager::deallocate(mExtraAlloc);
    mtl::MemManager::erase(value08);
    __dt__reslist_CVirtualLightObj(&res_6C, -1);
    __dt__reslist_CVirtualLightObj(&res_4C, -1);
    __dt__reslist_CVirtualLightObj(&res_2C, -1);
    __dt__reslist_CVirtualLightObj(&res_0C, -1);
}

// destroyList for the virtual-light reslist (retail __dt__804923A0): run the
// base list teardown, free the node array unless it is externally owned
// (field_0x1C), and reset the capacity. noinline keeps the tail-call
// `b __dt__804923A0` from scnVlDtorList (retail keeps it out-of-line).
extern "C" __declspec(noinline) void __dt__804923A0(CScnVirtualLightReslist* obj) {
    scnVlClearList(obj);
    if (obj->field_0x1C == false && obj->mList != nullptr) {
        delete[] obj->mList;
        obj->mList = nullptr;
    }
    obj->mCapacity = 0;
}


// Walk the ring from `b` while the value at scnVlNodeItem(b) differs from
// *d (scnVlWordsNe tests the pair, scnVlAdvance advances), then copy `b`
// into `a` (retail scnVlFindNode).
// Walk the ring from `b` while the value at scnVlNodeItem(b) differs from
// *d (scnVlWordsNe tests the pair, scnVlAdvance advances), then copy `b`
// into `a` (retail scnVlFindNode). The body/check/end labels mirror the
// retail loop rotation (body below the head with an initial jump over); the
// `cur` local keeps MWCC's cmplw operand order (rA = the call-derived value).
extern "C" __declspec(noinline) void scnVlFindNode(int* a, u32* b, const u32* c, const u32* d) {
    goto check;
body:
    scnVlAdvance(b);
check:
    if (!scnVlWordsNe(b, c)) {
        goto end;
    }
    u32 cur = *scnVlNodeItem(b);
    if (cur == *d) {
        goto end;
    }
    goto body;
end:
    scnVlCopyWord(a, (int*)b);
}

extern "C" __declspec(noinline) void scnVlSetValue(void* self, u32 val) { ((CScnVirtualLightData*)self)->value00 = val; }

__declspec(noinline) void scnVlLoadValue(CScnVirtualLightData* self, CScnVirtualLightValueSrc* src) {
    scnVlSetValue(self, *src->mValue);
}

extern "C" __declspec(noinline) u32* scnVlNodeItem(void* self){ return (u32*)((char*)*(void**)self + 8); }

extern "C" __declspec(noinline) void scnVlAdvance(u32* self) { *self = *(u32*)(*(u32**)self); }

// retail: lwz r4,0x4(r4); b scnVlSetValue (r3 passes through). noinline
// keeps the bl from scnVlInsertItem out-of-line (retail calls it).
extern "C" __declspec(noinline) void scnVlCopyValue(void* self, void* src) { scnVlSetValue(self, *(u32*)((char*)src + 4)); }

// Word-compare helper (retail scnVlWordsNe): returns *a != *b. noinline
// keeps the bl from scnVlFindNode out-of-line (retail calls it).
__declspec(noinline) u32 scnVlWordsNe(const u32* a, const u32* b) {
    return *a != *b;
}

extern "C" void scnVlClearList(CScnVirtualLightReslist* self);
extern "C" __declspec(noinline) void scnVlClearThunk(void* self) { scnVlClearList((CScnVirtualLightReslist*)self); }

extern "C" __declspec(noinline) int* scnVlCopyWord(int* dst, int* src){
    *dst = *src;
    return dst;
}

// Erase the node pointed to by *in from its ring: unlink it, destroy its
// item (scnVlItemNop), reset it (scnVlZeroNext) and write the following
// node to *out via scnVlSetValue (retail scnVlUnlinkNode). The *in re-reads
// mirror the retail aliasing reloads around the two calls.
extern "C" __declspec(noinline) void scnVlUnlinkNode(CScnVirtualLightData* out, CScnVirtualLightReslist* list,
                              CScnVirtualLightNode** in) {
    CScnVirtualLightNode* node = *in;
    CScnVirtualLightNode* prev = node->mPrev;
    CScnVirtualLightNode* next = node->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    scnVlItemNop(&(*in)->mItem);
    scnVlZeroNext(list, *in);
    scnVlSetValue(out, (u32)next);
}

extern "C" __declspec(noinline) void scnVlStoreWord(int* dst, int* src){
    *dst = *src;
}

// Retail scnVlInitLights: reset the manager (func_804923F8), then build the
// light slots. Each slot is a pair of vector builders - a vec3 direction
// (writeVec3f) and a vec4 color (CTaskGame_setVec4) - feeding
// scnVlMakeAmb0C (ambient) / scnVlMakeDir0C (directional); the +0x4C/+0x2C/
// +0x6C reslists share the lbl_eu_80658648 vec4 (scnVlMakeAmb4C/DE0/DFC).
// Ends by seeding the blend fields (+0x8C/+0x9C/+0xAC) with the AA14 vec4
// and the +0xBC factor with AA24.
extern "C" void scnVlInitLights(CScnVirtualLight* self) {
    CTaskGame_setVec4_tmp v_d8;
    CTaskGame_setVec4_tmp v_c8;
    ml::CVec3 v_b8;
    CTaskGame_setVec4_tmp v_a8;
    ml::CVec3 v_98;
    CTaskGame_setVec4_tmp v_88;
    ml::CVec3 v_78;
    CTaskGame_setVec4_tmp v_68;
    ml::CVec3 v_58;
    CTaskGame_setVec4_tmp v_48;
    ml::CVec3 v_38;
    CTaskGame_setVec4_tmp v_28;
    ml::CVec3 v_18;
    CTaskGame_setVec4_tmp v_8;
    // Direction-pointer group declared as plain locals (colors r30 in
    // retail); the B60C result feeding scnVlMakeDir2C is a nested call temp,
    // not a named local, so it takes its own callee-saved register (r31).
    ml::CVec3* p_b8;
    ml::CVec3* p_98;
    ml::CVec3* p_78;
    ml::CVec3* p_58;
    ml::CVec3* p_38;
    func_804923F8(self, 0);
    scnVlMakeAmb0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_d8, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  lbl_eu_8066AA18);
    p_b8 = writeVec3f(&v_b8, lbl_eu_8066AA1C, lbl_eu_8066AA20,
                         lbl_eu_8066AA1C);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_c8, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)p_b8, lbl_eu_8066AA18);
    p_98 = writeVec3f(&v_98, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_a8, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)p_98, lbl_eu_8066AA18);
    p_78 = writeVec3f(&v_78, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_88, lbl_eu_8066AA24, lbl_eu_8066AA24,
                                lbl_eu_8066AA24, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)p_78, lbl_eu_8066AA18);
    p_58 = writeVec3f(&v_58, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_68, lbl_eu_8066AA24, lbl_eu_8066AA24,
                                lbl_eu_8066AA24, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)p_58, lbl_eu_8066AA18);
    p_38 = writeVec3f(&v_38, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_48, lbl_eu_8066AA24, lbl_eu_8066AA24,
                                lbl_eu_8066AA24, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)p_38, lbl_eu_8066AA18);
    scnVlMakeAmb4C((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    scnVlMakeAmb2C((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    scnVlMakeDir2C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_28, lbl_eu_8066AA28, lbl_eu_8066AA2C,
                                lbl_eu_8066AA30, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)writeVec3f(
                      &v_18, lbl_eu_8066AA34, lbl_eu_8066AA10,
                      lbl_eu_8066AA34),
                  lbl_eu_8066AA18);
    scnVlMakeAmb6C((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    CTaskGame_setVec4_tmp* p_8 = CTaskGame_setVec4(
        &v_8, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA18);
    u32* d = copyWord4((u32*)&self->field_0xAC, (const u32*)p_8);
    d = copyWord4((u32*)&self->field_0x9C, d);
    copyWord4((u32*)&self->field_0x8C, d);
    self->field_0xBC = lbl_eu_8066AA24;
}

// Creates a CVirtualLightAmb for the given light-data field and initializes it
// from `data` (4 words) and `val`, returning the new light. Wrapper family:
// scnVlMakeAmb0C/DD4/DE0/DFC. extern "C" keeps the caller relocs (and the
// emitted symbol) flat; taking &ptr pins the light to the frame like retail.
extern "C" __declspec(noinline) CVirtualLightAmb* scnVlCreateAmb(CScnVirtualLightData* self, u8* field,
                              const CTaskGame_setVec4_tmp* data, f32 val) {
    CVirtualLightAmb* ptr;
    void* mem = mtl::MemManager::allocate(0x34, self->value08);
    if (mem != 0) {
        ptr = __ct__CVirtualLightAmb((CVirtualLightAmb*)mem);
    } else {
        ptr = (CVirtualLightAmb*)mem;
    }
    scnVlInsertItem((CScnVirtualLightData*)field, (u32)&ptr);
    copyWord4Offset((u32*)ptr, (const u32*)data);
    scnVlSetIntens(ptr, val);
    return ptr;
}

// Same family as scnVlCreateAmb but with the extra 3-float `val` pointer (r6)
// and the f32 scalar in f1; creates+initializes the light object at `field`.
// The direction-vector -> angle conversion keeps the sqrt(x^2+z^2) call even
// though its result is unused (extern call, may warn) - retail computes it
// into the dead f2 slot.
extern "C" __declspec(noinline) void func_804933AC(CScnVirtualLightData* self, u8* field,
                              const CTaskGame_setVec4_tmp* data, const CTaskGame_setVec4_tmp* val,
                              f32 value) {
    // Direction-vector -> dir-light angles. The sqrt result is unused by the
    // call; retail still keeps the call (it may warn). Writing x*x + z*z makes
    // MWCC schedule the z load (f2) up front next to the x load, like retail.
    // Direction-vector -> dir-light angles. Two retail artifacts live in this
    // block: a dead early read of the z component (retail schedules it at the
    // top of the stream, then reloads it for the multiply), and a dead sqrt
    // (len) whose result the scnVlCreateDir call setup overwrites - retail
    // keeps both the call and the dead fmr of its result.
    { const volatile f32& vz = val->unk00[2]; (void)vz; }
    f32 t1 = cfCam_mulSda2Const(-atan2AnimFIdx(-val->unk00[0]));
    f32 len = safeFSqrt(val->unk00[0] * val->unk00[0] + val->unk00[2] * val->unk00[2]);
    f32 t2 = cfCam_mulSda2Const(atan2AnimFIdx(-val->unk00[1]));
    // len is dead in retail too (its result register is overwritten by the
    // scnVlCreateDir argument setup); retail keeps the call regardless.
    (void)len;
    scnVlCreateDir(self, field, data, value, t1, t2);
}

__declspec(noinline) void scnVlMakeAmb0C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val) {
    scnVlCreateAmb(self, &self->_0C[0], arg, val);
}

// func_804933AC variant: also passes a 3-float `val` (direction) in r6, and an
// f32 scalar in f1 (see func_804933AC's own body for how it consumes them).
__declspec(noinline) void scnVlMakeDir0C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg,
                   const CTaskGame_setVec4_tmp* val, f32 value) {
    func_804933AC(self, &self->_0C[0], arg, val, value);
}

__declspec(noinline) void scnVlMakeAmb4C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val) {
    scnVlCreateAmb(self, (u8*)&self->value4C, arg, val);
}

__declspec(noinline) void scnVlMakeAmb2C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val) {
    scnVlCreateAmb(self, (u8*)&self->value2C, arg, val);
}

__declspec(noinline) void scnVlMakeDir2C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg,
                   const CTaskGame_setVec4_tmp* val, f32 value) {
    func_804933AC(self, (u8*)&self->value2C, arg, val, value);
}

__declspec(noinline) void scnVlMakeAmb6C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val) {
    scnVlCreateAmb(self, &self->_6C[0], arg, val);
}

// Retail scnVlInitLights2: richer sibling of scnVlInitLights - five directional
// slots built from (vec3 direction via writeVec3f, vec3 copy via
// scaleVec3f, vec4 color via CTaskGame_setVec4) before the shared
// lbl_eu_80658648 vec4 feeds scnVlMakeAmb4C/DE0/DFC, plus the same blend
// seed tail. The local declaration order mirrors the retail stack layout
// (vec4 then the direction/copy vec3 pair, top-down).
extern "C" void scnVlInitLights2(CScnVirtualLight* self) {
    CTaskGame_setVec4_tmp v100;
    CTaskGame_setVec4_tmp v_f0;
    ml::CVec3 v_e4;
    ml::CVec3 v_d8;
    CTaskGame_setVec4_tmp v_c8;
    ml::CVec3 v_bc;
    ml::CVec3 v_b0;
    CTaskGame_setVec4_tmp v_a0;
    ml::CVec3 v_94;
    ml::CVec3 v_88;
    CTaskGame_setVec4_tmp v_78;
    ml::CVec3 v_6c;
    ml::CVec3 v_60;
    CTaskGame_setVec4_tmp v_50;
    ml::CVec3 v_44;
    ml::CVec3 v_38;
    CTaskGame_setVec4_tmp v_28;
    ml::CVec3 v_18;
    CTaskGame_setVec4_tmp v_8;
    // No named local for the B60C result feeding scnVlMakeDir2C: as a nested
    // call temp it gets its own callee-saved register (r31), leaving r30 for
    // the lbl_eu_80658648 address constant, matching retail.

    func_804923F8(self, 0);
    scnVlMakeAmb0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v100, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  lbl_eu_8066AA18);
    scaleVec3f(&v_e4, writeVec3f(&v_d8, lbl_eu_8066AA3C, lbl_eu_8066AA40,
                                        lbl_eu_8066AA3C),
                  lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_f0, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)&v_e4, lbl_eu_8066AA18);
    scaleVec3f(&v_bc, writeVec3f(&v_b0, lbl_eu_8066AA44, lbl_eu_8066AA48,
                                        lbl_eu_8066AA44),
                  lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_c8, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)&v_bc, lbl_eu_8066AA18);
    scaleVec3f(&v_94, writeVec3f(&v_88, lbl_eu_8066AA4C, lbl_eu_8066AA50,
                                        lbl_eu_8066AA54),
                  lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_a0, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)&v_94, lbl_eu_8066AA18);
    scaleVec3f(&v_6c, writeVec3f(&v_60, lbl_eu_8066AA58, lbl_eu_8066AA5C,
                                        lbl_eu_8066AA60),
                  lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_78, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)&v_6c, lbl_eu_8066AA18);
    scaleVec3f(&v_44, writeVec3f(&v_38, lbl_eu_8066AA68, lbl_eu_8066AA6C,
                                        lbl_eu_8066AA70),
                  lbl_eu_8066AA20);
    scnVlMakeDir0C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_50, lbl_eu_8066AA64, lbl_eu_8066AA64,
                                lbl_eu_8066AA64, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)&v_44, lbl_eu_8066AA18);
    scnVlMakeAmb4C((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    scnVlMakeAmb2C((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    scnVlMakeDir2C((CScnVirtualLightData*)self,
                  CTaskGame_setVec4(&v_28, lbl_eu_8066AA28, lbl_eu_8066AA2C,
                                lbl_eu_8066AA30, lbl_eu_8066AA18),
                  (const CTaskGame_setVec4_tmp*)writeVec3f(
                      &v_18, lbl_eu_8066AA34, lbl_eu_8066AA10,
                      lbl_eu_8066AA34),
                  lbl_eu_8066AA18);
    // Fresh address expression for the tail call keeps the label web short.
    scnVlMakeAmb6C((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    CTaskGame_setVec4_tmp* p_8 = CTaskGame_setVec4(
        &v_8, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA18);
    u32* d = copyWord4((u32*)&self->field_0xAC, (const u32*)p_8);
    d = copyWord4((u32*)&self->field_0x9C, d);
    copyWord4((u32*)&self->field_0x8C, d);
    self->field_0xBC = lbl_eu_8066AA24;
}

// Retail scnVlSetIntens: store `val` into the light's +0x14 field. noinline
// keeps the bl from scnVlCreateAmb out-of-line (retail calls it; the trivial
// store body would otherwise be inlined, dropping the call-site reloc).
__declspec(noinline) void scnVlSetIntens(void* self, float val) { ((CScnVirtualLightData*)self)->value14 = val; }

// Forward decls for the reslist item-insert helpers (defined below; the
// scnVlCopyValue / scnVlInsertItem wrappers reference them).
extern "C" __declspec(noinline) CScnVirtualLightNode* scnVlAllocNode(CScnVirtualLightReslist* list, u32 val);
extern "C" __declspec(noinline) CScnVirtualLightNode* scnVlFindFree(CScnVirtualLightReslist* list);
extern "C" __declspec(noinline) void scnVlSetItem(CVirtualLightObj** item, u32 val);
extern "C" __declspec(noinline) void scnVlLinkNode(u32* out, CScnVirtualLightData* self,
                                                   u32* in, u32 arg2);

// Copy the light-data value at self+4 into *inValue, then hand off to the
// reslist insert helper (retail scnVlInsertItem). outValue is declared first so
// MWCC gives it the higher stack slot (sp+0xC) and inValue sp+0x8 (retail
// order: first call writes sp+8, second call reads sp+8 / writes sp+0xC).
// noinline keeps the bl from scnVlCreateAmb out-of-line (retail calls it).
__declspec(noinline) void scnVlInsertItem(CScnVirtualLightData* self, u32 arg2) {
    u32 outValue;
    u32 inValue;
    scnVlCopyValue(&inValue, self);
    scnVlLinkNode(&outValue, self, &inValue, arg2);
}

// (retail scnVlLinkNode) reslist item-insert step: allocate a node for arg2,
// link it before the node pointed to by *in (the sentinel; this is the
// push_back shape) and write the new node to *out via scnVlSetValue. The
// repeated *in loads mirror the retail aliasing reloads (non-const in keeps
// MWCC from caching the pointee across the node stores).
extern "C" __declspec(noinline) void scnVlLinkNode(u32* out, CScnVirtualLightData* self,
                                                   u32* in, u32 arg2) {
    CScnVirtualLightNode* node = scnVlAllocNode((CScnVirtualLightReslist*)self, arg2);
    node->mNext = (CScnVirtualLightNode*)*in;
    node->mPrev = ((CScnVirtualLightNode*)*in)->mPrev;
    ((CScnVirtualLightNode*)*in)->mPrev->mNext = node;
    ((CScnVirtualLightNode*)*in)->mPrev = node;
    scnVlSetValue(out, (u32)node);
}

// Allocate a node from the free slot and install `val` into its item slot
// (retail scnVlAllocNode; the push_front/back item-insert step).
CScnVirtualLightNode* scnVlAllocNode(CScnVirtualLightReslist* list, u32 val) {
    CScnVirtualLightNode* node = scnVlFindFree(list);
    scnVlSetItem(&node->mItem, val);
    return node;
}

// setItem for the reslist node (retail scnVlSetItem): placement-new a
// CVirtualLightObj* cell at *item via the placement-new operator
// (scnVlPlaceNew) and copy *val into it. try/catch is the retail setItem
// shape (forces MWCC's frame-pointer prologue).
extern "C" __declspec(noinline) void scnVlSetItem(CVirtualLightObj** item, u32 val) {
    CVirtualLightObj** p = (CVirtualLightObj**)scnVlPlaceNew((u8*)4, (u8*)item);
    if (p != NULL) {
        try {
            *p = *(CVirtualLightObj**)val;
        } catch (...) {
            throw;
        }
    }
}

// Placement-new operator (retail: mr r3,r4; blr). noinline keeps the `bl
// scnVlPlaceNew` at the setItem call sites out-of-line (a trivial body here
// would be inlined by MWCC, dropping the reloc the retail call sites have).
extern "C" __declspec(noinline) void* scnVlPlaceNew(void* self, void* param){ return param; }

// Find the first free slot (mNext == 0) in the node array and return its
// address (retail scnVlFindFree). Retail uses the counted-loop form
// (mtctr/bdnz) with a byte-offset induction variable plus the index for the
// final mulli; noinline keeps the bl from scnVlAllocNode out-of-line.
extern "C" __declspec(noinline) CScnVirtualLightNode* scnVlFindFree(CScnVirtualLightReslist* list) {
    int capacity = list->mCapacity;
    int i;
    for (i = 0; i < capacity; i++) {
        if (list->mList[i].mNext == 0) {
            break;
        }
    }
    return &list->mList[i];
}

// Creates a CVirtualLightDir for the given light-data field and initializes it
// from `data` (4 words), `val1` and the two direction floats, returning the
// new light. Sibling of scnVlCreateAmb (amb variant); the if/else phi shape
// pins the store to the merge point like retail. noinline keeps the bl from
// func_804933AC out-of-line (retail calls it).
__declspec(noinline) CVirtualLightDir* scnVlCreateDir(CScnVirtualLightData* self, u8* field, const CTaskGame_setVec4_tmp* data,
                   f32 val1, f32 val2, f32 val3) {
    CVirtualLightDir* ptr;
    void* mem = mtl::MemManager::allocate(0x3C, self->value08);
    if (mem != 0) {
        ptr = __ct__CVirtualLightDir((CVirtualLightDir*)mem);
    } else {
        ptr = (CVirtualLightDir*)mem;
    }
    scnVlInsertItem((CScnVirtualLightData*)field, (u32)&ptr);
    copyWord4Offset((u32*)ptr, (const u32*)data);
    scnVlSetIntens(ptr, val1);
    VirtLight_SetPair(ptr, val2, val3);
    return ptr;
}

// Sibling of scnVlApplyDir6C over the +0x4C reslist: applies `value`/`src` to
// every armed dir light, then stores the scaled vec4 into the +0x8C blend
// target and copies that into the +0xAC blend-current field.
void scnVlApplyDir4C(CScnVirtualLight* self, const ml::CVec4* src, f32 value) {
    ml::CVec4 v18;                  // sp+0x18
    CScnVirtualLightNode* it;       // sp+0x14
    CScnVirtualLightNode* itTmp;    // sp+0x10
    CScnVirtualLightNode* end;      // sp+0xC
    CScnVirtualLightNode* endTmp;   // sp+0x8
    scnVlLoadValue((CScnVirtualLightData*)&itTmp, (CScnVirtualLightValueSrc*)&self->res_4C);
    scnVlCopyInt((int*)&it, (int*)&itTmp);
    while ((scnVlCopyValue(&endTmp, (CScnVirtualLightValueSrc*)&self->res_4C),
            scnVlIterNe((const u32*)&it, (const u32*)scnVlCopyInt((int*)&end, (int*)&endTmp))) != 0) {
        if (scnVlGetType(*(CVirtualLightObj**)scnVlIterItem(&it)) == 1) {
            copyWord4Offset((u32*)*(CVirtualLightObj**)scnVlIterItem(&it), (const u32*)src);
            scnVlSetIntens(*(CVirtualLightObj**)scnVlIterItem(&it), value);
        }
        scnVlIterNext((u32*)&it);
    }
    func_80058BD8(&v18, src, value);
    copyWord4((u32*)&self->field_0x8C, (const u32*)&v18);
    copyWord4((u32*)&self->field_0xAC, (const u32*)&self->field_0x8C);
}

__declspec(noinline) s32 scnVlGetType(void* self) { return ((CScnVirtualLightData*)self)->value2C; }

extern "C" __declspec(noinline) int* scnVlCopyInt(int* dst, int* src){
    *dst = *src;
    return dst;
}

extern "C" __declspec(noinline) void* scnVlIterItem(void* self){ return (void*)((char*)*(void**)self + 8); }

extern "C" __declspec(noinline) void scnVlIterNext(u32* self) { *self = *(u32*)(*(u32**)self); }

__declspec(noinline) s32 scnVlIterNe(const u32* a, const u32* b) {
    return *a != *b;
}

// Apply `value` and the source vec4 `src` to every armed dir light in the
// +0x6C reslist (item +0x2C == 1), then copy the scaled vec4 into the +0x9C
// blend-start field. Iterates the reslist ring with the iterator-helper calls
// as retail does: begin via scnVlLoadValue + word copy, end re-fetched every
// iteration (scnVlCopyValue + copy), advance scnVlIterNext, deref scnVlIterItem.
// The item is re-dereferenced at every use (retail calls scnVlIterItem three
// times; MWCC cannot CSE the call across the other calls).
void scnVlApplyDir6C(CScnVirtualLight* self, const ml::CVec4* src, f32 value) {
    ml::CVec4 v18;                  // sp+0x18
    CScnVirtualLightNode* it;       // sp+0x14
    CScnVirtualLightNode* itTmp;    // sp+0x10
    CScnVirtualLightNode* end;      // sp+0xC
    CScnVirtualLightNode* endTmp;   // sp+0x8
    scnVlLoadValue((CScnVirtualLightData*)&itTmp, (CScnVirtualLightValueSrc*)&self->res_6C);
    scnVlCopyInt((int*)&it, (int*)&itTmp);
    while ((scnVlCopyValue(&endTmp, (CScnVirtualLightValueSrc*)&self->res_6C),
            scnVlIterNe((const u32*)&it, (const u32*)scnVlCopyInt((int*)&end, (int*)&endTmp))) != 0) {
        if (scnVlGetType(*(CVirtualLightObj**)scnVlIterItem(&it)) == 1) {
            copyWord4Offset((u32*)*(CVirtualLightObj**)scnVlIterItem(&it), (const u32*)src);
            scnVlSetIntens(*(CVirtualLightObj**)scnVlIterItem(&it), value);
        }
        scnVlIterNext((u32*)&it);
    }
    func_80058BD8(&v18, src, value);
    copyWord4((u32*)&self->field_0x9C, (const u32*)&v18);
}

// Retail func_804936AC: per-frame environment push. When the pool's flag bit
// is set, forwards (sub, env) to scnLgtDispatchEnv. Otherwise re-selects each of
// the four light slots (func_80495644) and re-dispatches every node of the
// reslists into the env (func_80493F08); then - when the manager is armed
// (valueCC) - walks the 128-slot light pool and copies nearby lights into the
// env's per-slot CLight banks (mSlotPtrs/mSlotCounts/mSlotFields). The local
// declaration order mirrors the retail stack layout (slot banks at sp+0x68..
// sp+0x38, then the four reslist-walk value/copy/end triples descending).
void func_804936AC(CScnVirtualLight* self, CLightEnv* env) {
    u32 slotFields[4];   // env mSlotFields (sp+0x68)
    u32 slotCounts[4];   // env mSlotCounts (sp+0x58)
    u32 slotPtrs[4];     // env mSlotPtrs (sp+0x48)
    ml::CVec3 v38;       // sp+0x38 (slot/camera delta)
    u32 v34, v30, v2c, v28;  // reslist walk values (sp+0x34/0x30/0x2c/0x28)
    u32 v24, v20;        // walk 1: copy/end (sp+0x24/0x20)
    u32 v1c, v18;        // walk 2
    u32 v14, v10;        // walk 3
    u32 vc, v8;          // walk 4
    f32 aa74;            // hoisted to function scope
    f32 range;           // named so it lands in f30 like retail (AA74 takes f31)

    if (checkBitFlag(getSubField7C(self->mSub)) != 0) {
        scnLgtDispatchEnv(getSubField7C(self->mSub), env);
    } else {
        func_80495644(env, 0);
        scnVlLoadValue((CScnVirtualLightData*)&v34, (CScnVirtualLightValueSrc*)&self->res_0C);
        goto check1;
    body1:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)scnVlCopyWord((int*)&v24, (int*)&v34));
        scnVlAdvance(&v34);
    check1:
        scnVlCopyValue(&v20, &self->res_0C);
        if (scnVlWordsNe(&v34, &v20) != 0) {
            goto body1;
        }
        resetLightSlotIndex(env);
        func_80495644(env, 1);
        scnVlLoadValue((CScnVirtualLightData*)&v30, (CScnVirtualLightValueSrc*)&self->res_4C);
        goto check2;
    body2:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)scnVlCopyWord((int*)&v1c, (int*)&v30));
        scnVlAdvance(&v30);
    check2:
        scnVlCopyValue(&v18, &self->res_4C);
        if (scnVlWordsNe(&v30, &v18) != 0) {
            goto body2;
        }
        resetLightSlotIndex(env);
        func_80495644(env, 2);
        scnVlLoadValue((CScnVirtualLightData*)&v2c, (CScnVirtualLightValueSrc*)&self->res_2C);
        goto check3;
    body3:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)scnVlCopyWord((int*)&v14, (int*)&v2c));
        scnVlAdvance(&v2c);
    check3:
        scnVlCopyValue(&v10, &self->res_2C);
        if (scnVlWordsNe(&v2c, &v10) != 0) {
            goto body3;
        }
        resetLightSlotIndex(env);
        func_80495644(env, 3);
        scnVlLoadValue((CScnVirtualLightData*)&v28, (CScnVirtualLightValueSrc*)&self->res_6C);
        goto check4;
    body4:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)scnVlCopyWord((int*)&vc, (int*)&v28));
        scnVlAdvance(&v28);
    check4:
        scnVlCopyValue(&v8, &self->res_6C);
        if (scnVlWordsNe(&v28, &v8) != 0) {
            goto body4;
        }
        resetLightSlotIndex(env);
    }
    if (self->valueCC != 0) {
        u32* p58;
        u32* p68;
        u32* p48;
        u32 cam = scnVlGetCamId((CScnCameraMan*)scnVlGetVal68(self->mSub));
        slotFields[0] = scnVlGetSlotFld(env, 0);
        slotFields[1] = scnVlGetSlotFld(env, 1);
        slotFields[2] = scnVlGetSlotFld(env, 2);
        slotFields[3] = scnVlGetSlotFld(env, 3);
        slotCounts[0] = scnVlGetSlotCnt(env, 0);
        slotCounts[1] = scnVlGetSlotCnt(env, 1);
        slotCounts[2] = scnVlGetSlotCnt(env, 2);
        slotCounts[3] = scnVlGetSlotCnt(env, 3);
        slotPtrs[0] = scnVlGetSlotPtr(env, 0);
        slotPtrs[1] = scnVlGetSlotPtr(env, 1);
        slotPtrs[2] = scnVlGetSlotPtr(env, 2);
        slotPtrs[3] = scnVlGetSlotPtr(env, 3);
        int slotCount = 0;
        aa74 = lbl_eu_8066AA74;
        int j = 0;
        for (;;) {
            if (self->mExtraAlloc[j] != 0) {
                if (scnVlTestArmed((CScnVirtualLightData*)self->mExtraAlloc[j]) != 0) {
                    p58 = &slotCounts[0];
                    p68 = &slotFields[0];
                    p48 = &slotPtrs[0];
                    for (int i = 0; i < 4; i++) {
                        if ((s32)*p68 < (s32)*p58) {
                            if (scnVlTestFlags((CScnVirtualLightData*)self->mExtraAlloc[j],
                                              1u << i) != 0) {
                                noopAnimVec3(&v38);
                                Vec* camPos =
                                    (Vec*)scnVlGetOff10C((void*)cam);
                                scnVlVecSub((Vec*)scnVlGetPad04(
                                                  (void*)self->mExtraAlloc[j]),
                                              (Vec*)&v38, camPos);
                                f32 dist2 = cfCam_vecLenSq(&v38);
                                range = scnVlGetRange((void*)self->mExtraAlloc[j]);
                                if (range * range >= dist2) {
                                    func_804C02E4(
                                        (CLight*)(*p48 + *p68 * 64),
                                        self->mExtraAlloc[j]);
                                    f32 scaled = range * aa74;
                                    if (scaled * scaled < dist2) {
                                        func_804C0928(
                                            (CLight*)(*p48 + *p68 * 64),
                                            (range - safeFSqrt(dist2)) /
                                                (range - scaled));
                                    } else {
                                        func_804C0928(
                                            (CLight*)(*p48 + *p68 * 64),
                                            lbl_eu_8066AA18);
                                    }
                                    (*p68)++;
                                }
                            }
                        }
                        p58++;
                        p68++;
                        p48++;
                    }
                }
                slotCount++;
                if (slotCount >= (int)self->valueCC) break;
            }
            j++;
            if (j >= 0x80) break;
        }
        scnVlSetSlotFld(env, slotFields[0], 0);
        scnVlSetSlotFld(env, slotFields[1], 1);
        scnVlSetSlotFld(env, slotFields[2], 2);
        scnVlSetSlotFld(env, slotFields[3], 3);
    }
}

// Item-destroy helper family used by func_804936AC / scnVlUpdate. noinline
// keeps the retail `bl` at the func_804936AC call sites out-of-line (the
// trivial load bodies would otherwise be inlined, dropping the retail call
// relocs).
__declspec(noinline) int scnVlGetCamId(CScnCameraMan* cam) {
    return CamMan_FindItemA_B158(cam, cam->mCamId);
}

__declspec(noinline) u32 scnVlGetVal68(void* self) { return ((CScnVirtualLightData*)self)->value68; }

extern "C" __declspec(noinline) u32 scnVlGetSlotFld(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1160); }

extern "C" __declspec(noinline) u32 scnVlGetSlotCnt(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1150); }

extern "C" __declspec(noinline) u32 scnVlGetSlotPtr(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1140); }

__declspec(noinline) s32 scnVlTestArmed(CScnVirtualLightData* self) {
    return testFlagMask(&self->value30, 0x10000);
}

__declspec(noinline) s32 scnVlTestFlags(CScnVirtualLightData* self, u32 flags) {
    return testFlagMask(&self->value30, flags);
}

// scnVlVecSub (out = a - b): retail psq_l/ps_sub/psq_st kernel with the
// mr-swap entry and `b .+4` scheduler barrier (MWCC never emits ps from a
// pure scalar subtract; see CScnVirtualLight_ps.inl for the PS backend).
// The scalar body below is the PC/NONMATCHING fallback.
#include "monolib/scn/CScnVirtualLight_ps.inl"

__declspec(noinline) void* scnVlGetPad04(void* self) { return &((CScnVirtualLightData*)self)->_04; }

// Retail: addi r3,r3,0x54; b .+4; addi r3,r3,0xB8; blr. The b .+4 scheduler
// barrier is the documented unreproducible MWCC artifact (MWCC_CASES sec. 4;
// GetTextColor is the only getter precedent and was never byte-matched).
// 2026 probe (~80 shapes, Wii/1.1 + GC/2.6/2.7/3.0a5/3.0a5.2, -O4,p/-O4,s,
// -ipa file/off, -inline auto/on/smart, -proc gekko/750/603/7400, scheduling/
// peephole/opt_propagation/optimization_level/global_optimizer pragmas, goto/
// Retail scnVlGetOff10C is a tail-call thunk: `addi r3,r3,0x54; b func_80493C10`
// into the adjacent getter (the retail annotation originally merged the two
// 0x8-byte bodies into one 0x10 symbol).  MWCC emits the tail call `b` only
// when the getter is a separate noinline leaf placed immediately after.
extern "C" __declspec(noinline) void* func_80493C10(void* self) {
    return (void*)((u8*)self + 0xB8);
}
extern "C" __declspec(noinline) void* scnVlGetOff10C(void* self) {
    return func_80493C10((void*)((u8*)self + 0x54));
}

__declspec(noinline) float scnVlGetRange(void* self) { return ((CScnVirtualLightData*)self)->value3C; }

extern "C" __declspec(noinline) void scnVlSetSlotFld(void* self, u32 val, u32 idx) { *(u32*)((u8*)self + idx*4 + 0x1160) = val; }

// Retail scnVlUpdate: per-frame virtual-light update. Applies the pooled
// light data (`data`) with the given position/fade inputs, then - when the
// pool's flag is clear - pushes the environment's slots into the scene data
// (func_80495AF4). Finally walks the 128 light-handle slots (mExtraAlloc):
// every armed slot within range of `vec` gets a CLight record from the
// data's light array (0x40 stride, at most 8), with distance-based intensity
// falloff toward AA74 * range.
extern "C" void scnVlUpdate(CScnVirtualLight* self, CScnEnvLgtData* data,
                              const ml::CVec3* vec, int mode, f32 f1) {
    CTaskGame_setVec4_tmp v18;
    ml::CVec3 v8;
    u32 enabled = scnVlGetEnabled((void*)self);
    CLight* dst;
    func_804BFA70(data, vec, mode, enabled, f1 + scnVlGetFade((void*)self));
    if (checkBitFlag(getSubField7C(self->mSub)) != 0) {
        scnLgtGatherLights(getSubField7C(self->mSub), f1, data, vec);
    } else {
        scnVlInitVec4(&v18);
        CScnVirtualLightData* p = (CScnVirtualLightData*)scnVlPtrVal14(
            (void*)scnVlGetHandle((void*)scnVlGetVal64((void*)self->mSub)));
        if (scnVlTestBit0((CScnVirtualLightData*)data) != 0) {
            func_80495AF4((CLightEnv*)p, data, 3);
        } else {
            func_80495AF4((CLightEnv*)p, data, 0);
        }
    }
    if (self->valueCC != 0) {
        // Declaration order drives callee-saved coloring (first -> highest):
        // retail colors dst>step>count>i>j descending.
        int step;
        int count;
        int i;
        int j;
        f32 f31v;
        void* base;
        // if/else (not ternary): gives step a two-path merged web like retail.
        if (scnVlTestBit0((CScnVirtualLightData*)data) != 0) {
            step = 8;
        } else {
            step = 1;
        }
        count = (int)scnVlGetCount((void*)data);
        base = scnVlSlotAt((void*)data, 0);
        f31v = lbl_eu_8066AA74;
        dst = (CLight*)((u8*)base + count * 64);
        i = 0;
        j = 0;
        while (true) {
            if (((u32*)self->mExtraAlloc)[j] != 0) {
                if (scnVlTestArmed((CScnVirtualLightData*)((u32*)self->mExtraAlloc)[j]) != 0 &&
                    scnVlTestFlags((CScnVirtualLightData*)((u32*)self->mExtraAlloc)[j],
                                  (u32)step) != 0 &&
                    count < 8) {
                    noopAnimVec3(&v8);
                    scnVlVecSub((Vec*)scnVlGetPad04(
                                      (void*)((u32*)self->mExtraAlloc)[j]),
                                  (Vec*)&v8, (const Vec*)vec);
                    f32 dist2 = cfCam_vecLenSq(&v8);
                    f32 range = f1 + scnVlGetRange(
                                         (void*)((u32*)self->mExtraAlloc)[j]);
                    if (range * range >= dist2) {
                        func_804C02E4(dst, ((u32*)self->mExtraAlloc)[j]);
                        f32 scaled = f31v * range;
                        if (scaled * scaled < dist2) {
                            func_804C0928(dst, (range - safeFSqrt(dist2)) /
                                                   (range - scaled));
                        } else {
                            func_804C0928(dst, lbl_eu_8066AA18);
                        }
                        dst = (CLight*)((u8*)dst + 0x40);
                        count++;
                    }
                }
                i++;
                if (i >= (int)self->valueCC) break;
            }
            j++;
            if (j >= 0x80) break;
        }
    }
    func_804BFEB4(data);
}

__declspec(noinline) float scnVlGetFade(void* self) { return ((CScnVirtualLightData*)self)->valueD4; }

__declspec(noinline) u32 scnVlGetEnabled(void* self) {
    return ((CScnVirtualLightData*)self)->enabled;
}

__declspec(noinline) void* scnVlPtrVal14(void* self) { return &((CScnVirtualLightData*)self)->value14; }

__declspec(noinline) u32 scnVlGetHandle(void* self) { return ((CScnVirtualLightData*)self)->value08; }

__declspec(noinline) u32 scnVlGetVal64(void* self) { return ((CScnVirtualLightData*)self)->value64; }

__declspec(noinline) s32 scnVlTestBit0(CScnVirtualLightData* self) {
    return testFlagMask(&self->value484, 1);
}

__declspec(noinline) u32 scnVlGetCount(void* self) { return ((CScnVirtualLightData*)self)->value480; }

extern "C" __declspec(noinline) void* scnVlSlotAt(void* self, u32 idx){ return (void*)((char*)self + idx * 64); }

// Retail func_80493F08: dispatch a light-environment slot setup on the type
// of the light object at *scnVlNodeItem(node) (+0x2C). The light's position
// comes from the vtable+0x0C virtual (ml::CVec3 by value); each type arms the
// slot's next light through the matching CVirtualLightObj.cpp helper:
// 1 = ambient (func_804952C4), 2 = directional with Euler angles
// (bindOrientedLight), 3 = directional with a direction pointer (bindAttenuatedLight),
// 4 = spot with full params (bindSpotLight). The first parameter is unused
// (retail keeps r3 untouched). The dispatch is an if/else-if compare chain in
// retail (not a jump-table switch), and each case re-fetches the object via
// scnVlNodeItem like retail (the call result is never cached).
extern "C" void func_80493F08(void* unused, CLightEnv* env,
                              CScnVirtualLightNode* node) {
    ml::CVec3 v38;   // sp+0x38 (case 1 position)
    ml::CVec3 v28;   // sp+0x28 (case 2 position)
    ml::CVec3 v18;   // sp+0x18 (case 3 position)
    ml::CVec3 v8;    // sp+0x08 (case 4 position)
    CScnVirtualLightData* obj;
    f32 angleY;
    f32 angleX;
    f32 color;
    void* p34;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    s32 type = scnVlGetType((void*)*scnVlNodeItem(node));
    if (type == 1) {
        obj = (CScnVirtualLightData*)*scnVlNodeItem(node);
        v38 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        func_804952C4(env, &v38);
    } else if (type == 2) {
        obj = (CScnVirtualLightData*)*scnVlNodeItem(node);
        angleY = scnVlGetAngleY((void*)obj);
        angleX = scnVlGetAngleX((void*)obj);
        v28 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        bindOrientedLight(env, (u32)&v28, angleX, angleY);
    } else if (type == 3) {
        obj = (CScnVirtualLightData*)*scnVlNodeItem(node);
        color = scnVlGetColor((void*)obj);
        p34 = scnVlPtrDir((void*)obj);
        v18 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        bindAttenuatedLight(env, (u32)&v18, (u32)p34, color);
    } else if (type == 4) {
        obj = (CScnVirtualLightData*)*scnVlNodeItem(node);
        // Fetch order mirrors retail (+0x48, +0x4C, ptr, +0x44, +0x40); the
        // spot helper receives d,c in f1/f2 and b,a in f3/f4.
        a = scnVlGetSpotA((void*)obj);
        b = scnVlGetSpotB((void*)obj);
        p34 = scnVlPtrSpotDir((void*)obj);
        c = scnVlGetSpotC((void*)obj);
        d = scnVlGetSpotD((void*)obj);
        v8 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        bindSpotLight(env, d, c, &v8, p34, b, a);
    }
}

__declspec(noinline) float scnVlGetAngleX(void* self) { return ((CScnVirtualLightData*)self)->value34; }

__declspec(noinline) float scnVlGetAngleY(void* self) { return ((CScnVirtualLightData*)self)->value38; }

extern "C" void* scnVlPtrDir(void* self) { return (void*)((u8*)self + 0x34); }

extern "C" float scnVlGetColor(void* self) { return ((CScnVirtualLightData*)self)->value40; }

extern "C" float scnVlGetSpotD(void* self) { return ((CScnVirtualLightData*)self)->value40; }

extern "C" float scnVlGetSpotC(void* self) { return ((CScnVirtualLightData*)self)->value44; }

extern "C" void* scnVlPtrSpotDir(void* self) { return (void*)((u8*)self + 0x34); }

extern "C" float scnVlGetSpotB(void* self) { return ((CScnVirtualLightData*)self)->value4C; }

extern "C" float scnVlGetSpotA(void* self) { return ((CScnVirtualLightData*)self)->value48; }

// Allocate a free slot from the 128-slot light pool: find the first handle
// slot whose pointer is null, link it to the record array entry and bump the
// slot count, returning the handle (retail scnVlAllocSlot). The member
// re-reads in the success path (mHandles / mCount) mirror the retail loads.
void* scnVlAllocSlot(CScnVirtualLightPool* self) {
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
// loads around the bl (retail scnVlFreeSlot).
void scnVlFreeSlot(CScnVirtualLightPool* self, CScnVirtualLightPoolSlot* slot) {
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

// Advance the light blend: bump the +0xBC factor toward a clamp (add+upper
// bound when `flag` is set, subtract+lower bound otherwise), then blend the
// two position vec4s (+0x9C scaled by t, +0x8C scaled by 1-t) into +0xAC
// (retail scnVlBlendStep).
void scnVlBlendStep(CScnVirtualLightData* self, int flag) {
    if (flag != 0) {
        // Declaration order fixes fpr assignment (lim=f0, step=f1,
        // cur=f2); initializing cur before step puts the member load first
        // in the instruction stream, as retail.
        f32 lim = lbl_eu_8066AA18;
        f32 step;
        f32 cur = self->field_0xBC;
        step = lbl_eu_8066AA78;
        f32 v = cur + step;
        self->field_0xBC = v;
        if (v > lim) {
            self->field_0xBC = lim;
        }
    } else {
        f32 lim = lbl_eu_8066AA24;
        f32 step;
        f32 cur = self->field_0xBC;
        step = lbl_eu_8066AA78;
        f32 v = cur - step;
        self->field_0xBC = v;
        if (v < lim) {
            self->field_0xBC = lim;
        }
    }
    ml::CVec4 v18;
    ml::CVec4 v8;
    func_80058BD8(&v18, (const ml::CVec4*)&self->field_0x9C, self->field_0xBC);
    copyWord4((u32*)&self->field_0xAC, (const u32*)&v18);
    func_80058BD8(&v8, (const ml::CVec4*)&self->field_0x8C, lbl_eu_8066AA18 - self->field_0xBC);
    scnVlVec4Add((f32*)&self->field_0xAC, (const f32*)&v8);
}

// Retail scnVlVec4Add: vec4 += (a[i] += b[i]). noinline keeps the bl from
// scnVlBlendStep out-of-line (retail calls it; the trivial body would otherwise
// be inlined, dropping the call-site reloc).
__declspec(noinline) void scnVlVec4Add(f32* a, const f32* b) {
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
    a[3] += b[3];
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static-init for the retail-owned vec4 object lbl_eu_80658648: fills it with
// (v, v, v, w) from the two sdata2 constants and tail-calls the vec4 setter.
void sinit_80494300() {
    CTaskGame_setVec4(&lbl_eu_80658648, lbl_eu_8066AA14, lbl_eu_8066AA14,
                  lbl_eu_8066AA14, lbl_eu_8066AA18);
}

#pragma push
#pragma auto_inline off
// clearList for the virtual-light reslist (retail scnVlClearList): walk the
// ring from the first node, destroy each node's item (scnVlItemNop) and
// reset its mNext (scnVlZeroNext), then relink the sentinel onto itself
// (reslist.hpp clearList shape).
extern "C" void scnVlClearList(CScnVirtualLightReslist* self) {
    CScnVirtualLightNode* node = self->mStartNodePtr->mNext;
    while (node != self->mStartNodePtr) {
        CScnVirtualLightNode* cur = node;
        node = node->mNext;
        scnVlItemNop(&cur->mItem);
        scnVlZeroNext(self, cur);
    }
    self->mStartNodePtr->mNext = self->mStartNodePtr;
    self->mStartNodePtr->mPrev = self->mStartNodePtr;
}
// reserve the reslist node array (retail scnVlAllocNodes): allocate count*0xC
// bytes via MemManager::allocate_array, zero the mNext of the first `count`
// nodes (MWCC auto-unrolls the fill loop 8x with its overflow guards), then
// set the capacity. `handle` is passed through untouched to allocate_array.
extern "C" void scnVlAllocNodes(CScnVirtualLightReslist* self, u32 handle, int capacity) {
    self->mList = (CScnVirtualLightNode*)mtl::MemManager::allocate_array((u32)capacity * 0xC, handle);
    for (int i = 0; i < capacity; i++) {
        self->mList[i].mNext = nullptr;
    }
    self->mCapacity = capacity;
}
#pragma pop
// Retail func_804923F8: scene-light reset. arg == 0 destroys every light
// object held by the four reslists (virtual deleting-dtor dispatch at vtable
// +0x8), clears each ring, zeroes the pool bound count and the 128-slot
// handle array. arg != 0 instead removes the single node whose item equals
// *arg, searching the four reslists in order and unlinking it via
// scnVlUnlinkNode. Frame slots mirror retail (first-declared -> higher
// offset); the destroy sequence re-calls scnVlNodeItem at every step like
// retail (the call result is never cached).
extern "C" void func_804923F8(CScnVirtualLight* self, int arg) {
    u32 it0;     // 0x88
    u32 it1;     // 0x84
    u32 it2;     // 0x80
    u32 it3;     // 0x7C
    u32 found;   // 0x78
    u32 e0;      // 0x74
    u32 e1;      // 0x70
    u32 e2;      // 0x6C
    u32 e3;      // 0x68
    u32 itA;     // 0x64
    u32 endA;    // 0x60
    u32 endA2;   // 0x5C
    u32 outA;    // 0x58
    u32 cpA;     // 0x54
    u32 foundB;  // 0x50
    u32 itB;     // 0x4C
    u32 endB;    // 0x48
    u32 endB2;   // 0x44
    u32 outB;    // 0x40
    u32 cpB;     // 0x3C
    u32 foundC;  // 0x38
    u32 itC;     // 0x34
    u32 endC;    // 0x30
    u32 endC2;   // 0x2C
    u32 outC;    // 0x28
    u32 cpC;     // 0x24
    u32 foundD;  // 0x20
    u32 itD;     // 0x1C
    u32 endD;    // 0x18
    u32 endD2;   // 0x14
    u32 outD;    // 0x10
    u32 cpD;     // 0x0C

    if (arg == 0) {
        // ---- clear walk: res_0C ----
        scnVlLoadValue((CScnVirtualLightData*)&it0, (CScnVirtualLightValueSrc*)&self->res_0C);
        goto check0;
    body0:
        if (*(u32*)scnVlNodeItem(&it0) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&it0));
            *(u32*)scnVlNodeItem(&it0) = 0;
        }
        scnVlAdvance(&it0);
    check0:
        scnVlCopyValue(&e0, &self->res_0C);
        if (scnVlWordsNe(&it0, &e0) != 0) {
            goto body0;
        }
        scnVlClearThunk(&self->res_0C);
        // ---- res_2C ----
        scnVlLoadValue((CScnVirtualLightData*)&it1, (CScnVirtualLightValueSrc*)&self->res_2C);
        goto check1;
    body1:
        if (*(u32*)scnVlNodeItem(&it1) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&it1));
            *(u32*)scnVlNodeItem(&it1) = 0;
        }
        scnVlAdvance(&it1);
    check1:
        scnVlCopyValue(&e1, &self->res_2C);
        if (scnVlWordsNe(&it1, &e1) != 0) {
            goto body1;
        }
        scnVlClearThunk(&self->res_2C);
        // ---- res_4C ----
        scnVlLoadValue((CScnVirtualLightData*)&it2, (CScnVirtualLightValueSrc*)&self->res_4C);
        goto check2;
    body2:
        if (*(u32*)scnVlNodeItem(&it2) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&it2));
            *(u32*)scnVlNodeItem(&it2) = 0;
        }
        scnVlAdvance(&it2);
    check2:
        scnVlCopyValue(&e2, &self->res_4C);
        if (scnVlWordsNe(&it2, &e2) != 0) {
            goto body2;
        }
        scnVlClearThunk(&self->res_2C);
        // ---- res_6C ----
        scnVlLoadValue((CScnVirtualLightData*)&it3, (CScnVirtualLightValueSrc*)&self->res_6C);
        goto check3;
    body3:
        if (*(u32*)scnVlNodeItem(&it3) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&it3));
            *(u32*)scnVlNodeItem(&it3) = 0;
        }
        scnVlAdvance(&it3);
    check3:
        scnVlCopyValue(&e3, &self->res_6C);
        if (scnVlWordsNe(&it3, &e3) != 0) {
            goto body3;
        }
        scnVlClearThunk(&self->res_6C);

        self->valueCC = 0;
        memset(self->mExtraAlloc, 0, 0x200);
        return;
    }

    // ---- arg != 0: remove the single node holding *arg ----
    // res_0C: find writes straight into `found`.
    scnVlCopyValue(&endA, &self->res_0C);
    scnVlLoadValue((CScnVirtualLightData*)&itA, (CScnVirtualLightValueSrc*)&self->res_0C);
    scnVlFindNode((int*)&found, &itA, &endA, (const u32*)&arg);
    scnVlCopyValue(&endA2, &self->res_0C);
    if (scnVlWordsNe(&found, &endA2) != 0) {
        if (*(u32*)scnVlNodeItem(&found) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&found));
            *(u32*)scnVlNodeItem(&found) = 0;
        }
        scnVlCopyWord((int*)&cpA, (int*)&found);
        scnVlUnlinkNode((CScnVirtualLightData*)&outA, &self->res_0C,
                      (CScnVirtualLightNode**)&cpA);
        return;
    }

    // res_2C: find result is staged in foundB, then copied into `found`.
    scnVlCopyValue(&endB, &self->res_2C);
    scnVlLoadValue((CScnVirtualLightData*)&itB, (CScnVirtualLightValueSrc*)&self->res_2C);
    scnVlFindNode((int*)&foundB, &itB, &endB, (const u32*)&arg);
    scnVlStoreWord((int*)&found, (int*)&foundB);
    scnVlCopyValue(&endB2, &self->res_2C);
    if (scnVlWordsNe(&found, &endB2) != 0) {
        if (*(u32*)scnVlNodeItem(&found) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&found));
            *(u32*)scnVlNodeItem(&found) = 0;
        }
        scnVlCopyWord((int*)&cpB, (int*)&found);
        scnVlUnlinkNode((CScnVirtualLightData*)&outB, &self->res_2C,
                      (CScnVirtualLightNode**)&cpB);
        return;
    }

    // res_4C
    scnVlCopyValue(&endC, &self->res_4C);
    scnVlLoadValue((CScnVirtualLightData*)&itC, (CScnVirtualLightValueSrc*)&self->res_4C);
    scnVlFindNode((int*)&foundC, &itC, &endC, (const u32*)&arg);
    scnVlStoreWord((int*)&found, (int*)&foundC);
    scnVlCopyValue(&endC2, &self->res_4C);
    if (scnVlWordsNe(&found, &endC2) != 0) {
        if (*(u32*)scnVlNodeItem(&found) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&found));
            *(u32*)scnVlNodeItem(&found) = 0;
        }
        scnVlCopyWord((int*)&cpC, (int*)&found);
        scnVlUnlinkNode((CScnVirtualLightData*)&outC, &self->res_4C,
                      (CScnVirtualLightNode**)&cpC);
        return;
    }

    // res_6C
    scnVlCopyValue(&endD, &self->res_6C);
    scnVlLoadValue((CScnVirtualLightData*)&itD, (CScnVirtualLightValueSrc*)&self->res_6C);
    scnVlFindNode((int*)&foundD, &itD, &endD, (const u32*)&arg);
    scnVlStoreWord((int*)&found, (int*)&foundD);
    scnVlCopyValue(&endD2, &self->res_6C);
    if (scnVlWordsNe(&found, &endD2) != 0) {
        if (*(u32*)scnVlNodeItem(&found) != 0) {
            delete (CVirtualLightDispatch*)(*(void**)scnVlNodeItem(&found));
            *(u32*)scnVlNodeItem(&found) = 0;
        }
        scnVlCopyWord((int*)&cpD, (int*)&found);
        scnVlUnlinkNode((CScnVirtualLightData*)&outD, &self->res_6C,
                      (CScnVirtualLightNode**)&cpD);
        return;
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.rodata] 0x80524030-0x80524050 (32B): RTTI name "CScnVirtualLight" (20B)
// + "VLGT Mem" alloc-region tag (12B, declared extern in CScnVirtualLight.hpp).
extern const char lbl_eu_80524030[20] = {
    0x43,0x53,0x63,0x6E,0x56,0x69,0x72,0x74,0x75,0x61,0x6C,0x4C,0x69,0x67,0x68,0x74,
    0x00,0x00,0x00,0x00,
};
const char lbl_eu_80524044[] = {
    'V','L','G','T',' ','M','e','m','\0','\0','\0','\0',
};

// [.bss] 0x80658648-0x80658658 (16B): shared vec4 written by sinit_80494300
// (declared extern in CScnVirtualLight.hpp).
CTaskGame_setVec4_tmp lbl_eu_80658648;

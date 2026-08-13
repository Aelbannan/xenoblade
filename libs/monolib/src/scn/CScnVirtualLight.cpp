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

extern "C" __declspec(noinline) void func_80492030(void* self) {}

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
// three blend vec4s (func_80492030), arms the enabled byte, then fetches the
// pool's ALLOC_HANDLE through func_8048C8BC + the vtable+0x2C virtual and
// creates the manager region. The reslists reserve 8/8/8/2 nodes, the
// CLight[0x80] / LightObj[0x80] arrays are built with __construct_new_array
// and cross-linked via func_804C0398, the 128-slot allocation is created,
// and the lights are initialized by func_80492B40.
extern "C" CScnVirtualLight* __ct__CScnVirtualLight(CScnVirtualLight* self,
                                                    CScnVirtualLightSrc* src) {
    *(void**)self = (void*)lbl_eu_8056E868;
    self->mSub = src;
    self->value08 = (u32)-1;
    __ct__80492074(&self->res_0C);
    __ct__80492074(&self->res_2C);
    __ct__80492074(&self->res_4C);
    __ct__80492074(&self->res_6C);
    func_80492030(&self->field_0x8C);
    func_80492030(&self->field_0x9C);
    func_80492030(&self->field_0xAC);
    f32 d4 = lbl_eu_8066AA10;
    u8 en = 1;
    self->enabled = en;
    self->valueD4 = d4;
    u32 handle = ((CScnVirtualLightHandleSrc*)func_8048C8BC(src))->GetHandle();
    self->value08 = mtl::MemManager::create(handle, 0x10000, lbl_eu_80524044);
    func_80492168(&self->res_0C, self->value08, 8);
    func_80492168(&self->res_2C, self->value08, 8);
    func_80492168(&self->res_4C, self->value08, 8);
    func_80492168(&self->res_6C, self->value08, 2);
    self->mCLights = (CLight*)__construct_new_array(
        mtl::MemManager::allocate_array(0x2010, self->value08),
        &__ct__6CLightFv, &__dt__6CLightFv, 0x40, 0x80);
    self->mLightObjs = (nw4r::g3d::LightObj*)__construct_new_array(
        mtl::MemManager::allocate_array(0x2210, self->value08),
        &__ct__Q34nw4r3g3d8LightObjFv, &__dt__Q34nw4r3g3d8LightObjFv, 0x44,
        0x80);
    int i = 0;
    do {
        func_804C0398(&self->mCLights[i], &self->mLightObjs[i]);
        i++;
    } while (i < 0x80);
    self->mExtraAlloc = (u32*)mtl::MemManager::allocate_head(self->value08, 0x200, 4);
    func_80492B40(self);
    return self;
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

extern "C" void func_8049216C(CScnVirtualLightReslist* self, u32 handle, int capacity);
extern "C" __declspec(noinline) void func_80492168(void* self, u32 handle, int capacity) {
    func_8049216C((CScnVirtualLightReslist*)self, handle, capacity);
}


// Base reslist teardown helper (retail func_804920E0; defined at the bottom
// of this TU under #pragma auto_inline off).
extern "C" void func_804920E0(CScnVirtualLightReslist* self);

extern "C" void __dt__804923A0(CScnVirtualLightReslist* obj);
extern "C" __declspec(noinline) void func_8049239C(CScnVirtualLightReslist* self) {
    __dt__804923A0(self);
}

// Destroy the scene light manager: reset via func_804923F8, destroyList the
// four reslists (func_8049239C), free the CLight/LightObj arrays and the
// extra allocation, erase the resource id, then run each reslist's explicit
// dtor (retail __dt__reslist_CVirtualLightObj, flat-name import) in reverse
// member order. The vtable (lbl_eu_8056E868) is stored manually because the
// class is __declspec(novtable); the virtual dtor supplies the deleting
// epilogue (flag > 0 -> __dl__FPv).
CScnVirtualLight::~CScnVirtualLight() {
    *(void**)this = (void*)lbl_eu_8056E868;
    func_804923F8(this, 0);
    func_8049239C(&res_0C);
    func_8049239C(&res_4C);
    func_8049239C(&res_2C);
    func_8049239C(&res_6C);
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
// `b __dt__804923A0` from func_8049239C (retail keeps it out-of-line).
extern "C" __declspec(noinline) void __dt__804923A0(CScnVirtualLightReslist* obj) {
    func_804920E0(obj);
    if (obj->field_0x1C == false && obj->mList != nullptr) {
        delete[] obj->mList;
        obj->mList = nullptr;
    }
    obj->mCapacity = 0;
}


// Walk the ring from `b` while the value at func_80492A64(b) differs from
// *d (func_80492A88 tests the pair, func_80492A70 advances), then copy `b`
// into `a` (retail func_804929C0).
// Walk the ring from `b` while the value at func_80492A64(b) differs from
// *d (func_80492A88 tests the pair, func_80492A70 advances), then copy `b`
// into `a` (retail func_804929C0). The body/check/end labels mirror the
// retail loop rotation (body below the head with an initial jump over); the
// `cur` local keeps MWCC's cmplw operand order (rA = the call-derived value).
void func_804929C0(int* a, u32* b, const u32* c, const u32* d) {
    goto check;
body:
    func_80492A70(b);
check:
    if (!func_80492A88(b, c)) {
        goto end;
    }
    u32 cur = *func_80492A64(b);
    if (cur == *d) {
        goto end;
    }
    goto body;
end:
    func_80492AA8(a, (int*)b);
}

extern "C" __declspec(noinline) void func_80492A5C(void* self, u32 val) { ((CScnVirtualLightData*)self)->value00 = val; }

__declspec(noinline) void func_80492A50(CScnVirtualLightData* self, CScnVirtualLightValueSrc* src) {
    func_80492A5C(self, *src->mValue);
}

extern "C" __declspec(noinline) u32* func_80492A64(void* self){ return (u32*)((char*)*(void**)self + 8); }

extern "C" __declspec(noinline) void func_80492A70(u32* self) { *self = *(u32*)(*(u32**)self); }

// retail: lwz r4,0x4(r4); b func_80492A5C (r3 passes through). noinline
// keeps the bl from func_80493148 out-of-line (retail calls it).
extern "C" __declspec(noinline) void func_80492A80(void* self, void* src) { func_80492A5C(self, *(u32*)((char*)src + 4)); }

// Word-compare helper (retail func_80492A88): returns *a != *b. noinline
// keeps the bl from func_804929C0 out-of-line (retail calls it).
__declspec(noinline) u32 func_80492A88(const u32* a, const u32* b) {
    return *a != *b;
}

extern "C" void func_804920E0(CScnVirtualLightReslist* self);
extern "C" void func_80492AA4(void* self) { func_804920E0((CScnVirtualLightReslist*)self); }

extern "C" __declspec(noinline) int* func_80492AA8(int* dst, int* src){
    *dst = *src;
    return dst;
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

// Retail func_80492B40: reset the manager (func_804923F8), then build the
// light slots. Each slot is a pair of vector builders - a vec3 direction
// (func_8004B60C) and a vec4 color (func_800407C8) - feeding
// func_80492DB8 (ambient) / func_80492DC4 (directional); the +0x4C/+0x2C/
// +0x6C reslists share the lbl_eu_80658648 vec4 (func_80492DD4/DE0/DFC).
// Ends by seeding the blend fields (+0x8C/+0x9C/+0xAC) with the AA14 vec4
// and the +0xBC factor with AA24.
extern "C" void func_80492B40(CScnVirtualLight* self) {
    ml::CVec3* p_18;
    func_800407C8_tmp v_d8;
    func_800407C8_tmp v_c8;
    ml::CVec3 v_b8;
    func_800407C8_tmp v_a8;
    ml::CVec3 v_98;
    func_800407C8_tmp v_88;
    ml::CVec3 v_78;
    func_800407C8_tmp v_68;
    ml::CVec3 v_58;
    func_800407C8_tmp v_48;
    ml::CVec3 v_38;
    func_800407C8_tmp v_28;
    ml::CVec3 v_18;
    func_800407C8_tmp v_8;
    ml::CVec3* p_b8;
    ml::CVec3* p_98;
    ml::CVec3* p_78;
    ml::CVec3* p_58;
    ml::CVec3* p_38;

    func_804923F8(self, 0);
    func_80492DB8((CScnVirtualLightData*)self,
                  func_800407C8(&v_d8, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  lbl_eu_8066AA18);
    p_b8 = func_8004B60C(&v_b8, lbl_eu_8066AA1C, lbl_eu_8066AA20,
                         lbl_eu_8066AA1C);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_c8, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)p_b8, lbl_eu_8066AA18);
    p_98 = func_8004B60C(&v_98, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_a8, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)p_98, lbl_eu_8066AA18);
    p_78 = func_8004B60C(&v_78, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_88, lbl_eu_8066AA24, lbl_eu_8066AA24,
                                lbl_eu_8066AA24, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)p_78, lbl_eu_8066AA18);
    p_58 = func_8004B60C(&v_58, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_68, lbl_eu_8066AA24, lbl_eu_8066AA24,
                                lbl_eu_8066AA24, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)p_58, lbl_eu_8066AA18);
    p_38 = func_8004B60C(&v_38, lbl_eu_8066AA20, lbl_eu_8066AA20,
                         lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_48, lbl_eu_8066AA24, lbl_eu_8066AA24,
                                lbl_eu_8066AA24, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)p_38, lbl_eu_8066AA18);
    func_80492DD4((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    func_80492DE0((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    p_18 = func_8004B60C(&v_18, lbl_eu_8066AA34, lbl_eu_8066AA10,
                         lbl_eu_8066AA34);
    func_80492DEC((CScnVirtualLightData*)self,
                  func_800407C8(&v_28, lbl_eu_8066AA28, lbl_eu_8066AA2C,
                                lbl_eu_8066AA30, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)p_18, lbl_eu_8066AA18);
    func_80492DFC((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    func_800407C8_tmp* p_8 = func_800407C8(
        &v_8, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA18);
    u32* d = copyWord4((u32*)&self->field_0xAC, (const u32*)p_8);
    d = copyWord4((u32*)&self->field_0x9C, d);
    copyWord4((u32*)&self->field_0x8C, d);
    self->field_0xBC = lbl_eu_8066AA24;
}

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
// The direction-vector -> angle conversion keeps the sqrt(x^2+z^2) call even
// though its result is unused (extern call, may warn) - retail computes it
// into the dead f2 slot.
extern "C" __declspec(noinline) void func_804933AC(CScnVirtualLightData* self, u8* field,
                              const func_800407C8_tmp* data, const func_800407C8_tmp* val,
                              f32 value) {
    // Direction-vector -> dir-light angles. The sqrt(z^2+x^2) result is unused
    // by the call; retail still keeps the call (it may warn) and the dead fmr
    // of the result (the two dead artifacts - the hoisted z load and the fmr -
    // resist source-level reproduction).
    f32 t1 = func_8006D410(-func_8004CC40(-val->unk00[0]));
    f32 len = func_8004EC78(val->unk00[2] * val->unk00[2] + val->unk00[0] * val->unk00[0]);
    f32 t2 = func_8006D410(func_8004CC40(-val->unk00[1]));
    func_80493300(self, field, data, value, t1, t2);
}

__declspec(noinline) void func_80492DB8(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, &self->_0C[0], arg, val);
}

// func_804933AC variant: also passes a 3-float `val` (direction) in r6, and an
// f32 scalar in f1 (see func_804933AC's own body for how it consumes them).
__declspec(noinline) void func_80492DC4(CScnVirtualLightData* self, const func_800407C8_tmp* arg,
                   const func_800407C8_tmp* val, f32 value) {
    func_804933AC(self, &self->_0C[0], arg, val, value);
}

__declspec(noinline) void func_80492DD4(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, (u8*)&self->value4C, arg, val);
}

__declspec(noinline) void func_80492DE0(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, (u8*)&self->value2C, arg, val);
}

__declspec(noinline) void func_80492DEC(CScnVirtualLightData* self, const func_800407C8_tmp* arg,
                   const func_800407C8_tmp* val, f32 value) {
    func_804933AC(self, (u8*)&self->value2C, arg, val, value);
}

__declspec(noinline) void func_80492DFC(CScnVirtualLightData* self, const func_800407C8_tmp* arg, f32 val) {
    func_804930BC(self, &self->_6C[0], arg, val);
}

// Retail func_80492E08: richer sibling of func_80492B40 - five directional
// slots built from (vec3 direction via func_8004B60C, vec3 copy via
// func_8004B75C, vec4 color via func_800407C8) before the shared
// lbl_eu_80658648 vec4 feeds func_80492DD4/DE0/DFC, plus the same blend
// seed tail. The local declaration order mirrors the retail stack layout
// (vec4 then the direction/copy vec3 pair, top-down).
extern "C" void func_80492E08(CScnVirtualLight* self) {
    func_800407C8_tmp v100;
    func_800407C8_tmp v_f0;
    ml::CVec3 v_e4;
    ml::CVec3 v_d8;
    func_800407C8_tmp v_c8;
    ml::CVec3 v_bc;
    ml::CVec3 v_b0;
    func_800407C8_tmp v_a0;
    ml::CVec3 v_94;
    ml::CVec3 v_88;
    func_800407C8_tmp v_78;
    ml::CVec3 v_6c;
    ml::CVec3 v_60;
    func_800407C8_tmp v_50;
    ml::CVec3 v_44;
    ml::CVec3 v_38;
    func_800407C8_tmp v_28;
    ml::CVec3 v_18;
    func_800407C8_tmp v_8;

    func_804923F8(self, 0);
    func_80492DB8((CScnVirtualLightData*)self,
                  func_800407C8(&v100, lbl_eu_8066AA14, lbl_eu_8066AA14,
                                lbl_eu_8066AA14, lbl_eu_8066AA18),
                  lbl_eu_8066AA18);
    func_8004B75C(&v_e4, func_8004B60C(&v_d8, lbl_eu_8066AA3C, lbl_eu_8066AA40,
                                        lbl_eu_8066AA3C),
                  lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_f0, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)&v_e4, lbl_eu_8066AA18);
    func_8004B75C(&v_bc, func_8004B60C(&v_b0, lbl_eu_8066AA44, lbl_eu_8066AA48,
                                        lbl_eu_8066AA44),
                  lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_c8, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)&v_bc, lbl_eu_8066AA18);
    func_8004B75C(&v_94, func_8004B60C(&v_88, lbl_eu_8066AA4C, lbl_eu_8066AA50,
                                        lbl_eu_8066AA54),
                  lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_a0, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)&v_94, lbl_eu_8066AA18);
    func_8004B75C(&v_6c, func_8004B60C(&v_60, lbl_eu_8066AA58, lbl_eu_8066AA5C,
                                        lbl_eu_8066AA60),
                  lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_78, lbl_eu_8066AA38, lbl_eu_8066AA38,
                                lbl_eu_8066AA38, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)&v_6c, lbl_eu_8066AA18);
    func_8004B75C(&v_44, func_8004B60C(&v_38, lbl_eu_8066AA68, lbl_eu_8066AA6C,
                                        lbl_eu_8066AA70),
                  lbl_eu_8066AA20);
    func_80492DC4((CScnVirtualLightData*)self,
                  func_800407C8(&v_50, lbl_eu_8066AA64, lbl_eu_8066AA64,
                                lbl_eu_8066AA64, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)&v_44, lbl_eu_8066AA18);
    func_80492DD4((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    func_80492DE0((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    ml::CVec3* p_18 = func_8004B60C(&v_18, lbl_eu_8066AA34, lbl_eu_8066AA10,
                                    lbl_eu_8066AA34);
    func_80492DEC((CScnVirtualLightData*)self,
                  func_800407C8(&v_28, lbl_eu_8066AA28, lbl_eu_8066AA2C,
                                lbl_eu_8066AA30, lbl_eu_8066AA18),
                  (const func_800407C8_tmp*)p_18, lbl_eu_8066AA18);
    func_80492DFC((CScnVirtualLightData*)self, &lbl_eu_80658648, lbl_eu_8066AA18);
    func_800407C8_tmp* p_8 = func_800407C8(
        &v_8, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA14, lbl_eu_8066AA18);
    u32* d = copyWord4((u32*)&self->field_0xAC, (const u32*)p_8);
    d = copyWord4((u32*)&self->field_0x9C, d);
    copyWord4((u32*)&self->field_0x8C, d);
    self->field_0xBC = lbl_eu_8066AA24;
}

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

// Creates a CVirtualLightDir for the given light-data field and initializes it
// from `data` (4 words), `val1` and the two direction floats, returning the
// new light. Sibling of func_804930BC (amb variant); the if/else phi shape
// pins the store to the merge point like retail. noinline keeps the bl from
// func_804933AC out-of-line (retail calls it).
__declspec(noinline) CVirtualLightDir* func_80493300(CScnVirtualLightData* self, u8* field, const func_800407C8_tmp* data,
                   f32 val1, f32 val2, f32 val3) {
    CVirtualLightDir* ptr;
    void* mem = mtl::MemManager::allocate(0x3C, self->value08);
    if (mem != 0) {
        ptr = __ct__CVirtualLightDir((CVirtualLightDir*)mem);
    } else {
        ptr = (CVirtualLightDir*)mem;
    }
    func_80493148((CScnVirtualLightData*)field, (u32)&ptr);
    copyWord4Offset((u32*)ptr, (const u32*)data);
    func_80493140(ptr, val1);
    func_8049474C(ptr, val2, val3);
    return ptr;
}

// Sibling of func_804935C0 over the +0x4C reslist: applies `value`/`src` to
// every armed dir light, then stores the scaled vec4 into the +0x8C blend
// target and copies that into the +0xAC blend-current field.
void func_8049347C(CScnVirtualLight* self, const ml::CVec4* src, f32 value) {
    ml::CVec4 v18;                  // sp+0x18
    CScnVirtualLightNode* it;       // sp+0x14
    CScnVirtualLightNode* itTmp;    // sp+0x10
    CScnVirtualLightNode* end;      // sp+0xC
    CScnVirtualLightNode* endTmp;   // sp+0x8
    func_80492A50((CScnVirtualLightData*)&itTmp, (CScnVirtualLightValueSrc*)&self->res_4C);
    func_8049357C((int*)&it, (int*)&itTmp);
    while ((func_80492A80(&endTmp, (CScnVirtualLightValueSrc*)&self->res_4C),
            func_804935A4((const u32*)&it, (const u32*)func_8049357C((int*)&end, (int*)&endTmp))) != 0) {
        if (func_80493574(*(CVirtualLightObj**)func_80493588(&it)) == 1) {
            copyWord4Offset((u32*)*(CVirtualLightObj**)func_80493588(&it), (const u32*)src);
            func_80493140(*(CVirtualLightObj**)func_80493588(&it), value);
        }
        func_80493594((u32*)&it);
    }
    func_80058BD8(&v18, src, value);
    copyWord4((u32*)&self->field_0x8C, (const u32*)&v18);
    copyWord4((u32*)&self->field_0xAC, (const u32*)&self->field_0x8C);
}

__declspec(noinline) s32 func_80493574(void* self) { return ((CScnVirtualLightData*)self)->value2C; }

extern "C" __declspec(noinline) int* func_8049357C(int* dst, int* src){
    *dst = *src;
    return dst;
}

extern "C" __declspec(noinline) void* func_80493588(void* self){ return (void*)((char*)*(void**)self + 8); }

extern "C" __declspec(noinline) void func_80493594(u32* self) { *self = *(u32*)(*(u32**)self); }

__declspec(noinline) s32 func_804935A4(const u32* a, const u32* b) {
    return *a != *b;
}

// Apply `value` and the source vec4 `src` to every armed dir light in the
// +0x6C reslist (item +0x2C == 1), then copy the scaled vec4 into the +0x9C
// blend-start field. Iterates the reslist ring with the iterator-helper calls
// as retail does: begin via func_80492A50 + word copy, end re-fetched every
// iteration (func_80492A80 + copy), advance func_80493594, deref func_80493588.
// The item is re-dereferenced at every use (retail calls func_80493588 three
// times; MWCC cannot CSE the call across the other calls).
void func_804935C0(CScnVirtualLight* self, const ml::CVec4* src, f32 value) {
    ml::CVec4 v18;                  // sp+0x18
    CScnVirtualLightNode* it;       // sp+0x14
    CScnVirtualLightNode* itTmp;    // sp+0x10
    CScnVirtualLightNode* end;      // sp+0xC
    CScnVirtualLightNode* endTmp;   // sp+0x8
    func_80492A50((CScnVirtualLightData*)&itTmp, (CScnVirtualLightValueSrc*)&self->res_6C);
    func_8049357C((int*)&it, (int*)&itTmp);
    while ((func_80492A80(&endTmp, (CScnVirtualLightValueSrc*)&self->res_6C),
            func_804935A4((const u32*)&it, (const u32*)func_8049357C((int*)&end, (int*)&endTmp))) != 0) {
        if (func_80493574(*(CVirtualLightObj**)func_80493588(&it)) == 1) {
            copyWord4Offset((u32*)*(CVirtualLightObj**)func_80493588(&it), (const u32*)src);
            func_80493140(*(CVirtualLightObj**)func_80493588(&it), value);
        }
        func_80493594((u32*)&it);
    }
    func_80058BD8(&v18, src, value);
    copyWord4((u32*)&self->field_0x9C, (const u32*)&v18);
}

// Retail func_804936AC: per-frame environment push. When the pool's flag bit
// is set, forwards (sub, env) to func_804C1338. Otherwise re-selects each of
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

    if (checkBitFlag(getSubField7C(self->mSub)) != 0) {
        func_804C1338(getSubField7C(self->mSub), env);
    } else {
        func_80495644(env, 0);
        func_80492A50((CScnVirtualLightData*)&v34, (CScnVirtualLightValueSrc*)&self->res_0C);
        goto check1;
    body1:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)func_80492AA8((int*)&v24, (int*)&v34));
        func_80492A70(&v34);
    check1:
        func_80492A80(&v20, &self->res_0C);
        if (func_80492A88(&v34, &v20) != 0) {
            goto body1;
        }
        func_804956F8(env);
        func_80495644(env, 1);
        func_80492A50((CScnVirtualLightData*)&v30, (CScnVirtualLightValueSrc*)&self->res_4C);
        goto check2;
    body2:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)func_80492AA8((int*)&v1c, (int*)&v30));
        func_80492A70(&v30);
    check2:
        func_80492A80(&v18, &self->res_4C);
        if (func_80492A88(&v30, &v18) != 0) {
            goto body2;
        }
        func_804956F8(env);
        func_80495644(env, 2);
        func_80492A50((CScnVirtualLightData*)&v2c, (CScnVirtualLightValueSrc*)&self->res_2C);
        goto check3;
    body3:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)func_80492AA8((int*)&v14, (int*)&v2c));
        func_80492A70(&v2c);
    check3:
        func_80492A80(&v10, &self->res_2C);
        if (func_80492A88(&v2c, &v10) != 0) {
            goto body3;
        }
        func_804956F8(env);
        func_80495644(env, 3);
        func_80492A50((CScnVirtualLightData*)&v28, (CScnVirtualLightValueSrc*)&self->res_6C);
        goto check4;
    body4:
        func_80493F08(self, env,
                      (CScnVirtualLightNode*)func_80492AA8((int*)&vc, (int*)&v28));
        func_80492A70(&v28);
    check4:
        func_80492A80(&v8, &self->res_6C);
        if (func_80492A88(&v28, &v8) != 0) {
            goto body4;
        }
        func_804956F8(env);
    }
    if (self->valueCC != 0) {
        u32* p58;
        u32* p68;
        u32* p48;
        f32 aa74;
        f32 range;
        f32 dist2;
        f32 scaled;
        u32 cam = func_80493B78((CScnCameraMan*)func_80493B80(self->mSub));
        slotFields[0] = func_80493B88(env, 0);
        slotFields[1] = func_80493B88(env, 1);
        slotFields[2] = func_80493B88(env, 2);
        slotFields[3] = func_80493B88(env, 3);
        slotCounts[0] = func_80493B98(env, 0);
        slotCounts[1] = func_80493B98(env, 1);
        slotCounts[2] = func_80493B98(env, 2);
        slotCounts[3] = func_80493B98(env, 3);
        slotPtrs[0] = func_80493BA8(env, 0);
        slotPtrs[1] = func_80493BA8(env, 1);
        slotPtrs[2] = func_80493BA8(env, 2);
        slotPtrs[3] = func_80493BA8(env, 3);
        int slotCount = 0;
        aa74 = lbl_eu_8066AA74;
        int j = 0;
        for (;;) {
            if (self->mExtraAlloc[j] != 0) {
                if (func_80493BB8((CScnVirtualLightData*)self->mExtraAlloc[j]) != 0) {
                    p58 = &slotCounts[0];
                    p68 = &slotFields[0];
                    p48 = &slotPtrs[0];
                    for (int i = 0; i < 4; i++) {
                        if ((s32)*p68 < (s32)*p58) {
                            if (func_80493BC4((CScnVirtualLightData*)self->mExtraAlloc[j],
                                              1u << i) != 0) {
                                func_8004B0B0(&v38);
                                Vec* camPos =
                                    (Vec*)func_80493C08((void*)cam);
                                func_80493BCC((Vec*)func_80493C00(
                                                  (void*)self->mExtraAlloc[j]),
                                              (Vec*)&v38, camPos);
                                dist2 = func_8006DFC8(&v38);
                                range = func_80493C18((void*)self->mExtraAlloc[j]);
                                if (range * range >= dist2) {
                                    func_804C02E4(
                                        (CLight*)(*p48 + *p68 * 64),
                                        self->mExtraAlloc[j]);
                                    scaled = aa74 * range;
                                    if (scaled * scaled < dist2) {
                                        func_804C0928(
                                            (CLight*)(*p48 + *p68 * 64),
                                            (range - func_8004EC78(dist2)) /
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
        func_80493C20(env, slotFields[0], 0);
        func_80493C20(env, slotFields[1], 1);
        func_80493C20(env, slotFields[2], 2);
        func_80493C20(env, slotFields[3], 3);
    }
}

// Item-destroy helper family used by func_804936AC / func_80493C30. noinline
// keeps the retail `bl` at the func_804936AC call sites out-of-line (the
// trivial load bodies would otherwise be inlined, dropping the retail call
// relocs).
__declspec(noinline) int func_80493B78(CScnCameraMan* cam) {
    return func_8049B158(cam, cam->mCamId);
}

__declspec(noinline) u32 func_80493B80(void* self) { return ((CScnVirtualLightData*)self)->value68; }

extern "C" __declspec(noinline) u32 func_80493B88(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1160); }

extern "C" __declspec(noinline) u32 func_80493B98(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1150); }

extern "C" __declspec(noinline) u32 func_80493BA8(void* self, u32 idx) { return *(u32*)((u8*)self + idx*4 + 0x1140); }

__declspec(noinline) s32 func_80493BB8(CScnVirtualLightData* self) {
    return func_8004B3D8(&self->value30, 0x10000);
}

__declspec(noinline) s32 func_80493BC4(CScnVirtualLightData* self, u32 flags) {
    return func_8004B3D8(&self->value30, flags);
}

// func_80493BCC (out = a - b): retail psq_l/ps_sub/psq_st kernel with the
// mr-swap entry and `b .+4` scheduler barrier (MWCC never emits ps from a
// pure scalar subtract; see CScnVirtualLight_ps.inl for the PS backend).
// The scalar body below is the PC/NONMATCHING fallback.
#include "monolib/scn/CScnVirtualLight_ps.inl"

__declspec(noinline) void* func_80493C00(void* self) { return &((CScnVirtualLightData*)self)->_04; }

// Retail: addi r3,r3,0x54; b .+4; addi r3,r3,0xB8; blr. The b .+4 scheduler
// barrier is the documented unreproducible MWCC artifact (MWCC_REFERENCE sec. 4;
// GetTextColor is the only getter precedent and was never byte-matched).
// 2026 probe (~80 shapes, Wii/1.1 + GC/2.6/2.7/3.0a5/3.0a5.2, -O4,p/-O4,s,
// -ipa file/off, -inline auto/on/smart, -proc gekko/750/603/7400, scheduling/
// peephole/opt_propagation/optimization_level/global_optimizer pragmas, goto/
// Retail func_80493C08 is a tail-call thunk: `addi r3,r3,0x54; b func_80493C10`
// into the adjacent getter (the retail annotation originally merged the two
// 0x8-byte bodies into one 0x10 symbol).  MWCC emits the tail call `b` only
// when the getter is a separate noinline leaf placed immediately after.
extern "C" __declspec(noinline) void* func_80493C10(void* self) {
    return (void*)((u8*)self + 0xB8);
}
extern "C" __declspec(noinline) void* func_80493C08(void* self) {
    return func_80493C10((void*)((u8*)self + 0x54));
}

__declspec(noinline) float func_80493C18(void* self) { return ((CScnVirtualLightData*)self)->value3C; }

extern "C" __declspec(noinline) void func_80493C20(void* self, u32 val, u32 idx) { *(u32*)((u8*)self + idx*4 + 0x1160) = val; }

// Retail func_80493C30: per-frame virtual-light update. Applies the pooled
// light data (`data`) with the given position/fade inputs, then - when the
// pool's flag is clear - pushes the environment's slots into the scene data
// (func_80495AF4). Finally walks the 128 light-handle slots (mExtraAlloc):
// every armed slot within range of `vec` gets a CLight record from the
// data's light array (0x40 stride, at most 8), with distance-based intensity
// falloff toward AA74 * range.
extern "C" void func_80493C30(CScnVirtualLight* self, CScnEnvLgtData* data,
                              const ml::CVec3* vec, int mode, f32 f1) {
    func_800407C8_tmp v18;
    ml::CVec3 v8;
    u32 enabled = func_80493EC8((void*)self);
    func_804BFA70(data, vec, mode, enabled, f1 + func_80493EC0((void*)self));
    if (checkBitFlag(getSubField7C(self->mSub)) != 0) {
        func_804C1500(getSubField7C(self->mSub), f1, data, vec);
    } else {
        func_80492030(&v18);
        CScnVirtualLightData* p = (CScnVirtualLightData*)func_80493ED0(
            (void*)func_80493ED8((void*)func_80493EE0((void*)self->mSub)));
        if (func_80493EE8((CScnVirtualLightData*)data) != 0) {
            func_80495AF4((CLightEnv*)p, data, 3);
        } else {
            func_80495AF4((CLightEnv*)p, data, 0);
        }
    }
    if (self->valueCC != 0) {
        int i;
        int count;
        int step;
        CLight* dst;
        int j;
        f32 f31v;
        void* base;
        step = func_80493EE8((CScnVirtualLightData*)data) != 0 ? 8 : 1;
        count = (int)func_80493EF4((void*)data);
        base = func_80493EFC((void*)data, 0);
        f31v = lbl_eu_8066AA74;
        dst = (CLight*)((u8*)base + count * 64);
        i = 0;
        j = 0;
        while (true) {
            if (((u32*)self->mExtraAlloc)[j] != 0) {
                if (func_80493BB8((CScnVirtualLightData*)((u32*)self->mExtraAlloc)[j]) != 0 &&
                    func_80493BC4((CScnVirtualLightData*)((u32*)self->mExtraAlloc)[j],
                                  (u32)step) != 0 &&
                    count < 8) {
                    func_8004B0B0(&v8);
                    func_80493BCC((Vec*)func_80493C00(
                                      (void*)((u32*)self->mExtraAlloc)[j]),
                                  (Vec*)&v8, (const Vec*)vec);
                    f32 dist2 = func_8006DFC8(&v8);
                    f32 range = f1 + func_80493C18(
                                         (void*)((u32*)self->mExtraAlloc)[j]);
                    if (range * range >= dist2) {
                        func_804C02E4(dst, ((u32*)self->mExtraAlloc)[j]);
                        f32 scaled = f31v * range;
                        if (scaled * scaled < dist2) {
                            func_804C0928(dst, (range - func_8004EC78(dist2)) /
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

__declspec(noinline) float func_80493EC0(void* self) { return ((CScnVirtualLightData*)self)->valueD4; }

__declspec(noinline) u32 func_80493EC8(void* self) {
    return ((CScnVirtualLightData*)self)->enabled;
}

__declspec(noinline) void* func_80493ED0(void* self) { return &((CScnVirtualLightData*)self)->value14; }

__declspec(noinline) u32 func_80493ED8(void* self) { return ((CScnVirtualLightData*)self)->value08; }

__declspec(noinline) u32 func_80493EE0(void* self) { return ((CScnVirtualLightData*)self)->value64; }

__declspec(noinline) s32 func_80493EE8(CScnVirtualLightData* self) {
    return func_8004B3D8(&self->value484, 1);
}

__declspec(noinline) u32 func_80493EF4(void* self) { return ((CScnVirtualLightData*)self)->value480; }

extern "C" __declspec(noinline) void* func_80493EFC(void* self, u32 idx){ return (void*)((char*)self + idx * 64); }

// Retail func_80493F08: dispatch a light-environment slot setup on the type
// of the light object at *func_80492A64(node) (+0x2C). The light's position
// comes from the vtable+0x0C virtual (ml::CVec3 by value); each type arms the
// slot's next light through the matching CVirtualLightObj.cpp helper:
// 1 = ambient (func_804952C4), 2 = directional with Euler angles
// (func_80495704), 3 = directional with a direction pointer (func_804957E4),
// 4 = spot with full params (func_804958B8). The first parameter is unused
// (retail keeps r3 untouched).
extern "C" void func_80493F08(void* unused, CLightEnv* env,
                              CScnVirtualLightNode* node) {
    ml::CVec3 v38;
    ml::CVec3 v28;
    ml::CVec3 v18;
    ml::CVec3 v8;
    switch (func_80493574((void*)*func_80492A64(node))) {
    case 1: {
        CScnVirtualLightData* obj = (CScnVirtualLightData*)*func_80492A64(node);
        v38 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        func_804952C4(env, &v38);
        break;
    }
    case 2: {
        CScnVirtualLightData* obj = (CScnVirtualLightData*)*func_80492A64(node);
        f32 angleY = func_804940E8((void*)obj);
        f32 angleX = func_804940E0((void*)obj);
        v28 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        func_80495704(env, (u32)&v28, angleX, angleY);
        break;
    }
    case 3: {
        CScnVirtualLightData* obj = (CScnVirtualLightData*)*func_80492A64(node);
        f32 color = func_804940F8((void*)obj);
        void* p34 = func_804940F0((void*)obj);
        v18 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        func_804957E4(env, (u32)&v18, (u32)p34, color);
        break;
    }
    case 4: {
        CScnVirtualLightData* obj = (CScnVirtualLightData*)*func_80492A64(node);
        f32 a = func_80494120((void*)obj);
        f32 b = func_80494118((void*)obj);
        void* p34 = func_80494110((void*)obj);
        f32 c = func_80494108((void*)obj);
        f32 d = func_80494100((void*)obj);
        v8 = ((CVirtualLightDispatch*)(void*)obj)->GetPosition();
        func_804958B8(env, d, c, &v8, p34, b, a);
        break;
    }
    }
}

__declspec(noinline) float func_804940E0(void* self) { return ((CScnVirtualLightData*)self)->value34; }

__declspec(noinline) float func_804940E8(void* self) { return ((CScnVirtualLightData*)self)->value38; }

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

// Advance the light blend: bump the +0xBC factor toward a clamp (add+upper
// bound when `flag` is set, subtract+lower bound otherwise), then blend the
// two position vec4s (+0x9C scaled by t, +0x8C scaled by 1-t) into +0xAC
// (retail func_80494208).
void func_80494208(CScnVirtualLightData* self, int flag) {
    if (flag != 0) {
        float v = lbl_eu_8066AA78 + self->field_0xBC;
        self->field_0xBC = v;
        if (v > lbl_eu_8066AA18) {
            self->field_0xBC = lbl_eu_8066AA18;
        }
    } else {
        float v = self->field_0xBC - lbl_eu_8066AA78;
        self->field_0xBC = v;
        if (v < lbl_eu_8066AA24) {
            self->field_0xBC = lbl_eu_8066AA24;
        }
    }
    ml::CVec4 v18;
    ml::CVec4 v8;
    func_80058BD8(&v18, (const ml::CVec4*)&self->field_0x9C, self->field_0xBC);
    copyWord4((u32*)&self->field_0xAC, (const u32*)&v18);
    func_80058BD8(&v8, (const ml::CVec4*)&self->field_0x8C, lbl_eu_8066AA18 - self->field_0xBC);
    func_804942BC((f32*)&self->field_0xAC, (const f32*)&v8);
}

// Retail func_804942BC: vec4 += (a[i] += b[i]). noinline keeps the bl from
// func_80494208 out-of-line (retail calls it; the trivial body would otherwise
// be inlined, dropping the call-site reloc).
__declspec(noinline) void func_804942BC(f32* a, const f32* b) {
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
// reserve the reslist node array (retail func_8049216C): allocate count*0xC
// bytes via MemManager::allocate_array, zero the mNext of the first `count`
// nodes (MWCC auto-unrolls the fill loop 8x with its overflow guards), then
// set the capacity. `handle` is passed through untouched to allocate_array.
extern "C" void func_8049216C(CScnVirtualLightReslist* self, u32 handle, int capacity) {
    self->mList = (CScnVirtualLightNode*)mtl::MemManager::allocate_array((u32)capacity * 0xC, handle);
    for (int i = 0; i < capacity; i++) {
        self->mList[i].mNext = nullptr;
    }
    self->mCapacity = capacity;
}
#pragma pop
extern "C" __declspec(noinline) void func_804923F8(CScnVirtualLight* self, int arg) {}

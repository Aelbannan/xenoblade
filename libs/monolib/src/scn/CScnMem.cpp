// Translation unit: monolib/src/scn/CScnMem
// CScnMem (a MemManager-backed alloc block) and CScnNw4r (a CScn subclass that
// installs its Nw4r function table).

#include <harness_catalog.h>
#include <nw4r/db/db_assert.h>
#include "libs/monolib/src/scn/CScnMem.hpp"

// vtables (data imports - global-scope names, no extern "C" needed).
extern u32 lbl_eu_8056E5B8[];   // CScnMem vtable
extern u32 lbl_eu_8056E5C8[];   // CScnNw4r function table

// CScnNw4r access table accessors (defined in CScnRootNw4r TU, C linkage).
extern "C" void* func_8048FDDC(void* self);
extern "C" void* func_8048FDE4(void* self);
extern "C" void* func_8048FDEC(void* self);
extern "C" void* func_8048FE0C(void* self);
extern "C" void* func_8048FE2C(void* self);

// WorkEvent thunk callees (CScn TU).
extern "C" void func_80496970(void* self);
extern "C" void func_8049695C(void* self, void* r4);
extern "C" void __dt__8CScnNw4rFv(void* self, int deleteFlag);

// Member ctors.
CScnMem::CScnMem() {
    *(void**)this = (void*)lbl_eu_8056E5B8;
    field_0x4 = 0;
    mAllocId = -1;
    field_0xc = 0;
}

CScnNw4r::CScnNw4r() : CScn() {
    field_0x10 = (u32)lbl_eu_8056E5C8;
    field_0x54 = (u32)lbl_eu_8056E5C8 + 0x24;
}

CScnNw4r::~CScnNw4r() {
    // ~CScn base dtor and the delete are emitted automatically by MWCC.
}

CScnMem::~CScnMem() {
    // novtable so the dtor must re-point the vptr manually, then release the
    // MemManager-backed allocation handle.
    *(void**)this = (void*)lbl_eu_8056E5B8;
    mtl::MemManager::erase(mAllocId);
}

u32 func_8048ECD0(CScn* self) { return (u32)self->mUnk8C; }


extern "C" void* func_8048ECD8(void* self) { return *(void**)((u8*)*(void**)((u8*)self + 0x8C) + 0x10); }

void* func_8048ECE4(CScn* self) { return func_8048FDDC(self->mUnk8C); }

void* func_8048ECEC(CScn* self) { return func_8048FDE4(self->mUnk8C); }

void* func_8048ECF4(CScn* self) { return func_8048FDEC(self->mUnk8C); }

void* func_8048ECFC(CScn* self) { return func_8048FE0C(self->mUnk8C); }

void* func_8048ED04(CScn* self) { return func_8048FE2C(self->mUnk8C); }

extern "C" void WorkEvent4__4CScnFv(void* self) { ((void(*)(void*))func_80496970)((char*)self - 0x54); }

extern "C" void WorkEvent3__4CScnFPv(void* self, void* r4) { ((void(*)(void*))func_8049695C)((char*)self - 0x54); }

void func_8048ED1C(void* self){ ((void(*)(void*))__dt__8CScnNw4rFv)((char*)self - 0x54); }

// Allocate a MemManager-backed block through this CScnMem. The region handle
// (a) tags field_0x4, the size (c) is shared by field_0xc and the allocation.
void func_8048EB30(CScnMem* self, u32 a, u32 b, u32 c) {
    self->field_0x4 = a;
    self->field_0xc = c;
    self->mAllocId = mtl::MemManager::create(b, c, lbl_eu_80523F98);
}

// func_8048EC14: return the idx-th child of the scene root only if it derives
// from ScnGroup, else NULL. Panics on an out-of-range index.
//
// The child array is read through the layout mirror (not the inline
// operator[]/Begin() wrappers): the array pointer is a distinct lowering
// object, which delays the idx saved-copy web past the group web so retail's
// group->r31 / idx->r30 coloring is reproduced byte-for-byte.
nw4r::g3d::ScnObj* func_8048EC14(CScn* self, u32 idx) {
    nw4r::g3d::ScnGroup* group =
        reinterpret_cast<CScnRootNw4rLayout*>(self->mUnk8C)->mRootGroup;

    if (idx >= group->Size()) {
        nw4r::db::Panic(lbl_eu_8056E720, 0x22b, (const char*)lbl_eu_8056E6F8);
    }

    nw4r::g3d::ScnObj* obj =
        ((ScnGroupChildLayout*)group)->mpScnObjArray[idx];
    if (obj != NULL &&
        obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788))) {
        return obj;
    }
    return NULL;
}
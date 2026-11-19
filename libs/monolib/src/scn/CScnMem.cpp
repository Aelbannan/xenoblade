// Translation unit: monolib/src/scn/CScnMem
// CScnMem (a MemManager-backed alloc block) and CScnNw4r (a CScn subclass that
// installs its Nw4r function table).

#include <harness_catalog.h>
#include <nw4r/db/db_assert.h>
#include "libs/monolib/src/scn/CScnMem.hpp"

// CScnNw4r access table accessors (defined in CScnRootNw4r TU, C linkage).
extern "C" void* RootNw4r_Ptr44C(void* self);
extern "C" void* RootNw4r_Ptr45C(void* self);
extern "C" void* RootNw4r_SelPtr46C(void* self);
extern "C" void* RootNw4r_SelPtr47C(void* self);
extern "C" void* RootNw4r_Ptr49C(void* self);

CScnMem::CScnMem() {
    field_0x4 = 0;
    mAllocId = -1;
    field_0xc = 0;
}

CScnMem::~CScnMem() {
    // Release the MemManager-backed allocation handle.
    mtl::MemManager::erase(mAllocId);
}

// Allocate a MemManager-backed block through this CScnMem. The region handle
// (a) tags field_0x4, the size (c) is shared by field_0xc and the allocation.
extern "C" void allocScnMem(CScnMem* self, u32 a, u32 b, u32 c) {
    self->field_0x4 = a;
    self->field_0xc = c;
    self->mAllocId = mtl::MemManager::create(b, c, "CScnMem");
}

CScnNw4r::CScnNw4r() {}

CScnNw4r::~CScnNw4r() {}

// getScnGroupChild: return the idx-th child of the scene root only if it derives
// from ScnGroup, else NULL. Panics on an out-of-range index.
//
// The child array is read through the layout mirror (not the inline
// operator[]/Begin() wrappers): the array pointer is a distinct lowering
// object, which delays the idx saved-copy web past the group web so retail's
// group->r31 / idx->r30 coloring is reproduced byte-for-byte.
extern "C" nw4r::g3d::ScnObj* getScnGroupChild(CScn* self, u32 idx) {
    nw4r::g3d::ScnGroup* group = ((CScnRootNw4rLayout*)self->mUnk8C)->mRootGroup;

    if (idx >= group->Size()) {
        nw4r::db::Panic(lbl_eu_8056E720, 0x22b, (const char*)lbl_eu_8056E6F8);
    }

    nw4r::g3d::ScnObj* obj = group->Begin()[idx];
    s32 isGroup = 0;
    if (obj != NULL) {
        // Separate TypeObj statement: retail materializes the ScnGroup type
        // object (lis/addi) before loading `this` into r3.
        nw4r::g3d::G3dObj::TypeObj type =
            nw4r::g3d::ScnGroup::GetTypeObjStatic();
        if (obj->IsDerivedFrom(type)) {
            isGroup = 1;
        }
    }
    if (isGroup) {
        return obj;
    }
    return NULL;
}

extern "C" u32 getScnRootPtr(CScn* self) { return (u32)self->mUnk8C; }

extern "C" nw4r::g3d::ScnGroup* getScnRootSlot10(CScn* self) { return ((CScnRootNw4rLayout*)self->mUnk8C)->mRootGroup; }

extern "C" void* getScnRoot44C(CScn* self) { return RootNw4r_Ptr44C(self->mUnk8C); }

extern "C" void* getScnRoot45C(CScn* self) { return RootNw4r_Ptr45C(self->mUnk8C); }

extern "C" void* getScnRootSel46C(CScn* self) { return RootNw4r_SelPtr46C(self->mUnk8C); }

extern "C" void* getScnRootSel47C(CScn* self) { return RootNw4r_SelPtr47C(self->mUnk8C); }

extern "C" void* getScnRoot49C(CScn* self) { return RootNw4r_Ptr49C(self->mUnk8C); }

// NW4R assert strings for getScnGroupChild (retail .data tail of this TU).
extern "C" __declspec(align(8)) char lbl_eu_8056E6F8[0x38] = {
    0x4E,0x57,0x34,0x52,0x3A,0x46,0x61,0x69,0x6C,0x65,0x64,0x20,0x61,0x73,0x73,0x65,
    0x72,0x74,0x69,0x6F,0x6E,0x20,0x69,0x64,0x78,0x20,0x3C,0x20,0x6D,0x4E,0x75,0x6D,
    0x53,0x63,0x6E,0x4F,0x62,0x6A,0x00,0x00,
    0x67,0x33,0x64,0x5F,0x73,0x63,0x6E,0x6F,0x62,0x6A,0x2E,0x68,0x00,0x00,0x00,0x00,
};


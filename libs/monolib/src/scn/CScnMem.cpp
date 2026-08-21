// Translation unit: monolib/src/scn/CScnMem
// CScnMem (a MemManager-backed alloc block) and CScnNw4r (a CScn subclass that
// installs its Nw4r function table).

#include <harness_catalog.h>
#include <nw4r/db/db_assert.h>
#include "libs/monolib/src/scn/CScnMem.hpp"

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
extern "C" u32 lbl_eu_80663978[2];  // RTTI locators (owned by CScnItemCamera.cpp)
extern "C" u32 lbl_eu_80663980[2];
extern "C" u32 lbl_eu_80663988[2];
extern "C" u32 lbl_eu_80663990[2];
extern "C" u32 lbl_eu_80661948[2];  // RTTI CProcess
extern "C" u32 lbl_eu_80661950[2];  // RTTI CChildListNode
extern "C" u32 lbl_eu_80661958[2];  // RTTI CDoubleListNode

// [.rodata] 0x80523F98-0x80523FD0 (0x38 = 56B): class name strings.
extern "C" __declspec(align(8)) const char lbl_eu_80523F98[0x38] = {
    0x43,0x53,0x63,0x6E,0x4D,0x65,0x6D,0x00,
    0x43,0x53,0x63,0x6E,0x4E,0x77,0x34,0x72,0x00,0x00,0x00,0x00,
    0x43,0x54,0x54,0x61,0x73,0x6B,0x3C,0x43,0x53,0x63,0x6E,0x3E,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x43,0x53,0x63,0x6E,0x52,0x6F,0x6F,0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.data] 0x8056E5B8-0x8056E730 (0x178 = 376B): CScnMem vtable, CScnNw4r
// function table, RTTI base lists, NW4R assert strings.
namespace CScnMemBlob {
extern "C" void* __dt__7CScnMemFv();
extern "C" void* __dt__8CScnNw4rFv();
extern "C" void Reset__14CChildListNodeFv();
extern "C" void Init__4CScnFv();
extern "C" void Term__4CScnFv();
extern "C" void Move__4CScnFv();
extern "C" void Draw__4CScnFv();
extern "C" void Tail__8CProcessFv();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
extern "C" void WorkEvent3__4CScnFPv();
extern "C" void WorkEvent4__4CScnFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" void func_8048ED1C();
extern "C" void func_80496970();
extern "C" void func_8049695C();
extern "C" u32 __RTTI__10IWorkEvent;
}
extern "C" u32 lbl_eu_8056E5B8[4] = {
    (u32)&lbl_eu_80663978, 0x00000000,
    (u32)&CScnMemBlob::__dt__7CScnMemFv, 0x00000000,
};
extern "C" u32 lbl_eu_8056E5C8[45] = {
    (u32)&lbl_eu_80663980, 0x00000000,
    (u32)&CScnMemBlob::__dt__8CScnNw4rFv,
    (u32)&CScnMemBlob::Reset__14CChildListNodeFv,
    (u32)&CScnMemBlob::Init__4CScnFv,
    (u32)&CScnMemBlob::Term__4CScnFv,
    (u32)&CScnMemBlob::Move__4CScnFv,
    (u32)&CScnMemBlob::Draw__4CScnFv,
    (u32)&CScnMemBlob::Tail__8CProcessFv,
    (u32)&lbl_eu_80663980, 0xFFFFFFAC,
    (u32)&CScnMemBlob::func_8048ED1C,
    (u32)&CScnMemBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&CScnMemBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&CScnMemBlob::WorkEvent3__4CScnFPv,
    (u32)&CScnMemBlob::WorkEvent4__4CScnFv,
    (u32)&CScnMemBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&CScnMemBlob::WorkEvent6__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent7__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent8__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent9__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent10__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent11__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent12__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent13__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent14__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent15__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent16__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent17__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent18__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent19__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent20__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent21__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent22__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent23__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent24__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent25__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent26__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent27__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent28__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent29__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent30__10IWorkEventFv,
    (u32)&CScnMemBlob::WorkEvent31__10IWorkEventFv,
    (u32)&CScnMemBlob::func_80496970,
    (u32)&CScnMemBlob::func_8049695C,
};
extern "C" u32 lbl_eu_8056E67C[13] = {
    (u32)&CScnMemBlob::__RTTI__10IWorkEvent, 0x00000054,
    (u32)&lbl_eu_80661958, 0x00000000,
    (u32)&lbl_eu_80661950, 0x00000000,
    (u32)&lbl_eu_80661948, 0x00000000,
    (u32)&lbl_eu_80663990, 0x00000000,
    (u32)&lbl_eu_80663988, 0x00000000,
    0x00000000,
};
extern "C" u32 lbl_eu_8056E6B0[11] = {
    (u32)&CScnMemBlob::__RTTI__10IWorkEvent, 0x00000054,
    (u32)&lbl_eu_80661958, 0x00000000,
    (u32)&lbl_eu_80661950, 0x00000000,
    (u32)&lbl_eu_80661948, 0x00000000,
    (u32)&lbl_eu_80663990, 0x00000000,
    0x00000000,
};
extern "C" u32 lbl_eu_8056E6DC[7] = {
    (u32)&lbl_eu_80661958, 0x00000000,
    (u32)&lbl_eu_80661950, 0x00000000,
    (u32)&lbl_eu_80661948, 0x00000000,
    0x00000000,
};
extern "C" __declspec(align(8)) char lbl_eu_8056E6F8[0x38] = {
    0x4E,0x57,0x34,0x52,0x3A,0x46,0x61,0x69,0x6C,0x65,0x64,0x20,0x61,0x73,0x73,0x65,
    0x72,0x74,0x69,0x6F,0x6E,0x20,0x69,0x64,0x78,0x20,0x3C,0x20,0x6D,0x4E,0x75,0x6D,
    0x53,0x63,0x6E,0x4F,0x62,0x6A,0x00,0x00,
    0x67,0x33,0x64,0x5F,0x73,0x63,0x6E,0x6F,0x62,0x6A,0x2E,0x68,0x00,0x00,0x00,0x00,
};
DECOMP_FORCEACTIVE(CScnMem_cpp, lbl_eu_8056E5B8);
DECOMP_FORCEACTIVE(CScnMem_cpp, lbl_eu_8056E5C8);
DECOMP_FORCEACTIVE(CScnMem_cpp, lbl_eu_8056E67C);
DECOMP_FORCEACTIVE(CScnMem_cpp, lbl_eu_8056E6B0);
DECOMP_FORCEACTIVE(CScnMem_cpp, lbl_eu_8056E6DC);
DECOMP_FORCEACTIVE(CScnMem_cpp, lbl_eu_8056E6F8);
DECOMP_FORCEACTIVE(CScnMem_cpp, lbl_eu_80523F98);

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
    CScnRootNw4rLayout* root =
        reinterpret_cast<CScnRootNw4rLayout*>(self->mUnk8C);
    nw4r::g3d::ScnGroup* group = root->mRootGroup;

    if (idx >= group->Size()) {
        nw4r::db::Panic(lbl_eu_8056E720, 0x22b, (const char*)lbl_eu_8056E6F8);
    }

    nw4r::g3d::ScnObj* obj = group->Begin()[idx];
    if (obj != NULL &&
        obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788))) {
        return obj;
    }
    return NULL;
}
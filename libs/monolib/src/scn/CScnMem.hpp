#pragma once

#include <types.h>
#include "monolib/util/MemManager.hpp"
#include <nw4r/g3d/g3d_scnroot.h>

// String/data labels this TU references (declared here so they resolve as
// properly-typed imports; no local extern "C" needed at global scope).
// Panic file path / format string for getScnGroupChild's bounds check.
extern char lbl_eu_8056E720[];
extern char lbl_eu_8056E6F8[];

// Minimal layout of the CScnRootNw4r object pointed to by CScn::mUnk8C.
// Only the +0x10 scene-root slot is touched by this TU.
struct CScnRootNw4rLayout {
    void* field_0x0;                          // +0x00 vptr
    char pad[0x10 - 0x4];                     // +0x04
    nw4r::g3d::ScnGroup* mRootGroup;          // +0x10 scene root (ScnGroup/Root)
};

// Layout mirror of nw4r::g3d::ScnGroup's private child-array slot (+0xDC),
// used to read the array directly (retail accesses it without the inline
// Begin()/operator[] wrappers).
struct ScnGroupChildLayout {
    char pad_0x0[0xDC];                       // +0x00
    nw4r::g3d::ScnObj** mpScnObjArray;        // +0xDC child array
    char pad_0xE0[0xE4 - 0xE0];               // +0xE0
    u32 mNumScnObj;                           // +0xE4 child count (Size())
};

// CScnNw4r (canonical class in monolib/scn/CScnNw4r.hpp) derives the real
// CScn (CTTask<CScn> + IWorkEvent at +0x54). This TU defines its ctor/dtor,
// so MWCC emits __vt__8CScnNw4r here together with the weak +0x54 thunks
// (@84@WorkEvent4__4CScnFv, @84@WorkEvent3__4CScnFPv, @84@__dt__8CScnNw4rFv)
// that retail keeps from this object.
#include "monolib/scn/CScnNw4r.hpp"

// A memory block allocated through the monolib MemManager. Holds an allocator
// A memory block allocated through the monolib MemManager. Holds an allocator
// handle (mAllocId) that the destructor erases via MemManager::erase.
// Retail __vt__7CScnMem is 0x10: [rtti, 0, dtor, 0] -- the trailing zero
// word is MWCC's encoding of a pure-virtual slot, so CScnMem is abstract.
class CScnMem {
public:
    CScnMem();
    virtual ~CScnMem();
    virtual void vf0C() = 0;   // +0x0C pure (zero slot in retail vtable)

    u32 field_0x4;   // +0x04 - init 0
    u32 mAllocId;    // +0x08 - MemManager alloc handle (init -1)
    u32 field_0xc;   // +0x0c - init 0
};


// Free functions in this TU (retail kept plain unmangled names).
extern "C" void allocScnMem(CScnMem* self, u32 a, u32 b, u32 c);
extern "C" nw4r::g3d::ScnObj* getScnGroupChild(CScn* self, u32 idx);

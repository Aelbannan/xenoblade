#pragma once

#include <types.h>
#include "monolib/util/MemManager.hpp"
#include <nw4r/g3d/g3d_scnroot.h>

// String/data labels this TU references (declared here so they resolve as
// properly-typed imports; no local extern "C" needed at global scope).
// lbl_eu_80523F98: MemManager region name passed to MemManager::create.
extern const char lbl_eu_80523F98[];
// Panic file path / format string for func_8048EC14's bounds check.
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

// A memory block allocated through the monolib MemManager. Holds an allocator
// handle (mAllocId) that the destructor erases via MemManager::erase.
// Polymorphic vtable: lbl_eu_8056E5B8 (auto vptr store at +0x00).
class __declspec(novtable) CScnMem {
public:
    CScnMem();
    virtual ~CScnMem();

    // +0x00: vptr (lbl_eu_8056E5B8)
    u32 field_0x4;   // +0x04 - init 0
    u32 mAllocId;    // +0x08 - MemManager alloc handle (init -1)
    u32 field_0xc;   // +0x0c - init 0
};

// Scene object. Base of CScnNw4r. The +0x8c root pointer (mUnk8C) is used by
// the accessor functions below; +0x10 and +0x54 are written by the Nw4r ctor.
class CScn {
public:
    CScn();
    virtual ~CScn();
    void WorkEvent4();
    void WorkEvent3();

    // +0x00: vptr (CScn vtable)
    char field_0x4[0x10 - 0x4];  // +0x04
    u32 field_0x10;             // +0x10 - accessor table ptr (lbl_eu_8056E5C8)
    char field_0x14[0x54 - 0x14];
    u32 field_0x54;             // +0x54 - IWorkEvent vtable ptr (lbl_eu_8056E5C8+0x24)
    char field_0x58[0x8c - 0x58];
    void* mUnk8C;               // +0x8c - CScnRootNw4r pointer
};

// Nw4r wrapper object. Base CScn ctor runs first; this ctor points the
// IWorkEvent slot (+0x54) and an accessor table slot (+0x10) at the CScnNw4r
// function table.
class __declspec(novtable) CScnNw4r : public CScn {
public:
    CScnNw4r();
    virtual ~CScnNw4r();
};

// Free functions in this TU (retail kept plain unmangled names).
void func_8048EB30(CScnMem* self, u32 a, u32 b, u32 c);
nw4r::g3d::ScnObj* func_8048EC14(CScn* self, u32 idx);
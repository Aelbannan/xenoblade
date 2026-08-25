#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

class CREvtModel;
struct ParentTask;
struct EvtRefOwner;

// Primary vtable layout for CREvtModelObj (retail lbl_eu_80531FA0).
// Entry 15 (0x3C) is dispatched virtually by the reset path.
struct CREvtModelObjVtbl {
    void* entries[15];              // 0x00-0x38
    void (*func3C)(void* self);     // 0x3C
};
extern CREvtModelObjVtbl lbl_eu_80531FA0[];

// Owner of a loaded-resource reference counted at 0x48/0x4C.
struct EvtRefOwner {
    u8 _pad00[0x48];
    u32 mFlag48;   // 0x48
    u32 mRefCount; // 0x4C
};

// CREvtModelObj class layout (manual field layout)
// Inherits: CREvtModel (at 0x00), IWorkEvent (at 0x38), UnknownInterface (at 0x3C)
struct CREvtModelObj {
    // 0x00: CREvtModel base
    void* vtable;          // 0x00
    u32 mType;             // 0x04
    u32 mCallback[3];      // 0x08: __ptmf triplet
    u32 mField14;          // 0x14
    u32 mFlags;            // 0x18
    ParentTask* mParent;   // 0x1C
    void* mModel;          // 0x20
    void* mData24;         // 0x24
    u8 _pad28[0x38 - 0x28];
    // 0x38: IWorkEvent vtable ptr - kept void*
    void* mIWorkVtbl;
    // 0x3C: other interface vtable ptr - kept void*
    void* mOtherVtbl;
    // 0x40: CFileHandle* - current file load handle
    CFileHandle* mFileHandle;
    // 0x44: u32 - file load state (0=none, 1=allocating, 2=archive read, 3=file read)
    u32 mFileState;
    // 0x48: u8* - allocated data
    u8* mAllocData;
    // 0x4C: s32 (signed: retail emits bc 4,1 "<= 0 continue" for this check)
    s32 mField4C;
    // 0x50: u32 - model type (1=arc, 2=chr, 3=?)
    u32 mModelType;
    // 0x54: owner holding a refcount on our resource
    EvtRefOwner* mRefOwner;
    // 0x58: s32 - file id (-1 = none)
    s32 mFileId;
    // 0x5C: u32 - resource id
    u32 mResId5C;
    // 0x60: u32 - resource id
    u32 mResId60;
    // 0x64: u8 - flag
    u8 mFlag64;
    u8 _pad65[3];
    // 0x68: u8* - extracted data pointer
    u8* mExtractData;
    // 0x6C: CFileHandle* - secondary file handle
    CFileHandle* mFileHandle2;
    // 0x70: u8* - allocated data
    u8* mAllocData2;
    // 0x74: s32 - id (-1 = none)
    s32 mField74;
    // 0x78: u8* - allocated data
    u8* mAllocData3;
    // 0x7C: CFileHandle* - event file handle
    CFileHandle* mFileHandle3;
    // 0x80: u32 - ref count
    u32 mCount80;
    // 0x84: u32 - state
    u32 mState84;
};

// Parent task object (referenced at offset 0x1C in CREvtModel)
struct ParentTask {
    u8 _pad00[0x28];
    u32 mField28; // 0x28
    u8 _pad2C[0x30 - 0x2C];
    u32 mField30; // 0x30
    u8 _pad34[0x48 - 0x34];
    u8* mField48; // 0x48
    s32 mField4C; // 0x4C (signed: retail compares > 0 with bng/bc 4,1)
    u8 _pad50[0x58 - 0x50];
    u32 mField58; // 0x58
    u8 _pad5C[0x5C - 0x58];
    s16 mField5C; // 0x5C
    s16 mField5E; // 0x5E
    u32 mField60; // 0x60
};

// __ptmf structure (12 bytes); code/vtable anchors kept void*.
struct Ptmf {
    void* mFunc; // 0x00
    void* mThisAdj; // 0x04
    void* mVtblAdj; // 0x08
};

// CEventFile structure (from monolib)
// Already defined in CEventFile.hpp, but we need the fields
// 0x00: BOOL unk0
// 0x04: CFileHandle* mFileHandle
// 0x14: u32 field_14

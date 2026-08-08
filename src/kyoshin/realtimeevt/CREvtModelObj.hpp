#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

// CREvtModelObj class layout (manual field layout)
// Inherits: CREvtModel (at 0x00), IWorkEvent (at 0x38), UnknownInterface (at 0x3C)
struct CREvtModelObj {
    // 0x00: CREvtModel base
    u8 _pad00[0x38];
    // 0x38: IWorkEvent vtable ptr
    void* mIWorkVtbl;
    // 0x3C: other interface vtable ptr
    void* mOtherVtbl;
    // 0x40: CFileHandle* - current file load handle
    CFileHandle* mFileHandle;
    // 0x44: u32 - file load state (0=none, 1=allocating, 2=archive read, 3=file read)
    u32 mFileState;
    // 0x48: void* - allocated data
    void* mAllocData;
    // 0x4C: u32
    u32 mField4C;
    // 0x50: u32 - model type (1=arc, 2=chr, 3=?)
    u32 mModelType;
    // 0x54: CREvtModel* - parent model reference
    void* mParentModel;
    // 0x58: s32 - file id (-1 = none)
    s32 mFileId;
    // 0x5C: u32 - resource id
    u32 mResId5C;
    // 0x60: u32 - resource id
    u32 mResId60;
    // 0x64: u8 - flag
    u8 mFlag64;
    u8 _pad65[3];
    // 0x68: void* - extracted data pointer
    void* mExtractData;
    // 0x6C: CFileHandle* - secondary file handle
    CFileHandle* mFileHandle2;
    // 0x70: void* - allocated data
    void* mAllocData2;
    // 0x74: s32 - id (-1 = none)
    s32 mField74;
    // 0x78: void* - allocated data
    void* mAllocData3;
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
    u32 mField30; // 0x30
    u8 _pad34[0x48 - 0x34];
    void* mField48; // 0x48
    u32 mField4C; // 0x4C
    u8 _pad50[0x58 - 0x50];
    u32 mField58; // 0x58
    u8 _pad5C[0x5C - 0x58];
    s16 mField5C; // 0x5C
    s16 mField5E; // 0x5E
    u32 mField60; // 0x60
};

// __ptmf structure (12 bytes)
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

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------
extern "C" void func_801832D4(void* self);
extern "C" void func_801836CC(void* self, unsigned long value);
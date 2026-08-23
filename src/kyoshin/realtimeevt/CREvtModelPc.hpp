#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

class CScnItemModel;

// Flattened layout of the retail CREvtModelPc object. The retail class
// inherits cf::CREvtObj / CREvtModel / IWorkEvent, but the base headers'
// member-function-pointer field compiles to 16 bytes vs retail's 12-byte
// __ptmf (same issue as CREvtModelMap), so the base region is declared
// explicitly to hold the retail offsets.
class CREvtModelPc {
public:
    // cf::CREvtObj base region (0x00-0x13)
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 mType;
    /* 0x08 */ u32 mCallback[3];      // __ptmf triplet

    // CREvtModel base region (0x14-0x37)
    /* 0x14 */ u32 mField14;
    /* 0x18 */ u32 mFlags;
    /* 0x1C */ void* mParent;
    /* 0x20 */ CScnItemModel* mModel;
    /* 0x24 */ void* mData24;
    /* 0x28 */ void* mPtr28;
    /* 0x2C */ s32 mField2C;
    /* 0x30 */ s32 mField30;
    /* 0x34 */ s32 mField34;

    // IWorkEvent subobject + derived fields
    /* 0x38 */ void* vtableIWork;     // IWorkEvent vtable (primary + 0x44)

    void func_80183A3C();
    void func_80183C1C();
    void func_80183C90();
    void func_801845F0();
    void func_801846C4();
    void func_80184730();
    void func_801848EC(int r4, int r5);
    bool func_8018497C();

    // File slots (6 slots: 0..5)
    CFileHandle* mFileHandle[6]; // 0x3C
    CFileHandle* mFileReq[6];    // 0x54
    void* mData[6];              // 0x6C
    u32 mStatus[6];              // 0x84
    f32 mSpeed;                  // 0xA8
    u8 mLoaded;                  // 0xAC
    s32 mCounter;                // 0xB0
};

#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"
#include "kyoshin/realtimeevt/CREvtModel.hpp"

class CREvtModel;

class CREvtModelPc : public CREvtModel, public IWorkEvent {
public:
    CREvtModelPc();
    virtual ~CREvtModelPc();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    void func_80183A3C();
    void func_80183C1C();
    void func_80183C90();
    void func_801845F0();
    void func_801846C4();
    void func_80184730();
    void func_801848EC(int r4, int r5);
    bool func_8018497C();

    // Vtable layout:
    // 0x00: CREvtModelPc vtable (base = CREvtModel)
    // 0x38: IWorkEvent vtable (vtable+0x44)

    // Fields from CREvtModel (base):
    // 0x00: vtable
    // 0x04: (parent field)
    // 0x08-0x10: ptmf (12 bytes)
    // 0x18: flags (u32)
    // 0x1C: parent (CREvtModel*)
    // 0x20: model ptr (void*)
    // 0x24-0x34: (inherited fields)
    // 0x38: IWorkEvent subobject vtable

    // File slots (6 slots: 0..5):
    // 0x3C/0x40/0x44/0x48/0x4C/0x50: CFileHandle* mFileHandle[6]
    // 0x54/0x58/0x5C/0x60/0x64/0x68: CFileHandle* mFileReq[6]
    // 0x6C/0x70/0x74/0x78/0x7C/0x80: void* mData[6]
    // 0x84/0x88/0x8C/0x90/0x94/0x98: u32 mStatus[6]
    // 0xA8: f32 mSpeed
    // 0xAC: u8 mLoaded
    // 0xB0: s32 mCounter
};
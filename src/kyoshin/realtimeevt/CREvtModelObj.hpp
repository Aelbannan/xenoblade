#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/CEventFile.hpp"

class CREvtModel; // forward decl

class CREvtModelObj : public CREvtModel, public IWorkEvent {
public:
    CREvtModelObj();
    virtual ~CREvtModelObj();

    // IWorkEvent overrides
    virtual bool OnFileEvent(CEventFile* pEventFile);

    void OnFileEvent();

    // Vtable layout:
    // 0x00: CREvtModelObj vtable
    // 0x38: IWorkEvent vtable (vtable+0x44)
    // 0x3C: another interface vtable (vtable+0xCC)

    // Fields starting at 0x38 (after CREvtModel's fields)
    // 0x38: IWorkEvent subobject vtable pointer
    // 0x3C: another interface subobject vtable pointer
    // 0x40: CFileHandle* - current file load handle
    // 0x44: u32 - file state (0=none, 1=allocating, 2=read from archive, 3=read from file)
    // 0x48: void* - allocated data buffer
    // 0x4C: u32 - ref count / buffer size
    // 0x50: u32 - model type (1=arc, 2=?, 3=?)
    // 0x54: CREvtModel* - parent model reference
    // 0x58: s32 - some id (initialized to -1)
    // 0x5C: u32 - texture/object resource ID
    // 0x60: u32 - another resource ID
    // 0x64: u8 - has bone data flag
    // 0x68: void* - extracted model data pointer
    // 0x6C: CFileHandle* - secondary file handle
    // 0x70: void* - allocated data for model
    // 0x74: s32 - some id (-1 = none)
    // 0x78: void* - allocated data buffer
    // 0x7C: CFileHandle* - file event handle
    // 0x80: u32 - ref count
    // 0x84: u32 - state flag
};
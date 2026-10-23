#pragma once

#include <types.h>
#include "monolib/monolib_types.hpp"

// Async file event CDeviceFile hands to IWorkEvent::OnFileEvent. Layout from
// the owner ctor (CDeviceFile.cpp, CEventFile::CEventFile(CBM, CFileHandle*)),
// which copies these fields out of the file handle. The ctor is not declared
// here: only CDeviceFile constructs events, and its TU keeps a private copy
// (it cannot include the monolib headers; see the KEEP note there).
class CEventFile {
public:
    BOOL unk0;                 //0x0 call mode (CBM); 1 = load finished
    CFileHandle* mFileHandle;  //0x4
    u32 field_08;              //0x8
    const char* mName;         //0xC handle->mName (file path)
    u32 field_10;              //0x10
    u32 field_14;              //0x14 handle->field_0x3C (data length)
    u32 field_18;              //0x18
    void* mRsrc;               //0x1C handle->getRsrc()

    void* getFileDataPtr();
};

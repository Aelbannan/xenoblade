#pragma once

#include <types.h>
#include "monolib/monolib_types.hpp"

class CEventFile {
public:
    BOOL unk0;                 //0x0
    CFileHandle* mFileHandle;  //0x4
    u8 _pad08[0x0C];           //0x8-0x13
    u32 field_14;              //0x14

    void* getFileDataPtr();
};

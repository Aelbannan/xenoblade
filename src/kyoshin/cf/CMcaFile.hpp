#pragma once

#include <types.h>

class CMcaFile {
public:
    virtual ~CMcaFile();

    // 0x00: vtable
    u8 mFlag;       // 0x04
    void* mData;    // 0x08
    void* mDataAdj; // 0x0C
};

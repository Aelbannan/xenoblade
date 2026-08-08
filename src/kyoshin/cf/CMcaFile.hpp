#pragma once

#include <types.h>
#include "monolib/util/FixStr.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80495EAC(void*, void*, ml::FixStr<64>&);

class CMcaFile {
public:
    virtual ~CMcaFile();

    // 0x00: vtable
    u8 mFlag;       // 0x04
    void* mData;    // 0x08
    void* mDataAdj; // 0x0C
};

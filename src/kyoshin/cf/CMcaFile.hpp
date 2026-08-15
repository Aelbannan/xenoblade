#pragma once

#include <types.h>
#include "monolib/util/FixStr.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80495EAC(void*, void*, ml::FixStr<64>&);
// Retail CMcaFile vtable data (kyoshin split .data @0x80526348). The class is
// __declspec(novtable), so the ctor assigns the retail label explicitly
// instead of the compiler-generated __vt__8CMcaFile (which would add
// .data/.rodata/RTTI to a retail-empty TU).
extern "C" void* lbl_eu_80526348[];

class __declspec(novtable) CMcaFile {
public:
    virtual ~CMcaFile();

    // 0x00: vtable
    u8 mFlag;       // 0x04
    void* mData;    // 0x08
    void* mDataAdj; // 0x0C
};

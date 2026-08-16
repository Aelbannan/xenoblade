#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

// Full object layout for CNumSelect (used by C-linkage accessors)
struct CNumSelectFull {
    u8 _00[0x2C];
    u8 field_2C;
    u8 field_2D;
    u8 field_2E;
};

class CNumSelect {
public:
    CNumSelect();
    virtual ~CNumSelect();
    void OnFileEvent();

    UnkClass_8045F564 mMemRegion;   // +0x04 (0x10 bytes)
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(void*, void*, u8);


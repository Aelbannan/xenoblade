#pragma once

#include <types.h>

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

    // TODO: add fields
};


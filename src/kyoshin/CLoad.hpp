#pragma once

#include <types.h>

// Full object layout for CLoad (used by C-linkage accessors)
struct CLoadFull {
    u8 _00[0x1C];
    u32 field_1C;
    u8 _20[0x28 - 0x20];
    u8 field_28;
    u8 _29[0x2B - 0x29];
    u8 field_2B;
    u8 field_2C;
};

class CLoad {
public:
    CLoad();
    virtual ~CLoad();
    void OnFileEvent();

    // TODO: add fields
};


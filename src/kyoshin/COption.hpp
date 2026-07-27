#pragma once

#include <types.h>

class CCur19 {
public:
    CCur19();

    // TODO: add fields
};

// Full object layout for COption (used by C-linkage accessors)
struct COptionFull {
    u8 _00[0x2B];
    u8 field_2B;
    u8 _2C[0x30 - 0x2C];
    u8 field_30;
    u8 _31;
    u8 field_32;
};

class COption {
public:
    COption();
    virtual ~COption();
    void OnFileEvent();

    // TODO: add fields
};


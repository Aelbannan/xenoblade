#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"

// Vtable symbol for CCur19 (6 entries, .data)
extern "C" void* lbl_eu_805396D0[];

class CCur19 : public CBaseCur {
public:
    CCur19(nw4r::lyt::ArcResourceAccessor* arcResAcc);

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


#pragma once

#include <types.h>

// Full object layout for CFloorMap (used by C-linkage accessors)
struct CFloorMapFull {
    u8 _00[0x40];
    u8 field_40;
    u8 _41;
    u8 _42[0x58 - 0x42];
    u8 field_58;
    u8 _59[0x208 - 0x59];
    u8 field_208;
};

class CFloorMap {
public:
    CFloorMap();
    virtual ~CFloorMap();
    void OnFileEvent() const;

    // TODO: add fields
};


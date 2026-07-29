#pragma once

#include <types.h>

// Full object layout for CFloorMap (used by C-linkage accessors)
struct CFloorMapFull {
    u8 _00[0x40];
    u8 field_40;
    u8 field_41;
    u8 _42[0x58 - 0x42];
    u8 field_58;
    u8 field_5D;
    u8 _5E[0x140 - 0x5E];
    void* field_140;
    u8 _144[0x208 - 0x144];
    u8 field_208;
};

class CFloorMap {
public:
    CFloorMap();
    virtual ~CFloorMap();
    void OnFileEvent() const;

    // TODO: add fields
};


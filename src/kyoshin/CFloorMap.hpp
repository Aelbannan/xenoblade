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
    u8 _144[0x205 - 0x144];
    s8 field_205;
    s8 field_206;
    s8 field_207;
    u8 field_208;
    u8 _209[0x30D0 - 0x209];
    u8 _30D0[0x32D4 - 0x30D0];
    void* field_32D4;
    u8 _32D8[0x333C - 0x32D8];
    u8 field_333C;
};

class CFloorMap {
public:
    CFloorMap();
    virtual ~CFloorMap();
    void OnFileEvent() const;

    // TODO: add fields
};


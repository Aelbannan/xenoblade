#pragma once

#include <types.h>

// Full `this` struct for harness decomp of CCollepedia.cpp
// Entry within the sub-block array (stride 0xA)
struct CCollepediaEntry {
    u8 _00[0x08];
    u16 field_08; // u16 at offset +0x08 within entry
};

struct CCollepediaFull {
    u8 _00[0xD8];
    u8 field_D8;
    u8 field_D9;
    u8 _DA[0xE8 - 0xDA];
    u8 field_E8_start; // sub-array starts at +0xE8
};

class CCollepedia {
public:
    CCollepedia();
    virtual ~CCollepedia();

    // TODO: add fields
};

#pragma once

#include <types.h>

class CModelDisp {
public:
    CModelDisp();
    virtual ~CModelDisp();

    // TODO: add fields
    u8 _pad2FD8[0x2FD8];
    u8 field_2FD8; // 0x2FD8
    u8 _pad2FD9[0x0B];
    u8 field_2FE4; // 0x2FE4
};


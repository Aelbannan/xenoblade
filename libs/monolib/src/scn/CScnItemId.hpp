#pragma once

#include <types.h>

class CScnItemId {
public:
    virtual ~CScnItemId();

    u8 _04[0x08];
    void* subObject;
    u8 _10[0x40];
    u32 value50;
};

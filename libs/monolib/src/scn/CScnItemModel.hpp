#pragma once

#include <types.h>

class CScnItemModel {
public:
    CScnItemModel();
    virtual ~CScnItemModel();

    u8 _04[0x7E0];
    u32 value7E4;
    u32 value7E8;
    u8 _7EC[0x6C];
    f32 value858;
};


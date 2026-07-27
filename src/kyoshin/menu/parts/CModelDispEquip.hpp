#pragma once

#include <types.h>

class CModelDispEquip {
public:
    CModelDispEquip();
    virtual ~CModelDispEquip();
    void OnFileEvent();

    // TODO: add fields
    u8 _pad1014[0x1014];
    u8 field_1014; // 0x1014
    u8 _pad1015[0x0B];
    u8 field_1020; // 0x1020
    u8 field_1021; // 0x1021
};


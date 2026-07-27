#pragma once

#include <types.h>

class CPartyStateWin {
public:
    CPartyStateWin();
    virtual ~CPartyStateWin();
    void cbRenderBefore();

    // TODO: add fields
    u8 _pad6BE5[0x6BE5];
    u8 field_6BE5; // 0x6BE5
};


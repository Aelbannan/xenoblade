#pragma once

#include <types.h>

class CPartyState {
public:
    CPartyState();
    virtual ~CPartyState();
    void OnFileEvent();

    u8 _00[0x30];
    u8 field_0x30;
    u8 field_0x31;
    u8 _32[0x4C - 0x32];
    u8 field_0x4C;
    s8 field_0x4D;
    u8 field_0x4E;
    u8 _4F[0x57 - 0x4F];
    u8 field_0x57;
};

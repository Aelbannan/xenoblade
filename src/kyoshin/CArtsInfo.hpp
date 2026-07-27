#pragma once

#include <types.h>

class CArtsInfo {
public:
    CArtsInfo();
    virtual ~CArtsInfo();
    void OnFileEvent();

    u8 _00[0x20];
    int field_0x20;
    u8 _24[0x40 - 0x24];
    u8 field_0x40;
    u8 _41[0x44 - 0x41];
    int field_0x44;
    u8 field_0x48;
    u8 field_0x49;
    u8 _4A[0x4C - 0x4A];
    int field_0x4C;
    int field_0x50;
    u8 field_0x54;
    u8 field_0x55;
    u8 field_0x56;
    u8 _57;
    u16 field_0x58;
    s8 field_0x5A;
};

#pragma once

#include <types.h>

class CKizunaRadar {
public:
    CKizunaRadar();
    virtual ~CKizunaRadar();

    u32 field_0x04;  // 0x04 - constructor r4 parameter stored here
    u32 field_0x08;  // 0x08
    u32 field_0x0C;  // 0x0C
    u32 field_0x10;  // 0x10
    float field_0x14; // 0x14
    float field_0x18; // 0x18
};

class CKizunaCur {
public:
    CKizunaCur();
    virtual ~CKizunaCur();

    // TODO: add fields
};

class CKizunaLine {
public:
    CKizunaLine();
    virtual ~CKizunaLine();

    // TODO: add fields
};

class CKizunaInfo {
public:
    CKizunaInfo();
    virtual ~CKizunaInfo();

    // TODO: add fields
};

class CKizunagram {
public:
    CKizunagram();
    virtual ~CKizunagram();
    void OnFileEvent();

    u8 _04[0x34];   // 0x04 - 0x37
    u8 field_0x38;  // 0x38
    u8 field_0x39;  // 0x39
    u8 _3A[2];      // 0x3A - 0x3B
    u8 field_0x3C;  // 0x3C
    u8 _3D[0xF];    // 0x3D - 0x4B
    u8 field_0x4C;  // 0x4C - subobject passed to func_8025B958
    u8 _4D[0x3F];   // 0x4D - 0x8B
    u8 field_8C;    // 0x8C
    u8 _8D[0x1F];   // 0x8D - 0xAB
    u8 field_0xAC;  // 0xAC - subobject passed to func_80257F9C
    u8 _AD[0x10];   // 0xAD - 0xBC
    u8 field_0xBD;  // 0xBD
};


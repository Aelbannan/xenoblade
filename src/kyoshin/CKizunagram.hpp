#pragma once

#include <types.h>

class CKizunaRadar {
public:
    CKizunaRadar();
    virtual ~CKizunaRadar();

    // TODO: add fields
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
    u8 _3D[0x4F];   // 0x3D - 0x8B
    u8 field_8C;    // 0x8C
    u8 _8D[0x30];   // 0x8D - 0xBC
    u8 field_0xBD;  // 0xBD
};


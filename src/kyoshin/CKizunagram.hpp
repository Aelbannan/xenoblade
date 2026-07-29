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

    // vtable pointer at 0x00 (implicit)
    u8 _04[0x26 - 0x04];
    u16 field_0x26;         // 0x26
    u8 _28[0x34 - 0x28];
    u8 field_0x34;          // 0x34
    u8 _35;
    u16 field_0x36;         // 0x36
    float field_0x38;       // 0x38
    u8 field_0x39;          // 0x39
    u8 _3A;
    u8 field_0x3B;          // 0x3B
    u8 field_0x3C;          // 0x3C
    u8 _3D[0x61 - 0x3D];
    u8 field_0x61;          // 0x61
    u8 field_0x62;          // 0x62
    u8 _63[0x8C - 0x63];
    u8 field_8C;            // 0x8C
};


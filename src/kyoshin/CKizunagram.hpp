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

    u8 _04[0x34 - 0x04];
    u8 field_0x34;
    u16 field_0x36;
    float field_0x38;
    u8 _3C[0x8C - 0x3C];
    u8 field_8C; // 0x8C
};


#pragma once

#include <types.h>

class CTitleLogo {
public:
    CTitleLogo();
    virtual ~CTitleLogo();

    /* 0x04 */ u8 field_0x04[0x0C]; // unknown
    /* 0x10 */ u8 field_0x10[0x08]; // unknown (AnimTransform ptrs? see func_802B66B8)
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u8 field_0x1A; // state: 0 -> 1 (func_802B6534), 2 -> 3 (func_802B6580)
};

class CTitleMenu {
public:
    CTitleMenu();
    virtual ~CTitleMenu();

    /* 0x04 */ u8 field_0x04[0x18]; // unknown
    /* 0x1C */ u8 field_0x1C[0x08]; // unknown (AnimTransform ptr? see func_802B6EF8)
    /* 0x24 */ u8 field_0x24;
    /* 0x25 */ u8 field_0x25;
    /* 0x26 */ u8 field_0x26; // state: 0 -> 1 (func_802B6B90), 2 -> 3 (func_802B6BDC), 5 -> 6 (func_802B6C74)
};

class CTitle {
public:
    CTitle();
    virtual ~CTitle();
    void OnFileEvent();

    // TODO: add fields
};

namespace cf {

class CHelp_ToAttack {
public:
    void func_802B7A90();

    // TODO: add fields
};
} // namespace cf


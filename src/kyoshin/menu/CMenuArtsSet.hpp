#pragma once

#include <types.h>

// 2D position used as source for func_80231848.
struct SArtsVec2 {
    float x;
    float y;
};

// Anonymous menu-sub-object constructed by __ct__8022FA58.
// Shared by the func_8023xxxx helpers (func_8023040C, func_8023080C,
// func_80231464, func_8022FE90).
class SArtsSub8022FA58 {
public:
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u8  field_0x14;
    u8  field_0x15;
    u8  field_0x16;
    u8  field_0x17;
    u8  field_0x18;
    u8  field_0x19[7];       // 0x19-0x1F
    s8  field_0x20;
    u8  field_0x21;
    u8  field_0x22;
    char _pad_0x23[0x27-0x23];
    u8  field_0x27;
    char _pad_0x28[0x31-0x28];
    u8  field_0x30;
    u8  field_0x31;
};

// Signed variant whose 0x20/0x21 bytes are tested with sign-extend
// compares in func_8023270C.
class SArts3270C {
public:
    char _pad[0x20];
    s8 field_0x20;
    s8 field_0x21;
};

// Object holding a flag at 0x16F and a value at 0x2A1 (func_80234C84).
class SArts34C84 {
public:
    char _pad[0x16F];
    u8 field_0x16F;
    char _pad2[0x2A1-0x170];
    u8 field_0x2A1;
};

// Object with a word at 0x1C and flags at 0x28/0x30 (func_80235108).
class SArts35108 {
public:
    char _pad[0x1C];
    u32 field_0x1C;
    char _pad20[0x28-0x20];
    u8 field_0x28;
    char _pad29[0x30-0x29];
    u8 field_0x30;
};

// Pointee of SArtsSub8022FA58.field_0x08; field_0x10 read by func_8023080C.
class SArts080C {
public:
    char _pad[0x10];
    void* field_0x10;
};

// Element of the 0x49-byte array indexed by func_80230FF0.
class SArtsSubDElem {
public:
    char _pad[0x30];
    u8 field_0x30;          // 0x30 flag
    u8 data[0x18];          // 0x31-0x48
};

class CMenuArtsSet {
public:
    CMenuArtsSet();

    void func_80231848(void* src);
    u8 func_80233880();
    void func_80233968();
    void func_802349F8(u8 val);
    void func_80234A00();

    // +0x00: vtable (placeholder for the 4-byte vtable pointer)
    void* _vtable;                       // 0x00
    char _pad_04[0x31 - 0x04];   // 0x04-0x30
    u8 mField31;                   // 0x31
    char _pad_32[0x44 - 0x32];   // 0x32-0x43
    float mField44;                // 0x44
    float mField48;                // 0x48
    char _pad_4C[0x58 - 0x4C];   // 0x4C-0x57
    char mSubObj58_start;          // 0x58 - sub-object (virtual interface, thunks adjust from here)
    char _pad_59[0x74 - 0x59];   // 0x59-0x73
    char mSubObj74[0x139 - 0x74]; // 0x74-0x138
    u8 mField139;               // 0x139
    char _pad_13A[0x148 - 0x13A]; // 0x13A-0x147
    char mSubObj148[1];         // 0x148+
};

class CArtsList {
public:
    CArtsList();
    virtual ~CArtsList();
    void OnFileEvent();

    // TODO: add fields
};

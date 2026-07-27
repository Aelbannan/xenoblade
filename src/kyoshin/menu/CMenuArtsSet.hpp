#pragma once

#include <types.h>

class CMenuArtsSet {
public:
    CMenuArtsSet();

    // +0x00: vtable
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

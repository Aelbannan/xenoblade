#pragma once

#include <types.h>

struct ResEntry {
    u8 _00[4];
    u8 data[4];      // 0x04 - returned by lookup functions
    u32 id;           // 0x08
    u8 _0C[0x2A];    // 0x0C - 0x35
    u8 value;         // 0x36
    u8 _37[5];        // 0x37 - 0x3B
};

struct ResContainer {
    u8 _00[0x14DC];
    ResEntry table[0x28]; // 0x14DC - 40 entries

    u8* findResEntry(u32 id, u32* outIndex, u32* outValue);
};

char* getEntryPtrGrid(char* self, int a, int b);
void clearWordFlag(u32* self, u32 mask);
bool testResInfoFlag(u32 flags);
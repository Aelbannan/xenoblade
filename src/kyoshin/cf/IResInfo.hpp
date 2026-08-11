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

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char* getEntryPtrGrid(char* self, int a, int b);
extern "C" void clearWordFlag(u32* self, u32 mask);
extern "C" void* func_80068494(void*, u32, u32*, u32*, int, int);
extern "C" bool func_800686E4(void*, bool);

extern "C" bool testResInfoFlag(u32 flags);
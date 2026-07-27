#pragma once

#include <types.h>

class CMCCrystalBox {
public:
    CMCCrystalBox();
    virtual ~CMCCrystalBox();
    void OnFileEvent();

    // Fields — populated from standalone accessor functions
    u8 pad_00[0x20];
    u8 unk20;                       // +0x20: guard byte (0 = no entries)
    u8 pad_21[0x29 - 0x21];
    u8 unk29;                       // +0x29: table index
    u8 pad_2A[0x69 - 0x2A];
    u8 unk69;                       // +0x69: read by getByte_69_802146C0
    u8 pad_6A[0x2D4 - 0x6A];
    u8 unk2D4;                      // +0x2D4: read by getByte_2D4_8021624C
    u8 unk2D5;                      // +0x2D5: read by getByte_2D5_80216254
};


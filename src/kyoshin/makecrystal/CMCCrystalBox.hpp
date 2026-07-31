#pragma once

#include <types.h>

class CMCCrystalBox {
public:
    CMCCrystalBox();
    virtual ~CMCCrystalBox();
    void OnFileEvent();

    // Fields — vtable at +0x00 (implicit), user fields start at +0x04
    u8 pad_00[0x1C];                    // +0x04 to +0x1F
    u8 unk20;                           // +0x20: entry count (0 = no entries)
    u8 pad_21[0x24 - 0x21];
    void* subObjPtrs[0x0F];            // +0x24 to +0x5F: array of 15 sub-object pointers
    u8 unk60;                           // +0x60
    u32 pad_61_pad_63;
    u32 unk64;                          // +0x64: state/status
    u8 unk68;                           // +0x68
    u8 unk69;                           // +0x69: active flag
    u8 pad_6A[0x6C - 0x6A];
    // +0x6C: CCur07
    // +0x84: CCur09
    // +0x9C: CCur18
    // +0xB4: CCur11
    // +0xCC: CCur16
    // +0xE4: CCur18
    // +0xFC: CSortMenu
    // +0x1EC: CMCCrystalInfo
    // +0x250: CScrollBar
    // +0x290: CSysWin
    u8 pad_6C[0x2CC - 0x6C];
    u8 unk2CC;                          // +0x2CC
    u8 unk2CD;                          // +0x2CD
    u8 unk2CE;                          // +0x2CE
    u8 unk2CF;                          // +0x2CF
    u8 unk2D0;                          // +0x2D0: parent type
    u8 unk2D1;                          // +0x2D1
    u8 unk2D2;                          // +0x2D2
    u8 unk2D3;                          // +0x2D3
    u8 unk2D4;                          // +0x2D4: read by getByte_2D4_8021624C
    u8 unk2D5;                          // +0x2D5: read by getByte_2D5_80216254
    u8 pad_2D6[0x1500 - 0x2D6];
    u8 unk1500;                         // +0x1500
    u8 pad_1501[0x1504 - 0x1501];
    u16 unk1504;                        // +0x1504
    u8 pad_1506[0x1508 - 0x1506];
};
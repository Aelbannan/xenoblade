#pragma once

#include <types.h>

namespace nw4r { namespace lyt { class AnimTransform; } }

// 8-byte crystal box param: two s16 counters (selected id, quantity), a
// spare s16, and two flag bytes. Inlined low-level value type for the
// 32-slot sub-tables at CMCCrystalBox+0x2D8 and the transient sort table.
struct CMCCrystalBoxParam {
    s16 m0;   // +0x00 selected id
    s16 m2;   // +0x02 quantity
    s16 m4;   // +0x04 spare
    u8  m6;   // +0x06 flag
    u8  m7;   // +0x07 flag

    void initCrystalBoxParam_80213E04(s16 a, s16 b);
};

// Entry in the crystal-state table rooted at CMCCrystalBox+0x3D8.
struct CMCCrystalDataEntry {
    s16  id;     // +0x00
    u8   flag;   // +0x02
    u8   pad3;   // +0x03
};

// Crystal-state table rooted at CMCCrystalBox+0x3D8: 1024 entries (4 bytes
// each), then a small state header, then the name buffer for sprintf.
struct CMCCrystalData {
    CMCCrystalDataEntry entries[0x400]; // +0x000
    u16  count;                         // +0x1000
    u8   byte_1002;                     // +0x1002 (category)
    u8   limit;                         // +0x1003
    s8   current;                       // +0x1004
    char name[0x10C8 - 0x1005];         // +0x1005 (buffer passed to sprintf)
};

class CMCCrystalBox {
public:
    CMCCrystalBox(u8 parentType);
    virtual ~CMCCrystalBox();
    void OnFileEvent();

    // Fields — vtable at +0x00 (implicit), user fields start at +0x04
    u8 pad_00[0x1C];                    // +0x04 to +0x1F
    u8 unk20;                           // +0x20: entry count (0 = no entries)
    u8 pad_21[0x24 - 0x21];
    nw4r::lyt::AnimTransform* subObjPtrs[0x0F]; // +0x24 to +0x5F: array of 15 sub-object pointers
    u8 unk60;                           // +0x60
    u8 pad_61_pad_63[3];
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
    u8 unk2D6;                          // +0x2D6: selected-crystal table cursor (written/returned by func_8021625C)
    u8 unk2D7;                          // +0x2D7: current item id byte
    CMCCrystalBoxParam subTable[32];    // +0x2D8 to +0x3D8: selected-crystal table (32 slots)
    CMCCrystalData data;                // +0x3D8: crystal-state table (ends at +0x14A0)
    u8 field_14A0;                      // +0x14A0
    u8 pad_14A1[0x14EC - 0x14A1];       // +0x14A1..0x14EB
    u8 field_14EC[5];                   // +0x14EC..0x14F0 (indexed by field_14F2)
    u8 field_14F1;                      // +0x14F1
    u8 field_14F2;                      // +0x14F2: countdown ticker / index
    u8 pad_14F3[0x1500 - 0x14F3];
    u8 unk1500;                         // +0x1500
    u8 pad_1501[0x1504 - 0x1501];
    u16 unk1504;                        // +0x1504
    u8 field_1506;                      // +0x1506
    u8 field_1507;                      // +0x1507
};
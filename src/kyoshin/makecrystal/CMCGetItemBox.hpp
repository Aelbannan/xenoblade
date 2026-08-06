#pragma once

#include <types.h>
#include "kyoshin/CItemBoxInfo.hpp"

/* 52-byte entry referenced by the item-box index table.
   Only the first word is known (read by func_80296D54). */
struct CMCItemBoxEntry {
    u32 field_00;                   // 0x00
    u8 bytes[0x34 - 0x04];          // 0x04-0x33
};

/* Small sub-object passed to the C-linkage menu helpers
   (func_801D216C / func_801599D4). Layout not recovered. */
struct CMCItemBoxSubObj {
    u8 bytes[0x1];
};

/* Echo/placeholder for the CSysWin sub-object ranges. */
struct CMCGetItemBoxSysWin {
    u8 bytes[0x3C];                 // 0x00-0x3B
};

/* Item-box index structure at +0x314 of CMCGetItemBox.
   An s16 offset table, count, counter and backing entry array. */
struct CMCItemBoxSub {
    s16 table[0x80];                // 0x000-0x0FF: offset table
    u16 count;                      // 0x100: number of entries
    u8  pad_102;                    // 0x102 (also read as an index source)
    u8  limit;                      // 0x103
    u8  counter;                    // 0x104
    u8  pad_105[0x108 - 0x105];     // 0x105-0x107
    u8  field_108;                  // 0x108
    u8  pad_109[0x148 - 0x109];     // 0x109-0x147
    u32 field_148;                  // 0x148
    u8  field_14C;                  // 0x14C
    u8  pad_14D[0x1CC - 0x14D];     // 0x14D-0x1CB
    u32 field_1CC;                  // 0x1CC
    CMCItemBoxEntry* listBase;      // 0x1D0: 52-byte Entry array base
    u8  field_1D4;                  // 0x1D4
    u8  pad_1D5[0x1D8 - 0x1D5];     // 0x1D5-0x1D7
};

class CMCGetItemBox {
public:
    CMCGetItemBox();
    virtual ~CMCGetItemBox();
    void OnFileEvent();

    u8 func_80297D1C();
    u8 func_80297D24();

    // +0x00: vtable
    u8 _pad_04[0x3C - 0x04];          // 0x04-0x3B
    u32 field_3C;                     // 0x3C
    u32 field_40;                     // 0x40 (layout/anim object w/ vtable)
    void* field_44;                   // 0x44 (AnimTransform)
    void* field_48;                   // 0x48 (AnimTransform)
    u8 field_4C;                      // 0x4C
    u8 field_4D;                      // 0x4D
    u8 _pad_4E[0x54 - 0x4E];          // 0x4E-0x53
    u8 field_54;                      // 0x54
    u8 mField55;                      // 0x55
    u8 _pad_56[0x58 - 0x56];          // 0x56-0x57
    CMCItemBoxSubObj subObj_58;       // 0x58
    u8 _pad_59[0x70 - 0x59];          // 0x59-0x6F
    CMCItemBoxSubObj subObj_70;       // 0x70
    u8 _pad_71[0x88 - 0x71];          // 0x71-0x87
    CMCItemBoxSubObj subObj_88;       // 0x88
    u8 _pad_89[0xA0 - 0x89];          // 0x89-0x9F
    CMCItemBoxSubObj subObj_A0;       // 0xA0
    u8 _pad_A1[0xB8 - 0xA1];          // 0xA1-0xB7
    CMCGetItemBoxSysWin sysWin_B8;    // 0xB8-0xF3
    CItemBoxInfo itemBox;             // 0xF4
    u8 _pad[0x301 - (0xF4 + sizeof(CItemBoxInfo))];  // pad to 0x301
    u8 field_301;                     // 0x301
    u8 _pad_302;                      // 0x302
    u8 mField303;                     // 0x303
    u8 _pad_304[0x314 - 0x304];       // 0x304-0x313
    CMCItemBoxSub sub_314;            // 0x314
};
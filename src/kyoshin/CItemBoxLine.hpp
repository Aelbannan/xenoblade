#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"

/* 12-byte item-box tab entry: halves/words/bytes as stored per slot. */
struct CIBLTabEntry {
    u16 f0;   // +0x0
    u32 f4;   // +0x4
    u8 f8;    // +0x8
    u8 f9;    // +0x9
};

/* Tab holder embedded in CItemBoxLine (+0x3A4): 12 entries then a halfword count. */
struct CIBLTab {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
};

class CIBLTabCur : public CBaseCur {
public:
    CIBLTabCur();

    // CBaseCur occupies sizeof(CBaseCur)=0x18 bytes (4-aligned); byte table at +0xA8.
    u8 pad_18[0xA8 - 0x18]; // 0x18..0xA7
    u8 unkA8[12];           // +0xA8
};

class CItemBoxLine {
public:
    CItemBoxLine();
    virtual ~CItemBoxLine();
    void OnFileEvent();

    void func_801EDA4C(unsigned char val);
    void func_801EECC8();

    // Tab storage (byte entries + count) + unknown byte fields.
    // CItemBoxLine has an implicit vtable pointer at +0x00, so the first
    // explicit member starts at +0x04.
    u8 pad_04[0x59 - 0x04]; // 0x04..0x58
    u8 unk59;               // +0x59: read by func_801ED800
    u8 tabEntries[9];           // 0x5A..0x62: entry bytes, cleared by func_801EDA08
    u8 tabCount;                // +0x63: count
    u8 pad_64[0x6D - 0x64];     // 0x64..0x6C
    u8 field6D;                 // +0x6D
    u8 pad_6E[0x38C - 0x6E];    // 0x6E..0x38B
    s16 unk38C;                 // +0x38C: read by func_801EECC8
    s16 unk38E;                 // +0x38E: read by func_801EECC8
    u8 pad_390[0x0E];           // 0x390..0x39D
    u8 unk39E;                  // +0x39E: read by func_801EECC0
    u8 pad_39F[0x05];           // 0x39F..0x3A3
    CIBLTab unk3A4;             // +0x3A4: target of func_801EC3B0 call
};
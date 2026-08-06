#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CNumSelect.hpp"
#include <monolib/util.hpp>

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

/* Tab holder with a trailing FixStr<32> name buffer at +0x94 (func_801EC3D0). */
struct CIBLTabFormat {
    CIBLTabEntry entries[12]; // +0x00..0x8F
    u16 count;                // +0x90
    u8 pad_92[2];             // +0x92..0x93
    ml::FixStr<32> str94;     // +0x94
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
    u8 pad_6E[0x2DC - 0x6E];    // 0x6E..0x2DB
    CNumSelectFull mNumSel;     // +0x2DC: member of func_801ED808
    u8 pad_30C[0x350 - 0x30B];  // 0x30B..0x34F (CNumSelectFull is 0x2F bytes)
    CSysWinFull mSysWin;        // +0x350: member of func_801ED808
    u8 pad_387[0x38C - 0x387];  // 0x387..0x38B
    s16 unk38C;                 // +0x38C: read by func_801EECC8
    s16 unk38E;                 // +0x38E: read by func_801EECC8
    u8 pad_390[0x0E];           // 0x390..0x39D
    u8 unk39E;                  // +0x39E: read by func_801EECC0
    u8 pad_39F;                 // 0x39F..0x39F
    u8 field3A0;                // +0x3A0: returned by func_801ED808
    u8 pad_3A1[0x3A4 - 0x3A1];  // 0x3A1..0x3A3
    CIBLTab unk3A4;             // +0x3A4: target of func_801EC3B0 call
};
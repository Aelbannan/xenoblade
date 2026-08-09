#pragma once

#include <types.h>

// Callback table record at lbl_eu_805739F8 (size 0x28).
// Owns an array of callbacks invoked by func_800C1EB8; field20 is the index
// of the "active" callback slot, field24 is the continue-flag that a callback
// can set to keep the dispatch loop running.
struct Unk805739F8 {
    void (*fns[8])(void);   // 0x00 - 0x1F  callback pointers
    u32 field20;            // 0x20  active slot index
    u8 field24;             // 0x24  continue flag
};
extern Unk805739F8 lbl_eu_805739F8;

// Flag/status record targeted by func_800C1CAC.
// Placed in .bss at lbl_eu_805739E8 (size 0x10).
struct Unk805739E8 {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u32 field_0C;
};
extern Unk805739E8 lbl_eu_805739E8;

// One parsed entry in the text/tips dispatch table: a filename hash plus an
// index/id word and a sequence number.  8 bytes, stored `stw pt, off; sth, off+4;
// sth, off+6`.
struct TextEntry {
    u32 hash;      // 0x00  func_800AA714(path) result
    u16 field4;    // 0x04  atoi(value) when provided
    u16 field6;    // 0x06  running sequence counter
};

// The 0x628-byte parsed-scenario/table buffer owned by CmTextProc::buf.
// Six sets of TextEntry arrays sit at disjoint offsets; each set has an index
// counter in the 0x618-0x622 region and a shared total counter at 0x624.
struct CmTextTable {
    TextEntry set0[0x20];  // 0x000
    TextEntry set1[0x20];  // 0x100
    TextEntry set2[0x40];  // 0x200
    TextEntry set3[0x40];  // 0x400
    TextEntry set4[0x02];  // 0x600
    TextEntry set5[0x01];  // 0x610
    u16 cnt0;              // 0x618
    u16 cnt1;              // 0x61a
    u16 cnt2;              // 0x61c
    u16 cnt3;              // 0x61e
    u16 cnt4;              // 0x620
    u16 cnt5;              // 0x622
    u16 cntAll;            // 0x624
    u16 pad_626;           // 0x626
}; // size 0x628

// Parser state: the table being filled, the original line base, and the
// read cursor advanced by func_800C1A18 as lines/tokens are consumed.
struct CmTextProc {
    CmTextTable* buf;      // 0x00
    const char* base;      // 0x04
    const char* cursor;    // 0x08
};

// Keyword dispatch table (data at lbl_eu_8052A528): maps a scenario keyword
// string to an id 0-5 selecting which TextEntry set to write into.
struct TextKeyword {
    const char* word;      // 0x00
    u32 id;                // 0x04
};
extern TextKeyword lbl_eu_8052A528[6];

// cf cue-script decoder routines (this TU).  Retail symbols are unmangled C
// names (func_XXXX), so the declarations use C linkage.
extern "C" {
int func_800C17DC(const char* str, int* type, int* len);
int func_800C1900(char* str, char** out, int maxLen);
int func_800C1A18(CmTextProc* self, char* out, int maxLen);
void func_800C1B30(CmTextProc* self, int key, const char* name, const char* value);
void func_800C1CC4(CmTextProc* self, const char* text, void* buf);
}
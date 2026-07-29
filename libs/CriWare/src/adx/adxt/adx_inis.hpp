#pragma once

#include <types.h>

// Forward declaration for ADXT_Tsvr (defined in adx_tsvr.c)
typedef struct ADXT_Tsvr ADXT_Tsvr;

// Handle entry in the ADXT handle array at offset 0x20 of AdxInisContext.
// Each entry is 0xC4 bytes; the first byte is a flag checked with extsb.
struct AdxInisHandle {
    s8 flag;                // 0x00 - checked with extsb.
    u8 field_0x01[0xC3];    // 0x01-0xC3
};

// Global context for ADXT_Init/ADXT_Finish, stored at lbl_eu_805E26C8.
struct AdxInisContext {
    s32 refcount;           // 0x00
    s32 field_0x04;         // 0x04
    s32 handle_0x08;        // 0x08
    s32 field_0x0C;         // 0x0C
    s32 handle_0x10;        // 0x10
    s32 field_0x14;         // 0x14
    u8* field_0x18;         // 0x18
    s32 field_0x1C;         // 0x1C
    struct AdxInisHandle handles[16];  // 0x20
};

extern struct AdxInisContext lbl_eu_805E26C8;
extern const char lbl_eu_80515FB8[];
extern const char lbl_eu_80516010[];

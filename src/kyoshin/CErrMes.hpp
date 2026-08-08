#pragma once

/**
 * Error-message UI helper (EU region).
 *
 * Trampolines into CBdat for the error-screen bdat index.
 */

#include <types.h>

// Tail-call trampoline: sets error bdat index 6 and forwards to CBdat.
void func_eu_802B1334();

// Small language-entry record patched by func_eu_802B14F8. Only bytes +4..+6
// are written by the retail code (two-char language code then '/' separator);
// the leading bytes are unknown/padding.
struct ErrMesLangEntry {
    char field_0; // +0x00
    char field_1; // +0x01
    char field_2; // +0x02
    char field_3; // +0x03
    char mLang0;  // +0x04
    char mLang1;  // +0x05
    char mSep;    // +0x06
};

// Array of bdat table pointers selectable by language (split-1 .data).
extern u32 lbl_eu_8053A420[];

// Extended language-entry pointer tables (split-1 .data). Each element points
// to an ErrMesLangEntry record mutated by func_eu_802B14F8.
extern ErrMesLangEntry* lbl_eu_8053A438[];
extern ErrMesLangEntry* lbl_eu_8053A458[];
extern ErrMesLangEntry* lbl_eu_8053A478[];

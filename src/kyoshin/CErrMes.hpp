#pragma once

/**
 * Error-message UI helper (EU region).
 *
 * Trampolines into CBdat for the error-screen bdat index.
 */

#include <types.h>

// Opaque struct for text objects: language code at 0x04-0x05, separator at 0x06.
// The full struct is larger; only these 3 byte fields are touched by this unit.
struct CErrMesTextObj {
    u8 _00[4];
    u8 field_0x04;
    u8 field_0x05;
    u8 field_0x06;
};

// Tail-call trampoline: sets error bdat index 6 and forwards to CBdat.
void func_eu_802B1334();
void* func_eu_802B12DC();
void func_eu_802B14F8(int param);

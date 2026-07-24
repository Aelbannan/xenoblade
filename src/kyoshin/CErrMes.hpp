#pragma once

/**
 * Error-message UI helper (EU region).
 *
 * Trampolines into CBdat for the error-screen bdat index.
 */

#include <types.h>

// Tail-call trampoline: sets error bdat index 6 and forwards to CBdat.
void func_eu_802B1334();

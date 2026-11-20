#pragma once

#include <types.h>

// Canonical C++ declarations for the packed-token helpers defined in
// code_800AA008.cpp. Kept separate from code_800AA008.hpp because that header
// also declares Tok_FormatStr with a different (legacy) linkage view.
u32 Tok_Pack08(u32 a, u32 b);
u32 Tok_Pack10(u32 a, u32 b, u32 c);
u32 Tok_Pack20(u32 a, u32 b, u32 c);
void Tok_Unpack(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);

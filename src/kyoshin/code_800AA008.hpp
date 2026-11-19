#pragma once

#include <types.h>
#include "monolib/util.hpp"

void Tok_AppendTxt(ml::FixStr<64>& buf, int type, u32 arg1, u32 arg2, u32 arg3);
int ParseDigitStr(const char* str, int digitCount, int* out);
u32 Tok_Pack08(u32 a, u32 b);
u32 Tok_PackE0(u32 a, u32 b, u32 c);
u32 Tok_Pack10(u32 a, u32 b, u32 c);
u32 Tok_Pack20(u32 a, u32 b, u32 c);
void Tok_Unpack(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
int Tok_FormatStr(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
void Tok_FormatRes();
u32 Tok_GetPrefix(const char* str);
// Retail symbol is unmangled (Tok_ParseFile), so C linkage: every caller's
// reloc must use the plain name.
extern "C" u32 Tok_ParseFile(const char* path);
void sinit_800AABBC();

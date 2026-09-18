#pragma once

#include <types.h>
#include "monolib/util.hpp"

void func_800AA008(ml::FixStr<64>& buf, int type, u32 arg1, u32 arg2, u32 arg3);
int func_800AA1B4(const char* str, int digitCount, int* out);
u32 Tok_Pack08(u32 a, u32 b);
u32 Tok_PackE0(u32 a, u32 b, u32 c);
u32 Tok_Pack10(u32 a, u32 b, u32 c);
u32 Tok_Pack20(u32 a, u32 b, u32 c);
void Tok_Unpack(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
void func_800AA5C0();
u32 func_800AA600(const char* str);
u32 func_800AA714(const char* path);
void sinit_800AABBC();

#pragma once

#include <types.h>

class CScn;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" float func_80496288(void*);
extern "C" u32 func_8049E51C(void*);
extern "C" void* func_804BE398(float*, u32, u32, u32, float, float);
extern "C" void func_804BE4B4(float*, u32);
extern "C" void func_804BE4E0(float*, u32);
extern "C" int func_8013EB90(int v);

// External data (retail linker symbols). Global-scope variables are not
// mangled by MWCC, so plain extern (C++ linkage) emits the same symbol.
extern CScn* lbl_eu_80663E14;
extern u32 lbl_eu_80663E24;
extern const float lbl_eu_80666B48;
extern const float lbl_eu_80666B4C;
extern const float lbl_eu_80666B50;
// String table at this address (address-of references emit lis/addi HA/LO).
extern const char lbl_eu_804FC648[];

// Retail keeps these BDAT entry points as their short/unmangled linker names.
extern "C" u32 getBdatStringColumnValue(void*, const char*, s32);

// BDAT manager reset/init (retail symbol unmangled; result discarded here).
extern "C" void* func_8003AA34(void);

extern void* lbl_eu_8052A3B0[];
extern void* lbl_eu_805294E0[];

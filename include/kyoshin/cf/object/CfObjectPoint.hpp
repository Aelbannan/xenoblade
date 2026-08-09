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

// External data (retail linker symbols)
extern "C" CScn* lbl_eu_80663E14;
extern "C" u32 lbl_eu_80663E24;
extern "C" float lbl_eu_80666B48;
extern "C" float lbl_eu_80666B4C;
extern "C" float lbl_eu_80666B50;
extern "C" const char* lbl_eu_804FC648;

// Retail keeps these BDAT entry points as their short/unmangled linker names.
extern "C" u32 getBdatStringColumnValue(void*, const char*, int);

extern "C" void* lbl_eu_8052A3B0[];
extern "C" void* lbl_eu_805294E0[];

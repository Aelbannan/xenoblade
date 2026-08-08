#pragma once

#include <types.h>

class UnkClass_800817BC;

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

// External data tables (u16 ID lookup arrays, 44 entries each)
extern "C" u16 lbl_eu_80501E48[];
extern "C" u16 lbl_eu_80501EB8[];
extern "C" u16 lbl_eu_80501F28[];
extern "C" u16 lbl_eu_80501F98[];

// External runtime / game functions (retail-unmangled symbols)
// CfGameManager utilities
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 value);
extern "C" UnkClass_800817BC* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);
extern "C" void func_800ACF78(void* obj, void* mgr, u32 val);
extern "C" void func_800ACEF8(void* obj, void* ptr);
extern "C" void func_800ACC50(void* self, float v);
extern "C" void func_80484EB0(void* ptr);

// External data (retail linker symbols)
// vtable for CCharEffectEne
extern "C" u8 lbl_eu_8052FDD8[];

// Base constructor
extern "C" void __ct__CCharEffect(void* self);

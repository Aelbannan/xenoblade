#pragma once

// Retail linker names for main.cpp's imports - declared here so the
// single-TU extern "C" block in main.cpp can be replaced by a header
// include (lint rule: no local extern "C" imports, use a proper header).

extern "C" {
// CErrMes helpers (defined in src/kyoshin/CErrMes.cpp - declared
// without C linkage there, but the retail TU exports the plain linker name;
// this C linkage in the importing TU preserves the plain-symbol reloc).
void func_eu_802B12DC();
void NandData_SetupBanner_11C0();
const char* getErrMesLangTable0();
const char* getErrMesLangTable1();
const char* getErrMesLangTable2();
const wchar_t* getErrMesText01();
const wchar_t* getErrMesText02();
const wchar_t* getErrMesText03();
const wchar_t* getErrMesText08();
const wchar_t* getErrMesText09();
const wchar_t* getErrMesText10();
void touchErrMesEntry();

// Device path/flag helpers (plain symbols exported by monolib device TUs).
void func_eu_804520B0(const void* p);
void CDeviceFont_SetSharedText(const wchar_t* msg);

// §17.6 asm backend reference - mangled CDesktop::entryTable bl target
// (defined as a member in libs/monolib/src/core/CDesktop.hpp).
void entryTable__8CDesktopFPQ28CDesktop16DESKTOP_ICON_DEFb(void* a, bool b);
}

// Data imports - at global scope plain extern is sufficient (no mangling).
extern unsigned char lbl_eu_8065FD00[];
extern signed char lbl_eu_80663B18;

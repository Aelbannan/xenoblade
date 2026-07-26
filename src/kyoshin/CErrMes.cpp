// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

void func_eu_802B1334() { CBdat::func_8003AA8C(6); }









extern "C" void func_eu_802B13FC() {}

extern "C" void func_eu_802B1414() {}

extern "C" void func_eu_802B142C() {}

extern "C" void func_eu_802B1444() {}

extern "C" void func_eu_802B145C() {}

extern "C" void func_eu_802B1474() {}

extern "C" void func_eu_802B148C() {}

extern "C" void func_eu_802B14A4() {}

extern "C" void func_eu_802B14BC() {}

extern "C" void* func_eu_802B14D4(void) { return 0; }

extern "C" void* func_eu_802B14E0(void) { return 0; }

extern "C" u32 lbl_eu_8053A478[];
extern "C" void* func_eu_802B14EC(void) { return (void*)lbl_eu_8053A478; }

extern "C" void func_eu_802B14F8() {}

extern "C" void getBdatStringColumnValue(void*, void*, int);
extern u32 lbl_eu_80664BF8;
extern u8 lbl_eu_80513420[];
extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

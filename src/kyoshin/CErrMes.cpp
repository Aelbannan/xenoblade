// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

void func_eu_802B1334() { CBdat::func_8003AA8C(6); }









void func_eu_802B13FC(){}

void func_eu_802B1414(){}

void func_eu_802B142C(){}

void func_eu_802B1444(){}

void func_eu_802B145C(){}

void func_eu_802B1474(){}

void func_eu_802B148C(){}

void func_eu_802B14A4(){}

void func_eu_802B14BC(){}

extern u32 lbl_eu_8053A438[];
extern u32 lbl_eu_8053A458[];
u32 lbl_eu_8053A478[];

u32* func_eu_802B14D4() { return lbl_eu_8053A438; }
u32* func_eu_802B14E0() { return lbl_eu_8053A458; }
extern "C" void* func_eu_802B14EC(void) { return (void*)lbl_eu_8053A478; }

void func_eu_802B14F8(){}

extern "C" u32 getBdatStringColumnValue(void*, const char*, int);
extern u32 lbl_eu_80664BF8;
extern u8 lbl_eu_80513420[];

// Tail-calls getBdatStringColumnValue with index=2 (cf. func_eu_802B133C uses index=1)
u32 func_eu_802B1354() {
    return getBdatStringColumnValue((void*)lbl_eu_80664BF8, (const char*)lbl_eu_80513420 + 9, 2);
}

extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

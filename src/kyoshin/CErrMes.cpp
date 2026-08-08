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

void* func_eu_802B14D4(void){ return 0; }

void* func_eu_802B14E0(void){ return 0; }

u32 lbl_eu_8053A478[];
extern "C" void* func_eu_802B14EC(void) { return (void*)lbl_eu_8053A478; }

void func_eu_802B14F8(){}

extern u32 lbl_eu_80664BF8;
extern u8 lbl_eu_80513420[];
extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

// Bdat column lookup import (retail signature from defining TU ocBdat.cpp;
// deliberately not promoted to ocBdat.hpp - see note there).
extern "C" u32 getBdatStringColumnValue(void* pData, const char* pColumnName, int index);

extern u32 lbl_eu_8053A438[];
extern u32 lbl_eu_8053A458[];
extern u32 lbl_eu_80664BF8;
extern u8 lbl_eu_80513420[];

void func_eu_802B1334() { CBdat::func_8003AA8C(6); }

// Error-screen bdat string getters: forward the column index to the shared
// bdat helper (bdat data from lbl_eu_80664BF8, column "???" at +9 of 0x80513420).
void func_eu_802B1354() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 2);
}

void func_eu_802B136C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 3);
}

void func_eu_802B1384() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 4);
}

void func_eu_802B139C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 5);
}

void func_eu_802B13B4() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 6);
}

void func_eu_802B13CC() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 7);
}

void func_eu_802B13E4() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 8);
}









void func_eu_802B13FC() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 9);
}

void func_eu_802B1414(){}

void func_eu_802B142C(){}

void func_eu_802B1444(){}

void func_eu_802B145C(){}

void func_eu_802B1474(){}

void func_eu_802B148C(){}

void func_eu_802B14A4(){}

void func_eu_802B14BC(){}

void* func_eu_802B14D4(void) { return (void*)lbl_eu_8053A438; }

void* func_eu_802B14E0(void) { return (void*)lbl_eu_8053A458; }

extern u32 lbl_eu_8053A478[];
extern "C" void* func_eu_802B14EC(void) { return (void*)lbl_eu_8053A478; }

void func_eu_802B14F8(){}

extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

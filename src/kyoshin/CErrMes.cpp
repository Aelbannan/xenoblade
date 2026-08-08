// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/device/CDeviceSC.hpp"

// Bdat column lookup import (retail signature from defining TU ocBdat.cpp;
// deliberately not promoted to ocBdat.hpp - see note there).
extern "C" u32 getBdatStringColumnValue(void* pData, const char* pColumnName, int index);

extern u32 lbl_eu_80664BF8;
extern u8 lbl_eu_80513420[];

// Loads the error-screen bdat table for the current (clamped) language, adds
// it to CBdat index 6, and caches the table pointer for the column getters.
void func_eu_802B12DC() {
    u8 lang = CDeviceSC::getLanguage();
    if (lang > 5) {
        lang = 1;
    }
    CBdat::func_8003AA78(6, (void*)lbl_eu_8053A420[lang]);
    lbl_eu_80664BF8 = (u32)CBdat::getFP((const char*)lbl_eu_80513420);
}

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

void func_eu_802B1414() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 10);
}

void func_eu_802B142C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 12);
}

void func_eu_802B1444() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 13);
}

void func_eu_802B145C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 14);
}

void func_eu_802B1474() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 15);
}

void func_eu_802B148C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 16);
}

void func_eu_802B14A4() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 19);
}

void func_eu_802B14BC() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 20);
}

void* func_eu_802B14D4(void) { return (void*)lbl_eu_8053A438; }

void* func_eu_802B14E0(void) { return (void*)lbl_eu_8053A458; }

extern "C" void* func_eu_802B14EC(void) { return (void*)lbl_eu_8053A478; }

void func_eu_802B14F8(){}

extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

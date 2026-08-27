// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/device/CDeviceSC.hpp"

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

void func_eu_802B1334() { CBdat::getEntry(6); }

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

extern "C" void* func_eu_802B14E0(void) { return (void*)lbl_eu_8053A458; }

extern "C" void* func_eu_802B14EC(void) { return (void*)lbl_eu_8053A478; }

// Language codes patched into the extended language-entry tables.
extern "C" void func_eu_804E2340(char* pStr);
extern "C" void func_804DDE3C(char* pStr, s32 flag);

void func_eu_802B14F8(s32 lang) {
    // The bdat-table array and the three language-entry arrays are contiguous
    // in .data; walk them all through one base pointer (retail codegen).
    ErrMesLangEntry** pEntries = (ErrMesLangEntry**)&lbl_eu_8053A420[6];

    // Reset the '/' separator byte on every entry of all three tables.
    int i;
    for (i = 0; i < 7; i++) {
        pEntries[i]->mSep = '/';
        pEntries[i + 7]->mSep = '/';
        pEntries[i + 14]->mSep = '/';
    }

    // Pick the two-char language code ('en' or 'jp') from the argument.
    char c0 = lang == 0 ? 'e' : 'j';
    char c1 = lang == 0 ? 'n' : 'p';

    bool changed = false;
    for (i = 1; i < 7; i++) {
        pEntries[i]->mLang0 = c0;
        pEntries[i]->mLang1 = c1;
        // Only the middle table reports whether anything actually changed.
        if (pEntries[i + 7]->mLang0 != c0 || pEntries[i + 7]->mLang1 != c1) {
            changed = true;
        }
        pEntries[i + 7]->mLang0 = c0;
        pEntries[i + 7]->mLang1 = c1;
        pEntries[i + 14]->mLang0 = c0;
        pEntries[i + 14]->mLang1 = c1;
    }

    if (changed) {
        char* pStr = (char*)lbl_eu_80513420;
        func_eu_804E2340(pStr + 0xe);
        func_eu_804E2340(pStr + 0x12);
        s32 flag = lang == 0 ? 1 : 0;
        func_804DDE3C(pStr + 0x16, flag);
        func_804DDE3C(pStr + 0x25, flag);
    }
}

extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/device/CDeviceSC.hpp"

void* sErrMesBdatData;  // name-guess: only CErrMes.cpp reads/writes this .sbss slot as the cached error-message bdat entry
extern u8 lbl_eu_80513420[];  // string pool kept: literals lose getErrMesText01 100%->81.67% (+4B) and loadErrMesTable reloc pairing

// Loads the error-screen bdat table for the current (clamped) language, adds
// it to CBdat index 6, and caches the table pointer for the column getters.
void loadErrMesTable() {
    u8 lang = CDeviceSC::getLanguage();
    if (lang > 5) {
        lang = 1;
    }
    CBdat::setBdatEntry(6, (void*)lbl_eu_8053A420[lang]);
    sErrMesBdatData = CBdat::getFP((const char*)lbl_eu_80513420);
}

extern "C" void touchErrMesEntry() { CBdat::getEntry(6); }

// Error-screen bdat string getters: forward the column index to the shared
// bdat helper (bdat data from sErrMesBdatData, column "???" at +9 of 0x80513420).
extern "C" void getErrMesText02() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 2);
}

extern "C" void getErrMesText03() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 3);
}

extern "C" void getErrMesText04() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 4);
}

extern "C" void getErrMesText05() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 5);
}

extern "C" void getErrMesText06() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 6);
}

extern "C" void getErrMesText07() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 7);
}

extern "C" void getErrMesText08() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 8);
}









extern "C" void getErrMesText09() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 9);
}

extern "C" void getErrMesText10() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 10);
}

extern "C" void getErrMesText12() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 12);
}

extern "C" void getErrMesText13() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 13);
}

extern "C" void getErrMesText14() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 14);
}

extern "C" void getErrMesText15() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 15);
}

extern "C" void getErrMesText16() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 16);
}

extern "C" void getErrMesText19() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 19);
}

extern "C" void getErrMesText20() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 20);
}

extern "C" void* getErrMesLangTable0(void) { return (void*)lbl_eu_8053A438; }

extern "C" void* getErrMesLangTable1(void) { return (void*)lbl_eu_8053A458; }

extern "C" void* getErrMesLangTable2(void) { return (void*)lbl_eu_8053A478; }

// Language codes patched into the extended language-entry tables.
extern "C" void removePackFile(char* pStr);
extern "C" void registerPackFile(char* pStr, s32 flag);

void setErrMesLanguage(s32 lang) {
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
        removePackFile("ahx/jp/ahx.pkh");
        removePackFile("jp/ahx.pkh");
        s32 flag = lang == 0 ? 1 : 0;
        registerPackFile("ahx.pkh", flag);
        registerPackFile("chr.pkh", flag);
    }
}

extern "C" void getErrMesText01() {
    getBdatStringColumnValue(sErrMesBdatData, (char*)&lbl_eu_80513420[9], 1);
}

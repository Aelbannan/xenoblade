// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/device/CDeviceSC.hpp"

void func_eu_802B1334() { CBdat::func_8003AA8C(6); }

// --- External data symbols ---
// lbl_eu_8053A420: 6 pointers indexed by language (0-5)
// lbl_eu_8053A438: 8 pointers, group 1 (indices 6-13)
// lbl_eu_8053A458: 8 pointers, group 2 (indices 14-21)
// lbl_eu_8053A478: 8 pointers, group 3 (indices 22-29)
// Together these form a contiguous 30-element pointer array.
extern CErrMesTextObj* lbl_eu_8053A420[6];
extern CErrMesTextObj* lbl_eu_8053A438[8];
extern CErrMesTextObj* lbl_eu_8053A458[8];
// lbl_eu_8053A478 already declared below as u32[]
extern const char lbl_eu_80513420[];
extern void* lbl_eu_80664BF8;

extern "C" void func_eu_804E2340(const char*);
extern "C" void func_804DDE3C(const char*, bool);

// Language-indexed lookup: loads the bdat pointer for the current language
// and stores the result of getFP into the global file-pointer variable.
void* func_eu_802B12DC() {
    u32 lang = CDeviceSC::getLanguage();
    if (lang > 5) {
        lang = 1;
    }
    CBdat::func_8003AA78(6, lbl_eu_8053A420[lang]);
    void* fp = getFP(lbl_eu_80513420);
    lbl_eu_80664BF8 = fp;
    return fp;
}









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

extern void* lbl_eu_8053A478[8];
extern "C" void* func_eu_802B14EC(void) { return lbl_eu_8053A478; }

// Sets language prefix bytes in the three text-object pointer groups.
// param == 0 sets "en", otherwise "jp".  If values in group 2 changed,
// triggers layout refresh callbacks.
void func_eu_802B14F8(int param) {
    // Loop 1: set byte 6 = '/' in all 7 entries of each group (skipping last of 8)
    for (int i = 0; i < 7; i++) {
        lbl_eu_8053A438[i]->field_0x06 = '/';
        lbl_eu_8053A458[i]->field_0x06 = '/';
        ((CErrMesTextObj*)lbl_eu_8053A478[i])->field_0x06 = '/';
    }

    // Choose language chars: en or jp
    u8 c1, c2;
    if (param == 0) {
        c1 = 'e'; c2 = 'n';
    } else {
        c1 = 'j'; c2 = 'p';
    }

    // Loop 2: set bytes 4,5 in entries 1-6 of each group (skip entry 0)
    // Track whether group 2 values changed compared to what was already there.
    bool changed = false;
    for (int i = 1; i < 7; i++) {
        CErrMesTextObj* p1 = lbl_eu_8053A438[i];
        p1->field_0x04 = c1;
        p1->field_0x05 = c2;

        CErrMesTextObj* p2 = lbl_eu_8053A458[i];
        if ((u8)p2->field_0x04 != c1 || (u8)p2->field_0x05 != c2) {
            changed = true;
        }
        p2->field_0x04 = c1;
        p2->field_0x05 = c2;

        CErrMesTextObj* p3 = (CErrMesTextObj*)lbl_eu_8053A478[i];
        p3->field_0x04 = c1;
        p3->field_0x05 = c2;
    }

    // If any values in group 2 actually changed, trigger layout updates
    if (changed) {
        func_eu_804E2340(&lbl_eu_80513420[14]);
        func_eu_804E2340(&lbl_eu_80513420[18]);
        func_804DDE3C(&lbl_eu_80513420[22], param == 0);
        func_804DDE3C(&lbl_eu_80513420[37], param == 0);
    }
}

extern "C" void getBdatStringColumnValue(void*, void*, int);
extern void* lbl_eu_80664BF8;
extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

// Error-message UI helper (EU region).

#include "kyoshin/CErrMes.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/device/CDeviceSC.hpp"

void func_eu_802B1334() { CBdat::func_8003AA8C(6); }

// --- External data symbols ---
// lbl_eu_8053A420 is the base of a contiguous 30-entry pointer array:
//   [0..5]  = lbl_eu_8053A420 (language-indexed, 6 entries)
//   [6..13] = lbl_eu_8053A438 (group 1, 8 entries)
//   [14..21]= lbl_eu_8053A458 (group 2, 8 entries)
//   [22..29]= lbl_eu_8053A478 (group 3, 8 entries)
extern u32 lbl_eu_8053A420[];
extern const char lbl_eu_80513420[];
extern void* lbl_eu_80664BF8;

extern "C" void func_eu_804E2340(const char*);
extern "C" void func_804DDE3C(const char*, bool);

// Language-indexed lookup: loads the bdat pointer for the current language
// and stores the result of getFP into the global file-pointer variable.
void* func_eu_802B12DC() {
    u8 lang = CDeviceSC::getLanguage();
    if (lang > 5) {
        lang = 1;
    }
    CBdat::func_8003AA78(6, (void*)lbl_eu_8053A420[lang]);
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

extern "C" void* func_eu_802B14EC(void) { return &lbl_eu_8053A420[22]; }

// Sets language prefix bytes in the three text-object pointer groups.
// param == 0 sets "en", otherwise "jp".  If values in group 2 changed,
// triggers layout refresh callbacks.
void func_eu_802B14F8(int param) {
    CErrMesTextObj** base = (CErrMesTextObj**)lbl_eu_8053A420;
    const char* strBase = lbl_eu_80513420;

    // Derive sub-group pointers from the single base
    CErrMesTextObj** grp1 = &base[6];   // 7 entries at indices 6..12
    CErrMesTextObj** grp2 = &base[14];  // 7 entries at indices 14..20
    CErrMesTextObj** grp3 = &base[22];  // 7 entries at indices 22..28

    // Loop 1: set byte 6 = '/' in all 7 entries of each group
    // Use goto-based loop to prevent MWCC -O4,p unrolling
    {
        int i = 0;
    loop1:
        grp1[i]->field_0x06 = '/';
        grp2[i]->field_0x06 = '/';
        grp3[i]->field_0x06 = '/';
        i++;
        if (i < 7) goto loop1;
    }

    // Choose language chars: en or jp
    u8 c1, c2;
    if (param == 0) {
        c1 = 'e'; c2 = 'n';
    } else {
        c1 = 'j'; c2 = 'p';
    }

    // Loop 2: set bytes 4,5 in entries 1-6 of each group (skip first)
    {
        int changed = 0;
        int i = 1;
    loop2:
        {
            CErrMesTextObj* p1 = grp1[i];
            p1->field_0x04 = c1;
            p1->field_0x05 = c2;

            CErrMesTextObj* p2 = grp2[i];
            if ((s8)p2->field_0x04 != (s8)c1 || (s8)p2->field_0x05 != (s8)c2) {
                changed = 1;
            }
            p2->field_0x04 = c1;
            p2->field_0x05 = c2;

            CErrMesTextObj* p3 = grp3[i];
            p3->field_0x04 = c1;
            p3->field_0x05 = c2;
        }
        i++;
        if (i < 7) goto loop2;

        // If any values in group 2 changed, trigger layout updates
        if (changed) {
            func_eu_804E2340(strBase + 14);
            func_eu_804E2340(strBase + 18);
            func_804DDE3C(strBase + 22, param == 0);
            func_804DDE3C(strBase + 37, param == 0);
        }
    }
}

extern "C" void getBdatStringColumnValue(void*, void*, int);
extern void* lbl_eu_80664BF8;
extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

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
    // Use lbl_eu_8053A420 as base; groups start at indices 6, 14, 22
    u32* base = lbl_eu_8053A420;
    u8* pBase = (u8*)base;
    u8* pGrp1 = pBase + 0x18;  // &base[6]
    u8* pGrp2 = pBase + 0x38;  // &base[14]
    u8* pGrp3 = pBase + 0x58;  // &base[22]

    // Loop 1: set byte 6 = '/' in all 7 entries of each group
    {
        volatile int off = 0;
        volatile int n = 7;
        do {
            ((CErrMesTextObj*)*(u32*)(pGrp1 + off))->field_0x06 = '/';
            ((CErrMesTextObj*)*(u32*)(pGrp2 + off))->field_0x06 = '/';
            ((CErrMesTextObj*)*(u32*)(pGrp3 + off))->field_0x06 = '/';
            off += 4;
        } while (--n > 0);
    }

    // Choose language chars: en or jp
    u8 c1, c2;
    if (param == 0) {
        c1 = 'e'; c2 = 'n';
    } else {
        c1 = 'j'; c2 = 'p';
    }

    // Loop 2: set bytes 4,5 in entries 1-6 of each group (skip entry 0)
    {
        volatile int off = 4;
        int changed = 0;
        volatile int n = 6;
        do {
            CErrMesTextObj* p1 = (CErrMesTextObj*)*(u32*)(pGrp1 + off);
            p1->field_0x04 = c1;
            p1->field_0x05 = c2;

            CErrMesTextObj* p2 = (CErrMesTextObj*)*(u32*)(pGrp2 + off);
            if ((s8)p2->field_0x04 != (s8)c1 || (s8)p2->field_0x05 != (s8)c2) {
                changed = 1;
            }
            p2->field_0x04 = c1;
            p2->field_0x05 = c2;

            CErrMesTextObj* p3 = (CErrMesTextObj*)*(u32*)(pGrp3 + off);
            p3->field_0x04 = c1;
            p3->field_0x05 = c2;

            off += 4;
        } while (--n > 0);

        // If any values in group 2 actually changed, trigger layout updates
        if (changed) {
            func_eu_804E2340(&lbl_eu_80513420[14]);
            func_eu_804E2340(&lbl_eu_80513420[18]);
            func_804DDE3C(&lbl_eu_80513420[22], param == 0);
            func_804DDE3C(&lbl_eu_80513420[37], param == 0);
        }
    }
}

extern "C" void getBdatStringColumnValue(void*, void*, int);
extern void* lbl_eu_80664BF8;
extern "C" void func_eu_802B133C() {
    getBdatStringColumnValue((void*)lbl_eu_80664BF8, (char*)lbl_eu_80513420 + 9, 1);
}

// Auto-scaffolded catalog TU for kyoshin/cf/CfTFile
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfTFile.hpp"

void __ct__cf_CfTFile(){}

void func_800699B0(){}

void func_80069A18(){}

void func_80069A78(){}

void func_80069ACC(){}

void func_80069C14(){}

void func_80069C28(){}

void func_80069C78(){}

void CfTFile::OnFileEvent() {}

cf::CfTFile* __dt__Q22cf7CfTFileFv(cf::CfTFile* obj, int flag) {
    if (obj != nullptr && flag > 0) {
        operator delete(obj);
    }
    return obj;
}

extern float lbl_eu_80661AF0;
float func_80069EA0() { return lbl_eu_80661AF0; }

void func_80069EA8(int param) {
    extern unsigned char lbl_eu_80663DA0;
    extern float lbl_eu_80666214;
    extern float lbl_eu_80666218;
    unsigned char val = lbl_eu_80663DA0;
    if (param != 0) {
        val |= 0x02;
        lbl_eu_80663DA0 = val;
        lbl_eu_80661AF0 = lbl_eu_80666214;
    } else {
        val &= 0x02;
        lbl_eu_80663DA0 = val;
        lbl_eu_80661AF0 = lbl_eu_80666218;
    }
}

void func_80069EE0(void){}

void func_80069EE4(){}

void func_80069F2C(){}

void func_8006A028(){}

void func_8006A03C(){}

void func_8006A12C(){}

void func_8006A1A0(){}

void func_8006A234(){}

void func_8006A2E0(){}

void func_8006A33C(){}

void func_8006A37C(){}

void func_8006A3BC(){}

extern u16 lbl_eu_80661AF8;
u16 func_8006A3FC(u16 val) { lbl_eu_80661AF8 = val; return val; }

extern u16 lbl_eu_80661AF8;
u16 func_8006A404() { return lbl_eu_80661AF8; }

void func_8006A40C(){}

void func_8006A53C(){}

void func_8006A6D0(){}

void func_8006A75C(){}

// Read-only getter for the file event frame timer.
u32 func_8006A80C() {
    return lbl_eu_80663DA8;
}

void func_8006A814(u32* self) {
    extern u32 lbl_eu_80663DA8;
    lbl_eu_80663DA8 = *self;
}

void func_eu_8006B238(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8006A820() {
    extern unsigned long lbl_eu_80663DA8;
    lbl_eu_80663DA8 = 0;
}

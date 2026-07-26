// Auto-scaffolded catalog TU for kyoshin/cf/CfTFile
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfTFile.hpp"

extern "C" void __ct__cf_CfTFile() {}

extern "C" void func_800699B0() {}

extern "C" void func_80069A18() {}

extern "C" void func_80069A78() {}

extern "C" void func_80069ACC() {}

extern "C" void func_80069C14() {}

extern "C" void func_80069C28() {}

extern "C" void func_80069C78() {}

extern "C" void OnFileEvent__7CfTFileFP10CEventFile() {}

extern "C" cf::CfTFile* __dt__Q22cf7CfTFileFv(cf::CfTFile* obj, int flag) {
    if (obj != nullptr && flag > 0) {
        operator delete(obj);
    }
    return obj;
}

extern float lbl_eu_80661AF0;
extern "C" float func_80069EA0() { return lbl_eu_80661AF0; }

extern "C" void func_80069EA8(int param) {
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

extern "C" void func_80069EE0(void) {}

extern "C" void func_80069EE4() {}

extern "C" void func_80069F2C() {}

extern "C" void func_8006A028() {}

extern "C" void func_8006A03C() {}

extern "C" void func_8006A12C() {}

extern "C" void func_8006A1A0() {}

extern "C" void func_8006A234() {}

extern "C" void func_8006A2E0() {}

extern "C" void func_8006A33C() {}

extern "C" void func_8006A37C() {}

extern "C" void func_8006A3BC() {}

extern u16 lbl_eu_80661AF8;
extern "C" u16 func_8006A3FC(u16 val) { lbl_eu_80661AF8 = val; return val; }

extern u16 lbl_eu_80661AF8;
extern "C" u16 func_8006A404() { return lbl_eu_80661AF8; }

extern "C" void func_8006A40C() {}

extern "C" void func_8006A53C() {}

extern "C" void func_8006A6D0() {}

extern "C" void func_8006A75C() {}

// Read-only getter for the file event frame timer.
extern "C" u32 func_8006A80C() {
    return lbl_eu_80663DA8;
}

extern "C" void func_8006A814(void* self) {
    extern unsigned long lbl_eu_80663DA8;
    lbl_eu_80663DA8 = *(unsigned long*)self;
}

extern "C" void func_eu_8006B238() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8006A820() {
    extern unsigned long lbl_eu_80663DA8;
    lbl_eu_80663DA8 = 0;
}

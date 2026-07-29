// Translation unit: monolib/src/coli/CColiProc
// Contains CColiProc constructor and field-management helpers.

#include <types.h>

struct CColiProcTarget {
    u32 field_0x0;
    u32 field_0x4;
    u16 field_0x6;
};

struct CColiProc {
    CColiProcTarget* field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xC;
};

// Default target data referenced by the constructor.
extern "C" u8 lbl_eu_8056F398[0x10];

extern "C" void __ct__CColiProc(CColiProc* this) {
    this->field_0x0 = (CColiProcTarget*)lbl_eu_8056F398;
    this->field_0x4 = 0;
}

extern "C" void func_804B2590(CColiProc* this, CColiProcTarget* r4, u32 r5) {
    this->field_0x0 = r4;
    this->field_0x4 = r5;
    this->field_0xC = 0;
}

extern "C" void func_804B25A4(CColiProc* this, CColiProcTarget* r4, u32 r5, u32 r6) {
    this->field_0x0 = r4;
    this->field_0x4 = r5;
    this->field_0x8 = r6;
    this->field_0xC = 0;
}

// Returns true if field_0x0 is non-null and bit 0 of its u16 at offset 6 is set.
extern "C" bool func_804B2F80(CColiProc* this) {
    if (this->field_0x0 == nullptr) {
        return false;
    }
    return (this->field_0x0->field_0x6 & 1) != 0;
}

// Returns bit 18 of field_0xC (extrwi extraction).
extern "C" bool func_804B2FA8(CColiProc* this) {
    return ((this->field_0xC >> 18) & 1) != 0;
}

// --- remaining stubs (not in this session's target list) ---
extern "C" void func_804B25BC(){}
extern "C" void func_804B27EC(){}
extern "C" void func_804B29EC(){}
extern "C" void func_804B2AA4(){}
extern "C" void func_804B2CBC(){}
extern "C" void func_804B2E3C(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_804B2FB4(){}

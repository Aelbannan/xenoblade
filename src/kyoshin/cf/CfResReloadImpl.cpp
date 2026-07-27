// Auto-scaffolded catalog TU for kyoshin/cf/CfResReloadImpl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__cf_CfResReloadImpl(){}

int func_8016CE3C(void* arg) {
    unsigned int v = *(unsigned short*)((char*)arg + 8);
    return (int)(((v | 0xFFFFFFFCu) - ((v - 3u) >> 1)) >> 31);
}

void func_8016CE5C(){}

// FULL_MATCH: vtable slot 6 - returns constant resource type identifier
int func_8016CF1C() { return 12; }

void func_8016CF24(){}

void func_8016CFBC(){}

void func_8016CFDC(){}

void func_8016D0C0(){}

void func_8016D144(){}

void func_8016D1D8(){}

void func_8016D240(){}

void func_8016D2FC(){}

void func_8016D390(){}

void func_8016D3F8(){}

void func_8016D688(){}

void func_8016DAF8(){}

void func_8016DCE4(){}

void func_8016DDE8(){}

void func_eu_8016F1C4(){}

void func_8016DE68(){}

void func_8016DE8C(){}

// FULL_MATCH: vtable slot 23 - returns constant resource sub-type
int func_8016DECC() { return 1; }

void func_8016DED4(void* thisPtr, int index, unsigned int value) {
    if (index < 2) {
        ((unsigned int*)((char*)thisPtr + 0x14))[index] = value;
    }
}

void* __dt__Q22cf15CfResReloadImplFv(void* this_, int deletingFlag) { if (this_ && deletingFlag > 0) ::operator delete(this_); return this_; }

extern u16 lbl_eu_80664278;
u16 func_8016DF2C() { return lbl_eu_80664278; }

float lbl_eu_806676B0;
float lbl_eu_80666210;
float lbl_eu_80664284;

void func_8016DF34(short param_1)
{
    lbl_eu_80664278 = param_1;
    lbl_eu_80664284 = lbl_eu_806676B0 * lbl_eu_80666210;
}

void func_8016DF4C(){}

extern u16 lbl_eu_80664280;
u16 func_8016E08C() { return lbl_eu_80664280; }

extern u16 lbl_eu_8066427E;
u16 func_8016E094() { return lbl_eu_8066427E; }

unsigned short lbl_eu_8066427A;
unsigned short lbl_eu_8066427C;
void* memset(void*, int, unsigned long);

void func_8016E09C(void* self) {
    unsigned char* p = static_cast<unsigned char*>(self);
    memset(self, 0, 0x10);
    *reinterpret_cast<float*>(p + 0x0) = lbl_eu_80664284;
    *reinterpret_cast<unsigned short*>(p + 0x4) = lbl_eu_80664278;
    *reinterpret_cast<unsigned short*>(p + 0x6) = lbl_eu_8066427E;
    *reinterpret_cast<unsigned short*>(p + 0xC) = lbl_eu_80664280;
    *reinterpret_cast<unsigned short*>(p + 0x8) = lbl_eu_8066427A;
    *reinterpret_cast<unsigned short*>(p + 0xA) = lbl_eu_8066427C;
}

void func_8016E100(){}

void func_8016E13C(){}

void func_8016E164(u16 r3, u16 r4) {
    extern u16 lbl_eu_8066427A;
    extern u16 lbl_eu_8066427C;
    extern u16 lbl_eu_80664288;
    extern float lbl_eu_806676C8;
    if (lbl_eu_8066427A == r3 && lbl_eu_8066427C == r4) {
        lbl_eu_80664288 &= 0xFFFE;
    } else {
        lbl_eu_80664288 |= 0x1;
    }
    lbl_eu_80664284 = lbl_eu_806676C8;
    lbl_eu_8066427A = r3;
    lbl_eu_8066427C = r4;
}

void func_8016E1AC(){}

void func_8016E430(){}

void func_8016E578(){}

void func_8016E654(){}

void func_8016E854(){}

void func_8016E9CC(){}

void func_8016EA68(){}

void func_8016EC58(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8016ED44() {
    extern unsigned short lbl_eu_80664288;
    lbl_eu_80664288 = 0;
}

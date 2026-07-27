// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmick
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CfGimmick.hpp"

namespace cf {
    void CfGimmick::func_8020896C(void* other) {
        void* cur = *(void**)((char*)this + 0x78);
        if (cur != other) return;
        *(void**)((char*)cur + 0xB0) = 0;
        *(void**)((char*)this + 0x78) = 0;
    }

    void CfGimmick::func_80208988() {
        extern void func_802089BC(void*, void*, void*);
        func_802089BC((char*)this + 0x1c, (char*)this + 4, (char*)this + 0x10);
    }

    int CfGimmick::func_8020A8AC() { return 1; }
}

void __ct__cf_CfGimmick(){}

void cf::CfGimmick::~CfGimmick() {}

void func_8020899C(){}

void func_802089BC(){}

void func_80208C48(){}

void func_80208C60(){}

void func_80208C78(){}

void func_80208CC0(){}

void func_80208E98(){}

bool func_80208EDC() { return false; }

void func_80208EE4(){}

void func_80208F34(){}

void func_80209020(){}

void func_8020915C(){}

void func_80209288(){}

void func_8020938C(){}

void func_80209488(){}

void func_802095D8(){}

void func_802096EC(){}

void func_8020971C(){}

void func_8009D018(void* self);
void func_8020974C(void* self) { ((void(*)(void*))func_8009D018)((char*)self + 0x2cc8); }

void func_80209754(){}

void func_802098EC(){}

void func_80209F2C(){}

void func_80209F5C() {
    extern void* getUnk80664658();
    void* p = getUnk80664658();
    *(volatile unsigned int*)((unsigned char*)p + 0x214) |= 0x000C0002;
}

void* getUnk80664658();
void func_80209F8C() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x8008;
}

void func_80209FB8(){}

void func_80209FE4(){}

void func_8020A010(){}

void func_8020A03C(){}

void func_8020A068(){}

void func_8020A0CC(){}

void func_8020A0F8(){}

void func_8020A124(){}

void func_8020A1DC(){}

void func_8020A294(){}

void func_8020A35C(){}

void func_8020A434(){}

void func_8020A484(){}

void func_8020A5DC(){}

void func_8020A608(){}

void func_8020A6B0(){}

void func_8020A87C(){}

void func_8020A8B4(){}

void func_8020A928(){}

void func_8020A9F4(){}

void func_8020AA8C(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8020AB7C() {}

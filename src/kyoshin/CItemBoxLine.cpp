// Auto-scaffolded catalog TU for kyoshin/CItemBoxLine
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_801ED800(void* self) { return ((u8*)self)[0x59]; }










extern "C" void func_801EC438() {}

extern "C" void func_801EC808() {}

extern "C" void func_801EC8B4() {}

extern "C" void func_801EC8D8() {}

extern "C" void func_801EC9E0() {}

extern "C" void __ct__CIBLTabCur() {}

extern "C" void __dt__801ECBC0() {}

extern "C" void func_801ECC10() {}

extern "C" void __ct__CItemBoxLine() {}

extern "C" void __dt__12CItemBoxLineFv() {}

extern "C" void func_801ED31C() {}

extern "C" void func_801ED3E8() {}

extern "C" void func_801ED4FC() {}

extern "C" void func_801ED618() {}

extern "C" void func_801ED774() {}


extern "C" void func_801ED808() {}

extern "C" void func_801ED864() {}

extern "C" void func_801ED97C() {}

extern "C" void func_801EDA08() {}

extern "C" void func_801EDA4C(unsigned char* self, unsigned char val) {
    unsigned char n = self[0x63];
    if (n >= 9) {
        return;
    }
    self[0x5a + n] = val;
    self[0x63] = n + 1;
}

extern "C" void func_801EDA6C() {}

extern "C" void func_801EDB80() {}

extern "C" void func_801EDC94() {}

extern "C" void func_801EDF40() {}

extern "C" void func_801EE228() {}

extern "C" void func_801EE448() {}

extern "C" void func_801EE684() {}

extern "C" void func_801EE788() {}

extern "C" u8 func_801EECC0(void* self) { return ((u8*)self)[0x39E]; }

extern "C" void func_801EC3B0(void*, unsigned int);

extern "C" void func_801EECC8(char* self) {
    func_801EC3B0(self + 0x3A4, (unsigned char)(*(short*)(self + 0x38C) + *(short*)(self + 0x38E)));
}

extern "C" void func_801D2E4C(void* self);
extern "C" void func_801EECE0(void* self) { ((void(*)(void*))func_801D2E4C)((char*)self + 0xb8); }

extern "C" void func_801EECE8() {}

extern "C" void func_801EED6C() {}

extern "C" void func_801EEDF8() {}

extern "C" void func_801EF034() {}

extern "C" void func_801EF050() {}

extern "C" void func_801EF0EC() {}

extern "C" void func_801EF1E4() {}

extern "C" void func_801EF260() {}

extern "C" void func_801EF2FC() {}

extern "C" void func_801EF378() {}

extern "C" void func_801EF3E8() {}

extern "C" void func_801EF45C() {}

extern "C" void func_801EF518() {}

extern "C" void func_801EF734() {}

extern "C" void func_801EF844() {}

extern "C" void func_801EF954() {}

extern "C" void func_801EFB24() {}

extern "C" void func_801EFDF4() {}

extern "C" void func_801EFE6C() {}

extern "C" void func_801EFFC4() {}

extern "C" void func_801F0030() {}

extern "C" void func_801F0488() {}

extern "C" void func_801F061C() {}

extern "C" void func_801F071C() {}

extern "C" void func_801F08B4() {}

extern "C" void func_801F0A58() {}

extern "C" void func_801F107C() {}

extern "C" void func_801F183C() {}

extern "C" void func_801F1E64() {}

extern "C" void func_801F20F0() {}

extern "C" void func_801F2298() {}

extern "C" void func_801F2434() {}

extern "C" void func_801F2880() {}

extern "C" void OnFileEvent__12CItemBoxLineFP10CEventFile() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_801F32EC() {}

extern "C" void func_801EBB88(void* self, u16 r4, u32 r5, u8 r6, u8 r7) {
    *(u16*)self = r4;
    *(u32*)((u8*)self + 4) = r5;
    *((u8*)self + 8) = r6;
    *((u8*)self + 9) = r7;
}

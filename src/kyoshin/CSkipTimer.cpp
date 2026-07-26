// Auto-scaffolded catalog TU for kyoshin/CSkipTimer
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSkipTimer.hpp"

extern "C" void __ct__CSkipTimer2() {}

extern "C" u8 func_8029FF1C(CSkipTimer* self) { return self->mActive; }










extern "C" void func_8029F6EC() {}

extern "C" void func_8029F73C() {}

extern "C" void func_8029F788(void* self) {
    if (*(u32*)((u8*)self + 0x14) != 0) {
        *(u8*)((u8*)self + 0x22) = 1;
        *(u8*)((u8*)self + 0x20) = 1;
    }
}

extern "C" void func_8029F7A4() {}

extern "C" void func_8029F82C() {}

extern "C" void __ct__CSkipTimer() {}

extern "C" void __dt__10CSkipTimerFv() {}

extern "C" void func_8029FBE0() {}

extern "C" void func_8029FCDC() {}

extern "C" void func_8029FDBC() {}

extern "C" void func_8029FE30() {}

extern "C" void func_8029FEBC() {}

extern "C" void func_8029FF00() {}


extern "C" void func_8029FF24() {}

extern "C" void func_8029FF98() {}

extern "C" void func_802A0008(void* obj) {
    unsigned char* bytes = (unsigned char*)obj;
    if (bytes[0x29] != 0) return;
    bytes[0x29] = 1;
    bytes[0x2b] = 0;
}

extern "C" void func_802A0028() {}

extern "C" void func_802A005C() {}

extern "C" void func_802A0148() {}

extern "C" void func_802A01F0() {}

extern "C" void func_802A0234() {}

extern "C" void func_802A0298() {}

extern "C" void func_802A02B8() {}

extern "C" void func_802A02D4() {}

extern "C" void func_802A0320() {}

extern "C" void func_802A0368() {}

extern "C" void func_802A03AC() {}

extern "C" void func_802A041C() {}

extern "C" void func_802A04F0() {}

extern "C" void func_802A055C() {}

extern "C" void func_802A05E4() {}

extern "C" void OnFileEvent__10CSkipTimerFP10CEventFile() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_802A07D8() {}

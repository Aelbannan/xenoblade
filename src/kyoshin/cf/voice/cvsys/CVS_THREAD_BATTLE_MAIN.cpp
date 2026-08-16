// Auto-scaffolded catalog TU for kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_MAIN
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

void func_802AED0C(){}

void func_802AEDB8(){}

void func_802AEF80(){}

void func_802AF02C(){}

void func_802AF13C(){}

void func_802AF2DC(){}

// Bit-2 gate on +0x3F00, then scan the 60-stride list at lbl_eu_8053A4B8 for
// an entry matching the +0x3F28 id; returns whether a match was found.
extern "C" int func_802AF388(void* self) {
    extern void* lbl_eu_8053A4B8[];
    if ((*(u32*)((u8*)self + 0x3F00) & 4) == 0)
        return 0;
    void** p = (void**)lbl_eu_8053A4B8;
    void* entry = 0;
    while (true) {
        entry = *p;
        if (entry == 0)
            break;
        if ((s32)entry == (s32)(u16)*(u16*)((u8*)self + 0x3F28))
            break;
        p = (void**)((u8*)p + 60);
    }
    return entry != 0;
}

void func_802AF3DC(){}

void func_802AF43C(){}

void func_802AF49C(){}

void func_802AF4FC(){}

void func_802AF56C(){}

void __ct__802AF5CC(){}

void func_802AF724(){}

void func_802AF844(){}

// When the CVS thread is not blocked, dispatch the vtable+8 slot (the
// battle-main update virtual) through the +0x1C vtable pointer.
extern "C" int func_802A3E88(void*);
extern "C" void func_802AF980(void* self) {
    if (func_802A3E88(self) != 0)
        return;
    ((CVS_THREAD*)self)->func_802A3B50();
}

int func_802AF9C8() { return 270; }

void func_802AF9D0(){}

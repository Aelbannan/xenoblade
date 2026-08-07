// Catalog TU for kyoshin/CFade
// FULL_MATCH: func_80244508 (returns mReady), func_80244510 (returns mVisible).
// Remaining stubs are NOT_STARTED.

#include "kyoshin/CFade.hpp"

u8 CFade::func_80244508() { return mReady; }


u8 CFade::func_80244510() { return mVisible; }


extern "C" void func_802445F0(void* self) {
    if (*(u32*)((u8*)self + 0x1C) != 0) {
        *(u8*)((u8*)self + 0x26) = 1;
        *(u8*)((u8*)self + 0x24) = 1;
    }
}

void CFade::OnFileEvent() const {}

extern "C" void func_8024439C() {}
extern "C" void func_802443E8() {}
extern "C" void func_80244460() {}
extern "C" void func_8024448C() {}
extern "C" void func_80244518() {}
extern "C" void func_80244538() {}
extern "C" void func_80244558() {}
extern "C" void func_802445A4() {}

// Catalog TU for kyoshin/CFade
// FULL_MATCH: func_80244508 (returns mReady), func_80244510 (returns mVisible).
// Remaining stubs are NOT_STARTED.

#include "kyoshin/CFade.hpp"

extern "C" u8 func_80244508(CFade* self) { return self->mReady; }







extern "C" u8 func_80244510(CFade* self) { return self->mVisible; }





extern "C" void func_802445F0(void* self) {
    if (*(u32*)((u8*)self + 0x1C) != 0) {
        *(u8*)((u8*)self + 0x26) = 1;
        *(u8*)((u8*)self + 0x24) = 1;
    }
}

void CFade::OnFileEvent() const {}

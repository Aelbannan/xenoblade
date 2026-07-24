// Catalog TU for kyoshin/CFade
// FULL_MATCH: func_80244508 (returns mReady), func_80244510 (returns mVisible).
// Remaining stubs are NOT_STARTED.

#include "kyoshin/CFade.hpp"

// LLM-HARNESS-BEGIN: us-802466c0
extern "C" u8 func_80244508(CFade* self) { return self->mReady; }
// LLM-HARNESS-END: us-802466c0







// LLM-HARNESS-BEGIN: us-802466c8
extern "C" u8 func_80244510(CFade* self) { return self->mVisible; }
// LLM-HARNESS-END: us-802466c8





// LLM-HARNESS-BEGIN: us-802467a8
extern "C" void func_802445F0(void* self) {
    if (*(u32*)((u8*)self + 0x1C) != 0) {
        *(u8*)((u8*)self + 0x26) = 1;
        *(u8*)((u8*)self + 0x24) = 1;
    }
}
// LLM-HARNESS-END: us-802467a8

// LLM-HARNESS-BEGIN: us-802467c4
extern "C" void OnFileEvent__5CFadeFP10CEventFile() {}
// LLM-HARNESS-END: us-802467c4

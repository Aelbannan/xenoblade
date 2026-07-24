// CVS_THREAD_FAINT: Voice thread for the "faint" status effect.
// FULL_MATCH: func_802A6DEC -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT.hpp"
#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802a91dc
extern "C" void __ct__802A6AA8() {}
// LLM-HARNESS-END: us-802a91dc

// LLM-HARNESS-BEGIN: us-802a92e4
extern "C" void func_802A6BB0() {}
// LLM-HARNESS-END: us-802a92e4

// LLM-HARNESS-BEGIN: us-802a93a0
extern "C" void func_802A6C6C() {}
// LLM-HARNESS-END: us-802a93a0

// LLM-HARNESS-BEGIN: us-802a9460
extern "C" void func_802A6D2C() {}
// LLM-HARNESS-END: us-802a9460

// LLM-HARNESS-BEGIN: us-802a94a8
extern "C" void func_802A6D74() {}
// LLM-HARNESS-END: us-802a94a8

// LLM-HARNESS-BEGIN: us-802a9520
// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; FAINT subclass returns 0xF0 (240).
extern "C" int func_802A6DEC(CVS_THREAD_FAINT* self) {
    return CVS_THREAD_FAINT::BUFFER_SIZE;
}
// LLM-HARNESS-END: us-802a9520

// LLM-HARNESS-BEGIN: us-802a9528
extern "C" void func_802A6DF4() {}
// LLM-HARNESS-END: us-802a9528

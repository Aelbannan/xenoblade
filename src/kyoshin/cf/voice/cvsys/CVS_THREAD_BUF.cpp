// CVS_THREAD_BUF: Voice thread buffer management.
// FULL_MATCH: func_802A5828 -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BUF.hpp"
#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802a757c
extern "C" void __ct__802A4E48() {}
// LLM-HARNESS-END: us-802a757c

// LLM-HARNESS-BEGIN: us-802a76d8
extern "C" void func_802A4FA4() {}
// LLM-HARNESS-END: us-802a76d8

// LLM-HARNESS-BEGIN: us-802a7794
extern "C" void func_802A5060() {}
// LLM-HARNESS-END: us-802a7794

// LLM-HARNESS-BEGIN: us-802a7814
extern "C" void func_802A50E0() {}
// LLM-HARNESS-END: us-802a7814

// LLM-HARNESS-BEGIN: us-802a78a8
extern "C" void func_802A5174() {}
// LLM-HARNESS-END: us-802a78a8

// LLM-HARNESS-BEGIN: us-802a7f5c
// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; BUF subclass returns 0x104 (260).
extern "C" int func_802A5828(CVS_THREAD_BUF* self) {
    return CVS_THREAD_BUF::BUFFER_SIZE;
}
// LLM-HARNESS-END: us-802a7f5c

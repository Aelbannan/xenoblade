// CVS_THREAD_HAGE: Voice thread variant for "Hage" (bald/bare) audio.
// FULL_MATCH: func_802A724C -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_HAGE.hpp"
#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802a95b8
extern "C" void __ct__802A6E84() {}
// LLM-HARNESS-END: us-802a95b8

// LLM-HARNESS-BEGIN: us-802a96c0
extern "C" void func_802A6F8C() {}
// LLM-HARNESS-END: us-802a96c0

// LLM-HARNESS-BEGIN: us-802a97fc
extern "C" void func_802A70C8() {}
// LLM-HARNESS-END: us-802a97fc

// LLM-HARNESS-BEGIN: us-802a98c0
extern "C" void func_802A718C() {}
// LLM-HARNESS-END: us-802a98c0

// LLM-HARNESS-BEGIN: us-802a9908
extern "C" void func_802A71D4() {}
// LLM-HARNESS-END: us-802a9908

// LLM-HARNESS-BEGIN: us-802a9980
// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; HAGE subclass returns 0xF0 (240).
extern "C" int func_802A724C(CVS_THREAD_HAGE* self) {
    return CVS_THREAD_HAGE::BUFFER_SIZE;
}
// LLM-HARNESS-END: us-802a9980

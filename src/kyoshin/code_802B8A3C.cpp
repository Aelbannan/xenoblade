// code_802B8A3C: CVS_THREAD_ORDER methods and voice-thread stubs.
// FULL_MATCH: func_802B8D44 -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_ORDER.hpp"
#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CVS_THREAD_ORDER() {}

extern "C" void func_802B8B0C() {}

extern "C" void func_802B8C00() {}

extern "C" void func_802B8CFC() {}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; ORDER subclass returns 0x78 (120).
extern "C" int func_802B8D44(CVS_THREAD_ORDER* self) {
    return CVS_THREAD_ORDER::BUFFER_SIZE;
}

extern "C" void func_802B8D4C() {}

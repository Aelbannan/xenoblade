// Auto-scaffolded catalog TU for kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.hpp"
#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__802A86CC() {}

extern "C" void func_802A87D4() {}

extern "C" void func_802A8904() {}

extern "C" void func_802A8A00() {}

extern "C" void func_802A8A48() {}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; REVIVE subclass returns 0xE6 (230).
extern "C" int func_802A8AC0(CVS_THREAD_REVIVE* self) {
    return CVS_THREAD_REVIVE::BUFFER_SIZE;
}

extern "C" void func_802A8AC8() {}

extern "C" void func_802A8B6C() {}

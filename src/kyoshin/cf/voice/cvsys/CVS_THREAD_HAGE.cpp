// CVS_THREAD_HAGE: Voice thread variant for "Hage" (bald/bare) audio.
// FULL_MATCH: func_802A724C -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_HAGE.hpp"
#include "kyoshin/harness_catalog.hpp"

void __ct__802A6E84(){}

void func_802A6F8C(){}

void func_802A70C8(){}

void func_802A718C(){}

void func_802A71D4(){}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; HAGE subclass returns 0xF0 (240).
extern "C" int func_802A724C(CVS_THREAD_HAGE* self) {
    return CVS_THREAD_HAGE::BUFFER_SIZE;
}

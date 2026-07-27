// CVS_THREAD_FAINT: Voice thread for the "faint" status effect.
// FULL_MATCH: func_802A6DEC -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT.hpp"
#include "kyoshin/harness_catalog.hpp"

void __ct__802A6AA8(){}

void func_802A6BB0(){}

void func_802A6C6C(){}

void func_802A6D2C(){}

void func_802A6D74(){}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; FAINT subclass returns 0xF0 (240).
extern "C" int func_802A6DEC(CVS_THREAD_FAINT* self) {
    return CVS_THREAD_FAINT::BUFFER_SIZE;
}

void func_802A6DF4(){}

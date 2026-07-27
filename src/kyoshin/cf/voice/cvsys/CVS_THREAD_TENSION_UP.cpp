// CVS_THREAD_TENSION_UP: Voice thread for tension-up audio events.
// FULL_MATCH: func_802A92D0 -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.hpp"
#include "kyoshin/harness_catalog.hpp"

void __ct__802A8DE8(){}

void func_802A8EEC(){}

void func_802A9030(){}

void func_802A9230(){}

void func_802A9278(){}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; TENSION_UP subclass returns 0x82 (130).
extern "C" int func_802A92D0(CVS_THREAD_TENSION_UP* self) {
    return CVS_THREAD_TENSION_UP::BUFFER_SIZE;
}

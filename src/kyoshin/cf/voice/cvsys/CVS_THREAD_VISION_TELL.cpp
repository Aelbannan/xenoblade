// Auto-scaffolded catalog TU for kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_TELL
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_TELL.hpp"
#include "kyoshin/harness_catalog.hpp"

void __ct__802A96C0(){}

void func_802A97A0(){}

void func_802A9924(){}

void func_802A9AC4(){}

void func_802A9B0C(){}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; VISION_TELL subclass returns 0x32 (50).
extern "C" int func_802A9B84(CVS_THREAD_VISION_TELL* self) {
    return CVS_THREAD_VISION_TELL::BUFFER_SIZE;
}

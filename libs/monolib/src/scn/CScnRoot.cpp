// Translation unit: monolib/src/scn/CScnRoot
// Contains CScnRoot class and related helpers.

#include <types.h>

// --- CScnRoot class ---

// Constructor: initializes vtable, stores mgr pointer, sets reserved to 0.
extern "C" void __ct__CScnRoot(void* self, void* mgr);

// Destructor
extern "C" void __dt__8CScnRootFv(void* self, int deleteFlag);

// --- CScnRoot helpers ---

// Allocates and constructs a CScnRootNw4r.
extern "C" void* func_8048ED80(void* self, u32 allocTag, void* param);

// --- FULL_MATCH virtual callback stubs ---
// These are extern "C" C-linkage functions referenced from the CScnRoot vtable.
// They unconditionally return -1 (no-op stubs for unused virtual slots).

extern "C" int func_8048EDD0()
{
    return -1;
}

extern "C" int func_8048EDD8()
{
    return -1;
}

extern "C" int func_8048EDE0()
{
    return -1;
}

// --- CScnRootNw4r class ---

// Constructor for CScnRootNw4r (inherits CScnRoot).
extern "C" void __ct__CScnRootNw4r(void* self, void* mgr, void* param);

// Destructor for CScnRootNw4r.
extern "C" void __dt__12CScnRootNw4rFv(void* self, int deleteFlag);

// --- CScnRootNw4r helpers ---

// Returns some value based on internal state.
extern "C" void* func_8048F2F0(void* self);

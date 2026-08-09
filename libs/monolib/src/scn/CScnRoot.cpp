// Translation unit: monolib/src/scn/CScnRoot
// Contains CScnRoot class and related helpers.

#include <types.h>
#include "monolib/scn/CScnRoot.hpp"

// --- CScnRoot class ---

// Constructor: initializes vtable, stores mgr pointer, sets reserved to 0.
void __ct__CScnRoot(u8* self, void* mgr);
// Destructor
void __dt__8CScnRootFv(u8* self, int deleteFlag);
// --- CScnRoot helpers ---

// Allocates and constructs a CScnRootNw4r.
void* func_8048ED80(u8* self, u32 allocTag, void* param);
// --- FULL_MATCH virtual callback stubs ---
// These are C-linkage functions referenced from the CScnRoot vtable.
// They unconditionally return -1 (no-op stubs for unused virtual slots).

int func_8048EDD0()
{
    return -1;
}

int func_8048EDD8()
{
    return -1;
}

extern "C" int func_8048EDE0()
{
    return -1;
}

// --- CScnRootNw4r class ---

// Constructor for CScnRootNw4r (inherits CScnRoot).
void __ct__CScnRootNw4r(u8* self, void* mgr, void* param);// for CScnRootNw4r.

// --- CScnRootNw4r helpers ---

// Returns some value based on internal state.
void* func_8048F2F0(u8* self);

CScnRoot::~CScnRoot() {}

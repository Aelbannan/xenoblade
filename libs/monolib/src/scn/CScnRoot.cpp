// Translation unit: monolib/src/scn/CScnRoot
// CScnRoot base class (compiler-emitted __vt__8CScnRoot + RTTI) and the
// CScnRootNw4r factory.

#include <types.h>
#include <stdlib.h>                    // labs
#include <revolution/MEM.h>
#include "monolib/util/MemManager.hpp"    // mtl::MemManager
#include "libs/monolib/src/scn/CScnRoot.hpp"       // flat layouts + retail data symbols
#include "libs/monolib/src/scn/CScnRootNw4r.hpp"   // CScnRootNw4r class


// --- CScnRoot class ---

CScnRoot::CScnRoot(CScn* scn) {
    mpScn = scn;
    mAttachCount = 0;
}

// Base object owns no resources.
CScnRoot::~CScnRoot() {}

// --- CScnRoot helpers ---

// Allocates a 0x4EC-byte CScnRootNw4r and constructs it in place.
extern "C" CScnRootNw4r* createScnRootNw4r(CScn* scn, u32 allocHandle, void* param) {
    return new (mtl::MemManager::allocate(sizeof(CScnRootNw4r), allocHandle))
        CScnRootNw4r(scn, param);
}

// Default handle getters: no handle (-1). Retail emits them in reverse slot
// order after createScnRootNw4r.
u32 CScnRoot::getAllocHandle() { return -1; }

u32 CScnRoot::getBaseHandle() { return -1; }

u32 CScnRoot::getGroupHandle() { return -1; }

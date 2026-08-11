// Translation unit for LOD::UnkClass_8046A530.
// Full layout information is in UnkClass_8046A530.hpp.

#include <harness_catalog.h>
#include "monolib/lod/LODMemMan.hpp"   // provides full LOD::LODMemMan definition

// Helper: access the LODMemMan embedded at offset 0xa44.
// Uses one-past-array addressing (well-defined C++) to avoid raw
// pointer arithmetic in callers.
LOD::LODMemMan& LOD::UnkClass_8046A530::GetMemMan() {
    return *static_cast<LOD::LODMemMan*>(
        static_cast<void*>(&mPad_00 + 1));
}

/**
 * Virtual thunk: adjust 'this' to point to the LODMemMan sub-object
 * embedded at offset 0xa44, then forward to LODMemMan::func_80471834().
 *
 * Equivalent to:
 *   addi r3, r3, 0xa44
 *   b func_80471834__Q23LOD9LODMemManFv
 */
void LOD::UnkClass_8046A530::func_8046CFD8() {
    GetMemMan().func_80471834();
}

// ---------------------------------------------------------------------------
// Stubs for non-FULL_MATCH functions - marked NonMatching in configure.py.
// These compile but do not yet produce matching code.
// ---------------------------------------------------------------------------

void LOD::UnkClass_8046A530::func_8046A530() {
    func_8046A5C4();
}

void __dt__8046A584(){}

void LOD::UnkClass_8046A530::func_8046A5C4() {}

void LOD::UnkClass_8046A530::func_8046AAD8() { func_8046AB54(); }

void LOD::UnkClass_8046A530::func_8046AADC() {}

#pragma push
#pragma auto_inline off
void LOD::UnkClass_8046A530::func_8046AB54() {}
#pragma pop

void LOD::UnkClass_8046A530::func_8046AD2C() {}

void LOD::UnkClass_8046A530::func_8046B0AC() {}

void LOD::UnkClass_8046A530::func_8046BAE0() {}

void LOD::UnkClass_8046A530::func_8046C580() {}

void LOD::UnkClass_8046A530::func_8046CFB4() {}

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

// LLM-HARNESS-BEGIN: us-80470fa8
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
// LLM-HARNESS-END: us-80470fa8

// ---------------------------------------------------------------------------
// Stubs for non-FULL_MATCH functions — marked NonMatching in configure.py.
// These compile but do not yet produce matching code.
// ---------------------------------------------------------------------------

// LLM-HARNESS-BEGIN: us-8046e500
void LOD::UnkClass_8046A530::func_8046A530() {
    func_8046A5C4();
}
// LLM-HARNESS-END: us-8046e500

// LLM-HARNESS-BEGIN: us-8046e554
extern "C" void __dt__8046A584() {}
// LLM-HARNESS-END: us-8046e554

// LLM-HARNESS-BEGIN: us-8046e594
void LOD::UnkClass_8046A530::func_8046A5C4() {}
// LLM-HARNESS-END: us-8046e594

// LLM-HARNESS-BEGIN: us-8046eaa8
void LOD::UnkClass_8046A530::func_8046AAD8() {}
// LLM-HARNESS-END: us-8046eaa8

// LLM-HARNESS-BEGIN: us-8046eaac
void LOD::UnkClass_8046A530::func_8046AADC() {}
// LLM-HARNESS-END: us-8046eaac

// LLM-HARNESS-BEGIN: us-8046eb24
void LOD::UnkClass_8046A530::func_8046AB54() {}
// LLM-HARNESS-END: us-8046eb24

// LLM-HARNESS-BEGIN: us-8046ecfc
void LOD::UnkClass_8046A530::func_8046AD2C() {}
// LLM-HARNESS-END: us-8046ecfc

// LLM-HARNESS-BEGIN: us-8046f07c
void LOD::UnkClass_8046A530::func_8046B0AC() {}
// LLM-HARNESS-END: us-8046f07c

// LLM-HARNESS-BEGIN: us-8046fab0
void LOD::UnkClass_8046A530::func_8046BAE0() {}
// LLM-HARNESS-END: us-8046fab0

// LLM-HARNESS-BEGIN: us-80470550
void LOD::UnkClass_8046A530::func_8046C580() {}
// LLM-HARNESS-END: us-80470550

// LLM-HARNESS-BEGIN: us-80470f84
void LOD::UnkClass_8046A530::func_8046CFB4() {}
// LLM-HARNESS-END: us-80470f84

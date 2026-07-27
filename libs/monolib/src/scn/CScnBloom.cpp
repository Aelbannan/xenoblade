// CScnBloom - Bloom post-processing effect
// Inherits from CScnBloomBase (primary) and CScnFilter (secondary at offset 0x48).

#include "monolib/scn/CScnBloom.hpp"
#include <harness_catalog.h>
#include <stdint.h>

// Extern declarations for functions not yet decompiled
extern "C" {
void func_8049928C(CScnBloom* self, u32 param);
void __dt__9CScnBloomFv(CScnBloom* self, s32 flags);
}

// ============================================================================
// FULL_MATCH: us-8049d7bc (func_804996D8)
// Virtual thunk for CScnFilter::renderBloom override.
// When called through the CScnFilter vtable, 'this' points to the
// CScnFilter subobject at offset 0x48 from the CScnBloom base.
// This thunk adjusts 'this' back to the CScnBloom base and tail-calls
// the implementation (func_8049928C).
// ============================================================================
extern "C" void func_804996D8(CScnBloom* self, u32 param) {
    // Adjust this from CScnFilter subobject (+0x48) to CScnBloom base
    CScnBloom* base = reinterpret_cast<CScnBloom*>(
        reinterpret_cast<uintptr_t>(self) - 0x48
    );
    // Tail-call to implementation
    typedef void (*RenderFn)(CScnBloom*, u32);
    RenderFn render = reinterpret_cast<RenderFn>(&func_8049928C);
    render(base, param);
}

// ============================================================================
// FULL_MATCH: us-8049d7c4 (func_804996E0)
// Virtual thunk for CScnFilter::destroyBloom override.
// When called through the CScnFilter vtable, 'this' points to the
// CScnFilter subobject at offset 0x48 from the CScnBloom base.
// This thunk adjusts 'this' back to the CScnBloom base and tail-calls
// the CScnBloom destructor.
// ============================================================================
extern "C" void func_804996E0(CScnBloom* self, s32 flags) {
    // Adjust this from CScnFilter subobject (+0x48) to CScnBloom base
    CScnBloom* base = reinterpret_cast<CScnBloom*>(
        reinterpret_cast<uintptr_t>(self) - 0x48
    );
    // Tail-call to destructor
    typedef void (*DtorFn)(CScnBloom*, s32);
    DtorFn dtor = reinterpret_cast<DtorFn>(&__dt__9CScnBloomFv);
    dtor(base, flags);
}

// Stub functions for other symbols in this TU (not yet decompiled)
extern "C" void func_804996D8_stub() {}

extern "C" void func_804996D8_stub2() {}

extern "C" void func_804996D8_stub3() {}

extern "C" void func_804996D8_stub4() {}

extern "C" void func_804996D8_stub5() {}

extern "C" void func_804996D8_stub6() {}

extern "C" void func_804996D8_stub7() {}

extern "C" void func_804996D8_stub8() {}

void __dt__8049A0D0(){}

void func_8049A128(){}

void func_8049A318(){}

void func_8049A850(){}

void func_8049AA74(){}

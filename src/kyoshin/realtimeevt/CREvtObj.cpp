// TU: kyoshin/realtimeevt/CREvtObj
// Class: cf::CREvtObj (namespace cf)
// Vtable at 0x80532320
// Size: 0x14 (vtable ptr + mType + __ptmf[3])
//
// Retail symbols in this split (.text 0x80186B18..0x80186BE8):
//   __ct__cf_CREvtObj       0x30  base constructor
//   __dt__Q22cf8CREvtObjFv  0x40  base destructor
//   EvtObj_RunCallback           0x48  run the callback ptmf if set
//   EvtObj_AllocBlock           0x0C  allocate a realtime-ev block (CREvtMem)
//   __dt__80185754          0x04  free a realtime-ev block (CREvtMem)
//   EvtObj_ConstFalse           0x08  constant-0 predicate
//
// Matching notes (why the ctor/dtor are C-linkage functions, not members):
//   * __ct__cf_CREvtObj is the retail SHORT constructor form (no parameter
//     suffix). MWCC 1.1 full-mangles member ctors (__ct__cf_CREvtObjFi), so
//     the ctor is written as a C-linkage function carrying the exact retail
//     name (MWCC_CASES "Constructor symbol name: MWCC 1.1 emits full
//     mangling, retail has short form").
//   * __dt__Q22cf8CREvtObjFv is Fv-mangled but reads the MWCC dtor delete
//     flag in r4: the flag-guarded call is freeEvtMemPtr (CREvtMem free),
//     not operator delete, so the two-arg C-linkage form reproduces the
//     retail body (MWCC_CASES "Extern-C for Fv-mangled functions that
//     take hidden parameters").

#include "kyoshin/realtimeevt/CREvtObj.hpp"

// ============================================================================
// __ct__cf_CREvtObj (0x80186B18)
// Base ctor: stores the object kind (r4) at +0x04, installs the cf::CREvtObj
// vtable at +0x00, and resets the callback ptmf at +0x08 to the null
// sentinel. Derived realtime-ev ctors pass their kind (0..4).
// ============================================================================
extern "C" void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg) {
    self->mType = arg;
    *(u32**)self = lbl_eu_80532320;
    self->mCallback = __ptmf_null;
}

// ============================================================================
// __dt__Q22cf8CREvtObjFv (0x80186B48)
// Two-arg dtor form returning `this` (MWCC dtor ABI). When the delete flag is
// set (r4 > 0), the instance is released through the realtime-ev memory
// manager (freeEvtMemPtr) instead of operator delete, so CREvtObj-family
// objects stay inside the CREvtMem arena.
// ============================================================================
extern "C" cf::CREvtObj* __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        freeEvtMemPtr(self);
    }
    return self;
}

// ============================================================================
// EvtObj_RunCallback (0x80186B88)
// Runs the stored callback (mCallback) when it is non-null; used by the
// per-frame step of derived realtime-ev objects (e.g. CREvtModel).
// ============================================================================
// Retail view of the CREvtObj base: C++ vptr @0, kind @4, callback ptmf @8.
struct EvtObjBaseView {
    u32* vtable;
    u32 mType;
    void (cf::CREvtObj::*mCallback)();
};

extern "C" void EvtObj_RunCallback(cf::CREvtObj* self) {
    EvtObjBaseView* o = (EvtObjBaseView*)self;
    if (__ptmf_test(&o->mCallback)) {
        (self->*o->mCallback)();
    }
}

// ============================================================================
// EvtObj_AllocBlock (0x80186BD0)
// Allocates a realtime-ev block of `ptr` bytes via func_80167F6C (4-byte
// aligned, MEM1 heap). Called by the CREvtModel factory with the derived
// class sizes (0x1F0 / 0xB4 / 0x88) before running the derived ctor.
// ============================================================================
extern "C" void* EvtObj_AllocBlock(u32 size) {
    // First arg is a byte count (callers pass 0x1F0/0xB4/0x88), not a pointer.
    return func_80167F6C((void*)size, 4, 1);
}

// ============================================================================
// __dt__80185754 (0x80186BDC)
// Deleting-dtor alias: a 4-byte tail-call to freeEvtMemPtr. Derived-class
// deleting-dtor epilogues call this to release the base through CREvtMem.
// ============================================================================
extern "C" void __dt__80185754(u8* block) {
    freeEvtMemPtr(block);
}

// ============================================================================
// EvtObj_ConstFalse (0x80186BE0)
// Constant-0 predicate (li r3,0 / blr).
// ============================================================================
extern "C" int EvtObj_ConstFalse() { return 0; }

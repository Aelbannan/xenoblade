// TU: kyoshin/realtimeevt/CREvtObj
// Class: cf::CREvtObj (namespace cf)
// Vtable at 0x80532320
// Size: 0x14 (vtable ptr + field_04 + __ptmf[3])

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

// __ptmf intrinsics and null sentinel
// __ptmf_null: zero/null member-function-pointer constant (3 words at +0x08)
extern void (cf::CREvtObj::*const __ptmf_null)();

// ============================================================================
// Constructor: __ct__cf_CREvtObj
// r3 = this, r4 = arg
// Stores vtable, arg at +0x04, initializes __ptmf at +0x08 to null
// ============================================================================
extern "C" void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg) {
    self->field_04 = arg;
    self->vtable = (void*)lbl_eu_80532320;
    self->mCallback = __ptmf_null;
}

// ============================================================================
// Destructor: __dt__Q22cf8CREvtObjFv
// Two-arg POD-destructor form returning `this` (MWCC dtor ABI), matching the
// repo DEFINE_POD_DTOR convention. deleteFlag>0 means the memory-manager free
// (func_80167FFC) should run -- CREvtObj instances are freed through CREvtMem,
// not operator delete, hence the direct call instead of MWCC's __dl__FPv.
// ============================================================================
extern "C" cf::CREvtObj* __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        func_80167FFC(self);
    }
    return self;
}

// ============================================================================
// func_80185700: Calls the stored __ptmf if non-null
// ============================================================================
extern "C" void func_80185700(cf::CREvtObj* self) {
    if (__ptmf_test(&self->mCallback)) {
        (self->*self->mCallback)();
    }
}

// ============================================================================
// func_80185748: Tail-calls func_80167F6C(this, 4, 1)
// ============================================================================
extern "C" void* func_80185748(void* ptr) {
    return func_80167F6C(ptr, 4, 1);
}

// ============================================================================
// __dt__80185754: Tail-calls func_80167FFC(this)
// ============================================================================
extern "C" void __dt__80185754(void* ptr) {
    func_80167FFC(ptr);
}

// ============================================================================
// func_80185758: Returns 0 unconditionally
// ============================================================================
extern "C" int func_80185758() { return 0; }
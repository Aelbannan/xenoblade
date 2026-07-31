// TU: kyoshin/realtimeevt/CREvtObj
// Class: cf::CREvtObj (namespace cf)
// Vtable at 0x80532320
// Size: 0x14 (vtable ptr + field_04 + __ptmf[3])

#include "kyoshin/realtimeevt/CREvtObj.hpp"
#include "kyoshin/harness_catalog.hpp"

#include <cstring>

// Forward declarations for functions in CREvtMem
extern "C" void* func_80167F6C(void* ptr, u32 alignment, int useMEM1);
extern "C" void func_80167FFC(void* ptr);

// __ptmf intrinsics
extern "C" {
extern u32 __ptmf_null[3];
long __ptmf_test(void* ptmf);
void __ptmf_scall(void* obj, ...);
}

// Vtable for cf::CREvtObj
extern "C" {
extern const u32 lbl_eu_80532320[];
}

// ============================================================================
// Constructor: cf::CREvtObj::CREvtObj(int arg)
// Stores vtable, arg at +0x04, initializes __ptmf at +0x08 to null
// ============================================================================
void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg) {
    self->mField04 = arg;
    self->mPtmf[0] = __ptmf_null[0];
    self->mPtmf[1] = __ptmf_null[1];
    self->mPtmf[2] = __ptmf_null[2];
}

// ============================================================================
// Destructor: cf::CREvtObj::~CREvtObj()
// If dealloc_flag > 0, calls func_80167FFC(this) to deallocate from CREvtMem
// ============================================================================
cf::CREvtObj::~CREvtObj() {
    // hidden int dealloc_flag is passed by MWCC
}

// Note: The destructor body in the assembly checks dealloc_flag and calls
// func_80167FFC. MWCC generates the flag check automatically. The actual
// __dt__Q22cf8CREvtObjFv gets the flag from the hidden int parameter.

// ============================================================================
// func_80185700: Calls the stored __ptmf if non-null
// ============================================================================
void func_80185700(cf::CREvtObj* self) {
    if (__ptmf_test(&self->mPtmf)) {
        __ptmf_scall(self, &self->mPtmf);
    }
}

// ============================================================================
// func_80185748: Allocates from CREvtMem with alignment 4, useMEM1=1
// ============================================================================
extern "C" void* func_80185748(cf::CREvtObj* self) {
    return func_80167F6C(self, 4, 1);
}

// ============================================================================
// __dt__80185754: Deallocates from CREvtMem
// ============================================================================
extern "C" void __dt__80185754(void* ptr) {
    func_80167FFC(ptr);
}

// ============================================================================
// func_80185758: Returns 0 unconditionally
// ============================================================================
extern "C" int func_80185758() { return 0; }
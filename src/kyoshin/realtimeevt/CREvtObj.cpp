// TU: kyoshin/realtimeevt/CREvtObj
// Class: cf::CREvtObj (namespace cf)
// Vtable at 0x80532320
// Size: 0x14 (vtable ptr + field_04 + __ptmf[3])

#include <types.h>
#include "kyoshin/realtimeevt/CREvtObj.hpp"

// Forward declarations for functions in CREvtMem
extern "C" void* func_80167F6C(void* ptr, u32 alignment, int useMEM1);
extern "C" void func_80167FFC(void* ptr);

// __ptmf intrinsics and null sentinel
extern "C" {
extern u32 __ptmf_null[3];
long __ptmf_test(void* ptmf);
void __ptmf_scall(void* obj, ...);
}

// Vtable for cf::CREvtObj
extern "C" void* lbl_eu_80532320[];

// ============================================================================
// Constructor: __ct__cf_CREvtObj
// r3 = this, r4 = arg
// Stores vtable, arg at +0x04, initializes __ptmf at +0x08 to null
// ============================================================================
extern "C" void __ct__cf_CREvtObj(cf::CREvtObj* self, int arg) {
    self->field_04 = arg;
    self->vtable = (void*)lbl_eu_80532320;
    const u32* src = &__ptmf_null[0];
    u32* dst = &self->ptmf[0];
    u32 tmp0 = *src++;
    dst[1] = *src++;
    dst[2] = *src;
    dst[0] = tmp0;
}

// ============================================================================
// Destructor: __dt__Q22cf8CREvtObjFv
// r3 = this, r4 = dealloc_flag
// If dealloc_flag > 0, calls func_80167FFC(this) to deallocate from CREvtMem
// ============================================================================
extern "C" void __dt__Q22cf8CREvtObjFv(cf::CREvtObj* self, int dealloc_flag) {
    if (self != nullptr) {
        if (dealloc_flag > 0) {
            func_80167FFC(self);
        }
    }
}

// ============================================================================
// func_80185700: Calls the stored __ptmf if non-null
// ============================================================================
extern "C" void func_80185700(cf::CREvtObj* self) {
    if (__ptmf_test(&self->ptmf)) {
        __ptmf_scall(self, &self->ptmf);
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
#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* func_80167F6C(void* ptr, u32 alignment, int useMEM1);
extern "C" void func_80167FFC(void* ptr);
extern "C" long __ptmf_test(void* ptmf);
extern "C" void* lbl_eu_80532320[]; // vtable for cf::CREvtObj

namespace cf {

// CREvtObj - base class for realtime event objects
// Vtable at 0x80532320 (size 0x28 = 10 entries)
// Total size: 0x14
struct CREvtObj {
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ void (CREvtObj::*mCallback)();  // __ptmf (12 bytes: 3 x u32)
    // mCallback layout at +0x08: [0]=pfn, [1]=obj/this, [2]=delta
};

} // namespace cf
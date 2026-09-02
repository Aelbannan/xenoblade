#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* func_80167F6C(void* ptr, u32 alignment, int useMEM1);
extern "C" void func_80167FFC(void* ptr);
extern "C" long __ptmf_test(void* ptmf);
extern "C" void* lbl_eu_80532320[]; // vtable for cf::CREvtObj

namespace cf {

// CREvtObj - base class for realtime event objects.
// Vtable at 0x80532320 (size 0x28 = 10 entries); total size 0x14.
//
// Layout:
//   0x00 vtable    vptr (lbl_eu_80532320), installed by the base ctor
//   0x04 mType     realtime-ev kind set by each derived ctor
//                  (0=camera, 1=effect, 2=light, 3=model, 4=movie)
//   0x08 mCallback callback ptmf (3 words: [0]=pfn, [1]=obj, [2]=delta);
//                  reset to the null sentinel by the base ctor
class __declspec(novtable) CREvtObj {
public:
    virtual ~CREvtObj();
    virtual void vfunc_04();
    virtual void vfunc_08();
    virtual void vfunc_0C();
    virtual void vfunc_10();
    virtual void vfunc_14();
    virtual bool isBusy();
    virtual void vfunc_1C();
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 mType;
    /* 0x08 */ void (CREvtObj::*mCallback)();  // __ptmf (12 bytes: 3 x u32)
};

} // namespace cf

// Null member-function-pointer constant - the default mCallback (3 words).
extern void (cf::CREvtObj::*const __ptmf_null)();

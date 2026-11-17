#pragma once

#include <types.h>
#include "kyoshin/realtimeevt/CREvtMem.hpp" // allocEvtMemAligned / freeEvtMemPtr

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

namespace cf {

// CREvtObj - base class for realtime event objects.
// Total size 0x14.
//
// Layout:
//   0x00 vtable    vptr
//   0x04 mType     realtime-ev kind set by each derived ctor
//                  (0=camera, 1=effect, 2=light, 3=model, 4=movie)
//   0x08 mCallback callback ptmf (3 words: [0]=pfn, [1]=obj, [2]=delta);
//                  reset to the null sentinel by the base ctor
class CREvtObj {
public:
    CREvtObj(int type);
    virtual ~CREvtObj();
    virtual void vfunc_04() = 0;
    virtual void vfunc_08();
    virtual int vfunc_0C();   // returns 0
    virtual int vfunc_10();   // returns 1 (defined in CTaskREvtSequence.cpp)
    virtual int vfunc_14();   // returns 1 (defined in CREvtCamera.cpp)
    virtual bool isBusy();    // returns false (defined in CTaskREvtSequence.cpp)
    virtual void vfunc_1C();  // runs mCallback when set
    /* 0x04 */ u32 mType;
    /* 0x08 */ void (CREvtObj::*mCallback)();  // __ptmf (12 bytes: 3 x u32)

    // Realtime-ev objects live in the CREvtMem arena.
    static void* operator new(size_t size);
    static void operator delete(void* p);
};

} // namespace cf

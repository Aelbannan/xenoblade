#pragma once

#include <types.h>

//size: 0x4
class __declspec(novtable) CDeviceVICb {
public:
    enum VICallback {
        VI_CALLBACK_BEFORE_DRAW_DONE,
        VI_CALLBACK_AFTER_DRAW_DONE,
        VI_CALLBACK_BEGIN_FRAME
    };

    CDeviceVICb();
    virtual ~CDeviceVICb();
    virtual void viBeforeDrawDone(){}
    virtual void viAfterDrawDone(){}
    virtual void viBeginFrame();
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Retail CDeviceVICb vtable data (monolibdata2.s @0x8056BF68). The class is
// __declspec(novtable), so ctor/dtor assign the retail label explicitly
// instead of the compiler-generated __vt__11CDeviceVICb (which would add
// .data/.rodata/RTTI to a retail-empty TU).
extern "C" void* lbl_eu_8056BF68[];

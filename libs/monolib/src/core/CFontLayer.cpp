// Translation unit: monolib/src/core/CFontLayer
// Contains CFontLayer constructor, destructor, and thin forwarding functions
// that delegate to CDeviceFont.

#include <types.h>
#include "monolib/core/CFontLayer.hpp"
#include "monolib/device/CDeviceFont.hpp"

// LLM-HARNESS-BEGIN: us-8044ba4c
extern "C" void __ct__CFontLayer() {}
// LLM-HARNESS-END: us-8044ba4c

// LLM-HARNESS-BEGIN: us-8044ba88
extern "C" void __dt__10CFontLayerFv() {}
// LLM-HARNESS-END: us-8044ba88

// LLM-HARNESS-BEGIN: us-8044bae8
extern "C" void func_80449078__10CFontLayerFv() {}
// LLM-HARNESS-END: us-8044bae8

// LLM-HARNESS-BEGIN: us-8044bbb8
extern "C" void func_80449148__10CFontLayerFv(void) {}
// LLM-HARNESS-END: us-8044bbb8

// LLM-HARNESS-BEGIN: us-8044bbbc
extern "C" void func_8044914C__10CFontLayerFv(void) {}
// LLM-HARNESS-END: us-8044bbbc

// LLM-HARNESS-BEGIN: us-8044bbc0
extern "C" void func_80449150__10CFontLayerFv(void) {}
// LLM-HARNESS-END: us-8044bbc0

// LLM-HARNESS-BEGIN: us-8044bbc4
extern "C" void func_80449154__10CFontLayerFv(void) {}
// LLM-HARNESS-END: us-8044bbc4

// LLM-HARNESS-BEGIN: us-8044bbc8
extern "C" void func_80449158__10CFontLayerFv(void) {}
// LLM-HARNESS-END: us-8044bbc8

// LLM-HARNESS-BEGIN: us-8044bbcc
// FULL_MATCH: Pure tail call to CDeviceFont::func_80452CF8 (same this).
// The 'channel' parameter is accepted but unused in this forwarding path.
void CFontLayer::fontFlush(int channel) {
    // Decomp note: reinterpret_cast is used because CFontLayer and
    // CDeviceFont are unrelated types -- the tail call relies on the
    // containing object's layout placing both at the same address.
    reinterpret_cast<CDeviceFont*>(this)->func_80452CF8();
}
// LLM-HARNESS-END: us-8044bbcc

// LLM-HARNESS-BEGIN: us-8044bbd0
extern "C" void func_80449160__10CFontLayerFv(void) {}
// LLM-HARNESS-END: us-8044bbd0

// Translation unit: monolib/src/core/CFontLayer
// Contains CFontLayer constructor, destructor, and thin forwarding functions
// that delegate to CDeviceFont.

#include <types.h>
#include "monolib/core/CFontLayer.hpp"
#include "monolib/device/CDeviceFont.hpp"

extern "C" void __ct__CFontLayer() {}

extern "C" void __dt__10CFontLayerFv() {}

extern "C" void func_80449078__10CFontLayerFv() {}

extern "C" void func_80449148__10CFontLayerFv(void) {}

extern "C" void func_8044914C__10CFontLayerFv(void) {}

extern "C" void func_80449150__10CFontLayerFv(void) {}

extern "C" void func_80449154__10CFontLayerFv(void) {}

extern "C" void func_80449158__10CFontLayerFv(void) {}

// FULL_MATCH: Pure tail call to CDeviceFont::func_80452CF8 (same this).
// The 'channel' parameter is accepted but unused in this forwarding path.
void CFontLayer::fontFlush(int channel) {
    // Decomp note: reinterpret_cast is used because CFontLayer and
    // CDeviceFont are unrelated types -- the tail call relies on the
    // containing object's layout placing both at the same address.
    reinterpret_cast<CDeviceFont*>(this)->func_80452CF8();
}

extern "C" void func_80449160__10CFontLayerFv(void) {}

// Translation unit: monolib/src/core/CFontLayer
// Contains CFontLayer constructor, destructor, and thin forwarding functions
// that delegate to CDeviceFont.

#include <types.h>
#include "monolib/core/CFontLayer.hpp"
#include "monolib/device/CDeviceFont.hpp"

void __ct__CFontLayer(){}

void CFontLayer::~CFontLayer() const {}

void CFontLayer::func_80449078() const {}

void CFontLayer::func_80449148(void) const {}

void CFontLayer::func_8044914C(void) const {}

void CFontLayer::func_80449150(void) const {}

void CFontLayer::func_80449154(void) const {}

void CFontLayer::func_80449158(void) const {}

// FULL_MATCH: Pure tail call to CDeviceFont::func_80452CF8 (same this).
// The 'channel' parameter is accepted but unused in this forwarding path.
void CFontLayer::fontFlush(int channel) {
    // Decomp note: reinterpret_cast is used because CFontLayer and
    // CDeviceFont are unrelated types -- the tail call relies on the
    // containing object's layout placing both at the same address.
    reinterpret_cast<CDeviceFont*>(this)->func_80452CF8();
}

void CFontLayer::func_80449160(void) const {}

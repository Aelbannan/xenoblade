#pragma once

#include <types.h>

// Forward declaration for tail-call target
class CDeviceFont;

/// Font layer base class (size 0x4: vtable pointer only).
/// CView inherits from this via multiple inheritance.
class CFontLayer {
public:
    CFontLayer();
    virtual ~CFontLayer();

    /// Flush pending font rendering state.
    /// Delegates to CDeviceFont internally; the int parameter is passed
    /// through to the device layer but may be unused depending on context.
    void fontFlush(int channel);
};

#pragma once

#include <types.h>

// Forward declaration for tail-call target
class CDeviceFont;

/// Font layer base class (size 0x4: vtable pointer only).
/// CView inherits from this via multiple inheritance. The vtable is emitted by
/// the shared monolib data blob (CFontLayer.o's .data owns the 0x10-byte
/// object), so the class is __declspec(novtable) and the constructor installs
/// the vptr by hand; that also keeps CFontLayer.o free of the weak RTTI
/// locator + name string that retail's linked image does not keep here.
class __declspec(novtable) CFontLayer {
public:
    // Declared so derived classes emit the base-constructor call; the retail
    // symbol is the un-mangled `__ct__CFontLayer`, which CFontLayer.cpp defines
    // as a C-linkage function (the CView.o UNIT_RULES retarget the mangled
    // reference onto it).
    CFontLayer();
    virtual ~CFontLayer();

    /// Flush pending font rendering state.
    /// Delegates to CDeviceFont internally; the int parameter is passed
    /// through to the device layer but may be unused depending on context.
    void fontFlush(int channel);

    void resetCursor();
    void clearBuffer();
    void updateLayout();
    void drawText();
    void flushBuffer();
    void setupDraw();
};

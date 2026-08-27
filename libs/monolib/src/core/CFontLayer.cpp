// Translation unit: monolib/src/core/CFontLayer
// Contains CFontLayer constructor, destructor, and thin forwarding functions
// that delegate to CDeviceFont.
//
// NOTE: CFontLayer and CDeviceFont share the same object address by layout, so
// every forward below passes the `this`/`self` pointer through unchanged.
// The CDeviceFont member functions referenced here are not declared by the
// locked include header, so they are imported as extern "C" declarations using
// their exact retail mangled names (extern "C" preserves the literal symbol
// while still allowing MWCC to set up the argument registers).

#include <types.h>
#include <stdio.h>
#include <stdarg.h>
#include "monolib/core/CFontLayer.hpp"
#include "monolib/device/CDeviceFont.hpp"

extern "C" {
void func_804525F0__11CDeviceFontFv(CDeviceFont* self);
void notifyLayerDestroy__11CDeviceFontFv(CDeviceFont* self);
void dispatchLayerBB4__11CDeviceFontFv(
    CDeviceFont* self, const char* a, const char* b, const char* str);
void getLayerStatus__11CDeviceFontFv(CDeviceFont* self);
void dispatchLayer3FF0__11CDeviceFontFv(CDeviceFont* self);
void dispatchLayer41F8__11CDeviceFontFv(CDeviceFont* self);
void dispatchLayer438C__11CDeviceFontFv(CDeviceFont* self);
void dispatchLayer4508__11CDeviceFontFv(CDeviceFont* self);
void getLayerData__11CDeviceFontFv(CDeviceFont* self);
void dispatchLayer4B70__11CDeviceFontFv(CDeviceFont* self);
void __dl__FPv(void*);
}

// Retail vtable (monolibdata2.s-owned .data 0x8056BF80): the class's ctor/dt
// reference it by name so no local __vt__/RTTI is emitted (retail CFontLayer.o
// is text-only; the vtable lives in the shared data split).
extern "C" u32 lbl_eu_80663598;
extern "C" u32 lbl_eu_8056BF80[];

// The retail ctor symbol is the un-mangled `__ct__CFontLayer` (not the C++
// member mangling); hand-writing it as an extern "C" free function also
// suppresses the local vtable emission (no key function defined in this TU).
extern "C" void* __ct__CFontLayer(CFontLayer* self) {
    *(void**)self = (void*)lbl_eu_8056BF80;
    func_804525F0__11CDeviceFontFv((CDeviceFont*)self);
    return self;
}

// Retail dtor keeps MWCC's delete-flag idiom: `cmpi r31, 0; bc 4, 1` (delete
// when flag > 0). The `if (flag > 0)` form reproduces the exact branch.
extern "C" void* __dt__10CFontLayerFv(CFontLayer* self, int flag) {
    if (self != 0) {
        *(void**)self = (void*)lbl_eu_8056BF80;
        notifyLayerDestroy__11CDeviceFontFv((CDeviceFont*)self);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Thin variadic forwarding: build a formatted string on the stack and push it
// to the device font layer along with the first two arguments.
extern "C" void printFormatted__10CFontLayerFv(
    CFontLayer* self, const char* a, const char* b, const char* fmt, ...) {
    char buffer[0x200];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    dispatchLayerBB4__11CDeviceFontFv((CDeviceFont*)self, a, b, buffer);
}

extern "C" void resetCursor__10CFontLayerFv(CFontLayer* self) {
    // Pure tail call to CDeviceFont (same this).
    getLayerStatus__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void clearBuffer__10CFontLayerFv(CFontLayer* self) {
    dispatchLayer3FF0__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void updateLayout__10CFontLayerFv(CFontLayer* self) {
    dispatchLayer41F8__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void drawText__10CFontLayerFv(CFontLayer* self) {
    dispatchLayer438C__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void flushBuffer__10CFontLayerFv(CFontLayer* self) {
    dispatchLayer4508__11CDeviceFontFv((CDeviceFont*)self);
}

// FULL_MATCH: Pure tail call to CDeviceFont::dispatchLayer4B70 (same this).
// The 'channel' parameter is accepted but unused in this forwarding path.
void CFontLayer::fontFlush(int channel) {
    // Decomp note: reinterpret_cast is used because CFontLayer and
    // CDeviceFont are unrelated types -- the tail call relies on the
    // containing object's layout placing both at the same address.
    reinterpret_cast<CDeviceFont*>(this)->dispatchLayer4B70();
}

extern "C" void setupDraw__10CFontLayerFv(CFontLayer* self) {
    getLayerData__11CDeviceFontFv((CDeviceFont*)self);
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056BF80-0x8056BF90 (16B): vtbl { RTTI-obj lbl_eu_80663598, 0, dtor __dt__10CFontLayerFv, 0 }
extern "C" u32 lbl_eu_8056BF80[4] = {
    (u32)&lbl_eu_80663598,
    0,
    (u32)&__dt__10CFontLayerFv,
    0,
};
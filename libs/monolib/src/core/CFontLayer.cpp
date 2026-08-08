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
void func_80452690__11CDeviceFontFv(CDeviceFont* self);
void func_8045271C__11CDeviceFontFv(
    CDeviceFont* self, const char* a, const char* b, const char* str);
void func_804527A4__11CDeviceFontFv(CDeviceFont* self);
void func_8045283C__11CDeviceFontFv(CDeviceFont* self);
void func_804528C4__11CDeviceFontFv(CDeviceFont* self);
void func_8045294C__11CDeviceFontFv(CDeviceFont* self);
void func_804529D4__11CDeviceFontFv(CDeviceFont* self);
void func_80452B78__11CDeviceFontFv(CDeviceFont* self);
}

CFontLayer::CFontLayer() {
    func_804525F0__11CDeviceFontFv((CDeviceFont*)this);
}

CFontLayer::~CFontLayer() {
    func_80452690__11CDeviceFontFv((CDeviceFont*)this);
}

// Thin variadic forwarding: build a formatted string on the stack and push it
// to the device font layer along with the first two arguments.
extern "C" void func_80449078__10CFontLayerFv(
    CFontLayer* self, const char* a, const char* b, const char* fmt, ...) {
    char buffer[0x200];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    func_8045271C__11CDeviceFontFv((CDeviceFont*)self, a, b, buffer);
}

extern "C" void func_80449148__10CFontLayerFv(CFontLayer* self) {
    // Pure tail call to CDeviceFont (same this).
    func_804527A4__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void func_8044914C__10CFontLayerFv(CFontLayer* self) {
    func_8045283C__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void func_80449150__10CFontLayerFv(CFontLayer* self) {
    func_804528C4__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void func_80449154__10CFontLayerFv(CFontLayer* self) {
    func_8045294C__11CDeviceFontFv((CDeviceFont*)self);
}

extern "C" void func_80449158__10CFontLayerFv(CFontLayer* self) {
    func_804529D4__11CDeviceFontFv((CDeviceFont*)self);
}

// FULL_MATCH: Pure tail call to CDeviceFont::func_80452CF8 (same this).
// The 'channel' parameter is accepted but unused in this forwarding path.
void CFontLayer::fontFlush(int channel) {
    // Decomp note: reinterpret_cast is used because CFontLayer and
    // CDeviceFont are unrelated types -- the tail call relies on the
    // containing object's layout placing both at the same address.
    reinterpret_cast<CDeviceFont*>(this)->func_80452CF8();
}

extern "C" void func_80449160__10CFontLayerFv(CFontLayer* self) {
    func_80452B78__11CDeviceFontFv((CDeviceFont*)self);
}
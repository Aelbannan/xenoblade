// Decompiled for monolib/src/scn/code_8049431C

#include <types.h>
#include <revolution/gx.h>
#include <monolib/core/CViewFrame.hpp>

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern void* getCurrentView__5CViewFv();
    extern void func_8043E928__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, void* view);
    extern void func_8044AA7C__8CGXCacheFii(void* self, int, int);
    extern void func_8044A94C__8CGXCacheFii(void* self, int, int);
    extern void func_8044ABAC__8CGXCacheFv(void* self);
    extern void func_8044A7F8__8CGXCacheFv(void* self, int, int, int, int, int);
    extern void* cacheInstance__9CDeviceGX;
}

// Extended texture object: GXTexObj (0x20 bytes) + extra fields.
struct ExtendedTexObj {
    u8 gxobj[0x20];
    u32 mField20;   // +0x20 (image pointer)
    u32 mField24;   // +0x24 (buffer size)
    u32 mField28;   // +0x28
};

extern "C" ExtendedTexObj* func_8049431C(ExtendedTexObj* self, void* image, u16 w, u16 h, u32 fmt, void* p6) {
    GXInitTexObj((GXTexObj*)self, image, w, h, (GXTexFmt)fmt, GX_CLAMP, GX_CLAMP, 0);
    GXInitTexObjFilter((GXTexObj*)self, GX_LINEAR, GX_NEAR);
    self->mField20 = (u32)image;
    self->mField24 = GXGetTexBufferSize(w, h, fmt, 0, 0);
    self->mField28 = (u32)p6;
    return self;
}

extern "C" void* __dt__804943A0(void* self, int deleteFlag) {
    if (self != 0 && deleteFlag > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" void func_80494540(ExtendedTexObj* self, ml::CRect* rect, u16 w, u16 h, GXBool p3, GXBool p4) {
    // Load/copy order below mirrors MWCC's scheduling: x, w, y, h, then fix parity,
    // Load/copy order mirrors MWCC's statement-by-statement emission.
    s16 x = rect->mPos.x;
    u16 dstW = w;
    s16 y = rect->mPos.y;
    u16 dstH = h;
    if ((x & 1) != 0)
        x = x - 1;
    s16 rw = rect->mSize.x;
    s16 rh = rect->mSize.y;
    GXBool clearFmt = p3;
    GXBool clearAll = p4;
    if ((y & 1) != 0)
        y = y - 1;
    if ((rw & 1) != 0)
        rw = rw - 1;
    if ((rh & 1) != 0)
        rh = rh - 1;
    if (rw != 0) {
        if (rh != 0) {
        func_8044AA7C__8CGXCacheFii(cacheInstance__9CDeviceGX, 0, 0);
        func_8044A94C__8CGXCacheFii(cacheInstance__9CDeviceGX, 0, 0);
        func_8044ABAC__8CGXCacheFv(cacheInstance__9CDeviceGX);
        func_8044A7F8__8CGXCacheFv(cacheInstance__9CDeviceGX, 1, 4, 5, 0, 0);
        GXSetTexCopySrc((u16)x, (u16)y, (u16)rw, (u16)rh);
        GXSetTexCopyDst(w, h, GXGetTexObjFmt((GXTexObj*)self), p3);
        GXCopyTex((void*)self->mField20, p4);
        GXPixModeSync();
        GXPixModeSync(); // PROBE
        GXInvalidateTexAll();
        DCFlushRange((void*)self->mField20, self->mField24);
        }
    }
}

extern "C" void func_804944DC(ExtendedTexObj* self, ml::CRect* rect, GXBool p3, GXBool p4) {
    func_80494540(self, rect, GXGetTexObjWidth((GXTexObj*)self), GXGetTexObjHeight((GXTexObj*)self), p3, p4);
}

extern "C" void func_804943E0(ExtendedTexObj* self, GXBool p2, GXBool p3) {
    void* view = getCurrentView__5CViewFv();
    if (view != 0) {
        ml::CRect rect;
        func_8043E928__5CViewFRQ22ml5CRectP5CView(rect, view);
        if (rect.mPos.x & 1) { rect.mPos.x++; rect.mSize.x--; }
        if (rect.mPos.y & 1) { rect.mPos.y++; rect.mSize.y--; }
        if (rect.mSize.x & 1) { rect.mSize.x--; }
        if (rect.mSize.y & 1) { rect.mSize.y--; }
        func_80494540(self, &rect, GXGetTexObjWidth((GXTexObj*)self), GXGetTexObjHeight((GXTexObj*)self), p2, p3);
    }
}

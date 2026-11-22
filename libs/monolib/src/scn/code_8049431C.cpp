// Decompiled for monolib/src/scn/code_8049431C

#include <types.h>
#include <revolution/gx.h>
#include <monolib/core/CViewFrame.hpp>
#include "libs/monolib/src/scn/ExtendedTexObj.hpp"

extern "C" {
    extern void* getCurrentView__5CViewFv();
    extern void insetRectToView__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect, void* view);
    extern void setZWriteMode__8CGXCacheFii(void* self, int, int);
    extern void setZCompareMD__8CGXCacheFii(void* self, int, int);
    extern void setAlphaBlend__8CGXCacheFv(void* self, int, int);
    extern void updateCmd1Words__8CGXCacheFv(void* self, int, int, int, int, int);
    extern void* cacheInstance__9CDeviceGX;
}

extern "C" ExtendedTexObj* TexObj_InitExt(ExtendedTexObj* self, void* image, u16 w, u16 h, u32 fmt, void* p6) {
    GXInitTexObj(&self->mTexObj, image, w, h, (GXTexFmt)fmt, GX_CLAMP, GX_CLAMP, 0);
    GXInitTexObjFilter(&self->mTexObj, GX_LINEAR, GX_NEAR);
    self->mField20 = (u32)image;
    self->mField24 = GXGetTexBufferSize(w, h, fmt, 0, 0);
    self->mField28 = (u32)p6;
    return self;
}

ExtendedTexObj::~ExtendedTexObj() {}

extern "C" void TexObj_CopyRect(ExtendedTexObj* self, ml::CRect* rect, u16 w, u16 h, GXBool p3, GXBool p4) {
    // Declare (for colors) then assign (for load order): MWCC numbers
    // copy-temps by declaration order but schedules loads by assignment order.
    s16 x;
    u16 dstW;
    s16 y;
    u16 dstH;
    s16 rw;
    s16 rh;
    GXBool clearFmt;
    GXBool clearAll;
    x = rect->mPos.x;
    dstW = w;
    y = rect->mPos.y;
    dstH = h;
    rw = rect->mSize.x;
    rh = rect->mSize.y;
    clearFmt = p3;
    clearAll = p4;
    if ((x & 1) != 0)
        x = x - 1;
    if ((y & 1) != 0)
        y = y - 1;
    if ((rw & 1) != 0)
        rw = rw - 1;
    if ((rh & 1) != 0)
        rh = rh - 1;
    if (rw != 0 && rh != 0) {
        setZWriteMode__8CGXCacheFii(cacheInstance__9CDeviceGX, 0, 0);
        setZCompareMD__8CGXCacheFii(cacheInstance__9CDeviceGX, 0, 0);
        setAlphaBlend__8CGXCacheFv(cacheInstance__9CDeviceGX, 0, 0);
        updateCmd1Words__8CGXCacheFv(cacheInstance__9CDeviceGX, 1, 4, 5, 0, 0);
        GXSetTexCopySrc((u16)x, (u16)y, (u16)rw, (u16)rh);
        GXSetTexCopyDst(w, h, GXGetTexObjFmt(&self->mTexObj), p3);
        GXCopyTex((void*)self->mField20, p4);
        GXPixModeSync();
        GXInvalidateTexAll();
        DCFlushRange((void*)self->mField20, self->mField24);
    }
}

extern "C" void TexObj_BlitRect(ExtendedTexObj* self, ml::CRect* rect, GXBool p3, GXBool p4) {
    TexObj_CopyRect(self, rect, GXGetTexObjWidth(&self->mTexObj), GXGetTexObjHeight(&self->mTexObj), p3, p4);
}

extern "C" void TexObj_BlitViewRect(ExtendedTexObj* self, GXBool p2, GXBool p3) {
    void* view = getCurrentView__5CViewFv();
    if (view != 0) {
        ml::CRect rect;
        insetRectToView__5CViewFRQ22ml5CRectP5CView(rect, view);
        if (rect.mPos.x & 1) { rect.mPos.x++; rect.mSize.x--; }
        if (rect.mPos.y & 1) { rect.mPos.y++; rect.mSize.y--; }
        if (rect.mSize.x & 1) { rect.mSize.x--; }
        if (rect.mSize.y & 1) { rect.mSize.y--; }
        TexObj_CopyRect(self, &rect, GXGetTexObjWidth(&self->mTexObj), GXGetTexObjHeight(&self->mTexObj), p2, p3);
    }
}

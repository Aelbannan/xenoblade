// Auto-scaffolded catalog TU for monolib/src/scn/CMdlMaterial
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/monolib/src/scn/CMdlMaterial.hpp"
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/db/db_assert.h>

// CMdlMaterial::~CMdlMaterial() — D0 deleting destructor.
// Body is empty; MWCC generates the conditional operator delete via the r4 flag.
CMdlMaterial::~CMdlMaterial() {}

void func_804E54B8(){}

void func_804E5990(){}

// Sub-object destructor for an unnamed internal class.
// Manages a dynamically-allocated buffer at offset 0x08 with an ownership
// flag at offset 0x10: when flag==0 the buffer is owned (delete[]'d here),
// when flag!=0 the buffer is merely released without freeing.
struct MdlSub {
    u8 pad_0x00[8];     // vtable + padding
    void* buf;           // 0x08
    u8 pad_0x0C[4];
    u8 flag;             // 0x10 — buffer ownership flag
};

extern "C" void __dt__804E5DE0(MdlSub* arg) {
    if (!arg->flag) {
        if (arg->buf) {
            delete[] static_cast<char*>(arg->buf);
            arg->buf = nullptr;
        }
    } else {
        arg->buf = nullptr;
    }
}

void func_804E5E38(){}

// Forward declaration: dispatches func_804E5E38 on the CMdlMaterial
// embedded at +0x16C8 of the owner. Second arg is passed but unused.
void func_80488C20(CMdlModelOwner* owner, void* arg2);

// ---------------------------------------------------------------------------
// func_804E5FD4 — Apply a uniform ambient colour derived from float RGB.
//
// Scales a float [0,1] RGB triplet to byte [0,255] RGBA (alpha=255),
// then sets the same ambient colour on all 4 GX channels of every material.
// ---------------------------------------------------------------------------
u32 func_804E5FD4(CMdlMaterial* self, f32* rgb) {
    if (!self->buffer) {
        return 0;
    }

    // Scale float [0-1] to byte [0-255], alpha = 0xFF
    f32 r = 255.0f * rgb[0];
    f32 g = 255.0f * rgb[1];
    f32 b = 255.0f * rgb[2];

    GXColor color;
    color.r = (u8)(s32)r;
    color.g = (u8)(s32)g;
    color.b = (u8)(s32)b;
    color.a = 0xFF;

    CMdlModelOwner* owner = self->owner;
    nw4r::g3d::ScnMdl* scnMdl = owner->scnMdl;

    for (u32 i = 0; i < owner->resMdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat resMat = owner->resMdl.GetResMat(i);
        if (!resMat.ptr()) {
            NW4R_PANIC("GetResMat returned null for index %u", i);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(scnMdl, resMat.GetID());
        nw4r::g3d::ResMatChan chan = matAccess.GetResMatChan(false);

        chan.GXSetChanAmbColor(GX_COLOR0, color);
        chan.GXSetChanAmbColor(GX_ALPHA0, color);
        chan.GXSetChanAmbColor(GX_COLOR1, color);
        chan.GXSetChanAmbColor(GX_ALPHA1, color);
    }

    return 1;
}

void func_804E6158(){}

// ---------------------------------------------------------------------------
// func_804E6358 — Apply per-material ambient channel colours from buffer.
//
// For each material in the ResMdl, reads 4 GXColor values from this->buffer
// (one group of 4 per material) and pushes them into channels 0/2/1/3
// (GX_COLOR0, GX_ALPHA0, GX_COLOR1, GX_ALPHA1).
// ---------------------------------------------------------------------------
u32 func_804E6358(CMdlMaterial* self) {
    if (!self->buffer) {
        return 0;
    }

    CMdlModelOwner* owner = self->owner;
    nw4r::g3d::ScnMdl* scnMdl = owner->scnMdl;
    int offs = 0;

    for (u32 i = 0; i < owner->resMdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat resMat = owner->resMdl.GetResMat(i);
        if (!resMat.ptr()) {
            NW4R_PANIC("GetResMat returned null for index %u", i);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(scnMdl, resMat.GetID());
        nw4r::g3d::ResMatChan chan = matAccess.GetResMatChan(false);

        chan.GXSetChanAmbColor(GX_COLOR0, *(GXColor*)((u8*)self->buffer + offs));
        offs += 4;
        chan.GXSetChanAmbColor(GX_ALPHA0, *(GXColor*)((u8*)self->buffer + offs));
        offs += 4;
        chan.GXSetChanAmbColor(GX_COLOR1, *(GXColor*)((u8*)self->buffer + offs));
        offs += 4;
        chan.GXSetChanAmbColor(GX_ALPHA1, *(GXColor*)((u8*)self->buffer + offs));
        offs += 4;
    }

    return 1;
}

// ---------------------------------------------------------------------------
// func_804E64B0 — Scan the inline byte array and dispatch on quotient match.
//
// For each byte b at byteArray[i] (i = 0 .. field_0x30-1), computes
// quotient = b / 10.  If it matches owner->targetQuotient, dispatches
// func_804E5E38 on the embedded CMdlMaterial at +0x16C8 via func_80488C20.
// ---------------------------------------------------------------------------
void func_804E64B0(CMdlMaterial* self, void* arg2, CMdlModelOwner* owner) {
    for (int i = 0; i < (int)self->field_0x30; i++) {
        int v = self->byteArray[i];
        int q = v / 10;
        int r = v % 10;
        if (q == owner->targetQuotient) {
            func_80488C20(owner, arg2);
        }
    }
}

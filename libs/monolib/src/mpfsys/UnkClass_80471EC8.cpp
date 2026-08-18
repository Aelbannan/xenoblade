// Auto-scaffolded catalog TU for monolib/src/mpfsys/UnkClass_80471EC8
// Replace stubs with high-level C/C++ during decomp.

#include "monolib/mpfsys/UnkClass_80471EC8.hpp"
#include "monolib/mpfsys/MPFDrawDisplayList.hpp"
#include <harness_catalog.h>

#include <revolution/GX.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/g3d_state.h>
#include <nw4r/g3d/res/g3d_resfile.h>

// mpfsys render-state globals (SDA-relative linker symbols). Plain
// global-scope externs - MWCC does not mangle global-scope variable names,
// so they emit the exact retail lbl_eu_* symbols without extern "C".
extern u8* lbl_eu_80665844;
extern s32 lbl_eu_80665854;
extern s16 lbl_eu_80665858;
extern s16 lbl_eu_8066585A;
extern s16 lbl_eu_8066585C;
extern u32 lbl_eu_8066586C;
// Ambient-color / key-color globals used by func_80474E24/E68/74DAC. The
// effective compiled header (libs/monolib/include/...) does not declare
// these, so they are declared here like the other lbl_* externs in this file.
extern u32 lbl_eu_80665878;
extern u32 lbl_eu_8066587C;
extern GXColor lbl_eu_80663858;
extern const f32 lbl_eu_8066A7D8;
extern const f32 lbl_eu_8066A734;
extern const f32 lbl_eu_8066A738;
extern const f32 lbl_eu_8066A73C;
extern u32 lbl_eu_8066A7A0;
extern u16* lbl_eu_80665850;
extern u32 lbl_eu_8066A7D0;
extern u32 lbl_eu_8066A7D4;
extern const f32 lbl_eu_8066A744;
extern const f32 lbl_eu_8066A74C;
extern const f32 lbl_eu_8066A758;
extern const f32 lbl_eu_8066A75C;
extern const f32 lbl_eu_8066A770;
// Embedded-subobject pointer cache (written by func_8047BE8C in
// code_8047BB54.cpp); func_80471FCC registers `this` here.
extern void* lbl_eu_80665838;
// Panic file-name / message strings (retail rodata).
extern const char lbl_eu_8052637C[];
extern const char lbl_eu_80526354[];

// Retail-named imports. These resolve against retail symbols (func_8049626C is
// defined in scn/CScn.cpp, func_80477F80 in MPFDrawDisplayList.cpp,
// func_80474064 is still retail at 0x80474064); extern "C" keeps the call-site
// relocs verbatim (MWCC would otherwise re-mangle the `__` names with the
// parameter encoding).
extern "C" void* func_8049626C(void* camera, void* view);
extern "C" void func_80477F80__Q26mpfsys18MPFDrawDisplayListFv(
    mpfsys::MPFDrawDisplayList* inst, u8* ptr, u8 byte, u32 count);
// Layer-draw push, retail member of UnkClass_80471EC8: Fv-annotated but
// actually receives (self, frame+0x9C, a5, a6, f1) from func_80471FCC.
extern "C" void func_80474064__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, u8* ptr, void* a5, void* a6, f32 f1);

// Map descriptor overlay: +0x10 is the display-list sub-count used both as the
// gate and as the byte offset to the per-layer list array.
struct MpfsysDescFCC {
    u8 field_0x0[0x10];
    u32 field_0x10;
};

// View frame returned by func_8049626C; +0x9C is handed to the layer draws,
// +0x1E0 holds the layer scale (mirrors UnkViewFrame in code_8047BB54.cpp).
struct MpfsysViewFrame {
    u8 field_0x0[0x9C];
    u8 field_0x9C[0x1E0 - 0x9C];
    f32 field_0x1E0;
};

// ResFile/camera state tail of UnkClass_80471EC8 (offsets recovered from
// func_80471FCC / func_80471EC8). The compiled class header has no fields.
struct MpfsysResState {
    u8 field_0x0[0x2E08];
    void* field_0x2E08; // camera (func_8049626C arg)
    void* field_0x2E0C; // view (func_8049626C arg)
    u8 field_0x2E10;
};

// ResFile container header: magic ('MPFF'), revision, and the relative
// offset + sub-count that gate the ResFile bind.
struct MpfsysResDesc {
    u32 field_0x0; // +0x00 magic
    u32 field_0x4; // +0x04 revision
    u8 field_0x8[0x14 - 0x8];
    u32 field_0x14; // +0x14 relative offset to ResFile data
    u32 field_0x18; // +0x18 sub-count gate
};

// Local layout overlay for the tail of UnkClass_80471EC8 (offsets recovered
// from func_8047230C). The class header shipped with the scaffold has no
// fields, so the layer-scale state is described here instead.
struct MpfsysLayerScaleState {
    u8 field_0x0[0x2DFC];
    f32 field_0x2DFC;
    f32 field_0x2E00;
};

// Local layout overlay for the layer-colour triple at 0x2DE0 and the mode
// field at 0x2E04 (offsets recovered from func_80472870). Same rationale as
// MpfsysLayerScaleState: the compiled class header has no fields.
struct MpfsysLayerColorState {
    u8 field_0x0[0x2DE0];
    f32 field_0x2DE0;
    f32 field_0x2DE4;
    f32 field_0x2DE8;
    u8 field_0x2DEC[0x18];
    s32 field_0x2E04;
};

// Local layout overlay for the 256-bit flag array at 0x2D00 (accessed by
// func_8047233C/func_80472370). Same rationale as MpfsysLayerScaleState: the
// class header's field layout is not compiled into this TU.
struct MpfsysFlagArray {
    u8 field_0x0[0x2D00];
    u32 field_0x2D00[8];
};

// Map attach: validate the descriptor magic/revision, then bind the embedded
// ResFile (alignment-panic, revision/tex checks, Init) and stash the camera /
// view pair, before refreshing the layer state. Returns 1 on success.
int func_80471EC8__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, void* a4, void* a5, void* a6, u32 a7) {
    MpfsysResDesc* d = (MpfsysResDesc*)a4;
    if (d->field_0x0 == 0x4D504646 && d->field_0x4 == 0x3EA) {
        if (d->field_0x18 != 0) {
            void* resData = (u8*)a4 + d->field_0x14;
            nw4r::g3d::ResFile resFile(resData);
            if (((u32)resData) & 0x1F) {
                nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
            }
            if (!resFile.CheckRevision()) {
                return 0;
            }
            if (!resFile.HasResTex()) {
                return 0;
            }
            resFile.Init();
            *(nw4r::g3d::ResFile*)a5 = resFile;
            MpfsysResState* s = (MpfsysResState*)self;
            s->field_0x2E08 = a6;
            s->field_0x2E0C = (void*)a7;
        }
        self->func_804724DC();
        return 1;
    }
    return 0;
}

void mpfsys::UnkClass_80471EC8::func_80471FC8(void) { func_80473394(); }

void func_80471FCC__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, void* a4, void* a5, void* a6, u32 a7) {
    // Register this instance in the shared pointer slot, then, when the map
    // descriptor carries display lists, resolve the current view frame and
    // push the layer draws into the MPF display list. The retail leaves r3
    // untouched at the end (the caller re-reads the view frame itself), so
    // this returns void.
    lbl_eu_80665838 = self;
    MpfsysResState* s = (MpfsysResState*)self;
    MpfsysDescFCC* d = (MpfsysDescFCC*)a4;
    if (d->field_0x10 != 0) {
        MpfsysViewFrame* vf =
            (MpfsysViewFrame*)func_8049626C(s->field_0x2E08, s->field_0x2E0C);
        f32 scale = vf->field_0x1E0;
        func_80474064__Q26mpfsys17UnkClass_80471EC8Fv(
            (mpfsys::UnkClass_80471EC8*)a4, (u8*)vf + 0x9C, a5, a6, scale);
        mpfsys::MPFDrawDisplayList* inst = mpfsys::MPFDrawDisplayList::getInstance();
        func_80477F80__Q26mpfsys18MPFDrawDisplayListFv(
            inst, (u8*)a4 + d->field_0x10, s->field_0x2E10, a7);
    }
    s->field_0x2E10 = 0;
}

void mpfsys::UnkClass_80471EC8::func_80472064(void) {}

// Bit-array flag accessors. The retail symbols are annotated Fv but the
// functions take (self, index) - the index arrives in r4 (see callers in
// code_8047BB54.cpp). The literal mangled names are kept (MWCC emits
// `__`-containing identifiers verbatim), matching the func_8047230C
// precedent in this file.
void func_8047233C__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, int index) {
    // Clear one bit of the 256-bit flag array at 0x2D00.
    MpfsysFlagArray* flags = (MpfsysFlagArray*)self;
    if (index > 0xFF) {
        index = 0xFF;
    }
    flags->field_0x2D00[index >> 5] &= ~(1 << (index & 31));
}

void func_80472370__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, int index) {
    // Set one bit of the 256-bit flag array at 0x2D00.
    MpfsysFlagArray* flags = (MpfsysFlagArray*)self;
    if (index > 0xFF) {
        index = 0xFF;
    }
    flags->field_0x2D00[index >> 5] |= 1 << (index & 31);
}

void mpfsys::UnkClass_80471EC8::func_804723A4(void) {}

void mpfsys::UnkClass_80471EC8::func_80472864() { *(u8*)((u8*)this + 0x2E10) = 1; }

void func_80472870__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, s32 mode) {
    // Layer colour-mode setter. The retail symbol is annotated Fv but takes
    // (self, mode) in r3/r4 - mode is stored at 0x2E04 and selects one of
    // three fixed RGB colours written to the f32 triple at 0x2DE0.
    MpfsysLayerColorState* state = (MpfsysLayerColorState*)self;
    state->field_0x2E04 = mode;
    if (mode == 0) {
        f32 f = lbl_eu_8066A744;
        state->field_0x2DE0 = f;
        state->field_0x2DE4 = f;
        state->field_0x2DE8 = f;
    } else if (mode == 1) {
        f32 f1 = lbl_eu_8066A758;
        f32 f0 = lbl_eu_8066A75C;
        state->field_0x2DE0 = f1;
        state->field_0x2DE4 = f0;
        state->field_0x2DE8 = f1;
    } else if (mode == 2) {
        f32 f1 = lbl_eu_8066A74C;
        f32 f0 = lbl_eu_8066A75C;
        state->field_0x2DE0 = f1;
        state->field_0x2DE4 = f0;
        state->field_0x2DE8 = f1;
    } else {
        f32 f1 = lbl_eu_8066A770;
        f32 f0 = lbl_eu_8066A74C;
        state->field_0x2DE0 = f1;
        state->field_0x2DE4 = f0;
        state->field_0x2DE8 = f1;
    }
}

void mpfsys::UnkClass_80471EC8::func_804728E8() {}

#pragma push
#pragma auto_inline off
void mpfsys::UnkClass_80471EC8::func_80473394() {}
#pragma pop

void* mpfsys::UnkClass_80471EC8::func_804734F4(u8 layerIndex) {
    return (u8*)this + layerIndex * 0x1680;
}

extern "C" {
#pragma dont_inline on
void func_80473500__Q26mpfsys17UnkClass_80471EC8Fiif(int texIndex, int texMap, f32 texScale) {}
#pragma dont_inline off
void func_8047230C__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, f32 f1) {
    // Layer-scale setter. The retail symbol is annotated Fv but the function
    // actually takes (self, f32) - see the caller in code_8047BB54.cpp.
    // Stores the raw scale at 0x2DFC and the folded scale at 0x2E00.
    MpfsysLayerScaleState* state = (MpfsysLayerScaleState*)self;
    f32 limit = lbl_eu_8066A734;
    state->field_0x2DFC = f1;
    if (f1 < limit) {
        state->field_0x2E00 = f1 - lbl_eu_8066A738 * f1;
    } else {
        state->field_0x2E00 = f1 - lbl_eu_8066A73C;
    }
}
}

extern "C" {
void func_804737CC__Q26mpfsys17UnkClass_80471EC8Fif(int texIndex, f32 texScale) {
    if (lbl_eu_80665858 == texIndex) {
        return;
    }
    if (texIndex == -1) {
        return;
    }
    lbl_eu_80665858 = texIndex;
    func_80473500__Q26mpfsys17UnkClass_80471EC8Fiif(texIndex, 0, texScale);
}
}

void mpfsys::UnkClass_80471EC8::func_804737F0() {}

void mpfsys::UnkClass_80471EC8::func_80473984() {}

void mpfsys::UnkClass_80471EC8::func_804742BC() {
    if (lbl_eu_8066585A == 0) {
        return;
    }
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0xB, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    GXSetArray((GXAttr)0xD, lbl_eu_80665844, 0x8);
    lbl_eu_8066585A = 0;
    if (lbl_eu_80665854 != 1) {
        GXSetNumTevStages(1);
        lbl_eu_80665854 = 1;
    }
    if (lbl_eu_8066586C & 0x10) {
        GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)1, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
        lbl_eu_8066586C &= ~0x10;
    }
}

void mpfsys::UnkClass_80471EC8::func_804743E0() {
    // Fullscreen TEV stage-0 / vertex-desc setup (idempotent per state 1).
    if (lbl_eu_8066585A == 1) {
        return;
    }
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    lbl_eu_8066585A = 1;
    if (lbl_eu_80665854 != 1) {
        GXSetNumTevStages(1);
        lbl_eu_80665854 = 1;
    }
    if (!(lbl_eu_8066586C & 0x10)) {
        GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)0, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
        lbl_eu_8066586C |= 0x10;
    }
}

void mpfsys::UnkClass_80471EC8::func_804744EC() {}

void mpfsys::UnkClass_80471EC8::func_8047466C() {
    // Fullscreen TEV stage-0 / vertex-desc setup (idempotent per state 3).
    if (lbl_eu_8066585A == 3) {
        return;
    }
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x0, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    lbl_eu_8066585A = 3;
    if (lbl_eu_80665854 != 1) {
        GXSetNumTevStages(1);
        lbl_eu_80665854 = 1;
    }
    if (!(lbl_eu_8066586C & 0x10)) {
        GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)0, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
        lbl_eu_8066586C |= 0x10;
    }
}

void mpfsys::UnkClass_80471EC8::func_80474780() {}

void mpfsys::UnkClass_80471EC8::func_8047491C() {}

void mpfsys::UnkClass_80471EC8::func_80474A40() {
    // One-shot alpha/blend setup: when the 0x2 flag is clear, push the
    // transparent-overlay blend state and set the flag.
    if (!(lbl_eu_8066586C & 0x2)) {
        GXSetAlphaCompare(GX_GEQUAL, 0x1, GX_AOP_AND, GX_LEQUAL, 0xff);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        lbl_eu_8066586C |= 0x2;
    }
}

void mpfsys::UnkClass_80471EC8::func_80474AA0() {
    // One-shot alpha/blend teardown: when the 0x2 flag is set, push the
    // opaque blend state and clear the flag.
    if (lbl_eu_8066586C & 0x2) {
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_LEQUAL, 0xff);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
        lbl_eu_8066586C &= ~0x2;
    }
}

void mpfsys::UnkClass_80471EC8::func_80474B00() {}

void mpfsys::UnkClass_80471EC8::func_80474CC4() {
    // Push the shared constant key color (RGBA latch) as TEV K-color 0.
    GXColor color;
    *(u32*)&color = lbl_eu_8066A7A0;
    GXSetTevKColor((GXTevKColorID)0, color);
}

void func_80474CF4__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self) {
    // Push the channel-material colour for table entry `self` of the shared
    // RGB565 colour table. The retail symbol is annotated Fv: r3 carries the
    // table index and is never dereferenced, so the literal mangled name is
    // kept (func_8047233C precedent) - a const member would mangle to
    // ...CFv and never resolve against the retail ...Fv symbol.
    u16 rgb565 = lbl_eu_80665850[(u32)self];
    GXColor color;
    *(u32*)&color = lbl_eu_8066A7D0;
    color.r = (u8)((rgb565 >> 8) & 0x1F);
    color.g = (u8)((rgb565 >> 3) & 0x3F);
    color.b = (u8)((rgb565 & 0x1F) << 3);
    GXSetChanMatColor(GX_COLOR0, color);
}

void mpfsys::UnkClass_80471EC8::func_80474D50() {
    // Same RGB565->GXColor expansion as func_80474CF4 (alpha byte from
    // lbl_eu_8066A7D4), pushed as TEV register colour 0.
    u16 rgb565 = lbl_eu_80665850[(u32)this];
    GXColor color;
    *(u32*)&color = lbl_eu_8066A7D4;
    color.r = (u8)((rgb565 >> 8) & 0x1F);
    color.g = (u8)((rgb565 >> 3) & 0x3F);
    color.b = (u8)((rgb565 & 0x1F) << 3);
    GXSetTevColor(GX_TEVREG0, color);
}

// TEV key-color alpha setter. The retail symbol is annotated Fv but takes an
// f32 scale in f1 (see the callers in MPFDrawBillLayTex.cpp), so the literal
// mangled name is kept. Scales lbl_eu_8066A7D8 by the argument, stores the
// converted byte as the alpha of the shared key color (lbl_eu_80663858),
// then pushes that color as TEV K-color 0.
void func_80474DAC__Q26mpfsys17UnkClass_80471EC8Fv(f32 value) {
    lbl_eu_80663858.a = (u8)(s32)(lbl_eu_8066A7D8 * value);
    GXColor color = lbl_eu_80663858;
    GXSetTevKColor(GX_KCOLOR0, color);
}

void mpfsys::UnkClass_80471EC8::func_80474DF8() {
    // Fog-index setter. The retail symbol is annotated Fv (no args), so this
    // is written as a no-arg member: r3 (this) itself carries the new fog
    // index value and is compared/stored/loaded without ever being
    // dereferenced.
    if ((s32)lbl_eu_8066585C == (s32)(u32)this) {
        return;
    }
    lbl_eu_8066585C = (s16)(u32)this;
    if ((u32)this != 0) {
        nw4r::g3d::G3DState::LoadFog((s32)(u32)this - 1);
    } else {
        nw4r::g3d::G3DState::LoadFog(0x40);
    }
}

void mpfsys::UnkClass_80471EC8::func_80474E24(void) {
    // One-shot ambient-color push: when the 0x20 "ambient set" flag is set,
    // clear it and push the shared ambient color (lbl_eu_80665878) for
    // channel 0.
    u32 flags = lbl_eu_8066586C;
    if (flags & 0x20) {
        lbl_eu_8066586C = flags & ~0x20;
        GXColor color;
        *(u32*)&color = lbl_eu_80665878;
        GXSetChanAmbColor(GX_COLOR0, color);
    }
}

void mpfsys::UnkClass_80471EC8::func_80474E68(void) {
    // One-shot ambient-color push: when the 0x20 flag is clear, push the
    // shared ambient color (lbl_eu_8066587C) for channel 0, then set the flag.
    if (!(lbl_eu_8066586C & 0x20)) {
        GXColor color;
        *(u32*)&color = lbl_eu_8066587C;
        GXSetChanAmbColor(GX_COLOR0, color);
        lbl_eu_8066586C |= 0x20;
    }
}

void mpfsys::UnkClass_80471EC8::func_80474EB0() {
    // Install the default TEV swap-mode tables: identity (R,G,B,A) for all
    // four swap selectors.
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void mpfsys::UnkClass_80471EC8::func_80474F2C() {
    // One-shot Z-mode disable: when the 0x40 "Z configured" flag is set,
    // clear it and push GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE).
    if (!(lbl_eu_8066586C & 0x40)) {
        return;
    }
    lbl_eu_8066586C &= ~0x40;
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}

void mpfsys::UnkClass_80471EC8::func_80474F54() {
    // One-shot Z-mode enable: when the 0x40 flag is clear, set it and push
    // GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE).
    if (lbl_eu_8066586C & 0x40) {
        return;
    }
    lbl_eu_8066586C |= 0x40;
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static initializer referenced by .ctors section.
// No runtime initialisation required; returns immediately (blr).
extern "C" void sinit_80474F7C(void)
{
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
extern "C" u32 lbl_eu_80523D80;  // .rodata: string
extern "C" u32 lbl_eu_8056DB90;  // .data: object

// [.sdata] 0x80663848-0x80663868 (0x20 = 32B)
extern "C" u32 lbl_eu_80663848[2] = { (u32)&lbl_eu_80523D80, (u32)&lbl_eu_8056DB90 };
extern "C" u32 lbl_eu_80663850 = 0x3B03126F;
extern "C" u32 lbl_eu_80663854 = 0x3BA3D70A;
GXColor lbl_eu_80663858 = { 0xFF, 0xFF, 0xFF, 0xFF };
extern "C" u32 lbl_eu_8066385C = 0x72656600;
extern "C" u32 lbl_eu_80663860 = 0x72656600;
extern "C" u32 lbl_eu_80663864 = 0x72656600;

// [.bss] 0x80658458-0x80658488 (0x30 = 48B) zero-fill
u8 lbl_eu_80658458[48];

// [.sbss] 0x80665860-0x80665888 (0x28 = 40B) zero-fill
u32 lbl_eu_80665860;
u32 lbl_eu_80665864;
u32 lbl_eu_80665868;
u32 lbl_eu_8066586C;
u32 lbl_eu_80665870;
u32 lbl_eu_80665874;
u32 lbl_eu_80665878;
u32 lbl_eu_8066587C;
u32 lbl_eu_80665880;
u32 lbl_eu_80665884;

DECOMP_FORCEACTIVE(UnkClass_80471EC8_cpp, lbl_eu_80663848);

// Auto-scaffolded catalog TU for monolib/src/mpfsys/UnkClass_80471EC8
// Replace stubs with high-level C/C++ during decomp.

#include "monolib/mpfsys/UnkClass_80471EC8.hpp"
#include "monolib/mpfsys/MPFDrawDisplayList.hpp"
#include "monolib/core/code_804E36DC.hpp" // declares func_80496288 (C ABI)
#include <harness_catalog.h>
#include <PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h>

#include <revolution/GX.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/g3d_state.h>
#include <nw4r/g3d/res/g3d_resfile.h>
#include <string.h>
#include "monolib/math/CVec3.hpp"

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
extern const f32 lbl_eu_8066A740; // 255.0f colour scale (func_804723A4)
extern const f32 lbl_eu_8066A744;
extern const f32 lbl_eu_8066A74C;
extern const f32 lbl_eu_8066A758;
extern const f32 lbl_eu_8066A75C;
extern const f32 lbl_eu_8066A770;
extern const f32 lbl_eu_8066A750; // sprite update constant (func_80473394)
extern const f32 lbl_eu_8066A748; // sprite velocity random scale (func_804724DC)
extern const f32 lbl_eu_8066A754; // layer fill constant (func_804724DC)
extern const f32 lbl_eu_8066A760; // layer anim constant (func_804724DC)
extern const f32 lbl_eu_8066A764; // layer anim constant (func_804724DC)
// Retail .sdata2 magic double used by MWCC's (f32)(u32) conversions (xoris
// form). Defined here with the exact retail label name/value (2^52 + 2^31) so
// the conversion pool emits an sda21 reference to the retail slot instead of
// a fresh local pool entry (CfResReloadImpl / CSuddenCommu idiom). Non-const
// so MWCC keeps the named symbol instead of folding it into the pool.
double lbl_eu_8066A768 = 4503601774854144.0;
extern const f32 lbl_eu_8066A778; // sprite update constant (func_80473394)
extern const f32 lbl_eu_8066A794; // 1000.0f rand() % 1000 divisor
extern const f32 lbl_eu_8066A798; // 20.0f rand() % 20 divisor
// Texture-slot table (count/offset pair per texture index) and texture data
// base for func_804737F0; same SDA-relative globals as the MPF draw units.
extern s32* lbl_eu_80665868;
extern u8* lbl_eu_80665840;
// Embedded-subobject pointer cache (written by func_8047BE8C in
// code_8047BB54.cpp); func_80471FCC registers `this` here.
extern void* lbl_eu_80665838;
// Panic file-name / message strings (retail rodata).
extern const char lbl_eu_8052637C[];
extern const char lbl_eu_80526354[];

// Sprite-animation matrix base pointer (points into the layer state); the
// retail stride is 0xC bytes per entry. Written by func_80474064.
extern u8* lbl_eu_8066584C;
// Sprite-matrix chain base (global 4x3 matrix).
extern Mtx lbl_eu_80658458;
// Sprite phase table mask / 2^-5 scale (frac = ((a+b) & 0x1F) * 0.03125).
extern u32 lbl_eu_8066A72C;
extern f32 lbl_eu_8066A730;
// Shared float constants used by the sprite matrix / camera math.
extern const f32 lbl_eu_8066A7A4; // 0.0f
// 2^52 double used by the (u32)->f32 conversion (MWCC pool magic constant).
extern const f64 lbl_eu_8066A7C8;
extern const f32 lbl_eu_8066A7AC;
extern const f32 lbl_eu_8066A7B0; // 0.5f
// Camera-math globals written by func_80474064.
extern void* lbl_eu_80665870;
extern void* lbl_eu_80665874;
extern f32 lbl_eu_80665880;
extern f32 lbl_eu_80665884;
extern u32* lbl_eu_80665864;
// ResFile bound by func_80473500 (retail SDA global holding the file data
// pointer).
extern nw4r::g3d::ResFileData* lbl_eu_80665848;
// Panic strings for the texture-load path (retail rodata).
extern const char lbl_eu_8052CF10[];
extern const char lbl_eu_8052CEF4[];
extern const char lbl_eu_80661E98[];
extern const char lbl_eu_8066385C[];
extern const char lbl_eu_8052CF6C[];
extern const char lbl_eu_8052CF50[];
extern const char lbl_eu_80661EA8[];
extern const char lbl_eu_80663864[];
extern const char lbl_eu_8052CF3C[];
extern const char lbl_eu_8052CF20[];
extern const char lbl_eu_80663860[];

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

// 4x3 matrix (12 f32s). Struct copy compiles to lwz/stw pairs like the
// retail, keeping all 12 values live in GPRs.
struct MpfsysMtx4x3 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xC;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2C;
};

// Sprite pair entry (0xB4 stride, two matrices at +0x54 / +0x84) used by
// func_80474B00 when tevStage != 0.
struct MpfsysSpritePair {
    u8 field_0x0[0x54];
    Mtx field_0x54;
    Mtx field_0x84;
};

// Camera-math region at lbl_eu_80658410: translation vec (+0x0), direction
// vec (+0xC), rotation matrix (+0x18), inverse matrix (+0x48).
struct MpfsysCamMatrixRegion {
    f32 field_0x0[3];
    f32 field_0xC[3];
    Mtx field_0x18;
    Mtx field_0x48;
};
extern MpfsysCamMatrixRegion lbl_eu_80658410;

// Sprite-table entry (0xB4 stride, 0x40 entries at this+0x0): three leading
// position/offset Vec3s, a zeroed second triplet, the velocity Vec3 (+0x48)
// and the two quaternion matrices (+0x54 / +0x84) driven by func_804728E8.
struct MpfsysSpriteEntry {
    ml::CVec3 field_0x0;
    ml::CVec3 field_0xC;
    ml::CVec3 field_0x18;
    ml::CVec3 field_0x24;
    ml::CVec3 field_0x30;
    ml::CVec3 field_0x3C;
    ml::CVec3 field_0x48;
    Mtx field_0x54;
    Mtx field_0x84;
};

// Tail state of UnkClass_80471EC8 (0x2D00..0x2E14): the 256-bit flag array,
// the 0xC0-byte fill block, the colour/animation floats and the mode fields.
struct MpfsysLayerTailState {
    u8 field_0x0[0x2D00];
    u32 field_0x2D00[8];
    f32 field_0x2D20[0x30];
    f32 field_0x2DE0;
    f32 field_0x2DE4;
    f32 field_0x2DE8;
    f32 field_0x2DEC[4];
    f32 field_0x2DFC;
    f32 field_0x2E00;
    s32 field_0x2E04;
    u8 field_0x2E08[8];
    u8 field_0x2E10;
    u8 field_0x2E11;
    u16 field_0x2E12;
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

void mpfsys::UnkClass_80471EC8::func_804724DC(void) {
    // Initialise the 0x40 sprite entries (0xB4 stride): the three leading
    // Vec3s carry constant Y offsets (0, 1, 2), the second triplet is zeroed,
    // the velocity is a random horizontal vector (normalised or defaulted),
    // and both quaternion matrices are identity. Then fill the layer tail
    // state and clear the flag array.
    //
    // The (f32)(u32) casts use MWCC's pooled double-bias idiom: storing
    // 0x43300000 | (v ^ 0x80000000) and subtracting the 2^52 + 2^31 magic
    // double (lbl_eu_8066A768) yields exactly (float)v. The conversion is
    // loop-invariant (offsets 0/1/2) so the constant materialisation is
    // hoisted; the stack round-trip stays in the loop because of the rand
    // calls.
    u32 k0 = 0;
    u32 k1 = 1;
    u32 k2 = 2;
    for (u32 i = k0; i < 0x40; i++) {
        MpfsysSpriteEntry* e = (MpfsysSpriteEntry*)((u8*)this + i * 0xB4);
        e->field_0x0.set(lbl_eu_8066A744, (f32)(u32)k0, lbl_eu_8066A744);
        e->field_0x24 = ml::CVec3::zero;
        e->field_0xC.set(lbl_eu_8066A744, (f32)(u32)k1, lbl_eu_8066A744);
        e->field_0x30 = ml::CVec3::zero;
        e->field_0x18.set(lbl_eu_8066A744, (f32)(u32)k2, lbl_eu_8066A744);
        e->field_0x3C = ml::CVec3::zero;
        PSMTXIdentity(e->field_0x54);
        PSMTXIdentity(e->field_0x84);
        f32 vz = lbl_eu_8066A748 * (f32)(u32)(rand() % 100) - lbl_eu_8066A74C;
        f32 vx = lbl_eu_8066A748 * (f32)(u32)(rand() % 100) - lbl_eu_8066A74C;
        e->field_0x48.x = vx;
        e->field_0x48.y = lbl_eu_8066A744;
        e->field_0x48.z = vz;
        if (vx == lbl_eu_8066A744 && e->field_0x48.y == lbl_eu_8066A744 &&
            e->field_0x48.z == lbl_eu_8066A744) {
            e->field_0x48.set(lbl_eu_8066A744, lbl_eu_8066A744, lbl_eu_8066A750);
        } else {
            f32 mag2 = e->field_0x48.y * e->field_0x48.y +
                       e->field_0x48.x * e->field_0x48.x +
                       e->field_0x48.z * e->field_0x48.z;
            if (mag2 == lbl_eu_8066A744) {
                e->field_0x48 = ml::CVec3::zero;
            } else {
                PSVECNormalize(e->field_0x48, e->field_0x48);
            }
        }
    }
    MpfsysLayerTailState* tail = (MpfsysLayerTailState*)this;
    f32 f5 = lbl_eu_8066A754;
    for (int j = 0; j < 0x30; j++) {
        tail->field_0x2D20[j] = f5;
    }
    f32 f4 = lbl_eu_8066A758;
    f32 f3 = lbl_eu_8066A75C;
    f32 f2 = lbl_eu_8066A760;
    f32 f1 = lbl_eu_8066A750;
    f32 f0 = lbl_eu_8066A764;
    tail->field_0x2DE0 = f4;
    tail->field_0x2DE4 = f3;
    tail->field_0x2DE8 = f2;
    tail->field_0x2DEC[0] = f1;
    tail->field_0x2DEC[1] = f1;
    tail->field_0x2DEC[2] = f0;
    tail->field_0x2DEC[3] = f0;
    tail->field_0x2E12 = 0;
    tail->field_0x2E04 = 10;
    tail->field_0x2E10 = 0;
    memset(tail->field_0x2D00, 0, 0x20);
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

// Camera colour chain for func_804723A4: this->0x2E08 is the camera, +0x64
// dereferences to an object whose +0x8 points at two 4-float colour quads at
// 0x11A8 and 0x11C8.
struct MpfsysCamColorNode {
    u8 field_0x0[0x64];
    void* field_0x64;
};
struct MpfsysCamColorMid {
    u8 field_0x0[0x8];
    struct MpfsysCamColorData* field_0x8;
};
struct MpfsysCamColorData {
    u8 field_0x0[0x11A8];
    f32 field_0x11A8[4];
    u8 field_0x11B8[0x10];
    f32 field_0x11C8[4];
};

void func_804723A4__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, GXColor* out1, GXColor* out2) {
    // Convert the camera's two float colour quads (0.0..1.0) to RGBA bytes
    // (x 255) and force the alpha channel opaque. The retail symbol is
    // Fv-mangled but receives the two output pointers in r4/r5.
    MpfsysResState* s = (MpfsysResState*)self;
    MpfsysCamColorNode* node = (MpfsysCamColorNode*)s->field_0x2E08;
    MpfsysCamColorMid* mid = (MpfsysCamColorMid*)node->field_0x64;
    MpfsysCamColorData* data = mid->field_0x8;
    f32 scale = lbl_eu_8066A740;
    GXColor c1;
    c1.r = (u8)(data->field_0x11A8[0] * scale);
    c1.g = (u8)(data->field_0x11A8[1] * scale);
    c1.b = (u8)(data->field_0x11A8[2] * scale);
    c1.a = (u8)(data->field_0x11A8[3] * scale);
    *out1 = c1;
    out1->a = 0xff;
    GXColor c2;
    c2.r = (u8)(data->field_0x11C8[0] * scale);
    c2.g = (u8)(data->field_0x11C8[1] * scale);
    c2.b = (u8)(data->field_0x11C8[2] * scale);
    c2.a = (u8)(data->field_0x11C8[3] * scale);
    *out2 = c2;
    out2->a = 0xff;
}

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

extern "C" {
#pragma dont_inline on
void func_804728E8__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* self, u8* ptr, f32 f1) {}
#pragma dont_inline off
}

// Layer-sprite animation state for func_80473394: the fall step (0x2DEC),
// the 0x2DF0..0x2DF8 walk state, the 0x2DE0 colour triple, the camera at
// 0x2E08, and the 0xB4-stride per-layer sprite table starting at 0x1680.
struct MpfsysSpriteAnimState {
    u8 field_0x0[0x1680];
    u8 field_0x1680[0x1760]; // 0x1680..0x2DE0
    f32 field_0x2DE0;
    f32 field_0x2DE4;
    f32 field_0x2DE8;
    f32 field_0x2DEC;
    f32 field_0x2DF0;
    f32 field_0x2DF4;
    f32 field_0x2DF8;
    u8 field_0x2DFC[0xC];    // 0x2DFC..0x2E08
    void* field_0x2E08;      // camera (func_80496288 arg)
};

#pragma push
#pragma auto_inline off
void mpfsys::UnkClass_80471EC8::func_80473394() {
    // Advance the layer-sprite animation: fold the camera step into 0x2DEC /
    // 0x2DF8; when the counter drops to (or below) zero re-randomize it and
    // the 0x2DE8 colour via rand(), then refresh every sprite entry (0xB4
    // stride) in both halves of the table.
    MpfsysSpriteAnimState* s = (MpfsysSpriteAnimState*)this;
    f32 step = s->field_0x2DF0 * func_80496288(s->field_0x2E08);
    s->field_0x2DEC = step;
    f32 counter = s->field_0x2DF8 - step;
    s->field_0x2DF8 = counter;
    if (counter <= lbl_eu_8066A744) {
        s->field_0x2DF8 = s->field_0x2DF4 * ((float)(rand() % 1000) / lbl_eu_8066A794);
        s->field_0x2DE8 = s->field_0x2DE0 * ((float)(rand() % 20) / lbl_eu_8066A798) + s->field_0x2DE4;
    }
    u8* p = (u8*)this;
    for (int i = 0; i < 0x20; i++, p += 0xB4) {
        func_804728E8__Q26mpfsys17UnkClass_80471EC8Fv(this, p, lbl_eu_8066A750);
    }
    p = (u8*)this + 0x1680;
    for (int i = 0x20; i < 0x40; i++, p += 0xB4) {
        func_804728E8__Q26mpfsys17UnkClass_80471EC8Fv(this, p, lbl_eu_8066A778);
    }
}
#pragma pop

void* mpfsys::UnkClass_80471EC8::func_804734F4(u8 layerIndex) {
    return (u8*)this + layerIndex * 0x1680;
}

extern "C" {
#pragma dont_inline on
void func_80473500__Q26mpfsys17UnkClass_80471EC8Fiif(int texIndex, int texMap, f32 texScale) {
    // Bind the texture selected by texIndex from the bound ResFile and load
    // it into the given tex map slot. CI-format textures additionally bind
    // the palette (looked up by the texture's name) into a TLUT and use the
    // CI tex-object initializer.
    nw4r::g3d::ResFile resFile(lbl_eu_80665848);
    nw4r::g3d::ResTex resTex = resFile.GetResTex(texIndex);
    if (!resTex.IsValid()) {
        nw4r::db::Panic(lbl_eu_8052CF10, 0x26, lbl_eu_8052CEF4,
                        lbl_eu_80661E98, lbl_eu_8066385C);
    }
    if (resTex.IsCIFmt()) {
        if (!resTex.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF10, 0x26, lbl_eu_8052CEF4,
                            lbl_eu_80661E98, lbl_eu_8066385C);
        }
        const char* plttName = resTex.ofs_to_ptr<const char>(resTex.ref().name);
        nw4r::g3d::ResFile parent = resTex.GetParent();
        nw4r::g3d::ResPltt pltt = parent.GetResPltt(plttName);
        void* texData;
        u16 texWidth;
        u16 texHeight;
        GXCITexFmt ciFmt;
        f32 minLod;
        f32 maxLod;
        GXBool mipMap;
        resTex.GetTexObjCIParam(&texData, &texWidth, &texHeight, &ciFmt,
                                &minLod, &maxLod, &mipMap);
        u32 tlutParam = 0;
        if (ciFmt == GX_TF_C14X2) {
            tlutParam = 0x10;
        }
        if (!pltt.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2A, lbl_eu_8052CF50,
                            lbl_eu_80661EA8, lbl_eu_80663864);
        }
        u16 numEntries = pltt.ref().numEntries;
        if (!pltt.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2A, lbl_eu_8052CF50,
                            lbl_eu_80661EA8, lbl_eu_80663864);
        }
        GXTlutFmt tlutFmt = pltt.ref().fmt;
        if (!pltt.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF3C, 0x2A, lbl_eu_8052CF20,
                            lbl_eu_80661EA8, lbl_eu_80663860);
        }
        GXTlutObj tlutObj;
        GXInitTlutObj(&tlutObj, pltt.ofs_to_ptr<void>(pltt.ref().toPlttData),
                      tlutFmt, numEntries);
        GXLoadTlut(&tlutObj, (u32)tlutParam);
        GXTexObj texObj;
        GXInitTexObjCI(&texObj, texData, texWidth, texHeight, (GXTexFmt)ciFmt,
                       GX_REPEAT, GX_REPEAT, mipMap, (u32)tlutParam);
        if (mipMap) {
            GXInitTexObjLOD(&texObj, GX_LIN_MIP_LIN, GX_LINEAR, minLod,
                            maxLod, texScale, GX_FALSE, GX_FALSE, GX_ANISO_1);
        } else {
            GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, lbl_eu_8066A7A4,
                            lbl_eu_8066A7A4, lbl_eu_8066A7A4, GX_FALSE,
                            GX_FALSE, GX_ANISO_1);
        }
        GXLoadTexObj(&texObj, (GXTexMapID)texMap);
    } else {
        void* texData;
        u16 texWidth;
        u16 texHeight;
        GXTexFmt texFmt;
        f32 minLod;
        f32 maxLod;
        GXBool mipMap;
        resTex.GetTexObjParam(&texData, &texWidth, &texHeight, &texFmt,
                              &minLod, &maxLod, &mipMap);
        GXTexObj texObj;
        GXInitTexObj(&texObj, texData, texWidth, texHeight, texFmt,
                     GX_REPEAT, GX_REPEAT, mipMap);
        if (mipMap) {
            GXInitTexObjLOD(&texObj, GX_LIN_MIP_LIN, GX_LINEAR, minLod,
                            maxLod, texScale, GX_FALSE, GX_FALSE, GX_ANISO_1);
        } else {
            GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, lbl_eu_8066A7A4,
                            lbl_eu_8066A7A4, lbl_eu_8066A7A4, GX_FALSE,
                            GX_FALSE, GX_ANISO_1);
        }
        GXLoadTexObj(&texObj, (GXTexMapID)texMap);
    }
}
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

// Texture slot entry: one u16 texture id plus padding (4-byte stride).
struct MpfsysTexSlotEntry {
    u16 texIndex;
    u16 field_0x2;
};

void func_804737F0__Q26mpfsys17UnkClass_80471EC8Fv(s16 texIndex, f32 texScale) {
    // Multi-texture billboard setup (retail symbol is Fv-mangled but takes
    // (texIndex, texScale) - see the callers in MPFDrawBillLayTex.cpp).
    // Resets the cached-texture marker, binds the texture slot selected by
    // texIndex (count/offset pair in lbl_eu_80665868), pushes the state-6
    // TEV/vertex setup once, then feeds every texture id of the slot.
    s32* table = lbl_eu_80665868;
    s32* entry = table + texIndex * 2;
    s32 offset = entry[1];
    u8* base = lbl_eu_80665840;
    lbl_eu_80665858 = -1;
    u8* slot = base + offset;
    s32 count = *(s32*)((u8*)table + texIndex * 8);
    if (lbl_eu_80665854 != 1) {
        GXSetNumTevStages(1);
        lbl_eu_80665854 = 1;
    }
    if (lbl_eu_8066585A != 6) {
        GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
        GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
        GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
        GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
        GXClearVtxDesc();
        GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x1);
        GXSetVtxDesc((GXAttr)0xB, (GXAttrType)0x1);
        GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
        GXSetArray((GXAttr)0xD, lbl_eu_80665844, 0x8);
        lbl_eu_8066585A = 6;
        if (lbl_eu_8066586C & 0x10) {
            GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)1, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
            lbl_eu_8066586C &= ~0x10;
        }
    }
    MpfsysTexSlotEntry* e = (MpfsysTexSlotEntry*)slot;
    for (int i = 0; i < count; e++, i++) {
        func_80473500__Q26mpfsys17UnkClass_80471EC8Fiif(e->texIndex, i, texScale);
    }
}

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

void mpfsys::UnkClass_80471EC8::func_804744EC() {
    // Two-stage TEV setup (stages 0 and 1), idempotent per state 2.
    if (lbl_eu_8066585A == 2) {
        return;
    }
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)2);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)2);
    GXSetTevColorIn((GXTevStageID)1, (GXTevColorArg)0xF, (GXTevColorArg)0x2, (GXTevColorArg)0x4, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)1, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)1, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x2);
    GXSetTevAlphaOp((GXTevStageID)1, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xB, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    lbl_eu_8066585A = 2;
    if (lbl_eu_80665854 != 2) {
        GXSetNumTevStages(2);
        lbl_eu_80665854 = 2;
    }
    if (lbl_eu_8066586C & 0x10) {
        GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)1, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
        lbl_eu_8066586C &= ~0x10;
    }
}

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

void mpfsys::UnkClass_80471EC8::func_80474780() {
    // Two-stage TEV setup (stages 0 and 1) with a stage-1 tex-order,
    // idempotent per state 4.
    if (lbl_eu_8066585A == 4) {
        return;
    }
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)2);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)2);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn((GXTevStageID)1, (GXTevColorArg)0xF, (GXTevColorArg)0x2, (GXTevColorArg)0x4, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)1, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)1, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x2);
    GXSetTevAlphaOp((GXTevStageID)1, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x0, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xB, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    lbl_eu_8066585A = 4;
    if (lbl_eu_80665854 != 2) {
        GXSetNumTevStages(2);
        lbl_eu_80665854 = 2;
    }
    if (lbl_eu_8066586C & 0x10) {
        GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)1, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
        lbl_eu_8066586C &= ~0x10;
    }
}

void mpfsys::UnkClass_80471EC8::func_8047491C() {
    // Fullscreen TEV/vertex setup, idempotent per state 5. This state clears
    // the vertex descriptors before the TEV calls (the other state setters
    // do the reverse order).
    if (lbl_eu_8066585A == 5) {
        return;
    }
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xB, (GXAttrType)0x2);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetArray((GXAttr)0xD, lbl_eu_80665844, 0x8);
    lbl_eu_8066585A = 5;
    if (lbl_eu_80665854 != 1) {
        GXSetNumTevStages(1);
        lbl_eu_80665854 = 1;
    }
    if (lbl_eu_8066586C & 0x10) {
        GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)1, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
        lbl_eu_8066586C &= ~0x10;
    }
}

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

void func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(mpfsys::UnkClass_80471EC8* group, s32 tevStage, f32 scale, u8 mtxSelect) {
    // Sprite matrix setup: build a diagonal-scale + translation matrix from
    // the 0xC-stride base entry (diagonal folds a (a+b)&0x1F phase when the
    // scale is non-zero), concat the global matrix and load it as position
    // matrix 3; then, when tevStage != 0, reload the two sprite-pair
    // matrices (ids 6 and 9).
    f32 offDiag = lbl_eu_8066A7A4;
    f32* base = (f32*)((u8*)lbl_eu_8066584C + (u32)group * 0xc);
    union {
        f64 d;
        u32 u[2];
    } conv;
    Mtx m;
    Mtx out;
    if (scale != offDiag) {
        // ((u32)->f32 via the 2^52 bias trick, referencing the named pool
        // constant so the lfd reloc matches retail).
        conv.u[0] = 0x43300000;
        conv.u[1] = ((u32)group + (u32)tevStage) & lbl_eu_8066A72C;
        f32 frac = (f32)(conv.d - lbl_eu_8066A7C8);
        frac = frac * lbl_eu_8066A730;
        f32 diag = lbl_eu_8066A7B0 + scale * frac;
        m[0][0] = diag;
        m[0][1] = offDiag;
        m[0][2] = offDiag;
        m[0][3] = base[0];
        m[1][0] = offDiag;
        m[1][1] = diag;
        m[1][2] = offDiag;
        m[1][3] = base[1];
        m[2][0] = offDiag;
        m[2][1] = offDiag;
        m[2][2] = diag;
        m[2][3] = base[2];
    } else {
        f32 diag = lbl_eu_8066A7B0;
        m[0][0] = diag;
        m[0][1] = offDiag;
        m[0][2] = offDiag;
        m[0][3] = base[0];
        m[1][0] = offDiag;
        m[1][1] = diag;
        m[1][2] = offDiag;
        m[1][3] = base[1];
        m[2][0] = offDiag;
        m[2][1] = offDiag;
        m[2][2] = diag;
        m[2][3] = base[2];
    }
    PSMTXConcat(lbl_eu_80658458, m, out);
    GXLoadPosMtxImm(out, 3);
    if (tevStage != 0) {
        mpfsys::UnkClass_80471EC8* inst =
            (mpfsys::UnkClass_80471EC8*)lbl_eu_80665838;
        MpfsysSpritePair* sprite = (MpfsysSpritePair*)((u8*)inst->func_804734F4(mtxSelect) +
            (((u32)group + (u32)tevStage) & lbl_eu_8066A72C) * 0xb4);
        PSMTXConcat(m, sprite->field_0x84, out);
        PSMTXConcat(lbl_eu_80658458, out, out);
        GXLoadPosMtxImm(out, 6);
        PSMTXConcat(m, sprite->field_0x54, out);
        PSMTXConcat(lbl_eu_80658458, out, out);
        GXLoadPosMtxImm(out, 9);
    }
}

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

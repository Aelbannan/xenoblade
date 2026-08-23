// Auto-scaffolded catalog TU for monolib/src/mpfsys/UnkClass_80471EC8
// Replace stubs with high-level C/C++ during decomp.

#include "monolib/mpfsys/UnkClass_80471EC8.hpp"
#include "monolib/mpfsys/MPFDrawDisplayList.hpp"
#include <harness_catalog.h>

#include <revolution/GX.h>
#include <revolution/mtx/mtx.h>
#include <revolution/mtx/vec.h>
#include <revolution/os/OSCache.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/g3d_state.h>
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_restex.h>
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
extern const f32 lbl_eu_8066A740;
extern u32 lbl_eu_8066A7A0;
extern u32 lbl_eu_80665868;
extern u16* lbl_eu_80665850;
extern const GXColor lbl_eu_8066A7D0;
extern const GXColor lbl_eu_8066A7D4;
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
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// Per-view render state shared by the layer draw path. lbl_eu_80665848 holds
// the active map ResFile handle table (indexed by texture slot),
// lbl_eu_8066584C the per-view display-list offset table (12-byte entries),
// and lbl_eu_80658410 the scratch camera block used by func_80474064.
extern void* lbl_eu_80665840;
extern u32 lbl_eu_80665860;
extern u32 lbl_eu_80665870;
extern u32 lbl_eu_80665874;
extern f32 lbl_eu_80665880;
extern u32 lbl_eu_80665884;
extern void* lbl_eu_80665848[];
struct MpfsysViewEntry {
    u8* base;     // +0x00 display-list base for this view slot
    u8 pad[8];
};
extern MpfsysViewEntry lbl_eu_8066584C[];
extern u8 lbl_eu_80658410[];
extern Mtx lbl_eu_80658458; // identity matrix (defined at the bottom of this TU)

// sdata2 constants used by the five functions below.
extern const f32 lbl_eu_8066A210;
extern const f32 lbl_eu_8066A7A4;
extern const f32 lbl_eu_8066A7B0;
extern const f32 lbl_eu_8066A748;
extern const f32 lbl_eu_8066A750;
extern const f32 lbl_eu_8066A754;
extern const f32 lbl_eu_8066A760;
extern const f32 lbl_eu_8066A764;
extern const u32 lbl_eu_8066A72C; // index mask
extern const f32 lbl_eu_8066A730;
extern const f32 lbl_eu_8066A7AC;
extern const f32 lbl_eu_8066A7B4;
extern const f32 lbl_eu_8066A7B8;
extern const f32 lbl_eu_8066A7BC;
extern const f32 lbl_eu_8066A7C0;
extern const f32 lbl_eu_8066A774;
extern const f32 lbl_eu_8066A778;
extern const f32 lbl_eu_8066A77C;
extern const f32 lbl_eu_8066A780;
extern const f32 lbl_eu_8066A784;
extern const f32 lbl_eu_8066A788;
extern const f32 lbl_eu_8066A78C;
extern const f32 lbl_eu_8066A790;
extern u32 lbl_eu_8066A7A8;
// Random-scale float constants (defined at the bottom of this TU).
extern f32 lbl_eu_80663850;
extern f32 lbl_eu_80663854;
extern const f64 lbl_eu_8066A768; // 2^52 (unsigned int -> double conversion)
extern const f64 lbl_eu_8066A7C8; // 2^32 (unsigned int -> double conversion)

// nw4r assertion strings for func_80473500.
extern const char lbl_eu_8052CF10[];
extern const char lbl_eu_8052CEF4[];
extern const char lbl_eu_8052CF6C[];
extern const char lbl_eu_8052CF50[];
extern const char lbl_eu_8052CF3C[];
extern const char lbl_eu_8052CF20[];
extern const char lbl_eu_80661E98[];
extern const char lbl_eu_80661EA8[];
extern u32 lbl_eu_8066385C;
extern u32 lbl_eu_80663860;
extern u32 lbl_eu_80663864;
extern u32 lbl_eu_80665884;

// Retail-named imports. These resolve against retail symbols (func_8049626C is
// defined in scn/CScn.cpp, func_80477F80 in MPFDrawDisplayList.cpp,
// func_80474064 is still retail at 0x80474064); extern "C" keeps the call-site
// relocs verbatim (MWCC would otherwise re-mangle the `__` names with the
// parameter encoding).
extern "C" void* func_8049626C(void* camera, void* view);
// Current-scene getter (scn TU) and its env-light controller push.
extern "C" void* func_8049698C();
extern "C" void func_804C19B8(void* ctrl);
// nw4r diagnostics / math helpers used by func_804728E8 (retail-named).
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32);
extern "C" void func_80477F80__Q26mpfsys18MPFDrawDisplayListFv(
    mpfsys::MPFDrawDisplayList* inst, u8* ptr, u8 byte, u32 count);
// Layer-draw push (defined below): Fv-annotated but actually receives
// (self, frame+0x9C, a5, a6, f1) from func_80471FCC.
void func_80474064__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* viewMtx, void* a5, void* a6,
    f32 fovScale);

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

// Map header / tail overlays used by the display-list builder.
struct MpfsysMapHead {
    u8 field_0x0[0x10];
    u32 field_0x10; // byte offset to the tail record, also the item-count gate
};
struct MpfsysMapTail {
    u32 blkOfs;    // +0 relative offset to the first source block
    u32 itemCount; // +4 number of 0x300-byte records to emit
};
// Output buffer: small header followed by the packed GX command stream.
struct MpfsysDlBuf {
    u8 field_0x0[8];
    u16 field_08;
    u16 pad;
    u32 field_0C;
    u32 field_10; // end pointer of the emitted region
    u8 pad2[4];
    u8 data[0xE00];
};

// Builds the packed primitive display list consumed by MPFDrawDisplayList:
// for each of the tail's items a 0x300-byte record is emitted into out->data
// - a 3-byte command header (81 00 FC) followed by 21 twelve-byte chunks
// interleaving a running index pair with the block's four palette bytes,
// then zero fill up to the 0x300 boundary. Finally the whole region is
// flushed from the data cache.
void func_80472064__Q26mpfsys17UnkClass_80471EC8Fv(u8* src, MpfsysDlBuf* out, u32 total) {
    u32 cnt = ((MpfsysMapHead*)src)->field_0x10;
    out->field_08 = 0;
    out->field_0C = 0;
    if (cnt == 0) {
        return;
    }
    MpfsysMapTail* tail = (MpfsysMapTail*)(src + cnt);
    u32 n = tail->itemCount;
    u8* blocks = src + tail->blkOfs;

    u32 size = n * 0xF00 + 0x20;
    out->field_0C = (u32)(total - size) / 0xFF;
    out->field_10 = (u32)((u8*)out + size);

    u8* buf = out->data;
    for (u32 item = 0; item < n; item++) {
        const u8* blk = blocks + item * 0x4c;
        u8* p = buf + item * 0x300;
        p[0] = 0x81;
        p[1] = 0;
        p[2] = 0xFC;
        // Each chunk carries a monotonically increasing index nibble pair
        // around the four constant bytes read from the block header.
        int a = 0;
        int b = 0;
        u8* q = p + 3;
        for (int c = 0; c < 21; c++) {
            q[0] = a++;
            q[1] = b;
            q[2] = blk[0xc];
            q[3] = a++;
            q[4] = b;
            q[5] = blk[0xd];
            q[6] = a++;
            q[7] = b;
            q[8] = blk[0xe];
            q[9] = a++;
            q[10] = b;
            b++;
            q[11] = blk[0xf];
            q += 12;
        }
        while (q < p + 0x300) {
            *q++ = 0;
        }
    }
    DCStoreRange(buf, n * 0x300);
}

// Per-layer animation record laid out by func_804724DC (stride 0xB4).
// Uses a trivial POD vector so member copies stay inline word copies
// (retail never calls the ml::CVec3 assignment operator here).
struct MpfsysVec3 {
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
        u32 w[3];
    };
};
struct MpfsysLayerRec {
    MpfsysVec3 f00;   // +0x00
    MpfsysVec3 f0C;   // +0x0C
    MpfsysVec3 f18;   // +0x18
    MpfsysVec3 f24;   // +0x24
    MpfsysVec3 f30;   // +0x30
    MpfsysVec3 f3C;   // +0x3C
    MpfsysVec3 dir;   // +0x48
    Mtx mtxA;         // +0x54
    Mtx mtxB;         // +0x84
};

// Reset overlay for the layer-state tail of UnkClass_80471EC8.
struct MpfsysInitLayout {
    u8 pad[0x2D00];
    f32 w[36];                    // 0x2D00 weight table
    f32 lod0[3];                  // 0x2DE0
    f32 lod1[4];                  // 0x2DEC..0x2DF8
    u8 gap[0x2E04 - 0x2DFC];
    s32 field_0x2E04;             // layer colour mode
    u8 gap2[0x2E10 - 0x2E08];
    u8 field_0x2E10;
    u16 field_0x2E12;
};

void mpfsys::UnkClass_80471EC8::func_804724DC() {
    MpfsysLayerRec* rec = (MpfsysLayerRec*)this;
    // Trivial view of the shared zero vector so copies stay inline.
    MpfsysVec3 zeroVec = *(MpfsysVec3*)&ml::CVec3::zero;
    for (int i = 0; i < 0x40; i++) {
        // Three fixed-axis vectors with one randomised component each.
        rec->f00.x = lbl_eu_8066A744;
        rec->f00.y = lbl_eu_8066A748 * (f32)(rand() % 100) - lbl_eu_8066A74C;
        rec->f00.z = lbl_eu_8066A744;
        rec->f0C.x = lbl_eu_8066A744;
        rec->f0C.y = (f32)1;
        rec->f0C.z = lbl_eu_8066A744;
        rec->f18.x = lbl_eu_8066A744;
        rec->f18.y = (f32)2;
        rec->f18.z = lbl_eu_8066A744;
        rec->f24.w[0] = zeroVec.w[0];
        rec->f24.w[1] = zeroVec.w[1];
        rec->f24.w[2] = zeroVec.w[2];
        rec->f30.w[0] = zeroVec.w[0];
        rec->f30.w[1] = zeroVec.w[1];
        rec->f30.w[2] = zeroVec.w[2];
        rec->f3C.w[0] = zeroVec.w[0];
        rec->f3C.w[1] = zeroVec.w[1];
        rec->f3C.w[2] = zeroVec.w[2];
        PSMTXIdentity(rec->mtxA);
        PSMTXIdentity(rec->mtxB);
        // Direction vector: two more random draws scaled into [-c, c].
        f32 dz = lbl_eu_8066A748 * (f32)(rand() % 100) - lbl_eu_8066A74C;
        rec->dir.x = lbl_eu_8066A748 * (f32)(rand() % 100) - lbl_eu_8066A74C;
        rec->dir.y = lbl_eu_8066A744;
        rec->dir.z = dz;
        if (rec->dir.x == lbl_eu_8066A744 && rec->dir.y == lbl_eu_8066A744 &&
            rec->dir.z == lbl_eu_8066A744) {
            rec->dir.x = lbl_eu_8066A744;
            rec->dir.y = lbl_eu_8066A744;
            rec->dir.z = lbl_eu_8066A750;
            goto next;
        }
        {
            f32 lenSq = dz * dz + (rec->dir.x * rec->dir.x +
                                   rec->dir.y * rec->dir.y);
            if (lenSq == lbl_eu_8066A744) {
                rec->dir.w[0] = zeroVec.w[0];
                rec->dir.w[1] = zeroVec.w[1];
                rec->dir.w[2] = zeroVec.w[2];
            } else {
                PSVECNormalize((Vec*)&rec->dir, (Vec*)&rec->dir);
            }
        }
    next:
        rec++;
    }

    // Reset the shared layer-state tail: weight table, LOD constants and the
    // colour-mode fields, then clear the first 0x20 bytes of the table.
    MpfsysInitLayout* st = (MpfsysInitLayout*)this;
    for (int i = 0; i < 36; i++) {
        st->w[i] = lbl_eu_8066A754;
    }
    st->lod0[0] = lbl_eu_8066A758;
    st->lod0[1] = lbl_eu_8066A75C;
    st->lod0[2] = lbl_eu_8066A760;
    st->lod1[0] = lbl_eu_8066A750;
    st->lod1[1] = lbl_eu_8066A750;
    st->lod1[2] = lbl_eu_8066A764;
    st->lod1[3] = lbl_eu_8066A764;
    st->field_0x2E12 = 0;
    st->field_0x2E04 = 0xA;
    st->field_0x2E10 = 0;
    memset(st->w, 0, 0x20);
}

// Self-referential offset table on UnkClass_80471EC8 used by
// func_80473984: each field holds an offset to a u32 that itself holds an
// offset from `this` (two-level pointer chase into the embedded state).
struct MpfsysDlRefSrc {
    u8 pad0[0xC];
    u32 offC;
    u8 pad1[0x1C - 0x10];
    u32 off1C;
    u8 pad2[0x24 - 0x20];
    u32 off24;
    u8 pad3[0x2C - 0x28];
    u32 off2C;
};

// Scene object tail consumed by the env-light refresh in func_80473984.
struct MpfsysSceneCtrl {
    u8 pad[0x7C];
    void* envLgtCtrl;
};

// Simulation constants tail of UnkClass_80471EC8 (func_804728E8).
struct MpfsysSimState {
    u8 pad[0x2DE8];
    f32 field_2DE8; // wobble amplitude
    f32 field_2DEC; // time scale
};

// Scratch camera block layout used by func_80474064/func_80473984 (at
// lbl_eu_80658410).
struct MpfsysMtxWords {
    union {
        Mtx m;
        u32 w[12];
    };
};
struct MpfsysCamBlock {
    ml::CVec3 trans;    // +0x00 translation extracted from the view matrix
    ml::CVec3 axis;     // +0x0C billboard axis (transformed in place)
    MpfsysMtxWords rot; // +0x18 rotation-only copy of the view matrix
    MpfsysMtxWords inv; // +0x48 inverted view matrix
};

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

// Colour quads owned by the bound animation object: two RGBA f32 quads at
// +0x11A8 / +0x11C8 behind the pointer chain self->0x2E08 -> +0x64 -> +0x8.
struct MpfsysAnimObj64 {
    u8 field_0x0[0x64];
    void* field_0x64;
};
struct MpfsysColorSrcPtr {
    u8 field_0x0[0x8];
    void* field_0x8;
};
struct MpfsysColorBlock {
    u8 field_0x0[0x11A8];
    f32 quad0[4]; // 0x11A8
    u8 gap[0x11C8 - 0x11B8];
    f32 quad1[4]; // 0x11C8
};

void func_804723A4__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, GXColor* outMat, GXColor* outFog) {
    // Build two GXColors from the animation object's colour quads: each f32
    // component is scaled by lbl_eu_8066A740 and truncated to a byte, then
    // alpha is forced to 0xFF.
    MpfsysResState* s = (MpfsysResState*)self;
    MpfsysAnimObj64* obj = (MpfsysAnimObj64*)s->field_0x2E08;
    MpfsysColorSrcPtr* src = (MpfsysColorSrcPtr*)obj->field_0x64;
    MpfsysColorBlock* cb = (MpfsysColorBlock*)src->field_0x8;
    f32 scale = lbl_eu_8066A740;

    GXColor mat;
    mat.r = (u8)(scale * cb->quad0[0]);
    mat.g = (u8)(scale * cb->quad0[1]);
    mat.b = (u8)(scale * cb->quad0[2]);
    mat.a = (u8)(scale * cb->quad0[3]);
    *outMat = mat;
    outMat->a = 0xFF;

    GXColor fog;
    fog.r = (u8)(scale * cb->quad1[0]);
    fog.g = (u8)(scale * cb->quad1[1]);
    fog.b = (u8)(scale * cb->quad1[2]);
    fog.a = (u8)(scale * cb->quad1[3]);
    *outFog = fog;
    outFog->a = 0xFF;
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
        // Reused component held in a named local, single-use component read
        // directly (matches the retail f1/f0 split).
        f32 f = lbl_eu_8066A758;
        state->field_0x2DE0 = f;
        state->field_0x2DE4 = lbl_eu_8066A75C;
        state->field_0x2DE8 = f;
    } else if (mode == 2) {
        f32 f = lbl_eu_8066A74C;
        state->field_0x2DE0 = f;
        state->field_0x2DE4 = lbl_eu_8066A75C;
        state->field_0x2DE8 = f;
    } else {
        f32 f = lbl_eu_8066A770;
        state->field_0x2DE0 = f;
        state->field_0x2DE4 = lbl_eu_8066A74C;
        state->field_0x2DE8 = f;
    }
}

// Layer spring simulation step. Retail symbol is Fv-annotated but receives
// (self, rec, dt): rec points at one 0xB4 layer record (points P[3] at +0,
// targets Q[3] at +0x24, dir at +0x48, matrices at +0x54/+0x84). Applies
// distance constraints along the P-chain, randomised jitter, clamps the
// targets, integrates positions, and re-derives both orientation quaternions
// from the point chain.
void func_804728E8__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, MpfsysLayerRec* rec, f32 arg) {
    MpfsysSimState* st = (MpfsysSimState*)self;
    // Trivial POD view of the shared zero vector so copies stay inline.
    MpfsysVec3 zeroVec = *(MpfsysVec3*)&ml::CVec3::zero;
    f32 dt = st->field_2DEC * arg;
    f32 scaledDt = dt;
    MpfsysVec3* P = &rec->f00;  // chain points   (+0x00, +0x0C, +0x18)
    MpfsysVec3* Q = &rec->f24;  // target points  (+0x24, +0x30, +0x3C)

    // When amplitude is at its floor and dt is exactly 1, soften the
    // constraint response for near-degenerate directions.
    if (st->field_2DE8 <= lbl_eu_8066A758 && arg == lbl_eu_8066A750) {
        f32 lsq = rec->dir.z * rec->dir.z +
                  (rec->dir.x * rec->dir.x + rec->dir.y * rec->dir.y);
        if (lsq <= lbl_eu_8066A774) {
            scaledDt = dt * lbl_eu_8066A74C;
        }
    }

    // Randomised jitter vector (three draws mod 500); wobble scales the first.
    f32 wobble = st->field_2DE8 * (lbl_eu_80663850 * (f32)(rand() % 500));
    MpfsysVec3 jitter;
    jitter.y = lbl_eu_8066A744;
    jitter.z = st->field_2DEC *
               (lbl_eu_8066A778 *
                (lbl_eu_80663850 * (f32)(rand() % 500) - lbl_eu_8066A74C));
    jitter.x = st->field_2DEC *
               (lbl_eu_8066A778 *
                (lbl_eu_80663850 * (f32)(rand() % 500) - lbl_eu_8066A74C));
    rec->dir.x += jitter.x;
    rec->dir.y += jitter.y;
    rec->dir.z += jitter.z;

    {
        f32 lsq = rec->dir.z * rec->dir.z +
                  (rec->dir.x * rec->dir.x + rec->dir.y * rec->dir.y);
        if (lsq == lbl_eu_8066A744) {
            rec->dir.w[0] = zeroVec.w[0];
            rec->dir.w[1] = zeroVec.w[1];
            rec->dir.w[2] = zeroVec.w[2];
        } else {
            PSVECNormalize((Vec*)&rec->dir, (Vec*)&rec->dir);
        }
    }

    MpfsysVec3 vel;
    vel.x = rec->dir.x * wobble;
    vel.y = rec->dir.y * wobble;
    vel.z = rec->dir.z * wobble;

    // Distance constraints between consecutive chain points: move the target
    // points Q toward satisfying |P[i]-P[i+1]| == rest length.
    MpfsysVec3 push; // last iteration's value feeds the drift below
    for (int i = 0; i < 2; i++) {
        MpfsysVec3 d;
        d.x = P[i].x - P[i + 1].x;
        d.y = P[i].y - P[i + 1].y;
        d.z = P[i].z - P[i + 1].z;
        f32 mag = PSVECMag((Vec*)&d);
        f32 strain = mag - lbl_eu_8066A750;
        f32 inv = (mag >= lbl_eu_8066A774) ? lbl_eu_8066A750 / mag
                                           : lbl_eu_8066A77C;
        f32 k = -(lbl_eu_8066A778 * strain +
                  lbl_eu_8066A74C * strain * inv) *
                scaledDt;
        push.x = d.x * inv * k;
        push.y = d.y * inv * k;
        push.z = d.z * inv * k;
        Q[i].x += push.x;
        Q[i].y += push.y;
        Q[i].z += push.z;
        Q[i + 1].x -= push.x;
        Q[i + 1].y -= push.y;
        Q[i + 1].z -= push.z;
    }

    // Pull targets 1/2 toward the mirrored anchor (-j on Y).
    for (int j = 1; j < 3; j++) {
        MpfsysVec3 v;
        v.x = -P[j].x;
        v.y = (f32)(-(f64)j) - P[j].y;
        v.z = -P[j].z;
        f32 mag = PSVECMag((Vec*)&v);
        if (mag >= lbl_eu_8066A774) {
            f32 inv = lbl_eu_8066A750 / mag;
            f32 k = lbl_eu_8066A780 * mag * scaledDt;
            Q[j].x += v.x * inv * k;
            Q[j].y += v.y * inv * k;
            Q[j].z += v.z * inv * k;
        }
    }

    // Target 2 also follows the direction velocity.
    f32 vk = lbl_eu_8066A784 * scaledDt;
    Q[2].x += vel.x * vk;
    Q[2].y += vel.y * vk;
    Q[2].z += vel.z * vk;

    // Randomised target drift (draws mod 100 offset by 100). Retail reuses
    // the last constraint-push stack slot as the drift base.
    MpfsysVec3& drift = push;
    f32 rndScale = lbl_eu_80663854 * (f32)(rand() % 100 + 100);
    drift.x *= rndScale;
    rndScale = lbl_eu_80663854 * (f32)(rand() % 100 + 100);
    drift.y *= rndScale;
    rndScale = lbl_eu_80663854 * (f32)(rand() % 100 + 100);
    drift.z *= rndScale;

    f32 shrink = lbl_eu_8066A750 - lbl_eu_8066A788 * dt;
    Q[1].x = (Q[1].x + drift.x) * shrink;
    Q[1].y = (Q[1].y + drift.y) * shrink;
    Q[1].z = (Q[1].z + drift.z) * shrink;

    // Clamp targets 1 and 2 component-wise into [min, max].
    MpfsysVec3* clamps[2] = { &Q[1], &Q[2] };
    for (int c = 0; c < 2; c++) {
        if (c == 1) {
            Q[2].x *= shrink;
            Q[2].y *= shrink;
            Q[2].z *= shrink;
        }
        f32* v = &clamps[c]->x;
        for (int a = 0; a < 3; a++) {
            if (v[a] > lbl_eu_8066A758) {
                v[a] = lbl_eu_8066A758;
            }
            if (v[a] < lbl_eu_8066A78C) {
                v[a] = lbl_eu_8066A78C;
            }
        }
    }

    // Integrate chain points by the scaled targets.
    P[1].x += Q[1].x * dt;
    P[1].y += Q[1].y * dt;
    P[1].z += Q[1].z * dt;
    P[2].x += Q[2].x * dt;
    P[2].y += Q[2].y * dt;
    P[2].z += Q[2].z * dt;

    // Re-derive both orientation quaternions from the point chain. Retail
    // inlines this block twice (once per matrix), so keep it duplicated.
    Quaternion quat;
    {
        MpfsysVec3 n;
        n.x = P[2].x - P[0].x;
        n.y = P[2].y - P[0].y;
        n.z = P[2].z - P[0].z;
        f32 lsq = n.z * n.z + (n.x * n.x + n.y * n.y);
        if (lsq == lbl_eu_8066A744) {
            const ml::CVec3& z = ml::CVec3::zero;
            n.x = z.x;
            n.y = z.y;
            n.z = z.z;
        } else {
            PSVECNormalize((Vec*)&n, (Vec*)&n);
        }
        f32 dot = ml::CVec3::unitY.x * n.x + ml::CVec3::unitY.y * n.y +
                  ml::CVec3::unitY.z * n.z;
        if (dot < lbl_eu_8066A790) {
            quat.x = lbl_eu_8066A744;
            quat.y = lbl_eu_8066A750;
            quat.z = lbl_eu_8066A744;
            quat.w = lbl_eu_8066A744;
        } else {
            Vec axis;
            Vec unitY;
            unitY.x = ml::CVec3::unitY.x;
            unitY.y = ml::CVec3::unitY.y;
            unitY.z = ml::CVec3::unitY.z;
            PSVECCrossProduct(&unitY, (Vec*)&n, &axis);
            f32 m = lbl_eu_8066A73C * (lbl_eu_8066A750 + dot);
            if (m < lbl_eu_8066A744) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                             lbl_eu_80526300);
            }
            f32 s4 = lbl_eu_8066A744;
            if (!(m <= lbl_eu_8066A744)) {
                s4 = m * FrSqrt__Q24nw4r4mathFf(m);
            }
            f32 scale = lbl_eu_8066A750 / s4;
            quat.x = axis.x * scale;
            quat.y = axis.y * scale;
            quat.z = axis.z * scale;
            quat.w = lbl_eu_8066A74C * s4;
        }
    }
    PSMTXQuat(rec->mtxA, &quat);
    {
        MpfsysVec3 n;
        n.x = P[1].x - P[0].x;
        n.y = P[1].y - P[0].y;
        n.z = P[1].z - P[0].z;
        f32 lsq = n.z * n.z + (n.x * n.x + n.y * n.y);
        if (lsq == lbl_eu_8066A744) {
            const ml::CVec3& z = ml::CVec3::zero;
            n.x = z.x;
            n.y = z.y;
            n.z = z.z;
        } else {
            PSVECNormalize((Vec*)&n, (Vec*)&n);
        }
        f32 dot = ml::CVec3::unitY.x * n.x + ml::CVec3::unitY.y * n.y +
                  ml::CVec3::unitY.z * n.z;
        if (dot < lbl_eu_8066A790) {
            quat.x = lbl_eu_8066A744;
            quat.y = lbl_eu_8066A750;
            quat.z = lbl_eu_8066A744;
            quat.w = lbl_eu_8066A744;
        } else {
            Vec axis;
            Vec unitY;
            unitY.x = ml::CVec3::unitY.x;
            unitY.y = ml::CVec3::unitY.y;
            unitY.z = ml::CVec3::unitY.z;
            PSVECCrossProduct(&unitY, (Vec*)&n, &axis);
            f32 m = lbl_eu_8066A73C * (lbl_eu_8066A750 + dot);
            if (m < lbl_eu_8066A744) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                             lbl_eu_80526300);
            }
            f32 s4 = lbl_eu_8066A744;
            if (!(m <= lbl_eu_8066A744)) {
                s4 = m * FrSqrt__Q24nw4r4mathFf(m);
            }
            f32 scale = lbl_eu_8066A750 / s4;
            quat.x = axis.x * scale;
            quat.y = axis.y * scale;
            quat.z = axis.z * scale;
            quat.w = lbl_eu_8066A74C * s4;
        }
    }
    PSMTXQuat(rec->mtxB, &quat);

    // Reset the first point/target pair.
    P[0].w[0] = zeroVec.w[0];
    P[0].w[1] = zeroVec.w[1];
    P[0].w[2] = zeroVec.w[2];
    Q[0].w[0] = zeroVec.w[0];
    Q[0].w[1] = zeroVec.w[1];
    Q[0].w[2] = zeroVec.w[2];
}

#pragma push
#pragma auto_inline off
void mpfsys::UnkClass_80471EC8::func_80473394() {}
#pragma pop

void* mpfsys::UnkClass_80471EC8::func_804734F4(u8 layerIndex) {
    return (u8*)this + layerIndex * 0x1680;
}

// ResPltt header overlay for func_80473500.
struct MpfsysPlttData {
    u8 field_0x0[0x10];
    u32 field_0x10; // relative offset to palette data
    u8 pad[4];
    u32 field_0x18; // GXTlutFmt
    u16 field_0x1C; // entry count
};

// Uploads the texture for display-list entry `texIndex` to texMap `texMap`.
// CI-format textures get their palette loaded as a TLUT first; mipmapped
// textures receive LOD parameters derived from `lodBias`.
void func_80473500__Q26mpfsys17UnkClass_80471EC8Fiif(int texIndex, int texMap, f32 lodBias) {
    nw4r::g3d::ResFile resFile(lbl_eu_80665848[texIndex]);
    nw4r::g3d::ResTex tex = resFile.GetResTex(texIndex);
    if (!tex.IsValid()) {
        nw4r::db::Panic(lbl_eu_8052CF10, 0x26, lbl_eu_8052CEF4, lbl_eu_80661E98,
                        lbl_eu_8066385C);
    }

    GXTexObj texObj;
    f32 minLod;
    f32 maxLod;
    u8 mipmap;
    if (tex.ref().flag & nw4r::g3d::ResTexData::FLAG_CIFMT) {
        if (!tex.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF10, 0x26, lbl_eu_8052CEF4, lbl_eu_80661E98,
                            lbl_eu_8066385C);
        }
        nw4r::g3d::ResFile parent = tex.GetParent();
        const char* plttName = (const char*)tex.ptr() + tex.ref().name;
        nw4r::g3d::ResPltt pltt = parent.GetResPltt(plttName);

        void* imgData;
        u16 width;
        u16 height;
        GXCITexFmt ciFmt;
        tex.GetTexObjCIParam(&imgData, &width, &height, &ciFmt, &minLod, &maxLod,
                             &mipmap);

        // C14X2 entries use the 0x10-entry TLUT slot.
        u32 tlutName = 0;
        if (ciFmt == (GXCITexFmt)0xA) {
            tlutName = 0x10;
        }

        // Palette data pointer / format / entry count, each guarded by an
        // assertion on the resolved ResPltt.
        if (!pltt.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2A, lbl_eu_8052CF50, lbl_eu_80661EA8,
                            lbl_eu_80663864);
        }
        MpfsysPlttData* pd = (MpfsysPlttData*)pltt.ptr();
        u16 numEntries = pd->field_0x1C;
        if (!pltt.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2A, lbl_eu_8052CF50, lbl_eu_80661EA8,
                            lbl_eu_80663864);
        }
        u32 dataOfs = pd->field_0x10;
        if (!pltt.IsValid()) {
            nw4r::db::Panic(lbl_eu_8052CF3C, 0x2A, lbl_eu_8052CF20, lbl_eu_80661EA8,
                            lbl_eu_80663860);
        }
        const void* table = (dataOfs != 0) ? (const char*)pd + dataOfs : NULL;

        GXTlutObj tlutObj;
        GXInitTlutObj(&tlutObj, (void*)table, (GXTlutFmt)pd->field_0x18, numEntries);
        GXLoadTlut(&tlutObj, tlutName);
        GXInitTexObjCI(&texObj, imgData, width, height, (GXTexFmt)ciFmt,
                       (GXTexWrapMode)1, (GXTexWrapMode)1, (GXBool)mipmap, tlutName);
    } else {
        void* imgData;
        u16 width;
        u16 height;
        GXTexFmt fmt;
        tex.GetTexObjParam(&imgData, &width, &height, &fmt, &minLod, &maxLod,
                           &mipmap);
        GXInitTexObj(&texObj, imgData, width, height, fmt, (GXTexWrapMode)1,
                     (GXTexWrapMode)1, (GXBool)mipmap);
    }

    if (mipmap != 0) {
        GXInitTexObjLOD(&texObj, (GXTexFilter)5, (GXTexFilter)1, minLod, maxLod,
                        lodBias, GX_FALSE, GX_FALSE, (GXAnisotropy)0);
    } else {
        f32 lod = lbl_eu_8066A7A4;
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, lod, lod, lod, GX_FALSE,
                        GX_FALSE, GX_ANISO_1);
    }
    GXLoadTexObj(&texObj, (GXTexMapID)texMap);
}
extern "C" {
#pragma dont_inline on
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

// Display-list entry table shape: { count, items[] } pairs indexed by the
// caller; each item is a 4-byte record whose low s16 is a texture index.
struct MpfsysTevItem {
    s16 id;
    u16 field_0x2;
};
struct MpfsysTevEntry {
    u32 count;
    MpfsysTevItem* items;
};

void func_804737F0__Q26mpfsys17UnkClass_80471EC8Fv(s32 index, f32 scale) {
    // Per-layer textured-draw setup: invalidate the cached texture index,
    // lazily install the fullscreen TEV/vertex-desc state for mode 6, then
    // feed every item of display-list entry `index` to func_80473500 with
    // the given scale.
    MpfsysTevEntry* entry = &((MpfsysTevEntry*)&lbl_eu_80665868)[index];
    lbl_eu_80665858 = -1;
    if (lbl_eu_80665854 != 1) {
        GXSetNumTevStages(1);
        lbl_eu_80665854 = 1;
    }
    if (lbl_eu_8066585A != 6) {
        GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
        GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
        GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
        GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
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
    MpfsysTevItem* items = entry->items;
    u32 count = entry->count;
    for (u32 i = 0; i < count; i++) {
        func_80473500__Q26mpfsys17UnkClass_80471EC8Fiif(items[i].id, i, scale);
    }
}

// Camera setup + full GX state push for the map layer draw. The retail
// symbol is Fv-annotated but actually receives (self, texBase, srcMtx, a6,
// a7, swapMode, fovScale); literal mangled name kept (func_8047233C
// precedent). Mirrors the prologue of func_80474064, then configures the
// fixed TEV/vertex/light pipeline and refreshes scene fog.
void func_80473984__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* texBase, const Mtx srcMtx, void* a6,
    void* a7, u32 swapMode, f32 fovScale) {
    MpfsysCamBlock* cam = (MpfsysCamBlock*)lbl_eu_80658410;
    MpfsysMtxWords* src = (MpfsysMtxWords*)srcMtx;

    // Invert a copy of the source view matrix into cam->inv.
    for (int i = 0; i < 12; i++) {
        cam->inv.w[i] = src->w[i];
    }
    lbl_eu_80665848[0] = texBase;
    PSMTXInverse(cam->inv.m, cam->inv.m);

    // Rotation-only copy (translation column zeroed).
    for (int i = 0; i < 12; i++) {
        cam->rot.w[i] = src->w[i];
    }
    cam->rot.m[0][3] = 0;
    cam->rot.m[1][3] = 0;
    cam->rot.m[2][3] = 0;

    // Translation column of the source matrix, then billboard axis through
    // the rotation part.
    const f32* pf = (const f32*)srcMtx;
    cam->trans.x = pf[3];
    cam->trans.y = pf[7];
    cam->trans.z = pf[11];
    cam->axis.x = lbl_eu_8066A7A4;
    cam->axis.y = lbl_eu_8066A7A4;
    cam->axis.z = lbl_eu_8066A7AC;
    PSMTXMultVec(cam->rot.m, (Vec*)&cam->axis, (Vec*)&cam->axis);

    f32 lenSq = cam->axis.z * cam->axis.z +
                (cam->axis.x * cam->axis.x + cam->axis.y * cam->axis.y);
    if (lenSq == lbl_eu_8066A7A4) {
        // Component-wise copy keeps the shared zero-vector loads inline
        // (retail never calls the CVec3 assignment operator here).
        const ml::CVec3& z = ml::CVec3::zero;
        cam->axis.x = z.x;
        cam->axis.y = z.y;
        cam->axis.z = z.z;
    } else {
        PSVECNormalize((Vec*)&cam->axis, (Vec*)&cam->axis);
    }

    GXSetCurrentMtx(0);
    GXLoadPosMtxImm(cam->inv.m, 0);
    lbl_eu_80665840 = self;

    // FOV: tan of the scaled half-angle plus inverse-square term.
    f32 t = (f32)tan(lbl_eu_8066A7B8 *
                     (lbl_eu_8066A7BC * (lbl_eu_8066A7B4 * (fovScale * lbl_eu_8066A210))));
    f32 lenSq2 = cam->axis.x * cam->axis.x + cam->axis.y * cam->axis.y +
                 cam->axis.z * cam->axis.z;

    // Refresh the display-list pointer table via the two-level offset chase.
    MpfsysDlRefSrc* refs = (MpfsysDlRefSrc*)self;
    u8* base = (u8*)self;
    lbl_eu_80665844 = base + *(u32*)(base + *(u32*)(base + 0x1C));
    lbl_eu_8066584C[0].base = base + *(u32*)(base + *(u32*)(base + 0x24));
    lbl_eu_80665850 = (u16*)(base + *(u32*)(base + *(u32*)(base + 0xC)));
    *(u32*)&lbl_eu_80665868 = (u32)(base + *(u32*)(base + *(u32*)(base + 0x2C)));
    lbl_eu_80665880 = lbl_eu_8066A7B0 / lenSq2;
    *(f32*)&lbl_eu_80665884 = lbl_eu_8066A7C0 * t;

    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0xB, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)0x9, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)0xB, (GXCompCnt)0x0, (GXCompType)0x0, 0);
    GXSetVtxAttrFmt((GXVtxFmt)0, (GXAttr)0xD, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)1, (GXAttr)0x9, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)1, (GXAttr)0xB, (GXCompCnt)0x0, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)1, (GXAttr)0xD, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)2, (GXAttr)0x9, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)2, (GXAttr)0xB, (GXCompCnt)0x0, (GXCompType)0x0, 0);
    GXSetVtxAttrFmt((GXVtxFmt)2, (GXAttr)0xD, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)3, (GXAttr)0x9, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)3, (GXAttr)0xD, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)4, (GXAttr)0x9, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetVtxAttrFmt((GXVtxFmt)4, (GXAttr)0xB, (GXCompCnt)0x0, (GXCompType)0x1, 0);
    GXSetVtxAttrFmt((GXVtxFmt)4, (GXAttr)0xD, (GXCompCnt)0x1, (GXCompType)0x4, 0);
    GXSetAlphaCompare((GXCompare)0x6, 0x80, GX_AOP_AND, (GXCompare)0x3, 0xff);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumTexGens(1);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetNumTevStages(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_POS, 0x3c, GX_FALSE,
                      0x7d);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, (GXTevColorArg)0xF, (GXTevColorArg)0xA,
                    (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2,
                    GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6,
                    (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0,
                    GX_TRUE, GX_TEVREG0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, (GXLightID)1,
                  GX_DF_NONE, GX_AF_SPOT);

    // Push light 1 with the shared key colour and refresh channel colours.
    GXColor keyColor;
    *(u32*)&keyColor = lbl_eu_8066A7A8;
    GXLightObj lightObj;
    GXInitLightColor(&lightObj, keyColor);
    GXLoadLightObjImm(&lightObj, GX_LIGHT1);

    func_804723A4__Q26mpfsys17UnkClass_80471EC8Fv(
        (mpfsys::UnkClass_80471EC8*)lbl_eu_80665838,
        (GXColor*)&lbl_eu_80665878, (GXColor*)&lbl_eu_8066587C);

    GXColor ambColor;
    *(u32*)&ambColor = lbl_eu_80665878;
    GXSetChanAmbColor(GX_COLOR0, ambColor);
    GXSetChanMatColor(GX_COLOR0, keyColor);

    // Invalidate cached render-state and publish per-view parameters.
    lbl_eu_80665858 = -1;
    lbl_eu_80665854 = 1;
    lbl_eu_8066585A = 0;
    lbl_eu_8066586C = 0;
    lbl_eu_80665860 = 0;
    lbl_eu_80665874 = (u32)a7;
    lbl_eu_80665870 = (u32)a6;

    GXColor kColor;
    *(u32*)&kColor = lbl_eu_8066A7A0;
    GXSetTevKColor(GX_KCOLOR0, kColor);

    GXSetArray((GXAttr)0xD, lbl_eu_80665844, 8);
    GXSetTevKColorSel(GX_TEVSTAGE0, (GXTevKColorSel)0xC);
    GXSetTevKColorSel(GX_TEVSTAGE1, (GXTevKColorSel)0xD);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, (GXTevKAlphaSel)0x1C);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, (GXTevKAlphaSel)0x1D);

    if (swapMode == 0) {
        // Identity tables (mode 0 renders unswapped).
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    } else {
        // R/R/R/A tables.
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    }

    // Refresh the scene env-light controller, then reload fog state 0x40.
    func_804C19B8(((MpfsysSceneCtrl*)func_8049698C())->envLgtCtrl);
    lbl_eu_8066585C = 0;
    nw4r::g3d::G3DState::LoadFog(0x40);
}

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

// View-slot matrix upload. idx selects a slot in the shared view-entry table;
// builds a translation (+ optional scale) matrix from the slot's offset vector
// and uploads it as position matrix 3. When layerIdx is non-zero, the layer's
// animated matrices at +0x84 / +0x54 are concatenated with it and uploaded as
// position matrices 6 and 9.
void func_80474B00__Q26mpfsys17UnkClass_80471EC8Fv(int idx, s32 layerIdx,
                                                   u8 layerSel, f32 scale) {
    ml::CVec3* ofs =
        (ml::CVec3*)(lbl_eu_8066584C[idx].base + idx * 0xC);
    Mtx m;
    Mtx out;
    f32 zero = lbl_eu_8066A7A4;
    f32 one = lbl_eu_8066A7B0;
    f32 diag;
    if (scale == zero) {
        diag = one;
    } else {
        // Scale factor grows with the masked index sum (converted through the
        // shared unsigned int -> double helper constants).
        u32 v = ((u32)idx + layerIdx) & lbl_eu_8066A72C;
        diag = one + scale * (f32)(((f64)v - lbl_eu_8066A7C8) * lbl_eu_8066A730);
    }
    m[0][0] = diag;
    m[0][1] = zero;
    m[0][2] = zero;
    m[0][3] = zero;
    m[1][0] = zero;
    m[1][1] = diag;
    m[1][2] = zero;
    m[1][3] = zero;
    m[2][0] = zero;
    m[2][1] = zero;
    m[2][2] = diag;
    m[2][3] = zero;
    m[0][3] = ofs->x;
    m[1][3] = ofs->y;
    m[2][3] = ofs->z;

    PSMTXConcat(lbl_eu_80658458, m, out);
    GXLoadPosMtxImm(out, 3);
    if (layerIdx != 0) {
        mpfsys::UnkClass_80471EC8* inst = (mpfsys::UnkClass_80471EC8*)lbl_eu_80665838;
        u8* ent = (u8*)inst->func_804734F4(layerSel);
        ent += (((u32)idx + layerIdx) & lbl_eu_8066A72C) * 0xB4;
        PSMTXConcat(m, *(Mtx*)(ent + 0x84), out);
        PSMTXConcat(lbl_eu_80658458, out, out);
        GXLoadPosMtxImm(out, 6);
        PSMTXConcat(m, *(Mtx*)(ent + 0x54), out);
        PSMTXConcat(lbl_eu_80658458, out, out);
        GXLoadPosMtxImm(out, 9);
    }
}

// Layer-draw push, retail member of UnkClass_80471EC8: Fv-annotated but
// actually receives (self, frame+0x9C, a5, a6, f1) from func_80471FCC.
// Inverts a copy of the view-frame matrix, derives the billboard axis by
// transforming the constant (., 0, k) vector through the rotation part,
// then computes the FOV scale factors published into the render globals.
void func_80474064__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* viewMtx, void* a5, void* a6,
    f32 fovScale) {
    lbl_eu_8066586C = 0;
    lbl_eu_80665874 = (u32)a6;
    lbl_eu_80665870 = (u32)a5;

    MpfsysCamBlock* cam = (MpfsysCamBlock*)lbl_eu_80658410;
    MpfsysMtxWords* src = (MpfsysMtxWords*)viewMtx;

    cam->inv.w[0] = src->w[0];
    cam->inv.w[1] = src->w[1];
    cam->inv.w[2] = src->w[2];
    cam->inv.w[3] = src->w[3];
    cam->inv.w[4] = src->w[4];
    cam->inv.w[5] = src->w[5];
    cam->inv.w[6] = src->w[6];
    cam->inv.w[7] = src->w[7];
    cam->inv.w[8] = src->w[8];
    cam->inv.w[9] = src->w[9];
    cam->inv.w[10] = src->w[10];
    cam->inv.w[11] = src->w[11];
    PSMTXInverse(cam->inv.m, cam->inv.m);

    const f32* pf = (const f32*)viewMtx;
    cam->rot.w[0] = src->w[0];
    cam->rot.w[1] = src->w[1];
    cam->rot.w[2] = src->w[2];
    cam->rot.w[3] = src->w[3];
    cam->rot.w[4] = src->w[4];
    cam->rot.w[5] = src->w[5];
    cam->rot.w[6] = src->w[6];
    cam->rot.w[7] = src->w[7];
    cam->rot.w[8] = src->w[8];
    cam->rot.w[9] = src->w[9];
    cam->rot.w[10] = src->w[10];
    cam->rot.w[11] = src->w[11];
    cam->rot.m[0][3] = 0;
    cam->rot.m[1][3] = 0;
    cam->rot.m[2][3] = 0;

    cam->trans.x = pf[3];
    cam->trans.y = pf[7];
    cam->trans.z = pf[11];
    cam->axis.y = lbl_eu_8066A7A4;
    cam->axis.z = lbl_eu_8066A7AC;
    PSMTXMultVec(cam->rot.m, cam->axis, cam->axis);

    f32 lenSq = cam->axis.z * cam->axis.z +
                (cam->axis.x * cam->axis.x + cam->axis.y * cam->axis.y);
    if (lenSq == lbl_eu_8066A7A4) {
        cam->axis = ml::CVec3::zero;
    } else {
        PSVECNormalize(cam->axis, cam->axis);
    }

    // FOV: tan of the scaled half-angle, plus the inverse-square term.
    f32 t = (f32)tan(lbl_eu_8066A7B8 *
                     (lbl_eu_8066A7BC * (lbl_eu_8066A7B4 * (fovScale * lbl_eu_8066A210))));
    f32 lenSq2 = cam->axis.x * cam->axis.x + cam->axis.y * cam->axis.y +
                 cam->axis.z * cam->axis.z;
    lbl_eu_80665880 = lbl_eu_8066A7B0 / lenSq2;

    lbl_eu_80665840 = self;
    *(f32*)((u8*)&lbl_eu_80665884 + (u32)lbl_eu_80665838 + 0x2D00) =
        lbl_eu_8066A7C0 * t;
}

void mpfsys::UnkClass_80471EC8::func_80474CC4() {
    // Push the shared constant key color (RGBA latch) as TEV K-color 0.
    GXColor color;
    *(u32*)&color = lbl_eu_8066A7A0;
    GXSetTevKColor((GXTevKColorID)0, color);
}

void func_80474CF4__Q26mpfsys17UnkClass_80471EC8Fv(s32 index) {
    // Push the channel-material colour for table entry `index` of the shared
    // RGB565 colour table. The retail symbol is annotated Fv: r3 carries the
    // table index and is never dereferenced, so the literal mangled name is
    // kept (func_8047233C precedent) - a const member would mangle to
    // ...CFv and never resolve against the retail ...Fv symbol.
    u16 rgb565 = lbl_eu_80665850[index];
    GXColor color = lbl_eu_8066A7D0;
    color.r = ((rgb565 >> 11) & 0x1F) << 3;
    color.g = ((rgb565 >> 5) & 0x3F) << 2;
    color.b = (rgb565 & 0x1F) << 3;
    GXSetChanMatColor(GX_COLOR0, color);
}

void mpfsys::UnkClass_80471EC8::func_80474D50() {
    // Same RGB565->GXColor expansion as func_80474CF4 (alpha byte from
    // lbl_eu_8066A7D4), pushed as TEV register colour 0.
    u16 rgb565 = lbl_eu_80665850[(u32)this];
    GXColor color = lbl_eu_8066A7D4;
    color.r = ((rgb565 >> 11) & 0x1F) << 3;
    color.g = ((rgb565 >> 5) & 0x3F) << 2;
    color.b = (rgb565 & 0x1F) << 3;
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


extern "C" u32 lbl_eu_80523D80;  // .rodata: string
extern "C" u32 lbl_eu_8056DB90;  // .data: object

// [.sdata] 0x80663848-0x80663868 (0x20 = 32B)
// Definition order matters: MWCC packs .sdata in first-definition order and
// retail has the pointer pair at +0x00/+0x04 BEFORE the two floats.
extern "C" u32 lbl_eu_80663848[2] = { (u32)&lbl_eu_80523D80, (u32)&lbl_eu_8056DB90 };

// Float constants shared with func_804724DC/func_804728E8. Retail bit
// patterns are 0x3B03126F / 0x3BA3D70A; use decimal literals (which round to
// exactly those bits) - writing the hex ints as f32 would int->float convert
// them into entirely different values.
f32 lbl_eu_80663850 = 0.002f;
f32 lbl_eu_80663854 = 0.005f;
GXColor lbl_eu_80663858 = { 0xFF, 0xFF, 0xFF, 0xFF };
extern "C" u32 lbl_eu_8066385C = 0x72656600;
extern "C" u32 lbl_eu_80663860 = 0x72656600;
extern "C" u32 lbl_eu_80663864 = 0x72656600;

// [.bss]
Mtx lbl_eu_80658458;

// [.sbss] 0x80665860-0x80665888 (0x28 = 40B) zero-fill
u32 lbl_eu_80665860;
u32 lbl_eu_80665864;
u32 lbl_eu_80665868;
u32 lbl_eu_8066586C;
u32 lbl_eu_80665870;
u32 lbl_eu_80665874;
u32 lbl_eu_80665878;
u32 lbl_eu_8066587C;
f32 lbl_eu_80665880;
u32 lbl_eu_80665884;

DECOMP_FORCEACTIVE(UnkClass_80471EC8_cpp, lbl_eu_80663848);

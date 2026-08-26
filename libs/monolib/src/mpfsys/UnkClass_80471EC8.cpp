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
extern u32 lbl_eu_80665864;
extern u32 lbl_eu_80665860;
extern u32 lbl_eu_80665870;
extern u32 lbl_eu_80665874;
extern f32 lbl_eu_80665880;
extern u32 lbl_eu_80665884;
// Active map ResFile handle (single 4-byte sbss object).
extern void* lbl_eu_80665848;
// Display-list base pointers for the view slots: 12-byte records whose
// first word is the display-list base. Explicit bound keeps the object
// small-data eligible (indexed SDA loads).
struct MpfsysViewEntry {
    u8* base; // +0x00 display-list base for this view slot
    u8 pad[8];
};
extern MpfsysViewEntry lbl_eu_8066584C[2];
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
extern const f32 lbl_eu_8066A794; // rand()%1000 divisor
extern const f32 lbl_eu_8066A798; // rand()%20 divisor
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
extern const char lbl_eu_80661E98[4];
extern const char lbl_eu_80661EA8[4];
extern char lbl_eu_8066385C[4];
extern char lbl_eu_80663860[4];
extern char lbl_eu_80663864[4];
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

// Stack ResFile handle: nw4r::g3d::ResFile is a single pointer wrapper
// (ResCommon<ResFileData>). The retail code builds the wrapper directly on
// the stack without emitting ctor calls, so we model the layout with a POD
// handle and reference the retail-named method symbols below.
struct MpfsysResFileHandle {
    void* mpData;
};

// Retail-named nw4r g3d ResFile readers. extern "C" keeps the call-site
// relocs verbatim (MWCC would otherwise append the parameter encoding).
extern "C" bool CheckRevision__Q34nw4r3g3d7ResFileCFv(const MpfsysResFileHandle*);
extern "C" bool HasResTex__Q34nw4r3g3d7ResFileCFv(const MpfsysResFileHandle*);
extern "C" void Init__Q34nw4r3g3d7ResFileFv(MpfsysResFileHandle*);

// Map attach: validate the descriptor magic/revision, then bind the embedded
// ResFile (alignment-panic, revision/tex checks, Init) and stash the camera /
// view pair, before refreshing the layer state. Returns 1 on success.
bool func_80471EC8__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* a4, MpfsysResFileHandle* a5, void* a6, u32 a7) {
    MpfsysResDesc* d = (MpfsysResDesc*)a4;
    if (d->field_0x0 == 0x4D504646 && d->field_0x4 == 0x3EA) {
        if (d->field_0x18 != 0) {
            MpfsysResFileHandle resFile;
            resFile.mpData = (u8*)a4 + d->field_0x14;
            // ResFile data must be 32-byte aligned.
            if ((u32)resFile.mpData & 0x1F) {
                nw4r::db::Panic(lbl_eu_8052637C, 0x3C, lbl_eu_80526354);
            }
            if (!CheckRevision__Q34nw4r3g3d7ResFileCFv(&resFile)) {
                return false;
            }
            if (!HasResTex__Q34nw4r3g3d7ResFileCFv(&resFile)) {
                return false;
            }
            Init__Q34nw4r3g3d7ResFileFv(&resFile);
            // Memberwise copy: struct assignment would emit an out-of-line
            // copy-assignment operator call that retail does not have.
            a5->mpData = resFile.mpData;
            MpfsysResState* s = (MpfsysResState*)self;
            s->field_0x2E08 = a6;
            s->field_0x2E0C = (void*)a7;
        }
        self->func_804724DC();
        return true;
    }
    return false;
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
    s32 field_0x10; // byte offset to the tail record, also the item-count gate
};
struct MpfsysMapTail {
    u32 blkOfs;    // +0 relative offset to the first source block
    u32 itemCount; // +4 number of 0x300-byte records to emit
};
// Output buffer: 0x20-byte header followed by the packed GX command stream.
struct MpfsysDlBuf {
    u8 field_0x0[8];
    u16 field_08;
    u16 pad;
    u32 field_0C;
    u32 field_10; // end pointer of the emitted region
    u8 pad2[0xC];
    u8 data[0xE00];
};

// Builds the packed primitive display list consumed by MPFDrawDisplayList:
// for each of the tail's items a 0x300-byte record is emitted into out+0x20
// - a 3-byte command header (81 00 FC) followed by 63 twelve-byte chunks
// interleaving two running index counters with the block's four palette
// bytes, then zero fill up to the 0x300 boundary. Finally the whole region
// is flushed from the data cache.
void func_80472064__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, MpfsysMapHead* src, MpfsysDlBuf* out,
    u32 total) {
    s32 cnt = src->field_0x10;
    out->field_08 = 0;
    out->field_0C = 0;
    if (cnt == 0) {
        return;
    }
    MpfsysMapTail* tail = (MpfsysMapTail*)((u8*)src + cnt);
    u8* blocks = (u8*)src + tail->blkOfs;

    // The item count is re-read from the tail every iteration (the output
    // buffer writes may alias it), so no local copy is kept.
    u32 size = tail->itemCount * 0xF00 + 0x20;
    out->field_10 = (u32)((u8*)out + size);
    out->field_0C = (total - size) / 0xFF;

    u8* bufBase = (u8*)out + 0x20;
    // Command header bytes; retail pins both in registers for the whole fn.
    const u8 cmd = 0x81;
    const u8 term = 0xFC;
    u32 recOff = 0;
    for (u32 item = 0; item < tail->itemCount; item++) {
        const u8* blk = blocks;
        u8* p = bufBase + recOff;
        p[0] = cmd;
        p[1] = 0;
        p[2] = term;
        // Each 12-byte chunk carries monotonically increasing index bytes
        // around the four fixed palette bytes read from the block header.
        // One running offset drives the chunk writes and the tail fill.
        // Palette bytes are cached in locals; MWCC sinks these loads into
        // the first unrolled iteration and pins them in registers.
        u8 c0 = blk[0xc];
        u8 c1 = blk[0xd];
        u8 c2 = blk[0xe];
        u8 c3 = blk[0xf];
        int b = 0;
        int a = 0;
        int off = 3;
        for (int c = 0; c < 21; c++) {
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c0;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c1;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c2;
            p[off++] = a++;
            p[off++] = b;
            b++;
            p[off++] = c3;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c0;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c1;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c2;
            p[off++] = a++;
            p[off++] = b;
            b++;
            p[off++] = c3;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c0;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c1;
            p[off++] = a++;
            p[off++] = b;
            p[off++] = c2;
            p[off++] = a++;
            p[off++] = b;
            b++;
            p[off++] = c3;
        }
        // Zero-fill the record tail up to the 0x300 boundary; the same
        // running offset drives this loop, matching retail's register flow.
        for (; off < 0x300; off++) {
            p[off] = 0;
        }
        recOff += 0x300;
        blocks += 0x4C;
    }
    DCStoreRange(bufBase, tail->itemCount * 0x300);
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
    f32 w[56];                    // 0x2D00 weight table (through 0x2DE0)
    f32 lod0[3];                  // 0x2DE0
    f32 lod1[4];                  // 0x2DEC..0x2DF8
    u8 gap[0x2E04 - 0x2DFC];
    s32 field_0x2E04;             // layer colour mode
    u8 gap2[0x2E10 - 0x2E08];
    u8 field_0x2E10;
    u16 field_0x2E12;
};

void mpfsys::UnkClass_80471EC8::func_804724DC() {
    // Anchor-row Y heights as ints: retail converts these at runtime
    // (li/xoris/magic-double path), never folding them to float literals.
    const u32* zw = (const u32*)&ml::CVec3::zero;
    MpfsysLayerRec* rec = (MpfsysLayerRec*)this;
    // Anchor-row heights as int locals: MWCC emits the runtime int->double
    // conversion (xoris/slot-store/lfd/fsubs) rather than folding literals.
    int row0 = 0;
    int row1 = 1;
    int row2 = 2;
    for (int i = 0; i < 0x40; i++) {
        rec->f00.x = lbl_eu_8066A744;
        rec->f00.y = (f32)row0;
        rec->f00.z = lbl_eu_8066A744;
        // Zero-vector copies: retail hoists the +4 word load into a temp
        // ahead of the +0 load/store pair, so mirror that evaluation order.
        u32 z1a = zw[1];
        rec->f24.w[0] = zw[0];
        rec->f24.w[1] = z1a;
        rec->f24.w[2] = zw[2];
        rec->f0C.x = lbl_eu_8066A744;
        rec->f0C.y = (f32)row1;
        rec->f0C.z = lbl_eu_8066A744;
        u32 z1b = zw[1];
        rec->f30.w[0] = zw[0];
        rec->f30.w[1] = z1b;
        rec->f30.w[2] = zw[2];
        rec->f18.x = lbl_eu_8066A744;
        rec->f18.y = (f32)row2;
        rec->f18.z = lbl_eu_8066A744;
        u32 z1c = zw[1];
        rec->f3C.w[0] = zw[0];
        rec->f3C.w[1] = z1c;
        rec->f3C.w[2] = zw[2];
        PSMTXIdentity(rec->mtxA);
        PSMTXIdentity(rec->mtxB);
        // Direction vector: two random draws scaled into [-c, c]. The first
        // draw lands on Z and is held in an FPR across the second call.
        f32 dz = lbl_eu_8066A748 * (f32)(rand() % 100) - lbl_eu_8066A74C;
        rec->dir.x = lbl_eu_8066A748 * (f32)(rand() % 100) - lbl_eu_8066A74C;
        rec->dir.y = lbl_eu_8066A744;
        rec->dir.z = dz;
        if (rec->dir.x == lbl_eu_8066A744 && rec->dir.y == lbl_eu_8066A744 &&
            rec->dir.z == lbl_eu_8066A744) {
            rec->dir.x = lbl_eu_8066A744;
            rec->dir.y = lbl_eu_8066A744;
            rec->dir.z = lbl_eu_8066A750;
        } else {
            f32 lenSq = rec->dir.z * rec->dir.z +
                        (rec->dir.x * rec->dir.x + rec->dir.y * rec->dir.y);
            if (lenSq == lbl_eu_8066A744) {
                u32 z1d = zw[1];
                rec->dir.w[0] = zw[0];
                rec->dir.w[1] = z1d;
                rec->dir.w[2] = zw[2];
            } else {
                PSVECNormalize((Vec*)&rec->dir, (Vec*)&rec->dir);
            }
        }
        rec++;
    }

    // Reset the shared layer-state tail: weight table, LOD constants and the
    // colour-mode fields. The memset below clears w[0..7], so only the
    // surviving weights are stored.
    MpfsysInitLayout* st = (MpfsysInitLayout*)this;
    for (int i = 8; i < 56; i++) {
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

// Scale one RGBA f32 quad by `scale` and truncate each component to a byte.
// Written as a by-value-returning helper so MWCC inlines it with the retail
// return-temp shape (bytes built in a local, returned in r3).
static inline GXColor ColorFromQuad(const f32* quad, f32 scale) {
    GXColor c;
    c.r = (u8)(scale * quad[0]);
    c.g = (u8)(scale * quad[1]);
    c.b = (u8)(scale * quad[2]);
    c.a = (u8)(scale * quad[3]);
    return c;
}

void func_804723A4__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, GXColor* outMat, GXColor* outFog) {
    // Build two GXColors from the animation object's colour quads; both alphas
    // are forced to 0xFF only after both colours have been copied out.
    MpfsysResState* s = (MpfsysResState*)self;
    MpfsysAnimObj64* obj = (MpfsysAnimObj64*)s->field_0x2E08;
    MpfsysColorSrcPtr* src = (MpfsysColorSrcPtr*)obj->field_0x64;
    MpfsysColorBlock* cb = (MpfsysColorBlock*)src->field_0x8;
    f32 scale = lbl_eu_8066A740;

    // Copies are named ahead of their sources so MWCC places each colour copy
    // above its byte-built source on the stack (retail layout); the copy
    // itself goes through a u32 pun so no operator= call is emitted.
    GXColor matC;
    GXColor fogC;
    GXColor mat;
    GXColor fog;

    mat.r = (u8)(scale * cb->quad0[0]);
    mat.g = (u8)(scale * cb->quad0[1]);
    mat.b = (u8)(scale * cb->quad0[2]);
    mat.a = (u8)(scale * cb->quad0[3]);
    *(u32*)&matC = *(u32*)&mat;
    outMat->r = matC.r;
    outMat->g = matC.g;
    outMat->b = matC.b;
    outMat->a = matC.a;

    fog.r = (u8)(scale * cb->quad1[0]);
    fog.g = (u8)(scale * cb->quad1[1]);
    fog.b = (u8)(scale * cb->quad1[2]);
    fog.a = (u8)(scale * cb->quad1[3]);
    *(u32*)&fogC = *(u32*)&fog;
    outFog->r = fogC.r;
    outFog->g = fogC.g;
    outFog->b = fogC.b;
    outFog->a = fogC.a;

    outMat->a = 0xFF;
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
    // Trivial POD view of the shared zero vector so copies stay inline word
    // copies (retail never emits a struct assignment operator call).
    const u32* zw = (const u32*)&ml::CVec3::zero;
    f32 dt = st->field_2DEC * arg;
    f32 scaledDt = dt;
    MpfsysVec3* P = &rec->f00;  // chain points   (+0x00, +0x0C, +0x18)
    MpfsysVec3* Q = &rec->f24;  // target points  (+0x24, +0x30, +0x3C)

    // When amplitude is at its floor and dt is exactly 1, soften the
    // constraint response; the gate reads the PREVIOUS frame's tip target
    // Q[2] (still holding its integrated value at this point).
    if (st->field_2DE8 <= lbl_eu_8066A758 && arg == lbl_eu_8066A750) {
        f32 lsq = Q[2].z * Q[2].z + (Q[2].x * Q[2].x + Q[2].y * Q[2].y);
        if (lsq <= lbl_eu_8066A774) {
            scaledDt = dt * lbl_eu_8066A74C;
        }
    }

    // Randomised jitter: wobble scalar from draw 1, Z jitter from draw 2,
    // Y jitter from draw 3; the X component always adds zero.
    f32 wob = st->field_2DE8 * (lbl_eu_80663850 * (f32)(rand() % 500));
    f32 jz = st->field_2DEC *
             (lbl_eu_8066A778 *
              (lbl_eu_80663850 * (f32)(rand() % 500) - lbl_eu_8066A74C));
    rec->dir.x += lbl_eu_8066A744;
    rec->dir.z += jz;
    f32 jy = st->field_2DEC *
             (lbl_eu_8066A778 *
              (lbl_eu_80663850 * (f32)(rand() % 500) - lbl_eu_8066A74C));
    rec->dir.y += jy;

    {
        f32 lsq = rec->dir.z * rec->dir.z +
                  (rec->dir.x * rec->dir.x + rec->dir.y * rec->dir.y);
        if (lsq == lbl_eu_8066A744) {
            u32 z1 = zw[1];
            rec->dir.w[0] = zw[0];
            rec->dir.w[1] = z1;
            rec->dir.w[2] = zw[2];
        } else {
            PSVECNormalize((Vec*)&rec->dir, (Vec*)&rec->dir);
        }
    }

    MpfsysVec3 vel;
    vel.x = rec->dir.x * wob;
    vel.y = rec->dir.y * wob;
    vel.z = rec->dir.z * wob;

    // Loop-pinned sdata2 constants: reading them into locals lets MWCC hold
    // them in callee-saved FPRs across the PSVEC calls (retail behaviour).
    const f32 rest = lbl_eu_8066A750;
    const f32 gain = lbl_eu_8066A778;
    const f32 eps = lbl_eu_8066A774;
    const f32 damp = lbl_eu_8066A74C;

    // Distance constraints between consecutive chain points: the correction
    // blends the strain term with the dot of the P-difference and the
    // Q-difference, all scaled by the softened step.
    for (int i = 0; i < 2; i++) {
        MpfsysVec3 d;
        d.x = P[i].x - P[i + 1].x;
        d.y = P[i].y - P[i + 1].y;
        d.z = P[i].z - P[i + 1].z;
        f32 mag = PSVECMag((Vec*)&d);
        f32 strain = mag - rest;
        MpfsysVec3 e;
        e.x = Q[i].x - Q[i + 1].x;
        e.y = Q[i].y - Q[i + 1].y;
        e.z = Q[i].z - Q[i + 1].z;
        f32 dot = d.x * e.x + d.y * e.y + d.z * e.z;
        f32 inv = (mag >= eps) ? rest / mag : lbl_eu_8066A77C;
        f32 k = -(gain * strain + damp * dot * inv) * scaledDt;
        MpfsysVec3 push;
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
        if (mag >= eps) {
            f32 inv = rest / mag;
            f32 k = lbl_eu_8066A780 * mag * scaledDt;
            Q[j].x += v.x * inv * k;
            Q[j].y += v.y * inv * k;
            Q[j].z += v.z * inv * k;
        }
    }

    // Tip velocity term lands in Q[2]; the randomised drift (draws mod 100
    // offset by 100) folds into Q[1], while Q[2] shrinks toward the origin.
    f32 vk = lbl_eu_8066A784 * scaledDt;
    f32 dx = vel.x * vk;
    f32 dy = vel.y * vk;
    f32 dz = vel.z * vk;
    dx *= lbl_eu_80663854 * (f32)(rand() % 100 + 100);
    dy *= lbl_eu_80663854 * (f32)(rand() % 100 + 100);
    f32 shrink = lbl_eu_8066A750 - lbl_eu_8066A788 * dt;
    dz *= lbl_eu_80663854 * (f32)(rand() % 100 + 100);
    Q[1].x += dx;
    Q[1].y += dy;
    Q[1].z += dz;

    // Clamp Q[1] into [min, max] (all highs first, then all lows).
    if (Q[1].x > lbl_eu_8066A758) {
        Q[1].x = lbl_eu_8066A758;
    }
    if (Q[1].y > lbl_eu_8066A758) {
        Q[1].y = lbl_eu_8066A758;
    }
    if (Q[1].z > lbl_eu_8066A758) {
        Q[1].z = lbl_eu_8066A758;
    }
    if (Q[1].x < lbl_eu_8066A78C) {
        Q[1].x = lbl_eu_8066A78C;
    }
    if (Q[1].y < lbl_eu_8066A78C) {
        Q[1].y = lbl_eu_8066A78C;
    }
    if (Q[1].z < lbl_eu_8066A78C) {
        Q[1].z = lbl_eu_8066A78C;
    }

    Q[2].x *= shrink;
    Q[2].y *= shrink;
    Q[2].z *= shrink;
    if (Q[2].x > lbl_eu_8066A758) {
        Q[2].x = lbl_eu_8066A758;
    }
    if (Q[2].y > lbl_eu_8066A758) {
        Q[2].y = lbl_eu_8066A758;
    }
    if (Q[2].z > lbl_eu_8066A758) {
        Q[2].z = lbl_eu_8066A758;
    }
    if (Q[2].x < lbl_eu_8066A78C) {
        Q[2].x = lbl_eu_8066A78C;
    }
    if (Q[2].y < lbl_eu_8066A78C) {
        Q[2].y = lbl_eu_8066A78C;
    }
    if (Q[2].z < lbl_eu_8066A78C) {
        Q[2].z = lbl_eu_8066A78C;
    }

    // Integrate chain points by the scaled targets.
    P[1].x += Q[1].x * dt;
    P[1].y += Q[1].y * dt;
    P[1].z += Q[1].z * dt;
    P[2].x += Q[2].x * dt;
    P[2].y += Q[2].y * dt;
    P[2].z += Q[2].z * dt;

    // Re-derive both orientation quaternions from the point chain (tip-first
    // for the A matrix, mid-first for B). Retail inlines this block twice,
    // so keep it duplicated.
    Quaternion quat;
    {
        MpfsysVec3 n;
        n.x = P[2].x - P[0].x;
        n.y = P[2].y - P[0].y;
        n.z = P[2].z - P[0].z;
        f32 lsq = n.z * n.z + (n.x * n.x + n.y * n.y);
        if (lsq == lbl_eu_8066A744) {
            u32 z1 = zw[1];
            n.w[0] = zw[0];
            n.w[1] = z1;
            n.w[2] = zw[2];
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
            PSVECCrossProduct((Vec*)&ml::CVec3::unitY, (Vec*)&n, &axis);
            f32 m = lbl_eu_8066A73C * (lbl_eu_8066A750 + dot);
            if (m < lbl_eu_8066A744) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                             lbl_eu_80526300);
            }
            // Degenerate case falls back to m itself (not zero).
            f32 s4 = m;
            if (m > lbl_eu_8066A744) {
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
            u32 z1 = zw[1];
            n.w[0] = zw[0];
            n.w[1] = z1;
            n.w[2] = zw[2];
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
            PSVECCrossProduct((Vec*)&ml::CVec3::unitY, (Vec*)&n, &axis);
            f32 m = lbl_eu_8066A73C * (lbl_eu_8066A750 + dot);
            if (m < lbl_eu_8066A744) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                             lbl_eu_80526300);
            }
            // Degenerate case falls back to m itself (not zero).
            f32 s4 = m;
            if (m > lbl_eu_8066A744) {
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
    u32 z1 = zw[1];
    P[0].w[0] = zw[0];
    P[0].w[1] = z1;
    P[0].w[2] = zw[2];
    u32 z2 = zw[1];
    Q[0].w[0] = zw[0];
    Q[0].w[1] = z2;
    Q[0].w[2] = zw[2];
}

// Randomised animation-state overlay for the tail of UnkClass_80471EC8
// (offsets recovered from func_80473394).
struct MpfsysRandState {
    u8 pad[0x2DE0];
    f32 field_0x2DE0;
    f32 field_0x2DE4;
    f32 field_0x2DE8;
    f32 field_0x2DEC;
    f32 field_0x2DF0;
    f32 field_0x2DF4;
    f32 field_0x2DF8;
};

// Scene-side time-step provider (returns a scalar in f1).
extern "C" f32 func_80496288(void* scene);

#pragma push
#pragma auto_inline off
void mpfsys::UnkClass_80471EC8::func_80473394() {
    MpfsysResState* res = (MpfsysResState*)this;
    MpfsysRandState* st = (MpfsysRandState*)this;

    // Decay the shared scale by the scene-provided step; when it bottoms out,
    // re-randomise both the scale and the wobble amplitude.
    f32 decay = st->field_0x2DF0 * func_80496288(res->field_0x2E08);
    st->field_0x2DEC = decay;
    f32 next = st->field_0x2DF8 - decay;
    st->field_0x2DF8 = next;
    if (next <= lbl_eu_8066A744) {
        f32 t = (f32)(rand() % 1000) / lbl_eu_8066A794;
        st->field_0x2DF8 = st->field_0x2DF4 * t;
        f32 u = (f32)(rand() % 20) / lbl_eu_8066A798;
        st->field_0x2DE8 = st->field_0x2DE0 * u + st->field_0x2DE4;
    }

    // Step all 64 layer records: the first half at unit rate, the second half
    // (base + 0x1680) at the scaled rate.
    MpfsysLayerRec* rec = (MpfsysLayerRec*)this;
    for (int i = 0; i < 0x20; i++) {
        func_804728E8__Q26mpfsys17UnkClass_80471EC8Fv(
            this, rec, lbl_eu_8066A750);
        rec++;
    }
    rec = (MpfsysLayerRec*)((u8*)this + 0x1680);
    for (int i = 0x20; i < 0x40; i++) {
        func_804728E8__Q26mpfsys17UnkClass_80471EC8Fv(
            this, rec, lbl_eu_8066A778);
        rec++;
    }
}
#pragma pop

void* mpfsys::UnkClass_80471EC8::func_804734F4(u8 layerIndex) {
    return (u8*)this + layerIndex * 0x1680;
}

// Uploads the texture for display-list entry `texIndex` to texMap `texMap`.
// CI-format textures get their palette loaded as a TLUT first; mipmapped
// textures receive LOD parameters derived from `lodBias`.

// Single-pointer resource handles (nw4r::g3d::ResCommon<T> wrappers are one
// pointer wide; the trivial members below are written as direct field accesses
// so they inline exactly like the retail build).
struct MpfsysResTexHandle {
    nw4r::g3d::ResTexData* mpData;
};
struct MpfsysResPlttHandle {
    nw4r::g3d::ResPlttData* mpData;
};

// Out-of-line nw4r g3d accessors (retail-named; see the CScnMaruShadowNw4r
// precedent - this build does not auto-inline the header wrappers).
extern "C" void* GetResTex__Q34nw4r3g3d7ResFileCFi(void* resFile, int idx);
extern "C" void* GetParent__Q34nw4r3g3d6ResTexFv(MpfsysResTexHandle* tex);
extern "C" void* GetResPltt__Q34nw4r3g3d7ResFileCFPCc(MpfsysResFileHandle* resFile,
                                                      const char* name);
extern "C" bool GetTexObjCIParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP11_GXCITexFmtPfPfPUc(
    MpfsysResTexHandle* tex, void** ppTexData, u16* pWidth, u16* pHeight,
    GXCITexFmt* pFormatCI, f32* pMinLod, f32* pMaxLod, u8* pMipMap);
extern "C" bool GetTexObjParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP9_GXTexFmtPfPfPUc(
    MpfsysResTexHandle* tex, void** ppTexData, u16* pWidth, u16* pHeight,
    GXTexFmt* pFormat, f32* pMinLod, f32* pMaxLod, u8* pMipMap);

void func_80473500__Q26mpfsys17UnkClass_80471EC8Fiif(int texIndex, int texMap, f32 lodBias) {
    // Resolve the active map's texture dictionary entry.
    MpfsysResTexHandle tex;
    tex.mpData = (nw4r::g3d::ResTexData*)GetResTex__Q34nw4r3g3d7ResFileCFi(
        lbl_eu_80665848, texIndex);
    if (tex.mpData == NULL) {
        nw4r::db::Panic(lbl_eu_8052CF10, 0x26, lbl_eu_8052CEF4, lbl_eu_80661E98,
                        lbl_eu_8066385C);
    }

    GXTexObj texObj;
    f32 minLod;
    f32 maxLod;
    u8 mipmap;
    if (tex.mpData->flag & nw4r::g3d::ResTexData::FLAG_CIFMT) {
        if (tex.mpData == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF10, 0x26, lbl_eu_8052CEF4, lbl_eu_80661E98,
                            lbl_eu_8066385C);
        }
        // Palette name: relative offset resolved against the tex data (null
        // when the offset is 0).
        const char* plttName = (tex.mpData->name != 0)
                                   ? (const char*)tex.mpData + tex.mpData->name
                                   : NULL;

        void* imgData;
        GXCITexFmt ciFmt;

        u16 width;
        u16 height;
        MpfsysResFileHandle parent;
        parent.mpData = GetParent__Q34nw4r3g3d6ResTexFv(&tex);
        MpfsysResPlttHandle pltt;
        pltt.mpData = (nw4r::g3d::ResPlttData*)GetResPltt__Q34nw4r3g3d7ResFileCFPCc(
            &parent, plttName);
        GetTexObjCIParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP11_GXCITexFmtPfPfPUc(
            &tex, &imgData, &width, &height, &ciFmt, &minLod, &maxLod, &mipmap);

        // C14X2 entries use the 0x10-entry TLUT slot.
        u32 tlutName = 0;
        if (ciFmt == (GXCITexFmt)0xA) {
            tlutName = 0x10;
        }

        // Palette entries / format / data offset, each access guarded by an
        // assertion on the resolved ResPltt.
        if (pltt.mpData == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2A, lbl_eu_8052CF50, lbl_eu_80661EA8,
                            lbl_eu_80663864);
        }
        u16 numEntries = pltt.mpData->numEntries;
        if (pltt.mpData == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2A, lbl_eu_8052CF50, lbl_eu_80661EA8,
                            lbl_eu_80663864);
        }
        GXTlutFmt plttFmt = pltt.mpData->fmt;
        if (pltt.mpData == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF3C, 0x2A, lbl_eu_8052CF20, lbl_eu_80661EA8,
                            lbl_eu_80663860);
        }
        u32 tblOfs = pltt.mpData->toPlttData;
        const void* table = (tblOfs != 0)
                                ? (const char*)pltt.mpData + tblOfs
                                : NULL;

        GXTlutObj tlutObj;
        GXInitTlutObj(&tlutObj, (void*)table, plttFmt, numEntries);
        GXLoadTlut(&tlutObj, tlutName);
        GXInitTexObjCI(&texObj, imgData, width, height, (GXTexFmt)ciFmt,
                       (GXTexWrapMode)1, (GXTexWrapMode)1, (GXBool)mipmap,
                       tlutName);
    } else {
        void* imgData;
        u16 width;
        u16 height;
        GXTexFmt fmt;
        GetTexObjParam__Q34nw4r3g3d6ResTexCFPPvPUsPUsP9_GXTexFmtPfPfPUc(
            &tex, &imgData, &width, &height, &fmt, &minLod, &maxLod, &mipmap);
        GXInitTexObj(&texObj, imgData, width, height, fmt, (GXTexWrapMode)1,
                     (GXTexWrapMode)1, (GXBool)mipmap);
    }

    if (mipmap != 0) {
        GXInitTexObjLOD(&texObj, (GXTexFilter)5, (GXTexFilter)1, minLod, maxLod,
                        lodBias, GX_FALSE, GX_FALSE, (GXAnisotropy)0);
    } else {
        f32 lod = lbl_eu_8066A7A4;
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, lod, lod, lod, GX_FALSE,
                        GX_FALSE, (GXAnisotropy)0);
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

// Display-list entry table shape: { count, offset } pairs indexed by the
// caller; the offset is relative to the UnkClass_80471EC8 instance held in
// lbl_eu_80665840, and each item is a 4-byte record whose low s16 is a
// texture index.
struct MpfsysTevItem {
    u16 id;
    u16 field_0x2;
};
struct MpfsysTevEntry {
    u32 count;
    u32 ofs;
};

void func_804737F0__Q26mpfsys17UnkClass_80471EC8Fv(s32 index, f32 scale) {
    // Per-layer textured-draw setup: invalidate the cached texture index,
    // lazily install the fullscreen TEV/vertex-desc state for mode 6, then
    // feed every item of display-list entry `index` to func_80473500 with
    // the given scale.
    lbl_eu_80665858 = -1;
    // The cached-index store above may alias the entry table, so the count
    // is re-read from the table rather than merged with the offset load.
    MpfsysTevItem* items =
        (MpfsysTevItem*)((u8*)lbl_eu_80665840 +
                         ((MpfsysTevEntry*)&lbl_eu_80665868)[index].ofs);
    u32 count = ((MpfsysTevEntry*)&lbl_eu_80665868)[index].count;
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
    // Sub-block pointers and leading source words are cached across the
    // PSMTXInverse / PSMTXMultVec calls; keeping these live matches the
    // retail callee-saved register allocation (r21-r25/r31).
    ml::CVec3* trans = &cam->trans;
    ml::CVec3* axis = &cam->axis;
    MpfsysMtxWords* rot = &cam->rot;
    // Leading source words cached across the PSMTXInverse call; retail keeps
    // these four in callee-saved registers and replays them for the
    // rotation-only copy below.
    u32 w0 = src->w[0];
    u32 w1 = src->w[1];
    u32 w2 = src->w[2];
    u32 w3 = src->w[3];

    // Full copy of the source view matrix into cam->inv, inverted in place.
    for (int i = 0; i < 12; i++) {
        cam->inv.w[i] = src->w[i];
    }
    lbl_eu_80665848 = texBase;
    PSMTXInverse(cam->inv.m, cam->inv.m);

    // Rotation-only copy: the cached leading words are replayed, and the
    // translation column is cleared with the shared zero constant.
    rot->w[0] = w0;
    rot->w[1] = w1;
    rot->w[2] = w2;
    rot->w[3] = w3;
    for (int i = 4; i < 12; i++) {
        rot->w[i] = src->w[i];
    }
    rot->m[0][3] = lbl_eu_8066A7A4;
    rot->m[1][3] = lbl_eu_8066A7A4;
    rot->m[2][3] = lbl_eu_8066A7A4;

    // Translation column of the source matrix, then billboard axis (0,0,1)
    // pushed through the rotation part.
    const f32* pf = (const f32*)srcMtx;
    trans->x = pf[3];
    trans->y = pf[7];
    trans->z = pf[11];
    axis->x = lbl_eu_8066A7A4;
    axis->y = lbl_eu_8066A7A4;
    axis->z = lbl_eu_8066A7AC;
    PSMTXMultVec(cam->rot.m, (Vec*)axis, (Vec*)axis);

    f32 lenSq = axis->z * axis->z +
                (axis->x * axis->x + axis->y * axis->y);
    if (lenSq == lbl_eu_8066A7A4) {
        // Component-wise copy keeps the shared zero-vector loads inline
        // (retail never calls the CVec3 assignment operator here).
        const ml::CVec3& z = ml::CVec3::zero;
        axis->x = z.x;
        axis->y = z.y;
        axis->z = z.z;
    } else {
        PSVECNormalize((Vec*)axis, (Vec*)axis);
    }

    GXSetCurrentMtx(0);
    GXLoadPosMtxImm(cam->inv.m, 0);
    lbl_eu_80665840 = self;

    f32 lenSq2 = axis->x * axis->x + axis->y * axis->y +
                 axis->z * axis->z;

    // Refresh the display-list pointer table via the two-level offset chase.
    u8* base = (u8*)self;
    lbl_eu_80665844 = base + *(u32*)(base + *(u32*)(base + 0x1C));
    lbl_eu_8066584C[0].base = base + *(u32*)(base + *(u32*)(base + 0x24));
    lbl_eu_80665850 = (u16*)(base + *(u32*)(base + *(u32*)(base + 0xC)));
    *(u32*)&lbl_eu_80665868 = (u32)(base + *(u32*)(base + *(u32*)(base + 0x2C)));
    lbl_eu_80665880 = lbl_eu_8066A7B0 / lenSq2;
    // FOV: tan of the scaled half-angle; its scaled store lands right after
    // the call, ahead of GXClearVtxDesc (retail schedule).
    f32 t = (f32)tan(lbl_eu_8066A7B8 *
                     (lbl_eu_8066A7BC * (lbl_eu_8066A7B4 * (fovScale * lbl_eu_8066A210))));
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

void mpfsys::UnkClass_80471EC8::func_804744EC() {
    // Fullscreen TEV two-stage setup (idempotent per state 2).
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
    // Fullscreen TEV two-stage setup with a texture order on stage 1
    // (idempotent per state 4).
    if (lbl_eu_8066585A == 4) {
        return;
    }
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)2);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)2);
    GXSetTevOrder((GXTevStageID)1, (GXTexCoordID)0xFF, (GXTexMapID)0xFF, (GXChannelID)4);
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
    // Fullscreen TEV stage-0 / vertex-desc setup (idempotent per state 5);
    // position attribute arrives as direct u16 pairs like the texcoord.
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
    f32 diag;
    if (scale == zero) {
        diag = lbl_eu_8066A7B0;
    } else {
        // Scale factor grows with the masked index sum (converted through the
        // shared unsigned int -> double helper constants).
        u32 v = ((u32)idx + layerIdx) & lbl_eu_8066A72C;
        diag = lbl_eu_8066A7B0 + scale * (f32)(((f64)v - lbl_eu_8066A7C8) * lbl_eu_8066A730);
    }
    // Store order mirrors the retail schedule: off-diagonal zeros first,
    // then the diagonal, then row 2.
    m[0][1] = zero;
    m[0][2] = zero;
    m[0][3] = zero;
    m[1][0] = zero;
    m[1][2] = zero;
    m[1][3] = zero;
    m[0][0] = diag;
    m[1][1] = diag;
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

// Map-object overlay consumed by func_80474064: +0x24 holds the offset to
// the display-list header record.
struct MpfsysDrawSrc {
    u8 field_0x0[0x24];
    u32 field_0x24; // offset from the object to the display-list header
};
// Display-list header record reached through field_0x24: base offset for the
// view-slot table and an auxiliary offset for the palette/index table.
struct MpfsysDlHead {
    u32 field_0x0; // offset from the object to the display-list base
    u8 field_0x4[0xC - 0x4];
    u32 field_0xC; // offset from the object to the index table
};
// Plain matrix wrapper: whole-struct assignment compiles to inline word
// copies (the union variant emitted an out-of-line operator= call).
struct MpfsysMtxPair {
    Mtx m;
};
// View overlay for the shared scratch camera block (same layout as
// MpfsysCamBlock).
struct MpfsysCamView {
    ml::CVec3 trans; // +0x00 translation extracted from the view matrix
    ml::CVec3 axis;  // +0x0C billboard axis (transformed in place)
    MpfsysMtxPair rot; // +0x18 rotation-only copy of the view matrix
    MpfsysMtxPair inv; // +0x48 inverted view matrix
};

// Layer-draw push, retail member of UnkClass_80471EC8: Fv-annotated but
// actually receives (self, frame+0x9C, a5, a6, f1) from func_80471FCC.
// Inverts a copy of the view-frame matrix, derives the billboard axis by
// transforming the constant (0, 0, 1) vector through the rotation part,
// then publishes the FOV scale factors and display-list pointers.
void func_80474064__Q26mpfsys17UnkClass_80471EC8Fv(
    mpfsys::UnkClass_80471EC8* self, void* viewMtx, void* a5, void* a6,
    f32 fovScale) {
    MpfsysCamBlock* cam = (MpfsysCamBlock*)lbl_eu_80658410;
    MpfsysMtxWords* src = (MpfsysMtxWords*)viewMtx;
    MpfsysMtxWords* inv = &cam->inv;

    // Cleared render-flags value published below; retail materializes it in
    // a callee-saved register ahead of the matrix-word loads.
    u32 flags = 0;

    // Buffer the source matrix words: retail batches all twelve loads ahead
    // of the render-global stores.
    u32 w0 = src->w[0];
    u32 w1 = src->w[1];
    u32 w2 = src->w[2];
    u32 w3 = src->w[3];
    u32 w4 = src->w[4];
    u32 w5 = src->w[5];
    u32 w6 = src->w[6];
    u32 w7 = src->w[7];
    u32 w8 = src->w[8];
    u32 w9 = src->w[9];
    u32 w10 = src->w[10];
    u32 w11 = src->w[11];

    lbl_eu_8066586C = flags;
    lbl_eu_80665874 = (u32)a6;
    lbl_eu_80665870 = (u32)a5;

    // Invert a copy of the source view matrix into cam->inv.
    inv->w[0] = w0;
    inv->w[1] = w1;
    inv->w[2] = w2;
    inv->w[3] = w3;
    inv->w[4] = w4;
    inv->w[5] = w5;
    inv->w[6] = w6;
    inv->w[7] = w7;
    inv->w[8] = w8;
    inv->w[9] = w9;
    inv->w[10] = w10;
    inv->w[11] = w11;
    PSMTXInverse(inv->m, inv->m);

    // Rotation-only copy (translation column zeroed).
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
    cam->rot.m[0][3] = lbl_eu_8066A7A4;
    cam->rot.m[1][3] = lbl_eu_8066A7A4;
    cam->rot.m[2][3] = lbl_eu_8066A7A4;

    // Translation column of the source matrix, then billboard axis (0,0,1)
    // transformed in place through the rotation part.
    cam->trans.x = src->m[0][3];
    cam->trans.y = src->m[1][3];
    cam->trans.z = src->m[2][3];
    cam->axis.x = lbl_eu_8066A7A4;
    cam->axis.y = lbl_eu_8066A7A4;
    cam->axis.z = lbl_eu_8066A7AC;
    PSMTXMultVec(cam->rot.m, (Vec*)&cam->axis, (Vec*)&cam->axis);

    f32 lenSq = cam->axis.z * cam->axis.z +
                (cam->axis.x * cam->axis.x + cam->axis.y * cam->axis.y);
    if (lenSq == lbl_eu_8066A7A4) {
        // Component-wise copy keeps the shared zero-vector loads inline
        // (retail never calls the CVec3 assignment operator here).
        cam->axis.x = ml::CVec3::zero.x;
        cam->axis.y = ml::CVec3::zero.y;
        cam->axis.z = ml::CVec3::zero.z;
    } else {
        PSVECNormalize((Vec*)&cam->axis, (Vec*)&cam->axis);
    }

    // FOV: tan of the scaled half-angle, plus the inverse-square term.
    f32 lenSq2 = cam->axis.x * cam->axis.x + cam->axis.y * cam->axis.y +
                 cam->axis.z * cam->axis.z;
    f32 tanArg = lbl_eu_8066A7B8 *
                 (lbl_eu_8066A7BC * (lbl_eu_8066A7B4 * (fovScale * lbl_eu_8066A210)));
    lbl_eu_80665880 = lbl_eu_8066A7B0 / lenSq2;
    f32 t = (f32)tan(tanArg);

    lbl_eu_80665840 = self;
    *(f32*)&lbl_eu_80665884 = lbl_eu_8066A7C0 * t;

    // Refresh the display-list pointer table via the two-level offset chase:
    // field_0x24 points at a header whose entries are offsets from the object
    // itself. Also publish the instance flag-array pointer.
    u8* obj = (u8*)self;
    MpfsysDlHead* head =
        (MpfsysDlHead*)(obj + ((MpfsysDrawSrc*)obj)->field_0x24);
    lbl_eu_8066584C[0].base = obj + head->field_0x0;
    lbl_eu_80665850 = (u16*)(obj + head->field_0xC);
    lbl_eu_80665864 = (u32)((u8*)lbl_eu_80665838 + 0x2D00);
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
char lbl_eu_8066385C[4] = {'r', 'e', 'f', '\0'};
char lbl_eu_80663860[4] = {'r', 'e', 'f', '\0'};
char lbl_eu_80663864[4] = {'r', 'e', 'f', '\0'};

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

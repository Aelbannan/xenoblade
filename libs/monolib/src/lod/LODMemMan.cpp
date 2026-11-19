// Auto-scaffolded catalog TU for monolib/src/lod/LODMemMan
// Replace stubs with high-level C/C++ during decomp.

// Retail elides the empty IWorkEvent base-dtor call in LODMemMan's out-of-line
// destructor (0x40 bytes, no base call); the inline-empty base dtor is the
// sanctioned shape for that (cf. CMCCrystalSupport in MONOLIB).  Must precede
// every include: harness_catalog.h already pulls in IWorkEvent.hpp.
#define IWORK_EVENT_INLINE_DTOR
#include <harness_catalog.h>
#include "monolib/lod/UnkClass_8046D144.hpp"
#include "monolib/core/CView.hpp"           // CView::getCurrentView
#include "monolib/core/code_804E36DC.hpp"   // Scn_GetFrameDelta (frame delta, C ABI)
#include "nw4r/g3d/g3d_obj.h"               // nw4r::g3d::G3dObj::Destroy
#include <string.h>                         // strlen / strcpy
#include <math.h>                           // tan
#include "monolib/util/MemManager.hpp"      // mtl::MemManager::deallocate
#include "monolib/math/Random.hpp"          // ml::math::mtRand
#include "monolib/device/CDeviceFile.hpp"   // CDeviceFile::cancel
#include <nw4r/g3d/g3d_scnobj.h>            // nw4r::g3d::ScnGroup (Remove at vtable+0x3C)
#include "monolib/work/CEventFile.hpp"   // CEventFile (global) - file-event record
#include <nw4r/g3d/res/g3d_resfile.h>       // nw4r::g3d::ResFile::Init
#include <nw4r/g3d/res/g3d_resmdl.h>        // nw4r::g3d::ResMdl texture-palette queries
#include <nw4r/g3d/g3d_scnmdl.h>           // nw4r::g3d::ScnMdl::Construct
#include <nw4r/math/math_types.h>           // nw4r::math::MTX34
#include <nw4r/math/math_triangular.h>      // SinFIdx / CosFIdx
#include <nw4r/g3d/g3d_scnproc.h>           // nw4r::g3d::ScnProc::Construct
#include <revolution/mtx.h>                 // PSMTXConcat
#include <revolution/GX.h>                  // GXInitLightColor / GXLoadLightObjImm
#include <nw4r/db/db_assert.h>              // nw4r::db::Panic
#include "monolib/scn/code_804BC9EC.hpp"
#include "monolib/scn/CScn.hpp"           // CScn::mItemPool (scene pool walk)

class CScn;  // scene wrapper (forward decl; CScnMem.hpp is not included here)

// The g3d sub-manager's record types.
using LOD::LODChildEntry;
using LOD::LODChildLight;
using LOD::LODG3dMgr;
using LOD::LODG3dSlot;

// ---------------------------------------------------------------------------
// Dissolved retail data (this TU owns these objects).  Bytes/relocs dumped
// from build/us/obj/monolib/src/lod/LODMemMan.o:
//   .rodata 0x18, .data 0x1E0, .sdata 0x20, .sbss 0x38.
// The IWorkEvent-derived LOD::LODMemMan vtable + RTTI records are emitted by
// MWCC in this TU (the out-of-line virtual dtor makes it the key function).
// ---------------------------------------------------------------------------
// .data: Panic file-name string.
extern "C" char lbl_eu_8056DAF8[16] __attribute__((aligned(8))) = "g3d_resmat_ac.h";

// .rodata zeroed reset-name slot ("" seeded into the name buffer by the
// reset/init paths).  Defined at the end of the TU; open item: MWCC emits
// explicit-section zero data first, so the compiler-emitted RTTI class-name
// string lands at +0x8 instead of retail's +0x0.
extern "C" const char lbl_eu_80523D90[8];

// .sdata: sentinel pair {0xFFFF, 0}, 90.0f divisor, three "ref" strings,
// flag word and trailing pad.
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663828[2] = {0x0000FFFF, 0x00000000};
extern "C" __declspec(section ".sdata") f32 lbl_eu_80663830 = 90.0f;
extern "C" __declspec(section ".sdata") const u32 lbl_eu_80663834 = 0;
extern "C" __declspec(section ".sdata") const char lbl_eu_80663838[4] = {'r', 'e', 'f', '\0'};
extern "C" __declspec(section ".sdata") const char lbl_eu_8066383C[4] = {'r', 'e', 'f', '\0'};
extern "C" __declspec(section ".sdata") const char lbl_eu_80663840[4] = {'r', 'e', 'f', '\0'};
extern "C" __declspec(section ".sdata") const u32 gap_09_80663844_sdata = 0;

// .sbss: cached scale / flag byte / shared persistent buffer handle and the
// unreferenced LOD distance cache slots.
extern "C" {
__attribute__((aligned(8))) f32 lbl_eu_80665828;
s8 lbl_eu_8066582C;
u8 gap_10_8066582D_sbss;
u8 gap_10_8066582E_sbss;
u8 gap_10_8066582F_sbss;
int lbl_eu_80665830;
u32 lbl_eu_80665834;
u32 lbl_eu_80665838;
u32 lbl_eu_8066583C;
u32 lbl_eu_80665840;
u32 lbl_eu_80665844;
u32 lbl_eu_80665848;
u32 lbl_eu_8066584C;
u32 lbl_eu_80665850;
u32 lbl_eu_80665854;
u16 lbl_eu_80665858;
u16 lbl_eu_8066585A;
u16 lbl_eu_8066585C;
u16 gap_10_8066585E_sbss;
}



// Retail data labels used by this TU.  Global-scope data imports keep their
// plain unmangled names, so plain extern declarations suffice (no extern "C").
// .bss global flag array: index 0..2 holds "3/4-kind descriptor seen" flags
// (registerChildSlot writes 1 at [index]).
extern int lbl_eu_80658368[];
extern int lbl_eu_80658374[];
extern const char lbl_eu_8052637C[];  // acquireResFileSlot Panic file path
// Float-pool slot for the +0x96C/+0x970 scale pair init (initLightScales).
// Panic message for the ResFile alignment check (acquireResFileSlot).
extern const char lbl_eu_80526354[];

// Runtime helper behind dynamic_cast expressions (used by refreshPaletteBinding's
// scene-model downcast; the retail call passes explicit RTTI locators).
extern "C" void* __dynamic_cast(const void* src, const void* srcType,
                                const void* dstType, long src2dst, int hint);
extern void* lbl_eu_806624C0;   // RTTI locator (static/source class)
extern void* lbl_eu_806624D8;   // RTTI locator (target class)

// Scene-item pool sub-list accessor (scn/CScnItemPool.cpp).
extern "C" void* CScnItemPool_lookupSubPool(void* pool, int kind);

// TU-internal callees defined later in this file.
// allocSecondaryBuffers / adjustBufferOffsets are LOD::LODMemMan members (header).

// refreshPaletteBinding assert strings (.data) + .sdata varargs.
extern const char lbl_eu_8056DA30[];
extern const char lbl_eu_8056DA10[];
extern const char lbl_eu_8056D9C0[];
extern const char lbl_eu_8056DA78[];
extern const char lbl_eu_8056DA48[];
extern const char lbl_eu_8056D9F8[];
extern const char lbl_eu_8056D9D8[];
extern const char lbl_eu_8056DAB8[];
extern const char lbl_eu_8056DA90[];
extern const char lbl_eu_8056DAD8[];
extern const char lbl_eu_8056DAC8[];

// Node of the scene model list walked by refreshPaletteBinding (+0x00 next link,
// +0x08 the registered scene item).
struct LODSceneNode {
    LODSceneNode* mNext;   // 0x00
    u8          mPad04[4]; // 0x04
    void*       mObj;      // 0x08
};
// Slot returned by the scene-item pool accessor (kind 1): sentinel node
// pointer at +0x04; iteration stops back at the sentinel.
struct LODSceneList {
    u8            mPad00[4];
    LODSceneNode* mAnchor; // 0x04
};

// View of the scene item handed to refreshPaletteBinding: only its ResMdl data
// pointer (CScnItemModelNw4r+0x146C) is consumed after the downcast.
struct LODMdlItemView {
    u8                    mPad00[0x146C];
    nw4r::g3d::ResMdlData* field_0x146C; // 0x146C
};

// Shared-buffer texture-name group (refreshPaletteBinding): an offset word at +0x70
// of the buffer selects a { count, pad, u32 offsets... } table whose entries
// point at texture name strings (offsets are relative to the group base).
struct LODTexGroup {
    u32 mCount;              // 0x00 entry count
    u8  mPad04[0x10 - 0x04];
    u32 mOffsets[1];  // 0x10 per-entry name-string offsets from the group base
};

// Light scale/clamp constants (.sdata2 pool).
extern f32 lbl_eu_8066A720;      // light scale clamp constant
// Light colour base: the low byte becomes the alpha; the RGB bytes are
// overwritten per slot.
extern GXColor lbl_eu_8066A71C;

// updateLodFrame per-type update constants (.sdata2 pool).
extern f64 lbl_eu_8066A6E0;   // 2^52 double magic (u8 -> double conversion)
extern f64 lbl_eu_8066A6E8;   // 2^52 double magic variant (rebuildFromSharedBuffer)
extern f64 lbl_eu_8066A6C8;   // double base (random offset addend)
extern f64 lbl_eu_8066A6D0;   // double scale (random offset factor)
extern f32 lbl_eu_8066A6D8;   // scale divisor
extern f32 lbl_eu_8066A6C0;   // 0.0f LOD distance constant
extern f32 lbl_eu_8066A6C4;   // random-value scale (rebuildFromSharedBuffer)
extern f32 lbl_eu_8066A6F8;   // FIdx angle scale (updateElementsByDesc)
// Shared LOD distance globals published by the per-element update
// (updateElementLod writes the float value and its integer truncation).
extern f32 lbl_eu_80665754;
extern s16 lbl_eu_80665758;
// One-time-initialised global scale: the byte flag gates the division
// c11 / lbl_eu_80663830, whose result is cached in lbl_eu_80665828.
extern f32 lbl_eu_8066A70C;
extern f32 lbl_eu_8066A710;
extern f32 lbl_eu_8066A714;


// Recovered LODMemMan field layout.  The shared header only exposes opaque
// padding up to 0x7c plus the 0xabc embedded sub-object, so the fields used
// by the targets below are described here and the object is viewed through
// this struct instead of raw pointer arithmetic.
// ---------------------------------------------------------------------------

// 0x20-byte LOD element.  The flag word at 0x0 is the embedded
// UnkClass_8046A530 view (checkLodFlagState is called with the element pointer).
// The element also participates in an index-linked list: +0x16 holds the next
// element index (sentinel lbl_eu_80663828), +0x1A the element's own index,
// and +0x1C/+0x1E/+0x1F are per-element byte state.
struct LODElem20 {
    u32 field_0x0;                 // 0x00 flag word (UnkClass_8046A530 view)
    f32 field_0x4;                 // 0x04 active value
    f32 field_0x8;                 // 0x08 cached value
    s16 field_0xC;                 // 0x0c LOD distance value
    s16 field_0xE;                 // 0x0e LOD kind (0xff = no kind)
    u8  mPad_10[0x16 - 0x10];      // 0x10..0x15
    u16 field_0x16;                // 0x16 next element index (list link)
    u16 field_0x18;                // 0x18 resolved type index
    s16 field_0x1A;                // 0x1a own element index (list link)
    u8  field_0x1C;                // 0x1c
    u8  field_0x1D;                // 0x1d
    u8  field_0x1E;                // 0x1e
    u8  field_0x1F;                // 0x1f
};

// 0x0C-byte element-list header at the shared-buffer base (initElementList).
struct LODElemList {
    LODElem20* mHead;     // 0x00 first element
    u32        mField_4;  // 0x04 payload word (set by the caller)
    LODElem20* mTail;     // 0x08 second element
};

// 0x18-strided LOD type descriptor (array base at LODMemMan+0x94).  The u16
// at +0x04 gates the element-chain link bits; the f32 at +0x10 is the
// distance threshold used by linkElementToChains.
struct LODTypeDesc18 {
    u32 field_0x0;       // 0x00 resource index
    u16 mFlags;          // 0x04
    u16 field_0x6;       // 0x06 kind (1: direct model, 0x8000 flag on +4: ResFile)
    u8  mPad_08[0x0C - 0x08];
    f32 field_0xC;       // 0x0C activation distance
    f32 mThreshold;      // 0x10
    u8  mPad_14[0x18 - 0x14];
};

// 3-float vector handed to the element update (delta / squared-length input).
struct LODVec3f {
    f32 x;   // 0x00
    f32 y;   // 0x04
    f32 z;   // 0x08
};

// Element linked into the rebuildElementChains chains: each chain slot writes its
// next-index through the u16 field at +0x10+2*slot.
struct LODLinkNode {
    u8  mPad_00[0x10];
    s16 mLinks[3];       // 0x10, 0x12, 0x14 (one per chain slot)
};

// rebuildElementChains view of LODMemMan: the { elem, next } pair array at +0x8 and
// the six list heads at +0x3C / +0x48.
struct LODBuildList {
    u8          mPad_00[0x08];
    LODElem20** mPairs;        // 0x08 { elem, next } pointer pairs
    u8          mPad_0C[0x3C - 0x0C];
    LODElem20*  mHeadClr[3];   // 0x3C..0x44 heads (bit0-clear elems)
    LODElem20*  mHeadSet[3];   // 0x48..0x50 heads (bit0-set elems)
};

// 0x48-byte LOD descriptor.  The u16 fields at 0x40/0x42/0x44 gate the
// per-element updates; the floats at 0x0/0x4/0x8 are the element's base
// position and the byte at 0x46 a per-element parameter.
struct LODElem48 {
    f32 field_0x0;                 // 0x00 base position x
    f32 field_0x4;                 // 0x04 base position y
    f32 field_0x8;                 // 0x08 base position z
    u8  mPad_0C[0x38 - 0xC];       // 0x0c..0x37
    f32 field_0x38;                // 0x38 culling radius (shared-buffer data)
    u8  mPad_3C[0x40 - 0x3C];      // 0x3c..0x3f
    u16 field_0x40;                // 0x40 element id
    u16 field_0x42;                // 0x42 enabled flag (nonzero)
    u16 field_0x44;                // 0x44 flag bits (bit 1: byte param, bit 6: ps source)
    u8  field_0x46;                // 0x46 byte parameter
    u8  field_0x47;                // 0x47 record index into the +0x98 array
};

// 0x74-byte per-type descriptor (array base at LODMemMan+0x9C).  The u16 at
// +0x04 is the referenced-element count, +0x18 the u16-group offset into the
// shared buffer, +0x24 flag bits (bit 0: skip the value update) and the
// floats at +0x40/+0x44 the scale / limit.
struct LODDesc74 {
    u8  mPad_00[0x4];
    u16 field_0x4;             // 0x04 element count
    u16 field_0x6;             // 0x06 type-index count
    u8  mPad_08[0x18 - 0x8];
    u32 field_0x18;            // 0x18 u16-group offset in shared buffer
    u16 mTypes[4];             // 0x1C type-index list (field_0x6 entries)
    u32 field_0x24;            // 0x24 flags (bit 0: skip value update,
                               //            bits 2-4: rotation applies)
    nw4r::math::VEC3 field_0x28;  // 0x28 default PS scale source
    f32 field_0x34;            // 0x34 rotation angle scale (bit 2)
    f32 field_0x38;            // 0x38 rotation angle scale (bit 3)
    f32 field_0x3C;            // 0x3C rotation angle scale (bit 4)
    f32 field_0x40;            // 0x40 scale
    f32 field_0x44;            // 0x44 limit
    u8  mPad_48[0x74 - 0x48];  // 0x48..0x73 (retail stride is 0x74)
};

// 0x48-byte output record (array base at LODMemMan+0xC).  The words at
// 0x30..0x38 hold raw float bits of the updated position.
struct LODOut48 {
    u8  mPad_00[0x30];
    u32 field_0x30;            // 0x30 position x bits
    u32 field_0x34;            // 0x34 position y bits
    u32 field_0x38;            // 0x38 position z bits
    f32 field_0x3C;            // 0x3c
    f32 field_0x40;            // 0x40 current value
    f32 field_0x44;            // 0x44 output value
};

// View of the 0x48-byte output record used by updateElementsByDesc: the leading
// 12 floats are the element's world matrix and the words at 0x30..0x38 hold
// the position consumed as the matrix translation column.
struct LODOutRec {
    f32 mMtx[12];              // 0x00..0x2f world matrix
    f32 mPos[3];               // 0x30..0x38 position
    f32 field_0x3C;
    f32 mValue;                // 0x40
    f32 mOutput;               // 0x44
};

// Float-matrix view of the 0x48-byte element descriptor (updateElementsByDesc):
// three base-position floats followed by a local matrix.
struct LODElem48Full {
    f32 field_0x0;             // 0x00
    f32 field_0x4;             // 0x04
    f32 field_0x8;             // 0x08 radius
    f32 mMtx[12];              // 0x0c..0x3b local matrix
    u16 field_0x40;            // 0x40
    u16 field_0x42;            // 0x42
    u16 field_0x44;            // 0x44
    u8  field_0x46;            // 0x46
    u8  field_0x47;            // 0x47
    u8  mPad_48[4];             // 0x48..0x4b trailing pad (0x48 stride)
};


// carry an 8-byte RTTI header, so declared slot 0 sits at vtable+0x8 and the
// slot destroyAllG3dObjects dispatches through (vtable+0x3C) is the 14th virtual.
struct LODG3dMgrVt {
    virtual void vf00(nw4r::g3d::G3dObj*) = 0;  // vtable+0x08
    virtual void vf01(nw4r::g3d::G3dObj*) = 0;  // vtable+0x0C
    virtual void vf02(nw4r::g3d::G3dObj*) = 0;  // vtable+0x10
    virtual void vf03(nw4r::g3d::G3dObj*) = 0;  // vtable+0x14
    virtual void vf04(nw4r::g3d::G3dObj*) = 0;  // vtable+0x18
    virtual void vf05(nw4r::g3d::G3dObj*) = 0;  // vtable+0x1C
    virtual void vf06(nw4r::g3d::G3dObj*) = 0;  // vtable+0x20
    virtual void vf07(nw4r::g3d::G3dObj*) = 0;  // vtable+0x24
    virtual void vf08(nw4r::g3d::G3dObj*) = 0;  // vtable+0x28
    virtual void vf09(nw4r::g3d::G3dObj*) = 0;  // vtable+0x2C
    virtual void vf0A(nw4r::g3d::G3dObj*) = 0;  // vtable+0x30
    virtual void vf0B(u32, nw4r::g3d::G3dObj*) = 0;  // vtable+0x34
    virtual void vf0C(nw4r::g3d::G3dObj*) = 0;  // vtable+0x38
    virtual void vf0D(nw4r::g3d::G3dObj*) = 0;  // vtable+0x3C (called slot)
};

// Data view of the g3d manager object (LODMemMan+0x0): bindModelToSlot hands
// the word at +0xE4 to the vtable+0x34 callback.
struct LODMgrObj {
    u8  mPad_00[0xE4];
    u32 field_0xE4;              // 0xE4
};

// 0xc-byte bind slot used by bindModelToSlot (overlay of LODMemMan+0xC8).
struct LODBindSlot {
    nw4r::g3d::ScnMdl* field_0x0;  // 0x00 ScnMdl
    u32 field_0x4;                 // 0x04 bound id
    u8  mPad_08[0xA - 0x8];
    u16 mFlags;                    // 0x0A (bit 0: bound)
};

// 16-slot view of LODMemMan+0xC8 for bindModelToSlot.
struct LODBindView {
    u8 mPad_00[0xC8];
    LODBindSlot mSlots[16];        // 0xC8..0x187
};

// View frame returned by Scn_HasCamItem; +0x9C is handed to the LOD layer
// updates and +0x1E0 holds the scale (mirrors UnkViewFrame in
// code_8047BB54.cpp).
struct LODViewFrame {
    u8  mPad_00[0x9C];             // 0x00..0x9b
    u8  field_0x9C[0x1E0 - 0x9C];  // 0x9c..0x1df
    f32 field_0x1E0;               // 0x1e0 layer scale
};

// Scene object returned by getScnRootPtr; the byte at +0x19 gates the
// per-frame LOD layer update.
struct LODScnGate {
    u8 mPad_00[0x19];              // 0x00..0x18
    u8 field_0x19;                 // 0x19 gate byte
};

// 0x20-byte pool block (allocatePoolBlock / findPoolBlockById list node).
struct LODPoolBlock {
    u32 mFlags;              // 0x00 (bit 0: in use, bit 1: ready)
    u32 mSize;               // 0x04
    LODPoolBlock* mNext;     // 0x08
    u32 field_0xC;           // 0x0C
    u32 mId;                 // 0x10
    u32 field_0x14;          // 0x14 block size (acquireResFileSlot checks alignment)
    u8  mPad_18[0x20 - 0x18];// 0x18..0x1F
};

// Buffer at field_0x4: 0x74-byte header followed by a per-id u32 table
// (getOrCreatePoolData reads the two words at id*2 / id*2+1 to size a block).
struct LODBuf74 {
    u8  mPad_00[0x74];     // 0x00..0x73
    u32 mPairs[64];        // 0x74.. per-id u32 table (2 words per id)
};

// 0x20-byte bounding-box element (element list at field_0x60).
struct LODBoxElem {
    f32 mMinX;               // 0x00
    f32 mMinY;               // 0x04
    f32 mMinZ;               // 0x08
    f32 mMaxX;               // 0x0C
    f32 mMaxY;               // 0x10
    f32 mMaxZ;               // 0x14
    u32 field_0x18;          // 0x18 u16-group offset in shared buffer
    u32 field_0x1C;          // 0x1C u16-group offset in shared buffer
};


// offsets into the shared buffer at field_0x5C (a u16 count followed by u16
// data).
struct LODViewDesc {
    u8  mPad_00[0x18];       // 0x00..0x17
    u32 field_0x18;          // 0x18 u16-group offset in shared buffer
    u32 field_0x1C;          // 0x1C u16-group offset in shared buffer
};

// +0xa44 sub-manager view.  The ctor sets the vtable, the first four words,
// the leading byte of the name string and the strlen word at +0x74.
struct LODSubA44 {
    u8*  field_0x0;            // 0x00 vtable
    u32  field_0x4;            // 0x04
    u32  field_0x8;            // 0x08
    u8   mPad_0C[0x20 - 0xC];  // 0x0C..0x1F
    u32  field_0x20;           // 0x20
    u8   mPad_24[0x30 - 0x24]; // 0x24..0x2F
    u32  field_0x30;           // 0x30
    char mStr_34[0x74 - 0x34]; // 0x34..0x73 name string
    u32  field_0x74;           // 0x74 strlen of the name
};

// 0xc-byte ResFile slot used by acquireResFileSlot: 16 entries at (self+0x8).
// The u16 at +0x0A is the in-use flag (bit 0); the id at +0x08 is -1 when
// the slot is free.  The u32 at +0x00 is the aligned ResFile data base and
// the u32 at +0x04 the pool block pointer it was carved from.
struct LODResFileSlot {
    u32 field_0x8;    // +0x00 ResFile/end base
    u32 field_0xC;    // +0x04 pool block pointer
    s16 field_0x10;   // +0x08 element id (-1 = free)
    u16 field_0x12;   // +0x0A flags (bit 0: in use)
};

// 16-slot view of the +0x8..0xc8 region used by acquireResFileSlot.
struct LODResFileSlotArray {
    u8  mPad_00[0x8];            // 0x00..0x07
    LODResFileSlot mSlots[16];   // 0x8..0xc8
};

// 0x1c-strided record (array base at LODMemMan+0x98).  Bit 2 of the flag
// word gates the u16 threshold at +0xA; bit 1 drives the ColiLodLookupRecordBin
// submit path and bit 0 the submitLodBillMtx billboard helper.
struct LODRec1C {
    u32 field_0x0;    // 0x00 flags (bit 0: billboard, bit 1: submit, bit 2: threshold)
    u8  mPad_04[0x8 - 0x4];
    u16 field_0x8;    // 0x08 index passed to submitLodBillMtx
    u16 field_0xA;    // 0x0A u16 clamp threshold
    u8  mPad_0C[0x1C - 0xC];
};

// Full field overlay for the LODMemMan targets in this TU.
struct LODMemManLayout {
    /* 0x00 */ nw4r::g3d::G3dObj* field_0x0;  // g3d manager object
    /* 0x04 */ LODElem20* field_0x4;          // 0x20-strided element list
    /* 0x08 */ u8* field_0x8;                 // secondary buffer (released on reset)
    /* 0x0C */ u8* field_0xC;                 // pool / descriptor region
    /* 0x10 */ LODElem20* field_0x10;         // index-list head
    /* 0x14 */ LODElem20* field_0x14;         // index-list tail
    /* 0x18 */ s32 mCount_18;                 // element count
    /* 0x1C */ u8* mView_1C;                  // view (Scn_GetFrameDelta arg)
    /* 0x20 */ u32 field_0x20;                // flag word (bit 6: buffer allocated)
    /* 0x24 */ u32 field_0x24;                // allocated block (getOrCreatePoolData)
    /* 0x28 */ u32 field_0x28;                // size-table value
    /* 0x2C */ u32 field_0x2C;                // size-table value
    /* 0x30 */ CFileHandle* field_0x30;       // file handle cancelled on reset
    /* 0x34 */ char mStr_34[0x54 - 0x34];     // reset-time name string
    /* 0x54 */ u32 field_0x54;                // cached element slot 0
    /* 0x58 */ u32 field_0x58;                // cached element slot 1
    /* 0x5C */ u8* field_0x5C;                // shared buffer base
    /* 0x60 */ LODBoxElem* field_0x60;        // 0x20-strided box elements
    /* 0x64 */ u32* field_0x64;               // used-element bitmap
    /* 0x68 */ u32 field_0x68;                // flag word (bit 4)
    /* 0x6C */ u32 field_0x6C;                // flag bits (bit 0: enabled)
    /* 0x70 */ u32 field_0x70;                // element-flag accumulator
    /* 0x74 */ s32 field_0x74;                // strlen of the name string
    /* 0x78 */ u8 mPad_78[0x7C - 0x78];
    /* 0x7C */ f32 field_0x7C;                // base scale
    /* 0x80 */ f32 field_0x80;                // scaled frame value
    /* 0x84 */ f32 field_0x84;                // second scale value (0.0f init)
    /* 0x88 */ f32 field_0x88;                 // shrink-path stored scale
    /* 0x8C */ f32 field_0x8C;                 // shrink-path stored value
    /* 0x90 */ LODElem48* field_0x90;         // 0x48-strided descriptor list
    /* 0x94 */ LODTypeDesc18* field_0x94;     // 0x18-strided type descriptors
    /* 0x98 */ LODRec1C* field_0x98;          // 0x1c-strided records
    /* 0x9C */ LODDesc74* field_0x9C;         // 0x74-strided type descriptors
    /* 0xA0 */ u8* field_0xA0;                // descriptor-array base
    /* 0xA4 */ s32 field_0xA4;                // type-descriptor count
    /* 0xA8 */ nw4r::g3d::G3dObj* field_0xA8;  // g3d object (destroyed on reset)
    /* 0xAC */ nw4r::g3d::G3dObj* field_0xAC;  // g3d object (destroyed on reset)
    /* 0xB0 */ u32 field_0xB0;                // cleared on reset
    /* 0xB4 */ u8* field_0xB4;                 // scene hook (set by attachSceneObjects)
    /* 0xB8 */ u16 field_0xB8;                // setter target (setTimeParams)
    /* 0xBA */ u16 field_0xBA;                // setter target (setTimeParams)
    /* 0xBC */ u16 field_0xBC;                // setter target (setTimeParams)
    /* 0xBE */ u16 field_0xBE;                // setter target (setTimeParams)
    /* 0xC0 */ f32 field_0xC0;                // LOD distance constants (1.0 init)
    /* 0xC4 */ f32 field_0xC4;
    /* 0xC8 */ LODG3dSlot field_0xC8[16];     // g3d object slots (0xc8..0x188)
    /* 0x188 */ u8 mPad_188[0x1CC8 - 0x188];
    /* 0x1CC8 */ f32 field_0x1CC8;           // second base-scale copy (setBaseScale)
    /* 0x1CCC */ u8 mPad_1CCC[0x1CDC - 0x1CCC];
    /* 0x1CDC */ f32 field_0x1CDC;            // shared LOD distance scale
};

// ---------------------------------------------------------------------------
// Retail-named imports for the LOD targets.  The Fv-mangled callees that
// receive extra args in retail are declared with the exact mangled names.
// MWCC re-mangles `__`-containing global names by appending the parameter
// signature, so the exact retail names are forced with C linkage (the
// reloc_map tool's approved fix for name drift).
// ---------------------------------------------------------------------------
// TU-internal callees with extra args (retail symbol names).  MWCC appends
// `__F<params>` to plain C++ declarations at call sites, so C linkage keeps
// the exact retail names (the Fv suffix is a decompiler guess).  The func_*
// bodies are the retail-named member stubs / definitions below.
extern "C" u32 ScnRes_IsStateActive_E4A0();
// ScnRes_Notify2Word_E458 returns the scan-query result in r3 (its void-typed
// definition in scn/code_804BD8E8.cpp tail-calls launchRayScan, leaving the
// result live; this TU's prototype exposes it so the refreshGroupFromList
// gate can test it).  Call codegen is identical either way.
extern "C" u32 ScnRes_Notify2Word_E458(u32 a1, u32 a2);
extern "C" LODPoolBlock* findPoolBlockById__Q23LOD9LODMemManFv(LODPoolBlock* self, int id);
// getOrCreatePoolData: now a real LOD::LODMemMan member (declaration in the header).
extern "C" u8* allocatePoolBlock__Q23LOD9LODMemManFv(LODPoolBlock* self, u32 size, int id);

// Scene / view helpers defined in other monolib TUs.  Retail kept the plain
// unmangled names; C linkage forces the verbatim names at the call sites.
extern "C" CScn* Scn_GetCurrentScene();
extern "C" LODViewFrame* Scn_HasCamItem(CScn* camera, CView* view);
extern "C" u32 getScnRootPtr(CScn* self);
extern "C" nw4r::g3d::ScnObj* getScnGroupChild(CScn* self, u32 idx);
extern "C" void* getScnRoot44C(CScn* self);
// CLight::LightCtlInitGXLight (retail keeps the plain unmangled call).  Retail
// passes the incoming mtx pointer as a third argument; r5 already holds it,
// so MWCC emits no arg move but keeps r5 live across the body.
extern "C" void LightCtlInitGXLight(u8* outLight, u8* matrix, u8* mtx);

// updateElementLod's callees: the LOD record helper (coli/code_804A6C60.cpp)
// and the scene resource helpers (scn/code_804BC9EC.cpp).  Retail keeps the
// plain unmangled names (C ABI); the Fv-suffixed names carry explicit ABI
// args (MWCC_CASES "Fv ABI note").
extern "C" s32 ColiLodLookupRecordBin(void* rec);
extern "C" u8 ScnData_FwdB80CC(void* ptr, u16 id);
extern "C" void ScnData_FwdB7D9C(void* unused, s32 a);
extern "C" void ScnData_FwdB7DD4(void* unused, s32 a);
extern "C" void ScnData_FwdB8078(void* unused, s32 a);
extern "C" void submitLodBillMtx__Q23LOD17UnkClass_80468434Fv(u32 idx, const f32* srcMtx, u8 arg2);

// Not decompiled yet (retail body 0x3b8): the callers see only this
// declaration, so the empty member definition below is never inlined into them.
extern "C" void updateSubViewFrame__Q23LOD17UnkClass_8046D144FPUci(
    LOD::UnkClass_8046D144* self, u8* p, int a);

// ScnProc draw callback (user-data = manager; defined later in this file).
extern "C" void handleViewUpdate__Q23LOD9LODMemManFv(
    nw4r::g3d::ScnProc* proc, int param);

// TU-internal sub-manager callbacks (definitions later in this file).
// driveArchiveLoadState: now a real LOD::LODMemMan member (declaration in the header).
// TU-internal sub-manager callback (definition later in this file).
// driveArchiveLoadState: now a real LOD::LODMemMan member (declaration in the header).
extern "C" void markPoolBlockReady__Q23LOD9LODMemManFv(LODPoolBlock* list, u32 target);
extern "C" void initElementList__Q23LOD9LODMemManFv(LODElemList* head, u32 arg);

// Shared-buffer view for updateLodTick: the cache-manager offset lives at
// +0x58 of the shared buffer.
struct LODCacheBuf58 {
    u8  mPad_00[0x58];
    u32 field_0x58;
};

// ResFile handle overlay at LODMemMan+0x04.
struct LODResView {
    u8                 mPad_00[4];
    nw4r::g3d::ResFile mResFile;
};

// --- FULL_MATCH functions ---

// Simple getter: return the float at offset 0x7c.
float LOD::UnkClass_8046D144::getBaseScale() {
    return mFloat_7C;
}

// Tail-forward call to embedded sub-object's updateLodPositions.
void LOD::UnkClass_8046D144::updateSubObject() {
    mSubObject.updateLodPositions();
}

// --- Remaining harness stubs (empty bodies) ---

// ---------------------------------------------------------------------------
// LOD manager ctor.  The embedded g3d sub-manager and the LODMemMan seed
// themselves; the body seeds the sub-manager's name string (MWCC stores the
// LODMemMan vtable from its own ctor) and runs the embedded
// UnkClass_8046A530 / resetFrameFields.
// ---------------------------------------------------------------------------
LOD::UnkClass_8046D144::UnkClass_8046D144() : field_0x0(0) {
    LODSubA44* sub = (LODSubA44*)&mMemMan;

    sub->mStr_34[0] = 0;
    sub->field_0x74 = 0;
    sub->field_0x4 = 0;
    sub->field_0x8 = 0;
    sub->field_0x20 = 0;
    sub->field_0x30 = 0;
    sub->field_0x74 = strlen((const char*)&lbl_eu_80663834);
    strcpy(sub->mStr_34, (const char*)&lbl_eu_80663834);

    mSubObject.initLod();
    resetFrameFields();
}

// Address-anchored retail symbol __dt__8046A584 (defined in
// lod/code_8046A530.cpp); C linkage keeps the verbatim retail reloc name.
extern "C" void* __dt__8046A584(LOD::UnkClass_8046A530* obj, int dealloc);

// Complete destructor (address-anchored retail symbol __dt__8046D144):
// releases both g3d objects (+0xA8/+0xAC) from their scene groups, tears down
// the +0xCC and +0xA44 sub-managers, keeps only flag bit 0x800 of +0x6C,
// runs the embedded sub-object destructor (+0xABC) and frees the instance
// when the deleting flag is positive.
LOD::UnkClass_8046D144::~UnkClass_8046D144() {
    LODMemManLayout* l = (LODMemManLayout*)this;
    if (l->field_0xA8 != 0) {
        nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)l->mView_1C, 7);
        if (((LODMgrObj*)group)->field_0xE4 != 0) {
            ((LODG3dMgrVt*)group)->vf0D(l->field_0xA8);
        }
        l->field_0xA8->Destroy();
        l->field_0xA8 = 0;
    }
    if (l->field_0xAC != 0) {
        nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)l->mView_1C, 8);
        if (((LODMgrObj*)group)->field_0xE4 != 0) {
            ((LODG3dMgrVt*)group)->vf0D(l->field_0xAC);
        }
        l->field_0xAC->Destroy();
        l->field_0xAC = 0;
    }
    l->mView_1C = 0;
    l->field_0x5C = 0;
    mG3dMgr.notifyG3dManager(0);
    mG3dMgr.destroyAllG3dObjects();
    mMemMan.resetManagerState();
    // Keep only bit 0x800 (bit 20) of the enable-flag word.
    l->field_0x6C &= 0x800u;
    __dt__8046A584(&mSubObject, -1);
}

// Cache manager helper called with (shared buffer base + offset at +0x58).
extern "C" void initLodTables__Q23LOD17CLODCacheManagerSFv(u8* p);
// Embedded UnkClass_8046A530 update (called with this+0xABC).
extern "C" void isLodActive__Q23LOD17UnkClass_8046A530Fv(
    LOD::UnkClass_8046A530* obj, u32 flags, f32 val);
// Allocator warm-up: takes the scene, returns its MEMAllocator (result feeds
// ScnGroup::Construct in initSceneGroup).
extern "C" MEMAllocator* getScnRoot45C(CScn* scene);
// Globals published by the element-list walk in updateLodTick.
extern u32 lbl_eu_80665778;
extern u32 lbl_eu_80665768;

// TU-internal callees used by the two rebuild/update passes below.
extern "C" void initLodState__Q23LOD17UnkClass_8046A530Fv(
    LOD::UnkClass_8046A530* self);
// dispatchViewUpdate is a static LOD::UnkClass_8046D144 member (declaration in the header).
extern "C" int bindResMdl__Q23LOD9LODMemManFv(
    LOD::UnkClass_8046D144* self, CScn* archive, u32 mdlIdx, u32 packed,
    const f32* mtxSrc);
extern "C" s32 acquireResFileSlot__Q23LOD9LODMemManFv(
    LOD::UnkClass_8046D144* self, LOD::UnkClass_8046D144* pool, s32 id, s32* out);

// Header of the shared buffer handed in r4 (offsets consumed here).
struct LOLDBufHdr {
    u8  mPad_00[0x08];
    u32 field_0x8;             // 0x08 flag bits (bit 4)
    u32 field_0xC;             // 0x0C optional ResFile block offset for B10
    u8  mPad_10[4];
    u32 field_0x14;            // 0x14 size adjustment for adjustBufferOffsets
    u32 field_0x18;            // 0x18 type-descriptor table offset
    u32 field_0x1C;            // 0x1C 0x18-byte record count
    u8  mPad_20[0x30 - 0x20];
    u32 field_0x30;            // 0x30 element-descriptor offset
    u32 field_0x34;            // 0x34 element count
    u32 field_0x38;            // 0x38 box-element array offset
    u8  mPad_3C[0x40 - 0x3C];
    u32 field_0x40;            // 0x40 ResFile data offset
    u32 field_0x44;            // 0x44 ResFile-present flag
    u8  mPad_48[0x64 - 0x48];
    u32 field_0x64;            // 0x64 record-table offset
    u32 field_0x68;            // 0x68 descriptor-array offset
    u32 field_0x6C;            // 0x6C descriptor count
    u32 field_0x70;            // 0x70 (loaded, unused)
};

// User-data slot of an nw4r::g3d::ScnProc (set inline, no setter call).
struct LODScnProcUD {
    u8    mPad_00[0xF0];
    void* mUserData;   // 0xF0
};

// Retail calls this member-shaped rebuild with the shared-buffer base in the
// second (register-only) argument; keep the exact retail ABI.
void LOD::UnkClass_8046D144::rebuildFromSharedBuffer(u32 arg) {
    LOLDBufHdr* bh = (LOLDBufHdr*)arg;
    u8* buf = (u8*)arg;

    // Detach and destroy both g3d objects through their scene groups.
    if (field_0xA8 != 0) {
        nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)mView_1C, 7);
        if (((LODMgrObj*)group)->field_0xE4 != 0) {
            ((LODG3dMgrVt*)group)->vf0D(field_0xA8);
        }
        field_0xA8->Destroy();
        field_0xA8 = 0;
    }
    if (field_0xAC != 0) {
        nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)mView_1C, 8);
        if (((LODMgrObj*)group)->field_0xE4 != 0) {
            ((LODG3dMgrVt*)group)->vf0D(field_0xAC);
        }
        field_0xAC->Destroy();
        field_0xAC = 0;
    }

    // Clear per-frame state and adopt the new shared buffer.
    field_0xA8 = 0;
    field_0xAC = 0;
    field_0xB0 = 0;
    field_0x10 = 0;
    field_0x14 = 0;
    field_0x44 = 0;
    field_0x40 = 0;
    field_0x3C = 0;
    field_0x50 = 0;
    field_0x4C = 0;
    field_0x48 = 0;
    field_0x5C = buf;

    // Sub-tables are buffer-relative.
    field_0x90 = (LODElem48*)(buf + bh->field_0x30);
    field_0x94 = (LODTypeDesc18*)(buf + bh->field_0x18);
    field_0x98 = (LODRec1C*)(buf + bh->field_0x64);

    if (bh->field_0xC != 0) {
        mG3dMgr.initSceneGroup(buf + bh->field_0xC, (CScn*)mView_1C);
    } else {
        mG3dMgr.initSceneGroup((u8*)0, (CScn*)mView_1C);
    }

    // Descriptor element-count sum (accumulated in r6, fed to allocSecondaryBuffers).
    u32 total = 0;
    // Descriptor table: base/count live right behind the u16 index list.
    if (bh->field_0x6C != 0) {
        field_0xA0 = field_0x5C + bh->field_0x68;
        u32 words = bh->field_0x34;
        if (words & 1) {
            words++;
        }
        field_0x9C = (LODDesc74*)((u8*)field_0xA0 + words * 2);
        field_0xA4 = bh->field_0x6C;

        // Retail sums every descriptor's element count.  The count is read
        // once and kept in a register (retail holds it for the remainder
        // mask), which is what lets MWCC unroll this loop.
        u32 cnt = bh->field_0x6C;
        LODDesc74* d = field_0x9C;
        for (u32 i = 0; i < cnt; i++) {
            total += d->field_0x4;
            d++;
        }
    } else {
        field_0x9C = 0;
        field_0xA0 = 0;
        field_0xA4 = 0;
    }

    // Flag word: bit 0x2000 mirrors the buffer's bit-4 flag, bits 18/19 are
    // cleared on either path.  The loaded word is not dead: it becomes the
    // fifth argument of allocSecondaryBuffers below (retail keeps it in r7).
    u32 accArg = 0;
    if (bh->field_0x8 & 0x10) {
        accArg = *(u32*)(field_0x5C + bh->field_0x70);
        field_0x6C = (field_0x6C | 0x2000) & ~0x80000u;
    } else {
        field_0x6C &= ~0xC0000u;
    }

    // Allocate the secondary buffers: count20 elements, count18 0x18-byte
    // records, no 0x48/4-byte blocks yet.  (sum, accArg) ride in as the
    // middle u32 arguments.
    this->mMemMan.allocSecondaryBuffers(
        bh->field_0x34, bh->field_0x1C,
        total, accArg, (u8**)&field_0x4, (u8**)&field_0x8,
        (u8**)&field_0xC, (u8**)&field_0xB0);
    this->mMemMan.adjustBufferOffsets(
        bh->field_0x34, bh->field_0x1C, bh->field_0x14);

    // Per-element init pass.  The float constants are scoped to this pass:
    // retail loads them fresh here (f27 = 0.0f, f26 = 2^52 magic).
    const f32 cZero = lbl_eu_8066A6C0;
    const f64 kMagic52 = lbl_eu_8066A6E0;
    (void)kMagic52;
    for (int i = 0; i < (int)bh->field_0x34; i++) {
        LODElem20* rec = field_0x4 + i;
        LODElem48* e = field_0x90 + i;
        rec->field_0x1A = i;
        rec->field_0x4 = cZero;
        rec->field_0x0 = 0;
        rec->field_0xC = 0xFF;
        rec->field_0xE = 0xFF;
        rec->field_0x1E = 0;
        rec->field_0x1F = 0;
        rec->field_0x8 = e->field_0x42;
        if (e->field_0x44 & 4) {
            rec->field_0x0 |= 0x20;
        }
        if (e->field_0x44 & 0x10) {
            rec->field_0x0 |= 0x1000;
        }
        if (e->field_0x44 & 8) {
            u32 w = field_0x98[e->field_0x47].field_0x0;
            if (w & 3) {
                void* p = getScnHandle();
                rec->field_0x1D = ScnData_FwdB80CC(p, e->field_0x40);
            }
        }
        rec->field_0x16 = (u16)lbl_eu_80663828[0];
        if (e->field_0x42 == 0) {
            rec->field_0x0 |= 0x40;
        } else {
            // Append to the active-element index list.
            if (field_0x10 == 0) {
                field_0x10 = rec;
            } else {
                ((LODElem20*)field_0x14)->field_0x16 = i;
            }
            field_0x14 = rec;
            rec->field_0x0 |= 0x10000;
        }
    }

    // Per-descriptor random initialisation of the output values.  All six
    // constants are (re)loaded at block entry in retail order:
    // f26=E8, f27=C4, f30=C0, f31=D8, f28=D0, f29=C8.
    if (field_0xA4 != 0) {
        const f64 kM52b = lbl_eu_8066A6E8;
        const f32 cScale = lbl_eu_8066A6C4;
        const f32 cZero2 = lbl_eu_8066A6C0;
        const f32 cDiv = lbl_eu_8066A6D8;
        const f64 kMul = lbl_eu_8066A6D0;
        const f64 kBase = lbl_eu_8066A6C8;
        (void)kM52b;
        LODDesc74* d = field_0x9C;
        LODOut48* o = (LODOut48*)field_0xC;
        for (int di = 0; di < field_0xA4; di++, d++) {
            int n1000 = (int)d->field_0x44 * 1000;
            for (int j = 0; j < d->field_0x4; j++, o++) {
                if (d->field_0x24 & 1) {
                    o->field_0x40 =
                        (f32)(cScale * (f64)(u32)ml::math::mtRand(n1000));
                    o->field_0x3C = (f32)(kBase -
                                          kMul * (f64)(u32)ml::math::mtRand(100));
                } else {
                    o->field_0x40 = cZero2;
                    o->field_0x3C = cDiv;
                }
            }
        }
    }

    mCount_18 = bh->field_0x34;

    // Optional aligned ResFile block appended to the shared buffer.
    if (bh->field_0x44 != 0) {
        u8* resBuf = field_0x5C + bh->field_0x40;
        if ((u32)resBuf & 0x1F) {
            nw4r::db::Panic(lbl_eu_8052637C, 60, lbl_eu_80526354);
        }
        field_0x5C = resBuf;
        *(nw4r::g3d::ResFile*)this = nw4r::g3d::ResFile(resBuf);
        ((nw4r::g3d::ResFile*)this)->Init();
    }

    // Build the two scene procs (opaque / translucent draw callbacks).
    CScn* scene = (CScn*)mView_1C;
    u32 sz1;
    field_0xA8 = nw4r::g3d::ScnProc::Construct(
        (MEMAllocator*)getScnRoot45C(scene), &sz1, 
        (nw4r::g3d::ScnProc::DrawProc)&LOD::UnkClass_8046D144::dispatchViewUpdate,
        true, false, 0);
    ((LODScnProcUD*)field_0xA8)->mUserData = this;
    u32 sz2;
    field_0xAC = nw4r::g3d::ScnProc::Construct(
        (MEMAllocator*)getScnRoot45C(scene), &sz2, 
        (nw4r::g3d::ScnProc::DrawProc)&handleViewUpdate__Q23LOD9LODMemManFv,
        false, true, 0);
    ((LODScnProcUD*)field_0xAC)->mUserData = this;

    // Register both procs in the scene groups.
    nw4r::g3d::G3dObj* grp1 = getScnGroupChild(scene, 7);
    ((LODG3dMgrVt*)grp1)->vf0B(((LODMgrObj*)grp1)->field_0xE4, field_0xA8);
    nw4r::g3d::G3dObj* grp2 = getScnGroupChild(scene, 8);
    ((LODG3dMgrVt*)grp2)->vf0B(((LODMgrObj*)grp2)->field_0xE4, field_0xAC);

    initLodState__Q23LOD17UnkClass_8046A530Fv(
        (LOD::UnkClass_8046A530*)&this->mSubObject);

    // Tail: reset the distance/scale state and enable flags 1|2.  Retail
    // reloads both constants here (the loop-scoped copies have died).
    u32 fl = field_0x6C | 6;
    mFloat_7C = lbl_eu_8066A6D8;
    field_0x70 = 0;
    field_0xC8 = lbl_eu_8066A6C0;
    field_0xC4 = lbl_eu_8066A6C0;
    field_0xC0 = lbl_eu_8066A6C0;
    field_0xBE = 0;
    field_0x6C = fl;
}

// ---------------------------------------------------------------------------
// resetAndReload: full reset/reload entry point.  When flag bit 0x4 of +0x6C is
// set the two g3d objects are detached/destroyed and the sub-managers torn
// down first (keeping only flag bit 0x8000); both paths then re-initialise
// the per-frame fields, store `view` into +0x1C, set flag bit 0 and run the
// scene refresh passes.
// ---------------------------------------------------------------------------
bool LOD::UnkClass_8046D144::resetAndReload(u32 arg, u8* view) {
    if (field_0x6C & 4) {
        if (field_0xA8 != 0) {
            nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)mView_1C, 7);
            if (((LODMgrObj*)group)->field_0xE4 != 0) {
                ((LODG3dMgrVt*)group)->vf0D(field_0xA8);
            }
            field_0xA8->Destroy();
            field_0xA8 = 0;
        }
        if (field_0xAC != 0) {
            nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)mView_1C, 8);
            if (((LODMgrObj*)group)->field_0xE4 != 0) {
                ((LODG3dMgrVt*)group)->vf0D(field_0xAC);
            }
            field_0xAC->Destroy();
            field_0xAC = 0;
        }
        this->mG3dMgr.notifyG3dManager((CScn*)mView_1C);
        this->mMemMan.cancelFileAndRelease();
        mView_1C = 0;
        field_0x5C = 0;
        // Clear flag bits 1,2,7,12,13 (retail mask 0xCF79), then set 0x8000.
        field_0x6C = (field_0x6C & ~0x3086u) | 0x8000u;
    }
    u32 flags = field_0x6C | 1;
    field_0x5C = 0;
    field_0xA8 = 0;
    field_0xAC = 0;
    field_0xB0 = 0;
    field_0x84 = lbl_eu_8066A6D8;
    field_0x44 = 0;
    field_0x40 = 0;
    field_0x3C = 0;
    field_0x50 = 0;
    field_0x4C = 0;
    field_0x48 = 0;
    mView_1C = view;
    field_0x6C = flags;
    // Retail passes the embedded archive sub-object (this+0xA44), not the base
    // instance (asm: `addi r3, r29, 2628` before the call), matching the
    // cancelFileAndRelease call above.
    this->mMemMan.setArchiveReadyFlag();
    rebuildFromSharedBuffer(arg);
    return true;
}

void LOD::UnkClass_8046D144::resetFrameFields() {
    // Float pool constants are cached in locals so MWCC CSEs them into
    // f1 (1.0f) / f0 (scale divisor); the clears follow in field order.
    // NOTE: MWCC's list scheduler pulls the first f0 consumer (the +0x7C
    // store) up into the load-latency slot; retail keeps it in program
    // order.  Every source lever tried (assignment order, zero/nine locals,
    // interleaving, dup-store elimination, chained stores, #pragma
    // scheduling off) leaves that one-instruction displacement unchanged.
    f32 v0, v1;
    v1 = lbl_eu_8066A6C0;
    v0 = lbl_eu_8066A6D8;
    field_0x4 = 0;
    field_0xC = 0;
    mCount_18 = 0;
    field_0x5C = 0;
    field_0xA8 = 0;
    field_0xAC = 0;
    field_0xB4 = 0;
    field_0x6C = 0;
    field_0xB8 = 9;
    field_0xBA = 0;
    field_0xBC = 0;
    field_0xC8 = v1;
    field_0xC4 = v1;
    field_0xC0 = v1;
    field_0xBE = 0;
    mFloat_7C = v0;
    field_0x1CDC = v0;
    field_0xA4 = 0;
    field_0x84 = v0;
}
// ---------------------------------------------------------------------------
// clearManagers: clear the view/shared-buffer pointers, then reset the +0xCC
// sub-manager (both g3d passes) and the +0xA44 sub-manager, and clear flag
// bits 0x800 (retail keeps li r4,0 for the first call's unused scene arg;
// the u8* self local keeps `self` in r31 so the +0xCC temps recompute per
// call instead of hoisting into a saved register).
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::clearManagers() {
    mView_1C = 0;
    field_0x5C = 0;
    mG3dMgr.notifyG3dManager(0);
    mG3dMgr.destroyAllG3dObjects();
    mMemMan.resetManagerState();
    field_0x6C &= 0x800;
}

// ---------------------------------------------------------------------------
// teardownG3dObjects: release the two g3d objects (+0xA8 / +0xAC), detaching each
// from the scene's root group (getScnGroupChild index 7/8) first, then tear down
// the +0xCC sub-manager and the +0xA44 sub-manager (plus its clearFlagAndResetName
// pass when `param` is set).  Ends by clearing the view/shared-buffer pointers
// and toggling flag bits 0x3087 -> 0x8000.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::teardownG3dObjects(int param) {
    LOD::UnkClass_8046D144* self = this;
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0xA8) {
        nw4r::g3d::ScnGroup* group = (nw4r::g3d::ScnGroup*)getScnGroupChild((CScn*)l->mView_1C, 7);
        if (group->Size() != 0) {
            group->Remove((nw4r::g3d::ScnObj*)l->field_0xA8);
        }
        l->field_0xA8->Destroy();
        l->field_0xA8 = 0;
    }
    if (l->field_0xAC) {
        nw4r::g3d::ScnGroup* group = (nw4r::g3d::ScnGroup*)getScnGroupChild((CScn*)l->mView_1C, 8);
        if (group->Size() != 0) {
            group->Remove((nw4r::g3d::ScnObj*)l->field_0xAC);
        }
        l->field_0xAC->Destroy();
        l->field_0xAC = 0;
    }
    self->mG3dMgr.notifyG3dManager((CScn*)l->mView_1C);
    self->mMemMan.cancelFileAndRelease();
    if (param) {
        this->mMemMan.clearFlagAndResetName();
    }
    l->mView_1C = 0;
    l->field_0x5C = 0;
    l->field_0x6C = (l->field_0x6C & -0x3087) | 0x8000;
}

// ---------------------------------------------------------------------------
// updateLodTick: per-frame LOD tick.  Release pass (flag bits 0x18000):
// clears the element list at +0x68, drops flag bit 0x1000000, tears down the
// +0xCC sub-manager when bit 0x10000 was held and re-marks bit 0x10000 while
// bit 0x8000 survives.  Load pass otherwise: drives the archive state
// machine, optionally re-runs rebuildFromSharedBuffer, scales the base value by the
// frame delta, walks the element index chain through updateElementLod, runs the
// per-type update, folds the shrink accumulator and finally updates the
// embedded UnkClass_8046A530 when bit 0x10 is clear.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateLodTick() {
    LOD::UnkClass_8046D144* self = this;
    u32 flags = field_0x6C;
    if (!(flags & 1)) return;
    field_0x6C = flags & ~0x80u;
    field_0x68 = 0;
    u32 f2 = flags & ~0x80u;
    if (flags & 0x18000) {
        // Release pass: tear down the +0xCC sub-manager when requested, then
        // re-mark the reload-request bit (0x10000) if the reset bit survives.
        if (f2 & 0x10000) {
            field_0x6C = f2 & ~0x10000u;
            self->mG3dMgr.destroyAllG3dObjects();
        }
        u32 g = field_0x6C;
        if (!(g & 0x8000)) return;
        field_0x6C = (g & ~0x8000u) | 0x10000u;
        return;
    }
    // Load pass.
    self->mMemMan.driveArchiveLoadState();
    if (!(field_0x6C & 4)) {
        if (((LODSubA44*)&mMemMan)->field_0x20 & 2) {
            rebuildFromSharedBuffer(((LODSubA44*)&mMemMan)->field_0x4);
        } else {
            return;
        }
    }
    field_0x80 = mFloat_7C * Scn_GetFrameDelta(mView_1C);
    if (!(field_0x6C & 8)) {
        field_0x6C |= 0x80;
        LODCacheBuf58* buf = (LODCacheBuf58*)field_0x5C;
        initLodTables__Q23LOD17CLODCacheManagerSFv((u8*)buf + buf->field_0x58);
        lbl_eu_80665778 = (u32)field_0x94;
        lbl_eu_80665768 = (u32)field_0x5C;
        if (field_0x10 != 0) {
            // Walk the index-linked element chain until the sentinel.
            int idx = field_0x10->field_0x1A;
            while ((u32)idx != lbl_eu_80663828[0]) {
                LODElem20* e = &field_0x4[idx];
                updateElementLod(e);
                idx = e->field_0x16;
            }
        }
        updateLodFrame();
        u32 flags2 = field_0x6C;
        if (flags2 & 0x600) {
            field_0x88 += field_0x80;
            // Body runs when the shrink accumulator reaches the limit.
            if (field_0x88 >= field_0x8C) {
                if (flags2 & 0x400) {
                    if (field_0x54 != 0) *(u32*)field_0x54 |= 0x10;
                    if (field_0x58 != 0) *(u32*)field_0x58 |= 0x10;
                }
                // Cleared whenever the limit check passes, even without bit 0x400.
                field_0x6C &= ~0x600u;
            }
        }
        if ((field_0x6C & 0x10) == 0) {
            isLodActive__Q23LOD17UnkClass_8046A530Fv(
                (LOD::UnkClass_8046A530*)&self->mSubObject, field_0x70, field_0x80);
        }
    }
}

// Retail-named imports used only by updateLodLayersForView.
// updateViewLayers: now a real LOD::UnkClass_8046D144 member (declaration in the header).
// rebuildElementChains: now a real LOD::UnkClass_8046D144 member (declaration in the header).
// teardownSubEntries: now a real LOD::UnkClass_8046D144 member (declaration in the header).
extern "C" u8* Coli_GetBitTable_5A68(u8* buf);           // bitmap allocator
extern "C" int dispatchLodPick__Q23LOD17UnkClass_8046368CFv(LODTypeDesc18* td);
extern f32 lbl_eu_8066A6F0;      // direction vector z constant
extern f32 lbl_eu_8066A6F4;      // angle chain factor
extern f32 lbl_eu_8066A6FC;      // sin scale (+0x74)
extern f32 lbl_eu_8066A704;      // tan arg factor
extern f32 lbl_eu_8066A708;      // degenerate FOV fallback
extern u32 lbl_eu_806657B8;      // elapsed time global
extern u32 lbl_eu_80665794;      // flag mirror global
extern const char lbl_eu_80526324[];  // Warning source path
extern const char lbl_eu_80526300[];  // Warning message

// Float view of the three distance channels at +0xC0.
struct DD9Chans {
    u8 mPad00[0xC0];
    f32 c[3];                // 0xC0, 0xC4, 0xC8
};

// ---------------------------------------------------------------------------
// updateLodLayersForView: per-frame LOD layer update for one view.  Requires flag bits
// 1|0x100 of +0x6C; advances the three distance channels (+0xC0..+0xC8)
// toward or away from their limits (mode byte at +0xBE selects which channel
// grows when bit 0x200 is set), derives the LOD plane distance (+0x38) from
// the view direction and the FOV factor (+0x78) from a tan/FrSqrt pair, then
// resets the used-element bitmap, rebuilds the element chains and refreshes
// texture bindings.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateLodLayersForView(u8* p, f32 scale) {
    u32 flags = field_0x6C;
    if ((flags & 1) == 0) return;
    if ((flags & 0x100) == 0) return;

    f32 step = field_0x80 * field_0x84;

    DD9Chans* ch = (DD9Chans*)this;
    if ((flags & 0x200) == 0) {
        ch->c[0] -= step; if (ch->c[0] < lbl_eu_8066A6C0) ch->c[0] = lbl_eu_8066A6C0;
        ch->c[1] -= step; if (ch->c[1] < lbl_eu_8066A6C0) ch->c[1] = lbl_eu_8066A6C0;
        ch->c[2] -= step; if (ch->c[2] < lbl_eu_8066A6C0) ch->c[2] = lbl_eu_8066A6C0;
    } else {
        // Exactly one channel grows: the one selected by the mode byte.
        s16 mode = field_0xBE;
        if (mode == 0) { ch->c[0] += step; if (ch->c[0] > lbl_eu_8066A6D8) ch->c[0] = lbl_eu_8066A6D8; }
        else           { ch->c[0] -= step; if (ch->c[0] < lbl_eu_8066A6C0) ch->c[0] = lbl_eu_8066A6C0; }
        if (mode == 1) { ch->c[1] += step; if (ch->c[1] > lbl_eu_8066A6D8) ch->c[1] = lbl_eu_8066A6D8; }
        else           { ch->c[1] -= step; if (ch->c[1] < lbl_eu_8066A6C0) ch->c[1] = lbl_eu_8066A6C0; }
        ch->c[2] += step; if (ch->c[2] > lbl_eu_8066A6D8) ch->c[2] = lbl_eu_8066A6D8;
    }

    // Transform {0,0,c} by the rotation-only copy of the view matrix.
    field_0x70 = 0;
    u8* buf = field_0x5C;
    const f32* vf = (const f32*)p;
    field_0x20 = vf[3];
    field_0x24 = vf[7];
    field_0x28 = vf[11];
    Mtx m;
    // Load every view word before storing: mirrors the retail scheduler and
    // raises register pressure onto the callee-saved set.
    f32 a00 = vf[0];
    f32 a01 = vf[1];
    f32 a02 = vf[2];
    f32 a10 = vf[4];
    f32 a11 = vf[5];
    f32 a12 = vf[6];
    f32 a20 = vf[8];
    f32 a21 = vf[9];
    f32 a22 = vf[10];
    ((f32*)m)[0] = a00;
    ((f32*)m)[1] = a01;
    ((f32*)m)[2] = a02;
    ((f32*)m)[4] = a10;
    ((f32*)m)[5] = a11;
    ((f32*)m)[6] = a12;
    ((f32*)m)[8] = a20;
    ((f32*)m)[9] = a21;
    ((f32*)m)[10] = a22;
    ((f32*)m)[3] = lbl_eu_8066A6C0;
    ((f32*)m)[7] = lbl_eu_8066A6C0;
    ((f32*)m)[11] = lbl_eu_8066A6C0;
    field_0x2C = lbl_eu_8066A6C0;
    field_0x30 = lbl_eu_8066A6C0;
    field_0x34 = lbl_eu_8066A6F0;
    PSMTXMultVec(m, (Vec*)&field_0x2C, (Vec*)&field_0x2C);

    f32 lenSq = field_0x2C * field_0x2C + field_0x30 * field_0x30
              + field_0x34 * field_0x34;
    field_0x38 = lbl_eu_8066A6D8 / lenSq;

    f32 ang = ((scale * ml::deg2rad) * lbl_eu_8066A6F4) * lbl_eu_8066A6F8;
    f32 s = nw4r::math::SinFIdx(ang);
    field_0x74 = lbl_eu_8066A6FC * s;
    if (s - scale > 0.0f) {
        f32 tv = tan((((s - scale) * ml::deg2rad) * lbl_eu_8066A6F8)
                     * lbl_eu_8066A704);
        f32 t = lbl_eu_8066A6D8 / tv;
        f32 q1 = t * t + lbl_eu_8066A6D8;
        if (q1 < 0.0f) {
            nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 q2 = t * t + lbl_eu_8066A6D8;
        if (q2 > 0.0f) {
            field_0x78 = nw4r::math::FrSqrt(q2) * q2;
        } else {
            field_0x78 = lbl_eu_8066A6C0;
        }
    } else {
        field_0x78 = lbl_eu_8066A708;
    }

    LOLDBufHdr* bh = (LOLDBufHdr*)buf;
    field_0x60 = (LODBoxElem*)(buf + bh->field_0x38);
    field_0x64 = (u32*)Coli_GetBitTable_5A68(buf);
    memset(field_0x64, 0, (((mCount_18 >> 5) + 1) << 2));
    memset(field_0x8, 0, bh->field_0x1C * 0x18);
    field_0x44 = 0;
    field_0x40 = 0;
    field_0x3C = 0;
    field_0x50 = 0;
    field_0x4C = 0;
    field_0x48 = 0;
    if ((field_0x6C & 0x10) == 0) {
        this->updateViewLayers((LODViewDesc*)field_0x60);
    }
    this->rebuildElementChains(bh->field_0x1C);
    this->mG3dMgr.teardownSubEntries(&this->mMemMan);
    if ((field_0x68 & 0x10) == 0) {
        // Publish the buffer/time bookkeeping globals.
        lbl_eu_806657B8 = ((field_0xB8 * 60) + field_0xBA) * 60
                        + field_0xBC;
        lbl_eu_80665768 = (u32)field_0x5C;
        lbl_eu_80665794 = field_0x68;
    }

    // Walk the pending-element chain starting from the node cached at +0x44
    // until an active model is found (then raise the scene gate byte).
    if (field_0x44 != 0) {
        int idx = ((LODElem20*)(int)field_0x44)->field_0x1A;
        while (true) {
            LODElem20* e = &field_0x4[idx];
            LODTypeDesc18* td = &field_0x94[e->field_0x18];
            if (dispatchLodPick__Q23LOD17UnkClass_8046368CFv(td) != 0) {
                LODScnGate* scn =
                    (LODScnGate*)getScnRootPtr((CScn*)mView_1C);
                scn->field_0x19 = 1;
                break;
            }
            idx = e->field_0x16;
            if (idx == (int)lbl_eu_80663828[0]) break;
        }
    }
    this->refreshPaletteBinding();
}

void LOD::UnkClass_8046D144::updateSubViewFrame(u8* p, int a) {}

// ---------------------------------------------------------------------------
// setPauseFlag: when `param` is zero set bit 3 of the +0x6C flag word,
// otherwise clear bit 6.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::setPauseFlag(int param) {
    LOD::UnkClass_8046D144* self = this;
    if (param != 0) {
        field_0x6C &= ~8;
    } else {
        field_0x6C |= 8;
    }
}

// ---------------------------------------------------------------------------
// attachSceneObjects: when `param` is nonzero, attach both g3d objects (+0xA8/
// +0xAC) to the scene groups from getScnGroupChild index 7/8 (notifying the
// group's g3d manager and re-attaching the object with its +0xE4 word), then
// store `param` into +0xB4 and set flag bit 0x800; otherwise clear +0xB4
// and the flag.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::attachSceneObjects(u32 param) {
    LOD::UnkClass_8046D144* self = this;
    if (param) {
        if (field_0xA8) {
            nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)mView_1C, 7);
            LODMgrObj* mgr = (LODMgrObj*)group;
            if (mgr->field_0xE4) {
                ((LODG3dMgrVt*)group)->vf0D(field_0xA8);
            }
            ((LODG3dMgrVt*)group)->vf0B(mgr->field_0xE4, field_0xA8);
        }
        if (field_0xAC) {
            nw4r::g3d::G3dObj* group = getScnGroupChild((CScn*)mView_1C, 8);
            LODMgrObj* mgr = (LODMgrObj*)group;
            if (mgr->field_0xE4) {
                ((LODG3dMgrVt*)group)->vf0D(field_0xAC);
            }
            ((LODG3dMgrVt*)group)->vf0B(mgr->field_0xE4, field_0xAC);
        }
        field_0xB4 = (u8*)param;
        field_0x6C = (field_0x6C | 0x800) & ~0x1000;
    } else {
        field_0xB4 = 0;
        field_0x6C &= ~0x1800;
    }
}

// ---------------------------------------------------------------------------
// setLodScaleAndRefresh: store the scale into +0x1CDC, then - while the +0x5C buffer
// is present - walk the elements and, for each whose +0x44 has bit 3 set and
// whose +0x47 descriptor's flag bits are nonzero, refresh the element via
// getScnHandle/ScnData_FwdB80CC and record the result byte at +0x1D.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::setLodScaleAndRefresh(float f1) {
    LOD::UnkClass_8046D144* self = this;
    field_0x1CDC = f1;
    if (field_0x5C == 0) return;
    LODElem48* p48 = field_0x90;
    LODElem20* p20 = field_0x4;
    LOLDBufHdr* buf = (LOLDBufHdr*)field_0x5C;
    u32 i = 0;
    while (i < buf->field_0x34) {
        if (!(p48->field_0x44 & 8)) goto next;
        if ((field_0x98[p48->field_0x47].field_0x0 & 3) == 0) goto next;
        p20->field_0x1D = ScnData_FwdB80CC(getScnHandle(), p48->field_0x40);
    next:
        i++;
        p20++;
        p48++;
    }
}

// ---------------------------------------------------------------------------
// setTimeParams: stores the three short params into the 0xB8..0xBC fields.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::setTimeParams(u32 a, u32 b, u32 c) {
    LOD::UnkClass_8046D144* self = this;
    field_0xB8 = a;
    field_0xBA = b;
    field_0xBC = c;
}

// ---------------------------------------------------------------------------
// clearElementFlag: when the LOD system is enabled, walk every element matching
// `task` and clear bit 1 of its flag word (retail keeps one loop copy).
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::clearElementFlag(int task) {
    LOD::UnkClass_8046D144* self = this;
    if (!(field_0x6C & 1)) return;
    LODElem48* p48 = field_0x90;
    LODElem20* p20 = field_0x4;
    int i = 0;
    while (i < mCount_18) {
        if (p48->field_0x40 == task) p20->field_0x0 &= ~2u;
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// setElementFlag: when the LOD system is enabled, walk every element matching
// `task` and set bit 1 of its flag word.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::setElementFlag(int task) {
    LOD::UnkClass_8046D144* self = this;
    if (!(field_0x6C & 1)) return;
    LODElem48* p48 = field_0x90;
    LODElem20* p20 = field_0x4;
    int i = 0;
    while (i < mCount_18) {
        if (p48->field_0x40 == task) p20->field_0x0 |= 2u;
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// toggleElementFlag: when the LOD system is enabled, walk every element matching
// `task` and flip bit 1 of its flag word - set when `flag` is zero, cleared
// otherwise (retail keeps two copies of the loop, one per bit operation).
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::toggleElementFlag(int task, int flag) {
    LOD::UnkClass_8046D144* self = this;
    if (flag) {
        if (!(field_0x6C & 1)) return;
        int i = 0;
        LODElem20* p20;
        LODElem48* p48;
        p48 = field_0x90;
        p20 = field_0x4;
        while (i < mCount_18) {
            if (p48->field_0x40 == task) p20->field_0x0 &= ~2u;
            i++;
            p48++;
            p20++;
        }
    } else {
        if (!(field_0x6C & 1)) return;
        int i = 0;
        LODElem20* p20;
        LODElem48* p48;
        p48 = field_0x90;
        p20 = field_0x4;
        while (i < mCount_18) {
            if (p48->field_0x40 == task) p20->field_0x0 |= 2u;
            i++;
            p48++;
            p20++;
        }
    }
}

// ---------------------------------------------------------------------------
// isElementFlagClear: when the LOD system is enabled, walk every element matching
// `task`; on the first match return whether bit 1 of its flag word is clear
// (bool; retail materializes !(flag&2) via rlwinm/cntlzw/srwi).
// ---------------------------------------------------------------------------
void* LOD::UnkClass_8046D144::isElementFlagClear(int task) {
    LOD::UnkClass_8046D144* self = this;
    if (field_0x6C & 1) {
        int i = 0;
        LODElem48* p48 = field_0x90;
        LODElem20* p20 = field_0x4;
        while (i < mCount_18) {
            if (p48->field_0x40 == task) {
                return (void*)!(p20->field_0x0 & 2);
            }
            i++;
            p48++;
            p20++;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// setElementLodLevel: when the LOD system is enabled, compute a fixed-point scale
// (u16 truncation of pool*f1) and store it into field_0xE of every element
// matching `task`.
// ---------------------------------------------------------------------------
void setElementLodLevel__Q23LOD17UnkClass_8046D144Fif(LOD::UnkClass_8046D144* self, int task, f32 f1) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    u16 v = (u16)(s32)(lbl_eu_8066A70C * f1);
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
        if (p48->field_0x40 == task) {
            p20->field_0xE = (s16)v;
        }
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// configureShrinkTargets: per-element enable/disable driven by a distance value.
// While the manager is enabled (bit 0 of +0x6C): when bit 0x400 is set the
// two cached element slots (+0x54/+0x58) get their flag word's bit 0x10
// re-set; then the +0x6C word has its 0x200/0x400 marker bits cleared, and
// a value greater than zero records up to two matching element pointers
// into +0x54/+0x58 (clearing their bit 0x10) and sets the 0x200/0x400 marker
// according to `flag`, while a non-positive value walks the matching
// elements setting/clearing their bit 0x10 per `flag`.
// ---------------------------------------------------------------------------
void configureShrinkTargets__Q23LOD17UnkClass_8046D144Ffsi(LOD::UnkClass_8046D144* self, f32 val, s16 task, int flag) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    u32 flags = l->field_0x6C;
    if (!(flags & 1)) return;
    if (flags & 0x400) {
        if (l->field_0x54 != 0) *(u32*)l->field_0x54 |= 0x10;
        if (l->field_0x58 != 0) *(u32*)l->field_0x58 |= 0x10;
    }
    f32 zero = lbl_eu_8066A6C0;
    // Reloaded after the aliasing |0x10 stores above.
    flags = l->field_0x6C & ~0x600u;
    l->field_0x6C = flags;
    if (val <= zero) {
        if (!(flags & 1)) return;
        int i;
        LODElem20* p20;
        LODElem48* p48 = l->field_0x90;
        p20 = l->field_0x4;
        i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                if (flag != 0) {
                    p20->field_0x0 &= ~0x10u;
                } else {
                    p20->field_0x0 |= 0x10u;
                }
            }
            i++;
            p48++;
            p20++;
        }
    } else {
        // Positive value: store the pair and remember up to two matches.
        u32* slot = (u32*)l;
        int found = 0;
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        l->field_0x88 = zero;
        l->field_0x8C = val;
        l->field_0x54 = 0;
        l->field_0x58 = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                // Cached-slot pair at +0x54/+0x58; the walker stays based at
                // the manager so the store keeps its 0x54 displacement.
                slot[0x15] = (u32)p20;
                slot++;
                found++;
                p20->field_0x0 &= ~0x10u;
                if (found >= 2) break;
            }
            i++;
            p48++;
            p20++;
        }
        if (found != 0) {
            if (flag != 0) {
                l->field_0x6C |= 0x200;
            } else {
                l->field_0x6C |= 0x400;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// setElementVisibility: when the LOD system is enabled, walk every element matching
// `task` and set/clear bit 0x10 of its flag word according to `flag`.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::setElementVisibility(int task, int flag) {
    LOD::UnkClass_8046D144* self = this;
    if (!(field_0x6C & 1)) return;
    LODElem48* p48 = field_0x90;
    LODElem20* p20 = field_0x4;
    int i = 0;
    while (i < mCount_18) {
        if (p48->field_0x40 == task) {
            if (flag != 0) {
                p20->field_0x0 &= ~0x20;
            } else {
                p20->field_0x0 |= 0x20;
            }
        }
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// setElementScale: if the LOD system is enabled, push the scale value into
// every element whose id matches `task`; elements with the follow-through
// flag bit and a nonzero enable field then refresh their animation state.
// ---------------------------------------------------------------------------
void setElementScale__Q23LOD17UnkClass_8046D144Fif(LOD::UnkClass_8046D144* self, int task, f32 v) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                p20->field_0x4 = v;
                if (p48->field_0x44 & 8) {
                    if (p48->field_0x42 != 0) {
                        ((LOD::UnkClass_8046A530*)p20)->checkLodFlagState();
                    }
                }
            }
            i++;
            p48++;
            p20++;
        }
    }
}

// ---------------------------------------------------------------------------
// refreshElementScale: same id walk as setElementScale, but the active value is
// refreshed from the cached value instead of a parameter.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::refreshElementScale(int task) {
    LOD::UnkClass_8046D144* self = this;
    if (field_0x6C & 1) {
        LODElem48* p48 = field_0x90;
        LODElem20* p20 = field_0x4;
        int i = 0;
        while (i < mCount_18) {
            if (p48->field_0x40 == task) {
                p20->field_0x4 = p20->field_0x8;
                if (p48->field_0x44 & 8) {
                    if (p48->field_0x42 != 0) {
                        ((LOD::UnkClass_8046A530*)p20)->checkLodFlagState();
                    }
                }
            }
            i++;
            p48++;
            p20++;
        }
    }
}

// ---------------------------------------------------------------------------
// getElementScale: when the LOD system is enabled, return the +0x4 float of the
// first element matching `task`, else the A6C0 pool constant.
// ---------------------------------------------------------------------------
f32 LOD::UnkClass_8046D144::getElementScale(int task) {
    LOD::UnkClass_8046D144* self = this;
    if (field_0x6C & 1) {
        LODElem48* p48 = field_0x90;
        LODElem20* p20 = field_0x4;
        int i = 0;
        while (i < mCount_18) {
            if (p48->field_0x40 == task) {
                return p20->field_0x4;
            }
            i++;
            p48++;
            p20++;
        }
    }
    return lbl_eu_8066A6C0;
}

// ---------------------------------------------------------------------------
// updateElementVisibility: when the LOD system is enabled, walk every element matching
// `task`; when `flag` is nonzero and the element's +0x42 is nonzero, clear
// bits 0x120 of its flag word; otherwise set bit 0x100 when +0x44 bit 3 is
// set, else bit 0x40.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateElementVisibility(int task, int flag) {
    LOD::UnkClass_8046D144* self = this;
    if (!(field_0x6C & 1)) return;
    LODElem48* p48 = field_0x90;
    LODElem20* p20 = field_0x4;
    int i = 0;
    while (i < mCount_18) {
        if (p48->field_0x40 == task) {
            if (flag != 0 && p48->field_0x42 != 0) {
                p20->field_0x0 &= ~0x40;
                p20->field_0x0 &= ~0x100;
            } else {
                if (p48->field_0x44 & 8) {
                    p20->field_0x0 |= 0x100;
                } else {
                    p20->field_0x0 |= 0x40;
                }
            }
        }
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// setElementFlag80: when the LOD system is enabled, walk every element matching
// `task` and set/clear bit 7 of its flag word according to `flag`.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::setElementFlag80(int task, int flag) {
    LOD::UnkClass_8046D144* self = this;
    if (!(field_0x6C & 1)) return;
    LODElem48* p48 = field_0x90;
    LODElem20* p20 = field_0x4;
    int i = 0;
    while (i < mCount_18) {
        if (p48->field_0x40 == task) {
            if (flag != 0) {
                p20->field_0x0 |= 0x80;
            } else {
                p20->field_0x0 &= ~0x80;
            }
        }
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// linkTaskElements: for each element matching `task`, when its byte state (+0x1E)
// differs from `value` and the 0x1000 flag is set, refresh +0x1C/+0x1E/+0x1F
// and link the element into the index-linked list (head/tail at +0x10/+0x14)
// unless it is already linked (0x10000 flag).
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::linkTaskElements(int task, int value) {
    LOD::UnkClass_8046D144* self = this;
    if (!(field_0x6C & 1)) return;
    LODElem48* p48 = field_0x90;
    LODElem20* p20 = field_0x4;
    int i = 0;
    while (i < mCount_18) {
        if (p48->field_0x40 == task) {
            int changed;
            if (p20->field_0x1E != value && (p20->field_0x0 & 0x1000)) {
                p20->field_0x0 |= 0x2000;
                changed = 1;
                p20->field_0x1F = p20->field_0x1E;
                p20->field_0x1E = value;
                p20->field_0x1C = 0;
            } else {
                changed = 0;
            }
            if (changed) {
                if (!(p20->field_0x0 & 0x10000)) {
                    if (field_0x14 == 0) {
                        field_0x10 = p20;
                    } else {
                        int own = p20->field_0x1A;  // int temp forces retail lha (sign-extend)
                        field_0x14->field_0x16 = own;
                    }
                    field_0x14 = p20;
                    p20->field_0x16 = lbl_eu_80663828[0];
                    p20->field_0x0 |= 0x10000;
                }
            }
        }
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// getElementState:
// first element matching `task`, or 0.
// ---------------------------------------------------------------------------
void* LOD::UnkClass_8046D144::getElementState(int task) {
    LOD::UnkClass_8046D144* self = this;
    if (field_0x6C & 1) {
        LODElem48* p48 = field_0x90;
        LODElem20* p20 = field_0x4;
        int i = 0;
        while (i < mCount_18) {
            if (p48->field_0x40 == task) {
                return (void*)p20->field_0x1E;
            }
            i++;
            p48++;
            p20++;
        }
    }
    return (void*)0;
}

// ---------------------------------------------------------------------------
// setElementCachedValue: when the LOD system is enabled, store `value` into the +0x8
// float of every element matching `task`.
// ---------------------------------------------------------------------------
void setElementCachedValue__Q23LOD17UnkClass_8046D144Fif(LOD::UnkClass_8046D144* self, int task, f32 value) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
        if (p48->field_0x40 == task) {
            p20->field_0x8 = value;
        }
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// getElementCachedValue: when the LOD system is enabled, return the +0x8 float of the
// first element matching `task`, else the A6C0 pool constant.
// ---------------------------------------------------------------------------
f32 LOD::UnkClass_8046D144::getElementCachedValue(int task) {
    LOD::UnkClass_8046D144* self = this;
    if (field_0x6C & 1) {
        LODElem48* p48 = field_0x90;
        LODElem20* p20 = field_0x4;
        int i = 0;
        while (i < mCount_18) {
            if (p48->field_0x40 == task) {
                return p20->field_0x8;
            }
            i++;
            p48++;
            p20++;
        }
    }
    return lbl_eu_8066A6C0;
}

// ---------------------------------------------------------------------------
// updateSingleElement: when the LOD system is enabled, scale the base value by the
// current frame delta and forward the matching element to updateElementLod.
// ---------------------------------------------------------------------------
void* LOD::UnkClass_8046D144::updateSingleElement(int id) {
    LOD::UnkClass_8046D144* self = this;
    if (field_0x6C & 1) {
        f32 dt = Scn_GetFrameDelta(mView_1C);
        field_0x80 = mFloat_7C * dt;
        LODElem48* p48 = field_0x90;
        LODElem20* p20 = field_0x4;
        int i = 0;
        while (i < mCount_18) {
            if (p48->field_0x40 == id) {
                updateElementLod(p20);
            }
            i++;
            p48++;
            p20++;
        }
    }
}

void LOD::UnkClass_8046D144::setBaseScale(float a) { ((LODMemManLayout*)this)->field_0x1CC8 = a; mFloat_7C = a; }

// ---------------------------------------------------------------------------
// configureFadeMode: when `mode` is nonzero, publish the `value` short into +0xBE
// and set flag bit 0x100 of +0x6C (forcing +0xBE to 1 when value >= 2);
// otherwise clear the flag.  Then scale +0x84 by 1/f1 unless f1 matches the
// A6C0 pool constant (then the stored scale is used as-is).
// ---------------------------------------------------------------------------
void configureFadeMode__Q23LOD17UnkClass_8046D144FiUsf(LOD::UnkClass_8046D144* self, int mode, u16 value, f32 f1) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (mode) {
        l->field_0xBE = value;
        l->field_0x6C |= 0x100;
        if ((s16)value >= 2) {
            l->field_0xBE = 1;
        }
    } else {
        l->field_0x6C &= ~0x100;
    }
    if (f1 != lbl_eu_8066A6C0) {
        l->field_0x84 = lbl_eu_8066A6D8 / f1;
    } else {
        l->field_0x84 = lbl_eu_8066A6D8;
    }
}

// ---------------------------------------------------------------------------
// updateViewLayers: per-frame update.  A nonzero scene-check result with the
// 0x20 flag of field_0x6C clear sets bit 4 of field_0x68, otherwise it is
// cleared.  Then the view descriptor's u16 group at +0x18 (count followed by
// element indices) refreshes each referenced box element, the +0x1C group
// feeds refreshGroupFromList, and the pass ends with updateElementsByDesc.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateViewLayers(LODViewDesc* view) {
    LOD::UnkClass_8046D144* self = this;
    if (ScnRes_IsStateActive_E4A0() != 0 && (field_0x6C & 0x20) == 0) {
        field_0x68 |= 0x10;
    } else {
        field_0x68 &= ~0x10;
    }
    if (view->field_0x18 != 0) {
        u8* p = field_0x5C;
        p += view->field_0x18;
        u16* entries = (u16*)(p + 2);
        u16 count = *(u16*)p;
        int i = 0;
        while (i < count) {
            updateBoxElement(&field_0x60[*entries]);
            i++;
            entries++;
        }
    }
    if (view->field_0x1C != 0) {
        u8* group = field_0x5C + view->field_0x1C;
        u16 value = *(u16*)group;
        refreshGroupFromList(value, group + 2);
    }
    updateElementsByDesc();
}

// ---------------------------------------------------------------------------
// updateBoxElement: if the element box overlaps the manager's box (a degenerate
// point at +0x20), refresh the element's u16 child groups the same way as
// updateViewLayers.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateBoxElement(LODBoxElem* box) {
    if (box->mMinX < field_0x20) return;
    if (box->mMinY < field_0x24) return;
    if (box->mMinZ < field_0x28) return;
    if (box->mMaxX > field_0x20) return;
    if (box->mMaxY > field_0x24) return;
    if (box->mMaxZ > field_0x28) return;
    if (box->field_0x18 != 0) {
        u8* p = field_0x5C;
        p += box->field_0x18;
        u16* entries = (u16*)(p + 2);
        u16 count = *(u16*)p;
        int i = 0;
        while (i < count) {
            updateBoxElement(&field_0x60[*entries]);
            i++;
            entries++;
        }
    }
    if (box->field_0x1C != 0) {
        u8* group = field_0x5C + box->field_0x1C;
        u16 value = *(u16*)group;
        refreshGroupFromList(value, group + 2);
    }
}

// refreshGroupFromList: per-pair element update driven by a { type, index }
// group list.  `value` counts the 4-byte pairs at `data` (u16 type index at
// +0x0, u16 element index at +0x2).  A per-element bitmap at +0x64 skips
// already-processed elements (and records the bit-0x2-flagged ones); every
// other element goes through the shared distance / type-match / plane-cull
// core (same shape as updateElementsByDesc): direct-model types bind through
// the scene, ResFile types resolve a slot, and survivors link into the
// element chains unless the Notify gate or the kind check stops them.
// The Fv retail symbol carries (value, data) in r4/r5; C linkage keeps the
// exact name while exposing the register args (cf. updateElementsByDesc).
// There are no member-form callers, so the member declaration is satisfied
// by this definition.
static inline f32 LODVecLenSq(const LODVec3f* v);
// Chain-hoist pinning (MWCC_PATTERNS mpv fix): retail recomputes the
// (u8*)self+0x20/0xCC/0xA44 derives at each use site, but the O4 scheduler
// speculatively hoists them into prologue saved regs.  scheduling-off pins
// advances to program order and stops the hoist.
void LOD::UnkClass_8046D144::refreshGroupFromList(u16 value, u8* data) {
    const f32 cZero = lbl_eu_8066A6C0;
    LODVec3f delta;
    LODVec3f work;
    for (int i = 0; i < value; i++, data += 4) {
        u16 idx = ((u16*)data)[1];
        if (field_0x64[idx >> 5] & (1u << (idx & 31))) {
            continue;
        }
        LODElem20* elem = &field_0x4[idx];
        if (elem->field_0x0 & 2) {
            field_0x64[idx >> 5] |= (1u << (idx & 31));
            continue;
        }
        u16 t = ((u16*)data)[0];
        LODTypeDesc18* td = &field_0x94[t];
        LODElem48* ed = &field_0x90[idx];
        // Paired-single SDK helpers (cf. MWCC_PATTERNS §1g): scalar f32 math
        // never fuses into the retail psq_l/ps_sub/ps_sum0 sequences.
        nw4r::math::VEC3Sub(
            (nw4r::math::VEC3*)&delta,
            (const nw4r::math::VEC3*)&ed->field_0x0,
            (const nw4r::math::VEC3*)&field_0x20);
        f32 distSq = nw4r::math::VEC3Dot(
            (const nw4r::math::VEC3*)&delta,
            (const nw4r::math::VEC3*)&delta);
        f32 lim = td->field_0xC;
        lim *= lim;
        if (!(distSq < lim)) {
            continue;
        }
        if (td->field_0x6 == 1) {
            field_0x64[idx >> 5] |= (1u << (idx & 31));
            bindResMdl__Q23LOD9LODMemManFv(
                (LOD::UnkClass_8046D144*)&this->mG3dMgr, (CScn*)mView_1C,
                td->field_0x0, idx, (const f32*)ed);
            continue;
        }
        LODElem20* rec = &field_0x4[idx];
        s32 slot;
        if (td->mFlags & 0x8000) {
            if (acquireResFileSlot__Q23LOD9LODMemManFv(
                    (LOD::UnkClass_8046D144*)&this->mG3dMgr,
                    (LOD::UnkClass_8046D144*)&this->mMemMan, td->field_0x0,
                    &slot) == 0) {
                continue;
            }
            rec->field_0x0 |= 1;
        } else {
            slot = t;
            rec->field_0x0 &= ~1u;
        }
        field_0x64[idx >> 5] |= (1u << (idx & 31));
        f32 rad = ed->field_0x38;
        f32 rad2 = rad * rad;
        if (rad2 < distSq) {
            f32 proj = nw4r::math::VEC3Dot(
                (const nw4r::math::VEC3*)&field_0x2C,
                (const nw4r::math::VEC3*)&delta);
            f32 g = proj * field_0x38;
            if (proj <= cZero && rad2 < g * g) {
                continue;
            }
            nw4r::math::VEC3Scale(
                (nw4r::math::VEC3*)&work,
                (const nw4r::math::VEC3*)&field_0x2C, g);
            nw4r::math::VEC3Sub(
                (nw4r::math::VEC3*)&work,
                (const nw4r::math::VEC3*)&delta,
                (const nw4r::math::VEC3*)&work);
            f32 limv = g * field_0x74 + rad * field_0x78;
            f32 workSq = nw4r::math::VEC3Dot(
                (const nw4r::math::VEC3*)&work,
                (const nw4r::math::VEC3*)&work);
            if (!(workSq < limv * limv)) {
                continue;
            }
        }
        if (field_0x68 & 0x10) {
            if (ed->field_0x44 & 1) {
                if (ScnRes_Notify2Word_E458((u32)&field_0x20, (u32)ed)) {
                    continue;
                }
            }
        }
        if (rec->field_0xE == 0) {
            continue;
        }
        rec->field_0x18 = (u16)slot;
        linkElementToChains(rec, t, &delta);
    }
}

// Squared length of *v.
// memory-resident, mirroring retail's store/reload shape.
static inline f32 LODVecLenSq(const LODVec3f* v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

// ---------------------------------------------------------------------------
// updateElementsByDesc: per-descriptor element pass.  For every type descriptor the
// shared-buffer u16 index list selects elements; each active element gets its
// distance to the manager computed, is matched against the descriptor's type
// table (direct model / ResFile slot / id-only paths), culled against the
// manager plane pair, and finally updated through linkElementToChains with an
// optional rotation matrix built from SinFIdx/CosFIdx.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateElementsByDesc() {
    if (field_0xA4 == 0) {
        return;
    }
    const f32 cZero = lbl_eu_8066A6C0;
    const f32 cAng = ml::deg2rad;
    const f32 cFIdx = lbl_eu_8066A6F8;
    const f32 cOne = lbl_eu_8066A6D8;
    LODOutRec* out = (LODOutRec*)field_0xC;
    LODVec3f delta;
    LODVec3f work;
    LODDesc74* d = field_0x9C;

    for (int di = 0; di < field_0xA4; di++, d++) {
        u16* words = (u16*)(field_0x5C + d->field_0x18);
        u16 tcount = d->field_0x6;
        for (int k = 0; k < d->field_0x4; k++, out++) {
            u16 idx = words[k];
            LODElem20* rec = field_0x4 + idx;
            if (rec->field_0x0 & 2) {
                continue;
            }
            LODElem48Full* ed = (LODElem48Full*)field_0x90 + idx;

            // Delta between the output position and the manager position.
            LODVec3f delta;
            delta.x = out->mPos[0] - field_0x20;
            delta.y = out->mPos[1] - field_0x24;
            delta.z = out->mPos[2] - field_0x28;
            f32 distSq = delta.x * delta.x + delta.y * delta.y +
                         delta.z * delta.z;

            // One stack slot backs the resolved bind id: the ResFile path
            // fills it through the callee's out-pointer.
            s32 slot = 0;
            for (int j = 0; j < tcount; j++) {
                u16 t = d->mTypes[j];
                LODTypeDesc18* tbl = &field_0x94[t];
                f32 lim = tbl->field_0xC * tbl->field_0xC;
                if (!(distSq < lim)) {
                    continue;
                }
                if (tbl->field_0x6 == 1) {
                    // Direct model reference.
                    bindResMdl__Q23LOD9LODMemManFv(
                        (LOD::UnkClass_8046D144*)&this->mG3dMgr, (CScn*)mView_1C,
                        tbl->field_0x0, idx, (const f32*)ed);
                    goto next_elem;
                } else if (tbl->mFlags & 0x8000) {
                    // ResFile slot path; on failure keep scanning types.
                    if (acquireResFileSlot__Q23LOD9LODMemManFv(
                            (LOD::UnkClass_8046D144*)&this->mG3dMgr,
                            (LOD::UnkClass_8046D144*)&this->mMemMan, tbl->field_0x0,
                            &slot) == 0) {
                        continue;
                    }
                    rec->field_0x0 |= 1;
                } else {
                    slot = t;
                    rec->field_0x0 &= ~1u;
                }

                // Plane culling against the manager's radius planes.
                // The culling radius sits at ed+0x38 (mMtx[11] in this view).
                f32 rad = ed->mMtx[11];
                f32 rad2 = rad * rad;
                if (rad2 < distSq) {
                    f32 proj = field_0x2C * delta.x + field_0x30 * delta.y +
                               field_0x34 * delta.z;
                    f32 g = proj * field_0x38;
                    if (proj <= cZero && rad2 < g * g) {
                        continue;
                    }
                    work.x = delta.x - field_0x2C * g;
                    work.y = delta.y - field_0x30 * g;
                    work.z = delta.z - field_0x34 * g;
                    f32 limv = g * field_0x74 + rad * field_0x78;
                    if (LODVecLenSq(&work) > limv * limv) {
                        continue;
                    }
                }

                // Apply: record the resolved bind id and update the chain.
                if (rec->field_0xE == 0) {
                    continue;
                }
                rec->field_0x18 = (u16)slot;
                linkElementToChains(rec, t, &delta);
                if (d->field_0x24 & 0x1C) {
                    // Rotation path: refresh the record's 3x3 from the
                    // element descriptor, then post-multiply the enabled
                    // axis rotations in bit order Y/X/Z.
                    f32* om = out->mMtx;
                    f32 scaleV = out->mOutput;
                    bool built = false;
                    f32 rm[12];
                    om[0] = ed->mMtx[0]; om[1] = ed->mMtx[1];
                    om[2] = ed->mMtx[2]; om[3] = cZero;
                    om[4] = ed->mMtx[3]; om[5] = ed->mMtx[4];
                    om[6] = ed->mMtx[5]; om[7] = cZero;
                    om[8] = ed->mMtx[6]; om[9] = ed->mMtx[7];
                    om[10] = ed->mMtx[8]; om[11] = cZero;
                    if (d->field_0x24 & 0x04) {
                        f32 prod = scaleV * d->field_0x34 * cAng;
                        f32 s = nw4r::math::SinFIdx(cFIdx * prod);
                        f32 c = nw4r::math::CosFIdx(cFIdx * prod);
                        rm[0] = cOne; rm[1] = cZero; rm[2] = cZero; rm[3] = cZero;
                        rm[4] = cZero; rm[5] = c;    rm[6] = -s;    rm[7] = cZero;
                        rm[8] = cZero; rm[9] = s;    rm[10] = c;    rm[11] = cZero;
                        built = true;
                    }
                    if (d->field_0x24 & 0x08) {
                        f32 prod = scaleV * d->field_0x38 * cAng;
                        f32 s = nw4r::math::SinFIdx(cFIdx * prod);
                        f32 c = nw4r::math::CosFIdx(cFIdx * prod);
                        if (built) {
                            f32 tm[12];
                            tm[0] = c;     tm[1] = cZero; tm[2] = s;     tm[3] = cZero;
                            tm[4] = cZero; tm[5] = cOne;  tm[6] = cZero; tm[7] = cZero;
                            tm[8] = -s;    tm[9] = cZero; tm[10] = c;    tm[11] = cZero;
                            PSMTXConcat((f32(*)[4])tm, (f32(*)[4])rm, (f32(*)[4])rm);
                        } else {
                            rm[0] = c;     rm[1] = cZero; rm[2] = s;     rm[3] = cZero;
                            rm[4] = cZero; rm[5] = cOne;  rm[6] = cZero; rm[7] = cZero;
                            rm[8] = -s;    rm[9] = cZero; rm[10] = c;    rm[11] = cZero;
                            built = true;
                        }
                    }
                    if (d->field_0x24 & 0x10) {
                        f32 prod = scaleV * d->field_0x3C * cAng;
                        f32 s = nw4r::math::SinFIdx(cFIdx * prod);
                        f32 c = nw4r::math::CosFIdx(cFIdx * prod);
                        if (built) {
                            f32 tm[12];
                            tm[0] = c;     tm[1] = -s;    tm[2] = cZero; tm[3] = cZero;
                            tm[4] = s;     tm[5] = c;     tm[6] = cZero; tm[7] = cZero;
                            tm[8] = cZero; tm[9] = cZero; tm[10] = cOne;  tm[11] = cZero;
                            PSMTXConcat((f32(*)[4])tm, (f32(*)[4])rm, (f32(*)[4])rm);
                        } else {
                            rm[0] = c;     rm[1] = -s;    rm[2] = cZero; rm[3] = cZero;
                            rm[4] = s;     rm[5] = c;     rm[6] = cZero; rm[7] = cZero;
                            rm[8] = cZero; rm[9] = cZero; rm[10] = cOne;  rm[11] = cZero;
                            built = true;
                        }
                    }
                    PSMTXConcat((f32(*)[4])om, (f32(*)[4])rm, (f32(*)[4])om);
                    // Restore the record's own translation column.
                    om[3] = out->mPos[0];
                    om[7] = out->mPos[1];
                    om[11] = out->mPos[2];
                } else {
                    // No rotation: plain copy keeping the record translation.
                    f32 px = out->mPos[0];
                    f32 py = out->mPos[1];
                    f32 pz = out->mPos[2];
                    f32* dst = out->mMtx;
                    dst[0] = ed->mMtx[0]; dst[1] = ed->mMtx[1];
                    dst[2] = ed->mMtx[2]; dst[3] = px;
                    dst[4] = ed->mMtx[3]; dst[5] = ed->mMtx[4];
                    dst[6] = ed->mMtx[5]; dst[7] = py;
                    dst[8] = ed->mMtx[6]; dst[9] = ed->mMtx[7];
                    dst[10] = ed->mMtx[8]; dst[11] = pz;
                }
                break;
            }
        next_elem:;
        }
    }
}

// ---------------------------------------------------------------------------
// refreshPaletteBinding: texture-palette binding refresh.  While flag bits 0x800 and
// 0x2000 are set, walk the scene's model list looking for the model registered
// at +0xB4; when found (and the 0x1000 bit was still clear), set it and resolve
// every texture name of the shared-buffer texture table against the model's
// ResMdl, storing each resolved ResTexPlttInfo pointer into the u32 array at
// +0xB0.  If no model matched, clear +0xB4 and drop flag bits 0x180000.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::refreshPaletteBinding() {
    u32 flags = ((LODMemManLayout*)this)->field_0x6C;
    if (!(flags & 0x800)) return;
    if (!(flags & 0x2000)) return;
    bool notFound = true;
    // The item-pool sub-list lives behind the scene's +0x60 pointer.
    LODSceneNode* anchor =
        ((LODSceneList*)CScnItemPool_lookupSubPool(
             ((CScn*)mView_1C)->mItemPool, 1))->mAnchor;
    for (LODSceneNode* n = anchor->mNext; n != anchor; n = n->mNext) {
        if ((u8*)n->mObj != field_0xB4) continue;
        notFound = false;
        if (!(field_0x6C & 0x1000)) field_0x6C |= 0x1000;
        // Texture-name table resolved ahead of the downcast (both pointers
        // stay live across the RTTI helper call).
        u8* buf = field_0x5C;
        LODTexGroup* group = (LODTexGroup*)(buf + *(u32*)(buf + 0x70));
        u32* entries = group->mOffsets;
        // Downcast the scene item to the nw4r model wrapper (RTTI walk);
        // retail passes the target locator twice (src2dst slot).
        LODMdlItemView* mdlItem = (LODMdlItemView*)__dynamic_cast(
            n->mObj, &lbl_eu_806624C0, &lbl_eu_806624D8,
            (long)&lbl_eu_806624D8, 0);
        if (mdlItem == NULL) break;
        nw4r::g3d::ResMdl resMdl(mdlItem->field_0x146C);
        u32 numEntries = resMdl.GetResTexPlttInfoOffsetFromTexNameNumEntries();
        int count = group->mCount;
        int outIdx = 0;
        for (int i = 0; i < count; i++, outIdx++, entries++) {
            const char* name = (const char*)((u8*)group + *entries);
            for (int j = 0; j < (int)numEntries; j++) {
                nw4r::g3d::ResTexPlttInfo info =
                    resMdl.GetResTexPlttInfoOffsetFromTexName(j);
                if (info.ptr() == NULL)
                    nw4r::db::Panic(lbl_eu_8056DA30, 0x20, lbl_eu_8056DA10,
                                    lbl_eu_8056D9C0, &lbl_eu_8066383C);
                if (*(u32*)info.ptr() == 0)
                    nw4r::db::Panic(lbl_eu_8056DA78, 0x26, lbl_eu_8056DA48);
                if (info.ptr() == NULL)
                    nw4r::db::Panic(lbl_eu_8056D9F8, 0x20, lbl_eu_8056D9D8,
                                    lbl_eu_8056D9C0, &lbl_eu_80663838);
                // Follow the palette offset (+0x08); result must stay aligned.
                u8* tex = (u8*)info.ptr();
                if (*(u32*)(tex + 8) != 0) {
                    tex += *(u32*)(tex + 8);
                    if ((u32)tex & 3)
                        nw4r::db::Panic(lbl_eu_8056DAB8, 0x230, lbl_eu_8056DA90);
                } else {
                    tex = 0;
                }
                // Store/reload the resolved pointer through +0xB0 (retail
                // re-reads the base for the null check).
                ((u32*)field_0xB0)[outIdx] = (u32)tex;
                if (((u32*)field_0xB0)[outIdx] == 0)
                    nw4r::db::Panic(lbl_eu_8056DAF8, 0x230, lbl_eu_8056DAD8,
                                    lbl_eu_8056DAC8, &lbl_eu_80663840);
                const char* texName =
                    tex ? (const char*)(tex + *(u32*)tex) : 0;
                if (strcmp(texName, name) == 0) break;
            }
        }
        break;
    }
    if (notFound) {
        field_0xB4 = 0;
        field_0x6C &= ~0x1800u;
    }
}

// ---------------------------------------------------------------------------
// unlinkElement: unlink `elem` from the index-linked list.  Each element's
// +0x16 holds the next element index (sentinel lbl_eu_80663828) and +0x1A the
// element's own index; head removal advances the head, empty-list removal
// clears both pointers, and mid-list removal splices the successor into the
// predecessor's link.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::unlinkElement(LODElem20* elem) {
    if (!(elem->field_0x0 & 0x10000)) return;
    elem->field_0x0 &= ~0x10000;
    if (field_0x14 == 0 || field_0x10 == 0) {
        elem->field_0x16 = lbl_eu_80663828[0];
        return;
    }
    if (field_0x10 == elem) {
        int next = elem->field_0x16;
        if (next != lbl_eu_80663828[0]) {
            field_0x10 = &field_0x4[next];
        } else {
            field_0x10 = 0;
            field_0x14 = 0;
        }
    } else {
        // Mid-list removal: walk from the head via +0x1A indices until the
        // predecessor of `elem` is found.  The sentinel is kept live across
        // the loop; field_0x4 and elem->field_0x16 are re-read per iteration.
        s16 elemIdx = elem->field_0x1A;
        int idx = field_0x10->field_0x1A;
        while (idx != lbl_eu_80663828[0]) {
            LODElem20* p = &field_0x4[idx];
            if (p->field_0x16 == elemIdx) {
                p->field_0x16 = elem->field_0x16;
                if (field_0x14 == elem) field_0x14 = p;
                break;
            }
            idx = p->field_0x16;
        }
    }
    elem->field_0x16 = lbl_eu_80663828[0];
}

// ---------------------------------------------------------------------------
// linkElementToChains: per-element chain update.  The element is linked into the
// +0x8 pair array (six pointers per id: {elem,next} for each of the three
// chain slots).  The descriptor's flag bits select the chain slots; bits 0/1
// of the +0x4 flags also drive the +0x0 flag-word update.  When the 0x80
// descriptor bit is set the element's +0xC distance is nudged toward the
// 0..0xff range based on the squared horizontal length of `v`.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::linkElementToChains(
    LODElem20* elem, u16 id, const LODVec3f* v)
{

    LODTypeDesc18* desc = &field_0x94[id];
    int base = id * 6;
    s32 flag1 = 0;
    s32 flag2 = 0;
    field_0x70 |= desc->mFlags;
    field_0x68 |= 1;
    if (elem->field_0xE != 0xff) {
        flag1 = 1;
        flag2 = 1;
    } else if (elem->field_0x0 & 0x8000) {
        flag1 = 1;
    }
    if (desc->mFlags & 0x80) {
        f32 dist2 = v->x * v->x + v->z * v->z;
        s32 t = elem->field_0xC;
        if (dist2 < desc->mThreshold) {
            t -= 26;
            if (t < 0) {
                t = 0;
            } else {
                flag1 = 1;
                flag2 = 1;
            }
        } else {
            t += 51;
            if (t > 0xff) {
                t = 0xff;
            } else {
                flag1 = 1;
                flag2 = 1;
            }
        }
        elem->field_0xC = (s16)t;
    }
    if (flag1) {
        if (flag2 && (desc->mFlags & 1)) {
            elem->field_0x0 |= 0x8404;
            if (desc->mFlags & 2) {
                elem->field_0x0 |= 0x4000;
            } else {
                elem->field_0x0 &= ~0x4000u;
                field_0x68 |= 4;
                if (((LODBuildList*)this)->mPairs[base + 2] != 0) {
                    ((LODLinkNode*)((LODBuildList*)this)->mPairs[base + 3])
                        ->mLinks[1] = elem->field_0x1A;
                } else {
                    ((LODBuildList*)this)->mPairs[base + 2] = elem;
                }
                ((LODBuildList*)this)->mPairs[base + 3] = elem;
                return;
            }
        } else {
            elem->field_0x0 &= ~0x8000u;
            elem->field_0x0 &= ~0x4u;
            elem->field_0x0 &= ~0x400u;
            elem->field_0x0 &= ~0x4000u;
        }
    }
    for (int i = 0, w = base; i < 3; i++, w += 2) {
        if (desc->mFlags & (1 << i)) {
            field_0x68 |= (2 << i);
            if (((LODBuildList*)this)->mPairs[w] != 0) {
                ((LODLinkNode*)((LODBuildList*)this)->mPairs[w + 1])
                    ->mLinks[i] = elem->field_0x1A;
            } else {
                ((LODBuildList*)this)->mPairs[w] = elem;
            }
            ((LODBuildList*)this)->mPairs[w + 1] = elem;
        }
    }
}

// ---------------------------------------------------------------------------
// rebuildElementChains: rebuild the six element chains from the pair array at +0x8.
// Each pair is { element, next }; the element's bit-0 flag selects the
// bit0-set chain (heads at +0x48) or the bit0-clear chain (heads at +0x3C).
// Element i of a group links into the group's chain through the u16 link
// field at +0x10+2*i, and each chain's final tail is terminated with the
// sentinel index.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::rebuildElementChains(int count) {
    LOD::UnkClass_8046D144* self = this;
    LODBuildList* l = (LODBuildList*)self;
    // Retail strength-reduces the inner loop to pointer/offset induction
    // (mtctr/bdnz over 3, pre-advanced tail/head/pair pointers), so the
    // source keeps explicit inductions instead of [i]/[w] indexing.
    // Declaration order drives saved-reg colors (ascending): bo, t1, off,
    // hb, t2, bound, w.  Assignments stay in prologue order (bound first).
    int bo;
    LODElem20** t1;
    int off;
    u8* hb;
    LODElem20** t2;
    int bound;
    int w;
    LODElem20* tail2[3] = { 0, 0, 0 };
    LODElem20* tail1[3] = { 0, 0, 0 };
    bound = count * 6;
    w = 0;
    bo = 0;
    while (w < bound) {
        t1 = tail1;
        off = 0;
        hb = (u8*)l;
        t2 = tail2;
        for (int k = 0; k < 3; k++) {
            LODElem20* elem = *(LODElem20**)((u8*)l->mPairs + bo);
            if (elem != 0) {
                if (elem->field_0x0 & 1) {
                    if (*t1 != 0) {
                        *(s16*)((u8*)*t1 + 0x10 + off) = elem->field_0x1A;
                    } else {
                        *(LODElem20**)(hb + 0x48) = elem;
                    }
                    *t1 = *(LODElem20**)((u8*)l->mPairs + bo + 4);
                } else {
                    if (*t2 != 0) {
                        *(s16*)((u8*)*t2 + 0x10 + off) = elem->field_0x1A;
                    } else {
                        *(LODElem20**)(hb + 0x3C) = elem;
                    }
                    *t2 = *(LODElem20**)((u8*)l->mPairs + bo + 4);
                }
            }
            t1++;
            off += 2;
            hb += 4;
            t2++;
            bo += 8;
            w += 2;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (tail2[i] != 0) {
            ((LODLinkNode*)tail2[i])->mLinks[i] = (u16)lbl_eu_80663828[0];
        }
        if (tail1[i] != 0) {
            ((LODLinkNode*)tail1[i])->mLinks[i] = (u16)lbl_eu_80663828[0];
        }
    }
}

// ---------------------------------------------------------------------------
// updateElementLod: per-element LOD update.  While both 0x1000/0x2000 flags are
// set the +0x1C byte counts toward 0x5A; reaching it clears 0x2000 and (when
// the element's descriptor has no enable flag) unlinks the element from the
// index list.  The main pass computes the signed frame step (byte param
// scaled by the frame value, or the frame value itself) and clamps the
// element value between the cached upper bound and the zero lower bound;
// with the descriptor's bit-3 flag a +0x98 record supplies both bounds.
// A changed or already-linked element is re-linked (the 0x100 flag restores
// the old value and sets bit 6), the step-scaled value is published to the
// shared LOD distance globals, and the record's render bits drive the
// billboard/submit helpers.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateElementLod(LODElem20* elem) {
    u32 flags0 = elem->field_0x0;
    if ((flags0 & 0x1000) && (flags0 & 0x2000)) {
        s32 next = elem->field_0x1C + 1;
        if (next >= 0x5A) {
            // Expired element: clear the enable bit; unlink when idle.
            elem->field_0x0 = flags0 & ~0x2000;
            LODElem48* d0 = &field_0x90[elem->field_0x1A];
            if (d0->field_0x42 == 0) {
                unlinkElement(elem);
            }
        } else {
            elem->field_0x1C = next;
        }
    }
    // Reload the (possibly rewritten) flag word; kept cached in a register.
    u32 flags = elem->field_0x0;
    if ((flags & 0x40) != 0 || (flags & 0x2) != 0) {
        return;
    }
    {
        LODElem48* desc = &field_0x90[elem->field_0x1A];
        f32 lo;
        f32 hi = elem->field_0x8;   // upper bound (cached value)
        lo = lbl_eu_8066A6C0;       // lower bound (0.0f)
        f32 orig;
        u16 d44 = desc->field_0x44;
        f32 step;
        if (d44 & 2) {
            // Integer byte parameter -> float via the 0x4330_0000 exponent
            // trick (MWCC expands the direct cast itself).
            f32 base = lbl_eu_8066A710 * (f32)desc->field_0x46;
            step = base * field_0x80;
        } else {
            step = field_0x80;
        }

        if (desc->field_0x44 & 8) {
            orig = elem->field_0x4;
            LODRec1C* rec = &field_0x98[desc->field_0x47];
            if (rec->field_0x0 & 4) {
                // Optional record clamp threshold (u16 -> float).
                f32 th = (f32)rec->field_0xA;
                if (orig >= th) {
                    u32 bit7 = flags & 0x80;
                    if (bit7) {
                        hi = th;
                    }
                    if (!bit7) {
                        lo = th;
                    }
                }
            }
            if (flags & 0x80) {
                step = -step;
                elem->field_0x4 += step;
                if (lo > elem->field_0x4) {
                    if (elem->field_0x0 & 0x20) {
                        elem->field_0x4 = lo;
                    } else {
                        elem->field_0x4 = hi - (lo - elem->field_0x4);
                    }
                }
            } else {
                elem->field_0x4 += step;
                if (hi <= elem->field_0x4) {
                    if (elem->field_0x0 & 0x20) {
                        elem->field_0x4 = hi;
                    } else {
                        elem->field_0x4 = lo + (elem->field_0x4 - hi);
                    }
                }
            }
            if (orig != elem->field_0x4 || (elem->field_0x0 & 0x20000)) {
                int changed = 0;
                u32 f0 = elem->field_0x0;
                u32 fn = f0 & ~0x20000;
                elem->field_0x0 = fn;
                if (f0 & 0x100) {
                    // Wrap: restore the old value, trade the direction flag
                    // for the disable bit.
                    elem->field_0x4 = orig;
                    fn &= ~0x100;
                    fn |= 0x40;
                    elem->field_0x0 = fn;
                    changed = 1;
                }
                f32 v0 = elem->field_0x4;
                f32 v = v0 + step * field_0x1CDC;
                lbl_eu_80665754 = v;
                lbl_eu_80665758 = (s16)v;
                if (rec->field_0x0 & 2) {
                    if (ColiLodLookupRecordBin(rec) != 0) {
                        void* p = getScnHandle();
                        ScnData_FwdB7D9C(p, desc->field_0x40);
                    } else {
                        void* p = getScnHandle();
                        ScnData_FwdB7DD4(p, desc->field_0x40);
                    }
                }
                if (rec->field_0x0 & 1) {
                    submitLodBillMtx__Q23LOD17UnkClass_80468434Fv(
                        rec->field_0x8, (const f32*)desc, elem->field_0x1D);
                    if (changed) {
                        void* p = getScnHandle();
                        ScnData_FwdB8078(p, elem->field_0x1D);
                    }
                }
            } else {
                if (rec->field_0x0 & 1) {
                    void* p = getScnHandle();
                    ScnData_FwdB8078(p, elem->field_0x1D);
                }
            }
        } else {
            if (flags & 0x80) {
                step = -step;
                elem->field_0x4 += step;
                if (lo > elem->field_0x4) {
                    if (elem->field_0x0 & 0x20) {
                        elem->field_0x4 = lo;
                    } else {
                        elem->field_0x4 = hi - (lo - elem->field_0x4);
                    }
                }
            } else {
                elem->field_0x4 += step;
                if (hi <= elem->field_0x4) {
                    if (elem->field_0x0 & 0x20) {
                        elem->field_0x4 = hi;
                    } else {
                        elem->field_0x4 = lo + (elem->field_0x4 - hi);
                    }
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// updateLodFrame: per-frame LOD update.  For each 0x74-byte type descriptor
// the referenced u16 group (shared buffer + desc->field_0x18) drives a
// per-element pass: each element's value is nudged toward its limit, a
// byte-parameter animation value is folded in, and the element's base
// position (plus a scaled PS vector) is written to the 0x48-byte output
// records.  The index field_0xE of the matched element is set from the
// current scale so the caller can pick the LOD kind.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::updateLodFrame() {
    int countA4 = field_0xA4;
    if (countA4 == 0) return;
    LODDesc74* desc = field_0x9C;
    LODOut48* out = (LODOut48*)field_0xC;
    union {
        double d;
        u32 w[2];
    } c;
    nw4r::math::VEC3 tmp;
    f64 magic = lbl_eu_8066A6E0;
    f32 c0 = lbl_eu_8066A710;
    f32 c11 = lbl_eu_8066A6D8;
    f32 c9 = lbl_eu_8066A70C;
    f32 c6 = lbl_eu_8066A714;
    double scale = lbl_eu_80665828;
    u8* ps = (u8*)desc + countA4 * 0x74;  // PS vector stream after descriptors
    int i = 0;
    while (i < field_0xA4) {
        f32 f2 = c11 / desc->field_0x44;
        f32 f3 = desc->field_0x40 * f2;
        double cur = (f32)scale;
        u16* group = (u16*)(field_0x5C + desc->field_0x18);
        int j = 0;
        while (j < desc->field_0x4) {
            LODElem48* d48 = &field_0x90[*group];
            if (!(desc->field_0x24 & 1)) {
                f32 v;
                if (d48->field_0x44 & 2) {
                    // Integer byte parameter -> double via the 0x4330_0000
                    // exponent trick, scaled and folded into the element value.
                    c.w[1] = d48->field_0x46;
                    c.w[0] = 0x43300000u;
                    v = (f32)(c.d - magic) * c0 * field_0x80 + out->field_0x40;
                } else {
                    v = field_0x80 + out->field_0x40;
                }
                if (desc->field_0x44 <= v) v -= desc->field_0x44;
                out->field_0x40 = v;
                if (lbl_eu_8066582C == 0) {
                    lbl_eu_8066582C = 1;
                    scale = c11 / lbl_eu_80663830;
                    lbl_eu_80665828 = scale;
                    cur = scale;
                }
                s32 idx = 0xff;
                LODElem20* elem = &field_0x4[*group];
                if (out->field_0x40 >= desc->field_0x44 - lbl_eu_80663830) {
                    idx = (s32)((f32)cur *
                                (c9 * (desc->field_0x44 - out->field_0x40)));
                } else if (out->field_0x40 <= lbl_eu_80663830) {
                    idx = (s32)((f32)cur * (c9 * out->field_0x40));
                }
                elem->field_0xE = (s16)idx;
            }
            if (d48->field_0x44 & 0x40) {
                nw4r::math::VEC3Scale(&tmp, (const nw4r::math::VEC3*)ps,
                                      f3 * out->field_0x40);
                ps += 0xc;
            } else {
                nw4r::math::VEC3Scale(&tmp, &desc->field_0x28,
                                      f3 * out->field_0x40);
            }
            f32 s0 = d48->field_0x0 + tmp.x;
            f32 s1 = d48->field_0x4 + tmp.y;
            f32 s2 = d48->field_0x8 + tmp.z;
            out->field_0x30 = *(u32*)&s0;
            out->field_0x34 = *(u32*)&s1;
            out->field_0x38 = *(u32*)&s2;
            out->field_0x44 = f2 * out->field_0x40 * c6 * out->field_0x3C;
            out++;
            group++;
            j++;
        }
        desc++;
        i++;
    }
}

struct LODChildEntry;
struct LODChildRec;
struct LODChild188;

// 0x40-byte child descriptor used by bindLodChildren's group walk and by
// registerChildSlot's slot registration.
struct LODChildDesc {
    u8  mPad_00[0x10];
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    u8  mPad_20[0x30 - 0x20];
    u32 field_0x30;            // 0x30 gate word (bit 16: seen this frame)
    s32 field_0x34;            // kind (3 or 4 -> lbl_eu_80658368 path)
    f32 field_0x38;            // scale
    u8  mPad_3C[0x40 - 0x3C];  // 64-byte stride
};

extern "C" void registerChildSlot__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, LODChildDesc* desc, u8* mtx, int index);

// ---------------------------------------------------------------------------
// bindLodChildren: per-frame LOD child binding.  Copies the caller's 13-word
// view matrix block onto the stack, inverts it in place (PSMTXInverse),
// resets the three child records (resetChildMasks) and the light color
// (resetLightColor), then for each of the three groups walks its 64-strided
// descriptor array and registers every element with the +0x30 bit 16 set
// (registerChildSlot), recording "seen" (2) vs "unseen" (0) into the two
// per-group .bss arrays.
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::bindLodChildren(const f32* matrixBlock) {
    Mtx mtx;
    *(nw4r::math::MTX34*)mtx = *(const nw4r::math::MTX34*)matrixBlock;
    PSMTXInverse(mtx, mtx);
    mG3dMgr.resetChildMasks();
    mG3dMgr.resetLightColor();

    // Group table: view -> +0x64 -> +0x8, plus the leading 0x14 skip.
    int* flagA = lbl_eu_80658368;
    int* flagB = lbl_eu_80658374;
    // Group table: view -> +0x64 -> +0x8, plus the leading 0x14 skip.
    u8* view = mView_1C;
    u8* tbl = *(u8**)(view + 0x64);
    tbl = *(u8**)(tbl + 8);
    u8* grp = tbl + 0x14;
    int g = 0;
    do {
        *flagA = 2;
        int seen = 0;
        int i = 0;
        LODChildDesc* d = *(LODChildDesc**)(grp + 0x1144);
        s32 count = *(s32*)(grp + 0x1154);
        while (i < count) {
            if (d->field_0x30 & 0x10000) {
                registerChildSlot__Q23LOD9LODMemManFv(
                    (LOD::UnkClass_8046D144*)&this->mG3dMgr, d, (u8*)mtx, g);
                seen++;
            }
            i++;
            d++;
        }
        if (seen != 0) {
            *flagB = 2;
        } else {
            *flagB = 0;
        }
        grp += 4;
        flagB++;
        flagA++;
        g++;
    } while (g < 3);
}

// ---------------------------------------------------------------------------
// dispatchViewUpdate: resolve the current view frame and, when the caller passes a
// nonzero task, push the layer scale and the frame data into the sub-manager
// (updateLodLayersForView is skipped while the scene gate byte is set).
// ---------------------------------------------------------------------------
void LOD::UnkClass_8046D144::dispatchViewUpdate(LOD::UnkClass_8046D144* self, int task) {
    CView* view = CView::getCurrentView();
    LODViewFrame* frame = Scn_HasCamItem(Scn_GetCurrentScene(), view);
    LOD::UnkClass_8046D144* sub = (LOD::UnkClass_8046D144*)self->mG3dMgr.mChildren[2].mBlock;
    if (task != 0) {
        if (((LODScnGate*)getScnRootPtr(Scn_GetCurrentScene()))->field_0x19 == 0) {
            sub->updateLodLayersForView(frame->field_0x9C, frame->field_0x1E0);
        }
        updateSubViewFrame__Q23LOD17UnkClass_8046D144FPUci(
            sub, frame->field_0x9C, task);
    }
}

// ---------------------------------------------------------------------------
// handleViewUpdate: when `param` is nonzero and the current scene's gate byte
// is set, resolve the current view frame and hand it (+0x9C) to the +0xF0
// sub-manager's updateSubViewFrame along with `param`.
// ---------------------------------------------------------------------------
extern "C" void handleViewUpdate__Q23LOD9LODMemManFv(nw4r::g3d::ScnProc* proc, int param) {
    if (param == 0) {
        CScn* scn = Scn_GetCurrentScene();
        CScn* scn2 = (CScn*)getScnRootPtr(scn);
        if (((LODScnGate*)scn2)->field_0x19 != 0) {
            return;
        }
        CView* view = CView::getCurrentView();
        CScn* scn3 = Scn_GetCurrentScene();
        LODViewFrame* vf = Scn_HasCamItem(scn3, view);
        LOD::UnkClass_8046D144* sub = (LOD::UnkClass_8046D144*)((LODScnProcUD*)proc)->mUserData;
        updateSubViewFrame__Q23LOD17UnkClass_8046D144FPUci(sub, vf->field_0x9C, param);
    }
}

// ---------------------------------------------------------------------------
// initSceneGroup: create the scene group (16 children max) for this manager,
// attach it to the scene's index-7 group, optionally bind the aligned ResFile
// passed in, and clear the g3d object slots and child-manager slots.
// ---------------------------------------------------------------------------
void LOD::LODG3dMgr::initSceneGroup(u8* resBuf, CScn* scene) {
    // getScnRoot45C fetches the scene's allocator; its result feeds Construct.
    MEMAllocator* alloc = getScnRoot45C(scene);
    u32 size;
    nw4r::g3d::ScnGroup* grp = nw4r::g3d::ScnGroup::Construct(alloc, &size, 0x10);
    mGroup = grp;
    nw4r::g3d::G3dObj* group = getScnGroupChild(scene, 7);
    ((LODG3dMgrVt*)group)->vf0B(((LODMgrObj*)group)->field_0xE4, grp);
    if (resBuf != 0) {
        // The resource base must be 32-byte aligned.
        if ((u32)resBuf & 0x1F) {
            nw4r::db::Panic(lbl_eu_8052637C, 60, lbl_eu_80526354);
        }
        mResFile = nw4r::g3d::ResFile(resBuf);
        mResFile.Init();
        mResFile.Bind(mResFile);
    }
    // Clear the per-slot bookkeeping records (retail unrolls both walks into
    // straight-line stores).
    for (int i = 0; i < 16; i++) {
        mSlots[i].mObject = 0;
        mSlots[i].mId = -1;
        mSlots[i].field_0x08 = 0;
        mSlots[i].mFlags = 0;
    }
    for (int i = 0; i < 16; i++) {
        mChildren[i].mBlock = 0;
        mChildren[i].mId = -1;
        mChildren[i].mFlags = 0;
    }
}

extern "C" int bindModelToSlot__Q23LOD9LODMemManFv(
    LOD::UnkClass_8046D144* self, CScn* archive, nw4r::g3d::ResFile* resFile,
    u32 id, int mdlIdx, const f32* mtxSrc);

// ---------------------------------------------------------------------------
// bindResMdl: thunk into bindModelToSlot.  The retail body is a pure tail
// call that repacks the (mdlIdx, packed) pair into the callee's id: the
// callee's resFile is this+4, its id is `(packed >> 16) | mdlIdx` (the high
// 16 bits of packed moved into the low half, OR'd with the index), its
// mdlIdx is passed through unchanged, and its mtxSrc is the 5th arg.
// ---------------------------------------------------------------------------
int bindResMdl__Q23LOD9LODMemManFv(
    LOD::UnkClass_8046D144* self, CScn* archive, u32 mdlIdx, u32 packed,
    const f32* mtxSrc)
{
    return bindModelToSlot__Q23LOD9LODMemManFv(
        self, archive, &((LODResView*)self)->mResFile,
        ((u16)packed << 16) | mdlIdx, mdlIdx, mtxSrc);
}

// ---------------------------------------------------------------------------
// acquireResFileSlot: resolve a ResFile slot for `id`.  A slot whose id already
// matches is marked in-use and its index returned.  Otherwise the id's pool
// block is fetched via getOrCreatePoolData and a free slot (id < 0) is claimed:
// the slot stores the block pointer, sets its id/flags, computes the aligned
// ResFile data end (block + block[+0x14]) and runs ResFile::Init on it.
// ---------------------------------------------------------------------------
extern "C" s32 acquireResFileSlot__Q23LOD9LODMemManFv(
    LOD::UnkClass_8046D144* self, LOD::UnkClass_8046D144* pool, s32 id, s32* out)
{
    s32 result = -1;
    LODResFileSlotArray& arr = *(LODResFileSlotArray*)self;

    for (int i = 0; i < 16; i++) {
        if (arr.mSlots[i].field_0x10 == id) {
            result = i;
            ((LODResFileSlotArray*)self)->mSlots[i].field_0x12 |= 1;
            break;
        }
    }

    if (result < 0) {
        u8* block = ((LOD::LODMemMan*)pool)->getOrCreatePoolData(id);
        if (block != 0) {
            for (int j = 0; j < 16; j++) {
                if (arr.mSlots[j].field_0x10 >= 0) {
                    continue;
                }
                result = j;
                arr.mSlots[j].field_0x10 = (s16)id;
                arr.mSlots[j].field_0xC = (u32)block;
                arr.mSlots[j].field_0x12 |= 1;
                u32 end = (u32)block + ((LODPoolBlock*)block)->field_0x14;
                if (end & 0x1F) {
                    nw4r::db::Panic(lbl_eu_8052637C, 0x3c, lbl_eu_80526354);
                }
                arr.mSlots[j].field_0x8 = end;
                ((nw4r::g3d::ResFile*)&arr.mSlots[j].field_0x8)->Init();
                break;
            }
        } else {
            return 0;
        }
    }

    *out = result;
    return 1;
}

// ---------------------------------------------------------------------------
// bindModelToSlot: bind a ResMdl to one of the 16 g3d slots at +0xC8.  A slot
// already holding the requested id just gets its busy bit set and returns 1;
// otherwise the first free slot constructs a ScnMdl from the ResFile model
// (through the scene allocator), notifies the g3d manager via its vtable+0x34
// callback, installs the placement matrix (column 3 from the first three
// floats, the 3x3 from the rest) and records the id.
// ---------------------------------------------------------------------------
int bindModelToSlot__Q23LOD9LODMemManFv(
    LOD::UnkClass_8046D144* self, CScn* archive, nw4r::g3d::ResFile* resFile,
    u32 id, int mdlIdx, const f32* mtxSrc)
{
    LODBindView* v = (LODBindView*)self;
    LODMemManLayout* l = (LODMemManLayout*)self;
    for (int i = 0; i < 16; i++) {
        if (v->mSlots[i].field_0x0 != 0 && v->mSlots[i].field_0x4 == id) {
            v->mSlots[i].mFlags |= 1;
            return 1;
        }
    }
    for (int i = 0; i < 16; i++) {
        if (v->mSlots[i].field_0x0 == 0) {
            nw4r::g3d::ResMdl resMdl = resFile->GetResMdl(mdlIdx);
            u32 p;
            nw4r::g3d::ScnMdl* scnMdl = (v->mSlots[i].field_0x0 =
                nw4r::g3d::ScnMdl::Construct(
                    (MEMAllocator*)getScnRoot44C(archive), &p, resMdl, 0, 2));
            ((LODG3dMgrVt*)l->field_0x0)->vf0B(
                ((LODMgrObj*)l->field_0x0)->field_0xE4, scnMdl);
            nw4r::math::MTX34 mtx;
            mtx._03 = mtxSrc[0];
            mtx._13 = mtxSrc[1];
            mtx._23 = mtxSrc[2];
            mtx._00 = mtxSrc[3];
            mtx._01 = mtxSrc[4];
            mtx._02 = mtxSrc[5];
            mtx._10 = mtxSrc[6];
            mtx._11 = mtxSrc[7];
            mtx._12 = mtxSrc[8];
            mtx._20 = mtxSrc[9];
            mtx._21 = mtxSrc[10];
            mtx._22 = mtxSrc[11];
            v->mSlots[i].field_0x0->SetMtx((nw4r::g3d::ScnObj::ScnObjMtxType)0, mtx);
            v->mSlots[i].field_0x4 = id;
            v->mSlots[i].mFlags |= 1;
            return 1;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// teardownSubEntries: walk the 16 g3d slots at +0xC8, destroying objects flagged
// done (bit 2), notifying the manager (vtable+0x3C) for pending ones, and
// clearing a per-slot busy bit.  Then walk the 16 child entries at +0xC and
// reset any active child through delegateMergeToSub.  `self` is the g3d-slot
// region base (object + 0xCC); `sub` is the +0xA44 sub-manager.
// ---------------------------------------------------------------------------
// TU-internal member callee, called through its exact retail symbol so MWCC
// does not inline its tail-call to mergeElementList (retail keeps a real bl).
extern "C" void delegateMergeToSub__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u32 target);
extern "C" void mergeElementList__Q23LOD9LODMemManFv(LODElem20* list, u32 target);

void LOD::LODG3dMgr::teardownSubEntries(LOD::LODMemMan* sub) {
    // Scoped zero constants: keeping them as explicit locals pins the zero
    // register materialization to the points retail chooses (hoisted before
    // the g3d walk, re-materialized at the child-walk head).
    u32 zero = 0;
    int i = 0;
    do {
        if (mSlots[i].mObject != 0) {
            u16 flags = mSlots[i].mFlags;
            if (flags & 4) {
                mSlots[i].mObject->Destroy();
                mSlots[i].mObject = (nw4r::g3d::G3dObj*)zero;
                mSlots[i].mFlags = zero;
            } else {
                if (!(flags & 1)) {
                    mSlots[i].mFlags = flags | 4;
                    ((LODG3dMgrVt*)mGroup)->vf0D(mSlots[i].mObject);
                } else {
                    mSlots[i].mFlags = flags & 0xFFFD;
                }
                // Both paths clear bit 0 via a reload of the stored halfword.
                mSlots[i].mFlags &= ~1;
            }
        }
        i++;
    } while (i < 16);

    u32 zero2 = 0;
    i = 0;
    do {
        if (mChildren[i].mBlock != 0) {
            if (!(mChildren[i].mFlags & 1)) {
                delegateMergeToSub__Q23LOD9LODMemManFv(sub, (u32)mChildren[i].mBlock);
                mChildren[i].mBlock = (u8*)zero2;
                mChildren[i].mId = -1;
            }
            mChildren[i].mFlags = zero2;
        }
        i++;
    } while (i < 16);
}

// Walk view of the slot array: base pointer starts at the object base and each
// step advances 0xC (one slot) while the access reads through the fixed +0xC8
// window (the shape MWCC's folded-displacement pointer walk needs).
struct LODSlotStep {
    u8         mPad[0xC8];
    LODG3dSlot mSlot;
};

// ---------------------------------------------------------------------------
// destroyAllG3dObjects: notify the g3d manager about every live object slot,
// destroy the slots and then the manager's own group object.
// ---------------------------------------------------------------------------
void LOD::LODG3dMgr::destroyAllG3dObjects() {
    int i = 0;
    while (i < 16) {
        if (mSlots[i].mObject) {
            ((LODG3dMgrVt*)mGroup)->vf0D(mSlots[i].mObject);
            mSlots[i].mObject->Destroy();
            mSlots[i].mObject = 0;
        }
        i++;
    }
    if (mGroup) {
        mGroup->Destroy();
        mGroup = 0;
    }
}

// ---------------------------------------------------------------------------
// notifyG3dManager: notify the g3d manager (vtable+0x3C) about every live
// object slot, then - when a scene is supplied - remove the manager's group
// from the scene's index-7 group.
// ---------------------------------------------------------------------------
void LOD::LODG3dMgr::notifyG3dManager(CScn* scene) {
    if (mGroup != 0) {
        // Walk the slots through a fixed +0xC8 window on a stepping base (the
        // shape MWCC folds the displacement into).
        LODSlotStep* p = (LODSlotStep*)this;
        int i = 0;
        do {
            if (p->mSlot.mObject != 0) {
                ((LODG3dMgrVt*)mGroup)->vf0D(p->mSlot.mObject);
            }
            i++;
            p = (LODSlotStep*)((u8*)p + sizeof(LODG3dSlot));
        } while (i < 16);
    }
    if (scene != 0) {
        if (mGroup != 0) {
            ((nw4r::g3d::ScnGroup*)getScnGroupChild(scene, 7))
                ->Remove((nw4r::g3d::ScnObj*)mGroup);
        }
    }
}

// ---------------------------------------------------------------------------
// resetChildMasks: clear the slot bookkeeping of the three child light
// records (slot count plus the bound/initialised masks).
// ---------------------------------------------------------------------------
void LOD::LODG3dMgr::resetChildMasks() {
    for (int i = 0; i < 3; i++) {
        mChildLights[i].mSlotCount = 0;
        mChildLights[i].mUsedMask = 0;
        mChildLights[i].mDoneMask = 0;
    }
}

// ---------------------------------------------------------------------------
// resetLightColor: initialise the shared light colour to zero (retail keeps a
// stack zero word passed by address).
// ---------------------------------------------------------------------------
void LOD::LODG3dMgr::resetLightColor() {
    GXColor zero = {0, 0, 0, 0};
    GXInitLightColor(&mLight, zero);
}

// ---------------------------------------------------------------------------
// Per-child record.  The LODMemMan is treated as an array of 0x28c-strided
// records (registerChildSlot indexes `self + index*0x28c`).  Each record holds 8
// 0x40-byte slots (+0x188) feeding LightCtlInitGXLight, 8 float-word groups
// (+0x388, 16 bytes per slot), and the slot-index / used / done words.
// ---------------------------------------------------------------------------
struct LODChildRec {
    u8  mPad_00[0x188];        // 0x00..0x187
    u8  mSlotData[8][0x40];    // 0x188..0x387: per-slot 0x40-byte records
    u32 mSlotFloats[8][4];     // 0x388..0x407: per-slot float-bit words
    s32 field_0x408;           // 0x408 slot index (capped at 8)
    u32 field_0x40C;           // 0x40C used-slot mask
    u32 field_0x410;           // 0x410 done-slot mask
};

// View of the same record anchored at +0x188 (retail holds this base in r31
// and addresses the slot data, float words and masks relative to it).
struct LODChild188 {
    u8  mSlotData[8][0x40];    // 0x00..0x1FF: per-slot 0x40-byte records
    u32 mSlotFloats[8][4];     // 0x200..0x27F: per-slot float-bit words
    s32 field_0x280;           // 0x280 slot index (child + 0x408)
    u32 field_0x284;           // 0x284 used-slot mask
    u32 field_0x288;           // 0x288 done-slot mask
};

// Descriptor passed in r4: four direction floats, the kind word (3/4 selects
// the global flag path) and the scale factor.
// ---------------------------------------------------------------------------
// registerChildSlot: register a new slot on the 0x28c-strided child record.  A
// 3/4-kind descriptor flags the shared .bss array instead of the record's
// used-mask.  The slot's four direction words are stored as raw float bits
// (the scale multiplies all but the last), the slot record is handed to
// LightCtlInitGXLight, and the done-mask bit is set before the slot index advances.
// (retail keeps the kind test as two compares; the goto chain reproduces it.)
// ---------------------------------------------------------------------------
void registerChildSlot__Q23LOD9LODMemManFv(LOD::UnkClass_8046D144* self, LODChildDesc* desc, u8* unused, int index) {
    // Declared high-to-low so MWCC's reverse-order stack allocation puts
    // the words at 0x8..0x14 like retail.
    f32 wf3;
    f32 wf2;
    f32 wf1;
    f32 wf0;
    u32* dst;
    f32 s;
    LODChildRec* child = (LODChildRec*)((u8*)self + index * 0x28c);
    if (child->field_0x408 >= 8) return;
    LODChild188* p = (LODChild188*)child->mSlotData;
    if (desc->field_0x34 == 3) goto kind34;
    if (desc->field_0x34 != 4) goto otherKind;
kind34:
    lbl_eu_80658368[index] = 1;
    goto floatsDone;
otherKind:
    p->field_0x284 |= 1 << p->field_0x280;
floatsDone:
    dst = p->mSlotFloats[p->field_0x280];
    s = desc->field_0x38;
    wf0 = desc->field_0x10 * s;
    wf1 = desc->field_0x14 * s;
    wf2 = desc->field_0x18 * s;
    wf3 = desc->field_0x1C;
    dst[0] = *(u32*)&wf0;
    dst[1] = *(u32*)&wf1;
    dst[2] = *(u32*)&wf2;
    dst[3] = *(u32*)&wf3;
    LightCtlInitGXLight((u8*)desc, p->mSlotData[p->field_0x280], unused);
    p->field_0x288 |= 1 << p->field_0x280;
    p->field_0x280++;
}

// ---------------------------------------------------------------------------
// setLightScale: store the halfword param into the base light scale via the
// GQR5 s16 fast cast (retail psq_l f0, 0(r4), 1, qr5 after an sth spill).
#pragma push
#include <revolution/os/OSFastCast.h>
#pragma pop
void LOD::LODG3dMgr::setLightScale(s16 value) {
    mLightScale = __OSs16tof32(&value);
}

// ---------------------------------------------------------------------------
// initLightScales: initialise the high offset scale pair (+0x96C/+0x970) and
// the +0x974 index to -1 (retail store order: 0x970, 0x96C, 0x974).
// ---------------------------------------------------------------------------
extern const f32 lbl_eu_8066A718;   // 1.0f pool slot (retail reads it once)
void LOD::LODG3dMgr::initLightScales() {
    f32 one = lbl_eu_8066A718;
    mStoredLightScale = one;
    mLightScale = one;
    mLastChildIdx = -1;
}

// ---------------------------------------------------------------------------
// reloadChildGxLights: (re)load the child record's GX light objects.  On a new
// child id the un-used slots are loaded first; then every used slot's colour
// is recomputed from its direction floats (base scale * clamp limit, capped)
// and the light object is re-initialised and loaded.  The manager keeps the
// last-bound child id and the stored scale, so an unchanged repeat load is a
// no-op.
// ---------------------------------------------------------------------------
void LOD::LODG3dMgr::reloadChildGxLights(int childIdx) {
    if (childIdx == -1) return;
    LODChildLight* p = &mChildLights[childIdx];
    if (p->mSlotCount == 0) return;
    if (childIdx == mLastChildIdx) {
        if (mStoredLightScale == mLightScale) return;
    } else {
        int i;
        for (i = 0; i < p->mSlotCount; i++) {
            if (!(p->mUsedMask & (1 << i))) {
                GXLoadLightObjImm(&p->mLightObjs[i], (GXLightID)(1 << i));
            }
        }
    }
    // Retail order: load the base scale, store the child id, load the clamp
    // limit, store the scale, i = 0, load the base colour, then the hoisted
    // shift-one.
    f32 scale = mLightScale;
    mLastChildIdx = childIdx;
    f32 limit = lbl_eu_8066A720;
    mStoredLightScale = scale;
    int i = 0;
    GXColor baseColor = lbl_eu_8066A71C;
    for (; i < p->mSlotCount; i++) {
        if (p->mUsedMask & (1 << i)) {
            f32 c0 = mLightScale * (limit * p->mDirScales[i][0]);
            f32 c1 = mLightScale * (limit * p->mDirScales[i][1]);
            f32 c2 = mLightScale * (limit * p->mDirScales[i][2]);
            if (c0 > limit) c0 = limit;
            if (c1 > limit) c1 = limit;
            if (c2 > limit) c2 = limit;
            GXColor color = baseColor;
            color.r = (u8)c0;
            color.g = (u8)c1;
            color.b = (u8)c2;
            GXInitLightColor(&p->mLightObjs[i], color);
            GXLoadLightObjImm(&p->mLightObjs[i], (GXLightID)(1 << i));
        }
    }
}

// ---------------------------------------------------------------------------
// allocatePoolBlock: walk the 0x20-strided block list and carve a free block of
// `size` (+0x20 header) out of the first block whose flag bit 0 is set and
// whose capacity fits.  The block is marked, its tail split off as a new
// block when at least 10240 bytes remain, and a pointer past the header is
// returned.
// ---------------------------------------------------------------------------
extern "C" u8* allocatePoolBlock__Q23LOD9LODMemManFv(LODPoolBlock* self, u32 size, int id) {
    LODPoolBlock* cur = self;
    s32 end = (s32)size + 32;
    while (cur) {
        u32 fl = cur->mFlags;
        if (!(fl & 1)) {
            if ((s32)cur->mSize >= end) {
                cur->mFlags = fl | 1;
                cur->field_0xC = 0;
                cur->mId = (u32)id;
                s32 rem = (s32)cur->mSize - end;
                if (rem >= 10240) {
                    cur->mSize = (u32)end;
                    LODPoolBlock* nb = (LODPoolBlock*)((u8*)cur + end);
                    nb->mFlags = 0;
                    nb->mSize = (u32)rem;
                    nb->mNext = cur->mNext;
                    cur->mNext = nb;
                }
                return (u8*)&cur[1];
            }
        }
        cur = cur->mNext;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// mergeElementList: walk the element list (head at +0x8) and merge the element
// whose +0x20 pointer equals `target` into the previous element: the flag
// word is cleared when there is no previous element, otherwise (previous
// element's flag bit 0 set) the previous element absorbs the target's +0x4
// value and skips it via +0x8.
// ---------------------------------------------------------------------------
extern "C" void mergeElementList__Q23LOD9LODMemManFv(LODElem20* list, u32 target) {
    u32* cur = (u32*)list;
    u32* prev = 0;
    while (cur) {
        if ((u32)&cur[8] == target) {
            cur[0] = 0;
            if (prev == 0) {
                return;
            }
            if ((prev[0] & 1) != 0) {
                return;
            }
            // Read the merge pair in retail load order (cur[1] before prev[1]):
            // the volatile pins the source order (MWCC_CASES sfmps_pesfn
            // load-pair rule - one-time state reads, no aliasing writes in
            // scope), and declaring vp first makes the allocator land prev's
            // word in r0 so the sum reuses it (add r0,r4,r0 like retail).
            u32 vp;
            u32 vc = *(volatile u32*)&cur[1];
            vp = *(volatile u32*)&prev[1];
            prev[1] = vc + vp;
            prev[2] = cur[2];            return;
        }
        prev = cur;
        cur = (u32*)cur[2];
    }
}

// ---------------------------------------------------------------------------
// findPoolBlockById: walk the 0x20-strided block list and return the first block
// whose flag bit 0 is set and whose id matches, or null.
// ---------------------------------------------------------------------------
extern "C" LODPoolBlock* findPoolBlockById__Q23LOD9LODMemManFv(LODPoolBlock* self, int id) {
    LODPoolBlock* cur = self;
    while (cur) {
        if ((cur->mFlags & 1) && id == (int)cur->mId) {
            return cur;
        }
        cur = cur->mNext;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// markPoolBlockReady: walk the 0x20-strided block list; the block whose data area
// (+0x20) matches `target` and whose flag bit 0 is set gets bit 1 set.
// ---------------------------------------------------------------------------
extern "C" void markPoolBlockReady__Q23LOD9LODMemManFv(LODPoolBlock* list, u32 target) {
    LODPoolBlock* cur = list;
    while (cur) {
        if ((cur->mFlags & 1) && (u32)&cur[1] == target) {
            cur->mFlags |= 2;
            return;
        }
        cur = cur->mNext;
    }
}

// ---------------------------------------------------------------------------
// initElementList: initialise the element-list head/tail links and buffer slot.
// Auto-inline off so callers emit a real tail call (retail `b`).
// ---------------------------------------------------------------------------
extern "C" void initElementList__Q23LOD9LODMemManFv(LODElemList* head, u32 arg) {
    head->mHead = 0;
    head->mTail = 0;
    head->mField_4 = arg;
}

// ---------------------------------------------------------------------------
// allocPersistentBuffer: release the shared persistent buffer (lbl_eu_80665830) if
// present (retail keeps a redundant inner null-check doubling the beq), then
// allocate a fresh 32-aligned block from `handle` into it.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::allocPersistentBuffer(u32 handle, u32 size) {
    int old = lbl_eu_80665830;
    if (old != 0) {
        if (old != 0) {
            mtl::MemManager::deallocate((void*)old);
            lbl_eu_80665830 = 0;
        }
    }
    lbl_eu_80665830 = (int)mtl::MemManager::allocate_head(handle, size, 0x20);
}

// ---------------------------------------------------------------------------
// freePersistentBuffer: release the shared persistent buffer (lbl_eu_80665830) and
// clear it (retail keeps a redundant inner null-check doubling the beq).
// ---------------------------------------------------------------------------
void LOD::LODMemMan::freePersistentBuffer() {
    int old = lbl_eu_80665830;
    if (old != 0) {
        if (old != 0) {
            mtl::MemManager::deallocate((void*)old);
        }
    }
    lbl_eu_80665830 = 0;
}

// ---------------------------------------------------------------------------
// initElementBuffers: (re)initialise the element buffer.  The secondary buffer
// (+0x8) is released first, then - when `size` is nonzero - the element
// buffer (+0x4) is either adopted from the shared persistent buffer
// (lbl_eu_80665830) or freshly allocated from `handle`; a zero size only
// releases the element buffer.  The tail re-seeds the name string and flags
// exactly like resetManagerState.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::initElementBuffers(u32 handle, u32 size) {
    if (mSecondaryData) {
        if (mSecondaryData) {
            mtl::MemManager::deallocate(mSecondaryData);
            mSecondaryData = 0;
        }
    }
    if (size != 0) {
        if (lbl_eu_80665830 != 0) {
            mData = (u8*)lbl_eu_80665830;
        } else {
            if (mData) {
                if (mData) {
                    mtl::MemManager::deallocate(mData);
                    mData = 0;
                }
            }
            mData = (u8*)mtl::MemManager::allocate_head(handle, size, 0x20);
        }
    } else {
        if (mData) {
            if (mData) {
                mtl::MemManager::deallocate(mData);
                mData = 0;
            }
        }
    }
    // Two-step local alias forces MWCC to materialise the full string
    // address into r31 up front (lis@ha + addi@l adjacent) and reuse it via
    // mr for both calls (same recipe as clearFlagAndResetName).
    const char* src = (const char*)&lbl_eu_80523D90;
    const char* name = src;
    mElementList = 0;
    mFlags = 1;
    mView_1C = (u8*)size;
    mFile = 0;
    mNameLen = strlen(name);
    strcpy(mName, name);
}

// ---------------------------------------------------------------------------
// resetManagerState: reset the manager.  The element buffers are released back to
// the MemManager only while the global reset flag is clear (retail keeps a
// redundant inner null-check that MWCC emits as a doubled beq), the file
// handle is cancelled, and the reset-time name string is re-seeded into +0x34
// with its length recorded at +0x74.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::resetManagerState() {
    if (lbl_eu_80665830 == 0) {
        if (mData) {
            if (mData) {
                mtl::MemManager::deallocate(mData);
                mData = 0;
            }
        }
    } else {
        mData = 0;
    }
    if (mSecondaryData) {
        if (mSecondaryData) {
            mtl::MemManager::deallocate(mSecondaryData);
            mSecondaryData = 0;
        }
    }
    if (mFile) {
        CDeviceFile::cancel(mFile);
        mFile = 0;
    }
    mElementList = 0;
    mFlags = 0;
    const char* name = static_cast<const char*>(
        static_cast<const void*>(lbl_eu_80523D90));
    // Reset-name re-seed.  NOTE (open item): retail coalesces &lbl_eu_80523D90
    // into r31 (lis@ha + addi@l adjacent, reused via mr); the Wii backend
    // always splits it (ha-base in r31 + per-use @l folds), one instruction
    // short - the documented adx_fsvr lis/addi coalescing ceiling.
    mNameLen = strlen(name);
    strcpy(mName, name);
}

// ---------------------------------------------------------------------------
// cancelFileAndRelease: cancel the file handle at +0x30, clear flag bits of +0x20
// (mask 0xFFFCFF8D), then release the +0x8 buffer (retail keeps a redundant
// inner null-check doubling the beq).
// ---------------------------------------------------------------------------
void LOD::LODMemMan::cancelFileAndRelease() {
    if (mFile) {
        CDeviceFile::cancel(mFile);
        mFile = 0;
    }
    mFlags &= 0xFFFCFF8D;
    if (mSecondaryData) {
        if (mSecondaryData) {
            mtl::MemManager::deallocate(mSecondaryData);
            mSecondaryData = 0;
        }
    }
}

// ---------------------------------------------------------------------------
// clearFlagAndResetName: clear flag bit 2 of the +0x20 word, then re-seed the reset-
// time name string: store strlen(lbl_eu_80523D90) into +0x74 and copy the
// string into +0x34 (retail hoists the string address into r31 up front).
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// clearFlagAndResetName: clear flag bit 2 of the +0x20 word, then re-seed the reset-
// time name string: store strlen(lbl_eu_80523D90) into +0x74 and copy the
// string into +0x34 (retail hoists the string address into r31 up front).
// ---------------------------------------------------------------------------
void LOD::LODMemMan::clearFlagAndResetName() {
    const char* src = (const char*)&lbl_eu_80523D90;
    const char* name = src;
    mFlags &= ~4u;
    mNameLen = strlen(name);
    strcpy(mName, name);
}

// ---------------------------------------------------------------------------
// allocSecondaryBuffers: (re)allocate the object's secondary buffer to hold `count20`
// 0x20-byte elements, `count48` 0x48-byte descriptors and a count4*4-byte
// table (0x20-aligned), followed by `count18` 0x18-byte records.  The old
// buffer is released first; the output pointers receive the base and the
// sub-region offsets.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::allocSecondaryBuffers(u32 count20, u32 count18, u32 count48,
                                           u32 count4, u8** outBase, u8** out18,
                                           u8** out48, u8** out4) {
    u32 size20 = count20 * 0x20;
    u32 size48 = count48 * 0x48;
    u8* old = mSecondaryData;
    u32 base;
    u32 offset18;
    u32 ofs;
    base = size20 + size48 + count4 * 4;
    base += 0x20 - (base & 0x1F);
    offset18 = base;
    base += count18 * 0x18;
    base += 0x20 - (base & 0x1F);
    if (old != 0) {
        if (old != 0) {
            mtl::MemManager::deallocate(old);
            mSecondaryData = 0;
        }
    }
    mSecondaryData = (u8*)mtl::MemManager::allocate_head(
        mtl::MemManager::getHandleMEM1(), base, 0x20);
    memset(mSecondaryData, 0, base);
    *outBase = mSecondaryData;
    ofs = size20;
    if (count48 != 0) {
        *out48 = mSecondaryData + size20;
        ofs += size48;
    }
    if (count4 != 0) *out4 = mSecondaryData + ofs;
    *out18 = mSecondaryData + offset18;
}

// ---------------------------------------------------------------------------
// adjustBufferOffsets: when the +0x4 buffer is present, adjust the +0xC and +0x18
// offsets by `delta` and tail-call initElementList with the new base.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// adjustBufferOffsets: when the +0x4 buffer is present, adjust the +0xC and +0x18
// offsets by `delta` (retail: r4 = self->field_0x1C reload, r6 = delta; r5 is
// an unused caller-leftover) and tail-call initElementList with the new base
// and the remaining size.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::adjustBufferOffsets(int unk, int unused, int delta) {
    if (mData != 0) {
        int rem = (int)mView_1C - delta;
        mElementList = mData + delta;
        mCount_18 = rem;
        initElementList__Q23LOD9LODMemManFv((LODElemList*)(mData + delta), (u32)rem);
    }
}

// ---------------------------------------------------------------------------
// getOrCreatePoolData: find the pool block tagged with `id`.  A ready block
// (flag bit 1) returns its data area; an allocated-but-unready block returns
// null.  When no block exists yet, a new one is carved from the pool with the
// size recorded in the field_0x4 size table and the bookkeeping stored in
// field_0x24..0x2C.
// ---------------------------------------------------------------------------
u8* LOD::LODMemMan::getOrCreatePoolData(int id) {
    LODPoolBlock* block = findPoolBlockById__Q23LOD9LODMemManFv((LODPoolBlock*)mElementList, id);
    if (block != 0) {
        // Ready bit set: hand back the data area behind the block header.
        if (!(block->mFlags & 2)) return 0;
        return (u8*)&block[1];
    }
    if (!(mFlags & 0x70)) {
        int id2 = id * 2;
        LODBuf74* buf = (LODBuf74*)mData;
        u32* arr = buf->mPairs;
        // Retail re-reads arr[id2+1] for the mBlockSize store rather than
        // reusing the allocator size argument.
        u8* data = allocatePoolBlock__Q23LOD9LODMemManFv((LODPoolBlock*)mElementList, arr[id2 + 1], id);
        if (data != 0) {
            mBlock = data;
            mBlockOffset = arr[id2];
            mBlockSize = arr[id2 + 1];
            mFlags |= 0x40;
        }
    }
    return 0;
}

// retail: lwz r3,0xC(r3); b mergeElementList -- delegates to the element-list
// header stored at +0xC of the LODMemMan.
void delegateMergeToSub__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u32 target) {
    mergeElementList__Q23LOD9LODMemManFv(
        (LODElem20*)((LODMemManLayout*)self)->field_0xC, target);
}

void LOD::LODMemMan::setArchiveReadyFlag() {
    mFlags |= 0x40000000;
}

// CDeviceFile archive header read through LODMemMan.field_0x4 (driveArchiveLoadState).
struct LODArchiveHdr {
    u32 mMagic;      // 0x00 "LODI"
    u32 mVersion;    // 0x04
    u8  mPad_08[0x14 - 0x08];
    u32 mSize;       // 0x14
    u8  mPad_18[0xC00 - 0x18];
    u8  mPayload[1]; // +0xC00 archive payload
};

// ---------------------------------------------------------------------------
// driveArchiveLoadState: drive the archive load state machine via the field_0x20 flag
// bits.  Each of the three phases (data/file/entry) is gated by a bit pair;
// when the archive-file bit is clear the object issues a read job through
// CDeviceFile and marks the bit, and when both bits are set it advances the
// phase mask instead.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::driveArchiveLoadState() {
    LOD::LODMemMan* self = this;
    u32 flags = mFlags;
    if (flags & 0x80000000u) return;
    if (flags & 0x40000000u) return;
    if (!(flags & 0x70)) return;
    if (flags & 0x10) {
        if (flags & 0x10000) {
            if (flags & 0x20000) {
                mFlags = (flags & ~0x30010u) | 0x20;
            }
        } else {
            mFlags = flags | 0x10000;
            mFile = CDeviceFile::readCommonArchiveFile(
                (mtl::ALLOC_HANDLE)mData, mName, (IWorkEvent*)self, 0, 0xC00);
        }
    }
    flags = mFlags;
    if (flags & 0x20) {
        if (flags & 0x10000) {
            if (flags & 0x20000) {
                mFlags = (flags & ~0x30020u) | 0x6;
            }
        } else {
            mFlags = flags | 0x10000;
            LODArchiveHdr* hdr = (LODArchiveHdr*)mData;
            if (hdr->mMagic == 0x4C4F4449u && hdr->mVersion >= 0x3ED) {
                mFile = CDeviceFile::readCommonArchiveFile(
                    (mtl::ALLOC_HANDLE)hdr->mPayload, mName, (IWorkEvent*)self,
                    0xC00, hdr->mSize - 0xC00);
            } else {
                if (mFile) {
                    CDeviceFile::cancel(mFile);
                    mFile = 0;
                }
                mFlags &= ~0x30072u;
                if (mSecondaryData) {
                    if (mSecondaryData) {
                        mtl::MemManager::deallocate(mSecondaryData);
                        mSecondaryData = 0;
                    }
                }
            }
        }
    }
    flags = mFlags;
    if (flags & 0x40) {
        if (flags & 0x10000) {
            if (flags & 0x20000) {
                mFlags = flags & ~0x30040u;
                markPoolBlockReady__Q23LOD9LODMemManFv((LODPoolBlock*)mElementList, (u32)mBlock);
            }
        } else {
            mFlags = flags | 0x10000;
            mFile = CDeviceFile::readCommonArchiveFile(
                (u32)mBlock, mName, (IWorkEvent*)self, mBlockOffset, mBlockSize);
        }
    }
}

bool LOD::LODMemMan::OnFileEvent(CEventFile* evt) {
    // File-event hook: when the event carries the currently adopted file
    // handle, clear the handle and set the in-use flag on the existing word.
    CFileHandle* current = mFile;
    if (current == evt->mFileHandle) {
        u32 flags = mFlags;
        mFile = 0;
        mFlags = flags | 0x20000;
    }
    return true;
}

extern "C" __declspec(section ".rodata") const char lbl_eu_80523D90[8] = {0};

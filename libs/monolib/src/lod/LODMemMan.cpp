// Auto-scaffolded catalog TU for monolib/src/lod/LODMemMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/lod/LODMemMan.hpp"
#include "monolib/core/CView.hpp"           // CView::getCurrentView
#include "monolib/core/code_804E36DC.hpp"   // func_80496288 (frame delta, C ABI)
#include "nw4r/g3d/g3d_obj.h"               // nw4r::g3d::G3dObj::Destroy
#include <string.h>                         // strlen / strcpy                         // strlen / strcpy
#include "monolib/util/MemManager.hpp"      // mtl::MemManager::deallocate
#include "monolib/device/CDeviceFile.hpp"   // CDeviceFile::cancel
#include <nw4r/g3d/g3d_scnobj.h>            // nw4r::g3d::ScnGroup (Remove at vtable+0x3C)
#include "monolib/work/CEventFile.hpp"   // CEventFile (global) — file-event record
#include <nw4r/g3d/res/g3d_resfile.h>       // nw4r::g3d::ResFile::Init
#include <nw4r/g3d/g3d_scnmdl.h>           // nw4r::g3d::ScnMdl::Construct
#include <nw4r/math/math_types.h>           // nw4r::math::MTX34
#include <revolution/GX.h>                  // GXInitLightColor / GXLoadLightObjImm
#include <nw4r/db/db_assert.h>              // nw4r::db::Panic

class CScn;  // scene wrapper (forward decl; CScnMem.hpp is not included here)

// Retail data labels used by this TU.  Global-scope data imports keep their
// plain unmangled names, so plain extern declarations suffice (no extern "C").
extern u32 lbl_eu_80663828;          // sentinel "no next" index for the element list
// .bss global flag array: index 0..2 holds "3/4-kind descriptor seen" flags
// (func_8047133C writes 1 at [index]).
extern int lbl_eu_80658368[];
extern int lbl_eu_80658374[];
extern int lbl_eu_80665830;          // global reset flag (0 = release buffers)
extern const char lbl_eu_80523D90[];  // reset-time name string (copied into +0x34)
extern const char lbl_eu_80663834[4]; // ctor name string (.sdata, sda21-addressed)
extern u32 lbl_eu_8056DB08[];         // vtable for the +0xa44 sub-manager object
extern const char lbl_eu_8052637C[];  // func_80470DCC Panic file path
// Float-pool slot for the +0x96C/+0x970 scale pair init (func_8047146C).
extern "C" const f32 lbl_eu_8066A718;
// Panic message for the ResFile alignment check (func_80470DCC).
extern const char lbl_eu_80526354[];

// Light-scale bookkeeping at LODMemMan+0x96C (func_80471484).
struct LODLightMgr {
    u8  mPad_00[0x96C];
    f32 field_0x96C;   // 0x96C base light scale
    f32 field_0x970;   // 0x970 stored light scale
    s32 field_0x974;   // 0x974 last-bound child id
};

extern f32 lbl_eu_8066A720;      // light scale clamp constant
// Light colour base: the low byte becomes the alpha; the RGB bytes are
// overwritten per slot.
extern GXColor lbl_eu_8066A71C;

// func_80470634 per-type update constants (.sdata2 pool).
extern f64 lbl_eu_8066A6E0;   // 2^52 double magic (u8 -> double conversion)
extern f32 lbl_eu_8066A6D8;   // scale divisor
extern f32 lbl_eu_8066A6C0;   // 0.0f LOD distance constant
// Shared LOD distance globals published by the per-element update
// (func_804702F0 writes the float value and its integer truncation).
extern f32 lbl_eu_80665754;
extern s16 lbl_eu_80665758;
// One-time-initialised global scale: the byte flag gates the division
// c11 / lbl_eu_80663830, whose result is cached in lbl_eu_80665828.
extern f32 lbl_eu_80665828;
extern s8  lbl_eu_8066582C;
extern f32 lbl_eu_80663830;
extern f32 lbl_eu_8066A70C;
extern f32 lbl_eu_8066A710;
extern f32 lbl_eu_8066A714;

// GX-typed view of the child-188 region (func_80471484).
struct LODLight188 {
    GXLightObj mSlotData[8];      // 0x00..0x1FF
    f32        mSlotFloats[8][4]; // 0x200..0x27F
    s32        field_0x280;       // 0x280 slot count
    u32        field_0x284;       // 0x284 used-slot mask
    u32        field_0x288;       // 0x288 done-slot mask
};

// ---------------------------------------------------------------------------
// Recovered LODMemMan field layout.  The shared header only exposes opaque
// padding up to 0x7c plus the 0xabc embedded sub-object, so the fields used
// by the targets below are described here and the object is viewed through
// this struct instead of raw pointer arithmetic.
// ---------------------------------------------------------------------------

// 0x20-byte LOD element.  The flag word at 0x0 is the embedded
// UnkClass_8046A530 view (func_8046CFB4 is called with the element pointer).
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
    u8  mPad_18[0x1A - 0x18];      // 0x18..0x19
    s16 field_0x1A;                // 0x1a own element index (list link)
    u8  field_0x1C;                // 0x1c
    u8  field_0x1D;                // 0x1d
    u8  field_0x1E;                // 0x1e
    u8  field_0x1F;                // 0x1f
};

// 0x18-strided LOD type descriptor (array base at LODMemMan+0x94).  The u16
// at +0x04 gates the element-chain link bits; the f32 at +0x10 is the
// distance threshold used by func_8046FF84.
struct LODTypeDesc18 {
    u8  mPad_00[0x04];
    u16 mFlags;          // 0x04
    u8  mPad_06[0x0A];   // 0x06..0x0F
    f32 mThreshold;      // 0x10
    u8  mPad_14[0x18 - 0x14];
};

// 3-float vector handed to the element update (delta / squared-length input).
struct LODVec3f {
    f32 x;   // 0x00
    f32 y;   // 0x04
    f32 z;   // 0x08
};

// Element linked into the func_80470184 chains: each chain slot writes its
// next-index through the u16 field at +0x10+2*slot.
struct LODLinkNode {
    u8  mPad_00[0x10];
    s16 mLinks[3];       // 0x10, 0x12, 0x14 (one per chain slot)
};

// func_80470184 view of LODMemMan: the { elem, next } pair array at +0x8 and
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
    u8  mPad_0C[0x40 - 0xC];       // 0x0c..0x3f
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
    u8  mPad_06[0x18 - 0x6];
    u32 field_0x18;            // 0x18 u16-group offset in shared buffer
    u8  mPad_1C[0x24 - 0x1C];
    u32 field_0x24;            // 0x24 flags (bit 0: skip value update)
    nw4r::math::VEC3 field_0x28;  // 0x28 default PS scale source
    u8  mPad_34[0x40 - 0x34];
    f32 field_0x40;            // 0x40 scale
    f32 field_0x44;            // 0x44 limit
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

// 0xc-byte g3d object slot (func_80471184 destroys the pointer at +0x0;
// func_8047108C uses the flag word at +0xA).
struct LODG3dSlot {
    nw4r::g3d::G3dObj* field_0x0;  // 0x00 g3d object
    u8  mPad_04[0xA - 0x4];        // 0x04..0x09
    u16 mFlags;                    // 0x0A (bit 0: busy, bit 2: done)
};

// Vtable mirror for the g3d manager object (LODMemMan+0x0).  MWCC vtables
// carry an 8-byte RTTI header, so declared slot 0 sits at vtable+0x8 and the
// slot func_80471184 dispatches through (vtable+0x3C) is the 14th virtual.
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

// Data view of the g3d manager object (LODMemMan+0x0): func_80470EF8 hands
// the word at +0xE4 to the vtable+0x34 callback.
struct LODMgrObj {
    u8  mPad_00[0xE4];
    u32 field_0xE4;              // 0xE4
};

// 0xc-byte bind slot used by func_80470EF8 (overlay of LODMemMan+0xC8).
struct LODBindSlot {
    nw4r::g3d::ScnMdl* field_0x0;  // 0x00 ScnMdl
    u32 field_0x4;                 // 0x04 bound id
    u8  mPad_08[0xA - 0x8];
    u16 mFlags;                    // 0x0A (bit 0: bound)
};

// 16-slot view of LODMemMan+0xC8 for func_80470EF8.
struct LODBindView {
    u8 mPad_00[0xC8];
    LODBindSlot mSlots[16];        // 0xC8..0x187
};

// View frame returned by func_8049626C; +0x9C is handed to the LOD layer
// updates and +0x1E0 holds the scale (mirrors UnkViewFrame in
// code_8047BB54.cpp).
struct LODViewFrame {
    u8  mPad_00[0x9C];             // 0x00..0x9b
    u8  field_0x9C[0x1E0 - 0x9C];  // 0x9c..0x1df
    f32 field_0x1E0;               // 0x1e0 layer scale
};

// Scene object returned by func_8048ECD0; the byte at +0x19 gates the
// per-frame LOD layer update.
struct LODScnGate {
    u8 mPad_00[0x19];              // 0x00..0x18
    u8 field_0x19;                 // 0x19 gate byte
};

// 0x20-byte pool block (func_8047163C / func_80471718 list node).
struct LODPoolBlock {
    u32 mFlags;              // 0x00 (bit 0: in use, bit 1: ready)
    u32 mSize;               // 0x04
    LODPoolBlock* mNext;     // 0x08
    u32 field_0xC;           // 0x0C
    u32 mId;                 // 0x10
    u8  mPad_14[0x20 - 0x14];// 0x14..0x1F
};

// Per-id size pair in the table at (field_0x4 + 0x74).
struct LODPoolPair {
    u32 field_0x0;           // 0x00
    u32 field_0x4;           // 0x04
};

// Buffer at field_0x4: 0x74-byte header followed by a per-id u32 table
// (func_80471BF4 reads the two words at id*2 / id*2+1 to size a block).
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

// 3-float box view at LODMemMan+0x20 (func_8046F164 compares element boxes
// against this degenerate box).
struct LODBoxCenterView {
    u8  mPad_00[0x20];       // 0x00..0x1f
    f32 mBox_20[3];          // 0x20..0x2b
};

// 0xC-byte child entry (array at base+0xC, func_8047108C second loop).
struct LODChildEntry {
    u8* field_0x0;           // 0x00 child object
    u16 field_0x4;           // 0x04
    u16 field_0x6;           // 0x06 flag bits (bit 0: active)
    u8  mPad_08[0xC - 0x8];  // 0x08..0x0B
};

// View of the child-entry array at base+0xC.
struct LODChildEntryView {
    u8 mPad_00[0xC];         // 0x00..0x0B
    LODChildEntry mEntries[16];  // 0x0C..0xCB
};

// func_8046E6DC: the shared buffer at +0x5C carries an element count at +0x34.
struct LODSharedBufView {
    u8  mPad_00[0x34];       // 0x00..0x33
    u32 mCount_34;           // 0x34 element count
};

// View descriptor passed to the per-frame update functions; +0x18/+0x1C are
// offsets into the shared buffer at field_0x5C (a u16 count followed by u16
// data).
struct LODViewDesc {
    u8  mPad_00[0x18];       // 0x00..0x17
    u32 field_0x18;          // 0x18 u16-group offset in shared buffer
    u32 field_0x1C;          // 0x1C u16-group offset in shared buffer
};

// Ctor-only view of the g3d-slot region: each 0xc-byte slot is three words.
// The +0xC8 region holds 17 slots (0xc8..0x193, the 17th slot is the +0x188
// word group); the func_8047108C slot array sits at +0x194 (16 slots).
struct LODCtorSlot {
    u32 field_0x0;   // 0x00
    u32 field_0x4;   // 0x04
    u32 field_0x8;   // 0x08
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

// Ctor-view overlay of the region at +0xC8..+0x254 and the +0xa44 sub-manager.
// Casting `this` to this view lets MWCC fold every offset into the this-base
// (retail emits `stw rX, 0xcc(r3)` etc. directly against r3).
struct LODCtorView {
    u8          mPad_00[0xC8];       // 0x00..0xC7
    LODCtorSlot mSlotsA[17];          // 0xC8..0x193
    LODCtorSlot mSlotsB[16];          // 0x194..0x253
    u8          mPad_254[0xA44 - 0x254];
    LODSubA44   mSub;                 // 0xA44..0xABB
};

// 0xc-byte ResFile slot used by func_80470DCC: 16 entries at (self+0x8).
// The u16 at +0x0A is the in-use flag (bit 0); the id at +0x08 is -1 when
// the slot is free.  The u32 at +0x00 is the aligned ResFile data base and
// the u32 at +0x04 the pool block pointer it was carved from.
struct LODResFileSlot {
    u32 field_0x8;    // +0x00 ResFile/end base
    u32 field_0xC;    // +0x04 pool block pointer
    s16 field_0x10;   // +0x08 element id (-1 = free)
    u16 field_0x12;   // +0x0A flags (bit 0: in use)
};

// 16-slot view of the +0x8..0xc8 region used by func_80470DCC.
struct LODResFileSlotArray {
    u8  mPad_00[0x8];            // 0x00..0x07
    LODResFileSlot mSlots[16];   // 0x8..0xc8
};

// 0x1c-strided record (array base at LODMemMan+0x98).  Bit 2 of the flag
// word gates the u16 threshold at +0xA; bit 1 drives the func_804A6D90
// submit path and bit 0 the func_8046A3B4 billboard helper.
struct LODRec1C {
    u32 field_0x0;    // 0x00 flags (bit 0: billboard, bit 1: submit, bit 2: threshold)
    u8  mPad_04[0x8 - 0x4];
    u16 field_0x8;    // 0x08 index passed to func_8046A3B4
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
    /* 0x1C */ u8* mView_1C;                  // view (func_80496288 arg)
    /* 0x20 */ u32 field_0x20;                // flag word (bit 6: buffer allocated)
    /* 0x24 */ u32 field_0x24;                // allocated block (func_80471BF4)
    /* 0x28 */ u32 field_0x28;                // size-table value
    /* 0x2C */ u32 field_0x2C;                // size-table value
    /* 0x30 */ CFileHandle* field_0x30;       // file handle cancelled on reset
    /* 0x34 */ char mStr_34[0x5C - 0x34];     // reset-time name string
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
    /* 0x88 */ u8 mPad_88[0x90 - 0x88];
    /* 0x90 */ LODElem48* field_0x90;         // 0x48-strided descriptor list
    /* 0x94 */ LODTypeDesc18* field_0x94;     // 0x18-strided type descriptors
    /* 0x98 */ LODRec1C* field_0x98;          // 0x1c-strided records
    /* 0x9C */ LODDesc74* field_0x9C;         // 0x74-strided type descriptors
    /* 0xA0 */ u8 mPad_A0[0xA4 - 0xA0];
    /* 0xA4 */ s32 field_0xA4;                // type-descriptor count
    /* 0xA8 */ nw4r::g3d::G3dObj* field_0xA8;  // g3d object (destroyed on reset)
    /* 0xAC */ nw4r::g3d::G3dObj* field_0xAC;  // g3d object (destroyed on reset)
    /* 0xB0 */ u8 mPad_B0[0xB4 - 0xB0];
    /* 0xB4 */ u8* field_0xB4;                 // scene hook (set by func_8046E5BC)
    /* 0xB8 */ u16 field_0xB8;                // setter target (func_8046E770)
    /* 0xBA */ u16 field_0xBA;                // setter target (func_8046E770)
    /* 0xBC */ u16 field_0xBC;                // setter target (func_8046E770)
    /* 0xBE */ u16 field_0xBE;                // setter target (func_8046E770)
    /* 0xC0 */ f32 field_0xC0;                // LOD distance constants (1.0 init)
    /* 0xC4 */ f32 field_0xC4;
    /* 0xC8 */ LODG3dSlot field_0xC8[16];     // g3d object slots (0xc8..0x188)
    /* 0x188 */ u8 mPad_188[0x1CDC - 0x188];
    /* 0x1CDC */ f32 field_0x1CDC;            // shared LOD distance scale
};

// Sub-manager pointer at +0xF0, which sits inside the 0xc8 slot range, is
// accessed through its own overlay.
struct LODSubMgrView {
    u8 mPad_00[0xF0];
    LOD::LODMemMan* field_0xF0;               // 0xf0 sub-manager
};

// ---------------------------------------------------------------------------
// Retail-named imports for the LOD targets.  The Fv-mangled callees that
// receive extra args in retail are declared with the exact mangled names.
// MWCC re-mangles `__`-containing global names by appending the parameter
// signature, so the exact retail names are forced with C linkage (the
// reloc_map tool's approved fix for name drift).
// ---------------------------------------------------------------------------
extern "C" void func_804702F0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODElem20* elem);
extern "C" void func_8046DD9C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u8* p, f32 f1);
extern "C" void func_8046E1DC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u8* p, int a);

// TU-internal callees with extra args (retail symbol names).  MWCC appends
// `__F<params>` to plain C++ declarations at call sites, so C linkage keeps
// the exact retail names (the Fv suffix is a decompiler guess).  The func_*
// bodies are the retail-named member stubs / definitions below.
extern "C" u32 func_804BE4A0();
extern "C" LODPoolBlock* func_80471718__Q23LOD9LODMemManFv(LODPoolBlock* self, int id);
extern "C" u8* func_80471BF4__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int id);
extern "C" u8* func_8047163C__Q23LOD9LODMemManFv(LODPoolBlock* self, u32 size, int id);
extern "C" void func_8046F258__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u16 value, u8* data);
extern "C" void func_8046F164__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODBoxElem* box);
extern "C" void func_8046F594__Q23LOD9LODMemManFv(LOD::LODMemMan* self);

// Scene / view helpers defined in other monolib TUs.  Retail kept the plain
// unmangled names; C linkage forces the verbatim names at the call sites.
extern "C" CScn* func_8049698C();
extern "C" LODViewFrame* func_8049626C(CScn* camera, CView* view);
extern "C" u32 func_8048ECD0(CScn* self);
extern "C" nw4r::g3d::ScnObj* func_8048EC14(CScn* self, u32 idx);
extern "C" void* func_8048ECE4(CScn* self);
// CLight::func_804C09E8 (retail keeps the plain unmangled call).
extern "C" void func_804C09E8(u8* outLight, u8* matrix);

// func_804702F0's callees: the LOD record helper (coli/code_804A6C60.cpp)
// and the scene resource helpers (scn/code_804BC9EC.cpp).  Retail keeps the
// plain unmangled names (C ABI); the Fv-suffixed names carry explicit ABI
// args (MWCC_CASES "Fv ABI note").
extern "C" s32 func_804A6D90(void* rec);
extern "C" void* func_804BC9EC__Fv(void);
extern "C" u8 func_804BCC6C(void* ptr, u16 id);
extern "C" void func_804BCC30(void* unused, s32 a);
extern "C" void func_804BCC3C(void* unused, s32 a);
extern "C" void func_804BCC60(void* unused, s32 a);
extern "C" void func_8046A3B4__Q23LOD17UnkClass_80468434Fv(u32 idx, const f32* srcMtx, u8 arg2);

// TU-internal unlink helper (definition later in this file).
extern "C" void func_8046FEB8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODElem20* elem);

// TU-internal sub-manager callbacks (definitions later in this file).
extern "C" void func_80471224__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CScn* scene);
extern "C" void func_80471184__Q23LOD9LODMemManFv(LOD::LODMemMan* self);
extern "C" void func_804712E0__Q23LOD9LODMemManFv(LOD::LODMemMan* self);
extern "C" void func_8047130C__Q23LOD9LODMemManFv(LOD::LODMemMan* self);
extern "C" void func_80471748__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u32 x);

// --- FULL_MATCH functions ---

// Simple getter: return the float at offset 0x7c.
float LOD::LODMemMan::func_8046F01C() {
    return mFloat_7C;
}

// Tail-forward call to embedded sub-object's func_8046AADC.
void LOD::LODMemMan::func_8046F088() {
    mSubObject.func_8046AADC();
}

// --- Remaining harness stubs (empty bodies) ---

// ---------------------------------------------------------------------------
// LODMemMan ctor.  Zeroes the g3d-slot region (+0xC8, 17 slots) and the
// func_8047108C slot array (+0x194, 16 slots): slot[0] of the +0xC8 region
// keeps its first word, slots[1..16] get words 0/4 cleared (the word-0 walk
// is a pointer loop that MWCC keeps as a real loop; the word-4 and +0x194
// walks unroll), then the +0xa44 sub-manager is initialised (vtable, first
// fields, name string) and the embedded UnkClass_8046A530 / func_8046DA04
// are run.
// ---------------------------------------------------------------------------
LOD::LODMemMan::LODMemMan() {
    LODCtorView* cv = (LODCtorView*)this;
    LODMemManLayout* l = (LODMemManLayout*)this;
    LODCtorSlot* s = &cv->mSlotsA[1];
    LODCtorSlot* end = &cv->mSlotsA[17];
    l->field_0x0 = 0;

    // Clear the g3d-slot region: slot[0] of the +0xC8 array keeps its first
    // word; slots[1..16] get words 0/4 cleared (the word-0 walk is a pointer
    // do-while that MWCC keeps rolled; the word-4 walk and the +0x194 array
    // unroll).
    cv->mSlotsA[0].field_0x8 = 0;
    do {
        s->field_0x0 = 0;
    } while (++s < end);
    cv->mSlotsA[0].field_0x4 = 0;
    for (int i = 1; i < 17; i++) cv->mSlotsA[i].field_0x4 = 0;
    for (int i = 0; i < 16; i++) cv->mSlotsB[i].field_0x0 = 0;

    // +0xa44 sub-manager: vtable, first fields, and the name string.
    cv->mSub.field_0x0 = (u8*)lbl_eu_8056DB08;
    cv->mSub.mStr_34[0] = 0;
    cv->mSub.field_0x74 = 0;
    cv->mSub.field_0x4 = 0;
    cv->mSub.field_0x8 = 0;
    cv->mSub.field_0x20 = 0;
    cv->mSub.field_0x30 = 0;
    cv->mSub.field_0x74 = strlen(lbl_eu_80663834);
    strcpy(cv->mSub.mStr_34, lbl_eu_80663834);

    mSubObject.func_8046A530();
    func_8046DA04();
}

LOD::LODMemMan::~LODMemMan() {}

void __dt__8046D144(){}

void LOD::LODMemMan::func_8046D264() {}

void LOD::LODMemMan::func_8046D898() {}

#pragma push
#pragma auto_inline off
#pragma scheduling off
void LOD::LODMemMan::func_8046DA04() {
    LODMemManLayout* l = (LODMemManLayout*)this;
    f32 v0;             // declared first -> colors f0 (retail lfs f1=1.0 first, lfs f0=scale second)
    f32 v1;             // -> f1
    s16 nine = 9;       // hoisted materialization for the +0xB8 store (retail li r0,9 in the prologue)
    v1 = lbl_eu_8066A6C0;   // 1.0f constant (0xC0/0xC4/0xC8 stores) - assigned first so it loads first
    v0 = lbl_eu_8066A6D8;   // scale divisor (0x7C/0x1CDC/0x84 stores)
    l->field_0x4 = 0;
    l->field_0xC = 0;
    l->mCount_18 = 0;
    l->field_0x5C = 0;
    l->field_0xA8 = 0;
    l->field_0xAC = 0;
    l->field_0xB4 = 0;
    l->field_0x6C = 0;
    l->field_0xB8 = nine;
    l->field_0xBA = 0;
    l->field_0xBC = 0;
    *(f32*)&l->field_0xC8[0].field_0x0 = v1;
    l->field_0xC4 = v1;
    l->field_0xC0 = v1;
    l->field_0xBE = 0;
    l->field_0x7C = v0;
    l->field_0x1CDC = v0;
    l->field_0xA4 = 0;
    l->field_0x84 = v0;
}
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_8046DA64: clear the view/shared-buffer pointers, then reset the +0xCC
// sub-manager (both g3d passes) and the +0xA44 sub-manager, and clear flag
// bits 0x800 (retail keeps li r4,0 for the first call's unused scene arg;
// the u8* self local keeps `self` in r31 so the +0xCC temps recompute per
// call instead of hoisting into a saved register).
// ---------------------------------------------------------------------------
void func_8046DA64__Q23LOD9LODMemManFv(LOD::LODMemMan* self) {
    u8* s = (u8*)self;
    LODMemManLayout* l = (LODMemManLayout*)self;
    l->mView_1C = 0;
    l->field_0x5C = 0;
    func_80471224__Q23LOD9LODMemManFv((LOD::LODMemMan*)(s + 0xCC), 0);
    func_80471184__Q23LOD9LODMemManFv((LOD::LODMemMan*)(s + 0xCC));
    ((LOD::LODMemMan*)(s + 0xA44))->func_80471938();
    l->field_0x6C &= 0x800;
}

// ---------------------------------------------------------------------------
// func_8046DAC0: release the two g3d objects (+0xA8 / +0xAC), detaching each
// from the scene's root group (func_8048EC14 index 7/8) first, then tear down
// the +0xCC sub-manager and the +0xA44 sub-manager (plus its func_80471A70
// pass when `param` is set).  Ends by clearing the view/shared-buffer pointers
// and toggling flag bits 0x3087 -> 0x8000.
// ---------------------------------------------------------------------------
void func_8046DAC0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int param) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0xA8) {
        nw4r::g3d::ScnGroup* group = (nw4r::g3d::ScnGroup*)func_8048EC14((CScn*)l->mView_1C, 7);
        if (group->Size() != 0) {
            group->Remove((nw4r::g3d::ScnObj*)l->field_0xA8);
        }
        l->field_0xA8->Destroy();
        l->field_0xA8 = 0;
    }
    if (l->field_0xAC) {
        nw4r::g3d::ScnGroup* group = (nw4r::g3d::ScnGroup*)func_8048EC14((CScn*)l->mView_1C, 8);
        if (group->Size() != 0) {
            group->Remove((nw4r::g3d::ScnObj*)l->field_0xAC);
        }
        l->field_0xAC->Destroy();
        l->field_0xAC = 0;
    }
    func_80471224__Q23LOD9LODMemManFv((LOD::LODMemMan*)((u8*)self + 0xCC), (CScn*)l->mView_1C);
    ((LOD::LODMemMan*)((u8*)self + 0xA44))->func_804719FC();
    if (param) {
        ((LOD::LODMemMan*)((u8*)l + 0xA44))->func_80471A70();
    }
    l->mView_1C = 0;
    l->field_0x5C = 0;
    l->field_0x6C = (l->field_0x6C & -0x3087) | 0x8000;
}

void LOD::LODMemMan::func_8046DBC8() {}

void LOD::LODMemMan::func_8046DD9C() {}

void LOD::LODMemMan::func_8046E1DC() {}

// ---------------------------------------------------------------------------
// func_8046E594: when `param` is zero set bit 3 of the +0x6C flag word,
// otherwise clear bit 6.
// ---------------------------------------------------------------------------
void func_8046E594__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int param) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (param != 0) {
        l->field_0x6C &= ~8;
    } else {
        l->field_0x6C |= 8;
    }
}

// ---------------------------------------------------------------------------
// func_8046E5BC: when `param` is nonzero, attach both g3d objects (+0xA8/
// +0xAC) to the scene groups from func_8048EC14 index 7/8 (notifying the
// group's g3d manager and re-attaching the object with its +0xE4 word), then
// store `param` into +0xB4 and set flag bit 0x800; otherwise clear +0xB4
// and the flag.
// ---------------------------------------------------------------------------
void func_8046E5BC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u32 param) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (param) {
        if (l->field_0xA8) {
            nw4r::g3d::G3dObj* group = func_8048EC14((CScn*)l->mView_1C, 7);
            LODMgrObj* mgr = (LODMgrObj*)group;
            if (mgr->field_0xE4) {
                ((LODG3dMgrVt*)group)->vf0D(l->field_0xA8);
            }
            ((LODG3dMgrVt*)group)->vf0B(mgr->field_0xE4, l->field_0xA8);
        }
        if (l->field_0xAC) {
            nw4r::g3d::G3dObj* group = func_8048EC14((CScn*)l->mView_1C, 8);
            LODMgrObj* mgr = (LODMgrObj*)group;
            if (mgr->field_0xE4) {
                ((LODG3dMgrVt*)group)->vf0D(l->field_0xAC);
            }
            ((LODG3dMgrVt*)group)->vf0B(mgr->field_0xE4, l->field_0xAC);
        }
        l->field_0xB4 = (u8*)param;
        l->field_0x6C = (l->field_0x6C | 0x800) & ~0x1000;
    } else {
        l->field_0xB4 = 0;
        l->field_0x6C &= ~0x1800;
    }
}

// ---------------------------------------------------------------------------
// func_8046E6DC: store the scale into +0x1CDC, then - while the +0x5C buffer
// is present - walk the elements and, for each whose +0x44 has bit 3 set and
// whose +0x47 descriptor's flag bits are nonzero, refresh the element via
// func_804BC9EC/func_804BCC6C and record the result byte at +0x1D.
// ---------------------------------------------------------------------------
void func_8046E6DC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, f32 f1) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    l->field_0x1CDC = f1;
    if (l->field_0x5C == 0) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    LODSharedBufView* buf = (LODSharedBufView*)l->field_0x5C;
    u32 i = 0;
    while (i < buf->mCount_34) {
        if (!(p48->field_0x44 & 8)) goto next;
        if ((l->field_0x98[p48->field_0x47].field_0x0 & 3) == 0) goto next;
        p20->field_0x1D = func_804BCC6C(func_804BC9EC__Fv(), p48->field_0x40);
    next:
        i++;
        p20++;
        p48++;
    }
}

// ---------------------------------------------------------------------------
// func_8046E770: stores the three short params into the 0xB8..0xBC fields.
// ---------------------------------------------------------------------------
void func_8046E770__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u16 a, u16 b, u16 c) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    l->field_0xB8 = a;
    l->field_0xBA = b;
    l->field_0xBC = c;
}

// ---------------------------------------------------------------------------
// func_8046E780: when the LOD system is enabled, walk every element matching
// `task` and clear bit 1 of its flag word (retail keeps one loop copy).
// ---------------------------------------------------------------------------
void func_8046E780__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
        if (p48->field_0x40 == task) p20->field_0x0 &= ~2u;
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// func_8046E7D0: when the LOD system is enabled, walk every element matching
// `task` and set bit 1 of its flag word.
// ---------------------------------------------------------------------------
void func_8046E7D0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
        if (p48->field_0x40 == task) p20->field_0x0 |= 2u;
        i++;
        p48++;
        p20++;
    }
}

// ---------------------------------------------------------------------------
// func_8046E820: when the LOD system is enabled, walk every element matching
// `task` and flip bit 1 of its flag word - set when `flag` is zero, cleared
// otherwise (retail keeps two copies of the loop, one per bit operation).
// ---------------------------------------------------------------------------
void func_8046E820__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, int flag) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (flag) {
        if (!(l->field_0x6C & 1)) return;
        int i = 0;
        LODElem20* p20;
        LODElem48* p48;
        p48 = l->field_0x90;
        p20 = l->field_0x4;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) p20->field_0x0 &= ~2u;
            i++;
            p48++;
            p20++;
        }
    } else {
        if (!(l->field_0x6C & 1)) return;
        int i = 0;
        LODElem20* p20;
        LODElem48* p48;
        p48 = l->field_0x90;
        p20 = l->field_0x4;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) p20->field_0x0 |= 2u;
            i++;
            p48++;
            p20++;
        }
    }
}

// ---------------------------------------------------------------------------
// func_8046E8C8: when the LOD system is enabled, walk every element matching
// `task`; on the first match return whether bit 1 of its flag word is clear
// (bool; retail materializes !(flag&2) via rlwinm/cntlzw/srwi).
// ---------------------------------------------------------------------------
bool func_8046E8C8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        int i = 0;
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                return !(p20->field_0x0 & 2);
            }
            i++;
            p48++;
            p20++;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// func_8046E920: when the LOD system is enabled, compute a fixed-point scale
// (u16 truncation of pool*f1) and store it into field_0xE of every element
// matching `task`.
// ---------------------------------------------------------------------------
void func_8046E920__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, f32 f1) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    u16 v = (u16)(s32)(lbl_eu_8066A6C0 * f1);
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

void LOD::LODMemMan::func_8046E988() {}

// ---------------------------------------------------------------------------
// func_8046EAE8: when the LOD system is enabled, walk every element matching
// `task` and set/clear bit 0x10 of its flag word according to `flag`.
// ---------------------------------------------------------------------------
void func_8046EAE8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, int flag) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
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
// func_8046EB50: if the LOD system is enabled, push the scale value into
// every element whose id matches `task`; elements with the follow-through
// flag bit and a nonzero enable field then refresh their animation state.
// ---------------------------------------------------------------------------
void func_8046EB50__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, f32 v) {
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
                        ((LOD::UnkClass_8046A530*)p20)->func_8046CFB4();
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
// func_8046EBEC: same id walk as func_8046EB50, but the active value is
// refreshed from the cached value instead of a parameter.
// ---------------------------------------------------------------------------
void func_8046EBEC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                p20->field_0x4 = p20->field_0x8;
                if (p48->field_0x44 & 8) {
                    if (p48->field_0x42 != 0) {
                        ((LOD::UnkClass_8046A530*)p20)->func_8046CFB4();
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
// func_8046EC88: when the LOD system is enabled, return the +0x4 float of the
// first element matching `task`, else the A6C0 pool constant.
// ---------------------------------------------------------------------------
f32 func_8046EC88__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
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
// func_8046ECD4: when the LOD system is enabled, walk every element matching
// `task`; when `flag` is nonzero and the element's +0x42 is nonzero, clear
// bits 0x120 of its flag word; otherwise set bit 0x100 when +0x44 bit 3 is
// set, else bit 0x40.
// ---------------------------------------------------------------------------
void func_8046ECD4__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, int flag) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
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
// func_8046ED68: when the LOD system is enabled, walk every element matching
// `task` and set/clear bit 7 of its flag word according to `flag`.
// ---------------------------------------------------------------------------
void func_8046ED68__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, int flag) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
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
// func_8046EDD0: for each element matching `task`, when its byte state (+0x1E)
// differs from `value` and the 0x1000 flag is set, refresh +0x1C/+0x1E/+0x1F
// and link the element into the index-linked list (head/tail at +0x10/+0x14)
// unless it is already linked (0x10000 flag).
// ---------------------------------------------------------------------------
void func_8046EDD0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, int value) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(l->field_0x6C & 1)) return;
    LODElem48* p48 = l->field_0x90;
    LODElem20* p20 = l->field_0x4;
    int i = 0;
    while (i < l->mCount_18) {
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
                    if (l->field_0x14 == 0) {
                        l->field_0x10 = p20;
                    } else {
                        l->field_0x14->field_0x16 = p20->field_0x1A;
                    }
                    l->field_0x14 = p20;
                    p20->field_0x16 = lbl_eu_80663828;
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
// func_8046EE9C: when the LOD system is enabled, return the +0x1E byte of the
// first element matching `task`, or 0.
// ---------------------------------------------------------------------------
u8 func_8046EE9C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == task) {
                return p20->field_0x1E;
            }
            i++;
            p48++;
            p20++;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// func_8046EEE8: when the LOD system is enabled, store `value` into the +0x8
// float of every element matching `task`.
// ---------------------------------------------------------------------------
void func_8046EEE8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task, f32 value) {
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
// func_8046EF30: when the LOD system is enabled, return the +0x8 float of the
// first element matching `task`, else the A6C0 pool constant.
// ---------------------------------------------------------------------------
f32 func_8046EF30__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
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
// func_8046EF7C: when the LOD system is enabled, scale the base value by the
// current frame delta and forward the matching element to func_804702F0.
// ---------------------------------------------------------------------------
void func_8046EF7C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int id) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x6C & 1) {
        f32 dt = func_80496288(l->mView_1C);
        l->field_0x80 = l->field_0x7C * dt;
        LODElem48* p48 = l->field_0x90;
        LODElem20* p20 = l->field_0x4;
        int i = 0;
        while (i < l->mCount_18) {
            if (p48->field_0x40 == id) {
                func_804702F0__Q23LOD9LODMemManFv(self, p20);
            }
            i++;
            p48++;
            p20++;
        }
    }
}

void LOD::LODMemMan::func_8046F010(float a) { *(float*)((u8*)this + 0x1CC8) = a; *(float*)((u8*)this + 0x7C) = a; }

// ---------------------------------------------------------------------------
// func_8046F024: when `mode` is nonzero, publish the `value` short into +0xBE
// and set flag bit 0x100 of +0x6C (forcing +0xBE to 1 when value >= 2);
// otherwise clear the flag.  Then scale +0x84 by 1/f1 unless f1 matches the
// A6C0 pool constant (then the stored scale is used as-is).
// ---------------------------------------------------------------------------
void func_8046F024__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int mode, u16 value, f32 f1) {
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
// func_8046F090: per-frame update.  A nonzero scene-check result with the
// 0x20 flag of field_0x6C clear sets bit 4 of field_0x68, otherwise it is
// cleared.  Then the view descriptor's u16 group at +0x18 (count followed by
// element indices) refreshes each referenced box element, the +0x1C group
// feeds func_8046F258, and the pass ends with func_8046F594.
// ---------------------------------------------------------------------------
void func_8046F090__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODViewDesc* view) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (func_804BE4A0() != 0 && (l->field_0x6C & 0x20) == 0) {
        l->field_0x68 |= 0x10;
    } else {
        l->field_0x68 &= ~0x10;
    }
    if (view->field_0x18 != 0) {
        u8* p = l->field_0x5C;
        p += view->field_0x18;
        u16* entries = (u16*)(p + 2);
        u16 count = *(u16*)p;
        int i = 0;
        while (i < count) {
            func_8046F164__Q23LOD9LODMemManFv(self, &l->field_0x60[*entries]);
            i++;
            entries++;
        }
    }
    if (view->field_0x1C != 0) {
        u8* group = l->field_0x5C + view->field_0x1C;
        u16 value = *(u16*)group;
        func_8046F258__Q23LOD9LODMemManFv(self, value, group + 2);
    }
    func_8046F594__Q23LOD9LODMemManFv(self);
}

// ---------------------------------------------------------------------------
// func_8046F164: if the element box overlaps the manager's box (a degenerate
// point at +0x20), refresh the element's u16 child groups the same way as
// func_8046F090.
// ---------------------------------------------------------------------------
void func_8046F164__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODBoxElem* box) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    LODBoxCenterView* bv = (LODBoxCenterView*)self;
    if (box->mMinX < bv->mBox_20[0]) return;
    if (box->mMinY < bv->mBox_20[1]) return;
    if (box->mMinZ < bv->mBox_20[2]) return;
    if (box->mMaxX > bv->mBox_20[0]) return;
    if (box->mMaxY > bv->mBox_20[1]) return;
    if (box->mMaxZ > bv->mBox_20[2]) return;
    if (box->field_0x18 != 0) {
        u8* p = l->field_0x5C;
        p += box->field_0x18;
        u16* entries = (u16*)(p + 2);
        u16 count = *(u16*)p;
        int i = 0;
        while (i < count) {
            func_8046F164__Q23LOD9LODMemManFv(self, &l->field_0x60[*entries]);
            i++;
            entries++;
        }
    }
    if (box->field_0x1C != 0) {
        u8* group = l->field_0x5C + box->field_0x1C;
        u16 value = *(u16*)group;
        func_8046F258__Q23LOD9LODMemManFv(self, value, group + 2);
    }
}

void LOD::LODMemMan::func_8046F258() {}

void LOD::LODMemMan::func_8046F594() {}

void LOD::LODMemMan::func_8046FC04() {}

// ---------------------------------------------------------------------------
// func_8046FEB8: unlink `elem` from the index-linked list.  Each element's
// +0x16 holds the next element index (sentinel lbl_eu_80663828) and +0x1A the
// element's own index; head removal advances the head, empty-list removal
// clears both pointers, and mid-list removal splices the successor into the
// predecessor's link.
// ---------------------------------------------------------------------------
void func_8046FEB8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODElem20* elem) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (!(elem->field_0x0 & 0x10000)) return;
    elem->field_0x0 &= ~0x10000;
    if (l->field_0x14 == 0 || l->field_0x10 == 0) {
        elem->field_0x16 = lbl_eu_80663828;
        return;
    }
    if (l->field_0x10 == elem) {
        int next = elem->field_0x16;
        if (next != lbl_eu_80663828) {
            l->field_0x10 = &l->field_0x4[next];
        } else {
            l->field_0x10 = 0;
            l->field_0x14 = 0;
        }
    } else {
        int idx = l->field_0x10->field_0x1A;
        while (idx != lbl_eu_80663828) {
            LODElem20* p = &l->field_0x4[idx];
            if (p->field_0x16 == elem->field_0x1A) {
                p->field_0x16 = elem->field_0x16;
                if (l->field_0x14 == elem) l->field_0x14 = p;
                break;
            }
            idx = p->field_0x16;
        }
    }
    elem->field_0x16 = lbl_eu_80663828;
}

// ---------------------------------------------------------------------------
// func_8046FF84: per-element chain update.  The element is linked into the
// +0x8 pair array (six pointers per id: {elem,next} for each of the three
// chain slots).  The descriptor's flag bits select the chain slots; bits 0/1
// of the +0x4 flags also drive the +0x0 flag-word update.  When the 0x80
// descriptor bit is set the element's +0xC distance is nudged toward the
// 0..0xff range based on the squared horizontal length of `v`.
// ---------------------------------------------------------------------------
extern "C" void func_8046FF84__Q23LOD9LODMemManFv(
    LOD::LODMemMan* self, LODElem20* elem, u16 id, const LODVec3f* v)
{
    LODMemManLayout* l = (LODMemManLayout*)self;
    LODTypeDesc18* desc = &l->field_0x94[id];
    int base = id * 6;
    s32 flag1 = 0;
    s32 flag2 = 0;
    l->field_0x70 |= desc->mFlags;
    l->field_0x68 |= 1;
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
                l->field_0x68 |= 4;
                if (((LODBuildList*)self)->mPairs[base + 2] != 0) {
                    ((LODLinkNode*)((LODBuildList*)self)->mPairs[base + 3])
                        ->mLinks[1] = elem->field_0x1A;
                } else {
                    ((LODBuildList*)self)->mPairs[base + 2] = elem;
                }
                ((LODBuildList*)self)->mPairs[base + 3] = elem;
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
            l->field_0x68 |= (2 << i);
            if (((LODBuildList*)self)->mPairs[w] != 0) {
                ((LODLinkNode*)((LODBuildList*)self)->mPairs[w + 1])
                    ->mLinks[i] = elem->field_0x1A;
            } else {
                ((LODBuildList*)self)->mPairs[w] = elem;
            }
            ((LODBuildList*)self)->mPairs[w + 1] = elem;
        }
    }
}

// ---------------------------------------------------------------------------
// func_80470184: rebuild the six element chains from the pair array at +0x8.
// Each pair is { element, next }; the element's bit-0 flag selects the
// bit0-set chain (heads at +0x48) or the bit0-clear chain (heads at +0x3C).
// Element i of a group links into the group's chain through the u16 link
// field at +0x10+2*i, and each chain's final tail is terminated with the
// sentinel index.
// ---------------------------------------------------------------------------
void func_80470184__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int count) {
    LODBuildList* l = (LODBuildList*)self;
    LODElem20* tail2[3] = { 0, 0, 0 };
    LODElem20* tail1[3] = { 0, 0, 0 };
    int bound = count * 6;
    int w = 0;
    while (w < bound) {
        for (int i = 0; i < 3; i++, w += 2) {
            LODElem20* elem = l->mPairs[w];
            if (elem != 0) {
                if (elem->field_0x0 & 1) {
                    if (tail1[i] != 0) {
                        ((LODLinkNode*)tail1[i])->mLinks[i] = elem->field_0x1A;
                    } else {
                        l->mHeadSet[i] = elem;
                    }
                    tail1[i] = l->mPairs[w + 1];
                } else {
                    if (tail2[i] != 0) {
                        ((LODLinkNode*)tail2[i])->mLinks[i] = elem->field_0x1A;
                    } else {
                        l->mHeadClr[i] = elem;
                    }
                    tail2[i] = l->mPairs[w + 1];
                }
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        if (tail2[i] != 0) {
            ((LODLinkNode*)tail2[i])->mLinks[i] = (u16)lbl_eu_80663828;
        }
        if (tail1[i] != 0) {
            ((LODLinkNode*)tail1[i])->mLinks[i] = (u16)lbl_eu_80663828;
        }
    }
}

// ---------------------------------------------------------------------------
// func_804702F0: per-element LOD update.  While both 0x1000/0x2000 flags are
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
void func_804702F0__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODElem20* elem) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    u32 flags = elem->field_0x0;
    if ((flags & 0x1000) && (flags & 0x2000)) {
        if (elem->field_0x1C + 1 >= 0x5A) {
            elem->field_0x0 = flags & ~0x2000;
            LODElem48* d0 = &l->field_0x90[elem->field_0x1A];
            if (d0->field_0x42 == 0) {
                func_8046FEB8__Q23LOD9LODMemManFv(self, elem);
            }
        } else {
            elem->field_0x1C++;
        }
    }
    if (elem->field_0x0 & 0x40) return;
    if (elem->field_0x0 & 0x2) return;

    LODElem48* desc = &l->field_0x90[elem->field_0x1A];
    f32 hi = elem->field_0x8;         // upper bound (cached value)
    f32 lo = lbl_eu_8066A6C0;         // lower bound (0.0f)
    f32 step;
    if (desc->field_0x44 & 2) {
        union {
            double d;
            u32 w[2];
        } c;
        c.w[1] = desc->field_0x46;
        c.w[0] = 0x43300000u;
        step = (f32)(c.d - lbl_eu_8066A6E0) * lbl_eu_8066A710 * l->field_0x80;
    } else {
        step = l->field_0x80;
    }

    if (desc->field_0x44 & 8) {
        LODRec1C* rec = &l->field_0x98[desc->field_0x47];
        f32 orig = elem->field_0x4;
        if (rec->field_0x0 & 4) {
            union {
                double d;
                u32 w[2];
            } c;
            c.w[1] = rec->field_0xA;
            c.w[0] = 0x43300000u;
            f32 th = (f32)(c.d - lbl_eu_8066A6E0);
            if (orig >= th) {
                u32 bit7 = elem->field_0x0 & 0x80;
                if (bit7) hi = th;
                if (!bit7) lo = th;
            }
        }
        if (elem->field_0x0 & 0x80) {
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
            u32 f = f0 & ~0x4000;
            elem->field_0x0 = f;
            if (f0 & 0x100) {
                elem->field_0x4 = orig;
                f = (f & ~0x80) | 0x40;
                elem->field_0x0 = f;
                changed = 1;
            }
            f32 v = step * l->field_0x1CDC + elem->field_0x4;
            lbl_eu_80665754 = v;
            lbl_eu_80665758 = (s16)v;
            if (rec->field_0x0 & 2) {
                if (func_804A6D90(rec) != 0) {
                    func_804BC9EC__Fv();
                    func_804BCC30(func_804BC9EC__Fv(), desc->field_0x40);
                } else {
                    func_804BC9EC__Fv();
                    func_804BCC3C(func_804BC9EC__Fv(), desc->field_0x40);
                }
            }
            if (rec->field_0x0 & 1) {
                func_8046A3B4__Q23LOD17UnkClass_80468434Fv(
                    rec->field_0x8, (const f32*)desc, elem->field_0x1D);
                if (changed) {
                    func_804BC9EC__Fv();
                    func_804BCC60(func_804BC9EC__Fv(), elem->field_0x1D);
                }
            }
        } else {
            if (rec->field_0x0 & 1) {
                func_804BC9EC__Fv();
                func_804BCC60(func_804BC9EC__Fv(), elem->field_0x1D);
            }
        }
    } else {
        if (elem->field_0x0 & 0x80) {
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

// ---------------------------------------------------------------------------
// func_80470634: per-frame LOD update.  For each 0x74-byte type descriptor
// the referenced u16 group (shared buffer + desc->field_0x18) drives a
// per-element pass: each element's value is nudged toward its limit, a
// byte-parameter animation value is folded in, and the element's base
// position (plus a scaled PS vector) is written to the 0x48-byte output
// records.  The index field_0xE of the matched element is set from the
// current scale so the caller can pick the LOD kind.
// ---------------------------------------------------------------------------
extern "C" void func_80470634__Q23LOD9LODMemManFv(LOD::LODMemMan* self) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0xA4 == 0) return;
    int count = l->field_0xA4;
    LODDesc74* desc = l->field_0x9C;
    LODOut48* out = (LODOut48*)l->field_0xC;
    nw4r::math::VEC3 tmp;
    f64 magic = lbl_eu_8066A6E0;
    f32 c0 = lbl_eu_8066A710;
    f32 c11 = lbl_eu_8066A6D8;
    f32 c9 = lbl_eu_8066A70C;
    f32 c6 = lbl_eu_8066A714;
    f32 scale = lbl_eu_80665828;
    u8* ps = (u8*)desc + count * 0x74;  // PS vector stream after the descriptors
    int i = 0;
    while (i < l->field_0xA4) {
        f32 f2 = c11 / desc->field_0x44;
        f32 f3 = desc->field_0x40 * f2;
        f32 cur = scale;
        u16* group = (u16*)(l->field_0x5C + desc->field_0x18);
        int j = 0;
        while (j < desc->field_0x4) {
            u16 id = *group;
            LODElem48* d48 = &l->field_0x90[id];
            if (!(desc->field_0x24 & 1)) {
                f32 v;
                if (d48->field_0x44 & 2) {
                    union {
                        double d;
                        u32 w[2];
                    } c;
                    c.w[1] = d48->field_0x46;
                    c.w[0] = 0x43300000u;
                    v = (f32)(c.d - magic) * c0 * l->field_0x80 + out->field_0x40;
                } else {
                    v = l->field_0x80 + out->field_0x40;
                }
                if (desc->field_0x44 <= v) v -= desc->field_0x44;
                out->field_0x40 = v;
                if (lbl_eu_8066582C == 0) {
                    lbl_eu_8066582C = 1;
                    scale = c11 / lbl_eu_80663830;
                    lbl_eu_80665828 = scale;
                    cur = scale;
                }
                f32 limit = desc->field_0x44;
                f32 g = lbl_eu_80663830;
                f32 val = out->field_0x40;
                s32 idx = 0xff;
                LODElem20* elem = &l->field_0x4[id];
                if (val >= limit - g) {
                    idx = (s32)(cur * c9 * (limit - val));
                } else if (val <= g) {
                    idx = (s32)(cur * c9 * val);
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
            u32 b0, b1, b2;
            *(f32*)&b0 = s0;
            *(f32*)&b1 = s1;
            *(f32*)&b2 = s2;
            out->field_0x30 = b0;
            out->field_0x34 = b1;
            out->field_0x38 = b2;
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

// 0x40-byte child descriptor used by func_804708B4's group walk and by
// func_8047133C's slot registration.
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

extern "C" void func_8047133C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODChildDesc* desc, u8* mtx, int index);

// ---------------------------------------------------------------------------
// func_804708B4: per-frame LOD child binding.  Copies the caller's 13-word
// view matrix block onto the stack, inverts it in place (PSMTXInverse),
// resets the three child records (func_804712E0) and the light color
// (func_8047130C), then for each of the three groups walks its 64-strided
// descriptor array and registers every element with the +0x30 bit 16 set
// (func_8047133C), recording "seen" (2) vs "unseen" (0) into the two
// per-group .bss arrays.
// ---------------------------------------------------------------------------
extern "C" void func_804708B4__Q23LOD9LODMemManFv(
    LOD::LODMemMan* self, const f32* matrixBlock)
{
    LODMemManLayout* l = (LODMemManLayout*)self;
    Mtx mtx;
    u32* words = (u32*)mtx;
    for (int k = 0; k < 12; k++) {
        words[k] = *(const u32*)(matrixBlock + k);
    }
    PSMTXInverse(mtx, mtx);
    func_804712E0__Q23LOD9LODMemManFv((LOD::LODMemMan*)((u8*)self + 0xCC));
    func_8047130C__Q23LOD9LODMemManFv((LOD::LODMemMan*)((u8*)self + 0xCC));
    u8* base = (u8*)l->mView_1C;
    int* bss68 = lbl_eu_80658368;
    int* bss74 = lbl_eu_80658374;
    for (int g = 0; g < 3; g++) {
        u8* region = base + 20 + g * 4;
        LODChildDesc* arr = *(LODChildDesc**)(region + 0x1144);
        s32 count = *(s32*)(region + 0x1154);
        int seen = 0;
        for (int i = 0; i < count; i++) {
            LODChildDesc* d = &arr[i];
            if (d->field_0x30 & 0x10000) {
                func_8047133C__Q23LOD9LODMemManFv(
                    (LOD::LODMemMan*)((u8*)self + 0xCC), d, (u8*)mtx, g);
                seen++;
            }
        }
        bss68[g] = seen ? 2 : 0;
        bss74[g] = seen ? 2 : 0;
    }
}

// ---------------------------------------------------------------------------
// func_804709FC: resolve the current view frame and, when the caller passes a
// nonzero task, push the layer scale and the frame data into the sub-manager
// (func_8046DD9C is skipped while the scene gate byte is set).
// ---------------------------------------------------------------------------
void func_804709FC__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int task) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    CView* view = CView::getCurrentView();
    LODViewFrame* frame = func_8049626C(func_8049698C(), view);
    LOD::LODMemMan* sub = ((LODSubMgrView*)self)->field_0xF0;
    if (task != 0) {
        if (((LODScnGate*)func_8048ECD0(func_8049698C()))->field_0x19 == 0) {
            func_8046DD9C__Q23LOD9LODMemManFv(sub, frame->field_0x9C, frame->field_0x1E0);
        }
        func_8046E1DC__Q23LOD9LODMemManFv(sub, frame->field_0x9C, task);
    }
}

// ---------------------------------------------------------------------------
// func_80470A90: when `param` is nonzero and the current scene's gate byte
// is set, resolve the current view frame and hand it (+0x9C) to the +0xF0
// sub-manager's func_8046E1DC along with `param`.
// ---------------------------------------------------------------------------
extern "C" void func_80470A90__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int param) {
    if (param == 0) {
        CScn* scn = func_8049698C();
        CScn* scn2 = (CScn*)func_8048ECD0(scn);
        if (((LODScnGate*)scn2)->field_0x19 != 0) {
            return;
        }
        CView* view = CView::getCurrentView();
        CScn* scn3 = func_8049698C();
        LODViewFrame* vf = func_8049626C(scn3, view);
        LOD::LODMemMan* sub = *(LOD::LODMemMan**)((u8*)self + 0xF0);
        func_8046E1DC__Q23LOD9LODMemManFv(sub, (u8*)vf + 0x9C, param);
    }
}

void LOD::LODMemMan::func_80470B10() {}

extern "C" int func_80470EF8__Q23LOD9LODMemManFv(
    LOD::LODMemMan* self, CScn* archive, nw4r::g3d::ResFile* resFile,
    u32 id, int mdlIdx, const f32* mtxSrc);

// ---------------------------------------------------------------------------
// func_80470DB0: thunk into func_80470EF8.  The retail body is a pure tail
// call that repacks the (mdlIdx, packed) pair into the callee's id: the
// callee's resFile is this+4, its id is `(packed >> 16) | mdlIdx` (the high
// 16 bits of packed moved into the low half, OR'd with the index), its
// mdlIdx is passed through unchanged, and its mtxSrc is the 5th arg.
// ---------------------------------------------------------------------------
__declspec(noinline) int func_80470DB0__Q23LOD9LODMemManFv(
    LOD::LODMemMan* self, CScn* archive, u32 mdlIdx, u32 packed,
    const f32* mtxSrc)
{
    return func_80470EF8__Q23LOD9LODMemManFv(
        self, archive, (nw4r::g3d::ResFile*)((u8*)self + 4),
        ((u16)packed << 16) | mdlIdx, mdlIdx, mtxSrc);
}

// ---------------------------------------------------------------------------
// func_80470DCC: resolve a ResFile slot for `id`.  A slot whose id already
// matches is marked in-use and its index returned.  Otherwise the id's pool
// block is fetched via func_80471BF4 and a free slot (id < 0) is claimed:
// the slot stores the block pointer, sets its id/flags, computes the aligned
// ResFile data end (block + block[+0x14]) and runs ResFile::Init on it.
// ---------------------------------------------------------------------------
extern "C" s32 func_80470DCC__Q23LOD9LODMemManFv(
    LOD::LODMemMan* self, LOD::LODMemMan* pool, s32 id, s32* out)
{
    LODResFileSlotArray* arr = (LODResFileSlotArray*)self;
    s32 result = -1;

    for (int i = 0; i < 16; i++) {
        if (arr->mSlots[i].field_0x10 == id) {
            result = i;
            arr->mSlots[i].field_0x12 |= 1;
            break;
        }
    }

    if (result < 0) {
        u8* block = func_80471BF4__Q23LOD9LODMemManFv(pool, id);
        if (block != 0) {
            for (int j = 0; j < 16; j++) {
                if (arr->mSlots[j].field_0x10 >= 0) {
                    continue;
                }
                result = j;
                arr->mSlots[j].field_0x10 = (s16)id;
                arr->mSlots[j].field_0xC = (u32)block;
                arr->mSlots[j].field_0x12 |= 1;
                u32 end = (u32)block + *(u32*)((u8*)block + 0x14);
                if (end & 0x1F) {
                    nw4r::db::Panic(lbl_eu_8052637C, 0x3c, lbl_eu_80526354);
                }
                arr->mSlots[j].field_0x8 = end;
                ((nw4r::g3d::ResFile*)&arr->mSlots[j].field_0x8)->Init();
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
// func_80470EF8: bind a ResMdl to one of the 16 g3d slots at +0xC8.  A slot
// already holding the requested id just gets its busy bit set and returns 1;
// otherwise the first free slot constructs a ScnMdl from the ResFile model
// (through the scene allocator), notifies the g3d manager via its vtable+0x34
// callback, installs the placement matrix (column 3 from the first three
// floats, the 3x3 from the rest) and records the id.
// ---------------------------------------------------------------------------
int func_80470EF8__Q23LOD9LODMemManFv(
    LOD::LODMemMan* self, CScn* archive, nw4r::g3d::ResFile* resFile,
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
                    (MEMAllocator*)func_8048ECE4(archive), &p, resMdl, 0, 2));
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
// func_8047108C: walk the 16 g3d slots at +0xC8, destroying objects flagged
// done (bit 2), notifying the manager (vtable+0x3C) for pending ones, and
// clearing a per-slot busy bit.  Then walk the 16 child entries at +0xC and
// reset any active child through func_80471CC4.  `self` is the g3d-slot
// region base (object + 0xCC); `sub` is the +0xA44 sub-manager.
// ---------------------------------------------------------------------------
void func_8047108C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LOD::LODMemMan* sub) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    for (int i = 0; i < 16; i++) {
        LODG3dSlot* slot = &l->field_0xC8[i];
        if (slot->field_0x0 != 0) {
            u16 flags = slot->mFlags;
            if (flags & 4) {
                slot->field_0x0->Destroy();
                slot->field_0x0 = 0;
                slot->mFlags = 0;
            } else {
                if (flags & 1) {
                    slot->mFlags = flags & ~2;
                } else {
                    slot->mFlags = flags | 4;
                    ((LODG3dMgrVt*)l->field_0x0)->vf0D(slot->field_0x0);
                }
                slot->mFlags &= ~1;
            }
        }
    }
    LODChildEntryView* ev = (LODChildEntryView*)self;
    for (int i = 0; i < 16; i++) {
        LODChildEntry* e = &ev->mEntries[i];
        if (e->field_0x0 != 0) {
            if (!(e->field_0x6 & 1)) {
                sub->func_80471CC4();
                e->field_0x0 = 0;
                e->field_0x4 = 0xFFFF;
            }
            e->field_0x6 = 0;
        }
    }
}

// ---------------------------------------------------------------------------
// func_80471184: destroy all 16 g3d object slots (notifying the manager via
// the vtable+0x3C callback first) and then the manager object itself.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// func_80471184: notify the g3d manager and destroy every live object slot,
// then the manager object itself.  Auto-inline is off so callers emit a real
// `bl func_80471184__...` (retail keeps the call).
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
void func_80471184__Q23LOD9LODMemManFv(LOD::LODMemMan* self) {
    int i = 0;
    LODMemManLayout* l = (LODMemManLayout*)self;
    while (i < 16) {
        if (l->field_0xC8[i].field_0x0) {
            ((LODG3dMgrVt*)l->field_0x0)->vf0D(l->field_0xC8[i].field_0x0);
            l->field_0xC8[i].field_0x0->Destroy();
            l->field_0xC8[i].field_0x0 = 0;
        }
        i++;
    }
    if (l->field_0x0) {
        l->field_0x0->Destroy();
        l->field_0x0 = 0;
    }
}
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_80471224: notify the g3d manager (vtable+0x3C) about every live object
// slot, then - when a scene is supplied - remove the manager object from the
// scene's root group (index 7).  Auto-inline is off so callers emit a real
// `bl func_80471224__...` (retail keeps the call, and the loop body would
// otherwise be inlined into func_8046DAC0 / the destructor).
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
void func_80471224__Q23LOD9LODMemManFv(LOD::LODMemMan* self, CScn* scene) {
    int i = 0;
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x0) {
        while (i < 16) {
            if (l->field_0xC8[i].field_0x0) {
                ((LODG3dMgrVt*)l->field_0x0)->vf0D(l->field_0xC8[i].field_0x0);
            }
            i++;
        }
    }
    if (scene) {
        if (l->field_0x0) {
            ((nw4r::g3d::ScnGroup*)func_8048EC14(scene, 7))
                ->Remove((nw4r::g3d::ScnObj*)l->field_0x0);
        }
    }
}
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_804712E0: zero the used-mask words of the three child records
// (offsets 0x408, 0x694, 0x920 - three words each).
// ---------------------------------------------------------------------------
void LOD::LODMemMan::func_804712E0() {
    u32* base = (u32*)((u8*)this + 0x408);
    base[0] = 0;
    base[1] = 0;
    base[2] = 0;
    base[0x28C / 4] = 0;
    base[0x28C / 4 + 1] = 0;
    base[0x28C / 4 + 2] = 0;
    base[0x518 / 4] = 0;
    base[0x518 / 4 + 1] = 0;
    base[0x518 / 4 + 2] = 0;
}

// ---------------------------------------------------------------------------
// func_8047130C: initialise the light colour at +0x92C to zero (retail keeps
// a stack zero word passed by address).
// ---------------------------------------------------------------------------
void LOD::LODMemMan::func_8047130C() {
    GXColor zero = {0, 0, 0, 0};
    GXInitLightColor((GXLightObj*)((u8*)this + 0x92C), zero);
}

// ---------------------------------------------------------------------------
// Per-child record.  The LODMemMan is treated as an array of 0x28c-strided
// records (func_8047133C indexes `self + index*0x28c`).  Each record holds 8
// 0x40-byte slots (+0x188) feeding func_804C09E8, 8 float-word groups
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
// func_8047133C: register a new slot on the 0x28c-strided child record.  A
// 3/4-kind descriptor flags the shared .bss array instead of the record's
// used-mask.  The slot's four direction words are stored as raw float bits
// (the scale multiplies all but the last), the slot record is handed to
// func_804C09E8, and the done-mask bit is set before the slot index advances.
// (retail keeps the kind test as two compares; the goto chain reproduces it.)
// ---------------------------------------------------------------------------
void func_8047133C__Q23LOD9LODMemManFv(LOD::LODMemMan* self, LODChildDesc* desc, u8* unused, int index) {
    LODChildRec* child = (LODChildRec*)((u8*)self + index * 0x28c);
    if (child->field_0x408 >= 8) return;
    LODChild188* p = (LODChild188*)((u8*)child + 0x188);
    if (desc->field_0x34 == 3) goto kind34;
    if (desc->field_0x34 != 4) goto otherKind;
kind34:
    lbl_eu_80658368[index] = 1;
    goto floatsDone;
otherKind:
    p->field_0x284 |= 1 << p->field_0x280;
floatsDone:
    u32* dst = p->mSlotFloats[p->field_0x280];
    f32 s = desc->field_0x38;
    u32 w0, w1, w2, w3;
    *(f32*)&w0 = desc->field_0x10 * s;
    *(f32*)&w1 = desc->field_0x14 * s;
    *(f32*)&w2 = desc->field_0x18 * s;
    *(f32*)&w3 = desc->field_0x1C;
    dst[0] = w0;
    dst[1] = w1;
    dst[2] = w2;
    dst[3] = w3;
    func_804C09E8((u8*)desc, p->mSlotData[p->field_0x280]);
    p->field_0x288 |= 1 << p->field_0x280;
    p->field_0x280++;
}

// ---------------------------------------------------------------------------
// func_80471450: store the halfword param into the +0x96C float via the
// GQR5 s16 fast cast (retail psq_l f0, 0(r4), 1, qr5 after an sth spill).
#pragma push
#pragma auto_inline off
#include <revolution/os/OSFastCast.h>
#pragma pop
void func_80471450__Q23LOD9LODMemManFv(LOD::LODMemMan* self, s16 value) {
    *(f32*)((u8*)self + 0x96C) = __OSs16tof32(&value);
}

// ---------------------------------------------------------------------------
// func_8047146C: initialise the high offset scale pair (+0x96C/+0x970) and
// the +0x974 index to -1 (retail store order: 0x970, 0x96C, 0x974).
// ---------------------------------------------------------------------------
void LOD::LODMemMan::func_8047146C() {
    *(f32*)((u8*)this + 0x970) = lbl_eu_8066A718;
    *(f32*)((u8*)this + 0x96C) = lbl_eu_8066A718;
    *(s32*)((u8*)this + 0x974) = -1;
}

// ---------------------------------------------------------------------------
// func_80471484: (re)load the child record's GX light objects.  On a new
// child id the un-used slots are loaded first; then every used slot's colour
// is recomputed from its direction floats (base scale * clamp limit, capped)
// and the light object is re-initialised and loaded.  The manager keeps the
// last-bound child id and the stored scale, so an unchanged repeat load is a
// no-op.
// ---------------------------------------------------------------------------
void func_80471484__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int childIdx) {
    if (childIdx == -1) return;
    LODChildRec* child = (LODChildRec*)((u8*)self + childIdx * 0x28c);
    LODLight188* p = (LODLight188*)((u8*)child + 0x188);
    LODLightMgr* m = (LODLightMgr*)self;
    if (child->field_0x408 == 0) return;
    if (childIdx == m->field_0x974) {
        if (m->field_0x970 == m->field_0x96C) return;
    } else {
        int i;
        for (i = 0; i < p->field_0x280; i++) {
            if (!(p->field_0x284 & (1 << i))) {
                GXLoadLightObjImm(&p->mSlotData[i], (GXLightID)(1 << i));
            }
        }
    }
    m->field_0x974 = childIdx;
    m->field_0x970 = m->field_0x96C;
    f32 limit = lbl_eu_8066A720;
    GXColor baseColor = lbl_eu_8066A71C;
    int i;
    for (i = 0; i < p->field_0x280; i++) {
        if (p->field_0x284 & (1 << i)) {
            f32 base = m->field_0x96C;
            f32 c0 = base * (limit * p->mSlotFloats[i][0]);
            f32 c1 = base * (limit * p->mSlotFloats[i][1]);
            f32 c2 = base * (limit * p->mSlotFloats[i][2]);
            if (c0 > limit) c0 = limit;
            if (c1 > limit) c1 = limit;
            if (c2 > limit) c2 = limit;
            GXColor color = baseColor;
            color.r = (u8)c0;
            color.g = (u8)c1;
            color.b = (u8)c2;
            GXInitLightColor(&p->mSlotData[i], color);
            GXLoadLightObjImm(&p->mSlotData[i], (GXLightID)i);
        }
    }
}

// ---------------------------------------------------------------------------
// func_8047163C: walk the 0x20-strided block list and carve a free block of
// `size` (+0x20 header) out of the first block whose flag bit 0 is set and
// whose capacity fits.  The block is marked, its tail split off as a new
// block when at least 10240 bytes remain, and a pointer past the header is
// returned.
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
extern "C" u8* func_8047163C__Q23LOD9LODMemManFv(LODPoolBlock* self, u32 size, int id) {
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
                return (u8*)cur + 32;
            }
        }
        cur = cur->mNext;
    }
    return 0;
}
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_804716B8: walk the element list (head at +0x8) and merge the element
// whose +0x20 pointer equals `target` into the previous element: the flag
// word is cleared when there is no previous element, otherwise (previous
// element's flag bit 0 set) the previous element absorbs the target's +0x4
// value and skips it via +0x8.
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
extern "C" void func_804716B8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u32 target) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    u32* cur = (u32*)self;
    u32* prev = 0;
    while (cur) {
        if ((u32)cur + 0x20 == target) {
            cur[0] = 0;
            if (prev == 0) {
                return;
            }
            if ((prev[0] & 1) != 0) {
                return;
            }
            // Read the merge pair in retail load order (cur[1] before prev[1]):
            // the volatile pins the source order (MWCC_CASES sfmps_pesfn
            // load-pair rule — one-time state reads, no aliasing writes in
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
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_80471718: walk the 0x20-strided block list and return the first block
// whose flag bit 0 is set and whose id matches, or null.
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
extern "C" LODPoolBlock* func_80471718__Q23LOD9LODMemManFv(LODPoolBlock* self, int id) {
    LODPoolBlock* cur = self;
    while (cur) {
        if ((cur->mFlags & 1) && id == (int)cur->mId) {
            return cur;
        }
        cur = cur->mNext;
    }
    return 0;
}
#pragma scheduling on
#pragma pop

#pragma push
#pragma auto_inline off
// ---------------------------------------------------------------------------
// func_80471748: walk the 0x20-strided block list; the block whose data area
// (+0x20) matches `target` and whose flag bit 0 is set gets bit 1 set.
// ---------------------------------------------------------------------------
extern "C" void func_80471748__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u32 target) {
    LODPoolBlock* cur = (LODPoolBlock*)self;
    while (cur) {
        if ((cur->mFlags & 1) && (u32)cur + 0x20 == target) {
            cur->mFlags |= 2;
            return;
        }
        cur = cur->mNext;
    }
}
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_80471780: initialise the element-list head/tail links and buffer slot.
// Auto-inline off so callers emit a real tail call (retail `b`).
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
extern "C" void func_80471780__Q23LOD9LODMemManFv(LOD::LODMemMan* self, void* arg) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    l->field_0x0 = 0;
    l->field_0x8 = 0;
    l->field_0x4 = (LODElem20*)arg;
}
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_80471794: release the shared persistent buffer (lbl_eu_80665830) if
// present (retail keeps a redundant inner null-check doubling the beq), then
// allocate a fresh 32-aligned block from `handle` into it.
// ---------------------------------------------------------------------------
extern "C" void func_80471794__Q23LOD9LODMemManFv(u32 handle, u32 size) {
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
// func_804717FC: release the shared persistent buffer (lbl_eu_80665830) and
// clear it (retail keeps a redundant inner null-check doubling the beq).
// ---------------------------------------------------------------------------
void func_804717FC__Q23LOD9LODMemManFv() {
    int old = lbl_eu_80665830;
    if (old != 0) {
        if (old != 0) {
            mtl::MemManager::deallocate((void*)old);
        }
    }
    lbl_eu_80665830 = 0;
}

// ---------------------------------------------------------------------------
// func_80471834: (re)initialise the element buffer.  The secondary buffer
// (+0x8) is released first, then - when `size` is nonzero - the element
// buffer (+0x4) is either adopted from the shared persistent buffer
// (lbl_eu_80665830) or freshly allocated from `handle`; a zero size only
// releases the element buffer.  The tail re-seeds the name string and flags
// exactly like func_80471938.
// ---------------------------------------------------------------------------
void func_80471834__Q23LOD9LODMemManFv(LOD::LODMemMan* self, u32 handle, u32 size) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x8) {
        if (l->field_0x8) {
            mtl::MemManager::deallocate(l->field_0x8);
            l->field_0x8 = 0;
        }
    }
    if (size != 0) {
        if (lbl_eu_80665830 != 0) {
            l->field_0x4 = (LODElem20*)lbl_eu_80665830;
        } else {
            if (l->field_0x4) {
                if (l->field_0x4) {
                    mtl::MemManager::deallocate(l->field_0x4);
                    l->field_0x4 = 0;
                }
            }
            l->field_0x4 = (LODElem20*)mtl::MemManager::allocate_head(handle, size, 0x20);
        }
    } else {
        if (l->field_0x4) {
            if (l->field_0x4) {
                mtl::MemManager::deallocate(l->field_0x4);
                l->field_0x4 = 0;
            }
        }
    }
    l->field_0xC = 0;
    l->field_0x20 = 1;
    l->mView_1C = (u8*)size;
    l->field_0x30 = 0;
    const char* name = lbl_eu_80523D90;
    l->field_0x74 = strlen(name);
    strcpy(l->mStr_34, name);
}

// ---------------------------------------------------------------------------
// func_80471938: reset the manager.  The element buffers are released back to
// the MemManager only while the global reset flag is clear (retail keeps a
// redundant inner null-check that MWCC emits as a doubled beq), the file
// handle is cancelled, and the reset-time name string is re-seeded into +0x34
// with its length recorded at +0x74.
// ---------------------------------------------------------------------------
void LOD::LODMemMan::func_80471938() {
    LODMemManLayout* l = (LODMemManLayout*)this;
    if (lbl_eu_80665830 == 0) {
        if (l->field_0x4) {
            if (l->field_0x4) {
                mtl::MemManager::deallocate(l->field_0x4);
                l->field_0x4 = 0;
            }
        }
    } else {
        l->field_0x4 = 0;
    }
    if (l->field_0x8) {
        if (l->field_0x8) {
            mtl::MemManager::deallocate(l->field_0x8);
            l->field_0x8 = 0;
        }
    }
    if (l->field_0x30) {
        CDeviceFile::cancel(l->field_0x30);
        l->field_0x30 = 0;
    }
    l->field_0xC = 0;
    l->field_0x20 = 0;
    const char* name = lbl_eu_80523D90;
    l->field_0x74 = strlen(name);
    strcpy(l->mStr_34, name);
}

// ---------------------------------------------------------------------------
// func_804719FC: cancel the file handle at +0x30, clear flag bits of +0x20
// (mask 0xFFFCFF8D), then release the +0x8 buffer (retail keeps a redundant
// inner null-check doubling the beq).
// ---------------------------------------------------------------------------
#pragma push
#pragma auto_inline off
void LOD::LODMemMan::func_804719FC() {
    LODMemManLayout* l = (LODMemManLayout*)this;
    if (l->field_0x30) {
        CDeviceFile::cancel(l->field_0x30);
        l->field_0x30 = 0;
    }
    l->field_0x20 &= 0xFFFCFF8D;
    if (l->field_0x8) {
        if (l->field_0x8) {
            mtl::MemManager::deallocate(l->field_0x8);
            l->field_0x8 = 0;
        }
    }
}
#pragma scheduling on
#pragma pop

// ---------------------------------------------------------------------------
// func_80471A70: clear flag bit 2 of the +0x20 word, then re-seed the reset-
// time name string: store strlen(lbl_eu_80523D90) into +0x74 and copy the
// string into +0x34 (retail hoists the string address into r31 up front).
// ---------------------------------------------------------------------------
void LOD::LODMemMan::func_80471A70() {
    const char* name = lbl_eu_80523D90;
    LODMemManLayout* l = (LODMemManLayout*)this;
    l->field_0x20 &= ~4u;
    l->field_0x74 = strlen(name);
    strcpy(l->mStr_34, name);
}

// ---------------------------------------------------------------------------
// func_80471ACC: (re)allocate the object's secondary buffer to hold `count20`
// 0x20-byte elements, `count48` 0x48-byte descriptors and a count4*4-byte
// table (0x20-aligned), followed by `count18` 0x18-byte records.  The old
// buffer is released first; the output pointers receive the base and the
// sub-region offsets.
// ---------------------------------------------------------------------------
void func_80471ACC__Q23LOD9LODMemManFv(
    LOD::LODMemMan* self, u32 count20, u32 count18, u32 count48, u32 count4,
    u8** outBase, u8** out18, u8** out48, u8** out4)
{
    LODMemManLayout* l = (LODMemManLayout*)self;
    u32 size20 = count20 * 0x20;
    u32 size48 = count48 * 0x48;
    u32 base = size20 + size48 + count4 * 4;
    base += 0x20 - (base & 0x1F);
    u32 offset18 = base;
    base += count18 * 0x18;
    base += 0x20 - (base & 0x1F);
    u8* old = l->field_0x8;
    if (old != 0) {
        if (old != 0) {
            mtl::MemManager::deallocate(old);
            l->field_0x8 = 0;
        }
    }
    l->field_0x8 = (u8*)mtl::MemManager::allocate_head(
        mtl::MemManager::getHandleMEM1(), base, 0x20);
    memset(l->field_0x8, 0, base);
    *outBase = l->field_0x8;
    if (count48 != 0) *out48 = l->field_0x8 + size20;
    if (count4 != 0) *out4 = l->field_0x8 + size20 + size48;
    *out18 = l->field_0x8 + offset18;
}

// ---------------------------------------------------------------------------
// func_80471BC8: when the +0x4 buffer is present, adjust the +0xC and +0x18
// offsets by `delta` and tail-call func_80471780 with the new base.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// func_80471BC8: when the +0x4 buffer is present, adjust the +0xC and +0x18
// offsets by `delta` (retail: r4 = self->field_0x1C reload, r6 = delta; r5 is
// an unused caller-leftover) and tail-call func_80471780 with the new base
// and the remaining size.
// ---------------------------------------------------------------------------
void func_80471BC8__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int unk, int unused, int delta) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    if (l->field_0x4 != 0) {
        int rem = (int)l->mView_1C - delta;
        l->field_0xC = (u8*)l->field_0x4 + delta;
        l->mCount_18 = rem;
        func_80471780__Q23LOD9LODMemManFv(
            (LOD::LODMemMan*)((u8*)l->field_0x4 + delta), (void*)rem);
    }
}

// ---------------------------------------------------------------------------
// func_80471BF4: find the pool block tagged with `id`.  A ready block
// (flag bit 1) returns its data area; an allocated-but-unready block returns
// null.  When no block exists yet, a new one is carved from the pool with the
// size recorded in the field_0x4 size table and the bookkeeping stored in
// field_0x24..0x2C.
// ---------------------------------------------------------------------------
u8* func_80471BF4__Q23LOD9LODMemManFv(LOD::LODMemMan* self, int id) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    LODPoolBlock* block = func_80471718__Q23LOD9LODMemManFv((LODPoolBlock*)l->field_0xC, id);
    if (block != 0) {
        if (block->mFlags & 2) return (u8*)block + 0x20;
        return 0;
    }
    if (l->field_0x20 & 0xE0) return 0;
    LODBuf74* buf = (LODBuf74*)l->field_0x4;
    u32* arr = buf->mPairs;
    u8* data = func_8047163C__Q23LOD9LODMemManFv((LODPoolBlock*)l->field_0xC, arr[id*2+1], id);
    if (data != 0) {
        l->field_0x24 = (u32)data;
        l->field_0x28 = arr[id*2];
        l->field_0x2C = arr[id*2+1];
        l->field_0x20 |= 0x40;
    }
    return 0;
}

// retail: lwz r3,0xC(r3); b func_804716B8 -- delegates to the sub-manager stored at +0xC
void LOD::LODMemMan::func_80471CC4() {
    (*(LOD::LODMemMan**)((u8*)this + 0xC))->func_804716B8();
}

void LOD::LODMemMan::func_80471CCC() {
    LODMemManLayout* l = (LODMemManLayout*)this;
    l->field_0x20 |= 0x40000000;
}

// CDeviceFile archive header read through LODMemMan.field_0x4 (func_80471CDC).
struct LODArchiveHdr {
    u32 mMagic;      // 0x00 "LODI"
    u32 mVersion;    // 0x04
    u8  mPad_08[0x14 - 0x08];
    u32 mSize;       // 0x14
};

// ---------------------------------------------------------------------------
// func_80471CDC: drive the archive load state machine via the field_0x20 flag
// bits.  Each of the three phases (data/file/entry) is gated by a bit pair;
// when the archive-file bit is clear the object issues a read job through
// CDeviceFile and marks the bit, and when both bits are set it advances the
// phase mask instead.
// ---------------------------------------------------------------------------
void func_80471CDC__Q23LOD9LODMemManFv(LOD::LODMemMan* self) {
    LODMemManLayout* l = (LODMemManLayout*)self;
    u32 flags = l->field_0x20;
    if (flags & 0x80000000u) return;
    if (flags & 0x40000000u) return;
    if (!(flags & 0x70)) return;
    if (flags & 0x10) {
        if (flags & 0x10000) {
            if (flags & 0x20000) {
                l->field_0x20 = (flags & ~0x30010u) | 0x20;
            }
        } else {
            l->field_0x20 = flags | 0x10000;
            l->field_0x30 = CDeviceFile::readCommonArchiveFile(
                (mtl::ALLOC_HANDLE)l->field_0x4, l->mStr_34, (IWorkEvent*)self, 0, 0xC00);
        }
    }
    flags = l->field_0x20;
    if (flags & 0x20) {
        if (flags & 0x10000) {
            if (flags & 0x20000) {
                l->field_0x20 = (flags & ~0x30020u) | 0x6;
            }
        } else {
            l->field_0x20 = flags | 0x10000;
            LODArchiveHdr* hdr = (LODArchiveHdr*)l->field_0x4;
            if (hdr->mMagic == 0x4C4F4449u && hdr->mVersion >= 0x3ED) {
                l->field_0x30 = CDeviceFile::readCommonArchiveFile(
                    (mtl::ALLOC_HANDLE)((u8*)hdr + 0xC00), l->mStr_34, (IWorkEvent*)self,
                    0xC00, hdr->mSize - 0xC00);
            } else {
                if (l->field_0x30) {
                    CDeviceFile::cancel(l->field_0x30);
                    l->field_0x30 = 0;
                }
                l->field_0x20 &= ~0x30072u;
                if (l->field_0x8) {
                    if (l->field_0x8) {
                        mtl::MemManager::deallocate(l->field_0x8);
                        l->field_0x8 = 0;
                    }
                }
            }
        }
    }
    flags = l->field_0x20;
    if (flags & 0x40) {
        if (flags & 0x10000) {
            if (flags & 0x20000) {
                l->field_0x20 = flags & ~0x30040u;
                func_80471748__Q23LOD9LODMemManFv((LOD::LODMemMan*)l->field_0xC, l->field_0x24);
            }
        } else {
            l->field_0x20 = flags | 0x10000;
            l->field_0x30 = CDeviceFile::readCommonArchiveFile(
                l->field_0x24, l->mStr_34, (IWorkEvent*)self, l->field_0x28, l->field_0x2C);
        }
    }
}

bool LOD::LODMemMan::OnFileEvent(CEventFile* evt) {
    // File-event hook: when the event carries the currently adopted file
    // handle, clear the handle and set the in-use flag on the existing word.
    LODMemManLayout* l = (LODMemManLayout*)this;
    CFileHandle* current = l->field_0x30;
    if (current == evt->mFileHandle) {
        u32 flags = l->field_0x20;
        l->field_0x30 = 0;
        l->field_0x20 = flags | 0x20000;
    }
    return true;
}

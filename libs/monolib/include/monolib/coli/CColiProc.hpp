#pragma once

#include <types.h>
#include <nw4r/math/math_types.h>

// Collision-procedure class hierarchy (monolib/src/coli CColiProc unit).
//
// Retail shape, read off the DOL tables:
//   .data 0x8056F380 __vt__12CColiObjCall  {__RTTI__12CColiObjCall, 0, Update__12CColiObjCallFv}
//   .data 0x8056F398 __vt__9CColiProc      {__RTTI__9CColiProc, 0, Update__9CColiProcFv}
//   .data 0x8056F3A8 __vt__12CColiResCall  {__RTTI__12CColiResCall, 0, Update__12CColiResCallFv}
//   both base lists are {__RTTI__9CColiProc, 0, 0} => single inheritance, one
//   virtual slot each, so the hierarchy is
//
//       class CColiProc      0x38, vtable + slot0 (empty default)
//       class CColiObjCall   0x48, overrides slot0
//       class CColiResCall   0x50, overrides slot0
//
// Sizes come from the two static instances in .bss (0x8065D0A0 = 0x48,
// 0x8065D0E8 = 0x50) and from the base sub-object at +0x38 that the +0x44
// matrix-source/target slots of the res object follow.
//
// The /proc* helpers in CColiProc.cpp do NOT operate on this class: they fill
// a <=0x10-byte stack work object {target, a, b, flags} (their first store is
// at +0x00, i.e. it would overwrite a CColiProc vptr), so they are members of
// CColiWork below.

struct CColiProcTarget {
    u32 field_0x0;
    u16 field_0x4;
    u16 field_0x6;
};

// Matrix source a res object tracks: ColiMoverUpdate reads cur.m0 (the world
// matrix) and old.m1 (the previous inverse), while the matrix-history helpers
// in code_804B2FF0 shift cur into old. CObjMtx keeps the word-copy shape those
// helpers use. Defined here once so both units see the same real type.
struct CObjMtx {
    f32 m[3][4];
};

struct CColiMtxPair {
    CObjMtx m0; //0x0
    CObjMtx m1; //0x30
};

struct CColiMtxOut {
    CColiMtxPair cur; //0x0
    CColiMtxPair old; //0x60
};

class CColiProc {
public:
    CColiProc();
    virtual void Update(); // slot 0 (empty default: retail ColiNoop)

    u32 field_0x4;                 // +0x04 zeroed by the ctor
    u32 field_0x8;                 // +0x08
    nw4r::math::VEC3 pos;          // +0x0C mover position
    u8 field_0x18[0x24 - 0x18];    // +0x18
    nw4r::math::VEC3 off;          // +0x24 mover offset
    u8 field_0x30[0x38 - 0x30];    // +0x30
};

// Shared 0.0f SDA2 pool entry the retail code refers to (its @sda21 relocs
// name this symbol rather than an anonymous literal).
extern const f32 lbl_eu_8066AE88;

class CColiObjCall : public CColiProc {
public:
    // Retail static instance (lbl_eu_8065D0A0) is built by this inline ctor:
    // base ctor, own vptr, then +0x38/+0x3C/+0x40 cleared and the +0x3C
    // scalar set to the pooled 0.0f constant.
    CColiObjCall() : field_0x38(0), field_0x3c(lbl_eu_8066AE88), field_0x40(0) {}
    virtual void Update(); // +0x38 accumulator, +0x3C scale, +0x40 flag

    nw4r::math::VEC3* field_0x38;  // +0x38 accumulator vector
    f32 field_0x3c;                // +0x3C scale
    u8 field_0x40;                 // +0x40 flag (overwrite acc.y with the pool constant)
    u8 field_0x41[0x48 - 0x41];    // +0x41
};

class CColiResCall : public CColiProc {
public:
    virtual void Update(); // ColiMoverUpdate
    nw4r::math::VEC3 acc;          // +0x38 accumulated displacement
    CColiMtxOut* src;              // +0x44 matrix source (cleared by the walkers)
    void* target;                  // +0x48 object the points are resolved against
    u32 field_0x4c;                // +0x4C
};

// 0x10-byte stack work object the /proc entry walkers fill in and classify.
class CColiWork {
public:
    void InitTriple(CColiProcTarget* target, u32 a);
    void InitQuad(CColiProcTarget* target, u32 a, u32 b);
    bool IsTargetFlagged();
    bool TestBit18State();

    CColiProcTarget* field_0x0; // +0x00 target data
    u32 field_0x4;              // +0x04
    u32 field_0x8;              // +0x08
    u32 field_0xC;              // +0x0C flags (bit 18 set by ColiProc_ClassifyEntries)
};

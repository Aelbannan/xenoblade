#pragma once

#include <types.h>
#include <revolution/gx/GXTypes.h>  // GXColor (lbl_eu_8066AFE0 fog color word)
#include "monolib/scn/CLight.hpp"
#include "libs/monolib/src/scn/CScnLightMan.hpp"  // CScnLightMan (func_804C172C light-manager slot)

class CScn;  // full definition in monolib/scn/CScn.hpp (CScnItemModelNw4r.hpp uses CScn*)

// Forward decl for the ctor-view union member (defined below).
struct CScnEnvLgtCtrlLgtGate;

// Cross-TU light types referenced by the CScnEnvLgtCtrl vtable interface and
// the func_804C1338 light-env dispatch (full definitions in their headers).
class CScnVirtualLight;      // CScnVirtualLight.hpp
class CLightEnv;             // CVirtualLightObj.hpp
struct CScnVirtualLightSub;  // CScnVirtualLight.hpp
namespace ml { struct CVec3; struct CVec4; }  // monolib/math

// 0x30-byte light-slot entry in the array at CScnEnvLgtCtrl+0x24.
// func_804C5F6C / 5FB0 / 6010 poke the u16 word at +0x28 (control flags:
// bit 0 and bit 2 cleared/set there) and the three u16 params at +0x2A..+0x2E.
// 12-byte vec3 used by the light-slot entries; copied wholesale by
// func_804C5E04 (word moves - retail treats the triple as u32 words).
struct CScnEnvLgtCtrlLgtVec3 {
    u32 x;  // +0x00
    u32 y;  // +0x04
    u32 z;  // +0x08
};

// Plain 3-float triple: assigning whole objects of this type makes MWCC
// move the three words with lwz/stw pairs (func_804C43A4's out/center rows).
struct CScnEnvLgtCtrlV3 {
    union {
        f32 e[3];   // indexable view (loop-driven updates stay memory-resident)
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
    };
};

// Float vec3 used as the scaling temp in func_804C3F58's two branches.
struct CScnEnvLgtCtrlLgtVec3f {
    f32 x;  // +0x00
    f32 y;  // +0x04
    f32 z;  // +0x08
};

// 4-float output written by func_804C3F58: the scaled vec3 in the first
// three words plus the shared .sdata2 constant in the +0x0C word.
struct CScnEnvLgtCtrlLgtVec4 {
    f32 x;  // +0x00
    f32 y;  // +0x04
    f32 z;  // +0x08
    f32 w;  // +0x0C
};

struct CScnEnvLgtCtrlLgtSlot {
    CScnEnvLgtCtrlLgtVec3 field_0x00;   // +0x00
    // +0x0C (same source triple as field_0x00, func_804C5E9C): word view for
    // the bulk copies, float view for func_804C31C8's scaled reads.
    union {
        CScnEnvLgtCtrlLgtVec3 w;
        CScnEnvLgtCtrlLgtVec3f f;
    } field_0x0C;
    // +0x18 position triple: word view for the bulk copies / byte-wise writes,
    // float view for func_804C3C9C's paired-single reads.
    union {
        CScnEnvLgtCtrlLgtVec3 field_0x18;
        CScnEnvLgtCtrlLgtVec3f field_0x18f;
    };
    f32 field_0x24;                     // +0x24 (float param, func_804C5E9C)
    u16 field_0x28;                     // +0x28 control flags
    u16 field_0x2A;                     // +0x2a
    u16 field_0x2C;                     // +0x2c
    u16 field_0x2E;                     // +0x2e
};

// Vtable-view of CScnEnvLgtCtrl itself for the slot-0x70 virtual dispatch in
// func_804C30E8 (self's own vtable). Declared virtual N sits at vtable offset
// (N+2)*4 (RTTI slots 0-1), so v26 -> 0x70. The two args are the s16 field_0xBC
// (sign-extended) and a literal 0. Abstract - never instantiated.
struct CScnEnvLgtCtrlVt {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26(int a, int b) = 0;  // vtable slot 28 (0x70)
};

// Virtual-dispatch view of the light-ring items (the objects linked at
// CScnEnvLgtCtrl+0x0C). MWCC reserves vtable slots 0-1 for RTTI, so declared
// virtual N sits at slot N+2 (offset (N+2)*4): the dummies below pin slot 10
// (0x28, func_804C1600) and slot 28 (0x70, func_804C2014). Abstract on
// purpose - never instantiated in this TU.
struct CScnEnvLgtCtrlListItem {
    virtual void v0(int flag) = 0;             // vtable slot 2 (0x08) - deleting-dtor dispatch (func_804C1D7C)
    virtual void v1() = 0;
    virtual void v2(void* arg) = 0;            // vtable slot 4 (0x10)
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8(void* arg) = 0;            // vtable slot 10 (0x28)
    virtual void v9(void* arg) = 0;            // vtable slot 11 (0x2C) - env dispatch (func_804C1338)
    virtual void v10(void* arg, void* data) = 0;  // vtable slot 12 (0x30) - env + camera data (func_804C1338)
    virtual void v11(void* arg, u8 byte) = 0;  // vtable slot 13 (0x34)
    virtual int v12(void* vec) = 0;            // vtable slot 14 (0x38) - vec4 query, nonzero = apply (func_804C1338)
    virtual int v13(void* vec) = 0;            // vtable slot 15 (0x3C) - vec4 query (func_804C1338)
    virtual void v14(void* data) = 0;          // vtable slot 16 (0x40) - light-data push (func_804C1500 fwd walk)
    virtual int v15(float f, void* data, void* arg,
                    u32 count) = 0;            // vtable slot 17 (0x44) - active-light query (func_804C1500 back walk)
    virtual void v16(void* arg) = 0;           // vtable slot 18 (0x48)
    virtual void v17(void* arg) = 0;           // vtable slot 19 (0x4C)
    virtual void v18(void* arg, float f) = 0;  // vtable slot 20 (0x50)
    virtual int v19(void* bits, int flag) = 0;     // vtable slot 21 (0x54)
    virtual int v20(void* base, int flag) = 0;     // vtable slot 22 (0x58)
    virtual void v21() = 0;
    // slot 22 sits at vtable 0x60: light push with (value, mode, submode).
    // func_804C1F10 calls it as v22(f, 1, flag-1) on the active pass and
    // v22(f, 0, 0) on the idle pass.
    virtual void v22(float f, s32 mode, s32 submode) = 0;
    virtual int v23() = 0;                     // vtable slot 25 (0x64)
    virtual void v24() = 0;
    virtual void v25(int flag) = 0;            // vtable slot 27 (0x6C) - teardown pass (func_804C1D7C)
    virtual void v26(void* a, void* b) = 0;    // vtable slot 28 (0x70)
    virtual void v27(void* a, void* b, float f) = 0;  // vtable slot 29 (0x74)
    virtual int v28(void* out, int flag) = 0;  // vtable slot 30 (0x78)
    virtual int v29() = 0;                     // vtable slot 31 (0x7C) - "idle" query (func_804C1BA0)
    virtual int v30() = 0;                     // vtable slot 32 (0x80) - "in use" query (func_804C1D7C / func_804C1BA0)
    virtual void v31() = 0;                    // vtable slot 33 (0x84)
    virtual void v32() = 0;                    // vtable slot 34 (0x88)
    virtual void v33() = 0;                    // vtable slot 35 (0x8C)
    virtual void v34() = 0;                    // vtable slot 36 (0x90)
    virtual void v35() = 0;                    // vtable slot 37 (0x94)
    virtual void v36() = 0;                    // vtable slot 38 (0x98)
    virtual void v37(int flag) = 0;            // vtable slot 39 (0x9C) - teardown dispatch (func_804C1BA0)
};

// Ring-list node for the light-object list cached at CScnEnvLgtCtrl+0x0C
// (mirrors _reslist_node<T>: links at +0/+0x4, item pointer at +0x8).
struct CScnEnvLgtCtrlListNode {
    CScnEnvLgtCtrlListNode* mNext;   // +0x00
    CScnEnvLgtCtrlListNode* mPrev;   // +0x04
    CScnEnvLgtCtrlListItem* mItem;   // +0x08
};

// Flattened reslist<IScnEnvCtl> layout (retail __dt___reslist_base_IScnEnvCtl /
// __dt__reslist_IScnEnvCtl). The template names cannot be reproduced by a
// Wii/1.1 instantiation (symbolic mangling), so the dtors are spelled out
// flat; this mirrors reslist.hpp: vtable@0, mStartNodePtr@4, sentinel@8,
// mList@0x14, mCapacity@0x18, externally-owned flag@0x1C.
struct CScnEnvLgtCtrlIScnResNode {
    CScnEnvLgtCtrlIScnResNode* mNext;   // +0x00
    CScnEnvLgtCtrlIScnResNode* mPrev;   // +0x04
    void* mItem;                        // +0x08
};

struct CScnEnvLgtCtrlIScnResBase {
    void* mVtable;                              // +0x00
    CScnEnvLgtCtrlIScnResNode* mStartNodePtr;   // +0x04
    CScnEnvLgtCtrlIScnResNode mStartNode;       // +0x08 sentinel
    CScnEnvLgtCtrlIScnResNode* mList;           // +0x14 node array
    int mCapacity;                              // +0x18
    bool field_0x1C;                            // +0x1C externally-owned flag
};

struct CScnItemModelNw4rOwner;  // full definition in CScnItemModelNw4r.hpp

// Forward decls for the blend/bloom objects owned by the reslist (full
// definitions below; __dt__804C0E48 releases them through these).
struct CScnEnvLgtBlend;
struct CScnEnvLgtBloom;

// reslist<IScnEnvCtl> object constructed by __ct__reslist_IScnEnvCtl (retail
// flat name; the template cannot be instantiated under Wii/1.1's symbolic
// mangling, cf. the two reslist destructors). The object carries an 8-byte
// prefix (interface vtable slot + scene-owner pointer) ahead of the
// _reslist_base<IScnEnvCtl> subobject at +0x08, then the derived reslist
// fields at +0x28..+0x40 (the blend/bloom pair released by __dt__804C0E48).
struct CScnEnvLgtCtrlResList {
    union {
        u32 mCount;                        // +0x00 interface vtable slot / count
        u32 flags;                         // +0x00 flags view (bit 3 = blend/bloom pair, __dt__804C0E48)
    };
    CScnItemModelNw4rOwner* mOwner;    // +0x04 scene owner (func_80496018 arg)
    CScnEnvLgtCtrlIScnResBase base;    // +0x08 _reslist_base<IScnEnvCtl>
    CScnEnvLgtBlend* field_0x28;       // +0x28 blend object (released via vtable slot 2)
    CScnEnvLgtBloom* field_0x2C;       // +0x2C bloom object (main vtable at +0x80, slot 7)
    u32 field_0x30;                    // +0x30
    f32 field_0x34;                    // +0x34
    f32 field_0x38;                    // +0x38
    u32 field_0x3C;                    // +0x3C
    u32 field_0x40;                    // +0x40
};

// Default light slot values written by the reslist ctor at +0x34/+0x38.
extern const float lbl_eu_8066AFD0;
extern const float lbl_eu_8066AFD4;

// Fog-enable gate at CScnEnvLgtCtrl+0x40 (func_804C5380 / func_804C54D4):
// a u16 flags word whose bit 0 permits the fog push. func_804C406C also
// walks a 0x3C-stride entry array located at field_0x2C + mOffset with
// mCount entries.
struct CScnEnvLgtCtrlFogGate {
    u16 mFlags;    // +0x00
    u8 _02[2];
    u32 mCount;    // +0x04
    u32 mOffset;   // +0x08
};

// Full-init view of the +0x30 light-param control blob written by
// func_804C26F0 (defaults reset).
struct CScnEnvLgtCtrlParamInit {
    u32 field_0x00[4];   // +0x00 float-bit quad (AFDC)
    u8 field_0x10;       // +0x10
    u8 field_0x11;       // +0x11
    u8 _12[0xE];         // +0x12 .. +0x20
    u8 field_0x20;       // +0x20
    u8 field_0x21;       // +0x21
    u8 _22[2];
    f32 field_0x24;      // +0x24
    u8 _28[0xC];
    u32 field_0x34;      // +0x34
    u32 field_0x38;
    u32 field_0x3C;
    u32 field_0x40;
    u8 _44[0x20];
    u8 field_0x64;       // +0x64
    u32 field_0x68[4];   // +0x68 float-bit quad (AFDC)
    u32 field_0x78[3];   // +0x78 float-bit triple (B014)
    u32 field_0x84[4];   // +0x84 (1, 4, 1, 0)
};

// View of the +0x28 object poked by func_804C26F0.
struct CScnEnvLgtCtrlBits28 {
    u8 _00[0x38];
    u32 field_0x38[7];   // +0x38 float-bit septet (AFDC x2, B014, B010 x3... see func)
    u8 _54[4];
    u32 field_0x58;      // +0x58
    u32 field_0x5C;      // +0x5C
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
};

// View of the +0x2C object poked by func_804C26F0.
struct CScnEnvLgtCtrlBits2C {
    u32 field_0x00[4];   // +0x00 float-bit quad (AFDC)
    u8 field_0x10;       // +0x10
    u8 field_0x11;       // +0x11
    u8 _12[2];
    u32 field_0x14;      // +0x14
    u8 _18[8];
    u8 field_0x20;       // +0x20
    u8 field_0x21;       // +0x21
    u8 _22[2];
    f32 field_0x24;      // +0x24
    u32 field_0x28;      // +0x28
    u8 _2C[8];
    u32 field_0x34;      // +0x34
    u32 field_0x38;
    u32 field_0x3C;
    u32 field_0x40;
    u32 field_0x44;
};

// Two per-view fog parameter sets at +0xF0 (views 0 and 1), interleaved by
// attribute: color[2] @ 0xF0/0xF4, type[2] @ 0xF8/0xFC, start[2] @ 0x100/0x104,
// end[2] @ 0x108/0x10C. func_804C5380 pushes view 0 into the scene root's fog
// slot 0, func_804C54D4 view 1.
struct CScnEnvLgtCtrlFogData {
    u32 mColor[2];  // +0x00
    u32 mType[2];   // +0x08
    f32 mStart[2];  // +0x10
    f32 mEnd[2];    // +0x18
};

// 0x40-byte light-command rows indexed from CScnEnvLgtCtrl+0x20 by
// func_804C392C (index = the control's u16 at +0x1E).
struct CScnEnvLgtCtrlLgtEntry40 {
    u8 _00[0x18];      // +0x00
    f32 field_0x18;    // +0x18
    f32 field_0x1C;    // +0x1C
    f32 field_0x20;    // +0x20
    u8 _24[0x04];      // +0x24
    f32 field_0x28;    // +0x28
    f32 field_0x2C;    // +0x2C
    f32 field_0x30;    // +0x30 (func_804C3AC8)
    u32 field_0x34;    // +0x34
    u32 field_0x38;    // +0x38 (func_804C3AC8)
    u32 field_0x3C;    // +0x3C enable bit tested by func_804C678C
};

// Control blob handed to func_804C392C: u16 light index at +0x00 (tested
// against the +0x28 bit array), flags at +0x04 (bit 15 selects the entry
// path), the +0x1E entry index into the +0x20 array, a color vec3 at +0x30
// and a direction pair at +0x44..+0x4C. Overlaps the front of
// CScnEnvLgtCtrlLgtParamEntry (the same object; that struct only covers 0x24).
struct CScnEnvLgtCtrlLgtCmd {
    u16 field_0x00;    // +0x00 (retail loads with lhz)
    u16 _02;
    u32 field_0x04;    // +0x04 (bit 15 = entry path)
    u8 _08[0x16];      // +0x08 .. +0x1E
    u16 field_0x1E;    // +0x1E
    u8 _20[0x10];      // +0x20 .. +0x30
    f32 field_0x30[3]; // +0x30
    u8 _3C[0x08];      // +0x3C .. +0x44
    u32 field_0x44;    // +0x44
    f32 field_0x48;    // +0x48
    f32 field_0x4C;    // +0x4C
    u32 field_0x50;    // +0x50 (func_804C3AC8)
    f32 field_0x54;    // +0x54
    f32 field_0x58;    // +0x58
    u32 field_0x5C;    // +0x5C
    f32 field_0x60;    // +0x60
};

// +0x28 object written by func_804C2124: a 7-word float block at +0x38
// (stored as u32 bit patterns: two color words, two scaled words, three
// intensity words) plus a u32 selector at +0x5C (1 or 3).
struct CScnEnvLgtCtrlLgtSink {
    u8 _00[0x38];     // +0x00
    u32 field_0x38;   // +0x38
    u32 field_0x3C;   // +0x3C
    u32 field_0x40;   // +0x40
    u32 field_0x44;   // +0x44
    u32 field_0x48;   // +0x48
    u32 field_0x4C;   // +0x4C
    u32 field_0x50;   // +0x50
    u8 _54[0x08];     // +0x54
    u32 field_0x5C;   // +0x5C
};

// Compact header + row block built by func_804C6BA8: 12 u16 words
// (0x00..0x16), four u32 pairs (+0x18/+0x20/+0x28/+0x2C) and five 0x20-byte
// float rows at +0x38.
struct CScnEnvLgtCtrlLgtHeader {
    u16 field_0x00;    // +0x00
    u16 field_0x02;    // +0x02
    u16 field_0x04;    // +0x04
    u16 field_0x06;    // +0x06
    u16 field_0x08;    // +0x08
    u16 field_0x0A;    // +0x0A
    u16 field_0x0C;    // +0x0C
    u16 field_0x0E;    // +0x0E
    u16 field_0x10;    // +0x10
    u16 field_0x12;    // +0x12 row-copy length
    u16 field_0x14;    // +0x14 flags (bits 0x8/0x10/0x20 per source bits)
    u16 field_0x16;    // +0x16
    u32 field_0x18[2]; // +0x18
    u32 field_0x20[2]; // +0x20
    // Interleaved 8-byte pairs (retail loop walks these with stride 8:
    // stores land at +0x28/+0x2C for entry 0 and +0x30/+0x34 for entry 1).
    struct {
        u32 off28;   // +0x00
        u32 off2C;   // +0x04
    } field_0x28[2];
    f32 field_0x38[5][8];  // +0x38 five 8-float rows
};

// Source blob consumed by func_804C6BA8: flags word at +0x00 (bits 0..4),
// u32 pairs at +0x04/+0x0C, u16 words at +0x14..+0x26, an 8-float row at
// +0x28 and a +0x58 base offset added to the caller's base pointer.
struct CScnEnvLgtCtrlLgtSrc {
    u32 field_0x00;    // +0x00
    u32 field_0x04[2]; // +0x04
    u32 field_0x0C[2]; // +0x0C
    u16 field_0x14;    // +0x14
    u16 field_0x16;    // +0x16
    u16 field_0x18;    // +0x18
    u16 field_0x1A;    // +0x1A
    u16 field_0x1C;    // +0x1C
    u16 field_0x1E;    // +0x1E
    u16 field_0x20;    // +0x20
    u16 field_0x22;    // +0x22
    u16 field_0x24;    // +0x24
    u16 field_0x26;    // +0x26
    f32 field_0x28[8]; // +0x28
    u8 _48[0x10];      // +0x48 gap before the +0x58 word
    u32 field_0x58;    // +0x58
};

// Minimal CScnBlend view for func_804C1094: vtable pointer at +0x00 with the
// deleting-dtor dispatch at slot 2 (0x08) and the filter flags at +0x08.
// The full class lives in CScnBlend.cpp (NonMatching); only the slots used
// here are modelled.
struct CScnEnvLgtBlend {
    virtual void release(int flag) = 0;  // vtable slot 2 (0x08)
    u8 mEnabled;   // +0x04
    u8 pad_05[3];
    u32 mFlags;    // +0x08
};

// Minimal CScnBloom view for func_804C1094. In retail the object's main
// vtable pointer sits at +0x80 (the derived class's own vtable, placed after
// the non-polymorphic CScnBloomBase and the CScnFilter subobject); the
// deleting-dtor thunk lives at vtable slot 7 (0x1C).
struct CScnEnvLgtBloomBase {   // +0x00 non-polymorphic prefix
    u8 _00[0x50];
    u32 field_0x50;   // +0x50
    u8 _54[0x10];     // +0x54 .. +0x64
    u8 field_0x64;    // +0x64
    u8 _65[0x1B];     // pad to 0x80
};
// CScnBloom object viewed by this TU: a 0x80-byte non-polymorphic prefix
// (CScnEnvLgtBloomBase) followed by the object's own vtable pointer at
// +0x80. Single inheritance puts the derived vptr right after the base, so
// the deleting-dtor dispatch (`lwz r12, 0x80(r3); lwz r12, 0x1c(r12)`) is a
// plain member load with no pointer adjustment (cf. the dtor).
struct CScnEnvLgtBloom : public CScnEnvLgtBloomBase {
    virtual void f0() = 0;      // vtable slot 2 (0x08)
    virtual void f1() = 0;      // vtable slot 3 (0x0C)
    virtual void f2() = 0;      // vtable slot 4 (0x10)
    virtual void f3() = 0;      // vtable slot 5 (0x14)
    virtual void f4() = 0;      // vtable slot 6 (0x18)
    virtual void release(int flag) = 0;  // vtable slot 7 (0x1C) - deleting-dtor dispatch
};

// View of the +0x30 light-param control's tail used by func_804C1094's init
// (clears a u8 at +0x64).
struct CScnEnvLgtCtrlLgtParamCtlTail {
    u8 _00[0x64];
    u8 field_0x64;   // +0x64
};

// View of the scene owner (field_0x04) for func_804C1094: the +0x70
// filter-manager slot handed to func_8049D1EC / func_8049D3D8.
struct CScnEnvLgtOwnerLgt {
    u8 _00[0x70];
    u8* field_0x70;   // +0x70
};

// Opaque render-view passed by func_804C392C to the light helpers.
struct CScnEnvLgtLightView;

// Scene environment light controller.
// vtable pointer at +0x00 (the retail dtor installs it), followed by the
// light-state arrays: field_0x38/0x58 hold the previous frame and
// field_0x78/0x98 the current frame (func_804C7530 copies previous -> current).
// +0x24 points at the light-slot array (func_804C5F6C/5FB0/6010) and +0x28 at
// a bit array (func_804C123C / func_804C5C08).
struct CScnEnvLgtCtrlLgtTypeEntry;
struct CScnEnvLgtCtrlLgtCtl;
struct CScnEnvLgtCtrlLgtParamCtl;
struct CScnEnvLgtCtrlLgtLimit;
struct CScnEnvLgtCtrlCtorCtl;

// +0x90..0xA8 region of CScnEnvLgtCtrl written by the ctor (two 3-word
// copies of lbl_eu_80656C58 plus the light-count cap at +0xA8).
struct CScnEnvLgtCtrlCtorCopy {
    u32 field_0x90;  // +0x90
    u32 field_0x94;  // +0x94
    u32 field_0x98;  // +0x98
    u32 field_0x9C;  // +0x9C
    u32 field_0xA0;  // +0xA0
    u32 field_0xA4;  // +0xA4
};

class CScnEnvLgtCtrl {
public:
    CScnEnvLgtCtrl();
    ~CScnEnvLgtCtrl();

    // +0x00: vtable pointer (dtor installs lbl_eu_8056F9B8); also used as a
    // flags word by func_804C12A4 / func_804C1A64.
    union {
        void* mVtable;                      // +0x00 vtable pointer
        u32 flags;                          // +0x00 flags view
    };
    // +0x04: control flags (0x800/0x1000 light bits, func_804C5DA0) and the
    // scene-view pointer handed to func_80496288 / func_8048ECD8.
    union {
        u32 field_0x04;                     // +0x04 control flags
        void* field_0x04_ptr;               // +0x04 view/owner pointer
    };
    void* field_0x08;                       // +0x08 owned buffer (dtor deallocates)
    CScnEnvLgtCtrlListNode* field_0x0C;     // +0x0C light-list ring sentinel
    u16 field_0x10;                         // +0x10
    u16 field_0x12;                         // +0x12 light count
    union {
        u8 field_0x14[8];                            // +0x14
        CScnEnvLgtCtrlLgtTypeEntry* field_0x14_ptr;  // +0x14 (0xd8-stride entry array)
        struct {
            u8 _14[0x04];                            // +0x14
            void* field_0x18_buf;                    // +0x18 (0x14-stride item array, func_804C406C)
        } alt14;
    };
    union {
        f32 field_0x1C;                       // +0x1C (value sampled by func_804C7880)
        CScnEnvLgtCtrlListNode* field_0x1C_nodes;  // +0x1C 0xc-stride node array (func_804C1BA0)
    };
    union {
        u32 field_0x20;                        // +0x20 flags word (func_804C42A8 test, func_804C7564 family)
        int field_0x20_count;                  // +0x20 node-array capacity (func_804C1BA0)
        CScnEnvLgtCtrlLgtEntry40* field_0x20_ptr;  // +0x20 light-command base (func_804C392C)
    };
    CScnEnvLgtCtrlLgtSlot* field_0x24;      // +0x24 light-slot array (0x30-byte entries)
    union {
        u32* field_0x28;                       // +0x28 light bit-array (>= 10 words)
        CScnEnvLgtBlend* field_0x28_blend;     // +0x28 CScnBlend handle (func_804C1094)
    };
    union {
        u8* field_0x2C;                        // +0x2C base of the light-param array (func_804C6054)
        CScnEnvLgtBloom* field_0x2C_bloom;     // +0x2C CScnBloom handle (func_804C1094)
    };
    union {
        CScnEnvLgtCtrlLgtParamCtl* field_0x30;  // +0x30 light-param control blob (func_804C6054 / null-check in func_804C3404)
        void* field_0x30_chk;                   // untyped view (CSE breaker for func_804C406C's null check)
    };
    union {
        u8 field_0x34[4];                   // +0x34 byte view
        f32 field_0x34_f;                   // +0x34 float view (func_804C2124)
        void* field_0x34_ptr;               // +0x34 pointer view (ctor: sub-blob base)
    };
    // +0x38 .. +0xB8: 4 rows of 8 floats at 0x38/0x58/0x78/0x98
    // (func_804C7530 copies rows 2,3 <- rows 0,1). +0x3C aliases row 0's
    // second word (func_804C1600 stores a light-data pointer there;
    // func_804C2124 reads it as a word).
    union {
        f32 field_0x38[0x20];               // +0x38
        struct {
            u8 pad_0x38[0x04];              // +0x38
            void* field_0x3C;               // +0x3C
        } alt;
        struct {
            u8 pad_0x3C[0x04];              // +0x38
            u32 field_0x3C;                 // +0x3C word view (func_804C2124)
        } alt6;
        struct {
            u8 pad_0x40[0x08];              // +0x38..+0x40
            CScnEnvLgtCtrlFogGate* field_0x40;  // +0x40 fog-enable gate (func_804C5380 / func_804C54D4)
            u8 _44[0x04];                   // +0x44
            CScnEnvLgtCtrlLgtCtl* field_0x48;  // +0x48 light-count control
            CScnEnvLgtCtrlLgtLimit* field_0x4C;  // +0x4C light limit object
            f32* field_0x50;                // +0x50 light-row array (func_804C4D28)
        } alt2;
        struct {
            u8 pad_0xB0[0x78];              // +0x38..+0xB0
            f32 field_0xB0;                 // +0xB0 (func_804C30E8)
            f32 field_0xB4;                 // +0xB4 (func_804C30E8)
        } alt3;
        struct {
            u8 pad_0x90[0x58];              // +0x38..+0x90
            f32 field_0x90;                 // +0x90 (func_804C3F58 scaled vec3)
            f32 field_0x94;                 // +0x94
            f32 field_0x98;                 // +0x98
        } alt4;

        struct {
            u8 pad_0x90b[0x58];             // +0x38..+0x90
            CScnEnvLgtCtrlCtorCopy ctor;    // +0x90..+0xA8 (ctor 3-word blob copies)
        } ctor90;
        struct {
            u8 pad_ctr[0x58];               // +0x38..+0x90
            CScnEnvLgtCtrlV3 ctr;           // +0x90 flash-center triple (func_804C43A4)
        } ctrView;
        struct {
            u8 pad_0x9C[0x64];              // +0x38..+0x9C
            f32 fadeW0;                     // +0x9C per-channel fade weights
            f32 fadeW1;                     // +0xA0
            f32 fadeW2;                     // +0xA4
        } altFade;
        struct {
            u8 pad_view[0x58];              // +0x38..+0x90
            f32 ctrCenter[3];               // +0x90 flash-center triple
            f32 fadeW[3];                   // +0x9C per-channel fade weights
        } lgtView;
        struct {
            void* field_0x38;               // +0x38 (ctor: optional sub-blob base)
            void* field_0x3C;               // +0x3C (ctor: optional sub-blob base)
        } ctor38;
        struct {
            CScnEnvLgtCtrlLgtGate* gate38;  // +0x38 light-ranking gate blob
        } altGate;
        struct {
            u8 pad_0x38c[0x08];             // +0x38..+0x40
            u32 flashCounter;               // +0x40 flash step counter (func_804C22F0)
        } altCnt;
        struct {
            u8 pad_0xAC[0x74];              // +0x38..+0xAC
            s32 field_0xAC;                 // +0xAC light-count cap (func_804C526C)
        } alt5;
        struct {
            u8 pad_0xA8[0x70];              // +0x38..+0xA8
            s32 field_0xA8;                 // +0xA8 curve-blend bound (func_804C31C8)
        } alt7;
    };
    union {
        u8 field_0xB8_bytes[0x18];          // +0xB8 .. +0xD0
        struct {
            f32 field_0xB8;                 // +0xB8 (func_804C30E8)
            s16 field_0xBC;                 // +0xBC (func_804C30E8 stores -1 here)
            u8 pad_0xBE[0x0A];              // +0xBE .. +0xC8
            f32 field_0xC8;                 // +0xC8 (func_804C42A8 / func_804C6B88)
            f32 field_0xCC;                 // +0xCC (func_804C30E8 / func_804C6B98)
        };
        // Second view used by func_804C5B00: the s16 slot index at +0xBE and
        // the f32 color pair at +0xC0/+0xC4 that the earlier struct leaves as
        // padding.
        struct {
            f32 field_0xB8;                 // +0xB8
            s16 field_0xBC;                 // +0xBC
            s16 field_0xBE;                 // +0xBE (func_804C5B00 stores the clamped slot)
            f32 field_0xC0;                 // +0xC0 (func_804C5B00)
            f32 field_0xC4;                 // +0xC4 (func_804C5B00)
            f32 field_0xC8;                 // +0xC8
            f32 field_0xCC;                 // +0xCC
        } lgt2;
    };
    // +0xD0 ambient color triple (func_804C3404): word view for the writers,
    // float view for func_804C3C9C's paired-single reads.
    union {
        struct {
            u32 field_0xD0;                 // +0xD0
            u32 field_0xD4;                 // +0xD4
            u32 field_0xD8;                 // +0xD8
        };
        struct {
            f32 mAmbX;                      // +0xD0
            f32 mAmbY;                      // +0xD4
            f32 mAmbZ;                      // +0xD8
        };
    };
    union {
        u8 field_0xDC[8];                   // +0xDC .. +0xE4
        struct {
            u32 field_0xDC_w;               // +0xDC accepted-entry mask (1 << slot)
            u32 field_0xE0;                 // +0xE0 reset selector (func_804C64A8)
        } dcw;
    };
    u32 field_0xE4;                         // +0xE4 active light count / slot field
    union {
        u8 field_0xE8[4];                   // +0xE8
        u32 field_0xE8_count;               // +0xE8 saved count across func_804C678C
    };
    f32 field_0xEC;                         // +0xEC (light value, func_804C3404)
    CScnEnvLgtCtrlFogData mFog;             // +0xF0 (two per-view fog parameter sets)
    f32 field_0x110[8];                     // +0x110 recorded light weights
    f32 field_0x130[8];                     // +0x130 recorded light priorities
    u16 field_0x150[8];                     // +0x150 recorded light row indices
    u8 field_0x160[8];                      // +0x160 recorded light row selectors
};

// Retail vtable data blob installed at CScnEnvLgtCtrl+0x00 by the dtor.
extern "C" u32 lbl_eu_8056F9B8[0x258 / 4];  // vtable (defined in CScnEnvLgtCtrl.cpp)

// Base vtable for the IScnEnvCtl reslist instances (installed by the two
// reslist destructors in this TU).
extern u8 lbl_eu_8056F978[];

// Derived reslist<IScnEnvCtl> vtable (overwrites the base vtable at the end
// of __ct__reslist_IScnEnvCtl).
extern u8 lbl_eu_8056F960[];

// View of the CLightEnv light-bank layout read/written by func_804C3404:
// per-slot CLight pointers (+0x1140) and the parallel u32 slot fields
// (+0x1160) refreshed by func_804C64A8. Mirrors CLightEnv in
// CVirtualLightObj.hpp.
struct CScnEnvLgtCtrlLightEnvView {
    u8 _00[0x1140];          // +0x00
    CLight* mSlotPtrs[4];    // +0x1140
    u8 _1150[0x10];          // +0x1150 (mSlotCounts)
    u32 mSlotFields[4];      // +0x1160
};

// Camera-item view for func_804C2654: the active light items receive a
// pointer to the camera item's +0x10C data block (vtable slot 0x10).
// Address-of-member form keeps MWCC from hoisting the +0x10C computation
// out of the ring-walk loop (cf. DecEscSj struct-member-address recompute).
struct CScnEnvLgtCtrlCamView {
    u8 _00[0x10C];      // +0x00
    u32 field_0x10C;    // +0x10C
};

// Camera-work blob at the scene view +0x68 (the CScn mCamWork slot);
// func_804C2654 resolves the active camera through it: the id at +0x34 is
// passed to func_8049B158 together with the blob.
struct CScnEnvLgtCtrlCamWork {
    u8 _00[0x34];   // +0x00
    s32 mCamId;     // +0x34
};

// Owner view for func_804C1338: the +0x5C virtual-light manager handed to
// func_8049347C / func_804935C0 and the +0x68 camera-work blob.
struct CScnEnvLgtCtrlOwnerLight {
    u8 _00[0x5C];                      // +0x00
    CScnVirtualLight* field_0x5C;      // +0x5C
    u8 _60[0x08];                      // +0x60
    CScnEnvLgtCtrlCamWork* field_0x68; // +0x68
    u8 _70[0x08];                      // +0x70
    void* field_0x78;                  // +0x78 (fog manager)
};

// Owner view for func_804C172C: the +0x64 light-manager slot handed to
// func_8048D160 (which deactivates the active light).
struct CScnEnvLgtCtrlOwnerLightMan {
    u8 _00[0x64];                 // +0x00
    CScnLightMan* field_0x64;     // +0x64
};

// Scene light-manager refresh (defined in CScnLightMan.cpp; flat retail
// name). extern "C" keeps the flat name on the call-site reloc (the retail
// call is `bl func_8048D160`).
extern "C" void func_8048D160(CScnLightMan* self);

// Count-control blob at CScnEnvLgtCtrl+0x48: entry count at +0x04, consulted
// by func_804C58D8 / func_804C5990 before walking the +0x14 entry array.
struct CScnEnvLgtCtrlLgtCtl {
    u32 field_0x00;   // +0x00 flags (bit 0x10 = dispatch active, func_804C42A8)
    u32 mCount;       // +0x04
};

// +0x4C target object: the light-limit float at +0x08 consulted by
// func_804C42A8's clamp and func_804C6B6C's getter.
struct CScnEnvLgtCtrlLgtLimit {
    u8 _00[0x08];   // +0x00
    f32 field_0x08; // +0x08 max value
};

// 0x20-byte work blob built on the stack by func_804C42A8 (via the matched
// func_804C7774) and handed to func_804C7910 / func_804C8054.
struct CScnEnvLgtCtrlWorkBlob {
    u32 field_0x00;   // +0x00
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08 (field_0x04 + 0x18)
    u32 field_0x0C;   // +0x0C
    u32 field_0x10;   // +0x10
    u32 field_0x14;   // +0x14
    u32 field_0x18;   // +0x18
    f32 field_0x1C;   // +0x1C
    u32 field_0x20;   // +0x20 handler status accumulator (bits 0-2 gate the
                      // direction rebuild in func_804C7B54)
};

// 0x40-byte destination rows written by func_804C7910 (indexed by each
// source item's u16 at +0x1E, scaled by 0x40).
// Float view over the work blob's +0x0C/+0x10/+0x14 words: retail stages the
// loop-2/loop-4 vec3s through these slots as floats before the byte-wise copy
// into the destination rows.
struct CScnEnvLgtCtrlWorkBlobF {
    u8 _00[0x0C];
    f32 field_0x0C;
    f32 field_0x10;
    f32 field_0x14;
    u8 _18[8];
};

struct CScnEnvLgtCtrlWorkDst40 {
    u32 field_0x00;      // +0x00
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;      // +0x0C float bits (direction x)
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;      // position/scale words copied from the item front
    u32 field_0x1C;
    u32 field_0x20;
    f32 field_0x24;      // fade weight
    f32 field_0x28;      // +0x28
    f32 field_0x2C;
    f32 field_0x30;      // +0x30
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3C;      // status flag bits
};

// Shared front half of the work items handed to func_804C7B54 (the caller
// strides 0x30/0x3C/0x50/0x64 all begin with this layout).
struct CScnEnvLgtCtrlWorkItemHead {
    u8 _00[4];
    u32 field_0x04;      // bit 0x8000: weight row update; bit 0x8: clamp
    u32 field_0x08;      // -> dst +0x18 / ptmf table 2 index
    u32 field_0x0C;      // -> dst +0x1C / ptmf table 1 index
    u32 field_0x10;      // -> dst +0x20
    f32 field_0x14;      // -> dst +0x24 weight
    u16 field_0x18;
    u16 field_0x1A;
    u16 field_0x1C;      // handler table index (* 0xC)
    u16 field_0x1E;      // destination index (* 0x40)
    u16 field_0x20;      // weight-row index (* 0x14)
};

// Source items walked by func_804C7910 (strides 0x30/0x3C/0x50/0x64).
// Shared front half: flags word at +0x04 (bit 0x10000 gates the update) and
// the u16 destination index at +0x1E.
struct CScnEnvLgtCtrlWorkItem30 {
    u8 _00[4];
    u32 field_0x04;      // bit 0x10000
    u8 _08[0x16];
    u16 field_0x1E;      // destination index (* 0x40)
    u8 _20[0x10];
};
struct CScnEnvLgtCtrlWorkItem3C {
    u8 _00[4];
    u32 field_0x04;
    u8 _08[0x16];
    u16 field_0x1E;
    u8 _20[0x10];
    f32 field_0x30;      // +0x30 vec3 copied into dst +0x0C..0x14
    f32 field_0x34;
    f32 field_0x38;
};
struct CScnEnvLgtCtrlWorkItem50 {
    u8 _00[4];
    u32 field_0x04;
    u8 _08[0x16];
    u16 field_0x1E;
    u8 _20[0x10];
    u32 field_0x30;      // +0x30 pos words -> dst +0x00..0x08
    u32 field_0x34;
    u32 field_0x38;
    u8 _3C[8];
    u32 field_0x44;      // -> dst +0x34
    f32 field_0x48;      // -> dst +0x28
    f32 field_0x4C;      // -> dst +0x2C
};
struct CScnEnvLgtCtrlWorkItem64 {
    u8 _00[4];
    u32 field_0x04;
    u8 _08[0x16];
    u16 field_0x1E;
    u8 _20[0x10];
    u32 field_0x30;      // -> dst +0x00..0x08
    u32 field_0x34;
    u32 field_0x38;
    u8 _3C[8];
    f32 field_0x44;      // -> dst +0x0C
    f32 field_0x48;      // -> dst +0x10
    f32 field_0x4C;      // -> dst +0x14
    u32 field_0x50;      // -> dst +0x34
    f32 field_0x54;      // -> dst +0x28
    f32 field_0x58;      // -> dst +0x2C
    u32 field_0x5C;      // -> dst +0x38
    f32 field_0x60;      // -> dst +0x30
};

// 0x20-byte scan row consumed by func_804C43A4 (array at self+0x34):
// weight*60 is compared against the +0xA8 curve bound during the scan.
struct CScnEnvLgtCtrlScanRow {
    f32 x;          // +0x00
    f32 y;          // +0x04
    f32 z;          // +0x08
    f32 f;          // +0x0C
    u16 weight;     // +0x10 (*60 vs the +0xA8 bound)
    u16 tail;       // +0x12 (second window term)
    u16 flags14;    // +0x14 (bit 0 pushes the item-flag bit 0)
    u8 _16[0x0A];
};

// Control entry handed to func_804C43A4 by func_804C406C (caller strides
// 0x30/0x3C/0x50/0x64 share this front half).
struct CScnEnvLgtCtrlFadeEntry {
    u32 _00;
    u32 flags;      // +0x04 (0xF00000 selects the phase-2 row table)
    f32 color[3];   // +0x08 fallback triple
    f32 wPrev;      // +0x14 fallback weight
    u16 start;      // +0x18 first scan row index
    u16 count;      // +0x1A scan row count
    u16 sel20;      // +0x20 destination row index (0x14-stride at self+0x0C)
    u16 sel22;      // +0x22 phase-2 row index
    u32 rowBase;    // +0x24 phase-2 table offset word
    u8 _28[0x3C];   // pad to the widest caller stride (0x64)
};

// Same-TU work-blob walkers (stub bodies in the .cpp; separate targets).
extern "C" void func_804C7910(CScnEnvLgtCtrlWorkBlob* blob,
                              const CScnEnvLgtCtrlCtorCtl* data, u8* dst);
extern "C" void func_804C7B54(CScnEnvLgtCtrlWorkBlob* blob,
                              CScnEnvLgtCtrlWorkDst40* dst,
                              const CScnEnvLgtCtrlWorkItemHead* item, float f);
extern "C" void func_804C43A4(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlFadeEntry* item);

// Center-color coefficient used by func_804C43A4's flash-center fold.
extern const float lbl_eu_8066B02C;
extern "C" void func_804C4954(CScnEnvLgtCtrl* self, void* entry);
extern "C" void func_804C7190(u8* entry, u32 a, int idx, float val);

// Light-target refresh dispatched by func_80498D98 (defined in another TU).
extern "C" void func_80498D98(void* obj);

// .sdata2 fade constants used by func_804C7190: B060 anchors the per-slot
// fade-scale pool (the 1.0 scalar shared with the keyframe helpers) and
// B068 is the int->double conversion bias (0x4330000000000000).
extern const f64 lbl_eu_8066B068;

// 0x30-byte fade-curve entry scanned by func_804C7190: start/end frame
// times (u16, scaled by 60) followed by the float row blended out of it.
struct CScnEnvLgtCtrlFadeCurve {
    u16 mStart;     // +0x00
    u16 mEnd;       // +0x02
    u8 _04[4];
    f32 mVals[10];  // +0x08 (first mCount floats used)
};

// Curve-list object referenced from the fade entry's +0x20/+0x24 slots:
// default float row at +0x08 and the 0x30-stride curve array at +0x30.
struct CScnEnvLgtCtrlFadeList {
    u8 _00[0x08];
    f32 mDefaults[10];                     // +0x08 (first mCount floats used)
    CScnEnvLgtCtrlFadeCurve mEntries[1];   // +0x30
};

// One row of the color-row tables at entry+0x24 (+ idx*4 + layer*8):
// rows are addressed by the matched curve-entry position.
struct CScnEnvLgtCtrlFadeRows {
    f32 mRow[10];
};

// View of the 0xd8-byte light-type entry processed by func_804C7190:
// float-row length at +0x12, control flags at +0x14 (bits 0x10/0x20 enable
// layers 0/1, bit 0x8000 marks the fade active), per-layer curve counts at
// +0x18 and curve lists at +0x20, the two destination float rows at +0x38
// and their mirror rows at +0x78.
struct CScnEnvLgtCtrlFadeView {
    u8 _00[0x12];
    u16 mCount;                          // +0x12 float-row length
    u16 mFlags;                          // +0x14
    u32 mCurveCount[2];                  // +0x18 per-layer curve-entry counts
    CScnEnvLgtCtrlFadeList* mLists[2];   // +0x20 per-layer curve lists
    u8 _28[0x10];                        // +0x28 .. +0x38 (color-row slots)
    f32 mRows[2][10];                    // +0x38 destination rows (stride 0x20)
    f32 mMirror[2][10];                  // +0x78 mirror rows (stride 0x20)
};

// .sdata2 constants used by func_804C31C8 / func_804C26F0 / func_804C5628 /
// func_804C7910 (int->float magics, clamps and blend weights).
extern const f64 lbl_eu_8066B018;   // 2^52 (curve-window conversion)
extern const f64 lbl_eu_8066B040;   // 2^52 (fog-byte conversion)
extern const u32 lbl_eu_8066B048;   // color-template word (alpha seed, func_804C5628)
extern const float lbl_eu_8066B030; // fog color clamp ceiling
extern const float lbl_eu_8066B004;
extern const float lbl_eu_8066B008;
extern const float lbl_eu_8066B07C; // func_804C7B54 weight (loop 4)
extern const float lbl_eu_8066B080; // degrees -> SinFIdx units scale
extern const float lbl_eu_8066B034; // fog color blend weight (func_804C4E04)
extern const float lbl_eu_8066B038; // fog color blend weight (func_804C4E04)
extern const float lbl_eu_8066B03C; // fog color blend weight (func_804C4E04)

// Member-function pointer tables dispatched by func_804C8054's item loop
// (12-byte ptmf stride; each entry is called with the work blob as `this`
// and one void* argument).
typedef void (CScnEnvLgtCtrlWorkBlob::*CScnEnvLgtCtrlWorkBlobFn)(void*);
extern CScnEnvLgtCtrlWorkBlobFn lbl_eu_8065FBD0[];
extern CScnEnvLgtCtrlWorkBlobFn lbl_eu_8065FAF8[];

// Light-param control blob at CScnEnvLgtCtrl+0x30 (func_804C6054): element
// count at +0x0C and the byte offset of the entry array within field_0x2C at
// +0x10.
struct CScnEnvLgtCtrlLgtParamCtl {
    u8 _00[0x0C];
    u32 mCount;    // +0x0C
    u32 mOffset;   // +0x10
};

// 0x3C-byte parameter entries walked by func_804C6054: bit-15 flag at +0x04
// gates the u16 item index at +0x20.
struct CScnEnvLgtCtrlLgtParamEntry {
    u8 _00[0x04];
    u32 field_0x04;   // +0x04 flags (bit 15)
    f32 field_0x08;   // +0x08 (func_804C3F58 vec3 x)
    f32 field_0x0C;   // +0x0C (y)
    f32 field_0x10;   // +0x10 (z)
    f32 field_0x14;   // +0x14 (scale factor)
    u8 _18[0x08];     // +0x18 .. +0x20
    u16 field_0x20;   // +0x20 item index
    u8 _22[0x1A];     // +0x22 .. +0x3C pad to the retail 0x3C row stride
};

// 0x14-byte item rows indexed from CScnEnvLgtCtrl+0x0C by func_804C6054:
// flags word at +0x10 toggled through bit 1.
struct CScnEnvLgtCtrlLgtItem20 {
    union {
        struct {
            f32 x;        // +0x00 (func_804C3F58 vec3)
            f32 y;        // +0x04
            f32 z;        // +0x08
        };
        CScnEnvLgtCtrlV3 pos;  // wholesale word-triple view (func_804C43A4)
    };
    f32 f;            // +0x0C (scale factor)
    u32 field_0x10;   // +0x10 flags (bits 0/2, func_804C3F58)
};

// 0xd8-byte entry of the array at CScnEnvLgtCtrl+0x14: the u16 type word at
// +0x02 selects the dispatch path (1 = func_804C6D64, 2 = func_804C6F78).
// Five 0x20-byte float rows live at +0x38; the last row (+0xB8, the
// interpolation target/dst row) doubles as the func_804C8054 sentinel fields.
struct CScnEnvLgtCtrlLgtTypeEntry {
    u16 field_0x00;      // +0x00
    u16 field_0x02;      // +0x02 type id
    union {
        u8 field_0x04[0x10];  // +0x04 .. +0x14
        struct {
            u16 field_0x04w;  // +0x04 (switch key, func_804C6D64/6F78)
            u16 field_0x06w;  // +0x06 (table index, func_804C6D64)
            u16 field_0x08w;  // +0x08 (enable flag, func_804C6D64)
            u16 field_0x0Aw;  // +0x0A (table index, func_804C6F78)
            u16 field_0x0Cw;  // +0x0C (table index, func_804C6F78)
            u16 field_0x0Ew;  // +0x0E (byte copy, func_804C6F78)
            u16 field_0x10w;  // +0x10 (table index, func_804C6F78)
            u16 field_0x12w;  // +0x12 (row-copy length)
        };
    };
    u16 field_0x14;      // +0x14 control flags (bit 0x8 gates func_804C8054, bit 0 set by it; bit 0/0x6 gate the row interpolators)
    u16 field_0x16;      // +0x16 ptmf-table index (func_804C8054)
    u8 field_0x18[0x20]; // +0x18 .. +0x38
    union {
        f32 field_0x38[5][8];  // +0x38 .. +0xD8 five 0x20-byte rows
        struct {
            f32 field_0x38r[4][8];  // +0x38 .. +0xB8 rows 0-3 (source rows)
            union {
                f32 field_0xB8_row[8];  // +0xB8 .. +0xD8 dst row (interpolation target)
                struct {
                    f32 field_0xB8;   // +0xB8 (func_804C8054 sentinel tests)
                    f32 field_0xBC;   // +0xBC
                    f32 field_0xC0;   // +0xC0
                    f32 field_0xC4;   // +0xC4
                    u8 _C8[0x10];     // +0xC8 .. +0xD8
                };
            };
        };
    };
};

// 0x14-byte items walked by func_804C8054's dispatch loop: the u16 words at
// +0x08 and +0x0C index the two 12-byte ptmf tables.
struct CScnEnvLgtCtrlLgtTypeItem {
    u8 _00[0x08];        // +0x00
    u16 field_0x08;      // +0x08 (ptmf table 2 index)
    u16 field_0x0A;      // +0x0A
    u16 field_0x0C;      // +0x0C (ptmf table 1 index)
    u8 _0E[0x06];        // +0x0E .. +0x14
};

// 12-byte table entries selected by func_804C8054 via entry->field_0x16
// (base = work blob +0x08 + index*0xC): u16 count at +0x00, u32 offset at
// +0x04 into the blob's +0x00 base.
struct CScnEnvLgtCtrlWorkTable {
    u16 count;   // +0x00
    u16 _02;     // +0x02
    u32 offset;  // +0x04
};

// 0x488-byte env-light container handed to func_804C526C: the +0x484 flags
// word's bit 0 selects the refresh path, and the +0xE4 index selects a
// 0x40-stride CLight slot.
struct CScnEnvLgtCtrlLgtEnv484 {
    u8 _00[0x484];       // +0x00
    u32 field_0x484;     // +0x484
};

// Light-control target handed to the entry walkers. The entry walkers
// func_804C6D64 (writes +0x38..0x68) and func_804C6F78 (writes +0x00..0x44)
// fill the light/color fields; the type-scan callers toggle the +0x08 / +0x50
// / +0x64 flag words on the result.
struct CScnEnvLgtCtrlLgtTarget {
    u32 field_0x00;   // +0x00 (vec4 bit-copy, func_804C6F78)
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08 (bit 1 toggled by func_804C58D8)
    u32 field_0x0C;   // +0x0C
    u8 field_0x10;    // +0x10 (func_804C6F78)
    u8 field_0x11;    // +0x11
    u8 _12[2];        // +0x12
    u32 field_0x14;   // +0x14 (func_804C6F78)
    u8 _18[0x08];     // +0x18 .. +0x20
    u8 field_0x20;    // +0x20 (func_804C6F78)
    u8 field_0x21;    // +0x21 (func_804C6F78)
    u8 _22[2];        // +0x22
    f32 field_0x24;   // +0x24 (func_804C6F78)
    u32 field_0x28;   // +0x28 (func_804C6F78)
    u8 _2C[0x08];     // +0x2C .. +0x34
    u32 field_0x34;   // +0x34 (switch config, func_804C6F78)
    u32 field_0x38;   // +0x38 (vec4 bit-copy, func_804C6D64)
    u32 field_0x3C;   // +0x3C
    u32 field_0x40;   // +0x40
    u32 field_0x44;   // +0x44
    u32 field_0x48;   // +0x48
    u32 field_0x4C;   // +0x4C
    u32 field_0x50;   // +0x50 (bit 1 toggled by func_804C5A48)
    u32 field_0x54;   // +0x54 (bit 1 toggled by func_80498DC0)
    u32 field_0x58;   // +0x58 (func_804C6D64)
    u32 field_0x5C;   // +0x5C (switch config, func_804C6D64)
    u32 field_0x60;   // +0x60
    union {
        u32 field_0x64;   // +0x64 (word write, func_804C6D64)
        u8 field_0x64b;   // +0x64 byte view (func_804C5990 toggles bit 0)
    };
    u32 field_0x68;   // +0x68
};

// 0x30-byte light entries walked by func_804C34A0 (sub-array A of the
// resource blob): a light id indexed into the +0x28 bit array, selection
// flags, a default scale triple and two destination-row indices.
struct CScnEnvLgtCtrlLgtItem30 {
    u16 id;           // +0x00 (bit-array index)
    u16 _02;
    u32 flags;        // +0x04 (bit 15/14 select the entry path; low bits = layers)
    f32 x;            // +0x08
    f32 y;            // +0x0C
    f32 z;            // +0x10
    f32 f;            // +0x14 (default-path scale)
    u8 _18[0x06];
    u16 field_0x1E;   // +0x1E (0x40-stride row index at self+0x20)
    u16 field_0x20;   // +0x20 (0x14-stride row index at self+0x0C)
    u8 _24[0x0C];
};

struct CScnEnvLgtPairU {
    u32 a;
    u32 b;
};

// 4-float slot accumulator pushed to the CLightEnv by func_804C34A0.
struct CScnEnvLgtCtrlLgtSum {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

// func_804C34A0's working accumulator: same 4-float layout as LgtSum plus
// a u32 pair view so the slot-color copy sites compile to the retail
// load-pair/store-pair-reversed struct assignments.
struct CScnEnvLgtCtrlLgtAcc {
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };
        CScnEnvLgtPairU pairs[2];
    };
};

// Front half shared by the ranking rows scanned by func_804C678C: flags,
// two row indices and a position + weight tail used by the distance test.
struct CScnEnvLgtCtrlLgtRankRow {
    u16 field_0x00;   // +0x00
    u16 _02;
    u32 field_0x04;   // +0x04 flags (bits 0x4000/0x8000 gate the enabled tests)
    u8 _08[0x12];
    u16 field_0x1E;   // +0x1E (0x14-row index, bit 0x8000 test)
    u16 field_0x20;   // +0x20 (0x40-row index, bit 0x4000 test)
    u8 _22[0x0E];
    f32 px;           // +0x30
    f32 py;           // +0x34
    f32 pz;           // +0x38
    f32 field_0x3C;   // +0x3C weight/radius term
};

// 4-byte ranked-light id pair walked by func_804C678C phase 1: the u16
// selector picks the control sub-table (nonzero -> mBaseD/0x64-stride,
// zero -> mBaseC/0x50-stride) and the u16 index selects the row.
struct CScnEnvLgtCtrlLgtIdPair {
    u16 sel;   // +0x00
    u16 idx;   // +0x02
};

// Gate blob (self ctor +0x38 slot) consumed by func_804C64A8 and
// func_804C678C: two ranked-light phases plus the row-base offsets.
struct CScnEnvLgtCtrlLgtGate {
    u8 _00[0x10];
    u32 mIdsOff;    // +0x10 word offset (*2 -> byte) into the +0x3C u16 array
    u32 mCount2;    // +0x14 phase-2 element count
    u32 mOff64;     // +0x18 dual-use: 0x64-row base into +0x2C; *2 into +0x3C ids
    u32 mCount1;    // +0x1C phase-1 element count
    u32 mOff50;     // +0x20 0x50-row base into +0x2C
};

// 0x20-byte vec4 rows walked by func_804C678C phase 2.
struct CScnEnvLgtCtrlRow20 {
    f32 x;   // +0x00
    f32 y;   // +0x04
    f32 z;   // +0x08
    f32 w;   // +0x0C
};

// 0x40-byte opaque CLight slot (cursor arithmetic in func_804C64A8).
struct CScnEnvLgtLightSlot {
    u8 _00[0x40];
};

// Virtual-light manager view used by func_804C22F0.
struct CScnEnvLgtMgrView {
    u8 _00[0xD0];
    u8 field_0xD0;    // +0xD0 enable byte
    u8 _D1[0x3B];
    f32 field_0x10C[3];  // +0x10C color triple
};

// Fog-manager view (owner +0x78): timeout word at +0x24 (func_804C22F0).
struct CScnEnvLgtFogManView {
    u8 _00[0x24];
    u32 field_0x24;
};

// Resource-blob front word poked by func_804C22F0.
struct CScnEnvLgtResFront {
    u8 _00[0x50];
    u32 field_0x50;
};

// Ranked-row arrays used by func_804C64A8's second phase (bases from the
// gate blob; the +0x3C/+0x40 pair drives the intensity normalization).
struct CScnEnvLgtCtrlLgtRow64 {
    u8 _00[0x3C];
    f32 field_0x3C;
    f32 field_0x40;
};
struct CScnEnvLgtCtrlLgtRow50 {
    u8 _00[0x3C];
    f32 field_0x3C;
    f32 field_0x40;
};

// Cross-TU imports used by func_804C22F0 / func_804C64A8.
extern "C" void func_8049DE68(u8* fogMan, int flag);
extern "C" void func_80494208(CScnVirtualLight* mgr, int flag);
// Retail ABI here passes four GPRs plus one FP arg (f1); f2 is left unset
// at this call site.
extern "C" int func_804BE398(ml::CVec3* req, u32 a, u32 b, u32 c, f32 d);
extern "C" int func_804BE5AC();
extern "C" int func_804BE5A0(int flag);

// Shared .sdata2 constants and .data assert strings for the light-refresh
// helpers (func_804C3C9C / func_804C64A8 / func_804C678C).
extern const float lbl_eu_8066B000;
extern const float lbl_eu_8066B020;
extern const float lbl_eu_8066B024;
extern const float lbl_eu_8066B028;
extern const float lbl_eu_8066B04C;
extern const float lbl_eu_8066B050;
extern char lbl_eu_80526324[];
extern char lbl_eu_80526300[];

// Same-TU helpers (definitions below).
extern "C" void func_804C678C(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtGate* gate);
extern "C" __declspec(noinline) void func_804C3C9C(CScnEnvLgtCtrl* self,
                                                   CLight* lights, int flag);
extern "C" __declspec(noinline) void func_804C34A0(CScnEnvLgtCtrl* self,
                                                   CLightEnv* env);
extern "C" void func_804C22F0(CScnEnvLgtCtrl* self);

// Camera-item lookup (defined in CScnCameraMan.cpp under the retail flat
// name func_8049B158). CScn.hpp only declares a 1-arg stub; the retail call
// site in func_804C2654 passes the (camera-work blob, id) pair, so the full
// signature is declared here. extern "C" keeps the flat retail name
// (typed-param globals get C++-mangled; cf. CScnVirtualLight.hpp).
struct CScnCameraItem;  // CScnCameraMan.hpp
extern "C" CScnCameraItem* func_8049B158(CScnEnvLgtCtrlCamWork* cam, s32 id);

// Cross-TU light-env helpers used by func_804C1338. The full declarations
// live in CScnVirtualLight.hpp, which cannot be included here (its
// func_804C03A0 signature differs from this header's); these identical
// redeclarations keep the call relocs flat.
extern "C" void func_80495644(CLightEnv* self, u32 idx);
extern "C" void func_804956F8(void* self);
extern "C" void func_804952C4(CLightEnv* self, const ml::CVec3* v);
extern "C" void func_8049347C(CScnVirtualLight* self, const ml::CVec4* src, f32 value);
extern "C" void func_804935C0(CScnVirtualLight* self, const ml::CVec4* src, f32 value);

// func_804C1338 (us-804c5494): per-frame env-light dispatch. The retail
// call site (CScnVirtualLight.cpp) passes the +0x7C pool sub-object (a
// CScnEnvLgtCtrl) plus the CLightEnv. C linkage keeps the flat retail name.
extern "C" void func_804C1338(CScnVirtualLightSub* sub, CLightEnv* env);

// Same-TU light-slot refresh called by func_804C392C. extern "C" keeps the
// flat retail name on the call reloc (the definition below matches).
extern "C" int func_804C3F58(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtVec4* out,
                             CScnEnvLgtCtrlLgtParamEntry* ctl, float f1);

// Light-slot refresh (same-TU function, retail flat name): recomputes one
// CLightEnv slot field. Signature from the func_804C3404 call site
// (self, light, slot index, reset flag) -> new slot field value. extern "C"
// keeps the flat retail name (typed-param globals get C++-mangled); noinline
// keeps the call out-of-line (the stub body would otherwise be inlined).
extern "C" __declspec(noinline) u32 func_804C64A8(CScnEnvLgtCtrl* self,
                                                   CLight* light, u32 slot,
                                                   u32 reset);

// .rodata u32 tables indexed by the entry walkers' u16 words
// (func_804C6D64 field_0x06; func_804C6F78 field_0x0A/0x0C via C0 and
// field_0x10 via D0).
extern const u32 lbl_eu_805244C0[4];
extern const u32 lbl_eu_805244D0[4];

// .sdata2 fog constants for func_804C172C: the 4-byte color word at AFE0
// (stored byte-wise into FogData::color) and the startz/endz/nearz/farz
// float pair at AFE4/AFE8. The color is typed GXColor so the source can copy
// it as a struct (single word load, member-wise byte stores) like retail.
extern GXColor lbl_eu_8066AFE0;
extern float lbl_eu_8066AFE4;
extern float lbl_eu_8066AFE8;

// Light-target flag byte toggle (defined in CScnBlend.cpp as a flat C-name
// function; toggles bit 1 of the target's +0x54 flags word).
extern "C" void func_80498DC0(u8* self, u32 enable);

// Same-TU entry-walkers dispatched by the type-scan functions (separate
// targets). extern "C" keeps the flat retail name on the call-site reloc
// (typed-param globals would get C++-mangled); noinline keeps the stub calls
// out-of-line (the stub bodies would otherwise be inlined, collapsing the
// retail `bl`). `row` selects the source row (0x20-stride from +0x38). The
// type-scan callers do not pass it (retail emits a 2-arg call, leaving r5
// undefined), so there is deliberately no default argument - callers pass a
// value already resident in the row-argument register instead.
extern "C" __declspec(noinline) int func_804C6D64(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* arg,
                                                    int row);
extern "C" __declspec(noinline) int func_804C6F78(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* arg,
                                                    int row);

// 8-byte gradient entry: time bound + color, sampled by func_804C7880.
struct CScnEnvLgtCtrlGradEntry {
    f32 time;   // +0x00
    f32 color;  // +0x04
};

// Time/gradient lookup table read by func_804C7880: min/max bounds, an
// 8-byte-strided (time,color) float-pair array base, and the pair count.
struct CScnEnvLgtCtrlGrad {
    f32 field_0x00;    // +0x00 min bound
    f32 field_0x04;    // +0x04 max bound
    u8 field_0x08[2];  // +0x08
    u16 field_0x0A;    // +0x0A entry count
    u8 field_0x0C[4];  // +0x0C
    u32 field_0x10;    // +0x10 array base offset (added to CScnEnvLgtCtrl+0x00)
};

// Shared .sdata2 float constant returned when no gradient entry matches.
extern float lbl_eu_8066B05C;

// .sdata2 row-interpolation blend factor used by the func_804C6D64 /
// func_804C6F78 entry walkers (retail `lfs f1, lbl_eu_8066B058@sda21`).
extern float lbl_eu_8066B058;

// Shared .sdata2 float constants used by func_804C5B00 / func_804C3F58
// (the 1.0 color value stored to the +0xC0/+0xC4 pair and the output's
// +0x0C word).
extern float lbl_eu_8066B014;

// .sdata2 light-blend constants used by func_804C2124 (ambient/diffuse
// coefficients and blend bounds) and func_804C1338 (slot color defaults).
extern const float lbl_eu_8066AFD8;
extern const float lbl_eu_8066AFDC;
extern const float lbl_eu_8066AFEC;
extern const float lbl_eu_8066AFF0;

// .sdata interpolation bounds consulted by func_804C2124 against the +0x3C
// word (signed compares in retail).
extern "C" u32 lbl_eu_80663AE8;  // .sdata raw words (defined in .cpp)
extern "C" u32 lbl_eu_80663AEC;
extern "C" u32 lbl_eu_80663AF0;
extern "C" u32 lbl_eu_80663AF4;

// Cubic-Hermite basis coefficients used by func_804C7790's keyframe
// interpolation (2, 3, 1, -2 in retail .sdata2).
extern float lbl_eu_8066B074;   // 2.0  (t^2 coeff)
extern float lbl_eu_8066B070;   // 3.0  (t^3/t^2 coeff)
extern const f32 lbl_eu_8066B060[2];   // [0] = 1.0 (per-slot fade scales)
extern float lbl_eu_8066B078;   // -2.0 (t^3 coeff)

// 16-byte keyframe entry walked by func_804C7790: time plus three sampled
// values; the interpolator blends prev.a/prev.b (offsets 4/8) with
// cur.a/cur.c (offsets 4/12) using the Hermite basis.
struct CScnEnvLgtCtrlGrad16Entry {
    f32 time;   // +0x00
    f32 a;      // +0x04
    f32 b;      // +0x08
    f32 c;      // +0x0C
};

// Panic assert strings referenced by func_804C1A64 (retail .data symbols).
extern char lbl_eu_8056EC00[];
extern char lbl_eu_8056EBE0[];

// Panic strings referenced by func_804C5380 / func_804C54D4 (retail .data).
extern char lbl_eu_8056EC60[];
extern char lbl_eu_8056EC40[];
extern char lbl_eu_8056EC30[];
extern char lbl_eu_8056EC10[];

// Panic strings referenced by func_804C172C's fog block (retail .data).
extern char lbl_eu_8056F9A8[];
extern char lbl_eu_8056F988[];

// CScnEnvLgtCtrl constructor (retail symbol is the unmangled short form).
// Builds the controller from an 'STGL' resource header plus an opaque arg.
// noinline matches the retail out-of-line call from func_804C6A70.
extern "C" __declspec(noinline) CScnEnvLgtCtrl* __ct__CScnEnvLgtCtrl(
    CScnEnvLgtCtrl* self, const u32* data, void* arg);

// 'STGL' env-light factory (defined in this TU; separate target
// us-804cabcc): validates the magic word, allocates the 0x168-byte object
// and runs the constructor. extern "C" keeps the flat retail name on the
// call-site reloc in func_804C1BA0; noinline keeps the retail out-of-line
// `bl func_804C6A70` there (the factory body would otherwise be inlined).
extern "C" __declspec(noinline) CScnEnvLgtCtrl* func_804C6A70(u32 handle,
                                                               const u32* data,
                                                               void* arg);

// --- func_804C1094 / func_804C1D7C cross-TU imports (retail flat names) ---

// Same-TU scene-root helper (stub defined in this TU). extern "C" keeps the
// flat retail name on the call reloc; noinline prevents the empty stub body
// from folding into the call sites.
extern "C" __declspec(noinline) void func_804C26F0(CScnEnvLgtCtrl* self);

// The blend/bloom ctors are owned by their own TUs (CScnBlend.cpp /
// CScnBloom.cpp, NonMatching); func_804C1094 only emits the ctor calls with
// the retail short-form symbols.
extern "C" CScnEnvLgtBlend* __ct__CScnBlend(CScnEnvLgtBlend* self);
extern "C" CScnEnvLgtBloom* __ct__CScnBloom(CScnEnvLgtBloom* self);

// Cross-TU helpers (definitions live in CScn.cpp / CScnFilterMan.cpp).
extern "C" CScnEnvLgtCtrlLgtParamCtl* func_8049699C(CScnItemModelNw4rOwner* owner);
extern "C" void func_8049D1EC(u8* man, CScnEnvLgtBlend* filter);
extern "C" void func_8049D3D8(u8* man, CScnEnvLgtBlend* filter);

// Light-render helpers called by func_804C392C / func_804C3778 /
// func_804C3AC8 (defined in the TU before this one; only the call-site ABI
// is needed here).
extern "C" void func_804C03A0(CScnEnvLgtLightView* view, u32 flag);
extern "C" void func_804C0454(CScnEnvLgtLightView* view, const u8* p);
extern "C" void func_804C0484(CScnEnvLgtLightView* view, const u8* p);
extern "C" void func_804C07F0(CScnEnvLgtLightView* view, CScnEnvLgtCtrlLgtVec4* out);
extern "C" void func_804C08C8(CScnEnvLgtLightView* view, u32 flag);
extern "C" void func_804C09E0(CScnEnvLgtLightView* view, u32 a, f32 b, f32 c);
extern "C" void func_804C0920(CScnEnvLgtLightView* view, u32 a, f32 b);

// 16-byte light-data block copied by func_804C5198 (self+0x54 -> out).
// Kept as a named struct so the copy sites use typed pointers: MWCC only
// emits grouped load-all/store-all for typed sources (a void* param forces
// interleaved r0-reuse pairs, cf. MWCC_CASES 3-word struct copy).
struct CScnEnvLgtCtrlLgtData {
    u32 field_0x00;   // +0x00
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08
    u32 field_0x0C;   // +0x0c
};

// Typed view over the CScnEnvLgtCtrl layout used by func_804C5198 (u32 copy
// of the +0x54 block) and func_804C5210 (float copy of +0x54 or +0x64 into
// CScnEnvLgtData::mAmbColorBase, selected by flags bits 0x200 / 0x400).
struct CScnEnvLgtCtrlLgtView {
    u8 _pad[0x04];                     // +0x00
    u32 flags;                         // +0x04 (bit 0x200 = light data at 0x54; bit 0x400 = data at 0x64)
    u8 _pad2[0x4C];                    // +0x08
    union {
        CScnEnvLgtCtrlLgtData data;    // +0x54 (u32 view, func_804C5198)
        f32 field_0x54[4];             // +0x54 (float view, func_804C5210)
        CScnEnvLgtPairU field_0x54_pairs[2];  // +0x54 (pair view, func_804C34A0)
    };
    union {
        f32 field_0x64[4];             // +0x64 (float view, func_804C5210)
        CScnEnvLgtPairU field_0x64_pairs[2];  // +0x64 (pair view, func_804C34A0)
    };
    f32 field_0x68;
    f32 field_0x6C;
};

// --- sinit_804C8174 static-initializer blob copy ---
// The TU's static initializer copies a 0x1A8-byte template blob from .data
// (lbl_eu_8056FA68) into a .bss object (lbl_eu_8065FA40). The destination
// layout differs from the source by a 4-byte gap at +0xB4 (not copied).
// The retail copy waves are: [0x58..0x84) [0x00..0x58) [0xDC..0x118)
// [0x84..0xB4) [0xB8..0xDC) [0x16C..0x1A8) [0x118..0x16C) - all under the
// 0x80-byte unroll threshold, so the members below mirror those chunks.

// Scalar fields (not arrays) so MWCC emits member-wise load/store waves
// instead of recognizing the copy as memcpy.
struct CScnEnvLgtCtrlSinitA {
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08, w09, wa;
    u32 wb, wc, wd, we, wf, w10, w11, w12, w13, w14, w15;
};
struct CScnEnvLgtCtrlSinitB {
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08, w09, wa;
};
struct CScnEnvLgtCtrlSinitC {
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08, w09, wa, wb;
};
struct CScnEnvLgtCtrlSinitD {
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08;
};
struct CScnEnvLgtCtrlSinitE {
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08, w09, wa;
    u32 wb, wc, wd, we;
};
struct CScnEnvLgtCtrlSinitF {
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08, w09, wa;
    u32 wb, wc, wd, we, wf, w10, w11, w12, w13, w14;
};
struct CScnEnvLgtCtrlSinitG {
    u32 w00, w01, w02, w03, w04, w05, w06, w07, w08, w09, wa;
    u32 wb, wc, wd, we;
};

// Source template (.data at 0x8056FA68): members packed back-to-back.
struct CScnEnvLgtCtrlSinitSrc {
    CScnEnvLgtCtrlSinitA a;  // +0x00
    CScnEnvLgtCtrlSinitB b;  // +0x58
    CScnEnvLgtCtrlSinitC c;  // +0x84
    CScnEnvLgtCtrlSinitD d;  // +0xB4
    CScnEnvLgtCtrlSinitE e;  // +0xD8
    CScnEnvLgtCtrlSinitF f;  // +0x114
    CScnEnvLgtCtrlSinitG g;  // +0x168
};

// Destination (.bss at 0x8065FA40): 4-byte gap at +0xB4 (set elsewhere),
// so d..g sit 4 bytes later than in the source.
struct CScnEnvLgtCtrlSinitDst {
    CScnEnvLgtCtrlSinitA a;  // +0x00
    CScnEnvLgtCtrlSinitB b;  // +0x58
    CScnEnvLgtCtrlSinitC c;  // +0x84
    u32 field_0xB4;          // +0xB4 (not copied by the sinit)
    CScnEnvLgtCtrlSinitD d;  // +0xB8
    CScnEnvLgtCtrlSinitE e;  // +0xDC
    CScnEnvLgtCtrlSinitF f;  // +0x118
    CScnEnvLgtCtrlSinitG g;  // +0x16C
};

extern CScnEnvLgtCtrlSinitSrc lbl_eu_8056FA68;
extern u8 lbl_eu_8065FA40[0xB8];  // .bss (defined in .cpp, dissolved monolibdata2)

// --- __ct__CScnEnvLgtCtrl ctor helper types ---
// 3-word .bss blob copied by the ctor into +0x90..0x98 and +0x9C..0xA4.
extern u32 lbl_eu_80656C58[3];

// 8-byte resource elements walked by the ctor at data+0x1C: a type word
// plus a byte offset into the resource blob.
struct CScnEnvLgtCtrlCtorElem {
    u32 mType;    // +0x00 (1/2/3/5/6)
    u32 mOffset;  // +0x04
};

// Resource sub-blob referenced by the type-1 element (also the +0x30
// light-param control): flag-loop counts/bases at +0x04..0x20, the +0x34
// sub-blob offset at +0x28, three optional sub-blob offsets at +0x3C..0x48
// and three item counts at +0x4C..0x54.
struct CScnEnvLgtCtrlCtorCtl {
    u32 mFlags;     // +0x00 (bits 0/1 select the +0x50/+0x54 counts)
    u32 mCountA;    // +0x04 flag-loop 1 count
    u32 mBaseA;     // +0x08 flag-loop 1 base
    u32 mCountB;    // +0x0C flag-loop 2 count
    u32 mBaseB;     // +0x10 flag-loop 2 base
    u32 mCountC;    // +0x14 flag-loop 3 count
    u32 mBaseC;     // +0x18 flag-loop 3 base
    u32 mCountD;    // +0x1C flag-loop 4 count
    u32 mBaseD;     // +0x20 flag-loop 4 base
    u8 _24[0x04];   // +0x24
    u32 mOff34;     // +0x28 -> self+0x34
    u8 _2C[0x10];   // +0x2C
    u32 mFlag38;    // +0x3C (nonzero enables the +0x38/+0x3C pair)
    u32 mOff38;     // +0x40 -> self+0x38
    u8 _44[0x04];   // +0x44
    u32 mOff3C;     // +0x48 -> self+0x3C
    u32 mCount0C;   // +0x4C (0x14-stride items at self+0x0C)
    u32 mCount18;   // +0x50 (0x14-stride items at self+0x18)
    u32 mCount20;   // +0x54 (0x40-stride items at self+0x20)
};

// Resource sub-blob referenced by the type-2 element (self+0x40).
struct CScnEnvLgtCtrlCtorElem2 {
    u16 mFlags;    // +0x00 (bit 1 selects the +0x18 count)
    u8 _02[0x0E];  // +0x02
    u32 mOff44;    // +0x10 -> self+0x44
    u32 mCount10;  // +0x14 (0x1C-stride items at self+0x10)
    u32 mCount1C;  // +0x18 (0x1C-stride items at self+0x1C)
};

// Resource sub-blob referenced by the type-5 element (self+0x48): flags at
// +0x00, the 0xD8-stride item count at +0x04, and the 0x5C-stride source
// entries at +0x10 handed to func_804C6BA8.
struct CScnEnvLgtCtrlCtorElem5 {
    u32 mFlags;    // +0x00 (bits 0/1/2 -> self flags 1/2/4)
    u32 mCount14;  // +0x04 (0xD8-stride items at self+0x14)
    u8 _08[0x08];  // +0x08
    u8 mSrc[0];    // +0x10 (0x5C-stride source entries)
};

// 0x1C-stride item rows installed at self+0x10 / self+0x1C by the ctor; the
// u16 at +0x18 is zeroed per item.
struct CScnEnvLgtCtrlCtorItem1C {
    u8 _00[0x18];    // +0x00
    u16 field_0x18;  // +0x18
};

// Front half shared by the four control sub-array entries walked by
// func_804C6110 (copy pass): flags word at +0x04 (bits 0x3C00 gate) and the
// u16 source/destination row indices at +0x20/+0x22.
struct CScnEnvLgtCtrlWorkEntBase {
    u32 _00;
    u32 flags;     // +0x04
    u8 _08[0x18];
    u16 idxSrc;    // +0x20 (*0x14 -> self+0x0C)
    u16 idxDst;    // +0x22 (*0x14 -> self+0x18)
};

// Padded variants matching the four caller strides (0x30/0x3C/0x50/0x64).
struct CScnEnvLgtCtrlWorkEnt30 : CScnEnvLgtCtrlWorkEntBase { u8 _24[0x0C]; };
struct CScnEnvLgtCtrlWorkEnt3C : CScnEnvLgtCtrlWorkEntBase { u8 _24[0x18]; };
struct CScnEnvLgtCtrlWorkEnt50 : CScnEnvLgtCtrlWorkEntBase { u8 _24[0x2C]; };
struct CScnEnvLgtCtrlWorkEnt64 : CScnEnvLgtCtrlWorkEntBase { u8 _24[0x40]; };

// Fog-gate entry walked by func_804C6110 (+0x3C stride): flags at +0x04
// (bits 27-30 gate the copy), u16 row indices at +0x10/+0x12 and a tail u16
// at +0x18.
struct CScnEnvLgtCtrlFogEnt {
    u32 _00;
    u32 flags;      // +0x04
    u8 _08[0x08];
    u16 idxSrc;     // +0x10 (*0x1C -> self+0x10)
    u16 idxDst;     // +0x12 (*0x1C -> self+0x1C)
    u8 _14[0x04];
    u16 field_0x18; // +0x18
    u16 _1A;
};

// Typed view of the 0x14-byte rows installed at self+0x0C / self+0x18
// (first three vectors copied as words, +0x0C as float).
struct CScnEnvLgtCtrlRow14 {
    u32 field_0x00;   // +0x00
    u32 field_0x04;
    u32 field_0x08;
    f32 field_0x0C;
    u32 field_0x10;
};

// Typed view of the 0x1C-byte rows installed at self+0x10 / self+0x1C.
struct CScnEnvLgtCtrlRow1C {
    u32 field_0x00;   // +0x00
    u32 field_0x04;
    u32 field_0x08;
    f32 field_0x0C;   // +0x0C (copied as float)
    f32 field_0x10;   // +0x10 (copied as float)
    u16 field_0x18;
};

// Runtime item-array view of CScnEnvLgtCtrl used by func_804C6110: the ctor
// installs sub-buffer pointers at +0x0C..+0x1C and the control handles at
// +0x30/+0x40/+0x48; +0x90..+0xA4 is the rotating color triple.
struct CScnEnvLgtCtrlRowsView {
    void* mVtable;                       // +0x00
    u32 field_0x04;                      // +0x04
    void* field_0x08;                    // +0x08
    CScnEnvLgtCtrlRow14* rows0C;         // +0x0C (0x14-stride)
    CScnEnvLgtCtrlRow1C* rows10;         // +0x10 (0x1C-stride)
    CScnEnvLgtCtrlLgtTypeEntry* rows14;  // +0x14 (0xD8-stride)
    CScnEnvLgtCtrlRow14* rows18;         // +0x18 (0x14-stride)
    CScnEnvLgtCtrlRow1C* rows1C;         // +0x1C (0x1C-stride)
    u8 _20[0x0C];                        // +0x20 .. +0x2C
    u8* base2C;                          // +0x2C resource blob base
    CScnEnvLgtCtrlCtorCtl* ctl30;        // +0x30 light-param control
    u8 _34[0x0C];                        // +0x34 .. +0x40
    CScnEnvLgtCtrlFogGate* gate40;       // +0x40 fog-enable gate
    u8 _44[0x04];                        // +0x44
    CScnEnvLgtCtrlLgtCtl* ctl48;         // +0x48 count-control blob
    u8 _4C[0x44];                        // +0x4C .. +0x90
    CScnEnvLgtCtrlCtorCopy rot;          // +0x90 .. +0xA8
    u8 _A8[0x18];                        // +0xA8 .. +0xC0
    f32 field_0xC0;                      // +0xC0
    f32 field_0xC4;
    f32 field_0xC8;
    f32 field_0xCC;
};

// Constructor-only view of the CScnEnvLgtCtrl fields written by
// __ct__CScnEnvLgtCtrl (the shared header models the post-init layout where
// +0x10/+0x18 are scalars, but the ctor installs item-array pointers there).
struct CScnEnvLgtCtrlCtorView {
    void* mVtable;                        // +0x00
    u32 field_0x04;                       // +0x04
    void* field_0x08;                     // +0x08 (allocated work buffer)
    void* field_0x0C;                     // +0x0C (0x14-stride item array)
    void* field_0x10;                     // +0x10 (0x1C-stride item array)
    void* field_0x14;                     // +0x14 (0xD8-stride item array)
    void* field_0x18;                     // +0x18 (0x14-stride item array)
    void* field_0x1C;                     // +0x1C (0x1C-stride item array)
    void* field_0x20;                     // +0x20 (0x40-stride item array)
    void* field_0x24;                     // +0x24 (0x30-stride array)
    void* field_0x28;                     // +0x28 (bit array / memset target)
    u8* field_0x2C;                       // +0x2C (resource blob base)
    CScnEnvLgtCtrlCtorCtl* field_0x30;    // +0x30 (light-param control)
    void* field_0x34;                     // +0x34
    void* field_0x38;                     // +0x38
    void* field_0x3C;                     // +0x3C
    CScnEnvLgtCtrlCtorElem2* field_0x40;  // +0x40 (type-2 element)
    void* field_0x44;                     // +0x44
    CScnEnvLgtCtrlCtorElem5* field_0x48;  // +0x48 (type-5 element)
    void* field_0x4C;                     // +0x4C (type-3 element)
    void* field_0x50;                     // +0x50 (type-6 element)
    u8 _54[0x20];                         // +0x54 .. +0x74
    f32 field_0x74[7];                    // +0x74 (zeroed to B014)
    u8 _8C[0x04];                         // +0x8C
    CScnEnvLgtCtrlCtorCopy ctor90;        // +0x90 .. +0xA8
    u32 field_0xA8;                       // +0xA8
    void* field_0xAC;                     // +0xAC (arg)
    s16 field_0xBC;                       // +0xBC (slot index, -1)
    s16 field_0xBE;                       // +0xBE
    f32 field_0xC0;                       // +0xC0
    f32 field_0xC4;                       // +0xC4
    f32 field_0xC8;                       // +0xC8
    f32 field_0xCC;                       // +0xCC
};

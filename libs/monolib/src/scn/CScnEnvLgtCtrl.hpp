#pragma once

#include <types.h>
#include "monolib/scn/CLight.hpp"
#include "libs/monolib/src/scn/CScnLightMan.hpp"  // CScnLightMan (func_804C172C light-manager slot)

class CScn;  // full definition in monolib/scn/CScn.hpp (CScnItemModelNw4r.hpp uses CScn*)

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
    CScnEnvLgtCtrlLgtVec3 field_0x0C;   // +0x0C (same source triple as field_0x00, func_804C5E9C)
    CScnEnvLgtCtrlLgtVec3 field_0x18;   // +0x18
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
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16(void* arg) = 0;           // vtable slot 18 (0x48)
    virtual void v17(void* arg) = 0;           // vtable slot 19 (0x4C)
    virtual void v18(void* arg, float f) = 0;  // vtable slot 20 (0x50)
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual int v23() = 0;                     // vtable slot 25 (0x64)
    virtual void v24() = 0;
    virtual void v25(int flag) = 0;            // vtable slot 27 (0x6C) - teardown pass (func_804C1D7C)
    virtual void v26(void* a, void* b) = 0;    // vtable slot 28 (0x70)
    virtual void v27(void* a, void* b, float f) = 0;  // vtable slot 29 (0x74)
    virtual void v28() = 0;                    // vtable slot 30 (0x78)
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
// a u16 flags word whose bit 0 permits the fog push.
struct CScnEnvLgtCtrlFogGate {
    u16 mFlags;   // +0x00
    u8 _02[2];
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
    u8 _3C[0x04];      // +0x3C pad to the retail 0x40 row stride
};

// Control blob handed to func_804C392C: u16 light index at +0x00 (tested
// against the +0x28 bit array), flags at +0x04 (bit 15 selects the entry
// path), the +0x1E entry index into the +0x20 array, a color vec3 at +0x30
// and a direction pair at +0x44..+0x4C. Overlaps the front of
// CScnEnvLgtCtrlLgtParamEntry (the same object; that struct only covers 0x24).
struct CScnEnvLgtCtrlLgtCmd {
    u16 field_0x00;    // +0x00
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
    u32 field_0x28[2]; // +0x28
    u32 field_0x2C[2]; // +0x2C
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
    CScnEnvLgtCtrlLgtParamCtl* field_0x30;  // +0x30 light-param control blob (func_804C6054 / null-check in func_804C3404)
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
            void* field_0x38;               // +0x38 (ctor: optional sub-blob base)
            void* field_0x3C;               // +0x3C (ctor: optional sub-blob base)
        } ctor38;
        struct {
            u8 pad_0xAC[0x74];              // +0x38..+0xAC
            s32 field_0xAC;                 // +0xAC light-count cap (func_804C526C)
        } alt5;
        struct {
            u8 pad_0xA8[0x70];              // +0x38..+0xA8
            u32 field_0xA8;                 // +0xA8 curve-blend bound (func_804C31C8)
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
    u32 field_0xD0;                         // +0xD0 ambient color triple (func_804C3404)
    u32 field_0xD4;                         // +0xD4
    u32 field_0xD8;                         // +0xD8
    u8 field_0xDC[8];                       // +0xDC .. +0xE4
    u32 field_0xE4;                         // +0xE4 (light slot field, func_804C3404)
    u8 field_0xE8[4];                       // +0xE8
    f32 field_0xEC;                         // +0xEC (light value, func_804C3404)
    CScnEnvLgtCtrlFogData mFog;             // +0xF0 (two per-view fog parameter sets)
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
};

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
};

// 0x14-byte item rows indexed from CScnEnvLgtCtrl+0x0C by func_804C6054:
// flags word at +0x10 toggled through bit 1.
struct CScnEnvLgtCtrlLgtItem20 {
    f32 x;            // +0x00 (func_804C3F58 vec3)
    f32 y;            // +0x04
    f32 z;            // +0x08
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
// float pair at AFE4/AFE8.
extern u32 lbl_eu_8066AFE0;
extern float lbl_eu_8066AFE4;
extern float lbl_eu_8066AFE8;

// Light-target flag byte toggle (defined in CScnBlend.cpp as a flat C-name
// function; toggles bit 1 of the target's +0x54 flags word).
extern "C" void func_80498DC0(u8* self, u32 enable);

// Same-TU entry-walkers dispatched by the type-scan functions (separate
// targets). extern "C" keeps the flat retail name on the call-site reloc
// (typed-param globals would get C++-mangled); noinline keeps the stub calls
// out-of-line (the stub bodies would otherwise be inlined, collapsing the
// retail `bl`). `row` selects the source row (0x20-stride from +0x38); the
// type-scan callers pass it in r5.
extern "C" __declspec(noinline) int func_804C6D64(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* arg,
                                                    int row = 0);
extern "C" __declspec(noinline) int func_804C6F78(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* arg,
                                                    int row = 0);

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
extern float lbl_eu_8066B060;   // 1.0  (constant)
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
// interleaved r0-reuse pairs, cf. MWCC_REFERENCE 3-word struct copy).
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
    };
    f32 field_0x64;                    // +0x64
    f32 field_0x68;                    // +0x68
    f32 field_0x6C;                    // +0x6c
};

// --- sinit_804C8174 static-initializer blob copy ---
// The TU's static initializer copies a 0x1A8-byte template blob from .data
// (lbl_eu_8056FA68) into a .bss object (lbl_eu_8065FA40). The destination
// layout differs from the source by a 4-byte gap at +0xB4 (not copied).
// The retail copy waves are: [0x58..0x84) [0x00..0x58) [0xDC..0x118)
// [0x84..0xB4) [0xB8..0xDC) [0x16C..0x1A8) [0x118..0x16C) - all under the
// 0x80-byte unroll threshold, so the members below mirror those chunks.

// 0x58-byte member (22 words).
struct CScnEnvLgtCtrlSinitA {
    u32 words[0x16];  // +0x00 .. +0x58
};
// 0x2C-byte member (11 words).
struct CScnEnvLgtCtrlSinitB {
    u32 words[0x0B];  // +0x00 .. +0x2C
};
// 0x30-byte member (12 words).
struct CScnEnvLgtCtrlSinitC {
    u32 words[0x0C];  // +0x00 .. +0x30
};
// 0x24-byte member (9 words).
struct CScnEnvLgtCtrlSinitD {
    u32 words[0x09];  // +0x00 .. +0x24
};
// 0x3C-byte member (15 words).
struct CScnEnvLgtCtrlSinitE {
    u32 words[0x0F];  // +0x00 .. +0x3C
};
// 0x54-byte member (21 words).
struct CScnEnvLgtCtrlSinitF {
    u32 words[0x15];  // +0x00 .. +0x54
};
// 0x3C-byte member (15 words).
struct CScnEnvLgtCtrlSinitG {
    u32 words[0x0F];  // +0x00 .. +0x3C
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

#pragma once

#include <types.h>
#include <revolution/gx/GXTypes.h>  // GXColor (sLgtFogColorWhite fog color word)
#include "monolib/scn/CLight.hpp"
#include "libs/monolib/src/scn/CScnLightMan.hpp"  // CScnLightMan (scnLgtRefreshLightMan light-manager slot)

class CScn;  // full definition in monolib/scn/CScn.hpp (CScnItemModelNw4r.hpp uses CScn*)

// Forward decl for the ctor-view union member (defined below).
struct CScnEnvLgtCtrlLgtGate;

// Cross-TU light types referenced by the CScnEnvLgtCtrl vtable interface and
// the scnLgtDispatchEnv light-env dispatch (full definitions in their headers).
class CScnVirtualLight;      // CScnVirtualLight.hpp
class CScnCameraMan;         // CScnCameraMan.hpp (+0x68 camera work / findItem)
class CLightEnv;             // CVirtualLightObj.hpp
struct CScnVirtualLightSub;  // CScnVirtualLight.hpp
namespace ml { struct CVec3; struct CVec4; }  // monolib/math

// 0x30-byte light-slot entry in the array at CScnEnvLgtCtrl+0x24.
// scnLgtClearSlotEn / 5FB0 / 6010 poke the u16 word at +0x28 (control flags:
// bit 0 and bit 2 cleared/set there) and the three u16 params at +0x2A..+0x2E.
// Two-word row pair used by the controller's row-block members and the
// accumulator copies in scnLgtRebuildSlotColors.
struct CScnEnvLgtPairU {
    u32 a;
    u32 b;
};

// 12-byte vec3 used by the light-slot entries; copied wholesale by
// scnLgtWriteSlotVecs (word moves - retail treats the triple as u32 words).
struct CScnEnvLgtCtrlLgtVec3 {
    u32 x;  // +0x00
    u32 y;  // +0x04
    u32 z;  // +0x08
};

// Plain 3-float triple: assigning whole objects of this type makes MWCC
// move the three words with lwz/stw pairs (scnLgtRefreshEntryFade's out/center rows).
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

// Float vec3 used as the scaling temp in scnLgtFillLightVec4's two branches.
struct CScnEnvLgtCtrlLgtVec3f {
    f32 x;  // +0x00
    f32 y;  // +0x04
    f32 z;  // +0x08
};

// 4-float output written by scnLgtFillLightVec4: the scaled vec3 in the first
// three words plus the shared .sdata2 constant in the +0x0C word.
struct CScnEnvLgtCtrlLgtVec4 {
    f32 x;  // +0x00
    f32 y;  // +0x04
    f32 z;  // +0x08
    f32 w;  // +0x0C
};

struct CScnEnvLgtCtrlLgtSlot {
    CScnEnvLgtCtrlLgtVec3 field_0x00;   // +0x00
    // +0x0C (same source triple as field_0x00, scnLgtWriteSlotScale): word view for
    // the bulk copies, float view for scnLgtBlendCurveSlots's scaled reads.
    union {
        CScnEnvLgtCtrlLgtVec3 w;
        CScnEnvLgtCtrlLgtVec3f f;
    } field_0x0C;
    // +0x18 position triple: word view for the bulk copies / byte-wise writes,
    // float view for scnLgtPushSlotLights's paired-single reads.
    union {
        CScnEnvLgtCtrlLgtVec3 field_0x18;
        CScnEnvLgtCtrlLgtVec3f field_0x18f;
    };
    f32 field_0x24;                     // +0x24 (float param, scnLgtWriteSlotScale)
    u16 field_0x28;                     // +0x28 control flags
    u16 field_0x2A;                     // +0x2a
    u16 field_0x2C;                     // +0x2c
    u16 field_0x2E;                     // +0x2e
};

// Vtable-view of CScnEnvLgtCtrl itself for the slot-0x70 virtual dispatch in
// scnLgtFrameStep (self's own vtable). Declared virtual N sits at vtable offset
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
// (0x28, scnLgtBindLightArg) and slot 28 (0x70, scnLgtCallLightV26). Abstract on
// purpose - never instantiated in this TU.
struct CScnEnvLgtCtrlListItem {
    virtual void v0(int flag) = 0;             // vtable slot 2 (0x08) - deleting-dtor dispatch (scnLgtRemoveLgtItem)
    virtual void v1() = 0;
    virtual void v2(void* arg) = 0;            // vtable slot 4 (0x10)
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8(void* arg) = 0;            // vtable slot 10 (0x28)
    virtual void v9(void* arg) = 0;            // vtable slot 11 (0x2C) - env dispatch (scnLgtDispatchEnv)
    virtual void v10(void* arg, void* data) = 0;  // vtable slot 12 (0x30) - env + camera data (scnLgtDispatchEnv)
    virtual void v11(void* arg, u8 byte) = 0;  // vtable slot 13 (0x34)
    virtual int v12(void* vec) = 0;            // vtable slot 14 (0x38) - vec4 query, nonzero = apply (scnLgtDispatchEnv)
    virtual int v13(void* vec) = 0;            // vtable slot 15 (0x3C) - vec4 query (scnLgtDispatchEnv)
    virtual void v14(void* data) = 0;          // vtable slot 16 (0x40) - light-data push (scnLgtGatherLights fwd walk)
    virtual int v15(float f, void* data, void* arg,
                    u32 count) = 0;            // vtable slot 17 (0x44) - active-light query (scnLgtGatherLights back walk)
    virtual void v16(void* arg) = 0;           // vtable slot 18 (0x48)
    virtual void v17(void* arg) = 0;           // vtable slot 19 (0x4C)
    virtual void v18(void* arg, float f) = 0;  // vtable slot 20 (0x50)
    virtual int v19(void* bits, int flag) = 0;     // vtable slot 21 (0x54)
    virtual int v20(void* base, int flag) = 0;     // vtable slot 22 (0x58)
    virtual void v21() = 0;
    // slot 22 sits at vtable 0x60: light push with (value, mode, submode).
    // scnLgtPushLgtValue calls it as v22(f, 1, flag-1) on the active pass and
    // v22(f, 0, 0) on the idle pass.
    virtual void v22(float f, s32 mode, s32 submode) = 0;
    virtual int v23() = 0;                     // vtable slot 25 (0x64)
    virtual void v24(int flag) = 0;            // vtable slot 26 (0x68) - retail passes r4 (CREvtLight / CTaskREvtSequence)
    virtual void v25(int flag) = 0;            // vtable slot 27 (0x6C) - teardown pass (scnLgtRemoveLgtItem)
    virtual void v26(void* a, void* b) = 0;    // vtable slot 28 (0x70)
    virtual void v27(void* a, void* b, float f) = 0;  // vtable slot 29 (0x74)
    virtual int v28(void* out, int flag) = 0;  // vtable slot 30 (0x78)
    virtual int v29() = 0;                     // vtable slot 31 (0x7C) - "idle" query (scnLgtCreateEnvController)
    virtual int v30() = 0;                     // vtable slot 32 (0x80) - "in use" query (scnLgtRemoveLgtItem / scnLgtCreateEnvController)
    virtual void v31(u32 a, u32 b) = 0;        // vtable slot 33 (0x84)
    virtual void v32(u32 a, u32 b, void* c, void* d) = 0; // vtable slot 34 (0x88)
    virtual void v33(u32 a, u32 b, void* c) = 0; // vtable slot 35 (0x8C)
    virtual void v34() = 0;                    // vtable slot 36 (0x90)
    virtual void v35(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) = 0; // vtable slot 37 (0x94)
    virtual void v36() = 0;                    // vtable slot 38 (0x98)
    virtual void v37(int flag) = 0;            // vtable slot 39 (0x9C) - teardown dispatch (scnLgtCreateEnvController)
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
    CScnItemModelNw4rOwner* mOwner;    // +0x04 scene owner (Scn_CallUnk8C_V8 arg)
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
extern const float kLgtBlendWeightLo;
extern const float kLgtBlendHalf;

// Fog-enable gate at CScnEnvLgtCtrl+0x40 (scnLgtPushFogView0 / scnLgtPushFogView1):
// a u16 flags word whose bit 0 permits the fog push. scnLgtDispatchFadeEntries also
// walks a 0x3C-stride entry array located at field_0x2C + mOffset with
// mCount entries.
struct CScnEnvLgtCtrlFogGate {
    u16 mFlags;    // +0x00
    u8 _02[2];
    u32 mCount;    // +0x04
    u32 mOffset;   // +0x08
};

// Full-init view of the +0x30 light-param control blob written by
// scnLgtResetLightState (defaults reset).
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

// View of the +0x28 object poked by scnLgtResetLightState.
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

// View of the +0x2C object poked by scnLgtResetLightState.
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
// end[2] @ 0x108/0x10C. scnLgtPushFogView0 pushes view 0 into the scene root's fog
// slot 0, scnLgtPushFogView1 view 1.
struct CScnEnvLgtCtrlFogData {
    u32 mColor[2];  // +0x00
    u32 mType[2];   // +0x08
    f32 mStart[2];  // +0x10
    f32 mEnd[2];    // +0x18
};

// 0x40-byte light-command rows indexed from CScnEnvLgtCtrl+0x20 by
// scnLgtApplyLightCmdScaled (index = the control's u16 at +0x1E).
struct CScnEnvLgtCtrlLgtEntry40 {
    u8 _00[0x18];      // +0x00
    f32 field_0x18;    // +0x18
    f32 field_0x1C;    // +0x1C
    f32 field_0x20;    // +0x20
    u8 _24[0x04];      // +0x24
    f32 field_0x28;    // +0x28
    f32 field_0x2C;    // +0x2C
    f32 field_0x30;    // +0x30 (scnLgtApplyLightCmdEx)
    u32 field_0x34;    // +0x34
    u32 field_0x38;    // +0x38 (scnLgtApplyLightCmdEx)
    u32 field_0x3C;    // +0x3C enable bit tested by scnLgtRankGateLights
};

// Control blob handed to scnLgtApplyLightCmdScaled: u16 light index at +0x00 (tested
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
    u32 field_0x50;    // +0x50 (scnLgtApplyLightCmdEx)
    f32 field_0x54;    // +0x54
    f32 field_0x58;    // +0x58
    u32 field_0x5C;    // +0x5C
    f32 field_0x60;    // +0x60
};

// +0x28 object written by scnLgtPushSlotBlend: a 7-word float block at +0x38
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

// Compact header + row block built by scnLgtBuildHeader: 12 u16 words
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

// Source blob consumed by scnLgtBuildHeader: flags word at +0x00 (bits 0..4),
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

// Minimal CScnBlend view for scnLgtSetBlendBloom: vtable pointer at +0x00 with the
// deleting-dtor dispatch at slot 2 (0x08) and the filter flags at +0x08.
// The full class lives in CScnBlend.cpp (NonMatching); only the slots used
// here are modelled.
struct CScnEnvLgtBlend {
    virtual void release(int flag) = 0;  // vtable slot 2 (0x08)
    u8 mEnabled;   // +0x04 /* 0x04 */
    u8 pad_05[3]; /* 0x05 */
    u32 mFlags;    // +0x08 /* 0x08 */
};

// Minimal CScnBloom view for scnLgtSetBlendBloom. In retail the object's main
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

// View of the +0x30 light-param control's tail used by scnLgtSetBlendBloom's init
// (clears a u8 at +0x64).
struct CScnEnvLgtCtrlLgtParamCtlTail {
    u8 _00[0x64];
    u8 field_0x64;   // +0x64
};

// View of the scene owner (field_0x04) for scnLgtSetBlendBloom: the +0x70
// filter-manager slot handed to ScnFilterMan_appendFilter / ScnFilterMan_toggleFilter.
struct CScnEnvLgtOwnerLgt {
    u8 _00[0x70];
    u8* field_0x70;   // +0x70
};

// Opaque render-view passed by scnLgtApplyLightCmdScaled to the light helpers.
struct CScnEnvLgtLightView;

// Scene environment light controller.
// vtable pointer at +0x00 (the retail dtor installs it), followed by the
// light-state arrays: field_0x38/0x58 hold the previous frame and
// field_0x78/0x98 the current frame (scnLgtCopyPrevRows copies previous -> current).
// +0x24 points at the light-slot array (scnLgtClearSlotEn/5FB0/6010) and +0x28 at
// a bit array (scnLgtSetSinkBit1 / scnLgtSetLightBit).
struct CScnEnvLgtCtrlLgtTypeEntry;
struct CScnEnvLgtCtrlLgtCtl;
struct CScnEnvLgtCtrlLgtParamCtl;
struct CScnEnvLgtCtrlLgtLimit;
struct CScnEnvLgtCtrlCtorCtl;
struct CScnEnvLgtCtrlGrad;
struct CScnEnvLgtCtrlWorkDst40;
struct CScnEnvLgtCtrlRow1C;   // 0x1c-stride row array installed at +0x10

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

// Layout recovery state (see docs/MWCC_CASES.md "CScnEnvLgtCtrl layout"):
// +0x04 is the scene-view owner object (CScnItemModelNw4rOwner / OwnerLight
// views) that holds the camera work at +0x68, the virtual-light manager at
// +0x5C, the light manager at +0x64 and the fog manager at +0x78; those four
// are now reached through their real classes (CScnCameraMan, CScnVirtualLight,
// CScnLightMan, CScnFogMan). Still views here: the +0x38..+0xB8 light-row block,
// the light-slot / 0xD8-stride entry work structs, and the ptmf target blob
// (owning class unidentified; the scnLgtPtmf* adapters set its words).
class CScnEnvLgtCtrl {
public:
    CScnEnvLgtCtrl();
    ~CScnEnvLgtCtrl();

    // +0x00: vtable pointer (dtor installs lbl_eu_8056F9B8); also used as a
    // flags word by scnLgtStepLightItems / scnLgtPushFogNone.
    union {
        void* mVtable;                      // +0x00 vtable pointer /* 0x00 */
        u32 flags;                          // +0x00 flags view /* 0x00 */
    };
    // +0x04: control flags (0x800/0x1000 light bits, scnLgtSetSlotBits) and the
    // scene-view pointer handed to Scn_GetFrameDelta / getScnRootSlot10.
    union {
        u32 field_0x04;                     // +0x04 control flags /* 0x04 */
        void* field_0x04_ptr;               // +0x04 view/owner pointer /* 0x04 */
    };
    void* field_0x08;                       // +0x08 owned buffer (dtor deallocates) /* 0x08 */
    CScnEnvLgtCtrlListNode* field_0x0C;     // +0x0C light-list ring sentinel /* 0x0C */
    union {
        struct {
            u16 field_0x10;                 // +0x10 /* 0x10 */
            u16 field_0x12;                 // +0x12 light count /* 0x12 */
        };
        CScnEnvLgtCtrlRow1C* rows10;        // +0x10 0x1c-stride row array
    };
    union {
        u8 field_0x14[8];                            // +0x14 /* 0x14 */
        CScnEnvLgtCtrlLgtTypeEntry* field_0x14_ptr;  // +0x14 (0xd8-stride entry array) /* 0x14 */
        struct {
            u8 _14[0x04];                            // +0x14 /* 0x14 */
            void* field_0x18_buf;                    // +0x18 (0x14-stride item array, scnLgtDispatchFadeEntries) /* 0x18 */
        } alt14;
    };
    union {
        f32 field_0x1C;                       // +0x1C (value sampled by scnLgtSampleGradColor) /* 0x1C */
        CScnEnvLgtCtrlListNode* field_0x1C_nodes;  // +0x1C 0xc-stride node array (scnLgtCreateEnvController) /* 0x1C */
    };
    union {
        u32 field_0x20;                        // +0x20 flags word (scnLgtStepWorkBlob test, scnLgtPtmfSetF18 family) /* 0x20 */
        int field_0x20_count;                  // +0x20 node-array capacity (scnLgtCreateEnvController) /* 0x20 */
        CScnEnvLgtCtrlLgtEntry40* field_0x20_ptr;  // +0x20 light-command base (scnLgtApplyLightCmdScaled) /* 0x20 */
    };
    CScnEnvLgtCtrlLgtSlot* field_0x24;      // +0x24 light-slot array (0x30-byte entries) /* 0x24 */
    union {
        u32* field_0x28;                       // +0x28 light bit-array (>= 10 words) /* 0x28 */
        CScnEnvLgtBlend* field_0x28_blend;     // +0x28 CScnBlend handle (scnLgtSetBlendBloom) /* 0x28 */
    };
    union {
        u8* field_0x2C;                        // +0x2C base of the light-param array (scnLgtToggleItemBit) /* 0x2C */
        CScnEnvLgtBloom* field_0x2C_bloom;     // +0x2C CScnBloom handle (scnLgtSetBlendBloom) /* 0x2C */
    };
    union {
        CScnEnvLgtCtrlLgtParamCtl* field_0x30;  // +0x30 light-param control blob (scnLgtToggleItemBit / null-check in scnLgtApplyAmbColor) /* 0x30 */
        void* field_0x30_chk;                   // untyped view (CSE breaker for scnLgtDispatchFadeEntries's null check) /* 0x30 */
    };
    union {
        u8 field_0x34[4];                   // +0x34 byte view /* 0x34 */
        f32 field_0x34_f;                   // +0x34 float view (scnLgtPushSlotBlend) /* 0x34 */
        void* field_0x34_ptr;               // +0x34 pointer view (ctor: sub-blob base) /* 0x34 */
    };
    // +0x38 .. +0xB8: 4 rows of 8 floats at 0x38/0x58/0x78/0x98
    // (scnLgtCopyPrevRows copies rows 2,3 <- rows 0,1). +0x3C aliases row 0's
    // second word (scnLgtBindLightArg stores a light-data pointer there;
    // scnLgtPushSlotBlend reads it as a word).
    union {
        f32 field_0x38[0x20];               // +0x38 /* 0x38 */
        // The two ambient-color quads at +0x54/+0x64 are part of this
        // controller's own row block (scnLgtCopyAmbBase / scnLgtReadLightDataWords
        // scnLgtRebuildSlotColors); declare them here instead of casting a
        // separate view over `this`.
        struct {
            u8 _rowPad[0x54 - 0x38];
            union {
                u32 lgtWords54[4];
                f32 lgtColors54[4];
                CScnEnvLgtPairU lgtPairs54[2];
            };
            union {
                u32 lgtWords64[4];
                f32 lgtColors64[4];
                CScnEnvLgtPairU lgtPairs64[2];
            };
        } lgtRows;
        struct {
            u8 pad_0x38[0x04];              // +0x38 /* 0x38 */
            void* field_0x3C;               // +0x3C /* 0x3C */
        } alt;
        struct {
            u8 pad_0x3C[0x04];              // +0x38 /* 0x38 */
            u32 field_0x3C;                 // +0x3C word view (scnLgtPushSlotBlend) /* 0x3C */
        } alt6;
        struct {
            u8 pad_0x40[0x08];              // +0x38..+0x40 /* 0x38 */
            CScnEnvLgtCtrlFogGate* field_0x40;  // +0x40 fog-enable gate (scnLgtPushFogView0 / scnLgtPushFogView1) /* 0x40 */
            u8 _44[0x04];                   // +0x44 /* 0x44 */
            CScnEnvLgtCtrlLgtCtl* field_0x48;  // +0x48 light-count control /* 0x48 */
            CScnEnvLgtCtrlLgtLimit* field_0x4C;  // +0x4C light limit object /* 0x4C */
            f32* field_0x50;                // +0x50 light-row array (scnLgtBlendRow5) /* 0x50 */
        } alt2;
        struct {
            u8 pad_0xB0[0x78];              // +0x38..+0xB0 /* 0x38 */
            f32 field_0xB0;                 // +0xB0 (scnLgtFrameStep) /* 0xB0 */
            f32 field_0xB4;                 // +0xB4 (scnLgtFrameStep) /* 0xB4 */
        } alt3;
        struct {
            u8 pad_0x90[0x58];              // +0x38..+0x90 /* 0x38 */
            f32 field_0x90;                 // +0x90 (scnLgtFillLightVec4 scaled vec3) /* 0x90 */
            f32 field_0x94;                 // +0x94 /* 0x94 */
            f32 field_0x98;                 // +0x98 /* 0x98 */
        } alt4;

        struct {
            u8 pad_0x90b[0x58];             // +0x38..+0x90 /* 0x38 */
            CScnEnvLgtCtrlCtorCopy ctor;    // +0x90..+0xA8 (ctor 3-word blob copies) /* 0x90 */
        } ctor90;
        struct {
            u8 pad_ctr[0x58];               // +0x38..+0x90 /* 0x38 */
            CScnEnvLgtCtrlV3 ctr;           // +0x90 flash-center triple (scnLgtRefreshEntryFade) /* 0x90 */
        } ctrView;
        struct {
            u8 pad_0x9C[0x64];              // +0x38..+0x9C /* 0x38 */
            f32 fadeW0;                     // +0x9C per-channel fade weights /* 0x9C */
            f32 fadeW1;                     // +0xA0 /* 0xA0 */
            f32 fadeW2;                     // +0xA4 /* 0xA4 */
        } altFade;
        struct {
            u8 pad_view[0x58];              // +0x38..+0x90 /* 0x38 */
            f32 ctrCenter[3];               // +0x90 flash-center triple /* 0x90 */
            f32 fadeW[3];                   // +0x9C per-channel fade weights /* 0x9C */
        } lgtView;
        struct {
            void* field_0x38;               // +0x38 (ctor: optional sub-blob base) /* 0x38 */
            void* field_0x3C;               // +0x3C (ctor: optional sub-blob base) /* 0x3C */
        } ctor38;
        struct {
            CScnEnvLgtCtrlLgtGate* gate38;  // +0x38 light-ranking gate blob /* 0x38 */
        } altGate;
        struct {
            u8 pad_0x38c[0x08];             // +0x38..+0x40 /* 0x38 */
            u32 flashCounter;               // +0x40 flash step counter (scnLgtUpdateSceneLights) /* 0x40 */
        } altCnt;
        struct {
            u8 pad_0xAC[0x74];              // +0x38..+0xAC /* 0x38 */
            s32 field_0xAC;                 // +0xAC light-count cap (scnLgtRefreshSlot) /* 0xAC */
        } alt5;
        struct {
            u8 pad_0xA8[0x70];              // +0x38..+0xA8 /* 0x38 */
            s32 field_0xA8;                 // +0xA8 curve-blend bound (scnLgtBlendCurveSlots) /* 0xA8 */
        } alt7;
    };
    union {
        u8 field_0xB8_bytes[0x18];          // +0xB8 .. +0xD0 /* 0xB8 */
        struct {
            f32 field_0xB8;                 // +0xB8 (scnLgtFrameStep) /* 0xB8 */
            s16 field_0xBC;                 // +0xBC (scnLgtFrameStep stores -1 here) /* 0xBC */
            u8 pad_0xBE[0x0A];              // +0xBE .. +0xC8 /* 0xBE */
            f32 field_0xC8;                 // +0xC8 (scnLgtStepWorkBlob / scnLgtGetTime) /* 0xC8 */
            f32 field_0xCC;                 // +0xCC (scnLgtFrameStep / scnLgtGetDelta) /* 0xCC */
        };
        // Second view used by scnLgtSetFlashSlot: the s16 slot index at +0xBE and
        // the f32 color pair at +0xC0/+0xC4 that the earlier struct leaves as
        // padding.
        struct {
            f32 field_0xB8;                 // +0xB8 /* 0xB8 */
            s16 field_0xBC;                 // +0xBC /* 0xBC */
            s16 field_0xBE;                 // +0xBE (scnLgtSetFlashSlot stores the clamped slot) /* 0xBE */
            f32 field_0xC0;                 // +0xC0 (scnLgtSetFlashSlot) /* 0xC0 */
            f32 field_0xC4;                 // +0xC4 (scnLgtSetFlashSlot) /* 0xC4 */
            f32 field_0xC8;                 // +0xC8 /* 0xC8 */
            f32 field_0xCC;                 // +0xCC /* 0xCC */
        } lgt2;
    };
    // +0xD0 ambient color triple (scnLgtApplyAmbColor): word view for the writers,
    // float view for scnLgtPushSlotLights's paired-single reads.
    union {
        struct {
            u32 field_0xD0;                 // +0xD0 /* 0xD0 */
            u32 field_0xD4;                 // +0xD4 /* 0xD4 */
            u32 field_0xD8;                 // +0xD8 /* 0xD8 */
        };
        struct {
            f32 mAmbX;                      // +0xD0 /* 0xD0 */
            f32 mAmbY;                      // +0xD4 /* 0xD4 */
            f32 mAmbZ;                      // +0xD8 /* 0xD8 */
        };
    };
    union {
        u8 field_0xDC[8];                   // +0xDC .. +0xE4 /* 0xDC */
        struct {
            u32 field_0xDC_w;               // +0xDC accepted-entry mask (1 << slot) /* 0xDC */
            u32 field_0xE0;                 // +0xE0 reset selector (scnLgtRecomputeSlotField) /* 0xE0 */
        } dcw;
    };
    u32 field_0xE4;                         // +0xE4 active light count / slot field /* 0xE4 */
    union {
        u8 field_0xE8[4];                   // +0xE8 /* 0xE8 */
        u32 field_0xE8_count;               // +0xE8 saved count across scnLgtRankGateLights /* 0xE8 */
    };
    f32 field_0xEC;                         // +0xEC (light value, scnLgtApplyAmbColor) /* 0xEC */
    CScnEnvLgtCtrlFogData mFog;             // +0xF0 (two per-view fog parameter sets) /* 0xF0 */
    f32 field_0x110[8];                     // +0x110 recorded light weights /* 0x110 */
    f32 field_0x130[8];                     // +0x130 recorded light priorities /* 0x130 */
    u16 field_0x150[8];                     // +0x150 recorded light row indices /* 0x150 */
    u8 field_0x160[8];                      // +0x160 recorded light row selectors /* 0x160 */
    void CallLightV26(void* a, void* b);
    void CallLightV27(void* a, void* b, float f);
    void StepWorkBlob(float f);
};

// Retail vtable data blob installed at CScnEnvLgtCtrl+0x00 by the dtor.
extern "C" u32 lbl_eu_8056F9B8[0xA0 / 4];
// The class's __RTTIBaseList (0x10: the IScnEnvCtl base entry plus its terminator).
extern "C" u32 lbl_eu_8056FA58[0x10 / 4];

// Base vtable for the IScnEnvCtl reslist instances (installed by the two
// reslist destructors in this TU).
extern u8 lbl_eu_8056F978[];

// Derived reslist<IScnEnvCtl> vtable (overwrites the base vtable at the end
// of __ct__reslist_IScnEnvCtl).
extern u8 lbl_eu_8056F960[];

// View of the CLightEnv light-bank layout read/written by scnLgtApplyAmbColor:
// per-slot CLight pointers (+0x1140) and the parallel u32 slot fields
// (+0x1160) refreshed by scnLgtRecomputeSlotField. Mirrors CLightEnv in
// CVirtualLightObj.hpp, which this TU cannot include yet: its
// LightCtlSetAttnMode(u8*,int) declaration clashes with the
// LightCtlSetAttnMode(CScnEnvLgtLightView*,u32) import below (four different
// spellings across the tree).
struct CScnEnvLgtCtrlLightEnvView {
    u8 _00[0x1140];          // +0x00
    CLight* mSlotPtrs[4];    // +0x1140
    u8 _1150[0x10];          // +0x1150 (mSlotCounts)
    u32 mSlotFields[4];      // +0x1160
};

// scnLgtBindCamera / scnLgtDispatchEnv hand the active light items a pointer to
// the camera item's +0x10C data block (vtable slot 0x10), i.e. the real
// CScnCameraItem::mPos (libs/monolib/src/scn/CScnCameraMan.hpp).

// The scene view +0x68 (the CScn mCamWork slot) is the real CScnCameraMan
// (libs/monolib/src/scn/CScnCameraMan.hpp); scnLgtBindCamera resolves the active
// camera item through mParam->mPool and the camera id at +0x34.

// Owner view for scnLgtDispatchEnv: the +0x5C virtual-light manager handed to
// scnVlApplyDir4C / scnVlApplyDir6C and the +0x68 camera-work blob.
struct CScnEnvLgtCtrlOwnerLight {
    u8 _00[0x5C];                      // +0x00
    CScnVirtualLight* field_0x5C;      // +0x5C
    u8 _60[0x08];                      // +0x60
    CScnCameraMan* field_0x68;         // +0x68
    u8 _70[0x08];                      // +0x70
    void* field_0x78;                  // +0x78 (fog manager)
};

// Owner view for scnLgtRefreshLightMan: the +0x64 light-manager slot handed to
// LightManDeactivateArmedLight (which deactivates the active light).
struct CScnEnvLgtCtrlOwnerLightMan {
    u8 _00[0x64];                 // +0x00
    CScnLightMan* field_0x64;     // +0x64
};

// Scene light-manager refresh (defined in CScnLightMan.cpp; flat retail
// name). extern "C" keeps the flat name on the call-site reloc (the retail
// call is `bl LightManDeactivateArmedLight`).
extern "C" void LightManDeactivateArmedLight(CScnLightMan* self);

// Count-control blob at CScnEnvLgtCtrl+0x48: entry count at +0x04, consulted
// by scnLgtBindType1Entry / scnLgtBindType2Entry before walking the +0x14 entry array.
struct CScnEnvLgtCtrlLgtCtl {
    u32 field_0x00;   // +0x00 flags (bit 0x10 = dispatch active, scnLgtStepWorkBlob)
    u32 mCount;       // +0x04
};

// +0x4C target object: the light-limit float at +0x08 consulted by
// scnLgtStepWorkBlob's clamp and scnLgtGetLimit's getter.
struct CScnEnvLgtCtrlLgtLimit {
    u8 _00[0x08];   // +0x00
    f32 field_0x08; // +0x08 max value
};

// 0x20-byte work blob built on the stack by scnLgtStepWorkBlob (via the matched
// scnLgtInitWorkBlob) and handed to scnLgtPushControlRows / scnLgtDispatchEntryEvents.
struct CScnEnvLgtCtrlWorkBlob {
    u32 field_0x00;   // +0x00
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08 (field_0x04 + 0x18)
    union {
        u32 field_0x0C;   // +0x0C
        f32 mF0C;         // float staging slot (scnLgtPtmfSetSelf0C)
    };
    union {
        u32 field_0x10;   // +0x10
        f32 mF10;
    };
    union {
        u32 field_0x14;   // +0x14
        f32 mF14;
    };
    u32 field_0x18;   // +0x18
    f32 field_0x1C;   // +0x1C
    u32 field_0x20;   // +0x20 handler status accumulator (bits 0-2 gate the
                      // direction rebuild in scnLgtSolveRowDirection)

    // Handler family installed in the three ptmf tables (sLgtWorkBlobSetters /
    // sLgtWorkBlobStores / sLgtWorkBlobGradFuncs). Each entry is dispatched with the
    // blob as `this` and a single pointer argument, so the handlers take the
    // concrete row/entry type plus the float the retail call site leaves set.
    void scnLgtPtmfSetF18(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfSetF1C(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfSetF20(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfSetF24(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreF0(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreF4(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreF8(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfSetSelf0C(float v);
    void scnLgtPtmfSetSelf10(float v);
    void scnLgtPtmfSetSelf14(float v);
    void scnLgtPtmfStoreF28(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreF2C(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreF30(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreI34(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreI38(CScnEnvLgtCtrlWorkDst40* dst, float v);
    void scnLgtPtmfStoreS04(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreFB8(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreFBC(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreFC0(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreFC4(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreFC8(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreFCC(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreFD0(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreS06(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreS08(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreS0A(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreS0C(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreS0E(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmfStoreS10(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmf2StoreC8(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmf2StoreCC(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmf2StoreD0(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    void scnLgtPtmf2StoreD4(CScnEnvLgtCtrlLgtTypeEntry* entry, float v);
    float scnLgtSampleGradHermite(const CScnEnvLgtCtrlGrad* grad);
    float scnLgtSampleGradColor(const CScnEnvLgtCtrlGrad* grad);
};

// 0x40-byte destination rows written by scnLgtPushControlRows (indexed by each
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
    // The handler family dispatched by scnLgtSolveRowDirection writes these
    // slots as floats while the row builders copy them as words, so each slot
    // is a word/float union of its own.
    union {
        u32 field_0x00;  // +0x00
        f32 mF00;
    };
    union {
        u32 field_0x04;
        f32 mF04;
    };
    union {
        u32 field_0x08;
        f32 mF08;
    };
    u32 field_0x0C;      // +0x0C float bits (direction x)
    u32 field_0x10;
    u32 field_0x14;
    union {
        u32 field_0x18;  // position/scale words copied from the item front
        f32 mF18;
    };
    union {
        u32 field_0x1C;
        f32 mF1C;
    };
    union {
        u32 field_0x20;
        f32 mF20;
    };
    f32 field_0x24;      // fade weight
    f32 field_0x28;      // +0x28 (scnLgtPtmfStoreF28)
    f32 field_0x2C;      // (scnLgtPtmfStoreF2C)
    f32 field_0x30;      // +0x30 (scnLgtPtmfStoreF30)
    union {
        u32 field_0x34;
        s32 mI34;
    };
    union {
        u32 field_0x38;
        s32 mI38;
    };
    u32 field_0x3C;      // status flag bits
};

// Shared front half of the work items handed to scnLgtSolveRowDirection (the caller
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

// Source items walked by scnLgtPushControlRows (strides 0x30/0x3C/0x50/0x64).
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

// 0x20-byte scan row consumed by scnLgtRefreshEntryFade (array at self+0x34):
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

// Control entry handed to scnLgtRefreshEntryFade by scnLgtDispatchFadeEntries (caller strides
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
extern "C" void scnLgtPushControlRows(CScnEnvLgtCtrlWorkBlob* blob,
                              const CScnEnvLgtCtrlCtorCtl* data, u8* dst);
extern "C" void scnLgtSolveRowDirection(CScnEnvLgtCtrlWorkBlob* blob,
                              CScnEnvLgtCtrlWorkDst40* dst,
                              const CScnEnvLgtCtrlWorkItemHead* item, float f);
extern "C" void scnLgtRefreshEntryFade(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlFadeEntry* item);

// Center-color coefficient used by scnLgtRefreshEntryFade's flash-center fold.
extern const float kLgtCurveThird;
extern "C" void scnLgtWalkFogGateEntry(CScnEnvLgtCtrl* self, void* entry);
extern "C" void scnLgtEvalSlotFade(u8* entry, u32 a, int idx, float val);

// Light-target refresh dispatched by func_80498D98 (defined in another TU).
extern "C" void func_80498D98(void* obj);

// .sdata2 fade constants used by scnLgtEvalSlotFade: B060 anchors the per-slot
// fade-scale pool (the 1.0 scalar shared with the keyframe helpers) and
// B068 is the int->double conversion bias (0x4330000000000000).
extern const f64 kFadeIntToF64Magic;

// 0x30-byte fade-curve entry scanned by scnLgtEvalSlotFade: start/end frame
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

// View of the 0xd8-byte light-type entry processed by scnLgtEvalSlotFade:
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

// .sdata2 constants used by scnLgtBlendCurveSlots / scnLgtResetLightState / scnLgtPushFogSlot0 /
// scnLgtPushControlRows (int->float magics, clamps and blend weights).
extern const f64 kSignedIntToF64Magic;   // 2^52+2^31 (slot-blend signed magic, scnLgtPushSlotBlend)
extern const f64 kCurveIntToF64Magic;   // 2^52 (curve-window conversion)
extern const f64 kFogByteToF64Magic;   // 2^52 (fog-byte conversion)
extern const u32 kLgtColorAlphaSeed;   // color-template word (alpha seed, scnLgtPushFogSlot0)
extern const float kLgtByteScale; // fog color clamp ceiling
extern const float kLgtTwo;
extern const f32 kLgtOneAndHalf[2];
extern const float kLgtNegOne; // scnLgtSolveRowDirection weight (loop 4)
extern const f32 kRadToSinFIdx[2]; // degrees -> SinFIdx units scale
extern const float kLgtLumaR; // fog color blend weight (scnLgtRefreshFogList)
extern const float kLgtLumaG; // fog color blend weight (scnLgtRefreshFogList)
extern const float kLgtLumaB; // fog color blend weight (scnLgtRefreshFogList)

// Member-function pointer tables dispatched by the item loops in
// scnLgtSolveRowDirection / scnLgtDispatchEntryEvents (12-byte ptmf stride; each
// entry is called with the work blob as `this` and one void* argument). The
// entries convert the concrete handler signatures, so MWCC keeps each table in
// .bss and copies the 12-byte constants from .data in __sinit.
typedef void (CScnEnvLgtCtrlWorkBlob::*CScnEnvLgtCtrlWorkBlobFn)(void*);

// 0x40+0x20+0x18-byte dispatch tables installed by the TU's static initializer.
struct CScnEnvLgtCtrlWorkDispatchTable {
    CScnEnvLgtCtrlWorkBlobFn mSlots[15];   // +0x00 (retail .bss 0xB8 spans the 4-byte tail)
    u32 mTail;                             // +0xB4 (never initialised)
};
extern CScnEnvLgtCtrlWorkDispatchTable sLgtWorkBlobSetters;
extern CScnEnvLgtCtrlWorkBlobFn sLgtWorkBlobStores[];
extern CScnEnvLgtCtrlWorkBlobFn sLgtWorkBlobGradFuncs[];

// Light-param control blob at CScnEnvLgtCtrl+0x30 (scnLgtToggleItemBit): element
// count at +0x0C and the byte offset of the entry array within field_0x2C at
// +0x10.
struct CScnEnvLgtCtrlLgtParamCtl {
    u8 _00[0x0C];
    u32 mCount;    // +0x0C
    u32 mOffset;   // +0x10
};

// 0x3C-byte parameter entries walked by scnLgtToggleItemBit: bit-15 flag at +0x04
// gates the u16 item index at +0x20.
struct CScnEnvLgtCtrlLgtParamEntry {
    u8 _00[0x04];
    u32 field_0x04;   // +0x04 flags (bit 15)
    f32 field_0x08;   // +0x08 (scnLgtFillLightVec4 vec3 x)
    f32 field_0x0C;   // +0x0C (y)
    f32 field_0x10;   // +0x10 (z)
    f32 field_0x14;   // +0x14 (scale factor)
    u8 _18[0x08];     // +0x18 .. +0x20
    u16 field_0x20;   // +0x20 item index
    u8 _22[0x1A];     // +0x22 .. +0x3C pad to the retail 0x3C row stride
};

// 0x14-byte item rows indexed from CScnEnvLgtCtrl+0x0C by scnLgtToggleItemBit:
// flags word at +0x10 toggled through bit 1.
struct CScnEnvLgtCtrlLgtItem20 {
    union {
        struct {
            f32 x;        // +0x00 (scnLgtFillLightVec4 vec3)
            f32 y;        // +0x04
            f32 z;        // +0x08
        };
        CScnEnvLgtCtrlV3 pos;  // wholesale word-triple view (scnLgtRefreshEntryFade)
    };
    f32 f;            // +0x0C (scale factor)
    u32 field_0x10;   // +0x10 flags (bits 0/2, scnLgtFillLightVec4)
};

// 0xd8-byte entry of the array at CScnEnvLgtCtrl+0x14: the u16 type word at
// +0x02 selects the dispatch path (1 = scnLgtFillTargetBack, 2 = scnLgtFillTargetFront).
// Five 0x20-byte float rows live at +0x38; the last row (+0xB8, the
// interpolation target/dst row) doubles as the scnLgtDispatchEntryEvents sentinel fields.
struct CScnEnvLgtCtrlLgtTypeEntry {
    u16 field_0x00;      // +0x00
    u16 field_0x02;      // +0x02 type id
    union {
        u8 field_0x04[0x10];  // +0x04 .. +0x14
        struct {
            u16 field_0x04w;  // +0x04 (switch key, scnLgtFillTargetBack/6F78)
            u16 field_0x06w;  // +0x06 (table index, scnLgtFillTargetBack)
            u16 field_0x08w;  // +0x08 (enable flag, scnLgtFillTargetBack)
            u16 field_0x0Aw;  // +0x0A (table index, scnLgtFillTargetFront)
            u16 field_0x0Cw;  // +0x0C (table index, scnLgtFillTargetFront)
            u16 field_0x0Ew;  // +0x0E (byte copy, scnLgtFillTargetFront)
            u16 field_0x10w;  // +0x10 (table index, scnLgtFillTargetFront)
            u16 field_0x12w;  // +0x12 (row-copy length)
        };
    };
    u16 field_0x14;      // +0x14 control flags (bit 0x8 gates scnLgtDispatchEntryEvents, bit 0 set by it; bit 0/0x6 gate the row interpolators)
    u16 field_0x16;      // +0x16 ptmf-table index (scnLgtDispatchEntryEvents)
    u8 field_0x18[0x20]; // +0x18 .. +0x38
    union {
        f32 field_0x38[5][8];  // +0x38 .. +0xD8 five 0x20-byte rows
        struct {
            f32 field_0x38r[4][8];  // +0x38 .. +0xB8 rows 0-3 (source rows)
            union {
                f32 field_0xB8_row[8];  // +0xB8 .. +0xD8 dst row (interpolation target)
                struct {
                    f32 field_0xB8;   // +0xB8 (scnLgtDispatchEntryEvents sentinel tests)
                    f32 field_0xBC;   // +0xBC
                    f32 field_0xC0;   // +0xC0
                    f32 field_0xC4;   // +0xC4
                    f32 field_0xC8;   // +0xC8 (scnLgtPtmf2StoreC8)
                    f32 field_0xCC;   // +0xCC (scnLgtPtmf2StoreCC)
                    f32 field_0xD0;   // +0xD0 (scnLgtPtmf2StoreD0)
                    f32 field_0xD4;   // +0xD4 (scnLgtPtmf2StoreD4)
                };
            };
        };
    };
};

// 0x14-byte items walked by scnLgtDispatchEntryEvents's dispatch loop: the u16 words at
// +0x08 and +0x0C index the two 12-byte ptmf tables.
struct CScnEnvLgtCtrlLgtTypeItem {
    u8 _00[0x08];        // +0x00
    u16 field_0x08;      // +0x08 (ptmf table 2 index)
    u16 field_0x0A;      // +0x0A
    u16 field_0x0C;      // +0x0C (ptmf table 1 index)
    u8 _0E[0x06];        // +0x0E .. +0x14
};

// 12-byte table entries selected by scnLgtDispatchEntryEvents via entry->field_0x16
// (base = work blob +0x08 + index*0xC): u16 count at +0x00, u32 offset at
// +0x04 into the blob's +0x00 base.
struct CScnEnvLgtCtrlWorkTable {
    u16 count;   // +0x00
    u16 _02;     // +0x02
    u32 offset;  // +0x04
};

// 0x488-byte env-light container handed to scnLgtRefreshSlot: the +0x484 flags
// word's bit 0 selects the refresh path, and the +0xE4 index selects a
// 0x40-stride CLight slot.
struct CScnEnvLgtCtrlLgtEnv484 {
    CLight mSlot0;            // +0x00 first light slot (base of the 0x40-stride CLight array)
    u8 _40[0x484 - 0x40];     // +0x40
    u32 field_0x484;     // +0x484
};

// Light-control target handed to the entry walkers. The entry walkers
// scnLgtFillTargetBack (writes +0x38..0x68) and scnLgtFillTargetFront (writes +0x00..0x44)
// fill the light/color fields; the type-scan callers toggle the +0x08 / +0x50
// / +0x64 flag words on the result.
struct CScnEnvLgtCtrlLgtTarget {
    u32 field_0x00;   // +0x00 (vec4 bit-copy, scnLgtFillTargetFront)
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08 (bit 1 toggled by scnLgtBindType1Entry)
    u32 field_0x0C;   // +0x0C
    u8 field_0x10;    // +0x10 (scnLgtFillTargetFront)
    u8 field_0x11;    // +0x11
    u8 _12[2];        // +0x12
    u32 field_0x14;   // +0x14 (scnLgtFillTargetFront)
    u8 _18[0x08];     // +0x18 .. +0x20
    u8 field_0x20;    // +0x20 (scnLgtFillTargetFront)
    u8 field_0x21;    // +0x21 (scnLgtFillTargetFront)
    u8 _22[2];        // +0x22
    f32 field_0x24;   // +0x24 (scnLgtFillTargetFront)
    u32 field_0x28;   // +0x28 (scnLgtFillTargetFront)
    u8 _2C[0x08];     // +0x2C .. +0x34
    u32 field_0x34;   // +0x34 (switch config, scnLgtFillTargetFront)
    u32 field_0x38;   // +0x38 (vec4 bit-copy, scnLgtFillTargetBack)
    u32 field_0x3C;   // +0x3C
    u32 field_0x40;   // +0x40
    u32 field_0x44;   // +0x44
    u32 field_0x48;   // +0x48
    u32 field_0x4C;   // +0x4C
    u32 field_0x50;   // +0x50 (bit 1 toggled by scnLgtBindType3Entry)
    u32 field_0x54;   // +0x54 (bit 1 toggled by Blend_SetFlag2)
    u32 field_0x58;   // +0x58 (scnLgtFillTargetBack)
    u32 field_0x5C;   // +0x5C (switch config, scnLgtFillTargetBack)
    u32 field_0x60;   // +0x60
    union {
        u32 field_0x64;   // +0x64 (word write, scnLgtFillTargetBack)
        u8 field_0x64b;   // +0x64 byte view (scnLgtBindType2Entry toggles bit 0)
    };
    u32 field_0x68;   // +0x68
};

// 0x30-byte light entries walked by scnLgtRebuildSlotColors (sub-array A of the
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

// 4-float slot accumulator pushed to the CLightEnv by scnLgtRebuildSlotColors.
struct CScnEnvLgtCtrlLgtSum {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

// scnLgtRebuildSlotColors's working accumulator: same 4-float layout as LgtSum plus
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

// Front half shared by the ranking rows scanned by scnLgtRankGateLights: flags,
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

// 4-byte ranked-light id pair walked by scnLgtRankGateLights phase 1: the u16
// selector picks the control sub-table (nonzero -> mBaseD/0x64-stride,
// zero -> mBaseC/0x50-stride) and the u16 index selects the row.
struct CScnEnvLgtCtrlLgtIdPair {
    u16 sel;   // +0x00
    u16 idx;   // +0x02
};

// Gate blob (self ctor +0x38 slot) consumed by scnLgtRecomputeSlotField and
// scnLgtRankGateLights: two ranked-light phases plus the row-base offsets.
struct CScnEnvLgtCtrlLgtGate {
    u8 _00[0x10];
    u32 mIdsOff;    // +0x10 word offset (*2 -> byte) into the +0x3C u16 array
    u32 mCount2;    // +0x14 phase-2 element count
    u32 mOff64;     // +0x18 dual-use: 0x64-row base into +0x2C; *2 into +0x3C ids
    u32 mCount1;    // +0x1C phase-1 element count
    u32 mOff50;     // +0x20 0x50-row base into +0x2C
};

// 0x20-byte vec4 rows walked by scnLgtRankGateLights phase 2.
struct CScnEnvLgtCtrlRow20 {
    f32 x;   // +0x00
    f32 y;   // +0x04
    f32 z;   // +0x08
    f32 w;   // +0x0C
};

// 0x40-byte opaque CLight slot (cursor arithmetic in scnLgtRecomputeSlotField).
struct CScnEnvLgtLightSlot {
    u8 _00[0x40];
};

// Virtual-light manager view used by scnLgtUpdateSceneLights.
struct CScnEnvLgtMgrView {
    u8 _00[0xD0];
    u8 field_0xD0;    // +0xD0 enable byte
    u8 _D1[0x3B];
    f32 field_0x10C[3];  // +0x10C color triple
};

// Owner +0x78 is the real CScnFogMan (CScnFogMan.hpp); scnLgtUpdateSceneLights
// stores its +0x24 timeout word.

// Resource-blob front word poked by scnLgtUpdateSceneLights.
struct CScnEnvLgtResFront {
    u8 _00[0x50];
    u32 field_0x50;
};

// Ranked-row arrays used by scnLgtRecomputeSlotField's second phase (bases from the
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

// Cross-TU imports used by scnLgtUpdateSceneLights / scnLgtRecomputeSlotField.
extern "C" void FogManSetValue08(u8* fogMan, int flag);
extern "C" void scnVlBlendStep(CScnVirtualLight* mgr, int flag);
// Retail ABI here passes four GPRs plus one FP arg (f1); f2 is left unset
// at this call site.
extern "C" int ScnRes_VertRayForward_E398(ml::CVec3* req, u32 a, u32 b, u32 c, f32 d);
extern "C" int ScnRes_HasHighFlagEntry_E5AC();
extern "C" int ScnRes_HasFlaggedEntry_E5A0(int flag);

// Shared .sdata2 constants and .data assert strings for the light-refresh
// helpers (scnLgtPushSlotLights / scnLgtRecomputeSlotField / scnLgtRankGateLights).
extern const float kLgtRayBias;
extern const float kLgtFogTypeScale;
extern const float kLgtSpotFalloff;
extern const float kLgtRankWeight;
extern const float kLgtRankThreshold;
extern const f32 kLgtRankGateValue[2];
extern char lbl_eu_80526324[];
extern char lbl_eu_80526300[];

// Same-TU helpers (definitions below).
extern "C" void scnLgtRankGateLights(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtGate* gate);
extern "C" void scnLgtPushSlotLights(CScnEnvLgtCtrl* self,
                                                   CLight* lights, int flag);
extern "C" void scnLgtRebuildSlotColors(CScnEnvLgtCtrl* self,
                                                   CLightEnv* env);
extern "C" void scnLgtUpdateSceneLights(CScnEnvLgtCtrl* self);

// Camera-item lookup is the CScnCameraMan member findItem(s32) (see
// CScnCameraMan.hpp). The retail call sites in scnLgtBindCamera /
// scnLgtDispatchEnv / scnLgtUpdateSceneLights resolve it through the camera-work
// blob at owner+0x68 (the CScn mCamWork slot) with the camera id at +0x34;
// this header keeps only the opaque item forward declaration, and the TU casts
// the blob to CScnCameraMan*.
struct CScnCameraItem;  // CScnCameraMan.hpp (call sites: CScnEnvLgtCtrl.cpp)

// Cross-TU light-env helpers used by scnLgtDispatchEnv. The full declarations
// live in CScnVirtualLight.hpp, which cannot be included here (its
// LightCtlSetAttnMode signature differs from this header's); these identical
// redeclarations keep the call relocs flat.
extern "C" void selectLightSlot(CLightEnv* self, u32 idx);
extern "C" void resetLightSlotIndex(void* self);
extern "C" void blendSlotAmbient(CLightEnv* self, const ml::CVec3* v);
extern "C" void scnVlApplyDir4C(CScnVirtualLight* self, const ml::CVec4* src, f32 value);
extern "C" void scnVlApplyDir6C(CScnVirtualLight* self, const ml::CVec4* src, f32 value);

// scnLgtDispatchEnv (us-804c5494): per-frame env-light dispatch. The retail
// call site (CScnVirtualLight.cpp) passes the +0x7C pool sub-object (a
// CScnEnvLgtCtrl) plus the CLightEnv. C linkage keeps the flat retail name.
extern "C" void scnLgtDispatchEnv(CScnVirtualLightSub* sub, CLightEnv* env);

// Same-TU light-slot refresh called by scnLgtApplyLightCmdScaled. extern "C" keeps the
// flat retail name on the call reloc (the definition below matches).
extern "C" int scnLgtFillLightVec4(CScnEnvLgtCtrl* self, CScnEnvLgtCtrlLgtVec4* out,
                             CScnEnvLgtCtrlLgtParamEntry* ctl, float f1);

// Light-slot refresh (same-TU function, retail flat name): recomputes one
// CLightEnv slot field. Signature from the scnLgtApplyAmbColor call site
// (self, light, slot index, reset flag) -> new slot field value. extern "C"
// keeps the flat retail name (typed-param globals get C++-mangled); -inline
// noauto keeps the call out-of-line (the stub body would otherwise be inlined).
extern "C" u32 scnLgtRecomputeSlotField(CScnEnvLgtCtrl* self,
                                                   CLight* light, u32 slot,
                                                   u32 reset);

// .rodata u32 tables indexed by the entry walkers' u16 words
// (scnLgtFillTargetBack field_0x06; scnLgtFillTargetFront field_0x0A/0x0C via C0 and
// field_0x10 via D0).
extern const u32 sLgtTargetModeTable[4];
extern const u32 sLgtTargetMaskTable[4];

// .sdata2 fog constants for scnLgtRefreshLightMan: the 4-byte color word at AFE0
// (stored byte-wise into FogData::color) and the startz/endz/nearz/farz
// float pair at AFE4/AFE8. The color is typed GXColor so the source can copy
// it as a struct (single word load, member-wise byte stores) like retail.
extern const GXColor sLgtFogColorWhite;
extern const float kLgtFogNearDefault;
extern const float kLgtFogFarDefault;

// Light-target flag byte toggle (defined in CScnBlend.cpp as a flat C-name
// function; toggles bit 1 of the target's +0x54 flags word).
extern "C" void Blend_SetFlag2(u8* self, u32 enable);

// Same-TU entry-walkers dispatched by the type-scan functions (separate
// targets). extern "C" keeps the flat retail name on the call-site reloc
// (typed-param globals would get C++-mangled); -inline noauto keeps the stub calls
// out-of-line (the stub bodies would otherwise be inlined, collapsing the
// retail `bl`). `row` selects the source row (0x20-stride from +0x38). The
// type-scan callers do not pass it (retail emits a 2-arg call, leaving r5
// undefined), so there is deliberately no default argument - callers pass a
// value already resident in the row-argument register instead.
extern "C" int scnLgtFillTargetBack(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* arg,
                                                    int row);
extern "C" int scnLgtFillTargetFront(u8* entry,
                                                    CScnEnvLgtCtrlLgtTarget* arg,
                                                    int row);

// 8-byte gradient entry: time bound + color, sampled by scnLgtSampleGradColor.
struct CScnEnvLgtCtrlGradEntry {
    f32 time;   // +0x00
    f32 color;  // +0x04
};

// Time/gradient lookup table read by scnLgtSampleGradColor: min/max bounds, an
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
extern const float kLgtFadeZero;

// .sdata2 row-interpolation blend factor used by the scnLgtFillTargetBack /
// scnLgtFillTargetFront entry walkers (retail `lfs f1, kLgtFadeWeight@sda21`).
extern const float kLgtFadeWeight;

// Shared .sdata2 float constants used by scnLgtSetFlashSlot / scnLgtFillLightVec4
// (the 1.0 color value stored to the +0xC0/+0xC4 pair and the output's
// +0x0C word).
extern const float kLgtOneShared;

// .sdata2 light-blend constants used by scnLgtPushSlotBlend (ambient/diffuse
// coefficients and blend bounds) and scnLgtDispatchEnv (slot color defaults).
extern const float kLgtBlendZero;
extern const float kLgtBlendOne;
extern const float kLgtSmoothStep;
extern const float kLgtBlendWeightHi;

// .sdata interpolation bounds consulted by scnLgtPushSlotBlend against the +0x3C
// word (signed compares in retail).
extern "C" u32 sLgtBlendRiseStart;  // .sdata raw words (defined in .cpp)
extern "C" u32 sLgtBlendRiseEnd;
extern "C" u32 sLgtBlendFallStart;
extern "C" u32 sLgtBlendFallEnd;

// Cubic-Hermite basis coefficients used by scnLgtSampleGradHermite's keyframe
// interpolation (2, 3, 1, -2 in retail .sdata2).
extern const float kHermiteThree;   // 3.0  (t^3 coef in h00)
extern const float kHermiteTwo;   // 2.0  (t^2 coef in h10)
extern const float kLgtFadeOne;   // 1.0 (per-slot fade scales)
extern const float kHermiteNegTwo;   // -2.0 (t^3 coef in h01)

// 16-byte keyframe entry walked by scnLgtSampleGradHermite: time plus three sampled
// values; the interpolator blends prev.a/prev.b (offsets 4/8) with
// cur.a/cur.c (offsets 4/12) using the Hermite basis.
struct CScnEnvLgtCtrlGrad16Entry {
    f32 time;   // +0x00
    f32 a;      // +0x04
    f32 b;      // +0x08
    f32 c;      // +0x0C
};

// Panic assert strings referenced by scnLgtPushFogNone (retail .data symbols).
extern char lbl_eu_8056EC00[];
extern char lbl_eu_8056EBE0[];

// Panic strings referenced by scnLgtPushFogView0 / scnLgtPushFogView1 (retail .data).
extern char lbl_eu_8056EC60[];
extern char lbl_eu_8056EC40[];
extern char lbl_eu_8056EC30[];
extern char lbl_eu_8056EC10[];

// Panic strings referenced by scnLgtRefreshLightMan's fog block (retail .data).
extern char sLgtAssertFile[];
extern char sLgtAssertMsg[];

// CScnEnvLgtCtrl constructor (retail symbol is the unmangled short form).
// Builds the controller from an 'STGL' resource header plus an opaque arg.
// -inline noauto matches the retail out-of-line call from scnLgtFactoryCreate.
extern "C" CScnEnvLgtCtrl* __ct__CScnEnvLgtCtrl(
    CScnEnvLgtCtrl* self, const u32* data, void* arg);

// 'STGL' env-light factory (defined in this TU; separate target
// us-804cabcc): validates the magic word, allocates the 0x168-byte object
// and runs the constructor. extern "C" keeps the flat retail name on the
// call-site reloc in scnLgtCreateEnvController; -inline noauto keeps the retail out-of-line
// `bl scnLgtFactoryCreate` there (the factory body would otherwise be inlined).
extern "C" CScnEnvLgtCtrl* scnLgtFactoryCreate(u32 handle,
                                                               const u32* data,
                                                               void* arg);

// --- scnLgtSetBlendBloom / scnLgtRemoveLgtItem cross-TU imports (retail flat names) ---

// Same-TU scene-root helper (stub defined in this TU). extern "C" keeps the
// flat retail name on the call reloc; -inline noauto prevents the empty stub body
// from folding into the call sites.
extern "C" void scnLgtResetLightState(CScnEnvLgtCtrl* self);

// The blend/bloom ctors are owned by their own TUs (CScnBlend.cpp /
// CScnBloom.cpp, NonMatching); scnLgtSetBlendBloom only emits the ctor calls with
// the retail short-form symbols.
extern "C" CScnEnvLgtBlend* __ct__CScnBlend(CScnEnvLgtBlend* self);
extern "C" CScnEnvLgtBloom* __ct__CScnBloom(CScnEnvLgtBloom* self);

// Cross-TU helpers (definitions live in CScn.cpp / CScnFilterMan.cpp).
extern "C" CScnEnvLgtCtrlLgtParamCtl* Scn_CallUnk8C_V11(CScnItemModelNw4rOwner* owner);
extern "C" void ScnFilterMan_appendFilter(u8* man, CScnEnvLgtBlend* filter);
extern "C" void ScnFilterMan_toggleFilter(u8* man, CScnEnvLgtBlend* filter);

// Light-render helpers called by scnLgtApplyLightCmdScaled / scnLgtApplyLightCmd /
// scnLgtApplyLightCmdEx (defined in the TU before this one; only the call-site ABI
// is needed here).
extern "C" void LightCtlSetAttnMode(CScnEnvLgtLightView* view, u32 flag);
extern "C" void LightCtlSetPos(CScnEnvLgtLightView* view, const u8* p);
extern "C" void LightCtlSetDir(CScnEnvLgtLightView* view, const u8* p);
extern "C" void LightCtlSetColor(CScnEnvLgtLightView* view, CScnEnvLgtCtrlLgtVec4* out);
extern "C" void LightCtlSetEnable(CScnEnvLgtLightView* view, u32 flag);
extern "C" void LightCtlInitDistAttn(CScnEnvLgtLightView* view, u32 a, f32 b, f32 c);
extern "C" void LightCtlInitSpot(CScnEnvLgtLightView* view, u32 a, f32 b);

// 16-byte light-data block copied by scnLgtReadLightDataWords (self+0x54 -> out).
// Kept as a named struct so the copy sites use typed pointers: MWCC only
// emits grouped load-all/store-all for typed sources (a void* param forces
// interleaved r0-reuse pairs, cf. MWCC_CASES 3-word struct copy).
struct CScnEnvLgtCtrlLgtData {
    u32 field_0x00;   // +0x00
    u32 field_0x04;   // +0x04
    u32 field_0x08;   // +0x08
    u32 field_0x0C;   // +0x0c
};

// Typed view over the CScnEnvLgtCtrl layout used by scnLgtReadLightDataWords (u32 copy
// of the +0x54 block) and scnLgtCopyAmbBase (float copy of +0x54 or +0x64 into
// CScnEnvLgtData::mAmbColorBase, selected by flags bits 0x200 / 0x400).
struct CScnEnvLgtCtrlLgtView {
    u8 _pad[0x04];                     // +0x00
    u32 flags;                         // +0x04 (bit 0x200 = light data at 0x54; bit 0x400 = data at 0x64)
    u8 _pad2[0x4C];                    // +0x08
    union {
        CScnEnvLgtCtrlLgtData data;    // +0x54 (u32 view, scnLgtReadLightDataWords)
        f32 field_0x54[4];             // +0x54 (float view, scnLgtCopyAmbBase)
        CScnEnvLgtPairU field_0x54_pairs[2];  // +0x54 (pair view, scnLgtRebuildSlotColors)
    };
    union {
        f32 field_0x64[4];             // +0x64 (float view, scnLgtCopyAmbBase)
        CScnEnvLgtPairU field_0x64_pairs[2];  // +0x64 (pair view, scnLgtRebuildSlotColors)
    };
    f32 field_0x68;
    f32 field_0x6C;
};


// --- __ct__CScnEnvLgtCtrl ctor helper types ---
// 3-word .bss blob copied by the ctor into +0x90..0x98 and +0x9C..0xA4.
// Its real name is ml::CCol3::black (declared in monolib/math/CCol3.hpp).

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
// entries at +0x10 handed to scnLgtBuildHeader.
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
// scnLgtRefreshLightRows (copy pass): flags word at +0x04 (bits 0x3C00 gate) and the
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

// Fog-gate entry walked by scnLgtRefreshLightRows (+0x3C stride): flags at +0x04
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
    u32 field_0x00;   // +0x00 fog color red (word-copied; float-read by the fog push)
    u32 field_0x04; u32 field_0x08;   // +0x04/+0x08 green/blue
    f32 field_0x0C;   // +0x0C fog start
    f32 field_0x10;   // +0x10 fog end
    u32 field_0x14;   // +0x14 saved GXFogType word (read back as a pointer)
    u16 field_0x18;   // +0x18 flags (bit 0 = fog type saved)
};

// Runtime item-array view of CScnEnvLgtCtrl used by scnLgtRefreshLightRows: the ctor
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

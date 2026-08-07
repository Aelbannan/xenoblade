#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>

// Global singleton object referenced by the CfCamEvent cam logic
// (address lbl_eu_80663DF0, accessed via sda21 as a pointer).
struct CfCamEventGlobal {
    u8  _000[0x38];
    u32 field_0x38;    // 0x38 - pointer/state word
    u8  _03C[0x3E - 0x3C];
    s16 field_0x3E;    // 0x3E
    u8  _040[0x46 - 0x40];
    u8  field_0x46;    // 0x46
};

// Sub-object embedded at manager offset 0x1F4.
// A single effect slot object referenced by the manager's 3-element slot
// array at offset 0.  Destructor is virtual (defined in another TU).
class CfCamEventSlot {
public:
    virtual ~CfCamEventSlot();
};

// Single element of the manager's shake table (0x14 bytes each,
// element 0 base sits at manager offset 0x1F8).
struct CfCamEventElem {
    f32 x0;      // +0x00
    f32 x4;      // +0x04 - value that gets shaken toward the previous element
    f32 x8;      // +0x08
    u16 id;      // +0x0C
    u16 c;       // +0x0E
    u16 d;       // +0x10
    u8  _12[2];  // +0x12
};

struct CfCamEventSub1F4 {
    u8 _000[0x04];            // 0x1F4 + 0x00
    CfCamEventElem elems[42]; // element array base at 0x1F4 + 0x04
    u8 _34C[0x356 - 0x34C];
    u8 field_0x162;    // 0x1F4 + 0x162 = 0x356
    u8 field_0x163;    // 0x1F4 + 0x163 = 0x357
    u8 _358[0x35A - 0x358];
    s16 field_0x166;   // 0x1F4 + 0x166 = 0x35A (shake table element count)
    u8 _168[0x2DA - 0x168];
    u8 field_0x2DA;    // 0x1F4 + 0x2DA = 0x4CE
    u8 field_0x2DB;    // 0x1F4 + 0x2DB = 0x4CF
};

// Secondary shake table located just past sub_0x1F4 at manager offset
// 0x4D0. (Modeled separately; the canonical third table starts at 0x36C.)
struct CfCamEventSub4D0 {
    u8 _00[0x02];
    s16 count;         // 0x4D2
    u8 _04[0x04];
    CfCamEventElem elems[42];  // base 0x4D8
};

// Generic shake table (used at manager offsets 0x7C and 0x36C): element
// array from +0, a base-copy anchor at +0x154, count at +0x166.
struct CfCamShakeTab {
    CfCamEventElem elems[16];      // +0x00, 16*0x14 = 0x140
    u8 _140[0x154 - 0x140];
    f32 baseX;                     // 0x154
    f32 baseY;                     // 0x158
    f32 baseZ;                     // 0x15C
    u8 _160[0x166 - 0x160];
    s16 count;                     // 0x166
};

// First shake table at manager offset 0x7C: same shape as CfCamShakeTab,
// but its flag bytes at +0x162/+0x163 double as the manager's 0x1DE/0x1DF
// and its count at +0x166 as the manager's 0x1E2.
struct CfCamEventTab7C {
    CfCamEventElem elems[16];      // 0x00..0x140
    u8 _140[0x154 - 0x140];
    f32 baseX;                     // 0x154
    f32 baseY;                     // 0x158
    f32 baseZ;                     // 0x15C
    u8 _160[0x162 - 0x160];
    u8 flag0;                      // 0x162 == 0x1DE
    u8 flag1;                      // 0x163 == 0x1DF
    u8 _164[0x166 - 0x164];
    s16 count;                     // 0x166 == 0x1E2
};

// Camera event manager - only forward-declared elsewhere; the complete layout
// is reconstructed here from the cam-event accessors.
class CfCamEventManager {
public:
    CfCamEventSlot* slots[3];  // 0x00 - 3 effect slot pointers
    u8  _00C[0x10 - 0x0C];
    u32 field_0x10;     // 0x10
    u8  _14[0x24 - 0x14];
    u32 field_0x24;     // 0x24
    u8  _28[0x34 - 0x28];
    u32 field_0x34;     // 0x34 - pointer/result slot
    u32 field_0x38;     // 0x38
    u8  _03C[0x3E - 0x3C];
    s16 field_0x3E;     // 0x3E - cam state id
    u16 field_0x40;     // 0x40
    u8  _042[0x44 - 0x42];
    u16 field_0x44;     // 0x44
    u8  field_0x46;     // 0x46
    u8  field_0x47;     // 0x47
    u32 field_0x48;     // 0x48
    u8  _4C[0x50 - 0x4C];
    u32 field_0x50;     // 0x50 - flags word
    u32 field_0x54;     // 0x54
    u32 field_0x58;     // 0x58
    u32 field_0x5C;     // 0x5C
    u32 field_0x60;     // 0x60
    u32 field_0x64;     // 0x64
    u32 field_0x68;     // 0x68
    u32 field_0x6C;     // 0x6C
    u32 field_0x70;     // 0x70
    u32 field_0x74;     // 0x74
    f32 field_0x78;     // 0x78
    CfCamEventTab7C tab0;       // 0x7C (decl has 0x7C..0x1E4)
    u8  _1E4[0x1F4 - 0x1E4];
    CfCamEventSub1F4 sub_0x1F4;  // 0x1F4
    CfCamShakeTab     sub_0x36C;  // 0x36C
};

// Body object returned by the camera-source virtual accessors: three f32
// aim/anchor coordinates at offsets 0x0C/0x1C/0x2C.
struct CinemAimBody {
    u8  _0[0x0C];
    f32 x;    // 0x0C
    u8  _10[0x1C - 0x10];
    f32 y;    // 0x1C
    u8  _20[0x2C - 0x20];
    f32 z;    // 0x2C
};

// Vector-returning virtual accessor result.
struct CinemVecOut {
    ml::CVec3 v;  // 0x00
};

// Virtual function table for the camera-position source object.
struct CinemSrcVtbl {
    void*            p00[0x2B];            // 0x00 .. 0xAC
    CinemVecOut* (*fn_0xAC)(void* self);   // 0xAC
    void*            rB0[0x1E];            // 0xB0 .. 0x128
    CinemAimBody* (*fn_0x128)(void* self); // 0x128
    CinemAimBody* (*fn_0x12C)(void* self, int a);  // 0x12C
};

// Camera position source: vtable plus a couple of flag/compare words.
struct CinemCamSrc {
    CinemSrcVtbl* vtable;  // 0x00
    u8   _04[0x64 - 0x04];
    u32  field_0x64;       // 0x64 - flags (0x2 = clamp, 0x4 = snap)
    u8   _68[0x70 - 0x68];
    u32  field_0x70;       // 0x70 - compare value
};

// Sub-object (a CCharVoice) embedded at source offset 0x3E9C.
// Holds a vtable with fns at 0x4C (returns object ptr) and 0xAC
// (returns a camera aim/source vector).
struct CamEventVoiceVtbl {
    void*           p00[0x4C / 4];
    void* (*fn_0x4C)(void* self);          // 0x4C
    void*           r50[(0xAC - 0x50) / 4];
    CinemVecOut* (*fn_0xAC)(void* self);   // 0xAC
    void*           rB0[8];
};

struct CamEventVoice {
    CamEventVoiceVtbl* vtable;  // 0x00
};

// Body returned by the source vtable fn at 0x298; field +0x04 is read.
struct CamEventBody {
    void* field_0x00;
    void* field_0x04;
};

// Source object returned by func_8016FE34. Has a vtable, a field at
// 0x3F10, and an embedded voice sub-object at 0x3E9C.
struct CamEventSrcVtbl {
    void*           p00[0x298 / 4];
    CamEventBody* (*fn_0x298)(void* self);  // 0x298
};

struct CamEventSrc {
    CamEventSrcVtbl* vtable;  // 0x00
    u8 _04[0x3E9C - 0x04];
    CamEventVoice voice;      // 0x3E9C
    u8 _3EA0[0x3F10 - 0x3EA0];
    void*           field_0x3F10; // 0x3F10
};

// Camera-source object used by the depth computation (targets 2/3).
// vtable fns at 0xAC (returns a vector) and 0x5B4 (returns a scalar);
// also embeds a voice with the same 0xAC accessor.
struct CamCamSrcVtbl {
    void*           p00[0xAC / 4];
    CinemVecOut* (*fn_0xAC)(void* self);   // 0xAC
    void*           rB0[(0x5B4 - 0xB0) / 4];
    f32 (*fn_0x5B4)(void* self);           // 0x5B4
};

struct CamCamSrc {
    CamCamSrcVtbl* vtable;  // 0x00
    u8 _04[0x3E9C - 0x04];
    CamEventVoice voice;    // 0x3E9C
};

// Object returned by the game-manager factory used in the cam-event update;
// only its +0x04 flags word is inspected here.
struct CfDynMgr {
    void* m_vtable;
    u32   m_field04;   // 0x04
};

// Cam data table stored at the global lbl_eu_80570C90 / 0x80570C90.
struct CfCamDataTable {
    u16 h00;   // 0x00
    u16 h02;
    u16 h04;
    u16 h06;
    u16 h08;
    u16 h0A;
    u16 h0C;
    u16 h0E;
    u16 h10;
    u8  _12[0x14 - 0x12];
    f32 f14;   // 0x14
    f32 f18;   // 0x18
    f32 f1C;
    f32 f20;
    f32 f24;
    f32 f28;
    f32 f2C;
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
    f32 f40;
    f32 f44;
    f32 f48;
};
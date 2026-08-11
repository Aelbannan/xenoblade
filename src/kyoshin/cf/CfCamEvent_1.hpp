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

// Single element of the manager's internal shake table (0x14 bytes each,
// element 0 base sits at manager offset 0x1F8). Only +0x04 is used for the
// shake update; the rest of the stride is padding.
struct CfCamEventElem {
    f32 x0;       // +0x00
    f32 x4;       // +0x04 - value that gets shaken toward the previous element
    f32 x8;       // +0x08
    u16 id;       // +0x0C - time point
    u16 c;        // +0x0E - waveform kind (1=bell, 2=bump, 4=sine, 3=clamp-sine)
    u16 d;        // +0x10
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
// 0x4D0. Count stored at +0x02 (=0x4D2); element stride is 0x14.
struct CfCamEventSub4D0 {
    u8 _00[0x02];
    s16 count;         // 0x4D2
    u8 _04[0x04];
    CfCamEventElem elems[42];  // base 0x4D8
};

// Camera event manager - only forward-declared elsewhere; the complete layout
// is reconstructed here from the cam-event accessors.
// Shake table 0 at manager offset 0x7C (16-entry cap).
struct CfCamEventTable {
    s16 count;                   // +0x00
    u8  _02[0x04 - 0x02];
    CfCamEventElem elems[16];    // +0x04
    f32 baseX;                   // +0x144
    f32 baseY;                   // +0x148
    f32 baseZ;                   // +0x14C
    u8  flag0;                   // +0x150
    u8  flag1;                   // +0x151
};

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
    u32 field_0x40;     // 0x40
    u32 field_0x44;     // 0x44
    u8  field_0x46;     // 0x46
    u8  field_0x47;     // 0x47
    u32 field_0x48;     // 0x48 - flag/state word
    u8  _04C[0x50 - 0x4C];
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
    CfCamEventTable tab0;       // 0x7C - shake table 0 (16-entry cap)
    u8  _1CC[0x1DE - 0x1CC];
    u8  field_0x1DE;    // 0x1DE
    u8  field_0x1DF;    // 0x1DF
    u8  _1E0[0x1E2 - 0x1E0];
    s16 field_0x1E2;    // 0x1E2
    u8  _1E4[0x1F4 - 0x1E4];
    CfCamEventSub1F4 sub_0x1F4;  // 0x1F4
    CfCamEventTable sub_0x36C;   // 0x36C - shake table 2 (16-entry cap)
    CfCamEventSub4D0 sub_0x4D0;  // 0x4D0
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
// Holds a vtable whose fn at offset 0x4C returns an object pointer.
struct CamEventVoiceVtbl {
    void*         p00[0x4C / 4];
    void* (*fn_0x4C)(void* self);  // 0x4C
    void*         r50[8];
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
// ── WIP scaffolding (func_80074F4C / func_80075934) ───────────────────────
// Shake-table element/state used by the WIP shake-advance reconstruction.
// Offsets confirmed from retail func_80074F4C: +0x154/+0x158 s32, +0x162
// flag_active, +0x164 flag_finish, +0x168 index(s16), +0x170 val(f32).
// Tail fields (end/base*/elems) placed after +0x170 pending full recovery.
struct CfCamShakeState {
    u8  _000[0x154];
    s32 count;          // +0x154
    s32 field_0x158;    // +0x158
    s16 field_0x160;    // +0x160
    u8  flag_active;    // +0x162
    u8  _163;           // +0x163
    u8  flag_finish;    // +0x164
    s16 field_0x166;    // +0x166
    s16 index;          // +0x168
    u8  _16A[0x170 - 0x16A];
    f32 val;            // +0x170
    f32 end;            // +0x174
    f32 baseX;          // +0x178
    f32 baseY;          // +0x17C
    f32 baseZ;          // +0x180
    CfCamEventElem elems[64];  // +0x184
};

// Source object used by func_80075934: vtable (fn_0x5B4) aliasing an embedded
// CinemCamSrc (voice) at +0x00.
struct CamCamSrcVtbl {
    void* p00[0xAC / 4];
    CinemVecOut* (*fn_0xAC)(void* self);       // 0xAC
    void* rB0[((0x5B4 - 0xB0) / 4)];
    f32   (*fn_0x5B4)(void* self);             // 0x5B4
};

struct CamCamSrc {
    union {
        CamCamSrcVtbl* vtable;   // +0x00
        CinemCamSrc voice;       // +0x00 (cast target of func_80075934)
    };
};

// Minimal WIP scaffolding: dynamic manager returned by func_800821F8
// (CfGameManager). Only +0x04 is read by the cam-event update.
struct CfDynMgr {
    u8  _00[0x04];
    u32 m_field04;   // +0x04
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" CfCamDataTable lbl_eu_80570C90;
extern "C" CfCamEventGlobal* lbl_eu_80663DF0;
extern "C" void* func_8016FE34(void* source);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" void* func_800B708C__Fi(int id);
extern "C" void* func_800BBC0C();
extern "C" f32   lbl_eu_8066A1F8;
extern "C" f32   lbl_eu_8066A1FC;
extern "C" f32   lbl_eu_8066641C;
extern "C" f32   lbl_eu_80666418;
extern "C" f32   lbl_eu_80666420;
extern "C" f32   lbl_eu_80666428;
extern "C" f32   lbl_eu_80666448;
extern "C" f32   lbl_eu_80666454;
extern "C" f32   lbl_eu_8066A208;
extern const float lbl_eu_8066A210;
extern "C" void* lbl_eu_806640BC;
extern "C" u8    lbl_eu_804FB5D0[];
extern "C" int   func_8003B1EC(void* self);
extern "C" int   getBdatStringColumnValue(void* a, const u8* b, int c);
extern "C" f32   Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
extern "C" int   CfRes_getD80Flag();
extern "C" f32   func_80496288();
extern "C" void  func_800756D0(ml::CVec3* out, CinemCamSrc* src);
extern "C" int __declspec(noinline) func_800755BC(CfCamEventManager* mgr, u32 idx);
extern "C" f32 lbl_eu_8066642C;
extern "C" f32 lbl_eu_80666430;
extern "C" f32 lbl_eu_8066A200;
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" void func_800A41BC(f32* out, CfCamEventElem* a, CfCamEventElem* b,
                              CfCamEventElem* c, CfCamEventElem* d);
extern "C" void func_800A40E8(f32* out, CfCamEventElem* a, CfCamEventElem* b);
extern "C" int func_8024125C(int state, int val);
extern "C" int func_80241344(int state, int val);
extern "C" int func_80240C98(int state, int arg0, int arg1);
extern "C" u32 func_800AA300(int a, int b, int c);

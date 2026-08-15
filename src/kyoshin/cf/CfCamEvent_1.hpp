#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>

class UnkClass_800821F8;  // fwd decl: only pointer-position use (func_800821F8__Q22cf13CfGameManagerFv result)

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
    // Only the virtual dtor: MWCC emits [typeinfo, dtor, deleting-dtor], so
    // `delete` dispatches vtable+8 exactly like retail.
    virtual ~CfCamEventSlot();
};

// Slot vtable view used by func_8007BAFC: dispatches the virtual fn at
// vtable+0x20 (a per-slot update).
struct CfCamEventSlotVtbl {
    void*         p00[0x20 / 4];
    void* (*fn_0x20)(void* self);  // 0x20
    void*         r24[8];
};

// Slot view used by func_8007BAFC: vtable plus the block pointer at +0x0C
// that holds the camera vector data (12 words at 0x9C..0xC8).
struct CfCamEventSlotView {
    CfCamEventSlotVtbl* vtable;  // 0x00
    u8 _04[0x0C - 0x04];
    void* field_0x0C;            // 0x0C - camera vector block
};

// Full slot vtable used by the cam-event setup paths (func_800784A0 / the
// func_8007B0C8 tail): camera-vector accessors at 0x10/0x1C/0x34 return a
// CVec3* whose words are copied as bit patterns, 0x58 returns f32, 0x20 is
// the per-slot update used by func_8007BAFC and 0x5C takes an extra arg.
struct CfCamEventSlotVtblFull {
    void*         p00[0x10 / 4];          // 0x00 .. 0x0C
    ml::CVec3* (*fn_0x10)(void* self);    // 0x10
    void*         r14[2];                 // 0x14, 0x18
    ml::CVec3* (*fn_0x1C)(void* self);    // 0x1C
    void* (*fn_0x20)(void* self);         // 0x20
    void*         r24[4];                 // 0x24 .. 0x30
    ml::CVec3* (*fn_0x34)(void* self);    // 0x34
    void*         r38[8];                 // 0x38 .. 0x54
    f32   (*fn_0x58)(void* self);         // 0x58
    void* (*fn_0x5C)(void* self, void* arg);  // 0x5C
};

// Slot pointer view: vtable at +0x00 (see CfCamEventSlotVtblFull).
struct CfCamEventSlotObj {
    CfCamEventSlotVtblFull* vtable;
};

// Vector block read by func_8007BAFC: twelve f32 words at 0x9C..0xC8. The
// first two triplets (0x9C / 0xAC) are self-dotted (squared lengths) and
// validated through acos against a threshold.
struct CamEventVecBlock {
    u8  _9C[0x9C - 0x00];
    f32 f_9C;   // 0x9C
    f32 f_A0;   // 0xA0
    f32 f_A4;   // 0xA4
    f32 f_A8;   // 0xA8
    f32 f_AC;   // 0xAC
    f32 f_B0;   // 0xB0
    f32 f_B4;   // 0xB4
    f32 f_B8;   // 0xB8
    f32 f_BC;   // 0xBC
    f32 f_C0;   // 0xC0
    f32 f_C4;   // 0xC4
    f32 f_C8;   // 0xC8
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

// Shake table payload (0x160 bytes): elements start AT the base (no count
// word - the count lives in the enclosing state at +0x166), followed by the
// base anchor triplet at +0x154. The union payload of CfCamEventShakeUnit and
// the manager's tab0 state both start with this shape.
struct CfCamEventTable {
    CfCamEventElem elems[16];    // +0x00
    u8  _140[0x154 - 0x140];     // +0x140 gap (padding before the base anchor)
    f32 baseX;                   // +0x154
    f32 baseY;                   // +0x158
    f32 baseZ;                   // +0x15C
};

// Full 0x178-byte shake state as embedded at the manager's tab0 (0x7C):
// table payload + the tail flags/counts/values at +0x160..+0x178. Same
// layout as CfCamEventShakeUnit and CfCamShakeState; the manager's legacy
// field_0x1DC/0x1DE/0x1E2/... names map onto this tail.
struct CfCamEventShakeTable {
    CfCamEventElem elems[16];    // +0x00
    u8  _140[0x154 - 0x140];     // +0x140 gap
    f32 baseX;                   // +0x154
    f32 baseY;                   // +0x158
    f32 baseZ;                   // +0x15C
    u16 field_0x160;             // +0x160 (abs 0x1DC) - spline method word
    u8  flag_active;             // +0x162 (abs 0x1DE)
    u8  field_0x163;             // +0x163 (abs 0x1DF)
    u8  flag_finish;             // +0x164 (abs 0x1E0)
    u8  field_0x165;             // +0x165 (abs 0x1E1)
    s16 count;                   // +0x166 (abs 0x1E2) - shake element count
    s16 field_0x168;             // +0x168 (abs 0x1E4)
    f32 field_0x16C;             // +0x16C (abs 0x1E8)
    f32 field_0x170;             // +0x170 (abs 0x1EC)
    f32 field_0x174;             // +0x174 (abs 0x1F0)
};

// 0x178-byte shake unit; two instances at manager 0x1F4 / 0x36C. Unit 0
// carries the 16-entry shake-table element array at +0x00; unit 1 carries a
// full CfCamEventTable (count first) at +0x00. Both units share the tail
// fields at +0x160..+0x174 (u16 id, flag bytes, s16 counts, f32 triplet) -
// the manager ctor clears +0x162..+0x168 in a 0x178-stride loop.
struct CfCamEventShakeUnit {
    union {
        CfCamEventElem elems[16];   // +0x00 - unit0: shake table 1 elements
        CfCamEventTable tab;        // +0x00 - unit1: shake table 2
        u8 _raw[0x160];             // +0x00 opaque payload
    } u;
    u16 field_0x160;   // +0x160 (abs 0x354 / 0x4CC)
    u8  field_0x162;   // +0x162 (abs 0x356 / 0x4CE)
    u8  field_0x163;   // +0x163 (abs 0x357 / 0x4CF)
    u8  field_0x164;   // +0x164 (abs 0x358 / 0x4D0)
    u8  field_0x165;   // +0x165 (abs 0x359 / 0x4D1)
    s16 field_0x166;   // +0x166 (abs 0x35A / 0x4D2) shake element count
    s16 field_0x168;   // +0x168 (abs 0x35C / 0x4D4)
    f32 field_0x16C;   // +0x16C (abs 0x360 / 0x4D8)
    f32 field_0x170;   // +0x170 (abs 0x364 / 0x4DC)
    f32 field_0x174;   // +0x174 (abs 0x368 / 0x4E0)
};

// Element view used by func_80079D6C: the retail body treats its first
// argument as an element pointer (x4 at +0x04, previous element one stride
// back at -0x10) and also reads an s16 count through the same pointer at
// +0x4D2 (the caller passes the manager base, so +0x4D2 aliases
// sub_0x4D0.count).
struct CfCamEventShakeElem {
    f32 x0;                // +0x00
    f32 x4;                // +0x04 - value shaken toward the previous element
    f32 x8;                // +0x08
    u16 id;                // +0x0C
    u16 c;                 // +0x0E
    u16 d;                 // +0x10
    u8  _012[0x4D2 - 0x12];
    s16 count;             // +0x4D2
};

// Camera-data block read by func_80075674 (source: object from
// func_80496264). Three f32 triplets at 0x10C/0x118/0x138 and an f32 at
// 0x1E0 are copied into the manager's 0x54..0x78 range (bit patterns, so
// the triplet members are f32 to keep the lwz/stw copy schedule).
struct CfCamEventCopySrc {
    u8  _000[0x10C - 0x000];
    f32 f_10C;   // 0x10C -> mgr 0x54
    f32 f_110;   // 0x110 -> mgr 0x58
    f32 f_114;   // 0x114 -> mgr 0x5C
    f32 f_118;   // 0x118 -> mgr 0x6C
    f32 f_11C;   // 0x11C -> mgr 0x70
    f32 f_120;   // 0x120 -> mgr 0x74
    u8  _124[0x138 - 0x124];
    f32 f_138;   // 0x138 -> mgr 0x60
    f32 f_13C;   // 0x13C -> mgr 0x64
    f32 f_140;   // 0x140 -> mgr 0x68
    u8  _144[0x1E0 - 0x144];
    f32 f_1E0;   // 0x1E0 -> mgr 0x78
};

// Camera event manager - only forward-declared elsewhere; the complete layout
// is reconstructed here from the cam-event accessors.
// Shake table 0 at manager offset 0x7C (16-entry cap).
class CfCamEventManager {
public:
    CfCamEventManager();
    ~CfCamEventManager();

    CfCamEventSlot* slots[3];  // 0x00 - 3 effect slot pointers
    u8  _00C[0x10 - 0x0C];
    u32 field_0x10;      // 0x10
    u8  _14[0x24 - 0x14];
    u32 field_0x24;      // 0x24
    u8  _28[0x34 - 0x28];
    u32 field_0x34;      // 0x34 - pointer/result slot
    u32 field_0x38;     // 0x38
    s16 field_0x3C;     // 0x3C (retail lha)
    s16 field_0x3E;     // 0x3E - cam state id
    u16 field_0x40;     // 0x40
    u8  field_0x42;     // 0x42
    u8  field_0x43;     // 0x43
    u16 field_0x44;     // 0x44 (retail lhz)
    u8  field_0x46;     // 0x46
    u8  field_0x47;     // 0x47
    u32 field_0x48;     // 0x48 - flag/state word
    u32 field_0x4C;     // 0x4C
    u32 field_0x50;     // 0x50 - flags word
    ml::CVec3 field_0x54;       // 0x54
    ml::CVec3 field_0x60;       // 0x60
    ml::CVec3 field_0x6C;       // 0x6C
    f32 field_0x78;     // 0x78
    CfCamEventShakeTable tab0;  // 0x7C - full 0x178 shake state (table payload
                                // + tail flags/counts). The state tail doubles
                                // as the legacy field_0x1DC/0x1DE/0x1E2/...
                                // region of the manager.
    CfCamEventShakeUnit shake[2];  // 0x1F4 - units at 0x1F4 / 0x36C
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
// Holds a vtable whose fn at offset 0x4C returns an object pointer; the
// same vtable also dispatches the aim-vector accessor at 0xAC.
struct CamEventVoiceVtbl {
    void*         p00[0x4C / 4];
    void* (*fn_0x4C)(void* self);  // 0x4C
    void*         r50[0xAC - 0x50];  // 0x50 .. 0xA8
    void* (*fn_0xAC)(void* self);  // 0xAC
};

struct CamEventVoice {
    CamEventVoiceVtbl* vtable;  // 0x00
};

// Body returned by the source vtable fn at 0x298; field +0x04 is read by
// func_80076F88, while func_80076D8C reads the state word at +0x50, the
// halfword at +0x5E and the float at +0x60.
struct CamEventBody {
    void* field_0x00;
    void* field_0x04;
    u8    _08[0x50 - 0x08];
    u32   field_0x50;   // +0x50 - state/flag word
    u8    _54[0x5E - 0x54];
    u16   field_0x5E;   // +0x5E
    f32   field_0x60;   // +0x60
};

// Object stored in manager slot 1 (the follow-cam object): vtable with
// two-arg fns at 0x40 and 0x5C.
struct CamEventMgrSubVtbl {
    void* p00[0x40 / 4];
    void* (*fn_0x40)(void* self, void* arg);  // 0x40
    void* r44[0x5C - 0x44];                   // 0x44 .. 0x58
    void* (*fn_0x5C)(void* self, void* arg);  // 0x5C
};

struct CamEventMgrSub {
    CamEventMgrSubVtbl* vtable;
};

// Voice/aim object view: vtable at +0x00 and an integer word at +0x74 read
// by func_80076F88 (copied into the manager's 0x10/0x24 words).
struct CamEventAimObj {
    void* vtable;         // 0x00
    u8 _04[0x74 - 0x04];
    u32 field_0x74;       // 0x74
};

// View of the manager's aim-vector region (offsets 0x0C..0x34) used by
// func_80076F88. Kept as a separate view so the manager class itself keeps
// its original opaque gaps (MWCC's copy-lowering for the 0x54..0x78 range
// is sensitive to the struct layout).
struct CfCamEventAimRegion {
    void* field_0x0C;    // 0x0C
    u32   field_0x10;    // 0x10
    f32   field_0x14;    // 0x14
    f32   field_0x18;    // 0x18
    f32   field_0x1C;    // 0x1C
    void* field_0x20;    // 0x20
    u32   field_0x24;    // 0x24
    f32   field_0x28;    // 0x28
    f32   field_0x2C;    // 0x2C
    f32   field_0x30;    // 0x30
    u32   field_0x34;    // 0x34
};

struct CamEventTargetInfo {
    u8  _000[0x15E4];
    u32 field_0x15E4;   // +0x15E4
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
// Integer word view of a shake-table element used by func_80074F4C's
// base-anchor copy: retail copies the x0/x4/x8 words with lwz/stw (GPR
// moves, bit patterns), so this element view carries them as u32.
struct CfCamShakeElemRaw {
    u32 x0;      // +0x00
    u32 x4;      // +0x04
    u32 x8;      // +0x08
    u16 id;      // +0x0C
    u16 c;       // +0x0E
    u16 d;       // +0x10
};

// ── WIP scaffolding (func_80074F4C / func_80075934) ───────────────────────
// Shake-table element/state view used by the shake-advance reconstruction.
// Offsets confirmed from retail func_80074F4C: elems at +0x00 (0x14 stride,
// id at +0x0C, waveform kind c at +0x0E), base anchor at +0x154..0x15C
// (copied as integer words), spline-method word at +0x160 (read with lha),
// flag_active at +0x162, flag_finish at +0x164, element count (s16) at
// +0x166, current index (s16) at +0x168, running value at +0x170 and end
// value at +0x174. The same code runs against the manager's three shake
// tables (0x7C / 0x1F4 / 0x36C) through this shared view layout.
struct CfCamShakeState {
    CfCamShakeElemRaw elems[16];  // +0x00
    u8  _140[0x154 - 0x140];      // +0x140
    u32 baseX;                    // +0x154 - bit pattern (spline output)
    u32 baseY;                    // +0x158
    u32 baseZ;                    // +0x15C
    s16 field_0x160;              // +0x160 - spline method (0/1/2)
    u8  flag_active;              // +0x162
    u8  field_0x163;              // +0x163
    u8  flag_finish;              // +0x164
    u8  field_0x165;              // +0x165
    s16 count;                    // +0x166 - element count
    s16 index;                    // +0x168 - current element index
    u8  _16A[0x170 - 0x16A];      // +0x16A
    f32 val;                      // +0x170 - running value
    f32 end;                      // +0x174 - end value
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

// Vtable of the dynamic-cast object used by func_80079E04 (r30): vector
// setters at 0x14/0x4C/0x64, a f32 setter at 0x3C, pointer-taking variants
// at 0x68/0x6C and a vector accessor at 0xAC.
struct CfCamAdvVtbl {
    void* p00[0x14 / 4];
    void* (*fn_0x14)(void* self, void* vec);          // 0x14
    void* r18[0x3C - 0x18];
    void* (*fn_0x3C)(void* self, f32 v);              // 0x3C
    void* r40[0x4C - 0x40];
    void* (*fn_0x4C)(void* self, void* vec);          // 0x4C
    void* r50[0x64 - 0x50];
    void* (*fn_0x64)(void* self, void* vec);          // 0x64
    void* (*fn_0x68)(void* self, void* obj, void* vec, int flag);  // 0x68
    void* (*fn_0x6C)(void* self, void* obj, void* vec, int flag);  // 0x6C
    void* r70[0xAC - 0x70];
    ml::CVec3* (*fn_0xAC)(void* self);                // 0xAC
};

struct CfCamAdvObj {
    CfCamAdvVtbl* vtable;  // 0x00
};
// 0x34-byte cam-table entry: two aim vectors at +0x04/+0x10 plus four floats
// (+0x1C..+0x28). The 12-entry table lives at 0x805273C8 and is filled by
// sinit_8007BE74 (which also fills a discarded local prototype array).
struct CamEventTableEntry {
    u8        _00[0x04];
    ml::CVec3 v0;    // +0x04
    ml::CVec3 v1;    // +0x10
    f32       f_1C;  // +0x1C
    f32       f_20;  // +0x20
    f32       f_24;  // +0x24
    f32       f_28;  // +0x28
    u8        _2C[0x34 - 0x2C];
};
extern CamEventTableEntry lbl_eu_805273C8[12];

extern f32 lbl_eu_80666458;   // sdata2 triplet constants used by the sinit
extern f32 lbl_eu_80666464;
extern f32 lbl_eu_80666474;
extern f32 lbl_eu_80666478;
extern f32 lbl_eu_8066647C;
extern f32 lbl_eu_80666480;
extern f32 lbl_eu_80666484;
extern f32 lbl_eu_80666488;
extern f32 lbl_eu_8066648C;
extern f32 lbl_eu_80666490;
// 0x28-byte camera-source view read by func_8007990C: two aim vectors at
// +0x00/+0x0C, a third triplet at +0x18, and a scalar at +0x24. The triplets
// are CVec3 so whole-vector copies keep the retail lwz/stw schedule.
struct CamEventVecSrc {
    ml::CVec3 v0;   // +0x00
    ml::CVec3 v1;   // +0x0C
    ml::CVec3 v2;   // +0x18
    f32 f_24;       // +0x24
};

extern f32 lbl_eu_8066A20C;   // sdata2 scale constant (state-9 branch)
extern "C" void func_80074230(ml::CVec3* v1, ml::CVec3* v2);

// Defined in this TU (retail unmangled free functions; the extern "C"
// declarations keep call relocs unmangled while the .cpp defines them as
// plain C++ bodies). Forward-declared for the cross-referencing bodies.
extern "C" void func_80078D08(CfCamEventManager* self, int add, ml::CVec3* p5,
                              ml::CVec3* r6, int p7, int p8, f32 p1);
extern "C" void func_8007990C(CfCamEventManager* self, u32 a, u32 b,
                              CamEventVecSrc* c, u32 d);
extern "C" int func_80074F4C(CfCamShakeState* self, int mode);
extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);
extern char lbl_eu_80661B00[];  // RTTI typeinfo pair used by the cam advance cast
extern char lbl_eu_80661B30[];
extern "C" f32 CosFIdx__Q24nw4r4mathFf(f32);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" CfCamDataTable lbl_eu_80570C90;
extern "C" CfCamEventGlobal* lbl_eu_80663DF0;
extern "C" void* func_8016FE34(void* source);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" void* func_800B708C__Fi(int id);
extern "C" void* func_800BBC0C(void* obj);
extern "C" f32   lbl_eu_8066A1F8;
extern "C" f32   lbl_eu_8066A1FC;
extern "C" f32   lbl_eu_8066641C;
extern "C" f32   lbl_eu_80666418;
extern "C" f64   lbl_eu_80666420;
extern "C" f32   lbl_eu_80666428;
extern "C" f32   lbl_eu_80666448;
extern "C" f32   lbl_eu_80666454;
extern "C" f32   lbl_eu_8066A208;
extern const float lbl_eu_8066A210;
extern "C" void* lbl_eu_806640BC;
extern "C" u8    lbl_eu_804FB5D0[];
extern "C" int   func_8003B1EC(void* self);
extern "C" void* func_8003AA34(void);
extern "C" u32   func_8003B41C(void* bdat);
extern "C" u32   getBdatStringColumnValue(void* bdat, const char* column, int index);
extern void* lbl_eu_80664164;      // sbss bdat-table pointer (set by func_8003AA34)
extern char  lbl_eu_80661BB8[6];   // sdata column-name buffer (digit at +4)
extern char  lbl_eu_80527638[0xA]; // data column-name buffer (digit at +8)
extern f32   lbl_eu_8066645C;      // sdata2 compare constant
extern "C" f32   Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
extern "C" int   CfRes_getD80Flag();
extern "C" f32   func_80496288();
extern "C" void  func_800756D0(ml::CVec3* out, CinemCamSrc* src);
extern "C" int func_8007AA4C(CfCamEventManager* self);
extern "C" int func_80079E04(CfCamEventManager* self);
extern "C" int __declspec(noinline) func_800755BC(CfCamEventManager* mgr, u32 idx);
extern "C" f32 lbl_eu_8066642C;
extern "C" f32 lbl_eu_80666430;
extern "C" f32 lbl_eu_80666440;
extern "C" f32 lbl_eu_80666468;   // acos-range lower bound
// nw4r debug warning (func_8007BAFC range violations)
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
extern char lbl_eu_805262F0[];   // warning file name
extern char lbl_eu_805262C8[];   // warning format string
extern "C" f32 lbl_eu_8066646C;   // acos-angle scale
// GameManager helpers used by the cam-event advance.
extern "C" void func_80085878__Q22cf13CfGameManagerFv();
extern "C" void func_8016FD84(f32 a, f32 b);
extern "C" void func_80240AAC(u32 state);
extern "C" void func_80240B10(u32 state, void* p);
extern "C" f32 lbl_eu_80666470;
extern "C" __declspec(noinline) void* func_800784A0(u32 first, void* second,
                                                      void* third, void* fourth,
                                                      void* fifth,
                                                      CfCamDataTable* sixth,
                                                      void* seventh);
extern "C" void  func_8008212C__Q22cf13CfGameManagerFv(u32 mode);
extern "C" void* func_80074CEC(void* self, void* arg2);
extern "C" void* __ct__8006B310(void* self, void* arg2);
extern "C" void  func_8006BC1C(void* obj, u32 mask);
extern "C" void  func_8006BBF4(void* obj, u32 mask, int flag);
extern "C" void  func_8006D450(void* a, void* b, void* c, f32 f,
                                void* d, void* e, void* g);
extern "C" __declspec(noinline) void func_80077F20(void* out, void* a,
                                                       void* b, u16 c, u16 d,
                                                       void* e);
extern "C" __declspec(noinline) void* func_800778E4(void* self, u32 a,
                                                      void* b, void* c);
extern "C" int func_80078400(int action, int param);
extern "C" void* func_80496264(void* obj, int index);
extern "C" void* func_80076F88(CfCamEventManager* self, int unk34,
                                void* srcArg, CfCamDataTable* cam);
extern "C" int func_800A4050(void* dst, void* b, void* c);
extern "C" void func_80075934(ml::CVec3* out1, ml::CVec3* out2, CamCamSrc* a,
                               CamCamSrc* b, ml::CVec3* v1, ml::CVec3* v2,
                               u16 c1, u16 c2, u8 s0, u8 s1);
extern "C" UnkClass_800821F8* func_800821F8__Q22cf13CfGameManagerFv(void);
extern "C" u32   func_80061FFC(void);
extern "C" void  func_80240878(u8* p);
extern "C" void  func_80240A64(u8* base);
extern "C" f32 lbl_eu_8066A200;
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" void func_800A41BC(f32* out, CfCamEventElem* a, CfCamEventElem* b,
                              CfCamEventElem* c, CfCamEventElem* d, f32 t);
extern "C" void func_800A40E8(f32* out, CfCamEventElem* a, CfCamEventElem* b,
                              f32 t);
extern "C" int func_8024125C(int state, int val);
extern "C" int func_80241344(int state, int val);
extern "C" int func_80240C98(int state, int arg0, int arg1);
extern "C" u32 func_800AA300(int a, int b, int c);

#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

class UnkClass_800821F8;  // fwd decl: only pointer-position use (getCameraDataBlock__Q22cf13CfGameManagerFv result)

// CActorParam now included directly

// Opaque stand-in type for generic cam-event object pointers passed
// through the event-cam APIs (replaces raw void* in signatures/locals).
class CfCamEventObj;

// Global singleton object referenced by the CfCamEvent cam logic
// (address lbl_eu_80663DF0, accessed via sda21 as a pointer).
struct CfCamEventGlobal {
    u8  _000[0x38];
    u32 field_0x38;    // 0x38 - pointer/state word
    u8  _03C[0x3E - 0x3C];
    s16 field_0x3E;    // 0x3E
    u8  _040[0x46 - 0x40];
    u8  field_0x46;    // 0x46
    u8  _047[0x4C - 0x47];
    u32 field_0x4C;    // 0x4C - cam-table flag word
};

// Sub-object embedded at manager offset 0x1F4.
// A single effect slot object referenced by the manager's 3-element slot
// array at offset 0.  Destructor is virtual (defined in another TU).
class CfCamEventSlot {
public:
    virtual ~CfCamEventSlot();
    virtual ml::CVec3* getVec10(); // 0x10
    virtual void* unk14(); // 0x14
    virtual void* unk18(); // 0x18
    virtual ml::CVec3* getVec1C(); // 0x1C
    virtual void* update(); // 0x20
    virtual void* unk24(); // 0x24
    virtual void* unk28(); // 0x28
    virtual void* unk2C(); // 0x2C
    virtual void* unk30(); // 0x30
    virtual ml::CVec3* getVec34(); // 0x34
    virtual void* unk38(); // 0x38
    virtual void* unk3C(); // 0x3C
    virtual void* unk40(); // 0x40
    virtual void* unk44(); // 0x44
    virtual void* unk48(); // 0x48
    virtual void* unk4C(); // 0x4C
    virtual void* unk50(); // 0x50
    virtual void* unk54(); // 0x54
    virtual float getFloat58(); // 0x58
    virtual void* set5C(void* arg); // 0x5C
    u8 _04[0x0C - 0x04];
    void* field_0x0C; // 0x0C camera vector block
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

// Typed 12-word region of the same block (three aim triplets starting AT the
// block base). func_8007BAFC copies this whole struct to the stack before
// the slot update call, which reproduces retail's 12-word GPR block copy.
struct CamEventData {
    ml::CVec3 v0;   // +0x00 (block 0x9C)
    ml::CVec3 v1;   // +0x0C (block 0xAC)
    ml::CVec3 v2;   // +0x18 (block 0xB8)
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

// Element region used by func_80079D6C: the retail body treats the element
// pointer argument as an element base (x4 at +0x04, previous element one stride
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
    union { f32 f_10C; u32 w_10C; };   // 0x10C -> mgr 0x54
    union { f32 f_110; u32 w_110; };   // 0x110
    union { f32 f_114; u32 w_114; };   // 0x114
    union { f32 f_118; u32 w_118; };   // 0x118 -> mgr 0x6C
    union { f32 f_11C; u32 w_11C; };   // 0x11C
    union { f32 f_120; u32 w_120; };   // 0x120
    u8  _124[0x138 - 0x124];
    union { f32 f_138; u32 w_138; };   // 0x138 -> mgr 0x60
    union { f32 f_13C; u32 w_13C; };   // 0x13C
    union { f32 f_140; u32 w_140; };   // 0x140
    u8  _144[0x1E0 - 0x144];
    f32 f_1E0;   // 0x1E0 -> mgr 0x78
};

// Word-wise region of an ml::CVec3 (kept for reference; the ctor uses plain
// struct assigns which MWCC lowers to the same GPR block copy).
struct CfCamEventVecWords {
    u32 w[3];
};

// Plain POD triplet for dead prototype locals (no union: keeps the array
// contiguous so sinit stores run through a compact stack block).
struct CamPrototypeVec {
    f32 x, y, z;
};

// Camera event manager - only forward-declared elsewhere; the complete layout
// is reconstructed here from the cam-event accessors.
// Word region of a camera-vector triplet: retail copies/fills these triplets as
// raw GPR words; ml::CVec3 would emit default-ctor/__as__ calls.
struct CamEventVecWords {
    union {
        u32 w[3];
        // Anonymous nesting makes MWCC emit lwz/stw for struct copies.
        struct {
            f32 x, y, z;
        };
    };
};

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
    // POD word regions (ml::CVec3 has a non-trivial default ctor; the manager
    // ctor in retail never calls it - these are filled with raw GPR stores).
    CamEventVecWords field_0x54;       // 0x54
    CamEventVecWords field_0x60;       // 0x60
    CamEventVecWords field_0x6C;       // 0x6C
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

// Camera position source: table plus a couple of flag/compare words.
// CinemCamSrcI is the polymorphic region: retail's vptr loads go to r12
// (MWCC's reserved register for true virtual dispatch), which only happens
// when the calls are real virtual member calls - not table-struct reads.
class CinemCamSrcI {
public:
    virtual ~CinemCamSrcI();
    virtual void dummy001();
    virtual void dummy002();
    virtual void dummy003();
    virtual void dummy004();
    virtual void dummy005();
    virtual void dummy006();
    virtual void dummy007();
    virtual void dummy008();
    virtual void dummy009();
    virtual void dummy010();
    virtual void dummy011();
    virtual void dummy012();
    virtual void dummy013();
    virtual void dummy014();
    virtual void dummy015();
    virtual void dummy016();
    virtual void dummy017();
    virtual void dummy018();
    virtual void dummy019();
    virtual void dummy020();
    virtual void dummy021();
    virtual void dummy022();
    virtual void dummy023();
    virtual void dummy024();
    virtual void dummy025();
    virtual void dummy026();
    virtual void dummy027();
    virtual void dummy028();
    virtual void dummy029();
    virtual void dummy030();
    virtual void dummy031();
    virtual void dummy032();
    virtual void dummy033();
    virtual void dummy034();
    virtual void dummy035();
    virtual void dummy036();
    virtual void dummy037();
    virtual void dummy038();
    virtual void dummy039();
    virtual void dummy040();
    virtual void dummy041();
    virtual void dummy042();
    virtual CinemVecOut* getVecOut(); // 0xAC
    virtual void dummy044();
    virtual void dummy045();
    virtual void dummy046();
    virtual void dummy047();
    virtual void dummy048();
    virtual void dummy049();
    virtual void dummy050();
    virtual void dummy051();
    virtual void dummy052();
    virtual void dummy053();
    virtual void dummy054();
    virtual void dummy055();
    virtual void dummy056();
    virtual void dummy057();
    virtual void dummy058();
    virtual void dummy059();
    virtual void dummy060();
    virtual void dummy061();
    virtual void dummy062();
    virtual void dummy063();
    virtual void dummy064();
    virtual void dummy065();
    virtual void dummy066();
    virtual void dummy067();
    virtual void dummy068();
    virtual void dummy069();
    virtual void dummy070();
    virtual void dummy071();
    virtual void dummy072();
    virtual void dummy073();
    virtual CinemAimBody* getAnchor(); // 0x128
    virtual CinemAimBody* getBody(int a); // 0x12C
    // end
};


class CinemCamSrc {
public:
    virtual ~CinemCamSrc();
    virtual void dummy001();
    virtual void dummy002();
    virtual void dummy003();
    virtual void dummy004();
    virtual void dummy005();
    virtual void dummy006();
    virtual void dummy007();
    virtual void dummy008();
    virtual void dummy009();
    virtual void dummy010();
    virtual void dummy011();
    virtual void dummy012();
    virtual void dummy013();
    virtual void dummy014();
    virtual void dummy015();
    virtual void dummy016();
    virtual void dummy017();
    virtual void dummy018();
    virtual void dummy019();
    virtual void dummy020();
    virtual void dummy021();
    virtual void dummy022();
    virtual void dummy023();
    virtual void dummy024();
    virtual void dummy025();
    virtual void dummy026();
    virtual void dummy027();
    virtual void dummy028();
    virtual void dummy029();
    virtual void dummy030();
    virtual void dummy031();
    virtual void dummy032();
    virtual void dummy033();
    virtual void dummy034();
    virtual void dummy035();
    virtual void dummy036();
    virtual void dummy037();
    virtual void dummy038();
    virtual void dummy039();
    virtual void dummy040();
    virtual void dummy041();
    virtual void dummy042();
    virtual CinemVecOut* getVecOut(); // 0xAC
    virtual void dummy044();
    virtual void dummy045();
    virtual void dummy046();
    virtual void dummy047();
    virtual void dummy048();
    virtual void dummy049();
    virtual void dummy050();
    virtual void dummy051();
    virtual void dummy052();
    virtual void dummy053();
    virtual void dummy054();
    virtual void dummy055();
    virtual void dummy056();
    virtual void dummy057();
    virtual void dummy058();
    virtual void dummy059();
    virtual void dummy060();
    virtual void dummy061();
    virtual void dummy062();
    virtual void dummy063();
    virtual void dummy064();
    virtual void dummy065();
    virtual void dummy066();
    virtual void dummy067();
    virtual void dummy068();
    virtual void dummy069();
    virtual void dummy070();
    virtual void dummy071();
    virtual void dummy072();
    virtual void dummy073();
    virtual CinemAimBody* getAnchor(); // 0x128
    virtual CinemAimBody* getBody(int a); // 0x12C
    // end
    u8   _04[0x64 - 0x04];
    u32  field_0x64;       // 0x64 - flags (0x2 = clamp, 0x4 = snap)
    u8   _68[0x70 - 0x68];
    u32  field_0x70;       // 0x70 - compare value
};

// Sub-object (a CCharVoice) embedded at source offset 0x3E9C.
// Holds a table whose fn at offset 0x4C returns an object pointer; the
// same table also dispatches the aim-vector accessor at 0xAC.

class CamEventVoice {
public:
    virtual ~CamEventVoice();
    virtual void dummy001();
    virtual void dummy002();
    virtual void dummy003();
    virtual void dummy004();
    virtual void dummy005();
    virtual void dummy006();
    virtual void dummy007();
    virtual void dummy008();
    virtual void dummy009();
    virtual void dummy010();
    virtual void dummy011();
    virtual void dummy012();
    virtual void dummy013();
    virtual void dummy014();
    virtual void dummy015();
    virtual void dummy016();
    virtual void dummy017();
    virtual void dummy018();
    virtual void* unk4C(); // 0x4C
    virtual void dummy020();
    virtual void dummy021();
    virtual void dummy022();
    virtual void dummy023();
    virtual void dummy024();
    virtual void dummy025();
    virtual void dummy026();
    virtual void dummy027();
    virtual void dummy028();
    virtual void dummy029();
    virtual void dummy030();
    virtual void dummy031();
    virtual void dummy032();
    virtual void dummy033();
    virtual void dummy034();
    virtual void dummy035();
    virtual void dummy036();
    virtual void dummy037();
    virtual void dummy038();
    virtual void dummy039();
    virtual void dummy040();
    virtual void dummy041();
    virtual void dummy042();
    virtual CinemVecOut* getVecOut(); // 0xAC
    // end
};

// Body returned by the source table fn at 0x298; field +0x04 is read by
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

// Object stored in manager slot 1 (the follow-cam object): table with
// two-arg fns at 0x40 and 0x5C.

class CamEventMgrSub {
public:
    virtual ~CamEventMgrSub();
    virtual void dummy001();
    virtual void dummy002();
    virtual void dummy003();
    virtual void dummy004();
    virtual void dummy005();
    virtual void dummy006();
    virtual void dummy007();
    virtual void dummy008();
    virtual void dummy009();
    virtual void dummy010();
    virtual void dummy011();
    virtual void dummy012();
    virtual void dummy013();
    virtual void dummy014();
    virtual void dummy015();
    virtual void* unk40(void* arg); // 0x40
    virtual void dummy017();
    virtual void dummy018();
    virtual void dummy019();
    virtual void dummy020();
    virtual void dummy021();
    virtual void dummy022();
    virtual void* unk5C(void* arg); // 0x5C
    // end
};

// Voice/aim object region: table at +0x00 and an integer word at +0x74 read
// by func_80076F88 (copied into the manager's 0x10/0x24 words).
struct CamEventAimObj {
    void* table;         // 0x00
    u8 _04[0x74 - 0x04];
    u32 field_0x74;       // 0x74
};

// Region of the manager's aim-vector region (offsets 0x0C..0x34) used by
// func_80076F88. Kept as a separate region so the manager class itself keeps
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

// Source object returned by func_8016FE34. Has a table, a field at
// 0x3F10, and an embedded voice sub-object at 0x3E9C.

class CamEventSrc {
public:
    virtual ~CamEventSrc();
    virtual void dummy001();
    virtual void dummy002();
    virtual void dummy003();
    virtual void dummy004();
    virtual void dummy005();
    virtual void dummy006();
    virtual void dummy007();
    virtual void dummy008();
    virtual void dummy009();
    virtual void dummy010();
    virtual void dummy011();
    virtual void dummy012();
    virtual void dummy013();
    virtual void dummy014();
    virtual void dummy015();
    virtual void dummy016();
    virtual void dummy017();
    virtual void dummy018();
    virtual void dummy019();
    virtual void dummy020();
    virtual void dummy021();
    virtual void dummy022();
    virtual void dummy023();
    virtual void dummy024();
    virtual void dummy025();
    virtual void dummy026();
    virtual void dummy027();
    virtual void dummy028();
    virtual void dummy029();
    virtual void dummy030();
    virtual void dummy031();
    virtual void dummy032();
    virtual void dummy033();
    virtual void dummy034();
    virtual void dummy035();
    virtual void dummy036();
    virtual void dummy037();
    virtual void dummy038();
    virtual void dummy039();
    virtual void dummy040();
    virtual void dummy041();
    virtual void dummy042();
    virtual void dummy043();
    virtual void dummy044();
    virtual void dummy045();
    virtual void dummy046();
    virtual void dummy047();
    virtual void dummy048();
    virtual void dummy049();
    virtual void dummy050();
    virtual void dummy051();
    virtual void dummy052();
    virtual void dummy053();
    virtual void dummy054();
    virtual void dummy055();
    virtual void dummy056();
    virtual void dummy057();
    virtual void dummy058();
    virtual void dummy059();
    virtual void dummy060();
    virtual void dummy061();
    virtual void dummy062();
    virtual void dummy063();
    virtual void dummy064();
    virtual void dummy065();
    virtual void dummy066();
    virtual void dummy067();
    virtual void dummy068();
    virtual void dummy069();
    virtual void dummy070();
    virtual void dummy071();
    virtual void dummy072();
    virtual void dummy073();
    virtual void dummy074();
    virtual void dummy075();
    virtual void dummy076();
    virtual void dummy077();
    virtual void dummy078();
    virtual void dummy079();
    virtual void dummy080();
    virtual void dummy081();
    virtual void dummy082();
    virtual void dummy083();
    virtual void dummy084();
    virtual void dummy085();
    virtual void dummy086();
    virtual void dummy087();
    virtual void dummy088();
    virtual void dummy089();
    virtual void dummy090();
    virtual void dummy091();
    virtual void dummy092();
    virtual void dummy093();
    virtual void dummy094();
    virtual void dummy095();
    virtual void dummy096();
    virtual void dummy097();
    virtual void dummy098();
    virtual void dummy099();
    virtual void dummy100();
    virtual void dummy101();
    virtual void dummy102();
    virtual void dummy103();
    virtual void dummy104();
    virtual void dummy105();
    virtual void dummy106();
    virtual void dummy107();
    virtual void dummy108();
    virtual void dummy109();
    virtual void dummy110();
    virtual void dummy111();
    virtual void dummy112();
    virtual void dummy113();
    virtual void dummy114();
    virtual void dummy115();
    virtual void dummy116();
    virtual void dummy117();
    virtual void dummy118();
    virtual void dummy119();
    virtual void dummy120();
    virtual void dummy121();
    virtual void dummy122();
    virtual void dummy123();
    virtual void dummy124();
    virtual void dummy125();
    virtual void dummy126();
    virtual void dummy127();
    virtual void dummy128();
    virtual void dummy129();
    virtual void dummy130();
    virtual void dummy131();
    virtual void dummy132();
    virtual void dummy133();
    virtual void dummy134();
    virtual void dummy135();
    virtual void dummy136();
    virtual void dummy137();
    virtual void dummy138();
    virtual void dummy139();
    virtual void dummy140();
    virtual void dummy141();
    virtual void dummy142();
    virtual void dummy143();
    virtual void dummy144();
    virtual void dummy145();
    virtual void dummy146();
    virtual void dummy147();
    virtual void dummy148();
    virtual void dummy149();
    virtual void dummy150();
    virtual void dummy151();
    virtual void dummy152();
    virtual void dummy153();
    virtual void dummy154();
    virtual void dummy155();
    virtual void dummy156();
    virtual void dummy157();
    virtual void dummy158();
    virtual void dummy159();
    virtual void dummy160();
    virtual void dummy161();
    virtual void dummy162();
    virtual void dummy163();
    virtual void dummy164();
    virtual void dummy165();
    virtual CamEventBody* unk298(); // 0x298
    // end
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
// Integer word region of a shake-table element used by func_80074F4C's
// base-anchor copy: retail copies the x0/x4/x8 words with lwz/stw (GPR
// moves, bit patterns), so this element region carries them as u32.
struct CfCamShakeElemRaw {
    u32 x0;      // +0x00
    u32 x4;      // +0x04
    u32 x8;      // +0x08
    u16 id;      // +0x0C
    u16 c;       // +0x0E
    u16 d;       // +0x10
};

// ── WIP scaffolding (func_80074F4C / func_80075934) ───────────────────────
// Shake-table element/state region used by the shake-advance reconstruction.
// Offsets confirmed from retail func_80074F4C: elems at +0x00 (0x14 stride,
// id at +0x0C, waveform kind c at +0x0E), base anchor at +0x154..0x15C
// (copied as integer words), spline-method word at +0x160 (read with lha),
// flag_active at +0x162, flag_finish at +0x164, element count (s16) at
// +0x166, current index (s16) at +0x168, running value at +0x170 and end
// value at +0x174. The same code runs against the manager's three shake
// tables (0x7C / 0x1F4 / 0x36C) through this shared region layout.
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

// Outer table of a camera-source object: scalar getter at 0x5B4.

// Camera-source object: outer table at +0x00, an embedded CinemCamSrc
// (position source) at +0x3E9C, and three follow-limit floats at +0x44D8.
class CamCamSrc {
public:
    virtual ~CamCamSrc();
    virtual void dummy001();
    virtual void dummy002();
    virtual void dummy003();
    virtual void dummy004();
    virtual void dummy005();
    virtual void dummy006();
    virtual void dummy007();
    virtual void dummy008();
    virtual void dummy009();
    virtual void dummy010();
    virtual void dummy011();
    virtual void dummy012();
    virtual void dummy013();
    virtual void dummy014();
    virtual void dummy015();
    virtual void dummy016();
    virtual void dummy017();
    virtual void dummy018();
    virtual void dummy019();
    virtual void dummy020();
    virtual void dummy021();
    virtual void dummy022();
    virtual void dummy023();
    virtual void dummy024();
    virtual void dummy025();
    virtual void dummy026();
    virtual void dummy027();
    virtual void dummy028();
    virtual void dummy029();
    virtual void dummy030();
    virtual void dummy031();
    virtual void dummy032();
    virtual void dummy033();
    virtual void dummy034();
    virtual void dummy035();
    virtual void dummy036();
    virtual void dummy037();
    virtual void dummy038();
    virtual void dummy039();
    virtual void dummy040();
    virtual void dummy041();
    virtual void dummy042();
    virtual void dummy043();
    virtual void dummy044();
    virtual void dummy045();
    virtual void dummy046();
    virtual void dummy047();
    virtual void dummy048();
    virtual void dummy049();
    virtual void dummy050();
    virtual void dummy051();
    virtual void dummy052();
    virtual void dummy053();
    virtual void dummy054();
    virtual void dummy055();
    virtual void dummy056();
    virtual void dummy057();
    virtual void dummy058();
    virtual void dummy059();
    virtual void dummy060();
    virtual void dummy061();
    virtual void dummy062();
    virtual void dummy063();
    virtual void dummy064();
    virtual void dummy065();
    virtual void dummy066();
    virtual void dummy067();
    virtual void dummy068();
    virtual void dummy069();
    virtual void dummy070();
    virtual void dummy071();
    virtual void dummy072();
    virtual void dummy073();
    virtual void dummy074();
    virtual void dummy075();
    virtual void dummy076();
    virtual void dummy077();
    virtual void dummy078();
    virtual void dummy079();
    virtual void dummy080();
    virtual void dummy081();
    virtual void dummy082();
    virtual void dummy083();
    virtual void dummy084();
    virtual void dummy085();
    virtual void dummy086();
    virtual void dummy087();
    virtual void dummy088();
    virtual void dummy089();
    virtual void dummy090();
    virtual void dummy091();
    virtual void dummy092();
    virtual void dummy093();
    virtual void dummy094();
    virtual void dummy095();
    virtual void dummy096();
    virtual void dummy097();
    virtual void dummy098();
    virtual void dummy099();
    virtual void dummy100();
    virtual void dummy101();
    virtual void dummy102();
    virtual void dummy103();
    virtual void dummy104();
    virtual void dummy105();
    virtual void dummy106();
    virtual void dummy107();
    virtual void dummy108();
    virtual void dummy109();
    virtual void dummy110();
    virtual void dummy111();
    virtual void dummy112();
    virtual void dummy113();
    virtual void dummy114();
    virtual void dummy115();
    virtual void dummy116();
    virtual void dummy117();
    virtual void dummy118();
    virtual void dummy119();
    virtual void dummy120();
    virtual void dummy121();
    virtual void dummy122();
    virtual void dummy123();
    virtual void dummy124();
    virtual void dummy125();
    virtual void dummy126();
    virtual void dummy127();
    virtual void dummy128();
    virtual void dummy129();
    virtual void dummy130();
    virtual void dummy131();
    virtual void dummy132();
    virtual void dummy133();
    virtual void dummy134();
    virtual void dummy135();
    virtual void dummy136();
    virtual void dummy137();
    virtual void dummy138();
    virtual void dummy139();
    virtual void dummy140();
    virtual void dummy141();
    virtual void dummy142();
    virtual void dummy143();
    virtual void dummy144();
    virtual void dummy145();
    virtual void dummy146();
    virtual void dummy147();
    virtual void dummy148();
    virtual void dummy149();
    virtual void dummy150();
    virtual void dummy151();
    virtual void dummy152();
    virtual void dummy153();
    virtual void dummy154();
    virtual void dummy155();
    virtual void dummy156();
    virtual void dummy157();
    virtual void dummy158();
    virtual void dummy159();
    virtual void dummy160();
    virtual void dummy161();
    virtual void dummy162();
    virtual void dummy163();
    virtual void dummy164();
    virtual void dummy165();
    virtual void dummy166();
    virtual void dummy167();
    virtual void dummy168();
    virtual void dummy169();
    virtual void dummy170();
    virtual void dummy171();
    virtual void dummy172();
    virtual void dummy173();
    virtual void dummy174();
    virtual void dummy175();
    virtual void dummy176();
    virtual void dummy177();
    virtual void dummy178();
    virtual void dummy179();
    virtual void dummy180();
    virtual void dummy181();
    virtual void dummy182();
    virtual void dummy183();
    virtual void dummy184();
    virtual void dummy185();
    virtual void dummy186();
    virtual void dummy187();
    virtual void dummy188();
    virtual void dummy189();
    virtual void dummy190();
    virtual void dummy191();
    virtual void dummy192();
    virtual void dummy193();
    virtual void dummy194();
    virtual void dummy195();
    virtual void dummy196();
    virtual void dummy197();
    virtual void dummy198();
    virtual void dummy199();
    virtual void dummy200();
    virtual void dummy201();
    virtual void dummy202();
    virtual void dummy203();
    virtual void dummy204();
    virtual void dummy205();
    virtual void dummy206();
    virtual void dummy207();
    virtual void dummy208();
    virtual void dummy209();
    virtual void dummy210();
    virtual void dummy211();
    virtual void dummy212();
    virtual void dummy213();
    virtual void dummy214();
    virtual void dummy215();
    virtual void dummy216();
    virtual void dummy217();
    virtual void dummy218();
    virtual void dummy219();
    virtual void dummy220();
    virtual void dummy221();
    virtual void dummy222();
    virtual void dummy223();
    virtual void dummy224();
    virtual void dummy225();
    virtual void dummy226();
    virtual void dummy227();
    virtual void dummy228();
    virtual void dummy229();
    virtual void dummy230();
    virtual void dummy231();
    virtual void dummy232();
    virtual void dummy233();
    virtual void dummy234();
    virtual void dummy235();
    virtual void dummy236();
    virtual void dummy237();
    virtual void dummy238();
    virtual void dummy239();
    virtual void dummy240();
    virtual void dummy241();
    virtual void dummy242();
    virtual void dummy243();
    virtual void dummy244();
    virtual void dummy245();
    virtual void dummy246();
    virtual void dummy247();
    virtual void dummy248();
    virtual void dummy249();
    virtual void dummy250();
    virtual void dummy251();
    virtual void dummy252();
    virtual void dummy253();
    virtual void dummy254();
    virtual void dummy255();
    virtual void dummy256();
    virtual void dummy257();
    virtual void dummy258();
    virtual void dummy259();
    virtual void dummy260();
    virtual void dummy261();
    virtual void dummy262();
    virtual void dummy263();
    virtual void dummy264();
    virtual void dummy265();
    virtual void dummy266();
    virtual void dummy267();
    virtual void dummy268();
    virtual void dummy269();
    virtual void dummy270();
    virtual void dummy271();
    virtual void dummy272();
    virtual void dummy273();
    virtual void dummy274();
    virtual void dummy275();
    virtual void dummy276();
    virtual void dummy277();
    virtual void dummy278();
    virtual void dummy279();
    virtual void dummy280();
    virtual void dummy281();
    virtual void dummy282();
    virtual void dummy283();
    virtual void dummy284();
    virtual void dummy285();
    virtual void dummy286();
    virtual void dummy287();
    virtual void dummy288();
    virtual void dummy289();
    virtual void dummy290();
    virtual void dummy291();
    virtual void dummy292();
    virtual void dummy293();
    virtual void dummy294();
    virtual void dummy295();
    virtual void dummy296();
    virtual void dummy297();
    virtual void dummy298();
    virtual void dummy299();
    virtual void dummy300();
    virtual void dummy301();
    virtual void dummy302();
    virtual void dummy303();
    virtual void dummy304();
    virtual void dummy305();
    virtual void dummy306();
    virtual void dummy307();
    virtual void dummy308();
    virtual void dummy309();
    virtual void dummy310();
    virtual void dummy311();
    virtual void dummy312();
    virtual void dummy313();
    virtual void dummy314();
    virtual void dummy315();
    virtual void dummy316();
    virtual void dummy317();
    virtual void dummy318();
    virtual void dummy319();
    virtual void dummy320();
    virtual void dummy321();
    virtual void dummy322();
    virtual void dummy323();
    virtual void dummy324();
    virtual void dummy325();
    virtual void dummy326();
    virtual void dummy327();
    virtual void dummy328();
    virtual void dummy329();
    virtual void dummy330();
    virtual void dummy331();
    virtual void dummy332();
    virtual void dummy333();
    virtual void dummy334();
    virtual void dummy335();
    virtual void dummy336();
    virtual void dummy337();
    virtual void dummy338();
    virtual void dummy339();
    virtual void dummy340();
    virtual void dummy341();
    virtual void dummy342();
    virtual void dummy343();
    virtual void dummy344();
    virtual void dummy345();
    virtual void dummy346();
    virtual void dummy347();
    virtual void dummy348();
    virtual void dummy349();
    virtual void dummy350();
    virtual void dummy351();
    virtual void dummy352();
    virtual void dummy353();
    virtual void dummy354();
    virtual void dummy355();
    virtual void dummy356();
    virtual void dummy357();
    virtual void dummy358();
    virtual void dummy359();
    virtual void dummy360();
    virtual void dummy361();
    virtual void dummy362();
    virtual void dummy363();
    virtual void dummy364();
    virtual void dummy365();
    virtual float getScale5B4();
    u8  _004[0x3E9C - 0x004];
    CinemCamSrc voice;
    u8  _3EB8[0x44D8 - 0x3EB8];
    f32 f_44D8;
    f32 f_44DC;
    f32 f_44E0;
};

// Minimal WIP scaffolding: dynamic manager returned by getCameraDataBlock
// (CfGameManager). Only +0x04 is read by the cam-event update.
struct CfDynMgr {
    u8  _00[0x04];
    u32 m_field04;   // +0x04
};

// table of the dynamic-cast object used by func_80079E04 (r30): vector
// setters at 0x14/0x4C/0x64, a f32 setter at 0x3C, pointer-taking variants
// at 0x68/0x6C and a vector accessor at 0xAC.

class CfCamAdvObj {
public:
    virtual ~CfCamAdvObj();
    virtual void dummy001();
    virtual void dummy002();
    virtual void dummy003();
    virtual void dummy004();
    virtual void* unk14(void* vec); // 0x14
    virtual void dummy006();
    virtual void dummy007();
    virtual void dummy008();
    virtual void dummy009();
    virtual void dummy010();
    virtual void dummy011();
    virtual void dummy012();
    virtual void dummy013();
    virtual void dummy014();
    virtual void* unk3C(float v); // 0x3C
    virtual void dummy016();
    virtual void dummy017();
    virtual void dummy018();
    virtual void* unk4C(void* vec); // 0x4C
    virtual void dummy020();
    virtual void dummy021();
    virtual void dummy022();
    virtual void dummy023();
    virtual void dummy024();
    virtual void* unk64(void* vec); // 0x64
    virtual void* unk68(void* obj, void* vec, int flag); // 0x68
    virtual void* unk6C(void* obj, void* vec, int flag); // 0x6C
    virtual void dummy028();
    virtual void dummy029();
    virtual void dummy030();
    virtual void dummy031();
    virtual void dummy032();
    virtual void dummy033();
    virtual void dummy034();
    virtual void dummy035();
    virtual void dummy036();
    virtual void dummy037();
    virtual void dummy038();
    virtual void dummy039();
    virtual void dummy040();
    virtual void dummy041();
    virtual void dummy042();
    virtual CinemVecOut* getVecOut(); // 0xAC
    // end
};

// 9-word float block used by the func_80079E04 rotation recurrence; kept as
// a struct so the composition steps compile to GPR block copies.
struct CamEventMtx {
    f32 m[9];
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

extern const f32 lbl_eu_80666458;   // sdata2 triplet constants used by the sinit
extern const f32 lbl_eu_80666464;
extern const f32 lbl_eu_80666474;
extern const f32 lbl_eu_80666478;
extern const f32 lbl_eu_8066647C;
extern const f32 lbl_eu_80666480;
extern const f32 lbl_eu_80666484;
extern const f32 lbl_eu_80666488;
extern const f32 lbl_eu_8066648C;
extern const f32 lbl_eu_80666490;
// 0x28-byte camera-source region read by func_8007990C: two aim vectors at
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
extern const void* lbl_eu_80661B00;  // RTTI typeinfo pair used by the cam advance cast
extern const void* lbl_eu_80661B30;  // (pointer-object decls so MWCC emits @sda21 li's)
extern "C" f32 CosFIdx__Q24nw4r4mathFf(f32);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" CfCamDataTable lbl_eu_80570C90;
extern "C" CfCamEventGlobal* lbl_eu_80663DF0;
extern "C" void* func_8016FE34(void* source);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" void* findObjectById__Fi(int id);
extern "C" void* func_800BBC0C(void* obj);
extern "C" f32   lbl_eu_8066A1F8;
extern "C" f32   lbl_eu_8066A1FC;
extern "C" f32   lbl_eu_8066641C;
extern "C" f32   lbl_eu_80666418;
extern "C" f64   lbl_eu_80666420;
extern "C" f64   lbl_eu_80666438;   // signed-int -> double conversion constant
extern "C" f32   lbl_eu_80666460;   // cam-table speed divisor
extern "C" void* lbl_eu_80664168;   // event-cam bdat table pointer
extern "C" f32   lbl_eu_80666428;
extern "C" f32   lbl_eu_80666448;
extern "C" f32   lbl_eu_80666444;
extern "C" f32   lbl_eu_8066644C;
extern "C" f32   lbl_eu_80666454;
extern const float lbl_eu_8066A210;
extern "C" void* lbl_eu_806640BC;
extern "C" u8    lbl_eu_804FB5D0[];
extern "C" u32   func_8003B1EC(void* self);
extern "C" void* func_8003AA34(void);
extern "C" u32   func_8003B41C(void* bdat);
extern void* lbl_eu_80664164;      // sbss bdat-table pointer (set by func_8003AA34)
extern char  lbl_eu_80661BB8[6];   // sdata column-name buffer (digit at +4)
extern char  lbl_eu_80527638[0xA]; // data column-name buffer (digit at +8)
extern f32   lbl_eu_8066645C;      // sdata2 compare constant
extern "C" f32   Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);
extern "C" int   CfRes_getD80Flag();
extern "C" void  func_800756D0(ml::CVec3* out, CinemCamSrc* src);
extern "C" int func_8007AA4C(CfCamEventManager* self);
extern "C" int func_80079E04(CfCamEventManager* self);
extern "C" int func_800755BC(CfCamEventManager* mgr, u32 idx);
extern "C" int func_80076D8C(int unused, int type_, CamEventSrc* src,
                             CamEventTargetInfo* other, u32* outRow, u32* outCol);
extern "C" void func_8007B030(u8* self);
// Vector-normalize helper (body provided by this TU).
extern "C" void func_800A3F8C(ml::CVec3* v);
extern "C" f32 lbl_eu_8066642C;
extern "C" f32 lbl_eu_80666430;
extern "C" f32 lbl_eu_80666440;
extern "C" f32 lbl_eu_80666468;   // acos-range lower bound
// nw4r debug warning (func_8007BAFC range violations)
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
extern char lbl_eu_805262F0[];   // warning file name
extern char lbl_eu_805262C8[];   // warning format string
extern "C" f32 lbl_eu_8066646C;   // acos-angle scale
// Dynamic-cast result region (getCameraDataBlock -> __dynamic_cast): a single
// flag byte is stored at +0x294 by the cam-event setup.
struct CfCamDynObj {
    u8 _000[0x294];
    u8 field_0x294;   // +0x294
};

// CfGameManager cam-event hooks (defined in this TU; retail symbols unmangled).
extern "C" void notifyBattleSystem__Q22cf13CfGameManagerFv(int a, u8 b, int c, int d, int e);
extern "C" void isEffectReady__Q22cf13CfGameManagerFv(int idx, void* vecA, void* vecB, int mode, f32 val);
extern "C" void notifyCameraManager__Q22cf13CfGameManagerFv(void);

// GameManager helpers used by the cam-event advance.
extern "C" void cleanupMapEffects__Q22cf13CfGameManagerFv();
extern "C" void func_8016FD84(f32 a, f32 b);
extern "C" void func_80240AAC(u32 state);
extern "C" void func_80240B10(u32 state, void* p);
extern "C" f32 lbl_eu_80666470;
extern "C" CfCamEventObj* func_800784A0(u32 first, CfCamEventObj* second,
                                                      CfCamEventObj* third, CfCamEventObj* fourth,
                                                      CfCamEventObj* fifth,
                                                      CfCamDataTable* sixth,
                                                      CfCamEventObj* seventh);
extern "C" void  getCamManagerData__Q22cf13CfGameManagerFv(u32 mode);
extern "C" void* func_80074CEC(void* self, void* arg2);
extern "C" void* __ct__8006B310(void* self, void* arg2);
extern "C" void  func_8006BC1C(void* obj, u32 mask);
extern "C" void  func_8006BBF4(void* obj, u32 mask, int flag);
extern "C" void  func_8006D450(void* a, void* b, void* c, f32 f,
                                void* d, void* e, void* g);
extern "C" void func_80077F20(void* out, void* a,
                                                       void* b, u16 c, u16 d,
                                                       void* e);
extern "C" void* func_800778E4(CfCamEventManager* self, int unk34,
                                                      void* srcArg, u32 rowOverride);
extern "C" int func_80078400(int action, int param);
extern "C" void* func_80496264(void* obj, int index);
extern "C" void* func_80076F88(CfCamEventManager* self, int unk34,
                                void* srcArg, CfCamDataTable* cam);
extern "C" int func_800A4050(void* dst, void* b, void* c);
// Pose-solver helper called by func_80075934.
extern "C" void func_80074010(void* out, void* in, f32 f, void* vec);
extern "C" f32 lbl_eu_80666450;   // sdata2 damping factor
extern "C" void* func_800FE68C(void);
extern "C" int func_804BE348(void* a, void* b, u32 c, u32 d);
extern "C" void func_80075934(ml::CVec3* out1, ml::CVec3* out2, CamCamSrc* a,
                               CamCamSrc* b, ml::CVec3* v1, ml::CVec3* v2,
                               u16 c1, u16 c2, u8 s0, u8 s1);
extern "C" UnkClass_800821F8* getCameraDataBlock__Q22cf13CfGameManagerFv(void);
extern "C" u32   func_80061FFC(void);
extern "C" void  func_80240878(u8* p);
extern "C" void  func_80240A64(u8* base);
extern const f32 lbl_eu_8066A200;
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" void func_800A41BC(f32* out, CfCamEventElem* a, CfCamEventElem* b,
                              CfCamEventElem* c, CfCamEventElem* d, f32 t);
extern "C" void func_800A40E8(f32* out, CfCamEventElem* a, CfCamEventElem* b,
                              f32 t);
extern "C" int func_8024125C(int state, int val);
extern "C" int func_80241344(int state, int val);
extern "C" int func_80240C98(int state, int arg0, int arg1);
extern "C" u32 func_800AA300(int a, int b, int c);

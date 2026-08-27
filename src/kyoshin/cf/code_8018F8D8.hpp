#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/device/CDeviceVI.hpp>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/scn/CScn.hpp>
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CPartyStateWin.hpp"

// Intermediate padding base so the IScnRender subobject lands at the retail
// offset 0x58 (same scheme as CMenuFade.hpp / CMenuFadeBase). This makes the
// non-primary base-cast in Term() emit addi r4, this, 0x58 with a null check.
class CMenuPTStateBase : public CProcess {
public:
    u8 field_base_pad[0x18]; // 0x3C-0x54
    u8 field_0x54;           // 0x54 (Move completion flag)
    u8 field_base_pad2[3];   // 0x55-0x58
};

class CMenuPTState : public CMenuPTStateBase, public IScnRender {
public:
    CMenuPTState();
    virtual ~CMenuPTState();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

private:
    // CMenuPTStateBase (0x0-0x58) + IScnRender (0x58-0x5C) bases.
    CScn* field_0x5C;              // 0x5C
    CBgTex field_0x60;             // 0x60 (0x20 bytes)
    CPartyStateWin field_0x80;     // 0x80
    // CPartyStateWin is 0x6BEC bytes, so this state byte lands at 0x6C6C.
    u8 field_0x6C6C;               // 0x6C6C (Move phase state)
};

// Container used by the catalog helper functions func_8019017C / func_80190034.
// The hosting object keeps a cf::CfGameManager* reference at offset 0x408.
struct CFuncHost {
    u8 _0[0x408];
    cf::CfGameManager* manager;  // 0x408
};

// Object used by func_eu_80191E88: a raw pointer at offset 0x408 that is
// treated as a byte buffer offset by +0x28 when passed to func_8009EB2C.
struct CFuncHost408 {
    u8 _0[0x408];
    u8* field_0x408;  // 0x408
};

// 4-float value type used by func_8008566C.
struct UnkFloat4 {
    float field_0x0;
    float field_0x4;
    float field_0x8;
    float field_0xC;
};

// Result struct returned by func_8009D5FC in func_eu_80191E88.
struct UnkR31_8019E88 {
    u16 field_0x0;
    u16 field_0x2;
};

// Typed vtable-proxy for issuing virtual calls at exact retail vtable slot
// byte offsets (PPC32: one pointer per 4 bytes). Only the slots actually
// called by this TU's functions get typed members; the rest are opaque
// padding. Used by func_8018FCA8 against the player (CfObjectMove) object.
struct CPlayerVtbl {
    void* padA8[0xa8 / 4];            // 0x000 - 0x0A4
    void (*fw_a8)(void* self, float* pos); // 0x0A8
    void* padAC[(0xc8 - 0xac) / 4];   // 0x0AC - 0x0C4
    void (*fw_c8)(void* self);        // 0x0C8
    void (*fw_cc)(void* self);        // 0x0CC
    void* padD0[(0xd4 - 0xd0) / 4];   // 0x0D0
    void (*fw_d4)(void* self, float* pos, float extra); // 0x0D4
    void* padD8[(0xdc - 0xd8) / 4];   // 0x0D8
    int (*fw_74)(void* self);         // 0xDC
    void* padE0[(0x168 - 0xe0) / 4];  // 0x0E0 - 0x164
    void (*fw_168)(void* self, float value); // 0x168
};

// Accessor for the player flag word at offset 0x68 (func_8018FCA8).
struct CPlayerFlags {
    u8 pad[0x68];
    u32 flags; // 0x68
};

// Vtable proxy for the actor objects used by func_80190940 (slots 0x2BC,
// 0x308) and the sub-object held at +0x4 (slot 0x30).
struct CActorSubVtbl {
    void* pad[0x30 / 4];
    void* (*fw_30)(void* self); // 0x30, returns a pointer read at [0]
};

struct CActorVtbl {
    void* padA[0x2bc / 4];
    void (*fw_2bc)(void* self);       // 0x2BC
    void* padB[(0x308 - 0x2c0) / 4];  // 0x2C0 - 0x304
    void* (*fw_308)(void* self);      // 0x308
};

// Extended actor vtable proxy for func_80190940: adds the 0x158 float getter.
struct CActorVtblExt {
    u32 padA[0x158 / 4];
    float (*fw_158)(void* self);      // 0x158
    void* padB[(0x2bc - 0x15c) / 4];
    long (*fw_2bc)(void* self);       // 0x2BC
    void* padC[(0x308 - 0x2c0) / 4];
    long (*fw_308)(void* self);       // 0x308
};

// Comparison-slot proxy (func_80190940 equality probe).
struct CActorCmpVtbl {
    void* pad[0x18c / 4];
    long (*fw_18c)(void* self);       // 0x18C
    long (*fw_190)(void* self);       // 0x190
};

// Position sub-object (actor + 0x3E9C) vtable views.
struct CSubPosVtbl84 {
    void* pad[0x84 / 4];
    long (*fw_84)(void* self);        // 0x84
};
struct CSubPosVtblAC {
    void* pad[0xac / 4];
    float* (*fw_ac)(void* self);      // 0xAC, returns Vec3 pointer
};

// Actor reference used by func_80190940 (both the queried actor and the
// enum-list candidates share this layout prefix).
struct FuncActorRef {
    u8 _00[0x4];
    u8* field_0004;                   // +0x04 state-word sub-object (vf30)
    u8* field_0008;                   // +0x08 tag-list sub-object
    u8 _0c[0x3388 - 0xc];
    volatile u16 field_3388;          // +0x3388 status bits (bit 28 probed)
    u8 _338a[0x3e9c - 0x338a];
    u8* field_3e9c;                   // +0x3E9C position sub-object
    u8 _3ea0[0x3f28 - 0x3ea0];
    volatile u16 field_3f28;          // +0x3F28 state word
    u8 _3f2a[0x3f60 - 0x3f2a];
    u8* field_3f60;                   // +0x3F60 talk/state object
    u8 _3f64[0x44d8 - 0x3f64];
    float field_44d8;                 // +0x44D8 distance bias
};

// Result record written by func_80190940 (first argument).
struct FuncResultRef {
    u8 _00[0x4];
    volatile u32 field_0004;                  // +0x04 result code
    volatile u32 field_0008;                  // +0x08 secondary flag
    FuncActorRef* field_000c;         // +0x0C chosen actor
};

// Battle-manager view for func_80190940.
struct BmView90940 {
    u8 _0[0x194];
    volatile u32 field_0x194;                 // +0x194 progress counter
    u8 _198[0x1aa - 0x198];
    volatile u8 field_0x1aa;                  // +0x1AA battle-active byte
    u8 _1ab[0x20c8 - 0x1ab];
    volatile s16 field_0x20c8;                // +0x20C8 signed counter
};
struct BmByte261A4 {
    u8 _0[0x261a4];
    volatile u8 field_0x261a4;
};
struct BmPtmf28354 {
    u8 _0[0x28354];
    u32 field_0x28354[3];             // 12-byte member-function pointer slot
};

// Sub-object at talk/state pointer +0x374.
struct Sub3F60View {
    u8 _0[0x374];
    volatile u32 field_0x374;
};

// Enum-list holder/list used by func_80190940's candidate sweep.
struct CfEnumListHolder90940 {
    u8 _0[8];
};
struct CfEnumList90940 {
    u8 _0[0x620];
    volatile u32 count;               // +0x620 element count
};

// 8-byte element copied by the state-blob copy functions func_80191C88 /
// func_80192268 (two u32 words per iteration).
struct SCopyPair {
    u32 lo;
    u32 hi;
};

// func_80192268's third/fourth pair runs start at misaligned offsets 0x2e9
// and 0x37e in the retail blob (retail emits misaligned lwz/stw for them).
// Wrap them in 1-packed containers so they can land there (same scheme as
// CPartyStateWin.hpp's packed tail).
#pragma pack(push, 1)
struct SPackPairs16 {
    SCopyPair items[16];
};
struct SPackPairs1024 {
    SCopyPair items[1024];
};
#pragma pack(pop)

// volatile scalars so each memberwise copy compiles to an immediate
// load+store pair in program order instead of being batched by the -O4,p
// scheduler into callee-saved regs/f14-f31 (which drags in _savegpr/_restgpr
// prologue/epilogue code retail doesn't have).
typedef volatile float vf32;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile u32 vu32;

// Volatile variant used by func_80191C88's rolled pair runs (see above).
struct SCopyPairV {
    vu32 lo;
    vu32 hi;
};

// Layout backing func_80191C88's memberwise copy. Field types mirror the
// retail access opcodes exactly: lwz/stw fields are vu32, lfs/stfs fields are
// vf32, lha/sth fields are vs16 (sign-extending loads), and the lone lhz at
// 0x530 is vu16. EVERY field is volatile so each copy stays an immediate
// load/store pair in program order even under the unit's default -O4,p
// (no batching into callee-saved r14-r31/f14-f31, no loop unrolling).
struct SCopy_80191C88 {
    u32 w_00;                 // 0x00 (not copied)
    vu32 f_04, f_08, f_0c;
    u8 pad_10[4];             // 0x10 (not copied)
    vu32 f_14, f_18, f_1c, f_20;
    vf32 f_24;                // retail uses lfs/stfs here
    vu32 f_28, f_2c, f_30, f_34;
    vu8 b_38;
    vu32 f_3c;
    SCopyPairV arrA[52];      // 0x3c - 0x1df (mtctr/bdnz loop)
    // 0x1e0 - 0x26c: word copies (retail emits them out of source order;
    // the listing below follows retail's emitted order).
    vu32 f_1e0, f_1e4, f_1e8, f_1ec, f_1f0, f_1f4, f_1f8, f_1fc;
    vu32 f_200, f_204, f_208, f_20c, f_210, f_214, f_218, f_21c, f_220;
    vu8 b_224, b_225, b_226;
    vu32 f_228, f_22c, f_230, f_234, f_238, f_23c, f_240, f_244, f_248, f_24c;
    vu32 f_250, f_254, f_258, f_25c, f_260, f_264, f_268, f_26c;
    SCopyPairV arrB[16];      // 0x270 - 0x2eb (mtctr/bdnz loop)
    SCopyPairV arrC[16];      // 0x2ec - 0x36b (mtctr/bdnz loop)
    vf32 f_370, f_374, f_378, f_37c;
    vf32 f_380, f_384, f_388, f_38c, f_390, f_394, f_398, f_39c;
    // 0x3a0 - 0x42c: word copies (retail-emitted order).
    vu32 f_3a0, f_3a4, f_3a8, f_3ac, f_3b0, f_3b4, f_3b8, f_3bc;
    vu32 f_3c0, f_3c4, f_3c8, f_3cc, f_3d0, f_3d4, f_3d8, f_3dc;
    vu32 f_3e0, f_3e4, f_3e8, f_3ec, f_3f0, f_3f4, f_3f8, f_3fc;
    vu32 f_400, f_404, f_408, f_40c, f_410, f_414, f_418, f_41c;
    vu32 f_420, f_424, f_428, f_42c;
    vf32 f_430, f_434, f_438, f_43c, f_440, f_444, f_448, f_44c;
    vf32 f_450, f_454, f_458, f_45c, f_460, f_464, f_468, f_46c;
    vf32 f_470, f_474, f_478, f_47c, f_480, f_484, f_488, f_48c, f_490, f_494;
    vu8 b_498;
    vf32 f_49c, f_4a0, f_4a4;
    vu32 f_4a8, f_4ac, f_4b0, f_4b4, f_4b8;
    vu8 b_4bc, b_4bd, b_4be, b_4bf;
    vu32 f_4c0, f_4c4, f_4c8;
    vf32 f_4cc, f_4d0;
    vs16 h_4d4, h_4d6, h_4d8; // retail sign-extends (lha)
    vu8 b_4da;
    vs16 h_4dc;
    vu32 f_4e0, f_4e4, f_4e8, f_4ec, f_4f0, f_4f4;
    vf32 f_4f8, f_4fc, f_500, f_504, f_508, f_50c, f_510;
    // 0x514 - 0x52c: word copies (retail-emitted order).
    vu32 f_514, f_518, f_51c, f_520, f_524, f_528, f_52c;
    vu16 h_530;               // retail zero-extends here (lhz)
    vs16 h_532, h_534, h_536, h_538;
};

// Layout backing func_80192268's memberwise copy, reconstructed against the
// retail access pattern (uncopied gaps are explicit pad arrays so every
// copied field lands at its retail offset). Only 0xe0-0xf0 are real floats
// in retail codegen (lfs/stfs); every other word-sized field is moved with
// lwz/stw, so it is typed u32.
struct SCopy_80192268 {
    u32 w_00;
    vu32 f_04, f_08, f_0c, f_10, f_14, f_18, f_1c, f_20, f_24, f_28, f_2c, f_30, f_34, f_38, f_3c;
    vu8 b_40, b_41, b_42, b_43;
    u8 pad_44[4];
    vu32 f_48, f_4c, f_50, f_54;
    vu8 b_58, b_59;
    u8 pad_5a[6];
    vu32 f_60, f_64, f_68, f_6c;
    vu8 b_70, b_71;
    u8 pad_72[6];
    vu32 f_78, f_7c, f_80, f_84;
    vu8 b_88, b_89;
    u8 pad_8a[6];
    vu32 f_90, f_94, f_98, f_9c, f_a0, f_a4, f_a8, f_ac, f_b0;
    vu8 b_b4, b_b5, b_b6, b_b7;
    u8 pad_b8[4];
    vu32 f_bc, f_c0, f_c4, f_c8, f_cc, f_d0, f_d4, f_d8;
    vu8 b_dc, b_dd, b_de, b_df;
    vf32 f_e0, f_e4, f_e8, f_ec, f_f0;
    vu8 b_f4;
    u8 pad_f5[3];
    SCopyPair arr1[16];
    vu8 b_178, b_179, b_17a;
    u8 pad_17b[5];
    vu32 f_180, f_184, f_188, f_18c, f_190, f_194, f_198, f_19c, f_1a0;
    vu8 b_1a4;
    u8 pad_1a5[3];
    vu32 f_1a8, f_1ac;
    vu8 b_1b0, b_1b1, b_1b2, b_1b3, b_1b4, b_1b5;
    u8 pad_1b6[6];
    vu32 f_1bc, f_1c0, f_1c4, f_1c8, f_1cc, f_1d0, f_1d4, f_1d8, f_1dc;
    vu8 b_1e0;
    u8 pad_1e1[3];
    vu32 f_1e4, f_1e8;
    vu8 b_1ec, b_1ed, b_1ee, b_1ef, b_1f0, b_1f1;
    u8 pad_1f2[2];
    vu8 b_1f4, b_1f5, b_1f6;
    u8 pad_1f7;
    vu16 h_1f8, h_1fa, h_1fc;
    vu8 b_1fe;
    u8 pad_1ff;
    vu32 f_200, f_204, f_208, f_20c, f_210, f_214, f_218, f_21c, f_220, f_224;
    u8 pad_225[3];
    SCopyPair arr2[18];
    vu32 f_2b8, f_2bc, f_2c0, f_2c4, f_2c8, f_2cc, f_2d0, f_2d4, f_2d8, f_2dc, f_2e0, f_2e4;
    vu8 b_2e8;
    SPackPairs16 arr3;   // retail-misaligned run at 0x2e9
    u8 pad_369[3];
    vu32 f_36c;
    vu16 h_370;
    vu8 b_372, b_373, b_374, b_375, b_376, b_377, b_378, b_379, b_37a, b_37b, b_37c;
    u8 pad_37d;
    SPackPairs1024 arr4; // retail-misaligned run at 0x37e
    vu16 h_237e;
    vu8 b_2380, b_2381, b_2382, b_2383;
    vu32 f_2384, f_2388, f_238c, f_2390, f_2394, f_2398, f_239c, f_23a0;
    SCopyPair arr5[128];
    vu32 f_27a4;
};

// Opaque blob copied by func_80166F80 inside Init()'s window-state copy
// (0x41F4..0x4400 region of the CPartyStateWin overlay).
struct CMenuPTBlob41F4 {
    u8 data[0x20c];
};
extern "C" void func_80166F80(CMenuPTBlob41F4* dst, const CMenuPTBlob41F4* src);

// CBgTex layout-load query called by Init (retail unmangled free-function
// form; same convention as func_801C3D54 below).
extern "C" bool func_801C3C14(CBgTex* self);

// Retail ctor/dtor symbols for the stack temporaries built by Init are the
// unmangled slice names (not the C++-mangled member forms), so Init calls
// them under C linkage into raw storage, mirroring CMenuPTState.cpp.
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CPartyStateWin(CPartyStateWin* self, u32 arg1, u32 arg2);
extern "C" CBgTex* __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" CPartyStateWin* __dt__14CPartyStateWinFv(CPartyStateWin* self,
                                                      int flags);

// Per-run view structs: each scalar run in Init's window-state copy is
// assigned wholesale so MWCC emits its canonical memberwise-copy pattern.
#pragma pack(push, 1)
struct CMenuPTKPack {   // 0x3084..0x3090 (retail misaligned lwz/lhz accesses)
    u8 b3084, b3085, b3086;
    u32 v3087;          // misaligned word
    u16 h308b;          // misaligned halfword
    u8 b308d, b308e, b308f;
};
struct CMenuPTOPack {   // 0x41E8..0x41F2 (retail misaligned words)
    u8 v41e8, v41e9;
    u32 v41ea;          // misaligned
    u32 v41ee;          // misaligned
};
#pragma pack(pop)
struct CMenuPTRunA {          // 0x08..0x64
    u32 v08, v0c, v10, v14;
    u32 pad18;
    u32 v1c, v20, v24, v28, v2c, v30, v34, v38, v3c;
    u8 v40;
    u8 pad41[3];
    u32 v44, v48;
    u8 v4c, v4d, v4e, v4f;
    u32 pad50;
    u32 v54, v58, v5c, v60;
};
struct CMenuPTRunE { u32 v[13]; };   // 0x1020..0x1054
struct CMenuPTRunH { u32 v[13]; };   // 0x2010..0x2044
struct CMenuPTRunK {          // 0x3000..0x30AC
    u32 v3000[10];            // 0x3000..0x3028
    u8 v3028;
    f32 v302c, v3030;
    u8 v3034;
    u8 pad3035[3];
    u32 pad3038;
    u32 v303c[9];             // 0x303C..0x3060
    u8 v3060;
    u32 v3064;
    u8 v3068, v3069;
    u8 pad306a[2];
    u32 pad306c;
    u32 v3070[4];             // 0x3070..0x3080
    u8 v3080, v3081;
    u8 pad3082[2];
    CMenuPTKPack kpack3084;
    u8 pad3090[0xc];
    u32 v309c[4];             // 0x309C..0x30AC
};
struct CMenuPTRunN1 {         // 0x4068..0x40B4 (pre-loop)
    u32 v4068[15];
    u8 v40a4;
    u8 pad40a5[3];
    f32 v40a8, v40ac;
    u8 v40b0, v40b1;
    u8 pad40b2[2];
};
struct CMenuPTRunO {          // 0x4120..0x41F2
    u32 v4120[5];
    f32 v4134;
    u32 v4138[6];
    u32 pad4150;
    u32 v4154[15];
    u8 pad4190[4];
    u32 v4198;
    u8 v419c, v419d;
    u8 pad419e[2];
    u32 pad41a0;
    u32 v41a4[4];
    u8 v41b4, v41b5;
    u8 pad41b6[2];
    u32 pad41b8;
    u32 v41bc[4];
    u8 v41cc, v41cd;
    u8 pad41ce[2];
    u32 pad41d0;
    u32 v41d4[4];
    u8 v41e4, v41e5;
    u8 pad41e6[2];
    CMenuPTOPack opack41e8;
};
struct CMenuPTRunQ {          // 0x6BAC..0x6BEC
    u32 v6bac[9];
    u8 v6bd0;
    u32 v6bd4[2];
    u8 v6bdc[6];
    u8 pad6be2[2];
    u8 v6be4, v6be5;
    u8 pad6be6[2];
    f32 v6be8;
};

// Byte-offset overlay of CPartyStateWin describing exactly which members
// CMenuPTState::Init copies from the stack-built temporary into field_0x80.
// Region boundaries mirror the retail copy decomposition: inline scalar runs,
// 0x53C-byte SCopy_80191C88 blobs, one misaligned-word pack island each in
// the 0x3084 / 0x41E8 windows, a 13-record pair loop at 0x40B4 and the big
// SCopy_80192268 blob at 0x4400.
struct CMenuPTWinCopyView {
    u32 v0000;                 // 0x0000 (not copied)
    u32 v0004;                 // 0x0004 (not copied)
    u32 v0008, v000c, v0010, v0014;
    u32 pad0018;
    u32 v001c, v0020, v0024, v0028, v002c, v0030, v0034, v0038, v003c;
    u8 v0040;
    u8 pad0041[3];
    u32 v0044, v0048;
    u8 v004c, v004d, v004e, v004f;
    u32 pad0050;
    u32 v0054, v0058, v005c, v0060;
    SCopy_80191C88 blob0064;   // 0x064..0x5A0 (func_80191C88)
    u8 pad05a0[8];
    SCopy_80191C88 blob05a8;   // 0x5A8..0xAE4
    SCopy_80191C88 blob0ae4;   // 0xAE4..0x1020
    u32 v1020, v1024, v1028, v102c, v1030, v1034, v1038, v103c, v1040,
        v1044, v1048, v104c, v1050;
    SCopy_80191C88 blob1054;   // 0x1054..0x1590
    u32 v1590, v1594;
    SCopy_80191C88 blob1598;   // 0x1598..0x1AD4
    SCopy_80191C88 blob1ad4;   // 0x1AD4..0x2010
    u32 v2010, v2014, v2018, v201c, v2020, v2024, v2028, v202c, v2030,
        v2034, v2038, v203c, v2040;
    SCopy_80191C88 blob2044;   // 0x2044..0x2580
    u32 v2580, v2584;
    SCopy_80191C88 blob2588;   // 0x2588..0x2AC4
    SCopy_80191C88 blob2ac4;   // 0x2AC4..0x3000
    // Head run 0x3000..0x30AC before the next blob.
    u32 v3000, v3004, v3008, v300c, v3010, v3014, v3018, v301c, v3020,
        v3024;
    u8 v3028;
    f32 v302c, v3030;
    u8 v3034;
    u8 pad3035[3];
    u32 pad3038;
    u32 v303c, v3040, v3044, v3048, v304c, v3050, v3054, v3058, v305c;
    u8 v3060;
    u32 v3064;
    u8 v3068, v3069;
    u8 pad306a[2];
    u32 pad306c;
    u32 v3070, v3074, v3078, v307c;
    u8 v3080, v3081;
    u8 pad3082[2];
    CMenuPTKPack kpack3084;    // 0x3084..0x3090
    u8 pad3090[0xc];
    u32 v309c, v30a0, v30a4, v30a8;
    SCopy_80191C88 blob30ac;   // 0x30AC..0x35E8
    u32 v35e8, v35ec;
    SCopy_80191C88 blob35f0;   // 0x35F0..0x3B2C
    SCopy_80191C88 blob3b2c;   // 0x3B2C..0x4068
    // Head run + 13-record pair loop, 0x4068..0x4120.
    u32 v4068[15];             // 0x4068..0x40A4
    u8 v40a4;
    u8 pad40a5[3];
    f32 v40a8, v40ac;
    u8 v40b0, v40b1;
    u8 pad40b2[2];
    SCopyPair pairs13[13];     // 0x40B4 (mtctr/bdnz loop, count 13)
    u32 v411c;                 // loop tail word
    // Scalar run 0x4120..0x41F2.
    u32 v4120, v4124, v4128, v412c, v4130;
    f32 v4134;
    u32 v4138, v413c, v4140, v4144, v4148, v414c;
    u32 pad4150;
    u32 v4154, v4158, v415c, v4160, v4164, v4168, v416c, v4170, v4174,
        v4178, v417c, v4180, v4184, v4188, v418c;
    u8 pad4190[4];
    u32 v4198;
    u8 v419c, v419d;
    u8 pad419e[2];
    u32 pad41a0;
    u32 v41a4, v41a8, v41ac, v41b0;
    u8 v41b4, v41b5;
    u8 pad41b6[2];
    u32 pad41b8;
    u32 v41bc, v41c0, v41c4, v41c8;
    u8 v41cc, v41cd;
    u8 pad41ce[2];
    u32 pad41d0;
    u32 v41d4, v41d8, v41dc, v41e0;
    u8 v41e4, v41e5;
    u8 pad41e6[2];
    CMenuPTOPack opack41e8;    // 0x41E8..0x41F2
    u8 pad41f2[2];
    CMenuPTBlob41F4 f41f4;     // 0x41F4..0x4400 (func_80166F80)
    SCopy_80192268 blob4400;   // 0x4400..0x6BA8 (func_80192268)
    // Tail run 0x6BA8..0x6BEC.
    u8 pad6ba8[4];
    u32 v6bac, v6bb0, v6bb4, v6bb8, v6bbc, v6bc0, v6bc4, v6bc8, v6bcc;
    u8 v6bd0;
    u32 v6bd4, v6bd8;
    u8 v6bdc, v6bdd, v6bde, v6bdf, v6be0, v6be1;
    u8 pad6be2[2];
    u8 v6be4, v6be5;
    u8 pad6be6[2];
    f32 v6be8;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" int func_8013BE50();
extern "C" void func_80137250__FPQ34nw4r3lyt8DrawInfo(void*);

// --- imports used by the Move / dispatcher functions ---------------------

// Colour-unpack scale/magic pair (func_801901A4's u8->float ladder).
extern const float lbl_eu_80667A78;
extern const f64 lbl_eu_80667A80;

// u32 word-pair / f64 view for MWCC's 0x43300000 u8->float conversion
// (same convention as CPSkillF64Conv in CPassiveSkill.hpp).
union PTStateF64Conv {
    u32 w[2];
    f64 d;
};
extern const float lbl_eu_80667A88;
extern const float lbl_eu_80667A8C;

// u16 pair tables read at element [1] by func_80190568's cmd-0x19 path.
// Sized [2]: MWCC only assumes .sbss small-data addressing (sda21 reloc)
// for extern arrays whose bounds are visible.
extern u16 lbl_eu_80663E48[2];
extern u16 lbl_eu_80663E4C[2];

// Object id looked up by func_8018FA2C through func_8007FF6C (.sdata halfword).
extern const u16 lbl_eu_80663E40;

// CPartyStateWin sub-object drivers used by CMenuPTState::Move (defined in
// CPartyStateWin.cpp under their retail unmangled names).
extern "C" int func_801FA524(CPartyStateWin* self);
extern "C" void func_801FA4F4(CPartyStateWin* self);
extern "C" int func_801FA4EC(CPartyStateWin* self);
extern "C" void func_801FA338(CPartyStateWin* self);

// CBgTex per-frame helpers - retail dispatches them under plain unmangled
// names (free functions taking the embedded sub-object), not member calls.
extern "C" int func_801C3E34(CBgTex* self);
extern "C" void func_801C3D54(CBgTex* self);

// Retail draws the background via the unmangled free symbol func_801C3D7C
// (not the CBgTex member mangling).
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);

// System-window open query + sound helpers (Move, state 3).
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
void playUISound(u32 id); // UI sound (C++-mangled retail name)

// CfGameManager pre-mangled imports (same convention as CPartyStateWin.hpp:
// verbatim retail symbols, real argument shapes observed at call sites).
extern "C" void assignCameraTarget__Q22cf13CfGameManagerFv(u32 value);
extern "C" cf::CfObjectMove* func_8007FF6C__Q22cf13CfGameManagerFv(u16 objectId,
                                                                    void* obj,
                                                                    u32 selector,
                                                                    float amount);
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int selector);
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" void triggerPlayerEffects__Q22cf13CfGameManagerFv(u32 objectValue, u32 flag,
                                                        float value);

// Character-data block returned by func_8009EC9C; the +0x17C sub-object gets
// two virtual calls (slots 0xA4/0xA8) from func_8018FA2C.
struct CCharDataSubVtbl {
    void* pad[0xa4 / 4];
    void (*fw_a4)(void* self, int arg); // 0xA4
    void (*fw_a8)(void* self, int arg); // 0xA8
};
struct CCharDataView {
    u8 _0[0x17c];
    CCharDataSubVtbl* vtbl17c; // 0x17C
};

// Slot list returned by func_8009ECB0 (arr1[3] + arr2[6] packed at +4;
// func_8018FA2C walks nine u32 slots).
struct PartySlotList {
    u8 _0[4];
    u32 slots[9];
};

// Opaque receiver type for the menu-command handler table dispatched by
// func_80190840 through member-function pointers.
class MenuCmdHost;
typedef int (MenuCmdHost::*MenuCmdHandler)(u16, u32, u32*, u32);

// 0x29-entry handler table indexed by the popped command byte (lbl_eu_80532838).
extern MenuCmdHandler lbl_eu_80532838[];

// CfRes ring-buffer pump used by func_801901A4-era dispatchers (defined in
// CfRes.cpp); this TU uses a minimal layout view of the ring object.
struct MenuCmdRingView {
    u8 ring[0x400];
    u32 field_400;
    u32 field_404;
};
extern "C" int func_80061C5C(MenuCmdRingView* buffer, u32* headerOut,
                             u32* dataOut);

// Party-slot character-data lookups used by func_8018FA2C.
extern "C" void* func_8009EC9C(u16 index);  // matches CAIAction.hpp declaration
extern "C" void func_800A30E4(void* data);
extern "C" void func_800A1370(void* data);
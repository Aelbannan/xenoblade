// kyoshin/menu/CMenuPTState -- PT (party-state) menu TU.
// CMenuPTState is an IUICf task (IScnRender at +0x58). Its singleton factory is
// CMenuPTState::create (retail `__ct__CMenuPTState`).

// PartyStateWin_OpenWindow's shared header declares it with (self, u32, u32), but the
// CMenuPTState::Move call site leaves r4/r5 unset in retail; shadow the
// 3-arg declaration during include and re-declare the 1-arg form below.
#define PartyStateWin_OpenWindow PartyStateWin_OpenWindow_3arg_unused
#include "kyoshin/CPartyStateWinImports.hpp"
#include "kyoshin/menu/CMenuPTState.hpp"
#undef PartyStateWin_OpenWindow
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/object/CAIAction.hpp"   // battle-list helpers (getObjectAt / isBattleStatusActive)
#include "kyoshin/CTitleAHelpImports.hpp"

// 8-byte element copied by the state-blob copy functions CopyPTSubBlob /
// CopyPTStateBlob (two u32 words per iteration).
struct SCopyPair {
    u32 lo;
    u32 hi;
};

// CopyPTStateBlob's third/fourth pair runs start at misaligned offsets 0x2e9
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

// Volatile variant used by CopyPTSubBlob's rolled pair runs (see above).
struct SCopyPairV {
    vu32 lo;
    vu32 hi;
};

// Layout backing CopyPTSubBlob's memberwise copy. Field types mirror the
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

// Layout backing CopyPTStateBlob's memberwise copy, reconstructed against the
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
    // NOTE: no pad here - retail's arr2 loop is based at 0x224, so arr2[18]
    // starts immediately at 0x228 (a stale pad_225[3] shifted every later
    // field +4 versus retail).
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

// Opaque blob copied by InfoCfCopyObjF80 inside Init()'s window-state copy
// (0x41F4..0x4400 region of the CPartyStateWin overlay).
struct CMenuPTBlob41F4 {
    u8 data[0x20c];
};
extern "C" void InfoCfCopyObjF80(CMenuPTBlob41F4* dst, const CMenuPTBlob41F4* src);

// CBgTex layout-load query called by Init (retail unmangled free-function
// form; same convention as BgTex_Tick_3D54 below).
extern "C" bool BgTex_Acquire_3C14(CBgTex* self);

// Retail ctor/dtor symbols for the stack temporaries built by Init are the
// unmangled slice names (not the C++-mangled member forms), so Init calls
// them under C linkage into raw storage, mirroring CMenuPTState.cpp.
extern "C" void __ct__6CBgTexFUc(CBgTex* self, u8 arg);
extern "C" void __ct__14CPartyStateWinFUlUl(CPartyStateWin* self, u32 arg1, u32 arg2);
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
// CMenuPTState::Init copies from the stack-built temporary into mPartyStateWin.
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
    SCopy_80191C88 blob0064;   // 0x064..0x5A0 (CopyPTSubBlob)
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
    CMenuPTBlob41F4 f41f4;     // 0x41F4..0x4400 (InfoCfCopyObjF80)
    SCopy_80192268 blob4400;   // 0x4400..0x6BA8 (CopyPTStateBlob)
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
extern "C" int IsMenuState621F0();
extern "C" void SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo(void*);

extern "C" void PartyStateWin_OpenWindow(CPartyStateWin* self);
extern "C" int UIBattleGetFlagE8();
extern "C" int Scn_SetPauseFlag(CScn* scn, int flag);
extern "C" void BgTex_Release_3D9C(u8* obj);
extern "C" void PartyStateWin_TeardownWindow(u8* obj);
extern "C" void CUICfManager_setTimeout30();
extern "C" void processEventList__Q22cf13CfGameManagerFv();
extern "C" void processEffectList__Q22cf13CfGameManagerFv();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);

// System-window open query + sound helpers (Move, state 3).
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();
void playUISound(u32 id); // UI sound (C++-mangled retail name)

// CPartyStateWin sub-object drivers used by CMenuPTState::Move (defined in
// CPartyStateWin.cpp under their retail unmangled names).
extern "C" int PartyStateWin_QueryMenuClose(CPartyStateWin* self);
extern "C" void PartyStateWin_StepPartySub(CPartyStateWin* self);
extern "C" int PartyStateWin_GetLatchFlag(CPartyStateWin* self);
extern "C" void PartyStateWin_FrameStep(CPartyStateWin* self);

// CBgTex per-frame helpers - retail dispatches them under plain unmangled
// names (free functions taking the embedded sub-object), not member calls.
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);

// Retail draws the background via the unmangled free symbol BgTex_Draw_3D7C
// (not the CBgTex member mangling).
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);

// Defined below (state-blob copy used by Init).
extern "C" void CopyPTStateBlob(SCopy_80192268* dst, const SCopy_80192268* src);

// Member dtors (~CPartyStateWin, ~CBgTex) and the CProcess base dtor are
// compiler-generated here, matching retail's guard structure.
CMenuPTState::~CMenuPTState() {}

// Party-menu state initialization: rebuild the background texture and the
// party-state window on the stack, copy them member-by-member into the
// persistent fields (region layout per CMenuPTWinCopyView), then register
// the IScnRender sub-object as a render callback at priority 0xd.
void CMenuPTState::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(true);
    processEffectList__Q22cf13CfGameManagerFv();
    {
        // Retail copies the scalar members of a fresh CBgTex field-by-field
        // (Init is too large for the implicit operator= to be auto-inlined).
        CBgTex bgTemp(0);
        mBgTex.mMemRegion = bgTemp.mMemRegion;
        mBgTex.mFileHandle = bgTemp.mFileHandle;
        mBgTex.mLayout = bgTemp.mLayout;
        mBgTex.mLayoutReady = bgTemp.mLayoutReady;
        mBgTex.mLoaded = bgTemp.mLoaded;
        mBgTex.mPtmMode = bgTemp.mPtmMode;
    }
    BgTex_Acquire_3C14(&mBgTex);

    u32 winStore[sizeof(CPartyStateWin) / 4];
#define winTmp reinterpret_cast<CPartyStateWin*>(winStore)
#define d reinterpret_cast<CMenuPTWinCopyView*>(&mPartyStateWin)
#define s reinterpret_cast<CMenuPTWinCopyView*>(winStore)
    __ct__14CPartyStateWinFUlUl(winTmp, (u32)this, (u32)mStoredParent);
    // Memberwise copy of the freshly built window into the persistent member.

    d->v0008 = s->v0008;
    d->v000c = s->v000c;
    d->v0010 = s->v0010;
    d->v0014 = s->v0014;
    d->v001c = s->v001c;
    d->v0020 = s->v0020;
    d->v0024 = s->v0024;
    d->v0028 = s->v0028;
    d->v002c = s->v002c;
    d->v0030 = s->v0030;
    d->v0034 = s->v0034;
    d->v0038 = s->v0038;
    d->v003c = s->v003c;
    d->v0040 = s->v0040;
    d->v0044 = s->v0044;
    d->v0048 = s->v0048;
    d->v004c = s->v004c;
    d->v004d = s->v004d;
    d->v004e = s->v004e;
    d->v004f = s->v004f;
    d->v0054 = s->v0054;
    d->v0058 = s->v0058;
    d->v005c = s->v005c;
    d->v0060 = s->v0060;
    // CopyPTSubBlob is imported under C linkage with opaque u8* parameters
    // (see CPartyStateWin.hpp); call sites pass the blob sub-objects.
    CopyPTSubBlob((u8*)&d->blob0064, (const u8*)&s->blob0064);
    CopyPTSubBlob((u8*)&d->blob05a8, (const u8*)&s->blob05a8);
    CopyPTSubBlob((u8*)&d->blob0ae4, (const u8*)&s->blob0ae4);
    d->v1020 = s->v1020;
    d->v1024 = s->v1024;
    d->v1028 = s->v1028;
    d->v102c = s->v102c;
    d->v1030 = s->v1030;
    d->v1034 = s->v1034;
    d->v1038 = s->v1038;
    d->v103c = s->v103c;
    d->v1040 = s->v1040;
    d->v1044 = s->v1044;
    d->v1048 = s->v1048;
    d->v104c = s->v104c;
    d->v1050 = s->v1050;
    CopyPTSubBlob((u8*)&d->blob1054, (const u8*)&s->blob1054);
    d->v1590 = s->v1590;
    d->v1594 = s->v1594;
    CopyPTSubBlob((u8*)&d->blob1598, (const u8*)&s->blob1598);
    CopyPTSubBlob((u8*)&d->blob1ad4, (const u8*)&s->blob1ad4);
    d->v2010 = s->v2010;
    d->v2014 = s->v2014;
    d->v2018 = s->v2018;
    d->v201c = s->v201c;
    d->v2020 = s->v2020;
    d->v2024 = s->v2024;
    d->v2028 = s->v2028;
    d->v202c = s->v202c;
    d->v2030 = s->v2030;
    d->v2034 = s->v2034;
    d->v2038 = s->v2038;
    d->v203c = s->v203c;
    d->v2040 = s->v2040;
    CopyPTSubBlob((u8*)&d->blob2044, (const u8*)&s->blob2044);
    d->v2580 = s->v2580;
    d->v2584 = s->v2584;
    CopyPTSubBlob((u8*)&d->blob2588, (const u8*)&s->blob2588);
    CopyPTSubBlob((u8*)&d->blob2ac4, (const u8*)&s->blob2ac4);
    d->v3000 = s->v3000;
    d->v3004 = s->v3004;
    d->v3008 = s->v3008;
    d->v300c = s->v300c;
    d->v3010 = s->v3010;
    d->v3014 = s->v3014;
    d->v3018 = s->v3018;
    d->v301c = s->v301c;
    d->v3020 = s->v3020;
    d->v3024 = s->v3024;
    d->v3028 = s->v3028;
    d->v302c = s->v302c;
    d->v3030 = s->v3030;
    d->v3034 = s->v3034;
    d->v303c = s->v303c;
    d->v3040 = s->v3040;
    d->v3044 = s->v3044;
    d->v3048 = s->v3048;
    d->v304c = s->v304c;
    d->v3050 = s->v3050;
    d->v3054 = s->v3054;
    d->v3058 = s->v3058;
    d->v305c = s->v305c;
    d->v3060 = s->v3060;
    d->v3064 = s->v3064;
    d->v3068 = s->v3068;
    d->v3069 = s->v3069;
    d->kpack3084.v3087 = s->kpack3084.v3087;
    d->v3070 = s->v3070;
    d->v3074 = s->v3074;
    d->v3078 = s->v3078;
    d->v307c = s->v307c;
    d->v3080 = s->v3080;
    d->v3081 = s->v3081;
    d->kpack3084.b3084 = s->kpack3084.b3084;
    d->kpack3084.b3085 = s->kpack3084.b3085;
    d->kpack3084.b3086 = s->kpack3084.b3086;
    d->kpack3084.h308b = s->kpack3084.h308b;
    d->kpack3084.b308d = s->kpack3084.b308d;
    d->kpack3084.b308e = s->kpack3084.b308e;
    d->kpack3084.b308f = s->kpack3084.b308f;
    d->v309c = s->v309c;
    d->v30a0 = s->v30a0;
    d->v30a4 = s->v30a4;
    d->v30a8 = s->v30a8;
    CopyPTSubBlob((u8*)&d->blob30ac, (const u8*)&s->blob30ac);
    d->v35e8 = s->v35e8;
    d->v35ec = s->v35ec;
    CopyPTSubBlob((u8*)&d->blob35f0, (const u8*)&s->blob35f0);
    CopyPTSubBlob((u8*)&d->blob3b2c, (const u8*)&s->blob3b2c);
    {
        // 15-word run, fully unrolled (retail emits lwz/stw per element).
        d->v4068[0] = s->v4068[0];
        d->v4068[1] = s->v4068[1];
        d->v4068[2] = s->v4068[2];
        d->v4068[3] = s->v4068[3];
        d->v4068[4] = s->v4068[4];
        d->v4068[5] = s->v4068[5];
        d->v4068[6] = s->v4068[6];
        d->v4068[7] = s->v4068[7];
        d->v4068[8] = s->v4068[8];
        d->v4068[9] = s->v4068[9];
        d->v4068[10] = s->v4068[10];
        d->v4068[11] = s->v4068[11];
        d->v4068[12] = s->v4068[12];
        d->v4068[13] = s->v4068[13];
        d->v4068[14] = s->v4068[14];
    }
    d->v40a4 = s->v40a4;
    d->v40a8 = s->v40a8;
    d->v40ac = s->v40ac;
    d->v40b0 = s->v40b0;
    d->v40b1 = s->v40b1;
    // 13-record 8-byte pair walk (retail lwzu/stwu mtctr/bdnz loop) plus the
    // trailing word copied after the loop.
    u32* sp13 = (u32*)&s->pairs13[0].lo - 1;
    u32* dp13 = (u32*)&d->pairs13[0].lo - 1;
    for (int n = 13; n != 0; n--) {
        u32 lo = *(sp13 + 1);
        u32 hi = *(sp13 + 2);
        *(dp13 + 1) = lo;
        *(dp13 + 2) = hi;
        sp13 += 2;
        dp13 += 2;
    }
    *(dp13 + 1) = *(sp13 + 1);
    d->v4120 = s->v4120;
    d->v4124 = s->v4124;
    d->v4128 = s->v4128;
    d->v412c = s->v412c;
    d->v4130 = s->v4130;
    d->v4134 = s->v4134;
    d->v4138 = s->v4138;
    d->v413c = s->v413c;
    d->v4140 = s->v4140;
    d->v4144 = s->v4144;
    d->v4148 = s->v4148;
    d->v414c = s->v414c;
    d->v4154 = s->v4154;
    d->v4158 = s->v4158;
    d->v415c = s->v415c;
    d->v4160 = s->v4160;
    d->v4164 = s->v4164;
    d->v4168 = s->v4168;
    d->v416c = s->v416c;
    d->v4170 = s->v4170;
    d->v4174 = s->v4174;
    d->v4178 = s->v4178;
    d->v417c = s->v417c;
    d->v4180 = s->v4180;
    d->v4184 = s->v4184;
    d->v4188 = s->v4188;
    d->v418c = s->v418c;
    d->v4198 = s->v4198;
    d->v419c = s->v419c;
    d->v419d = s->v419d;
    d->v41a4 = s->v41a4;
    d->v41a8 = s->v41a8;
    d->v41ac = s->v41ac;
    d->v41b0 = s->v41b0;
    d->v41b4 = s->v41b4;
    d->v41b5 = s->v41b5;
    d->v41bc = s->v41bc;
    d->v41c0 = s->v41c0;
    d->v41c4 = s->v41c4;
    d->v41c8 = s->v41c8;
    d->v41cc = s->v41cc;
    d->v41cd = s->v41cd;
    d->v41d4 = s->v41d4;
    d->v41d8 = s->v41d8;
    d->v41dc = s->v41dc;
    d->v41e0 = s->v41e0;
    d->opack41e8.v41e8 = s->opack41e8.v41e8;
    d->opack41e8.v41e9 = s->opack41e8.v41e9;
    d->opack41e8.v41ea = s->opack41e8.v41ea;
    d->opack41e8.v41ee = s->opack41e8.v41ee;
    InfoCfCopyObjF80(&d->f41f4, &s->f41f4);
    CopyPTStateBlob(&d->blob4400, &s->blob4400);
    // Tail run (CSysWin flags + window timer float).
    d->v6bac = s->v6bac;
    d->v6bb0 = s->v6bb0;
    d->v6bb4 = s->v6bb4;
    d->v6bb8 = s->v6bb8;
    d->v6bbc = s->v6bbc;
    d->v6bc0 = s->v6bc0;
    d->v6bc4 = s->v6bc4;
    d->v6bc8 = s->v6bc8;
    d->v6bcc = s->v6bcc;
    d->v6bd0 = s->v6bd0;
    d->v6bd4 = s->v6bd4;
    d->v6bd8 = s->v6bd8;
    d->v6bdc = s->v6bdc;
    d->v6bdd = s->v6bdd;
    d->v6bde = s->v6bde;
    d->v6bdf = s->v6bdf;
    d->v6be0 = s->v6be0;
    d->v6be1 = s->v6be1;
    d->v6be4 = s->v6be4;
    d->v6be5 = s->v6be5;
    d->v6be8 = s->v6be8;

    __dt__14CPartyStateWinFv(winTmp, -1);
#undef winTmp
#undef d
#undef s

    static_cast<CScn*>(mStoredParent)->addRenderCB(this, 0xd, 1);
    Scn_SetPauseFlag(static_cast<CScn*>(mStoredParent), 0);
}

// Memberwise copy of the SCopy_80191C88 state blob from src to dst, in the
// exact order retail reads them (regions 0x00/0x10/0x39/0x499/0x4db are not
// copied). The three 8-byte runs compile to mtctr/bdnz lwzu+stwu copy loops
// (loop counts: 52, 16, 16). Retail is size-optimized here: under unit -O4,p
// the long runs unroll and the float copies get batched into f14-f31 (which
// drags in _savegpr/_restgpr prologue code retail doesn't have), so this one
// function compiles with optimize_for_size like other rolled-copy sites
// (retail keeps the three pair runs rolled as mtctr/bdnz lwzu+stwu loops;
// under the unit default -O4,p the runs unroll and bloat .text by ~0x2E8).
// The vu*/vf* volatile field types force the retail load/store program order.
#pragma optimize_for_size on
extern "C" void CopyPTSubBlob(u8* dstBytes, const u8* srcBytes) {
    SCopy_80191C88* dst = reinterpret_cast<SCopy_80191C88*>(dstBytes);
    const SCopy_80191C88* src = reinterpret_cast<const SCopy_80191C88*>(srcBytes);
    dst->f_04 = src->f_04;
    dst->f_08 = src->f_08;
    dst->f_0c = src->f_0c;
    dst->f_14 = src->f_14;
    dst->f_18 = src->f_18;
    dst->f_1c = src->f_1c;
    dst->f_20 = src->f_20;
    dst->f_24 = src->f_24;
    dst->f_28 = src->f_28;
    dst->f_2c = src->f_2c;
    dst->f_30 = src->f_30;
    dst->f_34 = src->f_34;
    dst->b_38 = src->b_38;
    dst->f_3c = src->f_3c;

    // Biased one element back so the accesses fuse into lwzu/stwu forms
    // (retail mtctr/bdnz walk shape).
    u32* sA = (u32*)&src->arrA[0].lo - 1;
    u32* dA = (u32*)&dst->arrA[0].lo - 1;
    for (int n = 52; n != 0; n--) {
        u32 lo = *(sA + 1);
        u32 hi = *(sA + 2);
        *(dA + 1) = lo;
        *(dA + 2) = hi;
        sA += 2;
        dA += 2;
    }

    dst->f_1e0 = src->f_1e0; dst->f_1e4 = src->f_1e4;
    dst->f_1e8 = src->f_1e8; dst->f_1ec = src->f_1ec;
    dst->f_1f0 = src->f_1f0; dst->f_1f4 = src->f_1f4;
    dst->f_1f8 = src->f_1f8; dst->f_1fc = src->f_1fc;
    dst->f_200 = src->f_200; dst->f_204 = src->f_204;
    dst->f_208 = src->f_208; dst->f_20c = src->f_20c;
    dst->f_210 = src->f_210; dst->f_214 = src->f_214;
    dst->f_218 = src->f_218; dst->f_21c = src->f_21c;
    dst->f_220 = src->f_220;
    dst->b_224 = src->b_224; dst->b_225 = src->b_225;
    dst->b_226 = src->b_226;

    dst->f_228 = src->f_228; dst->f_22c = src->f_22c;
    dst->f_230 = src->f_230; dst->f_234 = src->f_234;
    dst->f_238 = src->f_238; dst->f_23c = src->f_23c;
    dst->f_240 = src->f_240; dst->f_244 = src->f_244;
    dst->f_248 = src->f_248; dst->f_24c = src->f_24c;
    dst->f_250 = src->f_250; dst->f_254 = src->f_254;
    dst->f_258 = src->f_258; dst->f_25c = src->f_25c;
    dst->f_260 = src->f_260; dst->f_264 = src->f_264;
    dst->f_268 = src->f_268;
    dst->f_26c = src->f_26c;

    // Biased walks matching retail (bases 0x26C/0x2EC, count 16 each).
    u32* sB = (u32*)&src->arrB[0].lo - 1;
    u32* dB = (u32*)&dst->arrB[0].lo - 1;
    for (int n = 16; n != 0; n--) {
        u32 lo = *(sB + 1);
        u32 hi = *(sB + 2);
        *(dB + 1) = lo;
        *(dB + 2) = hi;
        sB += 2;
        dB += 2;
    }
    u32* sC = (u32*)&src->arrC[0].lo - 1;
    u32* dC = (u32*)&dst->arrC[0].lo - 1;
    for (int n = 16; n != 0; n--) {
        u32 lo = *(sC + 1);
        u32 hi = *(sC + 2);
        *(dC + 1) = lo;
        *(dC + 2) = hi;
        sC += 2;
        dC += 2;
    }

    dst->f_370 = src->f_370; dst->f_374 = src->f_374;
    dst->f_378 = src->f_378; dst->f_37c = src->f_37c;
    dst->f_380 = src->f_380; dst->f_384 = src->f_384;
    dst->f_388 = src->f_388; dst->f_38c = src->f_38c;
    dst->f_390 = src->f_390; dst->f_394 = src->f_394;
    dst->f_398 = src->f_398; dst->f_39c = src->f_39c;

    dst->f_3a0 = src->f_3a0; dst->f_3a4 = src->f_3a4;
    dst->f_3a8 = src->f_3a8; dst->f_3ac = src->f_3ac;
    dst->f_3b0 = src->f_3b0; dst->f_3b4 = src->f_3b4;
    dst->f_3b8 = src->f_3b8; dst->f_3bc = src->f_3bc;
    dst->f_3c0 = src->f_3c0; dst->f_3c4 = src->f_3c4;
    dst->f_3c8 = src->f_3c8; dst->f_3cc = src->f_3cc;
    dst->f_3d0 = src->f_3d0; dst->f_3d4 = src->f_3d4;
    dst->f_3d8 = src->f_3d8; dst->f_3dc = src->f_3dc;
    dst->f_3e0 = src->f_3e0; dst->f_3e4 = src->f_3e4;
    dst->f_3e8 = src->f_3e8; dst->f_3ec = src->f_3ec;
    dst->f_3f0 = src->f_3f0; dst->f_3f4 = src->f_3f4;
    dst->f_3f8 = src->f_3f8; dst->f_3fc = src->f_3fc;
    dst->f_400 = src->f_400; dst->f_404 = src->f_404;
    dst->f_408 = src->f_408; dst->f_40c = src->f_40c;
    dst->f_410 = src->f_410; dst->f_414 = src->f_414;
    dst->f_418 = src->f_418; dst->f_41c = src->f_41c;
    dst->f_420 = src->f_420; dst->f_424 = src->f_424;
    dst->f_428 = src->f_428; dst->f_42c = src->f_42c;

    dst->f_430 = src->f_430; dst->f_434 = src->f_434;
    dst->f_438 = src->f_438; dst->f_43c = src->f_43c;
    dst->f_440 = src->f_440; dst->f_444 = src->f_444;
    dst->f_448 = src->f_448; dst->f_44c = src->f_44c;
    dst->f_450 = src->f_450; dst->f_454 = src->f_454;
    dst->f_458 = src->f_458; dst->f_45c = src->f_45c;
    dst->f_460 = src->f_460; dst->f_464 = src->f_464;
    dst->f_468 = src->f_468; dst->f_46c = src->f_46c;
    dst->f_470 = src->f_470; dst->f_474 = src->f_474;
    dst->f_478 = src->f_478; dst->f_47c = src->f_47c;
    dst->f_480 = src->f_480; dst->f_484 = src->f_484;
    dst->f_488 = src->f_488; dst->f_48c = src->f_48c;
    dst->f_490 = src->f_490; dst->f_494 = src->f_494;
    dst->b_498 = src->b_498;
    dst->f_49c = src->f_49c; dst->f_4a0 = src->f_4a0;
    dst->f_4a4 = src->f_4a4;

    dst->f_4a8 = src->f_4a8; dst->f_4ac = src->f_4ac;
    dst->f_4b0 = src->f_4b0; dst->f_4b4 = src->f_4b4;
    dst->f_4b8 = src->f_4b8;
    dst->b_4bc = src->b_4bc; dst->b_4bd = src->b_4bd;
    dst->b_4be = src->b_4be; dst->b_4bf = src->b_4bf;
    dst->f_4c0 = src->f_4c0; dst->f_4c4 = src->f_4c4;
    dst->f_4c8 = src->f_4c8;
    dst->f_4cc = src->f_4cc; dst->f_4d0 = src->f_4d0;
    dst->h_4d4 = src->h_4d4; dst->h_4d6 = src->h_4d6;
    dst->h_4d8 = src->h_4d8;
    dst->b_4da = src->b_4da;
    dst->h_4dc = src->h_4dc;
    dst->f_4e0 = src->f_4e0; dst->f_4e4 = src->f_4e4;
    dst->f_4e8 = src->f_4e8; dst->f_4ec = src->f_4ec;
    dst->f_4f0 = src->f_4f0; dst->f_4f4 = src->f_4f4;
    dst->f_4f8 = src->f_4f8; dst->f_4fc = src->f_4fc;
    dst->f_500 = src->f_500; dst->f_504 = src->f_504;
    dst->f_508 = src->f_508; dst->f_50c = src->f_50c;
    dst->f_510 = src->f_510;
    dst->f_514 = src->f_514; dst->f_518 = src->f_518;
    dst->f_51c = src->f_51c; dst->f_520 = src->f_520;
    dst->f_524 = src->f_524; dst->f_528 = src->f_528;
    dst->f_52c = src->f_52c;
    dst->h_530 = src->h_530;
    dst->h_532 = src->h_532; dst->h_534 = src->h_534;
    dst->h_536 = src->h_536; dst->h_538 = src->h_538;
}
#pragma optimize_for_size off

// Per-field copy of a large (~0x27A8-byte) state blob from src to dst, in the
// exact order retail reads them. The five runs of 8-byte elements compile to
// mtctr/bdnz lwzu+stwu copy loops (loop counts: 16, 18, 16, 0x400, 0x80).
// Same size story as CopyPTSubBlob above: retail keeps the runs rolled, so
// this function also compiles with optimize_for_size (saves ~0x400 .text).
// __declspec(noinline): retail Init keeps a real bl to this symbol; without
// it MWCC inlines the shrunken body into Init's single call site (+0x5C8).
#pragma optimize_for_size on
__declspec(noinline) void CopyPTStateBlob(SCopy_80192268* dst, const SCopy_80192268* src) {
    dst->f_04 = src->f_04; dst->f_08 = src->f_08;
    dst->f_0c = src->f_0c; dst->f_10 = src->f_10;
    dst->f_14 = src->f_14; dst->f_18 = src->f_18;
    dst->f_1c = src->f_1c; dst->f_20 = src->f_20;
    dst->f_24 = src->f_24; dst->f_28 = src->f_28;
    dst->f_2c = src->f_2c; dst->f_30 = src->f_30;
    dst->f_34 = src->f_34; dst->f_38 = src->f_38;
    dst->f_3c = src->f_3c;

    dst->b_40 = src->b_40; dst->b_41 = src->b_41;
    dst->b_42 = src->b_42; dst->b_43 = src->b_43;

    dst->f_48 = src->f_48; dst->f_4c = src->f_4c;
    dst->f_50 = src->f_50; dst->f_54 = src->f_54;
    dst->b_58 = src->b_58; dst->b_59 = src->b_59;

    dst->f_60 = src->f_60; dst->f_64 = src->f_64;
    dst->f_68 = src->f_68; dst->f_6c = src->f_6c;
    dst->b_70 = src->b_70; dst->b_71 = src->b_71;

    dst->f_78 = src->f_78; dst->f_7c = src->f_7c;
    dst->f_80 = src->f_80; dst->f_84 = src->f_84;
    dst->b_88 = src->b_88; dst->b_89 = src->b_89;

    dst->f_90 = src->f_90; dst->f_94 = src->f_94;
    dst->f_98 = src->f_98; dst->f_9c = src->f_9c;
    dst->f_a0 = src->f_a0; dst->f_a4 = src->f_a4;
    dst->f_a8 = src->f_a8; dst->f_ac = src->f_ac;
    dst->f_b0 = src->f_b0;

    dst->b_b4 = src->b_b4; dst->b_b5 = src->b_b5;
    dst->b_b6 = src->b_b6; dst->b_b7 = src->b_b7;

    dst->f_bc = src->f_bc; dst->f_c0 = src->f_c0;
    dst->f_c4 = src->f_c4; dst->f_c8 = src->f_c8;
    dst->f_cc = src->f_cc; dst->f_d0 = src->f_d0;
    dst->f_d4 = src->f_d4; dst->f_d8 = src->f_d8;
    dst->b_dc = src->b_dc; dst->b_dd = src->b_dd;
    dst->b_de = src->b_de; dst->b_df = src->b_df;

    dst->f_e0 = src->f_e0; dst->f_e4 = src->f_e4;
    dst->f_e8 = src->f_e8; dst->f_ec = src->f_ec;
    dst->f_f0 = src->f_f0;
    dst->b_f4 = src->b_f4;

    // Biased one element back so the accesses fuse into lwzu/stwu forms
    // (retail mtctr/bdnz walk shape, base 0xF4, count 16).
    u32* sA1 = (u32*)&src->arr1[0].lo - 1;
    u32* dA1 = (u32*)&dst->arr1[0].lo - 1;
    for (int n = 16; n != 0; n--) {
        u32 lo = *(sA1 + 1);
        u32 hi = *(sA1 + 2);
        *(dA1 + 1) = lo;
        *(dA1 + 2) = hi;
        sA1 += 2;
        dA1 += 2;
    }

    dst->b_178 = src->b_178; dst->b_179 = src->b_179;
    dst->b_17a = src->b_17a;

    dst->f_180 = src->f_180; dst->f_184 = src->f_184;
    dst->f_188 = src->f_188; dst->f_18c = src->f_18c;
    dst->f_190 = src->f_190; dst->f_194 = src->f_194;
    dst->f_198 = src->f_198; dst->f_19c = src->f_19c;
    dst->f_1a0 = src->f_1a0;
    dst->b_1a4 = src->b_1a4;
    dst->f_1a8 = src->f_1a8; dst->f_1ac = src->f_1ac;

    dst->b_1b0 = src->b_1b0; dst->b_1b1 = src->b_1b1;
    dst->b_1b2 = src->b_1b2; dst->b_1b3 = src->b_1b3;
    dst->b_1b4 = src->b_1b4; dst->b_1b5 = src->b_1b5;

    dst->f_1bc = src->f_1bc; dst->f_1c0 = src->f_1c0;
    dst->f_1c4 = src->f_1c4; dst->f_1c8 = src->f_1c8;
    dst->f_1cc = src->f_1cc; dst->f_1d0 = src->f_1d0;
    dst->f_1d4 = src->f_1d4; dst->f_1d8 = src->f_1d8;
    dst->f_1dc = src->f_1dc;
    dst->b_1e0 = src->b_1e0;
    dst->f_1e4 = src->f_1e4; dst->f_1e8 = src->f_1e8;

    dst->b_1ec = src->b_1ec; dst->b_1ed = src->b_1ed;
    dst->b_1ee = src->b_1ee; dst->b_1ef = src->b_1ef;
    dst->b_1f0 = src->b_1f0; dst->b_1f1 = src->b_1f1;

    dst->b_1f4 = src->b_1f4; dst->b_1f5 = src->b_1f5;
    dst->b_1f6 = src->b_1f6;
    dst->h_1f8 = src->h_1f8; dst->h_1fa = src->h_1fa;
    dst->h_1fc = src->h_1fc;
    dst->b_1fe = src->b_1fe;

    dst->f_200 = src->f_200; dst->f_204 = src->f_204;
    dst->f_208 = src->f_208; dst->f_20c = src->f_20c;
    dst->f_210 = src->f_210; dst->f_214 = src->f_214;
    dst->f_218 = src->f_218; dst->f_21c = src->f_21c;
    dst->f_220 = src->f_220;
    dst->f_224 = src->f_224;

    // Same biased walk shape (retail base 0x224, count 18).
    u32* sA2 = (u32*)&src->arr2[0].lo - 1;
    u32* dA2 = (u32*)&dst->arr2[0].lo - 1;
    for (int n = 18; n != 0; n--) {
        u32 lo = *(sA2 + 1);
        u32 hi = *(sA2 + 2);
        *(dA2 + 1) = lo;
        *(dA2 + 2) = hi;
        sA2 += 2;
        dA2 += 2;
    }

    dst->f_2b8 = src->f_2b8; dst->f_2bc = src->f_2bc;
    dst->f_2c0 = src->f_2c0; dst->f_2c4 = src->f_2c4;
    dst->f_2c8 = src->f_2c8; dst->f_2cc = src->f_2cc;
    dst->f_2d0 = src->f_2d0; dst->f_2d4 = src->f_2d4;
    dst->f_2d8 = src->f_2d8; dst->f_2dc = src->f_2dc;
    dst->f_2e0 = src->f_2e0; dst->f_2e4 = src->f_2e4;
    dst->b_2e8 = src->b_2e8;

    // Retail-misaligned run: biased base 0x2E5, count 16.
    u32* sA3 = (u32*)&src->arr3.items[0].lo - 1;
    u32* dA3 = (u32*)&dst->arr3.items[0].lo - 1;
    for (int n = 16; n != 0; n--) {
        u32 lo = *(sA3 + 1);
        u32 hi = *(sA3 + 2);
        *(dA3 + 1) = lo;
        *(dA3 + 2) = hi;
        sA3 += 2;
        dA3 += 2;
    }

    dst->f_36c = src->f_36c;
    dst->h_370 = src->h_370;
    dst->b_372 = src->b_372; dst->b_373 = src->b_373;
    dst->b_374 = src->b_374; dst->b_375 = src->b_375;
    dst->b_376 = src->b_376; dst->b_377 = src->b_377;
    dst->b_378 = src->b_378; dst->b_379 = src->b_379;
    dst->b_37a = src->b_37a; dst->b_37b = src->b_37b;
    dst->b_37c = src->b_37c;

    // Retail-misaligned big run: biased base 0x37A, count 0x400.
    u32* sA4 = (u32*)&src->arr4.items[0].lo - 1;
    u32* dA4 = (u32*)&dst->arr4.items[0].lo - 1;
    for (int n = 1024; n != 0; n--) {
        u32 lo = *(sA4 + 1);
        u32 hi = *(sA4 + 2);
        *(dA4 + 1) = lo;
        *(dA4 + 2) = hi;
        sA4 += 2;
        dA4 += 2;
    }

    dst->h_237e = src->h_237e;
    dst->b_2380 = src->b_2380; dst->b_2381 = src->b_2381;
    dst->b_2382 = src->b_2382; dst->b_2383 = src->b_2383;
    dst->f_2384 = src->f_2384; dst->f_2388 = src->f_2388;
    dst->f_238c = src->f_238c; dst->f_2390 = src->f_2390;
    dst->f_2394 = src->f_2394; dst->f_2398 = src->f_2398;
    dst->f_239c = src->f_239c; dst->f_23a0 = src->f_23a0;

    // Same biased walk shape (retail base 0x23A0, count 0x80).
    u32* sA5 = (u32*)&src->arr5[0].lo - 1;
    u32* dA5 = (u32*)&dst->arr5[0].lo - 1;
    for (int n = 128; n != 0; n--) {
        u32 lo = *(sA5 + 1);
        u32 hi = *(sA5 + 2);
        *(dA5 + 1) = lo;
        *(dA5 + 2) = hi;
        sA5 += 2;
        dA5 += 2;
    }

    dst->f_27a4 = src->f_27a4;
}
#pragma optimize_for_size off

void CMenuPTState::Term() {
    CDeviceVI::waitForDrawDone();
    Scn_SetPauseFlag(static_cast<CScn*>(mStoredParent), 1);
    static_cast<CScn*>(mStoredParent)->removeRenderCB(this);
    BgTex_Release_3D9C((u8*)&mBgTex);
    PartyStateWin_TeardownWindow((u8*)&mPartyStateWin);
    sInstance = 0;
    processEventList__Q22cf13CfGameManagerFv();
    setPresentationFlag__Q22cf13CfGameManagerFv(false);
    CUICfManager_setTimeout30();
}

// Minimal view of the pad word block returned by getCurrentPad() (only the
// +4 button word is read by CMenuPTState::Move).
struct MenuPTPadView {
    u8 _0[4];
    u32 buttons; // 0x4
};

// Per-frame party-menu update: gate on task state, run the open/close input
// edge while the window is settling, then advance the 4-state phase byte.
void CMenuPTState::Move() {
    CTaskGame::getInstance();
    // OR-combined guard: retail emits bne-exit for the first disjunct and a
    // beq-body / b-exit pair for the second (same shape as cbRenderBefore).
    if (CTaskGame::isFlag01Set() != 0 || (lbl_eu_80663E28 & 0x200000) != 0) {
        return;
    }
    {
        if ((u8)(mPhase + 0xff) <= 1) {
            if (PartyStateWin_QueryMenuClose(&mPartyStateWin) == 0) {
                MenuPTPadView* pad = reinterpret_cast<MenuPTPadView*>(
                    cf::CfGameManager::getCurrentPad());
                int accept;
                // Retail extracts single bits via rotate-left forms.
                if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
                    accept = (pad->buttons >> 23) & 1;
                } else {
                    accept = (pad->buttons >> 10) & 1;
                }
                if (accept != 0) {
                    PartyStateWin_StepPartySub(&mPartyStateWin);
                    if (CMainMenu_GetInstancePtr() != 0) {
                        ArtsInfo_SetReadyFlag();
                        playUISound(6);
                    }
                    mPhase = 3;
                }
            }
        }
        switch (mPhase) {
        case 0:
            // Background layout finished loading -> start the window open-in.
            if (BgTex_IsLoaded_3E34(&mBgTex) != 0) {
                // Retail leaves arg2/arg3 (r4/r5) unset at this call site.
                PartyStateWin_OpenWindow(&mPartyStateWin);
                mPhase = 1;
            }
            break;
        case 1:
            mPhase = 2;
            break;
        case 2:
            if (PartyStateWin_GetLatchFlag(&mPartyStateWin) != 0) {
                mPhase = 3;
            }
            break;
        case 3:
            // System window opened over us: flag completion on the owner.
            if (UIBattleGetFlagE8() != 0) {
                mField54 = 1;
            }
            break;
        }
        BgTex_Tick_3D54(&mBgTex);
        if (mPhase != 3) {
            PartyStateWin_FrameStep(&mPartyStateWin);
        }
    }
}

void CMenuPTState::cbRenderBefore() {
    // OR-combined guard: MWCC emits the retail beq-body/b-exit pair for the
    // second disjunct (MWCC_CASES "OR-combined guard" pattern).
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() != 0 || (lbl_eu_80663E28 & (1u << 21)) != 0) {
        return;
    }
    if (IsMenuState621F0() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
}
// Constructor: builds the IUICf base, the IScnRender subobject, and the members.
// Retail has no separate constructor function: the allocating factory inlines it.
inline CMenuPTState::CMenuPTState(CProcess* storedParent)
    : mStoredParent(storedParent), mBgTex(0), mPartyStateWin(0, 0), mPhase(0) {}

// create: singleton factory (retail `__ct__CMenuPTState`). Fails if the singleton
// already exists; otherwise allocates + constructs, publishes it, and registers it
// under `parent`.
CMenuPTState* CMenuPTState::sInstance;

CMenuPTState* CMenuPTState::create(CProcess* parent, CProcess* storedParent) {
    if (sInstance != nullptr) {
        return nullptr;
    }
    CMenuPTState* menu = new (CWorkThreadSystem::getWorkMem()) CMenuPTState(storedParent);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

// Singleton-existence query (retail-unmangled symbol; see refreshPtStateAccum).
extern "C" unsigned long menuPTStateIsActive() { return CMenuPTState::sInstance != 0; }

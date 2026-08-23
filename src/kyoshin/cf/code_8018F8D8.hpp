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

// 8-byte element copied by the state-blob copy functions func_80191C88 /
// func_80192268 (two u32 words per iteration).
struct SCopyPair {
    u32 lo;
    u32 hi;
};

// Layout backing func_80191C88's memberwise copy.
struct SCopy_80191C88 {
    float f_04, f_08, f_0c;
    float f_14, f_18, f_1c, f_20, f_24, f_28, f_2c, f_30, f_34;
    u8 b_38;
    float f_3c;
    SCopyPair arrA[52];
    float f_1e0, f_1e4, f_1e8, f_1ec, f_1f0, f_1f4, f_1f8, f_1fc;
    float f_200, f_204, f_208, f_20c, f_210, f_214, f_218, f_21c, f_220;
    u8 b_224, b_225, b_226;
    float f_228, f_22c, f_230, f_234, f_238, f_23c, f_240, f_244, f_248, f_24c;
    float f_250, f_254, f_258, f_25c, f_260, f_264, f_268, f_26c;
    SCopyPair arrB[16];
    SCopyPair arrC[16];
    float f_370, f_374, f_378, f_37c, f_380, f_384, f_388, f_38c, f_390, f_394, f_398, f_39c;
    float f_3a0, f_3a4, f_3a8, f_3ac, f_3b0, f_3b4, f_3b8, f_3bc, f_3c0, f_3c4, f_3c8, f_3cc;
    float f_3d0, f_3d4, f_3d8, f_3dc, f_3e0, f_3e4, f_3e8, f_3ec, f_3f0, f_3f4, f_3f8, f_3fc;
    float f_400, f_404, f_408, f_40c, f_410, f_414, f_418, f_41c, f_420, f_424, f_428, f_42c;
    float f_430, f_434, f_438, f_43c, f_440, f_444, f_448, f_44c, f_450, f_454, f_458, f_45c;
    float f_460, f_464, f_468, f_46c, f_470, f_474, f_478, f_47c, f_480, f_484, f_488, f_48c, f_490, f_494;
    u8 b_498;
    float f_49c, f_4a0, f_4a4, f_4a8, f_4ac, f_4b0, f_4b4, f_4b8;
    u8 b_4bc, b_4bd, b_4be, b_4bf;
    float f_4c0, f_4c4, f_4c8, f_4cc, f_4d0;
    u16 h_4d4, h_4d6, h_4d8;
    u8 b_4da;
    u16 h_4dc;
    float f_4e0, f_4e4, f_4e8, f_4ec, f_4f0, f_4f4, f_4f8, f_4fc, f_500, f_504, f_508, f_50c, f_510;
    float f_514, f_518, f_51c, f_520, f_524, f_528, f_52c;
    u16 h_530, h_532, h_534, h_536, h_538;
};

// Layout backing func_80192268's memberwise copy.
struct SCopy_80192268 {
    float f_04, f_08, f_0c, f_10, f_14, f_18, f_1c, f_20, f_24, f_28, f_2c, f_30, f_34, f_38, f_3c;
    u8 b_40, b_41, b_42, b_43;
    float f_48, f_4c, f_50, f_54;
    u8 b_58, b_59;
    float f_60, f_64, f_68, f_6c;
    u8 b_70, b_71;
    float f_78, f_7c, f_80, f_84;
    u8 b_88, b_89;
    float f_90, f_94, f_98, f_9c, f_a0, f_a4, f_a8, f_ac, f_b0;
    u8 b_b4, b_b5, b_b6, b_b7;
    float f_bc, f_c0, f_c4, f_c8, f_cc, f_d0, f_d4, f_d8;
    u8 b_dc, b_dd, b_de, b_df;
    float f_e0, f_e4, f_e8, f_ec, f_f0;
    u8 b_f4;
    SCopyPair arr1[16];
    u8 b_178, b_179, b_17a;
    float f_180, f_184, f_188, f_18c, f_190, f_194, f_198, f_19c, f_1a0;
    u8 b_1a4;
    float f_1a8, f_1ac;
    u8 b_1b0, b_1b1, b_1b2, b_1b3, b_1b4, b_1b5;
    float f_1bc, f_1c0, f_1c4, f_1c8, f_1cc, f_1d0, f_1d4, f_1d8, f_1dc;
    u8 b_1e0;
    float f_1e4, f_1e8;
    u8 b_1ec, b_1ed, b_1ee, b_1ef, b_1f0, b_1f1;
    u8 b_1f4, b_1f5, b_1f6;
    u16 h_1f8, h_1fa, h_1fc;
    u8 b_1fe;
    float f_200, f_204, f_208, f_20c, f_210, f_214, f_218, f_21c, f_220, f_224;
    SCopyPair arr2[18];
    float f_2b8, f_2bc, f_2c0, f_2c4, f_2c8, f_2cc, f_2d0, f_2d4, f_2d8, f_2dc, f_2e0, f_2e4;
    u8 b_2e8;
    SCopyPair arr3[16];
    float f_36c;
    u16 h_370;
    u8 b_372, b_373, b_374, b_375, b_376, b_377, b_378, b_379, b_37a, b_37b, b_37c;
    SCopyPair arr4[1024];
    u16 h_237e;
    u8 b_2380, b_2381, b_2382, b_2383;
    float f_2384, f_2388, f_238c, f_2390, f_2394, f_2398, f_239c, f_23a0;
    SCopyPair arr5[128];
    float f_27a4;
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
extern u16 lbl_eu_80663E48[];
extern u16 lbl_eu_80663E4C[];

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

// System-window open query + sound helpers (Move, state 3).
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
void func_80138078(u32 id); // UI sound (C++-mangled retail name)

// CfGameManager pre-mangled imports (same convention as CPartyStateWin.hpp:
// verbatim retail symbols, real argument shapes observed at call sites).
extern "C" void func_80081D8C__Q22cf13CfGameManagerFv(u32 value);
extern "C" cf::CfObjectMove* func_8007FF6C__Q22cf13CfGameManagerFv(u16 objectId,
                                                                    void* obj,
                                                                    float amount);
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int selector);
extern "C" void func_800BC3B0(cf::CfObjectMove* player, float value);
extern "C" void func_8008402C__Q22cf13CfGameManagerFv(u32 objectValue, u32 flag,
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
typedef int (MenuCmdHost::*MenuCmdHandler)(u16, u32*);

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
extern "C" void* func_8009EC9C(u16 index);
extern "C" void func_800A30E4(void* data);
extern "C" void func_800A1370(void* data);
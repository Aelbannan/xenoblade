#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/CScn.hpp>
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CPartyStateWinImports.hpp"
// Container used by the catalog helper functions CfCmd_Call86778 / CfCmd_Call82C48.
// The hosting object keeps a cf::CfGameManager* reference at offset 0x408.
struct CFuncHost {
    u8 _0[0x408];
    cf::CfGameManager* manager;  // 0x408
};

// Object used by CfCmd_ResolveSlot1E4: a raw pointer at offset 0x408 that is
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

// Result struct returned by CtrlRemote_GetFileEventIds in CfCmd_ResolveSlot1E4.
struct UnkR31_8019E88 {
    u16 field_0x0;
    u16 field_0x2;
};

// (Fake vtable proxies removed: player slots now dispatch through real
// cf::CfObject virtuals - UVF22/30/31/33/9; flag word is CfObject::mFlags68.
// See code_8018F8D8.cpp.)

// (Fake actor/pos vtable proxies removed: slots now dispatch through real
// cf::CObjectState (0x30), cf::CActorParam (0x158/0x18C/0x190/0x2BC/0x308)
// and cf::CfObject (0x84/0xAC) virtuals. See code_8018F8D8.cpp.)
// Actor reference used by CfCmd_PickChain (both the queried actor and the
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

// Result record written by CfCmd_PickChain (first argument).
struct FuncResultRef {
    u8 _00[0x4];
    volatile u32 field_0004;                  // +0x04 result code
    volatile u32 field_0008;                  // +0x08 secondary flag
    FuncActorRef* field_000c;         // +0x0C chosen actor
};

// Battle-manager view for CfCmd_PickChain.
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

// Enum list view used by CfCmd_PickChain's candidate sweep.
struct CfEnumList90940 {
    u8 _0[0x620];
    volatile u32 count;               // +0x620 element count
};


// --- imports used by the Move / dispatcher functions ---------------------

// Colour-unpack scale/magic pair (CfCmd_FadeColor's u8->float ladder).
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

// u16 pair tables read at element [1] by CfCmd_PartyMenuDisp's cmd-0x19 path.
// Sized [2]: MWCC only assumes .sbss small-data addressing (sda21 reloc)
// for extern arrays whose bounds are visible.
extern u16 gmFlagPairs48[2];
extern u16 gmFlagPairs4C[2];

// Object id looked up by CfCmd_PartyRebuild through func_8007FF6C (.sdata halfword).
extern const u16 gmInitArg;


// CfGameManager pre-mangled imports (same convention as CPartyStateWin.hpp:
// verbatim retail symbols, real argument shapes observed at call sites).
extern "C" void assignCameraTarget__Q22cf13CfGameManagerFv(u32 value);
extern "C" cf::CfObjectMove* func_8007FF6C__Q22cf13CfGameManagerFv(u16 objectId,
                                                                    void* obj,
                                                                    u32 selector,
                                                                    float amount);
extern "C" void CfObjectMove_setMoveSpeedGated(cf::CfObjectMove* player, float value);
extern "C" void triggerPlayerEffects__Q22cf13CfGameManagerFv(u32 objectValue, u32 flag,
                                                        float value);

// (Fake char-data vtable proxy removed: +0x17C sub-object slots 0xA4/0xA8
// now dispatch through real cf::CActorParam virtuals Func4/Func5.
// See code_8018F8D8.cpp.)

// Slot list returned by CtrlObjectParam_GetSlotTableBase (arr1[3] + arr2[6] packed at +4;
// CfCmd_PartyRebuild walks nine u32 slots).
struct PartySlotList {
    u8 _0[4];
    u32 slots[9];
};

// Opaque receiver type for the menu-command handler table dispatched by
// CfCmd_PumpRingBuf through member-function pointers.
class MenuCmdHost;
typedef int (MenuCmdHost::*MenuCmdHandler)(u16, u32, u32*, u32);

// 0x29-entry handler table indexed by the popped command byte (lbl_eu_80532838).
extern MenuCmdHandler lbl_eu_80532838[];

// CfRes ring-buffer pump used by CfCmd_FadeColor-era dispatchers (defined in
// CfRes.cpp); the ring object is the real CfResBuffer (CfRes.hpp) and the
// record pop is its popRecord() member.
struct CfResBuffer;

// Party-slot character-data lookups used by CfCmd_PartyRebuild.
extern "C" void* CtrlObjectParam_GetCharEntry(u32 index);  // matches CAIAction.hpp declaration
extern "C" void CtrlObjectParam_RecollectArtsRows(void* data);
extern "C" void CtrlObjectParam_SyncParamFromActor(void* data);
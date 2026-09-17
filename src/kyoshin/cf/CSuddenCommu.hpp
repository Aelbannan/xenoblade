#pragma once

#include <types.h>
#include "kyoshin/cf/object/IObjectInfo.hpp"
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

// Global-scope fwd decl (cue object defined below); used by the CSuddenCommu
// field_20 volatile-view union.
struct CSuddenCommuVoiceCue;

namespace cf{
    //size: 0x30?
    // novtable: the retail vtable is a split-data object (lbl_eu_805339C0);
    // suppressing MWCC's own __vt__ keeps the ctor reloc name byte-identical.
    class __declspec(novtable) CSuddenCommu : public IObjectInfo {
    public:
        CSuddenCommu();
        virtual ~CSuddenCommu(){}
        virtual void IObjectInfo_UnkVirtualFunc1();

        //0x0: vtable
        //0x0-4: IObjectInfo?
        /* 0x04 */ s16 field_4;
        /* 0x06 */ union { struct { s16 field_6; s16 field_8; }; s16 pair_6[2]; };
        /* 0x0A */ s16 field_A;
        /* 0x0C */ s16 field_C;
        /* 0x0E */ s16 field_E;
        /* 0x10 */ volatile u32 field_10; // voice id probed via func_802A3748
        /* 0x14 */ u32 field_14;
        /* 0x18 */ volatile float field_18;
        /* 0x1C */ float field_1C;
        /* 0x20 */ union {
            u32 field_20;
            // Typed-pointer view used by func_801BB81C: accessing the cue
            // through the real CSuddenCommuVoiceCue* type lets MWCC's alias
            // analysis prove the field_B0 store cannot touch the sdata2
            // float pool, which reproduces the retail issue order.
            ::CSuddenCommuVoiceCue* field_20p;
        };
        /* 0x24 */ volatile u32 field_24;
        /* 0x28 */ u32 unk28;
        /* 0x2C */ float unk2C;
    };
}

// Battle-actor base pointer: CfGameManager::getPlayer(i) returns the embedded
// CfObjectMove at base+0x3E9C; subtracting 0x3E9C recovers the base used by
// the sudden-commu functions. Only the touched fields are declared.
struct CSuddenCommuVoiceAct {
    u8 pad_00[0x8];
    u16 field_3388;   // 0x3388 relative to the actor base
};

// Sub-object at actor+0x04: virtual slot 0x30 returns the object-list head
// (its first word is read as u32 and handed to func_80174C98).
// Abstract - cast-only, never constructed.
struct CSuddenCommuSub4 {
    virtual void v00() = 0; virtual void v01() = 0; virtual void v02() = 0; virtual void v03() = 0;
    virtual void v04() = 0; virtual void v05() = 0; virtual void v06() = 0; virtual void v07() = 0;
    virtual void v08() = 0; virtual void v09() = 0;
    virtual u32* vf30() = 0;   // slot 0x30 (index 12, incl. MWCC vtable prefix)
};

// Object behind actor+0x3ED4: u32 at +0x36C (non-zero test in func_801BB464).
struct CSuddenCommuBody36C {
    u8 pad_00[0x36C];
    u32 field_36C;                        // 0x36C
};

struct CSuddenCommuActor {
    void** vtable;                  // 0x00 (slots 0x88/0xE0/0x128 used here)
    CSuddenCommuSub4* field_4;      // 0x04 (sub-object, vtable slot 0x30)
    u8 pad_08[0x15E0 - 0x08];
    void* field_15E0;               // 0x15E0 (battle-object handle probed via func_80260518)
    u8 pad_15E4[0x3380 - 0x15E4];
    CSuddenCommuVoiceAct voiceAct;  // 0x3380 (handed to func_801537E0)
    u8 pad_338A[0x3E9C - 0x338A];
    void* moveSpot;                 // 0x3E9C (getPlayer() points here)
    u8 pad_3EA0[0x3ED4 - 0x3EA0];
    CSuddenCommuBody36C* field_3ED4;  // 0x3ED4
    u8 pad_3ED8[0x3F00 - 0x3ED8];
    u32 flags3F00;                  // 0x3F00 (bit 0x2 = player-controlled)
    u8 pad_3F04[0x0C];
    void* field_3F10;               // 0x3F10
    u8 pad_3F14[0x14];
    u16 field_3F28;                 // 0x3F28
    u8 pad_3F2A[0x3F60 - 0x3F2A];
    void* subField3F60;             // 0x3F60
};

// Local battle-move flag view (retail E2A9C_BattleMoveData lives in
// CBattleManager.cpp; only the flag words at 0x74/0x78 are touched).
struct CSuddenCommuMoveData {
    u8 pad_00[0x74];
    u32 field_74;    // 0x74
    u32 field_78;    // 0x78
};

// Effect/cue object produced by func_800451D8 for the sudden-commu voice cue.
struct CSuddenCommuVoiceCue {
    void** vtable;          // 0x00 (slot 0x88 takes a float)
    u8 pad_04[0xB0 - 0x04];
    void* field_B0;         // 0xB0
};



struct CSuddenCommuActBody;

// 8-byte holder around a CfObjEnumList* (CTaskGame_enumListCtor / __dt__80043E88).
// Derives from the shared CfMoveEnumHolder view so the C-linkage import
// declared for the move TUs accepts this TU's holder without casts.
#include "kyoshin/cfsys/CfObjectImplMove.hpp"
struct CSuddenCommuEnumHolder : CfMoveEnumHolder {
};

// View of the CBattleManager actor list: the reslist sentinel pointer
// (mActorList3.mHead) sits at +0x48.
struct CSuddenCommuBmList {
    u8 pad_00[0x48];
    void* mHead;    // 0x48
};

// Intrusive list node (next link at +0x00).
struct CSuddenCommuListNode {
    CSuddenCommuListNode* next;   // 0x00
};

// CBattleManager field view (retail CBattleManager.hpp is not includable from
// this TU - CfObjectActor.hpp decl conflict): only 0x1AA/0x20C8 are touched.
struct CSuddenCommuBmView {
    u8 pad_00[0x1AA];
    u8 field_1AA;                   // 0x1AA
    u8 pad_1AB[0x20C8 - 0x1AB];
    s16 field_20C8;                 // 0x20C8
};

// Global settings object (retail CfGimmickGlobal): only 0x214 touched.
struct CSuddenCommuGlobal {
    u8 pad_00[0x214];
    u32 field_214;                  // 0x214
};

// Object returned by the actor vtable slot 0x298: int state at +0x48.
struct CSuddenCommuActBody {
    u8 pad_00[0x48];
    int field_48;                   // 0x48
};

// 0x34-byte command payload built for func_800EA9A8 (voice/battle cue); only
// the fields the retail writes are declared.
struct CSuddenCommuCmd {
    u8 pad_00[0xC];
    u16 field_C;                    // 0xC
    u8 pad_E[2];
    u32 field_10;                   // 0x10
    u8 pad_14[0x20 - 0x14];
    f32 field_20;                   // 0x20
    u8 pad_24[0x34 - 0x24];
};

// u32-word pair / f64 view used for MWCC's 0x43300000 int->float conversion
// (subtracts the retail .sdata2 magic double lbl_eu_80667E40 so the pool reloc
// matches the retail constant instead of an MWCC-synthesised @N entry).
union CSuddenCommuF64Conv {
    u32 w[2];
    f64 d;
};
// CBattleManager sub-object at +0x194 (party gauge), handed to func_8018C820.
struct CSuddenCommuBmGauge {
    u8 pad_00[0x194];
    u32 field_194;                  // 0x194
};

// Forward decls for the C-linkage imports below, matching the types the chain
// / scene headers use for the same retail symbols (CChain.hpp takes
// CChainBattleObjTail*, CfObjectMove.hpp / CTaskGame.hpp take CScn*).
class CChainBattleObjTail;
class CScn;
class CBattleManagerView;

// C-linkage callees defined in CSuddenCommu.cpp (retail symbols are unmangled
// global func_* names - the declarations below give the definitions C linkage
// so the object emits the exact retail symbol names).
extern "C" {
    void func_801BA490(cf::CSuddenCommu* self);
    void func_801BA978(cf::CSuddenCommu* self);
    void func_801BC6A4(cf::CSuddenCommu* self, int val, int num);
    // Imported voice-node lookup (defined in voice/CCharVoiceMan.cpp): returns
    // 0 when no voice node is registered for the given voice id.
    int func_802A3748(u32 arg);
    // Same-TU siblings (defined in CSuddenCommu.cpp; C linkage so the call
    // relocs and definitions keep the unmangled retail names).
    int func_801BA2DC(cf::CSuddenCommu* self);
    void func_801BB464(cf::CSuddenCommu* self, int playerIdx, int mode, CSuddenCommuActor* player, int arg5);
    void func_801BC474(cf::CSuddenCommu* self);
    void func_801BC590(cf::CSuddenCommu* self);
    // Same-TU init/reset (clears the commu state and retires any voice id).
    void func_801BA1DC(cf::CSuddenCommu* self);
    // Voice-manager imports (defined in voice/CCharVoiceMan.cpp): retire/next
    // voice id. C linkage keeps the call relocs at the unmangled retail names.
    void func_802A35B8(u32 arg);
    int CCharVoiceMan_AllocCommuVoiceId();
    // Battle-voice dispatch (defined in voice/CCharVoiceMan.cpp): run the
    // voice node for `voiceId` with the target spot and the commu's result id.
    // Signature matches CChain.hpp (int/CChainBattleObjTail*/int) so both
    // headers can be included in one TU without a rename guard.
    int func_802A3680(int a, CChainBattleObjTail* b, int c);
    // Battle-manager singleton: declared once in
    // kyoshin/cf/CBattleManagerApi.hpp (included at the top of this header).
    // Voice/help imports used by the sudden-commu triggers (unmangled retail
    // names - C linkage keeps the call relocs verbatim).
    u32 func_8009CF8C(u32 resourceId);
    // No-arg form: retail callers never materialize r3 before this call
    // (stale-r3 pattern - the callee ignores any incoming argument).
    int func_8017FD44(void);
    CSuddenCommuGlobal* getUnk80664658();
}

// Retail CSuddenCommu vtable (.data, split unit). The class is
// __declspec(novtable), so the ctor stores this label explicitly instead of
// the compiler-generated __vt__Q22cf12CSuddenCommu.
extern void* lbl_eu_805339C0[];

// Battle-manager voice-volume broadcast (CBattleManager.cpp owner; declared
// here because CChainTime.hpp's decl conflicts with the current CChainEffect
// return type in this TU).
namespace cf { class CBattleManager; }
extern "C" void func_800EA484(cf::CBattleManager*, f32, int);

// Effect-linkage allocator (CTaskGameEff.cpp owner).
extern "C" void* func_800451D8(u32 cls, int param);

// Retail sdata2 float constants (values live in the retail binary). Non-const:
// MWCC must reload them after calls, so loop uses stay at the use site (retail
// shape) instead of being hoisted into a saved FPR.
extern volatile float lbl_eu_80667E30;
extern float lbl_eu_80667E34;   // voice-count scale (held in a saved FPR across the sweep)
extern float lbl_eu_80667E38;
extern const f64 lbl_eu_80667E40;    // .sdata2 double: 0x4330000080000000 u32->f32 magic (const so MWCC CSEs one FPR copy across calls)
extern float lbl_eu_80667E48;
extern float lbl_eu_80667E4C;   // commu partner timer clamp (voice re-arm)
extern float lbl_eu_80667E50;
extern float lbl_eu_80667E5C;   // cue countdown timeout threshold
extern float lbl_eu_80667E60;   // camera/trigger frame value (func_80133F48 arg)
extern float lbl_eu_80667E64;   // field_18 lower clamp (idle commu path)
extern float lbl_eu_80667E54;
extern float lbl_eu_80667E58;

// Running voice-count seed (.sdata) accumulated across the player sweep.
extern u32 lbl_eu_806625E0;

// C-ABI imports used by the sudden-commu voice sweep (retail symbols are
// unmangled global func_* names - declared here rather than TU-locally).
extern "C" {
    // Battle-object stat / event-flag probes (code_8025FB10.cpp).
    int func_80260518(void* self, int id, u32* outVal, f32* outF);
    int func_80260264(void* self, int id, void* outVal);
    // Battle-command dispatch and battle-move accessor (CBattleManager.cpp).
    void func_800EA9A8(void* bm, void* actor, CSuddenCommuCmd* cmd, int size, int flag);
    float func_800D81A8(void* obj, void* target, void* source);
    void func_8018C820(void* obj, int value);
    // Voice/battle-manager helpers.
    void func_80280BF0();
    void addTableValueWithClamp__Q22cf13CfGameManagerFv(int a, int b, int c);
    // CfGameManager gate used by the commu re-arm (retail pre-mangled name;
    // the class member is declared in CfGameManager.hpp, this is the bare
    // Fv-form import so the call reloc keeps the retail symbol verbatim).
    bool isTimerActive__Q22cf13CfGameManagerFv();
    bool isGlobalCamFlagSet__Fi(int mask);
    // Commu camera/trigger helpers (retail pre-mangled names; the Fv suffix
    // does not reflect the actual argument counts - see addTableValueWithClamp).
    int isClassicController__Q22cf13CfGameManagerFv(int arg);
    void clearPlayerEffect__Q22cf13CfGameManagerFv(void* obj);
    // Camera/trigger object accessors (lbl_eu_80663E14 is a global pointer).
    int func_8017FD4C(int);
    void func_8017FEF0(void* obj, int arg);
}

// Commu camera/trigger globals (.sdata). E14's CScn* matches CfObjectMove.hpp
// / CTaskGame.hpp / code_8027513C.hpp; a void* decl here would clash with
// those in any TU that includes both this header and the scene headers.

// Pad button-flag view (retail CPad first word).
struct CSuddenCommuPadView {
    u32 field_00;   // +0 button flags
};

// Camera/trigger position view (func_8049603C result): f32 at +0xC.
struct CSuddenCommuCamView {
    u8 pad_00[0xC];
    f32 field_C;    // 0xC
};

// Voice-id / voice-act tables (@sda21 in func_801BC6A4 / func_801BB464).
// Declared as leading-element scalars so MWCC emits the retail `li rX,
// lbl@sda21 / lbzx` indexed form instead of lis/addi HA/LO addressing.
extern int lbl_eu_806625E4;
extern u8 lbl_eu_806625E8;
extern u8 lbl_eu_806625F0;
extern u8 lbl_eu_806625F8;
extern u8 lbl_eu_80662600;
extern u16 lbl_eu_80662608[1];
extern const u8 lbl_eu_805050B0[];
extern u32 lbl_eu_80575870[];   // .bss: state-handler PMF table (12-byte ptmf stride)
extern "C" int func_80133F48(int id, float f);   // camera/trigger helper (CVision.hpp owner)
extern "C" void func_801537E0(void* voiceAct);   // voice-act reset (CVision.hpp owner)
// Battle-actor enum-list helpers are declared by CfObjectImplMove.hpp
// (owner: code_800B06A4.hpp); this TU picks them up through the include chain.
// Retail aliases the CfGameManager singleton getter under its raw symbol
// name; CSuddenCommu's driver calls that alias.
extern "C" bool isSceneLoading__Q22cf13CfGameManagerFv(); // canonical bool form (CfObjectMove.hpp owner)
extern "C" void func_801BADE4(cf::CSuddenCommu* self);   // re-arm scan (plain retail symbol)
extern "C" int func_801BBCBC(cf::CSuddenCommu* self);    // per-frame tick (plain retail symbol)

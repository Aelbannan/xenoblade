#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "monolib/util/MemManager.hpp"

namespace cf{

    // Forward declaration for the C-linkage imports below (pointer-only use).
    class CfObjectMove;

    // Character-voice event-list node owned by CCharVoiceMan. Link at +0x0C.
    // The first 0x0C bytes double as a __ptmf for type comparison; the +0x1C
    // word is the voice trigger's dispatch slot (a per-node virtual indirection
    // whose actual handler lives at a fixed offset in the block it points to).
    struct CVoiceNodeDispatch {
        u8 pad0[8];
        void (*fn_08)(void* node);                // +0x08 (level thread)
        void (*fn_0C)(void* node, void* arg);     // +0x0C (interact voice)
        u8 pad10[8];
        void* (*fn_18)(void* node, void* a, void* b); // +0x18 (battle voice)
    };

    struct CSoundNode {
        u8 unk0[0xC];
        CSoundNode* next;   // +0x0C
        u8 unk10[0x8];      // +0x10 .. +0x17
        u32 field_18;       // +0x18
        CVoiceNodeDispatch* field_1C;  // +0x1C dispatch block
        u8 unk20[0x10];     // +0x20 .. +0x30
    };

    // Actor handed to the voice factory (flag word at +0x64).
    struct CVoiceFactoryActor {
        u8 unk0[0x64];
        u32 field_64;   // +0x64
    };

    // Actor layout for func_802A2210 (reads a u32 at +0x3F10).
    struct CVoiceActorInfo {
        u8 unk0[0x3F10];
        u32 field_3F10;   // +0x3F10
    };

    // Player object whose character-voice move-base lives at +0x3E9C.
    // (Only the address of moveBase is taken; it is the CfObjectMove block.)
    struct CVoiceActorBase {
        u8 unk0[0x3E9C];
        u8 moveBase;   // +0x3E9C (marker; only address is used)
    };

    // vtable slot at +0x2BC used by func_802A1EA8's state check.
    struct CVoiceActorVTable {
        u8 pad[0x108];
        int (*fn_108)(void* self);        // +0x108 actor level
        u8 pad108[0x130 - 0x10C];
        float (*fn_130)(void* self);      // +0x130 actor gauge float
        u8 pad130[0x2BC - 0x134];
        int (*fn_2BC)(void* self);        // +0x2BC state check
    };

    // Move-base object embedded at CVoiceActorState +0x3E9C (CfObjectMove block).
    // Its vtable's +0x4C slot is invoked by func_802A1EA8.
    struct CVoiceMoveVTable {
        u8 pad[0x4C];
        void* (*fn_4C)(void* self);  // +0x4C
    };
    struct CVoiceMoveBase {
        CVoiceMoveVTable* vtable;    // +0x00
    };

    // Large CfObjectPc-derived actor with a flag word at +0x3F00 used by the
    // voice trigger functions (bit 30 battle/interact, bit 29 other-actor flag).
    struct CVoiceActorState {
        CVoiceActorVTable* vtable;   // +0x00
        u8 unk4[0x3E9C - 4];         // -> +0x3E9C
        CVoiceMoveBase moveBase;     // +0x3E9C (CfObjectMove block)
        u8 unk3EA0[0x3F00 - 0x3EA0]; // -> +0x3F00
        u32 field_3F00;   // flags word at +0x3F00
        u8 unk3F04[4];    // -> +0x3F08
        u32 field_3F08;   // +0x3F08 (bit 15 = 0x10000)
        u8 unk3F0C[0x1C]; // -> +0x3F28
        u16 field_3F28;   // +0x3F28
        u8 unk3F2A[0x36]; // -> +0x3F60
        void* field_3F60; // +0x3F60
    };

    // Signed word at CBattleManager +0x194 (battle-participant count).
    struct CBattleCountAccessor {
        u8 unk0[0x194];
        int field_194;   // +0x194
    };

    // Ring-list node used by CBattleManager lists (next at +0x00).
    struct CVoiceBattleNode {
        CVoiceBattleNode* next;   // +0x00
    };

    // Accessor for CBattleManager return: list sentinel lives at +0x28.
    struct CBattleListAccessor {
        u8 unk0[0x28];
        CVoiceBattleNode* list0;   // +0x28
    };

    // CCharVoiceMan-side ring sentinel: CBattleManager +0x48.
    struct CBattleManagerNodeList {
        u8 unk0[0x48];
        CVoiceBattleNode* sentinel;   // +0x48
    };

    // Result of func_800BFC68 (a move-converted object); +0x3ED4 is a pointer
    // to an object holding a vtable whose +0x40 slot is probed with 0x800.
    struct CVoiceEdge;
    struct CVoiceBFC68 {
        u8 pad0[0x3ED4];
        CVoiceEdge* field_3ED4;   // +0x3ED4
    };
    struct CVoiceEdgeVTable {
        u8 pad[0x40];
        int (*fn_40)(void* self, int a);  // +0x40
    };
    struct CVoiceEdge {
        CVoiceEdgeVTable* vtable;   // +0x00
    };

    // Node returned by func_802A7A54; +0x04 is a pointer to an object whose
    // vtable +0x30 slot returns a pointer.
    struct CVoiceSrcInnerVTable {
        u8 pad[0x30];
        void* (*fn_30)(void* self);  // +0x30
    };
    struct CVoiceSrcInner {
        CVoiceSrcInnerVTable* vtable;   // +0x00
    };
    struct CVoiceSrcNode {
        u8 pad0[4];
        CVoiceSrcInner* field_4;   // +0x04
    };

    class CCharVoiceMan : public IFactoryEvent {
    public:
        CCharVoiceMan();
        virtual ~CCharVoiceMan();
        virtual void FactoryEvent2();

        u8 unk4[0x200];
        u32 unk204;
        u32 unk208;
        CSoundNode* unk20C;   // head of voice-event list
        CSoundNode* unk210;   // tail of voice-event list
        u8 unk214;
        u8 unk215;
        u8 unk216[2]; //filler?
        float unk218;
        //Definitely classes of some kind. Maybe related to position?
        u32 unk21C;
        u8 unk220;
        u8 unk221;
        u8 unk222;
        u8 unk223;
        u32 unk224;
        u8 unk228;
        u8 unk229;
        u8 unk22A;
        u8 unk22B;
        u32 unk22C;
        u32 unk230;
    };

}

// -- C-linkage imports (retail symbols) --------------------------------------
// CCharVoiceMan ctor symbol (placement construction of a heaped 0x234 buffer);
// kept external so the call site emits a real call rather than being inlined.
extern "C" cf::CCharVoiceMan* __ct__CCharVoiceMan(cf::CCharVoiceMan* self);
// Retail @sda21 reset float for unk218.
extern "C" float lbl_eu_80668C68;
// Retail @sda21 globals.
extern "C" u32 lbl_eu_80663E24;    // presentation/event bitfield (bit 0x00400000)
class CScn;
extern "C" CScn* lbl_eu_80663E14;  // scene / allocation-source handle (defined CScn* in kyoshin/cf/CfGameManager.cpp)
extern "C" u32 lbl_eu_80664A5C;    // global character-voice counter
// Scene-model helper returning an allocation-region handle (mtl MemManager).
extern "C" mtl::ALLOC_HANDLE func_80496004(void* src);
// Sibling voice-module free functions (cvsys TUs) - un-mangled retail names.
extern "C" int func_802B03A4(void* self);
extern "C" cf::CSoundNode* func_802A9604();
extern "C" cf::CSoundNode* __ct__802A4E48();
extern "C" cf::CSoundNode* __ct__802A96C0();
// Additional retail node constructors / helpers used by the enqueue functions.
extern "C" cf::CSoundNode* func_802A8B6C();
extern "C" cf::CSoundNode* func_802B5970();
extern "C" cf::CSoundNode* __ct__CVS_THREAD_ORDER(int order);
extern "C" cf::CSoundNode* __ct__802A8DE8();
extern "C" cf::CSoundNode* func_802A6958();
extern "C" cf::CSoundNode* __ct__802A5830();
extern "C" cf::CSoundNode* __ct__802A8C04();
extern "C" cf::CSoundNode* func_802B8D4C();
// Node factory helpers for the frequency / level-up voice enqueue functions.
extern "C" cf::CSoundNode* func_802A3EF0();
extern "C" cf::CSoundNode* func_802AF3DC(void* self);
extern "C" cf::CSoundNode* func_802A5A14(void* a, void* c);
extern "C" cf::CSoundNode* __ct__802A7254(void* self, void* c);
extern "C" cf::CSoundNode* __ct__802A5ED4();
extern "C" cf::CSoundNode* func_802AF4FC(void* self, void* c);
extern "C" cf::CSoundNode* func_802AF43C(void* a, void* b, void* c);
extern "C" cf::CSoundNode* func_802A5B04(void* self, int flag);
extern "C" cf::CSoundNode* func_802AF49C(void* self);
// Per-object state flag lookup (tests a slot at self+0x8 against an id).
extern "C" int func_80148778(void* self, int id);
// MWCC member-function-pointer runtime helpers (compare / probe).
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" long __ptmf_test(void* ptmf);
class CVS_THREAD;
// Reference member-function-pointer the voice nodes are matched against.
extern "C" void* lbl_eu_805398C0;
// Resolve a voice-action object from a voice source id (C-linkage).
extern "C" void* func_8016FE34(void* src);
// Play a specific character-voice sound (cvsys sibling; C-linkage retail name).
extern "C" void func_802AF9D0(void* r3, int voiceId, int r5);
extern "C" cf::CSoundNode* func_802AF56C(void* c);
extern "C" int func_802A7870(void* arr, int capacity, int unused);
extern "C" cf::CSoundNode* __ct__802A92D8(void* b, void* a);
extern "C" int func_802A3E88(CVS_THREAD* node);
// Create a battle-voice source node from an old battle-count (cvsys sibling).
extern "C" cf::CSoundNode* func_802A6820(int oldCount);
// Pure voice nodes created with an int argument (1=levelled, 0=normal).
extern "C" cf::CSoundNode* __ct__802AF5CC(int a);
extern "C" cf::CSoundNode* __ct__802AB5F0();
extern "C" cf::CSoundNode* __ct__802A4870(int a);
// Convert a CfObjectMove to a battle-participant object.
extern "C" void* func_800BFC68__FPQ22cf12CfObjectMove(void* self);
// Create a battle-voice node from a voice-action source.
extern "C" cf::CSoundNode* func_802B0344(void* src);
// Current battle/field phase id.
extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv(void);
// Create a battle-voice trigger node.
extern "C" void* func_802A7A54(int a);
// Run one node's level-thread handler (list sweep).
extern "C" void func_802A3ACC(void* node);
// Check a just-resolved voice action against a source handle.
extern "C" int func_80174C98(void* r3, u32* r4, int r5);
// Byte/word/shift @sda21 globals used by func_802A1610.
extern "C" u8  lbl_eu_80663DA0;
extern "C" u16 lbl_eu_80663E42;
extern "C" float lbl_eu_80668C6C;
extern "C" float lbl_eu_80668C70;
extern "C" float lbl_eu_80668C74;
extern "C" float lbl_eu_80668C78;
extern "C" float lbl_eu_80668C7C;
extern "C" cf::CSoundNode* func_802A6DF4();
extern "C" float lbl_eu_80668C80;  // 0x3F00-flag probability divisor
// Retail node constructors (cvsys TUs) - un-mangled retail names.
extern "C" cf::CSoundNode* __ct__CVS_THREAD_PARTY_GAGE();
extern "C" cf::CSoundNode* __ct__802A6AA8();
extern "C" cf::CSoundNode* __ct__802A5B88();
extern "C" cf::CSoundNode* __ct__802A86CC();
extern "C" cf::CSoundNode* __ct__802A6E84();
extern "C" cf::CSoundNode* func_802B9064();
extern "C" cf::CSoundNode* func_802A4798();
extern "C" cf::CSoundNode* func_802AE38C();
extern "C" cf::CSoundNode* func_802A7674();
extern "C" cf::CSoundNode* func_802A8AC8();
extern "C" cf::CSoundNode* func_802A7744();
#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"

namespace cf{

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
    struct CVoiceBFC68 {
        u8 pad0[0x3ED4];
        void* field_3ED4;   // +0x3ED4
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
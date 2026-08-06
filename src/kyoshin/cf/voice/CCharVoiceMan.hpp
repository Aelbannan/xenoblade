#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"

namespace cf{

    // Character-voice event-list node owned by CCharVoiceMan. Link at +0x0C.
    struct CSoundNode {
        u8 unk0[0xC];
        CSoundNode* next;   // +0x0C
        u8 unk10[0x8];      // +0x10 .. +0x17
        u32 field_18;       // +0x18
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

    // Ring-list node used by CBattleManager lists (next at +0x00).
    struct CVoiceBattleNode {
        CVoiceBattleNode* next;   // +0x00
    };

    // Accessor for CBattleManager return: list sentinel lives at +0x28.
    struct CBattleListAccessor {
        u8 unk0[0x28];
        CVoiceBattleNode* list0;   // +0x28
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
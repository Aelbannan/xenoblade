#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_BATTLE_END: voice thread state machine for battle-end sequences.
// Allocates 0x46 (70) bytes.  Overrides vmethod[2] (blank1) to return that size.
// Layout: 0x20 is an array of voice-handle slots, 0x2C/0x30 are an active-index
// / limit pair, 0x34 the slot count, and 0x38-0x3A are small state flags.
class CVS_THREAD_BATTLE_END : public CVS_THREAD {
public:
    CVoiceHandle* field_0x20[3];  // 0x20: voice-handle slots (array seed)
    s32 field_0x2c;               // 0x2c: slot/voice limit
    s32 field_0x30;               // 0x30: active-slot index / counter
    s32 field_0x34;               // 0x34: slot count
    u8  field_0x38;               // 0x38: counter direction flag
    u8  field_0x39;               // 0x39: active-voice flag
    u8  field_0x3a;               // 0x3a: random-direction flag
};

// C-linkage imports used by this TU's thread helpers / factory.  Kept here
// (not in the .cpp) because they are genuine retail C/ABI symbols imported
// from sibling voice modules.
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*           func_802A34E4(int size);
    void          __ct__cf_CVS_THREAD();
    int           func_802A77E8(CVoiceHandle* handle);
    int           func_802A7870(void* arr, int capacity, int unused);
}

// Intrusive voice list returned by the global voice-item manager.
// Layout mirrors kyoshin/cf/CfGameManagerUnityHelpers.hpp.
struct ItemListNode {
    ItemListNode* next;   // 0x00
    u32 field_0x4;        // 0x04
    u8*  object;          // 0x08: embedded CCharVoice (handle+0x3E9C)
};
struct ItemListManager {
    u32 field_0x0;        // 0x00
    ItemListNode* sentinel; // 0x04
};
extern "C" ItemListManager* func_800B6BA4__Fv();

// Globals: init-data triples and this subclass's vtable.
extern u32 lbl_eu_80539958[3];
extern u32 lbl_eu_80539964[3];
extern u32 lbl_eu_80539970[3];
extern u32 lbl_eu_8053997C[3];
extern u32 lbl_eu_80539988[];
extern u32 lbl_eu_805399A8[];
extern u32 lbl_eu_805399B8[];
extern u32 lbl_eu_80662CC8[];
extern u32 lbl_eu_80662CD0[];
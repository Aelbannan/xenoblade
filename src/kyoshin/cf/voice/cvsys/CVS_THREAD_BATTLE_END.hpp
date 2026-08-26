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

// Word view over the CVS_THREAD head (0x00-0x17).  The installed voice-event
// handler ptmf occupies 0x00-0x0B; retail treats those bytes as three raw
// words when re-seeding the state triple, so access them through this view.
struct CVS_THREAD_HEAD_VIEW {
    u32 word0;   // 0x00: ptmf word 0
    u32 word4;   // 0x04: ptmf word 1
    u32 word8;   // 0x08: ptmf word 2
    u32 wordC;   // 0x0C
    u32 word10;  // 0x10
    s32 word14;  // 0x14
    u32 word18;  // 0x18
};

// Raw layout exposing the implicit vtable pointer at 0x1C so the factory
// can override it with the derived vtable.
struct CVS_THREAD_BATTLE_END_raw {
    u32* state0;                // 0x00
    u32 state1;                 // 0x04
    u32 state2;                 // 0x08
    u32 field_0xC;              // 0x0C
    u32 field_0x10;             // 0x10
    u32 field_0x14;             // 0x14
    u32 field_0x18;             // 0x18
    u32* vtable;                // 0x1C
};

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
// Voice/actor-manager list accessor (retail mangled name func_800B6BA4__Fv -
// a global-scope C++ function with no args, so plain C++ linkage is correct).
ItemListManager* func_800B6BA4();

// C-linkage imports used by this TU's thread helpers / factory.  Kept here
// (not in the .cpp) because they are genuine retail C/ABI symbols imported
// from sibling voice modules.
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*           func_802A34E4(int size);
    CVS_THREAD*   __ct__cf_CVS_THREAD(CVS_THREAD* object);
    int           func_802A77E8(CVoiceHandle* handle);
    int           func_802A7870(void* arr, int capacity, int unused);

    // Runtime rethrow (NMWException runtime, C ABI): declared noreturn so MWCC
    // elides the __end__catch epilogue of a catch-all handler that ends with
    // `bl __throw`. NMWException.h itself cannot be included here: its
    // __ppc_eabi_linker.h chain redeclares _stack_addr against OS.h.
    __declspec(noreturn) void __throw(char* throwtype, u32 location,
                                      u32 dtor);
}

// Globals: init-data triples and this subclass's vtable.
extern u32 lbl_eu_80539958[3];
extern u32 lbl_eu_80539964[3];
extern u32 lbl_eu_80539970[3];
extern u32 lbl_eu_8053997C[3];
extern u32 lbl_eu_80539988[];
extern u32 lbl_eu_805399A8[];
extern u32 lbl_eu_805399B8[];
// Fixed-size so MWCC treats these .sdata objects as sda21-addressable
// (retail uses R_PPC_EMB_SDA21 for both).
extern u32 lbl_eu_80662CC8[2];
extern u32 lbl_eu_80662CD0[2];
#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

/**
 * CVS_THREAD_BATTLE_END_SP - voice thread for special battle-end sequences.
 *
 * Object layout (allocated 0x34 bytes):
 *   0x20..0x2A  slots[3]      array of active CVoiceHandle pointers
 *   0x2C        count         number of active voice slots
 *   0x30        cmdString     pointer to the current command string
 *
 * Overrides blank1() (vtable index 2) to return the logical allocation
 * size 0x46 (70) -- same getter as the plain BATTLE_END variant.
 */
class CVS_THREAD_BATTLE_END_SP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0x46;

    CVoiceHandle* slots[3];  // 0x20: active voice slots
    s32 count;               // 0x2C: number of active slots
    char* cmdString;         // 0x30: current command string

    int blank1() override;   // vtable[2], returns 0x46
};

// Circular voice-node list returned by the voice manager func_800B6BA4().
// Each node's field_8 points at the CCharVoice embedded in the handle
// (i.e. handle + 0x3E9C).
struct CVoiceListNode {
    CVoiceListNode* field_0;  // 0x00: next node
    CVoiceListNode* field_4;  // 0x04: previous node (mirrors CVS_THREAD_EHP.hpp CVoiceHandleListNode)
    CCharVoice* field_8;      // 0x08: &handle->voice
};

struct CVoiceManager {
    void* field_0;              // 0x00
    CVoiceListNode* field_4;    // 0x04: circular-list sentinel
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" {
    int  func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int  func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int  func_802A77E8(CVoiceHandle* handle);
    int  func_802A8140();
    CVoiceHandle* func_802A330C(int size, int align);
    u8*  func_802A34E4(int size);
    void __ct__cf_CVS_THREAD(CVS_THREAD_BATTLE_END_SP* self);
    int  labs(int n);
}

// C++-linkage voice-manager query (retail mangled name func_800B6BA4__Fv).
CVoiceManager* func_800B6BA4();

// Data tables / glove for this thread variant.
extern "C" u32 lbl_eu_8053A194[3];  // final init-state triple
extern "C" u32 lbl_eu_8053A1A0[3];  // reset init-state triple (func_802AB900)
extern "C" u32 lbl_eu_8053A1AC[3];  // init-state triple (func_802ABA70)
extern "C" void** lbl_eu_8053A100;  // command-list table for the constructor
extern "C" void** lbl_eu_8053A1B8;  // CVS_THREAD_BATTLE_END_SP vtable
extern "C" u8 lbl_eu_80668DB0;      // sentinel command string
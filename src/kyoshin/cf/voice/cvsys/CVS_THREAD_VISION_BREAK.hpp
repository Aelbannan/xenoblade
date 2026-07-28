#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// Forward declaration of a polymorphic sub-object reached through a voice
// handle (CVoiceHandle+0x04). Only the vtable pointer at offset 0 is used.
struct CVSubObj {
    void** vtable;               // 0x00: vtable pointer
};

// Voice-handle type. The actual CCharVoice is embedded at offset 0x3E9C
// within the handle allocation (0x3E9C bytes of handle data + CCharVoice).
struct CVoiceHandle {
    void** vtable;                     // 0x00: vtable pointer
    CVSubObj* field_0x04;              // 0x04: sub-object pointer
    u8 _pad[0x3E9C - 0x08];             // 0x08-0x3E9B: handle data
    CCharVoice voice;                  // 0x3E9C: the actual voice object
    u8 _pad2[0x3F00 - (0x3E9C + 0x40)]; // 0x3EDC-0x3EFF: padding past CCharVoice
    u32 field_0x3F00;                  // 0x3F00: flags (bit 30 = ready flag)
};

/**
 * CVS_THREAD_VISION_BREAK - voice thread for vision-break status effect.
 *
 * Overrides blank1() to return 0x0F (15), indicating a small buffer
 * allocation for this thread type. Object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointer and a phase/state counter.
 */
class CVS_THREAD_VISION_BREAK : public CVS_THREAD {
public:
    // Size of the thread-local buffer in bytes.
    static const int BUFFER_SIZE = 0x0F;

    CVoiceHandle* field_0x20;   // 0x20: voice handle pointer
    s32 field_0x24;             // 0x24: phase counter / state selector

    int func_802A95FC() { return BUFFER_SIZE; }
};
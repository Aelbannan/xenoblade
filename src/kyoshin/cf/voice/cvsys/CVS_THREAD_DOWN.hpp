#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

/**
 * CVoiceHandle - voice handle with embedded CCharVoice at +0x3E9C.
 */
struct CVoiceHandle {
    void** vtable;                     // 0x00: vtable pointer
    u8 _pad[0x3E9C - sizeof(void*)];    // 0x04-0x3E9B: handle data
    CCharVoice voice;                  // 0x3E9C: the actual voice object
};

/**
 * CVS_THREAD_DOWN - voice thread variant for "Down" (knocked-down) audio.
 *
 * Inherits from CVS_THREAD and returns a thread-local buffer.
 * Total object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointers to the two voice slots.
 */
class CVS_THREAD_DOWN : public CVS_THREAD {
public:
    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)
};
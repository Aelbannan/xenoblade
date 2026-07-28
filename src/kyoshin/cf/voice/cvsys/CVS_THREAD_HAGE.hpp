#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// Forward declaration for voice handle type.
struct CVoiceHandle;

/**
 * CVS_THREAD_HAGE - voice thread variant for "Hage" (bald/bare) audio.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (240 bytes) compared to the BUF variant (260 bytes).
 */
class CVS_THREAD_HAGE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xF0;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};

// Voice-handle type. The actual CCharVoice is embedded at offset 0x3E9C
// within the handle allocation (0x3E9C bytes of handle data + CCharVoice).
struct CVoiceHandle {
    u32* vtable;                         // 0x00: vtable pointer
    u8 _pad[0x3E9C - sizeof(u32*)];       // 0x04-0x3E9B: handle data
    CCharVoice voice;                    // 0x3E9C: the actual voice object
};

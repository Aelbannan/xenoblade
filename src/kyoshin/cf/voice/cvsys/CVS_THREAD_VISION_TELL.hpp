#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// Voice-handle type. The actual CCharVoice is embedded at offset 0x3E9C
// within the handle allocation (0x3E9C bytes of handle data + CCharVoice).
struct CVoiceHandle {
    void** vtable;                     // 0x00: vtable pointer
    u8 _pad[0x3E9C - sizeof(u32)];    // 0x04-0x3E9B: handle data (u32 = ptr width on PPC32)
    CCharVoice voice;                  // 0x3E9C: the actual voice object
};

/**
 * CVS_THREAD_VISION_TELL - voice thread for vision-tell audio events.
 *
 * Overrides blank1() to return 0x32 (50), indicating a small buffer
 * allocation for this thread type. Object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointers for two voice slots.
 */
class CVS_THREAD_VISION_TELL : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x32;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};
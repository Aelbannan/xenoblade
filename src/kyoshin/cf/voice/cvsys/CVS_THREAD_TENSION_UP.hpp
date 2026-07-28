#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp"

/**
 * CVS_THREAD_TENSION_UP - voice thread for tension-up audio events.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (130 bytes) compared to other variants like BUF (260) or FAINT (240).
 */
class CVS_THREAD_TENSION_UP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0x82;

    CVoiceHandle* field_0x20;  // 0x20: back-pointer to owning manager
    s32 field_0x24;            // 0x24: thread index
    u8 field_0x28;             // 0x28: flag (0 = normal, 1 = reversed)

    int blank1() override;
};

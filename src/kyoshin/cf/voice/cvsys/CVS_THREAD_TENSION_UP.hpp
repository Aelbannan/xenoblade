#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_TENSION_UP - voice thread for tension-up audio events.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (130 bytes) compared to other variants like BUF (260) or FAINT (240).
 */
class CVS_THREAD_TENSION_UP : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x82;
};

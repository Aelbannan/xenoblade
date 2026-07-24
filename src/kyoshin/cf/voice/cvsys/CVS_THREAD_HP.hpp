#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_HP - voice thread for HP-change audio events.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (210 bytes) compared to the BUF variant (260 bytes).
 */
class CVS_THREAD_HP : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xD2;
};

#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_REVIVE - voice thread variant for revival/healing audio.
 *
 * Inherits from CVS_THREAD and returns a thread-local buffer of 230 bytes
 * (0xE6) for voice processing during character revival events.
 */
class CVS_THREAD_REVIVE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xE6;
};

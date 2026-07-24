#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_VISION_TELL - voice thread for vision-tell audio events.
 *
 * Overrides blank1() to return 0x32 (50), indicating a small buffer
 * allocation for this thread type.
 */
class CVS_THREAD_VISION_TELL : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x32;
};

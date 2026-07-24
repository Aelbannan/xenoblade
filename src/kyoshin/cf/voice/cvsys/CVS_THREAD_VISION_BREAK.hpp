#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_VISION_BREAK: voice thread for vision-break status effect.
// Overrides vmethod[2] to return a small buffer size (15 bytes).
class CVS_THREAD_VISION_BREAK : public CVS_THREAD {
public:
    // Size of the thread-local buffer in bytes.
    static const int BUFFER_SIZE = 0x0F;
};

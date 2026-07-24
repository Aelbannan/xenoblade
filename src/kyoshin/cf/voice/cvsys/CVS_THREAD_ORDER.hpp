#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_ORDER - voice thread for order/command audio events.
 *
 * Overrides blank1() to return a smaller buffer size (120 bytes)
 * compared to the BUF (260) and HP (210) variants.
 */
class CVS_THREAD_ORDER : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x78;
};

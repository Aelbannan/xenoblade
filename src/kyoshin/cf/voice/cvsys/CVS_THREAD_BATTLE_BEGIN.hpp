#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_BATTLE_BEGIN - voice thread for battle-start audio events.
 *
 * Inherits from CVS_THREAD and uses a minimal thread-local buffer
 * of 30 bytes for battle-start voice lines.
 */
class CVS_THREAD_BATTLE_BEGIN : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x1E;
};

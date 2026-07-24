#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_BUF: Voice thread that manages a dedicated buffer allocation.
// Allocates 0x104 (260) bytes for its voice processing buffer.
// Full class layout not yet decompiled; only the buffer-size getter is matched.
class CVS_THREAD_BUF : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x104;
};

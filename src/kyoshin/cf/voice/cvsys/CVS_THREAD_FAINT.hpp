#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_FAINT: voice thread state machine for the "faint" status effect.
// Allocates 0xF0 (240) bytes.  Overrides vmethod[2] to return that size.
// Fields and remaining method overrides are filled in as the TU is decompiled.
class CVS_THREAD_FAINT : public CVS_THREAD {
public:
    // Size of the thread-local buffer in bytes.
    static const int BUFFER_SIZE = 0xF0;
};

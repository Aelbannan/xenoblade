#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Allocates 0xB4 (180) bytes.  Overrides a virtual method to return that size.
// Fields and remaining method overrides are filled in as the TU is decompiled.
class CVS_THREAD_EHP : public CVS_THREAD {
    // Fields at 0x1C..0xB0 to be documented as sibling functions are matched
};

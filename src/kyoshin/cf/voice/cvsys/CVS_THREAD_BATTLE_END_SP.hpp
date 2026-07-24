#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_BATTLE_END_SP: voice thread for special battle-end sequences.
// Allocates 0x46 (70) bytes.  Overrides vmethod[2] to return that size.
// Fields and remaining method overrides are filled in as the TU is decompiled.
class CVS_THREAD_BATTLE_END_SP : public CVS_THREAD {
    // Fields at 0x20..0x3A to be documented as sibling functions are matched
};

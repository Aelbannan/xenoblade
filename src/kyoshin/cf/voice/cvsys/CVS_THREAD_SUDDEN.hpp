#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_SUDDEN - voice thread for "sudden" audio events.
 *
 * Overrides blank1() to return 1, indicating a minimal/flag-only buffer
 * rather than a full voice processing allocation.
 */
class CVS_THREAD_SUDDEN : public CVS_THREAD {
};

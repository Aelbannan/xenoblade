#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

namespace cf {

/**
 * CVS_THREAD_CHAIN - voice thread chain node.
 *
 * Inherits from CVS_THREAD and adds a link pointer for chaining
 * multiple voice threads together (e.g. multi-part battle voices).
 */
class CVS_THREAD_CHAIN : public CVS_THREAD {
};

} // namespace cf

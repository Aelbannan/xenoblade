#pragma once

/**
 * Classic Controller camera control interface.
 *
 * The real tree (cf::ICamControl base plus Remote/Gc/Classic leaves) now
 * lives in kyoshin/cf/ICamControl.hpp; this header re-exports it for the
 * classic-controller TU. The old local interface/status views are
 * gone: call sites use this->func_80274xxx(...) on the owning class.
 */

#include "kyoshin/cf/ICamControl.hpp"

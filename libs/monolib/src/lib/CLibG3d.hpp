#pragma once

#include "monolib/lib/CLibG3d.hpp"

// Retail singleton backpointer (.sbss:0x806656F0): written by the
// constructor/destructor, read by getInstance()/isInitialized(). Plain
// global-scope extern - MWCC does not mangle global data names, so this
// emits the exact retail reloc symbol directly.
extern CLibG3d* lbl_eu_806656F0;

#pragma once

#include "monolib/lib/CLibG3d.hpp"

// Retail singleton backpointer (.sbss:0x806656F0): written by the
// constructor/destructor, read by getInstance()/isInitialized(). Declared
// C-linkage so the cpp's plain definition (CWorkSystemMem pattern) links up.
extern "C" CLibG3d* lbl_eu_806656F0[2];  // 8-byte sbss; word 0 = singleton

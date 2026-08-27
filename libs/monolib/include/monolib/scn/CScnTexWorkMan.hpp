#pragma once

#include <types.h>
#include "monolib/util.hpp"

mtl::ALLOC_HANDLE getScnCounter();
void updateScnCounter(u32 r3);

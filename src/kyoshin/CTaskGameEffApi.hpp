#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/CTaskGameEff.cpp's C-linkage exports
// (CTaskGameEff.hpp itself is heavy). Retail names are unmangled.

// Resolve effect `index` off the game manager, attach it to `manager` (an
// embedded CfObjectMove sub-object, may be null) and return it (or null).
extern "C" void* bindIndexedEffect(int index, void* manager);

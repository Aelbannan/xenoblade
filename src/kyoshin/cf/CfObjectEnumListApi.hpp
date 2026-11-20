#pragma once

#include <types.h>

// Entry points of the object-enumeration list, defined in CfObjectEnumList.cpp.
// The retail linker names are flat (config/us/symbols.txt), so these are C
// functions with one canonical declaration for every caller. The list itself is
// opaque to callers; CfObjectEnumList.hpp supplies the concrete class view to
// the owning TU.
extern "C" void startEnumObjects(void* list, u32 type, u32 filter);
extern "C" void* getObjectIdAt(void* list, int index);

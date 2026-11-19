#pragma once

#include <types.h>

// Canonical declaration of the tag-context identity compare defined in
// CTagProcessor.cpp. CTagProcessor.hpp and CKizunagram.hpp define overlapping
// private view types and cannot include one another, so both include this
// lean header instead of repeating the declaration with a different signature.
// Callers pass the address of a u32 id slot or of a wchar_t* context cursor.
extern "C" int TagProcCompareIds(const void* a, const void* b);

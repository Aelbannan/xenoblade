#pragma once

#include <types.h>
#include "monolib/util/MemManager.hpp"

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" u8 lbl_eu_80526418[];
extern "C" u8 lbl_eu_80526430[];
extern "C" u8 lbl_eu_80526448[];
extern "C" const float lbl_eu_80666038;

extern "C" void __dl__FPv(void* ptr);
extern "C" void __dla__FPv(void* ptr);
extern "C" mtl::ALLOC_HANDLE func_80496004(void* ptr);
extern "C" void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);

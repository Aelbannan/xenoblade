#pragma once

#include <types.h>

// Forward declarations needed for the C-linkage import signatures
struct MonoRequestState;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" s32 func_804DA9C4(MonoRequestState* request, u8 flagA);  // lbl_eu_* pool
extern "C" s32 func_eu_804DEB4C(s32 result, u8 flagB, u32 mode);    // lbl_eu_* pool

#pragma once

#include <types.h>

// Thin owner-API header for monolib/src/scn/CScn (retail func_80496288,
// defined in CScn.cpp; canonical declaration also appears in
// monolib/core/code_804E36DC.hpp). Scene delta-time query: PAL consoles
// without the speed fix run at 1.2x.

extern "C" f32 func_80496288(void* view);

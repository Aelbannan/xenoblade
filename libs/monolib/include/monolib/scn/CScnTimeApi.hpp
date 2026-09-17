#pragma once

#include <types.h>

// Thin owner-API header for monolib/src/scn/CScn (retail Scn_GetFrameDelta,
// defined in CScn.cpp; canonical declaration also appears in
// monolib/core/code_804E36DC.hpp). Scene delta-time query: PAL consoles
// without the speed fix run at 1.2x.

extern "C" f32 Scn_GetFrameDelta(void* view);

#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/CTaskGame (the definitions live in
// CTaskGame.cpp, canonical declarations in CTaskGame.hpp). TUs whose include
// closure conflicts with unrelated CTaskGame.hpp copies (e.g. Scn_QueryUnk80State)
// include this instead; the declaration here is identical to the owner copy.

// Vision resource-copy helper (retail CTaskGame_openVision).
extern "C" int CTaskGame_openVision(u32 a, u32 b, u32 c, u32 d, u32 e, f32 f);

#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/CUIWindowManager (definition lives in
// CUIWindowManager.cpp, canonical declaration in CUIWindowManager.hpp). TUs
// whose include closure cannot take the full owner header include this
// instead; the declaration here is identical to the owner copy.

// Flag-buffer builder (retail func_801412D0).
extern "C" u8* func_801412D0(u32 target);

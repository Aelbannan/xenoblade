#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/CUIWindowManager (definition lives in
// CUIWindowManager.cpp, canonical declaration in CUIWindowManager.hpp). TUs
// whose include closure cannot take the full owner header include this
// instead; the declaration here is identical to the owner copy.

// Flag-buffer builder (retail func_801412D0).
extern "C" u8* func_801412D0(u32 target);

// Item-availability query (retail func_80140854); same signature as the
// owner copy in CUIWindowManager.hpp.
class CItemQuery;
extern "C" int func_80140854(CItemQuery* self, u32 arg1, u32 arg2);

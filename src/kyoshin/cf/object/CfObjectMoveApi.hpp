#pragma once

#include <types.h>

// Thin owner-API header for kyoshin/cf/object/CfObjectMove (definition lives
// in CfObjectMove.cpp, canonical declaration in CfObjectMove.hpp). TUs whose
// include closure cannot take the full owner header include this instead;
// the declaration here is identical to the owner copy.

// Battle-status add/remove helper (retail func_800BE12C).
extern "C" void func_800BE12C(u8* obj, int a, int b, int c, int d);
